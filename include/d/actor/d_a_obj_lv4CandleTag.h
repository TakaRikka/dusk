#ifndef D_A_OBJ_LV4CANDLETAG_H
#define D_A_OBJ_LV4CANDLETAG_H

#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class dalv4CandleTag_c
 * @brief Arbiter's Grounds Torch Tag
 *
 * @details
 *
 */
class dalv4CandleTag_c : public fopAc_ac_c, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    enum Mode {
        MODE_WATCH,
        MODE_NG,
        MODE_NG_WAIT,
        MODE_END,
    };

    dalv4CandleTag_c() : dEvLib_callback_c(this) {}
    ~dalv4CandleTag_c() {}

    void setBaseMtx();
    int create();
    int Execute();
    void procMain();
    void init_modeWatch();
    void modeWatch();
    void init_modeNG();
    void modeNG();
    void init_modeNGWait();
    void modeNGWait();
    void init_modeEnd();
    void modeEnd();
    bool eventStart();
    int Draw();
    int Delete();

    u8 getSwTop() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwOK() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getSwNG() { return fopAcM_GetParamBit(this, 0x10, 8); }
    u8 getSwTotal() { return fopAcM_GetParamBit(this, 0x18, 8); }
    int getEventID() { return shape_angle.x & 0xff; }
    u8 getNGSwStat() { return fopAcM_isSwitch(this, mSwNG); }

    /* 0x584 */ u8 mMode;
    /* 0x585 */ u8 mSwTop;
    /* 0x586 */ u8 mSwOK;
    /* 0x587 */ u8 mSwNG;
    /* 0x588 */ u8 mSwTotal;
    /* 0x589 */ u8 mTimer;
};

STATIC_ASSERT(sizeof(dalv4CandleTag_c) == 0x58C);

#endif /* D_A_OBJ_LV4CANDLETAG_H */
