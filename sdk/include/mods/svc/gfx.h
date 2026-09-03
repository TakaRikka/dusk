#pragma once

#include <mods/api.h>
#include <mods/svc/window.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#if !defined(DUSK_BUILDING_GAME) && !defined(DUSK_MOD_FEATURE_WEBGPU)
#error "mods/svc/gfx.h requires add_mod(... FEATURES webgpu)"
#endif

#include <webgpu/webgpu.h>

/*
 * Direct WebGPU access at various stages of the rendering pipeline. Mods use the wgpu* C API
 * (via webgpu/webgpu.h) for custom draws and compute dispatches.
 *
 * Every service function must be called on the game thread. GfxStageFn callbacks run on the game
 * thread during frame recording. push_draw, push_* and pass functions are valid from a stage
 * callback and anywhere else GX commands are being recorded.
 *
 * GfxDrawFn and GfxComputeFn callbacks run on the render worker thread while the frame is encoded.
 * They may use only the handles in their context struct and raw wgpu* calls; no other service may
 * be called from them.
 *
 * All WGPU handles provided by this service are borrowed. Handles in callback contexts are valid
 * only for the duration of the callback; views in GfxResolvedTargets are valid for the current
 * frame only. GPU objects a mod creates through raw wgpu calls are its own responsibility and
 * should be released in mod_shutdown. The device outlives all mods.
 */

#define GFX_SERVICE_ID "dev.twilitrealm.dusklight.gfx"
#define GFX_SERVICE_MAJOR 1u
#define GFX_SERVICE_MINOR 2u

/* Maximum size for push_draw payload */
#define GFX_INLINE_DRAW_PAYLOAD_SIZE 128u
#define GFX_MAX_COLOR_ATTACHMENTS 8u
#define GFX_SCENE_COLOR_ATTACHMENT_INDEX 0u

typedef enum GfxAttachmentSemantic {
    GFX_ATTACHMENT_SCENE_COLOR,
    GFX_ATTACHMENT_NORMAL,
    GFX_ATTACHMENT_AUXILIARY,
} GfxAttachmentSemantic;

typedef struct GfxColorAttachmentLayout {
    GfxAttachmentSemantic semantic;
    WGPUTextureFormat format;
    uint32_t width;
    uint32_t height;
} GfxColorAttachmentLayout;

typedef struct GfxRenderTargetLayout {
    uint32_t struct_size;
    uint64_t key;
    /* At least one; scene color is at GFX_SCENE_COLOR_ATTACHMENT_INDEX. */
    uint32_t color_attachment_count;
    GfxColorAttachmentLayout color_attachments[GFX_MAX_COLOR_ATTACHMENTS];
    WGPUTextureFormat depth_stencil_format;
    uint32_t sample_count;
} GfxRenderTargetLayout;

#define GFX_RENDER_TARGET_LAYOUT_INIT                                                              \
    {sizeof(GfxRenderTargetLayout), 0u, 0u,                                                        \
        {{GFX_ATTACHMENT_AUXILIARY, WGPUTextureFormat_Undefined}}, WGPUTextureFormat_Undefined,    \
        1u}

/*
 * Initializes pipeline color targets for a render-target layout. Only scene color is writable;
 * callers that write another semantic should override that target afterward.
 */
static uint32_t gfx_init_color_target_states(const GfxRenderTargetLayout* layout,
    WGPUColorTargetState targets[GFX_MAX_COLOR_ATTACHMENTS], const WGPUBlendState* scene_blend,
    WGPUColorWriteMask scene_write_mask) {
    if (layout == NULL || targets == NULL) {
        return 0;
    }
    const uint32_t count = layout->color_attachment_count < GFX_MAX_COLOR_ATTACHMENTS ?
                               layout->color_attachment_count :
                               GFX_MAX_COLOR_ATTACHMENTS;
    for (uint32_t i = 0; i < GFX_MAX_COLOR_ATTACHMENTS; ++i) {
        WGPUColorTargetState target = WGPU_COLOR_TARGET_STATE_INIT;
        if (i < count) {
            target.format = layout->color_attachments[i].format;
            target.writeMask = WGPUColorWriteMask_None;
        }
        targets[i] = target;
    }
    if (count != 0) {
        targets[GFX_SCENE_COLOR_ATTACHMENT_INDEX].blend = scene_blend;
        targets[GFX_SCENE_COLOR_ATTACHMENT_INDEX].writeMask = scene_write_mask;
    }
    return count;
}

/* 0 is never a valid handle. */
typedef uint64_t GfxDrawTypeHandle;
typedef uint64_t GfxStageHookHandle;
typedef uint64_t GfxComputeTypeHandle;
typedef uint64_t GfxPresentTargetHandle;

/* A suballocation in one of the shared per-frame streaming buffers. */
typedef struct GfxRange {
    uint32_t offset;
    uint32_t size;
} GfxRange;

/*
 * Device and legacy primary scene-pass configuration. Use get_scene_target_layout when creating
 * scene pipelines. Offscreen passes from create_pass are always single-sample.
 */
typedef struct GfxDeviceInfo {
    uint32_t struct_size;
    WGPUDevice device;              /* borrowed */
    WGPUQueue queue;                /* borrowed */
    WGPUTextureFormat color_format; /* scene color target format */
    WGPUTextureFormat depth_format; /* scene depth target format */
    uint32_t sample_count;          /* scene pass MSAA sample count */
    bool uses_reversed_z;           /* true means depth 1.0 is near */
    WGPUInstance instance;          /* borrowed; added in GfxService 1.1 */
    WGPUAdapter adapter;            /* borrowed; added in GfxService 1.1 */
} GfxDeviceInfo;

#define GFX_DEVICE_INFO_INIT                                                                       \
    {sizeof(GfxDeviceInfo), NULL, NULL, WGPUTextureFormat_Undefined, WGPUTextureFormat_Undefined,  \
        1u, false, NULL, NULL}

/*
 * Passed to GfxDrawFn on the render worker thread; valid only during the call. The pass pipeline,
 * bind group, viewport, and scissor state is restored by the host after the callback returns.
 */
typedef struct GfxDrawContext {
    uint32_t struct_size;
    WGPUDevice device;
    WGPUQueue queue;
    WGPURenderPassEncoder pass;
    WGPUBuffer vertex_buffer;
    WGPUBuffer index_buffer;
    WGPUBuffer uniform_buffer;
    WGPUBuffer storage_buffer;
    /* deprecated: use layout.color_attachments[GFX_SCENE_COLOR_ATTACHMENT_INDEX].format */
    WGPUTextureFormat color_format;
    /* deprecated: use layout.depth_stencil_format */
    WGPUTextureFormat depth_format;
    /* deprecated: use layout.sample_count */
    uint32_t sample_count;
    /* deprecated: use layout.color_attachments[GFX_SCENE_COLOR_ATTACHMENT_INDEX].width */
    uint32_t target_width;
    /* deprecated: use layout.color_attachments[GFX_SCENE_COLOR_ATTACHMENT_INDEX].height */
    uint32_t target_height;
    bool uses_reversed_z;
    GfxRenderTargetLayout layout; /* added in GfxService 1.2 */
} GfxDrawContext;

typedef void (*GfxDrawFn)(ModContext* ctx, const GfxDrawContext* draw_ctx, const void* payload,
    size_t payload_size, void* user_data);

typedef struct GfxDrawTypeDesc {
    uint32_t struct_size;
    const char* label; /* optional debug label */
    GfxDrawFn draw;    /* required; called from the render worker thread */
    void* user_data;
} GfxDrawTypeDesc;

#define GFX_DRAW_TYPE_DESC_INIT {sizeof(GfxDrawTypeDesc), NULL, NULL, NULL}

typedef enum GfxStage {
    GFX_STAGE_SCENE_AFTER_TERRAIN = 0,
    GFX_STAGE_FRAME_BEFORE_HUD = 1,
    GFX_STAGE_FRAME_AFTER_HUD = 2,
    GFX_STAGE_SCENE_BEGIN = 3,
    GFX_STAGE_SCENE_AFTER_OPAQUE = 4,
} GfxStage;

typedef struct GfxStageContext {
    uint32_t struct_size;
    GfxStage stage;
    const void* game_view;     /* view_class* for world-camera stages; NULL otherwise */
    const void* game_viewport; /* view_port_class* for world-camera stages; NULL otherwise */
} GfxStageContext;

typedef void (*GfxStageFn)(ModContext* ctx, const GfxStageContext* stage_ctx, void* user_data);

typedef struct GfxStageHookDesc {
    uint32_t struct_size;
    GfxStageFn callback; /* required */
    void* user_data;
} GfxStageHookDesc;

#define GFX_STAGE_HOOK_DESC_INIT {sizeof(GfxStageHookDesc), NULL, NULL}

typedef struct GfxResolveDesc {
    uint32_t struct_size;
    bool color;
    bool depth;
} GfxResolveDesc;

#define GFX_RESOLVE_DESC_INIT {sizeof(GfxResolveDesc), true, false}

typedef struct GfxResolvedTargets {
    uint32_t struct_size;
    WGPUTextureView color; /* single-sample snapshot in color_format */
    WGPUTextureView depth; /* single-sample raw depth snapshot, R32Float when available */
    WGPUTextureFormat color_format;
    uint32_t width;
    uint32_t height;
} GfxResolvedTargets;

#define GFX_RESOLVED_TARGETS_INIT                                                                  \
    {sizeof(GfxResolvedTargets), NULL, NULL, WGPUTextureFormat_Undefined, 0u, 0u}

/*
 * Passed to GfxComputeFn on the render worker thread; valid only during the call. The encoder is
 * the frame command encoder between scene render passes. Leave no pass open and never finish or
 * release the encoder.
 */
typedef struct GfxComputeContext {
    uint32_t struct_size;
    WGPUDevice device;
    WGPUQueue queue;
    WGPUCommandEncoder encoder;
    WGPUBuffer vertex_buffer;
    WGPUBuffer index_buffer;
    WGPUBuffer uniform_buffer;
    WGPUBuffer storage_buffer;
} GfxComputeContext;

typedef void (*GfxComputeFn)(ModContext* ctx, const GfxComputeContext* compute_ctx,
    const void* payload, size_t payload_size, void* user_data);

typedef struct GfxComputeTypeDesc {
    uint32_t struct_size;
    const char* label;     /* optional debug label */
    GfxComputeFn callback; /* required; called from the render worker thread */
    void* user_data;
} GfxComputeTypeDesc;

#define GFX_COMPUTE_TYPE_DESC_INIT {sizeof(GfxComputeTypeDesc), NULL, NULL, NULL}

/*
 * Invoked on the render worker while the frame encoder is open. The target texture and view have
 * been acquired by the host and are borrowed for the callback. Record all target work on encoder,
 * leave no pass open, and do not finish, submit, or present it. The host submits the shared command
 * buffer and presents the target after submission. The streaming buffers contain data appended on
 * the game thread before push_present.
 */
typedef struct GfxPresentContext {
    uint32_t struct_size;
    WGPUDevice device;
    WGPUQueue queue;
    WGPUCommandEncoder encoder;
    WGPUTexture target_texture;
    WGPUTextureView target_view;
    WGPUTextureFormat target_format;
    uint32_t target_width;
    uint32_t target_height;
    WGPUBuffer vertex_buffer;
    WGPUBuffer index_buffer;
    WGPUBuffer uniform_buffer;
    WGPUBuffer storage_buffer;
} GfxPresentContext;

typedef void (*GfxPresentFn)(ModContext* ctx, const GfxPresentContext* present_ctx,
    const void* payload, size_t payload_size, void* user_data);

typedef struct GfxPresentTargetDesc {
    uint32_t struct_size;
    const char* label; /* optional debug label */
    uint32_t width;    /* required for raw surfaces; ignored for WindowService windows */
    uint32_t height;
    WGPUTextureUsage usage; /* 0 defaults to RenderAttachment */
    WGPUTextureFormat preferred_format;
    WGPUCompositeAlphaMode preferred_alpha_mode;
    GfxPresentFn render;
    void* user_data;
} GfxPresentTargetDesc;

#define GFX_PRESENT_TARGET_DESC_INIT                                                               \
    {sizeof(GfxPresentTargetDesc), NULL, 0u, 0u, WGPUTextureUsage_None,                            \
        WGPUTextureFormat_Undefined, WGPUCompositeAlphaMode_Auto, NULL, NULL}

typedef struct GfxService {
    ServiceHeader header;

    ModResult (*get_device_info)(ModContext* ctx, GfxDeviceInfo* out_info);
    void* (*get_proc_address)(ModContext* ctx, const char* name);

    ModResult (*register_draw_type)(
        ModContext* ctx, const GfxDrawTypeDesc* desc, GfxDrawTypeHandle* out_handle);
    ModResult (*unregister_draw_type)(ModContext* ctx, GfxDrawTypeHandle handle);
    ModResult (*push_draw)(
        ModContext* ctx, GfxDrawTypeHandle handle, const void* payload, size_t payload_size);

    ModResult (*register_compute_type)(
        ModContext* ctx, const GfxComputeTypeDesc* desc, GfxComputeTypeHandle* out_handle);
    ModResult (*unregister_compute_type)(ModContext* ctx, GfxComputeTypeHandle handle);
    ModResult (*push_compute)(
        ModContext* ctx, GfxComputeTypeHandle handle, const void* payload, size_t payload_size);

    ModResult (*push_verts)(
        ModContext* ctx, const void* data, size_t size, size_t alignment, GfxRange* out_range);
    ModResult (*push_indices)(
        ModContext* ctx, const void* data, size_t size, size_t alignment, GfxRange* out_range);
    ModResult (*push_uniform)(ModContext* ctx, const void* data, size_t size, GfxRange* out_range);
    ModResult (*push_storage)(ModContext* ctx, const void* data, size_t size, GfxRange* out_range);

    ModResult (*register_stage_hook)(ModContext* ctx, GfxStage stage, const GfxStageHookDesc* desc,
        GfxStageHookHandle* out_handle);
    ModResult (*unregister_stage_hook)(ModContext* ctx, GfxStageHookHandle handle);

    ModResult (*resolve_pass)(
        ModContext* ctx, const GfxResolveDesc* desc, GfxResolvedTargets* out_targets);
    ModResult (*create_pass)(ModContext* ctx, uint32_t width, uint32_t height);

    /* Minor version 1 */

    ModResult (*register_present_target)(ModContext* ctx, WGPUSurface surface,
        const GfxPresentTargetDesc* desc, GfxPresentTargetHandle* out_handle);
    ModResult (*register_window_present_target)(ModContext* ctx, WindowHandle window,
        const GfxPresentTargetDesc* desc, GfxPresentTargetHandle* out_handle);
    /* Raw-surface targets only; WindowService target resizes are managed automatically. */
    ModResult (*resize_present_target)(
        ModContext* ctx, GfxPresentTargetHandle handle, uint32_t width, uint32_t height);
    ModResult (*unregister_present_target)(ModContext* ctx, GfxPresentTargetHandle handle);
    /*
     * MOD_OK means the task was queued.
     * MOD_UNAVAILABLE means no task could be queued now (for example, a window has no pixel size).
     * MOD_ERROR means an earlier task found the surface lost or deterministically invalid;
     * unregister and recreate the target before pushing again.
     */
    ModResult (*push_present)(
        ModContext* ctx, GfxPresentTargetHandle handle, const void* payload, size_t payload_size);

    /* Minor version 2 */

    ModResult (*get_scene_target_layout)(ModContext* ctx, GfxRenderTargetLayout* out_layout);
} GfxService;

MOD_DECLARE_SERVICE(GfxService, svc_gfx, GFX_SERVICE_ID, GFX_SERVICE_MAJOR, GFX_SERVICE_MINOR);
