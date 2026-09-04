#include "mods/service.hpp"
#include "mods/svc/flow.hpp"
#include "mods/svc/log.hpp"

#include <array>
#include <cstdint>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(FlowService, svc_flow);
IMPORT_SERVICE(MessageService, svc_message);

namespace {

// The Midna flow lives in BMG group 0.
constexpr uint16_t kMessageGroup = 0;
// Midna's "speaker" value for messages.
constexpr uint16_t kMidnaSpeaker = 21;

// Vanilla flow node/branch/edge IDs that we reference or override.
constexpr uint16_t kMidnaPromptHumanNode = 0x018c;
constexpr uint16_t kMidnaPromptWolfNode = 0x018d;
constexpr uint16_t kMidnaHumanBranch = 0x0190;
constexpr uint16_t kMidnaWolfBranch = 0x0193;
constexpr uint16_t kMidnaTalkNode = 0x018f;
constexpr uint16_t kMidnaHumanTalkEdge = 0x0113;
constexpr uint16_t kMidnaWolfTalkEdge = 0x0119;

// For vanilla messages, we need both the entry index and the message ID.
// For registered custom messages, they're the same.
constexpr uint16_t kMidnaMenuPromptEntry = 3003;  // message index used in flow message data
constexpr uint16_t kMidnaMenuPromptId = 2042;     // message ID used to override

constexpr std::array kAllLanguages{
    MESSAGE_LANGUAGE_ENGLISH,
    MESSAGE_LANGUAGE_GERMAN,
    MESSAGE_LANGUAGE_FRENCH,
    MESSAGE_LANGUAGE_SPANISH,
    MESSAGE_LANGUAGE_ITALIAN,
    MESSAGE_LANGUAGE_JAPANESE,
};

constexpr mods::flow::MessageStyle kResponseStyle =
    mods::flow::MessageStyle{}.speaker(kMidnaSpeaker).box_kind(MESSAGE_BOX_MIDNA);
constexpr mods::flow::MessageStyle kPromptStyle =
    kResponseStyle.draw_type(MESSAGE_DRAW_INSTANT).talk_anim(31).face_anim(31);

mods::flow::Query g_demoQuery;
mods::flow::Event g_demoEvent;
mods::flow::Graph g_graph;
std::vector<mods::flow::RegisteredMessage> g_messages;
std::vector<mods::flow::MessageOverride> g_overrides;
std::vector<uint8_t> g_visitedPromptText;
uint32_t g_queryExecutions = 0;
uint32_t g_eventExecutions = 0;

mods::flow::MessageBuilder build_prompt(std::string_view suffix) {
    return mods::flow::MessageBuilder{kPromptStyle}
        .text("What is it, ")
        .text_color(MESSAGE_COLOR_RED)
        .player_name()
        .text_color(MESSAGE_COLOR_DEFAULT)
        .text("?\n")
        .text_scale(85)
        .text(suffix)
        .text_scale(100)
        .await_choice();
}

// Registers a single message for all languages.
mods::flow::RegisteredMessage register_message(const mods::flow::MessageBuilder& builder) {
    std::vector<mods::flow::MessageVariant> variants;
    variants.reserve(kAllLanguages.size());
    for (const MessageLanguage language : kAllLanguages) {
        variants.push_back(builder.build(language));
    }
    return mods::flow::register_message(kMessageGroup, variants);
}

ModResult add_message(const mods::flow::MessageBuilder& builder, MessageId& outId) {
    auto message = register_message(builder);
    if (!message) {
        return message.result();
    }
    outId = message.id();
    g_messages.push_back(std::move(message));
    return MOD_OK;
}

ModResult add_fixed_override(
    uint16_t messageId, const std::vector<uint8_t>& text, bool addCallback) {
    for (const MessageLanguage language : kAllLanguages) {
        auto fixed =
            mods::flow::override_message(kMessageGroup, messageId, language, std::span{text});
        if (!fixed) {
            return fixed.result();
        }
        g_overrides.push_back(std::move(fixed));
        if (!addCallback) {
            continue;
        }
        auto callback = mods::flow::override_message_fn(kMessageGroup, messageId, language,
            [](ModContext*, const MessageOverrideContext*, MessageTextData* outText,
                void*) -> bool {
                if (g_eventExecutions == 0 || outText == nullptr) {
                    return false;
                }
                outText->text = g_visitedPromptText.data();
                outText->text_size = g_visitedPromptText.size();
                return true;
            });
        if (!callback) {
            return callback.result();
        }
        g_overrides.push_back(std::move(callback));
    }
    return MOD_OK;
}

uint16_t demo_query(ModContext*, const FlowQueryContext* query, void*) {
    if (query == nullptr || query->parameter != 1234 || query->result_count < 3) {
        return 0;
    }
    const auto result = static_cast<uint16_t>(g_queryExecutions % 3);
    if (query->phase == FLOW_QUERY_PHASE_EXECUTE) {
        ++g_queryExecutions;
        mods::log::info("Flow demo query selected result {}", result);
    }
    return result;
}

void demo_event(ModContext*, const FlowEventContext* event, void*) {
    ++g_eventExecutions;
    const uint8_t path = event != nullptr ? event->parameters[3] : 0;
    mods::log::info("Flow demo event {} executed (activation {})", path, g_eventExecutions);
}

}  // namespace

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError* outError) {
    g_queryExecutions = 0;
    g_eventExecutions = 0;

    MessageId humanSelectionId = 0;
    MessageId wolfSelectionId = 0;
    MessageId labPromptId = 0;
    MessageId labSelectionId = 0;
    MessageId formattedId = 0;
    MessageId evenId = 0;
    MessageId oddId = 0;
    MessageId timeoutId = 0;

    auto result = add_message(mods::flow::MessageBuilder{}
                                  .speaker(kMidnaSpeaker)
                                  .options("Transform into human", "Warp", "Flow demo"),
        humanSelectionId);
    if (result == MOD_OK) {
        result = add_message(mods::flow::MessageBuilder{}
                                 .speaker(kMidnaSpeaker)
                                 .options("Transform into wolf", "Warp", "Flow demo"),
            wolfSelectionId);
    }
    if (result == MOD_OK) {
        result = add_message(build_prompt("Choose a FlowService test."), labPromptId);
    }
    if (result == MOD_OK) {
        result = add_message(mods::flow::MessageBuilder{}
                                 .speaker(kMidnaSpeaker)
                                 .options("Formatted message", "Callback branch", "Talk to Midna"),
            labSelectionId);
    }
    if (result == MOD_OK) {
        result = add_message(mods::flow::MessageBuilder{kResponseStyle}
                                 .text_color(MESSAGE_COLOR_DEFAULT)
                                 .text("Custom ")
                                 .text_color(0xFF0000FF)
                                 .text("colors")
                                 .text_color(MESSAGE_COLOR_DEFAULT)
                                 .text(" and even ")
                                 .text_color(0x00FF00FF, 0x00FFFFFF)
                                 .text("gradients")
                                 .text_color(MESSAGE_COLOR_DEFAULT)
                                 .text("!\n")
                                 .character_delay(3)
                                 .text("This text is slow. ")
                                 .character_delay(0)
                                 .pause(12)
                                 .text_scale(150)
                                 .text("Big")
                                 .text_scale(100)
                                 .text(" text too.")
                                 .input_after_delay(15),
            formattedId);
    }
    if (result == MOD_OK) {
        result = add_message(mods::flow::MessageBuilder{kResponseStyle}
                                 .text("Query returned ")
                                 .text_color(MESSAGE_COLOR_GREEN)
                                 .text("zero")
                                 .text_color(MESSAGE_COLOR_DEFAULT)
                                 .text(".")
                                 .auto_advance(75),
            evenId);
    }
    if (result == MOD_OK) {
        result = add_message(mods::flow::MessageBuilder{kResponseStyle}
                                 .box_kind(MESSAGE_BOX_LIGHT_SPIRIT)
                                 .text("Query returned ")
                                 .text_color(MESSAGE_COLOR_RED)
                                 .text("one")
                                 .text_color(MESSAGE_COLOR_DEFAULT)
                                 .text(".")
                                 .auto_advance(75),
            oddId);
    }
    if (result == MOD_OK) {
        result = add_message(mods::flow::MessageBuilder{kResponseStyle}
                                 .text("Query returned ")
                                 .text_color(MESSAGE_COLOR_YELLOW)
                                 .text("three")
                                 .text_color(MESSAGE_COLOR_DEFAULT)
                                 .text(".\nThis will timeout after three seconds.")
                                 .input_or_timeout(90),
            timeoutId);
    }
    if (result != MOD_OK) {
        return mods::set_error(outError, result, "failed to register custom demo messages");
    }

    g_visitedPromptText =
        build_prompt("A custom event has run.").build(MESSAGE_LANGUAGE_ENGLISH).text();
    result = add_fixed_override(kMidnaMenuPromptId,
        build_prompt("Flow demo is installed.").build(MESSAGE_LANGUAGE_ENGLISH).text(), true);
    if (result != MOD_OK) {
        return mods::set_error(outError, result, "failed to register demo message overrides");
    }

    g_demoQuery = mods::flow::register_query("flow_demo cycling branch", demo_query);
    g_demoEvent = mods::flow::register_event("flow_demo activation", demo_event);
    if (!g_demoQuery || !g_demoEvent) {
        const auto callbackResult = !g_demoQuery ? g_demoQuery.result() : g_demoEvent.result();
        return mods::set_error(outError, callbackResult, "failed to register flow callbacks");
    }

    // Set up our demo graph
    mods::flow::GraphBuilder graph{kMessageGroup};
    const auto demoSetup = graph.add_event(FLOW_EVENT_SELECT_VERTICAL, {0, 0, 0, 4});
    const auto formattedMessage = graph.add_message(formattedId).next(demoSetup);
    const auto formattedEvent =
        graph.add_event(g_demoEvent.id(), {0, 0, 0, 1}).next(formattedMessage);
    const auto evenMessage = graph.add_message(evenId).next(demoSetup);
    const auto evenEvent = graph.add_event(g_demoEvent.id(), {0, 0, 0, 2}).next(evenMessage);
    const auto oddMessage = graph.add_message(oddId).next(demoSetup);
    const auto oddEvent = graph.add_event(g_demoEvent.id(), {0, 0, 0, 3}).next(oddMessage);
    const auto timeoutMessage = graph.add_message(timeoutId).next(demoSetup);
    const auto timeoutEvent = graph.add_event(g_demoEvent.id(), {0, 0, 0, 4}).next(timeoutMessage);
    const auto callbackBranch =
        graph.add_branch(g_demoQuery.id(), 1234).results({evenEvent, oddEvent, timeoutEvent});
    const auto choiceBranch =
        graph.add_branch(FLOW_QUERY_SELECT_3_CANCEL, 0)
            .results({formattedEvent, callbackBranch, kMidnaTalkNode, mods::flow::kEnd});
    const auto demoSelection = graph.add_message(labSelectionId).next(choiceBranch);
    const auto demoPrompt = graph.add_message(labPromptId).next(demoSelection);
    demoSetup.next(demoPrompt);

    // Patch original nodes and edges so they flow into our custom graph
    const auto humanSelection = graph.add_message(humanSelectionId).next(kMidnaHumanBranch);
    const auto wolfSelection = graph.add_message(wolfSelectionId).next(kMidnaWolfBranch);
    graph.patch_node(
        kMidnaPromptHumanNode, mods::flow::message(0, kMidnaMenuPromptEntry, humanSelection));
    graph.patch_node(
        kMidnaPromptWolfNode, mods::flow::message(0, kMidnaMenuPromptEntry, wolfSelection));
    graph.patch_edge(kMidnaHumanTalkEdge, demoSetup);
    graph.patch_edge(kMidnaWolfTalkEdge, demoSetup);

    g_graph = graph.commit();
    if (!g_graph) {
        return mods::set_error(outError, g_graph.result(), "failed to commit the flow demo graph");
    }

    mods::log::info("Flow demo ready: {} custom messages", g_messages.size());
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    mods::log::info("Flow demo unloaded after {} events", g_eventExecutions);
    g_graph.reset();
    g_overrides.clear();
    g_messages.clear();
    g_visitedPromptText.clear();
    return MOD_OK;
}
}
