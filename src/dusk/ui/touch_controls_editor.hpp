#pragma once

#include "controls.hpp"
#include "document.hpp"
#include "touch_controls_common.hpp"

#include <array>
#include <cstddef>
#include <optional>

namespace dusk::ui {

class TouchControlsEditor final : public Document {
public:
    TouchControlsEditor();

    void show() override;
    void hide(bool close) override;
    void update() override;
    bool focus() override;

    enum class EditHandle {
        Move,
        Left,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
    };

private:
    struct LayoutState {
        std::optional<ControlRect> visualRect;
        std::optional<ControlRect> appliedBox;
        float layoutScale = 1.0f;
        std::optional<float> appliedTransform;
    };

    struct EditElement {
        Rml::Element* root = nullptr;
        LayoutState layout;
    };

    struct PointerEdit {
        std::size_t index = kTouchLayoutControlCount;
        SDL_FingerID touchId = 0;
        Rml::Vector2f startPointerDp;
        ControlRect startVisual;
        ControlProps startProps;
        std::optional<ControlProps> storedProps;
        EditHandle handle = EditHandle::Move;
        bool active = false;
        bool touch = false;
        bool dragging = false;
    };

    void bind_control_events() noexcept;
    void bind_handle_events() noexcept;
    void bind_toolbar_events() noexcept;
    void bind_button_command(
        Rml::Element* element, void (TouchControlsEditor::*callback)()) noexcept;
    void sync_control_layouts() noexcept;
    void sync_selection_frame() noexcept;
    void set_selected_control(std::size_t index) noexcept;
    void clear_selected_control() noexcept;
    ControlProps props_for(std::size_t index) const;
    void store_props(std::size_t index, ControlRect visual, ControlProps props) noexcept;
    void restore_active_control() noexcept;
    bool begin_edit(std::size_t index, EditHandle handle, Rml::Vector2f positionPx, bool touch,
        SDL_FingerID touchId = 0) noexcept;
    bool continue_edit(Rml::Vector2f positionPx) noexcept;
    bool end_edit(bool touch, SDL_FingerID touchId, bool cancelled) noexcept;
    Rml::Vector2f pointer_position_dp(Rml::Vector2f positionPx) const noexcept;
    ControlRect rect_for_edit(Rml::Vector2f pointerDp, ControlProps& props) const noexcept;
    ControlRect clamp_visual_rect(std::size_t index, ControlRect rect) const noexcept;
    Rml::Vector2f min_visual_size(std::size_t index) const noexcept;
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;
    void save_layout();
    void request_reset();
    void reset_working_layout() noexcept;
    void cancel_edit();

    Rml::Element* mRoot = nullptr;
    Rml::Element* mSelectionFrame = nullptr;
    Rml::Element* mSaveButton = nullptr;
    Rml::Element* mResetButton = nullptr;
    Rml::Element* mCancelButton = nullptr;
    std::array<EditElement, kTouchLayoutControlCount> mElements{};
    ControlLayout mWorkingLayout;
    PointerEdit mPointerEdit;
    std::optional<ControlRect> mAppliedSelectionFrame;
    std::size_t mSelectedIndex = kTouchLayoutControlCount;
};

}  // namespace dusk::ui
