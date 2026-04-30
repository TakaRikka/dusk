#pragma once

#include "button.hpp"
#include "document.hpp"
#include "tab_bar.hpp"

#include <memory>

namespace dusk::ui {

class Popup : public Document {
public:
    Popup();

    Popup(const Popup&) = delete;
    Popup& operator=(const Popup&) = delete;

    void show() override;
    void hide() override;
    bool focus() override;

    void toggle();
    bool is_visible() const;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    Rml::Element* mRoot;
    std::unique_ptr<TabBar> mTabBar;
    std::unique_ptr<Button> mCloseButton;
    bool mVisible = false;
};

}  // namespace dusk::ui
