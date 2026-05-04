#pragma once

#include "select_button.hpp"

#include <RmlUi/Config/Config.h>

namespace dusk::ui {

class BaseStringButton : public BaseControlledSelectButton {
public:
    struct Props {
        Rml::String key;
        Rml::String type = "text";
        int maxLength = -1;
    };

    BaseStringButton(Rml::Element* parent, Props props);
    void update() override;
    void start_editing();
    void request_stop_editing(bool commit, bool refocusRoot);

protected:
    bool handle_nav_command(NavCommand cmd) override;
    virtual void set_value(Rml::String value) = 0;
    virtual Rml::String input_value() { return format_value(); }

private:
    void focus_input();
    void stop_editing(bool commit = true, bool refocusRoot = false);

    Rml::ElementFormControlInput* mInputElem = nullptr;
    std::vector<std::unique_ptr<ScopedEventListener> > mInputListeners;
    Rml::String mType;
    int mMaxLength;
    int mPendingInputFocusFrames = 0;
    bool mPendingStopEditing = false;
    bool mPendingCommit = true;
    bool mPendingRefocusRoot = false;
};

class StringButton : public BaseStringButton {
public:
    struct Props {
        Rml::String key;
        std::function<Rml::String()> getValue;
        std::function<void(Rml::String)> setValue;
        int maxLength = -1;
    };

    StringButton(Rml::Element* parent, Props props);

protected:
    Rml::String format_value() override { return mGetValue(); }
    void set_value(Rml::String value) override {
        if (mSetValue) {
            mSetValue(std::move(value));
        }
    }

private:
    std::function<Rml::String()> mGetValue;
    std::function<void(Rml::String)> mSetValue;
};

}  // namespace dusk::ui
