#include "logging.hpp"

#include "mods/service.hpp"
#include "mods/svc/gfx.h"
#include "mods/svc/log.hpp"
#include "mods/svc/ui.h"
#include "mods/svc/window.h"

#include <cmath>
#include <cstring>
#include <webgpu/webgpu.h>

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(WindowService, svc_window);
IMPORT_SERVICE(GfxService, svc_gfx);

namespace {

WindowHandle g_window = 0;
GfxPresentTargetHandle g_presentTarget = 0;
GfxStageHookHandle g_stageHook = 0;
uint32_t g_frame = 0;
bool g_recreatePresentTarget = false;

struct ClearPayload {
    float red;
    float green;
    float blue;
    float alpha;
};
static_assert(sizeof(ClearPayload) <= GFX_INLINE_DRAW_PAYLOAD_SIZE);

ModResult close_window() {
    g_recreatePresentTarget = false;
    if (g_presentTarget != 0) {
        const auto result = svc_gfx->unregister_present_target(mod_ctx, g_presentTarget);
        if (result != MOD_OK) {
            return result;
        }
        g_presentTarget = 0;
    }
    if (g_window != 0) {
        const auto result = svc_window->destroy_window(mod_ctx, g_window);
        if (result != MOD_OK) {
            return result;
        }
        g_window = 0;
    }
    return MOD_OK;
}

void on_window_event(ModContext*, WindowHandle, const WindowEvent* event, void*) {
    window_demo::log_window_event(event);

    if (event->type == WINDOW_EVENT_CLOSE_REQUESTED) {
        if (close_window() != MOD_OK) {
            mods::log::error("failed to close auxiliary window");
        }
    }
}

// Render worker thread: record a clear of the acquired auxiliary surface texture.
void on_present(
    ModContext*, const GfxPresentContext* ctx, const void* payload, size_t payloadSize, void*) {
    if (payloadSize != sizeof(ClearPayload)) {
        return;
    }
    ClearPayload color;
    std::memcpy(&color, payload, sizeof(color));

    WGPURenderPassColorAttachment colorAttachment = WGPU_RENDER_PASS_COLOR_ATTACHMENT_INIT;
    colorAttachment.view = ctx->target_view;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = WGPUColor{
        color.red,
        color.green,
        color.blue,
        color.alpha,
    };

    WGPURenderPassDescriptor passDesc = WGPU_RENDER_PASS_DESCRIPTOR_INIT;
    passDesc.label = {"Auxiliary window clear", WGPU_STRLEN};
    passDesc.colorAttachmentCount = 1;
    passDesc.colorAttachments = &colorAttachment;
    WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(ctx->encoder, &passDesc);
    wgpuRenderPassEncoderEnd(pass);
    wgpuRenderPassEncoderRelease(pass);
}

ModResult register_present_target() {
    if (g_window == 0 || g_presentTarget != 0) {
        return MOD_CONFLICT;
    }
    GfxPresentTargetDesc presentDesc = GFX_PRESENT_TARGET_DESC_INIT;
    presentDesc.label = "Auxiliary window surface";
    presentDesc.render = on_present;
    presentDesc.preferred_alpha_mode =
        WGPUCompositeAlphaMode_Premultiplied;  // For transparent window
    return svc_gfx->register_window_present_target(
        mod_ctx, g_window, &presentDesc, &g_presentTarget);
}

ModResult open_window() {
    if (g_window != 0) {
        return MOD_CONFLICT;
    }

    WindowDesc windowDesc = WINDOW_DESC_INIT;
    windowDesc.title = "Mod window";
    windowDesc.width = 640;
    windowDesc.height = 480;
    windowDesc.on_event = on_window_event;
    windowDesc.flags |= WINDOW_FLAG_TRANSPARENT;  // For transparent window
    auto result = svc_window->create_window(mod_ctx, &windowDesc, &g_window);
    if (result != MOD_OK) {
        return result;
    }

    result = register_present_target();
    if (result != MOD_OK) {
        close_window();
        return result;
    }

    result = svc_window->show_window(mod_ctx, g_window);
    if (result != MOD_OK) {
        close_window();
    }
    return result;
}

void on_frame_after_hud(ModContext*, const GfxStageContext*, void*) {
    if (g_presentTarget == 0) {
        return;
    }
    const float phase = static_cast<float>(g_frame++) * 0.015f;
    const ClearPayload color{
        .red = 0.08f + 0.06f * (std::sin(phase) + 1.0f),
        .green = 0.10f + 0.06f * (std::sin(phase + 2.1f) + 1.0f),
        .blue = 0.14f + 0.08f * (std::sin(phase + 4.2f) + 1.0f),
        .alpha = 0.5f,
    };
    if (svc_gfx->push_present(mod_ctx, g_presentTarget, &color, sizeof(color)) == MOD_ERROR) {
        g_recreatePresentTarget = true;
    }
}

void on_toggle_window(ModContext*, void*) {
    if (g_window != 0) {
        if (close_window() != MOD_OK) {
            mods::log::error("failed to close auxiliary window");
        }
        return;
    }
    if (open_window() != MOD_OK) {
        mods::log::error("failed to open auxiliary window");
    }
}

ModResult build_panel(ModContext*, UiElementHandle panel, void*, ModError*) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_BUTTON;
    control.label = "Open / Close Window";
    control.on_pressed = on_toggle_window;
    return svc_ui->pane_add_control(mod_ctx, panel, &control, nullptr);
}

}  // namespace

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    GfxStageHookDesc stageDesc = GFX_STAGE_HOOK_DESC_INIT;
    stageDesc.callback = on_frame_after_hud;
    if (svc_gfx->register_stage_hook(
            mod_ctx, GFX_STAGE_FRAME_AFTER_HUD, &stageDesc, &g_stageHook) != MOD_OK)
    {
        return mods::set_error(error, MOD_ERROR, "failed to register presentation hook");
    }

    UiModsPanelDesc panelDesc = UI_MODS_PANEL_DESC_INIT;
    panelDesc.build = build_panel;
    if (svc_ui->register_mods_panel(mod_ctx, &panelDesc) != MOD_OK) {
        svc_gfx->unregister_stage_hook(mod_ctx, g_stageHook);
        g_stageHook = 0;
        return mods::set_error(error, MOD_ERROR, "failed to register mod panel");
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError* error) {
    if (!g_recreatePresentTarget || g_window == 0) {
        return MOD_OK;
    }
    g_recreatePresentTarget = false;
    if (g_presentTarget != 0) {
        const auto result = svc_gfx->unregister_present_target(mod_ctx, g_presentTarget);
        if (result != MOD_OK) {
            return mods::set_error(error, result, "failed to unregister lost present target");
        }
        g_presentTarget = 0;
    }
    const auto result = register_present_target();
    if (result != MOD_OK) {
        return mods::set_error(error, result, "failed to recreate present target");
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    if (g_stageHook != 0) {
        svc_gfx->unregister_stage_hook(mod_ctx, g_stageHook);
        g_stageHook = 0;
    }
    close_window();
    return MOD_OK;
}
}
