#pragma once

#include "mods/svc/hook.h"

#include <array>
#include <cstddef>
#include <cstring>
#include <memory>
#include <string_view>
#include <type_traits>

namespace dusk::mods {

template <class T>
T arg(void* argsRaw, int n) noexcept {
    void** args = static_cast<void**>(argsRaw);
    return *static_cast<std::add_pointer_t<std::remove_reference_t<T> > >(args[n]);
}

template <class T>
std::remove_reference_t<T>& arg_ref(void* argsRaw, int n) noexcept {
    void** args = static_cast<void**>(argsRaw);
    return *static_cast<std::add_pointer_t<std::remove_reference_t<T> > >(args[n]);
}

template <class F>
void* mfp_addr(F fn) noexcept {
    void* p = nullptr;
    static_assert(sizeof(fn) >= sizeof(void*), "unexpected function pointer size");
    std::memcpy(&p, &fn, sizeof(void*));
    return p;
}

/* A string usable as a template argument: carries the hook target's symbol name and
 * makes each NamedHook instantiation's static state unique. */
template <size_t N>
struct FixedString {
    char chars[N]{};
    constexpr FixedString(const char (&s)[N]) noexcept {
        for (size_t i = 0; i < N; ++i) {
            chars[i] = s[i];
        }
    }
};

namespace detail {

template <class T>
constexpr std::string_view class_name() {
#if defined(__clang__) || defined(__GNUC__)
    // "... class_name() [T = daAlink_c]" / "... [with T = daAlink_c; ...]"
    constexpr std::string_view fn = __PRETTY_FUNCTION__;
    constexpr size_t start = fn.find("T = ") + 4;
    return fn.substr(start, fn.find_first_of(";]", start) - start);
#elif defined(_MSC_VER)
    // "... class_name<class daAlink_c>(void)"
    constexpr std::string_view fn = __FUNCSIG__;
    constexpr size_t start = fn.find("class_name<") + 11;
    constexpr std::string_view name = fn.substr(start, fn.rfind(">(") - start);
    if constexpr (name.starts_with("class ")) {
        return name.substr(6);
    } else if constexpr (name.starts_with("struct ")) {
        return name.substr(7);
    } else {
        return name;
    }
#else
#error "unsupported compiler"
#endif
}

/* The manifest name of C's vtable. Only unscoped, non-template class names are
 * supported (an empty result fails resolution and the install reports it). */
template <class C>
constexpr auto vtable_symbol() {
    constexpr std::string_view name = class_name<C>();
    constexpr bool simple = name.find_first_of(":<> ") == std::string_view::npos;
    // "_ZTV" + decimal length + name / "??_7" + name + "@@6B@", NUL-terminated
    std::array<char, name.size() + 12> out{};
    if constexpr (!simple) {
        return out;
    }
    size_t n = 0;
#if defined(_WIN32)
    for (char c : {'?', '?', '_', '7'}) {
        out[n++] = c;
    }
    for (char c : name) {
        out[n++] = c;
    }
    for (char c : {'@', '@', '6', 'B', '@'}) {
        out[n++] = c;
    }
#else
    for (char c : {'_', 'Z', 'T', 'V'}) {
        out[n++] = c;
    }
    size_t len = name.size();
    char digits[8]{};
    size_t d = 0;
    while (len != 0) {
        digits[d++] = static_cast<char>('0' + len % 10);
        len /= 10;
    }
    while (d != 0) {
        out[n++] = digits[--d];
    }
    for (char c : name) {
        out[n++] = c;
    }
#endif
    return out;
}

#if defined(_WIN32)
/* Follow jump stubs, then match the MSVC vcall thunk a virtual mfp points at.
 * Returns the vtable slot's byte offset, or npos when fn is not a vcall thunk. */
inline size_t vcall_slot_offset(const void*& fn) noexcept {
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
#endif

/* Code address of the member function a mfp designates. Virtual mfps don't carry
 * one; recover it from the class's vtable (resolved from the symbol manifest), so
 * Hook works uniformly on virtual and non-virtual members. */
template <class C, class F>
ModResult member_target(const HookService* hooks, F mfp, void** out) {
    *out = nullptr;
    uintptr_t words[sizeof(F) > sizeof(uintptr_t) ? 2 : 1] = {};
    std::memcpy(words, &mfp, sizeof(words) < sizeof(F) ? sizeof(words) : sizeof(F));

#if defined(_WIN32)
    const void* fn = reinterpret_cast<const void*>(words[0]);
    const size_t slot = vcall_slot_offset(fn);
    if (slot == static_cast<size_t>(-1)) {  // not a vcall thunk: direct address
        *out = const_cast<void*>(fn);
        return MOD_OK;
    }
    void* vtable = nullptr;
    const ModResult resolved = hooks->resolve(mod_ctx, vtable_symbol<C>().data(), &vtable, nullptr);
    if (resolved != MOD_OK) {
        return resolved;
    }
    // ??_7 points at the first slot.
    *out = *reinterpret_cast<void**>(static_cast<char*>(vtable) + slot);
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
        *out = reinterpret_cast<void*>(words[0]);
        return MOD_OK;
    }
    if (thisAdjust != 0) {
        // this-adjusting mfp (member of a secondary base): the slot offset is
        // relative to a vtable we can't locate. Hook the overrider by name instead.
        return MOD_UNSUPPORTED;
    }
    void* vtable = nullptr;
    const ModResult resolved = hooks->resolve(mod_ctx, vtable_symbol<C>().data(), &vtable, nullptr);
    if (resolved != MOD_OK) {
        return resolved;
    }
    // _ZTV points at the offset-to-top slot; the address point mfps index from is
    // two pointers in (past offset-to-top and the typeinfo pointer).
    *out = *reinterpret_cast<void**>(static_cast<char*>(vtable) + 2 * sizeof(void*) + slotOffset);
#endif
    return *out != nullptr ? MOD_OK : MOD_UNAVAILABLE;
}

}  // namespace detail

/* Trampoline generator + per-target state shared by Hook and NamedHook. Tag makes
 * each hooked target's statics distinct; the target address is filled in at install. */
template <class Tag, class R, class... A>
struct HookImpl {
    static inline R (*g_orig)(A...) = nullptr;
    static inline const HookService* hooks = nullptr;
    static inline void* target = nullptr;

    static bool dispatch_pre(void* args, void* retval) {
        if (hooks == nullptr) {
            return false;
        }

        int skipOriginal = 0;
        const ModResult result = hooks->dispatch_pre(mod_ctx, target, args, retval, &skipOriginal);
        return result == MOD_OK && skipOriginal != 0;
    }

    static void dispatch_post(void* args, void* retval) {
        if (hooks != nullptr) {
            hooks->dispatch_post(mod_ctx, target, args, retval);
        }
    }

    static R trampoline(A... args) {
        if constexpr (sizeof...(A) == 0) {
            if constexpr (std::is_void_v<R>) {
                const bool skipOriginal = dispatch_pre(nullptr, nullptr);
                if (!skipOriginal) {
                    g_orig(args...);
                }
                dispatch_post(nullptr, nullptr);
            } else {
                R result{};
                const bool skipOriginal =
                    dispatch_pre(nullptr, static_cast<void*>(std::addressof(result)));
                if (!skipOriginal) {
                    result = g_orig(args...);
                }
                dispatch_post(nullptr, static_cast<void*>(std::addressof(result)));
                return result;
            }
        } else {
            void* ptrs[] = {static_cast<void*>(std::addressof(args))...};
            if constexpr (std::is_void_v<R>) {
                const bool skipOriginal = dispatch_pre(static_cast<void*>(ptrs), nullptr);
                if (!skipOriginal) {
                    g_orig(args...);
                }
                dispatch_post(static_cast<void*>(ptrs), nullptr);
            } else {
                R result{};
                const bool skipOriginal = dispatch_pre(
                    static_cast<void*>(ptrs), static_cast<void*>(std::addressof(result)));
                if (!skipOriginal) {
                    result = g_orig(args...);
                }
                dispatch_post(static_cast<void*>(ptrs), static_cast<void*>(std::addressof(result)));
                return result;
            }
        }
    }
};

namespace detail {
template <auto Target>
using TargetTag = std::integral_constant<decltype(Target), Target>;
template <FixedString Name>
struct NameTag {};
}  // namespace detail

/*
 * Typed hook on a function named at compile time (&daAlink_c::execute, &free_fn).
 * Member functions may be virtual: the install decodes the member function pointer and hooks the
 * class's own overrider.
 */
template <auto Target>
struct Hook;

template <class C, class R, class... A, R (C::*Target)(A...)>
struct Hook<Target> : HookImpl<detail::TargetTag<Target>, R, C*, A...> {
    static ModResult resolve_target(const HookService* hooks, void** out) {
        return detail::member_target<C>(hooks, Target, out);
    }
};

template <class C, class R, class... A, R (C::*Target)(A...) const>
struct Hook<Target> : HookImpl<detail::TargetTag<Target>, R, const C*, A...> {
    static ModResult resolve_target(const HookService* hooks, void** out) {
        return detail::member_target<C>(hooks, Target, out);
    }
};

template <class R, class... A, R (*Target)(A...)>
struct Hook<Target> : HookImpl<detail::TargetTag<Target>, R, A...> {
    static ModResult resolve_target(const HookService*, void** out) {
        *out = mfp_addr(Target);
        return MOD_OK;
    }
};

/*
 * Typed hook on a function by its symbol name, for targets you can't name in C++: file-local
 * statics, private members, or symbols without a header. The signature is written free-style with
 * the receiver first and is *not* compiler-checked.
 *
 *   using HookshotHit = dusk::mods::NamedHook<
 *       "daAlink_hookshotAtHitCallBack",
 *       void(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*)>;
 *   dusk::mods::hook_add_pre<HookshotHit>(svc_hook, on_hookshot_hit);
 */
template <FixedString Name, class Sig>
struct NamedHook;

template <FixedString Name, class R, class... A>
struct NamedHook<Name, R(A...)> : HookImpl<detail::NameTag<Name>, R, A...> {
    static ModResult resolve_target(const HookService* hooks, void** out) {
        HookSymbolFlags flags{};
        const ModResult resolved = hooks->resolve(mod_ctx, Name.chars, out, &flags);
        if (resolved == MOD_OK && (flags & HOOK_SYMBOL_CODE) == 0) {
            *out = nullptr;
            return MOD_INVALID_ARGUMENT;
        }
        return resolved;
    }
};

template <class Entry>
ModResult hook_install(const HookService* hooks) {
    if (hooks == nullptr) {
        return MOD_UNAVAILABLE;
    }

    Entry::hooks = hooks;
    if (Entry::target == nullptr) {
        const ModResult resolved = Entry::resolve_target(hooks, &Entry::target);
        if (resolved != MOD_OK) {
            return resolved;
        }
    }
    return hooks->install(mod_ctx, Entry::target, reinterpret_cast<void*>(Entry::trampoline),
        reinterpret_cast<void**>(&Entry::g_orig));
}

template <auto Target>
ModResult hook_install(const HookService* hooks) {
    return hook_install<Hook<Target> >(hooks);
}

template <class Entry>
ModResult hook_add_pre(
    const HookService* hooks, HookPreFn callback, const HookOptions* options = nullptr) {
    const ModResult installed = hook_install<Entry>(hooks);
    if (installed != MOD_OK) {
        return installed;
    }

    return hooks->add_pre(mod_ctx, Entry::target, callback, options);
}

template <auto Target>
ModResult hook_add_pre(
    const HookService* hooks, HookPreFn callback, const HookOptions* options = nullptr) {
    return hook_add_pre<Hook<Target> >(hooks, callback, options);
}

template <class Entry>
ModResult hook_add_post(
    const HookService* hooks, HookPostFn callback, const HookOptions* options = nullptr) {
    const ModResult installed = hook_install<Entry>(hooks);
    if (installed != MOD_OK) {
        return installed;
    }

    return hooks->add_post(mod_ctx, Entry::target, callback, options);
}

template <auto Target>
ModResult hook_add_post(
    const HookService* hooks, HookPostFn callback, const HookOptions* options = nullptr) {
    return hook_add_post<Hook<Target> >(hooks, callback, options);
}

template <class Entry>
ModResult hook_replace(
    const HookService* hooks, HookReplaceFn callback, const HookOptions* options = nullptr) {
    const ModResult installed = hook_install<Entry>(hooks);
    if (installed != MOD_OK) {
        return installed;
    }

    return hooks->replace(mod_ctx, Entry::target, callback, options);
}

template <auto Target>
ModResult hook_replace(
    const HookService* hooks, HookReplaceFn callback, const HookOptions* options = nullptr) {
    return hook_replace<Hook<Target> >(hooks, callback, options);
}

}  // namespace dusk::mods
