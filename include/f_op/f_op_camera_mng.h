#ifndef F_F_OP_CAMERA_MNG_H_
#define F_F_OP_CAMERA_MNG_H_

#include "f_op/f_op_view.h"
#include "d/d_camera.h"

typedef struct leafdraw_method_class leafdraw_method_class;

struct fopCamM_prm_class {
    /* 0x00 */ fopAcM_prmBase_class base;
};  // Size: 0x18

class camera_process_class : public view_class {
public:
    /* 0x210 */ create_tag_class create_tag;
    /* 0x224 */ leafdraw_method_class* submethod;
    /* 0x228 */ u8 field_0x228[4];
    /* 0x22C */ s8 prm1;
    /* 0x22D */ s8 prm2;
    /* 0x22E */ s8 prm3;
    /* 0x22F */ s8 field_0x22f;
    /* 0x230 */ csXyz angle;
    /* 0x238 */ int field_0x238;
};

class camera_class : public camera_process_class {
public:
    /* 0x23C */ int field_0x23c;
    /* 0x240 */ request_of_phase_process_class phase_request;
    /* 0x248 */ dCamera_c mCamera;
};

inline void fopCamM_SetAngleX(camera_class* i_camera, s16 angle) {
    i_camera->angle.x = angle;
}

inline void fopCamM_SetAngleY(camera_class* i_camera, s16 angle) {
    i_camera->angle.y = angle;
}

inline void fopCamM_SetNear(camera_class* i_this, f32 near) {
    i_this->near = near;
}

inline void fopCamM_SetFar(camera_class* i_this, f32 far) {
    i_this->far = far;
}

inline void fopCamM_SetFovy(camera_class* i_this, f32 fovy) {
    i_this->fovy = fovy;
}

inline void fopCamM_SetAspect(camera_class* i_this, f32 aspect) {
    i_this->aspect = aspect;
}

inline void fopCamM_SetEye(camera_class* i_this, f32 x, f32 y, f32 z) {
    i_this->lookat.eye.set(x, y, z);
}

inline void fopCamM_SetCenter(camera_class* i_this, f32 x, f32 y, f32 z) {
    i_this->lookat.center.set(x, y, z);
}

inline void fopCamM_SetUp(camera_class* i_this, f32 x, f32 y, f32 z) {
    i_this->lookat.up.set(x, y, z);
}

inline void fopCamM_SetBank(camera_class* i_this, s16 bank) {
    i_this->bank = bank;
}

inline void fopCamM_SetPrm1(camera_class* i_this, int prm1) {
    i_this->prm1 = prm1;
}

inline void fopCamM_SetPrm2(camera_class* i_this, int prm2) {
    i_this->prm2 = prm2;
}

inline void fopCamM_SetPrm3(camera_class* i_this, int prm3) {
    i_this->prm3 = prm3;
}

inline s16 fopCamM_GetAngleX(camera_class* i_camera) {
    return i_camera->angle.x;
}

inline s16 fopCamM_GetAngleY(camera_class* i_camera) {
    return i_camera->angle.y;
}

inline s16 fopCamM_GetAngleZ(camera_class* i_camera) {
    return i_camera->angle.z;
}

inline f32 fopCamM_GetFovy(camera_class* i_camera) {
    return i_camera->fovy;
}

inline cXyz* fopCamM_GetEye_p(camera_class* i_camera) {
    return &i_camera->lookat.eye;
}

inline cXyz* fopCamM_GetCenter_p(camera_class* i_camera) {
    return &i_camera->lookat.center;
}

inline cXyz* fopCamM_GetUp_p(camera_class* i_camera) {
    return &i_camera->lookat.up;
}

inline s16 fopCamM_GetBank(camera_class* i_camera) {
    return i_camera->bank;
}

fpc_ProcID fopCamM_Create(int i_cameraIdx, s16 i_procName, void* i_append);
void fopCamM_Management();
u32 fopCamM_GetParam(camera_class* i_this);
void fopCamM_Init();

#endif
