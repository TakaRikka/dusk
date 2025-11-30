#ifndef D_A_OBJ_SS_DRINK_H
#define D_A_OBJ_SS_DRINK_H

#include "d/actor/d_a_obj_ss_item.h"

/**
 * @ingroup actors-objects
 * @class daObj_SSDrink_c
 * @brief ???
 *
 * @details
 *
 */
class daObj_SSDrink_c : public daObj_SSBase_c {
public:
    typedef int (daObj_SSDrink_c::*ProcessFunc)(void*);

    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    char* getResName();
    u8 getTypeFromParam();
    u8 getSwitchFromParam();
    u8 getCapacityFromParam();
    u16 getFlowNodeNum();
    u16 getValue();
    void restart();
    void initialize();
    int checkProcess(ProcessFunc);
    int setProcess(ProcessFunc);
    void setParam();
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();
    void setAttnPos();
    void animeEntry();
    void animePlay();
    int chkEvent();
    int orderEvent();
    int wait(void*);
    int talk(void*);
    int drink(void*);

    virtual ~daObj_SSDrink_c();
    virtual void setSoldOut();

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x578 */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x57C */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x580 */ u8 field_0x580;
    /* 0x584 */ request_of_phase_process_class mPhase;
    /* 0x58C */ J3DModel* mpModel;
    /* 0x590 */ dBgS_ObjAcch mAcch;
    /* 0x768 */ dCcD_Stts mStts;
    /* 0x7A4 */ dBgS_AcchCir mAcchCir;
    /* 0x7E4 */ dCcD_Cyl mCyl1;
    /* 0x920 */ dCcD_Cyl mCyl2;
    /* 0xA5C */ cBgS_GndChk mGndChk;
    /* 0xA98 */ cXyz field_0xa98;
    /* 0xAA4 */ dMsgFlow_c mFlow;
    /* 0xAF0 */ ProcessFunc field_0xaf0;
    /* 0xAFC */ f32 field_0xafc;
    /* 0xB00 */ f32 mLeftHandPosY;
    /* 0xB04 */ u8 field_0xb04[4];
    /* 0xB08 */ u16 field_0xb08;
    /* 0xB0A */ u8 field_0xb0a;
    /* 0xB0B */ u8 field_0xb0b;
    /* 0xB0C */ u8 field_0xb0c;
};

STATIC_ASSERT(sizeof(daObj_SSDrink_c) == 0xb10);

#endif /* D_A_OBJ_SS_DRINK_H */
