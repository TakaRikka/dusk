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
    virtual void hide();
    virtual void update();
    virtual bool focus();
    virtual bool visible() const;

    void listen(Rml::Element* element, Rml::EventId event, ScopedEventListener::Callback callback,
        bool capture = false);
    void listen(Rml::EventId event, ScopedEventListener::Callback callback, bool capture = false) {
        listen(mDocument, event, std::move(callback), capture);
    }

    bool can_destroy() const;

protected:
    virtual bool handle_nav_command(Rml::Event& event, NavCommand cmd);

    Rml::ElementDocument* mDocument;
    std::vector<std::unique_ptr<ScopedEventListener> > mListeners;
};

}  // namespace dusk::ui
