#include "hooks.hpp"
#include "session.hpp"
#include "randomizer_context.hpp"
#include "ui/rando_config.hpp"

#include <mods/svc/hook.hpp>
#include <mods/svc/log.hpp>

#include "d/d_file_select.h"

DEFINE_HOOK(&dFile_select_c::selectDataNameMove, dFile_select_c__selectDataNameMove);
DEFINE_HOOK(&dFile_select_c::dataSelect, dFile_select_c__dataSelect);

namespace randomizer::ui {
dialogSelectModeState g_dialogSelectModeState = SelectReady;
}

namespace randomizer::hooks {
namespace {
UiDialogHandle playModeDialog{0};

HookAction hookPreDataSelect(ModContext*, void* args, void* retval, void* userdata) {
    ui::g_dialogSelectModeState = ui::SelectReady;
    ui::g_file_select_window_ctx.is_proceed = false;
    return HOOK_CONTINUE;
}

HookAction hookPreSelectDataNameMove(ModContext*, void* args, void* retval, void* userdata) {
    dFile_select_c* i_this = mods::arg<dFile_select_c*>(args, 0);

    // if coming from "start randomizer" button, let transition occur as normal
    if (ui::g_file_select_window_ctx.is_proceed) {
        return HOOK_CONTINUE;
    }

    bool isHeaderTxtChange = i_this->headerTxtChangeAnm();
    bool isFileRecScale = i_this->fileRecScaleAnm2();
    bool isModoruTxtDisp = i_this->modoruTxtDispAnm();

    // if selected vanilla mode, let transition occur as normal
    if (ui::g_dialogSelectModeState == ui::SelectVanilla) {
        return HOOK_CONTINUE;
    }

    if (ui::g_dialogSelectModeState == ui::SelectReady && isHeaderTxtChange == true && isFileRecScale == true && isModoruTxtDisp == true) {
        ui::g_dialogSelectModeState = ui::SelectWait;

        auto buildDialog = [i_this]() {
            UiDialogDesc desc = UI_DIALOG_DESC_INIT;
            desc.title = "Play Type";
            desc.body_rml = "What mode would you like to play?";
            desc.icon = "question-mark";
            desc.variant = UI_DIALOG_NORMAL;

            UiDialogAction actions[2];
            actions[0] = {
                .label = "Vanilla",
                .on_pressed = [](ModContext* ctx, UiDialogHandle dialogHandle, void*) {
                    mDoAud_seStartMenu(Z2SE_SY_CURSOR_OK);
                    randomizer_GetContext() = RandomizerContext();
                    ui::g_dialogSelectModeState = ui::SelectVanilla;
                    session::svc_mng.ui->dialog_close(ctx, dialogHandle);
                },
                .user_data = nullptr,
                .keep_open = false,
            };
            actions[1] = {
                .label = "Randomizer",
                .on_pressed = [](ModContext* ctx, UiDialogHandle dialogHandle, void* userdata) {
                    mDoAud_seStartMenu(Z2SE_SY_CURSOR_OK);
                    ui::g_dialogSelectModeState = ui::SelectRandomizer;
                    ui::buildFileSelectGateMenu(static_cast<dFile_select_c*>(userdata));
                    session::svc_mng.ui->dialog_close(ctx, dialogHandle);
                },
                .user_data = i_this,
                .keep_open = false,
            };
            desc.actions = actions;
            desc.action_count = 2;

            if (session::svc_mng.ui->dialog_push(session::svc_mng.mod_ctx, &desc, &playModeDialog) != MOD_OK) {
                mods::log::error("Failed to push dialog");
                return MOD_ERROR;
            }

            return MOD_OK;
        };

        if (buildDialog() != MOD_OK) {
            mods::log::error("Failed to build dialog");
            return HOOK_CONTINUE;
        }
    }

    return HOOK_SKIP_ORIGINAL;
}
}

ModResult initialize() {
#define ADD_HOOK_PRE(originalFn, hookFn)                             \
    if (mods::hook::add_pre<originalFn>(hookFn) != MOD_OK) {         \
        mods::log::error("Failed to add pre-hook for " #originalFn); \
        return MOD_ERROR;                                            \
    }

    ADD_HOOK_PRE(dFile_select_c__selectDataNameMove, hookPreSelectDataNameMove);
    ADD_HOOK_PRE(dFile_select_c__dataSelect, hookPreDataSelect);

    return MOD_OK;
}
}