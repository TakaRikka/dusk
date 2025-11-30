#ifndef D_A_OBJ_ZRAMARK_H
#define D_A_OBJ_ZRAMARK_H

#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

class daNpc_Hoz_c;

class _ZraMark_Hahen_c {
public:
    ~_ZraMark_Hahen_c() {}

    _ZraMark_Hahen_c() {
        mpModel = NULL;
        setColor(0);
    }

    int create(cXyz const&, s8, s8, s8, s8);
    BOOL loadModel();
    void init(cXyz const&, s8, s8, s8);
    void calcSpeed();
    void setMtx();
    void draw();

    inline void setColor(const s8& i_color) { mColor = i_color; }
    s8 getColor() { return mColor; }
    void setPos(const cXyz& i_pos) { mPos = i_pos; }
    void setSpeed(const cXyz& i_speed) { mSpeed = i_speed; }

    void calcAngle() {
        mAngle += field_0x3aa;
    }

    void posMove() {
        mPos += mSpeed;
    }

    void fall() {
        calcAngle();
        calcSpeed();
        posMove();
    }

    /* 0x000 */ J3DModel* mpModel;
    /* 0x004 */ dKy_tevstr_c mTevstr;
    /* 0x38C */ cXyz mPos;
    /* 0x398 */ cXyz mSpeed;
    /* 0x3A4 */ csXyz mAngle;
    /* 0x3AA */ csXyz field_0x3aa;
    /* 0x3B0 */ cXyz field_0x3b0;
    /* 0x3BC */ s16 field_0x3bc;
    /* 0x3C0 */ f32 field_0x3c0;
    /* 0x3C4 */ u8 field_0x3C4[0x3C8 - 0x3C4];
    /* 0x3C8 */ s8 mColor;
    /* 0x3CC */ Z2SoundObjSimple mSound;
};

class _ZraMark_Hahen_Mng_c {
public:
    _ZraMark_Hahen_Mng_c() : field_0x2(0) {}

    int create(cXyz const&, s8, s8, s8, s8);
    void calc();
    void draw();

    s16 getTimer() { return mTimer; }
    void setTimer(s16 i_timer) { mTimer = i_timer; }

    void decTimer() {
        if (mTimer > 0) {
            mTimer--;
        }
    }

    /* 0x0 */ s16 mTimer;
    /* 0x2 */ u8 field_0x2;
    /* 0x4 */ _ZraMark_Hahen_c mHahen[14];
};

/**
 * @ingroup actors-objects
 * @class daObjZraMark_c
 * @brief Iza's River Ride Destructible Buoy
 *
 * @details
 *
 */
class daObjZraMark_c : public fopAc_ac_c {
public:
    enum TYPE_e {
        TYPE_BRANCH_GREEN,
        TYPE_GREEN,
        TYPE_BRANCH_RED,
        TYPE_RED,
    };

    int Create();
    int CreateHeap();
    int Execute();
    int Draw();
    int Delete();
    int getType();
    void setBaseMtx();
    void col_init();
    void col_set();
    daNpc_Hoz_c* schHoz();
    bool entryPointer(fopAc_ac_c*);
    void informBlast();

    u8 getMarkNo() { return fopAcM_GetParam(this) & 0xff; }
    void initBlastFlag() { mBlastFlag = false; }
    void onBlastFlag() { mBlastFlag = true; } 
    bool is_blasted() { return mBlastFlag == true; }
    bool is_branch_pat() { return mType == 0 || mType == 2; }

    int get_MarkType() {
        u8 type = fopAcM_GetParam(this) >> 0x10;
        if (type == 0xFF) {
            type = 0;
        }

        return type;
    }

    void _Disp_Vase() {
        _Material_On(0);
        _Material_On(2);
    }

    void _Vanish_Vase() {
        _Material_Off(0);
        _Material_Off(2);
    }

    void _Material_On(u16 i_jointNo) {
        mpModel->getModelData()->getMaterialNodePointer(i_jointNo)->getShape()->show();
    }

    void _Material_Off(u16 i_jointNo) {
        mpModel->getModelData()->getMaterialNodePointer(i_jointNo)->getShape()->hide();
    }

    BOOL checkHitAt(cCcD_Obj* i_hitobj) {
        if (is_blasted()) {
            return false;
        }

        if (get_MarkType() != 0) {
            return i_hitobj->ChkAtType(AT_TYPE_IRON_BALL | AT_TYPE_BOMB);
        } else {
            return i_hitobj->ChkAtType(AT_TYPE_BOMB);
        }
    }

    /* 0x0568 */ request_of_phase_process_class mPhase[1];
    /* 0x0570 */ J3DModel* mpModel;
    /* 0x0574 */ Mtx field_0x574;
    /* 0x05A4 */ dCcD_Stts mCcStts;
    /* 0x05E0 */ dCcD_Cyl mCcCyl;
    /* 0x071C */ daNpcF_ActorMngr_c mActorMngr[2];
    /* 0x072C */ u8 field_0x72c;
    /* 0x072D */ u8 mType;
    /* 0x072E */ u8 mBlastFlag;
    /* 0x0730 */ daNpc_Hoz_c* mpHoz;
    /* 0x0734 */ Z2SoundObjSimple mSound;
    /* 0x0754 */ _ZraMark_Hahen_Mng_c mHahenMngr;
};

STATIC_ASSERT(sizeof(daObjZraMark_c) == 0x3e40);

class daObjZraMark_Hio_Param_c {
public:
    /* 0x00 */ f32 mMarkSize;
    /* 0x04 */ f32 mInteriorMarkSize;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 mMarkCollisionRadius;
    /* 0x10 */ f32 mMarkCollisionHeight;
};

#endif /* D_A_OBJ_ZRAMARK_H */
