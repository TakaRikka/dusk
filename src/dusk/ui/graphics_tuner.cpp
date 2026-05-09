#include "graphics_tuner.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include <dolphin/gx/GXAurora.h>
#include <dolphin/vi.h>
#include <fmt/format.h>

#include "dusk/config.hpp"
#include "dusk/settings.h"

#include <algorithm>
#include <string>

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

int get_value(GraphicsOption option) {
    switch (option) {
    case GraphicsOption::InternalResolution:
        return getSettings().game.internalResolutionScale.getValue();
    case GraphicsOption::ShadowResolution:
        return getSettings().game.shadowResolutionMultiplier.getValue();
    case GraphicsOption::BloomMode:
        return static_cast<int>(getSettings().game.bloomMode.getValue());
    case GraphicsOption::BloomMultiplier:
        return std::clamp(
            static_cast<int>(getSettings().game.bloomMultiplier.getValue() * 100.0f + 0.5f), 0,
            100);
    }
    return 0;
}

void set_value(GraphicsOption option, int value) {
    switch (option) {
    case GraphicsOption::InternalResolution:
        getSettings().game.internalResolutionScale.setValue(value);
        VISetFrameBufferScale(static_cast<float>(value));
        break;
    case GraphicsOption::ShadowResolution:
        getSettings().game.shadowResolutionMultiplier.setValue(value);
        break;
    case GraphicsOption::BloomMode:
        getSettings().game.bloomMode.setValue(static_cast<BloomMode>(std::clamp(
            value, static_cast<int>(BloomMode::Off), static_cast<int>(BloomMode::Dusk))));
        break;
    case GraphicsOption::BloomMultiplier:
        getSettings().game.bloomMultiplier.setValue(std::clamp(value, 0, 100) / 100.0f);
        break;
    }
    config::Save();
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

void SteppedCarousel::update() {
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

Rml::String format_graphics_setting_value(GraphicsOption option, int value) {
    switch (option) {
    case GraphicsOption::InternalResolution: {
        u32 width = 0;
        u32 height = 0;
        AuroraGetRenderSize(&width, &height);
        if (value <= 0) {
            return fmt::format("Auto ({}×{})", width, height);
        } else {
            return fmt::format("{}× ({}×{})", value, width, height);
        }
    }
    case GraphicsOption::ShadowResolution:
        return fmt::format("{}×", value);
    case GraphicsOption::BloomMode:
        switch (static_cast<BloomMode>(value)) {
        case BloomMode::Off:
            return "Off";
        case BloomMode::Classic:
            return "Classic";
        case BloomMode::Dusk:
            return "Dusk";
        }
        break;
    case GraphicsOption::BloomMultiplier:
        return fmt::format("{}%", value);
    }
    return "";
}

GraphicsTuner::GraphicsTuner(GraphicsTunerProps props, bool prelaunch)
    : Document(kDocumentSource), mOption(props.option), mValueMin(props.valueMin),
      mValueMax(props.valueMax), mDefaultValue(props.defaultValue), mPrelaunch(prelaunch) {
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
                .min = mValueMin,
                .max = mValueMax,
                .step = 1,
                .getValue = [this] { return get_value(mOption); },
                .onChange = [this](int value) { set_value(mOption, value); },
                .formatValue =
                    [this](int value) { return format_graphics_setting_value(mOption, value); },
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

void GraphicsTuner::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
    mDoAud_seStartMenu(kSoundWindowOpen);
}

void GraphicsTuner::hide(bool close) {
    mRoot->RemoveAttribute("open");
    if (close) {
        mPendingClose = true;
        mDoAud_seStartMenu(kSoundWindowClose);
    }
}

void GraphicsTuner::update() {
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

    return mPrelaunch ? false : Document::handle_nav_command(event, cmd);
}

void GraphicsTuner::reset_default() {
    set_value(mOption, mDefaultValue);
}

}  // namespace dusk::ui
