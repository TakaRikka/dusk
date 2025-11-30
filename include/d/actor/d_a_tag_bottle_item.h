#ifndef D_A_TAG_BOTTLE_ITEM_H
#define D_A_TAG_BOTTLE_ITEM_H

#include "d/d_com_inf_game.h"


class daTag_BottleItem_c;
typedef int (daTag_BottleItem_c::*ProcessFunc)(void*);

class daTag_BottleItem_c : public fopAc_ac_c {
private:
    /* 0x0568 */ ProcessFunc mProcessFunc;
    /* 0x056C */ u16 mEventType;
    /* 0x0576 */ u8 mBottleItemType;

public:
    s32 create();
    s32 Delete();
    s32 Execute();
    s32 Draw();
    void setTypeFromParam();
    void restart();
    void initialize();
    int checkProcess(ProcessFunc);
    int setProcess(ProcessFunc);
    void setAttnPos();
    s32 chkEvent();
    s32 orderEvent();
    void makeSoup();
    int wait(void*);
    virtual ~daTag_BottleItem_c();
};

STATIC_ASSERT(sizeof(daTag_BottleItem_c) == 0x57C);

#endif /* D_A_TAG_BOTTLE_ITEM_H */
