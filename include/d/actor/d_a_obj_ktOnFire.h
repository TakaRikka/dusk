#ifndef D_A_OBJ_KTONFIRE_H
#define D_A_OBJ_KTONFIRE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daKtOnFire_c
 * @brief Lantern Fire
 *
 * @details
 *
 */
class daKtOnFire_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    void lightInit();
    void setLight();
    void cutLight();
    int Execute();
    int Draw();
    int Delete();

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x568 */ dCcD_Stts mCcStts;
    /* 0x5A4 */ dCcD_Cyl mCcCyl;
    /* 0x6E0 */ u8 mWaitTime;
    /* 0x6E1 */ u8 mIsSwitch;
    /* 0x6E2 */ u8 field_0x6e2;
    /* 0x6E4 */ cXyz mTorchPos;
    /* 0x6F0 */ u32 field_0x6f0;
    /* 0x6F4 */ u32 field_0x6f4;
    /* 0x6F8 */ u32 field_0x6f8;
    /* 0x6FC */ cXyz mLightPos;
    /* 0x708 */ LIGHT_INFLUENCE mLight;
};

STATIC_ASSERT(sizeof(daKtOnFire_c) == 0x728);

#endif /* D_A_OBJ_KTONFIRE_H */
