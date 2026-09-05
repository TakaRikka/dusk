#pragma once

#include "document.hpp"
#include "dusk/commands.hpp"

#include <chrono>
#include <deque>
#include <string>

namespace Rml {
class ElementFormControlInput;
}

namespace dusk::ui {

class CommandConsole : public Document {
public:
    CommandConsole();

    void update() override;
    void show() override;
    bool focus() override;
    void hide(bool close) override;
    bool visible() const override;
    bool active() const override;
    bool permanent() const override { return true; }

    bool input_active() const { return mInputActive; }

private:
    struct MessageLine {
        std::string text;
        Rml::Element* element = nullptr;
        clock::time_point fadeAt;
        clock::time_point hideAt;
        bool fading = false;
        bool expired = false;
    };

    static constexpr auto kMessageDuration = std::chrono::seconds{6};
    static constexpr auto kFadeDuration = std::chrono::milliseconds{800};
    static constexpr std::size_t kMaxVisibleLines = 24;
    static constexpr std::size_t kMaxMessageHistory = 500;

    Rml::Element* mConsole = nullptr;
    Rml::Element* mOutput = nullptr;
    Rml::ElementFormControlInput* mInput = nullptr;

    std::deque<MessageLine> mMessages;
    int mHistoryPos = -1;
    bool mInputActive = false;
    bool mStickToBottom = true;

    CommandState mState;

    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

    void append_message(std::string text);
    void close_input();
    void execute_from_input();
    bool has_onscreen_messages() const;
    void limit_visible_messages();
    void navigate_history(int direction);
    void scroll_messages(int pages);
    void update_message_lifetimes();
};

}  // namespace dusk::ui
