#pragma once

#include "modal.hpp"
#include "window.hpp"

namespace dusk::ui {

class PrelaunchOptions : public Window {
public:
    PrelaunchOptions();
    void update() override;
    void hide(bool close) override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd);
    bool consume_close_request() override;

private:
    void push_modal(Modal::Props props);
};

}  // namespace dusk::ui
