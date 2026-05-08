#pragma once

#include "button.hpp"
#include "document.hpp"
#include "dusk/iso_validate.hpp"

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
    bool mRestartSuppressed = false;
    std::vector<std::unique_ptr<Button> > mMenuButtons;
    Rml::Element* mRoot = nullptr;
    Rml::Element* mDiscStatus = nullptr;
    Rml::Element* mDiscDetail = nullptr;
    Rml::Element* mVersion = nullptr;
};

class PrelaunchOptions;

struct PrelaunchState {
    bool initialized = false;
    std::string configuredDiscPath;
    bool configuredDiscCanLaunch = false;
    iso::DiscInfo configuredDiscInfo{};
    iso::ValidationError configuredDiscValidation = iso::ValidationError::Unknown;
    std::string activeDiscPath;
    iso::DiscInfo activeDiscInfo{};
    GameLanguage initialLanguage = GameLanguage::English;
    std::string initialGraphicsBackend;
    int initialCardFileType = 0;
    std::string errorString;
    std::string pendingDiscPath;
    iso::DiscInfo pendingDiscInfo{};
    iso::ValidationError pendingDiscValidation = iso::ValidationError::Unknown;
};

PrelaunchState& prelaunch_state() noexcept;
void ensure_initialized() noexcept;
void refresh_configured_disc_state() noexcept;
void open_iso_picker() noexcept;
bool is_restart_pending() noexcept;
void try_push_verification_modal(Document& host);

}  // namespace dusk::ui
