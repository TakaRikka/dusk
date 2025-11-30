#ifndef D_A_OBJ_LV2CANDLE_H
#define D_A_OBJ_LV2CANDLE_H

#include "d/d_cc_d.h"
#include "m_Do/m_Do_hostIO.h"
#include "d/actor/d_a_obj_lv4CandleTag.h"


/**
 * @ingroup actors-objects
 * @class daLv2Candle_c
 * @brief Goron Mines Torch
 *
 * @details
 *
 */
class daLv2Candle_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    u8 isSwitch();
    static int createHeapCallBack(fopAc_ac_c*);
    static void* searchDemoTagAct(void*, void*);
    void lightInit();
    void setLight();
    void cutLight();
    void pointLightProc();
    int Execute();
    int Draw();
    int Delete();

    int getOffType() { return fopAcM_GetParamBit(this, 0x18, 4); }
    u8 getModelType() { return fopAcM_GetParamBit(this, 0x14, 4); }
    u8 getLightAnm() { return fopAcM_GetParamBit(this, 0x10, 4); }
    u8 getLightType() { return fopAcM_GetParamBit(this, 0xc, 4); }
    u8 getSwType() { return fopAcM_GetParamBit(this, 8, 4); }
    u8 getSw() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dCcD_Stts mStts;
    /* 0x5B0 */ dCcD_Cyl mCyl;
    /* 0x6EC */ cXyz mTorchPos;
    /* 0x6F8 */ u8 mSw;
    /* 0x6F9 */ bool mIsLit;
    /* 0x6FA */ u8 mIsSwitch;
    /* 0x6FB */ u8 mTimer;
    /* 0x6FC */ u8 mSwType;
    /* 0x6FD */ u8 mModelType;
    /* 0x6FE */ u8 mLightAnm;
    /* 0x700 */ cXyz mLightPos;
    /* 0x70C */ u8 mLightType;
    /* 0x70D */ u8 mAnmTimer;
    /* 0x70E */ u8 mAnmStage;
    /* 0x710 */ f32 mIntensity;
    /* 0x714 */ LIGHT_INFLUENCE mLight;
    /* 0x734 */ dalv4CandleTag_c* field_0x734;
    /* 0x738 */ u8 mTgHit;
    /* 0x73C */ Z2SoundObjSimple mSound;

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;
};

STATIC_ASSERT(sizeof(daLv2Candle_c) == 0x75C);

class daLv2Candle_HIO_c : public mDoHIO_entry_c {
public:
    daLv2Candle_HIO_c();
    ~daLv2Candle_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 mTimer;
    /* 0x08 */ f32 mPow;
    /* 0x0C */ f32 mAnmMaxStep;
    /* 0x10 */ f32 mAnmIntensity[6];
    /* 0x28 */ u8 mAnmTimers[6];
    /* 0x2E */ u8 mColor1R;
    /* 0x2F */ u8 mColor1G;
    /* 0x30 */ u8 mColor1B;
    /* 0x31 */ u8 mColor2R;
    /* 0x32 */ u8 mColor2G;
    /* 0x33 */ u8 mColor2B;
};

#endif /* D_A_OBJ_LV2CANDLE_H */
