#pragma once

#include "button.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace dusk::ui {

class List : public FluentComponent<List> {
public:
    struct Item {
        uint64_t key = 0;
        Rml::String label;
    };

    struct Props {
        std::vector<Item> items;
        std::function<void(uint64_t)> onPressed;
        std::function<bool(uint64_t)> isSelected;
        std::function<bool(uint64_t)> isDisabled;
    };

    List(Rml::Element* parent, Props props);

    void set_items(std::vector<Item> items);
    void update() override;
    bool focus() override;
    bool focus_from(NavCommand direction) override;

private:
    struct SnapshotFocus {
        bool owned = false;
        std::optional<uint64_t> key;
        int index = -1;
    };

    struct Row {
        uint64_t key = 0;
        std::unique_ptr<ControlledButton> button;
        bool culled = true;
    };

    Row* row_from_element(Rml::Element* element) const;
    int row_index(uint64_t key) const;
    SnapshotFocus capture_snapshot_focus();
    std::unique_ptr<Row> create_row(const Item& item);
    void apply_items(std::vector<Item> items, const std::optional<SnapshotFocus>& snapshotFocus = {});
    void update_culling();
    void show_row(Row& row);
    bool focus_row(int index, bool mayEnterList);
    void request_focus(uint64_t key, bool mayEnterList);
    void update_pending_focus();
    void handle_keydown(Rml::Event& event);

    Props mProps;
    Rml::Element* mViewport = nullptr;
    Rml::Element* mContent = nullptr;
    Rml::Element* mEmpty = nullptr;
    std::vector<Item> mItems;
    std::optional<std::vector<Item>> mPendingItems;
    std::optional<SnapshotFocus> mPendingSnapshotFocus;
    std::vector<std::unique_ptr<Row>> mRows;
    std::unordered_map<uint64_t, Row*> mRowsByKey;
    std::optional<uint64_t> mActiveKey;
    std::optional<uint64_t> mPendingFocusKey;
    int mPendingFocusFrames = 0;
    bool mPendingFocusMayEnterList = false;
    bool mCullDirty = true;
    int mLayoutScanFrames = 2;
    float mLastScrollTop = -1.0f;
    float mLastViewportWidth = -1.0f;
    float mLastViewportHeight = -1.0f;
};

}  // namespace dusk::ui
