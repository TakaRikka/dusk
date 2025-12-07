#ifndef D_A_OBJ_KBACKET_H
#define D_A_OBJ_KBACKET_H

#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

struct daObj_KBacket_HIOParam
{
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
};

class daObj_KBacket_HIO_c
#if DEBUG
    : public mDoHIO_entry_c
#endif
{
public:
#if DEBUG
    void genMessage(JORMContext*);

    daObj_KBacket_HIOParam param;
#endif
};

class daObj_KBacket_Param_c {
public:
    virtual ~daObj_KBacket_Param_c() {}

    static const daObj_KBacket_HIOParam m;
};

/**
 * @ingroup actors-objects
 * @class daObj_KBacket_c
 * @brief (Kakariko?) Bucket
 *
 * @details
 *
 */
class daObj_KBacket_c : public fopAc_ac_c {
public:
    /* 0x568 */ daObj_KBacket_HIO_c* mHIO;
    /* 0x56C */ request_of_phase_process_class field_0x56c;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_ObjAcch mObjAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Cyl mCyl;
    /* 0x90C */ cM3dGLin mLine;
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
    /* 0xA08 */ u8 field_0xa08[0xa0c - 0xa08];
    /* 0xA0C */ f32 field_0xa0c;
    /* 0xA10 */ f32 field_0xa10;
    /* 0xA14 */ s16 field_0xa14;
    /* 0xA18 */ s16 field_0xa16;
    /* 0xA18 */ s16 field_0xa18;
    /* 0xA1A */ s16 field_0xa1a;
    /* 0xA1A */ s16 field_0xa1c;
    /* 0xA1A */ u8 field_0xa1e[0xa24 - 0xa1e];
    /* 0xA24 */ u32 field_0xa24[4];
    /* 0xA34 */ u32 field_0xa34;
    /* 0xA38 */ u8 field_0xa38[0xa3c - 0xa38];
    /* 0xA3C */ u32 field_0xa3c;
    /* 0xA40 */ u32 field_0xa40;
    /* 0xA44 */ u8 field_0xa44;
    /* 0xA45 */ u8 field_0xa45;
    /* 0xA46 */ u8 field_0xa46;
    /* 0xA47 */ u8 field_0xa47;
    /* 0xA48 */ u8 field_0xa48;
    /* 0xA49 */ u8 field_0xa49;
    /* 0xA49 */ u8 field_0xa4a;
    /* 0xA4B */ u8 field_0xa4b;
    /* 0xA4C */ u8 field_0xa4c[0xa50 - 0xa4c];

    virtual ~daObj_KBacket_c();
    int create();
    int setLaunchParam(float param_0, float param_1, short param_2) {
        float fVar2;

        s16 iVar1 = cM_deg2s(param_1);
        speed.setall(0.0f);
        speed.y = param_0 * cM_ssin(iVar1);
        speedF = param_0 * cM_scos(iVar1);
        if (field_0x9d0 == 0x0) {
            current.angle.y = param_2 + 0x4000;
        }
        if (field_0x9d0 == 0x1) {
            current.angle.y = param_2 + -0x4000;
        }
        shape_angle.y = param_2;
        field_0xa18 = 0x800;
        field_0xa44 = 1;
        field_0xa47 = 1;
    }
    u8 getType() {
        u8 param = fopAcM_GetParam(this);
        switch (param & 0xff) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return 0;
        }
    }
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static void createHeapCallBack(fopAc_ac_c*);
    void setEnvTevColor();
    void setRoomNo();
    void reset();
    void setMtx();
    s16 calcRollAngle(s16, int);
    int getWallAngle(s16, s16*);
    void setSmokePrtcl();
    void setWaterPrtcl();
    void setHamonPrtcl();
};

STATIC_ASSERT(sizeof(daObj_KBacket_c) == 0xa54);

#endif /* D_A_OBJ_KBACKET_H */
