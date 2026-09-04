#include "internal.hpp"
#include "registry.hpp"

#include "dusk/camera_operators.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/camera.h"

#include "SSystem/SComponent/c_angle.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_view.h"
#include "m_Do/m_Do_mtx.h"

#include <algorithm>
#include <aurora/gfx.hpp>
#include <cstring>
#include <exception>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace dusk::mods::svc {
namespace {

void to_column_major(const Mtx44 in, float out[16]) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            out[c * 4 + r] = in[r][c];
        }
    }
}

void store_affine(const Mtx in, float out[16]) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 3; ++r) {
            out[c * 4 + r] = in[r][c];
        }
        out[c * 4 + 3] = 0.0f;
    }
    out[15] = 1.0f;
}

/* affine * 4x4; operand-order mirror of cMtx_concatProjView */
void concat_affine_proj(const Mtx a, const Mtx44 b, Mtx44 out) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 4; ++c) {
            out[r][c] =
                a[r][0] * b[0][c] + a[r][1] * b[1][c] + a[r][2] * b[2][c] + a[r][3] * b[3][c];
        }
    }
    std::memcpy(out[3], b[3], sizeof(f32) * 4);
}

ModResult snapshot_view(const view_class* view, CameraInfo* outInfo) {
    if (view == nullptr || !(view->near_ > 0.0f) || !(view->far_ > view->near_) ||
        !(view->fovy > 0.0f) || view->fovy >= 180.0f || !(view->aspect > 0.0f))
    {
        return MOD_UNAVAILABLE;
    }

    // Build from the GXSetProjection values
    const f32 p00 = view->projMtx[0][0];
    const f32 p02 = view->projMtx[0][2];
    const f32 p11 = view->projMtx[1][1];
    const f32 p12 = view->projMtx[1][2];
    const f32 p22 = view->projMtx[2][2];
    const f32 p23 = view->projMtx[2][3];
    if (view->projMtx[3][2] != -1.0f || p00 == 0.0f || p11 == 0.0f || p23 == 0.0f) {
        return MOD_UNAVAILABLE;
    }

    // WebGPU-convention projection + Aurora reversed Z
    const bool reversedZ = aurora::gfx::uses_reversed_z();
    const f32 e = reversedZ ? -p22 : p22 - 1.0f;
    const f32 f = reversedZ ? -p23 : p23;
    Mtx44 proj{};
    proj[0][0] = p00;
    proj[0][2] = p02;
    proj[1][1] = p11;
    proj[1][2] = p12;
    proj[2][2] = e;
    proj[2][3] = f;
    proj[3][2] = -1.0f;

    // Analytic inverse of the sparse perspective form
    Mtx44 invProj{};
    invProj[0][0] = 1.0f / p00;
    invProj[0][3] = p02 / p00;
    invProj[1][1] = 1.0f / p11;
    invProj[1][3] = p12 / p11;
    invProj[2][3] = -1.0f;
    invProj[3][2] = 1.0f / f;
    invProj[3][3] = e / f;

    Mtx44 projWorld;
    cMtx_concatProjView(proj, view->viewMtx, projWorld);
    Mtx44 worldProj;
    concat_affine_proj(view->invViewMtx, invProj, worldProj);

    store_affine(view->viewMtx, outInfo->view_from_world);
    store_affine(view->invViewMtx, outInfo->world_from_view);
    to_column_major(proj, outInfo->proj_from_view);
    to_column_major(invProj, outInfo->view_from_proj);
    to_column_major(projWorld, outInfo->proj_from_world);
    to_column_major(worldProj, outInfo->world_from_proj);

    outInfo->eye[0] = view->lookat.eye.x;
    outInfo->eye[1] = view->lookat.eye.y;
    outInfo->eye[2] = view->lookat.eye.z;
    outInfo->fovy = view->fovy;
    outInfo->aspect = view->aspect;
    outInfo->near_plane = view->near_;
    outInfo->far_plane = view->far_;
    return MOD_OK;
}

ModResult camera_get_camera_from_view(
    ModContext* context, const void* gameView, CameraInfo* outInfo) {
    if (outInfo == nullptr || outInfo->struct_size < sizeof(CameraInfo) ||
        mod_from_context(context) == nullptr || gameView == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    const uint32_t structSize = outInfo->struct_size;
    std::memset(outInfo, 0, sizeof(CameraInfo));
    outInfo->struct_size = structSize;
    return snapshot_view(static_cast<const view_class*>(gameView), outInfo);
}

struct CameraOperator {
    std::string debugName;
    int32_t priority = 0;
    uint64_t sequence = 0;
    CameraOperateFn operate = nullptr;
    void* userData = nullptr;
};

SlotMap<CameraOperator> sOperators;
uint64_t sNextOperatorSequence = 0;

ModResult camera_register_operator(
    ModContext* context, const CameraOperatorDesc* desc, CameraOperatorHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }

    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(CameraOperatorDesc) ||
        desc->debug_name == nullptr || desc->debug_name[0] == '\0' || desc->operate == nullptr ||
        outHandle == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }

    *outHandle = sOperators.emplace(*mod, CameraOperator{
                                              .debugName = desc->debug_name,
                                              .priority = desc->priority,
                                              .sequence = sNextOperatorSequence++,
                                              .operate = desc->operate,
                                              .userData = desc->user_data,
                                          });
    return MOD_OK;
}

ModResult camera_unregister_operator(ModContext* context, CameraOperatorHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return sOperators.erase_owned(handle, *mod) ? MOD_OK : MOD_INVALID_ARGUMENT;
}

CameraOperatorState make_operator_state(const dCamera_c& camera) {
    return CameraOperatorState{
        .struct_size = sizeof(CameraOperatorState),
        .frame_counter = camera.mFrameCounter,
        .ticks = camera.mTicks,
        .aspect = mDoGph_gInf_c::getAspect(),
        .eye = {camera.mEye.x, camera.mEye.y, camera.mEye.z},
        .center = {camera.mCenter.x, camera.mCenter.y, camera.mCenter.z},
        .fovy = camera.mFovy,
        .bank_degrees = camera.mBank.Degree(),
    };
}

bool run_operators(dCamera_c* camera) {
    auto* mainCamera = dComIfGp_getCamera(0);
    if (camera == nullptr || mainCamera == nullptr || &mainCamera->mCamera != camera) {
        return false;
    }

    struct PendingOperator {
        CameraOperatorHandle handle;
        CameraOperator value;
        LoadedMod* owner;
    };
    std::vector<PendingOperator> operators;
    sOperators.for_each([&](CameraOperatorHandle handle, const auto& entry) {
        operators.push_back({.handle = handle, .value = entry.value, .owner = entry.owner});
    });
    std::ranges::sort(operators, [](const PendingOperator& lhs, const PendingOperator& rhs) {
        if (lhs.value.priority != rhs.value.priority) {
            return lhs.value.priority > rhs.value.priority;
        }
        return lhs.value.sequence < rhs.value.sequence;
    });

    const auto initialState = make_operator_state(*camera);
    for (const auto& entry : operators) {
        if (!entry.owner->active || sOperators.find(entry.handle) == nullptr) {
            continue;
        }

        auto state = initialState;
        bool useState = false;
        try {
            useState =
                entry.value.operate(entry.owner->context.get(), &state, entry.value.userData);
        } catch (const std::exception& e) {
            fail_mod(*entry.owner, MOD_ERROR,
                fmt::format(
                    "exception in camera operator '{}': {}", entry.value.debugName, e.what()));
        } catch (...) {
            fail_mod(*entry.owner, MOD_ERROR,
                fmt::format("unknown exception in camera operator '{}'", entry.value.debugName));
        }

        if (!useState) {
            continue;
        }

        camera->Reset(cXyz{state.center[0], state.center[1], state.center[2]},
            cXyz{state.eye[0], state.eye[1], state.eye[2]}, state.fovy,
            cAngle::Degree_to_SAngle(state.bank_degrees));
        return true;
    }
    return false;
}

void camera_mod_detached(LoadedMod& mod) {
    sOperators.erase_all(mod);
}

constexpr CameraService s_cameraService{
    .header = SERVICE_HEADER(CameraService, CAMERA_SERVICE_MAJOR, CAMERA_SERVICE_MINOR),
    .get_camera = camera_get_camera_from_view,
    .register_camera_operator = camera_register_operator,
    .unregister_camera_operator = camera_unregister_operator,
};

}  // namespace

constinit const ServiceModule g_cameraModule{
    .id = CAMERA_SERVICE_ID,
    .majorVersion = CAMERA_SERVICE_MAJOR,
    .minorVersion = CAMERA_SERVICE_MINOR,
    .service = &s_cameraService,
    .modDetached = camera_mod_detached,
};

}  // namespace dusk::mods::svc

bool dusk::mods::camera_run_operators(dCamera_c* camera) {
    return svc::run_operators(camera);
}
