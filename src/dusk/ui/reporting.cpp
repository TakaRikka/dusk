#if DUSK_ENABLE_SENTRY_NATIVE

#include "reporting.hpp"

#include "button.hpp"
#include "dusk/crash_reporting.h"
#include "ui.hpp"

#include <dolphin/gx/GXAurora.h>

namespace dusk::ui {

CrashReportWindow::CrashReportWindow() : WindowSmall("modal", "modal-dialog") {
    mDialog->SetClass("modal-dialog", true);

    auto* header = append(mDialog, "div");
    header->SetClass("modal-header", true);

    auto* title = append(header, "div");
    title->SetClass("modal-title", true);
    title->SetInnerRML("Send Crash Reports");

    auto* headIcon = append(header, "icon");
    headIcon->SetClass("question-mark", true);

    auto* intro = append(mDialog, "div");
    intro->SetClass("modal-body", true);
    intro->SetInnerRML(
        "Dusk can automatically send crash reports to the developers. Crash reports contain the "
        "following:"
        "<br/>• Operating system version<br/>• CPU architecture<br/>• GPU model & driver version"
        "<br/>• File paths (may include account username)<br/>• Stack trace<br/><br/>"
        "This can be changed in the Settings menu at any time.");

    auto* grid = append(mDialog, "div");
    grid->SetClass("preset-grid", true);

    struct OptionInfo {
        const char* name;
        const char* desc;
        void (*apply)();
    };

    static constexpr OptionInfo kOptions[] = {
        {"Enable",
            "Send crash reports to Dusk developers. Reports will include the information described "
            "above.",
            [] { crash_reporting::set_consent(true); }},
        {"Disable",
            "Do not send crash reports. This may make it more difficult to resolve issues you "
            "encounter.",
            [] { crash_reporting::set_consent(false); }},
    };

    for (const auto& option : kOptions) {
        auto* col = append(grid, "div");
        col->SetClass("preset-col", true);

        auto btn = std::make_unique<Button>(col, Rml::String(option.name));
        btn->on_nav_command([this, apply = option.apply](Rml::Event&, NavCommand cmd) {
            if (cmd == NavCommand::Confirm) {
                apply();
                hide(true);
                return true;
            }
            return false;
        });
        mButtons.push_back(std::move(btn));

        auto* desc = append(col, "div");
        desc->SetClass("preset-desc", true);
        desc->SetInnerRML(option.desc);
    }
}

bool CrashReportWindow::focus() {
    if (!mButtons.empty()) {
        return mButtons.back()->focus();
    }
    return false;
}

bool CrashReportWindow::handle_nav_command(Rml::Event& event, NavCommand cmd) {
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
                    mDoAud_seStartMenu(kSoundItemFocus);
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

}  // namespace dusk::ui

#endif
