#include "button.hpp"

#include "i18n.hpp"
#include "ui.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include <utility>

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent, const Rml::String& tagName) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement(tagName);
    return parent->AppendChild(std::move(elem));
}

}  // namespace

Button::Button(Rml::Element* parent, Props props, const Rml::String& tagName)
    : FluentComponent(createRoot(parent, tagName)) {
    update_props(std::move(props));
}

void Button::set_text(const Rml::String& text) {
    if (mProps.text != text) {
        mProps.text = text;
        render_text();
    }
}

void Button::update() {
    if (mRenderedLanguage != i18n::language()) {
        render_text();
    }
    Component::update();
}

Button& Button::on_pressed(ButtonCallback callback) {
    if (!callback) {
        return *this;
    }
    // TODO: convert this to a FluentComponent method?
    on_nav_command([callback = std::move(callback)](Rml::Event&, NavCommand cmd) {
        if (cmd == NavCommand::Confirm) {
            callback();
            return true;
        }
        return false;
    });
    return *this;
}

void Button::update_props(Props props) {
    set_text(props.text);
    mProps = std::move(props);
    render_text();
}

void Button::render_text() {
    mRoot->SetInnerRML(escape(i18n::tr(mProps.text)));
    mRenderedLanguage = i18n::language();
}

void ControlledButton::update() {
    if (mIsSelected) {
        set_selected(mIsSelected());
    }
    Button::update();
}

bool ControlledButton::selected() const {
    if (mIsSelected) {
        return mIsSelected();
    }
    return Button::selected();
}

}  // namespace dusk::ui
