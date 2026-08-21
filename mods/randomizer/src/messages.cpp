#include "messages.hpp"

#include "custom_flow_ids.hpp"
#include "randomizer_context.hpp"
#include "stages.h"
#include "tools.h"

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_kankyo.h"
#include "d/d_save.h"
#include "d/d_stage.h"

#include <mods/bits.hpp>
#include <mods/svc/flow.hpp>
#include <mods/svc/log.hpp>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace randomizer::messages {
namespace {

constexpr uint16_t kMessageGroupCount = 9;
constexpr uint16_t kLegacyQueryEventFlag = 53;
constexpr uint16_t kLegacyQueryChangeTime = 54;
constexpr uint16_t kLegacyQueryReturnToSpawn = 55;
constexpr uint8_t kLegacyEventNoOp = 43;
constexpr uint8_t kLegacyEventChangeTime = 44;
constexpr uint8_t kLegacyEventReturnToSpawn = 45;
constexpr uint8_t kLegacyEventSetTrackerFlag = 46;
constexpr uint8_t kLegacyEventRemoveTradeItem = 47;

using LegacyIdMap = std::unordered_map<uint16_t, uint16_t>;
using MessageIdMaps = std::array<LegacyIdMap, kMessageGroupCount>;

mods::flow::Query s_eventFlagQuery;
mods::flow::Query s_changeTimeQuery;
mods::flow::Query s_returnToSpawnQuery;
mods::flow::Event s_changeTimeEvent;
mods::flow::Event s_returnToSpawnEvent;
mods::flow::Event s_removeTradeItemEvent;
std::vector<mods::flow::Graph> s_graphs;
std::vector<mods::flow::RegisteredMessage> s_messages;
std::vector<mods::flow::MessageOverride> s_overrides;

uint32_t read_parameter(const uint8_t parameters[4]) {
    return static_cast<uint32_t>(parameters[0]) << 24 | static_cast<uint32_t>(parameters[1]) << 16 |
           static_cast<uint32_t>(parameters[2]) << 8 | parameters[3];
}

uint16_t query_event_flag(ModContext*, const FlowQueryContext* query, void*) {
    if (query == nullptr || query->parameter >= std::size(dSv_event_flag_c::saveBitLabels)) {
        return 0;
    }
    return dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[query->parameter]) ? 1 : 0;
}

uint16_t query_change_time(ModContext*, const FlowQueryContext*, void*) {
    if (!dKy_darkworld_check() && !daAlink_c::checkForestOldCentury() &&
        (daAlink_c::checkField() || daAlink_c::checkCastleTown()) &&
        !daAlink_c::checkStageName("R_SP161"))
    {
        return 0;
    }
    return 1;
}

uint16_t query_return_to_spawn(ModContext*, const FlowQueryContext*, void*) {
    const int stageId = getStageID();
    if (stageId <= Darkhammer) {
        return randomizer_GetContext().mReturnToPlaceOverrides.contains(stageId) ? 0 : 1;
    }
    return 2;
}

void event_change_time(ModContext*, const FlowEventContext*, void*) {
    if (daPy_py_c::checkNowWolf()) {
        g_randomizerState.setHasPendingToDChange(true);
    } else {
        g_randomizerState.handleTimeOfDayChange();
    }
}

void event_return_to_spawn(ModContext*, const FlowEventContext* event, void*) {
    if (event != nullptr) {
        randomizer_returnToSpawn(read_parameter(event->parameters) != 0);
    }
}

void event_remove_trade_item(ModContext*, const FlowEventContext* event, void*) {
    if (event == nullptr) {
        return;
    }
    const uint8_t item = static_cast<uint8_t>(read_parameter(event->parameters));
    if (item == dItemNo_LETTER_e || item == dItemNo_BILL_e || item == dItemNo_WOOD_STATUE_e ||
        item == dItemNo_IRIAS_PENDANT_e)
    {
        offWarashibeItem(item);
    }
}

std::vector<uint8_t> encoded_text(const std::string& text) {
    std::vector<uint8_t> result{text.begin(), text.end()};
    if (result.empty() || result.back() != 0) {
        result.push_back(0);
    }
    return result;
}

MessageEntryData default_message_entry() {
    MessageEntryData entry{};
    entry.bytes[8] = 0x24;
    entry.bytes[12] = 0xff;
    entry.bytes[16] = 0x02;
    entry.bytes[17] = 0x03;
    entry.bytes[18] = 0x04;
    return entry;
}

MessageEntryData message_entry(const RandomizerContext& context, uint32_t legacyKey) {
    MessageEntryData entry = default_message_entry();
    const auto found = context.mAttributeOverrides.find(legacyKey);
    if (found != context.mAttributeOverrides.end()) {
        std::memcpy(entry.bytes, found->second.data(), sizeof(entry.bytes));
        std::fill_n(entry.bytes, 6, uint8_t{});
    }
    return entry;
}

std::vector<uint32_t> text_keys(const RandomizerContext& context, uint16_t group) {
    std::unordered_set<uint32_t> unique;
    for (const auto& [language, overrides] : context.mTextOverrides) {
        for (const auto& [key, text] : overrides) {
            if (key >> 16 == group) {
                unique.insert(key);
            }
        }
    }
    std::vector<uint32_t> result{unique.begin(), unique.end()};
    std::ranges::sort(result);
    return result;
}

ModResult register_custom_messages(const RandomizerContext& context, MessageIdMaps& messageIds) {
    const auto keys = text_keys(context, CUSTOM_BMG_GROUP);
    for (uint16_t group = 0; group < kMessageGroupCount; ++group) {
        for (const uint32_t key : keys) {
            std::vector<mods::flow::MessageVariant> variants;
            for (const auto& [language, overrides] : context.mTextOverrides) {
                const auto found = overrides.find(key);
                if (found == overrides.end() || language < MESSAGE_LANGUAGE_ENGLISH ||
                    language > MESSAGE_LANGUAGE_ITALIAN)
                {
                    continue;
                }
                variants.emplace_back(static_cast<MessageLanguage>(language),
                    message_entry(context, key), encoded_text(found->second));
            }
            if (variants.empty()) {
                return MOD_INVALID_ARGUMENT;
            }
            auto message = mods::flow::register_message(group, variants);
            if (!message) {
                return message.result();
            }
            messageIds[group].emplace(static_cast<uint16_t>(key), message.id());
            s_messages.push_back(std::move(message));
        }
    }
    return MOD_OK;
}

bool formatted_override(
    ModContext*, const MessageOverrideContext* message, MessageTextData* outText, void*) {
    if (message == nullptr || outText == nullptr || !randomizer_IsActive()) {
        return false;
    }
    const uint32_t key = static_cast<uint32_t>(message->group) << 16 | message->message_id;
    const auto language = static_cast<int>(message->language);
    const auto languageIt = randomizer_GetContext().mTextOverrides.find(language);
    if (languageIt == randomizer_GetContext().mTextOverrides.end()) {
        return false;
    }
    const auto textIt = languageIt->second.find(key);
    if (textIt == languageIt->second.end()) {
        return false;
    }

    uint32_t value = 0;
    // For item counts, execItemGet hasn't run yet, so add one to the count
    switch (key) {
    case 325:  // Group 0, id 325
        // Poe Soul get item text
        value = dComIfGs_getPohSpiritNum() + 1;
        break;
    case 335:  // Group 0, id 335
        // Sky book characters get item text
        value = getAncientDocumentNum() + 1;
        break;
    default:
        return false;
    }

    thread_local std::vector<uint8_t> formatted;
    formatted = encoded_text(fmt::format(fmt::runtime(textIt->second), value));
    outText->text = formatted.data();
    outText->text_size = formatted.size();
    return true;
}

ModResult register_native_overrides(const RandomizerContext& context) {
    for (uint16_t group = 0; group < kMessageGroupCount; ++group) {
        for (const uint32_t key : text_keys(context, group)) {
            for (const auto& [language, overrides] : context.mTextOverrides) {
                const auto found = overrides.find(key);
                if (found == overrides.end() || language < MESSAGE_LANGUAGE_ENGLISH ||
                    language > MESSAGE_LANGUAGE_ITALIAN)
                {
                    continue;
                }
                mods::flow::MessageOverride message;
                if (key == 325 || key == 335) {
                    message = mods::flow::override_message_fn(group, static_cast<uint16_t>(key),
                        static_cast<MessageLanguage>(language), formatted_override);
                } else {
                    const auto text = encoded_text(found->second);
                    message = mods::flow::override_message(group, static_cast<uint16_t>(key),
                        static_cast<MessageLanguage>(language), std::span{text});
                }
                if (!message) {
                    return message.result();
                }
                s_overrides.push_back(std::move(message));
            }
        }
    }
    return MOD_OK;
}

std::vector<uint16_t> custom_node_ids(const RandomizerContext& context) {
    std::vector<uint16_t> result;
    for (const auto& [key, node] : context.mFlowPatches) {
        if (key >> 16 == CUSTOM_BMG_GROUP) {
            result.push_back(static_cast<uint16_t>(key));
        }
    }
    std::ranges::sort(result);
    return result;
}

ModResult allocate_shared_node_ids(const std::array<FlowGraphHandle, kMessageGroupCount>& handles,
    size_t count, std::array<std::vector<uint16_t>, kMessageGroupCount>& allocated,
    LegacyIdMap& nodeIds, const std::vector<uint16_t>& legacyIds) {
    if (count == 0) {
        return MOD_OK;
    }

    std::array<std::unordered_set<uint16_t>, kMessageGroupCount> allocatedSets;
    std::vector<uint16_t> common;
    while (common.size() < count) {
        for (size_t group = 0; group < handles.size(); ++group) {
            uint16_t id = 0;
            const ModResult result = svc_flow->allocate_node(mod_ctx, handles[group], &id);
            if (result != MOD_OK) {
                return result;
            }
            allocated[group].push_back(id);
            allocatedSets[group].insert(id);
        }

        common.clear();
        for (const uint16_t candidate : allocatedSets.front()) {
            const bool present = std::ranges::all_of(
                allocatedSets, [candidate](const auto& ids) { return ids.contains(candidate); });
            if (present) {
                common.push_back(candidate);
            }
        }
    }

    std::ranges::sort(common);
    for (size_t i = 0; i < legacyIds.size(); ++i) {
        nodeIds.emplace(legacyIds[i], common[i]);
    }
    return MOD_OK;
}

ModResult remap_target(uint16_t legacy, const LegacyIdMap& nodeIds, uint16_t& out) {
    if (legacy == mods::flow::kEnd || legacy < BASE_CUSTOM_MSG_AND_FLOW_ID) {
        out = legacy;
        return MOD_OK;
    }
    const auto found = nodeIds.find(legacy);
    if (found == nodeIds.end()) {
        return MOD_INVALID_ARGUMENT;
    }
    out = found->second;
    return MOD_OK;
}

ModResult remap_query(FlowNodeData& node) {
    const uint16_t legacy = mods::read_bits<uint16_t>(node.bytes + 2);
    FlowQueryId query = legacy;
    switch (legacy) {
    case kLegacyQueryEventFlag:
        query = s_eventFlagQuery.id();
        break;
    case kLegacyQueryChangeTime:
        query = s_changeTimeQuery.id();
        break;
    case kLegacyQueryReturnToSpawn:
        query = s_returnToSpawnQuery.id();
        break;
    default:
        if (legacy >= FLOW_QUERY_BUILTIN_COUNT) {
            return MOD_INVALID_ARGUMENT;
        }
        break;
    }
    mods::write_bits(node.bytes + 2, query);
    return MOD_OK;
}

ModResult remap_event(FlowNodeData& node) {
    switch (node.bytes[1]) {
    case kLegacyEventNoOp:
        node.bytes[1] = FLOW_EVENT_UNUSED_42;
        break;
    case kLegacyEventChangeTime:
        node.bytes[1] = s_changeTimeEvent.id();
        break;
    case kLegacyEventReturnToSpawn:
        node.bytes[1] = s_returnToSpawnEvent.id();
        break;
    case kLegacyEventSetTrackerFlag:
        // Older seed files may contain the retired pre-grant tracker event.
        node.bytes[1] = FLOW_EVENT_UNUSED_42;
        break;
    case kLegacyEventRemoveTradeItem:
        node.bytes[1] = s_removeTradeItemEvent.id();
        break;
    default:
        if (node.bytes[1] >= FLOW_EVENT_BUILTIN_COUNT) {
            return MOD_INVALID_ARGUMENT;
        }
        break;
    }
    return MOD_OK;
}

FlowNodeData raw_node(uint64_t value) {
    FlowNodeData node{};
    std::memcpy(node.bytes, &value, sizeof(node.bytes));
    return node;
}

ModResult remap_node(const RandomizerContext& context, uint32_t legacyKey, uint16_t group,
    FlowGraphHandle handle, const LegacyIdMap& nodeIds, const MessageIdMaps& messageIds,
    FlowNodeData& node) {
    switch (node.bytes[0]) {
    case 1: {
        const uint16_t legacyMessage = mods::read_bits<uint16_t>(node.bytes + 2);
        if (legacyMessage >= BASE_CUSTOM_MSG_AND_FLOW_ID) {
            const auto found = messageIds[group].find(legacyMessage);
            if (found == messageIds[group].end()) {
                return MOD_INVALID_ARGUMENT;
            }
            mods::write_bits(node.bytes + 2, found->second);
        }
        uint16_t target = 0;
        const ModResult result =
            remap_target(mods::read_bits<uint16_t>(node.bytes + 4), nodeIds, target);
        if (result != MOD_OK) {
            return result;
        }
        mods::write_bits(node.bytes + 4, target);
        return MOD_OK;
    }
    case 2: {
        ModResult result = remap_query(node);
        if (result != MOD_OK) {
            return result;
        }
        const auto overrides = context.mFlowPatchesBranchOverrides.find(legacyKey);
        if (overrides == context.mFlowPatchesBranchOverrides.end()) {
            return MOD_OK;
        }
        if (overrides->second.empty() || overrides->second.size() != node.bytes[1] ||
            overrides->second.size() > std::numeric_limits<uint16_t>::max())
        {
            return MOD_INVALID_ARGUMENT;
        }
        std::vector<uint16_t> targets;
        targets.reserve(overrides->second.size());
        for (const uint16_t legacyTarget : overrides->second) {
            uint16_t target = 0;
            result = remap_target(legacyTarget, nodeIds, target);
            if (result != MOD_OK) {
                return result;
            }
            targets.push_back(target);
        }
        uint16_t firstEdge = 0;
        result = svc_flow->add_edges(
            mod_ctx, handle, targets.data(), static_cast<uint16_t>(targets.size()), &firstEdge);
        if (result == MOD_OK) {
            mods::write_bits(node.bytes + 6, firstEdge);
        }
        return result;
    }
    case 3: {
        ModResult result = remap_event(node);
        if (result != MOD_OK || node.bytes[1] == FLOW_EVENT_JUMP_FLOW) {
            return result;
        }
        const uint16_t legacyEdge = mods::read_bits<uint16_t>(node.bytes + 2);
        if (legacyEdge < BASE_CUSTOM_MSG_AND_FLOW_ID) {
            return MOD_OK;
        }
        uint16_t target = 0;
        result = remap_target(legacyEdge, nodeIds, target);
        if (result != MOD_OK) {
            return result;
        }
        uint16_t edge = 0;
        result = svc_flow->add_edges(mod_ctx, handle, &target, 1, &edge);
        if (result == MOD_OK) {
            mods::write_bits(node.bytes + 2, edge);
        }
        return result;
    }
    default:
        return MOD_INVALID_ARGUMENT;
    }
}

void remap_actor_record(std::vector<uint8_t>& bytes, const LegacyIdMap& nodeIds) {
    if (bytes.size() < sizeof(stage_actor_data_class)) {
        return;
    }
    stage_actor_data_class actor{};
    std::memcpy(&actor, bytes.data(), sizeof(actor));
    if (std::memcmp(actor.name, "Obj_kn2", 7) != 0) {
        return;
    }
    const uint16_t legacy = static_cast<uint16_t>(actor.base.angle.x);
    const auto found = nodeIds.find(legacy);
    if (found == nodeIds.end()) {
        return;
    }
    actor.base.angle.x = static_cast<int16_t>(found->second);
    std::memcpy(bytes.data(), &actor, sizeof(actor));
}

void remap_actor_flows(RandomizerContext& context, const LegacyIdMap& nodeIds) {
    for (auto& [stage, patches] : context.mObjectPatches) {
        for (auto& [crc, bytes] : patches) {
            remap_actor_record(bytes, nodeIds);
        }
    }
    for (auto& [stage, additions] : context.mObjectAdditions) {
        for (auto& bytes : additions) {
            remap_actor_record(bytes, nodeIds);
        }
    }
}

ModResult register_graphs(RandomizerContext& context, const MessageIdMaps& messageIds) {
    const auto legacyIds = custom_node_ids(context);
    std::array<FlowGraphHandle, kMessageGroupCount> handles{};
    std::array<mods::flow::Graph, kMessageGroupCount> guards;
    for (uint16_t group = 0; group < kMessageGroupCount; ++group) {
        ModResult result = svc_flow->begin_graph(mod_ctx, group, &handles[group]);
        if (result != MOD_OK) {
            return result;
        }
        guards[group] = mods::flow::Graph{handles[group], MOD_OK};
    }

    std::array<std::vector<uint16_t>, kMessageGroupCount> allocated;
    LegacyIdMap nodeIds;
    ModResult result =
        allocate_shared_node_ids(handles, legacyIds.size(), allocated, nodeIds, legacyIds);
    if (result != MOD_OK) {
        return result;
    }

    std::unordered_map<uint16_t, uint16_t> dynamicToLegacy;
    for (const auto& [legacy, dynamic] : nodeIds) {
        dynamicToLegacy.emplace(dynamic, legacy);
    }

    const FlowNodeData unused = mods::flow::event(FLOW_EVENT_UNUSED_42, 0, {});
    for (uint16_t group = 0; group < kMessageGroupCount; ++group) {
        for (const uint16_t dynamicId : allocated[group]) {
            FlowNodeData node = unused;
            const auto legacy = dynamicToLegacy.find(dynamicId);
            if (legacy != dynamicToLegacy.end()) {
                const uint32_t key = static_cast<uint32_t>(CUSTOM_BMG_GROUP) << 16 | legacy->second;
                const auto raw = context.mFlowPatches.find(key);
                if (raw == context.mFlowPatches.end()) {
                    return MOD_INVALID_ARGUMENT;
                }
                node = raw_node(raw->second);
                result = remap_node(context, key, group, handles[group], nodeIds, messageIds, node);
                if (result != MOD_OK) {
                    return result;
                }
            }
            result = svc_flow->fill_node(mod_ctx, handles[group], dynamicId, &node);
            if (result != MOD_OK) {
                return result;
            }
        }

        for (const auto& [key, value] : context.mFlowPatches) {
            if (key >> 16 != group) {
                continue;
            }
            FlowNodeData node = raw_node(value);
            result = remap_node(context, key, group, handles[group], nodeIds, messageIds, node);
            if (result != MOD_OK) {
                return result;
            }
            result =
                svc_flow->patch_node(mod_ctx, handles[group], static_cast<uint16_t>(key), &node);
            if (result != MOD_OK) {
                return result;
            }
        }
    }

    for (const FlowGraphHandle handle : handles) {
        result = svc_flow->commit_graph(mod_ctx, handle);
        if (result != MOD_OK) {
            return result;
        }
    }
    for (auto& graph : guards) {
        s_graphs.push_back(std::move(graph));
    }
    remap_actor_flows(context, nodeIds);
    return MOD_OK;
}

}  // namespace

ModResult initialize() {
    s_eventFlagQuery = mods::flow::register_query("randomizer event flag", query_event_flag);
    s_changeTimeQuery = mods::flow::register_query("randomizer change time", query_change_time);
    s_returnToSpawnQuery =
        mods::flow::register_query("randomizer return to spawn", query_return_to_spawn);
    s_changeTimeEvent = mods::flow::register_event("randomizer change time", event_change_time);
    s_returnToSpawnEvent =
        mods::flow::register_event("randomizer return to spawn", event_return_to_spawn);
    s_removeTradeItemEvent =
        mods::flow::register_event("randomizer remove trade item", event_remove_trade_item);

    if (!s_eventFlagQuery) {
        return s_eventFlagQuery.result();
    }
    if (!s_changeTimeQuery) {
        return s_changeTimeQuery.result();
    }
    if (!s_returnToSpawnQuery) {
        return s_returnToSpawnQuery.result();
    }
    if (!s_changeTimeEvent) {
        return s_changeTimeEvent.result();
    }
    if (!s_returnToSpawnEvent) {
        return s_returnToSpawnEvent.result();
    }
    return s_removeTradeItemEvent ? MOD_OK : s_removeTradeItemEvent.result();
}

ModResult activate(RandomizerContext& context) {
    deactivate();

    MessageIdMaps messageIds;
    ModResult result = register_custom_messages(context, messageIds);
    if (result == MOD_OK) {
        result = register_native_overrides(context);
    }
    if (result == MOD_OK) {
        result = register_graphs(context, messageIds);
    }
    if (result != MOD_OK) {
        mods::log::error("failed to register randomizer flow data: {}", static_cast<int>(result));
        deactivate();
    }
    return result;
}

void deactivate() {
    s_graphs.clear();
    s_overrides.clear();
    s_messages.clear();
}

}  // namespace randomizer::messages
