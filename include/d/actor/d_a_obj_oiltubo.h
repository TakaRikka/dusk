#ifndef D_A_OBJ_OILTUBO_H
#define D_A_OBJ_OILTUBO_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObj_Oiltubo_c
 * @brief Oil Jar
 *
 * @details
 *
 */
class daObj_Oiltubo_c : public fopAc_ac_c {
public:
    typedef int (daObj_Oiltubo_c::*processFn)(void*);

    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    const char* getResName();
    void restart();
    void initialize();
    int setProcess(processFn);
    void setParam();
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();
    void setAttnPos();
    BOOL chkEvent();
    int wait(void*);

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ J3DModel* mpBModel;
    /* 0x578 */ mDoExt_btkAnm mBtk;
    /* 0x590 */ dBgS_ObjAcch mAcch;
    /* 0x768 */ dCcD_Stts mCcStts;
    /* 0x7A4 */ dBgS_AcchCir mAcchCir;
    /* 0x7E4 */ dCcD_Cyl mCcAtCyl;
    /* 0x920 */ dCcD_Cyl mCcCoCyl;
    /* 0xA5C */ cBgS_GndChk mGroundChk;
    /* 0xA98 */ processFn mProcess;
    /* 0xAA4 */ Z2SoundObjSimple mSound;
    /* 0xAC4 */ f32 mGroundHeight;
    /* 0xAC8 */ u32 mShadowKey;
    /* 0xACC */ u16 mMode;

public:
    virtual ~daObj_Oiltubo_c() {}
};

STATIC_ASSERT(sizeof(daObj_Oiltubo_c) == 0xAD4);

#endif /* D_A_OBJ_OILTUBO_H */
