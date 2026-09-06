#include "item.hpp"

#include "registry.hpp"

#include "dusk/mods/item.hpp"
#include "dusk/mods/loader/loader.hpp"

#include "d/d_item_data.h"

#include <string_view>

namespace dusk::mods::svc {
namespace {

constexpr size_t kMaxCheckNameLength = 256;
constexpr uint32_t kGiveFlagMask = ITEM_GIVE_SILENT | ITEM_GIVE_RESOLVE;

bool is_valid_check_name(const char* name) {
    if (name == nullptr) {
        return false;
    }
    const std::string_view view{name};
    return !view.empty() && view.size() <= kMaxCheckNameLength;
}

ModResult item_set_check_override(ModContext* context, const char* name, uint8_t itemNo) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_check_name(name)) {
        return MOD_INVALID_ARGUMENT;
    }
    return item_check_set_override(*mod, name, itemNo);
}

ModResult item_clear_check_override(ModContext* context, const char* name) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !is_valid_check_name(name)) {
        return MOD_INVALID_ARGUMENT;
    }
    return item_check_clear_override(*mod, name);
}

ModResult item_set_check_resolver(ModContext* context, const char* name, ItemCheckResolveFn fn,
    void* userData, ItemCheckHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr || fn == nullptr || (name != nullptr && !is_valid_check_name(name))) {
        return MOD_INVALID_ARGUMENT;
    }

    ItemCheckHandle handle = 0;
    const auto result = item_check_add_resolver(*mod, name, fn, userData, handle);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return result;
}

ModResult item_clear_check_resolver(ModContext* context, ItemCheckHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return item_check_remove_resolver(*mod, handle);
}

ModResult item_resolve_check(
    ModContext* context, const char* name, uint8_t originalItemNo, uint8_t* outItem) {
    if (mod_from_context(context) == nullptr || !is_valid_check_name(name) || outItem == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    *outItem = item_check(name, originalItemNo, nullptr);
    return MOD_OK;
}

ModResult item_resolve_check_full(ModContext* context, const char* name, uint8_t originalItemNo,
    ItemCheckResolution* outResolution) {
    if (mod_from_context(context) == nullptr || !is_valid_check_name(name) ||
        outResolution == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    *outResolution = item_check_resolve(name, originalItemNo, nullptr);
    return MOD_OK;
}

ModResult item_give_item(
    ModContext* context, const char* checkName, uint8_t itemNo, uint32_t flags) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || (checkName != nullptr && !is_valid_check_name(checkName)) ||
        (flags & ~kGiveFlagMask) != 0)
    {
        return MOD_INVALID_ARGUMENT;
    }
    if ((flags & ITEM_GIVE_RESOLVE) != 0) {
        if (checkName == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
    } else if (itemNo == dItemNo_NONE_e) {
        return MOD_INVALID_ARGUMENT;
    }
    return item_give_enqueue(*mod, checkName, itemNo, flags);
}

ModResult item_observe_gives(
    ModContext* context, ItemGiveObserveFn fn, void* userData, ItemGiveHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr || fn == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    ItemGiveHandle handle = 0;
    const auto result = item_give_add_observer(*mod, fn, userData, handle);
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return result;
}

ModResult item_unobserve_gives(ModContext* context, ItemGiveHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return item_give_remove_observer(*mod, handle);
}

constexpr ItemService s_itemService{
    .header = SERVICE_HEADER(ItemService, ITEM_SERVICE_MAJOR, ITEM_SERVICE_MINOR),
    .set_check_override = item_set_check_override,
    .clear_check_override = item_clear_check_override,
    .set_check_resolver = item_set_check_resolver,
    .clear_check_resolver = item_clear_check_resolver,
    .resolve_check = item_resolve_check,
    .give_item = item_give_item,
    .observe_gives = item_observe_gives,
    .unobserve_gives = item_unobserve_gives,
    .resolve_check_full = item_resolve_check_full,
};

}  // namespace

constinit const ServiceModule g_itemModule{
    .id = ITEM_SERVICE_ID,
    .majorVersion = ITEM_SERVICE_MAJOR,
    .minorVersion = ITEM_SERVICE_MINOR,
    .service = &s_itemService,
    .modDetached =
        [](LoadedMod& mod) {
            item_checks_remove_mod(mod);
            item_gives_remove_mod(mod);
        },
    .frameEnd = item_gives_tick,
    .shutdown = item_gives_clear,
};

}  // namespace dusk::mods::svc
