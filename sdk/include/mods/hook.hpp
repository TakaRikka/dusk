#pragma once

#if !defined(DUSK_BUILDING_GAME) && !defined(DUSK_MOD_FEATURE_GAME)
#error "DEFINE_HOOK requires add_mod(... FEATURES game)"
#endif

#include <mods/svc/hook.h>

#include <memory>
#include <type_traits>

namespace dusk::mods {

template <class T>
T arg(void* argsRaw, int n) noexcept {
    void** args = static_cast<void**>(argsRaw);
    return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(args[n]);
}

template <class T>
std::remove_reference_t<T>& arg_ref(void* argsRaw, int n) noexcept {
    void** args = static_cast<void**>(argsRaw);
    return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(args[n]);
}

/*
 * Trampoline generator + per-target state. Tag makes each hooked target's statics distinct; the
 * target address comes from the declaration's metadata record, resolved by the host at mod
 * initialization.
 */
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
 * Typed base for a hook on a function named at compile time (&daAlink_c::execute, &free_fn).
 * Instantiate through DEFINE_HOOK, which pairs it with the metadata record the host resolves.
 */
template <auto Target>
struct Hook;

template <class C, class R, class... A, R (C::*Target)(A...)>
struct Hook<Target> : HookImpl<detail::TargetTag<Target>, R, C*, A...> {};

template <class C, class R, class... A, R (C::*Target)(A...) const>
struct Hook<Target> : HookImpl<detail::TargetTag<Target>, R, const C*, A...> {};

template <class R, class... A, R (*Target)(A...)>
struct Hook<Target> : HookImpl<detail::TargetTag<Target>, R, A...> {};

/*
 * Typed base for a hook on a function by its symbol name, for targets you can't name in C++:
 * file-local statics, private members, or symbols without a header. The signature is written
 * free-style with the receiver first and is *not* compiler-checked. Instantiate through
 * DEFINE_HOOK_SYMBOL.
 */
template <FixedString Name, class Sig>
struct NamedHook;

template <FixedString Name, class R, class... A>
struct NamedHook<Name, R(A...)> : HookImpl<detail::NameTag<Name>, R, A...> {};

/*
 * Declare a hook target.  The declaration emits a metadata record that the host resolves at mod
 * initialization. Every hook target must be declared.
 *
 *   DEFINE_HOOK(&daAlink_c::execute, LinkExecute);
 *   DEFINE_HOOK_SYMBOL("daAlink_hookshotAtHitCallBack",
 *       void(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*), HookshotHit);
 *
 *   dusk::mods::hook_add_pre<LinkExecute>(svc_hook, on_link_execute);
 *
 * DEFINE_HOOK_SYMBOL names may be the platform mangled name (dlopen convention, no Mach-O
 * leading underscore) or the demangled qualified display name; overloaded display names are
 * ambiguous and need the mangled form.
 */
#define DEFINE_HOOK(target, alias)                                                                 \
    [[maybe_unused]] static const void* const mod_meta_hook_##alias =                              \
        &::dusk::mods::detail::HookRecordFor<(target),                                             \
            ::dusk::mods::FixedString{#target}>::Holder::record;                                   \
    struct alias : ::dusk::mods::Hook<(target)> {                                                  \
        static void* resolved_target() {                                                           \
            return ::dusk::mods::detail::HookRecordFor<(target),                                   \
                ::dusk::mods::FixedString{#target}>::Holder::record.resolved;                      \
        }                                                                                          \
    }

#define DEFINE_HOOK_SYMBOL(name, sig, alias)                                                       \
    MOD_META_RECORD static constinit auto mod_meta_hook_##alias =                                  \
        ::dusk::mods::detail::make_hook_name_record<::dusk::mods::FixedString{name}>();            \
    struct alias : ::dusk::mods::NamedHook<::dusk::mods::FixedString{name}, sig> {                 \
        static void* resolved_target() { return mod_meta_hook_##alias.resolved; }                  \
    }

template <class Entry>
ModResult hook_install(const HookService* hooks) {
    if (hooks == nullptr) {
        return MOD_UNAVAILABLE;
    }

    Entry::hooks = hooks;
    if (Entry::target == nullptr) {
        void* resolved = Entry::resolved_target();
        if (resolved == nullptr) {
            return MOD_UNAVAILABLE;
        }
        Entry::target = resolved;
    }
    return hooks->install(mod_ctx, Entry::target, reinterpret_cast<void*>(Entry::trampoline),
        reinterpret_cast<void**>(&Entry::g_orig));
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

template <class Entry>
ModResult hook_add_post(
    const HookService* hooks, HookPostFn callback, const HookOptions* options = nullptr) {
    const ModResult installed = hook_install<Entry>(hooks);
    if (installed != MOD_OK) {
        return installed;
    }

    return hooks->add_post(mod_ctx, Entry::target, callback, options);
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

}  // namespace dusk::mods
