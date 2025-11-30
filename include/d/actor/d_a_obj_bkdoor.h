#ifndef D_A_OBJ_BKDOOR_H
#define D_A_OBJ_BKDOOR_H

#include "d/d_bg_s_movebg_actor.h"
#include "SSystem/SComponent/c_phase.h"

class J3DModel;

/**
 * @ingroup actors-objects
 * @class daObjBkDoor_c
 * @brief Ranch Door
 *
 * @details
 *
 */
class daObjBkDoor_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    virtual int Create();
    virtual int CreateHeap();
    int create1st();
    void openCheck();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ csXyz mRotation;
    /* 0x5B4 */ f32 mSwingSpeed;
    /* 0x5B8 */ s32 mFrameCounter;
    /* 0x5BC */ u8 mOrientation;
    /* 0x5BD */ bool mOpenCheckDone;
    /* 0x5BE */ bool mOpening;
};  // Size: 0x5C0

STATIC_ASSERT(sizeof(daObjBkDoor_c) == 0x5C0);

#endif /* D_A_OBJ_BKDOOR_H */
