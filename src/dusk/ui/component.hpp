#pragma once

#include "event.hpp"

#include <RmlUi/Core.h>

#include <memory>
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

    void listen(Rml::Element* element, Rml::EventId event, ScopedEventListener::Callback callback,
        bool capture = false);

    Rml::Element* root() const { return mRoot; }

protected:
    static Rml::Element* append(Rml::Element* parent, const Rml::String& tag);
    void clear_children();

    template <typename T, typename... Args>
    requires std::is_base_of_v<Component, T> T& add_child(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        mChildren.emplace_back(std::move(child));
        return ref;
    }

    Rml::Element* mRoot = nullptr;
    std::vector<std::unique_ptr<Component> > mChildren;
    std::vector<std::unique_ptr<ScopedEventListener> > mListeners;
};

}  // namespace dusk::ui
