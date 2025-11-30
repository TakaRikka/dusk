#ifndef D_A_OBJ_LV4RAILWALL_H
#define D_A_OBJ_LV4RAILWALL_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_event_lib.h"

class daObjSwSpinner_c;

/**
 * @ingroup actors-objects
 * @class daObjLv4Wall_c
 * @brief Arbiters Grounds Spinner Rail Wall (Stallord Arena)
 *
 * @details
 *
 */
class daObjLv4Wall_c : public dBgS_MoveBgActor, public dEvLib_callback_c {
public:
    enum {
        MODE_WAIT,
        MODE_MOVE,
        MODE_DEAD,
    };

    daObjLv4Wall_c() : dEvLib_callback_c(this) {}

    void initBaseMtx();
    void setBaseMtx();
    int create1st();
    void action();
    void mode_init_wait();
    void mode_wait();
    void mode_init_move();
    void mode_move();
    void mode_init_dead();
    void mode_dead();

    virtual ~daObjLv4Wall_c() {}

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    virtual bool eventStart();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5B0 */ request_of_phase_process_class mPhase;
    /* 0x5B8 */ J3DModel* mpModel;
    /* 0x5BC */ dBgS_ObjAcch field_0x5bc;
    /* 0x794 */ dBgS_AcchCir field_0x794;
    /* 0x7D4 */ dCcD_Stts field_0x7d4;
    /* 0x810 */ dCcD_Cyl field_0x810;
    /* 0x94C */ daObjSwSpinner_c* mpSwSpinner;
    /* 0x950 */ f32 mHeight;
    /* 0x954 */ int mRotCounter;
    /* 0x958 */ u16 mMoveCounter;
    /* 0x95A */ u8 mMode;
};

STATIC_ASSERT(sizeof(daObjLv4Wall_c) == 0x95c);


#endif /* D_A_OBJ_LV4RAILWALL_H */
