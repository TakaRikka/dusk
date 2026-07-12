#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/manifest.hpp"
#include "mods/svc/hook.h"

#if DUSK_CODE_MODS
#include "dusk/logging.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fmt/format.h>
#include <funchook.h>
#include <unordered_map>
#include <vector>
#endif

namespace dusk::mods::svc {
namespace {

#if DUSK_CODE_MODS

struct PreHookFn {
    ModContext* context = nullptr;
    HookPreFn callback = nullptr;
    HookOptions options = HOOK_OPTIONS_INIT;
    uint64_t order = 0;
};

struct VoidHookFn {
    ModContext* context = nullptr;
    HookReplaceFn replaceCallback = nullptr;
    HookPostFn postCallback = nullptr;
    HookOptions options = HOOK_OPTIONS_INIT;
    uint64_t order = 0;
};

struct HookSlot {
    std::vector<PreHookFn> pre;
    VoidHookFn replace{};
    std::vector<VoidHookFn> post;
};

// One per mod that requested a hook on a target: its template-generated trampoline and the
// address of its Hook::g_orig, both living in the mod's dylib. Any candidate's trampoline
// is interchangeable (dispatch walks the shared HookSlot), so when the active installer's mod
// unloads, the funchook detour is handed off to a surviving candidate and every candidate's
// *orig_store is rewritten to the new original pointer.
struct HookCandidate {
    ModContext* context = nullptr;
    void* trampoline = nullptr;
    void** origStore = nullptr;
    uint64_t order = 0;
};

struct InstalledHook {
    funchook_t* handle = nullptr;
    void* original = nullptr;
    ModContext* active = nullptr;
    std::vector<HookCandidate> candidates;
};

std::unordered_map<uintptr_t, HookSlot> s_registry;
std::unordered_map<uintptr_t, InstalledHook> s_installed;
uint64_t s_nextOrder = 0;

HookOptions normalize_options(const HookOptions* options) {
    if (options == nullptr || options->struct_size < sizeof(HookOptions)) {
        return HOOK_OPTIONS_INIT;
    }
    return *options;
}

void fail_hook_callback(ModContext* context, const char* kind, const std::exception& e) {
    if (auto* mod = mod_from_context(context)) {
        fail_mod(*mod, MOD_ERROR, fmt::format("Exception in {} hook callback: {}", kind, e.what()));
    }
}

void fail_unknown_hook_callback(ModContext* context, const char* kind) {
    if (auto* mod = mod_from_context(context)) {
        fail_mod(*mod, MOD_ERROR, fmt::format("Unknown exception in {} hook callback", kind));
    }
}

template <class T>
void sort_hooks(std::vector<T>& hooks) {
    std::ranges::stable_sort(hooks, [](const T& a, const T& b) {
        if (a.options.priority != b.options.priority) {
            return a.options.priority > b.options.priority;
        }
        return a.order < b.order;
    });
}

// Follow E9/FF25 chains to skip MSVC incremental-link and import stubs.
void* resolve_import_thunk(void* addr) {
#if defined(_WIN32) && (defined(_M_X64) || defined(__x86_64__))
    for (int i = 0; i < 8; ++i) {
        const auto* p = static_cast<const uint8_t*>(addr);
        if (p[0] == 0x48 && p[1] == 0xFF && p[2] == 0x25) {  // lld emits a REX.W prefix
            ++p;
        }
        if (p[0] == 0xFF && p[1] == 0x25) {
            int32_t offset;
            std::memcpy(&offset, p + 2, 4);
            addr = const_cast<void*>(*reinterpret_cast<const void* const*>(p + 6 + offset));
            break;
        }
        if (p[0] == 0xE9) {
            int32_t offset;
            std::memcpy(&offset, p + 1, 4);
            addr = const_cast<uint8_t*>(p) + 5 + offset;
        } else {
            break;
        }
    }
#elif defined(_WIN32) && (defined(_M_ARM64) || defined(__aarch64__))
    // Import thunks are `adrp x16; ldr x16, [x16, #off]; br x16` (deref the IAT slot);
    // incremental-link stubs are a plain `b`, or `adrp x16; add x16, x16, #off; br x16`
    // range-extension thunks when the target is out of B range.
    for (int i = 0; i < 8; ++i) {
        const auto* p = static_cast<const uint8_t*>(addr);
        uint32_t insn0, insn1, insn2;
        std::memcpy(&insn0, p, 4);
        if ((insn0 & 0xFC000000u) == 0x14000000u) {  // b imm26
            auto imm26 = static_cast<int32_t>(insn0 << 6) >> 6;
            addr = const_cast<uint8_t*>(p) + static_cast<intptr_t>(imm26) * 4;
            continue;
        }
        if ((insn0 & 0x9F00001Fu) != 0x90000010u) {  // adrp x16, page
            break;
        }
        std::memcpy(&insn1, p + 4, 4);
        std::memcpy(&insn2, p + 8, 4);
        if (insn2 != 0xD61F0200u) {  // br x16
            break;
        }
        auto immhi = static_cast<int64_t>(static_cast<int32_t>(insn0 << 8) >> 13);  // bits 23:5
        auto immlo = static_cast<int64_t>((insn0 >> 29) & 3);
        auto page = (reinterpret_cast<uintptr_t>(p) & ~uintptr_t{0xFFF}) +
                    (static_cast<intptr_t>((immhi << 2) | immlo) << 12);
        if ((insn1 & 0xFFC003FFu) == 0xF9400210u) {  // ldr x16, [x16, #imm12*8]
            auto slot = page + ((insn1 >> 10) & 0xFFF) * 8;
            addr = *reinterpret_cast<void**>(slot);
            break;
        }
        if ((insn1 & 0xFF8003FFu) == 0x91000210u) {  // add x16, x16, #imm12{, lsl #12}
            auto imm = static_cast<uintptr_t>((insn1 >> 10) & 0xFFF);
            addr = reinterpret_cast<void*>(page + (((insn1 >> 22) & 1) != 0 ? imm << 12 : imm));
            continue;
        }
        break;
    }
#endif
    return addr;
}

funchook_t* install_trampoline(void* fnAddr, void* trampoline, void** outOriginal) {
    funchook_t* fh = funchook_create();
    if (fh == nullptr) {
        DuskLog.warn("HookSystem: funchook_create failed for {:p}", fnAddr);
        return nullptr;
    }

    void* fn = fnAddr;
    const int prep = funchook_prepare(fh, &fn, trampoline);
    const int inst = prep == 0 ? funchook_install(fh, 0) : -1;
    if (prep != 0 || inst != 0) {
        const char* message = funchook_error_message(fh);
        DuskLog.warn("HookSystem: funchook failed for {:p} (prepare={} install={}): {}", fnAddr,
            prep, inst, message != nullptr && message[0] != '\0' ? message : "no details");
        funchook_destroy(fh);
        return nullptr;
    }

    *outOriginal = fn;
    return fh;
}

ModResult hook_install(ModContext* context, void* fnAddr, void* trampolineFn, void** outOriginal) {
    if (fnAddr == nullptr || trampolineFn == nullptr || outOriginal == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    // Try to detect an invalid function pointer (possibly a vtable slot offset or Itanium mfp
    // value) and provide a helpful warning instead of faulting
    const auto raw = reinterpret_cast<uintptr_t>(fnAddr);
    if (raw < 0x10000
#if defined(__aarch64__) || defined(_M_ARM64)
        || (raw & 3) != 0  // code is 4-aligned
#endif
    )
    {
        DuskLog.warn("HookSystem: {:p} from {} is not a code address (virtual member function "
                     "pointer? hook via dusk::mods::Hook or resolve())",
            fnAddr, mod_id_from_context(context));
        return MOD_INVALID_ARGUMENT;
    }

    fnAddr = resolve_import_thunk(fnAddr);
    const auto key = reinterpret_cast<uintptr_t>(fnAddr);
    if (const auto it = s_installed.find(key); it != s_installed.end()) {
        auto& entry = it->second;
        // hook_add_pre + hook_add_post on the same target share one g_orig per mod.
        const bool known = std::ranges::any_of(entry.candidates, [&](const HookCandidate& cand) {
            return cand.context == context && cand.origStore == outOriginal;
        });
        if (!known) {
            entry.candidates.push_back({context, trampolineFn, outOriginal, s_nextOrder++});
        }
        *outOriginal = entry.original;
        return MOD_OK;
    }

    // Inlining can't be intercepted by an entry patch: warn once per target when this
    // build inlined the function into callers.
    if (const char* name = nullptr; manifest::has_inline_sites(fnAddr, &name)) {
        DuskLog.warn("HookSystem: '{}' ({:p}) for {} was inlined into callers in this build; "
                     "the hook only covers the calls that were not inlined",
            name != nullptr ? name : "?", fnAddr, mod_id_from_context(context));
    }

    void* original = nullptr;
    funchook_t* fh = install_trampoline(fnAddr, trampolineFn, &original);
    if (fh == nullptr) {
        return MOD_ERROR;
    }

    auto& entry = s_installed[key];
    entry.handle = fh;
    entry.original = original;
    entry.active = context;
    entry.candidates.push_back({context, trampolineFn, outOriginal, s_nextOrder++});
    *outOriginal = original;
    return MOD_OK;
}

ModResult hook_add_pre(
    ModContext* context, void* fnAddr, HookPreFn callback, const HookOptions* options) {
    if (fnAddr == nullptr || context == nullptr || callback == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    fnAddr = resolve_import_thunk(fnAddr);
    auto& hooks = s_registry[reinterpret_cast<uintptr_t>(fnAddr)].pre;
    hooks.push_back({context, callback, normalize_options(options), s_nextOrder++});
    sort_hooks(hooks);
    return MOD_OK;
}

ModResult hook_add_post(
    ModContext* context, void* fnAddr, HookPostFn callback, const HookOptions* options) {
    if (fnAddr == nullptr || context == nullptr || callback == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    fnAddr = resolve_import_thunk(fnAddr);
    auto& hooks = s_registry[reinterpret_cast<uintptr_t>(fnAddr)].post;
    hooks.push_back({context, nullptr, callback, normalize_options(options), s_nextOrder++});
    sort_hooks(hooks);
    return MOD_OK;
}

ModResult hook_replace(
    ModContext* context, void* fnAddr, HookReplaceFn callback, const HookOptions* options) {
    if (fnAddr == nullptr || context == nullptr || callback == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    const HookOptions normalized = normalize_options(options);
    fnAddr = resolve_import_thunk(fnAddr);
    auto& slot = s_registry[reinterpret_cast<uintptr_t>(fnAddr)];
    if (slot.replace.replaceCallback == nullptr) {
        slot.replace = {context, callback, nullptr, normalized, s_nextOrder++};
        return MOD_OK;
    }

    switch (normalized.replace_policy) {
    case HOOK_REPLACE_CONFLICT:
        DuskLog.error("HookSystem: '{}' conflicts with '{}', both replace the same function",
            mod_id_from_context(context), mod_id_from_context(slot.replace.context));
        return MOD_CONFLICT;
    case HOOK_REPLACE_PRIORITY:
        if (normalized.priority <= slot.replace.options.priority) {
            return MOD_CONFLICT;
        }
        slot.replace = {context, callback, nullptr, normalized, s_nextOrder++};
        return MOD_OK;
    case HOOK_REPLACE_OVERRIDE:
        slot.replace = {context, callback, nullptr, normalized, s_nextOrder++};
        return MOD_OK;
    }
    return MOD_INVALID_ARGUMENT;
}

ModResult hook_dispatch_pre(
    ModContext*, void* fnAddr, void* args, void* retval, int* outSkipOriginal) {
    if (outSkipOriginal != nullptr) {
        *outSkipOriginal = 0;
    }
    if (fnAddr == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    fnAddr = resolve_import_thunk(fnAddr);
    const auto it = s_registry.find(reinterpret_cast<uintptr_t>(fnAddr));
    if (it == s_registry.end()) {
        return MOD_OK;
    }
    auto& slot = it->second;
    for (auto& hook : slot.pre) {
        if (hook.callback == nullptr) {
            continue;
        }
        HookAction action = HOOK_CONTINUE;
        try {
            action = hook.callback(hook.context, args, retval, hook.options.userdata);
        } catch (const std::exception& e) {
            fail_hook_callback(hook.context, "pre", e);
            continue;
        } catch (...) {
            fail_unknown_hook_callback(hook.context, "pre");
            continue;
        }
        if (action == HOOK_SKIP_ORIGINAL) {
            if (outSkipOriginal != nullptr) {
                *outSkipOriginal = 1;
            }
            return MOD_OK;
        }
    }
    if (slot.replace.replaceCallback != nullptr) {
        try {
            slot.replace.replaceCallback(
                slot.replace.context, args, retval, slot.replace.options.userdata);
        } catch (const std::exception& e) {
            fail_hook_callback(slot.replace.context, "replace", e);
            return MOD_ERROR;
        } catch (...) {
            fail_unknown_hook_callback(slot.replace.context, "replace");
            return MOD_ERROR;
        }
        if (outSkipOriginal != nullptr) {
            *outSkipOriginal = 1;
        }
    }
    return MOD_OK;
}

ModResult hook_dispatch_post(ModContext*, void* fnAddr, void* args, void* retval) {
    if (fnAddr == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    fnAddr = resolve_import_thunk(fnAddr);
    const auto it = s_registry.find(reinterpret_cast<uintptr_t>(fnAddr));
    if (it == s_registry.end()) {
        return MOD_OK;
    }
    for (auto& hook : it->second.post) {
        if (hook.postCallback != nullptr) {
            try {
                hook.postCallback(hook.context, args, retval, hook.options.userdata);
            } catch (const std::exception& e) {
                fail_hook_callback(hook.context, "post", e);
            } catch (...) {
                fail_unknown_hook_callback(hook.context, "post");
            }
        }
    }
    return MOD_OK;
}

void hook_remove_mod(LoadedMod& mod) {
    ModContext* context = mod.context.get();

    for (auto it = s_registry.begin(); it != s_registry.end();) {
        auto& slot = it->second;
        std::erase_if(slot.pre, [&](const PreHookFn& hook) { return hook.context == context; });
        std::erase_if(slot.post, [&](const VoidHookFn& hook) { return hook.context == context; });
        if (slot.replace.context == context) {
            slot.replace = {};
        }
        if (slot.pre.empty() && slot.post.empty() && slot.replace.replaceCallback == nullptr) {
            it = s_registry.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = s_installed.begin(); it != s_installed.end();) {
        auto& entry = it->second;
        // The departing mod's g_orig slots are about to be unmapped; drop its candidates before
        // any orig_store rewrites below.
        std::erase_if(entry.candidates,
            [context](const HookCandidate& cand) { return cand.context == context; });
        if (entry.active != context) {
            ++it;
            continue;
        }

        auto* target = reinterpret_cast<void*>(it->first);
        const int uninst = funchook_uninstall(entry.handle, 0);
        const int destr = funchook_destroy(entry.handle);
        if (uninst != 0 || destr != 0) {
            DuskLog.warn("HookSystem: funchook uninstall/destroy for {:p} returned {}/{}", target,
                uninst, destr);
        }
        entry.handle = nullptr;
        entry.active = nullptr;

        if (entry.candidates.empty()) {
            it = s_installed.erase(it);
            continue;
        }

        // Hand the detour off to a surviving candidate (lowest registration order first; the
        // vector is append-ordered). A candidate whose install fails stays in the list: its
        // g_orig must still track the current original pointer.
        for (auto& cand : entry.candidates) {
            void* original = nullptr;
            funchook_t* fh = install_trampoline(target, cand.trampoline, &original);
            if (fh == nullptr) {
                continue;
            }
            entry.handle = fh;
            entry.original = original;
            entry.active = cand.context;
            DuskLog.info("HookSystem: reinstalled trampoline for {:p}: {} -> {} (tramp={:p})",
                target, mod_id_from_context(context), mod_id_from_context(cand.context),
                cand.trampoline);
            break;
        }

        if (entry.active == nullptr) {
            DuskLog.warn("HookSystem: no reinstallable trampoline for {:p}; hooks there are "
                         "disabled until a mod reinstalls one",
                target);
            for (auto& cand : entry.candidates) {
                *cand.origStore = target;
            }
            it = s_installed.erase(it);
            continue;
        }

        for (auto& cand : entry.candidates) {
            *cand.origStore = entry.original;
        }
        ++it;
    }
}

#else  // DUSK_CODE_MODS

ModResult hook_install(ModContext*, void*, void*, void**) {
    return MOD_UNSUPPORTED;
}
ModResult hook_add_pre(ModContext*, void*, HookPreFn, const HookOptions*) {
    return MOD_UNSUPPORTED;
}
ModResult hook_add_post(ModContext*, void*, HookPostFn, const HookOptions*) {
    return MOD_UNSUPPORTED;
}
ModResult hook_replace(ModContext*, void*, HookReplaceFn, const HookOptions*) {
    return MOD_UNSUPPORTED;
}
ModResult hook_dispatch_pre(ModContext*, void*, void*, void*, int* outSkipOriginal) {
    if (outSkipOriginal != nullptr) {
        *outSkipOriginal = 0;
    }
    return MOD_UNSUPPORTED;
}
ModResult hook_dispatch_post(ModContext*, void*, void*, void*) {
    return MOD_UNSUPPORTED;
}
void hook_remove_mod(LoadedMod&) {}

#endif  // DUSK_CODE_MODS

// By-name resolution reads the symbol manifest, which is independent of the hook engine.
ModResult hook_resolve(ModContext*, const char* symbol, void** outAddr, HookSymbolFlags* outFlags) {
    if (symbol == nullptr || outAddr == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    switch (manifest::resolve(symbol, outAddr, outFlags)) {
    case manifest::ResolveStatus::Ok:
        return MOD_OK;
    case manifest::ResolveStatus::Unavailable:
        return MOD_UNSUPPORTED;
    case manifest::ResolveStatus::NotFound:
        return MOD_UNAVAILABLE;
    case manifest::ResolveStatus::Ambiguous:
        return MOD_CONFLICT;
    }
    return MOD_ERROR;
}

constexpr HookService s_hookService{
    .header = SERVICE_HEADER(HookService, HOOK_SERVICE_MAJOR, HOOK_SERVICE_MINOR),
    .install = hook_install,
    .add_pre = hook_add_pre,
    .add_post = hook_add_post,
    .replace = hook_replace,
    .dispatch_pre = hook_dispatch_pre,
    .dispatch_post = hook_dispatch_post,
    .resolve = hook_resolve,
};

}  // namespace

constinit const ServiceModule g_hookModule{
    .id = HOOK_SERVICE_ID,
    .majorVersion = HOOK_SERVICE_MAJOR,
    .minorVersion = HOOK_SERVICE_MINOR,
    .service = &s_hookService,
    .modDetached = hook_remove_mod,
};

}  // namespace dusk::mods::svc
