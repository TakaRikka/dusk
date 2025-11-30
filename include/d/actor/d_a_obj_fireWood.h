#ifndef D_A_OBJ_FIREWOOD_H
#define D_A_OBJ_FIREWOOD_H

#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_hostIO.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daFireWood_c
 * @brief Kakariko Stove Flame?
 *
 * @details
 *
 */
class daFireWood_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    void lightInit();
    void setLight();
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
    /* 0x6F8 */ u8 mIsSwitch;
    /* 0x6F9 */ u8 mIsLit;
    /* 0x6FC */ u32 mFlameEmtrIDs[4];
    /* 0x70C */ u8 mFlameType;
    /* 0x710 */ cXyz mLightPos;
    /* 0x71C */ LIGHT_INFLUENCE mLightInfluence;
    /* 0x73C */ u8 mSwWaitTime;
};

STATIC_ASSERT(sizeof(daFireWood_c) == 0x740);

class daFireWood_HIO_c : public mDoHIO_entry_c {
public:
    daFireWood_HIO_c();
    virtual ~daFireWood_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 mFlameOffsetY;
    /* 0x8 */ f32 mCollisionRadius;
    /* 0xC */ u8 mWaitTime;
};

#endif /* D_A_OBJ_FIREWOOD_H */
