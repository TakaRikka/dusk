#ifndef D_A_OBJ_SAIDAN_H
#define D_A_OBJ_SAIDAN_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daSaidan_c
 * @brief Altar
 *
 * @details
 *
 */
class daSaidan_c : public dBgS_MoveBgActor {
public:
    enum Mode_e {
        /* 0 */ MODE_WAIT_e,
        /* 1 */ MODE_MOVE_e,
        /* 2 */ MODE_MOVE_END_e,
    };

    void setBaseMtx();
    virtual int CreateHeap();
    cPhs__Step create();
    virtual int Execute(Mtx**);
    void moveProc();
    void init_modeWait();
    void modeWait();
    void init_modeMove();
    void modeMove();
    void init_modeMoveEnd();
    void modeMoveEnd();
    virtual int Draw();
    virtual int Delete();

private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mMode;
    /* 0x5AD */ u8 mIsSwitch;
    /* 0x5B0 */ f32 mOriginalPosX;
};

STATIC_ASSERT(sizeof(daSaidan_c) == 0x5B4);

struct daSaidan_HIO_c : public mDoHIO_entry_c {
    daSaidan_HIO_c();
    virtual ~daSaidan_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x00 vtable */
    /* 0x04 */ f32 mMaxStep;
    /* 0x08 */ f32 mTargetPosX;
};

#endif /* D_A_OBJ_SAIDAN_H */
