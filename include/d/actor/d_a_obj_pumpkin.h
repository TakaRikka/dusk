#ifndef D_A_OBJ_PUMPKIN_H
#define D_A_OBJ_PUMPKIN_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"

struct daObj_Pumpkin_HIOParam {
    /* 0x00 */ f32 featured_offset;
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
    /* 0x2C */ s16 hold_on;
    /* 0x2E */ s16 pull_sound_delay;
    /* 0x30 */ s16 growth_time_1;
    /* 0x32 */ s16 growth_time_2;
};

class daObj_Pumpkin_Param_c {
public:
    virtual ~daObj_Pumpkin_Param_c() {}

    static const daObj_Pumpkin_HIOParam m;
};

#if DEBUG
class daObj_Pumpkin_HIO_c : public mDoHIO_entry_c {
public:
    daObj_Pumpkin_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    daObj_Pumpkin_HIOParam m;
};

#define OBJ_PUMPKIN_HIO_CLASS daObj_Pumpkin_HIO_c
#else
#define OBJ_PUMPKIN_HIO_CLASS daObj_Pumpkin_Param_c
#endif

/**
 * @ingroup actors-objects
 * @class daObj_Pumpkin_c
 * @brief Ordon Village Pumpkin
 *
 * @details
 *
 */
class daObj_Pumpkin_c : public fopAc_ac_c {
private:
    /* 0x568 */ OBJ_PUMPKIN_HIO_CLASS* mpHIO;
    /* 0x56C */ request_of_phase_process_class mPhaseReq;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dBgS_ObjAcch mObjAcch;
    /* 0x750 */ dCcD_Stts mStts;
    /* 0x78C */ dBgS_AcchCir mAcchCir;
    /* 0x7CC */ cBgS_GndChk mGndChk;
    /* 0x808 */ dCcD_Cyl mCyl0;
    /* 0x944 */ dCcD_Cyl mCyl1;
    /* 0xA80 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0xA8C */ cM3dGLin mLin;
    /* 0xAA8 */ dBgS_LinChk mLinChk;
    /* 0xB18 */ f32 mPumpkinScale;
    /* 0xB1C */ u8 mType;
    /* 0xB20 */ cXyz field_0xB20;
    /* 0xB2C */ cXyz field_0xB2C;
    /* 0xB38 */ csXyz field_0xB38;
    /* 0xB40 */ fpc_ProcID mItemProcId;
    /* 0xB44 */ int field_0xB44;
    /* 0xB48 */ int field_0xB48;
    /* 0xB4C */ int field_0xB4C;
    /* 0xB50 */ int field_0xB50;
    /* 0xB54 */ int field_0xB54;
    /* 0xB58 */ int field_0xB58;
    /* 0xB5C */ f32 field_0xB5C;
    /* 0xB60 */ f32 field_0xB60;
    /* 0xB64 */ f32 mWaterY;
    /* 0xB68 */ f32 field_0xB68;
    /* 0xB6C */ u8 field_0xB6C[0xB70 - 0xB6C];
    /* 0xB70 */ f32 field_0xB70;
    /* 0xB74 */ f32 field_0xB74;
    /* 0xB78 */ s16 field_0xB78;
    /* 0xB7A */ s16 field_0xB7A;
    /* 0xB7C */ s16 field_0xB7C;
    /* 0xB7E */ s16 field_0xB7E;
    /* 0xB80 */ s16 field_0xB80;
    /* 0xB82 */ u8 field_0xB82[0xB88 - 0xB82];
    /* 0xB88 */ u32 mWaterPrtcls[4];
    /* 0xB98 */ u32 mHamonPrtcl;
    /* 0xB9C */ u8 field_0xB9C[0xBA0 - 0xB9C];
    /* 0xBA0 */ u32 mSmokePrtcl1;
    /* 0xBA4 */ u32 mSmokePrtcl2;
    /* 0xBA8 */ u8 field_0xBA8;
    /* 0xBA9 */ u8 field_0xBA9;
    /* 0xBAA */ u8 field_0xBAA;
    /* 0xBAB */ u8 field_0xBAB;
    /* 0xBAC */ u8 field_0xBAC;
    /* 0xBAD */ u8 field_0xBAD;
    /* 0xBAE */ u8 field_0xBAE;
    /* 0xBAF */ u8 field_0xBAF;
    /* 0xBB0 */ u8 field_0xBB0;
    /* 0xBB1 */ u8 field_0xBB1;
    /* 0xBB2 */ u8 field_0xBB2;
    /* 0xBB3 */ u8 field_0xbb3;
    /* 0xBB4 */ u8 field_0xbb4;

public:
    virtual ~daObj_Pumpkin_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    int isDelete();
    void setEnvTevColor();
    void setRoomNo();
    void reset();
    void setMtx();
    s16 calcRollAngle(s16, int);
    int getWallAngle(s16, s16*);
    void setSmokePrtcl();
    void setWaterPrtcl();
    void setHamonPrtcl();
    void crash();

    void setMtx(MtxP mtx) {
        cXyz newPos;
        field_0xbb3 = 1;
        mDoMtx_stack_c::copy(mtx);
        mDoMtx_stack_c::multVecZero(&newPos);
        current.pos = newPos;
        old.pos = current.pos;
        mpModel->setBaseTRMtx(mtx);
    }

    int getItemTableNo() {
        u8 prm = fopAcM_GetParam(this) & 0xFF;
        if (prm == 0xFF) {
            return -1;
        }

        return prm;
    }

    u8 getType() {
        u8 prm = (fopAcM_GetParam(this) & 0xFF00) >> 8; 
        switch (prm) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        default:
            return 0;
        }
    }

    u8 getProcType() {
        u8 prm = (fopAcM_GetParam(this) & 0xF0000000) >> 0x1C;
        if (prm == 0xF) {
            return 0;
        }

        return prm;
    }

    void popup(f32 i_f0, f32 i_deg, cXyz* ip_vec) {
        if (ip_vec != NULL) {
            current.pos = *ip_vec;
            old.pos = current.pos;
        }

        s16 temp_r30 = cM_deg2s(i_deg);
        speed.setall(0.0f);
        speed.y = i_f0 * cM_ssin(temp_r30);
        speedF = i_f0 * cM_scos(temp_r30);
        field_0xB50 = 6;
        field_0xB7C = 0x4000;
        field_0xBAD = 1;
        field_0xB74 = 0.0f;
    }
};

STATIC_ASSERT(sizeof(daObj_Pumpkin_c) == 0xbbc);


#endif /* D_A_OBJ_PUMPKIN_H */
