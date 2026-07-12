// Ambient occlusion (GTAO) example mod.
//
// Showcases the gfx service's compute tasks and the camera service: after opaque scene draws,
// before translucent/fog overlays, the scene depth is resolved and a three-dispatch compute
// chain (depth MIP prefilter, GTAO, spatial denoise) produces a visibility texture that a
// fullscreen draw multiplies over the world.
//
// The WGSL in res/ is ported from Bevy Engine's SSAO implementation (MIT OR Apache-2.0),
// itself based on Intel XeGTAO (MIT); see res/licenses/ and the `PORT:` notes in the shaders.

#include "mods/service.hpp"
#include "mods/svc/camera.h"
#include "mods/svc/config.h"
#include "mods/svc/gfx.h"
#include "mods/svc/log.h"
#include "mods/svc/resource.h"
#include "mods/svc/ui.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <vector>
#include <webgpu/webgpu.h>

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(ConfigService, svc_config);
IMPORT_SERVICE(ResourceService, svc_resource);
IMPORT_SERVICE(UiService, svc_ui);
IMPORT_SERVICE(GfxService, svc_gfx);
IMPORT_SERVICE(CameraService, svc_camera);

namespace {

ConfigVarHandle g_cvarEnabled = 0;
ConfigVarHandle g_cvarQuality = 0;
ConfigVarHandle g_cvarRadius = 0;
ConfigVarHandle g_cvarIntensity = 0;
ConfigVarHandle g_cvarHalfRes = 0;
ConfigVarHandle g_cvarDebugView = 0;

GfxComputeTypeHandle g_computeType = 0;
GfxDrawTypeHandle g_drawType = 0;
GfxStageHookHandle g_afterOpaqueHook = 0;
UiWindowHandle g_controlsWindow = 0;

ResourceBuffer g_preprocessSource = RESOURCE_BUFFER_INIT;
ResourceBuffer g_gtaoSource = RESOURCE_BUFFER_INIT;
ResourceBuffer g_denoiseSource = RESOURCE_BUFFER_INIT;
ResourceBuffer g_compositeSource = RESOURCE_BUFFER_INIT;

GfxDeviceInfo g_deviceInfo = GFX_DEVICE_INFO_INIT;
WGPUComputePipeline g_preprocessPipeline = nullptr;
WGPUComputePipeline g_mip4Pipeline = nullptr;
WGPUComputePipeline g_gtaoPipeline = nullptr;
WGPUComputePipeline g_denoisePipeline = nullptr;
WGPUBindGroupLayout g_preprocessLayout = nullptr;
WGPUBindGroupLayout g_mip4Layout = nullptr;
WGPUBindGroupLayout g_gtaoLayout = nullptr;
WGPUBindGroupLayout g_denoiseLayout = nullptr;
WGPURenderPipeline g_compositePipeline = nullptr;
WGPURenderPipeline g_compositeDebugPipeline = nullptr;
WGPUBindGroupLayout g_compositeLayout = nullptr;
WGPUBindGroupLayout g_compositeDebugLayout = nullptr;
WGPUTexture g_hilbertLut = nullptr;
WGPUTextureView g_hilbertLutView = nullptr;

// AO chain targets, recreated when the render size (or halfRes) changes. Old sets are retired
// for a few frames instead of released immediately: payloads embedding their views may still
// be in flight on the render worker.
struct AoTargets {
    uint32_t width = 0;
    uint32_t height = 0;
    WGPUTexture preprocessedDepth = nullptr;
    WGPUTextureView preprocessedDepthMips[5] = {};
    WGPUTextureView preprocessedDepthAll = nullptr;
    WGPUTexture aoNoisy = nullptr;
    WGPUTextureView aoNoisyView = nullptr;
    WGPUTexture depthDifferences = nullptr;
    WGPUTextureView depthDifferencesView = nullptr;
    WGPUTexture aoFinal = nullptr;
    WGPUTextureView aoFinalView = nullptr;
};
AoTargets g_targets;
struct RetiredTargets {
    AoTargets targets;
    int framesLeft = 0;
};
std::vector<RetiredTargets> g_retiredTargets;

bool g_warnedNoDepth = false;
bool g_loggedChain = false;
std::atomic g_chainExecuted{false};

// Mirror of the WGSL Uniforms struct (keep in sync with res/*.wgsl).
struct AoUniforms {
    float projection[16];
    float inverse_projection[16];
    float size[2];
    float inv_size[2];
    float depth_scale[2];
    float effect_radius;
    float intensity;
    float slice_count;
    float samples_per_slice_side;
    uint32_t debug_view;
    float _pad;
};
static_assert(sizeof(AoUniforms) % 16 == 0);

struct ComputePayload {
    WGPUTextureView depth;  // frame-pooled scene depth snapshot
    WGPUTextureView preprocessedDepthMips[5];
    WGPUTextureView preprocessedDepthAll;
    WGPUTextureView aoNoisy;
    WGPUTextureView depthDifferences;
    WGPUTextureView aoFinal;
    uint32_t uniform_offset;
    uint32_t uniform_size;
    uint32_t width;
    uint32_t height;
};
static_assert(sizeof(ComputePayload) <= GFX_INLINE_DRAW_PAYLOAD_SIZE);
static_assert(std::is_trivially_copyable_v<ComputePayload>);

struct CompositePayload {
    WGPUTextureView aoFinal;
    WGPUTextureView preprocessedDepth;  // debug views reconstruct normals/depth from it
    WGPUTextureView sceneDepth;         // raw snapshot, for the bypass debug views
    uint32_t uniform_offset;
    uint32_t uniform_size;
    uint32_t debug_view;
};
static_assert(sizeof(CompositePayload) <= GFX_INLINE_DRAW_PAYLOAD_SIZE);
static_assert(std::is_trivially_copyable_v<CompositePayload>);

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

// XeGTAO/Bevy quality presets: slices x (samples per slice side * 2).
void quality_counts(int64_t quality, float& sliceCount, float& samplesPerSliceSide) {
    switch (std::clamp<int64_t>(quality, 0, 3)) {
    case 0:
        sliceCount = 1.0f;
        samplesPerSliceSide = 2.0f;
        break;
    case 1:
        sliceCount = 2.0f;
        samplesPerSliceSide = 2.0f;
        break;
    default:
    case 2:
        sliceCount = 3.0f;
        samplesPerSliceSide = 3.0f;
        break;
    case 3:
        sliceCount = 9.0f;
        samplesPerSliceSide = 3.0f;
        break;
    }
}

WGPUShaderModule create_shader_module(const char* label, const ResourceBuffer& source) {
    WGPUShaderSourceWGSL wgsl = WGPU_SHADER_SOURCE_WGSL_INIT;
    wgsl.code = {static_cast<const char*>(source.data), source.size};
    WGPUShaderModuleDescriptor moduleDesc = WGPU_SHADER_MODULE_DESCRIPTOR_INIT;
    moduleDesc.nextInChain = &wgsl.chain;
    moduleDesc.label = {label, WGPU_STRLEN};
    return wgpuDeviceCreateShaderModule(g_deviceInfo.device, &moduleDesc);
}

bool build_compute_pipeline(const char* label, const ResourceBuffer& source, const char* entry,
    WGPUComputePipeline& outPipeline, WGPUBindGroupLayout& outLayout) {
    WGPUShaderModule module = create_shader_module(label, source);
    if (module == nullptr) {
        return false;
    }
    WGPUComputePipelineDescriptor pipelineDesc = WGPU_COMPUTE_PIPELINE_DESCRIPTOR_INIT;
    pipelineDesc.label = {label, WGPU_STRLEN};
    pipelineDesc.compute.module = module;
    pipelineDesc.compute.entryPoint = {entry, WGPU_STRLEN};
    outPipeline = wgpuDeviceCreateComputePipeline(g_deviceInfo.device, &pipelineDesc);
    wgpuShaderModuleRelease(module);
    if (outPipeline == nullptr) {
        return false;
    }
    outLayout = wgpuComputePipelineGetBindGroupLayout(outPipeline, 0);
    return outLayout != nullptr;
}

bool build_composite_pipeline(
    bool blend, WGPURenderPipeline& outPipeline, WGPUBindGroupLayout& outLayout) {
    WGPUShaderModule module = create_shader_module("AO composite", g_compositeSource);
    if (module == nullptr) {
        return false;
    }

    // Multiply blend
    WGPUBlendState blendState{
        .color =
            {
                .operation = WGPUBlendOperation_Add,
                .srcFactor = WGPUBlendFactor_Dst,
                .dstFactor = WGPUBlendFactor_Zero,
            },
        .alpha =
            {
                .operation = WGPUBlendOperation_Add,
                .srcFactor = WGPUBlendFactor_Zero,
                .dstFactor = WGPUBlendFactor_One,
            },
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
    // Depth state must match the EFB pass despite never touching depth.
    WGPUDepthStencilState depthStencil = WGPU_DEPTH_STENCIL_STATE_INIT;
    depthStencil.format = g_deviceInfo.depth_format;
    depthStencil.depthWriteEnabled = WGPUOptionalBool_False;
    depthStencil.depthCompare = WGPUCompareFunction_Always;

    WGPURenderPipelineDescriptor pipelineDesc = WGPU_RENDER_PIPELINE_DESCRIPTOR_INIT;
    pipelineDesc.label = {blend ? "AO composite" : "AO composite (debug)", WGPU_STRLEN};
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

// Hilbert curve index LUT for the R2 noise sequence, generated once at init.
// Ported from Bevy's generate_hilbert_index_lut (https://www.shadertoy.com/view/3tB3z3).
uint16_t hilbert_index(uint16_t x, uint16_t y) {
    uint16_t index = 0;
    for (uint16_t level = 32; level > 0; level /= 2) {
        const uint16_t regionX = (x & level) > 0 ? 1 : 0;
        const uint16_t regionY = (y & level) > 0 ? 1 : 0;
        index += level * level * ((3 * regionX) ^ regionY);
        if (regionY == 0) {
            if (regionX == 1) {
                x = 63 - x;
                y = 63 - y;
            }
            std::swap(x, y);
        }
    }
    return index;
}

bool build_hilbert_lut() {
    WGPUTextureDescriptor texDesc = WGPU_TEXTURE_DESCRIPTOR_INIT;
    texDesc.label = {"AO hilbert LUT", WGPU_STRLEN};
    texDesc.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
    texDesc.size = {64, 64, 1};
    texDesc.format = WGPUTextureFormat_R16Uint;
    g_hilbertLut = wgpuDeviceCreateTexture(g_deviceInfo.device, &texDesc);
    if (g_hilbertLut == nullptr) {
        return false;
    }
    g_hilbertLutView = wgpuTextureCreateView(g_hilbertLut, nullptr);
    if (g_hilbertLutView == nullptr) {
        return false;
    }

    uint16_t lut[64 * 64];
    for (uint16_t y = 0; y < 64; ++y) {
        for (uint16_t x = 0; x < 64; ++x) {
            lut[y * 64 + x] = hilbert_index(x, y);
        }
    }
    WGPUTexelCopyTextureInfo dst = WGPU_TEXEL_COPY_TEXTURE_INFO_INIT;
    dst.texture = g_hilbertLut;
    WGPUTexelCopyBufferLayout layout{.offset = 0, .bytesPerRow = 64 * 2, .rowsPerImage = 64};
    WGPUExtent3D extent{64, 64, 1};
    wgpuQueueWriteTexture(g_deviceInfo.queue, &dst, lut, sizeof(lut), &layout, &extent);
    return true;
}

void release_targets(AoTargets& targets) {
    for (auto*& view : targets.preprocessedDepthMips) {
        if (view != nullptr) {
            wgpuTextureViewRelease(view);
            view = nullptr;
        }
    }
    const auto releaseView = [](WGPUTextureView& view) {
        if (view != nullptr) {
            wgpuTextureViewRelease(view);
            view = nullptr;
        }
    };
    const auto releaseTexture = [](WGPUTexture& texture) {
        if (texture != nullptr) {
            wgpuTextureRelease(texture);
            texture = nullptr;
        }
    };
    releaseView(targets.preprocessedDepthAll);
    releaseView(targets.aoNoisyView);
    releaseView(targets.depthDifferencesView);
    releaseView(targets.aoFinalView);
    releaseTexture(targets.preprocessedDepth);
    releaseTexture(targets.aoNoisy);
    releaseTexture(targets.depthDifferences);
    releaseTexture(targets.aoFinal);
    targets.width = targets.height = 0;
}

void tick_retired_targets() {
    for (auto it = g_retiredTargets.begin(); it != g_retiredTargets.end();) {
        if (--it->framesLeft <= 0) {
            release_targets(it->targets);
            it = g_retiredTargets.erase(it);
        } else {
            ++it;
        }
    }
}

bool ensure_targets(uint32_t width, uint32_t height) {
    if (g_targets.width == width && g_targets.height == height) {
        return true;
    }
    if (g_targets.width != 0) {
        g_retiredTargets.push_back(RetiredTargets{std::exchange(g_targets, AoTargets{}), 4});
    }

    const auto createStorageTexture = [&](const char* label, WGPUTextureFormat format,
                                          uint32_t mipCount, WGPUTexture& outTexture) {
        WGPUTextureDescriptor texDesc = WGPU_TEXTURE_DESCRIPTOR_INIT;
        texDesc.label = {label, WGPU_STRLEN};
        texDesc.usage = WGPUTextureUsage_StorageBinding | WGPUTextureUsage_TextureBinding;
        texDesc.size = {width, height, 1};
        texDesc.format = format;
        texDesc.mipLevelCount = mipCount;
        outTexture = wgpuDeviceCreateTexture(g_deviceInfo.device, &texDesc);
        return outTexture != nullptr;
    };

    bool ok = createStorageTexture("AO preprocessed depth", WGPUTextureFormat_R32Float, 5,
                  g_targets.preprocessedDepth) &&
              createStorageTexture("AO noisy", WGPUTextureFormat_R32Float, 1, g_targets.aoNoisy) &&
              createStorageTexture("AO depth differences", WGPUTextureFormat_R32Uint, 1,
                  g_targets.depthDifferences) &&
              createStorageTexture("AO final", WGPUTextureFormat_R32Float, 1, g_targets.aoFinal);
    if (ok) {
        for (uint32_t mip = 0; mip < 5 && ok; ++mip) {
            WGPUTextureViewDescriptor viewDesc = WGPU_TEXTURE_VIEW_DESCRIPTOR_INIT;
            viewDesc.baseMipLevel = mip;
            viewDesc.mipLevelCount = 1;
            g_targets.preprocessedDepthMips[mip] =
                wgpuTextureCreateView(g_targets.preprocessedDepth, &viewDesc);
            ok = g_targets.preprocessedDepthMips[mip] != nullptr;
        }
    }
    if (ok) {
        g_targets.preprocessedDepthAll =
            wgpuTextureCreateView(g_targets.preprocessedDepth, nullptr);
        g_targets.aoNoisyView = wgpuTextureCreateView(g_targets.aoNoisy, nullptr);
        g_targets.depthDifferencesView = wgpuTextureCreateView(g_targets.depthDifferences, nullptr);
        g_targets.aoFinalView = wgpuTextureCreateView(g_targets.aoFinal, nullptr);
        ok = g_targets.preprocessedDepthAll != nullptr && g_targets.aoNoisyView != nullptr &&
             g_targets.depthDifferencesView != nullptr && g_targets.aoFinalView != nullptr;
    }
    if (!ok) {
        release_targets(g_targets);
        return false;
    }
    g_targets.width = width;
    g_targets.height = height;
    return true;
}

constexpr uint32_t div_ceil(uint32_t numerator, uint32_t denominator) {
    return (numerator + denominator - 1) / denominator;
}

// Render worker thread: the AO chain as one compute pass with three dispatches.
void on_compute(
    ModContext*, const GfxComputeContext* ctx, const void* payload, size_t payloadSize, void*) {
    if (payloadSize != sizeof(ComputePayload)) {
        return;
    }
    ComputePayload data;
    std::memcpy(&data, payload, sizeof(data));
    if (data.depth == nullptr || g_preprocessPipeline == nullptr) {
        return;
    }

    const auto makeBindGroup = [&](WGPUBindGroupLayout layout,
                                   std::initializer_list<WGPUBindGroupEntry> entries) {
        WGPUBindGroupDescriptor bindGroupDesc = WGPU_BIND_GROUP_DESCRIPTOR_INIT;
        bindGroupDesc.layout = layout;
        bindGroupDesc.entryCount = entries.size();
        bindGroupDesc.entries = entries.begin();
        return wgpuDeviceCreateBindGroup(ctx->device, &bindGroupDesc);
    };
    const auto textureEntry = [](uint32_t binding, WGPUTextureView view) {
        WGPUBindGroupEntry entry = WGPU_BIND_GROUP_ENTRY_INIT;
        entry.binding = binding;
        entry.textureView = view;
        return entry;
    };
    const auto uniformEntry = [&](uint32_t binding) {
        WGPUBindGroupEntry entry = WGPU_BIND_GROUP_ENTRY_INIT;
        entry.binding = binding;
        entry.buffer = ctx->uniform_buffer;
        entry.offset = data.uniform_offset;
        entry.size = data.uniform_size;
        return entry;
    };

    WGPUBindGroup preprocessGroup = makeBindGroup(g_preprocessLayout,
        {textureEntry(0, data.depth), textureEntry(1, data.preprocessedDepthMips[0]),
            textureEntry(2, data.preprocessedDepthMips[1]),
            textureEntry(3, data.preprocessedDepthMips[2]),
            textureEntry(4, data.preprocessedDepthMips[3]), uniformEntry(5)});
    WGPUBindGroup mip4Group =
        makeBindGroup(g_mip4Layout, {textureEntry(6, data.preprocessedDepthMips[3]),
                                        textureEntry(7, data.preprocessedDepthMips[4])});
    WGPUBindGroup gtaoGroup = makeBindGroup(
        g_gtaoLayout, {textureEntry(0, data.preprocessedDepthAll),
                          textureEntry(1, g_hilbertLutView), textureEntry(2, data.aoNoisy),
                          textureEntry(3, data.depthDifferences), uniformEntry(4)});
    WGPUBindGroup denoiseGroup = makeBindGroup(
        g_denoiseLayout, {textureEntry(0, data.aoNoisy), textureEntry(1, data.depthDifferences),
                             textureEntry(2, data.aoFinal), uniformEntry(3)});
    if (preprocessGroup == nullptr || mip4Group == nullptr || gtaoGroup == nullptr ||
        denoiseGroup == nullptr)
    {
        const auto release = [](WGPUBindGroup group) {
            if (group != nullptr) {
                wgpuBindGroupRelease(group);
            }
        };
        release(preprocessGroup);
        release(mip4Group);
        release(gtaoGroup);
        release(denoiseGroup);
        return;
    }

    WGPUComputePassDescriptor passDesc = WGPU_COMPUTE_PASS_DESCRIPTOR_INIT;
    passDesc.label = {"AO chain", WGPU_STRLEN};
    WGPUComputePassEncoder pass = wgpuCommandEncoderBeginComputePass(ctx->encoder, &passDesc);
    // Each preprocess workgroup covers 16x16 MIP-0 texels (8x8 invocations, 2x2 texels each).
    wgpuComputePassEncoderSetPipeline(pass, g_preprocessPipeline);
    wgpuComputePassEncoderSetBindGroup(pass, 0, preprocessGroup, 0, nullptr);
    wgpuComputePassEncoderDispatchWorkgroups(
        pass, div_ceil(data.width, 16), div_ceil(data.height, 16), 1);
    wgpuComputePassEncoderSetPipeline(pass, g_mip4Pipeline);
    wgpuComputePassEncoderSetBindGroup(pass, 0, mip4Group, 0, nullptr);
    wgpuComputePassEncoderDispatchWorkgroups(pass, div_ceil(std::max(data.width >> 4, 1u), 8),
        div_ceil(std::max(data.height >> 4, 1u), 8), 1);
    wgpuComputePassEncoderSetPipeline(pass, g_gtaoPipeline);
    wgpuComputePassEncoderSetBindGroup(pass, 0, gtaoGroup, 0, nullptr);
    wgpuComputePassEncoderDispatchWorkgroups(
        pass, div_ceil(data.width, 8), div_ceil(data.height, 8), 1);
    wgpuComputePassEncoderSetPipeline(pass, g_denoisePipeline);
    wgpuComputePassEncoderSetBindGroup(pass, 0, denoiseGroup, 0, nullptr);
    wgpuComputePassEncoderDispatchWorkgroups(
        pass, div_ceil(data.width, 8), div_ceil(data.height, 8), 1);
    wgpuComputePassEncoderEnd(pass);
    wgpuComputePassEncoderRelease(pass);

    wgpuBindGroupRelease(preprocessGroup);
    wgpuBindGroupRelease(mip4Group);
    wgpuBindGroupRelease(gtaoGroup);
    wgpuBindGroupRelease(denoiseGroup);
    g_chainExecuted.store(true, std::memory_order_release);
}

// Render worker thread: composite the AO over the scene (or show it, in debug view).
void on_draw(
    ModContext*, const GfxDrawContext* ctx, const void* payload, size_t payloadSize, void*) {
    if (payloadSize != sizeof(CompositePayload)) {
        return;
    }
    CompositePayload data;
    std::memcpy(&data, payload, sizeof(data));
    WGPURenderPipeline pipeline =
        data.debug_view != 0 ? g_compositeDebugPipeline : g_compositePipeline;
    WGPUBindGroupLayout layout = data.debug_view != 0 ? g_compositeDebugLayout : g_compositeLayout;
    if (data.aoFinal == nullptr || data.preprocessedDepth == nullptr ||
        data.sceneDepth == nullptr || pipeline == nullptr)
    {
        return;
    }

    WGPUBindGroupEntry entries[4] = {WGPU_BIND_GROUP_ENTRY_INIT, WGPU_BIND_GROUP_ENTRY_INIT,
        WGPU_BIND_GROUP_ENTRY_INIT, WGPU_BIND_GROUP_ENTRY_INIT};
    entries[0].binding = 0;
    entries[0].textureView = data.aoFinal;
    entries[1].binding = 1;
    entries[1].textureView = data.preprocessedDepth;
    entries[2].binding = 2;
    entries[2].textureView = data.sceneDepth;
    entries[3].binding = 3;
    entries[3].buffer = ctx->uniform_buffer;
    entries[3].offset = data.uniform_offset;
    entries[3].size = data.uniform_size;
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

// Game thread, after opaque scene draws and before translucent/fog overlay lists.
void on_scene_after_opaque(ModContext*, const GfxStageContext* stageCtx, void*) {
    tick_retired_targets();
    if (!get_bool_option(g_cvarEnabled, true)) {
        return;
    }
    if (stageCtx == nullptr || stageCtx->struct_size < sizeof(GfxStageContext) ||
        stageCtx->game_view == nullptr)
    {
        return;
    }

    CameraInfo camera = CAMERA_INFO_INIT;
    if (svc_camera->get_camera(mod_ctx, stageCtx->game_view, &camera) != MOD_OK) {
        return;
    }

    GfxResolveDesc resolveDesc = GFX_RESOLVE_DESC_INIT;
    resolveDesc.color = false;
    resolveDesc.depth = true;
    GfxResolvedTargets resolved = GFX_RESOLVED_TARGETS_INIT;
    if (svc_gfx->resolve_pass(mod_ctx, &resolveDesc, &resolved) != MOD_OK ||
        resolved.depth == nullptr)
    {
        if (!g_warnedNoDepth) {
            g_warnedNoDepth = true;
            svc_log->warn(mod_ctx, "depth snapshots unavailable; AO disabled");
        }
        return;
    }

    const bool halfRes = get_bool_option(g_cvarHalfRes, false);
    const uint32_t divisor = halfRes ? 2 : 1;
    const uint32_t width = resolved.width / divisor;
    const uint32_t height = resolved.height / divisor;
    if (width < 32 || height < 32 || !ensure_targets(width, height)) {
        return;
    }

    AoUniforms uniforms{};
    std::memcpy(uniforms.projection, camera.proj_from_view, sizeof(uniforms.projection));
    std::memcpy(
        uniforms.inverse_projection, camera.view_from_proj, sizeof(uniforms.inverse_projection));
    uniforms.size[0] = static_cast<float>(width);
    uniforms.size[1] = static_cast<float>(height);
    uniforms.inv_size[0] = 1.0f / uniforms.size[0];
    uniforms.inv_size[1] = 1.0f / uniforms.size[1];
    uniforms.depth_scale[0] = static_cast<float>(resolved.width) / uniforms.size[0];
    uniforms.depth_scale[1] = static_cast<float>(resolved.height) / uniforms.size[1];
    uniforms.effect_radius =
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarRadius, 70), 10, 500));
    uniforms.intensity =
        static_cast<float>(std::clamp<int64_t>(get_int_option(g_cvarIntensity, 100), 0, 100)) /
        100.0f;
    quality_counts(
        get_int_option(g_cvarQuality, 2), uniforms.slice_count, uniforms.samples_per_slice_side);
    const uint32_t debugMode =
        static_cast<uint32_t>(std::clamp<int64_t>(get_int_option(g_cvarDebugView, 0), 0, 4));
    uniforms.debug_view = debugMode;

    GfxRange uniformRange{0, 0};
    if (svc_gfx->push_uniform(mod_ctx, &uniforms, sizeof(uniforms), &uniformRange) != MOD_OK) {
        return;
    }

    ComputePayload computePayload{};
    computePayload.depth = resolved.depth;
    for (int mip = 0; mip < 5; ++mip) {
        computePayload.preprocessedDepthMips[mip] = g_targets.preprocessedDepthMips[mip];
    }
    computePayload.preprocessedDepthAll = g_targets.preprocessedDepthAll;
    computePayload.aoNoisy = g_targets.aoNoisyView;
    computePayload.depthDifferences = g_targets.depthDifferencesView;
    computePayload.aoFinal = g_targets.aoFinalView;
    computePayload.uniform_offset = uniformRange.offset;
    computePayload.uniform_size = uniformRange.size;
    computePayload.width = width;
    computePayload.height = height;
    if (svc_gfx->push_compute(mod_ctx, g_computeType, &computePayload, sizeof(computePayload)) !=
        MOD_OK)
    {
        return;
    }

    const CompositePayload drawPayload{g_targets.aoFinalView, g_targets.preprocessedDepthAll,
        resolved.depth, uniformRange.offset, uniformRange.size, debugMode};
    svc_gfx->push_draw(mod_ctx, g_drawType, &drawPayload, sizeof(drawPayload));
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

ModResult build_controls_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    (void)right;

    svc_ui->pane_add_section(mod_ctx, left, "Ambient Occlusion");
    add_toggle(left, "Enabled", g_cvarEnabled, "Enables the GTAO pass.");

    static const char* kQualityOptions[] = {"Low", "Medium", "High", "Ultra"};
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_SELECT;
    control.label = "Quality";
    control.help_rml = "Horizon slices and samples per pixel (XeGTAO presets: 4/8/18/54 spp).";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = g_cvarQuality;
    control.options = kQualityOptions;
    control.option_count = 4;
    add_control(left, control);

    control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_NUMBER;
    control.label = "Radius";
    control.help_rml = "Occlusion sampling radius in world units.";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = g_cvarRadius;
    control.min = 10;
    control.max = 500;
    control.step = 10;
    add_control(left, control);

    control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_NUMBER;
    control.label = "Intensity";
    control.help_rml = "How strongly occlusion darkens the scene.";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = g_cvarIntensity;
    control.min = 0;
    control.max = 100;
    control.step = 5;
    control.suffix = "%";
    add_control(left, control);

    add_toggle(left, "Half Resolution", g_cvarHalfRes,
        "Computes AO at half resolution and upscales; faster, slightly softer.");

    static const char* kDebugOptions[] = {"Off", "AO", "Normals", "Depth", "Staircase"};
    control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_SELECT;
    control.label = "Debug View";
    control.help_rml = "AO: raw visibility as grayscale.<br/>Normals: the view-space "
                       "normals the GTAO pass consumes.<br/>Depth: the preprocessed depth "
                       "as a distance gradient.<br/>Staircase: detects quantized depth - smooth "
                       "depth is near-black with thin triangle edges, quantized depth lights "
                       "up across surfaces.";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = g_cvarDebugView;
    control.options = kDebugOptions;
    control.option_count = 5;
    add_control(left, control);
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
        svc_log->error(mod_ctx, "failed to open AO controls window");
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
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register AO option");
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
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register AO option");
    }
    return MOD_OK;
}

}  // namespace

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    ModResult result = svc_resource->load(mod_ctx, "preprocess_depth.wgsl", &g_preprocessSource);
    if (result == MOD_OK) {
        result = svc_resource->load(mod_ctx, "gtao.wgsl", &g_gtaoSource);
    }
    if (result == MOD_OK) {
        result = svc_resource->load(mod_ctx, "denoise.wgsl", &g_denoiseSource);
    }
    if (result == MOD_OK) {
        result = svc_resource->load(mod_ctx, "composite.wgsl", &g_compositeSource);
    }
    if (result != MOD_OK) {
        return dusk::mods::set_error(error, result, "failed to load AO shaders");
    }

    result = register_bool_option("effectEnabled", false, g_cvarEnabled, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("quality", 2, g_cvarQuality, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("radius", 70, g_cvarRadius, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("intensity", 100, g_cvarIntensity, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_bool_option("halfRes", false, g_cvarHalfRes, error);
    if (result != MOD_OK) {
        return result;
    }
    result = register_int_option("debugMode", 0, g_cvarDebugView, error);
    if (result != MOD_OK) {
        return result;
    }

    if (svc_gfx->get_device_info(mod_ctx, &g_deviceInfo) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to query device info");
    }
    if (!build_compute_pipeline("AO preprocess depth", g_preprocessSource, "preprocess_depth",
            g_preprocessPipeline, g_preprocessLayout) ||
        !build_compute_pipeline("AO downsample mip4", g_preprocessSource, "downsample_mip4",
            g_mip4Pipeline, g_mip4Layout) ||
        !build_compute_pipeline("AO gtao", g_gtaoSource, "gtao", g_gtaoPipeline, g_gtaoLayout) ||
        !build_compute_pipeline(
            "AO denoise", g_denoiseSource, "spatial_denoise", g_denoisePipeline, g_denoiseLayout))
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to create AO compute pipelines");
    }
    if (!build_composite_pipeline(true, g_compositePipeline, g_compositeLayout) ||
        !build_composite_pipeline(false, g_compositeDebugPipeline, g_compositeDebugLayout))
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to create AO composite pipeline");
    }
    if (!build_hilbert_lut()) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to create AO noise LUT");
    }

    GfxComputeTypeDesc computeDesc = GFX_COMPUTE_TYPE_DESC_INIT;
    computeDesc.label = "AO chain";
    computeDesc.callback = on_compute;
    if (svc_gfx->register_compute_type(mod_ctx, &computeDesc, &g_computeType) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register compute type");
    }
    GfxDrawTypeDesc drawDesc = GFX_DRAW_TYPE_DESC_INIT;
    drawDesc.label = "AO composite";
    drawDesc.draw = on_draw;
    if (svc_gfx->register_draw_type(mod_ctx, &drawDesc, &g_drawType) != MOD_OK) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register draw type");
    }
    GfxStageHookDesc stageDesc = GFX_STAGE_HOOK_DESC_INIT;
    stageDesc.callback = on_scene_after_opaque;
    if (svc_gfx->register_stage_hook(
            mod_ctx, GFX_STAGE_SCENE_AFTER_OPAQUE, &stageDesc, &g_afterOpaqueHook) != MOD_OK)
    {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to register stage hook");
    }

    UiModsPanelDesc panelDesc = UI_MODS_PANEL_DESC_INIT;
    panelDesc.build = build_panel;
    svc_ui->register_mods_panel(mod_ctx, &panelDesc);

    svc_log->info(mod_ctx, "ao_mod ready");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    if (!g_loggedChain && g_chainExecuted.load(std::memory_order_acquire)) {
        g_loggedChain = true;
        svc_log->info(mod_ctx, "AO chain executed OK");
    }
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    svc_resource->free(mod_ctx, &g_preprocessSource);
    svc_resource->free(mod_ctx, &g_gtaoSource);
    svc_resource->free(mod_ctx, &g_denoiseSource);
    svc_resource->free(mod_ctx, &g_compositeSource);

    release_targets(g_targets);
    for (auto& retired : g_retiredTargets) {
        release_targets(retired.targets);
    }
    g_retiredTargets.clear();

    const auto releasePipeline = [](WGPUComputePipeline& pipeline) {
        if (pipeline != nullptr) {
            wgpuComputePipelineRelease(pipeline);
            pipeline = nullptr;
        }
    };
    const auto releaseLayout = [](WGPUBindGroupLayout& layout) {
        if (layout != nullptr) {
            wgpuBindGroupLayoutRelease(layout);
            layout = nullptr;
        }
    };
    releasePipeline(g_preprocessPipeline);
    releasePipeline(g_mip4Pipeline);
    releasePipeline(g_gtaoPipeline);
    releasePipeline(g_denoisePipeline);
    releaseLayout(g_preprocessLayout);
    releaseLayout(g_mip4Layout);
    releaseLayout(g_gtaoLayout);
    releaseLayout(g_denoiseLayout);
    if (g_compositePipeline != nullptr) {
        wgpuRenderPipelineRelease(g_compositePipeline);
        g_compositePipeline = nullptr;
    }
    if (g_compositeDebugPipeline != nullptr) {
        wgpuRenderPipelineRelease(g_compositeDebugPipeline);
        g_compositeDebugPipeline = nullptr;
    }
    releaseLayout(g_compositeLayout);
    releaseLayout(g_compositeDebugLayout);
    if (g_hilbertLutView != nullptr) {
        wgpuTextureViewRelease(g_hilbertLutView);
        g_hilbertLutView = nullptr;
    }
    if (g_hilbertLut != nullptr) {
        wgpuTextureRelease(g_hilbertLut);
        g_hilbertLut = nullptr;
    }
    g_cvarEnabled = g_cvarQuality = g_cvarRadius = g_cvarIntensity = 0;
    g_cvarHalfRes = g_cvarDebugView = 0;
    g_computeType = g_drawType = 0;
    g_afterOpaqueHook = 0;
    g_controlsWindow = 0;
    return MOD_OK;
}
}
