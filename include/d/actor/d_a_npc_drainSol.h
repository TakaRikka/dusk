#ifndef D_A_NPC_DRAINSOL_H
#define D_A_NPC_DRAINSOL_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcDrSol_c
 * @brief Drain Soldier (Hyrule Castle Sewer Soldier?)
 *
 * @details
 *
 */
class daNpcDrSol_c : public daNpcF_c {
public:
    daNpcDrSol_c();
    virtual ~daNpcDrSol_c();
    int Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    void reset();
    inline int setAction(bool (daNpcDrSol_c::*)(void*));
    bool chkFindPlayer();
    bool wait(void*);
    bool talk(void*);

    virtual void setParam();
    virtual BOOL main();
    virtual void adjustShapeAngle() {}
    virtual void setAttnPos();
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual BOOL drawDbgInfo();
    virtual void drawOtherMdls();

    inline void playMotion();

    u8 getType() { return argument & 0x7F; }
    int getSwitchNo() { return fopAcM_GetParam(this) & 0xFF; }
    s16 getMessageNo() { return (fopAcM_GetParam(this) >> 8) & 0xFFFF; }

    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ J3DModel* field_0xbd8;
    /* 0xBDC */ u8 field_0xBDC[0xBE0 - 0xBDC];
    /* 0xBE0 */ dCcD_Cyl mCyl;
    /* 0xD1C */ bool (daNpcDrSol_c::*mAction)(void*);
    /* 0xD28 */ request_of_phase_process_class mPhase;
    /* 0xD30 */ int mMessageNo;
    /* 0xD34 */ s16 field_0xd34;
    /* 0xD36 */ u16 mMode;
    /* 0xD38 */ u8 field_0xd38;
    /* 0xD39 */ u8 mType;
};

STATIC_ASSERT(sizeof(daNpcDrSol_c) == 0xd3c);

struct daNpcDrSol_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
};

class daNpcDrSol_Param_c {
public:
    virtual ~daNpcDrSol_Param_c() {}

    static const daNpcDrSol_HIOParam m;
};


#endif /* D_A_NPC_DRAINSOL_H */
