#ifndef D_A_OBJ_LV1CANDLE00_H
#define D_A_OBJ_LV1CANDLE00_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "m_Do/m_Do_hostIO.h"



/**
 * @ingroup actors-objects
 * @class daLv1Cdl00_c
 * @brief Forest Temple Torch 00
 *
 * @details
 *
 */
class daLv1Cdl00_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    static int createHeapCallBack(fopAc_ac_c*);
    void lightInit();
    void setLight();
    void cutLight();
    void pointLightProc();
    int Execute();
    int Draw();
    int Delete();

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dCcD_Stts mStts;
    /* 0x5B0 */ dCcD_Cyl mCyl;
    /* 0x6EC */ cXyz mTorchPos;
    /* 0x6F8 */ bool mIsLit;
    /* 0x6F9 */ u8 mIsSwitch;
    /* 0x6FA */ u8 mSwType;
    /* 0x6FC */ cXyz mLightPos;
    /* 0x708 */ u8 mLightType;
    /* 0x70C */ f32 mIntensity;
    /* 0x710 */ LIGHT_INFLUENCE mLight;
    /* 0x730 */ u8 mTgHit;
    /* 0x734 */ Z2SoundObjSimple mSound;

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;
};

STATIC_ASSERT(sizeof(daLv1Cdl00_c) == 0x754);

class daLv1Cdl00_HIO_c : public mDoHIO_entry_c {
public:
    daLv1Cdl00_HIO_c();
    ~daLv1Cdl00_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
};

#endif /* D_A_OBJ_LV1CANDLE00_H */
