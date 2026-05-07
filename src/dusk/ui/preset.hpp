#pragma once
#include "component.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

namespace dusk::ui {

class PresetWindow : public WindowSmall {
public:
    PresetWindow();

    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    std::vector<std::unique_ptr<Component>> mButtons;
};

}  // namespace dusk::ui
