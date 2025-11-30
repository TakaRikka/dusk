#ifndef D_A_E_FB_H
#define D_A_E_FB_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_FB_c
 * @brief Freezard
 *
 * @details
 *
 */
class daE_FB_c : public fopEn_enemy_c {
public:
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int JointCallBack(J3DJoint*, int);
    int draw();
    void setBck(int, u8, f32, f32);
    void setActionMode(int, int);
    void damage_check();
    bool mBgLineCheck();
    bool search_check();
    void executeWait();
    void executeAttack();
    void executeDamage();
    void executeBullet();
    void action();
    void mtx_set();
    void cc_set();
    void normal_eff_set();
    void dead_eff_set();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();
    daE_FB_c();

private:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5B8 */ mDoExt_invisibleModel mInvisibleModel;
    /* 0x5C0 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x5C4 */ Z2CreatureEnemy mCreatureSound;
    /* 0x668 */ s32 mActionMode;
    /* 0x66C */ s32 mMoveMode;
    /* 0x670 */ s32 field_0x670;
    /* 0x674 */ cXyz mUnusedVec;
    /* 0x680 */ s16 field_0x680;
    /* 0x684 */ f32 mModelSize;
    /* 0x688 */ u32 mShadowKey;
    /* 0x68C */ u16 field_0x68c;
    /* 0x68E */ u8 field_0x68e;
    /* 0x68F */ u8 field_0x68f;
    /* 0x690 */ u8 field_0x690;
    /* 0x691 */ u8 swBit0;
    /* 0x692 */ u8 swBit1;
    /* 0x694 */ s16 mRotation;
    /* 0x696 */ s16 field_0x696;
    /* 0x698 */ s16 mHeadAngle;
    /* 0x69C */ s32 field_0x69c;
    /* 0x6A0 */ u8 mType;
    /* 0x6A4 */ dBgS_AcchCir mAcchCir;
    /* 0x6E4 */ dBgS_ObjAcch mObjAcch;
    /* 0x8BC */ dCcD_Stts mStts;
    /* 0x8F8 */ dCcD_Sph mSphere;
    /* 0xA30 */ dCcD_Sph mSphere2;
    /* 0xB68 */ dCcD_Sph mAtSph;
    /* 0xCA0 */ dCcU_AtInfo mAtInfo;
    // Seems that the orig developers didn't size mKeys correctly, as ideally it should be len=8, not len=7:
    /* 0xCC4 */ u32 mKeys[7];
    /* 0xCE0 */ u8 mHIOInit;
    /* 0xCE1 */ u8 field_0xce1[0xCE8 - 0xCE1];
};

STATIC_ASSERT(sizeof(daE_FB_c) == 0xce8);

class daE_FB_HIO_c : public JORReflexible {
public:
    daE_FB_HIO_c();
    virtual ~daE_FB_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 player_detection_range;
    /* 0x10 */ s16 next_attack_waiting_time;
    /* 0x12 */ s16 maximum_rotation_width;
    /* 0x14 */ s16 minimum_turning_range;
    /* 0x16 */ s16 rotation_width_stairs;  // ""Rotation width (speed) for stairs""
    /* 0x18 */ s16 color_register_1r;
    /* 0x1A */ s16 color_register_1g;
    /* 0x1C */ s16 color_register_1b;
    /* 0x1E */ s16 color_register_1a;
    /* 0x20 */ s16 maximum_rotation_width_2;
};

#endif /* D_A_E_FB_H */
