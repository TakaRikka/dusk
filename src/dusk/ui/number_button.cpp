#include "number_button.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include <charconv>
#include <fmt/format.h>

namespace dusk::ui {

NumberButton::NumberButton(Rml::Element* parent, Props props)
    : BaseStringButton(parent, {.key = std::move(props.key), .type = "number"}),
      mGetValue(std::move(props.getValue)), mSetValue(std::move(props.setValue)),
      mIsDisabled(std::move(props.isDisabled)), mIsModified(std::move(props.isModified)),
      mMin(props.min), mMax(props.max), mStep(props.step), mPrefix(std::move(props.prefix)),
      mSuffix(std::move(props.suffix)) {}

bool NumberButton::modified() const {
    if (mIsModified) {
        return mIsModified();
    }
    return BaseStringButton::modified();
}

bool NumberButton::disabled() const {
    if (mIsDisabled) {
        return mIsDisabled();
    }
    return BaseStringButton::disabled();
}

Rml::String NumberButton::format_value() {
    return fmt::format("{}{}{}", mPrefix, mGetValue(), mSuffix);
}

Rml::String NumberButton::input_value() {
    return fmt::to_string(mGetValue());
}

void NumberButton::set_value(Rml::String value) {
    if (!mSetValue) {
        return;
    }

    int parsedValue = 0;
    const char* begin = value.data();
    const char* end = begin + value.size();
    const auto result = std::from_chars(begin, end, parsedValue);
    if (result.ec != std::errc() || result.ptr != end) {
        return;
    }

    mSetValue(std::clamp(parsedValue, mMin, mMax));
}

bool NumberButton::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Left || cmd == NavCommand::Right) {
        const int newValue = std::clamp(
            mGetValue() + (cmd == NavCommand::Right ? mStep : -mStep), mMin, mMax);
        if (newValue != mGetValue()) {
            mSetValue(newValue);
            mDoAud_seStartMenu(Z2SE_SY_NAME_CURSOR);
        }
        return true;
    }
    return BaseStringButton::handle_nav_command(cmd);
}

}  // namespace dusk::ui