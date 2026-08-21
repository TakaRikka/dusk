#pragma once

#include "component.hpp"

namespace dusk::ui {

class NavGroup : public Component {
public:
    enum class Layout {
        Horizontal,
        Vertical,
        Grid,
    };
    enum class Boundary {
        Bubble,
        Stop,
    };
    struct Props {
        Layout layout = Layout::Vertical;
        int columns = 1;
        Boundary horizontalBoundary = Boundary::Bubble;
        Boundary verticalBoundary = Boundary::Bubble;
    };

    NavGroup(Rml::Element* root, Props props);

    bool focus() override;
    bool focus_from(NavCommand direction) override;

    template <typename T, typename... Args>
        requires std::is_base_of_v<Component, T>
    T& add_item(Args&&... args) {
        auto child = std::make_unique<T>(mRoot, std::forward<Args>(args)...);
        return add_item(std::move(child));
    }

    template <typename T, typename... Args>
        requires std::is_base_of_v<Component, T>
    T& add_existing_item(Rml::Element* root, Args&&... args) {
        auto child = std::make_unique<T>(root, std::forward<Args>(args)...);
        return add_item(std::move(child));
    }

private:
    template <typename T>
    T& add_item(std::unique_ptr<T> child) {
        T& ref = *child;
        mItems.push_back(&ref);
        mChildren.emplace_back(std::move(child));
        return ref;
    }
    enum class MoveResult {
        Unhandled,
        Consumed,
        Moved,
    };

    MoveResult navigate(Rml::Element* target, NavCommand direction);
    MoveResult navigate_linear(int current, NavCommand direction);
    MoveResult navigate_grid(int current, NavCommand direction);
    MoveResult handle_boundary(NavCommand direction);
    bool focus_index(int index, NavCommand direction);
    bool focus_linear(int start, int step, NavCommand direction);
    bool focus_grid_row(int row, int preferredColumn, NavCommand direction);
    int item_index(Rml::Element* target) const;
    Boundary boundary_for(NavCommand direction) const;

    Props mProps;
    std::vector<Component*> mItems;
    int mRememberedIndex = -1;
};

}  // namespace dusk::ui
