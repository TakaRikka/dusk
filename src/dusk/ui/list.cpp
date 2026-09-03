#include "list.hpp"

#include "ui.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include <algorithm>
#include <ranges>
#include <utility>

namespace dusk::ui {
namespace {

Rml::Element* create_root(Rml::Element* parent) {
    auto* document = parent->GetOwnerDocument();
    auto root = document->CreateElement("ui-list");
    return parent->AppendChild(std::move(root));
}

Rml::Element* append_element(Rml::Element* parent, const Rml::String& tag) {
    auto element = parent->GetOwnerDocument()->CreateElement(tag);
    return parent->AppendChild(std::move(element));
}

}  // namespace

List::List(Rml::Element* parent, Props props)
    : FluentComponent(create_root(parent)), mProps(std::move(props)) {
    mViewport = append_element(mRoot, "ui-list-viewport");
    mContent = append_element(mViewport, "ui-list-content");
    mEmpty = append_element(mRoot, "ui-list-empty");
    append_text(mEmpty, "No items");

    Component::listen(mViewport, Rml::EventId::Scroll, [this](Rml::Event&) { mCullDirty = true; });
    listen(Rml::EventId::Keydown, [this](Rml::Event& event) { handle_keydown(event); });

    apply_items(std::move(mProps.items));
}

void List::set_items(std::vector<Item> items) {
    mPendingSnapshotFocus = capture_snapshot_focus();
    mPendingItems = std::move(items);
}

void List::update() {
    if (mPendingItems) {
        auto items = std::move(*mPendingItems);
        const auto snapshotFocus = mPendingSnapshotFocus;
        mPendingItems.reset();
        mPendingSnapshotFocus.reset();
        apply_items(std::move(items), snapshotFocus);
    }

    for (const auto& row : mRows) {
        if (!row->culled) {
            row->button->update();
        }
    }

    const float scrollTop = mViewport->GetScrollTop();
    const float viewportWidth = mViewport->GetClientWidth();
    const float viewportHeight = mViewport->GetClientHeight();
    if (scrollTop != mLastScrollTop || viewportWidth != mLastViewportWidth ||
        viewportHeight != mLastViewportHeight)
    {
        mCullDirty = true;
        if (viewportWidth != mLastViewportWidth || viewportHeight != mLastViewportHeight) {
            mLayoutScanFrames = std::max(mLayoutScanFrames, 2);
        }
        mLastScrollTop = scrollTop;
        mLastViewportWidth = viewportWidth;
        mLastViewportHeight = viewportHeight;
    }

    if (mCullDirty || mLayoutScanFrames > 0) {
        update_culling();
        mCullDirty = false;
        if (mLayoutScanFrames > 0) {
            --mLayoutScanFrames;
        }
    }

    update_pending_focus();
}

bool List::focus() {
    if (mActiveKey) {
        const int activeIndex = row_index(*mActiveKey);
        if (activeIndex >= 0 && focus_row(activeIndex, true)) {
            return true;
        }
    }
    for (int i = 0; i < static_cast<int>(mRows.size()); ++i) {
        if (focus_row(i, true)) {
            return true;
        }
    }
    return false;
}

bool List::focus_from(NavCommand direction) {
    if (direction != NavCommand::Up) {
        return focus();
    }
    for (int i = static_cast<int>(mRows.size()) - 1; i >= 0; --i) {
        if (focus_row(i, true)) {
            return true;
        }
    }
    return false;
}

List::Row* List::row_from_element(Rml::Element* element) const {
    for (const auto& row : mRows) {
        if (row->button->contains(element)) {
            return row.get();
        }
    }
    return nullptr;
}

int List::row_index(uint64_t key) const {
    for (int i = 0; i < static_cast<int>(mRows.size()); ++i) {
        if (mRows[i]->key == key) {
            return i;
        }
    }
    return -1;
}

List::SnapshotFocus List::capture_snapshot_focus() {
    auto* context = mRoot->GetContext();
    auto* focusedElement = context != nullptr ? context->GetFocusElement() : nullptr;
    Row* focusedRow = row_from_element(focusedElement);
    if (focusedRow == nullptr) {
        return {};
    }
    mActiveKey = focusedRow->key;
    return {
        .owned = true,
        .key = focusedRow->key,
        .index = row_index(focusedRow->key),
    };
}

std::unique_ptr<List::Row> List::create_row(const Item& item) {
    auto row = std::make_unique<Row>();
    row->key = item.key;
    row->button = std::make_unique<ControlledButton>(mContent,
        ControlledButton::Props{
            .text = item.label,
            .isSelected =
                [this, key = item.key] { return mProps.isSelected && mProps.isSelected(key); },
            .isDisabled =
                [this, key = item.key] { return mProps.isDisabled && mProps.isDisabled(key); },
        });
    row->button->root()->SetClass("ui-list-row", true);
    row->button->root()->SetProperty("visibility", "hidden");
    row->button->Component::listen(row->button->root(), Rml::EventId::Focus,
        [this, key = item.key](Rml::Event&) { mActiveKey = key; });
    row->button->on_pressed([this, key = item.key] {
        mActiveKey = key;
        if (mProps.onPressed) {
            mProps.onPressed(key);
        }
    });
    return row;
}

void List::apply_items(std::vector<Item> items, const std::optional<SnapshotFocus>& snapshotFocus) {
    const SnapshotFocus focusState = snapshotFocus ? *snapshotFocus : capture_snapshot_focus();

    std::unordered_map<uint64_t, std::unique_ptr<Row>> oldRows;
    oldRows.reserve(mRows.size());
    for (auto& row : mRows) {
        oldRows.emplace(row->key, std::move(row));
    }
    mRows.clear();
    mRows.reserve(items.size());

    for (const auto& item : items) {
        if (auto node = oldRows.extract(item.key); !node.empty()) {
            auto row = std::move(node.mapped());
            row->button->set_text(item.label);
            mRows.push_back(std::move(row));
        } else {
            mRows.push_back(create_row(item));
        }
    }

    for (auto& row : oldRows | std::views::values) {
        mContent->RemoveChild(row->button->root());
    }

    for (int i = 0; i < static_cast<int>(mRows.size()); ++i) {
        auto* desired = mRows[i]->button->root();
        auto* current = mContent->GetChild(i);
        if (current != desired) {
            auto element = mContent->RemoveChild(desired);
            mContent->InsertBefore(std::move(element), current);
        }
    }

    mRowsByKey.clear();
    mRowsByKey.reserve(mRows.size());
    for (const auto& row : mRows) {
        mRowsByKey.emplace(row->key, row.get());
    }
    mItems = std::move(items);

    if (mRows.empty()) {
        mViewport->SetProperty("display", "none");
        mEmpty->RemoveProperty("display");
    } else {
        mViewport->RemoveProperty("display");
        mEmpty->SetProperty("display", "none");
    }

    if (focusState.owned && !mRows.empty()) {
        uint64_t targetKey =
            mRows[std::min(focusState.index, static_cast<int>(mRows.size()) - 1)]->key;
        if (focusState.key && mRowsByKey.contains(*focusState.key)) {
            targetKey = *focusState.key;
        }
        request_focus(targetKey, false);
    } else {
        mPendingFocusKey.reset();
        mPendingFocusFrames = 0;
        mPendingFocusMayEnterList = false;
    }

    mCullDirty = true;
    mLayoutScanFrames = 2;
}

void List::update_culling() {
    const float viewTop = mViewport->GetAbsoluteOffset(Rml::BoxArea::Border).y;
    const float viewHeight = mViewport->GetClientHeight();
    if (viewHeight <= 0.0f) {
        return;
    }
    auto* context = mRoot->GetContext();
    const Row* focusedRow = context != nullptr ? row_from_element(context->GetFocusElement()) : nullptr;

    for (const auto& row : mRows) {
        auto* element = row->button->root();
        const float top = element->GetAbsoluteOffset(Rml::BoxArea::Border).y - viewTop;
        const bool inWindow =
            top + element->GetOffsetHeight() >= -viewHeight && top <= viewHeight * 2.0f;
        const bool focusGuard =
            row.get() == focusedRow || (mPendingFocusKey && row->key == *mPendingFocusKey);
        const bool shouldShow = inWindow || focusGuard;
        if (shouldShow && row->culled) {
            show_row(*row);
        } else if (!shouldShow && !row->culled) {
            row->culled = true;
            element->SetProperty("visibility", "hidden");
        }
    }
}

void List::show_row(Row& row) {
    if (!row.culled) {
        return;
    }
    row.button->update();
    row.button->root()->RemoveProperty("visibility");
    row.culled = false;
}

bool List::focus_row(int index, bool mayEnterList) {
    if (index < 0 || index >= static_cast<int>(mRows.size())) {
        return false;
    }
    auto& row = *mRows[index];
    show_row(row);
    row.button->update();
    if (row.button->root()->IsPseudoClassSet("disabled")) {
        return false;
    }
    if (row.button->focus()) {
        mActiveKey = row.key;
        mPendingFocusKey.reset();
        mPendingFocusFrames = 0;
        mPendingFocusMayEnterList = false;
        return true;
    }
    request_focus(row.key, mayEnterList);
    return true;
}

void List::request_focus(uint64_t key, bool mayEnterList) {
    const auto it = mRowsByKey.find(key);
    if (it == mRowsByKey.end()) {
        return;
    }
    show_row(*it->second);
    mPendingFocusKey = key;
    mPendingFocusFrames = 2;
    mPendingFocusMayEnterList = mayEnterList;
}

void List::update_pending_focus() {
    if (!mPendingFocusKey) {
        return;
    }
    const auto it = mRowsByKey.find(*mPendingFocusKey);
    if (it == mRowsByKey.end()) {
        mPendingFocusKey.reset();
        return;
    }

    auto* context = mRoot->GetContext();
    auto* focusedElement = context != nullptr ? context->GetFocusElement() : nullptr;
    if (!mPendingFocusMayEnterList && focusedElement != nullptr && !contains(focusedElement)) {
        mPendingFocusKey.reset();
        return;
    }
    if (it->second->button->contains(focusedElement)) {
        mPendingFocusKey.reset();
        mPendingFocusFrames = 0;
        mPendingFocusMayEnterList = false;
        return;
    }
    if (mPendingFocusFrames > 0) {
        --mPendingFocusFrames;
        return;
    }

    auto& row = *it->second;
    show_row(row);
    row.button->update();
    if (!row.button->root()->IsPseudoClassSet("disabled") && row.button->focus()) {
        mActiveKey = row.key;
    }
    mPendingFocusKey.reset();
    mPendingFocusMayEnterList = false;
}

void List::handle_keydown(Rml::Event& event) {
    const NavCommand command = map_nav_event(event);
    if (command != NavCommand::Down && command != NavCommand::Up) {
        return;
    }
    Row* focusedRow = row_from_element(event.GetTargetElement());
    if (focusedRow == nullptr) {
        return;
    }
    mActiveKey = focusedRow->key;
    const int direction = command == NavCommand::Down ? 1 : -1;
    for (int i = row_index(focusedRow->key) + direction;
        i >= 0 && i < static_cast<int>(mRows.size()); i += direction)
    {
        if (focus_row(i, true)) {
            mDoAud_seStartMenu(kSoundItemFocus);
            event.StopPropagation();
            return;
        }
    }
}

}  // namespace dusk::ui
