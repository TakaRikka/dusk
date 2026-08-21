#include "messages.hpp"

#include "randomizer_context.hpp"
#include "stages.h"
#include "tools.h"

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_kankyo.h"
#include "d/d_save.h"
#include "d/d_stage.h"

#include <mods/svc/flow.hpp>
#include <mods/svc/log.hpp>

#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <span>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace randomizer::messages {
namespace {

constexpr uint16_t kMessageGroupCount = 9;
constexpr uint32_t kPoeSoulGetMessage = 325;
constexpr uint32_t kSkyCharacterGetMessage = 335;

using MessageIdMaps = std::array<std::unordered_map<std::string, MessageId>, kMessageGroupCount>;
using NodeIdMaps = std::array<std::unordered_map<std::string, uint16_t>, kMessageGroupCount>;

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

std::array<uint8_t, 4> parameter_bytes(uint32_t parameter) {
    return {
        static_cast<uint8_t>(parameter >> 24),
        static_cast<uint8_t>(parameter >> 16),
        static_cast<uint8_t>(parameter >> 8),
        static_cast<uint8_t>(parameter),
    };
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

mods::flow::MessageStyle message_style(const RandomizerContext::MessageStyleData& source) {
    return mods::flow::MessageStyle{}
        .event_label_id(source.eventLabelId)
        .speaker(source.speaker)
        .box_kind(static_cast<MessageBoxKind>(source.boxKind))
        .draw_type(static_cast<MessageDrawType>(source.drawType))
        .box_position(static_cast<MessageBoxPosition>(source.boxPosition))
        .line_alignment(source.lineAlignment)
        .speaker_mood(source.speakerMood)
        .camera_attr(source.cameraAttr)
        .talk_anim(source.talkAnim)
        .face_anim(source.faceAnim)
        .trailing_data(source.trailingData);
}

ModResult register_custom_messages(const RandomizerContext& context, MessageIdMaps& messageIds) {
    for (const auto& definition : context.mCustomMessages) {
        if (definition.group >= kMessageGroupCount || definition.name.empty() ||
            messageIds[definition.group].contains(definition.name))
        {
            return MOD_INVALID_ARGUMENT;
        }
        const auto style = message_style(definition.style);
        std::vector<mods::flow::MessageVariant> variants;
        for (const auto& [language, text] : definition.text) {
            if (language < MESSAGE_LANGUAGE_ENGLISH || language > MESSAGE_LANGUAGE_ITALIAN) {
                continue;
            }
            variants.emplace_back(static_cast<MessageLanguage>(language),
                style.data(), encoded_text(text));
        }
        if (variants.empty()) {
            return MOD_INVALID_ARGUMENT;
        }
        auto message = mods::flow::register_message(definition.group, variants);
        if (!message) {
            return message.result();
        }
        messageIds[definition.group].emplace(definition.name, message.id());
        s_messages.push_back(std::move(message));
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
    // Message resolution runs before the acquisition updates these counters.
    switch (key) {
    case kPoeSoulGetMessage:
        value = dComIfGs_getPohSpiritNum() + 1;
        break;
    case kSkyCharacterGetMessage:
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
    for (const auto& [language, overrides] : context.mTextOverrides) {
        if (language < MESSAGE_LANGUAGE_ENGLISH || language > MESSAGE_LANGUAGE_ITALIAN) {
            continue;
        }
        for (const auto& [key, value] : overrides) {
            const uint16_t group = key >> 16;
            const uint16_t messageId = static_cast<uint16_t>(key);
            mods::flow::MessageOverride message;
            if (key == kPoeSoulGetMessage || key == kSkyCharacterGetMessage) {
                message = mods::flow::override_message_fn(group, messageId,
                    static_cast<MessageLanguage>(language), formatted_override);
            } else {
                const auto text = encoded_text(value);
                message = mods::flow::override_message(
                    group, messageId, static_cast<MessageLanguage>(language), std::span{text});
            }
            if (!message) {
                return message.result();
            }
            s_overrides.push_back(std::move(message));
        }
    }
    return MOD_OK;
}

bool resolve_query(const std::string& name, FlowQueryId& out) {
    static const std::unordered_map<std::string, FlowQueryId> builtins{
        {"event flag", FLOW_QUERY_EVENT_FLAG},
        {"rupees", FLOW_QUERY_RUPEES},
        {"item owned", FLOW_QUERY_ITEM_OWNED},
        {"empty bottles", FLOW_QUERY_EMPTY_BOTTLES},
        {"select 2 cancel", FLOW_QUERY_SELECT_2_CANCEL},
        {"select 3 cancel", FLOW_QUERY_SELECT_3_CANCEL},
    };
    if (const auto found = builtins.find(name); found != builtins.end()) {
        out = found->second;
        return true;
    }
    if (name == "randomizer event flag") {
        out = s_eventFlagQuery.id();
    } else if (name == "randomizer change time") {
        out = s_changeTimeQuery.id();
    } else if (name == "randomizer return to spawn") {
        out = s_returnToSpawnQuery.id();
    } else {
        return false;
    }
    return true;
}

bool resolve_event(const std::string& name, FlowEventId& out) {
    static const std::unordered_map<std::string, FlowEventId> builtins{
        {"remove rupees", FLOW_EVENT_REMOVE_RUPEES},
        {"start event", FLOW_EVENT_START_EVENT},
        {"select vertical", FLOW_EVENT_SELECT_VERTICAL},
        {"set switch", FLOW_EVENT_SET_SWITCH},
        {"shop sold out", FLOW_EVENT_SHOP_SOLD_OUT},
        {"add donation", FLOW_EVENT_ADD_DONATION},
        {"no-op", FLOW_EVENT_UNUSED_42},
    };
    if (const auto found = builtins.find(name); found != builtins.end()) {
        out = found->second;
        return true;
    }
    if (name == "randomizer change time") {
        out = s_changeTimeEvent.id();
    } else if (name == "randomizer return to spawn") {
        out = s_returnToSpawnEvent.id();
    } else if (name == "randomizer remove trade item") {
        out = s_removeTradeItemEvent.id();
    } else {
        return false;
    }
    return true;
}

bool resolve_reference(const RandomizerContext::FlowReference& reference, uint16_t group,
    const NodeIdMaps& nodeIds, uint16_t& out) {
    if (reference.nativeId.has_value()) {
        out = reference.nativeId.value();
        return true;
    }
    const auto found = nodeIds[group].find(reference.name);
    if (found == nodeIds[group].end()) {
        return false;
    }
    out = found->second;
    return true;
}

bool resolve_message(const RandomizerContext::FlowReference& reference, uint16_t group,
    const MessageIdMaps& messageIds, uint16_t& out) {
    if (reference.nativeId.has_value()) {
        out = reference.nativeId.value();
        return true;
    }
    const auto found = messageIds[group].find(reference.name);
    if (found == messageIds[group].end()) {
        return false;
    }
    out = found->second;
    return true;
}

ModResult add_custom_node(const RandomizerContext::FlowNode& flow,
    const MessageIdMaps& messageIds, mods::flow::GraphBuilder& builder,
    mods::flow::NodeRef& out) {
    if (flow.type == RandomizerContext::FlowNodeType::MESSAGE) {
        uint16_t message = 0;
        if (!resolve_message(flow.message, flow.group, messageIds, message)) {
            return MOD_INVALID_ARGUMENT;
        }
        out = builder.add_message(message);
        return MOD_OK;
    }
    if (flow.type == RandomizerContext::FlowNodeType::BRANCH) {
        FlowQueryId query = 0;
        if (flow.parameters > 0xffff || !resolve_query(flow.operation, query)) {
            return MOD_INVALID_ARGUMENT;
        }
        out = builder.add_branch(query, static_cast<uint16_t>(flow.parameters));
        return MOD_OK;
    }
    FlowEventId event = 0;
    if (!resolve_event(flow.operation, event)) {
        return MOD_INVALID_ARGUMENT;
    }
    out = builder.add_event(event, parameter_bytes(flow.parameters));
    return MOD_OK;
}

ModResult wire_custom_node(const RandomizerContext::FlowNode& flow, uint16_t group,
    const NodeIdMaps& nodeIds, mods::flow::NodeRef node) {
    if (flow.type == RandomizerContext::FlowNodeType::BRANCH) {
        std::vector<uint16_t> targets;
        targets.reserve(flow.results.size());
        for (const auto& result : flow.results) {
            uint16_t target = 0;
            if (!resolve_reference(result, group, nodeIds, target)) {
                return MOD_INVALID_ARGUMENT;
            }
            targets.push_back(target);
        }
        node.results(targets);
        return MOD_OK;
    }
    uint16_t target = 0;
    if (!resolve_reference(flow.next, group, nodeIds, target)) {
        return MOD_INVALID_ARGUMENT;
    }
    node.next(target);
    return MOD_OK;
}

ModResult apply_flow_patch(const RandomizerContext::FlowNode& flow, uint16_t group,
    const MessageIdMaps& messageIds, const NodeIdMaps& nodeIds,
    mods::flow::GraphBuilder& builder) {
    if (!flow.patchIndex.has_value()) {
        return MOD_INVALID_ARGUMENT;
    }
    if (flow.type == RandomizerContext::FlowNodeType::MESSAGE) {
        uint16_t message = 0;
        uint16_t target = 0;
        if (!resolve_message(flow.message, group, messageIds, message) ||
            !resolve_reference(flow.next, group, nodeIds, target))
        {
            return MOD_INVALID_ARGUMENT;
        }
        builder.patch_node(flow.patchIndex.value(), mods::flow::message(0, message, target));
        return MOD_OK;
    }
    if (flow.type == RandomizerContext::FlowNodeType::BRANCH) {
        FlowQueryId query = 0;
        if (flow.parameters > 0xffff || !resolve_query(flow.operation, query)) {
            return MOD_INVALID_ARGUMENT;
        }
        std::vector<uint16_t> targets;
        targets.reserve(flow.results.size());
        for (const auto& result : flow.results) {
            uint16_t target = 0;
            if (!resolve_reference(result, group, nodeIds, target)) {
                return MOD_INVALID_ARGUMENT;
            }
            targets.push_back(target);
        }
        builder.patch_branch(flow.patchIndex.value(), query,
            static_cast<uint16_t>(flow.parameters), targets);
        return MOD_OK;
    }
    FlowEventId event = 0;
    uint16_t target = 0;
    if (!resolve_event(flow.operation, event) ||
        !resolve_reference(flow.next, group, nodeIds, target))
    {
        return MOD_INVALID_ARGUMENT;
    }
    builder.patch_event(
        flow.patchIndex.value(), event, parameter_bytes(flow.parameters), target);
    return MOD_OK;
}

ModResult resolve_actor_flow(RandomizerContext::ActorData& actor, uint16_t group,
    const NodeIdMaps& nodeIds) {
    if (actor.flow.empty()) {
        return MOD_OK;
    }
    const auto found = nodeIds[group].find(actor.flow);
    if (found == nodeIds[group].end() || actor.bytes.size() < sizeof(stage_actor_data_class)) {
        return MOD_INVALID_ARGUMENT;
    }
    stage_actor_data_class data{};
    std::memcpy(&data, actor.bytes.data(), sizeof(data));
    data.base.angle.x = static_cast<int16_t>(found->second);
    std::memcpy(actor.bytes.data(), &data, sizeof(data));
    return MOD_OK;
}

ModResult resolve_actor_flows(RandomizerContext& context, const NodeIdMaps& nodeIds) {
    auto group_for_key = [](uint32_t key, uint16_t& group) {
        const uint32_t stage = key >> 16;
        if (stage >= std::size(allStageMessageGroups)) {
            return false;
        }
        group = allStageMessageGroups[stage];
        return group < kMessageGroupCount;
    };
    for (auto& [key, patches] : context.mObjectPatches) {
        uint16_t group = 0;
        if (!group_for_key(key, group)) {
            return MOD_INVALID_ARGUMENT;
        }
        for (auto& [crc, actor] : patches) {
            const ModResult result = resolve_actor_flow(actor, group, nodeIds);
            if (result != MOD_OK) {
                return result;
            }
        }
    }
    for (auto& [key, additions] : context.mObjectAdditions) {
        uint16_t group = 0;
        if (!group_for_key(key, group)) {
            return MOD_INVALID_ARGUMENT;
        }
        for (auto& actor : additions) {
            const ModResult result = resolve_actor_flow(actor, group, nodeIds);
            if (result != MOD_OK) {
                return result;
            }
        }
    }
    return MOD_OK;
}

ModResult register_graphs(RandomizerContext& context, const MessageIdMaps& messageIds) {
    NodeIdMaps nodeIds;
    for (const auto& flow : context.mFlowNodes) {
        if (flow.group >= kMessageGroupCount) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    for (uint16_t group = 0; group < kMessageGroupCount; ++group) {
        bool hasNodes = false;
        for (const auto& flow : context.mFlowNodes) {
            hasNodes |= flow.group == group;
        }
        if (!hasNodes) {
            continue;
        }

        mods::flow::GraphBuilder builder{group};
        std::unordered_map<std::string, mods::flow::NodeRef> refs;
        for (const auto& flow : context.mFlowNodes) {
            if (flow.group != group || flow.patchIndex.has_value()) {
                continue;
            }
            if (flow.name.empty() || refs.contains(flow.name)) {
                return MOD_INVALID_ARGUMENT;
            }
            mods::flow::NodeRef ref;
            const ModResult result = add_custom_node(flow, messageIds, builder, ref);
            if (result != MOD_OK) {
                return result;
            }
            refs.emplace(flow.name, ref);
            nodeIds[group].emplace(flow.name, ref.id());
        }
        for (const auto& flow : context.mFlowNodes) {
            if (flow.group != group || flow.patchIndex.has_value()) {
                continue;
            }
            const ModResult result = wire_custom_node(flow, group, nodeIds, refs.at(flow.name));
            if (result != MOD_OK) {
                return result;
            }
        }
        for (const auto& flow : context.mFlowNodes) {
            if (flow.group != group || !flow.patchIndex.has_value()) {
                continue;
            }
            const ModResult result =
                apply_flow_patch(flow, group, messageIds, nodeIds, builder);
            if (result != MOD_OK) {
                return result;
            }
        }
        auto graph = builder.commit();
        if (!graph) {
            return graph.result();
        }
        s_graphs.push_back(std::move(graph));
    }
    return resolve_actor_flows(context, nodeIds);
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
