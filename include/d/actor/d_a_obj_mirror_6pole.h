#ifndef D_A_OBJ_MIRROR_6POLE_H
#define D_A_OBJ_MIRROR_6POLE_H

#include "d/d_com_inf_game.h"

class daObjMirror6Pole_c;
typedef void (daObjMirror6Pole_c::*actionFunc)();

/**
 * @ingroup actors-objects
 * @class daObjMirror6Pole_c
 * @brief Twilight Mirror Pole
 *
 * @details
 *
 */
class daObjMirror6Pole_c : public fopAc_ac_c {
public:
    enum Mode_e {
        MODE_WAIT_e,
        MODE_DEMO_e,
        MODE_MAX_e,
    };

    void initWait();
    void executeWait();
    void initDemo();
    void executeDemo();
    inline void setBaseMtx();
    inline void initBaseMtx();
    inline void create_init();
    static int createHeapCallBack(fopAc_ac_c* i_this);
    int CreateHeap();

    inline ~daObjMirror6Pole_c();

    inline int execute();
    inline int create();
    inline int draw();

    u8 getSwitchNo() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }

    void setAnmSpeed(f32 speed) { mpBck->setPlaySpeed(speed); }

    bool isSwitch() {
        return fopAcM_isSwitch(this, getSwitchNo()) ||
               /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
               dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361]) ||
               /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
               dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354]);
    }

    inline void setAction(Mode_e i_action);
    inline void callInit();
    inline void callExecute();

    static const actionFunc ActionTable[][2];

private:
    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_bckAnm* mpBck;
    /* 0x570 */ request_of_phase_process_class mPhase;
    /* 0x578 */ actionFunc* mAction;
    /* 0x57C */ Mode_e mMode;
};

STATIC_ASSERT(sizeof(daObjMirror6Pole_c) == 0x580);

#endif /* D_A_OBJ_MIRROR_6POLE_H */
