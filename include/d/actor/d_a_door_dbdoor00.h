#ifndef D_A_DOOR_DBDOOR00_H
#define D_A_DOOR_DBDOOR00_H

#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-doors
 * @class daDbDoor00_c
 * @brief Double Door
 *
 * @details
 *
 */
class daDbDoor00_c : public fopAc_ac_c {
public:
    char* getAlwaysArcName();
    char* getDzb();
    char* getDummyBmdName();
    char* getBmdArcName();
    char* getBmdName();
    J3DModelData* getDoorModelData();
    int CreateHeap();
    void calcMtx();
    int CreateInit();
    int create();
    int getDemoAction();
    void demoProc();
    int checkArea();
    int checkUnlock();
    int actionWait();
    int actionLockWait();
    int actionLockOff();
    int actionLockDemo();
    int actionCloseWait();
    int actionOpen();
    int execute();
    int checkDraw();
    int draw();
    int Delete();

    void setAction(u8 i_action) { mAction = i_action; }

// private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ request_of_phase_process_class mPhaseReq2;
    /* 0x578 */ J3DModel* mpModel;
    /* 0x57C */ J3DModel* mpModel2;
    /* 0x580 */ dBgW* mpBgW;
    /* 0x584 */ u8 field_0x584;
    /* 0x585 */ u8 field_0x585;
    /* 0x586 */ u8 mAction;
    /* 0x587 */ u8 field_0x587[0x58A - 0x587];
    /* 0x58A */ s16 field_0x58a;
    /* 0x58C */ s16 field_0x58c;
    /* 0x58E */ u8 field_0x58E[0x590 - 0x58E];
    /* 0x590 */ s16 field_0x590;
    /* 0x594 */ s32 field_0x594;
    /* 0x598 */ s32 field_0x598;
    /* 0x59C */ cXyz field_0x59c;
    /* 0x5A8 */ cXyz field_0x5a8;
    /* 0x5B4 */ cXyz field_0x5b4;
    /* 0x5C0 */ s8 field_0x5c0;
    /* 0x5C1 */ s8 field_0x5c1;
    /* 0x5C2 */ s16 field_0x5c2;
};

STATIC_ASSERT(sizeof(daDbDoor00_c) == 0x5C4);

#endif /* D_A_DOOR_DBDOOR00_H */
