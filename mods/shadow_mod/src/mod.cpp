// Dynamic shadows example mod.
//
// Replays the game's populated opaque scene draw lists into an offscreen pass with a light-space
// projection to produce a shadow map of the live scene, then composites deferred shadows over the
// world (scene depth + CameraService unproject + PCF against the map).
//
// The optional contact-shadow raymarch in the composite is reimplemented from Panos Karabelas'
// screen-space shadows (MIT, via Spartan Engine); see res/shadow.wgsl.

#include "global.h"

#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DU/J3DUClipper.h"
#include "JSystem/JMath/JMath.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_kankyo_rain.h"
#include "dolphin/gx/GXAurora.h"
#include "dolphin/gx/GXGet.h"
#include "dolphin/gx/GXPixel.h"
#include "dolphin/gx/GXTransform.h"
#include "m_Do/m_Do_mtx.h"
#include "mods/hook.hpp"
#include "mods/service.hpp"
#include "mods/svc/camera.h"
#include "mods/svc/config.h"
#include "mods/svc/game.h"
#include "mods/svc/gfx.h"
#include "mods/svc/hook.h"
#include "mods/svc/log.h"
#include "mods/svc/resource.h"
#include "mods/svc/ui.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <type_traits>
#include <utility>
#include <webgpu/webgpu.h>

DEFINE_MOD();
IMPORT_SERVICE(ConfigService, svc_config);
IMPORT_SERVICE(ResourceService, svc_resource);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(GfxService, svc_gfx);
IMPORT_SERVICE(CameraService, svc_camera);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(GameService, svc_game);
IMPORT_SERVICE(LogService, svc_log);

namespace {

ConfigVarHandle g_cvarEnabled = 0;
ConfigVarHandle g_cvarMapSize = 0;
ConfigVarHandle g_cvarNoFrustumClipping = 0;
ConfigVarHandle g_cvarStrength = 0;
ConfigVarHandle g_cvarPcf = 0;
ConfigVarHandle g_cvarBias = 0;
ConfigVarHandle g_cvarBoxRadius = 0;
ConfigVarHandle g_cvarEdgeFadeWidth = 0;
ConfigVarHandle g_cvarContactShadows = 0;
ConfigVarHandle g_cvarDebugView = 0;

GfxDrawTypeHandle g_drawType = 0;
GfxStageHookHandle g_sceneBeginHook = 0;
GfxStageHookHandle g_sceneAfterTerrainHook = 0;
GfxStageHookHandle g_frameBeforeHudHook = 0;
UiWindowHandle g_controlsWindow = 0;
ResourceBuffer g_shaderSource = RESOURCE_BUFFER_INIT;
GfxDeviceInfo g_deviceInfo = GFX_DEVICE_INFO_INIT;
WGPURenderPipeline g_compositePipeline = nullptr;       // multiply blend
WGPURenderPipeline g_compositeDebugPipeline = nullptr;  // no blend (debug views)
WGPUBindGroupLayout g_compositeLayout = nullptr;
WGPUBindGroupLayout g_compositeDebugLayout = nullptr;

struct MapPassOutput {
    bool ready = false;
    WGPUTextureView shadowMap = nullptr;   // frame-pooled
    WGPUTextureView lightColor = nullptr;  // frame-pooled
    uint32_t mapSize = 0;
    Mtx44 lightVp;             // world -> light receiver projection, row-major game convention
    float dirToLightWorld[3];  // toward the light, normalized
    float fade = 0.0f;
};

MapPassOutput g_mapPass;
bool g_replayingSceneLists = false;

constexpr float kLightDistance = 30000.0f;
constexpr float kLightNear = 100.0f;
constexpr float kLightFar = 60000.0f;
constexpr float kMaxLightLookahead = 10000.0f;
constexpr float kSunMoonDistance = 80000.0f;
constexpr float kSunMoonZDistance = -48000.0f;

using ClipperSphereClip = int (J3DUClipper::*)(f32 const (*)[4], Vec, f32) const;
using ClipperBoxClip = int (J3DUClipper::*)(f32 const (*)[4], Vec*, Vec*) const;
constexpr ClipperSphereClip kClipperSphereClip = static_cast<ClipperSphereClip>(&J3DUClipper::clip);
constexpr ClipperBoxClip kClipperBoxClip = static_cast<ClipperBoxClip>(&J3DUClipper::clip);

// Mirror of the WGSL Uniforms struct (keep in sync with res/shadow.wgsl).
struct ShadowUniforms {
    float world_from_proj[16];
    float view_from_proj[16];
    float proj_from_view[16];
    float light_vp[16];
    float light_dir_view[3];
    float bias;
    float size[2];
    float inv_size[2];
    float edge_fade_width;
    float strength;
    float pcf_taps;
    float contact_enabled;
    float contact_thickness;
    float contact_length;
    uint32_t debug_mode;
    float _pad0;
};
static_assert(sizeof(ShadowUniforms) % 16 == 0);

struct DrawPayload {
    WGPUTextureView sceneDepth;  // frame-pooled
    WGPUTextureView shadowMap;   // frame-pooled
    WGPUTextureView lightColor;  // frame-pooled
    uint32_t uniform_offset;
    uint32_t uniform_size;
    uint32_t debug_mode;
};
static_assert(sizeof(DrawPayload) <= GFX_INLINE_DRAW_PAYLOAD_SIZE);
static_assert(std::is_trivially_copyable_v<DrawPayload>);

struct LightCamera {
    Mtx view;
    Mtx44 ortho;
    Mtx44 vp;
    float dirToLight[3];
    float fade = 0.0f;
};

struct SceneCamera {
    bool valid = false;
    bool raw_valid = false;
    CameraInfo info = CAMERA_INFO_INIT;
    Mtx raw_view;
    f32 raw_projection[7]{};
    Mtx44 raw_projection_mtx;
};

SceneCamera g_sceneCamera;

struct ActualLightDebugState {
    bool active = false;
    Mtx savedView;
    f32 savedProjection[7];
    f32 savedViewport[6];
    u32 savedScissor[4];
};

ActualLightDebugState g_actualLightDebug;

struct replay_scope {
    replay_scope() { g_replayingSceneLists = true; }
    ~replay_scope() { g_replayingSceneLists = false; }
};

int64_t get_int_option(ConfigVarHandle handle, int64_t fallback) {
    int64_t value = fallback;
    if (handle == 0 || svc_config->get_int(mod_ctx, handle, &value) != MOD_OK) {
        return fallback;
    }
    return value;
}

bool get_bool_option(ConfigVarHandle handle, bool fallback) {
    bool value = fallback;
    if (handle == 0 || svc_config->get_bool(mod_ctx, handle, &value) != MOD_OK) {
        return fallback;
    }
    return value;
}

int64_t get_debug_mode() {
    return std::clamp<int64_t>(get_int_option(g_cvarDebugView, 0), 0, 10);
}

bool matrix_ready(const Mtx m) {
    float basis = 0.0f;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (!std::isfinite(m[r][c])) {
                return false;
            }
            if (c < 3) {
                basis += std::fabs(m[r][c]);
            }
        }
    }
    return basis > 0.001f;
}

bool projection_vector_ready(const f32 projection[7]) {
    if (projection[0] != 0.0f) {
        return false;
    }
    for (int i = 1; i < 7; ++i) {
        if (!std::isfinite(projection[i])) {
            return false;
        }
    }
    return std::fabs(projection[1]) > 0.001f && std::fabs(projection[3]) > 0.001f &&
           std::fabs(projection[6]) > 0.001f;
}

// Row-major game matrix -> column-major WGSL layout (matching CameraService).
void store_column_major(const Mtx44 in, float out[16]) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            out[c * 4 + r] = in[r][c];
        }
    }
}

void copy_projection(const Mtx44 in, Mtx44 out) {
    std::memcpy(out, in, sizeof(Mtx44));
    // TODO: check GfxDeviceInfo.uses_reversed_z
    for (int c = 0; c < 4; ++c) {
        out[2][c] = -out[2][c];
    }
}

void projection_vector_from_perspective(const Mtx44 projection, f32 out[7]) {
    out[0] = 0.0f;
    out[1] = projection[0][0];
    out[2] = projection[0][2];
    out[3] = projection[1][1];
    out[4] = projection[1][2];
    out[5] = projection[2][2];
    out[6] = projection[2][3];
}

const view_class* stage_game_view(const GfxStageContext* stageCtx) {
    if (stageCtx == nullptr || stageCtx->struct_size < sizeof(GfxStageContext) ||
        stageCtx->game_view == nullptr)
    {
        return nullptr;
    }
    return static_cast<const view_class*>(stageCtx->game_view);
}

bool capture_raw_camera(
    const view_class* gameView, Mtx outView, Mtx44 outProjectionMtx, f32 outProjection[7]) {
    if (gameView == nullptr || !matrix_ready(gameView->viewMtx)) {
        return false;
    }
    std::memcpy(outProjectionMtx, gameView->projMtx, sizeof(Mtx44));
    projection_vector_from_perspective(outProjectionMtx, outProjection);
    if (!projection_vector_ready(outProjection)) {
        return false;
    }
    cMtx_copy(gameView->viewMtx, outView);
    return true;
}

bool capture_scene_camera(const GfxStageContext* stageCtx) {
    g_sceneCamera.valid = false;
    g_sceneCamera.raw_valid = false;
    const view_class* gameView = stage_game_view(stageCtx);
    if (gameView == nullptr) {
        return false;
    }
    CameraInfo info = CAMERA_INFO_INIT;
    if (svc_camera->get_camera(mod_ctx, stageCtx->game_view, &info) != MOD_OK) {
        return false;
    }
    g_sceneCamera.info = info;
    g_sceneCamera.valid = true;
    g_sceneCamera.raw_valid = capture_raw_camera(gameView, g_sceneCamera.raw_view,
        g_sceneCamera.raw_projection_mtx, g_sceneCamera.raw_projection);
    return true;
}

bool get_replay_camera(Mtx outView, Mtx44 outProjectionMtx, f32 outProjection[7]) {
    if (g_sceneCamera.raw_valid && matrix_ready(g_sceneCamera.raw_view)) {
        cMtx_copy(g_sceneCamera.raw_view, outView);
        std::memcpy(outProjectionMtx, g_sceneCamera.raw_projection_mtx,
            sizeof(g_sceneCamera.raw_projection_mtx));
        std::memcpy(
            outProjection, g_sceneCamera.raw_projection, sizeof(g_sceneCamera.raw_projection));
        return projection_vector_ready(outProjection);
    }

    return false;
}

float wrap_daytime(float daytime) {
    if (!std::isfinite(daytime)) {
        return 180.0f;
    }
    float wrapped = std::fmod(daytime, 360.0f);
    if (wrapped < 0.0f) {
        wrapped += 360.0f;
    }
    return wrapped;
}

float daytime_percent(float max, float min, float value) {
    const float range = max - min;
    if (range == 0.0f) {
        return 1.0f;
    }
    const float percent = 1.0f - ((max - value) / range);
    return percent < 1.0f ? percent : 1.0f;
}

float sun_moon_angle(float daytime) {
    daytime = wrap_daytime(daytime);
    if (daytime >= 90.0f && daytime <= 270.0f) {
        return daytime_percent(270.0f, 90.0f, daytime) * 150.0f + 105.0f;
    }

    float angle = daytime;
    if (angle < 90.0f) {
        angle += 360.0f;
    }

    angle = daytime_percent(450.0f, 270.0f, angle) * 210.0f + 255.0f;
    if (angle > 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

cXyz sun_moon_offset(float daytime) {
    const float angle = DEG_TO_RAD(sun_moon_angle(daytime));
    const float angleSin = sinf(angle);
    const float angleCos = cosf(angle);
    return cXyz{
        angleSin * kSunMoonDistance, -angleCos * kSunMoonDistance, angleCos * kSunMoonZDistance};
}

bool build_composite_pipeline(
    bool blend, WGPURenderPipeline& outPipeline, WGPUBindGroupLayout& outLayout) {
    WGPUShaderSourceWGSL wgsl = WGPU_SHADER_SOURCE_WGSL_INIT;
    wgsl.code = {static_cast<const char*>(g_shaderSource.data), g_shaderSource.size};
    WGPUShaderModuleDescriptor moduleDesc = WGPU_SHADER_MODULE_DESCRIPTOR_INIT;
    moduleDesc.nextInChain = &wgsl.chain;
    moduleDesc.label = {"shadow composite", WGPU_STRLEN};
    WGPUShaderModule module = wgpuDeviceCreateShaderModule(g_deviceInfo.device, &moduleDesc);
    if (module == nullptr) {
        return false;
    }

    // Multiply blend: fragment output is the darkening multiplier (result = dst * src).
    WGPUBlendState blendState{
        .color = {.operation = WGPUBlendOperation_Add,
            .srcFactor = WGPUBlendFactor_Dst,
            .dstFactor = WGPUBlendFactor_Zero},
        .alpha = {.operation = WGPUBlendOperation_Add,
            .srcFactor = WGPUBlendFactor_Zero,
            .dstFactor = WGPUBlendFactor_One},
    };
    WGPUColorTargetState colorTarget = WGPU_COLOR_TARGET_STATE_INIT;
    colorTarget.format = g_deviceInfo.color_format;
    if (blend) {
        colorTarget.blend = &blendState;
    }
    WGPUFragmentState fragment = WGPU_FRAGMENT_STATE_INIT;
    fragment.module = module;
    fragment.entryPoint = {"fs_main", WGPU_STRLEN};
    fragment.targetCount = 1;
    fragment.targets = &colorTarget;
    WGPUDepthStencilState depthStencil = WGPU_DEPTH_STENCIL_STATE_INIT;
    depthStencil.format = g_deviceInfo.depth_format;
    depthStencil.depthWriteEnabled = WGPUOptionalBool_False;
    depthStencil.depthCompare = WGPUCompareFunction_Always;

    WGPURenderPipelineDescriptor pipelineDesc = WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT;
    pipelineDesc.label = {blend ? "shadow composite" : "shadow composite (debug)", WGPU_STRLEN};
    pipelineDesc.vertex.module = module;
    pipelineDesc.vertex.entryPoint = {"vs_main", WGPU_STRLEN};
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.depthStencil = &depthStencil;
    pipelineDesc.multisample.count = g_deviceInfo.sample_count;
    pipelineDesc.fragment = &fragment;
    outPipeline = wgpuDeviceCreateRenderPipeline(g_deviceInfo.device, &pipelineDesc);
    wgpuShaderModuleRelease(module);
    if (outPipeline == nullptr) {
        return false;
    }
    outLayout = wgpuRenderPipelineGetBindGroupLayout(outPipeline, 0);
    return outLayout != nullptr;
}

// Render worker thread: fullscreen deferred-shadow composite.
void on_draw(
    ModContext*, const GfxDrawContext* ctx, const void* payload, size_t payloadSize, void*) {
    if (payloadSize != sizeof(DrawPayload)) {
        return;
    }
    DrawPayload data;
    std::memcpy(&data, payload, sizeof(data));

    WGPURenderPipeline pipeline =
        data.debug_mode != 0 ? g_compositeDebugPipeline : g_compositePipeline;
    WGPUBindGroupLayout layout = data.debug_mode != 0 ? g_compositeDebugLayout : g_compositeLayout;
    if (data.sceneDepth == nullptr || data.shadowMap == nullptr || data.lightColor == nullptr ||
        pipeline == nullptr)
    {
        return;
    }

    WGPUBindGroupEntry entries[4] = {WGPU_BIND_GROUP_ENTRY_INIT, WGPU_BIND_GROUP_ENTRY_INIT,
        WGPU_BIND_GROUP_ENTRY_INIT, WGPU_BIND_GROUP_ENTRY_INIT};
    entries[0].binding = 0;
    entries[0].textureView = data.sceneDepth;
    entries[1].binding = 1;
    entries[1].textureView = data.shadowMap;
    entries[2].binding = 2;
    entries[2].buffer = ctx->uniform_buffer;
    entries[2].offset = data.uniform_offset;
    entries[2].size = data.uniform_size;
    entries[3].binding = 3;
    entries[3].textureView = data.lightColor;
    WGPUBindGroupDescriptor bindGroupDesc = WGPU_BIND_GROUP_DESCRIPTOR_INIT;
    bindGroupDesc.layout = layout;
    bindGroupDesc.entryCount = 4;
    bindGroupDesc.entries = entries;
    WGPUBindGroup bindGroup = wgpuDeviceCreateBindGroup(ctx->device, &bindGroupDesc);
    if (bindGroup == nullptr) {
        return;
    }

    wgpuRenderPassEncoderSetPipeline(ctx->pass, pipeline);
    wgpuRenderPassEncoderSetBindGroup(ctx->pass, 0, bindGroup, 0, nullptr);
    wgpuRenderPassEncoderDraw(ctx->pass, 3, 1, 0, 0);
    wgpuBindGroupRelease(bindGroup);
}

// Picks the sun or moon (whichever is above the horizon) and returns the normalized
// world-space direction *toward* the light plus a horizon fade factor. False = no light.
bool compute_light(float outDirToLight[3], float& outFade) {
    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    if (envLight == nullptr) {
        return false;
    }

    // The packet positions can be stale when this runs before the world lists are consumed.
    // Mirror dScnKy_env_light_c::setSunpos() so --time-of-day directly moves the debug light.
    const float daytime = wrap_daytime(dComIfGs_getTime());
    cXyz offset = sun_moon_offset(daytime);
    if (offset.y <= 0.0f) {
        offset = sun_moon_offset(daytime + 180.0f);
    }
    const float length = std::sqrt(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
    if (length < 1.0f) {
        return false;
    }
    outDirToLight[0] = offset.x / length;
    outDirToLight[1] = offset.y / length;
    outDirToLight[2] = offset.z / length;
    // Fade shadows out as the light approaches the horizon (elevation below ~11 degrees).
    outFade = std::clamp((outDirToLight[1] - 0.05f) / 0.15f, 0.0f, 1.0f);
    return outFade > 0.0f;
}

bool build_light_camera(const Mtx cameraView, uint32_t mapSize, float radius, LightCamera& out) {
    Mtx cameraInvView;
    cMtx_inverse(cameraView, cameraInvView);
    if (!matrix_ready(cameraInvView)) {
        return false;
    }
    if (!compute_light(out.dirToLight, out.fade)) {
        return false;
    }

    // Fit a fixed-radius ortho box around the visible play space. The camera target alone can sit
    // behind the receiver field, while a far-horizon center drops foreground receivers.
    const cXyz eye{cameraInvView[0][3], cameraInvView[1][3], cameraInvView[2][3]};
    cXyz forward{-cameraInvView[0][2], -cameraInvView[1][2], -cameraInvView[2][2]};
    const float forwardLength =
        std::sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    if (forwardLength > 0.001f) {
        forward = forward / forwardLength;
    } else {
        forward = cXyz{0.0f, 0.0f, -1.0f};
    }
    const float lookahead = std::min(radius * 0.75f, kMaxLightLookahead);
    const cXyz center = eye + forward * lookahead;
    const cXyz lightEye{center.x + out.dirToLight[0] * kLightDistance,
        center.y + out.dirToLight[1] * kLightDistance,
        center.z + out.dirToLight[2] * kLightDistance};
    const bool nearlyVertical = std::fabs(out.dirToLight[1]) > 0.99f;
    cXyz up = nearlyVertical ? cXyz{0.0f, 0.0f, 1.0f} : cXyz{0.0f, 1.0f, 0.0f};

    cMtx_lookAt(out.view, &lightEye, &center, &up, 0);
    const float unitsPerTexel = (2.0f * radius) / static_cast<float>(mapSize);
    out.view[0][3] = std::round(out.view[0][3] / unitsPerTexel) * unitsPerTexel;
    out.view[1][3] = std::round(out.view[1][3] / unitsPerTexel) * unitsPerTexel;

    C_MTXOrtho(out.ortho, radius, -radius, -radius, radius, kLightNear, kLightFar);
    cMtx_concatProjView(out.ortho, out.view, out.vp);
    return true;
}

bool build_light_replay_projection(
    const LightCamera& lightCamera, const Mtx cameraView, Mtx44 out) {
    Mtx cameraInvView;
    cMtx_inverse(cameraView, cameraInvView);
    if (!matrix_ready(cameraInvView)) {
        return false;
    }

    Mtx lightFromCamera;
    cMtx_concat(lightCamera.view, cameraInvView, lightFromCamera);
    cMtx_concatProjView(lightCamera.ortho, lightFromCamera, out);
    return true;
}

// True when the dynamic shadow pass will run this frame: enabled, a camera exists, and the
// sun or moon is above the horizon. Also gates the game-shadow skip hooks, which fire earlier
// in the painter than our SCENE_AFTER_TERRAIN hook.
bool dynamic_shadows_wanted() {
    if (!get_bool_option(g_cvarEnabled, true)) {
        return false;
    }
    if (!g_sceneCamera.raw_valid) {
        return false;
    }
    float dirToLight[3];
    float fade = 0.0f;
    return compute_light(dirToLight, fade);
}

HookAction on_game_shadow_pre(ModContext*, void*, void*, void*) {
    if (!dynamic_shadows_wanted()) {
        return HOOK_CONTINUE;
    }
    return HOOK_SKIP_ORIGINAL;
}

HookAction on_frustum_clip_pre(ModContext*, void*, void* retval, void*) {
    if (!get_bool_option(g_cvarNoFrustumClipping, false) || !dynamic_shadows_wanted()) {
        return HOOK_CONTINUE;
    }
    if (retval != nullptr) {
        *static_cast<int*>(retval) = 0;
    }
    return HOOK_SKIP_ORIGINAL;
}

HookAction on_copy_tex_pre(ModContext*, void*, void*, void*) {
    return g_replayingSceneLists ? HOOK_SKIP_ORIGINAL : HOOK_CONTINUE;
}

void draw_opaque_scene_lists() {
    dComIfGd_drawOpaListBG();
    dComIfGd_drawOpaListDarkBG();
    dComIfGd_drawOpaListMiddle();
    dComIfGd_drawOpaList();
    dComIfGd_drawOpaListDark();
    dComIfGd_drawOpaListPacket();
}

bool draw_lists_ready() {
    return dComIfGd_getOpaListBG() != nullptr && dComIfGd_getOpaList() != nullptr &&
           dComIfGd_getOpaListDark() != nullptr && dComIfGd_getXluListBG() != nullptr &&
           dComIfGd_getListPacket() != nullptr;
}

void render_shadow_map(
    const Mtx replayView, const Mtx44 replayProjectionMtx, const f32 replayProjection[7]);

void restore_actual_light_debug() {
    if (!g_actualLightDebug.active) {
        return;
    }

    j3dSys.setViewMtx(g_actualLightDebug.savedView);
    GXSetProjectionv(g_actualLightDebug.savedProjection);
    GXSetViewport(g_actualLightDebug.savedViewport[0], g_actualLightDebug.savedViewport[1],
        g_actualLightDebug.savedViewport[2], g_actualLightDebug.savedViewport[3],
        g_actualLightDebug.savedViewport[4], g_actualLightDebug.savedViewport[5]);
    GXSetScissor(g_actualLightDebug.savedScissor[0], g_actualLightDebug.savedScissor[1],
        g_actualLightDebug.savedScissor[2], g_actualLightDebug.savedScissor[3]);
    dKy_setLight();
    J3DShape::resetVcdVatCache();

    g_actualLightDebug.active = false;
}

void on_scene_begin(ModContext*, const GfxStageContext* stageCtx, void*) {
    restore_actual_light_debug();
    capture_scene_camera(stageCtx);
    if (!get_bool_option(g_cvarEnabled, true) || get_debug_mode() != 9) {
        return;
    }

    Mtx cameraView;
    if (!g_sceneCamera.raw_valid || !matrix_ready(g_sceneCamera.raw_view)) {
        return;
    }
    cMtx_copy(g_sceneCamera.raw_view, cameraView);

    const uint32_t mapSize = 1024u << std::clamp<int64_t>(get_int_option(g_cvarMapSize, 1), 0, 2);
    const float radius =
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarBoxRadius, 6000), 1000, 20000));
    LightCamera lightCamera{};
    if (!build_light_camera(cameraView, mapSize, radius, lightCamera)) {
        return;
    }
    Mtx44 lightProjection;
    if (!build_light_replay_projection(lightCamera, cameraView, lightProjection)) {
        return;
    }

    cMtx_copy(cameraView, g_actualLightDebug.savedView);
    GXGetProjectionv(g_actualLightDebug.savedProjection);
    GXGetViewportv(g_actualLightDebug.savedViewport);
    GXGetScissor(&g_actualLightDebug.savedScissor[0], &g_actualLightDebug.savedScissor[1],
        &g_actualLightDebug.savedScissor[2], &g_actualLightDebug.savedScissor[3]);
    g_actualLightDebug.active = true;

    j3dSys.setViewMtx(g_actualLightDebug.savedView);
    GXSetProjectionFull(lightProjection);
    dKy_setLight();
    J3DShape::resetVcdVatCache();
}

void on_scene_after_terrain(ModContext*, const GfxStageContext* stageCtx, void*) {
    if (g_mapPass.ready) {
        return;
    }

    const view_class* gameView = stage_game_view(stageCtx);
    Mtx replayView;
    Mtx44 replayProjectionMtx;
    f32 replayProjection[7];
    if (!capture_raw_camera(gameView, replayView, replayProjectionMtx, replayProjection)) {
        return;
    }
    render_shadow_map(replayView, replayProjectionMtx, replayProjection);
}

// Game thread, after the draw handlers have populated next frame's scene lists: replay opaque scene
// geometry from the light's point of view.
void render_shadow_map(
    const Mtx replayView, const Mtx44 replayProjectionMtx, const f32 replayProjection[7]) {
    if (g_mapPass.ready || !get_bool_option(g_cvarEnabled, true)) {
        return;
    }
    const int64_t debugMode = get_debug_mode();
    if (debugMode == 9) {
        return;
    }
    if (!matrix_ready(replayView)) {
        return;
    }
    Mtx replayViewMtx;
    cMtx_copy(replayView, replayViewMtx);

    const uint32_t mapSize = 1024u << std::clamp<int64_t>(get_int_option(g_cvarMapSize, 1), 0, 2);
    const bool cameraReplayDebug = debugMode == 10;
    const float radius =
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarBoxRadius, 6000), 1000, 20000));
    LightCamera lightCamera{};
    if (!build_light_camera(replayViewMtx, mapSize, radius, lightCamera)) {
        return;
    }
    Mtx44 lightReplayProjection;
    if (!build_light_replay_projection(lightCamera, replayViewMtx, lightReplayProjection)) {
        return;
    }
    f32 savedProjection[7];
    GXGetProjectionv(savedProjection);
    f32 savedViewport[6];
    GXGetViewportv(savedViewport);
    u32 savedScissor[4];
    GXGetScissor(&savedScissor[0], &savedScissor[1], &savedScissor[2], &savedScissor[3]);
    Mtx savedView;
    cMtx_copy(j3dSys.getViewMtx(), savedView);

    auto restore_game_camera = [&]() {
        j3dSys.setViewMtx(savedView);
        GXSetProjectionv(savedProjection);
        GXSetViewport(savedViewport[0], savedViewport[1], savedViewport[2], savedViewport[3],
            savedViewport[4], savedViewport[5]);
        GXSetScissor(savedScissor[0], savedScissor[1], savedScissor[2], savedScissor[3]);
        dKy_setLight();
    };
    auto set_replay_camera = [&]() {
        j3dSys.setViewMtx(replayViewMtx);
        if (cameraReplayDebug) {
            GXSetProjectionv(replayProjection);
        } else {
            GXSetProjectionFull(lightReplayProjection);
        }
        dKy_setLight();
    };
    if (!draw_lists_ready()) {
        return;
    }
    if (svc_gfx->create_pass(mod_ctx, mapSize, mapSize) != MOD_OK) {
        return;
    }
    J3DShape::resetVcdVatCache();

    set_replay_camera();
    GXSetViewport(0.0f, 0.0f, static_cast<float>(mapSize), static_cast<float>(mapSize), 0.0f, 1.0f);
    GXSetViewportRender(
        0.0f, 0.0f, static_cast<float>(mapSize), static_cast<float>(mapSize), 0.0f, 1.0f);
    GXSetScissorRender(0, 0, mapSize, mapSize);
    dKy_setLight();
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    {
        replay_scope replay;
        draw_opaque_scene_lists();
    }
    j3dSys.reinitGX();
    J3DShape::resetVcdVatCache();
    restore_game_camera();

    GfxResolveDesc resolveDesc = GFX_RESOLVE_DESC_INIT;
    resolveDesc.color = true;
    resolveDesc.depth = true;
    GfxResolvedTargets resolved = GFX_RESOLVED_TARGETS_INIT;
    if (svc_gfx->resolve_pass(mod_ctx, &resolveDesc, &resolved) != MOD_OK ||
        resolved.color == nullptr || resolved.depth == nullptr)
    {
        return;
    }

    j3dSys.reinitGX();
    J3DShape::resetVcdVatCache();
    restore_game_camera();

    g_mapPass.lightColor = resolved.color;
    g_mapPass.shadowMap = resolved.depth;
    g_mapPass.mapSize = mapSize;
    copy_projection(lightCamera.vp, g_mapPass.lightVp);
    std::memcpy(
        g_mapPass.dirToLightWorld, lightCamera.dirToLight, sizeof(g_mapPass.dirToLightWorld));
    g_mapPass.fade = lightCamera.fade;
    g_mapPass.ready = true;
}

// Game thread, after the full 3D scene: deferred composite.
void on_frame_before_hud(ModContext*, const GfxStageContext*, void*) {
    const int64_t debugMode = get_debug_mode();
    restore_actual_light_debug();

    const MapPassOutput mapPass = std::exchange(g_mapPass, {});
    if (debugMode == 9) {
        return;
    }
    if (!mapPass.ready || mapPass.shadowMap == nullptr || mapPass.lightColor == nullptr) {
        return;
    }
    if (!g_sceneCamera.valid) {
        return;
    }
    const CameraInfo& camera = g_sceneCamera.info;

    GfxResolveDesc resolveDesc = GFX_RESOLVE_DESC_INIT;
    resolveDesc.color = false;
    resolveDesc.depth = true;
    GfxResolvedTargets resolved = GFX_RESOLVED_TARGETS_INIT;
    if (svc_gfx->resolve_pass(mod_ctx, &resolveDesc, &resolved) != MOD_OK ||
        resolved.depth == nullptr)
    {
        return;
    }

    ShadowUniforms uniforms{};
    std::memcpy(uniforms.world_from_proj, camera.world_from_proj, sizeof(uniforms.world_from_proj));
    std::memcpy(uniforms.view_from_proj, camera.view_from_proj, sizeof(uniforms.view_from_proj));
    std::memcpy(uniforms.proj_from_view, camera.proj_from_view, sizeof(uniforms.proj_from_view));
    store_column_major(mapPass.lightVp, uniforms.light_vp);
    // Rotate the world-space light direction into view space (w = 0).
    for (int r = 0; r < 3; ++r) {
        uniforms.light_dir_view[r] =
            camera.view_from_world[0 * 4 + r] * mapPass.dirToLightWorld[0] +
            camera.view_from_world[1 * 4 + r] * mapPass.dirToLightWorld[1] +
            camera.view_from_world[2 * 4 + r] * mapPass.dirToLightWorld[2];
    }
    // Bias is configured in world units along the light direction.
    uniforms.bias =
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarBias, 15), 0, 200)) /
        (kLightFar - kLightNear);
    uniforms.size[0] = static_cast<float>(mapPass.mapSize);
    uniforms.size[1] = static_cast<float>(mapPass.mapSize);
    uniforms.inv_size[0] = 1.0f / uniforms.size[0];
    uniforms.inv_size[1] = 1.0f / uniforms.size[1];
    uniforms.edge_fade_width =
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarEdgeFadeWidth, 32), 0, 128));
    uniforms.strength =
        mapPass.fade *
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarStrength, 45), 0, 100)) /
        100.0f;
    uniforms.pcf_taps = static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarPcf, 1), 0, 2));
    uniforms.contact_enabled = get_bool_option(g_cvarContactShadows, false) ? 1.0f : 0.0f;
    uniforms.contact_thickness = 25.0f;
    uniforms.contact_length = 60.0f;
    uniforms.debug_mode = static_cast<uint32_t>(debugMode);

    GfxRange uniformRange{0, 0};
    if (svc_gfx->push_uniform(mod_ctx, &uniforms, sizeof(uniforms), &uniformRange) != MOD_OK) {
        return;
    }
    const DrawPayload payload{resolved.depth, mapPass.shadowMap, mapPass.lightColor,
        uniformRange.offset, uniformRange.size, static_cast<uint32_t>(debugMode)};
    svc_gfx->push_draw(mod_ctx, g_drawType, &payload, sizeof(payload));
}

void add_control(UiElementHandle pane, const UiControlDesc& desc) {
    svc_ui->pane_add_control(mod_ctx, pane, &desc, nullptr);
}

void add_toggle(UiElementHandle pane, const char* label, ConfigVarHandle cvar, const char* help) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_TOGGLE;
    control.label = label;
    control.help_rml = help;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    add_control(pane, control);
}

void add_select(UiElementHandle pane, const char* label, ConfigVarHandle cvar, const char** options,
    uint32_t optionCount, const char* help) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_SELECT;
    control.label = label;
    control.help_rml = help;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.options = options;
    control.option_count = optionCount;
    add_control(pane, control);
}

void add_number(UiElementHandle pane, const char* label, ConfigVarHandle cvar, int64_t min,
    int64_t max, int64_t step, const char* suffix, const char* help) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_NUMBER;
    control.label = label;
    control.help_rml = help;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.min = min;
    control.max = max;
    control.step = step;
    control.suffix = suffix;
    add_control(pane, control);
}

ModResult build_controls_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    (void)right;

    svc_ui->pane_add_section(mod_ctx, left, "Shadow Map");
    add_toggle(left, "Enabled", g_cvarEnabled, "Enables dynamic shadows.");
    static const char* kMapSizes[] = {"1024", "2048", "4096"};
    add_select(left, "Map Size", g_cvarMapSize, kMapSizes, 3,
        "Shadow map resolution. Larger is sharper and slower.");
    add_toggle(left, "No Frustum Clipping", g_cvarNoFrustumClipping,
        "Keeps camera-frustum-culled objects in draw lists so off-screen objects can cast "
        "dynamic shadows. This can be expensive.");
    add_number(left, "Coverage", g_cvarBoxRadius, 1000, 20000, 500, nullptr,
        "Radius of the shadowed area around the camera, in world units. Smaller is sharper.");
    add_number(left, "Fade Out", g_cvarEdgeFadeWidth, 0, 128, 32, " texels",
        "Fade out shadows gradually near the edge of the coverage area.");

    svc_ui->pane_add_section(mod_ctx, left, "Appearance");
    add_number(left, "Strength", g_cvarStrength, 0, 100, 5, "%", "How dark shadowed areas become.");
    static const char* kPcfOptions[] = {"Off", "3x3", "5x5"};
    add_select(left, "Soft Shadows", g_cvarPcf, kPcfOptions, 3,
        "Percentage-closer filtering tap pattern; softens shadow edges.");
    add_number(left, "Bias", g_cvarBias, 0, 200, 5, nullptr,
        "Depth bias in world units. Raise to remove shadow acne; lower to reduce peter-panning.");
    add_toggle(left, "Contact Shadows", g_cvarContactShadows,
        "Adds a screen-space raymarch for small-scale contact darkening the map misses.");

    svc_ui->pane_add_section(mod_ctx, left, "Debug");
    static const char* kDebugOptions[] = {"Off", "Shadow Map", "Shadow Factor", "Occlusion",
        "Light UV", "Compare Sign", "Depth Values", "Receiver Range", "Bounds", "Light View",
        "Camera Replay"};
    add_select(left, "Debug View", g_cvarDebugView, kDebugOptions, 11,
        "Shadow Map: light-space depth buffer<br/>Shadow Factor: final "
        "darkening term<br/>Occlusion: map comparison result<br/>Light UV: receiver "
        "projection coverage<br/>Compare Sign: current comparison in red and opposite "
        "comparison in blue<br/>Depth Values: receiver depth in red and map depth in green<br/>"
        "Receiver Range: beyond-far in red, valid depth in green, and before-near in blue<br/>"
        "Bounds: valid X in red, valid Y in green, and valid depth in blue<br/>Light View: "
        "renders the game world directly from the light camera<br/>Camera Replay: "
        "captures the same draw-list replay from the gameplay camera");
    return MOD_OK;
}

void on_controls_window_closed(ModContext*, UiWindowHandle, void*) {
    g_controlsWindow = 0;
}

void on_open_controls(ModContext*, void*) {
    if (g_controlsWindow != 0) {
        return;
    }
    UiTabDesc tabs[1] = {UI_TAB_DESC_INIT};
    tabs[0].title = "Controls";
    tabs[0].build = build_controls_tab;
    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 1;
    desc.on_closed = on_controls_window_closed;
    if (svc_ui->window_push(mod_ctx, &desc, &g_controlsWindow) != MOD_OK) {
        svc_log->error(mod_ctx, "failed to open shadow controls window");
    }
}

ModResult build_panel(ModContext*, UiElementHandle panel, void*, ModError*) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_TOGGLE;
    control.label = "Enabled";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = g_cvarEnabled;
    add_control(panel, control);

    control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_BUTTON;
    control.label = "Open Controls";
    control.on_pressed = on_open_controls;
    add_control(panel, control);
    return MOD_OK;
}

ModResult register_bool_option(
    const char* name, bool defaultValue, ConfigVarHandle& outHandle, ModError* error) {
    ConfigVarDesc cvarDesc = CONFIG_VAR_DESC_INIT;
    cvarDesc.name = name;
    cvarDesc.type = CONFIG_VAR_BOOL;
    cvarDesc.default_bool = defaultValue;
    if (svc_config->register_var(mod_ctx, &cvarDesc, &outHandle) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register shadow option");
    }
    return MOD_OK;
}

ModResult register_int_option(
    const char* name, int64_t defaultValue, ConfigVarHandle& outHandle, ModError* error) {
    ConfigVarDesc cvarDesc = CONFIG_VAR_DESC_INIT;
    cvarDesc.name = name;
    cvarDesc.type = CONFIG_VAR_INT;
    cvarDesc.default_int = defaultValue;
    if (svc_config->register_var(mod_ctx, &cvarDesc, &outHandle) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register shadow option");
    }
    return MOD_OK;
}

}  // namespace

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result = svc_resource->load(mod_ctx, "shadow.wgsl", &g_shaderSource);
    if (result != MOD_OK || g_shaderSource.data == nullptr) {
        return dusk::mods::set_error(error, result, "failed to load shadow.wgsl");
    }

    result = register_bool_option("effectEnabled", false, g_cvarEnabled, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("mapSize", 2, g_cvarMapSize, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_bool_option("noFrustumClipping", true, g_cvarNoFrustumClipping, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("strength", 45, g_cvarStrength, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("pcf", 2, g_cvarPcf, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("bias", 55, g_cvarBias, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("boxRadius", 6000, g_cvarBoxRadius, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("edgeFadeWidth", 32, g_cvarEdgeFadeWidth, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_bool_option("contactShadows", true, g_cvarContactShadows, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("debugView", 0, g_cvarDebugView, error);
    if (result != MOD_OK) {
        return result;
    }

    if (svc_gfx->get_device_info(mod_ctx, &g_deviceInfo) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to query device info");
    }
    if (!build_composite_pipeline(true, g_compositePipeline, g_compositeLayout) ||
        !build_composite_pipeline(false, g_compositeDebugPipeline, g_compositeDebugLayout))
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to create composite pipeline");
    }

    GfxDrawTypeDesc drawDesc = GFX_DRAW_TYPE_DESC_INIT;
    drawDesc.label = "shadow composite";
    drawDesc.draw = on_draw;
    if (svc_gfx->register_draw_type(mod_ctx, &drawDesc, &g_drawType) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register draw type");
    }
    GfxStageHookDesc stageDesc = GFX_STAGE_HOOK_DESC_INIT;
    stageDesc.callback = on_scene_begin;
    if (svc_gfx->register_stage_hook(
            mod_ctx, GFX_STAGE_SCENE_BEGIN, &stageDesc, &g_sceneBeginHook) != MOD_OK)
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register stage hook");
    }
    stageDesc.callback = on_scene_after_terrain;
    if (svc_gfx->register_stage_hook(
            mod_ctx, GFX_STAGE_SCENE_AFTER_TERRAIN, &stageDesc, &g_sceneAfterTerrainHook) != MOD_OK)
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register stage hook");
    }
    stageDesc.callback = on_frame_before_hud;
    if (svc_gfx->register_stage_hook(
            mod_ctx, GFX_STAGE_FRAME_BEFORE_HUD, &stageDesc, &g_frameBeforeHudHook) != MOD_OK)
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register stage hook");
    }

    // Skip the game's own shadow rendering while the dynamic pass is active: the
    // shadowControl pair covers the actor real/blob shadows, drawCloudShadow the weather
    // cloud shadows.
    if (dusk::mods::hook_add_pre<&dDlst_shadowControl_c::imageDraw>(svc_hook, on_game_shadow_pre) !=
            MOD_OK ||
        dusk::mods::hook_add_pre<&dDlst_shadowControl_c::draw>(svc_hook, on_game_shadow_pre) !=
            MOD_OK ||
        dusk::mods::hook_add_pre<&drawCloudShadow>(svc_hook, on_game_shadow_pre) != MOD_OK)
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to hook game shadow rendering");
    }
    if (dusk::mods::hook_add_pre<kClipperSphereClip>(svc_hook, on_frustum_clip_pre) != MOD_OK ||
        dusk::mods::hook_add_pre<kClipperBoxClip>(svc_hook, on_frustum_clip_pre) != MOD_OK)
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to hook frustum clipping");
    }
    if (dusk::mods::hook_add_pre<GXCopyTex>(svc_hook, on_copy_tex_pre) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to hook GXCopyTex");
    }
    UiModsPanelDesc panelDesc = UI_MODS_PANEL_DESC_INIT;
    panelDesc.build = build_panel;
    svc_ui->register_mods_panel(mod_ctx, &panelDesc);

    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    restore_actual_light_debug();
    svc_resource->free(mod_ctx, &g_shaderSource);
    if (g_compositePipeline != nullptr) {
        wgpuRenderPipelineRelease(g_compositePipeline);
        g_compositePipeline = nullptr;
    }
    if (g_compositeDebugPipeline != nullptr) {
        wgpuRenderPipelineRelease(g_compositeDebugPipeline);
        g_compositeDebugPipeline = nullptr;
    }
    if (g_compositeLayout != nullptr) {
        wgpuBindGroupLayoutRelease(g_compositeLayout);
        g_compositeLayout = nullptr;
    }
    if (g_compositeDebugLayout != nullptr) {
        wgpuBindGroupLayoutRelease(g_compositeDebugLayout);
        g_compositeDebugLayout = nullptr;
    }
    g_cvarEnabled = g_cvarMapSize = g_cvarNoFrustumClipping = 0;
    g_cvarStrength = 0;
    g_cvarPcf = g_cvarBias = g_cvarBoxRadius = g_cvarEdgeFadeWidth = g_cvarContactShadows =
        g_cvarDebugView = 0;
    g_drawType = g_sceneBeginHook = g_sceneAfterTerrainHook = g_frameBeforeHudHook = 0;
    g_controlsWindow = 0;
    g_mapPass = {};
    g_sceneCamera.valid = false;
    g_sceneCamera.raw_valid = false;
    return MOD_OK;
}
}
