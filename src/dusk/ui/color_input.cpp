#include "color_input.hpp"

#include "button.hpp"
#include "input.hpp"
#include "nav_group.hpp"

#include "m_Do/m_Do_audio.h"

#include <SDL3/SDL_clipboard.h>
#include <fmt/format.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <optional>
#include <string_view>

namespace dusk::ui {
namespace {

// These dimensions must match the controls in res/rml/popover.rcss.
constexpr float kSvWidthDp = 240.0f;
constexpr float kSvHeightDp = 150.0f;
constexpr float kBarHeightDp = 14.0f;
constexpr float kCursorDp = 14.0f;

constexpr size_t kHistoryLimit = 8;
constexpr int kSwatchColumns = 8;
constexpr float kSvAnalogRate = 0.7f;
constexpr float kHueAnalogRate = 240.0f;
constexpr float kAlphaAnalogRate = 0.7f;
constexpr float kSvNudge = 1.0f / 255.0f;
constexpr float kHueNudge = 1.0f;
constexpr float kAlphaNudge = 1.0f / 255.0f;

std::vector<Rml::String> sColorHistory;

bool directional_command(NavCommand command) {
    return command == NavCommand::Up || command == NavCommand::Down ||
           command == NavCommand::Left || command == NavCommand::Right;
}

float analog_response(float value) {
    return value * std::abs(value);
}

class AdjustmentRegion : public Component {
public:
    struct Props {
        std::function<void()> begin;
        std::function<void()> cancel;
        std::function<void(NavCommand)> nudge;
        std::function<void(float, float, float)> adjust;
    };

    AdjustmentRegion(Rml::Element* root, Props props) : Component{root}, mProps{std::move(props)} {
        listen(mRoot, Rml::EventId::Keydown, [this](Rml::Event& event) {
            const auto command = map_nav_event(event);
            if (command == NavCommand::Confirm) {
                if (mActive) {
                    finish();
                } else {
                    if (mProps.begin) {
                        mProps.begin();
                    }
                    mActive = true;
                    mAnalogReady = mAxis.x == 0.0f && mAxis.y == 0.0f;
                    mRoot->SetClass("adjusting", true);
                }
                event.StopPropagation();
                return;
            }
            if (command == NavCommand::Cancel && mActive) {
                if (mProps.cancel) {
                    mProps.cancel();
                }
                finish();
                event.StopPropagation();
                return;
            }
            if (directional_command(command) && (mActive || mAwaitingNeutral)) {
                if (mActive && mAxis.x == 0.0f && mAxis.y == 0.0f && mProps.nudge) {
                    mProps.nudge(command);
                }
                event.StopPropagation();
            }
        });
        listen(mRoot, Rml::String{input::kNavAxisEvent}, [this](Rml::Event& event) {
            mAxis = {
                event.GetParameter<float>("x", 0.0f),
                event.GetParameter<float>("y", 0.0f),
            };
            if (mAxis.x == 0.0f && mAxis.y == 0.0f) {
                mAwaitingNeutral = false;
                mAnalogReady = true;
            }
            const float deltaSeconds = event.GetParameter<float>("dt", 0.0f);
            if (mActive && mAnalogReady && deltaSeconds > 0.0f && mProps.adjust) {
                mProps.adjust(mAxis.x, mAxis.y, deltaSeconds);
            }
        });
        listen(mRoot, Rml::EventId::Blur, [this](Rml::Event&) {
            if (mActive) {
                finish();
            }
        });
    }

private:
    void finish() {
        mActive = false;
        mAnalogReady = false;
        mAwaitingNeutral = mAxis.x != 0.0f || mAxis.y != 0.0f;
        mRoot->SetClass("adjusting", false);
    }

    Props mProps;
    Rml::Vector2f mAxis;
    bool mActive = false;
    bool mAnalogReady = false;
    bool mAwaitingNeutral = false;
};

const std::vector<Rml::String>& default_presets() {
    static const std::vector<Rml::String> presets = {
        "ff4d4d",
        "ff8a3d",
        "ffd43b",
        "61c454",
        "3dc7c2",
        "4d88ff",
        "8b5cf6",
        "e56aa6",
        "8b6a45",
        "ffffff",
        "888888",
        "202020",
    };
    return presets;
}

int hex_digit(char value) {
    if (value >= '0' && value <= '9') {
        return value - '0';
    }
    if (value >= 'a' && value <= 'f') {
        return value - 'a' + 10;
    }
    if (value >= 'A' && value <= 'F') {
        return value - 'A' + 10;
    }
    return -1;
}

std::optional<Rml::Colourb> parse_color(std::string_view value, bool alpha) {
    if (value.starts_with('#')) {
        value.remove_prefix(1);
    }
    if (value.size() != 6 && (!alpha || value.size() != 8)) {
        return std::nullopt;
    }

    uint8_t channels[4] = {0, 0, 0, 255};
    for (size_t i = 0; i < value.size() / 2; ++i) {
        const int high = hex_digit(value[i * 2]);
        const int low = hex_digit(value[i * 2 + 1]);
        if (high < 0 || low < 0) {
            return std::nullopt;
        }
        channels[i] = static_cast<uint8_t>((high << 4) | low);
    }
    return Rml::Colourb{channels[0], channels[1], channels[2], channels[3]};
}

Rml::String normalize_value(Rml::String value, bool alpha) {
    if (value == "rainbow") {
        return value;
    }
    const auto color = parse_color(value, alpha);
    if (!color.has_value()) {
        return value;
    }
    if (alpha && color->alpha != 255) {
        return fmt::format(
            "{:02x}{:02x}{:02x}{:02x}", color->red, color->green, color->blue, color->alpha);
    }
    return fmt::format("{:02x}{:02x}{:02x}", color->red, color->green, color->blue);
}

Rml::String css_color(Rml::Colourb color) {
    return fmt::format("rgba({},{},{},{})", color.red, color.green, color.blue, color.alpha);
}

Rml::String format_color(Rml::Colourb color, bool hex, bool alpha) {
    if (hex) {
        if (alpha && color.alpha != 255) {
            return fmt::format(
                "#{:02X}{:02X}{:02X}{:02X}", color.red, color.green, color.blue, color.alpha);
        }
        return fmt::format("#{:02X}{:02X}{:02X}", color.red, color.green, color.blue);
    }
    if (alpha) {
        return fmt::format("rgba({}, {}, {}, {:.0f}%)", color.red, color.green, color.blue,
            static_cast<float>(color.alpha) / 255.0f * 100.0f);
    }
    return fmt::format("rgb({}, {}, {})", color.red, color.green, color.blue);
}

Rml::Colourb hsv_to_rgb(float hue, float sat, float val, float alpha) {
    hue -= 360.0f * std::floor(hue / 360.0f);
    const float chroma = val * sat;
    const float intermediate = chroma * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
    const float offset = val - chroma;
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    switch (static_cast<int>(hue / 60.0f) % 6) {
    case 0:
        red = chroma;
        green = intermediate;
        break;
    case 1:
        red = intermediate;
        green = chroma;
        break;
    case 2:
        green = chroma;
        blue = intermediate;
        break;
    case 3:
        green = intermediate;
        blue = chroma;
        break;
    case 4:
        red = intermediate;
        blue = chroma;
        break;
    case 5:
    default:
        red = chroma;
        blue = intermediate;
        break;
    }
    const auto toByte = [](float value) {
        return static_cast<Rml::byte>(std::clamp(value, 0.0f, 1.0f) * 255.0f + 0.5f);
    };
    return {toByte(red + offset), toByte(green + offset), toByte(blue + offset), toByte(alpha)};
}

void rgb_to_hsv(Rml::Colourb color, float& hue, float& sat, float& val) {
    const float red = static_cast<float>(color.red) / 255.0f;
    const float green = static_cast<float>(color.green) / 255.0f;
    const float blue = static_cast<float>(color.blue) / 255.0f;
    const float maxChannel = std::max({red, green, blue});
    const float minChannel = std::min({red, green, blue});
    const float delta = maxChannel - minChannel;

    val = maxChannel;
    sat = maxChannel > 0.0f ? delta / maxChannel : 0.0f;
    if (delta <= 0.0f) {
        hue = 0.0f;
        return;
    }
    if (maxChannel == red) {
        hue = 60.0f * std::fmod((green - blue) / delta + 6.0f, 6.0f);
    } else if (maxChannel == green) {
        hue = 60.0f * ((blue - red) / delta + 2.0f);
    } else {
        hue = 60.0f * ((red - green) / delta + 4.0f);
    }
}

void apply_swatch(Rml::Element* element, const Rml::String& value, bool alpha) {
    element->RemoveProperty("background-color");
    element->RemoveProperty("decorator");
    element->SetClass("empty", false);
    element->SetClass("rainbow", value == "rainbow");
    if (value == "rainbow") {
        element->SetProperty("decorator", "linear-gradient(90deg, #ff4d4d, #ffd43b, #61c454, "
                                          "#3dc7c2, #4d88ff, #8b5cf6, #e56aa6)");
        return;
    }
    const auto color = parse_color(value, alpha);
    if (!color.has_value()) {
        element->SetClass("empty", true);
        return;
    }
    element->SetProperty("background-color", css_color(*color));
}

void remember_color(Rml::String value, bool alpha) {
    value = normalize_value(std::move(value), alpha);
    if (value != "rainbow" && !parse_color(value, alpha).has_value()) {
        return;
    }
    std::erase(sColorHistory, value);
    sColorHistory.insert(sColorHistory.begin(), std::move(value));
    if (sColorHistory.size() > kHistoryLimit) {
        sColorHistory.resize(kHistoryLimit);
    }
}

}  // namespace

ColorInput::ColorInput(Rml::Element* parent, Props props)
    : BaseControlledSelectButton{parent, {.key = props.key, .submit = false}},
      mProps{std::move(props)} {
    mRoot->SetClass("color-input", true);
    mSwatch = append(mRoot, "color-swatch");
    refresh_swatch();
}

ColorInput::~ColorInput() {
    if (mPicker != nullptr) {
        mPicker->on_close(nullptr);
        mPicker->on_focus(nullptr);
        mPicker->dismiss();
    }
}

void ColorInput::update() {
    refresh_swatch();
    if (mPickerNavigation != nullptr) {
        mPickerNavigation->update();
    }
    BaseControlledSelectButton::update();
}

bool ColorInput::modified() const {
    return mProps.isModified ? mProps.isModified() : BaseControlledSelectButton::modified();
}

bool ColorInput::disabled() const {
    return mProps.isDisabled ? mProps.isDisabled() : BaseControlledSelectButton::disabled();
}

Rml::String ColorInput::format_value() {
    const Rml::String value = mProps.getValue ? mProps.getValue() : "";
    if (value == "rainbow") {
        return "Rainbow";
    }
    const auto color = parse_color(value, mProps.alpha);
    if (color.has_value()) {
        return format_color(*color, mHexFormat, mProps.alpha);
    }
    return value.empty() ? "Default" : value;
}

bool ColorInput::handle_nav_command(NavCommand cmd) {
    if (cmd == NavCommand::Confirm) {
        toggle_picker();
        return true;
    }
    return false;
}

void ColorInput::toggle_picker() {
    if (mPicker != nullptr) {
        mPicker->dismiss();
        return;
    }

    mInitialValue = mProps.getValue ? mProps.getValue() : "";
    mInitialValueRemembered = false;
    const auto initialColor = parse_color(mInitialValue, mProps.alpha);
    const Rml::Colourb color = initialColor.value_or(Rml::Colourb{128, 128, 128, 255});
    rgb_to_hsv(color, mHue, mSat, mVal);
    mAlpha = static_cast<float>(color.alpha) / 255.0f;

    auto picker = std::make_unique<Popover>(mRoot, mProps.side, "color-picker");
    mPicker = picker.get();
    mPicker->on_close([this] {
        if (mInitialValueRemembered && mProps.getValue) {
            remember_color(mProps.getValue(), mProps.alpha);
        }
        mPicker = nullptr;
        mPickerNavigation.reset();
        mPickerListeners.clear();
        mSvArea = nullptr;
        mSvCursor = nullptr;
        mHueBar = nullptr;
        mHueCursor = nullptr;
        mAlphaBar = nullptr;
        mAlphaCursor = nullptr;
        mPickerValue = nullptr;
    });
    build_picker();
    refresh_picker();
    mPicker->on_focus(
        [this] { return mPickerNavigation != nullptr && mPickerNavigation->focus(); });
    push_document(std::move(picker));
}

void ColorInput::build_picker() {
    auto* body = mPicker->body();
    mPickerNavigation = std::make_unique<NavGroup>(
        body, NavGroup::Props{
                  .layout = NavGroup::Layout::Vertical,
                  .horizontalBoundary = NavGroup::Boundary::Stop,
                  .verticalBoundary = NavGroup::Boundary::Stop,
              });

    mSvArea = append(body, "color-sv");
    mSvCursor = append(mSvArea, "color-cursor");
    mPickerNavigation->add_existing_item<AdjustmentRegion>(
        mSvArea, AdjustmentRegion::Props{
                     .begin = [this] { begin_adjustment(); },
                     .cancel = [this] { cancel_adjustment(); },
                     .nudge = [this](NavCommand direction) { nudge_sv(direction); },
                     .adjust = [this](float x, float y,
                                   float deltaSeconds) { adjust_sv(x, y, deltaSeconds); },
                 });

    mHueBar = append(body, "color-hue");
    mHueCursor = append(mHueBar, "color-cursor");
    mPickerNavigation->add_existing_item<AdjustmentRegion>(mHueBar,
        AdjustmentRegion::Props{
            .begin = [this] { begin_adjustment(); },
            .cancel = [this] { cancel_adjustment(); },
            .nudge = [this](NavCommand direction) { nudge_hue(direction); },
            .adjust = [this](float x, float, float deltaSeconds) { adjust_hue(x, deltaSeconds); },
        });

    if (mProps.alpha) {
        mAlphaBar = append(body, "color-alpha");
        mAlphaCursor = append(mAlphaBar, "color-cursor");
        mPickerNavigation->add_existing_item<AdjustmentRegion>(
            mAlphaBar, AdjustmentRegion::Props{
                           .begin = [this] { begin_adjustment(); },
                           .cancel = [this] { cancel_adjustment(); },
                           .nudge = [this](NavCommand direction) { nudge_alpha(direction); },
                           .adjust = [this](float x, float,
                                         float deltaSeconds) { adjust_alpha(x, deltaSeconds); },
                       });
    }

    add_presets(*mPickerNavigation);
    add_history(*mPickerNavigation);

    auto* footer = append(body, "color-footer");
    auto& footerNavigation = mPickerNavigation->add_existing_item<NavGroup>(
        footer, NavGroup::Props{
                    .layout = NavGroup::Layout::Horizontal,
                    .horizontalBoundary = NavGroup::Boundary::Stop,
                    .verticalBoundary = NavGroup::Boundary::Bubble,
                });
    const auto formatLabel = [this] {
        return mHexFormat ? "Hex" : (mProps.alpha ? "RGBA" : "RGB");
    };
    auto& formatButton = footerNavigation.add_item<Button>(formatLabel());
    formatButton.on_pressed([this, formatButtonPtr = &formatButton, formatLabel] {
        mHexFormat = !mHexFormat;
        formatButtonPtr->set_text(formatLabel());
        refresh_picker();
    });

    footerNavigation.add_item<Button>("Default").on_pressed([this] { commit_value(""); });

    auto& valueButton = footerNavigation.add_item<Button>("", "color-value");
    mPickerValue = valueButton.root();
    mPickerValue->SetAttribute("title", "Copy color");
    valueButton.on_pressed([this] {
        SDL_SetClipboardText(format_color(current_color(), mHexFormat, mProps.alpha).c_str());
        push_toast({
            .type = "info",
            .title = "Color",
            .content = "Copied to clipboard",
            .duration = std::chrono::seconds{2},
        });
    });

    const auto track = [this](Rml::Element* element, float widthDp, float heightDp,
                           std::function<void(float, float)> apply) {
        const auto handler = [this, element, widthDp, heightDp, apply = std::move(apply)](
                                 Rml::Event& event) {
            const float dpRatio = element->GetContext()->GetDensityIndependentPixelRatio();
            const auto origin = element->GetAbsoluteOffset(Rml::BoxArea::Border);
            const float x =
                (event.GetParameter<float>("mouse_x", 0.0f) - origin.x) / (widthDp * dpRatio);
            const float y =
                (event.GetParameter<float>("mouse_y", 0.0f) - origin.y) / (heightDp * dpRatio);
            element->Focus(true);
            apply(std::clamp(x, 0.0f, 1.0f), std::clamp(y, 0.0f, 1.0f));
            commit_color();
        };
        mPickerListeners.push_back(
            std::make_unique<ScopedEventListener>(element, Rml::EventId::Mousedown, handler));
        mPickerListeners.push_back(
            std::make_unique<ScopedEventListener>(element, Rml::EventId::Drag, handler));
    };

    track(mSvArea, kSvWidthDp, kSvHeightDp, [this](float x, float y) {
        mSat = x;
        mVal = 1.0f - y;
    });
    track(mHueBar, kSvWidthDp, kBarHeightDp,
        [this](float x, float) { mHue = std::min(x * 360.0f, 359.9f); });
    if (mAlphaBar != nullptr) {
        track(mAlphaBar, kSvWidthDp, kBarHeightDp, [this](float x, float) { mAlpha = x; });
    }
}

void ColorInput::add_presets(NavGroup& navigation) {
    append_text(append(navigation.root(), "color-heading"), "Presets");
    auto* gridElement = append(navigation.root(), "color-presets");
    auto& grid = navigation.add_existing_item<NavGroup>(
        gridElement, NavGroup::Props{
                         .layout = NavGroup::Layout::Grid,
                         .columns = kSwatchColumns,
                         .horizontalBoundary = NavGroup::Boundary::Stop,
                         .verticalBoundary = NavGroup::Boundary::Bubble,
                     });
    const auto& presets = mProps.presets.empty() ? default_presets() : mProps.presets;
    for (const auto& preset : presets) {
        add_swatch_button(grid, preset);
    }
}

void ColorInput::add_history(NavGroup& navigation) {
    const bool supportsRainbow =
        std::ranges::any_of(mProps.presets, [](const auto& preset) { return preset == "rainbow"; });
    std::vector<Rml::String> compatibleHistory;
    for (const auto& value : sColorHistory) {
        if (parse_color(value, mProps.alpha).has_value() || (value == "rainbow" && supportsRainbow))
        {
            compatibleHistory.push_back(value);
        }
    }
    if (compatibleHistory.empty()) {
        return;
    }
    append_text(append(navigation.root(), "color-heading"), "Recent");
    auto* gridElement = append(navigation.root(), "color-history");
    auto& grid = navigation.add_existing_item<NavGroup>(
        gridElement, NavGroup::Props{
                         .layout = NavGroup::Layout::Grid,
                         .columns = kSwatchColumns,
                         .horizontalBoundary = NavGroup::Boundary::Stop,
                         .verticalBoundary = NavGroup::Boundary::Bubble,
                     });
    for (const auto& value : compatibleHistory) {
        add_swatch_button(grid, value);
    }
}

void ColorInput::add_swatch_button(NavGroup& navigation, const Rml::String& value) {
    auto& button = navigation.add_item<Button>("");
    button.root()->SetClass("color-swatch-button", true);
    button.root()->SetInnerRML("");
    auto* chip = append(button.root(), "color-chip");
    apply_swatch(chip, value, mProps.alpha);
    Rml::String title = value == "rainbow" ? "Rainbow" : value;
    if (const auto color = parse_color(value, mProps.alpha)) {
        title = format_color(*color, true, mProps.alpha);
    }
    button.root()->SetAttribute("title", title);
    button.on_pressed([this, value] {
        mDoAud_seStartMenu(kSoundItemChange);
        commit_value(value);
    });
}

void ColorInput::begin_adjustment() {
    mAdjustmentStartValue = mProps.getValue ? mProps.getValue() : "";
    mAdjustmentStartHue = mHue;
    mAdjustmentStartSat = mSat;
    mAdjustmentStartVal = mVal;
    mAdjustmentStartAlpha = mAlpha;
}

void ColorInput::cancel_adjustment() {
    mHue = mAdjustmentStartHue;
    mSat = mAdjustmentStartSat;
    mVal = mAdjustmentStartVal;
    mAlpha = mAdjustmentStartAlpha;
    commit_value(mAdjustmentStartValue);
}

void ColorInput::adjust_sv(float x, float y, float deltaSeconds) {
    const float sat =
        std::clamp(mSat + analog_response(x) * kSvAnalogRate * deltaSeconds, 0.0f, 1.0f);
    const float val =
        std::clamp(mVal - analog_response(y) * kSvAnalogRate * deltaSeconds, 0.0f, 1.0f);
    if (sat == mSat && val == mVal) {
        return;
    }
    mSat = sat;
    mVal = val;
    commit_color();
}

void ColorInput::adjust_hue(float x, float deltaSeconds) {
    const float hue =
        std::clamp(mHue + analog_response(x) * kHueAnalogRate * deltaSeconds, 0.0f, 359.9f);
    if (hue == mHue) {
        return;
    }
    mHue = hue;
    commit_color();
}

void ColorInput::adjust_alpha(float x, float deltaSeconds) {
    const float alpha =
        std::clamp(mAlpha + analog_response(x) * kAlphaAnalogRate * deltaSeconds, 0.0f, 1.0f);
    if (alpha == mAlpha) {
        return;
    }
    mAlpha = alpha;
    commit_color();
}

void ColorInput::nudge_sv(NavCommand direction) {
    const float sat = std::clamp(mSat + (direction == NavCommand::Right   ? kSvNudge :
                                            direction == NavCommand::Left ? -kSvNudge :
                                                                            0.0f),
        0.0f, 1.0f);
    const float val = std::clamp(mVal + (direction == NavCommand::Up      ? kSvNudge :
                                            direction == NavCommand::Down ? -kSvNudge :
                                                                            0.0f),
        0.0f, 1.0f);
    if (sat == mSat && val == mVal) {
        return;
    }
    mSat = sat;
    mVal = val;
    mDoAud_seStartMenu(kSoundItemChange);
    commit_color();
}

void ColorInput::nudge_hue(NavCommand direction) {
    const float hue = std::clamp(mHue + (direction == NavCommand::Right   ? kHueNudge :
                                            direction == NavCommand::Left ? -kHueNudge :
                                                                            0.0f),
        0.0f, 359.9f);
    if (hue == mHue) {
        return;
    }
    mHue = hue;
    mDoAud_seStartMenu(kSoundItemChange);
    commit_color();
}

void ColorInput::nudge_alpha(NavCommand direction) {
    const float alpha = std::clamp(mAlpha + (direction == NavCommand::Right   ? kAlphaNudge :
                                                direction == NavCommand::Left ? -kAlphaNudge :
                                                                                0.0f),
        0.0f, 1.0f);
    if (alpha == mAlpha) {
        return;
    }
    mAlpha = alpha;
    mDoAud_seStartMenu(kSoundItemChange);
    commit_color();
}

void ColorInput::commit_color() {
    const auto color = current_color();
    Rml::String value;
    if (mProps.alpha && color.alpha != 255) {
        value = fmt::format(
            "{:02x}{:02x}{:02x}{:02x}", color.red, color.green, color.blue, color.alpha);
    } else {
        value = fmt::format("{:02x}{:02x}{:02x}", color.red, color.green, color.blue);
    }
    const bool changed = !mProps.getValue || mProps.getValue() != value;
    if (changed) {
        remember_initial_value();
        if (mProps.setValue) {
            mProps.setValue(std::move(value));
        }
        refresh_swatch();
    }
    refresh_picker();
}

void ColorInput::commit_value(Rml::String value) {
    remember_initial_value();
    value = normalize_value(std::move(value), mProps.alpha);
    if (const auto color = parse_color(value, mProps.alpha)) {
        rgb_to_hsv(*color, mHue, mSat, mVal);
        mAlpha = static_cast<float>(color->alpha) / 255.0f;
    }
    if (mProps.setValue) {
        mProps.setValue(std::move(value));
    }
    refresh_swatch();
    refresh_picker();
}

void ColorInput::remember_initial_value() {
    if (mInitialValueRemembered) {
        return;
    }
    remember_color(mInitialValue, mProps.alpha);
    mInitialValueRemembered = true;
}

void ColorInput::refresh_swatch() {
    const Rml::String value = mProps.getValue ? mProps.getValue() : "";
    if (mHasDisplayedValue && value == mDisplayedValue) {
        return;
    }
    mHasDisplayedValue = true;
    mDisplayedValue = value;
    apply_swatch(mSwatch, value, mProps.alpha);
}

void ColorInput::refresh_picker() {
    if (mPicker == nullptr) {
        return;
    }

    const float dpRatio = mSvArea->GetContext()->GetDensityIndependentPixelRatio();
    const auto place = [dpRatio](Rml::Element* cursor, float xDp, float yDp) {
        cursor->SetProperty(Rml::PropertyId::Left,
            Rml::Property{(xDp - kCursorDp / 2.0f) * dpRatio, Rml::Unit::PX});
        cursor->SetProperty(
            Rml::PropertyId::Top, Rml::Property{(yDp - kCursorDp / 2.0f) * dpRatio, Rml::Unit::PX});
    };

    const Rml::Colourb hueColor = hsv_to_rgb(mHue, 1.0f, 1.0f, 1.0f);
    const Rml::Colourb color = current_color();
    const Rml::Colourb opaque{color.red, color.green, color.blue, 255};

    mSvArea->SetProperty(
        "decorator", fmt::format("linear-gradient(180deg, rgba(0,0,0,0), #000000), "
                                 "linear-gradient(90deg, #ffffff, {})",
                         css_color(hueColor)));
    place(mSvCursor, mSat * kSvWidthDp, (1.0f - mVal) * kSvHeightDp);
    place(mHueCursor, mHue / 360.0f * kSvWidthDp, kBarHeightDp / 2.0f);
    if (mAlphaBar != nullptr) {
        mAlphaBar->SetProperty(
            "decorator", fmt::format("linear-gradient(90deg, rgba({0},{1},{2},0), {3})", opaque.red,
                             opaque.green, opaque.blue, css_color(opaque)));
        place(mAlphaCursor, mAlpha * kSvWidthDp, kBarHeightDp / 2.0f);
    }
    mPickerValue->SetInnerRML(format_color(color, mHexFormat, mProps.alpha));
}

Rml::Colourb ColorInput::current_color() const {
    return hsv_to_rgb(mHue, mSat, mVal, mAlpha);
}

}  // namespace dusk::ui
