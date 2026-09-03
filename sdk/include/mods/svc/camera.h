#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define CAMERA_SERVICE_ID "dev.twilitrealm.dusklight.camera"
#define CAMERA_SERVICE_MAJOR 1u
#define CAMERA_SERVICE_MINOR 1u

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

/* 0 is never a valid handle. */
typedef uint64_t CameraOperatorHandle;

typedef struct CameraOperatorState {
    uint32_t struct_size;

    /* Host inputs. */
    uint64_t frame_counter;
    uint64_t ticks;
    float aspect;

    /* Initial camera state and callback output. */
    float eye[3];
    float center[3];
    float fovy;
    float bank_degrees;
} CameraOperatorState;

/* Return true to use state for the current frame. Game thread only. */
typedef bool (*CameraOperateFn)(ModContext* ctx, CameraOperatorState* state, void* user_data);

typedef struct CameraOperatorDesc {
    uint32_t struct_size;
    const char* debug_name;
    int32_t priority;
    CameraOperateFn operate;
    void* user_data;
} CameraOperatorDesc;

#define CAMERA_OPERATOR_DESC_INIT {sizeof(CameraOperatorDesc), NULL, 0, NULL, NULL}

typedef struct CameraService {
    ServiceHeader header;

    /*
     * Snapshots a camera. game_view must be a view_class pointer, such as from a render stage
     * callback's game view. Game thread only. Returns MOD_UNAVAILABLE when the view is not a valid
     * perspective camera.
     */
    ModResult (*get_camera)(ModContext* ctx, const void* game_view, CameraInfo* out_info);

    /* Minor version 1 */

    /*
     * Register an operator for the main camera. Operators run by descending priority, then
     * registration order, until one returns true. debug_name and operate must be set; debug_name
     * is copied. out_handle must not be NULL.
     */
    ModResult (*register_camera_operator)(
        ModContext* ctx, const CameraOperatorDesc* desc, CameraOperatorHandle* out_handle);
    ModResult (*unregister_camera_operator)(ModContext* ctx, CameraOperatorHandle handle);
} CameraService;

MOD_DECLARE_SERVICE(
    CameraService, svc_camera, CAMERA_SERVICE_ID, CAMERA_SERVICE_MAJOR, CAMERA_SERVICE_MINOR);
