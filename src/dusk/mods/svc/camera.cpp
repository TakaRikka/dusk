#include "registry.hpp"

#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/camera.h"

#include "f_op/f_op_view.h"
#include "m_Do/m_Do_mtx.h"

#include <aurora/gfx.hpp>
#include <cstring>

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

constexpr CameraService s_cameraService{
    .header = SERVICE_HEADER(CameraService, CAMERA_SERVICE_MAJOR, CAMERA_SERVICE_MINOR),
    .get_camera = camera_get_camera_from_view,
};

}  // namespace

constinit const ServiceModule g_cameraModule{
    .id = CAMERA_SERVICE_ID,
    .majorVersion = CAMERA_SERVICE_MAJOR,
    .minorVersion = CAMERA_SERVICE_MINOR,
    .service = &s_cameraService,
};

}  // namespace dusk::mods::svc
