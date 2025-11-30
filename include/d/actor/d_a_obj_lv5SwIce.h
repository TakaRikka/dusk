#ifndef D_A_OBJ_LV5SWICE_H
#define D_A_OBJ_LV5SWICE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daLv5SwIce_c
 * @brief Snowpeak Ruins Ice Switch
 *
 * @details
 *
 */
class daLv5SwIce_c : public dBgS_MoveBgActor {
public:
    enum daLv5SwIce_MODE {
        MODE_WAIT,
        MODE_BREAK,
    };

    void setBaseMtx();
    int create();
    void procMain();
    void init_modeWait();
    void modeWait();
    void init_modeBreak();
    void modeBreak();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSwBit1() { return fopAcM_GetParamBit(this, 0, 8); }

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x5A0 */ dCcD_Stts mCcStts;
    /* 0x5DC */ dCcD_Cyl mCcCyl;
    /* 0x718 */ request_of_phase_process_class mPhase;
    /* 0x720 */ J3DModel* mpModel;
    /* 0x724 */ u8 mMode;
    /* 0x725 */ u8 mHitcount;
    /* 0x726 */ u8 mSwBit1;
    /* 0x728 */ u32 mEmitter0ID[2];
    /* 0x730 */ u32 mEmitter1ID[2];
};

STATIC_ASSERT(sizeof(daLv5SwIce_c) == 0x738);

class daLv5SwIce_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daLv5SwIce_HIO_c();
    virtual ~daLv5SwIce_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ u8 hitcount;
};


#endif /* D_A_OBJ_LV5SWICE_H */
