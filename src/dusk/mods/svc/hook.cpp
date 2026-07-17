#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"
#if DUSK_HAS_PREPATCH
#include "dusk/mods/loader/prepatch.hpp"
#endif
#include "dusk/mods/manifest.hpp"
#include "mods/svc/hook.h"

#if DUSK_CODE_MODS
#include "dusk/logging.h"
#include "dusk/mods/log_buffer.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fmt/format.h>
#if DUSK_HAS_FUNCHOOK
#include <funchook.h>
#endif
#include <string>
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
// unloads, the backend is handed off to a surviving candidate.
struct HookCandidate {
    ModContext* context = nullptr;
    void* trampoline = nullptr;
    void** origStore = nullptr;
    uint64_t order = 0;
};

enum class BackendKind {
    None,
#if DUSK_HAS_FUNCHOOK
    Funchook,
#endif
#if DUSK_HAS_PREPATCH
    Prepatch,
#endif
};

struct InstalledBackend {
    BackendKind kind = BackendKind::None;
#if DUSK_HAS_FUNCHOOK
    funchook_t* handle = nullptr;
#endif
#if DUSK_HAS_PREPATCH
    prepatch::Site prepatchSite{};
#endif
};

struct InstalledHook {
    InstalledBackend backend{};
    void* original = nullptr;
    ModContext* active = nullptr;
    std::vector<HookCandidate> candidates;
};

std::unordered_map<uintptr_t, HookSlot> s_registry;
std::unordered_map<uintptr_t, InstalledHook> s_installed;
std::unordered_map<const ModContext*, std::vector<uintptr_t>> s_declaredTargets;
uint64_t s_nextOrder = 0;

bool declared_target(const ModContext* context, void* fnAddr) {
    if (context == nullptr) {
        return false;
    }
    const auto it = s_declaredTargets.find(context);
    if (it == s_declaredTargets.end()) {
        return false;
    }
    const auto addr = reinterpret_cast<uintptr_t>(fnAddr);
    return std::ranges::find(it->second, addr) != it->second.end();
}

ModResult reject_undeclared(ModContext* context, void* fnAddr) {
    log::write(mod_id_from_context(context), LOG_LEVEL_ERROR,
        "tried to hook undeclared target {:p}; hook targets must be declared with "
        "DEFINE_HOOK/DEFINE_HOOK_SYMBOL",
        fnAddr);
    return MOD_INVALID_ARGUMENT;
}

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

// Once a hook is installed, funchook has patched the target's entry: its bytes lead to the
// detour, not the function, so canonicalization must stop there.
[[maybe_unused]] bool installed_target(void* addr) {
    return s_installed.contains(reinterpret_cast<uintptr_t>(addr));
}

// Follow E9/FF25 chains to skip MSVC incremental-link and import stubs.
void* resolve_import_thunk(void* addr) {
#if defined(_WIN32) && (defined(_M_X64) || defined(__x86_64__))
    for (int i = 0; i < 8; ++i) {
        if (installed_target(addr)) {
            break;
        }
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
        if (installed_target(addr)) {
            break;
        }
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

// Resolve thunks recursively (max of 8 steps) until we find our target.
void* resolve_target(void* addr) {
    for (int i = 0; i < 8; ++i) {
        void* next = resolve_import_thunk(addr);
        if (next == addr) {
            break;
        }
        addr = next;
    }
    return addr;
}

#if DUSK_HAS_FUNCHOOK
funchook_t* install_funchook(void* fnAddr, void* trampoline, void** outOriginal) {
    funchook_t* fh = funchook_create();
    if (fh == nullptr) {
        DuskLog.warn("HookSystem: funchook_create failed for {:p}", fnAddr);
        return nullptr;
    }

    void* fn = fnAddr;
    const int prep = funchook_prepare(fh, &fn, trampoline);
    if (prep == 0) {
        *outOriginal = fn;
    }
    const int inst = prep == 0 ? funchook_install(fh, 0) : -1;
    if (prep != 0 || inst != 0) {
        const char* message = funchook_error_message(fh);
        DuskLog.warn("HookSystem: funchook failed for {:p} (prepare={} install={}): {}", fnAddr,
            prep, inst, message != nullptr && message[0] != '\0' ? message : "no details");
        funchook_destroy(fh);
        *outOriginal = nullptr;
        return nullptr;
    }
    return fh;
}
#endif

bool install_backend(
    void* fnAddr, void* trampoline, InstalledBackend& outBackend, void** originalStore) {
#if DUSK_HAS_PREPATCH
    if (const auto site = prepatch::lookup(fnAddr)) {
        *originalStore = site->original;
        prepatch::publish(*site, trampoline);
        outBackend.kind = BackendKind::Prepatch;
        outBackend.prepatchSite = *site;
        return true;
    }
#endif

#if DUSK_HAS_FUNCHOOK
    funchook_t* handle = install_funchook(fnAddr, trampoline, originalStore);
    if (handle == nullptr) {
        return false;
    }
    outBackend.kind = BackendKind::Funchook;
    outBackend.handle = handle;
    return true;
#else
#if DUSK_HAS_PREPATCH
    DuskLog.warn("HookSystem: prepatch backend cannot install target {:p}: {}", fnAddr,
        prepatch::available() ? "target has no valid gateway" : prepatch::unavailable_reason());
#else
    DuskLog.warn("HookSystem: no hook backend can install target {:p}", fnAddr);
#endif
    return false;
#endif
}

void deactivate_backend(void* target, InstalledBackend& backend) {
#if DUSK_HAS_PREPATCH
    if (backend.kind == BackendKind::Prepatch) {
        prepatch::publish(backend.prepatchSite, nullptr);
        backend = {};
        return;
    }
#endif
#if DUSK_HAS_FUNCHOOK
    if (backend.kind == BackendKind::Funchook) {
        const int uninst = funchook_uninstall(backend.handle, 0);
        const int destr = funchook_destroy(backend.handle);
        if (uninst != 0 || destr != 0) {
            DuskLog.warn("HookSystem: funchook uninstall/destroy for {:p} returned {}/{}", target,
                uninst, destr);
        }
    }
#else
    (void)target;
#endif
    backend = {};
}

bool handoff_backend(
    void* target, InstalledHook& entry, const HookCandidate& candidate, void** outOriginal) {
#if DUSK_HAS_PREPATCH
    if (entry.backend.kind == BackendKind::Prepatch) {
        *candidate.origStore = entry.original;
        prepatch::publish(entry.backend.prepatchSite, candidate.trampoline);
        *outOriginal = entry.original;
        return true;
    }
#endif
#if DUSK_HAS_FUNCHOOK
    InstalledBackend backend;
    if (!install_backend(target, candidate.trampoline, backend, candidate.origStore)) {
        return false;
    }
    entry.backend = backend;
    *outOriginal = *candidate.origStore;
    return true;
#else
    (void)target;
    (void)candidate;
    (void)outOriginal;
    return false;
#endif
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

    fnAddr = resolve_target(fnAddr);
    if (!declared_target(context, fnAddr)) {
        return reject_undeclared(context, fnAddr);
    }
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

    InstalledBackend backend;
    if (!install_backend(fnAddr, trampolineFn, backend, outOriginal)) {
        return MOD_ERROR;
    }

    auto& entry = s_installed[key];
    entry.backend = backend;
    entry.original = *outOriginal;
    entry.active = context;
    entry.candidates.push_back({context, trampolineFn, outOriginal, s_nextOrder++});
    return MOD_OK;
}

ModResult hook_add_pre(
    ModContext* context, void* fnAddr, HookPreFn callback, const HookOptions* options) {
    if (fnAddr == nullptr || context == nullptr || callback == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    fnAddr = resolve_target(fnAddr);
    if (!declared_target(context, fnAddr)) {
        return reject_undeclared(context, fnAddr);
    }
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
    fnAddr = resolve_target(fnAddr);
    if (!declared_target(context, fnAddr)) {
        return reject_undeclared(context, fnAddr);
    }
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
    fnAddr = resolve_target(fnAddr);
    if (!declared_target(context, fnAddr)) {
        return reject_undeclared(context, fnAddr);
    }
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

    fnAddr = resolve_target(fnAddr);
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

    fnAddr = resolve_target(fnAddr);
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

#if defined(_WIN32)
/* Follow jump stubs, then match the MSVC vcall thunk a virtual mfp points at.
 * Returns the vtable slot's byte offset, or npos when fn is not a vcall thunk. */
size_t vcall_slot_offset(const void*& fn) noexcept {
    constexpr size_t npos = static_cast<size_t>(-1);
#if defined(_M_X64) || defined(__x86_64__)
    const auto* p = static_cast<const uint8_t*>(fn);
    for (int i = 0; i < 8 && p[0] == 0xE9; ++i) {  // incremental-link stubs
        int32_t rel;
        std::memcpy(&rel, p + 1, 4);
        p += 5 + rel;
    }
    fn = p;
    // The vptr load. Unoptimized clang-cl thunks spill/reload rcx first
    // (push rax; mov [rsp], rcx; mov rcx, [rsp]), so scan a short window.
    const uint8_t* q = nullptr;
    for (int i = 0; i <= 12; ++i) {
        if (p[i] == 0x48 && p[i + 1] == 0x8B && p[i + 2] == 0x01) {  // mov rax, [rcx]
            q = p + i + 3;
            break;
        }
    }
    if (q == nullptr) {
        return npos;
    }
    if (q[0] == 0xFF && q[1] == 0x20) {  // jmp [rax]  (MSVC)
        return 0;
    }
    if (q[0] == 0xFF && q[1] == 0x60) {  // jmp [rax + imm8]
        return static_cast<int8_t>(q[2]);
    }
    if (q[0] == 0xFF && q[1] == 0xA0) {  // jmp [rax + imm32]
        int32_t off;
        std::memcpy(&off, q + 2, 4);
        return off;
    }
    // clang-cl: mov rax, [rax + off]; (pop r10;) jmp rax. Requiring the jmp rax
    // distinguishes the thunk from an ordinary getter that begins the same way.
    if (q[0] == 0x48 && q[1] == 0x8B && (q[2] == 0x00 || q[2] == 0x40 || q[2] == 0x80)) {
        size_t off = 0;
        const uint8_t* r = q + 3;
        if (q[2] == 0x40) {
            off = static_cast<int8_t>(q[3]);
            r = q + 4;
        } else if (q[2] == 0x80) {
            int32_t off32;
            std::memcpy(&off32, q + 3, 4);
            off = off32;
            r = q + 7;
        }
        for (int i = 0; i <= 8; ++i) {
            if (r[i] == 0xFF && r[i + 1] == 0xE0) {  // jmp rax (48 REX optional)
                return off;
            }
        }
    }
    return npos;
#elif defined(_M_ARM64) || defined(__aarch64__)
    const auto* p = static_cast<const uint8_t*>(fn);
    uint32_t insn[3];
    for (int i = 0; i < 8; ++i) {  // incremental-link `b` stubs
        std::memcpy(insn, p, 4);
        if ((insn[0] & 0xFC000000u) != 0x14000000u) {
            break;
        }
        const auto imm26 = static_cast<int32_t>(insn[0] << 6) >> 6;
        p += static_cast<intptr_t>(imm26) * 4;
    }
    fn = p;
    std::memcpy(insn, p, 12);
    // ldr Xt, [x0]; ldr Xs, [Xt, #imm12*8]; br Xs
    if ((insn[0] & 0xFFFFFFE0u) != 0xF9400000u) {
        return npos;
    }
    const uint32_t t = insn[0] & 0x1Fu;
    if ((insn[1] & 0xFFC003E0u) != (0xF9400000u | (t << 5))) {
        return npos;
    }
    const uint32_t s = insn[1] & 0x1Fu;
    if (insn[2] != (0xD61F0000u | (s << 5))) {
        return npos;
    }
    return ((insn[1] >> 10) & 0xFFFu) * 8;
#else
    (void)fn;
    return npos;
#endif
}
#endif  // _WIN32

bool resolve_symbol_checked(const char* symbol, bool requireCode, void** out, std::string& why) {
    HookSymbolFlags flags{};
    switch (manifest::resolve(symbol, out, &flags)) {
    case manifest::ResolveStatus::Ok:
        if (requireCode && (flags & HOOK_SYMBOL_CODE) == 0) {
            why = fmt::format("'{}' is not a code symbol", symbol);
            return false;
        }
        return true;
    case manifest::ResolveStatus::Unavailable:
        why = "no symbol manifest for this build";
        return false;
    case manifest::ResolveStatus::NotFound:
        why = fmt::format("symbol '{}' not found", symbol);
        return false;
    case manifest::ResolveStatus::Ambiguous:
        why = fmt::format("'{}' maps to more than one address; use the mangled name", symbol);
        return false;
    }
    why = "unexpected resolve failure";
    return false;
}

/*
 * Decodes a member hook record's pointer-to-member representation into the target code address.
 * Virtual members prefer their named overrider, then fall back to reading the primary vtable slot.
 */
void* resolve_member_record(const unsigned char* pmf, size_t pmfSize, const char* vtableSymbol,
    const char* displayName, std::string& why) {
    if (pmfSize < sizeof(uintptr_t)) {
        why = "truncated pointer-to-member representation";
        return nullptr;
    }
    uintptr_t words[2]{};
    std::memcpy(words, pmf, std::min(sizeof(words), pmfSize));

#if defined(_WIN32)
    const void* fn = reinterpret_cast<const void*>(words[0]);
    if (fn == nullptr) {
        why = "null pointer-to-member target";
        return nullptr;
    }
    const size_t slot = vcall_slot_offset(fn);
    if (slot == static_cast<size_t>(-1)) {  // not a vcall thunk: direct address
        return const_cast<void*>(fn);
    }

    // A display name resolves the actual overrider rather than an ABI vcall thunk. Besides being
    // more direct, this covers secondary vtables: the MSVC representation has `this` adjustment
    // but does not have the base-path suffix used by the decorated vtable symbol.
    std::string displayWhy;
    void* displayTarget = nullptr;
    if (displayName[0] != '\0' &&
        resolve_symbol_checked(displayName, true, &displayTarget, displayWhy))
    {
        return displayTarget;
    }

    int32_t thisAdjustment = 0;
    if (pmfSize >= sizeof(uintptr_t) + sizeof(thisAdjustment)) {
        std::memcpy(&thisAdjustment, pmf + sizeof(uintptr_t), sizeof(thisAdjustment));
    }
    if (thisAdjustment != 0) {
        why = fmt::format(
            "virtual member requires a {}-byte this adjustment and its overrider did not "
            "resolve by name ({})",
            thisAdjustment, displayWhy);
        return nullptr;
    }
    int32_t firstVirtualField = 0;
    if (pmfSize > MOD_META_HOOK_MEM_CAPACITY && pmfSize >= sizeof(uintptr_t) + 2 * sizeof(int32_t))
    {
        std::memcpy(&firstVirtualField, pmf + sizeof(uintptr_t) + sizeof(int32_t), sizeof(int32_t));
    }
    int32_t secondVirtualField = 0;
    if (pmfSize > MOD_META_HOOK_MEM_CAPACITY && pmfSize >= sizeof(uintptr_t) + 3 * sizeof(int32_t))
    {
        std::memcpy(
            &secondVirtualField, pmf + sizeof(uintptr_t) + 2 * sizeof(int32_t), sizeof(int32_t));
    }
    if (firstVirtualField != 0 || secondVirtualField != 0) {
        why = fmt::format("virtual-base member did not resolve by name ({})", displayWhy);
        return nullptr;
    }
    if (vtableSymbol[0] == '\0') {
        why = "class name is not representable as a vtable symbol";
        return nullptr;
    }
    void* vtable = nullptr;
    if (!resolve_symbol_checked(vtableSymbol, false, &vtable, why)) {
        return nullptr;
    }
    // ??_7 points at the first slot.
    return *reinterpret_cast<void**>(static_cast<char*>(vtable) + slot);
#else
#if defined(__aarch64__) || defined(__arm__)
    // AAPCS C++ ABI: the virtual flag is bit 0 of the adjustment word (function
    // addresses can't spare their low bit), and ptr holds the slot offset directly.
    const bool isVirtual = (words[1] & 1) != 0;
    const uintptr_t thisAdjust = words[1] >> 1;
    const uintptr_t slotOffset = words[0];
#else
    // Itanium C++ ABI: virtual mfps set bit 0 of ptr; the slot offset is ptr - 1.
    const bool isVirtual = (words[0] & 1) != 0;
    const uintptr_t thisAdjust = words[1];
    const uintptr_t slotOffset = words[0] - 1;
#endif
    if (!isVirtual) {  // non-virtual: the address itself
        return reinterpret_cast<void*>(words[0]);
    }

    std::string displayWhy;
    void* displayTarget = nullptr;
    if (displayName[0] != '\0' &&
        resolve_symbol_checked(displayName, true, &displayTarget, displayWhy))
    {
        return displayTarget;
    }
    if (thisAdjust != 0) {
        // The slot is relative to a secondary vtable whose base path is not encoded in the mfp.
        why = fmt::format(
            "virtual member of a secondary base did not resolve by name ({})", displayWhy);
        return nullptr;
    }
    if (vtableSymbol[0] == '\0') {
        why = "class name is not representable as a vtable symbol";
        return nullptr;
    }
    void* vtable = nullptr;
    if (!resolve_symbol_checked(vtableSymbol, false, &vtable, why)) {
        return nullptr;
    }
    // _ZTV points at the offset-to-top slot; the address point mfps index from is
    // two pointers in (past offset-to-top and the typeinfo pointer).
    void* target =
        *reinterpret_cast<void**>(static_cast<char*>(vtable) + 2 * sizeof(void*) + slotOffset);
    if (target == nullptr) {
        why = "vtable slot is empty";
    }
    return target;
#endif
}

void hook_remove_mod(LoadedMod& mod) {
    ModContext* context = mod.context.get();
    s_declaredTargets.erase(context);

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
        if (entry.candidates.empty()) {
            deactivate_backend(target, entry.backend);
            it = s_installed.erase(it);
            continue;
        }

        // A prepatch may be atomically updated directly.
        // Funchook must first restore the original instructions before reinstalling.
#if DUSK_HAS_PREPATCH
        const bool prepatched = entry.backend.kind == BackendKind::Prepatch;
#else
        constexpr bool prepatched = false;
#endif
        if (!prepatched) {
            deactivate_backend(target, entry.backend);
        }
        entry.active = nullptr;
        for (auto& cand : entry.candidates) {
            void* original = nullptr;
            if (!handoff_backend(target, entry, cand, &original)) {
                continue;
            }
            entry.original = original;
            entry.active = cand.context;
            DuskLog.info("HookSystem: replaced trampoline for {:p}: {} -> {} (tramp={:p})", target,
                mod_id_from_context(context), mod_id_from_context(cand.context), cand.trampoline);
            break;
        }

        if (entry.active == nullptr) {
            DuskLog.warn("HookSystem: no reinstallable trampoline for {:p}; hooks there are "
                         "disabled until a mod reinstalls one",
                target);
            for (auto& cand : entry.candidates) {
                *cand.origStore = target;
            }
            deactivate_backend(target, entry.backend);
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

#if DUSK_CODE_MODS
void hook_resolve_mod_records(LoadedMod& mod) {
    auto& declared = s_declaredTargets[mod.context.get()];
    declared.clear();
    if (!mod.native) {
        return;
    }

    const auto resolved = [&](void* target, void** slot) {
        target = resolve_target(target);
        *slot = target;
        declared.push_back(reinterpret_cast<uintptr_t>(target));
    };
    const auto unresolved = [&](const char* what, std::string_view why, void** slot) {
        *slot = nullptr;
        log::write(mod.metadata.id, LOG_LEVEL_WARN,
            "hook target '{}' did not resolve ({}); installing this hook will fail", what, why);
    };

    for (auto* record : mod.native->parsed.hookFns) {
        if (record->target != nullptr) {
            resolved(record->target, &record->resolved);
        } else {
            unresolved("<fn>", "null link-time target", &record->resolved);
        }
    }
    const auto resolveMember = [&](auto* record, const unsigned char* pmf, size_t pmfSize) {
        const char* displayName = hook_mem_display_name(*record);
        std::string why;
        void* target =
            resolve_member_record(pmf, pmfSize, hook_mem_vtable_symbol(*record), displayName, why);
        if (target != nullptr) {
            resolved(target, &record->resolved);
        } else {
            unresolved(displayName, why, &record->resolved);
        }
    };
    for (auto* record : mod.native->parsed.hookMems) {
        resolveMember(record, record->pmf, sizeof(record->pmf));
    }
    for (auto* record : mod.native->parsed.hookMemExts) {
        alignas(std::max_align_t) unsigned char pmf[MOD_META_HOOK_MEM_EXT_CAPACITY]{};
        record->materialize(pmf);
        resolveMember(record, pmf, record->pmf_size);
    }
    for (auto* record : mod.native->parsed.hookNames) {
        const char* name = hook_name_symbol(*record);
        std::string why;
        void* target = nullptr;
        if (resolve_symbol_checked(name, true, &target, why)) {
            resolved(target, &record->resolved);
        } else {
            unresolved(name, why, &record->resolved);
        }
    }
}
#else
void hook_resolve_mod_records(LoadedMod&) {}
#endif  // DUSK_CODE_MODS

constinit const ServiceModule g_hookModule{
    .id = HOOK_SERVICE_ID,
    .majorVersion = HOOK_SERVICE_MAJOR,
    .minorVersion = HOOK_SERVICE_MINOR,
    .service = &s_hookService,
    .modDetached = hook_remove_mod,
};

}  // namespace dusk::mods::svc
