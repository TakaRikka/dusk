#include "document.hpp"

#include "aurora/rmlui.hpp"
#include "ui.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

namespace dusk::ui {
namespace {

Rml::ElementDocument* load_document(const Rml::String& source) {
    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return nullptr;
    }
    return context->LoadDocumentFromMemory(source);
}

}  // namespace

Document::Document(const Rml::String& source) : mDocument(load_document(source)) {
    // Block events while hidden (except for Menu command); play nav sounds when visible
    listen(
        Rml::EventId::Keydown,
        [this](Rml::Event& event) {
            const auto cmd = map_nav_event(event);
            if (cmd != NavCommand::Menu && !visible()) {
                event.StopImmediatePropagation();
            }
        },
        true);
    const auto blockUnlessVisible = [this](Rml::Event& event) {
        if (!visible()) {
            event.StopImmediatePropagation();
        }
    };
    listen(Rml::EventId::Mouseover, blockUnlessVisible, true);
    listen(Rml::EventId::Click, blockUnlessVisible, true);
    listen(Rml::EventId::Scroll, blockUnlessVisible, true);

    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);
        if (cmd == NavCommand::None) {
            return;
        }
        if (handle_nav_command(event, cmd)) {
            event.StopPropagation();
        }
    });
}

Document::~Document() {
    mListeners.clear();
    if (mDocument != nullptr) {
        mDocument->Close();
        mDocument = nullptr;
    }
}

void Document::show() {
    if (mDocument != nullptr) {
        // Attempt to preserve the previously focused element
        mDocument->Show(Rml::ModalFlag::None, Rml::FocusFlag::Keep, Rml::ScrollFlag::None);
        // If nothing is focused, let the document decide the initial focus
        auto* leaf = mDocument->GetFocusLeafNode();
        if (leaf == nullptr || leaf == mDocument) {
            focus();
        }
    }
}

void Document::hide(bool close) {
    if (mDocument != nullptr) {
        mDocument->Hide();
    }
    if (close) {
        mClosed = true;
    }
}

void Document::update() {}

bool Document::focus() {
    return false;
}

void Document::listen(Rml::Element* element, Rml::EventId event,
    ScopedEventListener::Callback callback, bool capture) {
    if (element == nullptr) {
        element = mDocument;
    }
    if (element == nullptr || !callback) {
        return;
    }
    mListeners.emplace_back(
        std::make_unique<ScopedEventListener>(element, event, std::move(callback), capture));
}

bool Document::visible() const {
    if (mDocument == nullptr) {
        return false;
    }
    return *mDocument->GetProperty(Rml::PropertyId::Visibility) == Rml::Style::Visibility::Visible;
}

bool Document::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Menu) {
        mDoAud_seStartMenu(visible() ? kSoundMenuClose : kSoundMenuOpen);
        toggle();
        return true;
    }
    return false;
}

}  // namespace dusk::ui
