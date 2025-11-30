#ifndef D_A_OBJ_STONE_H
#define D_A_OBJ_STONE_H

#include "d/d_com_inf_game.h"

enum {
    OBJ_STONE_TYPE_SMALL = 0,
    OBJ_STONE_TYPE_BIG = 1
};

/**
 * @ingroup actors-objects
 * @class daObjStone_c
 * @brief Small / Large Stones
 *
 * @details
 *
 */
class daObjStone_c : public fopAc_ac_c {
public:
    typedef void (daObjStone_c::*modeFunc)();

    u8 getType() { return fopAcM_GetParamBit(this,0,4); }
    u8 getItemType() { return fopAcM_GetParamBit(this,4,2); }
    u8 getItemNo() { return fopAcM_GetParamBit(this,8,8); }
    u8 getItemBit() { return fopAcM_GetParamBit(this,16,8); }

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    bool CreateHeap();
    int create();
    int execute();
    void mode_proc_call();
    void init_modePutWait();
    void modePutWait();
    void init_modeWait();
    void modeWait();
    void init_modeDrop();
    void modeDrop();
    void init_modeCarry();
    void modeCarry();
    void init_modeWalk();
    void modeWalk();
    void init_modePushWolfNonBreak();
    void init_modeYogan();
    void modeYogan();
    void init_modeBreak();
    void modeBreak();
    void CrrPos();
    void createItem();
    void calcRotBaseMtx(s16*);
    int checkRollAngle();
    void set_carry_eff();
    void effect_delete(bool);
    int draw();
    int _delete();

private:
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ J3DModel* mpModel;
    /* 0x0574 */ dBgS_ObjAcch mChkObj;
    /* 0x074C */ dBgS_AcchCir mChkCircle;
    /* 0x078C */ dCcD_Stts mStatus;
    /* 0x07C8 */ dCcD_Sph mCollider;
    /* 0x0900 */ csXyz mShapeAngle;
    /* 0x0906 */ u8 mStoneType;                 // 0: small rock, 1: big rock
    /* 0x0907 */ u8 field_0x0907;
    /* 0x0908 */ u8 field_0x0908;
    /* 0x090C */ u32 mMode; // maybe a better name for this
    /* 0x0910 */ cXyz field_0x0910;
    /* 0x091C */ s16 field_0x091c;
    /* 0x091E */ u8 field_0x091E[0x0920 - 0x091E];
    /* 0x0920 */ Quaternion field_0x0920;
    /* 0x0930 */ Quaternion field_0x0930;
    /* 0x0940 */ f32 mInitialOffsetY;
    /* 0x0944 */ f32 mLastPosY; 
    /* 0x0948 */ f32 mLastSpeedY; // maybe a better name for this
    /* 0x094C */ u8 field_0x094c;
    /* 0x094D */ u8 field_0x094d;
    /* 0x094E */ bool mIsInWater;
    /* 0x094F */ u8 field_0x094f;
    /* 0x0950 */ u8 field_0x0950;
    /* 0x0951 */ u8 field_0x0951;
    /* 0x0954 */ u32 field_0x0954;
    /* 0x0954 */ u32 field_0x0958;
    /* 0x0954 */ u32 field_0x095c;
    /* 0x0960 */ u32 field_0x0960;
    /* 0x0964 */ JPABaseEmitter* field_0x0964[3];
    /* 0x0970 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(daObjStone_c) == 0x990);

#endif /* D_A_OBJ_STONE_H */
