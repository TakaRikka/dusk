#include "overlay.hpp"

#include <dolphin/gx/GXAurora.h>
#include <dolphin/vi.h>
#include <fmt/format.h>

#include "dusk/config.hpp"
#include "dusk/settings.h"

#include <string>

namespace dusk::ui {
namespace {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/overlay.rcss" />
</head>
<body>
    <div id="root" class="overlay-root">
        <div class="overlay">
            <div class="header">
                <div id="title"></div>
                <div id="carousel-container" class="carousel-container"></div>
            </div>
            <div id="description" class="description"></div>
            <div class="divider"></div>
            <div class="footer">
                <div id="return"></div>
                <div id="reset"></div>
            </div>
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
    button->SetInnerRML(escape(label));
    return parent->AppendChild(std::move(button));
}

}  // namespace

SteppedCarousel::SteppedCarousel(Rml::Element* parent, Props props)
    : Component(create_stepped_carousel_root(parent)), mProps(std::move(props)) {
    Rml::Element* prevElem = create_stepped_carousel_arrow(mRoot, "prev", "<");
    mValueElem = append(mRoot, "div");
    mValueElem->SetClass("stepped-carousel-value", true);
    Rml::Element* nextElem = create_stepped_carousel_arrow(mRoot, "next", ">");

    listen(prevElem, Rml::EventId::Click,
        [this](Rml::Event&) { handle_nav_command(NavCommand::Left); });
    listen(nextElem, Rml::EventId::Click,
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
    if (mProps.onChange) {
        mProps.onChange(nextValue);
    }
}

Rml::String format_graphics_setting_value(GraphicsOption option, int value) {
    switch (option) {
    case GraphicsOption::InternalResolution:
        if (value <= 0) {
            return "Auto";
        } else {
            u32 width = 0;
            u32 height = 0;
            AuroraGetRenderSize(&width, &height);
            return fmt::format("{}x ({}x{})", value, width, height);
        }
    case GraphicsOption::ShadowResolution:
        return fmt::format("{}x", value);
    }
    return "";
}

Overlay::Overlay(OverlayProps props)
    : Document(kDocumentSource), mOption(props.option), mValueMin(props.valueMin),
      mValueMax(props.valueMax) {
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
        add_component<SteppedCarousel>(carouselParent,
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

    if (auto* returnParent = mDocument->GetElementById("return")) {
        auto& returnButton =
            add_component<Button>(returnParent, "Return", "footer-button").on_pressed(pop_document);
        returnButton.root()->SetClass("return", true);
    }
    if (auto* resetParent = mDocument->GetElementById("reset")) {
        auto& resetButton = add_component<Button>(resetParent, "Reset to default", "footer-button")
                                .on_pressed([this] { reset_default(); });
        resetButton.root()->SetClass("reset", true);
    }

    // Hide document after transition completion
    mRoot = mDocument->GetElementById("root");
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot &&
            *mRoot->GetProperty(Rml::PropertyId::Visibility) == Rml::Style::Visibility::Visible &&
            mDismissed)
        {
            Document::hide();
        }
    });
}

void Overlay::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
}

void Overlay::hide() {
    mRoot->RemoveAttribute("open");
    mDismissed = true;
}

void Overlay::update() {
    for (const auto& component : mComponents) {
        component->update();
    }
    Document::update();
}

bool Overlay::focus() {
    for (const auto& component : mComponents) {
        if (component->focus()) {
            return true;
        }
    }
    return false;
}

bool Overlay::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel) {
        pop_document();
        return true;
    }
    return Document::handle_nav_command(event, cmd);
}

void Overlay::reset_default() {
    set_value(mOption, mValueMin);
}

}  // namespace dusk::ui
