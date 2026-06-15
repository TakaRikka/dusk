#include "touch_controls_editor.hpp"

#include "modal.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "dusk/config.hpp"
#include "dusk/settings.h"
#include "m_Do/m_Do_audio.h"

#include <aurora/rmlui.hpp>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

namespace dusk::ui {
namespace {

constexpr float kDragThresholdDp = 6.f;
constexpr float kMinControlDp = 36.f;
constexpr float kMinTriggerWidthDp = 44.f;
constexpr float kMinTriggerHeightDp = 32.f;
constexpr float kMinActionBarWidthDp = 112.f;
constexpr float kMinActionBarHeightDp = 36.f;
constexpr float kMinScale = 0.25f;

struct HandleBinding {
    const char* id = nullptr;
    TouchControlsEditor::EditHandle handle = TouchControlsEditor::EditHandle::Move;
};

constexpr std::array kHandleBindings = {
    HandleBinding{"editor-handle-left", TouchControlsEditor::EditHandle::Left},
    HandleBinding{"editor-handle-right", TouchControlsEditor::EditHandle::Right},
    HandleBinding{"editor-handle-top", TouchControlsEditor::EditHandle::Top},
    HandleBinding{"editor-handle-bottom", TouchControlsEditor::EditHandle::Bottom},
    HandleBinding{"editor-handle-top-left", TouchControlsEditor::EditHandle::TopLeft},
    HandleBinding{"editor-handle-top-right", TouchControlsEditor::EditHandle::TopRight},
    HandleBinding{"editor-handle-bottom-left", TouchControlsEditor::EditHandle::BottomLeft},
    HandleBinding{"editor-handle-bottom-right", TouchControlsEditor::EditHandle::BottomRight},
};

Rml::String touch_controls_editor_document_source() {
    const auto fragment = touch_controls_rml_fragment();
    return Rml::String{R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/touch_controls.rcss" />
    <link type="text/rcss" href="res/rml/touch_controls_editor.rcss" />
</head>
<body id="root" class="touch-editor">
)RML"} + Rml::String{fragment.data(), fragment.size()} + Rml::String{R"RML(
    <selection-frame id="editor-selection-frame">
        <resize-handle id="editor-handle-left" class="edge horizontal left" />
        <resize-handle id="editor-handle-right" class="edge horizontal right" />
        <resize-handle id="editor-handle-top" class="edge vertical top" />
        <resize-handle id="editor-handle-bottom" class="edge vertical bottom" />
        <resize-handle id="editor-handle-top-left" class="corner top left" />
        <resize-handle id="editor-handle-top-right" class="corner top right" />
        <resize-handle id="editor-handle-bottom-left" class="corner bottom left" />
        <resize-handle id="editor-handle-bottom-right" class="corner bottom right" />
    </selection-frame>
    <editor-toolbar id="editor-toolbar">
        <button id="editor-save" class="editor-command primary"><span>Save</span></button>
        <button id="editor-reset" class="editor-command"><span>Reset</span></button>
        <button id="editor-cancel" class="editor-command"><span>Cancel</span></button>
    </editor-toolbar>
</body>
</rml>
)RML"};
}

bool is_corner(TouchControlsEditor::EditHandle handle) noexcept {
    using EditHandle = TouchControlsEditor::EditHandle;
    return handle == EditHandle::TopLeft || handle == EditHandle::TopRight ||
           handle == EditHandle::BottomLeft || handle == EditHandle::BottomRight;
}

bool is_horizontal_edge(TouchControlsEditor::EditHandle handle) noexcept {
    using EditHandle = TouchControlsEditor::EditHandle;
    return handle == EditHandle::Left || handle == EditHandle::Right;
}

bool is_vertical_edge(TouchControlsEditor::EditHandle handle) noexcept {
    using EditHandle = TouchControlsEditor::EditHandle;
    return handle == EditHandle::Top || handle == EditHandle::Bottom;
}

bool control_valid(std::size_t index) noexcept {
    return index < touch_layout_controls().size();
}

float squared_distance(Rml::Vector2f a, Rml::Vector2f b) noexcept {
    const auto delta = a - b;
    return delta.x * delta.x + delta.y * delta.y;
}

}  // namespace

TouchControlsEditor::TouchControlsEditor()
    : Document(touch_controls_editor_document_source()),
      mRoot(mDocument != nullptr ? mDocument->GetElementById("root") : nullptr),
      mSelectionFrame(
          mDocument != nullptr ? mDocument->GetElementById("editor-selection-frame") : nullptr),
      mSaveButton(mDocument != nullptr ? mDocument->GetElementById("editor-save") : nullptr),
      mResetButton(mDocument != nullptr ? mDocument->GetElementById("editor-reset") : nullptr),
      mCancelButton(mDocument != nullptr ? mDocument->GetElementById("editor-cancel") : nullptr),
      mWorkingLayout(getSettings().game.touchControlsLayout.getValue()) {
    mWorkingLayout.version = ControlLayout::Version;

    const auto controls = touch_layout_controls();
    for (std::size_t i = 0; i < controls.size() && i < mElements.size(); ++i) {
        mElements[i].root =
            mDocument != nullptr ? mDocument->GetElementById(controls[i].elementId) : nullptr;
    }

    bind_control_events();
    bind_handle_events();
    bind_toolbar_events();

    listen(mRoot, aurora::rmlui::TouchStartEvent, [this](Rml::Event& event) {
        if (event.GetTargetElement() != mRoot) {
            return;
        }
        clear_selected_control();
        event.StopPropagation();
    });
    listen(mRoot, Rml::EventId::Mousedown, [this](Rml::Event& event) {
        const s32 button = event.GetParameter("button", -1);
        if (button != 0 || event.GetTargetElement() != mRoot) {
            return;
        }
        clear_selected_control();
        event.StopPropagation();
    });
    listen(mRoot, aurora::rmlui::TouchMoveEvent, [this](Rml::Event& event) {
        if (continue_edit(touch_event_position(event))) {
            event.StopPropagation();
        }
    });
    listen(mRoot, aurora::rmlui::TouchEndEvent, [this](Rml::Event& event) {
        if (end_edit(true, touch_event_id(event), false)) {
            event.StopPropagation();
        }
    });
    listen(mRoot, aurora::rmlui::TouchCancelEvent, [this](Rml::Event& event) {
        if (end_edit(true, touch_event_id(event), true)) {
            event.StopPropagation();
        }
    });
    listen(mRoot, Rml::EventId::Mousemove, [this](Rml::Event& event) {
        if (continue_edit(mouse_event_position(event))) {
            event.StopPropagation();
        }
    });
    listen(mRoot, Rml::EventId::Mouseup, [this](Rml::Event& event) {
        if (end_edit(false, 0, false)) {
            event.StopPropagation();
        }
    });
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
        }
    });
}

void TouchControlsEditor::show() {
    Document::show();
    if (mRoot != nullptr) {
        mRoot->SetAttribute("open", "");
    }
}

void TouchControlsEditor::hide(bool close) {
    if (mRoot != nullptr) {
        mRoot->RemoveAttribute("open");
        mPendingClose = close;
    } else {
        Document::hide(close);
    }
}

void TouchControlsEditor::update() {
    sync_control_layouts();
    sync_selection_frame();
    Document::update();
}

bool TouchControlsEditor::focus() {
    return mSaveButton != nullptr && mSaveButton->Focus(true);
}

void TouchControlsEditor::bind_control_events() noexcept {
    const auto controls = touch_layout_controls();
    for (std::size_t i = 0; i < controls.size() && i < mElements.size(); ++i) {
        auto* element = mElements[i].root;
        if (element == nullptr) {
            continue;
        }

        listen(element, aurora::rmlui::TouchStartEvent, [this, i](Rml::Event& event) {
            if (begin_edit(i, EditHandle::Move, touch_event_position(event), true,
                    touch_event_id(event)))
            {
                event.StopPropagation();
            }
        });
        listen(element, Rml::EventId::Mousedown, [this, i](Rml::Event& event) {
            const s32 button = event.GetParameter("button", -1);
            if (button != 0) {
                return;
            }
            if (begin_edit(i, EditHandle::Move, mouse_event_position(event), false)) {
                event.StopPropagation();
            }
        });
    }
}

void TouchControlsEditor::bind_handle_events() noexcept {
    for (const auto& binding : kHandleBindings) {
        auto* element = mDocument != nullptr ? mDocument->GetElementById(binding.id) : nullptr;
        if (element == nullptr) {
            continue;
        }

        listen(element, aurora::rmlui::TouchStartEvent, [this, handle = binding.handle](
                                                        Rml::Event& event) {
            if (!control_valid(mSelectedIndex)) {
                return;
            }
            if (begin_edit(mSelectedIndex, handle, touch_event_position(event), true,
                    touch_event_id(event)))
            {
                event.StopPropagation();
            }
        });
        listen(element, Rml::EventId::Mousedown, [this, handle = binding.handle](Rml::Event& event) {
            const s32 button = event.GetParameter("button", -1);
            if (button != 0 || !control_valid(mSelectedIndex)) {
                return;
            }
            if (begin_edit(mSelectedIndex, handle, mouse_event_position(event), false)) {
                event.StopPropagation();
            }
        });
    }
}

void TouchControlsEditor::bind_toolbar_events() noexcept {
    bind_button_command(mSaveButton, &TouchControlsEditor::save_layout);
    bind_button_command(mResetButton, &TouchControlsEditor::request_reset);
    bind_button_command(mCancelButton, &TouchControlsEditor::cancel_edit);
}

void TouchControlsEditor::bind_button_command(
    Rml::Element* element, void (TouchControlsEditor::*callback)()) noexcept {
    if (element == nullptr) {
        return;
    }

    listen(element, Rml::EventId::Click, [this, callback](Rml::Event& event) {
        (this->*callback)();
        event.StopPropagation();
    });
    listen(element, Rml::EventId::Keydown, [this, callback](Rml::Event& event) {
        if (map_nav_event(event) != NavCommand::Confirm) {
            return;
        }
        (this->*callback)();
        event.StopPropagation();
    });
}

void TouchControlsEditor::sync_control_layouts() noexcept {
    auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr;
    const auto docSize = touch_document_size_dp(context);
    if (docSize.w <= 0.f || docSize.h <= 0.f || context == nullptr) {
        return;
    }

    const auto controls = touch_layout_controls();
    for (std::size_t i = 0; i < controls.size() && i < mElements.size(); ++i) {
        const auto layout = resolve_control_layout(props_for(i), docSize);
        auto& element = mElements[i];
        element.layout.visualRect = layout.visual;
        element.layout.layoutScale = layout.scale;
        if (element.root != nullptr) {
            element.root->SetPseudoClass("hidden", false);
        }
        apply_control_box_if_changed(element.root, element.layout.appliedBox, layout.box);
        apply_control_dock_classes(
            element.root, touch_control_dock_anchor(layout.visual, docSize));
        apply_control_transform_if_changed(
            element.root, element.layout.appliedTransform, element.layout.layoutScale);
    }
}

void TouchControlsEditor::sync_selection_frame() noexcept {
    const bool hasSelection =
        control_valid(mSelectedIndex) && mElements[mSelectedIndex].layout.visualRect;
    if (mSelectionFrame == nullptr) {
        return;
    }

    mSelectionFrame->SetClass("visible", hasSelection);
    for (std::size_t i = 0; i < mElements.size(); ++i) {
        if (mElements[i].root != nullptr) {
            mElements[i].root->SetClass("editor-selected", hasSelection && i == mSelectedIndex);
        }
    }
    if (!hasSelection) {
        mAppliedSelectionFrame = std::nullopt;
        return;
    }

    apply_control_box_if_changed(
        mSelectionFrame, mAppliedSelectionFrame, *mElements[mSelectedIndex].layout.visualRect);
}

void TouchControlsEditor::set_selected_control(std::size_t index) noexcept {
    if (!control_valid(index)) {
        clear_selected_control();
        return;
    }
    mSelectedIndex = index;
    sync_selection_frame();
}

void TouchControlsEditor::clear_selected_control() noexcept {
    mSelectedIndex = kTouchLayoutControlCount;
    sync_selection_frame();
}

ControlProps TouchControlsEditor::props_for(std::size_t index) const {
    const auto controls = touch_layout_controls();
    if (!control_valid(index)) {
        return {};
    }

    const auto& info = controls[index];
    if (const auto iter = mWorkingLayout.controls.find(info.layoutId);
        iter != mWorkingLayout.controls.end())
    {
        return iter->second;
    }
    return info.props;
}

void TouchControlsEditor::store_props(
    std::size_t index, ControlRect visual, ControlProps props) noexcept {
    if (!control_valid(index)) {
        return;
    }

    auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr;
    const auto docSize = touch_document_size_dp(context);
    if (docSize.w <= 0.f || docSize.h <= 0.f) {
        return;
    }

    props.w = std::max(props.w, 1.f);
    props.h = std::max(props.h, 1.f);
    props.scale = std::max(props.scale, kMinScale);
    props = encode_control_props(visual, docSize, props, touch_control_dock_anchor(visual, docSize));
    mWorkingLayout.version = ControlLayout::Version;
    mWorkingLayout.controls[std::string{touch_layout_controls()[index].layoutId}] = props;
    sync_control_layouts();
    sync_selection_frame();
}

void TouchControlsEditor::restore_active_control() noexcept {
    if (!control_valid(mPointerEdit.index)) {
        return;
    }

    auto& controls = mWorkingLayout.controls;
    const auto key = std::string{touch_layout_controls()[mPointerEdit.index].layoutId};
    if (mPointerEdit.storedProps) {
        controls[key] = *mPointerEdit.storedProps;
    } else {
        controls.erase(key);
    }
    sync_control_layouts();
    sync_selection_frame();
}

bool TouchControlsEditor::begin_edit(
    std::size_t index, EditHandle handle, Rml::Vector2f positionPx, bool touch,
    SDL_FingerID touchId) noexcept {
    if (!control_valid(index) || mPointerEdit.active) {
        return false;
    }

    auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr;
    const auto docSize = touch_document_size_dp(context);
    if (docSize.w <= 0.f || docSize.h <= 0.f) {
        return false;
    }

    const auto props = props_for(index);
    const auto layout = resolve_control_layout(props, docSize);
    std::optional<ControlProps> storedProps;
    if (const auto iter = mWorkingLayout.controls.find(touch_layout_controls()[index].layoutId);
        iter != mWorkingLayout.controls.end())
    {
        storedProps = iter->second;
    }

    mPointerEdit = {
        .index = index,
        .touchId = touchId,
        .startPointerDp = pointer_position_dp(positionPx),
        .startVisual = layout.visual,
        .startProps = props,
        .storedProps = storedProps,
        .handle = handle,
        .active = true,
        .touch = touch,
    };
    set_selected_control(index);
    return true;
}

bool TouchControlsEditor::continue_edit(Rml::Vector2f positionPx) noexcept {
    if (!mPointerEdit.active) {
        return false;
    }

    const auto pointerDp = pointer_position_dp(positionPx);
    if (!mPointerEdit.dragging) {
        if (squared_distance(pointerDp, mPointerEdit.startPointerDp) <
            kDragThresholdDp * kDragThresholdDp)
        {
            return true;
        }
        mPointerEdit.dragging = true;
    }

    auto props = mPointerEdit.startProps;
    auto rect = rect_for_edit(pointerDp, props);
    rect = clamp_visual_rect(mPointerEdit.index, rect);
    if (is_corner(mPointerEdit.handle)) {
        props.scale = std::max(rect.w / std::max(mPointerEdit.startProps.w, 1.f), kMinScale);
    } else if (is_horizontal_edge(mPointerEdit.handle)) {
        props.w = rect.w / std::max(props.scale, kMinScale);
    } else if (is_vertical_edge(mPointerEdit.handle)) {
        props.h = rect.h / std::max(props.scale, kMinScale);
    }
    store_props(mPointerEdit.index, rect, props);
    return true;
}

bool TouchControlsEditor::end_edit(bool touch, SDL_FingerID touchId, bool cancelled) noexcept {
    if (!mPointerEdit.active || mPointerEdit.touch != touch ||
        (touch && mPointerEdit.touchId != touchId))
    {
        return false;
    }

    if (cancelled && mPointerEdit.dragging) {
        restore_active_control();
    }
    mPointerEdit = {};
    return true;
}

Rml::Vector2f TouchControlsEditor::pointer_position_dp(Rml::Vector2f positionPx) const noexcept {
    auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr;
    return positionPx / touch_dp_scale(context);
}

ControlRect TouchControlsEditor::rect_for_edit(
    Rml::Vector2f pointerDp, ControlProps& props) const noexcept {
    const auto& edit = mPointerEdit;
    auto rect = edit.startVisual;
    const auto delta = pointerDp - edit.startPointerDp;

    switch (edit.handle) {
    case EditHandle::Move:
        rect.l += delta.x;
        rect.t += delta.y;
        return rect;
    case EditHandle::Left: {
        const float right = edit.startVisual.l + edit.startVisual.w;
        rect.l = pointerDp.x;
        rect.w = right - rect.l;
        return rect;
    }
    case EditHandle::Right:
        rect.w = pointerDp.x - edit.startVisual.l;
        return rect;
    case EditHandle::Top: {
        const float bottom = edit.startVisual.t + edit.startVisual.h;
        rect.t = pointerDp.y;
        rect.h = bottom - rect.t;
        return rect;
    }
    case EditHandle::Bottom:
        rect.h = pointerDp.y - edit.startVisual.t;
        return rect;
    case EditHandle::TopLeft:
    case EditHandle::TopRight:
    case EditHandle::BottomLeft:
    case EditHandle::BottomRight:
        break;
    }

    auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr;
    const auto docSize = touch_document_size_dp(context);
    const bool left = edit.handle == EditHandle::TopLeft || edit.handle == EditHandle::BottomLeft;
    const bool top = edit.handle == EditHandle::TopLeft || edit.handle == EditHandle::TopRight;
    const Rml::Vector2f fixed{
        left ? edit.startVisual.l + edit.startVisual.w : edit.startVisual.l,
        top ? edit.startVisual.t + edit.startVisual.h : edit.startVisual.t,
    };
    const float desiredW = left ? fixed.x - pointerDp.x : pointerDp.x - fixed.x;
    const float desiredH = top ? fixed.y - pointerDp.y : pointerDp.y - fixed.y;
    const auto minSize = min_visual_size(edit.index);
    const float minRatio =
        std::max(minSize.x / std::max(edit.startVisual.w, 1.f),
            minSize.y / std::max(edit.startVisual.h, 1.f));
    const float maxW = left ? fixed.x : docSize.w - fixed.x;
    const float maxH = top ? fixed.y : docSize.h - fixed.y;
    const float maxRatio =
        std::max(minRatio, std::min(maxW / std::max(edit.startVisual.w, 1.f),
                                maxH / std::max(edit.startVisual.h, 1.f)));
    const float ratio =
        std::clamp(std::max(desiredW / std::max(edit.startVisual.w, 1.f),
                       desiredH / std::max(edit.startVisual.h, 1.f)),
            minRatio, maxRatio);

    rect.w = edit.startVisual.w * ratio;
    rect.h = edit.startVisual.h * ratio;
    rect.l = left ? fixed.x - rect.w : fixed.x;
    rect.t = top ? fixed.y - rect.h : fixed.y;
    props.scale = std::max(edit.startProps.scale * ratio, kMinScale);
    return rect;
}

ControlRect TouchControlsEditor::clamp_visual_rect(std::size_t index, ControlRect rect) const noexcept {
    auto* context = mDocument != nullptr ? mDocument->GetContext() : nullptr;
    const auto docSize = touch_document_size_dp(context);
    if (docSize.w <= 0.f || docSize.h <= 0.f || !control_valid(index)) {
        return rect;
    }

    const auto minSize = min_visual_size(index);
    const float minW = std::min(minSize.x, docSize.w);
    const float minH = std::min(minSize.y, docSize.h);
    rect.w = std::clamp(rect.w, minW, docSize.w);
    rect.h = std::clamp(rect.h, minH, docSize.h);
    rect.l = std::clamp(rect.l, 0.f, std::max(0.f, docSize.w - rect.w));
    rect.t = std::clamp(rect.t, 0.f, std::max(0.f, docSize.h - rect.h));
    return rect;
}

Rml::Vector2f TouchControlsEditor::min_visual_size(std::size_t index) const noexcept {
    if (!control_valid(index)) {
        return {kMinControlDp, kMinControlDp};
    }

    const auto id = touch_layout_controls()[index].layoutId;
    if (id == "actionBar") {
        return {kMinActionBarWidthDp, kMinActionBarHeightDp};
    }
    if (id == "triggerL" || id == "triggerR" || id == "buttonZ" || id == "skip") {
        return {kMinTriggerWidthDp, kMinTriggerHeightDp};
    }
    return {kMinControlDp, kMinControlDp};
}

bool TouchControlsEditor::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel || cmd == NavCommand::Menu) {
        cancel_edit();
        return true;
    }
    return Document::handle_nav_command(event, cmd);
}

void TouchControlsEditor::save_layout() {
    mWorkingLayout.version = ControlLayout::Version;
    getSettings().game.touchControlsLayout.setValue(mWorkingLayout);
    config::Save();
    mDoAud_seStartMenu(kSoundItemChange);
    pop();
}

void TouchControlsEditor::request_reset() {
    auto dismiss = [](Modal& modal) { modal.pop(); };
    push(std::make_unique<Modal>(Modal::Props{
        .title = "Reset Touch Layout?",
        .bodyRml = "Reset controls to their default layout. This will not be saved until you press Save.",
        .actions =
            {
                ModalAction{
                    .label = "Reset",
                    .onPressed =
                        [this, dismiss](Modal& modal) {
                            reset_working_layout();
                            mDoAud_seStartMenu(kSoundItemChange);
                            dismiss(modal);
                        },
                },
                ModalAction{
                    .label = "Cancel",
                    .onPressed = dismiss,
                },
            },
    }));
}

void TouchControlsEditor::reset_working_layout() noexcept {
    mWorkingLayout = ControlLayout{};
    mWorkingLayout.version = ControlLayout::Version;
    mPointerEdit = {};
    sync_control_layouts();
    sync_selection_frame();
}

void TouchControlsEditor::cancel_edit() {
    mDoAud_seStartMenu(kSoundWindowClose);
    pop();
}

}  // namespace dusk::ui
