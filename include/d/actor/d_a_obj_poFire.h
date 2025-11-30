#ifndef D_A_OBJ_POFIRE_H
#define D_A_OBJ_POFIRE_H

#include "d/actor/d_a_obj_poCandle.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_poFire.h"
#include "d/d_event_lib.h"


/**
 * @ingroup actors-objects
 * @class daPoFire_c
 * @brief Poe Fire
 *
 * @details
 *
 */
class daPoFire_c : public fopAc_ac_c,
                   public request_of_phase_process_class,
                   public dEvLib_callback_c {
public:
    typedef void (daPoFire_c::*modeProc)();
    daPoFire_c() : dEvLib_callback_c(this) {}

    enum Mode {
        MODE_INIT_MOVE = 0,
        MODE_WAIT = 1,
        MODE_WAIT_SET_EFFECT = 2,
        MODE_MOVE_OUT = 3,
        MODE_MOVE = 4,
        MODE_NO_MOVE = 5,
        MODE_MOVE_WAIT = 6,
        MODE_ON_FIRE = 7,
        MODE_FIRE_END = 8,
    };

    void setBaseMtx();
    int create();
    int Execute();
    void procMain();
    void setFire();
    static void* searchCandle(void*, void*);
    static void* searchFireTag(void*, void*);
    void init_modeInitMove();
    void modeInitMove();
    void modeWait();
    void init_modeWaitSetEffect();
    void modeWaitSetEffect();
    void init_modeMoveOut();
    void modeMoveOut();
    void init_modeMove();
    void modeMove();
    void init_modeMoveWait();
    void modeMoveWait();
    void modeNoMove();
    void init_modeOnFire();
    void modeOnFire();
    void init_modeFireEnd();
    void modeFireEnd();
    int Draw();
    int Delete();

    virtual ~daPoFire_c() {}
    virtual bool eventStart();
    virtual bool eventRun();
    virtual bool eventEnd();

    u8 getMoveEnd() { return mMoveEnd; }
    u32 getID() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x584 */ request_of_phase_process_class mPhase;
    /* 0x58C */ int mStaffID;
    /* 0x590 */ s16 unk590;
    /* 0x592 */ u8 mMode;
    /* 0x593 */ u8 mActorID;  // Identifies each of the 4 Poe flames
    /* 0x594 */ bool mMoveEnd;
    /* 0x598 */ u32 mParticleKeys[2];
    /* 0x5A0 */ cXyz unk5A0;
    /* 0x5AC */ cXyz unk5AC;
    /* 0x5B4 */ cXyz unk5B4;
    /* 0x5C4 */ u8 unk5C4;
    /* 0x5C5 */ u8 mWaitTimer;
    /* 0x5C8 */ daPoCandle_c* mpPoCandle;
    /* 0x5CC */ daTagPoFire_c* mpTagPoFire;
    /* 0x5D0 */ f32 mGroundY;
    /* 0x5D4 */ f32 unk5D4;
    /* 0x5D8 */ f32 unk5D8;
    /* 0x5DC */ u8 pad5DC[0xC];  // Padding
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 unk5EC;
    /* 0x5F0 */ f32 unk5F0;
    /* 0x5F4 */ f32 unk5F4;
    /* 0x5F8 */ cXyz unk5F8;
    /* 0x604 */ s32 unk604;
    /* 0x608 */ f32 unk608;
    /* 0x610 */ cXyz unk610;
};

STATIC_ASSERT(sizeof(daPoFire_c) == 0x618);

#endif /* D_A_OBJ_POFIRE_H */
