#include "pane.hpp"

#include "ui.hpp"

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("pane");
    return parent->AppendChild(std::move(elem));
}

}  // namespace

Pane::Pane(Rml::Element* parent, Direction direction)
    : Component(createRoot(parent)), mDirection(direction) {
    listen(mRoot, Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);
        int direction = 0;
        if ((mDirection == Direction::Vertical && cmd == NavCommand::Down) ||
            (mDirection == Direction::Horizontal && cmd == NavCommand::Right))
        {
            direction = 1;
        } else if ((mDirection == Direction::Vertical && cmd == NavCommand::Up) ||
                   (mDirection == Direction::Horizontal && cmd == NavCommand::Left))
        {
            direction = -1;
        } else {
            return;
        }
        auto* target = event.GetTargetElement();
        int focusedChild = -1;
        for (size_t i = 0; i < mChildren.size(); ++i) {
            if (mChildren[i]->contains(target)) {
                focusedChild = i;
                break;
            }
        }
        if (focusedChild == -1) {
            return;
        }
        int i = focusedChild + direction;
        while (i >= 0 && i < static_cast<int>(mChildren.size())) {
            if (mChildren[i]->focus()) {
                event.StopPropagation();
                break;
            }
            i += direction;
        }
    });
}

void Pane::update() {
    finalize();
    Component::update();
}

bool Pane::focus() {
    // If there's a selected child, focus that
    for (const auto& child : mChildren) {
        if (child->selected() && child->focus()) {
            return true;
        }
    }
    for (const auto& child : mChildren) {
        if (child->focus()) {
            return true;
        }
    }
    return false;
}

Rml::Element* Pane::add_section(const Rml::String& text) {
    auto* elem = append(mRoot, "div");
    elem->SetClass("section-heading", true);
    elem->SetInnerRML(escape(text));
    return elem;
}

Rml::Element* Pane::add_text(const Rml::String& text) {
    auto* elem = append(mRoot, "div");
    elem->SetInnerRML(escape(text));
    return elem;
}

Rml::Element* Pane::add_rml(const Rml::String& rml) {
    auto* elem = append(mRoot, "div");
    elem->SetInnerRML(rml);
    return elem;
}

void Pane::finalize() {
    if (finalized) {
        return;
    }
    finalized = true;

    // Append spacer element to the bottom. RmlUi does not properly handle
    // padding-bottom or margin-bottom on a scrollable flex container, so
    // we need to create a fake spacer with an actual layout height to get
    // padding at the bottom of a scrollable container.
    append(mRoot, "spacer");
}

void Pane::clear() {
    clear_children();
    finalized = false;
}

}  // namespace dusk::ui
