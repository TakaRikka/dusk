#include "graphics_tuner.hpp"

#include "button.hpp"

#include "dusk/config.hpp"
#include "dusk/logging.h"
#include "dusk/settings.h"
#include "m_Do/m_Do_audio.h"

#include <dolphin/gx/GXAurora.h>
#include <fmt/format.h>

#include <algorithm>
#include <string>
#include <type_traits>

namespace dusk::ui {
namespace {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/tuner.rcss" />
</head>
<body>
    <div id="root" class="tuner-root">
        <div class="tuner">
            <div class="header">
                <div id="title"></div>
                <div id="carousel-container" class="carousel-container"></div>
            </div>
            <div id="description" class="description"></div>
            <div class="divider"></div>
            <div id="footer" class="footer"></div>
        </div>
    </div>
</body>
</rml>
)RML";

Rml::String format_internal_resolution(int value) {
    u32 width = 0;
    u32 height = 0;
    AuroraGetRenderSize(&width, &height);
    if (value <= 0) {
        return fmt::format("Auto ({}×{})", width, height);
    }
    return fmt::format("{}× ({}×{})", value, width, height);
}

Rml::String format_resampler(int value) {
    switch (static_cast<Resampler>(value)) {
    case Resampler::Bilinear:
        return "Bilinear";
    case Resampler::Area:
        return "Area";
    default:
        return "";
    }
}

Rml::String format_post_process_mode(int value) {
    switch (static_cast<BloomMode>(value)) {
    case BloomMode::Off:
        return "Off";
    case BloomMode::Classic:
        return "Classic";
    case BloomMode::Dusk:
        return "Dusklight";
    default:
        return "";
    }
}

Rml::String format_times(int value) { return fmt::format("{}×", value); }

Rml::String format_percent(int value) { return fmt::format("{}%", value); }

Rml::String format_bool(int value) { return value ? "On" : "Off"; }

template <typename T>
int read_cvar(const ConfigVar<T>& var) {
    if constexpr (std::is_same_v<T, float>) {
        return static_cast<int>(var.getValue() * 100.0f + 0.5f);
    } else {
        return static_cast<int>(var.getValue());
    }
}

template <typename T>
void write_cvar(ConfigVar<T>& var, int value) {
    if constexpr (std::is_same_v<T, float>) {
        var.setValue(static_cast<float>(value) / 100.0f);
    } else if constexpr (std::is_same_v<T, bool>) {
        var.setValue(static_cast<bool>(value));
    } else {
        var.setValue(static_cast<T>(value));
    }
}

template <auto Var, typename Min, typename Max, typename Def>
const GraphicsSetting& bind(Min min, Max max, Def def, int step, Rml::String (*label)(int),
    bool watchSize = false) {
    static const GraphicsSetting desc{
        .min = static_cast<int>(min),
        .max = static_cast<int>(max),
        .defaultValue = static_cast<int>(def),
        .step = step,
        .watchesRenderSize = watchSize,
        .read = []() -> int { return read_cvar(Var()); },
        .write = [](int value) { write_cvar(Var(), value); },
        .label = label,
        .cvarName = []() -> const char* { return Var().getName(); },
        .isModified = []() -> bool { return Var().getValue() != Var().getDefaultValue(); },
    };
    return desc;
}

Rml::Element* create_stepped_carousel_root(Rml::Element* parent) {
    auto* doc = parent->GetOwnerDocument();
    auto root = doc->CreateElement("div");
    root->SetClass("stepped-carousel", true);
    root->SetAttribute("tabindex", "0");
    return parent->AppendChild(std::move(root));
}

Rml::Element* create_stepped_carousel_arrow(
    Rml::Element* parent, const Rml::String& className, const Rml::String& label) {
    auto* doc = parent->GetOwnerDocument();
    auto button = doc->CreateElement("button");
    button->SetClass("stepped-carousel-arrow", true);
    button->SetClass(className, true);
    button->SetInnerRML(label);
    return parent->AppendChild(std::move(button));
}

void update_carousel_arrow_color(Rml::Element* arrow, bool dim) {
    const Rml::Colourb& color = Rml::Colourb(255, 255, 255, dim ? 128 : 255);
    arrow->SetProperty(Rml::PropertyId::Color, Rml::Property(color, Rml::Unit::COLOUR));
}

}  // namespace

const GraphicsSetting& GraphicsSetting::of(GraphicsOption option) {
    switch (option) {
    case GraphicsOption::InternalResolution:
        return bind<[]() -> auto& { return getSettings().game.internalResolutionScale; }>(
            0, 12, 0, 1, format_internal_resolution, true);
    case GraphicsOption::ShadowResolution:
        return bind<[]() -> auto& { return getSettings().game.shadowResolutionMultiplier; }>(
            1, 8, 1, 1, format_times);
    case GraphicsOption::Resampler:
        return bind<[]() -> auto& { return getSettings().game.resampler; }>(
            Resampler::Bilinear, Resampler::Area, Resampler::Bilinear, 1, format_resampler);
    case GraphicsOption::BloomMode:
        return bind<[]() -> auto& { return getSettings().game.bloomMode; }>(
            BloomMode::Off, BloomMode::Dusk, BloomMode::Classic, 1, format_post_process_mode);
    case GraphicsOption::BloomMultiplier:
        return bind<[]() -> auto& { return getSettings().game.bloomMultiplier; }>(
            0, 100, 100, 10, format_percent);
    case GraphicsOption::DepthOfFieldMode:
        return bind<[]() -> auto& { return getSettings().game.depthOfFieldMode; }>(
            DepthOfFieldMode::Off, DepthOfFieldMode::Dusk, DepthOfFieldMode::Classic, 1,
            format_post_process_mode);
    case GraphicsOption::TextureReplacements:
        return bind<[]() -> auto& { return getSettings().game.enableTextureReplacements; }>(
            0, 1, 0, 1, format_bool);
    }
    DuskLog.error("{} is an invalid GraphicsOption", static_cast<int>(option));
    abort();
}

SteppedCarousel::SteppedCarousel(Rml::Element* parent, Props props)
    : Component(create_stepped_carousel_root(parent)), mProps(std::move(props)) {
    mPrevElem = create_stepped_carousel_arrow(mRoot, "prev", "&#xe5cb;");
    mValueElem = append(mRoot, "div");
    mValueElem->SetClass("stepped-carousel-value", true);
    mNextElem = create_stepped_carousel_arrow(mRoot, "next", "&#xe5cc;");

    listen(mPrevElem, Rml::EventId::Click,
        [this](Rml::Event&) { handle_nav_command(NavCommand::Left); });
    listen(mNextElem, Rml::EventId::Click,
        [this](Rml::Event&) { handle_nav_command(NavCommand::Right); });
    listen(mRoot, Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);
        if (cmd != NavCommand::None && handle_nav_command(cmd)) {
            event.StopPropagation();
        }
    });
}

bool SteppedCarousel::focus() {
    return Component::focus();
}

void SteppedCarousel::update() {}

void SteppedCarousel::refresh() {
    if (mValueElem == nullptr) {
        return;
    }
    const int value = std::clamp(mProps.getValue ? mProps.getValue() : 0, mProps.min, mProps.max);
    if (mProps.formatValue) {
        mValueElem->SetInnerRML(mProps.formatValue(value));
    } else {
        mValueElem->SetInnerRML(std::to_string(value));
    }

    update_carousel_arrow_color(mPrevElem, value == mProps.min);
    update_carousel_arrow_color(mNextElem, value == mProps.max);
}

bool SteppedCarousel::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Left) {
        const int value = mProps.getValue ? mProps.getValue() : 0;
        apply(std::clamp(value - mProps.step, mProps.min, mProps.max));
        return true;
    }
    if (cmd == NavCommand::Right) {
        const int value = mProps.getValue ? mProps.getValue() : 0;
        apply(std::clamp(value + mProps.step, mProps.min, mProps.max));
        return true;
    }
    return false;
}

void SteppedCarousel::apply(int value) {
    const int nextValue = std::clamp(value, mProps.min, mProps.max);
    const int currentValue =
        std::clamp(mProps.getValue ? mProps.getValue() : 0, mProps.min, mProps.max);
    if (nextValue == currentValue) {
        return;
    }
    mDoAud_seStartMenu(kSoundItemChange);
    if (mProps.onChange) {
        mProps.onChange(nextValue);
    }
}

GraphicsTuner::GraphicsTuner(GraphicsTunerProps props)
    : Document(kDocumentSource, false, DocumentScope::GraphicsTuner),
      mSetting(GraphicsSetting::of(props.option)) {
    if (mDocument == nullptr) {
        return;
    }

    if (auto* title = mDocument->GetElementById("title")) {
        title->SetInnerRML(escape(props.title));
    }
    if (auto* description = mDocument->GetElementById("description")) {
        description->SetInnerRML(escape(props.helpText));
    }
    if (auto* carouselParent = mDocument->GetElementById("carousel-container")) {
        mCarousel = &add_component<SteppedCarousel>(carouselParent,
            SteppedCarousel::Props{
                .min = mSetting.min,
                .max = mSetting.max,
                .step = mSetting.step,
                .getValue = [this] { return mSetting.read(); },
                .onChange = [this](int value) { mSetting.set(value); },
                .formatValue = [this](int value) { return mSetting.label(value); },
            });
    }

    if (auto* footer = mDocument->GetElementById("footer")) {
        auto& returnButton = add_component<Button>(footer, "\xE2\x86\x90 Return", "footer-button")
                                 .on_pressed([this] { pop(); });
        returnButton.root()->SetClass("return", true);
        auto& resetButton =
            add_component<Button>(footer, "Reset to default", "footer-button").on_pressed([this] {
                mDoAud_seStartMenu(kSoundItemChange);
                reset_default();
            });
        resetButton.root()->SetClass("reset", true);
    }

    if (mCarousel != nullptr) {
        if (const char* name = mSetting.cvarName()) {
            mSubscription = config::subscribe(name,
                [this](config::ConfigVarBase&, const void*) { mCarousel->refresh(); });
        }
        mCarousel->refresh();
        if (mSetting.watchesRenderSize) {
            AuroraGetRenderSize(&mLastRenderWidth, &mLastRenderHeight);
        }
    }

    // Hide document after transition completion
    mRoot = mDocument->GetElementById("root");
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
        }
    });
}

GraphicsTuner::~GraphicsTuner() {
    if (mSubscription != 0) {
        config::unsubscribe(mSubscription);
    }
}

void GraphicsTuner::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
    mDoAud_seStartMenu(kSoundWindowOpen);
}

void GraphicsTuner::hide(bool close) {
    config::save();
    mRoot->RemoveAttribute("open");
    if (close) {
        mPendingClose = true;
        mDoAud_seStartMenu(kSoundWindowClose);
    }
}

void GraphicsTuner::update() {
    if (mSetting.watchesRenderSize && mCarousel != nullptr) {
        u32 width = 0;
        u32 height = 0;
        AuroraGetRenderSize(&width, &height);
        if (width != mLastRenderWidth || height != mLastRenderHeight) {
            mLastRenderWidth = width;
            mLastRenderHeight = height;
            mCarousel->refresh();
        }
    }
    for (const auto& component : mComponents) {
        component->update();
    }
    Document::update();
}

bool GraphicsTuner::focus() {
    for (const auto& component : mComponents) {
        if (component->focus()) {
            return true;
        }
    }
    return false;
}

bool GraphicsTuner::visible() const {
    return mRoot->HasAttribute("open");
}

bool GraphicsTuner::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel) {
        pop();
        return true;
    }

    if (mCarousel && mCarousel->handle_nav_command(cmd)) {
        return true;
    }

    return Document::handle_nav_command(event, cmd);
}

void GraphicsTuner::reset_default() {
    mSetting.set(mSetting.defaultValue);
}

}  // namespace dusk::ui
