#ifndef D_A_OBJ_FIREWOOD2_H
#define D_A_OBJ_FIREWOOD2_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daFireWood2_c
 * @brief Firewood 2 (Flame)
 *
 * @details
 *
 */
class daFireWood2_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    void lightInit();
    void setLight();
    void cutLight();
    void pointLightProc();
    int Execute();
    int Draw();
    int Delete();

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x568 */ u8 field_0x568[0x574 - 0x568];
    /* 0x574 */ dCcD_Stts mCcStts;
    /* 0x5B0 */ dCcD_Cyl mCcCyl;
    /* 0x6EC */ cXyz mFlamePos;
    /* 0x6F8 */ u8 mIsLit;
    /* 0x6F9 */ u8 mIsSwitch;
    /* 0x6FA */ u8 mSwitchType;
    /* 0x6FB */ u8 mFlameType;
    /* 0x6FC */ u32 mFlameEmtrIDs[4];
    /* 0x70C */ cXyz mLightPos;
    /* 0x718 */ u8 mIsPLight;
    /* 0x71C */ f32 mBossLightStrength;
    /* 0x720 */ LIGHT_INFLUENCE mLightInfluence;
};

STATIC_ASSERT(sizeof(daFireWood2_c) == 0x740);

class daFireWood2_HIO_c : public mDoHIO_entry_c {
public:
    daFireWood2_HIO_c();
    virtual ~daFireWood2_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 mFlameOffsetY;
    /* 0x08 */ f32 mCollisionDiameter;
    /* 0x0C */ f32 mCollisionHeight;
};


#endif /* D_A_OBJ_FIREWOOD2_H */
