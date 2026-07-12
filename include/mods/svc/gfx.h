#pragma once

#include "mods/api.h"

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
#define GFX_SERVICE_MINOR 0u

/* Maximum size for push_draw payload */
#define GFX_INLINE_DRAW_PAYLOAD_SIZE 128u

/* 0 is never a valid handle. */
typedef uint64_t GfxDrawTypeHandle;
typedef uint64_t GfxStageHookHandle;
typedef uint64_t GfxComputeTypeHandle;

/* A suballocation in one of the shared per-frame streaming buffers. */
typedef struct GfxRange {
    uint32_t offset;
    uint32_t size;
} GfxRange;

/*
 * Device and scene pass configuration. Valid from mod_initialize onward and stable for the
 * session. Offscreen passes from create_pass are always single-sample.
 */
typedef struct GfxDeviceInfo {
    uint32_t struct_size;
    WGPUDevice device;              /* borrowed */
    WGPUQueue queue;                /* borrowed */
    WGPUTextureFormat color_format; /* scene color target format */
    WGPUTextureFormat depth_format; /* scene depth target format */
    uint32_t sample_count;          /* scene pass MSAA sample count */
    bool uses_reversed_z;           /* true means depth 1.0 is near */
} GfxDeviceInfo;

#define GFX_DEVICE_INFO_INIT                                                                       \
    {sizeof(GfxDeviceInfo), NULL, NULL, WGPUTextureFormat_Undefined, WGPUTextureFormat_Undefined,  \
        1u, false}

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
    WGPUTextureFormat color_format;
    WGPUTextureFormat depth_format;
    uint32_t sample_count;
    uint32_t target_width;
    uint32_t target_height;
    bool uses_reversed_z;
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
} GfxService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<GfxService> {
    static constexpr const char* id = GFX_SERVICE_ID;
    static constexpr uint16_t major_version = GFX_SERVICE_MAJOR;
};
#endif
