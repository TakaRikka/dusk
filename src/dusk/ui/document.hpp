#pragma once

#include "component.hpp"
#include "ui.hpp"

#include <span>

namespace dusk::ui {

class Document {
public:
    explicit Document(
        const Rml::String& source, bool passive = false, DocumentScope scope = DocumentScope::None);
    virtual ~Document();

    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

    virtual void show();
    virtual void hide(bool close);
    virtual void update();
    virtual bool focus();
    virtual bool visible() const;
    virtual bool active() const;
    virtual bool obscures_game() const { return false; }
    virtual void cover() {
        mWasVisible = visible();
        hide(false);
    }
    virtual void uncover() {
        if (mWasVisible) {
            show();
        } else {
            focus();
        }
    }

    DocumentScope scope() const { return mScope; }
    bool set_document_styles(const Rml::String& rcss);
    void restyle(std::span<const Rml::StyleSheetContainer* const> sheets);
    void listen(Rml::Element* element, Rml::EventId event, ScopedEventListener::Callback callback,
        bool capture = false);
    void listen(Rml::Element* element, const Rml::String& event,
        ScopedEventListener::Callback callback, bool capture = false);
    void listen(Rml::EventId event, ScopedEventListener::Callback callback, bool capture = false) {
        listen(mDocument, event, std::move(callback), capture);
    }
    void listen(
        const Rml::String& event, ScopedEventListener::Callback callback, bool capture = false) {
        listen(mDocument, event, std::move(callback), capture);
    }
    void toggle() {
        if (visible()) {
            hide(false);
        } else {
            show();
        }
    }
    void push(std::unique_ptr<Document> document) {
        push_document(std::move(document));
        cover();
    }
    void pop() {
        hide(true);
        uncover_top_document();
    }

    bool closed() const { return mClosed; }

    bool handle_nav_event(Rml::Event& event);

protected:
    virtual bool handle_nav_command(Rml::Event& event, NavCommand cmd);

    Rml::ElementDocument* mDocument;
    std::vector<std::unique_ptr<ScopedEventListener>> mListeners;
    Rml::SharedPtr<Rml::StyleSheetContainer> mBaseStyleSheets;
    Rml::SharedPtr<Rml::StyleSheetContainer> mDocumentStyleSheets;
    DocumentScope mScope = DocumentScope::None;
    bool mPendingClose = false;
    bool mClosed = false;
    bool mPassive = false;
    bool mRestyled = false;
    bool mWasVisible = false;
};

}  // namespace dusk::ui
