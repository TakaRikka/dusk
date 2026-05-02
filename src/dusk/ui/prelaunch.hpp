#pragma once

#include "button.hpp"
#include "document.hpp"

#include <memory>
#include <string>
#include <vector>

namespace dusk::ui {

class Prelaunch : public Document {
public:
    Prelaunch();

    void update() override;
    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    bool mEntranceAnimationStarted = false;
    std::vector<std::unique_ptr<Button>> mMenuButtons;
    Rml::Element* mDiscStatus = nullptr;
    Rml::Element* mDiscDetail = nullptr;
    Rml::Element* mVersion = nullptr;
};

class PrelaunchOptions;

struct PrelaunchState {
    std::string selectedIsoPath;
    std::string errorString;
    std::string initialGraphicsBackend;
    bool isPal = false;
    bool initialized = false;
};

PrelaunchState& prelaunch_state() noexcept;
void ensure_initialized() noexcept;
void refresh_path_state() noexcept;
bool is_selected_path_valid() noexcept;
void open_iso_picker() noexcept;

}  // namespace dusk::ui
