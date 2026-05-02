#include "prelaunch.hpp"

#include "dusk/config.hpp"
#include "dusk/file_select.hpp"
#include "dusk/iso_validate.hpp"
#include "dusk/main.h"
#include "dusk/ui/prelaunch_options.hpp"
#include "version.h"

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_filesystem.h>
#include <aurora/lib/window.hpp>

namespace dusk::ui {
namespace {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/prelaunch.rcss" />
</head>
<body>
    <content id="root" open>
        <menu>
            <hero class="intro-item delay-0">
                <div class="eyebrow"><span>Twilit Realm</span> presents</div>
                <img src="res/logo-mascot.png" />
            </hero>
            <div id="menu-list" />
        </menu>
        <disk-status class="intro-item delay-4">
            <span id="status" class="status" />
            <span id="detail" class="detail" />
        </disk-status>
        <version-info class="intro-item delay-5">
            <div class="version">Version <span id="version-text"></span></div>
            <div class="update"><span>Update available!</span> Download</div>
        </version-info>
    </content>
</body>
</rml>
)RML";

constexpr std::array<SDL_DialogFileFilter, 2> kDiscFileFilters{{
    {"Game Disc Images", "iso;gcm;ciso;gcz;nfs;rvz;wbfs;wia;tgc"},
    {"All Files", "*"},
}};

void file_dialog_callback(void*, const char* path, const char* error) {
    auto& state = prelaunch_state();
    if (error != nullptr) {
        return;
    }
    if (path == nullptr) {
        return;
    }

    state.selectedIsoPath = path;
    state.errorString.clear();
    refresh_path_state();
    getSettings().backend.isoPath.setValue(state.selectedIsoPath);
    config::Save();
}

}  // namespace

PrelaunchState sPrelaunchState;

PrelaunchState& prelaunch_state() noexcept {
    return sPrelaunchState;
}

void refresh_path_state() noexcept {
    auto& state = prelaunch_state();
    state.isPal = !state.selectedIsoPath.empty() && iso::isPal(state.selectedIsoPath.c_str());
}

void ensure_initialized() noexcept {
    auto& state = prelaunch_state();
    if (state.initialized) {
        return;
    }

    state.selectedIsoPath = getSettings().backend.isoPath;
    state.initialGraphicsBackend = getSettings().backend.graphicsBackend;
    state.errorString.clear();
    state.initialized = true;
    refresh_path_state();
}

bool is_selected_path_valid() noexcept {
    return !prelaunch_state().selectedIsoPath.empty() &&
           SDL_GetPathInfo(prelaunch_state().selectedIsoPath.c_str(), nullptr);
}

void open_iso_picker() noexcept {
    ensure_initialized();
    ShowFileSelect(&file_dialog_callback, nullptr, aurora::window::get_sdl_window(),
        kDiscFileFilters.data(), static_cast<int>(kDiscFileFilters.size()), nullptr, false);
}

void apply_intro_animation(Rml::Element* element, const char* delay_class) {
    if (element == nullptr || delay_class == nullptr) {
        return;
    }
    element->SetClass("intro-item", true);
    element->SetClass(delay_class, true);
}

Prelaunch::Prelaunch() : Document(kDocumentSource), mRoot(mDocument->GetElementById("root")) {
    ensure_initialized();

    if (auto* menuList = mDocument->GetElementById("menu-list")) {
        const bool hasValidPath = is_selected_path_valid();
        mMenuButtons.push_back(
            std::make_unique<Button>(menuList, hasValidPath ? "Start Game" : "Select Disk Image"));
        mMenuButtons.back()->on_pressed([] {
            if (!is_selected_path_valid()) {
                open_iso_picker();
                return;
            }
            IsGameLaunched = true;
            pop_document();
        });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-1");

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Options"));
        mMenuButtons.back()->on_pressed(
            [] { push_document(std::make_unique<PrelaunchOptions>()); });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-2");

        mMenuButtons.push_back(std::make_unique<Button>(menuList, "Quit To Desktop"));
        mMenuButtons.back()->on_pressed([] { IsRunning = false; });
        apply_intro_animation(mMenuButtons.back()->root(), "delay-3");
    }

    mDiscStatus = mDocument->GetElementById("status");
    mDiscDetail = mDocument->GetElementById("detail");
    mVersion = mDocument->GetElementById("version-text");

    listen(mDocument, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        auto* target = event.GetTargetElement();
        if (target == nullptr) {
            return;
        }
        if (target == mDocument && !mDocument->HasAttribute("open")) {
            Document::hide();
        } else if (target->GetTagName() == "button") {
            target->SetClass("anim-done", true);
        }
    });
}

void Prelaunch::show() {
    Document::show();
    mDocument->SetAttribute("open", "");
    mRoot->SetAttribute("open", "");
}

void Prelaunch::hide() {
    if (IsGameLaunched) {
        mDocument->RemoveAttribute("open");
    } else {
        mRoot->RemoveAttribute("open");
    }
}

void Prelaunch::update() {
    ensure_initialized();
    refresh_path_state();

    if (!mEntranceAnimationStarted && mDocument != nullptr) {
        mDocument->SetClass("animate-in", true);
        mEntranceAnimationStarted = true;
    }

    auto& state = prelaunch_state();
    const bool hasValidPath = is_selected_path_valid();
    if (hasValidPath && getSettings().backend.skipPreLaunchUI) {
        pop_document();
        IsGameLaunched = true;
    }

    if (!mMenuButtons.empty()) {
        mMenuButtons[0]->set_text(hasValidPath ? "Start Game" : "Select Disk Image");
    }
    if (mDiscStatus != nullptr) {
        if (hasValidPath) {
            mDiscStatus->RemoveAttribute("bad");
            mDiscStatus->SetInnerRML("Disc Ready");
        } else {
            mDiscStatus->SetAttribute("bad", "");
            mDiscStatus->SetInnerRML("Disk Not Found");
        }
    }
    if (mDiscDetail != nullptr) {
        if (hasValidPath) {
            mDiscDetail->SetProperty(Rml::PropertyId::Display, Rml::Style::Display::Block);
            mDiscDetail->SetInnerRML(prelaunch_state().isPal ? "GameCube • PAL" : "GameCube • USA");
        } else {
            mDiscDetail->SetProperty(Rml::PropertyId::Display, Rml::Style::Display::None);
        }
    }
    if (mVersion != nullptr) {
        mVersion->SetInnerRML(escape(DUSK_WC_DESCRIBE));
    }

    Document::update();
}

bool Prelaunch::focus() {
    if (mMenuButtons.empty()) {
        return false;
    }
    return mMenuButtons[0]->focus();
}

bool Prelaunch::visible() const {
    return mDocument->HasAttribute("open") && mRoot->HasAttribute("open");
}

bool Prelaunch::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    int direction = 0;
    if (cmd == NavCommand::Down) {
        direction = 1;
    } else if (cmd == NavCommand::Up) {
        direction = -1;
    } else {
        return false;
    }
    auto* target = event.GetTargetElement();
    int focusedButton = -1;
    for (size_t i = 0; i < mMenuButtons.size(); ++i) {
        if (mMenuButtons[i]->contains(target)) {
            focusedButton = i;
            break;
        }
    }
    int i = (focusedButton + direction) % mMenuButtons.size();
    while (i >= 0 && i < mMenuButtons.size()) {
        if (mMenuButtons[i]->focus()) {
            event.StopPropagation();
            return true;
        }
        i += direction;
    }
    return false;
}

}  // namespace dusk::ui
