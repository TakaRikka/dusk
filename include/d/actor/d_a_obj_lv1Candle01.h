#ifndef D_A_OBJ_LV1CANDLE01_H
#define D_A_OBJ_LV1CANDLE01_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daLv1Cdl01_c
 * @brief Forest Temple Torch 01
 *
 * @details
 *
 */
class daLv1Cdl01_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    void lightInit();
    void setLight();
    void cutLight();
    void pointLightProc();
    int Execute(Mtx**);
    int Draw();
    int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dCcD_Stts mStts;
    /* 0x5E8 */ dCcD_Cyl mCyl;
    /* 0x724 */ cXyz mTorchPos;
    /* 0x730 */ bool mIsLit;
    /* 0x731 */ u8 mTimer;
    /* 0x734 */ cXyz mLightPos;
    /* 0x740 */ u8 mLightType;
    /* 0x744 */ f32 mIntensity;
    /* 0x748 */ LIGHT_INFLUENCE mLight;
    /* 0x768 */ u8 mTgHit;
    /* 0x76C */ Z2SoundObjSimple mSound;

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;
};

STATIC_ASSERT(sizeof(daLv1Cdl01_c) == 0x78C);

class daLv1Cdl01_HIO_c : public mDoHIO_entry_c {
public:
    daLv1Cdl01_HIO_c();
    ~daLv1Cdl01_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
};

#endif /* D_A_OBJ_LV1CANDLE01_H */
