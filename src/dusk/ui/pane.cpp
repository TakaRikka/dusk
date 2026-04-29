#include "pane.hpp"

#include "ui.hpp"

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent, const Rml::String& className) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("div");
    elem->SetClass(className, true);
    return parent->AppendChild(std::move(elem));
}

}  // namespace

Pane::Pane(Rml::Element* parent, Direction direction, const Rml::String& className)
    : Component(createRoot(parent, className)), mDirection(direction) {
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
        int i = focusedChild + direction;
        if (focusedChild == -1) {
            // If the container itself is focused and next is pressed, focus the first element
            if (direction == 1) {
                i = 0;
            } else {
                // Otherwise, allow event to bubble
                return;
            }
        }
        while (i >= 0 && i < static_cast<int>(mChildren.size())) {
            if (mChildren[i]->focus()) {
                event.StopPropagation();
                break;
            }
            i += direction;
        }
    });
}

bool Pane::focus() {
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

void Pane::clear() {
    clear_children();
}

}  // namespace dusk::ui