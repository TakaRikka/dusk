#ifndef D_A_OBJ_SS_ITEM_H
#define D_A_OBJ_SS_ITEM_H

#include "SSystem/SComponent/c_phase.h"
#include "d/actor/d_a_obj_ss_base.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"

class daObj_SSItem_c;
typedef int (daObj_SSItem_c::*ProcessFunc)(void*);

/**
 * @ingroup actors-objects
 * @class daObj_SSItem_c
 * @brief ???
 *
 * @details
 *
 */
class daObj_SSItem_c : public daObj_SSBase_c {
public:
    ~daObj_SSItem_c();
    int create();
    BOOL CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c* i_this);
    void setSoldOut();
    u32 getProcessID();
    daObj_SSItem_c* getExchangeItemPtr();
    char* getResName();
    u8 getTypeFromParam();
    u16 getFlowNodeNum();
    u16 getValue();
    void restart();
    void initialize();
    int setProcess(ProcessFunc i_process);
    void setParam();
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();
    void setAttnPos();
    int wait(void* param_0);
    int buy(void* param_0);
    int cancel(void* param_0);

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x578 */ request_of_phase_process_class mPhase;
    /* 0x580 */ J3DModel* mpModel;
    /* 0x584 */ dBgS_ObjAcch mAcch;
    /* 0x75C */ dCcD_Stts mStts;
    /* 0x798 */ dBgS_AcchCir mAcchCir;
    /* 0x7D8 */ dCcD_Cyl mAtCyl;
    /* 0x914 */ dCcD_Cyl mTgCyl;
    /* 0xA50 */ cBgS_GndChk mGndChk;
    /* 0xA8C */ cXyz field_0xA8C;
    /* 0xA98 */ dMsgFlow_c mMsgFlow;
    /* 0xAE4 */ ProcessFunc mProcessFunc;
    /* 0xAF0 */ cXyz field_0xAF0;
    /* 0xAFC */ s32 field_0xAFC;
    /* 0xB00 */ f32 field_0xB00;
    /* 0xB04 */ s32 field_0xB04;
    /* 0xB08 */ u8 padding[4];
    /* 0xB0C */ u16 mEventType;
    /* 0xB0E */ u8 field_0xB0E;
};

STATIC_ASSERT(sizeof(daObj_SSItem_c) == 0xB10);

#endif /* D_A_OBJ_SS_ITEM_H */
