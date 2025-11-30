#ifndef D_SHOP_D_SHOP_CAMERA_H
#define D_SHOP_D_SHOP_CAMERA_H

#include "f_op/f_op_actor.h"


class ShopCam_action_c {
public:
    typedef int (ShopCam_action_c::*ShopCamFunc)();

    ShopCam_action_c() {
        setCameraSpeed(0.25f, 20.0f, 0.25f, 5.0f);
        field_0xda = true;
        field_0xd0 = 0;
        field_0xd2 = 0;
        field_0xd4 = -1;
        setCamDataIdx(0);
        field_0x7c.set(0.0f, 0.0f, 0.0f);
        field_0x88.set(0.0f, 0.0f, 100.0f);
        field_0x94 = 60.0f;
        field_0xb0.set(0.0f, 0.0f, 0.0f);
        field_0xd8 = 0;
        field_0x18 = NULL;
        field_0x1c = field_0x20 = field_0x24 = NULL;
        field_0x28 = field_0x2c = field_0x30 = NULL;
        field_0x34 = NULL;
        setCamAction(NULL);
    }

    int shop_cam_action_init();
    int shop_cam_action();
    void Save();
    void EventRecoverNotime();
    void Reset();
    void move();
    void setCamDataIdx(fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*, cXyz*,
                                      cXyz*);
    void setCamDataIdx2(fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*,
                                       fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*, cXyz*, cXyz*);
    void _debugSetCamera();
    void setMasterCamCtrPos(cXyz*);

    void setCamAction(ShopCamFunc func) {
        mCamAction = func;
    }

    void setCameraSpeed(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
        field_0xc0 = param_0;
        field_0xc4 = param_1;
        field_0xc8 = param_2;
        field_0xcc = param_3;
    }

    void SetSelectIdx(int param_0) {
        field_0xd4 = param_0;
        setCameraSpeed(0.25f, 20.0f, 0.25f, 5.0f);
    }

    void setCamDataIdx(int data_idx) { mCamDataIdx = data_idx; }

    bool isCameraStopFlag() { return field_0xda; }

    /* 0x00 */ ShopCamFunc mCamAction;
    /* 0x0c */ u8 field_0xc[0x18 - 0xc];
    /* 0x18 */ fopAc_ac_c* field_0x18;
    /* 0x1c */ fopAc_ac_c* field_0x1c;
    /* 0x20 */ fopAc_ac_c* field_0x20;
    /* 0x24 */ fopAc_ac_c* field_0x24;
    /* 0x28 */ fopAc_ac_c* field_0x28;
    /* 0x2c */ fopAc_ac_c* field_0x2c;
    /* 0x30 */ fopAc_ac_c* field_0x30;
    /* 0x34 */ fopAc_ac_c* field_0x34;
    /* 0x38 */ cXyz field_0x38;
    /* 0x44 */ cXyz field_0x44;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ cXyz field_0x54;
    /* 0x60 */ cXyz field_0x60;
    /* 0x6c */ f32 field_0x6c;
    /* 0x70 */ cXyz field_0x70;
    /* 0x7C */ cXyz field_0x7c;
    /* 0x88 */ cXyz field_0x88;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ cXyz field_0x98;
    /* 0xA4 */ cXyz field_0xa4;
    /* 0xB0 */ cXyz field_0xb0;
    /* 0xBC */ f32 field_0xbc;
    /* 0xC0 */ f32 field_0xc0;
    /* 0xC4 */ f32 field_0xc4;
    /* 0xC8 */ f32 field_0xc8;
    /* 0xCC */ f32 field_0xcc;
    /* 0xD0 */ s16 field_0xd0;
    /* 0xD2 */ s16 field_0xd2;
    /* 0xD4 */ s16 field_0xd4;
    /* 0xD6 */ s16 mCamDataIdx;
    /* 0xD8 */ s16 field_0xd8;
    /* 0xDA */ bool field_0xda;
};

#endif /* D_SHOP_D_SHOP_CAMERA_H */
