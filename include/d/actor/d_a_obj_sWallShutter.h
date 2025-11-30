#ifndef D_A_OBJ_SWALLSHUTTER_H
#define D_A_OBJ_SWALLSHUTTER_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daSwShutter_c
 * @brief Shutter Wall (Switch)
 *
 * @details
 *
 */
class daSwShutter_c : public dBgS_MoveBgActor {
public:
    enum TYPE_e {
        TYPE_ROCKWALL_e,  // Goron Mines Shutter Wall
        TYPE_SUBDAN_e,    // Cave of Ordeals Shutter Wall
    };

    enum MODE_e {
        MODE_WAIT,
        MODE_MOVE_DOWN_INIT,
        MODE_MOVE_DOWN,
        MODE_MOVE_DOWN_END,
    };

    void setBaseMtx();
    int create();
    void moveMain();
    void init_modeWait();
    void modeWait();
    void init_modeMoveDownInit();
    void modeMoveDownInit();
    void init_modeMoveDown();
    void modeMoveDown();
    void init_modeMoveDownEnd();
    void modeMoveDownEnd();
    
    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }
    int getModelType() { return fopAcM_GetParamBit(this, 8, 4); }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mModelType;
    /* 0x5AD */ u8 mMode;
    /* 0x5B0 */ f32 field_0x5b0;
    /* 0x5B4 */ u8 field_0x5B4[0x5B8 - 0x5B4];
    /* 0x5B8 */ csXyz mShakeRot;
    /* 0x5C0 */ f32 mShakeStrength;
    /* 0x5C4 */ f32 mShakeAmpZ;
    /* 0x5C8 */ f32 mShakeAmpY;
    /* 0x5CC */ f32 mShakeAtten;
    /* 0x5D0 */ f32 mMaxAtten;
    /* 0x5D4 */ f32 mMinAtten;
    /* 0x5D8 */ int mCounter;
    /* 0x5DC */ u32 mEmitterID0;
    /* 0x5E0 */ u32 mEmitterID1;
    /* 0x5E4 */ u8 field_0x5e4[0x5e8 - 0x5e4];
};

STATIC_ASSERT(sizeof(daSwShutter_c) == 0x5e8);

class daSwShutter_HIO_c : public mDoHIO_entry_c {
public:
    daSwShutter_HIO_c();
    virtual ~daSwShutter_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 mInitSpeed;
    /* 0x08 */ f32 mMaxSpeed;
    /* 0x0C */ f32 mAcceleration;
    /* 0x10 */ u8 mVibrationStrength;
    /* 0x14 */ f32 mShakeStrength;
    /* 0x18 */ f32 mShakeAmpZ;
    /* 0x1C */ f32 mShakeAmpY;
    /* 0x20 */ f32 mShakeAtten;
    /* 0x24 */ f32 mMaxAtten;
    /* 0x28 */ f32 mMinAtten;
};


#endif /* D_A_OBJ_SWALLSHUTTER_H */
