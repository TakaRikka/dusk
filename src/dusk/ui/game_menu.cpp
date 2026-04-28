#include "game_menu.hpp"

#include "element.hpp"
#include "game_option.hpp"
#include "label.hpp"
#include "theme.hpp"
#include "ui.hpp"
#include "window.hpp"

#include "dusk/config.hpp"
#include "dusk/imgui/ImGuiEngine.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"
#include "m_Do/m_Do_graphic.h"

#include <RmlUi/Core.h>
#include <RmlUi/Core/ElementDocument.h>
#include <aurora/aurora.h>
#include <aurora/gfx.h>
#include <aurora/rmlui.hpp>
#include <dolphin/vi.h>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace aurora::gx {
extern bool enableLodBias;
}

namespace dusk::ui::game_menu {
namespace {

enum class Tab : int {
    Audio = 0,
    Cheats,
    Gameplay,
    Graphics,
    Input,
    Interface,
    Count,
};

struct TabDef {
    const char* id;
    const char* label;
};

constexpr std::array<TabDef, static_cast<size_t>(Tab::Count)> kTabs{{
    {"audio", "Audio"},
    {"cheats", "Cheats"},
    {"gameplay", "Gameplay"},
    {"graphics", "Graphics"},
    {"input", "Input"},
    {"interface", "Interface"},
}};

constexpr int kInternalResolutionScaleMax = 12;
constexpr int kShadowResolutionMax = 8;

constexpr std::array<float, 5> kBloomMultiplierStops{0.0f, 0.25f, 0.50f, 0.75f, 1.00f};
constexpr std::array<const char*, 3> kBloomModeNames{"Off", "Classic", "Dusk"};

// TODO: Needs more spacing for newlines
static const char* get_description_for_item(std::string_view id) {
    if (id == "internal-resolution") {
        return "Auto renders at the native window resolution.\nHigher values scale the internal "
               "framebuffer.";
    }
    if (id == "shadow-resolution") {
        return "Improves the shadow resolution, making them higher quality.";
    }
    if (id == "frame-interp") {
        return "Uses inter-frame interpolation to enable higher frame rates.\nVisual artifacts, "
               "animation glitches, or instability may occur.";
    }

    return "No description found.";
}

struct Row {
    std::string id;
    std::function<void()> activate;
    std::function<void(int direction)> cycle;
};

class Screen : public Rml::EventListener {
public:
    bool initialize() {
        if (m_initialized) {
            return true;
        }
        if (!ui::initialize()) {
            return false;
        }
        m_initialized = true;
        return true;
    }

    void shutdown() {
        close_document();
        if (ui::is_active()) {
            ui::set_active(false);
        }
        m_initialized = false;
        m_focusIds.clear();
        m_rows.clear();
    }

    bool is_open() const { return m_open; }

    void set_open(bool open) {
        if (open == m_open) {
            return;
        }
        if (open && !initialize()) {
            return;
        }
        m_open = open;
        if (open) {
            ui::set_active(true);
            rebuild();
        } else {
            close_document();
            ui::set_active(false);
        }
    }

    void toggle() { set_open(!m_open); }

    void handle_event(const SDL_Event& event) {
        if (!m_open) {
            return;
        }
        ui::handle_event(event);
    }

    void update() {
        if (!m_open) {
            return;
        }

        if (m_requestClose) {
            m_requestClose = false;
            set_open(false);
            return;
        }

        if (!m_pendingTabId.empty()) {
            const std::string tabId = std::move(m_pendingTabId);
            m_pendingTabId.clear();
            apply_tab_selection(tabId);
            if (!m_open) {
                return;
            }
        }

        if (!m_requestedActivation.empty()) {
            const std::string id = std::move(m_requestedActivation);
            m_requestedActivation.clear();
            invoke_activate(id);
            if (!m_open) {
                return;
            }
        }

        if (m_requestedCycleDirection != 0) {
            const std::string id = std::move(m_requestedCycleId);
            const int direction = m_requestedCycleDirection;
            m_requestedCycleId.clear();
            m_requestedCycleDirection = 0;
            invoke_cycle(id, direction);
            if (!m_open) {
                return;
            }
        }

        if (m_needsRebuild) {
            m_needsRebuild = false;
            rebuild();
        }

        ui::update();
        sync_description_pane();
    }

    void ProcessEvent(Rml::Event& event) override {
        if (event.GetId() != Rml::EventId::Keydown) {
            return;
        }
        const auto key = static_cast<Rml::Input::KeyIdentifier>(
            event.GetParameter<int>("key_identifier", Rml::Input::KI_UNKNOWN));
        if (handle_key(key)) {
            event.StopImmediatePropagation();
        }
    }

private:
    bool m_initialized = false;
    bool m_open = false;
    Tab m_tab = Tab::Graphics;
    Rml::ElementDocument* m_document = nullptr;
    std::unique_ptr<Window> m_window;
    std::vector<std::unique_ptr<GameOption> > m_options;
    std::vector<Row> m_rows;
    std::vector<std::string> m_focusIds;
    std::string m_pendingFocusId;
    std::string m_requestedActivation;
    std::string m_requestedCycleId;
    int m_requestedCycleDirection = 0;
    bool m_requestClose = false;
    bool m_needsRebuild = false;
    std::string m_pendingTabId;
    Rml::Element* m_descriptionElement = nullptr;
    Rml::Element* m_lastDescriptionSyncFocus = nullptr;

    Row* find_row(std::string_view id) {
        for (auto& row : m_rows) {
            if (row.id == id) {
                return &row;
            }
        }
        return nullptr;
    }

    void invoke_activate(const std::string& id) {
        if (Row* row = find_row(id); row && row->activate) {
            row->activate();
        }
    }

    void invoke_cycle(const std::string& id, int direction) {
        if (Row* row = find_row(id); row && row->cycle) {
            row->cycle(direction);
        }
    }

    void close_document() {
        if (m_document == nullptr) {
            return;
        }
        m_document->RemoveEventListener(Rml::EventId::Keydown, this);
        m_options.clear();
        m_rows.clear();
        m_focusIds.clear();
        m_descriptionElement = nullptr;
        m_lastDescriptionSyncFocus = nullptr;
        m_window.reset();
        m_document->Close();
        m_document = nullptr;
    }

    void style_document(Rml::ElementDocument* document) {
        using namespace theme;
        set_props(document, {
                                {Rml::PropertyId::Width, rml_percent(100.0f)},
                                {Rml::PropertyId::Height, rml_percent(100.0f)},
                                {Rml::PropertyId::MarginTop, rml_px(0.0f)},
                                {Rml::PropertyId::MarginRight, rml_px(0.0f)},
                                {Rml::PropertyId::MarginBottom, rml_px(0.0f)},
                                {Rml::PropertyId::MarginLeft, rml_px(0.0f)},
                                {Rml::PropertyId::PaddingTop, rml_px(0.0f)},
                                {Rml::PropertyId::PaddingRight, rml_px(0.0f)},
                                {Rml::PropertyId::PaddingBottom, rml_px(0.0f)},
                                {Rml::PropertyId::PaddingLeft, rml_px(0.0f)},
                                {Rml::PropertyId::FontFamily, rml_string("Inter")},
                                {Rml::PropertyId::Color, rml_color(Text)},
                            });
    }

    Rml::Element* add_screen() {
        using namespace theme;
        return append(m_document, "div", "game-menu-screen",
            {
                {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                {Rml::PropertyId::Position, Rml::Style::Position::Absolute},
                {Rml::PropertyId::Left, rml_px(0.0f)},
                {Rml::PropertyId::Top, rml_px(0.0f)},
                {Rml::PropertyId::Right, rml_px(0.0f)},
                {Rml::PropertyId::Bottom, rml_px(0.0f)},
                {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
                {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
                {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
                {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
                {Rml::PropertyId::PaddingTop, rml_dp(32.0f)},
                {Rml::PropertyId::PaddingRight, rml_dp(32.0f)},
                {Rml::PropertyId::PaddingBottom, rml_dp(32.0f)},
                {Rml::PropertyId::PaddingLeft, rml_dp(32.0f)},
            });
    }

    Rml::Element* add_section_header(Rml::Element* parent, std::string_view title) {
        auto* row = append(parent, "div", {},
            {
                {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Row},
                {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
                {Rml::PropertyId::Width, rml_percent(100.0f)},
                {Rml::PropertyId::PaddingTop, rml_dp(8.0f)},
                {Rml::PropertyId::PaddingBottom, rml_dp(4.0f)},
            });
        auto* label = add_label(row, title, LabelStyle::Annotation);
        set_props(label, {
                             {Rml::PropertyId::FontSize, rml_dp(14.0f)},
                             {Rml::PropertyId::LetterSpacing, rml_dp(3.0f)},
                             {Rml::PropertyId::Color, rml_color(theme::WindowAccentSoft)},
                             {Rml::PropertyId::FlexShrink, rml_number(0.0f)},
                         });
        return row;
    }

    Rml::Element* add_scroll_body(Rml::Element* parent) {
        return append(parent, "div", {},
            {
                {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
                {Rml::PropertyId::Width, rml_percent(100.0f)},
                {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
                {Rml::PropertyId::MinHeight, rml_px(0.0f)},
                {Rml::PropertyId::RowGap, rml_dp(8.0f)},
                {Rml::PropertyId::ColumnGap, rml_dp(8.0f)},
                {Rml::PropertyId::OverflowY, Rml::Style::Overflow::Auto},
            });
    }

    std::function<void()> queue_activate(std::string id) {
        return [this, id = std::move(id)] { m_requestedActivation = id; };
    }

    void register_row(Row row, std::unique_ptr<GameOption> option) {
        m_focusIds.push_back(row.id);
        m_rows.push_back(std::move(row));
        m_options.push_back(std::move(option));
    }

    void add_toggle(Rml::Element* parent, std::string id, std::string_view title,
        config::ConfigVar<bool>& var, std::function<void(bool)> sideEffect = {},
        std::string_view detail = {}) {
        auto mutate = [this, &var, sideEffect = std::move(sideEffect)] {
            const bool next = !var.getValue();
            var.setValue(next);
            Save();
            if (sideEffect) {
                sideEffect(next);
            }
            m_needsRebuild = true;
        };
        const std::string_view valueText = var.getValue() ? "On" : "Off";
        auto option =
            std::make_unique<GameOption>(parent, id, title, valueText, detail, queue_activate(id));
        register_row(Row{id, mutate, [mutate](int) { mutate(); }}, std::move(option));
    }

    void add_action(Rml::Element* parent, std::string id, std::string_view title,
        std::function<void()> action, std::string_view valueText = ">",
        std::string_view detail = {}) {
        auto mutate = [this, action = std::move(action)] {
            action();
            m_needsRebuild = true;
        };
        auto option =
            std::make_unique<GameOption>(parent, id, title, valueText, detail, queue_activate(id));
        register_row(Row{id, mutate, {}}, std::move(option));
    }

    template <typename T>
    void add_cycle_row(Rml::Element* parent, std::string id, std::string_view title,
        std::string_view valueText, std::string_view detail, std::function<void(int)> cycle) {
        auto mutate = [this, cycle] {
            cycle(1);
            m_needsRebuild = true;
        };
        auto cycleWithRebuild = [this, cycle](int direction) {
            cycle(direction);
            m_needsRebuild = true;
        };
        auto option =
            std::make_unique<GameOption>(parent, id, title, valueText, detail, queue_activate(id));
        register_row(Row{id, mutate, cycleWithRebuild}, std::move(option));
    }

    void add_int_cycle(Rml::Element* parent, std::string id, std::string_view title,
        config::ConfigVar<int>& var, int minValue, int maxValue,
        std::function<std::string(int)> formatter, std::function<void(int)> sideEffect = {}) {
        const int current = std::clamp(var.getValue(), minValue, maxValue);
        const std::string valueText = formatter(current);
        auto cycle = [&var, minValue, maxValue, sideEffect = std::move(sideEffect)](int dir) {
            int next = std::clamp(var.getValue(), minValue, maxValue) + dir;
            if (next < minValue) {
                next = maxValue;
            } else if (next > maxValue) {
                next = minValue;
            }
            var.setValue(next);
            Save();
            if (sideEffect) {
                sideEffect(next);
            }
        };
        add_cycle_row<int>(parent, std::move(id), title, valueText, {}, std::move(cycle));
    }

    void add_bloom_mode_row(Rml::Element* parent) {
        auto& var = getSettings().game.bloomMode;
        const int current = std::clamp(
            static_cast<int>(var.getValue()), 0, static_cast<int>(kBloomModeNames.size() - 1));
        const std::string_view valueText = kBloomModeNames[static_cast<size_t>(current)];
        auto cycle = [&var](int dir) {
            const int count = kBloomModeNames.size();
            int next = static_cast<int>(var.getValue()) + dir;
            next = (next % count + count) % count;
            var.setValue(static_cast<BloomMode>(next));
            Save();
        };
        add_cycle_row<int>(parent, "bloom-mode", "Bloom", valueText, {}, std::move(cycle));
    }

    void add_bloom_brightness_row(Rml::Element* parent) {
        auto& var = getSettings().game.bloomMultiplier;
        const std::string valueText = fmt::format("{:.2f}", var.getValue());
        auto cycle = [&var](int dir) {
            const float currentValue = var.getValue();
            int closest = 0;
            float bestDelta = std::abs(currentValue - kBloomMultiplierStops[0]);
            for (int i = 1; i < static_cast<int>(kBloomMultiplierStops.size()); ++i) {
                const float delta = std::abs(currentValue - kBloomMultiplierStops[i]);
                if (delta < bestDelta) {
                    bestDelta = delta;
                    closest = i;
                }
            }
            const int count = kBloomMultiplierStops.size();
            const int next = (closest + dir + count) % count;
            var.setValue(kBloomMultiplierStops[next]);
            Save();
        };
        const std::string_view detail =
            getSettings().game.bloomMode.getValue() == BloomMode::Off ? "Bloom is disabled" : "";
        add_cycle_row<int>(
            parent, "bloom-brightness", "Bloom Brightness", valueText, detail, std::move(cycle));
    }

    void build_description_pane() {
        m_descriptionElement = nullptr;
        m_lastDescriptionSyncFocus = nullptr;
        Rml::Element* right = m_window->right_pane();
        if (right == nullptr) {
            return;
        }
        m_descriptionElement = append_text(right, "p", " ", "option-description");
        set_props(m_descriptionElement,
            {
                {Rml::PropertyId::Color, rml_color(theme::TextActive)},
                {Rml::PropertyId::FontSize, rml_dp(20.0f)},
                {Rml::PropertyId::LineHeight, Rml::Property(1.45f, Rml::Unit::EM)},
                {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Left},
                {Rml::PropertyId::AlignSelf, Rml::Style::AlignSelf::Stretch},
                {Rml::PropertyId::Width, rml_percent(100.0f)},
                {Rml::PropertyId::BoxSizing, Rml::Style::BoxSizing::BorderBox},
                {Rml::PropertyId::WhiteSpace, Rml::Style::WhiteSpace::Preline},
            });
    }

    void sync_description_pane() {
        if (m_descriptionElement == nullptr) {
            return;
        }
        if (m_tab != Tab::Graphics) {
            return;
        }
        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return;
        }
        Rml::Element* const focused = context->GetFocusElement();
        if (focused == m_lastDescriptionSyncFocus) {
            return;
        }
        m_lastDescriptionSyncFocus = focused;
        if (focused == nullptr) {
            set_text(m_descriptionElement, get_description_for_item({}));
        } else {
            set_text(m_descriptionElement, get_description_for_item(focused->GetId()));
        }
    }

    void build_graphics_tab(Rml::Element* body) {
        auto* scroll = add_scroll_body(body);

        add_section_header(scroll, "Display");

        // TODO: Replace this with a Display Mode toggle.
        add_toggle(scroll, "fullscreen", "Toggle Fullscreen", getSettings().video.enableFullscreen,
            [](bool enabled) { VISetWindowFullscreen(enabled); });

        u32 internalWidth = 0;
        u32 internalHeight = 0;
        AuroraGetRenderSize(&internalWidth, &internalHeight);
        const std::string detail = fmt::format("Current: {}x{}", internalWidth, internalHeight);

        const int currentScale = std::clamp(
            getSettings().game.internalResolutionScale.getValue(), 0, kInternalResolutionScaleMax);
        const std::string scaleValue =
            currentScale == 0 ? std::string("Auto") : fmt::format("{}x", currentScale);

        auto scaleCycle = [](int dir) {
            int next = std::clamp(getSettings().game.internalResolutionScale.getValue(), 0,
                           kInternalResolutionScaleMax) +
                       dir;
            if (next < 0) {
                next = kInternalResolutionScaleMax;
            } else if (next > kInternalResolutionScaleMax) {
                next = 0;
            }
            getSettings().game.internalResolutionScale.setValue(next);
            VISetFrameBufferScale(static_cast<float>(next));
            Save();
        };

        add_cycle_row<int>(scroll, "internal-resolution", "Internal Resolution", scaleValue, detail,
            std::move(scaleCycle));

        add_int_cycle(scroll, "shadow-resolution", "Shadow Resolution",
            getSettings().game.shadowResolutionMultiplier, 1, kShadowResolutionMax,
            [](int v) { return fmt::format("x{}", v); });

        add_toggle(scroll, "lock-aspect", "Force 4:3 Aspect Ratio",
            getSettings().video.lockAspectRatio, [](bool enabled) {
                AuroraSetViewportPolicy(enabled ? AURORA_VIEWPORT_FIT : AURORA_VIEWPORT_STRETCH);
            });

        add_toggle(scroll, "vsync", "VSync", getSettings().video.enableVsync,
            [](bool enabled) { aurora_enable_vsync(enabled); });

        add_toggle(scroll, "frame-interp", "Unlock Framerate",
            getSettings().game.enableFrameInterpolation, {}, "Experimental");

        add_section_header(scroll, "Post-Processing");

        add_bloom_mode_row(scroll);
        if (getSettings().game.bloomMode.getValue() != BloomMode::Off) {
            add_bloom_brightness_row(scroll);
        }

        add_toggle(
            scroll, "depth-of-field", "Depth of Field", getSettings().game.enableDepthOfField);

        add_section_header(scroll, "Developer Options");

        const std::string lodValue = aurora::gx::enableLodBias ? "On" : "Off";
        auto lodMutate = [this] {
            aurora::gx::enableLodBias = !aurora::gx::enableLodBias;
            m_needsRebuild = true;
        };
        auto lodOption = std::make_unique<GameOption>(scroll, "lod-bias", "LOD Bias", lodValue,
            std::string_view{}, queue_activate("lod-bias"));
        register_row(
            Row{"lod-bias", lodMutate, [lodMutate](int) { lodMutate(); }}, std::move(lodOption));

        add_toggle(
            scroll, "minimap-shadows", "Mini-Map Shadows", getSettings().game.enableMapBackground);
    }

    void build_placeholder_tab(Rml::Element* body, std::string_view tabLabel) {
        auto* wrap = append(body, "div", {},
            {
                {Rml::PropertyId::Display, Rml::Style::Display::Flex},
                {Rml::PropertyId::FlexDirection, Rml::Style::FlexDirection::Column},
                {Rml::PropertyId::AlignItems, Rml::Style::AlignItems::Center},
                {Rml::PropertyId::JustifyContent, Rml::Style::JustifyContent::Center},
                {Rml::PropertyId::Width, rml_percent(100.0f)},
                {Rml::PropertyId::FlexGrow, rml_number(1.0f)},
                {Rml::PropertyId::RowGap, rml_dp(12.0f)},
                {Rml::PropertyId::ColumnGap, rml_dp(12.0f)},
            });
        auto* heading = add_label(wrap, tabLabel, LabelStyle::Large);
        set_props(heading, {{Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Center}});
        auto* sub = add_label(wrap, "Not yet ported.", LabelStyle::Body);
        set_props(sub, {
                           {Rml::PropertyId::TextAlign, Rml::Style::TextAlign::Center},
                           {Rml::PropertyId::Color, rml_color(theme::TextDim)},
                       });
    }

    void build_body() {
        m_window->set_right_pane_visible(m_tab == Tab::Graphics);
        Rml::Element* body = m_window->body();
        switch (m_tab) {
        case Tab::Graphics:
            build_graphics_tab(body);
            build_description_pane();
            break;
        default:
            build_placeholder_tab(body, kTabs[static_cast<size_t>(m_tab)].label);
            break;
        }
    }

    void rebuild() {
        if (!m_open) {
            return;
        }

        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return;
        }

        const std::string preferredFocus =
            m_pendingFocusId.empty() ? current_focus_id() : m_pendingFocusId;
        m_pendingFocusId.clear();

        close_document();

        m_document = context->CreateDocument();
        if (m_document == nullptr) {
            return;
        }

        style_document(m_document);
        Rml::Element* screen = add_screen();

        m_window = std::make_unique<Window>(screen, "game-menu", [this] { request_close(); });

        for (const TabDef& tab : kTabs) {
            const std::string tabId = tab.id;
            m_window->add_tab(tabId, tab.label, [this, tabId] { m_pendingTabId = tabId; });
        }
        m_window->set_selected_tab(kTabs[static_cast<size_t>(m_tab)].id);

        build_body();

        m_document->AddEventListener(Rml::EventId::Keydown, this);
        m_document->Show();

        focus_id(preferredFocus.empty() ? first_focus_id() : preferredFocus);
        sync_description_pane();
    }

    void request_close() { m_requestClose = true; }

    void switch_tab(int direction) {
        const int count = kTabs.size();
        const int next = (static_cast<int>(m_tab) + direction + count) % count;
        m_pendingTabId = kTabs[static_cast<size_t>(next)].id;
    }

    void apply_tab_selection(std::string_view tabId) {
        for (size_t i = 0; i < kTabs.size(); ++i) {
            if (tabId == kTabs[i].id) {
                if (m_tab == static_cast<Tab>(i)) {
                    return;
                }
                m_tab = static_cast<Tab>(i);
                m_pendingFocusId.clear();
                rebuild();
                return;
            }
        }
    }

    std::string current_focus_id() const {
        if (Rml::Context* context = aurora::rmlui::get_context()) {
            if (Rml::Element* focused = context->GetFocusElement()) {
                return focused->GetId();
            }
        }
        return {};
    }

    std::string first_focus_id() const {
        return m_focusIds.empty() ? std::string{} : m_focusIds.front();
    }

    int focus_index() const {
        const std::string id = current_focus_id();
        if (id.empty()) {
            return -1;
        }
        for (int i = 0; i < static_cast<int>(m_focusIds.size()); ++i) {
            if (m_focusIds[i] == id) {
                return i;
            }
        }
        return -1;
    }

    void focus_id(std::string_view id) {
        if (m_document == nullptr) {
            return;
        }
        if (!id.empty()) {
            if (Rml::Element* element = m_document->GetElementById(std::string(id))) {
                element->Focus(true);
                return;
            }
        }
        const std::string fallback = first_focus_id();
        if (!fallback.empty()) {
            if (Rml::Element* element = m_document->GetElementById(fallback)) {
                element->Focus(true);
            }
        }
    }

    void move_focus(int direction) {
        if (m_focusIds.empty()) {
            return;
        }
        const int index = focus_index();
        if (index < 0) {
            focus_id(m_focusIds.front());
            return;
        }
        const int next = index + direction;
        if (next < 0 || next >= static_cast<int>(m_focusIds.size())) {
            return;
        }
        focus_id(m_focusIds[static_cast<size_t>(next)]);
    }

    void queue_activate_focused() {
        const std::string id = current_focus_id();
        if (!id.empty()) {
            m_requestedActivation = id;
        }
    }

    void queue_cycle_focused(int direction) {
        const std::string id = current_focus_id();
        if (!id.empty()) {
            m_requestedCycleId = id;
            m_requestedCycleDirection = direction;
        }
    }

    bool handle_key(Rml::Input::KeyIdentifier key) {
        switch (key) {
        case Rml::Input::KI_UP:
            move_focus(-1);
            return true;
        case Rml::Input::KI_DOWN:
            move_focus(1);
            return true;
        case Rml::Input::KI_LEFT:
            queue_cycle_focused(-1);
            return true;
        case Rml::Input::KI_RIGHT:
            queue_cycle_focused(1);
            return true;
        case Rml::Input::KI_F16:
            switch_tab(-1);
            return true;
        case Rml::Input::KI_F17:
            switch_tab(1);
            return true;
        case Rml::Input::KI_RETURN:
            queue_activate_focused();
            return true;
        case Rml::Input::KI_ESCAPE:
        case Rml::Input::KI_F15:
            request_close();
            return true;
        default:
            return false;
        }
    }
};

Screen s_screen;

}  // namespace

bool initialize() {
    return s_screen.initialize();
}

void shutdown() {
    s_screen.shutdown();
}

bool is_active() {
    return s_screen.is_open();
}

void toggle() {
    s_screen.toggle();
}

void set_active(bool active) {
    s_screen.set_open(active);
}

void handle_event(const SDL_Event& event) {
    s_screen.handle_event(event);
}

void update() {
    s_screen.update();
}

}  // namespace dusk::ui::game_menu
