#ifndef D_A_OBJ_LV3WATER_H
#define D_A_OBJ_LV3WATER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_event_lib.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daLv3Water_c
 * @brief Lakebed Temple Water
 *
 * @details
 *
 */
class daLv3Water_c : public dBgS_MoveBgActor,
                     public request_of_phase_process_class,
                     public dEvLib_callback_c {
public:
    typedef void (daLv3Water_c::*modeFunc)();

    enum Mode {
        WAIT,
        LEVEL_CTRL,
    };

    daLv3Water_c() : dEvLib_callback_c(this) {}

    void setBaseMtx();
    virtual int CreateHeap();
    int create();
    virtual int Execute(Mtx**);
    void effectSet();
    void mode_proc_wait();
    void mode_init_levelCtrl();
    void mode_proc_levelCtrl();
    virtual int Draw();
    virtual int Delete();

    virtual ~daLv3Water_c() {}
    virtual bool eventStart();

    int getParam() { return fpcM_GetParam(this) >> 0xC & 0x0FFF; }
    int getParamSw() { return fpcM_GetParam(this) & 0xFF; }
    int getParamEvent() { return shape_angle.x & 0xFF; }
    u8 getParamType() { return (shape_angle.x >> 8) & 0xFF; }
    u8 getType() { return mType; }

private:
    /* 0x5B8 */ request_of_phase_process_class mPhase;
    /* 0x5C0 */ J3DModel* mpModel1;
    /* 0x5C4 */ J3DModel* mpModel2;
    /* 0x5C8 */ mDoExt_btkAnm mBtk1;
    /* 0x5E0 */ mDoExt_btkAnm mBtk2;
    /* 0x5F8 */ u8 mMode;
    /* 0x5F9 */ u8 mType;
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ u8 field_0x600;
    /* 0x601 */ u8 field_0x601;
    /* 0x602 */ u8 mSwitch1;
    /* 0x603 */ u8 mSwitch2;
    /* 0x604 */ u8 field_0x604;
    /* 0x605 */ u8 field_0x605;
    /* 0x608 */ u32 mEmitterIDs[8];
};

STATIC_ASSERT(sizeof(daLv3Water_c) == 0x628);

#endif /* D_A_OBJ_LV3WATER_H */
