#include "nav_group.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include <algorithm>

namespace dusk::ui {

NavGroup::NavGroup(Rml::Element* root, Props props) : Component{root}, mProps{props} {
    mProps.columns = std::max(mProps.columns, 1);

    listen(
        mRoot, Rml::EventId::Focus,
        [this](Rml::Event& event) {
            const int index = item_index(event.GetTargetElement());
            if (index >= 0) {
                mRememberedIndex = index;
            }
        },
        true);
    listen(mRoot, Rml::EventId::Keydown, [this](Rml::Event& event) {
        const auto direction = map_nav_event(event);
        const auto result = navigate(event.GetTargetElement(), direction);
        if (result == MoveResult::Moved) {
            mDoAud_seStartMenu(kSoundItemFocus);
        }
        if (result != MoveResult::Unhandled) {
            event.StopPropagation();
        }
    });
}

bool NavGroup::focus() {
    if (mRememberedIndex >= 0 && focus_index(mRememberedIndex, NavCommand::None)) {
        return true;
    }
    return focus_linear(0, 1, NavCommand::None);
}

bool NavGroup::focus_from(NavCommand direction) {
    if (mRememberedIndex >= 0 && focus_index(mRememberedIndex, direction)) {
        return true;
    }
    const bool reverse = direction == NavCommand::Up || direction == NavCommand::Left;
    return focus_linear(
        reverse ? static_cast<int>(mItems.size()) - 1 : 0, reverse ? -1 : 1, direction);
}

NavGroup::MoveResult NavGroup::navigate(Rml::Element* target, NavCommand direction) {
    const bool horizontal = direction == NavCommand::Left || direction == NavCommand::Right;
    const bool vertical = direction == NavCommand::Up || direction == NavCommand::Down;
    if (!horizontal && !vertical) {
        return MoveResult::Unhandled;
    }

    const int current = item_index(target);
    if (current < 0) {
        return MoveResult::Unhandled;
    }
    mRememberedIndex = current;

    if (mProps.layout == Layout::Grid) {
        return navigate_grid(current, direction);
    }
    if ((mProps.layout == Layout::Horizontal && horizontal) ||
        (mProps.layout == Layout::Vertical && vertical))
    {
        return navigate_linear(current, direction);
    }
    return handle_boundary(direction);
}

NavGroup::MoveResult NavGroup::navigate_linear(int current, NavCommand direction) {
    const int step = direction == NavCommand::Left || direction == NavCommand::Up ? -1 : 1;
    if (focus_linear(current + step, step, direction)) {
        return MoveResult::Moved;
    }
    return handle_boundary(direction);
}

NavGroup::MoveResult NavGroup::navigate_grid(int current, NavCommand direction) {
    const int columns = mProps.columns;
    const int row = current / columns;
    const int column = current % columns;
    const int rowStart = row * columns;
    const int rowEnd = std::min(rowStart + columns, static_cast<int>(mItems.size()));

    if (direction == NavCommand::Left || direction == NavCommand::Right) {
        const int step = direction == NavCommand::Left ? -1 : 1;
        const int edge = direction == NavCommand::Left ? rowStart - 1 : rowEnd;
        for (int index = current + step; index != edge; index += step) {
            if (focus_index(index, direction)) {
                return MoveResult::Moved;
            }
        }
        return handle_boundary(direction);
    }

    const int rowCount = (static_cast<int>(mItems.size()) + columns - 1) / columns;
    const int step = direction == NavCommand::Up ? -1 : 1;
    for (int nextRow = row + step; nextRow >= 0 && nextRow < rowCount; nextRow += step) {
        if (focus_grid_row(nextRow, column, direction)) {
            return MoveResult::Moved;
        }
    }
    return handle_boundary(direction);
}

NavGroup::MoveResult NavGroup::handle_boundary(NavCommand direction) {
    switch (boundary_for(direction)) {
    case Boundary::Bubble:
        return MoveResult::Unhandled;
    case Boundary::Stop:
        return MoveResult::Consumed;
    }
    return MoveResult::Unhandled;
}

bool NavGroup::focus_index(int index, NavCommand direction) {
    if (index < 0 || index >= static_cast<int>(mItems.size())) {
        return false;
    }
    if (!mItems[index]->focus_from(direction)) {
        return false;
    }
    mRememberedIndex = index;
    return true;
}

bool NavGroup::focus_linear(int start, int step, NavCommand direction) {
    for (int index = start; index >= 0 && index < static_cast<int>(mItems.size()); index += step) {
        if (focus_index(index, direction)) {
            return true;
        }
    }
    return false;
}

bool NavGroup::focus_grid_row(int row, int preferredColumn, NavCommand direction) {
    const int rowStart = row * mProps.columns;
    const int rowEnd = std::min(rowStart + mProps.columns, static_cast<int>(mItems.size()));
    if (rowStart >= rowEnd) {
        return false;
    }

    const int column = std::min(preferredColumn, rowEnd - rowStart - 1);
    if (focus_index(rowStart + column, direction)) {
        return true;
    }
    for (int distance = 1; distance < mProps.columns; ++distance) {
        if (column - distance >= 0 && focus_index(rowStart + column - distance, direction)) {
            return true;
        }
        if (rowStart + column + distance < rowEnd &&
            focus_index(rowStart + column + distance, direction))
        {
            return true;
        }
    }
    return false;
}

int NavGroup::item_index(Rml::Element* target) const {
    for (int index = 0; index < static_cast<int>(mItems.size()); ++index) {
        if (mItems[index]->contains(target)) {
            return index;
        }
    }
    return -1;
}

NavGroup::Boundary NavGroup::boundary_for(NavCommand direction) const {
    if (direction == NavCommand::Left || direction == NavCommand::Right) {
        return mProps.horizontalBoundary;
    }
    return mProps.verticalBoundary;
}

}  // namespace dusk::ui
