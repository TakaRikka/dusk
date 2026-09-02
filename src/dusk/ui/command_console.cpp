#include "command_console.hpp"

#include <RmlUi/Core.h>
#include <RmlUi/Core/Elements/ElementFormControlInput.h>
#include <aurora/rmlui.hpp>

#include "dusk/settings.h"

#include <algorithm>
#include <string>
#include <string_view>

#include "ui.hpp"

namespace dusk::ui {
namespace {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/command_console.rcss" />
</head>
<body>
    <console id="console">
        <output id="console-output"></output>
        <input id="console-input" type="text" maxlength="255" />
    </console>
</body>
</rml>
)RML";

bool is_command(std::string_view text) {
    return text.size() >= 2 && text[0] == '>' && text[1] == ' ';
}

}  // namespace

CommandConsole::CommandConsole() : Document(kDocumentSource, false, DocumentScope::CommandConsole) {
    mConsole = mDocument ? mDocument->GetElementById("console") : nullptr;
    mOutput = mDocument ? mDocument->GetElementById("console-output") : nullptr;
    auto* rawInput = mDocument ? mDocument->GetElementById("console-input") : nullptr;
    mInput = rmlui_dynamic_cast<Rml::ElementFormControlInput*>(rawInput);

    listen(
        Rml::EventId::Keydown,
        [this](Rml::Event& event) {
            if (!mInputActive) {
                return;
            }
            const auto key = static_cast<Rml::Input::KeyIdentifier>(
                event.GetParameter<int>("key_identifier", Rml::Input::KI_UNKNOWN));
            if (key == Rml::Input::KI_RETURN) {
                execute_from_input();
                event.StopImmediatePropagation();
            } else if (key == Rml::Input::KI_ESCAPE) {
                hide(false);
                event.StopImmediatePropagation();
            } else if (key == Rml::Input::KI_UP) {
                navigate_history(-1);
                event.StopImmediatePropagation();
            } else if (key == Rml::Input::KI_DOWN) {
                navigate_history(1);
                event.StopImmediatePropagation();
            } else if (key == Rml::Input::KI_PRIOR) {
                scroll_messages(-1);
                event.StopImmediatePropagation();
            } else if (key == Rml::Input::KI_NEXT) {
                scroll_messages(1);
                event.StopImmediatePropagation();
            }
        },
        true);

    listen(mOutput, Rml::EventId::Scroll, [this](Rml::Event&) {
        const float bottom =
            std::max(0.0f, mOutput->GetScrollHeight() - mOutput->GetClientHeight());
        mStickToBottom = mOutput->GetScrollTop() >= bottom - 4.0f;
    });
}

bool CommandConsole::handle_nav_command(Rml::Event&, NavCommand) {
    return false;
}

void CommandConsole::update() {
    if (!getSettings().backend.enableAdvancedSettings || is_prelaunch_open()) {
        close_input();
        Document::hide(false);
        return;
    }

    update_message_lifetimes();
    if (!mInputActive && !has_onscreen_messages()) {
        Document::hide(false);
        return;
    }

    if (!Document::visible()) {
        Document::show();
    }
    if (mInputActive && mStickToBottom && mOutput != nullptr) {
        mOutput->SetScrollTop(mOutput->GetScrollHeight() - mOutput->GetClientHeight());
    }
}

void CommandConsole::show() {
    if (mInputActive || !getSettings().backend.enableAdvancedSettings || is_prelaunch_open()) {
        return;
    }
    mInputActive = true;
    mHistoryPos = -1;
    mStickToBottom = true;
    if (mConsole != nullptr) {
        mConsole->SetAttribute("open", "");
    }
    if (mOutput != nullptr) {
        mOutput->SetAttribute("open", "");
    }
    if (mInput != nullptr) {
        mInput->SetValue("");
    }
    Document::show();
    focus();
}

bool CommandConsole::focus() {
    if (mInputActive && mInput != nullptr) {
        aurora::rmlui::set_input_type(aurora::rmlui::InputType::Text);
        return mInput->Focus(true);
    }
    return false;
}

void CommandConsole::hide(bool close) {
    close_input();
    if (close) {
        Document::hide(true);
    } else if (!has_onscreen_messages()) {
        Document::hide(false);
    }

    if (auto* doc = top_document()) {
        doc->focus();
    }
}

bool CommandConsole::visible() const {
    return mInputActive && Document::visible();
}

bool CommandConsole::active() const {
    return mInputActive && Document::active();
}

void CommandConsole::close_input() {
    if (!mInputActive) {
        return;
    }
    mInputActive = false;
    mHistoryPos = -1;
    if (mConsole != nullptr) {
        mConsole->RemoveAttribute("open");
    }
    if (mInput != nullptr) {
        mInput->SetValue("");
        mInput->Blur();
    }
    if (mOutput != nullptr) {
        mOutput->RemoveAttribute("open");
    }
}

void CommandConsole::execute_from_input() {
    if (mInput == nullptr) {
        return;
    }
    const Rml::String value = mInput->GetValue();
    hide(false);
    if (!value.empty()) {
        runCommand(value, mState, [this](std::string text) { append_message(std::move(text)); });
    }
}

void CommandConsole::navigate_history(int direction) {
    if (mState.history.empty() || mInput == nullptr) {
        return;
    }
    const int prev = mHistoryPos;
    if (direction < 0) {
        if (mHistoryPos == -1) {
            mHistoryPos = (int)mState.history.size() - 1;
        } else if (mHistoryPos > 0) {
            --mHistoryPos;
        }
    } else {
        if (mHistoryPos != -1 && ++mHistoryPos >= (int)mState.history.size()) {
            mHistoryPos = -1;
        }
    }
    if (prev != mHistoryPos) {
        const char* str = mHistoryPos >= 0 ? mState.history[mHistoryPos].c_str() : "";
        mInput->SetValue(str);
        const int end = static_cast<int>(Rml::StringUtilities::LengthUTF8(mInput->GetValue()));
        mInput->SetSelectionRange(end, end);
    }
}

void CommandConsole::scroll_messages(int pages) {
    if (mOutput == nullptr) {
        return;
    }
    const float bottom = std::max(0.0f, mOutput->GetScrollHeight() - mOutput->GetClientHeight());
    const float pageHeight = std::max(1.0f, mOutput->GetClientHeight() * 0.9f);
    const float scrollTop =
        std::clamp(mOutput->GetScrollTop() + static_cast<float>(pages) * pageHeight, 0.0f, bottom);
    mOutput->SetScrollTop(scrollTop);
    mStickToBottom = scrollTop >= bottom - 4.0f;
}

void CommandConsole::append_message(std::string text) {
    auto* element = append(mOutput, "line");
    if (element != nullptr) {
        element->SetClass("cmd", is_command(text));
        append_text(element, text);
    }

    const auto now = clock::now();
    mMessages.push_back({
        .text = std::move(text),
        .element = element,
        .fadeAt = now + kMessageDuration - kFadeDuration,
        .hideAt = now + kMessageDuration,
    });

    while (mMessages.size() > kMaxMessageHistory) {
        if (auto* oldElement = mMessages.front().element; oldElement != nullptr) {
            if (auto* parent = oldElement->GetParentNode()) {
                parent->RemoveChild(oldElement);
            }
        }
        mMessages.pop_front();
    }
    limit_visible_messages();
    if (mConsole != nullptr) {
        mConsole->RemoveAttribute("fading");
    }

    if (getSettings().backend.enableAdvancedSettings && !is_prelaunch_open() &&
        !Document::visible())
    {
        Document::show();
    }
}

void CommandConsole::limit_visible_messages() {
    std::size_t visibleCount = 0;
    for (auto it = mMessages.rbegin(); it != mMessages.rend(); ++it) {
        if (it->expired) {
            continue;
        }
        if (++visibleCount <= kMaxVisibleLines) {
            continue;
        }
        it->expired = true;
        if (it->element != nullptr) {
            it->element->SetAttribute("expired", "");
        }
    }
}

void CommandConsole::update_message_lifetimes() {
    const auto now = clock::now();
    for (auto& message : mMessages) {
        if (!message.fading && now >= message.fadeAt) {
            message.fading = true;
            if (message.element != nullptr) {
                message.element->SetAttribute("fading", "");
            }
        }
        if (!message.expired && now >= message.hideAt) {
            message.expired = true;
            if (message.element != nullptr) {
                message.element->SetAttribute("expired", "");
            }
        }
    }

    const bool hasVisibleMessages = has_onscreen_messages();
    const bool hasOpaqueMessages = std::ranges::any_of(
        mMessages, [](const auto& message) { return !message.fading && !message.expired; });
    if (mConsole != nullptr) {
        if (hasVisibleMessages && !hasOpaqueMessages) {
            mConsole->SetAttribute("fading", "");
        } else {
            mConsole->RemoveAttribute("fading");
        }
    }
}

bool CommandConsole::has_onscreen_messages() const {
    return std::ranges::any_of(mMessages, [](const auto& message) { return !message.expired; });
}

}  // namespace dusk::ui
