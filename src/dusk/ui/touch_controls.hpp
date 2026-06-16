#pragma once

#include "controls.hpp"
#include "document.hpp"

#include "dusk/action_bindings.h"
#include "dusk/menu_pointer.h"
#include "dusk/ui/controls.hpp"

#include <array>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>

namespace dusk::ui {

enum class ControlOverride {
    Default,
    Action,
};

bool get_equip_target(int slot, EquipTarget& target) noexcept;
void set_control_override(Control control, ControlOverride override) noexcept;
void sync_virtual_input() noexcept;

class TouchControls final : public Document {
public:
    TouchControls();
    ~TouchControls() override;

    void show() override;
    void hide(bool close) override;
    void update() override;
    void sync_virtual_input() noexcept;

private:
    struct StickTouch {
        SDL_FingerID id = 0;
        Rml::Vector2f start;
        Rml::Vector2f current;
        bool active = false;
    };
    struct ControlTouch {
        SDL_FingerID id = 0;
        clock::time_point startTime{};
        bool active = false;
        bool longPressFired = false;
    };
    struct LayoutState {
        std::optional<ControlRect> visualRect;
        std::optional<ControlRect> appliedBox;
        float layoutScale = 1.0f;
        std::optional<float> appliedTransform;
    };
    struct ControlElements {
        Rml::Element* root = nullptr;
        Rml::Element* icon = nullptr;
        Rml::Element* oil = nullptr;
        Rml::Element* oilFill = nullptr;
        Rml::Element* count = nullptr;
        LayoutState layout;
    };
    enum class ControlAction {
        Tap,
        Hold,
    };

    void set_control_pressed(Control control, bool pressed);
    void release_control(Control control) noexcept;
    void sync_control_button_mask() noexcept;
    bool fire_control_action(Control control, ControlAction action) noexcept;
    bool start_control_touch(SDL_FingerID id, Control control) noexcept;
    void set_control_visual(Control control, bool pressed) noexcept;
    void sync_l_lock_state() noexcept;
    void clear_motion_touch_input() noexcept;
    void clear_control_input() noexcept;
    void clear_virtual_input() noexcept;
    void sync_touch_state() noexcept;
    void sync_visibility() noexcept;
    void sync_safe_area() noexcept;
    void sync_control_layouts() noexcept;
    void sync_visual_state() noexcept;
    void sync_action_bar_state() noexcept;
    void sync_control_displays() noexcept;
    void apply_control_transform(Control control) noexcept;
    void handle_touch_down(Rml::Event& event) noexcept;
    void handle_touch_motion(Rml::Event& event) noexcept;
    void handle_touch_up(Rml::Event& event) noexcept;
    void handle_touch_cancel(Rml::Event& event) noexcept;
    void handle_mouse_move(Rml::Event& event) noexcept;
    void handle_mouse_down(Rml::Event& event) noexcept;
    void handle_mouse_up(Rml::Event& event) noexcept;
    void sync_control_long_presses() noexcept;
    bool release_control_touch(SDL_FingerID id, bool cancelled) noexcept;
    bool handle_menu_event(Rml::Event& event, menu_pointer::Phase phase) noexcept;

    Rml::Element* mRoot = nullptr;
    Rml::Element* mControlStick = nullptr;
    Rml::Element* mControlKnob = nullptr;
    Rml::Element* mActionBar = nullptr;
    std::array<ControlElements, static_cast<std::size_t>(Control::COUNT)> mControlElements{};
    std::string mButtonBIconSource;
    std::string mButtonXIconSource;
    std::string mButtonYIconSource;
    std::string mZTriggerIconSource;
    uint64_t mZTriggerIconRevision = 0;
    std::string mButtonXCountLabel;
    std::string mButtonYCountLabel;
    StickTouch mMoveTouch;
    StickTouch mCameraTouch;
    SDL_FingerID mMenuPointerTouch = 0;
    int mMenuPointerMouseSuppressions = 0;
    std::array<ControlTouch, static_cast<std::size_t>(Control::COUNT)> mControlTouches{};
    std::array<bool, static_cast<std::size_t>(Control::COUNT)> mControlVisualPressed{};
    std::bitset<static_cast<std::size_t>(ActionBinds::COUNT)> mQueuedActions;
    LayoutState mActionBarLayout;
    Insets mSafeInsets;
    u16 mButtonMask = 0;
    bool mLPressed = false;
    bool mLLatched = false;
    bool mManualLLatched = false;
    bool mLReleasePending = false;
    bool mRTriggerHeld = false;
    bool mWantsVirtualPad = false;
    bool mWasSuppressed = true;
    bool mMenuPointerTouchActive = false;
    clock::time_point mLPressStartTime{};
    clock::time_point mLastLTapTime{};
};

}  // namespace dusk::ui
