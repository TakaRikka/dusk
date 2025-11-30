#ifndef D_A_OBJ_LV3CANDLE_H
#define D_A_OBJ_LV3CANDLE_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"


/**
 * @ingroup actors-objects
 * @class daLv3Candle_c
 * @brief Lakebed Temple Torch
 *
 * @details
 *
 */
class daLv3Candle_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    static int createHeapCallBack(fopAc_ac_c*);
    void lightInit();
    void pointLightProc();
    int Execute();
    int Draw();
    int Delete();

    u8 getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getType() { return fopAcM_GetParamBit(this, 8, 8); }

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ u8 mType;
    /* 0x578 */ dCcD_Stts mStts;
    /* 0x5B4 */ dCcD_Sph mSph;
    /* 0x6EC */ cXyz mTorchPos;
    /* 0x6F8 */ u32 field_0x6f8;  // Unused?
    /* 0x6FC */ cXyz mLightPos;
    /* 0x708 */ u8 mIsLit;
    /* 0x70C */ f32 mIntensity;
    /* 0x710 */ LIGHT_INFLUENCE mLight;
    /* 0x730 */ u8 mTgHit;
    /* 0x734 */ Z2SoundObjSimple mSound;

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcSph mCcDSph;

};  // Size: 0x754

STATIC_ASSERT(sizeof(daLv3Candle_c) == 0x754);

class daLv3Candle_HIO_c : public mDoHIO_entry_c {
public:
    daLv3Candle_HIO_c();
    virtual ~daLv3Candle_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x00 vtable */
    /* 0x04 */ u8 field_0x04;
};

#endif /* D_A_OBJ_LV3CANDLE_H */
