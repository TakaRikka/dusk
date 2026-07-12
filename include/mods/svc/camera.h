#pragma once

#include "mods/api.h"

#define CAMERA_SERVICE_ID "dev.twilitrealm.dusklight.camera"
#define CAMERA_SERVICE_MAJOR 1u
#define CAMERA_SERVICE_MINOR 0u

/*
 * Snapshot of a game camera for the frame currently being recorded.
 *
 * Matrix conventions: every matrix is a column-major float[16] using the matrix * column-vector
 * convention, ready to memcpy into a WGSL mat4x4f uniform. NOTE: this is the TRANSPOSE of the
 * game's row-major Mtx/Mtx44 layout; mods that want the raw game matrices should read the
 * view_class directly instead.
 *
 * View space is right-handed with -Z forward. Projection matrices are in WebGPU clip convention
 * and follow the renderer's depth mode: reversed-Z by default (depth 1.0 at the near plane,
 * 0.0 at far).
 *
 * Unprojecting a depth-buffer texel at uv with sampled depth d:
 *   let ndc = vec3f(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0, d); // WebGPU framebuffer y is down
 *   let world4 = world_from_proj * vec4f(ndc, 1.0);
 *   let world = world4.xyz / world4.w;
 */
typedef struct CameraInfo {
    uint32_t struct_size;

    float view_from_world[16]; /* the view matrix */
    float world_from_view[16]; /* its inverse; column 3 is the camera position */
    float proj_from_view[16];  /* WebGPU-convention projection (+ Aurora reversed-Z) */
    float view_from_proj[16];  /* its inverse */
    float proj_from_world[16]; /* proj_from_view * view_from_world */
    float world_from_proj[16]; /* one-step depth-buffer -> world unproject */

    float eye[3]; /* camera position in world space */
    float fovy;   /* vertical field of view, degrees */
    float aspect;
    float near_plane;
    float far_plane;
} CameraInfo;

#define CAMERA_INFO_INIT {sizeof(CameraInfo)}

typedef struct CameraService {
    ServiceHeader header;

    /*
     * Snapshots a camera. game_view must be a view_class pointer, such as from a render stage
     * callback's game view. Game thread only. Returns MOD_UNAVAILABLE when the view is not a valid
     * perspective camera.
     */
    ModResult (*get_camera)(ModContext* ctx, const void* game_view, CameraInfo* out_info);
} CameraService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<CameraService> {
    static constexpr const char* id = CAMERA_SERVICE_ID;
    static constexpr uint16_t major_version = CAMERA_SERVICE_MAJOR;
};
#endif
