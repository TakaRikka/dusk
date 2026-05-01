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

Pane::Pane(Rml::Element* parent, Type type) : FluentComponent(createRoot(parent)), mType(type) {
    listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto cmd = map_nav_event(event);

        // If navigating to the next pane, select the focused item
        if (mType == Type::Controlled && cmd == NavCommand::Right) {
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
        if (cmd == NavCommand::Down) {
            direction = 1;
        } else if (cmd == NavCommand::Up) {
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

    if (type == Type::Controlled) {
        // For controlled panes, handle SelectButton Submit events for item selection
        listen(Rml::EventId::Submit, [this](Rml::Event& event) {
            int childIndex = -1;
            for (int i = 0; i < mChildren.size(); ++i) {
                if (event.GetTargetElement() == mChildren[i]->root()) {
                    childIndex = i;
                }
            }
            set_selected_item(childIndex);
            // If the selection was handled locally, don't allow it to bubble up to window
            if (event.GetParameter("handled", false)) {
                event.StopPropagation();
            }
        });
    }
}

void Pane::update() {
    finalize();
    Component::update();
}

void Pane::set_selected_item(int index) {
    if (mType == Type::Uncontrolled) {
        return;
    }
    for (int i = 0; i < mChildren.size(); ++i) {
        mChildren[i]->set_selected(i == index);
    }
}

bool Pane::focus() {
    // Focus the first selected child
    for (const auto& child : mChildren) {
        if (child->selected() && child->focus()) {
            return true;
        }
    }
    // Otherwise, focus the first focusable child
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
