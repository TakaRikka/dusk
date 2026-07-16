#include "registry.hpp"
#include "slot_map.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/gfx.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/gfx.h"

#include <aurora/gfx.hpp>
#include <fmt/format.h>

#include <cstdint>
#include <exception>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace dusk::mods {
namespace {

aurora::Module Log("dusk::mods::gfx");

enum class GfxSlotKind : uint8_t {
    DrawType,
    StageHook,
    ComputeType,
};

enum class GfxStreamBuffer : uint8_t {
    Verts,
    Indices,
    Uniform,
    Storage,
};

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
};

struct WorkerFailure {
    std::string modId;
    std::string message;
    std::vector<aurora::gfx::DrawTypeId> drawIds;
    std::vector<aurora::gfx::EncoderTaskId> taskIds;
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

void collect_mod_slots_locked(LoadedMod& owner, std::vector<aurora::gfx::DrawTypeId>& drawIds,
    std::vector<aurora::gfx::EncoderTaskId>& taskIds) {
    auto entries = s_slots.take_all(owner);
    for (auto& entry : entries) {
        const auto& slot = entry.value;
        if (slot.kind == GfxSlotKind::DrawType && slot.auroraDrawId != aurora::gfx::InvalidDrawType)
        {
            drawIds.push_back(slot.auroraDrawId);
        } else if (slot.kind == GfxSlotKind::ComputeType &&
                   slot.auroraTaskId != aurora::gfx::InvalidEncoderTask)
        {
            taskIds.push_back(slot.auroraTaskId);
        }
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

void draw_trampoline(const aurora::gfx::DrawContext& ctx, const wgpu::RenderPassEncoder& pass,
    const void* payload, size_t payloadSize, void* userdata) {
    const auto handle = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(userdata));
    GfxDrawFn fn = nullptr;
    void* userData = nullptr;
    ModContext* modContext = nullptr;
    LoadedMod* owner = nullptr;
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
        owner = entry->owner;
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
        .color_format = static_cast<WGPUTextureFormat>(ctx.colorFormat),
        .depth_format = static_cast<WGPUTextureFormat>(ctx.depthFormat),
        .sample_count = ctx.sampleCount,
        .target_width = ctx.targetWidth,
        .target_height = ctx.targetHeight,
        .uses_reversed_z = aurora::gfx::uses_reversed_z(),
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
    collect_mod_slots_locked(*owner, record.drawIds, record.taskIds);
    s_workerFailures.push_back(std::move(record));
}

void compute_trampoline(const aurora::gfx::EncoderTaskContext& ctx, const wgpu::CommandEncoder& cmd,
    const void* payload, size_t payloadSize, void* userdata) {
    const auto handle = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(userdata));
    GfxComputeFn fn = nullptr;
    void* userData = nullptr;
    ModContext* modContext = nullptr;
    LoadedMod* owner = nullptr;
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
        owner = entry->owner;
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
    collect_mod_slots_locked(*owner, record.drawIds, record.taskIds);
    s_workerFailures.push_back(std::move(record));
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

void gfx_drain_worker_failures() {
    std::vector<WorkerFailure> failures;
    {
        std::lock_guard lock{s_mutex};
        failures.swap(s_workerFailures);
    }
    if (failures.empty()) {
        return;
    }

    bool needsSynchronize = false;
    for (const auto& failure : failures) {
        unregister_aurora_types(failure.drawIds, failure.taskIds);
        needsSynchronize = needsSynchronize || !failure.drawIds.empty() || !failure.taskIds.empty();
    }
    if (needsSynchronize) {
        aurora::gfx::synchronize();
    }

    for (const auto& failure : failures) {
        for (auto& mod : ModLoader::instance().mods()) {
            if (mod.metadata.id == failure.modId && mod.active) {
                fail_mod(mod, MOD_ERROR, failure.message);
                break;
            }
        }
    }
}

void gfx_remove_mod(LoadedMod& mod) {
    std::vector<aurora::gfx::DrawTypeId> drawIds;
    std::vector<aurora::gfx::EncoderTaskId> taskIds;
    {
        std::lock_guard lock{s_mutex};
        collect_mod_slots_locked(mod, drawIds, taskIds);
    }
    if (drawIds.empty() && taskIds.empty()) {
        return;
    }
    unregister_aurora_types(drawIds, taskIds);
    aurora::gfx::synchronize();
}

}  // namespace dusk::mods

namespace dusk::mods::svc {
namespace {

ModResult gfx_get_device_info(ModContext* context, GfxDeviceInfo* outInfo) {
    if (outInfo == nullptr || outInfo->struct_size < sizeof(GfxDeviceInfo)) {
        return MOD_INVALID_ARGUMENT;
    }
    const uint32_t structSize = outInfo->struct_size;
    *outInfo = GfxDeviceInfo{.struct_size = structSize};

    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    outInfo->device = aurora::gfx::device().Get();
    outInfo->queue = aurora::gfx::queue().Get();
    outInfo->color_format = static_cast<WGPUTextureFormat>(aurora::gfx::color_format());
    outInfo->depth_format = static_cast<WGPUTextureFormat>(aurora::gfx::depth_format());
    outInfo->sample_count = aurora::gfx::sample_count();
    outInfo->uses_reversed_z = aurora::gfx::uses_reversed_z();
    return MOD_OK;
}

void* gfx_get_proc_address(ModContext* context, const char* name) {
    if (mod_from_context(context) == nullptr || name == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<void*>(wgpuGetProcAddress(WGPUStringView{name, WGPU_STRLEN}));
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
};

}  // namespace

constinit const ServiceModule g_gfxModule{
    .id = GFX_SERVICE_ID,
    .majorVersion = GFX_SERVICE_MAJOR,
    .minorVersion = GFX_SERVICE_MINOR,
    .service = &s_gfxService,
    .modDetached = gfx_remove_mod,
    .frameBegin = gfx_drain_worker_failures,
};

}  // namespace dusk::mods::svc
