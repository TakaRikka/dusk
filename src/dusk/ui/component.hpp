#pragma once

#include "event.hpp"
#include "ui.hpp"

#include <RmlUi/Core.h>

#include <memory>
#include <utility>
#include <vector>

namespace Rml {
class Element;
}

namespace dusk::ui {

class Component {
public:
    Component() = default;
    explicit Component(Rml::Element* root);
    virtual ~Component();

    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    virtual void update();
    virtual bool focus();

    virtual bool selected() const { return mRoot->IsPseudoClassSet("selected"); }
    virtual void set_selected(bool selected);
    virtual bool disabled() const { return mRoot->IsPseudoClassSet("disabled"); }
    virtual void set_disabled(bool disabled);

    void listen(Rml::Element* element, Rml::EventId event, ScopedEventListener::Callback callback,
        bool capture = false);
    bool contains(Rml::Element* element) const;

    template <typename T, typename... Args>
    requires std::is_base_of_v<Component, T> T& add_child(Args&&... args) {
        auto child = std::make_unique<T>(mRoot, std::forward<Args>(args)...);
        T& ref = *child;
        mChildren.emplace_back(std::move(child));
        return ref;
    }

    Rml::Element* root() const { return mRoot; }

protected:
    static Rml::Element* append(Rml::Element* parent, const Rml::String& tag);
    void clear_children();

    Rml::Element* mRoot = nullptr;
    std::vector<std::unique_ptr<Component> > mChildren;
    std::vector<std::unique_ptr<ScopedEventListener> > mListeners;
};

template <class Derived>
class FluentComponent : public Component {
public:
    using Component::Component;

    Derived& listen(
        Rml::EventId event, ScopedEventListener::Callback callback, bool capture = false) {
        Component::listen(mRoot, event, std::move(callback), capture);
        return static_cast<Derived&>(*this);
    }

    Derived& on_focus(ScopedEventListener::Callback callback) {
        return listen(
            Rml::EventId::Focus, [this, callback = std::move(callback)](Rml::Event& event) {
                if (!disabled()) {
                    callback(event);
                }
            });
    }

    Derived& on_nav_command(std::function<bool(Rml::Event&, NavCommand)> callback) {
        listen(Rml::EventId::Click, [this, callback](Rml::Event& event) {
            if (!disabled() && callback(event, NavCommand::Confirm)) {
                event.StopPropagation();
            }
        });
        listen(Rml::EventId::Keydown, [this, callback = std::move(callback)](Rml::Event& event) {
            if (disabled()) {
                return;
            }
            const auto cmd = map_nav_event(event);
            if (cmd != NavCommand::None && callback(event, cmd)) {
                event.StopPropagation();
            }
        });
        return static_cast<Derived&>(*this);
    }
};

}  // namespace dusk::ui
