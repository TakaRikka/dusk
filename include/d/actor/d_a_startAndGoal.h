#ifndef D_A_STARTANDGOAL_H
#define D_A_STARTANDGOAL_H

#include "d/actor/d_a_npc4.h"

class daStartAndGoal_Path_c : public daNpcF_Path_c {
public:
    BOOL chkPassed2(cXyz);

    virtual ~daStartAndGoal_Path_c();
};

class daStartAndGoal_c : public fopAc_ac_c {
public:
    int getType();
    int Create();
    void init();
    void actorPosCheck();
    int readyStartTimer();
    BOOL isStartCheck();
    BOOL isReadyCheck();
    int Execute();
    int Draw();
    int Delete();

    /* 0x568 */ daStartAndGoal_Path_c mPath;
    /* 0xB98 */ u8 field_0xb98;
    /* 0xB99 */ u8 mType;
    /* 0xB9C */ int mAction;
    /* 0xBA0 */ bool mStarted;
};

#endif /* D_A_STARTANDGOAL_H */
