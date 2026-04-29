#include "component.hpp"

#include "aurora/lib/dolphin/gd/gd.hpp"

namespace dusk::ui {
static aurora::Module Log{"dusk::ui::component"};

Component::Component(Rml::Element* root) : mRoot(root) {}

Component::~Component() = default;

void Component::update() {
    for (const auto& child : mChildren) {
        child->update();
    }
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

void Component::clear_children() {
    mChildren.clear();
    while (mRoot->GetNumChildren() > 0) {
        mRoot->RemoveChild(mRoot->GetFirstChild());
    }
}

}  // namespace dusk::ui