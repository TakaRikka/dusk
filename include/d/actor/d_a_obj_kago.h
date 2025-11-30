#ifndef D_A_OBJ_KAGO_H
#define D_A_OBJ_KAGO_H

#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObj_Kago_c
 * @brief Basket
 *
 * @details
 *
 */
class daObj_Kago_c : public fopAc_ac_c {
public:
    cPhs__Step create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    BOOL isDelete();
    void setEnvTevColor();
    void setRoomNo();
    void reset();
    void setMtx();
    int getWallAngle(s16, s16*);
    void setGoalPosAndAngle();
    void setSmokePrtcl();
    void setWaterPrtcl();
    void setHamonPrtcl();

    u8 getType() {
        int iVar1 = fopAcM_GetParam(this) & 0xFF;
        u8 rv;

        switch ((argument & 127)) {
            case 0:
                switch (iVar1) {
                    case 0:
                        return 0;
                }
                break;

            case 1:
                switch (iVar1) {
                    case 0:
                        return 1;

                    case 1:
                        return 2;

                    case 2:
                        return 3;
                }
        }

        return 4;
    }

    void setMtx(Mtx param_1) {
        cXyz sp18;
        field_0xba2 = 1;
        mDoMtx_stack_c::copy(param_1);
        mDoMtx_stack_c::multVecZero(&sp18);
        current.pos = sp18;
        old.pos = current.pos;
        field_0x574->setBaseTRMtx(param_1);
    }

    void popup(f32 param_1, f32 param_2, cXyz* param_3) {
        if (param_3 != NULL) {
            current.pos = *param_3;
            old.pos = current.pos;
        }

        int iVar1 = cM_deg2s(param_2);
        speed.setall(0.0f);
        speed.y = param_1 * cM_ssin(iVar1);
        speedF = param_1 * cM_scos(iVar1);
        field_0xb44 = 6;
        field_0xb70 = 0x4000;
        field_0xb9e = 1;
        field_0xb68 = 0.0f;
    }
    
private:
    /* 0x568 */ u8 field_0x568[0x56c - 0x568];
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* field_0x574;
    /* 0x578 */ dBgS_ObjAcch mObjAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk field_0x7cc;
    /* 0x808 */ dCcD_Cyl field_0x808[2];
    /* 0xA80 */ cM3dGLin field_0xa80;
    /* 0xA9C */ dBgS_LinChk field_0xa9c;
    /* 0xB0C */ f32 field_0xb0c;
    /* 0xB10 */ u8 mType;
    /* 0xB14 */ cXyz field_0xb14;
    /* 0xB20 */ cXyz field_0xb20;
    /* 0xB2C */ csXyz field_0xb2c;
    /* 0xB32 */ u8 field_0xb32[0xb44 - 0xb32];
    /* 0xB44 */ int field_0xb44;
    /* 0xB48 */ int field_0xb48;
    /* 0xB4C */ int field_0xb4c;
    /* 0xB50 */ f32 field_0xb50;
    /* 0xB54 */ f32 mGroundH;
    /* 0xB58 */ f32 mWaterY;
    /* 0xB5C */ u8 field_0xb5c[0xb64 - 0xb5c];
    /* 0xB64 */ f32 field_0xb64;
    /* 0xB68 */ f32 field_0xb68;
    /* 0xB6C */ s16 field_0xb6c;
    /* 0xB6E */ s16 field_0xb6e;
    /* 0xB70 */ s16 field_0xb70;
    /* 0xB72 */ u8 field_0xb72[0xb74 - 0xb72];
    /* 0xB74 */ s16 field_0xb74;
    /* 0xB76 */ s16 field_0xb76;
    /* 0xB78 */ u32 field_0xb78;
    /* 0xB7C */ u32 mWaterPrtcls[4];
    /* 0xB8C */ u32 mHamonPrtcl;
    /* 0xB90 */ u8 field_0xb90[0xb94 - 0xb90];
    /* 0xB94 */ u32 field_0xb94;
    /* 0xB98 */ u32 field_0xb98;
    /* 0xB9C */ u8 field_0xb9c;
    /* 0xB9D */ u8 field_0xb9d;
    /* 0xB9E */ u8 field_0xb9e;
    /* 0xB9F */ u8 field_0xb9f;
    /* 0xBA0 */ u8 field_0xba0;
    /* 0xBA1 */ u8 field_0xba1;
    /* 0xBA2 */ u8 field_0xba2;
    /* 0xBA3 */ u8 field_0xba3;
    // vtable
    virtual ~daObj_Kago_c();
};

STATIC_ASSERT(sizeof(daObj_Kago_c) == 0xba8);

class daObj_Kago_Param_c {
public:
    virtual ~daObj_Kago_Param_c() {}

    struct Data {
        /* 0x00 */ f32 field_0x00;
        /* 0x04 */ f32 mGravity;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0c;
        /* 0x10 */ f32 mWeight;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ f32 mWallH;
        /* 0x1C */ f32 mWallR;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
    };
    static const Data m;
};


#endif /* D_A_OBJ_KAGO_H */
