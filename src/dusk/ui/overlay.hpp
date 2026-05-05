#pragma once

#include "document.hpp"

#include <chrono>

namespace dusk::ui {

using clock = std::chrono::steady_clock;

class Overlay : public Document {
public:
    Overlay();

    void show() override;
    void update() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

};

}  // namespace dusk::ui
