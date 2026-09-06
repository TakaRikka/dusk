#include "internal.hpp"
#include "registry.hpp"
#include "window.hpp"

#include <borealis/log.hpp>
#include "dusk/gfx.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/gfx.h"

#include <aurora/gfx.hpp>
#include <aurora/webgpu.hpp>
#include <dolphin/gx/GXAurora.h>
#include <fmt/format.h>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace dusk::mods {
namespace {

constexpr borealis::Log Log{"dusk::mods::gfx"};

enum class GfxSlotKind : uint8_t {
    DrawType,
    StageHook,
    ComputeType,
    PresentTarget,
};

enum class GfxStreamBuffer : uint8_t {
    Verts,
    Indices,
    Uniform,
    Storage,
};

enum class PresentTargetStatus : uint8_t {
    Pending,
    Ready,
    TransientUnavailable,
    Lost,
    Error,
};

struct PresentTargetState {
    wgpu::Surface surface;
    wgpu::SurfaceConfiguration configuration;
    wgpu::Texture currentTexture;
    wgpu::TextureView currentView;
    std::atomic<PresentTargetStatus> status = PresentTargetStatus::Pending;
    bool configured = false;
    bool presentPending = false;
    bool vsync = true;
};

bool present_target_failed(const std::shared_ptr<PresentTargetState>& state) {
    if (state == nullptr) {
        return true;
    }
    const auto status = state->status.load(std::memory_order_acquire);
    return status == PresentTargetStatus::Lost || status == PresentTargetStatus::Error;
}

struct GfxSlot {
    GfxSlotKind kind = GfxSlotKind::DrawType;
    ModContext* ownerContext = nullptr;
    std::string ownerId;
    void* userData = nullptr;

    GfxDrawFn drawFn = nullptr;
    aurora::gfx::DrawTypeId auroraDrawId = aurora::gfx::InvalidDrawType;

    GfxStageFn stageFn = nullptr;
    GfxStage stage = GFX_STAGE_SCENE_AFTER_TERRAIN;

    GfxComputeFn computeFn = nullptr;
    aurora::gfx::EncoderTaskId auroraTaskId = aurora::gfx::InvalidEncoderTask;

    GfxPresentFn presentFn = nullptr;
    std::shared_ptr<PresentTargetState> presentState;
    WindowHandle window = 0;
    uint32_t targetWidth = 0;
    uint32_t targetHeight = 0;
    WGPUTextureUsage targetUsage = WGPUTextureUsage_RenderAttachment;
    WGPUTextureFormat preferredFormat = WGPUTextureFormat_Undefined;
    WGPUCompositeAlphaMode preferredAlphaMode = WGPUCompositeAlphaMode_Auto;
    uint32_t lastPresentFrame = UINT32_MAX;
};

struct WorkerFailure {
    std::string modId;
    std::string message;
};

std::mutex s_mutex;
using GfxSlotMap = svc::SlotMap<GfxSlot>;
GfxSlotMap s_slots;
std::vector<WorkerFailure> s_workerFailures;
bool s_modOffscreenOpen = false;

GfxSlotMap::Entry* resolve_entry_locked(uint64_t handle, GfxSlotKind kind) {
    auto* entry = s_slots.find(handle);
    if (entry == nullptr || entry->value.kind != kind) {
        return nullptr;
    }
    return entry;
}

GfxSlot* resolve_slot_locked(uint64_t handle, GfxSlotKind kind) {
    auto* entry = resolve_entry_locked(handle, kind);
    return entry != nullptr ? &entry->value : nullptr;
}

GfxSlot* resolve_owned_slot_locked(LoadedMod& mod, uint64_t handle, GfxSlotKind kind) {
    auto* entry = s_slots.find_owned(handle, mod);
    if (entry == nullptr || entry->value.kind != kind) {
        return nullptr;
    }
    return &entry->value;
}

void take_mod_types_locked(LoadedMod& owner, std::vector<aurora::gfx::DrawTypeId>& drawIds,
    std::vector<aurora::gfx::EncoderTaskId>& taskIds) {
    std::vector<uint64_t> drawHandles;
    std::vector<uint64_t> taskHandles;
    s_slots.for_each([&](uint64_t handle, const auto& entry) {
        if (entry.owner != &owner) {
            return;
        }
        const auto& slot = entry.value;
        if (slot.kind == GfxSlotKind::DrawType && slot.auroraDrawId != aurora::gfx::InvalidDrawType)
        {
            drawHandles.push_back(handle);
        } else if ((slot.kind == GfxSlotKind::ComputeType ||
                       slot.kind == GfxSlotKind::PresentTarget) &&
                   slot.auroraTaskId != aurora::gfx::InvalidEncoderTask)
        {
            taskHandles.push_back(handle);
        }
    });
    for (const auto handle : drawHandles) {
        auto* entry = s_slots.find(handle);
        drawIds.push_back(std::exchange(entry->value.auroraDrawId, aurora::gfx::InvalidDrawType));
    }
    for (const auto handle : taskHandles) {
        auto* entry = s_slots.find(handle);
        taskIds.push_back(
            std::exchange(entry->value.auroraTaskId, aurora::gfx::InvalidEncoderTask));
    }
}

void unregister_aurora_types(const std::vector<aurora::gfx::DrawTypeId>& drawIds,
    const std::vector<aurora::gfx::EncoderTaskId>& taskIds) {
    for (const auto id : drawIds) {
        aurora::gfx::unregister_draw_type(id);
    }
    for (const auto id : taskIds) {
        aurora::gfx::unregister_encoder_task_type(id);
    }
}

void gfx_mod_deactivating(LoadedMod& mod) {
    std::vector<aurora::gfx::DrawTypeId> drawIds;
    std::vector<aurora::gfx::EncoderTaskId> taskIds;
    {
        std::lock_guard lock{s_mutex};
        take_mod_types_locked(mod, drawIds, taskIds);
    }
    unregister_aurora_types(drawIds, taskIds);
    if (!drawIds.empty() || !taskIds.empty()) {
        aurora::gfx::synchronize();
    }
}

GfxAttachmentSemantic gfx_attachment_semantic(aurora::gfx::ColorAttachmentSemantic semantic) {
    switch (semantic) {
    case aurora::gfx::ColorAttachmentSemantic::SceneColor:
        return GFX_ATTACHMENT_SCENE_COLOR;
    case aurora::gfx::ColorAttachmentSemantic::Normal:
        return GFX_ATTACHMENT_NORMAL;
    case aurora::gfx::ColorAttachmentSemantic::Auxiliary:
        return GFX_ATTACHMENT_AUXILIARY;
    }
    return GFX_ATTACHMENT_AUXILIARY;
}

GfxRenderTargetLayout gfx_render_target_layout(const aurora::gfx::RenderTargetLayout& layout) {
    GfxRenderTargetLayout result = GFX_RENDER_TARGET_LAYOUT_INIT;
    result.key = layout.key;
    result.color_attachment_count =
        std::min<uint32_t>(layout.colorAttachmentCount, GFX_MAX_COLOR_ATTACHMENTS);
    for (uint32_t i = 0; i < result.color_attachment_count; ++i) {
        result.color_attachments[i] = {
            .semantic = gfx_attachment_semantic(layout.colorAttachments[i].semantic),
            .format = static_cast<WGPUTextureFormat>(layout.colorAttachments[i].format),
            .width = layout.colorAttachments[i].width,
            .height = layout.colorAttachments[i].height,
        };
    }
    result.depth_stencil_format = static_cast<WGPUTextureFormat>(layout.depthStencilFormat);
    result.sample_count = layout.sampleCount;
    return result;
}

void draw_trampoline(const aurora::gfx::DrawContext& ctx, const wgpu::RenderPassEncoder& pass,
    const void* payload, size_t payloadSize, void* userdata) {
    const auto handle = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(userdata));
    GfxDrawFn fn = nullptr;
    void* userData = nullptr;
    ModContext* modContext = nullptr;
    std::string ownerId;
    {
        std::lock_guard lock{s_mutex};
        auto* entry = resolve_entry_locked(handle, GfxSlotKind::DrawType);
        if (entry == nullptr) {
            return;
        }
        const auto& slot = entry->value;
        fn = slot.drawFn;
        userData = slot.userData;
        modContext = slot.ownerContext;
        ownerId = slot.ownerId;
    }

    GfxDrawContext drawContext{
        .struct_size = sizeof(GfxDrawContext),
        .device = ctx.device.Get(),
        .queue = ctx.queue.Get(),
        .pass = pass.Get(),
        .vertex_buffer = ctx.vertexBuffer.Get(),
        .index_buffer = ctx.indexBuffer.Get(),
        .uniform_buffer = ctx.uniformBuffer.Get(),
        .storage_buffer = ctx.storageBuffer.Get(),
        .color_format = static_cast<WGPUTextureFormat>(
            ctx.layout.colorAttachments[GFX_SCENE_COLOR_ATTACHMENT_INDEX].format),
        .depth_format = static_cast<WGPUTextureFormat>(ctx.layout.depthStencilFormat),
        .sample_count = ctx.layout.sampleCount,
        .uses_reversed_z = aurora::gfx::uses_reversed_z(),
        .layout = gfx_render_target_layout(ctx.layout),
    };

    std::string failure;
    try {
        fn(modContext, &drawContext, payload, payloadSize, userData);
        return;
    } catch (const std::exception& e) {
        failure = fmt::format("exception in gfx draw callback: {}", e.what());
    } catch (...) {
        failure = "unknown exception in gfx draw callback";
    }

    std::lock_guard lock{s_mutex};
    WorkerFailure record{
        .modId = std::move(ownerId),
        .message = std::move(failure),
    };
    s_workerFailures.push_back(std::move(record));
}

void compute_trampoline(const aurora::gfx::EncoderTaskContext& ctx, const wgpu::CommandEncoder& cmd,
    const void* payload, size_t payloadSize, void* userdata) {
    const auto handle = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(userdata));
    GfxComputeFn fn = nullptr;
    void* userData = nullptr;
    ModContext* modContext = nullptr;
    std::string ownerId;
    {
        std::lock_guard lock{s_mutex};
        auto* entry = resolve_entry_locked(handle, GfxSlotKind::ComputeType);
        if (entry == nullptr) {
            return;
        }
        const auto& slot = entry->value;
        fn = slot.computeFn;
        userData = slot.userData;
        modContext = slot.ownerContext;
        ownerId = slot.ownerId;
    }

    GfxComputeContext computeContext{
        .struct_size = sizeof(GfxComputeContext),
        .device = ctx.device.Get(),
        .queue = ctx.queue.Get(),
        .encoder = cmd.Get(),
        .vertex_buffer = ctx.vertexBuffer.Get(),
        .index_buffer = ctx.indexBuffer.Get(),
        .uniform_buffer = ctx.uniformBuffer.Get(),
        .storage_buffer = ctx.storageBuffer.Get(),
    };

    std::string failure;
    try {
        fn(modContext, &computeContext, payload, payloadSize, userData);
        return;
    } catch (const std::exception& e) {
        failure = fmt::format("exception in gfx compute callback: {}", e.what());
    } catch (...) {
        failure = "unknown exception in gfx compute callback";
    }

    std::lock_guard lock{s_mutex};
    WorkerFailure record{
        .modId = std::move(ownerId),
        .message = std::move(failure),
    };
    s_workerFailures.push_back(std::move(record));
}

template <typename T>
bool contains(const T* values, size_t count, T value) {
    for (size_t i = 0; i < count; ++i) {
        if (values[i] == value) {
            return true;
        }
    }
    return false;
}

bool configure_present_target(const std::shared_ptr<PresentTargetState>& state,
    const aurora::gfx::EncoderTaskContext& ctx, uint32_t width, uint32_t height,
    WGPUTextureUsage requestedUsage, WGPUTextureFormat preferredFormat,
    WGPUCompositeAlphaMode preferredAlphaMode, bool vsync) {
    if (width == 0 || height == 0) {
        state->status.store(PresentTargetStatus::TransientUnavailable, std::memory_order_release);
        return false;
    }
    if (!state->surface) {
        state->status.store(PresentTargetStatus::Error, std::memory_order_release);
        return false;
    }

    const auto adapter = ctx.device.GetAdapter();
    wgpu::SurfaceCapabilities capabilities;
    if (!adapter ||
        state->surface.GetCapabilities(adapter, &capabilities) != wgpu::Status::Success ||
        capabilities.formatCount == 0 || capabilities.presentModeCount == 0 ||
        capabilities.alphaModeCount == 0)
    {
        state->status.store(PresentTargetStatus::Error, std::memory_order_release);
        return false;
    }

    auto format = static_cast<wgpu::TextureFormat>(preferredFormat);
    if (format == wgpu::TextureFormat::Undefined ||
        !contains(capabilities.formats, capabilities.formatCount, format))
    {
        format = aurora::gfx::color_format();
    }
    if (format == wgpu::TextureFormat::Undefined ||
        !contains(capabilities.formats, capabilities.formatCount, format))
    {
        format = capabilities.formats[0];
    }

    const auto presentMode = aurora::webgpu::select_present_mode(capabilities);

    auto alphaMode = static_cast<wgpu::CompositeAlphaMode>(preferredAlphaMode);
    if (alphaMode != wgpu::CompositeAlphaMode::Auto &&
        !contains(capabilities.alphaModes, capabilities.alphaModeCount, alphaMode))
    {
        alphaMode = capabilities.alphaModes[0];
    }

    auto usage = static_cast<wgpu::TextureUsage>(requestedUsage);
    if (usage == wgpu::TextureUsage::None) {
        usage = wgpu::TextureUsage::RenderAttachment;
    }
    if ((usage & wgpu::TextureUsage::RenderAttachment) == wgpu::TextureUsage::None ||
        (usage & ~capabilities.usages) != wgpu::TextureUsage::None)
    {
        state->status.store(PresentTargetStatus::Error, std::memory_order_release);
        return false;
    }

    state->configuration = wgpu::SurfaceConfiguration{
        .device = ctx.device,
        .format = format,
        .usage = usage,
        .width = width,
        .height = height,
        .alphaMode = alphaMode,
        .presentMode = presentMode,
    };
    state->surface.Configure(&state->configuration);
    state->configured = true;
    state->vsync = vsync;
    state->status.store(PresentTargetStatus::Pending, std::memory_order_release);
    return true;
}

void present_trampoline(const aurora::gfx::EncoderTaskContext& ctx, const wgpu::CommandEncoder& cmd,
    const void* payload, size_t payloadSize, void* userdata) {
    const auto handle = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(userdata));
    GfxPresentFn fn = nullptr;
    void* userData = nullptr;
    ModContext* modContext = nullptr;
    std::string ownerId;
    std::shared_ptr<PresentTargetState> state;
    uint32_t width = 0;
    uint32_t height = 0;
    WGPUTextureUsage usage = WGPUTextureUsage_RenderAttachment;
    WGPUTextureFormat preferredFormat = WGPUTextureFormat_Undefined;
    WGPUCompositeAlphaMode preferredAlphaMode = WGPUCompositeAlphaMode_Auto;
    {
        std::lock_guard lock{s_mutex};
        auto* entry = resolve_entry_locked(handle, GfxSlotKind::PresentTarget);
        if (entry == nullptr) {
            return;
        }
        const auto& slot = entry->value;
        fn = slot.presentFn;
        userData = slot.userData;
        modContext = slot.ownerContext;
        ownerId = slot.ownerId;
        state = slot.presentState;
        width = slot.targetWidth;
        height = slot.targetHeight;
        usage = slot.targetUsage;
        preferredFormat = slot.preferredFormat;
        preferredAlphaMode = slot.preferredAlphaMode;
    }
    if (fn == nullptr || state == nullptr || present_target_failed(state)) {
        return;
    }

    state->presentPending = false;
    state->currentView = {};
    state->currentTexture = {};
    const bool vsync = aurora::webgpu::vsync_enabled();
    if (!state->configured || state->configuration.width != width ||
        state->configuration.height != height || state->vsync != vsync)
    {
        if (!configure_present_target(
                state, ctx, width, height, usage, preferredFormat, preferredAlphaMode, vsync))
        {
            return;
        }
    }

    wgpu::SurfaceTexture surfaceTexture;
    state->surface.GetCurrentTexture(&surfaceTexture);
    if (surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal &&
        surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal)
    {
        if (surfaceTexture.status == wgpu::SurfaceGetCurrentTextureStatus::Lost) {
            state->status.store(PresentTargetStatus::Lost, std::memory_order_release);
            state->configured = false;
        } else if (surfaceTexture.status == wgpu::SurfaceGetCurrentTextureStatus::Outdated) {
            state->status.store(
                PresentTargetStatus::TransientUnavailable, std::memory_order_release);
            state->configured = false;
        } else if (surfaceTexture.status == wgpu::SurfaceGetCurrentTextureStatus::Error) {
            state->status.store(PresentTargetStatus::Error, std::memory_order_release);
            state->configured = false;
        } else {
            state->status.store(
                PresentTargetStatus::TransientUnavailable, std::memory_order_release);
        }
        return;
    }

    state->currentTexture = std::move(surfaceTexture.texture);
    state->currentView = state->currentTexture.CreateView();
    if (!state->currentTexture || !state->currentView) {
        state->status.store(PresentTargetStatus::Error, std::memory_order_release);
        return;
    }

    const GfxPresentContext presentContext{
        .struct_size = sizeof(GfxPresentContext),
        .device = ctx.device.Get(),
        .queue = ctx.queue.Get(),
        .encoder = cmd.Get(),
        .target_texture = state->currentTexture.Get(),
        .target_view = state->currentView.Get(),
        .target_format = static_cast<WGPUTextureFormat>(state->configuration.format),
        .target_width = width,
        .target_height = height,
        .vertex_buffer = ctx.vertexBuffer.Get(),
        .index_buffer = ctx.indexBuffer.Get(),
        .uniform_buffer = ctx.uniformBuffer.Get(),
        .storage_buffer = ctx.storageBuffer.Get(),
    };

    std::string failure;
    try {
        fn(modContext, &presentContext, payload, payloadSize, userData);
        state->presentPending = true;
        state->status.store(PresentTargetStatus::Ready, std::memory_order_release);
        if (surfaceTexture.status == wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal) {
            state->configured = false;
        }
        return;
    } catch (const std::exception& e) {
        failure = fmt::format("exception in gfx present callback: {}", e.what());
    } catch (...) {
        failure = "unknown exception in gfx present callback";
    }

    state->presentPending = false;
    state->status.store(PresentTargetStatus::Error, std::memory_order_release);
    std::lock_guard lock{s_mutex};
    s_workerFailures.push_back(WorkerFailure{
        .modId = std::move(ownerId),
        .message = std::move(failure),
    });
}

void present_after_submit_trampoline(
    const aurora::gfx::EncoderTaskCompletionContext&, const void*, size_t, void* userdata) {
    const auto handle = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(userdata));
    std::shared_ptr<PresentTargetState> state;
    {
        std::lock_guard lock{s_mutex};
        const auto* entry = resolve_entry_locked(handle, GfxSlotKind::PresentTarget);
        if (entry == nullptr) {
            return;
        }
        state = entry->value.presentState;
    }
    if (state == nullptr || !state->presentPending) {
        return;
    }

    const bool presented = state->surface.Present();
    state->presentPending = false;
    state->currentView = {};
    state->currentTexture = {};
    if (!presented) {
        state->configured = false;
        state->status.store(PresentTargetStatus::Error, std::memory_order_release);
    }
}

}  // namespace

ModResult gfx_register_draw_type(
    LoadedMod& mod, const char* label, GfxDrawFn draw, void* userData, uint64_t& outHandle) {
    outHandle = 0;

    uint64_t handle = 0;
    {
        std::lock_guard lock{s_mutex};
        handle = s_slots.emplace(mod, GfxSlot{
                                          .kind = GfxSlotKind::DrawType,
                                          .ownerContext = mod.context.get(),
                                          .ownerId = mod.metadata.id,
                                          .userData = userData,
                                          .drawFn = draw,
                                      });
    }

    const auto auroraId = aurora::gfx::register_draw_type(aurora::gfx::DrawTypeDescriptor{
        .label = label,
        .draw = draw_trampoline,
        .userdata = reinterpret_cast<void*>(static_cast<uintptr_t>(handle)),
    });
    if (auroraId == aurora::gfx::InvalidDrawType) {
        std::lock_guard lock{s_mutex};
        s_slots.erase_owned(handle, mod);
        return MOD_ERROR;
    }

    {
        std::lock_guard lock{s_mutex};
        if (auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::DrawType)) {
            slot->auroraDrawId = auroraId;
        }
    }
    outHandle = handle;
    return MOD_OK;
}

ModResult gfx_unregister_draw_type(LoadedMod& mod, uint64_t handle) {
    aurora::gfx::DrawTypeId auroraId = aurora::gfx::InvalidDrawType;
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::DrawType);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        auroraId = slot->auroraDrawId;
        s_slots.erase_owned(handle, mod);
    }
    aurora::gfx::unregister_draw_type(auroraId);
    return MOD_OK;
}

ModResult gfx_push_draw(LoadedMod& mod, uint64_t handle, const void* payload, size_t payloadSize) {
    aurora::gfx::DrawTypeId auroraId = aurora::gfx::InvalidDrawType;
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::DrawType);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        auroraId = slot->auroraDrawId;
    }
    if (!aurora::gfx::push_custom_draw(auroraId, payload, payloadSize)) {
        return MOD_UNAVAILABLE;
    }
    return MOD_OK;
}

ModResult gfx_push_stream(
    GfxStreamBuffer buffer, const void* data, size_t size, size_t alignment, GfxRange& outRange) {
    aurora::gfx::Range range;
    const auto* bytes = static_cast<const uint8_t*>(data);
    switch (buffer) {
    case GfxStreamBuffer::Verts:
        range = aurora::gfx::push_verts(bytes, size, alignment);
        break;
    case GfxStreamBuffer::Indices:
        range = aurora::gfx::push_indices(bytes, size, alignment);
        break;
    case GfxStreamBuffer::Uniform:
        range = aurora::gfx::push_uniform(bytes, size);
        break;
    case GfxStreamBuffer::Storage:
        range = aurora::gfx::push_storage(bytes, size);
        break;
    }
    if (range.size == 0) {
        return MOD_UNAVAILABLE;
    }
    outRange = GfxRange{.offset = range.offset, .size = range.size};
    return MOD_OK;
}

ModResult gfx_register_stage_hook(
    LoadedMod& mod, GfxStage stage, GfxStageFn callback, void* userData, uint64_t& outHandle) {
    outHandle = 0;
    std::lock_guard lock{s_mutex};
    outHandle = s_slots.emplace(mod, GfxSlot{
                                         .kind = GfxSlotKind::StageHook,
                                         .ownerContext = mod.context.get(),
                                         .ownerId = mod.metadata.id,
                                         .userData = userData,
                                         .stageFn = callback,
                                         .stage = stage,
                                     });
    return MOD_OK;
}

ModResult gfx_unregister_stage_hook(LoadedMod& mod, uint64_t handle) {
    std::lock_guard lock{s_mutex};
    auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::StageHook);
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    s_slots.erase_owned(handle, mod);
    return MOD_OK;
}

ModResult gfx_resolve_pass(LoadedMod& mod, const GfxResolveDesc& desc, GfxResolvedTargets& out) {
    out = GfxResolvedTargets{.struct_size = sizeof(GfxResolvedTargets)};
    if (aurora::gfx::is_offscreen() && !s_modOffscreenOpen) {
        Log.error(
            "[{}] resolve_pass: the active offscreen pass belongs to the game", mod.metadata.id);
        return MOD_UNAVAILABLE;
    }
    const bool closesModOffscreen = s_modOffscreenOpen;

    aurora::gfx::ResolvedTargets resolved;
    if (!aurora::gfx::resolve_pass(
            aurora::gfx::ResolveDesc{.color = desc.color, .depth = desc.depth}, resolved))
    {
        return MOD_UNAVAILABLE;
    }
    if (closesModOffscreen) {
        s_modOffscreenOpen = false;
    }

    out.color = resolved.color.Get();
    out.depth = resolved.depth.Get();
    out.color_format = static_cast<WGPUTextureFormat>(resolved.colorFormat);
    out.width = resolved.width;
    out.height = resolved.height;
    return MOD_OK;
}

ModResult gfx_create_pass(LoadedMod& mod, uint32_t width, uint32_t height) {
    if (aurora::gfx::is_offscreen()) {
        Log.error("[{}] create_pass: an offscreen pass is already active", mod.metadata.id);
        return MOD_UNAVAILABLE;
    }
    if (!aurora::gfx::create_pass(width, height)) {
        return MOD_UNAVAILABLE;
    }
    s_modOffscreenOpen = true;
    return MOD_OK;
}

ModResult gfx_register_compute_type(
    LoadedMod& mod, const char* label, GfxComputeFn callback, void* userData, uint64_t& outHandle) {
    outHandle = 0;

    uint64_t handle = 0;
    {
        std::lock_guard lock{s_mutex};
        handle = s_slots.emplace(mod, GfxSlot{
                                          .kind = GfxSlotKind::ComputeType,
                                          .ownerContext = mod.context.get(),
                                          .ownerId = mod.metadata.id,
                                          .userData = userData,
                                          .computeFn = callback,
                                      });
    }

    const auto auroraId =
        aurora::gfx::register_encoder_task_type(aurora::gfx::EncoderTaskDescriptor{
            .label = label,
            .callback = compute_trampoline,
            .userdata = reinterpret_cast<void*>(static_cast<uintptr_t>(handle)),
        });
    if (auroraId == aurora::gfx::InvalidEncoderTask) {
        std::lock_guard lock{s_mutex};
        s_slots.erase_owned(handle, mod);
        return MOD_ERROR;
    }

    {
        std::lock_guard lock{s_mutex};
        if (auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::ComputeType)) {
            slot->auroraTaskId = auroraId;
        }
    }
    outHandle = handle;
    return MOD_OK;
}

ModResult gfx_unregister_compute_type(LoadedMod& mod, uint64_t handle) {
    aurora::gfx::EncoderTaskId auroraId = aurora::gfx::InvalidEncoderTask;
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::ComputeType);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        auroraId = slot->auroraTaskId;
        s_slots.erase_owned(handle, mod);
    }
    aurora::gfx::unregister_encoder_task_type(auroraId);
    return MOD_OK;
}

ModResult gfx_push_compute(
    LoadedMod& mod, uint64_t handle, const void* payload, size_t payloadSize) {
    aurora::gfx::EncoderTaskId auroraId = aurora::gfx::InvalidEncoderTask;
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::ComputeType);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        auroraId = slot->auroraTaskId;
    }
    if (!aurora::gfx::push_encoder_task(auroraId, payload, payloadSize)) {
        return MOD_UNAVAILABLE;
    }
    return MOD_OK;
}

ModResult gfx_register_present_target(LoadedMod& mod, wgpu::Surface surface, WindowHandle window,
    const GfxPresentTargetDesc& desc, uint64_t& outHandle) {
    outHandle = 0;
    auto state = std::make_shared<PresentTargetState>();
    state->surface = std::move(surface);

    uint64_t handle = 0;
    {
        std::lock_guard lock{s_mutex};
        handle = s_slots.emplace(mod, GfxSlot{
                                          .kind = GfxSlotKind::PresentTarget,
                                          .ownerContext = mod.context.get(),
                                          .ownerId = mod.metadata.id,
                                          .userData = desc.user_data,
                                          .presentFn = desc.render,
                                          .presentState = state,
                                          .window = window,
                                          .targetWidth = desc.width,
                                          .targetHeight = desc.height,
                                          .targetUsage = desc.usage == WGPUTextureUsage_None ?
                                                             WGPUTextureUsage_RenderAttachment :
                                                             desc.usage,
                                          .preferredFormat = desc.preferred_format,
                                          .preferredAlphaMode = desc.preferred_alpha_mode,
                                      });
    }

    const auto auroraId =
        aurora::gfx::register_encoder_task_type(aurora::gfx::EncoderTaskDescriptor{
            .label = desc.label,
            .callback = present_trampoline,
            .userdata = reinterpret_cast<void*>(static_cast<uintptr_t>(handle)),
            .afterSubmit = present_after_submit_trampoline,
        });
    if (auroraId == aurora::gfx::InvalidEncoderTask) {
        std::lock_guard lock{s_mutex};
        s_slots.erase_owned(handle, mod);
        return MOD_ERROR;
    }

    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::PresentTarget);
        if (slot == nullptr) {
            aurora::gfx::unregister_encoder_task_type(auroraId);
            return MOD_ERROR;
        }
        slot->auroraTaskId = auroraId;
    }
    outHandle = handle;
    return MOD_OK;
}

ModResult gfx_unregister_present_target(LoadedMod& mod, uint64_t handle) {
    aurora::gfx::EncoderTaskId auroraId = aurora::gfx::InvalidEncoderTask;
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::PresentTarget);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        auroraId = slot->auroraTaskId;
    }

    if (auroraId != aurora::gfx::InvalidEncoderTask) {
        aurora::gfx::unregister_encoder_task_type(auroraId);
        aurora::gfx::synchronize();
    }

    std::optional<GfxSlotMap::Entry> removed;
    {
        std::lock_guard lock{s_mutex};
        removed = s_slots.take_owned(handle, mod);
    }
    if (!removed.has_value()) {
        return MOD_INVALID_ARGUMENT;
    }
    if (removed->value.presentState != nullptr && removed->value.presentState->configured) {
        removed->value.presentState->surface.Unconfigure();
    }
    if (removed->value.window != 0) {
        svc::window_release_for_graphics(mod, removed->value.window);
    }
    return MOD_OK;
}

ModResult gfx_resize_present_target(
    LoadedMod& mod, uint64_t handle, uint32_t width, uint32_t height) {
    std::lock_guard lock{s_mutex};
    auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::PresentTarget);
    if (slot == nullptr || width == 0 || height == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    if (slot->window != 0) {
        return MOD_UNSUPPORTED;
    }
    slot->targetWidth = width;
    slot->targetHeight = height;
    return MOD_OK;
}

ModResult gfx_push_present(
    LoadedMod& mod, uint64_t handle, const void* payload, size_t payloadSize) {
    WindowHandle window = 0;
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::PresentTarget);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        if (present_target_failed(slot->presentState)) {
            return MOD_ERROR;
        }
        window = slot->window;
    }

    uint32_t width = 0;
    uint32_t height = 0;
    if (window != 0 && !svc::window_get_pixel_size(mod, window, width, height)) {
        return MOD_UNAVAILABLE;
    }

    aurora::gfx::EncoderTaskId auroraId = aurora::gfx::InvalidEncoderTask;
    const uint32_t frame = aurora::gfx::current_frame();
    {
        std::lock_guard lock{s_mutex};
        auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::PresentTarget);
        if (slot == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
        if (present_target_failed(slot->presentState)) {
            return MOD_ERROR;
        }
        if (slot->lastPresentFrame == frame) {
            return MOD_CONFLICT;
        }
        if (window != 0) {
            slot->targetWidth = width;
            slot->targetHeight = height;
        }
        auroraId = slot->auroraTaskId;
    }
    if (!aurora::gfx::push_encoder_task(auroraId, payload, payloadSize)) {
        return MOD_UNAVAILABLE;
    }
    {
        std::lock_guard lock{s_mutex};
        if (auto* slot = resolve_owned_slot_locked(mod, handle, GfxSlotKind::PresentTarget)) {
            slot->lastPresentFrame = frame;
        }
    }
    return MOD_OK;
}

void gfx_run_stage(
    GfxStage stage, const view_class* gameView, const view_port_class* gameViewport) {
    struct StageEntry {
        uint64_t handle;
        GfxStageFn fn;
        void* userData;
        ModContext* context;
        LoadedMod* owner;
    };

    std::vector<StageEntry> entries;
    {
        std::lock_guard lock{s_mutex};
        s_slots.for_each([&](uint64_t handle, const auto& slotEntry) {
            const auto& slot = slotEntry.value;
            if (slot.kind == GfxSlotKind::StageHook && slot.stage == stage) {
                entries.push_back(StageEntry{
                    .handle = handle,
                    .fn = slot.stageFn,
                    .userData = slot.userData,
                    .context = slot.ownerContext,
                    .owner = slotEntry.owner,
                });
            }
        });
    }
    if (entries.empty()) {
        return;
    }

    const GfxStageContext stageContext{
        .struct_size = sizeof(GfxStageContext),
        .stage = stage,
        .game_view = gameView,
        .game_viewport = gameViewport,
    };

    AuroraGXSync();
    for (const auto& entry : entries) {
        {
            std::lock_guard lock{s_mutex};
            if (resolve_slot_locked(entry.handle, GfxSlotKind::StageHook) == nullptr) {
                continue;
            }
        }
        if (!entry.owner->active) {
            continue;
        }

        const bool wasOffscreen = aurora::gfx::is_offscreen();
        try {
            entry.fn(entry.context, &stageContext, entry.userData);
        } catch (const std::exception& e) {
            fail_mod(*entry.owner, MOD_ERROR,
                fmt::format("exception in gfx stage callback: {}", e.what()));
        } catch (...) {
            fail_mod(*entry.owner, MOD_ERROR, "unknown exception in gfx stage callback");
        }

        AuroraGXSync();
        if (aurora::gfx::is_offscreen() != wasOffscreen) {
            aurora::gfx::ResolvedTargets discarded;
            aurora::gfx::resolve_pass(
                aurora::gfx::ResolveDesc{.color = false, .depth = false}, discarded);
            s_modOffscreenOpen = false;
            fail_mod(*entry.owner, MOD_ERROR,
                "gfx stage callback returned with its offscreen pass still open");
        }
    }
}

void gfx_mod_detached(LoadedMod& mod) {
    gfx_mod_deactivating(mod);

    std::vector<GfxSlotMap::Entry> entries;
    {
        std::lock_guard lock{s_mutex};
        entries = s_slots.take_all(mod);
    }
    for (auto& entry : entries) {
        auto& slot = entry.value;
        if (slot.kind != GfxSlotKind::PresentTarget) {
            continue;
        }
        if (slot.presentState != nullptr && slot.presentState->configured) {
            slot.presentState->surface.Unconfigure();
        }
        if (slot.window != 0) {
            svc::window_release_for_graphics(mod, slot.window);
        }
    }
}

void gfx_frame_begin() {
    std::vector<WorkerFailure> failures;
    {
        std::lock_guard lock{s_mutex};
        failures.swap(s_workerFailures);
    }
    if (failures.empty()) {
        return;
    }

    for (const auto& failure : failures) {
        for (auto& mod : ModLoader::instance().mods()) {
            if (mod.metadata.id == failure.modId && mod.active) {
                gfx_mod_detached(mod);
                fail_mod(mod, MOD_ERROR, failure.message);
                break;
            }
        }
    }
}

}  // namespace dusk::mods

namespace dusk::mods::svc {
namespace {

ModResult gfx_get_device_info(ModContext* context, GfxDeviceInfo* outInfo) {
    constexpr uint32_t v0Size = offsetof(GfxDeviceInfo, instance);
    if (outInfo == nullptr || outInfo->struct_size < v0Size) {
        return MOD_INVALID_ARGUMENT;
    }
    const uint32_t structSize = outInfo->struct_size;
    outInfo->device = nullptr;
    outInfo->queue = nullptr;
    outInfo->color_format = WGPUTextureFormat_Undefined;
    outInfo->depth_format = WGPUTextureFormat_Undefined;
    outInfo->sample_count = 1;
    outInfo->uses_reversed_z = false;
    if (structSize >= sizeof(GfxDeviceInfo)) {
        outInfo->instance = nullptr;
        outInfo->adapter = nullptr;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    const auto device = aurora::gfx::device();
    outInfo->device = device.Get();
    outInfo->queue = aurora::gfx::queue().Get();
    outInfo->color_format = static_cast<WGPUTextureFormat>(aurora::gfx::color_format());
    outInfo->depth_format = static_cast<WGPUTextureFormat>(aurora::gfx::depth_format());
    outInfo->sample_count = aurora::gfx::sample_count();
    outInfo->uses_reversed_z = aurora::gfx::uses_reversed_z();
    if (structSize >= sizeof(GfxDeviceInfo)) {
        const auto adapter = device.GetAdapter();
        const auto instance = adapter ? adapter.GetInstance() : wgpu::Instance{};
        outInfo->instance = instance.Get();
        outInfo->adapter = adapter.Get();
    }
    return MOD_OK;
}

ModResult gfx_get_scene_target_layout(ModContext* context, GfxRenderTargetLayout* outLayout) {
    if (outLayout == nullptr || outLayout->struct_size < sizeof(GfxRenderTargetLayout) ||
        mod_from_context(context) == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }

    const uint32_t structSize = outLayout->struct_size;
    *outLayout = gfx_render_target_layout(aurora::gfx::scene_render_target_layout());
    outLayout->struct_size = structSize;
    return MOD_OK;
}

void* gfx_get_proc_address(ModContext* context, const char* name) {
    if (mod_from_context(context) == nullptr || name == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<void*>(wgpuGetProcAddress(WGPUStringView{name, WGPU_STRLEN}));
}

bool valid_present_desc(const GfxPresentTargetDesc* desc) {
    return desc != nullptr && desc->struct_size >= sizeof(GfxPresentTargetDesc) &&
           desc->render != nullptr;
}

ModResult gfx_register_present_target_impl(ModContext* context, WGPUSurface surface,
    const GfxPresentTargetDesc* desc, GfxPresentTargetHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || surface == nullptr || !valid_present_desc(desc) || outHandle == nullptr ||
        desc->width == 0 || desc->height == 0)
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result = gfx_register_present_target(*mod, wgpu::Surface{surface}, 0, *desc, handle);
    if (result == MOD_OK) {
        *outHandle = handle;
    }
    return result;
}

ModResult gfx_register_window_present_target_impl(ModContext* context, WindowHandle window,
    const GfxPresentTargetDesc* desc, GfxPresentTargetHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || window == 0 || !valid_present_desc(desc) || outHandle == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    SDL_Window* sdlWindow = nullptr;
    const auto acquireResult = window_acquire_for_graphics(*mod, window, sdlWindow);
    if (acquireResult != MOD_OK) {
        return acquireResult;
    }
    uint32_t width = 0;
    uint32_t height = 0;
    if (!window_get_pixel_size(*mod, window, width, height)) {
        window_release_for_graphics(*mod, window);
        return MOD_UNAVAILABLE;
    }

    const auto device = aurora::gfx::device();
    const auto adapter = device.GetAdapter();
    const auto instance = adapter ? adapter.GetInstance() : wgpu::Instance{};
    auto surface = aurora::webgpu::create_window_surface(instance, sdlWindow, desc->label);
    if (!surface) {
        window_release_for_graphics(*mod, window);
        return MOD_UNAVAILABLE;
    }

    auto windowDesc = *desc;
    windowDesc.width = width;
    windowDesc.height = height;
    uint64_t handle = 0;
    const auto result =
        gfx_register_present_target(*mod, std::move(surface), window, windowDesc, handle);
    if (result != MOD_OK) {
        window_release_for_graphics(*mod, window);
        return result;
    }
    *outHandle = handle;
    return MOD_OK;
}

ModResult gfx_resize_present_target_impl(
    ModContext* context, GfxPresentTargetHandle handle, uint32_t width, uint32_t height) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0 || width == 0 || height == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_resize_present_target(*mod, handle, width, height);
}

ModResult gfx_unregister_present_target_impl(ModContext* context, GfxPresentTargetHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_unregister_present_target(*mod, handle);
}

ModResult gfx_push_present_impl(
    ModContext* context, GfxPresentTargetHandle handle, const void* payload, size_t payloadSize) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0 || payloadSize > GFX_INLINE_DRAW_PAYLOAD_SIZE ||
        (payloadSize > 0 && payload == nullptr))
    {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_push_present(*mod, handle, payload, payloadSize);
}

ModResult gfx_register_draw_type_impl(
    ModContext* context, const GfxDrawTypeDesc* desc, GfxDrawTypeHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(GfxDrawTypeDesc) ||
        desc->draw == nullptr || outHandle == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result =
        gfx_register_draw_type(*mod, desc->label, desc->draw, desc->user_data, handle);
    if (result != MOD_OK) {
        return result;
    }
    *outHandle = handle;
    return MOD_OK;
}

ModResult gfx_unregister_draw_type_impl(ModContext* context, GfxDrawTypeHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_unregister_draw_type(*mod, handle);
}

ModResult gfx_push_draw_impl(
    ModContext* context, GfxDrawTypeHandle handle, const void* payload, size_t payloadSize) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0 || payloadSize > GFX_INLINE_DRAW_PAYLOAD_SIZE ||
        (payloadSize > 0 && payload == nullptr))
    {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_push_draw(*mod, handle, payload, payloadSize);
}

ModResult gfx_push_stream_impl(ModContext* context, GfxStreamBuffer buffer, const void* data,
    size_t size, size_t alignment, GfxRange* outRange) {
    if (outRange != nullptr) {
        *outRange = GfxRange{0, 0};
    }
    if (mod_from_context(context) == nullptr || data == nullptr || size == 0 || outRange == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_push_stream(buffer, data, size, alignment, *outRange);
}

ModResult gfx_push_verts_impl(
    ModContext* context, const void* data, size_t size, size_t alignment, GfxRange* outRange) {
    return gfx_push_stream_impl(context, GfxStreamBuffer::Verts, data, size, alignment, outRange);
}

ModResult gfx_push_indices_impl(
    ModContext* context, const void* data, size_t size, size_t alignment, GfxRange* outRange) {
    return gfx_push_stream_impl(context, GfxStreamBuffer::Indices, data, size, alignment, outRange);
}

ModResult gfx_push_uniform_impl(
    ModContext* context, const void* data, size_t size, GfxRange* outRange) {
    return gfx_push_stream_impl(context, GfxStreamBuffer::Uniform, data, size, 0, outRange);
}

ModResult gfx_push_storage_impl(
    ModContext* context, const void* data, size_t size, GfxRange* outRange) {
    return gfx_push_stream_impl(context, GfxStreamBuffer::Storage, data, size, 0, outRange);
}

bool valid_stage(GfxStage stage) {
    return stage == GFX_STAGE_SCENE_AFTER_TERRAIN || stage == GFX_STAGE_FRAME_BEFORE_HUD ||
           stage == GFX_STAGE_FRAME_AFTER_HUD || stage == GFX_STAGE_SCENE_BEGIN ||
           stage == GFX_STAGE_SCENE_AFTER_OPAQUE;
}

ModResult gfx_register_stage_hook_impl(ModContext* context, GfxStage stage,
    const GfxStageHookDesc* desc, GfxStageHookHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(GfxStageHookDesc) ||
        desc->callback == nullptr || outHandle == nullptr || !valid_stage(stage))
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result =
        gfx_register_stage_hook(*mod, stage, desc->callback, desc->user_data, handle);
    if (result != MOD_OK) {
        return result;
    }
    *outHandle = handle;
    return MOD_OK;
}

ModResult gfx_unregister_stage_hook_impl(ModContext* context, GfxStageHookHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_unregister_stage_hook(*mod, handle);
}

ModResult gfx_resolve_pass_impl(
    ModContext* context, const GfxResolveDesc* desc, GfxResolvedTargets* outTargets) {
    if (outTargets != nullptr && outTargets->struct_size >= sizeof(GfxResolvedTargets)) {
        *outTargets = GfxResolvedTargets{.struct_size = sizeof(GfxResolvedTargets)};
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(GfxResolveDesc) ||
        outTargets == nullptr || outTargets->struct_size < sizeof(GfxResolvedTargets) ||
        (!desc->color && !desc->depth))
    {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_resolve_pass(*mod, *desc, *outTargets);
}

ModResult gfx_create_pass_impl(ModContext* context, uint32_t width, uint32_t height) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || width == 0 || height == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_create_pass(*mod, width, height);
}

ModResult gfx_register_compute_type_impl(
    ModContext* context, const GfxComputeTypeDesc* desc, GfxComputeTypeHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(GfxComputeTypeDesc) ||
        desc->callback == nullptr || outHandle == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result =
        gfx_register_compute_type(*mod, desc->label, desc->callback, desc->user_data, handle);
    if (result != MOD_OK) {
        return result;
    }
    *outHandle = handle;
    return MOD_OK;
}

ModResult gfx_unregister_compute_type_impl(ModContext* context, GfxComputeTypeHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_unregister_compute_type(*mod, handle);
}

ModResult gfx_push_compute_impl(
    ModContext* context, GfxComputeTypeHandle handle, const void* payload, size_t payloadSize) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0 || payloadSize > GFX_INLINE_DRAW_PAYLOAD_SIZE ||
        (payloadSize > 0 && payload == nullptr))
    {
        return MOD_INVALID_ARGUMENT;
    }
    return gfx_push_compute(*mod, handle, payload, payloadSize);
}

constexpr GfxService s_gfxService{
    .header = SERVICE_HEADER(GfxService, GFX_SERVICE_MAJOR, GFX_SERVICE_MINOR),
    .get_device_info = gfx_get_device_info,
    .get_proc_address = gfx_get_proc_address,
    .register_draw_type = gfx_register_draw_type_impl,
    .unregister_draw_type = gfx_unregister_draw_type_impl,
    .push_draw = gfx_push_draw_impl,
    .register_compute_type = gfx_register_compute_type_impl,
    .unregister_compute_type = gfx_unregister_compute_type_impl,
    .push_compute = gfx_push_compute_impl,
    .push_verts = gfx_push_verts_impl,
    .push_indices = gfx_push_indices_impl,
    .push_uniform = gfx_push_uniform_impl,
    .push_storage = gfx_push_storage_impl,
    .register_stage_hook = gfx_register_stage_hook_impl,
    .unregister_stage_hook = gfx_unregister_stage_hook_impl,
    .resolve_pass = gfx_resolve_pass_impl,
    .create_pass = gfx_create_pass_impl,
    .register_present_target = gfx_register_present_target_impl,
    .register_window_present_target = gfx_register_window_present_target_impl,
    .resize_present_target = gfx_resize_present_target_impl,
    .unregister_present_target = gfx_unregister_present_target_impl,
    .push_present = gfx_push_present_impl,
    .get_scene_target_layout = gfx_get_scene_target_layout,
};

}  // namespace

constinit const ServiceModule g_gfxModule{
    .id = GFX_SERVICE_ID,
    .majorVersion = GFX_SERVICE_MAJOR,
    .minorVersion = GFX_SERVICE_MINOR,
    .service = &s_gfxService,
    .modDeactivating = gfx_mod_deactivating,
    .modDetached = gfx_mod_detached,
    .frameBegin = gfx_frame_begin,
};

}  // namespace dusk::mods::svc
