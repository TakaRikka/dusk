#include "item.hpp"

#include "dusk/mod_loader.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/svc/item.hpp"
#include "mods/items.h"

#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"

#include <aurora/lib/logging.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <exception>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dusk::mods {
namespace {

aurora::Module Log{"dusk::mods::item_checks"};

struct CheckOverride {
    std::string name;
    uint8_t itemNo = 0;
};

struct CheckResolver {
    ItemCheckHandle handle = 0;
    std::string name;
    ItemCheckResolveFn fn = nullptr;
    void* userData = nullptr;
};

struct ModItemChecks {
    std::vector<CheckOverride> overrides;
    std::vector<CheckResolver> resolvers;
};

struct PendingResolve {
    LoadedMod* mod = nullptr;
    bool fixedValue = false;
    uint8_t itemNo = 0;
    ItemCheckResolveFn fn = nullptr;
    void* userData = nullptr;
};

struct MessageCheck {
    uint16_t group = 0;
    uint16_t messageId = 0;
    const char* name = nullptr;
    uint8_t vanillaItem = 0;
    bool enqueueAtDisplay = true;
};

constexpr std::array kMessageChecks{
    MessageCheck{
        .group = 0,
        .messageId = 1822,
        .name = ITEM_CHECK_FISHING_BOTTLE,
        .vanillaItem = dItemNo_EMPTY_BOTTLE_e,
    },
    MessageCheck{
        .group = 7,
        .messageId = 7564,
        .name = ITEM_CHECK_FISHING_HEART_PIECE,
        .vanillaItem = dItemNo_KAKERA_HEART_e,
    },
    MessageCheck{
        .group = 7,
        .messageId = 7578,
        .name = ITEM_CHECK_FISHING_HEART_PIECE,
        .vanillaItem = dItemNo_KAKERA_HEART_e,
    },
    MessageCheck{
        .group = 5,
        .messageId = 5001,
        .name = ITEM_CHECK_DUNGEON_REWARD_FOREST,
        .vanillaItem = dItemNo_NONE_e,
    },
    MessageCheck{
        .group = 5,
        .messageId = 6011,
        .name = ITEM_CHECK_DUNGEON_REWARD_GORON,
        .vanillaItem = dItemNo_NONE_e,
    },
    MessageCheck{
        .group = 5,
        .messageId = 7001,
        .name = ITEM_CHECK_DUNGEON_REWARD_LAKEBED,
        .vanillaItem = dItemNo_NONE_e,
    },
    MessageCheck{
        .group = 5,
        .messageId = 9301,
        .name = ITEM_CHECK_DUNGEON_REWARD_SNOWPEAK,
        .vanillaItem = dItemNo_NONE_e,
    },
    MessageCheck{
        .group = 5,
        .messageId = 9401,
        .name = ITEM_CHECK_DUNGEON_REWARD_TIME,
        .vanillaItem = dItemNo_NONE_e,
    },
    MessageCheck{
        .group = 5,
        .messageId = 11001,
        .name = ITEM_CHECK_DUNGEON_REWARD_CITY,
        .vanillaItem = dItemNo_NONE_e,
    },
    MessageCheck{
        .group = 0,
        .messageId = 233,
        .name = ITEM_CHECK_ILIA_MEMORY,
        .vanillaItem = dItemNo_HORSE_FLUTE_e,
        .enqueueAtDisplay = false,
    },
    MessageCheck{
        .group = 2,
        .messageId = 6531,
        .name = ITEM_CHECK_SHAD_DOMINION_ROD,
        .vanillaItem = dItemNo_COPY_ROD_2_e,
    }
};

std::unordered_map<LoadedMod*, ModItemChecks> s_modChecks;
std::unordered_set<std::string> s_warnedCollisions;
ItemCheckHandle s_nextCheckHandle = 1;

const char* current_stage_name() {
    const char* stageName = dComIfGp_getStartStageName();
    return stageName != nullptr ? stageName : "";
}

std::string chest_check_name(uint8_t boxNo) {
    return fmt::format("chest:{}:{}", current_stage_name(), boxNo);
}

std::string boss_check_name() {
    return fmt::format("boss:{}", current_stage_name());
}

std::string freestanding_check_name(uint8_t bitNo) {
    return fmt::format("freestanding:{}:{}", current_stage_name(), bitNo);
}

std::string golden_wolf_check_name(uint16_t eventFlag) {
    return fmt::format("golden_wolf:{}", eventFlag);
}

std::string poe_check_name(uint8_t bitNo) {
    return fmt::format("poe:{}:{}", current_stage_name(), bitNo);
}

std::string shop_check_name(uint8_t itemNo) {
    return fmt::format("shop:{}:{}:{}", current_stage_name(), dStage_roomControl_c::getStayNo(), itemNo);
}

std::string bug_check_name(uint8_t insectId) {
    return fmt::format("bug:{}", insectId);
}

std::string sky_character_check_name() {
    return fmt::format("sky:{}:{}", current_stage_name(), dStage_roomControl_c::getStayNo());
}

bool ilia_memory_context() {
    return std::strcmp(current_stage_name(), "R_SP109") == 0 &&
           dComIfGp_roomControl_getStayNo() == 0 && dComIfG_play_c::getLayerNo(0) == 9;
}

uint16_t item_get_message(uint8_t itemNo) {
    if (itemNo == dItemNo_KAKERA_HEART_e) {
        constexpr std::array<uint16_t, 5> heartPieceMessages{0x86, 0x9C, 0x9D, 0x9E, 0x9F};
        return heartPieceMessages[dComIfGs_getMaxLife() % heartPieceMessages.size()];
    }
    return static_cast<uint16_t>(itemNo + 0x65);
}

void invalidate_committed_check(const std::string& name) {
    if (name.empty()) {
        item_check_clear_committed();
    } else {
        item_check_cancel(item_give_tag(name.c_str()));
    }
}

}  // namespace

ItemCheckResolution item_check_resolve(const char* name, uint8_t itemNo, fopAc_ac_c* giver) {
    if (name == nullptr || *name == '\0' || s_modChecks.empty()) {
        return {.item = itemNo, .display_item = itemNo};
    }

    // Callbacks may change registrations, so copy the applicable chain before invoking one.
    std::vector<PendingResolve> resolves;
    LoadedMod* previousOverrideOwner = nullptr;
    for (auto& mod : ModLoader::instance().mods()) {
        if (!mod.active) {
            continue;
        }

        const auto modIt = s_modChecks.find(&mod);
        if (modIt == s_modChecks.end()) {
            continue;
        }

        for (const auto& checkOverride : modIt->second.overrides) {
            if (checkOverride.name != name) {
                continue;
            }
            if (previousOverrideOwner != nullptr && s_warnedCollisions.emplace(name).second) {
                Log.warn("check '{}' is overridden by [{}] and [{}]; [{}] wins by load order", name,
                    previousOverrideOwner->metadata.id, mod.metadata.id, mod.metadata.id);
            }
            previousOverrideOwner = &mod;
            resolves.push_back({.mod = &mod, .fixedValue = true, .itemNo = checkOverride.itemNo});
        }

        for (const auto& resolver : modIt->second.resolvers) {
            if (resolver.name.empty() || resolver.name == name) {
                resolves.push_back({.mod = &mod, .fn = resolver.fn, .userData = resolver.userData});
            }
        }
    }

    ItemCheckInfo info{
        .name = name,
        .giver_actor = giver,
        .vanilla_item = itemNo,
        .current_item = itemNo,
        .current_display_item = itemNo,
        .was_resolved = false,
    };
    for (const auto& resolve : resolves) {
        if (!resolve.mod->active) {
            continue;
        }
        if (resolve.fixedValue) {
            info.current_item = resolve.itemNo;
            info.current_display_item = resolve.itemNo;
            info.was_resolved = true;
            continue;
        }

        ItemCheckResolution resolution{
            .item = info.current_item,
            .display_item = dItemNo_NONE_e,
        };
        try {
            if (resolve.fn(resolve.mod->context.get(), &info, &resolution, resolve.userData)) {
                if (resolution.display_item == UINT8_MAX) {
                    resolution.display_item = resolution.item;
                }
                info.current_item = resolution.item;
                info.current_display_item = resolution.display_item;
                info.was_resolved = true;
            }
        } catch (const std::exception& e) {
            fail_mod(*resolve.mod, MOD_ERROR,
                fmt::format("Exception in item check resolver for '{}': {}", name, e.what()));
        } catch (...) {
            fail_mod(*resolve.mod, MOD_ERROR,
                fmt::format("Unknown exception in item check resolver for '{}'", name));
        }
    }
    return {
        .item = info.current_item,
        .display_item = info.current_display_item,
        .was_resolved = info.was_resolved,
    };
}

uint8_t item_check(const char* name, uint8_t itemNo, fopAc_ac_c* giver) {
    return item_check_resolve(name, itemNo, giver).item;
}

uint8_t item_check_chest(uint8_t boxNo, uint8_t itemNo, fopAc_ac_c* chest) {
    if (s_modChecks.empty()) {
        return itemNo;
    }
    const auto name = chest_check_name(boxNo);
    return item_check(name.c_str(), itemNo, chest);
}

uint8_t item_check_boss(uint8_t itemNo, fopAc_ac_c* boss) {
    if (s_modChecks.empty()) {
        return itemNo;
    }
    const auto name = boss_check_name();
    return item_check(name.c_str(), itemNo, boss);
}

uint8_t item_check_freestanding(uint8_t bitNo, uint8_t itemNo, fopAc_ac_c* item) {
    if (s_modChecks.empty()) {
        return itemNo;
    }
    const auto name = freestanding_check_name(bitNo);
    return item_check(name.c_str(), itemNo, item);
}

uint8_t item_check_golden_wolf(uint16_t eventFlag, uint8_t itemNo, fopAc_ac_c* item) {
    if (s_modChecks.empty()) {
        return itemNo;
    }
    const auto name = golden_wolf_check_name(eventFlag);
    return item_check(name.c_str(), itemNo, item);
}

uint8_t item_check_shop(uint8_t itemNo, fopAc_ac_c* giver) {
    if (s_modChecks.empty()) {
        return itemNo;
    }
    const auto name = shop_check_name(itemNo);
    return item_check(name.c_str(), itemNo, giver);
}

uint32_t item_check_message(uint16_t group, uint32_t messageId) {
    for (const auto& check : kMessageChecks) {
        if (check.group != group || check.messageId != messageId ||
            (check.group == 0 && check.messageId == 233 && !ilia_memory_context()))
        {
            continue;
        }

        const ItemCheckResult result = item_check_commit(check.name, check.vanillaItem, nullptr);
        if (result.itemNo == check.vanillaItem || result.itemNo == dItemNo_NONE_e) {
            return messageId;
        }
        if (check.enqueueAtDisplay) {
            item_check_enqueue(result, ItemGiveMode::Silent);
        }
        return item_get_message(result.itemNo);
    }
    return messageId;
}

uint32_t item_give_tag_chest(uint8_t boxNo) {
    return item_give_tag(chest_check_name(boxNo).c_str());
}

uint32_t item_give_tag_boss() {
    return item_give_tag(boss_check_name().c_str());
}

uint32_t item_give_tag_freestanding(uint8_t bitNo) {
    return item_give_tag(freestanding_check_name(bitNo).c_str());
}

uint32_t item_give_tag_golden_wolf(uint16_t eventFlag) {
    return item_give_tag(golden_wolf_check_name(eventFlag).c_str());
}

uint32_t item_give_tag_poe(uint8_t bitNo) {
    return item_give_tag(poe_check_name(bitNo).c_str());
}

uint32_t item_give_tag_shop(uint8_t itemNo) {
    return item_give_tag(shop_check_name(itemNo).c_str());
}

uint32_t item_give_tag_bug(uint8_t insectId) {
    return item_give_tag(bug_check_name(insectId).c_str());
}

uint32_t item_give_tag_sky_character() {
    return item_give_tag(sky_character_check_name().c_str());
}

namespace svc {

ModResult item_check_set_override(LoadedMod& mod, const char* name, uint8_t itemNo) {
    auto& checks = s_modChecks[&mod];
    for (auto& checkOverride : checks.overrides) {
        if (checkOverride.name == name) {
            checkOverride.itemNo = itemNo;
            invalidate_committed_check(checkOverride.name);
            return MOD_OK;
        }
    }
    checks.overrides.push_back({.name = name, .itemNo = itemNo});
    invalidate_committed_check(checks.overrides.back().name);
    return MOD_OK;
}

ModResult item_check_clear_override(LoadedMod& mod, const char* name) {
    const auto modIt = s_modChecks.find(&mod);
    if (modIt == s_modChecks.end()) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto removed = std::erase_if(modIt->second.overrides,
        [&](const auto& checkOverride) { return checkOverride.name == name; });
    if (removed != 0) {
        invalidate_committed_check(name);
    }
    return removed != 0 ? MOD_OK : MOD_INVALID_ARGUMENT;
}

ModResult item_check_add_resolver(LoadedMod& mod, const char* name, ItemCheckResolveFn fn,
    void* userData, ItemCheckHandle& outHandle) {
    auto& resolver = s_modChecks[&mod].resolvers.emplace_back();
    resolver.handle = s_nextCheckHandle++;
    resolver.name = name != nullptr ? name : "";
    resolver.fn = fn;
    resolver.userData = userData;
    outHandle = resolver.handle;
    invalidate_committed_check(resolver.name);
    return MOD_OK;
}

ModResult item_check_remove_resolver(LoadedMod& mod, ItemCheckHandle handle) {
    const auto modIt = s_modChecks.find(&mod);
    if (modIt == s_modChecks.end()) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto resolverIt =
        std::find_if(modIt->second.resolvers.begin(), modIt->second.resolvers.end(),
            [&](const auto& resolver) { return resolver.handle == handle; });
    if (resolverIt == modIt->second.resolvers.end()) {
        return MOD_INVALID_ARGUMENT;
    }
    const std::string name = resolverIt->name;
    modIt->second.resolvers.erase(resolverIt);
    invalidate_committed_check(name);
    return MOD_OK;
}

void item_checks_remove_mod(LoadedMod& mod) {
    if (const auto modIt = s_modChecks.find(&mod); modIt != s_modChecks.end()) {
        for (const auto& resolver : modIt->second.resolvers) {
            if (resolver.name.empty()) {
                item_check_clear_committed();
                break;
            }
            invalidate_committed_check(resolver.name);
        }
        for (const auto& checkOverride : modIt->second.overrides) {
            invalidate_committed_check(checkOverride.name);
        }
    }
    s_modChecks.erase(&mod);
    s_warnedCollisions.clear();
}

}  // namespace svc
}  // namespace dusk::mods
