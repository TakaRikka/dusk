#include "dusk/hook_system.hpp"
#include "dusk/logging.h"
#include "dusk/mod_loader.hpp"

#include <cstdint>
#include <cstring>
#include <funchook.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace dusk {

namespace modding {
    extern thread_local void* g_dusk_hook_current_mod;
}

struct PreHookFn {
    void* mod;
    int32_t (*fn)(void* args);
};
struct VoidHookFn {
    void* mod;
    const char* mod_name;
    void (*fn)(void* args, void* retval);
};

struct HookSlot {
    std::vector<PreHookFn> pre;
    VoidHookFn replace = {};
    std::vector<VoidHookFn> post;
};

static std::unordered_map<uintptr_t, HookSlot> s_registry;
static std::unordered_map<uintptr_t, void*> s_installed;

// Follow E9/FF25 chains to skip MSVC incremental-link and import stubs
static void* resolveImportThunk(void* addr) {
#if defined(_WIN32) && (defined(_M_X64) || defined(__x86_64__))
    for (int i = 0; i < 8; ++i) {
        const auto* p = static_cast<const uint8_t*>(addr);
        if (p[0] == 0xFF && p[1] == 0x25) {
            int32_t offset;
            std::memcpy(&offset, p + 2, 4);
            addr = const_cast<void*>(*reinterpret_cast<const void* const*>(p + 6 + offset));
            break;
        } else if (p[0] == 0xE9) {
            int32_t offset;
            std::memcpy(&offset, p + 1, 4);
            addr = const_cast<uint8_t*>(p) + 5 + offset;
        } else {
            break;
        }
    }
#endif
    return addr;
}

struct ModGuard {
    void* prev;
    explicit ModGuard(void* mod) : prev(modding::g_dusk_hook_current_mod) { modding::g_dusk_hook_current_mod = mod; }
    ~ModGuard() { modding::g_dusk_hook_current_mod = prev; }
};

void hookInstallByAddr(void* fn_addr, void* tramp_fn, void** orig_store) {
    fn_addr = resolveImportThunk(fn_addr);
    auto key = reinterpret_cast<uintptr_t>(fn_addr);
    auto it = s_installed.find(key);
    if (it != s_installed.end()) {
        *orig_store = it->second;
        return;
    }

    funchook_t* fh = funchook_create();
    void* fn = fn_addr;
    int prep = funchook_prepare(fh, &fn, tramp_fn);
    int inst = (prep == 0) ? funchook_install(fh, 0) : -1;
    if (prep != 0 || inst != 0) {
        DuskLog.warn(
            "HookSystem: funchook failed for {:p} (prepare={} install={})", fn_addr, prep, inst);
        funchook_destroy(fh);
        // Surface the failure on the mod so it shows as failed (toast + UI marker)
        // instead of silently running with a dead hook.
        if (auto* mod = static_cast<LoadedMod*>(modding::g_dusk_hook_current_mod)) {
            mod->load_failed = true;
            if (mod->load_error.empty()) {
                mod->load_error = "a hook could not be installed (function not patchable)";
            }
        }
        return;
    }

    funchook_destroy(fh);
    s_installed[key] = fn;
    *orig_store = fn;
}

bool hookDispatchPre(void* fn_addr, void* args, void* retval) {
    auto it = s_registry.find(reinterpret_cast<uintptr_t>(fn_addr));
    if (it == s_registry.end()) {
        return false;
    }
    auto& slot = it->second;
    for (auto& h : slot.pre) {
        ModGuard g(h.mod);
        if (h.fn(args) != 0) {
            return true;
        }
    }
    if (slot.replace.fn) {
        ModGuard g(slot.replace.mod);
        slot.replace.fn(args, retval);
        return true;
    }
    return false;
}

void hookDispatchPost(void* fn_addr, void* args, void* retval) {
    auto it = s_registry.find(reinterpret_cast<uintptr_t>(fn_addr));
    if (it == s_registry.end()) {
        return;
    }
    for (auto& h : it->second.post) {
        if (h.fn) {
            ModGuard g(h.mod);
            h.fn(args, retval);
        }
    }
}

void hookRegisterPre(void* fn_addr, void* mod, int32_t (*fn)(void* args)) {
    s_registry[reinterpret_cast<uintptr_t>(fn_addr)].pre.push_back({mod, fn});
}

void hookRegisterPost(void* fn_addr, void* mod, const char* mod_name, void (*fn)(void* args, void* retval)) {
    s_registry[reinterpret_cast<uintptr_t>(fn_addr)].post.push_back({mod, mod_name, fn});
}

bool hookSetReplace(void* fn_addr, void* mod, const char* mod_name, void (*fn)(void* args, void* retval)) {
    auto& slot = s_registry[reinterpret_cast<uintptr_t>(fn_addr)];
    if (slot.replace.fn) {
        DuskLog.error("HookSystem: '{}' conflicts with '{}', both replace the same function",
            mod_name, slot.replace.mod_name);
        return false;
    }
    slot.replace = {mod, mod_name, fn};
    return true;
}

void hookClearMod(void* mod) {
    for (auto& [addr, slot] : s_registry) {
        auto erase = [&](auto& v) {
            v.erase(
                std::remove_if(v.begin(), v.end(), [mod](const auto& h) { return h.mod == mod; }),
                v.end());
        };
        erase(slot.pre);
        erase(slot.post);
        if (slot.replace.mod == mod) {
            slot.replace = {};
        }
    }
}

}  // namespace dusk
