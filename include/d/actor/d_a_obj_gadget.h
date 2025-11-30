#ifndef D_A_OBJ_GADGET_H
#define D_A_OBJ_GADGET_H

#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

class daObj_Gadget_HIO_c;

/**
 * @ingroup actors-objects
 * @class daObj_Gadget_c
 * @brief Gadget
 *
 * @details
 *
 */
class daObj_Gadget_c : public fopAc_ac_c {
private:
    /* 0x568 */ daObj_Gadget_HIO_c* field_0x568;
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mModel;
    /* 0x578 */ dBgS_ObjAcch mAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Cyl mCyl;
    /* 0x944 */ cM3dGLin mLine;
    /* 0x960 */ dBgS_LinChk mLinChk;
    /* 0x9D0 */ u8 field_0x9d0;
    /* 0x9D4 */ cXyz field_0x9d4;
    /* 0x9E0 */ cXyz field_0x9e0;
    /* 0x9EC */ csXyz field_0x9ec;
    /* 0x9F4 */ int field_0x9f4;
    /* 0x9F8 */ int field_0x9f8;
    /* 0x9FC */ f32 field_0x9fc;
    /* 0xA00 */ f32 field_0xa00;
    /* 0xA04 */ f32 field_0xa04;
    /* 0xA08 */ f32 field_0xa08;
    /* 0xA0C */ f32 field_0xa0c;
    /* 0xA10 */ s16 field_0xa10;
    /* 0xA12 */ s16 field_0xa12;
    /* 0xA14 */ s16 field_0xa14;
    /* 0xA16 */ s16 field_0xa16;
    /* 0xA18 */ s16 field_0xa18;
    /* 0xA1A */ u8 field_0xa1a[0xa20 - 0xa1a];
    /* 0xA20 */ u32 field_0xa20[4];
    /* 0xA30 */ u32 field_0xa30;
    /* 0xA34 */ u32 field_0xa34;
    /* 0xA38 */ u32 field_0xa38;
    /* 0xA3C */ u32 field_0xa3c;
    /* 0xA40 */ u8 field_0xa40;
    /* 0xA41 */ u8 field_0xa41;
    /* 0xA42 */ u8 field_0xa42;
    /* 0xA43 */ u8 field_0xa43;
    /* 0xA44 */ u8 mHide;
    /* 0xA45 */ u8 field_0xa45;
    /* 0xA46 */ u8 field_0xa46;
    /* 0xA47 */ u8 field_0xa47;

public:
    virtual ~daObj_Gadget_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    void setEnvTevColor();
    void setRoomNo();
    void reset();
    void setMtx();
    s16 calcRollAngle(s16, int);
    BOOL getWallAngle(s16, s16*);
    void setSmokePrtcl();
    void setWaterPrtcl();
    void setHamonPrtcl();

    u8 getType() { return fopAcM_GetParam(this); }
    void setCoNone() { field_0xa40 = 1; }
    void offHide() { mHide = 0; }
    void onHide() { mHide = 1; }

    void popup(f32 param_1, f32 param_2, cXyz* param_3) {
        if (param_3 != NULL) {
            current.pos = *param_3;
            old.pos = current.pos;
        }

        s16 iVar1 = cM_deg2s(param_2);
        speed.setall(0.0f);
        speed.y = param_1 * cM_ssin(iVar1);
        speedF = param_1 * cM_scos(iVar1);
        field_0xa14 = 0x4000;
        field_0xa43 = 1;
    }
};

STATIC_ASSERT(sizeof(daObj_Gadget_c) == 0xa4c);

struct daObj_Gadget_HIOParam {
    /* 0x00 */ f32 focus_offset;
    /* 0x04 */ f32 gravity;
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 real_shadow_size;
    /* 0x10 */ f32 weight;
    /* 0x14 */ f32 height;
    /* 0x18 */ f32 knee_length;
    /* 0x1C */ f32 width;
    /* 0x20 */ f32 fire_rate;
    /* 0x24 */ f32 launch_angle;
    /* 0x28 */ f32 floating_offset;
};

class daObj_Gadget_Param_c {
public:
    virtual ~daObj_Gadget_Param_c() {}

    static daObj_Gadget_HIOParam const m;
};


#endif /* D_A_OBJ_GADGET_H */
