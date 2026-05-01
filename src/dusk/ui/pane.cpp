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
    : FluentComponent(createRoot(parent)), mDirection(direction) {
    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);

        // If
        if ((mDirection == Direction::Vertical && cmd == NavCommand::Right) ||
            (mDirection == Direction::Horizontal && cmd == NavCommand::Down))
        {
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
            set_selected_item(focusedChild);
            return;
        }

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
        while (i >= 0 && i < mChildren.size()) {
            if (mChildren[i]->focus()) {
                event.StopPropagation();
                break;
            }
            i += direction;
        }
    });

    // Listen for selection change events
    listen(Rml::EventId::Change, [this](Rml::Event& event) {
        const auto it = std::find_if(event.GetParameters().begin(), event.GetParameters().end(),
            [](const auto& param) { return param.first == "selected"; });
        if (it != event.GetParameters().end()) {
            const auto selected = it->second.Get<bool>();
            int childIndex = -1;
            for (int i = 0; i < mChildren.size(); ++i) {
                if (event.GetTargetElement() == mChildren[i]->root()) {
                    childIndex = i;
                }
            }
            if (childIndex != -1) {
                if (selected) {
                    set_selected_item(childIndex);
                } else if (childIndex == mSelectedItem) {
                    set_selected_item(-1);
                }
            } else {
                set_selected_item(-1);
            }
        }
    });
}

void Pane::update() {
    finalize();
    Component::update();
}

void Pane::set_selected_item(int index) {
    if (mSelectedItem == index) {
        return;
    }
    if (mSelectedItem >= 0 && mSelectedItem < mChildren.size()) {
        mChildren[mSelectedItem]->set_selected(false);
    }
    if (index >= 0 && index < mChildren.size()) {
        mSelectedItem = index;
        mChildren[index]->set_selected(true);
    } else {
        mSelectedItem = -1;
    }
}

bool Pane::focus() {
    // Update selected child
    for (int i = 0; i < mChildren.size(); ++i) {
        if (mChildren[i]->selected()) {
            mSelectedItem = i;
        }
    }
    // If there's a selected child, focus that
    if (mSelectedItem >= 0 && mSelectedItem < mChildren.size() && mChildren[mSelectedItem]->focus())
    {
        return true;
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
