#include "rando_seed_generation.hpp"

#include <mods/svc/log.hpp>

#include "../session.hpp"
#include "../randomizer_context.hpp"

#include "m_Do/m_Do_audio.h"

#include <thread>
#include <atomic>
#include <string>

namespace randomizer::ui {
enum class SeedGenerateStatus {
    Ready,
    Generating,
    Success,
    Error,
};

UiDialogHandle seedGenDialog{0};
static std::atomic seedGenStatus = SeedGenerateStatus::Ready;
static std::string generationStatusMsg{};

void OnDialogActionOK(ModContext* ctx, UiDialogHandle dialogHandle, void*) {
    mDoAud_seStartMenu(Z2SE_SY_MENU_BACK);
    if (seedGenDialog == dialogHandle) {
        seedGenDialog = 0;
    }
    session::svc_mng.ui->dialog_close(ctx, dialogHandle);
}

static void StartSeedGeneration() {
    if (GenerateAndWriteSeed(generationStatusMsg)) {
        seedGenStatus.store(SeedGenerateStatus::Success);
    } else {
        seedGenStatus.store(SeedGenerateStatus::Error);
    }

    mods::log::debug("{}", generationStatusMsg);
}

static ModResult buildDialog() {
    UiDialogDesc desc = UI_DIALOG_DESC_INIT;
    desc.title = "Randomizer";
    desc.body_rml = "Generating Seed...";
    desc.icon = "verifying";
    desc.variant = UI_DIALOG_NORMAL;

    UiDialogAction action = {
        .label = "OK",
        .on_pressed = OnDialogActionOK,
        .user_data = nullptr,
        .keep_open = false,
    };
    desc.actions = &action;
    desc.action_count = 1;

    if (session::svc_mng.ui->dialog_push(session::svc_mng.mod_ctx, &desc, &seedGenDialog) != MOD_OK) {
        mods::log::error("Failed to push dialog");
        return MOD_ERROR;
    }

    return MOD_OK;
}

void GenerateRandomizerSeed() {
    if (seedGenStatus.load() != SeedGenerateStatus::Ready) {
        return;
    }
    if (buildDialog() != MOD_OK) {
        return;
    }

    // Start generation thread
    seedGenStatus.store(SeedGenerateStatus::Generating);
    std::thread rando_gen_thread(StartSeedGeneration);
    rando_gen_thread.detach();
}

ModResult UpdateSeedGenerationDialog() {
    if (seedGenDialog == 0) {
        const auto status = seedGenStatus.load();
        if (status == SeedGenerateStatus::Success || status == SeedGenerateStatus::Error) {
            seedGenStatus.store(SeedGenerateStatus::Ready);
        }
        return MOD_OK;
    }

    auto curSeedGenStatus = seedGenStatus.load();

    // Change the modal text if we've finished attempting to generate
    if (curSeedGenStatus == SeedGenerateStatus::Success ||
        curSeedGenStatus == SeedGenerateStatus::Error)
    {
        auto* ctx = session::svc_mng.mod_ctx;
        auto* ui_svc = session::svc_mng.ui;

        if (curSeedGenStatus == SeedGenerateStatus::Success) {
            mDoAud_seStartMenu(Z2SE_SY_FILE_SAVE_OK);
            ui_svc->dialog_set_icon(ctx, seedGenDialog, "celebration");
        } else {
            mDoAud_seStartMenu(Z2SE_SYS_RESULT_WRONG);
            ui_svc->dialog_set_icon(ctx, seedGenDialog, "error");
        }

        ui_svc->dialog_set_body(ctx, seedGenDialog, generationStatusMsg.c_str());
        seedGenStatus.store(SeedGenerateStatus::Ready);
    }

    return MOD_OK;
}
}
