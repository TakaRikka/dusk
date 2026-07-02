#include "touch_controls.hpp"
#include "touch_controls_common.hpp"

#include <aurora/rmlui.hpp>
#include <dolphin/pad.h>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <optional>

#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "dusk/action_bindings.h"
#include "dusk/menu_pointer.h"
#include "dusk/settings.h"
#include "dusk/touch_camera.h"
#include "f_op/f_op_overlap_mng.h"
#include "icon_provider.hpp"
#include "m_Do/m_Do_graphic.h"
#include "ui.hpp"

namespace dusk::ui {
namespace {

constexpr u32 kPort = PAD_CHAN0;
constexpr float kStickRadiusDp = 62.f;
constexpr float kStickKnobRadiusDp = 24.f;
constexpr float kAnalogZoneTopDp = 92.f;
constexpr float kAnalogZoneBottomDp = 30.f;
constexpr float kLeftZoneWidth = 0.46f;
constexpr float kRightZoneStart = 0.52f;
constexpr u8 kTriggerAnalog = 180;
constexpr auto kLDoubleTapWindow = std::chrono::milliseconds(300);
constexpr auto kHoldActionDuration = std::chrono::milliseconds(450);
constexpr float kFaceIconTargetRatio = 0.76f;
constexpr float kPressedScale = 0.94f;
constexpr size_t kEquipTargetCount = 4;

std::array<EquipTarget, kEquipTargetCount> sEquipTargets{};
std::array<ControlOverride, static_cast<std::size_t>(Control::COUNT)> sControlOverrides{};
TouchControls* sTouchControls = nullptr;

struct ControlInfo {
    const char* id = nullptr;
    const char* iconId = nullptr;
    const char* oilId = nullptr;
    const char* oilFillId = nullptr;
    const char* countId = nullptr;
    u16 padButton = 0;
    std::optional<ActionBinds> tapAction;
    std::optional<ActionBinds> holdAction;
};

constexpr std::array<ControlInfo, static_cast<std::size_t>(Control::COUNT)> kControls = {{
    {
        .id = "button-a",
        .padButton = PAD_BUTTON_A,
    },
    {
        .id = "button-b",
        .iconId = "button-b-icon",
        .padButton = PAD_BUTTON_B,
    },
    {
        .id = "button-x",
        .iconId = "button-x-icon",
        .oilId = "button-x-oil",
        .oilFillId = "button-x-oil-fill",
        .countId = "button-x-count",
        .padButton = PAD_BUTTON_X,
    },
    {
        .id = "button-y",
        .iconId = "button-y-icon",
        .oilId = "button-y-oil",
        .oilFillId = "button-y-oil-fill",
        .countId = "button-y-count",
        .padButton = PAD_BUTTON_Y,
    },
    {
        .id = "button-z",
        .iconId = "z-midna-icon",
        .padButton = PAD_TRIGGER_Z,
    },
    {
        .id = "trigger-l",
    },
    {
        .id = "trigger-r",
    },
    {
        .id = "first-person",
        .tapAction = ActionBinds::FIRST_PERSON_CAMERA,
    },
    {
        .id = "items",
        .padButton = PAD_BUTTON_UP,
    },
    {
        .id = "collections",
        .padButton = PAD_BUTTON_START,
    },
    {
        .id = "map",
        .tapAction = ActionBinds::OPEN_MAP_SCREEN,
        .holdAction = ActionBinds::TOGGLE_MINIMAP,
    },
    {
        .id = "skip",
        .padButton = PAD_BUTTON_START,
    },
}};

constexpr const ControlInfo* control_info(Control control) noexcept {
    const auto index = static_cast<std::size_t>(control);
    return index < kControls.size() ? &kControls[index] : nullptr;
}

bool control_override_active(Control control) noexcept {
    const auto index = static_cast<std::size_t>(control);
    return index < sControlOverrides.size() && sControlOverrides[index] != ControlOverride::Default;
}

Rml::String touch_controls_document_source() {
    const auto fragment = touch_controls_rml_fragment();
    return Rml::String{R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/touch_controls.rcss" />
</head>
<body id="root">
    <touch-stick id="control-stick">
        <stick-ring />
        <stick-knob id="control-knob" />
    </touch-stick>
)RML"} + Rml::String{fragment.data(), fragment.size()} +
           Rml::String{R"RML(
</body>
</rml>
)RML"};
}

s8 stick_value(float value) noexcept {
    return static_cast<s8>(std::clamp(std::lround(value * 127.f), -127l, 127l));
}

bool player_attention_locked() noexcept {
    const auto* player = daPy_getPlayerActorClass();
    return player != nullptr && (player->checkAttentionLock() || player->checkEnemyAttentionLock());
}

bool hawkeye_active() noexcept {
    return dCamera_c::isAimActive() && dComIfGp_checkPlayerStatus0(0, 0x200000);
}

bool item_wheel_active() noexcept {
    return dMeter2Info_getWindowStatus() == 2;
}

bool fishing_controls_active() noexcept {
    const auto* player = daAlink_getAlinkActorClass();
    if (player == nullptr) {
        return false;
    }
    return player->checkCanoeFishingWaitAnime();
}

enum class StickOutput {
    MainStick,
    CStick,
};

StickOutput stick_output_mode() noexcept {
    if (fishing_controls_active() || hawkeye_active()) {
        return StickOutput::CStick;
    }
    return StickOutput::MainStick;
}

bool controls_available(bool allowItemWheel) noexcept {
    if (dComIfGp_getLinkPlayer() == nullptr) {
        return false;
    }

    const auto* fader = mDoGph_gInf_c::getFader();
    if (fader == nullptr || fader->getStatus() != JUTFader::Wait || mDoGph_gInf_c::isFade()) {
        return false;
    }

    const bool itemWheelActive = allowItemWheel && item_wheel_active();
    const auto heapLock = dComIfGp_isHeapLockFlag();
    if ((heapLock != 0 && heapLock != 5 && !(itemWheelActive && heapLock == 1)) ||
        (dComIfGp_isPauseFlag() && !itemWheelActive) || dComIfGp_getMesgStatus() != 0 ||
        dComIfGp_isEnableNextStage() || fopOvlpM_IsDoingReq())
    {
        return false;
    }

    return true;
}

Rml::Vector2f clamped_stick_delta(
    Rml::Vector2f start, Rml::Vector2f current, float stickRadius) noexcept {
    Rml::Vector2f delta = current - start;
    const float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    delta *= length > stickRadius ? stickRadius / length : 1.f;
    return delta;
}

struct FaceButtonState {
    std::string iconSource;
    uint64_t iconRevision = 0;
    bool visible = false;
    bool showIcon = false;
};

void release_rml_texture(const std::string& source) noexcept {
    if (!source.empty()) {
        Rml::ReleaseTexture(source);
    }
}

FaceButtonState override_button_state(Control control) {
    if (control_override_active(control)) {
        return {
            .iconSource = "",
            .visible = true,
            .showIcon = false,
        };
    }

    return {};
}

bool game_controls_suppressed() noexcept {
    return !controls_available(true) || dComIfGp_event_runCheck() ||
           (dComIfGp_getMsgObjectClass() != nullptr && dMsgObject_isTalkNowCheck());
}

FaceButtonState xy_button_state(Control control) {
    if (sControlOverrides[static_cast<size_t>(control)] != ControlOverride::Default) {
        return override_button_state(control);
    }
    if (game_controls_suppressed()) {
        return {};
    }

    const bool itemMode = dComIfGp_getLinkPlayer() != nullptr && daPy_py_c::checkNowWolf() == 0;
    const auto source = itemMode ? item_icon_source_for_button(control) : std::string();
    return {
        .iconSource = source,
        .visible = true,
        .showIcon = itemMode && !source.empty(),
    };
}

FaceButtonState z_button_state() {
    if (sControlOverrides[static_cast<size_t>(Control::Z)] != ControlOverride::Default) {
        return override_button_state(Control::Z);
    }
    if (game_controls_suppressed()) {
        return {};
    }

    const auto source = midna_icon_source();
    return {
        .iconSource = source,
        .iconRevision = midna_icon_revision(),
        .visible = true,
        .showIcon = !source.empty(),
    };
}

FaceButtonState b_button_state() {
    if (game_controls_suppressed()) {
        return {
            .iconSource = "",
            .visible = true,
            .showIcon = false,
        };
    }

    const auto source = item_icon_source_for_button(Control::B);
    return {
        .iconSource = source,
        .visible = true,
        .showIcon = dMeter2Info_isUseButton(METER2_USEBUTTON_B) && !source.empty(),
    };
}

void clear_equip_targets() noexcept {
    for (auto& target : sEquipTargets) {
        target.valid = false;
    }
}

void sync_equip_target(int slot, ControlRect rectDp, float widthRatio, float heightRatio,
    bool square = false) noexcept {
    if (slot < 0 || static_cast<size_t>(slot) >= sEquipTargets.size()) {
        return;
    }

    auto& target = sEquipTargets[slot];
    target.valid = false;

    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }

    const auto dimensions = context->GetDimensions();
    if (dimensions.x <= 0 || dimensions.y <= 0) {
        return;
    }

    const float scale = std::max(context->GetDensityIndependentPixelRatio(), 1.f);
    const Rml::Vector2f buttonPosition{rectDp.l * scale, rectDp.t * scale};
    const Rml::Vector2f buttonSize{rectDp.w * scale, rectDp.h * scale};
    if (buttonSize.x <= 0.f || buttonSize.y <= 0.f) {
        return;
    }

    Rml::Vector2f targetSize{buttonSize.x * widthRatio, buttonSize.y * heightRatio};
    if (square) {
        const float side = std::min(buttonSize.x, buttonSize.y) * widthRatio;
        targetSize = Rml::Vector2f{side, side};
    }
    const auto targetPosition = buttonPosition + (buttonSize - targetSize) * 0.5f;
    const float scaleX = mDoGph_gInf_c::getWidthF() / static_cast<float>(dimensions.x);
    const float scaleY = mDoGph_gInf_c::getHeightF() / static_cast<float>(dimensions.y);

    target.left = mDoGph_gInf_c::getMinXF() + targetPosition.x * scaleX;
    target.top = mDoGph_gInf_c::getMinYF() + targetPosition.y * scaleY;
    target.width = targetSize.x * scaleX;
    target.height = targetSize.y * scaleY;
    target.valid = true;
}

}  // namespace

bool get_equip_target(int slot, EquipTarget& target) noexcept {
    if (slot < 0 || static_cast<size_t>(slot) >= sEquipTargets.size()) {
        return false;
    }

    const auto& stored = sEquipTargets[slot];
    if (!stored.valid) {
        return false;
    }

    target = stored;
    return true;
}

void set_control_override(Control control, ControlOverride override) noexcept {
    const auto index = static_cast<std::size_t>(control);
    if (index >= sControlOverrides.size()) {
        return;
    }
    sControlOverrides[index] = override;
}

void sync_virtual_input() noexcept {
    if (sTouchControls != nullptr) {
        sTouchControls->sync_virtual_input();
    }
}

TouchControls::TouchControls()
    : Document(touch_controls_document_source(), true),
      mRoot(mDocument != nullptr ? mDocument->GetElementById("root") : nullptr),
      mControlStick(mDocument != nullptr ? mDocument->GetElementById("control-stick") : nullptr),
      mControlKnob(mDocument != nullptr ? mDocument->GetElementById("control-knob") : nullptr),
      mActionBar(mDocument != nullptr ? mDocument->GetElementById("action-bar") : nullptr) {
    sTouchControls = this;
    if (mDocument != nullptr) {
        for (std::size_t i = 0; i < kControls.size(); ++i) {
            const auto& info = kControls[i];
            auto& elements = mControlElements[i];
            elements.root = info.id != nullptr ? mDocument->GetElementById(info.id) : nullptr;
            elements.icon =
                info.iconId != nullptr ? mDocument->GetElementById(info.iconId) : nullptr;
            elements.oil = info.oilId != nullptr ? mDocument->GetElementById(info.oilId) : nullptr;
            elements.oilFill =
                info.oilFillId != nullptr ? mDocument->GetElementById(info.oilFillId) : nullptr;
            elements.count =
                info.countId != nullptr ? mDocument->GetElementById(info.countId) : nullptr;
        }
    }

    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
        }
    });

    auto listenControl = [this](Control control) {
        const auto index = static_cast<std::size_t>(control);
        auto* element = index < mControlElements.size() ? mControlElements[index].root : nullptr;
        if (element == nullptr) {
            return;
        }
        listen(element, aurora::rmlui::TouchStartEvent, [this, control](Rml::Event& event) {
            if (!visible() || mWasSuppressed || !getSettings().game.enableTouchControls) {
                return;
            }
            if (start_control_touch(touch_event_id(event), control)) {
                event.StopPropagation();
            }
        });
        listen(element, aurora::rmlui::TouchEndEvent, [this](Rml::Event& event) {
            if (release_control_touch(touch_event_id(event), false)) {
                event.StopPropagation();
            }
        });
        listen(element, aurora::rmlui::TouchCancelEvent, [this](Rml::Event& event) {
            if (release_control_touch(touch_event_id(event), true)) {
                event.StopPropagation();
            }
        });
    };
    for (std::size_t i = 0; i < kControls.size(); ++i) {
        listenControl(static_cast<Control>(i));
    }

    listen(mRoot, aurora::rmlui::TouchStartEvent,
        [this](Rml::Event& event) { handle_touch_down(event); });
    listen(mRoot, aurora::rmlui::TouchMoveEvent,
        [this](Rml::Event& event) { handle_touch_motion(event); });
    listen(
        mRoot, aurora::rmlui::TouchEndEvent, [this](Rml::Event& event) { handle_touch_up(event); });
    listen(mRoot, aurora::rmlui::TouchCancelEvent,
        [this](Rml::Event& event) { handle_touch_cancel(event); });
    listen(mRoot, Rml::EventId::Mousemove, [this](Rml::Event& event) { handle_mouse_move(event); });
    listen(mRoot, Rml::EventId::Mousedown, [this](Rml::Event& event) { handle_mouse_down(event); });
    listen(mRoot, Rml::EventId::Mouseup, [this](Rml::Event& event) { handle_mouse_up(event); });
}

TouchControls::~TouchControls() {
    clear_virtual_input();
    clearAllVirtualActionBinds();
    if (sTouchControls == this) {
        sTouchControls = nullptr;
    }
}

void TouchControls::show() {
    Document::show();
    if (mRoot != nullptr) {
        mRoot->SetAttribute("open", "");
    }
}

void TouchControls::hide(bool close) {
    clear_virtual_input();
    if (mRoot != nullptr) {
        mRoot->RemoveAttribute("open");
        mPendingClose = close;
    } else {
        Document::hide(close);
    }
}

void TouchControls::set_control_pressed(Control control, bool pressed) {
    set_control_visual(control, pressed);
    sync_control_button_mask();

    switch (control) {
    case Control::L:
        if (control_override_active(control)) {
            mLPressed = pressed;
            mLLatched = false;
            mManualLLatched = false;
            mLReleasePending = false;
            mLPressStartTime = {};
            mLastLTapTime = {};
            break;
        }
        switch (getSettings().game.touchTargeting.getValue()) {
        case TouchTargeting::Hold:
            mLPressed = pressed;
            mLLatched = false;
            mManualLLatched = false;
            mLReleasePending = false;
            mLPressStartTime = {};
            mLastLTapTime = {};
            break;
        case TouchTargeting::Switch:
            if (pressed) {
                const bool wasLatched = mLPressed || mLLatched || mManualLLatched;
                mLPressed = false;
                mLLatched = false;
                mManualLLatched = !wasLatched;
                mLReleasePending = true;
            } else {
                mLPressed = false;
                mLLatched = false;
                mLReleasePending = false;
            }
            mLPressStartTime = {};
            mLastLTapTime = {};
            break;
        case TouchTargeting::Hybrid:
        default:
            if (pressed && (mLLatched || mManualLLatched)) {
                mLLatched = false;
                mManualLLatched = false;
                mLPressed = false;
                mLReleasePending = true;
                mLPressStartTime = {};
                mLastLTapTime = {};
                set_control_visual(control, false);
            } else if (pressed) {
                const auto now = clock::now();
                if (!player_attention_locked() && mLastLTapTime != clock::time_point{} &&
                    now - mLastLTapTime <= kLDoubleTapWindow)
                {
                    mManualLLatched = true;
                    mLPressed = false;
                    mLReleasePending = true;
                    mLPressStartTime = {};
                    mLastLTapTime = {};
                } else if (!mLReleasePending) {
                    mLPressed = true;
                    mLPressStartTime = now;
                }
            } else if (!mLReleasePending) {
                mLPressed = false;
            }
            if (!pressed) {
                const auto now = clock::now();
                if (!mLReleasePending) {
                    const bool wasQuickTap = mLPressStartTime != clock::time_point{} &&
                                             now - mLPressStartTime <= kLDoubleTapWindow;
                    mLastLTapTime = wasQuickTap ? now : clock::time_point{};
                }
                mLPressStartTime = {};
                mLReleasePending = false;
            }
            if (!pressed && !player_attention_locked()) {
                mLLatched = false;
            }
            break;
        }
        break;
    case Control::R:
        mRTriggerHeld = pressed;
        break;
    default:
        break;
    }
}

bool TouchControls::fire_control_action(Control control, ControlAction action) noexcept {
    const auto* info = control_info(control);
    if (info == nullptr) {
        return false;
    }

    const auto actionBind = action == ControlAction::Tap ? info->tapAction : info->holdAction;
    if (!actionBind) {
        return false;
    }

    const auto actionIndex = static_cast<std::size_t>(*actionBind);
    if (actionIndex >= mQueuedActions.size()) {
        return false;
    }

    mQueuedActions.set(actionIndex);
    return true;
}

bool TouchControls::start_control_touch(SDL_FingerID id, Control control) noexcept {
    const auto index = static_cast<std::size_t>(control);
    if (index >= mControlTouches.size()) {
        return false;
    }

    auto& touch = mControlTouches[index];
    if (touch.active) {
        return false;
    }

    touch = {
        .id = id,
        .startTime = clock::now(),
        .active = true,
        .longPressFired = false,
    };
    set_control_pressed(control, true);
    return true;
}

void TouchControls::release_control(Control control) noexcept {
    const auto index = static_cast<std::size_t>(control);
    if (index < mControlTouches.size()) {
        mControlTouches[index] = {};
    }

    sync_control_button_mask();
    switch (control) {
    case Control::L:
        mLPressed = false;
        mLLatched = false;
        mManualLLatched = false;
        mLReleasePending = false;
        mLPressStartTime = {};
        mLastLTapTime = {};
        break;
    case Control::R:
        mRTriggerHeld = false;
        break;
    default:
        break;
    }
    set_control_visual(control, false);
}

void TouchControls::sync_control_button_mask() noexcept {
    u16 buttonMask = 0;
    for (std::size_t i = 0; i < mControlTouches.size() && i < kControls.size(); ++i) {
        if (mControlTouches[i].active) {
            buttonMask |= kControls[i].padButton;
        }
    }
    mButtonMask = buttonMask;
}

void TouchControls::set_control_visual(Control control, bool pressed) noexcept {
    const auto index = static_cast<std::size_t>(control);
    auto* element = index < mControlElements.size() ? mControlElements[index].root : nullptr;
    if (index >= mControlVisualPressed.size()) {
        return;
    }
    mControlVisualPressed[index] = pressed;
    if (element != nullptr) {
        element->SetClass("pressed", pressed);
    }
    apply_control_transform(control);
}

void TouchControls::apply_control_transform(Control control) noexcept {
    const auto index = static_cast<std::size_t>(control);
    if (index >= mControlElements.size()) {
        return;
    }
    auto& elements = mControlElements[index];
    auto& layout = elements.layout;
    const float pressedScale =
        index < mControlVisualPressed.size() && mControlVisualPressed[index] ? kPressedScale : 1.f;
    apply_control_transform_if_changed(
        elements.root, layout.appliedTransform, layout.layoutScale * pressedScale);
}

void TouchControls::sync_l_lock_state() noexcept {
    const auto targeting = getSettings().game.touchTargeting.getValue();
    if (targeting == TouchTargeting::Hold) {
        mLLatched = false;
        mManualLLatched = false;
        return;
    }
    if (targeting == TouchTargeting::Switch) {
        mLLatched = false;
        return;
    }

    if (player_attention_locked()) {
        if (mLPressed) {
            mLLatched = true;
        }
    } else {
        mLLatched = false;
    }
}

void TouchControls::clear_motion_touch_input() noexcept {
    mMoveTouch = {};
    mCameraTouch = {};
    touch_camera::clear();
    if (mControlStick != nullptr) {
        mControlStick->SetClass("active", false);
    }
}

void TouchControls::clear_control_input() noexcept {
    for (std::size_t i = 0; i < mControlTouches.size(); ++i) {
        release_control(static_cast<Control>(i));
    }
    mQueuedActions.reset();
    mButtonMask = 0;
    mWantsVirtualPad = false;
    PADClearVirtualStatus(kPort);
    for (const auto& control : kControls) {
        if (control.tapAction) {
            setVirtualActionBind(*control.tapAction, kPort, false, false);
        }
        if (control.holdAction) {
            setVirtualActionBind(*control.holdAction, kPort, false, false);
        }
    }
}

void TouchControls::clear_virtual_input() noexcept {
    clear_motion_touch_input();
    mMenuPointerTouch = 0;
    mMenuPointerMouseSuppressions = 0;
    mMenuPointerTouchActive = false;
    clear_control_input();
}

void TouchControls::sync_touch_state() noexcept {
    const bool controlsEnabled = getSettings().game.enableTouchControls;
    const bool pointerMenuActive = menu_pointer::active();
    if (mWasSuppressed || (!controlsEnabled && !pointerMenuActive)) {
        clear_virtual_input();
        return;
    }

    if (pointerMenuActive) {
        clear_motion_touch_input();
        if (!controlsEnabled) {
            clear_control_input();
            return;
        }
    }

    sync_l_lock_state();
    const bool aimActive = dCamera_c::isAimActive();
    if (aimActive && !hawkeye_active() && mMoveTouch.active) {
        if (!mCameraTouch.active) {
            mCameraTouch = mMoveTouch;
            mCameraTouch.start = mMoveTouch.current;
        }
        mMoveTouch = {};
    }

    const float stickRadius = kStickRadiusDp * touch_dp_scale();
    if (mControlStick != nullptr) {
        if (mMoveTouch.active && stickRadius > 0.f) {
            const auto delta =
                clamped_stick_delta(mMoveTouch.start, mMoveTouch.current, stickRadius);
            const float knobRadius = kStickKnobRadiusDp * touch_dp_scale();
            mControlStick->SetClass("active", true);
            mControlStick->SetProperty(Rml::PropertyId::Left,
                Rml::Property(mMoveTouch.start.x - stickRadius, Rml::Unit::PX));
            mControlStick->SetProperty(Rml::PropertyId::Top,
                Rml::Property(mMoveTouch.start.y - stickRadius, Rml::Unit::PX));
            mControlKnob->SetProperty(Rml::PropertyId::Left,
                Rml::Property(stickRadius + delta.x - knobRadius, Rml::Unit::PX));
            mControlKnob->SetProperty(Rml::PropertyId::Top,
                Rml::Property(stickRadius + delta.y - knobRadius, Rml::Unit::PX));
        } else {
            mControlStick->SetClass("active", false);
        }
    }

    sync_visual_state();
}

void TouchControls::sync_virtual_input() noexcept {
    sync_touch_state();
    if (mWasSuppressed || !getSettings().game.enableTouchControls) {
        return;
    }

    PADStatus status{};
    status.err = PAD_ERR_NONE;
    status.button = mButtonMask;

    if (mLPressed || mLLatched || mManualLLatched) {
        status.button |= PAD_TRIGGER_L;
        status.triggerLeft = kTriggerAnalog;
    }
    if (mRTriggerHeld) {
        status.button |= PAD_TRIGGER_R;
        status.triggerRight = kTriggerAnalog;
    }

    const float stickRadius = kStickRadiusDp * touch_dp_scale();
    if (mMoveTouch.active && stickRadius > 0.f) {
        const auto delta = clamped_stick_delta(mMoveTouch.start, mMoveTouch.current, stickRadius);
        const float stickX = stick_value(delta.x / stickRadius);
        const float stickY = stick_value(-delta.y / stickRadius);
        switch (stick_output_mode()) {
        case StickOutput::CStick:
            status.substickX = stickX;
            status.substickY = stickY;
            break;
        case StickOutput::MainStick:
        default:
            status.stickX = stickX;
            status.stickY = stickY;
            break;
        }
    }

    mWantsVirtualPad = status.button != 0 || status.stickX != 0 || status.stickY != 0 ||
                       status.substickX != 0 || status.substickY != 0 || status.triggerLeft != 0 ||
                       status.triggerRight != 0;
    if (mWantsVirtualPad) {
        PADSetVirtualStatus(kPort, &status);
    } else {
        PADClearVirtualStatus(kPort);
    }

    for (const auto& control : kControls) {
        if (control.tapAction) {
            const bool queued = mQueuedActions.test(static_cast<std::size_t>(*control.tapAction));
            setVirtualActionBind(
                *control.tapAction, kPort, queued, queued && visible() && !mWasSuppressed);
        }
        if (control.holdAction) {
            const bool queued = mQueuedActions.test(static_cast<std::size_t>(*control.holdAction));
            setVirtualActionBind(
                *control.holdAction, kPort, queued, queued && visible() && !mWasSuppressed);
        }
    }
    mQueuedActions.reset();
}

void TouchControls::sync_visibility() noexcept {
    mWasSuppressed = any_document_visible();
    if ((getSettings().game.enableTouchControls ||
            (menu_pointer::enabled() && menu_pointer::active())) &&
        !mWasSuppressed)
    {
        show();
    } else if (visible()) {
        hide(false);
    } else {
        clear_virtual_input();
    }
}

void TouchControls::sync_safe_area() noexcept {
    if (mDocument == nullptr) {
        return;
    }
    const auto insets = safe_area_insets(mDocument->GetContext());
    if (insets == mSafeInsets) {
        return;
    }
    mSafeInsets = insets;
}

void TouchControls::sync_control_layouts() noexcept {
    auto* context = mDocument != nullptr ? mDocument->GetContext() : aurora::rmlui::get_context();
    const auto docSize = touch_document_size_dp(context);
    if (docSize.w <= 0.f || docSize.h <= 0.f || context == nullptr) {
        return;
    }

    const auto& customControls = getSettings().game.touchControlsLayout.getValue().controls;
    for (const auto& info : touch_layout_controls()) {
        auto props = info.props;
        if (const auto iter = customControls.find(info.layoutId); iter != customControls.end()) {
            props = iter->second;
        }

        const auto layout = resolve_control_layout(props, docSize);
        if (info.hasControl) {
            const auto index = static_cast<std::size_t>(info.control);
            if (index >= mControlElements.size()) {
                continue;
            }

            auto& elements = mControlElements[index];
            auto& state = elements.layout;
            state.visualRect = layout.visual;
            state.layoutScale = layout.scale;
            apply_control_box_if_changed(elements.root, state.appliedBox, layout.box);
            apply_control_dock_classes(
                elements.root, touch_control_dock_anchor(layout.visual, docSize));
            apply_control_transform(info.control);
            continue;
        }

        mActionBarLayout.visualRect = layout.visual;
        mActionBarLayout.layoutScale = layout.scale;
        apply_control_box_if_changed(mActionBar, mActionBarLayout.appliedBox, layout.box);
        apply_control_dock_classes(mActionBar, touch_control_dock_anchor(layout.visual, docSize));
        apply_control_transform_if_changed(
            mActionBar, mActionBarLayout.appliedTransform, mActionBarLayout.layoutScale);
    }
}

void TouchControls::sync_visual_state() noexcept {
    if (mWasSuppressed || !getSettings().game.enableTouchControls) {
        clear_motion_touch_input();
        for (const auto control : {Control::L, Control::R}) {
            const auto& elements = mControlElements[static_cast<std::size_t>(control)];
            if (elements.root != nullptr) {
                elements.root->SetPseudoClass("hidden", true);
            }
            release_control(control);
        }
        return;
    }

    const bool hideGameplayControls = game_controls_suppressed();
    const auto& lTrigger = mControlElements[static_cast<std::size_t>(Control::L)];
    const auto& rTrigger = mControlElements[static_cast<std::size_t>(Control::R)];
    const bool lHidden = hideGameplayControls && !control_override_active(Control::L);
    const bool rHidden = hideGameplayControls && !control_override_active(Control::R);

    if (lTrigger.root != nullptr) {
        lTrigger.root->SetPseudoClass("hidden", lHidden);
        lTrigger.root->SetClass("active",
            !lHidden && (mLPressed || mLLatched || mManualLLatched ||
                            (!control_override_active(Control::L) && player_attention_locked())));
    }
    if (rTrigger.root != nullptr) {
        rTrigger.root->SetPseudoClass("hidden", rHidden);
    }

    if (lHidden) {
        release_control(Control::L);
    }
    if (rHidden) {
        release_control(Control::R);
    }
}

void TouchControls::sync_action_bar_state() noexcept {
    if (mWasSuppressed || !getSettings().game.enableTouchControls) {
        if (mActionBar != nullptr) {
            mActionBar->SetPseudoClass("hidden", true);
        }
        const auto& skip = mControlElements[static_cast<std::size_t>(Control::SKIP)];
        if (skip.root != nullptr) {
            skip.root->SetPseudoClass("hidden", true);
        }
        for (const auto control : {Control::FIRST_PERSON, Control::ITEMS, Control::COLLECTIONS,
                 Control::MAP, Control::SKIP})
        {
            release_control(control);
        }
        return;
    }

    auto* event = dComIfGp_getEvent();
    const bool skipVisible = event != nullptr && event->mEventStatus == 1 &&
                             event->mSkipFunc != nullptr && !event->chkFlag2(2);
    const bool hidden =
        !skipVisible &&
        (!controls_available(false) || dComIfGp_event_runCheck() ||
            (dComIfGp_getMsgObjectClass() != nullptr && dMsgObject_isTalkNowCheck()));
    const auto& skip = mControlElements[static_cast<std::size_t>(Control::SKIP)];
    if (mActionBar != nullptr) {
        mActionBar->SetPseudoClass("hidden", hidden || skipVisible);
    }
    if (skip.root != nullptr) {
        skip.root->SetPseudoClass("hidden", !skipVisible);
    }
    if (skipVisible) {
        for (const auto control :
            {Control::FIRST_PERSON, Control::ITEMS, Control::COLLECTIONS, Control::MAP})
        {
            release_control(control);
        }
        return;
    }

    release_control(Control::SKIP);
    if (!hidden) {
        return;
    }

    for (const auto control :
        {Control::FIRST_PERSON, Control::ITEMS, Control::COLLECTIONS, Control::MAP})
    {
        release_control(control);
    }
}

void TouchControls::sync_control_displays() noexcept {
    if (mWasSuppressed || !getSettings().game.enableTouchControls) {
        for (const auto control : {Control::A, Control::B, Control::X, Control::Y, Control::Z}) {
            const auto& elements = mControlElements[static_cast<std::size_t>(control)];
            if (elements.root != nullptr) {
                elements.root->SetPseudoClass("hidden", true);
            }
            release_control(control);
        }
        clear_equip_targets();
        return;
    }

    const auto bState = b_button_state();
    const auto xState = xy_button_state(Control::X);
    const auto yState = xy_button_state(Control::Y);
    const auto zState = z_button_state();

    const auto& a = mControlElements[static_cast<std::size_t>(Control::A)];
    const auto& b = mControlElements[static_cast<std::size_t>(Control::B)];
    const auto& x = mControlElements[static_cast<std::size_t>(Control::X)];
    const auto& y = mControlElements[static_cast<std::size_t>(Control::Y)];
    const auto& z = mControlElements[static_cast<std::size_t>(Control::Z)];

    if (a.root != nullptr) {
        a.root->SetPseudoClass("hidden", false);
    }
    if (z.root != nullptr) {
        z.root->SetPseudoClass("hidden", !zState.visible);
        z.root->SetClass("has-icon", zState.showIcon);
    }
    if (!zState.visible) {
        release_control(Control::Z);
    }
    if (z.icon != nullptr) {
        z.icon->SetClass("visible", zState.showIcon);
    }

    const bool zSourceChanged = zState.iconSource != mZTriggerIconSource;
    const bool zRevisionChanged = zState.iconRevision != mZTriggerIconRevision;
    if (zSourceChanged || zRevisionChanged) {
        const std::string previousSource = mZTriggerIconSource;
        mZTriggerIconSource = zState.iconSource;
        mZTriggerIconRevision = zState.iconRevision;
        if (z.icon == nullptr) {
            release_rml_texture(previousSource);
        } else if (zState.iconSource.empty()) {
            z.icon->RemoveAttribute("src");
        } else {
            release_rml_texture(zState.iconSource);
            if (!zSourceChanged) {
                z.icon->RemoveAttribute("src");
            }
            z.icon->SetAttribute("src", zState.iconSource);
        }
        if (zSourceChanged) {
            release_rml_texture(previousSource);
        }
    }

    const auto syncIcon = [this](Rml::Element* button, Rml::Element* icon, std::string& lastSource,
                              Control control, const FaceButtonState& state) {
        if (button != nullptr) {
            button->SetPseudoClass("hidden", !state.visible);
            button->SetClass("has-item", state.showIcon);
        }
        if (!state.visible) {
            release_control(control);
        }

        if (icon != nullptr) {
            icon->SetClass("visible", state.showIcon);
        }

        if (state.iconSource == lastSource) {
            return;
        }

        const std::string previousSource = lastSource;
        lastSource = state.iconSource;
        if (icon == nullptr) {
            release_rml_texture(previousSource);
            return;
        }
        if (state.iconSource.empty()) {
            icon->RemoveAttribute("src");
        } else {
            icon->SetAttribute("src", state.iconSource);
        }
        release_rml_texture(previousSource);
    };

    syncIcon(b.root, b.icon, mButtonBIconSource, Control::B, bState);
    syncIcon(x.root, x.icon, mButtonXIconSource, Control::X, xState);
    syncIcon(y.root, y.icon, mButtonYIconSource, Control::Y, yState);

    const auto syncCount = [](Rml::Element* countElement, std::string& lastLabel, Control control,
                               const FaceButtonState& state) {
        const std::string label = state.showIcon ? item_count_label_for_button(control) : "";
        if (label == lastLabel) {
            return;
        }

        lastLabel = label;
        if (countElement == nullptr) {
            return;
        }
        countElement->SetClass("visible", !label.empty());
        countElement->SetInnerRML(label);
    };

    syncCount(x.count, mButtonXCountLabel, Control::X, xState);
    syncCount(y.count, mButtonYCountLabel, Control::Y, yState);

    const auto syncOil = [](Rml::Element* meter, Rml::Element* fill, Control control,
                             const FaceButtonState& state) {
        const auto oilFill = state.showIcon ? item_oil_fill_for_button(control) : std::nullopt;
        if (meter != nullptr) {
            meter->SetClass("visible", oilFill.has_value());
        }
        if (fill != nullptr) {
            const float percent = oilFill ? *oilFill * 100.f : 0.f;
            fill->SetProperty(Rml::PropertyId::Width, Rml::Property(percent, Rml::Unit::PERCENT));
        }
    };

    syncOil(b.oil, b.oilFill, Control::B, bState);
    syncOil(x.oil, x.oilFill, Control::X, xState);
    syncOil(y.oil, y.oilFill, Control::Y, yState);

    clear_equip_targets();
    if (!visible() || mWasSuppressed || !getSettings().game.enableTouchControls) {
        return;
    }

    if (xState.showIcon && x.layout.visualRect) {
        sync_equip_target(0, *x.layout.visualRect, kFaceIconTargetRatio, kFaceIconTargetRatio);
    }
    if (yState.showIcon && y.layout.visualRect) {
        sync_equip_target(1, *y.layout.visualRect, kFaceIconTargetRatio, kFaceIconTargetRatio);
    }
    if (zState.showIcon && z.layout.visualRect) {
        sync_equip_target(2, *z.layout.visualRect, 1.f, 1.f, true);
    }
    if (bState.showIcon && b.layout.visualRect) {
        sync_equip_target(3, *b.layout.visualRect, kFaceIconTargetRatio, kFaceIconTargetRatio);
    }
}

void TouchControls::update() {
    sync_visibility();
    sync_control_long_presses();
    sync_safe_area();
    sync_control_layouts();
    sync_visual_state();
    sync_action_bar_state();
    sync_control_displays();
    sync_touch_state();
}

bool TouchControls::release_control_touch(SDL_FingerID id, bool cancelled) noexcept {
    for (std::size_t i = 0; i < mControlTouches.size(); ++i) {
        auto& touch = mControlTouches[i];
        if (!touch.active || touch.id != id) {
            continue;
        }

        const auto control = static_cast<Control>(i);
        const bool shouldFireTapAction = !cancelled && !touch.longPressFired;
        touch = {};
        set_control_pressed(control, false);
        if (shouldFireTapAction) {
            fire_control_action(control, ControlAction::Tap);
        }
        return true;
    }

    return false;
}

void TouchControls::sync_control_long_presses() noexcept {
    const auto now = clock::now();
    for (std::size_t i = 0; i < mControlTouches.size(); ++i) {
        auto& touch = mControlTouches[i];
        if (!touch.active || touch.longPressFired || now - touch.startTime < kHoldActionDuration) {
            continue;
        }

        if (!fire_control_action(static_cast<Control>(i), ControlAction::Hold)) {
            continue;
        }

        touch.longPressFired = true;
    }
}

bool TouchControls::handle_menu_event(Rml::Event& event, menu_pointer::Phase phase) noexcept {
    if (!menu_pointer::active() || event.GetTargetElement() != mRoot) {
        return false;
    }
    if (!menu_pointer::enabled()) {
        mMenuPointerTouch = 0;
        mMenuPointerMouseSuppressions = 0;
        mMenuPointerTouchActive = false;
        event.StopPropagation();
        return true;
    }

    const auto id = touch_event_id(event);
    switch (phase) {
    case menu_pointer::Phase::Press:
        if (mMenuPointerTouchActive) {
            event.StopPropagation();
            return true;
        }
        mMenuPointerTouch = id;
        mMenuPointerTouchActive = true;
        break;
    case menu_pointer::Phase::Move:
        if (!mMenuPointerTouchActive || mMenuPointerTouch != id) {
            event.StopPropagation();
            return true;
        }
        break;
    case menu_pointer::Phase::Release:
    case menu_pointer::Phase::Cancel:
        if (!mMenuPointerTouchActive || mMenuPointerTouch != id) {
            event.StopPropagation();
            return true;
        }
        mMenuPointerTouchActive = false;
        break;
    }

    const auto position = touch_event_position(event);
    menu_pointer::handle_fallthrough_pointer(position.x, position.y, phase, true);
    switch (phase) {
    case menu_pointer::Phase::Press:
    case menu_pointer::Phase::Release:
        mMenuPointerMouseSuppressions = 2;
        break;
    case menu_pointer::Phase::Move:
    case menu_pointer::Phase::Cancel:
        mMenuPointerMouseSuppressions = 1;
        break;
    }
    event.StopPropagation();
    return true;
}

void TouchControls::handle_touch_down(Rml::Event& event) noexcept {
    if (!visible() || mWasSuppressed) {
        return;
    }
    if (handle_menu_event(event, menu_pointer::Phase::Press)) {
        return;
    }
    if (!getSettings().game.enableTouchControls) {
        return;
    }

    const auto position = touch_event_position(event);
    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }

    const auto id = touch_event_id(event);
    const auto dimensions = context->GetDimensions();
    const float top = mSafeInsets.top + kAnalogZoneTopDp * touch_dp_scale();
    const float bottom = static_cast<float>(dimensions.y) - mSafeInsets.bottom -
                         kAnalogZoneBottomDp * touch_dp_scale();
    const auto width = static_cast<float>(dimensions.x);
    const bool inAnalogZone = position.y >= top && position.y <= bottom;
    const bool inLeftZone = position.x < width * kLeftZoneWidth;
    if (dCamera_c::isAimActive()) {
        if (hawkeye_active() && inAnalogZone && inLeftZone) {
            if (!mMoveTouch.active) {
                mMoveTouch = {
                    .id = id,
                    .start = position,
                    .current = position,
                    .active = true,
                };
            }
            return;
        }

        if (!mCameraTouch.active) {
            mCameraTouch = {
                .id = id,
                .start = position,
                .current = position,
                .active = true,
            };
        }
        return;
    }

    if (!inAnalogZone) {
        return;
    }

    if (!mMoveTouch.active && inLeftZone) {
        mMoveTouch = {
            .id = id,
            .start = position,
            .current = position,
            .active = true,
        };
    } else if (!mCameraTouch.active && position.x > width * kRightZoneStart) {
        mCameraTouch = {
            .id = id,
            .start = position,
            .current = position,
            .active = true,
        };
    }
}

void TouchControls::handle_touch_motion(Rml::Event& event) noexcept {
    if (!visible() || mWasSuppressed) {
        return;
    }
    if (handle_menu_event(event, menu_pointer::Phase::Move)) {
        return;
    }
    if (!getSettings().game.enableTouchControls) {
        return;
    }

    const auto id = touch_event_id(event);
    const auto position = touch_event_position(event);
    if (mMoveTouch.active && mMoveTouch.id == id) {
        mMoveTouch.current = position;
    }
    if (mCameraTouch.active && mCameraTouch.id == id) {
        const auto delta = position - mCameraTouch.current;
        mCameraTouch.current = position;
        const float scale = touch_dp_scale();
        touch_camera::add_delta(delta.x / scale, delta.y / scale);
    }
}

void TouchControls::handle_touch_up(Rml::Event& event) noexcept {
    if (!visible() || mWasSuppressed) {
        return;
    }
    const auto id = touch_event_id(event);
    if (release_control_touch(id, false)) {
        return;
    }
    if (handle_menu_event(event, menu_pointer::Phase::Release)) {
        return;
    }
    if (mMoveTouch.active && mMoveTouch.id == id) {
        mMoveTouch = {};
    }
    if (mCameraTouch.active && mCameraTouch.id == id) {
        mCameraTouch = {};
    }
}

void TouchControls::handle_touch_cancel(Rml::Event& event) noexcept {
    if (!visible() || mWasSuppressed) {
        return;
    }
    const auto id = touch_event_id(event);
    if (release_control_touch(id, true)) {
        return;
    }
    if (handle_menu_event(event, menu_pointer::Phase::Cancel)) {
        return;
    }
    if (mMoveTouch.active && mMoveTouch.id == id) {
        mMoveTouch = {};
    }
    if (mCameraTouch.active && mCameraTouch.id == id) {
        mCameraTouch = {};
    }
}

void TouchControls::handle_mouse_move(Rml::Event& event) noexcept {
    if (mMenuPointerMouseSuppressions > 0) {
        --mMenuPointerMouseSuppressions;
        return;
    }
    if (!visible() || mWasSuppressed || !menu_pointer::active() ||
        !menu_pointer::enabled() || event.GetTargetElement() != mRoot)
    {
        return;
    }

    const auto position = mouse_event_position(event);
    menu_pointer::handle_fallthrough_pointer(
        position.x, position.y, menu_pointer::Phase::Move, false);
    event.StopPropagation();
}

void TouchControls::handle_mouse_down(Rml::Event& event) noexcept {
    if (mMenuPointerMouseSuppressions > 0) {
        --mMenuPointerMouseSuppressions;
        return;
    }
    if (!visible() || mWasSuppressed || !menu_pointer::active() ||
        !menu_pointer::enabled() || event.GetTargetElement() != mRoot)
    {
        return;
    }

    const auto position = mouse_event_position(event);
    const s32 button = event.GetParameter("button", -1);
    if (!menu_pointer::handle_fallthrough_pointer(
            position.x, position.y, menu_pointer::Phase::Press, false, button))
    {
        return;
    }
    event.StopPropagation();
}

void TouchControls::handle_mouse_up(Rml::Event& event) noexcept {
    if (mMenuPointerMouseSuppressions > 0) {
        --mMenuPointerMouseSuppressions;
        return;
    }
    if (!visible() || mWasSuppressed ||
        !menu_pointer::enabled() ||
        (!menu_pointer::active() && !menu_pointer::mouse_capture_active()) ||
        event.GetTargetElement() != mRoot)
    {
        return;
    }

    const auto position = mouse_event_position(event);
    const s32 button = event.GetParameter("button", -1);
    if (!menu_pointer::handle_fallthrough_pointer(
            position.x, position.y, menu_pointer::Phase::Release, false, button))
    {
        return;
    }
    event.StopPropagation();
}

}  // namespace dusk::ui
