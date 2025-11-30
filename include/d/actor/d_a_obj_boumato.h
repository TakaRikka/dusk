#ifndef D_A_OBJ_BOUMATO_H
#define D_A_OBJ_BOUMATO_H

#include "d/d_bg_s_acch.h"
#include "d/actor/d_a_arrow.h"
#include "d/d_jnt_col.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObj_BouMato_c
 * @brief Stick Target
 *
 * @details
 *
 */
class daObj_BouMato_c : public fopAc_ac_c {
private:
    /* 0x568 */ int field_0x568;
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mModel;
    /* 0x578 */ dBgS_ObjAcch mAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Cyl mCyl;
    /* 0x944 */ cM3dGCyl mCyl2;
    /* 0x95C */ cM3dGLin mGLin;
    /* 0x978 */ dBgS_LinChk mLinChk;
    /* 0x9E8 */ dJntCol_c mJntCol;
    /* 0x9F8 */ csXyz field_0x9f8[3];
    /* 0xA0A */ u16 field_0xa0a;
    /* 0xA0C */ int mCutType;
    /* 0xA10 */ int field_0xa10;
    /* 0xA14 */ f32 mGroundH;
    /* 0xA18 */ f32 field_0xa18[3];
    /* 0xA24 */ s16 field_0xa24[3];
    /* 0xA2A */ s16 field_0xa2a;
    /* 0xA2C */ int mShadowId;
    /* 0xA30 */ s16 field_0xa30;
    /* 0xA32 */ u8 field_0xa32;
    /* 0xA34 */ fpc_ProcID mTargetId;
    /* 0xA38 */ u8 field_0xa38;
    /* 0xA39 */ bool mIsCurTurnRight;

public:
    virtual ~daObj_BouMato_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static void tgHitCallBack(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*);
    static void* srchArrow(void*, void*);
    void deleteStuckArrow();
    char* getResName();
    void setSwayParam(fopAc_ac_c*);
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();

    int checkCrs(fopAc_ac_c* param_0, cXyz param_1, cXyz param_2, f32 param_3) {
        daArrow_c* arrow_p = (daArrow_c*)param_0;
        cXyz sp2C;
        cXyz sp20;

        JUT_ASSERT(164, NULL != arrow_p);

        if (field_0xa38 != 0) {
            return 0;
        }

        mCyl2.cM3dGCyl::Set(current.pos, mCyl.GetShapeP()->GetR() + 120.0f, mCyl.GetShapeP()->GetH() + 30.0f);
        if ((param_1 - current.pos).abs() < param_3) {
            mGLin.SetStartEnd(param_1, param_2);
            if (cM3d_Cross_CylLin(&mCyl2, &mGLin, &sp2C, &sp20)) {
                return 2;
            }
        }

        return 0;
    }

    u8 getType() { return 0; }
    u8 getOffSwBit() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }
    u8 getOnSwBit() { return (fopAcM_GetParam(this) & 0xff0000) >> 16; }
    void setCutType(u8 cutType) { mCutType = cutType; }
    fpc_ProcID getTgHitAcId() { return mTargetId; }
    void clrTgHitAcId() { mTargetId = fpcM_ERROR_PROCESS_ID_e; }
};

STATIC_ASSERT(sizeof(daObj_BouMato_c) == 0xa40);

class daObj_BouMato_Param_c {
public:
    virtual ~daObj_BouMato_Param_c() {}

    static f32 const m[7];
};


#endif /* D_A_OBJ_BOUMATO_H */
