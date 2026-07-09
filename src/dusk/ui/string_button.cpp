#include "string_button.hpp"

#include <aurora/rmlui.hpp>

namespace dusk::ui {

BaseStringButton::BaseStringButton(Rml::Element* parent, Props props)
    : BaseControlledSelectButton(parent, {std::move(props.key)}), mType(std::move(props.type)),
      mMaxLength(props.maxLength) {
    mInputListeners.reserve(4);
}

void BaseStringButton::update() {
    if (mPendingStopEditing) {
        stop_editing(mPendingCommit, mPendingRefocusRoot);
    }
    if (mPendingInputFocusFrames > 0) {
        --mPendingInputFocusFrames;
        if (mPendingInputFocusFrames == 0) {
            focus_input();
        }
    }
    BaseControlledSelectButton::update();
}

void BaseStringButton::start_editing() {
    if (is_editing()) {
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
    mInputElem->SetAttribute("value", input_value());
    if (mMaxLength > -1) {
        mInputElem->SetAttribute("maxlength", mMaxLength);
    }
    mRoot->AppendChild(std::move(elemPtr));

    // Hide value element
    mValueElem->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Hidden);

    // RmlUi lays out the new input during render. Wait one full frame before focusing it so
    // mobile keyboard placement gets a valid caret rectangle.
    mPendingInputFocusFrames = 2;

    // Dispatch a submit event so the pane can handle item selection
    // However, mark it as "handled" to ensure that we don't steal focus away
    mRoot->DispatchEvent(Rml::EventId::Submit, {{"handled", Rml::Variant{true}}});

    // Register input listeners
    mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(
        mInputElem, Rml::EventId::Textinput, [this](Rml::Event& event) {
            if (event.GetTargetElement() == mInputElem) {
                const Rml::String text = event.GetParameter("text", Rml::String{});
                if (!text.empty() && std::ranges::all_of(text, [](const char c) { return c == '\r' || c == '\n' || c == '\t'; })) {
                    event.StopImmediatePropagation();
                }
            }
        }));
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
        if (!is_editing()) {
            start_editing();
        } else {
            request_stop_editing(true, true);
        }
        return true;
    } else if (cmd == NavCommand::Cancel) {
        if (is_editing()) {
            request_stop_editing(false, true);
            return true;
        }
    }
    return false;
}

void BaseStringButton::focus_input() {
    if (!is_editing()) {
        return;
    }

    aurora::rmlui::set_input_type(
        mType == "number" ? aurora::rmlui::InputType::Number : aurora::rmlui::InputType::Text);

    if (mInputElem->Focus(true)) {
        const int end = static_cast<int>(Rml::StringUtilities::LengthUTF8(mInputElem->GetValue()));
        mInputElem->SetSelectionRange(0, end);
    }
}

void BaseStringButton::stop_editing(bool commit, bool refocusRoot) {
    mPendingStopEditing = false;
    mPendingInputFocusFrames = 0;
    if (!is_editing()) {
        return;
    }
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
      mGetValue(std::move(props.getValue)), mSetValue(std::move(props.setValue)),
      mIsDisabled(std::move(props.isDisabled)), mIsModified(std::move(props.isModified)) {}

bool StringButton::modified() const {
    if (mIsModified) {
        return mIsModified();
    }
    return BaseStringButton::modified();
}

bool StringButton::disabled() const {
    if (mIsDisabled) {
        return mIsDisabled();
    }
    return BaseStringButton::disabled();
}

}  // namespace dusk::ui
