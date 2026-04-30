#include "string_button.hpp"

namespace dusk::ui {

BaseStringButton::BaseStringButton(Rml::Element* parent, Props props)
    : ControlledSelectButton(parent, {std::move(props.key)}), mType(std::move(props.type)),
      mMaxLength(props.maxLength) {
    mInputListeners.reserve(3);
}

void BaseStringButton::update() {
    if (mPendingStopEditing) {
        stop_editing(mPendingCommit, mPendingRefocusRoot);
    }
    ControlledSelectButton::update();
}

void BaseStringButton::start_editing() {
    if (mInputElem != nullptr) {
        return;
    }

    // Create input element
    auto* doc = mRoot->GetOwnerDocument();
    auto elemPtr = doc->CreateElement("input");
    mInputElem = rmlui_dynamic_cast<Rml::ElementFormControlInput*>(elemPtr.get());
    if (mInputElem == nullptr) {
        return;
    }
    mInputElem->SetAttribute("type", mType);
    mInputElem->SetAttribute("value", format_value());
    if (mMaxLength > -1) {
        mInputElem->SetAttribute("maxlength", mMaxLength);
    }
    mRoot->AppendChild(std::move(elemPtr));

    // Hide value element
    mValueElem->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Hidden);

    // Focus and select text within input
    mInputElem->Focus(true);
    const int end = static_cast<int>(Rml::StringUtilities::LengthUTF8(mInputElem->GetValue()));
    mInputElem->SetSelectionRange(0, end);

    // Mark button as selected to indicate "active"
    set_selected(true);

    // Register input listeners
    mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(
        mInputElem, Rml::EventId::Keydown, [this](Rml::Event& event) {
            const auto cmd = map_nav_event(event);
            if (cmd == NavCommand::Confirm) {
                request_stop_editing(true, true);
                event.StopImmediatePropagation();
            } else if (cmd == NavCommand::Cancel) {
                request_stop_editing(false, true);
                event.StopImmediatePropagation();
            }
        }));
    mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(
        mInputElem, Rml::EventId::Click, [](Rml::Event& event) { event.StopPropagation(); }));
    mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(mInputElem,
        Rml::EventId::Blur, [this](Rml::Event&) { request_stop_editing(true, false); }));
}

void BaseStringButton::request_stop_editing(bool commit, bool refocusRoot) {
    mPendingStopEditing = true;
    mPendingCommit = commit;
    mPendingRefocusRoot = refocusRoot;
}

bool BaseStringButton::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Confirm) {
        if (mInputElem == nullptr) {
            start_editing();
        } else {
            request_stop_editing(true, true);
        }
        return true;
    } else if (cmd == NavCommand::Cancel) {
        request_stop_editing(false, true);
        return true;
    }
    return false;
}

void BaseStringButton::stop_editing(bool commit, bool refocusRoot) {
    if (mInputElem == nullptr) {
        return;
    }
    mPendingStopEditing = false;
    if (commit) {
        set_value(mInputElem->GetValue());
    }
    mInputListeners.clear();
    mRoot->RemoveChild(mInputElem);
    mInputElem = nullptr;

    // Restore value element
    mValueElem->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Visible);

    set_selected(false);
    if (refocusRoot) {
        mRoot->Focus(true);
    }
}

StringButton::StringButton(Rml::Element* parent, Props props)
    : BaseStringButton(parent, {.key = std::move(props.key), .maxLength = props.maxLength}),
      mGetValue(std::move(props.getValue)), mSetValue(std::move(props.setValue)) {}

}  // namespace dusk::ui