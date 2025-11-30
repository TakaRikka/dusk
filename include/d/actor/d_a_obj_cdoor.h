#ifndef D_A_OBJ_CDOOR_H
#define D_A_OBJ_CDOOR_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjCdoor_c
 * @brief Chain Door
 *
 * @details
 *
 */
class daObjCdoor_c : public dBgS_MoveBgActor {
public:
    enum Mode {
        /* 0 */ MODE_WAIT,
        /* 1 */ MODE_OPEN,
        /* 2 */ MODE_CLOSE,
    };

    enum Action {
        /* 0 */ ACT_WAIT,
        /* 1 */ ACT_ORDER_EVENT,
        /* 2 */ ACT_EVENT,
        /* 3 */ ACT_DEAD,
    };

    virtual ~daObjCdoor_c();
    int CreateHeap();
    cPhs__Step create();
    void setMatrix();
    int Execute(Mtx**);
    void execCdoor();
    void execWgate();
    void init_modeWait();
    void modeWait();
    void init_modeOpen();
    void modeOpen();
    void init_modeClose();
    void modeClose();
    void event_proc_call();
    void actionWait();
    void actionOrderEvent();
    void actionEvent();
    void actionDead();
    int Draw();

    void setAction(u8 i_action) { mAction = i_action; }
    int getSwitchNum() { return mSw; }
    void setChainID(u32 i_id) { mChainID = i_id; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ bool mEnd;
    /* 0x5AD */ bool mCamLock;
    /* 0x5AE */ u8 mType;
    /* 0x5AF */ u8 field_0x5af;
    /* 0x5B0 */ u8 mIsOpen;
    /* 0x5B1 */ u8 field_0x5b1[3];
    /* 0x5B4 */ s16 mEventID;
    /* 0x5B6 */ u8 mMapToolID;
    /* 0x5B7 */ u8 mAction;
    /* 0x5B8 */ u32 mChainID;
    /* 0x5BC */ int mSw;
    /* 0x5C0 */ mDoExt_bckAnm mOpenAnm;
    /* 0x5DC */ mDoExt_bckAnm mCloseAnm;
    /* 0x5F8 */ u8 field_0x5f8;
    /* 0x5F9 */ u8 mMode;
};

STATIC_ASSERT(sizeof(daObjCdoor_c) == 0x5FC);

#endif /* D_A_OBJ_CDOOR_H */
