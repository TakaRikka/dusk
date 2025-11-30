#ifndef D_A_OBJ_LV4CANDLEDEMOTAG_H
#define D_A_OBJ_LV4CANDLEDEMOTAG_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_event_lib.h"
#include "d/actor/d_a_obj_lv4EdShutter.h"

/**
 * @ingroup actors-objects
 * @class dalv4CandleDemoTag_c
 * @brief Arbiter's Grounds Torch Cutscene Tag
 *
 * @details
 *
 */
class dalv4CandleDemoTag_c : public fopAc_ac_c, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    typedef void (dalv4CandleDemoTag_c::*procFunc)();
    dalv4CandleDemoTag_c() : dEvLib_callback_c(this) {}

    void setBaseMtx();
    int create();
    int Execute();
    void procMain();
    void init_modeWatch();
    void modeWatch();
    void init_modeEnemyCreate();
    void modeEnemyCreate();
    void init_modeNG();
    void modeNG();
    void init_modeEnd();
    void modeEnd();
    static void* searchShutterAct(void*, void*);
    int Draw();
    int Delete();

    u8 getPathID() { return fopAcM_GetParamBit(this, 0x10, 8); }
    int getSwNG() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSwDest() { return fopAcM_GetParamBit(this, 8, 8); }
    int getSwOK() { return shape_angle.x & 0xFF; }
    int getEventID() { return fopAcM_GetParamBit(this, 0x18, 8); }

    virtual ~dalv4CandleDemoTag_c() {}
    virtual bool eventStart();

private:
    /* 0x584 */ u8 mMode;
    /* 0x585 */ u8 mPathID;
    /* 0x586 */ u8 mSwNG;
    /* 0x587 */ u8 mSwDest;
    /* 0x588 */ u16 mEnemyWaitTime;
    /* 0x58C */ daLv4EdShutter_c* mpShutterActor;
};

STATIC_ASSERT(sizeof(dalv4CandleDemoTag_c) == 0x590);


#endif /* D_A_OBJ_LV4CANDLEDEMOTAG_H */
