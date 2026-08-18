#pragma once

#if defined(_MSC_VER)
#pragma message("warning: <mods/hook.hpp> is deprecated; include <mods/svc/hook.hpp> instead")
#else
#warning "<mods/hook.hpp> is deprecated; include <mods/svc/hook.hpp> instead"
#endif

#include <mods/svc/hook.hpp>

namespace mods {

template <class Entry>
ModResult hook_install(const HookService* hooks) {
    return hook::install<Entry>(hooks);
}

template <class Entry>
ModResult hook_install() {
    return hook::install<Entry>();
}

template <class Entry>
ModResult hook_add_pre(
    const HookService* hooks, HookPreFn callback, const HookOptions* options = nullptr) {
    return hook::add_pre<Entry>(hooks, callback, options);
}

template <class Entry>
ModResult hook_add_pre(HookPreFn callback, const HookOptions* options = nullptr) {
    return hook::add_pre<Entry>(callback, options);
}

template <class Entry>
ModResult hook_add_post(
    const HookService* hooks, HookPostFn callback, const HookOptions* options = nullptr) {
    return hook::add_post<Entry>(hooks, callback, options);
}

template <class Entry>
ModResult hook_add_post(HookPostFn callback, const HookOptions* options = nullptr) {
    return hook::add_post<Entry>(callback, options);
}

template <class Entry>
ModResult hook_replace(
    const HookService* hooks, HookReplaceFn callback, const HookOptions* options = nullptr) {
    return hook::replace<Entry>(hooks, callback, options);
}

template <class Entry>
ModResult hook_replace(HookReplaceFn callback, const HookOptions* options = nullptr) {
    return hook::replace<Entry>(callback, options);
}

template <class Entry>
ModResult hook_uninstall(const HookService* hooks) {
    return hook::uninstall<Entry>(hooks);
}

template <class Entry>
ModResult hook_uninstall() {
    return hook::uninstall<Entry>();
}

}  // namespace mods
