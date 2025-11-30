#ifndef D_A_DSHUTTER_H
#define D_A_DSHUTTER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-unsorted
 * @class daDsh_c
 * @brief Death Sword Shutter Gate
 *
 * @details
 *
 */
class daDsh_c : public dBgS_MoveBgActor {
public:
    class action_c {
    public:
        typedef int (daDsh_c::*actionFunc)();

        action_c(actionFunc i_initFunc, actionFunc i_execFunc) {
            mInitFunc = i_initFunc;
            mExecuteFunc = i_execFunc;
        }

        actionFunc& getInit() { return mInitFunc; }
        actionFunc& getExecute() { return mExecuteFunc; }

        /* 0x0 */ actionFunc mInitFunc;
        /* 0xC */ actionFunc mExecuteFunc;
    };

    ~daDsh_c();
    void setMtx();
    void setAction(daDsh_c::action_c*);
    int callInit();
    int callExecute();
    int initOpenWait();
    int executeOpenWait();
    int initOpen();
    int executeOpen();
    int initCloseWait();
    int executeCloseWait();
    int initClose();
    int executeClose();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();

    inline int create();

    u8 getType() { return (fopAcM_GetParam(this) >> 8) & 0xF; }
    int getSw() { return fopAcM_GetParam(this) & 0xFF; }

    u8 getOpenTiltTime() { return 0; }
    f32 getOpenAccel() { return OPEN_ACCEL; }
    f32 getOpenSpeed() { return OPEN_SPEED; }
    f32 getOpenBoundSpeed() { return OPEN_BOUND_SPEED; }
    f32 getOpenBoundRatio() { return OPEN_BOUND_RATIO; }

    u8 getCloseTiltTime() { return 0; }
    f32 getCloseAccel() { return CLOSE_ACCEL; }
    f32 getCloseSpeed() { return CLOSE_SPEED; }
    f32 getCloseBoundSpeed() { return CLOSE_BOUND_SPEED; }
    f32 getCloseBoundRatio() { return CLOSE_BOUND_RATIO; }

    static f32 const OPEN_SIZE;
    static f32 const OPEN_ACCEL;
    static f32 const OPEN_SPEED;
    static f32 const OPEN_BOUND_SPEED;
    static f32 const OPEN_BOUND_RATIO;
    static f32 const CLOSE_ACCEL;
    static f32 const CLOSE_SPEED;
    static f32 const CLOSE_BOUND_SPEED;
    static f32 const CLOSE_BOUND_RATIO;

    /* 0x5A0 */ dComIfG_resLoader_c mResLoader;
    /* 0x5B0 */ J3DModel* mpModel;
    /* 0x5B4 */ action_c* mpAction;
    /* 0x5B8 */ s16 mTiltTime;
    /* 0x5BA */ u8 mSw;
    /* 0x5BB */ u8 mType;
};

#endif /* D_A_DSHUTTER_H */
