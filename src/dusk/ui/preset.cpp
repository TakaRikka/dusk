#include "preset.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "button.hpp"
#include "dusk/config.hpp"
#include "dusk/settings.h"
#include "m_Do/m_Do_audio.h"
#include "ui.hpp"

#include <dolphin/gx/GXAurora.h>

namespace dusk::ui {
namespace {

void applyPresetClassic() {
    auto& s = getSettings();
    s.video.lockAspectRatio.setValue(true);
    s.game.bloomMode.setValue(BloomMode::Classic);
    s.game.enableAchievementNotifications.setValue(false);
    s.game.internalResolutionScale.setValue(1);
    s.game.shadowResolutionMultiplier.setValue(1);
    s.game.hideTvSettingsScreen.setValue(false);
    s.game.skipWarningScreen.setValue(false);
    AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
}

void applyPresetDusk() {
    auto& s = getSettings();
    s.game.hideTvSettingsScreen.setValue(true);
    s.game.skipWarningScreen.setValue(true);
    s.game.noReturnRupees.setValue(true);
    s.game.disableRupeeCutscenes.setValue(true);
    s.game.noSwordRecoil.setValue(true);
    s.game.fastClimbing.setValue(true);
    s.game.noMissClimbing.setValue(true);
    s.game.fastTears.setValue(true);
    s.game.biggerWallets.setValue(true);
    s.game.invertCameraXAxis.setValue(true);
    s.game.no2ndFishForCat.setValue(true);
    s.game.enableAchievementNotifications.setValue(true);
    s.game.enableQuickTransform.setValue(true);
    s.game.instantSaves.setValue(true);
    s.game.midnasLamentNonStop.setValue(true);
    s.game.enableFrameInterpolation.setValue(true);
    s.game.sunsSong.setValue(true);
    s.game.bloomMode.setValue(BloomMode::Dusk);
    s.game.internalResolutionScale.setValue(0);
    s.game.shadowResolutionMultiplier.setValue(4);
    s.game.enableGyroAim.setValue(true);
}

Rml::Element* createElement(Rml::Element* parent, const Rml::String& tag) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement(tag);
    return parent->AppendChild(std::move(elem));
}

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/window.rcss" />
</head>
<body>
    <window id="window" class="small preset">
        <div id="preset-dialog" class="preset-dialog"></div>
    </window>
</body>
</rml>
)RML";

}  // namespace

PresetWindow::PresetWindow()
    : Document(kDocumentSource), mRoot(mDocument->GetElementById("window")) {
    listen(mRoot, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mRoot && !mRoot->HasAttribute("open") &&
            Document::visible()) {
            Document::hide(mPendingClose);
        }
    });

    auto* dialog = mDocument->GetElementById("preset-dialog");

    auto* title = createElement(dialog, "div");
    title->SetClass("preset-title", true);
    title->SetInnerRML("Welcome to Dusk!");

    auto* intro = createElement(dialog, "div");
    intro->SetClass("preset-intro", true);
    intro->SetInnerRML(
        "Choose a preset to get started.<br/>"
        "You can change any setting later from the Settings menu.");

    auto* grid = createElement(dialog, "div");
    grid->SetClass("preset-grid", true);

    struct PresetInfo {
        const char* name;
        const char* desc;
        void (*apply)();
    };

    static constexpr PresetInfo kPresets[] = {
        {"Classic",
         "Enhancements disabled to match the GameCube version. "
         "Good for speedrunning or simple nostalgia!",
         applyPresetClassic},
        {"Dusk",
         "Graphics & quality of life tweaks, including some from the Wii U version. "
         "Our recommended way to play!",
         applyPresetDusk},
    };

    for (const auto& preset : kPresets) {
        auto* col = createElement(grid, "div");
        col->SetClass("preset-col", true);

        auto btn = std::make_unique<Button>(col, Rml::String(preset.name));
        btn->root()->SetClass("preset-btn", true);
        btn->on_nav_command([this, apply = preset.apply](Rml::Event&, NavCommand cmd) {
            if (cmd == NavCommand::Confirm) {
                apply();
                getSettings().backend.wasPresetChosen.setValue(true);
                config::Save();
                hide(true);
                return true;
            }
            return false;
        });
        mButtons.push_back(std::move(btn));

        auto* desc = createElement(col, "div");
        desc->SetClass("preset-desc", true);
        desc->SetInnerRML(preset.desc);
    }
}

void PresetWindow::show() {
    Document::show();
    mRoot->SetAttribute("open", "");
}

void PresetWindow::hide(bool close) {
    mRoot->RemoveAttribute("open");
    mPendingClose = close;
}

bool PresetWindow::visible() const {
    return mRoot->HasAttribute("open");
}

bool PresetWindow::focus() {
    if (!mButtons.empty()) {
        return mButtons.back()->focus();
    }
    return false;
}

bool PresetWindow::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    if (cmd == NavCommand::Cancel || cmd == NavCommand::Menu) {
        return true;
    }
    int direction = 0;
    if (cmd == NavCommand::Left) {
        direction = -1;
    } else if (cmd == NavCommand::Right) {
        direction = 1;
    } else {
        return false;
    }
    auto* target = event.GetTargetElement();
    for (int i = 0; i < static_cast<int>(mButtons.size()); ++i) {
        if (mButtons[i]->contains(target)) {
            const int next = i + direction;
            if (next >= 0 && next < static_cast<int>(mButtons.size())) {
                if (mButtons[next]->focus()) {
                    mDoAud_seStartMenu(Z2SE_SY_NAME_CURSOR);
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

}  // namespace dusk::ui
