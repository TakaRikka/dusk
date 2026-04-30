#pragma once

#include "button.hpp"
#include "document.hpp"
#include "event.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <vector>

namespace dusk::ui {

class Popup : public Document {
public:
    Popup();

    Popup(const Popup&) = delete;
    Popup& operator=(const Popup&) = delete;

    void show() override;
    void hide() override;
    void update() override;

    void toggle();
    bool is_visible() const;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    void set_selected_tab(int index);
    bool focus_tab(int index);

    std::vector<std::unique_ptr<Button> > mTabs;
    std::vector<std::function<void()> > mTabActions;
    std::unique_ptr<Button> mCloseButton;
    int mSelectedTabIndex = 0;
    bool mVisible = false;
    std::optional<std::chrono::steady_clock::time_point> mHideDeadline;
};

}  // namespace dusk::ui
