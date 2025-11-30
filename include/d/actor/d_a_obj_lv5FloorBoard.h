#ifndef D_A_OBJ_LV5FLOORBOARD_H
#define D_A_OBJ_LV5FLOORBOARD_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daFlorBoad_c
 * @brief Snowpeak Ruins Destructable Floor
 *
 * @details
 *
 */
class daFlorBoad_c : public dBgS_MoveBgActor {
public:
    enum daFlorBoad_MODE {
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
    /* 0x725 */ u8 field_0x725;
    /* 0x726 */ u8 mSwBit1;
    /* 0x727 */ u8 field_0x727[0x734 - 0x727];
};

STATIC_ASSERT(sizeof(daFlorBoad_c) == 0x734);

class daFlorBoad_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daFlorBoad_HIO_c();
    virtual ~daFlorBoad_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ cXyz field_0x4;
    /* 0x10 */ u8 field_0x10;
    /* 0x11 */ u8 field_0x11;
};


#endif /* D_A_OBJ_LV5FLOORBOARD_H */
