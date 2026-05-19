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
    title->SetInnerRML("[SEND_CRASH_REPORTS]");

    auto* headIcon = append(header, "icon");
    headIcon->SetClass("question-mark", true);

    auto* intro = append(mDialog, "div");
    intro->SetClass("modal-body", true);
    intro->SetInnerRML("[CRASH_REPORTING_INTRO]");

    auto* grid = append(mDialog, "div");
    grid->SetClass("preset-grid", true);

    struct OptionInfo {
        const char* name;
        const char* desc;
        void (*apply)();
    };

    static constexpr OptionInfo kOptions[] = {
        {"[ENABLE]",
            "[SEND_CRASH_REPORTS_TO_DUSKLIGHT_DEVELOPERS_REPORTS_WILL_INCLUDE_THE_INFOR]",
            [] { crash_reporting::set_consent(true); }},
        {"[DISABLE]",
            "[DO_NOT_SEND_CRASH_REPORTS_THIS_MAY_MAKE_IT_MORE_DIFFICULT_TO_RESOLVE_ISS]",
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
