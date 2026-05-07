#pragma once

#include "document.hpp"

#include <chrono>

namespace dusk::ui {

class Overlay : public Document {
public:
    Overlay();

    void show() override;
    void update() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

    Rml::Element* mCurrentToast = nullptr;
    clock::time_point mCurrentToastStartTime;
};

}  // namespace dusk::ui
