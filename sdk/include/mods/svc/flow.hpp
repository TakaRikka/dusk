#pragma once

#include <mods/bits.hpp>
#include <mods/svc/flow.h>
#include <mods/svc/message.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace mods::flow {

static_assert(sizeof(FlowNodeData) == 8);
static_assert(sizeof(MessageEntryData) == 20);

inline constexpr uint16_t kCustomNodeMin = 0x8000;
inline constexpr uint16_t kCustomEdgeMin = 0x8000;
inline constexpr uint16_t kCustomMessageMin = 0x8000;
inline constexpr uint16_t kEnd = 0xffff;

/* entryIndex is the INF1 entry index, not the message ID stored inside the entry.
 * For custom messages the entry index equals the MessageId. */
constexpr FlowNodeData message(
    uint8_t subtype, uint16_t entryIndex, uint16_t nextNode, uint16_t unknown = 0) {
    FlowNodeData node{};
    node.bytes[0] = 1;
    node.bytes[1] = subtype;
    write_bits(node.bytes + 2, entryIndex);
    write_bits(node.bytes + 4, nextNode);
    write_bits(node.bytes + 6, unknown);
    return node;
}

constexpr FlowNodeData branch(
    uint8_t resultCount, FlowQueryId query, uint16_t parameter, uint16_t firstEdge) {
    FlowNodeData node{};
    node.bytes[0] = 2;
    node.bytes[1] = resultCount;
    write_bits(node.bytes + 2, query);
    write_bits(node.bytes + 4, parameter);
    write_bits(node.bytes + 6, firstEdge);
    return node;
}

constexpr FlowNodeData event(FlowEventId eventId, uint16_t edge, std::array<uint8_t, 4> params) {
    FlowNodeData node{};
    node.bytes[0] = 3;
    node.bytes[1] = eventId;
    write_bits(node.bytes + 2, edge);
    for (size_t i = 0; i < params.size(); ++i) {
        node.bytes[4 + i] = params[i];
    }
    return node;
}

class Graph {
public:
    Graph() = default;
    Graph(FlowGraphHandle handle, ModResult result) : mHandle{handle}, mResult{result} {}
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;
    Graph(Graph&& other) noexcept { *this = std::move(other); }
    Graph& operator=(Graph&& other) noexcept {
        if (this != &other) {
            reset();
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }
    ~Graph() { reset(); }

    explicit operator bool() const { return mResult == MOD_OK; }
    ModResult result() const { return mResult; }
    FlowGraphHandle handle() const { return mHandle; }
    void reset() {
        if (mHandle != 0 && svc_flow != nullptr) {
            svc_flow->remove_graph(mod_ctx, mHandle);
            mHandle = 0;
        }
    }

private:
    FlowGraphHandle mHandle{};
    ModResult mResult = MOD_UNAVAILABLE;
};

class GraphBuilder;

class NodeRef {
public:
    NodeRef() = default;

    /* Set the successor of a message or event node: a node ID or kEnd */
    NodeRef next(uint16_t target) const;
    /* Set a branch node's result targets */
    NodeRef results(std::initializer_list<uint16_t> targets) const;
    NodeRef results(std::span<const uint16_t> targets) const;

    uint16_t id() const { return mId; }
    operator uint16_t() const { return mId; }

private:
    friend class GraphBuilder;
    NodeRef(GraphBuilder* builder, uint16_t id) : mBuilder{builder}, mId{id} {}

    GraphBuilder* mBuilder = nullptr;
    uint16_t mId = 0;
};

class GraphBuilder {
public:
    explicit GraphBuilder(uint16_t group) {
        if (svc_flow == nullptr) {
            mResult = MOD_UNAVAILABLE;
            return;
        }
        mResult = svc_flow->begin_graph(mod_ctx, group, &mHandle);
    }
    GraphBuilder(const GraphBuilder&) = delete;
    GraphBuilder& operator=(const GraphBuilder&) = delete;
    ~GraphBuilder() {
        if (mHandle != 0 && svc_flow != nullptr) {
            svc_flow->remove_graph(mod_ctx, mHandle);
        }
    }

    /* entryIndex is an INF1 entry index for native messages, or a registered MessageId */
    NodeRef add_message(uint16_t entryIndex, uint8_t subtype = 0) {
        return append(message(subtype, entryIndex, 0), true);
    }
    NodeRef add_branch(FlowQueryId query, uint16_t parameter) {
        return append(branch(0, query, parameter, 0), true);
    }
    NodeRef add_event(FlowEventId eventId, std::array<uint8_t, 4> params) {
        return append(event(eventId, 0, params), true);
    }
    NodeRef add_node(const FlowNodeData& node) { return append(node, false); }

    GraphBuilder& patch_node(uint16_t nodeIndex, const FlowNodeData& node) {
        if (mResult == MOD_OK) {
            mResult = svc_flow->patch_node(mod_ctx, mHandle, nodeIndex, &node);
        }
        return *this;
    }
    GraphBuilder& patch_edge(uint16_t edgeIndex, uint16_t targetNode) {
        if (mResult == MOD_OK) {
            mResult = svc_flow->patch_edge(mod_ctx, mHandle, edgeIndex, targetNode);
        }
        return *this;
    }
    GraphBuilder& patch_branch(uint16_t nodeIndex, FlowQueryId query, uint16_t parameter,
        std::span<const uint16_t> targets) {
        if (mResult != MOD_OK || targets.empty() || targets.size() > 0xff) {
            if (mResult == MOD_OK) {
                mResult = MOD_INVALID_ARGUMENT;
            }
            return *this;
        }
        uint16_t first = 0;
        mResult = svc_flow->add_edges(
            mod_ctx, mHandle, targets.data(), static_cast<uint16_t>(targets.size()), &first);
        if (mResult == MOD_OK) {
            patch_node(nodeIndex,
                branch(static_cast<uint8_t>(targets.size()), query, parameter, first));
        }
        return *this;
    }
    GraphBuilder& patch_event(uint16_t nodeIndex, FlowEventId eventId,
        std::array<uint8_t, 4> params, uint16_t target) {
        if (mResult != MOD_OK) {
            return *this;
        }
        uint16_t edge = 0;
        mResult = svc_flow->add_edges(mod_ctx, mHandle, &target, 1, &edge);
        if (mResult == MOD_OK) {
            patch_node(nodeIndex, event(eventId, edge, params));
        }
        return *this;
    }

    Graph commit() {
        for (const auto& node : mNodes) {
            if (node.typed && !node.wired) {
                mResult = MOD_INVALID_ARGUMENT;
            }
        }
        for (const auto& node : mNodes) {
            if (mResult != MOD_OK) {
                break;
            }
            mResult = svc_flow->fill_node(mod_ctx, mHandle, node.id, &node.data);
        }
        if (mResult == MOD_OK) {
            mResult = svc_flow->commit_graph(mod_ctx, mHandle);
        }
        if (mResult != MOD_OK) {
            return {0, mResult};  // the builder destructor removes the graph
        }
        return {std::exchange(mHandle, 0), MOD_OK};
    }

private:
    friend class NodeRef;

    struct BuilderNode {
        uint16_t id;
        FlowNodeData data;
        bool typed;
        bool wired;
    };

    NodeRef append(const FlowNodeData& node, bool typed) {
        uint16_t id = 0;
        if (mResult == MOD_OK) {
            mResult = svc_flow->allocate_node(mod_ctx, mHandle, &id);
        }
        if (mResult == MOD_OK) {
            mNodes.push_back({id, node, typed, false});
        }
        return {this, id};
    }

    BuilderNode* find(uint16_t id) {
        for (auto& node : mNodes) {
            if (node.id == id) {
                return &node;
            }
        }
        return nullptr;
    }

    void set_next(uint16_t id, uint16_t target) {
        if (mResult != MOD_OK) {
            return;
        }
        auto* node = find(id);
        if (node == nullptr || node->wired) {
            mResult = MOD_INVALID_ARGUMENT;
            return;
        }
        node->wired = true;
        switch (node->data.bytes[0]) {
        case 1:
            write_bits(node->data.bytes + 4, target);
            break;
        case 3: {
            uint16_t first = 0;
            mResult = svc_flow->add_edges(mod_ctx, mHandle, &target, 1, &first);
            if (mResult == MOD_OK) {
                write_bits(node->data.bytes + 2, first);
            }
            break;
        }
        default:
            mResult = MOD_INVALID_ARGUMENT;
        }
    }

    void set_results(uint16_t id, std::span<const uint16_t> targets) {
        if (mResult != MOD_OK) {
            return;
        }
        auto* node = find(id);
        if (node == nullptr || node->wired || node->data.bytes[0] != 2 || targets.size() == 0 ||
            targets.size() > 0xff)
        {
            mResult = MOD_INVALID_ARGUMENT;
            return;
        }
        node->wired = true;
        uint16_t first = 0;
        mResult = svc_flow->add_edges(
            mod_ctx, mHandle, targets.data(), static_cast<uint16_t>(targets.size()), &first);
        if (mResult == MOD_OK) {
            node->data.bytes[1] = static_cast<uint8_t>(targets.size());
            write_bits(node->data.bytes + 6, first);
        }
    }

    FlowGraphHandle mHandle{};
    std::vector<BuilderNode> mNodes;
    ModResult mResult = MOD_OK;
};

inline NodeRef NodeRef::next(uint16_t target) const {
    if (mBuilder != nullptr) {
        mBuilder->set_next(mId, target);
    }
    return *this;
}

inline NodeRef NodeRef::results(std::initializer_list<uint16_t> targets) const {
    return results(std::span{targets.begin(), targets.size()});
}

inline NodeRef NodeRef::results(std::span<const uint16_t> targets) const {
    if (mBuilder != nullptr) {
        mBuilder->set_results(mId, targets);
    }
    return *this;
}

/* Single-patch convenience functions */
inline Graph patch_node(uint16_t group, uint16_t nodeIndex, const FlowNodeData& node) {
    GraphBuilder builder{group};
    builder.patch_node(nodeIndex, node);
    return builder.commit();
}

inline Graph patch_edge(uint16_t group, uint16_t edgeIndex, uint16_t targetNode) {
    GraphBuilder builder{group};
    builder.patch_edge(edgeIndex, targetNode);
    return builder.commit();
}

class Query {
public:
    Query() = default;
    Query(FlowQueryId id, ModResult result) : mId{id}, mResult{result} {}
    explicit operator bool() const { return mResult == MOD_OK; }
    FlowQueryId id() const { return mId; }
    ModResult result() const { return mResult; }

private:
    FlowQueryId mId{};
    ModResult mResult = MOD_UNAVAILABLE;
};

inline Query register_query(const char* debugName, FlowQueryFn fn, void* userData = nullptr) {
    FlowQueryId id{};
    const ModResult result = svc_flow != nullptr ?
                                 svc_flow->register_query(mod_ctx, debugName, fn, userData, &id) :
                                 MOD_UNAVAILABLE;
    return {id, result};
}

class Event {
public:
    Event() = default;
    Event(FlowEventId id, ModResult result) : mId{id}, mResult{result} {}
    explicit operator bool() const { return mResult == MOD_OK; }
    FlowEventId id() const { return mId; }
    ModResult result() const { return mResult; }

private:
    FlowEventId mId{};
    ModResult mResult = MOD_UNAVAILABLE;
};

inline Event register_event(const char* debugName, FlowEventFn fn, void* userData = nullptr) {
    FlowEventId id{};
    const ModResult result = svc_flow != nullptr ?
                                 svc_flow->register_event(mod_ctx, debugName, fn, userData, &id) :
                                 MOD_UNAVAILABLE;
    return {id, result};
}

/* Message presentation/style attributes (INF1) */
class MessageStyle {
public:
    constexpr MessageStyle() { mData.bytes[12] = 0xff; }
    constexpr explicit MessageStyle(MessageEntryData data) : mData{data} {}

    /* saveBitLabels index set when the message displays. */
    [[nodiscard]] constexpr MessageStyle event_label_id(uint16_t value) const {
        return set_u16(6, value);
    }
    /* Z2SpeechMgr2 voice bank ID */
    [[nodiscard]] constexpr MessageStyle speaker(uint8_t value) const { return set_u8(8, value); }
    [[nodiscard]] constexpr MessageStyle box_kind(MessageBoxKind value) const {
        return set_u8(9, static_cast<uint8_t>(value));
    }
    [[nodiscard]] constexpr MessageStyle draw_type(MessageDrawType value) const {
        return set_u8(10, static_cast<uint8_t>(value));
    }
    [[nodiscard]] constexpr MessageStyle box_position(MessageBoxPosition value) const {
        return set_u8(11, static_cast<uint8_t>(value));
    }
    /* 0 centered (JP builds only), 1 left */
    [[nodiscard]] constexpr MessageStyle line_alignment(uint8_t value) const {
        return set_u8(13, value);
    }
    /* Grunt emotion index for the voice bank */
    [[nodiscard]] constexpr MessageStyle speaker_mood(uint8_t value) const {
        return set_u8(14, value);
    }
    /* 1-10 focus talk-actor slot, >=11 talk-camera style, 0 none */
    [[nodiscard]] constexpr MessageStyle camera_attr(uint8_t value) const {
        return set_u8(15, value);
    }
    /* NPC talk motion attribute */
    [[nodiscard]] constexpr MessageStyle talk_anim(uint8_t value) const {
        return set_u8(16, value);
    }
    /* NPC talk face attribute */
    [[nodiscard]] constexpr MessageStyle face_anim(uint8_t value) const {
        return set_u8(17, value);
    }
    /* INF1 bytes 18-19, normally 0x0400. */
    [[nodiscard]] constexpr MessageStyle trailing_data(uint16_t value) const {
        return set_u16(18, value);
    }
    [[nodiscard]] constexpr const MessageEntryData& data() const { return mData; }

private:
    [[nodiscard]] constexpr MessageStyle set_u8(size_t offset, uint8_t value) const {
        MessageStyle style = *this;
        style.mData.bytes[offset] = value;
        return style;
    }
    [[nodiscard]] constexpr MessageStyle set_u16(size_t offset, uint16_t value) const {
        MessageStyle style = *this;
        write_bits(style.mData.bytes + offset, value);
        return style;
    }
    MessageEntryData mData{};
};

class MessageVariant {
public:
    MessageVariant() = default;
    MessageVariant(MessageLanguage language, MessageEntryData entry, std::vector<uint8_t> text,
        ModResult result = MOD_OK)
        : mLanguage{language}, mEntry{entry}, mText{std::move(text)}, mResult{result} {}

    explicit operator bool() const { return mResult == MOD_OK; }
    ModResult result() const { return mResult; }
    MessageLanguage language() const { return mLanguage; }
    const MessageEntryData& entry() const { return mEntry; }
    const std::vector<uint8_t>& text() const { return mText; }
    MessageVariantData data() const {
        return {static_cast<uint8_t>(mLanguage), mEntry, mText.data(), mText.size()};
    }

private:
    MessageLanguage mLanguage = MESSAGE_LANGUAGE_ENGLISH;
    MessageEntryData mEntry{};
    std::vector<uint8_t> mText;
    ModResult mResult = MOD_UNAVAILABLE;
};

class MessageBuilder {
public:
    explicit MessageBuilder(MessageStyle style = {}) : mStyle{style} { mText.push_back(0); }

    /* Style setters forwarded to MessageStyle */
    MessageBuilder& event_label_id(uint16_t value) {
        return style(&MessageStyle::event_label_id, value);
    }
    MessageBuilder& speaker(uint8_t value) { return style(&MessageStyle::speaker, value); }
    MessageBuilder& box_kind(MessageBoxKind value) { return style(&MessageStyle::box_kind, value); }
    MessageBuilder& draw_type(MessageDrawType value) {
        return style(&MessageStyle::draw_type, value);
    }
    MessageBuilder& box_position(MessageBoxPosition value) {
        return style(&MessageStyle::box_position, value);
    }
    MessageBuilder& line_alignment(uint8_t value) {
        return style(&MessageStyle::line_alignment, value);
    }
    MessageBuilder& speaker_mood(uint8_t value) {
        return style(&MessageStyle::speaker_mood, value);
    }
    MessageBuilder& camera_attr(uint8_t value) { return style(&MessageStyle::camera_attr, value); }
    MessageBuilder& talk_anim(uint8_t value) { return style(&MessageStyle::talk_anim, value); }
    MessageBuilder& face_anim(uint8_t value) { return style(&MessageStyle::face_anim, value); }
    MessageBuilder& trailing_data(uint16_t value) {
        return style(&MessageStyle::trailing_data, value);
    }

    /* Content builder functions */
    MessageBuilder& text(std::string_view value) {
        if (!mText.empty()) {
            mText.pop_back();
        }
        mText.insert(mText.end(), value.begin(), value.end());
        mText.push_back(0);
        return *this;
    }
    MessageBuilder& raw_tag(uint8_t group, uint16_t type, std::span<const uint8_t> arguments) {
        if (arguments.size() > 250) {
            mResult = MOD_INVALID_ARGUMENT;
            return *this;
        }
        mText.pop_back();
        mText.push_back(0x1a);
        mText.push_back(static_cast<uint8_t>(5 + arguments.size()));
        mText.push_back(group);
        mText.push_back(static_cast<uint8_t>(type >> 8));
        mText.push_back(static_cast<uint8_t>(type));
        mText.insert(mText.end(), arguments.begin(), arguments.end());
        mText.push_back(0);
        return *this;
    }
    MessageBuilder& text_color(MessageTextColor color) {
        const auto index = static_cast<uint8_t>(color);
        return raw_tag(0xFF, 0, {&index, 1});
    }
    /* Full 32-bit text color (Dusklight extension) */
    MessageBuilder& text_color(uint32_t rgba) {
        std::array<uint8_t, 4> arguments{};
        write_bits(arguments.data(), rgba);
        return raw_tag(0xFF, 0, arguments);
    }
    /* Full 32-bit text vertical gradient colors (Dusklight extension) */
    MessageBuilder& text_color(uint32_t upperRgba, uint32_t lowerRgba) {
        std::array<uint8_t, 8> arguments{};
        write_bits(arguments.data(), upperRgba);
        write_bits(arguments.data() + sizeof(upperRgba), lowerRgba);
        return raw_tag(0xFF, 0, arguments);
    }
    MessageBuilder& text_scale(uint16_t percent) { return timed_tag(255, 1, percent); }
    MessageBuilder& character_delay(uint16_t frames) { return timed_tag(0, 6, frames); }
    MessageBuilder& pause(uint16_t frames) { return timed_tag(0, 7, frames); }
    MessageBuilder& auto_advance(uint16_t frames) { return timed_tag(0, 4, frames); }
    MessageBuilder& auto_advance_alternate(uint16_t frames) { return timed_tag(0, 3, frames); }
    MessageBuilder& input_or_timeout(uint16_t frames) { return timed_tag(0, 5, frames); }
    MessageBuilder& input_after_delay(uint16_t frames) { return timed_tag(0, 54, frames); }
    /* Insert the player's name. */
    MessageBuilder& player_name() { return raw_tag(0, 0, {}); }
    /* End a prompt that presents a selection; the options live in the target flow node. */
    MessageBuilder& await_choice() { return raw_tag(0, 32, {}); }
    /* Option list for a vertical selection message. Only two or three options are supported.
     * `initial` configures the highlighted option on open. */
    MessageBuilder& options(std::string_view first, std::string_view second, uint8_t initial = 0) {
        option(8, 0, initial, first);
        return option(8, 1, initial, second);
    }
    MessageBuilder& options(std::string_view first, std::string_view second, std::string_view third,
        uint8_t initial = 0) {
        option(9, 0, initial, first);
        option(9, 1, initial, second);
        return option(9, 2, initial, third);
    }
    MessageVariant build(MessageLanguage language) const {
        return {language, mStyle.data(), mText, mResult};
    }

private:
    MessageBuilder& option(
        uint16_t type, uint8_t position, uint8_t initial, std::string_view value) {
        if (position > 0) {
            text("\n");
        }
        uint8_t marker = static_cast<uint8_t>(position + 1);
        if (position == initial) {
            marker = 1;
        } else if (position == 0) {
            marker = static_cast<uint8_t>(initial + 1);
        }
        raw_tag(0, type, {&marker, 1});
        return text(value);
    }

    template <typename Setter, typename Value>
    MessageBuilder& style(Setter setter, Value value) {
        mStyle = (mStyle.*setter)(value);
        return *this;
    }

    MessageBuilder& timed_tag(uint8_t group, uint16_t type, uint16_t value) {
        return raw_tag(
            group, type, std::array{static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value)});
    }

    MessageStyle mStyle;
    std::vector<uint8_t> mText;
    ModResult mResult = MOD_OK;
};

class MessageOverride {
public:
    MessageOverride() = default;
    MessageOverride(MessageOverrideHandle handle, ModResult result)
        : mHandle{handle}, mResult{result} {}
    MessageOverride(const MessageOverride&) = delete;
    MessageOverride& operator=(const MessageOverride&) = delete;
    MessageOverride(MessageOverride&& other) noexcept { *this = std::move(other); }
    MessageOverride& operator=(MessageOverride&& other) noexcept {
        if (this != &other) {
            reset();
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }
    ~MessageOverride() { reset(); }

    explicit operator bool() const { return mResult == MOD_OK; }
    ModResult result() const { return mResult; }
    MessageOverrideHandle handle() const { return mHandle; }
    void reset() {
        if (mHandle != 0 && svc_message != nullptr) {
            svc_message->remove_override(mod_ctx, mHandle);
            mHandle = 0;
        }
    }

private:
    MessageOverrideHandle mHandle{};
    ModResult mResult = MOD_UNAVAILABLE;
};

inline MessageOverride override_message(
    uint16_t group, uint16_t messageId, MessageLanguage language, std::span<const uint8_t> text) {
    MessageOverrideHandle handle{};
    const ModResult result = svc_message != nullptr ? svc_message->override_message(mod_ctx, group,
                                                          messageId, static_cast<uint8_t>(language),
                                                          text.data(), text.size(), &handle) :
                                                      MOD_UNAVAILABLE;
    return {handle, result};
}

inline MessageOverride override_message_fn(uint16_t group, uint16_t messageId,
    MessageLanguage language, MessageOverrideFn fn, void* userData = nullptr) {
    MessageOverrideHandle handle{};
    const ModResult result = svc_message != nullptr ?
                                 svc_message->override_message_fn(mod_ctx, group, messageId,
                                     static_cast<uint8_t>(language), fn, userData, &handle) :
                                 MOD_UNAVAILABLE;
    return {handle, result};
}

class RegisteredMessage {
public:
    RegisteredMessage() = default;
    RegisteredMessage(MessageId id, MessageHandle handle, ModResult result)
        : mId{id}, mHandle{handle}, mResult{result} {}
    RegisteredMessage(const RegisteredMessage&) = delete;
    RegisteredMessage& operator=(const RegisteredMessage&) = delete;
    RegisteredMessage(RegisteredMessage&& other) noexcept { *this = std::move(other); }
    RegisteredMessage& operator=(RegisteredMessage&& other) noexcept {
        if (this != &other) {
            reset();
            mId = other.mId;
            mHandle = std::exchange(other.mHandle, 0);
            mResult = other.mResult;
        }
        return *this;
    }
    ~RegisteredMessage() { reset(); }
    explicit operator bool() const { return mResult == MOD_OK; }
    MessageId id() const { return mId; }
    ModResult result() const { return mResult; }
    void reset() {
        if (mHandle != 0 && svc_message != nullptr) {
            svc_message->remove_message(mod_ctx, mHandle);
            mHandle = 0;
        }
    }

private:
    MessageId mId{};
    MessageHandle mHandle{};
    ModResult mResult = MOD_UNAVAILABLE;
};

inline RegisteredMessage register_message(
    uint16_t group, std::span<const MessageVariant> variants) {
    std::vector<MessageVariantData> data;
    data.reserve(variants.size());
    for (const auto& variant : variants) {
        if (!variant) {
            return {0, 0, variant.result()};
        }
        data.push_back(variant.data());
    }
    MessageId id{};
    MessageHandle handle{};
    const ModResult result = svc_message != nullptr ? svc_message->register_message(mod_ctx, group,
                                                          data.data(), data.size(), &id, &handle) :
                                                      MOD_UNAVAILABLE;
    return {id, handle, result};
}

inline RegisteredMessage register_message(
    uint16_t group, std::initializer_list<MessageVariant> variants) {
    return register_message(group, std::span{variants.begin(), variants.size()});
}

}  // namespace mods::flow
