#include "document.hpp"

#include "aurora/rmlui.hpp"
#include "ui.hpp"

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
    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);
        if (cmd != NavCommand::None && handle_nav_command(event, cmd)) {
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
        mDocument->Show();
        focus();
    }
}

void Document::hide() {
    if (mDocument != nullptr) {
        mDocument->Hide();
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

bool Document::can_destroy() const {
    if (mDocument == nullptr) {
        return true;
    }
    return *mDocument->GetProperty(Rml::PropertyId::Visibility) == Rml::Style::Visibility::Hidden;
}

bool Document::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    return false;
}

}  // namespace dusk::ui
