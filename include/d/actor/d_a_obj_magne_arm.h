#ifndef D_A_OBJ_MAGNE_ARM_H
#define D_A_OBJ_MAGNE_ARM_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjMarm_c
 * @brief Magnetic Arm
 *
 * @details Magnetic Arm Cranes in Goron Mines.
 * The model is split into 6 parts labeled A through F.
 * A Parts is the unmoving crane base.
 * B Parts is the vertical gear moving the crane.
 * C Parts is the main crane body.
 * D Parts are the vertical gears at the top where the crane bends.
 * E Parts is the wheel at the end of the crane.
 * F Parts is the magnetic crane head accessible to Link.
 * The ropes connecting the crane head are identified separately.
 */

class daObjMarm_c : public dBgS_MoveBgActor {
public:
    enum MOVETYPE_e {
        MOVETYPE_A_e,
        MOVETYPE_B_e,
        MOVETYPE_C_e,
        MOVETYPE_D_e
    };

    enum MODE_e {
        MODE_WAIT_e,
        MODE_MHOLE_ON_e,
        MODE_LIFT_UP_e,
        MODE_LIFT_DOWN_e,
        MODE_ROTATE_e,
        MODE_END_e
    };

    void getBpartsOffset(cXyz*);
    void getDpartsOffset(cXyz*);
    void getEpartsOffset(cXyz*);
    void getFpartsOffset(cXyz*);
    void getRopeStartPos(cXyz*);
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    cPhs__Step phase_0();
    cPhs__Step phase_1();
    cPhs__Step phase_2();
    int create1st();
    int Execute(Mtx**);
    void action();
    void init_typeA_modeWait();
    void typeA_modeWait();
    void init_typeA_modeMholeOn();
    void typeA_modeMholeOn();
    void typeA_modeLiftUp();
    void typeA_modeLiftDown();
    void init_typeA_modeRotate();
    void typeA_modeRotate();
    void typeA_modeEnd();
    void init_typeB_modeWait();
    void typeB_modeWait();
    void init_typeB_modeMholeOn();
    void typeB_modeMholeOn();
    void init_typeB_modeLiftUp();
    void typeB_modeLiftUp();
    void typeB_modeLiftDown();
    void init_typeB_modeRotate();
    void typeB_modeRotate();
    void typeB_modeEnd();
    void init_typeC_modeWait();
    void typeC_modeWait();
    void init_typeC_modeMholeOn();
    void typeC_modeMholeOn();
    void init_typeC_modeLiftUp();
    void typeC_modeLiftUp();
    void init_typeC_modeLiftDown();
    void typeC_modeLiftDown();
    void init_typeC_modeRotate();
    void typeC_modeRotate();
    void typeC_modeEnd();
    void init_typeD_modeWait();
    void typeD_modeWait();
    void init_typeD_modeMholeOn();
    void typeD_modeMholeOn();
    void init_typeD_modeLiftUp();
    void typeD_modeLiftUp();
    void init_typeD_modeLiftDown();
    void typeD_modeLiftDown();
    void init_typeD_modeRotate();
    void typeD_modeRotate();
    void typeD_modeEnd();
    void setMagneHoleEffect();
    void endMagneHoleEffect();
    void calcHimo();
    void seStart_MOVESTART();
    void seStartLevel_MOVE();
    void seStart_STOP();
    void seStartLevel_UP();
    void seStartLevel_DOWN();
    void seStart_SWING();
    int Draw();
    void debugDraw();
    int Delete();

    u32 getMoveType() { return fopAcM_GetParamBit(this, 8, 4); }
    u32 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel[6];
    /* 0x5C0 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x5C4 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5C8 */ JPABaseEmitter* mpEmitter;
    /* 0x5CC */ dBgW* mpBgW1;
    /* 0x5D0 */ Mtx mBgMtx1;
    /* 0x600 */ Mtx mBgMtx2;
    /* 0x630 */ dBgW* mpBgW2;
    /* 0x634 */ Mtx mBgMtx3;
    /* 0x664 */ dBgS_ObjAcch mAcch;
    /* 0x83C */ dBgS_AcchCir mAcchCir;
    /* 0x87C */ dCcD_Stts unused_0x87C;         // Declaration needed to generate vtables
    /* 0x8B8 */ dCcD_Cyl unused_0x8B8;          // Declaration needed to generate vtables
    /* 0x9F4 */ s32 mRotOffsetSwing;            // Stop animation swing rotation
    /* 0x9F8 */ u8 unused_0x9F8[0x4];
    /* 0x9FC */ s16 mBPartsXRot;
    /* 0x9FE */ s16 mCPartsYRot;
    /* 0xA00 */ s16 mDPartsXRot;
    /* 0xA02 */ s16 mEPartsXRot;
    /* 0xA04 */ f32 mLiftTotal;
    /* 0xA08 */ u8 mIsYRotForward;
    /* 0xA09 */ s8 mYRotDirection;
    /* 0xA0A */ s16 mRotationAngle;
    /* 0xA0C */ s16 mLiftRotation;
    /* 0xA10 */ s32 mRotationTotal;
    /* 0xA14 */ u8 mMode;
    /* 0xA15 */ u8 mMoveType;
    /* 0xA16 */ u8 unused_0xA16;
    /* 0xA17 */ u8 mStopTimer;
    /* 0xA18 */ fpc_ProcID mID;
    /* 0xA1C */ u8 mPhaseIndex;
    /* 0xA1D */ u8 mPlayerRide;
    /* 0xA20 */ mDoExt_3DlineMat1_c* mpRope1;
    /* 0xA24 */ mDoExt_3DlineMat1_c* mpRope2; 
    /* 0xA28 */ u8 field_0xA28;                 // mpRope1 Segment Count?
    /* 0xA29 */ u8 field_0xA29;                 // mpRope2 Segment Count?
    /* 0xA2C */ f32 mRotOffsetForce;            // Swing Force applied during stop animation
    /* 0xA30 */ s16 mYRotOffset;                // Y rot Offset  on stop animation
    /* 0xA32 */ s16 mFPartsZRot;                // F Parts Z rot on stop animation
    /* 0xA34 */ u8 unused_0xA34[0x4];
    /* 0xA38 */ s16 mZRotOffset;
    /* 0xA3C */ f32 mZRotForce;                 // Swing Force applied during stop animation
    /* 0xA40 */ s16 mZRotSwing;                 // Stop animation swing rotation
    /* 0xA44 */ Vec mSeMarmPos;
    /* 0xA50 */ Vec mSeMarmLiftPos;
    /* 0xA5C */ Vec mSeMarmSwingPos;
    /* 0xA68 */ u32 mShadowKey;
};

STATIC_ASSERT(sizeof(daObjMarm_c) == 0xa6c);


#endif /* D_A_OBJ_MAGNE_ARM_H */
