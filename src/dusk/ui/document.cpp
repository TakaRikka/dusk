#include "document.hpp"

#include "aurora/rmlui.hpp"
#include "ui.hpp"

#include "m_Do/m_Do_audio.h"

#include <algorithm>

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

Document::Document(const Rml::String& source, bool passive, DocumentScope scope)
    : mDocument(load_document(source)), mScope(scope), mPassive(passive) {
    if (mDocument != nullptr) {
        if (const auto* base = mDocument->GetStyleSheetContainer()) {
            // Clone a pristine snapshot to rebuild from on every restyle
            mBaseStyleSheets = base->CombineStyleSheetContainer(Rml::StyleSheetContainer{});
        }
        apply_scoped_styles(*this);
    }

    // Block events while hidden (except for Menu command); play nav sounds when visible
    listen(
        Rml::EventId::Keydown,
        [this](Rml::Event& event) {
            if (mPassive) {
                return;
            }
            const auto cmd = map_nav_event(event);
            if (cmd != NavCommand::Menu && (!visible() || !active())) {
                event.StopImmediatePropagation();
            }
        },
        true);
    const auto blockUnlessActive = [this](Rml::Event& event) {
        if (!visible() || !active()) {
            event.StopImmediatePropagation();
        }
    };
    listen(Rml::EventId::Mouseover, blockUnlessActive, true);
    listen(Rml::EventId::Click, blockUnlessActive, true);
    listen(Rml::EventId::Scroll, blockUnlessActive, true);

    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        if (mPassive) {
            auto* doc = top_document();
            if (doc != nullptr && doc->handle_nav_event(event)) {
                event.StopPropagation();
            }
            return;
        }
        if (handle_nav_event(event)) {
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

bool Document::set_document_styles(const Rml::String& rcss) {
    if (rcss.empty()) {
        mDocumentStyleSheets = nullptr;
    } else {
        auto sheet = Rml::Factory::InstanceStyleSheetString(rcss);
        if (sheet == nullptr) {
            return false;
        }
        mDocumentStyleSheets = std::move(sheet);
    }
    apply_scoped_styles(*this);
    return true;
}

void Document::restyle(std::span<const Rml::StyleSheetContainer* const> sheets) {
    if (mDocument == nullptr) {
        return;
    }
    const bool wantsExtra =
        mDocumentStyleSheets != nullptr ||
        std::ranges::any_of(sheets, [](const auto* sheet) { return sheet != nullptr; });
    // Nothing to add
    if (!wantsExtra && !mRestyled) {
        return;
    }
    auto combined = mBaseStyleSheets;
    const auto combine = [&combined](const Rml::StyleSheetContainer& sheet) {
        if (combined != nullptr) {
            combined = combined->CombineStyleSheetContainer(sheet);
        } else {
            combined = sheet.CombineStyleSheetContainer(Rml::StyleSheetContainer{});
        }
    };
    for (const auto* sheet : sheets) {
        if (sheet != nullptr) {
            combine(*sheet);
        }
    }
    if (mDocumentStyleSheets != nullptr) {
        combine(*mDocumentStyleSheets);
    }
    mDocument->SetStyleSheetContainer(std::move(combined));
    mRestyled = wantsExtra;
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

void Document::listen(Rml::Element* element, const Rml::String& event,
    ScopedEventListener::Callback callback, bool capture) {
    if (element == nullptr) {
        element = mDocument;
    }
    if (element == nullptr || event.empty() || !callback) {
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

bool Document::active() const {
    return !mClosed && !mPendingClose;
}

bool Document::handle_nav_event(Rml::Event& event) {
    if (!active()) {
        return false;
    }
    const auto cmd = map_nav_event(event);
    if (cmd == NavCommand::None || (cmd != NavCommand::Menu && !visible())) {
        return false;
    }
    return handle_nav_command(event, cmd);
}

bool Document::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Menu) {
        if (game_obscured_below(*this)) {
            return true;
        }
        mDoAud_seStartMenu(visible() ? kSoundMenuClose : kSoundMenuOpen);
        toggle();
        return true;
    }
    return false;
}

}  // namespace dusk::ui
