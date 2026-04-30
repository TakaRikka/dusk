#include "component.hpp"

namespace dusk::ui {

Component::Component(Rml::Element* root) : mRoot(root) {}

Component::~Component() = default;

void Component::update() {
    for (const auto& child : mChildren) {
        child->update();
    }
}

bool Component::focus() {
    // Can we focus self?
    if (mRoot->Focus(true)) {
        return true;
    }
    // Otherwise, try to focus a child
    for (const auto& child : mChildren) {
        if (child->focus()) {
            return true;
        }
    }
    return false;
}

Rml::Element* Component::append(Rml::Element* parent, const Rml::String& tag) {
    if (parent == nullptr) {
        return nullptr;
    }
    auto* doc = parent->GetOwnerDocument();
    if (doc == nullptr) {
        return nullptr;
    }
    return parent->AppendChild(doc->CreateElement(tag));
}

void Component::listen(Rml::Element* element, Rml::EventId event,
    ScopedEventListener::Callback callback, bool capture) {
    if (element == nullptr) {
        element = mRoot;
    }
    mListeners.emplace_back(
        std::make_unique<ScopedEventListener>(element, event, std::move(callback), capture));
}

bool Component::contains(Rml::Element* element) const {
    for (const auto* node = element; node != nullptr; node = node->GetParentNode()) {
        if (node == mRoot) {
            return true;
        }
    }
    return false;
}

void Component::clear_children() {
    mChildren.clear();
    while (mRoot->GetNumChildren() > 0) {
        mRoot->RemoveChild(mRoot->GetFirstChild());
    }
}

}  // namespace dusk::ui