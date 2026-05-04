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

    void show() override;
    void hide(bool close) override;
    void update() override;
    bool focus() override;
    bool visible() const override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    bool mEntranceAnimationStarted = false;
    std::vector<std::unique_ptr<Button>> mMenuButtons;
    Rml::Element* mRoot = nullptr;
    Rml::Element* mDiscStatus = nullptr;
    Rml::Element* mDiscDetail = nullptr;
    Rml::Element* mVersion = nullptr;
};

class PrelaunchOptions;

struct PrelaunchState {
    bool initialized = false;
    std::string selectedDiscPath;
    std::string initialDiscPath;
    GameLanguage initialLanguage = GameLanguage::English;
    std::string initialGraphicsBackend;
    int initialCardFileType = 0;
    std::string errorString;
    bool selectedDiscIsValid = false;
    bool selectedDiscIsPal = false;
    bool initialDiscIsPal = false;
};

PrelaunchState& prelaunch_state() noexcept;
void ensure_initialized() noexcept;
void refresh_state() noexcept;
void open_iso_picker() noexcept;

}  // namespace dusk::ui
