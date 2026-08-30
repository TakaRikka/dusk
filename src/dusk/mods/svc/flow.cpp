#include "flow.hpp"

#include "registry.hpp"

#include "dusk/logging.h"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/settings.h"

#include "helpers/bits.hpp"

#include "JSystem/JMessage/control.h"
#include "JSystem/JMessage/processor.h"
#include "JSystem/JMessage/resource.h"
#include "d/d_msg_class.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <exception>
#include <limits>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace dusk::flow {
namespace {

constexpr uint16_t kCustomMax = 0xfffe;
constexpr uint16_t kGroupMax = 8;
constexpr size_t kDebugNameMax = 256;

struct ResourceInfo {
    const uint8_t* bmg = nullptr;
    uint16_t group = 0;
    const uint8_t* nodes = nullptr;
    const uint8_t* edges = nullptr;
    uint16_t nodeCount = 0;
    uint16_t edgeCount = 0;
    const uint8_t* entries = nullptr;
    uint16_t entryCount = 0;
    uint16_t entrySize = 0;
    const uint8_t* textBegin = nullptr;
    const uint8_t* textEnd = nullptr;
    bool valid = false;
};

struct PatchRecord {
    FlowGraphHandle graph{};
    mods::LoadedMod* owner = nullptr;
    uint16_t group = 0;
    uint16_t index = 0;
    uint64_t sequence = 0;
    bool edge = false;
    bool active = false;
    FlowNodeData node{};
    uint16_t target = 0;
};

struct GraphRecord {
    FlowGraphHandle handle{};
    mods::LoadedMod* owner = nullptr;
    uint16_t group = 0;
    std::unordered_map<uint16_t, std::optional<FlowNodeData>> nodes;
    std::unordered_map<uint16_t, uint16_t> edges;
    std::vector<std::pair<uint16_t, uint16_t>> edgeRuns;
    bool committed = false;
};

struct QueryRecord {
    mods::LoadedMod* owner = nullptr;
    std::string debugName;
    FlowQueryFn fn = nullptr;
    void* userData = nullptr;
};

struct EventRecord {
    mods::LoadedMod* owner = nullptr;
    std::string debugName;
    FlowEventFn fn = nullptr;
    void* userData = nullptr;
};

struct MessageVariantRecord {
    uint8_t language = 0;
    MessageEntryData entry{};
    std::vector<uint8_t> text;
};

struct MessageRecord {
    MessageHandle handle{};
    MessageId id{};
    uint16_t group = 0;
    mods::LoadedMod* owner = nullptr;
    std::unordered_map<uint8_t, std::shared_ptr<const MessageVariantRecord>> variants;
};

struct OverrideKey {
    uint16_t group = 0;
    uint16_t messageId = 0;
    uint8_t language = 0;
    bool operator==(const OverrideKey&) const = default;
};

struct OverrideKeyHash {
    size_t operator()(const OverrideKey& key) const {
        return static_cast<size_t>(key.group) << 24 | static_cast<size_t>(key.messageId) << 8 |
               key.language;
    }
};

struct OverrideRecord {
    MessageOverrideHandle handle{};
    mods::LoadedMod* owner = nullptr;
    uint64_t sequence = 0;
    bool callback = false;
    std::vector<uint8_t> text;
    MessageOverrideFn fn = nullptr;
    void* userData = nullptr;
};

struct ActiveBinding {
    std::vector<std::shared_ptr<const MessageVariantRecord>> variants;
    std::vector<std::shared_ptr<const std::vector<uint8_t>>> texts;
};

std::unordered_map<const void*, ResourceInfo> s_resources;
std::vector<PatchRecord> s_patches;
std::unordered_map<FlowGraphHandle, GraphRecord> s_graphs;
std::unordered_map<uint16_t, QueryRecord> s_queries;
std::unordered_map<uint8_t, EventRecord> s_events;
std::unordered_map<MessageId, std::shared_ptr<MessageRecord>> s_messages;
std::unordered_map<MessageHandle, std::shared_ptr<MessageRecord>> s_messagesByHandle;
std::unordered_map<OverrideKey, std::vector<OverrideRecord>, OverrideKeyHash> s_overrides;
std::unordered_map<const JMessage::TControl*, ActiveBinding> s_bindings;
std::unordered_map<const JMessage::TProcessor*, ActiveBinding> s_processorBindings;

std::array<uint16_t, kGroupMax + 1> s_nextNode{};
std::array<uint16_t, kGroupMax + 1> s_nextEdge{};
uint16_t s_nextMessage = kCustomMessageMin;
uint16_t s_nextQuery = kCustomQueryMin;
uint16_t s_nextEvent = kCustomEventMin;
std::vector<uint16_t> s_freeQueries;
std::vector<uint8_t> s_freeEvents;
std::array<std::vector<uint16_t>, kGroupMax + 1> s_freeNodes;
std::array<std::vector<std::pair<uint16_t, uint16_t>>, kGroupMax + 1> s_freeEdgeRuns;
std::vector<uint16_t> s_freeMessages;
uint64_t s_nextHandle = 1;
uint64_t s_nextSequence = 1;

std::unordered_set<uint64_t> s_warnedConflicts;
std::unordered_set<uint32_t> s_warnedMissingCallbacks;
std::unordered_set<uint64_t> s_warnedUnresolved;

uint8_t active_language() {
    return static_cast<uint8_t>(getSettings().game.language.getValue());
}

bool valid_group(uint16_t group) {
    return group <= kGroupMax;
}

bool valid_debug_name(const char* name) {
    if (name == nullptr) {
        return false;
    }
    const std::string_view value{name};
    return !value.empty() && value.size() <= kDebugNameMax;
}

int32_t mod_priority(const mods::LoadedMod& mod) {
    int32_t priority = 0;
    for (const auto& candidate : mods::ModLoader::instance().mods()) {
        ++priority;
        if (&candidate == &mod) {
            return priority;
        }
    }
    return priority + 1;
}

const ResourceInfo* find_resource(const void* bmgData) {
    const auto found = s_resources.find(bmgData);
    return found != s_resources.end() && found->second.valid ? &found->second : nullptr;
}

bool parse_resource(const void* data, uint16_t group, ResourceInfo& out) {
    const auto* bmg = static_cast<const uint8_t*>(data);
    if (bmg == nullptr || std::memcmp(bmg, "MESGbmg1", 8) != 0) {
        return false;
    }
    if (read_bits<uint32_t>(bmg + 8) < 0x20) {
        return false;
    }

    out = {.bmg = bmg, .group = group};
    const uint8_t* section = nullptr;
    size_t sectionSize = 0;
    if (detail::find_section(bmg, MULTI_CHAR('INF1'), section, sectionSize)) {
        if (sectionSize < 16) {
            return false;
        }
        out.entryCount = read_bits<uint16_t>(section + 8);
        out.entrySize = read_bits<uint16_t>(section + 10);
        if (out.entrySize < sizeof(MessageEntryData) ||
            static_cast<size_t>(out.entryCount) * out.entrySize > sectionSize - 16)
        {
            return false;
        }
        out.entries = section + 16;
    }
    if (detail::find_section(bmg, MULTI_CHAR('DAT1'), section, sectionSize)) {
        out.textBegin = section + 8;
        out.textEnd = section + sectionSize;
    }
    if (detail::find_section(bmg, MULTI_CHAR('FLW1'), section, sectionSize)) {
        if (sectionSize < 16) {
            return false;
        }
        out.nodeCount = read_bits<uint16_t>(section + 8);
        out.edgeCount = read_bits<uint16_t>(section + 10);
        const auto nodesSize = static_cast<size_t>(out.nodeCount) * 8;
        const auto edgesSize = static_cast<size_t>(out.edgeCount) * 2;
        if (nodesSize + edgesSize > sectionSize - 16) {
            return false;
        }
        out.nodes = section + 16;
        out.edges = out.nodes + nodesSize;
    }

    if (out.nodeCount >= kCustomNodeMin || out.edgeCount >= kCustomEdgeMin ||
        out.entryCount >= kCustomMessageMin)
    {
        return false;
    }
    for (uint16_t i = 0; i < out.entryCount; ++i) {
        const uint8_t* entry = out.entries + static_cast<size_t>(i) * out.entrySize;
        if (read_bits<uint16_t>(entry + 4) >= kCustomMessageMin) {
            return false;
        }
    }
    for (uint16_t i = 0; i < out.nodeCount; ++i) {
        const uint8_t* node = out.nodes + static_cast<size_t>(i) * 8;
        if (node[0] == 1 && (read_bits<uint16_t>(node + 2) >= kCustomMessageMin ||
                                (read_bits<uint16_t>(node + 4) != kEnd &&
                                    read_bits<uint16_t>(node + 4) >= kCustomNodeMin)))
        {
            return false;
        }
        if (node[0] == 2) {
            const auto lastEdge = static_cast<uint32_t>(read_bits<uint16_t>(node + 6)) +
                                  (node[1] == 0 ? 0 : node[1] - 1);
            if (read_bits<uint16_t>(node + 2) >= kCustomQueryMin || lastEdge >= kCustomEdgeMin) {
                return false;
            }
        }
        if (node[0] == 3 && (node[1] >= kCustomEventMin ||
                                (node[1] != 9 && read_bits<uint16_t>(node + 2) >= kCustomEdgeMin)))
        {
            return false;
        }
    }
    for (uint16_t i = 0; i < out.edgeCount; ++i) {
        const auto target = read_bits<uint16_t>(out.edges + static_cast<size_t>(i) * 2);
        if (target != kEnd && target >= kCustomNodeMin) {
            return false;
        }
    }
    out.valid = true;
    return true;
}

const GraphRecord* graph_for_node(uint16_t group, uint16_t index, bool requireCommitted) {
    for (const auto& [handle, graph] : s_graphs) {
        if (graph.group == group && graph.nodes.contains(index) &&
            (!requireCommitted || graph.committed))
        {
            return &graph;
        }
    }
    return nullptr;
}

const GraphRecord* graph_for_edge(uint16_t group, uint16_t index, bool requireCommitted) {
    for (const auto& [handle, graph] : s_graphs) {
        if (graph.group == group && graph.edges.contains(index) &&
            (!requireCommitted || graph.committed))
        {
            return &graph;
        }
    }
    return nullptr;
}

bool owner_has_message(const mods::LoadedMod& owner, uint16_t group, uint16_t id) {
    const auto found = s_messages.find(id);
    return found != s_messages.end() && found->second->owner == &owner &&
           found->second->group == group;
}

bool valid_target_owner(const mods::LoadedMod& owner, uint16_t group, uint16_t target) {
    if (target == kEnd || target < kCustomNodeMin) {
        return true;
    }
    const auto* graph = graph_for_node(group, target, false);
    return graph != nullptr && graph->owner == &owner;
}

bool valid_edge_owner(const mods::LoadedMod& owner, uint16_t group, uint16_t edge) {
    if (edge < kCustomEdgeMin) {
        return true;
    }
    const auto* graph = graph_for_edge(group, edge, false);
    return graph != nullptr && graph->owner == &owner;
}

bool validate_node_owner(const mods::LoadedMod& owner, uint16_t group, const FlowNodeData& node) {
    const auto* bytes = node.bytes;
    switch (bytes[0]) {
    case 1: {
        const auto messageIndex = read_bits<uint16_t>(bytes + 2);
        const auto target = read_bits<uint16_t>(bytes + 4);
        if (messageIndex >= kCustomMessageMin &&
            (messageIndex > kCustomMessageMax || !owner_has_message(owner, group, messageIndex)))
        {
            return false;
        }
        return valid_target_owner(owner, group, target);
    }
    case 2: {
        const uint8_t resultCount = bytes[1];
        const auto query = read_bits<uint16_t>(bytes + 2);
        const auto firstEdge = read_bits<uint16_t>(bytes + 6);
        if (resultCount == 0 || static_cast<uint32_t>(firstEdge) + resultCount - 1 > kCustomMax) {
            return false;
        }
        if (query >= kCustomQueryMin) {
            const auto found = s_queries.find(query);
            if (query > kCustomMax || found == s_queries.end() || found->second.owner != &owner) {
                return false;
            }
        } else if (query >= FLOW_QUERY_BUILTIN_COUNT) {
            return false;
        }
        for (uint16_t i = 0; i < resultCount; ++i) {
            if (!valid_edge_owner(owner, group, static_cast<uint16_t>(firstEdge + i))) {
                return false;
            }
        }
        return true;
    }
    case 3: {
        const uint8_t event = bytes[1];
        const auto edge = read_bits<uint16_t>(bytes + 2);
        if (event >= kCustomEventMin) {
            const auto found = s_events.find(event);
            if (event == 0xff || found == s_events.end() || found->second.owner != &owner) {
                return false;
            }
        } else if (event >= FLOW_EVENT_BUILTIN_COUNT) {
            return false;
        }
        return event == 9 || valid_edge_owner(owner, group, edge);
    }
    default:
        return false;
    }
}

const PatchRecord* winning_patch(uint16_t group, uint16_t index, bool edge) {
    const PatchRecord* winner = nullptr;
    int32_t winnerPriority = 0;
    const mods::LoadedMod* firstOwner = nullptr;
    for (const auto& patch : s_patches) {
        if (patch.group != group || patch.index != index || patch.edge != edge || !patch.active ||
            !patch.owner->active)
        {
            continue;
        }
        if (firstOwner == nullptr) {
            firstOwner = patch.owner;
        } else if (firstOwner != patch.owner) {
            const uint64_t key =
                static_cast<uint64_t>(edge) << 63 | static_cast<uint64_t>(group) << 16 | index;
            if (s_warnedConflicts.insert(key).second) {
                DuskLog.warn("flow {} {}:{} is patched by multiple mods; later load wins",
                    edge ? "edge" : "node", group, index);
            }
        }
        const int32_t priority = mod_priority(*patch.owner);
        if (winner == nullptr || priority > winnerPriority ||
            (priority == winnerPriority && patch.sequence > winner->sequence))
        {
            winner = &patch;
            winnerPriority = priority;
        }
    }
    return winner;
}

bool target_in_resource(const ResourceInfo& resource, uint16_t target) {
    if (target == kEnd) {
        return true;
    }
    if (target < kCustomNodeMin) {
        return target < resource.nodeCount;
    }
    return graph_for_node(resource.group, target, true) != nullptr;
}

bool edge_in_resource(const ResourceInfo& resource, uint16_t edge) {
    if (edge < kCustomEdgeMin) {
        return edge < resource.edgeCount;
    }
    return graph_for_edge(resource.group, edge, true) != nullptr;
}

bool node_resolves(const ResourceInfo& resource, const FlowNodeData& node) {
    switch (node.bytes[0]) {
    case 1: {
        const auto messageIndex = read_bits<uint16_t>(node.bytes + 2);
        const auto customMessage = s_messages.find(messageIndex);
        const bool messageValid =
            messageIndex < kCustomMessageMin ?
                messageIndex < resource.entryCount :
                customMessage != s_messages.end() &&
                    customMessage->second->group == resource.group &&
                    customMessage->second->variants.contains(active_language());
        return messageValid && target_in_resource(resource, read_bits<uint16_t>(node.bytes + 4));
    }
    case 2: {
        const auto query = read_bits<uint16_t>(node.bytes + 2);
        if (node.bytes[1] == 0 || (query >= FLOW_QUERY_BUILTIN_COUNT && query < kCustomQueryMin) ||
            query == kEnd)
        {
            return false;
        }
        const auto firstEdge = read_bits<uint16_t>(node.bytes + 6);
        for (uint16_t i = 0; i < node.bytes[1]; ++i) {
            if (!edge_in_resource(resource, static_cast<uint16_t>(firstEdge + i))) {
                return false;
            }
        }
        return true;
    }
    case 3: {
        const uint8_t event = node.bytes[1];
        if ((event >= FLOW_EVENT_BUILTIN_COUNT && event < kCustomEventMin) || event == 0xff) {
            return false;
        }
        return event == 9 || edge_in_resource(resource, read_bits<uint16_t>(node.bytes + 2));
    }
    default:
        return false;
    }
}

bool valid_encoded_text(const uint8_t* text, size_t size) {
    if (text == nullptr || size == 0) {
        return false;
    }
    size_t offset = 0;
    while (offset < size) {
        if (text[offset] == 0) {
            return offset + 1 == size;
        }
        if (text[offset] == JMessage::data::gcTagBegin) {
            if (offset + 2 > size || text[offset + 1] < 5 || text[offset + 1] > size - offset) {
                return false;
            }
            const size_t tagSize = text[offset + 1];
            const uint32_t tag =
                MSGTAG_GROUP(text[offset + 2]) | read_bits<uint16_t>(text + offset + 3);
            const size_t argumentSize = tagSize - 5;
            if (tag == MSGTAG_COLOR && argumentSize != 1 && argumentSize != 4 && argumentSize != 8)
            {
                return false;
            }
            offset += tagSize;
        } else {
            ++offset;
        }
    }
    return false;
}

size_t encoded_text_size(const ResourceInfo& resource, const char* text) {
    const auto* bytes = reinterpret_cast<const uint8_t*>(text);
    if (bytes == nullptr || bytes < resource.textBegin || bytes >= resource.textEnd) {
        return 0;
    }
    const auto maximum = static_cast<size_t>(resource.textEnd - bytes);
    size_t offset = 0;
    while (offset < maximum) {
        if (bytes[offset] == 0) {
            return offset + 1;
        }
        if (bytes[offset] == JMessage::data::gcTagBegin) {
            if (offset + 2 > maximum || bytes[offset + 1] < 5 ||
                bytes[offset + 1] > maximum - offset)
            {
                return 0;
            }
            offset += bytes[offset + 1];
        } else {
            ++offset;
        }
    }
    return 0;
}

std::shared_ptr<const MessageVariantRecord> active_variant_shared(const MessageRecord& message) {
    const auto found = message.variants.find(active_language());
    return found != message.variants.end() ? found->second : nullptr;
}

std::shared_ptr<const std::vector<uint8_t>> resolve_override(
    const ResourceInfo& resource, uint16_t messageId, const char* originalText) {
    const OverrideKey key{resource.group, messageId, active_language()};
    const auto found = s_overrides.find(key);
    if (found == s_overrides.end()) {
        return nullptr;
    }
    std::vector<OverrideRecord> candidates;
    for (const auto& record : found->second) {
        if (record.owner->active) {
            candidates.push_back(record);
        }
    }
    std::ranges::sort(candidates, [](const auto& left, const auto& right) {
        const int32_t leftPriority = mod_priority(*left.owner);
        const int32_t rightPriority = mod_priority(*right.owner);
        return leftPriority != rightPriority ? leftPriority > rightPriority :
                                               left.sequence > right.sequence;
    });

    const size_t originalSize = encoded_text_size(resource, originalText);
    for (const auto& candidate : candidates) {
        if (!candidate.callback) {
            return std::make_shared<const std::vector<uint8_t>>(candidate.text);
        }
        const OverrideRecord local = candidate;
        MessageTextData resolved{};
        const MessageOverrideContext context{
            resource.group,
            messageId,
            active_language(),
            reinterpret_cast<const uint8_t*>(originalText),
            originalSize,
        };
        try {
            const bool accepted =
                local.fn(local.owner->context.get(), &context, &resolved, local.userData);
            if (accepted && valid_encoded_text(resolved.text, resolved.text_size)) {
                return std::make_shared<const std::vector<uint8_t>>(
                    resolved.text, resolved.text + resolved.text_size);
            }
            if (accepted) {
                dusk::mods::fail_mod(*local.owner, MOD_INVALID_ARGUMENT,
                    "message override returned malformed encoded text");
            }
        } catch (const std::exception& error) {
            dusk::mods::fail_mod(*local.owner, MOD_ERROR,
                fmt::format("exception in message override: {}", error.what()));
        } catch (...) {
            dusk::mods::fail_mod(*local.owner, MOD_ERROR, "unknown exception in message override");
        }
    }
    return nullptr;
}

void retain_binding(JMessage::TControl* control, const JMessage::TProcessor* processor,
    std::shared_ptr<const MessageVariantRecord> variant,
    std::shared_ptr<const std::vector<uint8_t>> text) {
    ActiveBinding* binding = nullptr;
    if (control != nullptr) {
        binding = &s_bindings[control];
    } else if (processor != nullptr) {
        binding = &s_processorBindings[processor];
    } else {
        return;
    }
    if (variant != nullptr &&
        std::ranges::find(binding->variants, variant) == binding->variants.end())
    {
        binding->variants.push_back(std::move(variant));
    }
    if (text != nullptr && std::ranges::find(binding->texts, text) == binding->texts.end()) {
        binding->texts.push_back(std::move(text));
    }
}

JMessage::TControl* control_for_processor(const JMessage::TProcessor* processor) {
    for (const auto& entry : s_bindings) {
        const auto* control = entry.first;
        if (control->pSequenceProcessor_ == processor || control->pRenderingProcessor_ == processor)
        {
            return const_cast<JMessage::TControl*>(control);
        }
    }
    return nullptr;
}

ModResult add_patch(
    GraphRecord& graph, uint16_t index, bool edge, const FlowNodeData* node, uint16_t target) {
    if (index >= kCustomNodeMin || (edge && index >= kCustomEdgeMin) || (!edge && node == nullptr))
    {
        return MOD_INVALID_ARGUMENT;
    }
    PatchRecord record{
        .graph = graph.handle,
        .owner = graph.owner,
        .group = graph.group,
        .index = index,
        .edge = edge,
        .target = target,
    };
    if (node != nullptr) {
        record.node = *node;
    }
    s_patches.push_back(record);
    return MOD_OK;
}

void reset_state() {
    s_resources.clear();
    s_patches.clear();
    s_graphs.clear();
    s_queries.clear();
    s_events.clear();
    s_messages.clear();
    s_messagesByHandle.clear();
    s_overrides.clear();
    s_bindings.clear();
    s_processorBindings.clear();
    s_nextNode.fill(kCustomNodeMin);
    s_nextEdge.fill(kCustomEdgeMin);
    s_nextMessage = kCustomMessageMin;
    s_nextQuery = kCustomQueryMin;
    s_nextEvent = kCustomEventMin;
    s_freeQueries.clear();
    s_freeEvents.clear();
    for (auto& freeNodes : s_freeNodes) {
        freeNodes.clear();
    }
    for (auto& freeRuns : s_freeEdgeRuns) {
        freeRuns.clear();
    }
    s_freeMessages.clear();
    s_nextHandle = 1;
    s_nextSequence = 1;
    s_warnedConflicts.clear();
    s_warnedMissingCallbacks.clear();
    s_warnedUnresolved.clear();
}

void remove_mod(mods::LoadedMod& mod) {
    std::erase_if(s_patches, [&](const auto& patch) { return patch.owner == &mod; });

    for (auto iterator = s_graphs.begin(); iterator != s_graphs.end();) {
        auto& graph = iterator->second;
        if (graph.owner == &mod) {
            auto& freeNodes = s_freeNodes[graph.group];
            for (const auto& [id, data] : graph.nodes) {
                freeNodes.push_back(id);
            }
            auto& freeRuns = s_freeEdgeRuns[graph.group];
            freeRuns.insert(freeRuns.end(), graph.edgeRuns.begin(), graph.edgeRuns.end());
            iterator = s_graphs.erase(iterator);
        } else {
            ++iterator;
        }
    }

    for (auto iterator = s_queries.begin(); iterator != s_queries.end();) {
        if (iterator->second.owner == &mod) {
            s_freeQueries.push_back(iterator->first);
            iterator = s_queries.erase(iterator);
        } else {
            ++iterator;
        }
    }
    for (auto iterator = s_events.begin(); iterator != s_events.end();) {
        if (iterator->second.owner == &mod) {
            s_freeEvents.push_back(iterator->first);
            iterator = s_events.erase(iterator);
        } else {
            ++iterator;
        }
    }

    for (auto iterator = s_messagesByHandle.begin(); iterator != s_messagesByHandle.end();) {
        if (iterator->second->owner == &mod) {
            s_freeMessages.push_back(iterator->second->id);
            s_messages.erase(iterator->second->id);
            iterator = s_messagesByHandle.erase(iterator);
        } else {
            ++iterator;
        }
    }
    for (auto iterator = s_overrides.begin(); iterator != s_overrides.end();) {
        std::erase_if(iterator->second, [&](const auto& record) { return record.owner == &mod; });
        iterator = iterator->second.empty() ? s_overrides.erase(iterator) : std::next(iterator);
    }
}

}  // namespace

bool bind_resource(const void* bmgData, uint16_t group) {
    if (bmgData == nullptr || !valid_group(group)) {
        return false;
    }
    ResourceInfo resource;
    if (!parse_resource(bmgData, group, resource)) {
        DuskLog.error(
            "message resource for group {} violates the custom ID reservation or is malformed",
            group);
        s_resources.insert_or_assign(
            bmgData, ResourceInfo{.bmg = static_cast<const uint8_t*>(bmgData), .group = group});
        return false;
    }
    s_resources.insert_or_assign(bmgData, resource);
    for (const auto& patch : s_patches) {
        if (patch.group != group || !patch.active) {
            continue;
        }
        const bool resolves =
            patch.edge ?
                patch.index < resource.edgeCount && target_in_resource(resource, patch.target) :
                patch.index < resource.nodeCount && node_resolves(resource, patch.node);
        if (resolves) {
            continue;
        }
        const uint64_t key = static_cast<uint64_t>(patch.edge) << 63 |
                             static_cast<uint64_t>(group) << 32 | patch.index;
        if (s_warnedUnresolved.insert(key).second) {
            DuskLog.error("[{}] flow {} patch {}:{:#06x} is unresolved for the loaded resource",
                patch.owner->metadata.id, patch.edge ? "edge" : "node", group, patch.index);
        }
    }
    return true;
}

bool resolve_node(const void* bmgData, uint16_t nodeIndex, FlowNodeData& outNode) {
    const auto* resource = find_resource(bmgData);
    if (resource == nullptr || nodeIndex == kEnd) {
        return false;
    }
    const auto unresolved = [&] {
        const uint64_t key = static_cast<uint64_t>(resource->group) << 32 | nodeIndex;
        if (s_warnedUnresolved.insert(key).second) {
            DuskLog.error("flow node {}:{:#06x} is unresolved; terminating at END", resource->group,
                nodeIndex);
        }
        return false;
    };
    if (nodeIndex < kCustomNodeMin) {
        if (nodeIndex >= resource->nodeCount) {
            return unresolved();
        }
        if (const auto* patch = winning_patch(resource->group, nodeIndex, false)) {
            outNode = patch->node;
        } else {
            std::memcpy(outNode.bytes, resource->nodes + static_cast<size_t>(nodeIndex) * 8, 8);
        }
    } else {
        const auto* graph = graph_for_node(resource->group, nodeIndex, true);
        if (graph == nullptr) {
            return unresolved();
        }
        outNode = *graph->nodes.at(nodeIndex);
    }
    if (!node_resolves(*resource, outNode)) {
        return unresolved();
    }
    return true;
}

bool resolve_edge(const void* bmgData, uint16_t edgeIndex, uint16_t& outTarget) {
    const auto* resource = find_resource(bmgData);
    if (resource == nullptr) {
        outTarget = kEnd;
        return false;
    }
    const auto unresolved = [&] {
        const uint64_t key =
            uint64_t{1} << 63 | static_cast<uint64_t>(resource->group) << 32 | edgeIndex;
        if (s_warnedUnresolved.insert(key).second) {
            DuskLog.error(
                "flow edge {}:{:#06x} is unresolved; targeting END", resource->group, edgeIndex);
        }
        outTarget = kEnd;
        return false;
    };
    if (edgeIndex < kCustomEdgeMin) {
        if (edgeIndex >= resource->edgeCount) {
            return unresolved();
        }
        if (const auto* patch = winning_patch(resource->group, edgeIndex, true)) {
            outTarget = patch->target;
        } else {
            outTarget = read_bits<uint16_t>(resource->edges + static_cast<size_t>(edgeIndex) * 2);
        }
    } else {
        const auto* graph = graph_for_edge(resource->group, edgeIndex, true);
        if (graph == nullptr) {
            return unresolved();
        }
        outTarget = graph->edges.at(edgeIndex);
    }
    if (!target_in_resource(*resource, outTarget)) {
        return unresolved();
    }
    return true;
}

bool resolve_message_entry(const void* bmgData, uint16_t messageIndex, MessageEntryData& outEntry) {
    const auto* resource = find_resource(bmgData);
    if (resource == nullptr) {
        return false;
    }
    if (messageIndex < kCustomMessageMin) {
        if (messageIndex >= resource->entryCount) {
            return false;
        }
        std::memcpy(outEntry.bytes,
            resource->entries + static_cast<size_t>(messageIndex) * resource->entrySize,
            sizeof(outEntry.bytes));
        return true;
    }
    const auto found = s_messages.find(messageIndex);
    if (found == s_messages.end() || found->second->group != resource->group) {
        return false;
    }
    const auto variant = active_variant_shared(*found->second);
    if (variant == nullptr) {
        return false;
    }
    outEntry = variant->entry;
    return true;
}

uint16_t dispatch_query(uint16_t queryId, const void* speakerActor, uint16_t parameter,
    uint8_t resultCount, FlowQueryPhase phase, uint16_t nodeIndex) {
    const auto found = s_queries.find(queryId);
    if (found == s_queries.end()) {
        if (s_warnedMissingCallbacks.insert(queryId).second) {
            DuskLog.warn("custom flow query {:#06x} is not registered; using result zero", queryId);
        }
        return 0;
    }
    const QueryRecord local = found->second;
    const FlowQueryContext context{
        speakerActor, parameter, resultCount, static_cast<uint8_t>(phase)};
    try {
        const uint16_t result = local.fn(local.owner->context.get(), &context, local.userData);
        if (result < resultCount) {
            return result;
        }
        dusk::mods::fail_mod(*local.owner, MOD_INVALID_ARGUMENT,
            fmt::format("flow query '{}' returned {} for {} results at node {:#06x}",
                local.debugName, result, resultCount, nodeIndex));
    } catch (const std::exception& error) {
        dusk::mods::fail_mod(*local.owner, MOD_ERROR,
            fmt::format("exception in flow query '{}': {}", local.debugName, error.what()));
    } catch (...) {
        dusk::mods::fail_mod(*local.owner, MOD_ERROR,
            fmt::format("unknown exception in flow query '{}'", local.debugName));
    }
    return 0;
}

void dispatch_event(uint8_t eventId, const void* speakerActor, const uint8_t parameters[4]) {
    const auto found = s_events.find(eventId);
    if (found == s_events.end()) {
        const uint32_t warningKey = 0x10000 | eventId;
        if (s_warnedMissingCallbacks.insert(warningKey).second) {
            DuskLog.warn("custom flow event {:#04x} is not registered; continuing", eventId);
        }
        return;
    }
    const EventRecord local = found->second;
    FlowEventContext context{.speaker_actor = speakerActor};
    std::memcpy(context.parameters, parameters, sizeof(context.parameters));
    try {
        local.fn(local.owner->context.get(), &context, local.userData);
    } catch (const std::exception& error) {
        dusk::mods::fail_mod(*local.owner, MOD_ERROR,
            fmt::format("exception in flow event '{}': {}", local.debugName, error.what()));
    } catch (...) {
        dusk::mods::fail_mod(*local.owner, MOD_ERROR,
            fmt::format("unknown exception in flow event '{}'", local.debugName));
    }
}

bool custom_message_group(uint16_t messageId, uint16_t& outGroup) {
    const auto found = s_messages.find(messageId);
    if (found == s_messages.end()) {
        return false;
    }
    outGroup = found->second->group;
    return true;
}

bool custom_message_for_control(
    JMessage::TControl* control, uint16_t messageId, const void*& outEntry, const char*& outText) {
    const auto found = s_messages.find(messageId);
    if (found == s_messages.end()) {
        return false;
    }
    const auto variant = active_variant_shared(*found->second);
    if (variant == nullptr) {
        return false;
    }
    outEntry = &variant->entry;
    outText = reinterpret_cast<const char*>(variant->text.data());
    retain_binding(control, nullptr, variant, nullptr);
    return true;
}

static const JMessage::TResource* processor_resource_for_group(
    const JMessage::TProcessor* processor, uint16_t group) {
    if (processor == nullptr) {
        return nullptr;
    }
    const auto matches_group = [group](const JMessage::TResource* resource) {
        if (resource == nullptr) {
            return false;
        }
        const auto* info = find_resource(resource->oParse_THeader_.getRaw());
        return info != nullptr && info->group == group;
    };
    if (matches_group(processor->getResourceCache())) {
        return processor->getResourceCache();
    }
    if (processor->getResourceContainer() == nullptr) {
        return nullptr;
    }
    const auto* resources = processor->getResourceContainer()->getResourceContainer();
    JGadget::TContainerEnumerator_const iterator{resources};
    while (iterator) {
        const JMessage::TResource& resource = *iterator;
        if (matches_group(&resource)) {
            return &resource;
        }
    }
    return nullptr;
}

bool message_code_for_id(
    const JMessage::TProcessor* processor, uint32_t messageId, uint32_t& outCode) {
    if (processor == nullptr || messageId > std::numeric_limits<uint16_t>::max()) {
        return false;
    }
    uint16_t group = 0;
    if (!custom_message_group(static_cast<uint16_t>(messageId), group)) {
        return false;
    }
    const auto* resource = processor_resource_for_group(processor, group);
    if (resource == nullptr) {
        return false;
    }
    outCode = static_cast<uint32_t>(resource->getGroupID()) << 16 | messageId;
    return true;
}

bool custom_message_for_processor(JMessage::TControl* control,
    const JMessage::TProcessor* processor, uint16_t messageIndex,
    const JMessage::TResource*& outResource, const void*& outEntry, const char*& outText) {
    const auto found = s_messages.find(messageIndex);
    if (found == s_messages.end()) {
        return false;
    }
    const auto* resource = processor_resource_for_group(processor, found->second->group);
    const auto variant = active_variant_shared(*found->second);
    if (resource == nullptr || variant == nullptr) {
        return false;
    }
    outResource = resource;
    outEntry = &variant->entry;
    outText = reinterpret_cast<const char*>(variant->text.data());
    retain_binding(control, processor, variant, nullptr);
    return true;
}

static bool resolve_message_impl(JMessage::TControl* control, const JMessage::TProcessor* processor,
    const void* bmgData, uint16_t messageIndex, const void* nativeEntry, const char* nativeText,
    const void*& outEntry, const char*& outText) {
    const auto* resource = find_resource(bmgData);
    if (resource == nullptr) {
        return false;
    }
    if (control != nullptr) {
        s_bindings.try_emplace(control);
    }
    if (messageIndex >= kCustomMessageMin) {
        const auto found = s_messages.find(messageIndex);
        if (found == s_messages.end() || found->second->group != resource->group) {
            return false;
        }
        const auto variant = active_variant_shared(*found->second);
        if (variant == nullptr) {
            return false;
        }
        outEntry = &variant->entry;
        outText = reinterpret_cast<const char*>(variant->text.data());
        retain_binding(control, processor, variant, nullptr);
        return true;
    }
    if (nativeEntry == nullptr || nativeText == nullptr) {
        return false;
    }
    const auto* bytes = static_cast<const uint8_t*>(nativeEntry);
    const auto messageId = read_bits<uint16_t>(bytes + 4);
    auto overrideText = resolve_override(*resource, messageId, nativeText);
    if (overrideText == nullptr) {
        return false;
    }
    outEntry = nativeEntry;
    outText = reinterpret_cast<const char*>(overrideText->data());
    retain_binding(control, processor, nullptr, overrideText);
    return true;
}

bool resolve_message_for_control(JMessage::TControl* control, const void* bmgData,
    uint16_t messageIndex, const void* nativeEntry, const char* nativeText, const void*& outEntry,
    const char*& outText) {
    return resolve_message_impl(
        control, nullptr, bmgData, messageIndex, nativeEntry, nativeText, outEntry, outText);
}

bool resolve_message(const JMessage::TProcessor* processor, const void* bmgData,
    uint16_t messageIndex, const void* nativeEntry, const char* nativeText, const void*& outEntry,
    const char*& outText) {
    return resolve_message_impl(control_for_processor(processor), processor, bmgData, messageIndex,
        nativeEntry, nativeText, outEntry, outText);
}

void release_message_control(const JMessage::TControl* control) {
    s_bindings.erase(control);
}

void release_message_processor(const JMessage::TProcessor* processor) {
    s_processorBindings.erase(processor);
}

}  // namespace dusk::flow

namespace dusk::mods::svc {
namespace {

ModResult begin_graph(ModContext* context, uint16_t group, FlowGraphHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outHandle == nullptr || !flow::valid_group(group)) {
        return MOD_INVALID_ARGUMENT;
    }
    const FlowGraphHandle handle = flow::s_nextHandle++;
    flow::s_graphs.emplace(
        handle, flow::GraphRecord{.handle = handle, .owner = mod, .group = group});
    *outHandle = handle;
    return MOD_OK;
}

flow::GraphRecord* owned_graph(ModContext* context, FlowGraphHandle handle) {
    const auto* mod = mod_from_context(context);
    const auto found = flow::s_graphs.find(handle);
    return mod != nullptr && found != flow::s_graphs.end() && found->second.owner == mod ?
               &found->second :
               nullptr;
}

ModResult allocate_node(ModContext* context, FlowGraphHandle handle, uint16_t* outId) {
    if (outId != nullptr) {
        *outId = 0;
    }
    auto* graph = owned_graph(context, handle);
    if (graph == nullptr || graph->committed || outId == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto& freeNodes = flow::s_freeNodes[graph->group];
    uint16_t id = 0;
    if (!freeNodes.empty()) {
        id = freeNodes.back();
        freeNodes.pop_back();
    } else if (flow::s_nextNode[graph->group] < flow::kEnd) {
        id = flow::s_nextNode[graph->group]++;
    } else {
        DuskLog.error("[{}] custom flow node pool for group {} is exhausted",
            graph->owner->metadata.id, graph->group);
        return MOD_UNAVAILABLE;
    }
    graph->nodes.emplace(id, std::nullopt);
    *outId = id;
    return MOD_OK;
}

ModResult add_edges(ModContext* context, FlowGraphHandle handle, const uint16_t* targets,
    uint16_t count, uint16_t* outFirst) {
    if (outFirst != nullptr) {
        *outFirst = 0;
    }
    auto* graph = owned_graph(context, handle);
    if (graph == nullptr || graph->committed || targets == nullptr || count == 0 ||
        outFirst == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    auto& freeRuns = flow::s_freeEdgeRuns[graph->group];
    const auto run =
        std::ranges::find_if(freeRuns, [&](const auto& item) { return item.second >= count; });
    uint16_t first = 0;
    if (run != freeRuns.end()) {
        first = run->first;
        run->first += count;
        run->second -= count;
        if (run->second == 0) {
            freeRuns.erase(run);
        }
    } else if (static_cast<uint32_t>(flow::s_nextEdge[graph->group]) + count <= flow::kEnd) {
        first = flow::s_nextEdge[graph->group];
        flow::s_nextEdge[graph->group] = static_cast<uint16_t>(first + count);
    } else {
        DuskLog.error("[{}] custom flow edge pool for group {} is exhausted",
            graph->owner->metadata.id, graph->group);
        return MOD_UNAVAILABLE;
    }
    for (uint16_t i = 0; i < count; ++i) {
        graph->edges.emplace(static_cast<uint16_t>(first + i), targets[i]);
    }
    graph->edgeRuns.emplace_back(first, count);
    *outFirst = first;
    return MOD_OK;
}

ModResult fill_node(
    ModContext* context, FlowGraphHandle handle, uint16_t nodeIndex, const FlowNodeData* node) {
    auto* graph = owned_graph(context, handle);
    if (graph == nullptr || graph->committed || node == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto slot = graph->nodes.find(nodeIndex);
    if (slot == graph->nodes.end()) {
        return MOD_INVALID_ARGUMENT;
    }
    slot->second = *node;
    return MOD_OK;
}

ModResult patch_node(
    ModContext* context, FlowGraphHandle handle, uint16_t nodeIndex, const FlowNodeData* node) {
    auto* graph = owned_graph(context, handle);
    if (graph == nullptr || graph->committed) {
        return MOD_INVALID_ARGUMENT;
    }
    return flow::add_patch(*graph, nodeIndex, false, node, 0);
}

ModResult patch_edge(
    ModContext* context, FlowGraphHandle handle, uint16_t edgeIndex, uint16_t targetNode) {
    auto* graph = owned_graph(context, handle);
    if (graph == nullptr || graph->committed) {
        return MOD_INVALID_ARGUMENT;
    }
    return flow::add_patch(*graph, edgeIndex, true, nullptr, targetNode);
}

ModResult commit_graph(ModContext* context, FlowGraphHandle handle) {
    auto* graph = owned_graph(context, handle);
    const bool hasPatches =
        graph != nullptr && std::ranges::any_of(flow::s_patches,
                                [&](const auto& patch) { return patch.graph == handle; });
    if (graph == nullptr || graph->committed ||
        (graph->nodes.empty() && graph->edges.empty() && !hasPatches) ||
        std::ranges::any_of(
            graph->nodes, [](const auto& item) { return !item.second.has_value(); }))
    {
        return MOD_INVALID_ARGUMENT;
    }
    for (const auto& [id, node] : graph->nodes) {
        if (!flow::validate_node_owner(*graph->owner, graph->group, *node)) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    for (const auto& [id, target] : graph->edges) {
        if (!flow::valid_target_owner(*graph->owner, graph->group, target)) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    for (const auto& patch : flow::s_patches) {
        if (patch.graph != handle) {
            continue;
        }
        const bool valid = patch.edge ?
                               flow::valid_target_owner(*graph->owner, graph->group, patch.target) :
                               flow::validate_node_owner(*graph->owner, graph->group, patch.node);
        if (!valid) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    for (auto& patch : flow::s_patches) {
        if (patch.graph == handle) {
            patch.sequence = flow::s_nextSequence++;
            patch.active = true;
        }
    }
    graph->committed = true;
    return MOD_OK;
}

ModResult remove_graph(ModContext* context, FlowGraphHandle handle) {
    auto* graph = owned_graph(context, handle);
    if (graph == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    std::erase_if(flow::s_patches, [&](const auto& patch) { return patch.graph == handle; });
    flow::s_graphs.erase(handle);
    return MOD_OK;
}

ModResult register_query(ModContext* context, const char* debugName, FlowQueryFn fn, void* userData,
    FlowQueryId* outId) {
    if (outId != nullptr) {
        *outId = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !flow::valid_debug_name(debugName) || fn == nullptr || outId == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    uint16_t id = 0;
    if (!flow::s_freeQueries.empty()) {
        id = flow::s_freeQueries.back();
        flow::s_freeQueries.pop_back();
    } else if (flow::s_nextQuery <= flow::kCustomMax) {
        id = flow::s_nextQuery++;
    } else {
        DuskLog.error("[{}] flow query pool exhausted (32767 registrations)", mod->metadata.id);
        return MOD_UNAVAILABLE;
    }
    flow::s_queries.emplace(id, flow::QueryRecord{mod, debugName, fn, userData});
    *outId = id;
    return MOD_OK;
}

ModResult register_event(ModContext* context, const char* debugName, FlowEventFn fn, void* userData,
    FlowEventId* outId) {
    if (outId != nullptr) {
        *outId = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !flow::valid_debug_name(debugName) || fn == nullptr || outId == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    uint8_t id = 0;
    if (!flow::s_freeEvents.empty()) {
        id = flow::s_freeEvents.back();
        flow::s_freeEvents.pop_back();
    } else if (flow::s_nextEvent <= 0xfe) {
        id = static_cast<uint8_t>(flow::s_nextEvent++);
    } else {
        DuskLog.error("[{}] flow event pool exhausted (127 registrations)", mod->metadata.id);
        return MOD_UNAVAILABLE;
    }
    flow::s_events.emplace(id, flow::EventRecord{mod, debugName, fn, userData});
    *outId = id;
    return MOD_OK;
}

ModResult override_message(ModContext* context, uint16_t group, uint16_t messageId,
    uint8_t language, const uint8_t* text, size_t textSize, MessageOverrideHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outHandle == nullptr || !flow::valid_group(group) ||
        messageId >= flow::kCustomMessageMin || !flow::valid_encoded_text(text, textSize))
    {
        return MOD_INVALID_ARGUMENT;
    }
    flow::OverrideRecord record{
        .handle = flow::s_nextHandle++,
        .owner = mod,
        .sequence = flow::s_nextSequence++,
        .text = {text, text + textSize},
    };
    *outHandle = record.handle;
    flow::s_overrides[{group, messageId, language}].push_back(std::move(record));
    return MOD_OK;
}

ModResult override_message_fn(ModContext* context, uint16_t group, uint16_t messageId,
    uint8_t language, MessageOverrideFn fn, void* userData, MessageOverrideHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outHandle == nullptr || !flow::valid_group(group) ||
        messageId >= flow::kCustomMessageMin || fn == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    flow::OverrideRecord record{
        .handle = flow::s_nextHandle++,
        .owner = mod,
        .sequence = flow::s_nextSequence++,
        .callback = true,
        .fn = fn,
        .userData = userData,
    };
    *outHandle = record.handle;
    flow::s_overrides[{group, messageId, language}].push_back(std::move(record));
    return MOD_OK;
}

ModResult remove_override(ModContext* context, MessageOverrideHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    for (auto iterator = flow::s_overrides.begin(); iterator != flow::s_overrides.end(); ++iterator)
    {
        const size_t removed = std::erase_if(iterator->second,
            [&](const auto& record) { return record.handle == handle && record.owner == mod; });
        if (removed != 0) {
            if (iterator->second.empty()) {
                flow::s_overrides.erase(iterator);
            }
            return MOD_OK;
        }
    }
    return MOD_INVALID_ARGUMENT;
}

ModResult register_message(ModContext* context, uint16_t group, const MessageVariantData* variants,
    size_t variantCount, MessageId* outId, MessageHandle* outHandle) {
    if (outId != nullptr) {
        *outId = 0;
    }
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || !flow::valid_group(group) || variants == nullptr || variantCount == 0 ||
        outId == nullptr || outHandle == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint16_t messageId = 0;
    if (!flow::s_freeMessages.empty()) {
        messageId = flow::s_freeMessages.back();
        flow::s_freeMessages.pop_back();
    } else if (flow::s_nextMessage <= flow::kCustomMessageMax) {
        messageId = flow::s_nextMessage++;
    } else {
        DuskLog.error("[{}] custom message ID pool exhausted", mod->metadata.id);
        return MOD_UNAVAILABLE;
    }
    auto message = std::make_shared<flow::MessageRecord>();
    message->handle = flow::s_nextHandle++;
    message->id = messageId;
    message->group = group;
    message->owner = mod;
    for (size_t i = 0; i < variantCount; ++i) {
        const auto& input = variants[i];
        if (read_bits<uint32_t>(input.entry.bytes) != 0 ||
            read_bits<uint16_t>(input.entry.bytes + 4) != 0 ||
            !flow::valid_encoded_text(input.text, input.text_size) ||
            message->variants.contains(input.language))
        {
            return MOD_INVALID_ARGUMENT;
        }
        auto variant = std::make_shared<flow::MessageVariantRecord>();
        variant->language = input.language;
        variant->entry = input.entry;
        write_bits(variant->entry.bytes + 4, message->id);
        variant->text.assign(input.text, input.text + input.text_size);
        message->variants.emplace(input.language, std::move(variant));
    }
    flow::s_messages.emplace(message->id, message);
    flow::s_messagesByHandle.emplace(message->handle, message);
    *outId = message->id;
    *outHandle = message->handle;
    return MOD_OK;
}

ModResult remove_message(ModContext* context, MessageHandle handle) {
    auto* mod = mod_from_context(context);
    const auto found = flow::s_messagesByHandle.find(handle);
    if (mod == nullptr || handle == 0 || found == flow::s_messagesByHandle.end() ||
        found->second->owner != mod)
    {
        return MOD_INVALID_ARGUMENT;
    }
    flow::s_messages.erase(found->second->id);
    flow::s_messagesByHandle.erase(found);
    return MOD_OK;
}

constexpr FlowService s_flowService{
    .header = SERVICE_HEADER(FlowService, FLOW_SERVICE_MAJOR, FLOW_SERVICE_MINOR),
    .begin_graph = begin_graph,
    .allocate_node = allocate_node,
    .add_edges = add_edges,
    .fill_node = fill_node,
    .patch_node = patch_node,
    .patch_edge = patch_edge,
    .commit_graph = commit_graph,
    .remove_graph = remove_graph,
    .register_query = register_query,
    .register_event = register_event,
};

constexpr MessageService s_messageService{
    .header = SERVICE_HEADER(MessageService, MESSAGE_SERVICE_MAJOR, MESSAGE_SERVICE_MINOR),
    .override_message = override_message,
    .override_message_fn = override_message_fn,
    .remove_override = remove_override,
    .register_message = register_message,
    .remove_message = remove_message,
};

}  // namespace

constinit const ServiceModule g_flowModule{
    .id = FLOW_SERVICE_ID,
    .majorVersion = FLOW_SERVICE_MAJOR,
    .minorVersion = FLOW_SERVICE_MINOR,
    .service = &s_flowService,
    .initialize = flow::reset_state,
    .modDetached = flow::remove_mod,
    .shutdown = flow::reset_state,
};

constinit const ServiceModule g_messageModule{
    .id = MESSAGE_SERVICE_ID,
    .majorVersion = MESSAGE_SERVICE_MAJOR,
    .minorVersion = MESSAGE_SERVICE_MINOR,
    .service = &s_messageService,
};

}  // namespace dusk::mods::svc
