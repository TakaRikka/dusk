#pragma once

#include "component.hpp"
#include "ui.hpp"

namespace dusk::ui {

class Document {
public:
    Document(const Rml::String& source);
    virtual ~Document();

    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

    virtual void show();
    virtual void hide(bool close);
    virtual void update();
    virtual bool focus();
    virtual bool visible() const;

    void listen(Rml::Element* element, Rml::EventId event, ScopedEventListener::Callback callback,
        bool capture = false);
    void listen(Rml::EventId event, ScopedEventListener::Callback callback, bool capture = false) {
        listen(mDocument, event, std::move(callback), capture);
    }
    void toggle() {
        if (visible()) {
            hide(false);
        } else {
            show();
        }
    }
    void pop() {
        hide(true);
        show_top_document();
    }

    bool pending_close() const { return mPendingClose; }
    bool closed() const { return mClosed; }

protected:
    virtual bool handle_nav_command(Rml::Event& event, NavCommand cmd);

    Rml::ElementDocument* mDocument;
    std::vector<std::unique_ptr<ScopedEventListener> > mListeners;
    bool mPendingClose = false;
    bool mClosed = false;
};

}  // namespace dusk::ui
