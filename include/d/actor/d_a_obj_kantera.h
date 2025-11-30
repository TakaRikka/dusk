#ifndef D_A_OBJ_KANTERA_H
#define D_A_OBJ_KANTERA_H

#include "d/actor/d_a_itembase.h"

/**
 * @ingroup actors-objects
 * @class daItemKantera_c
 * @brief Lantern
 *
 * @details
 *
 */
class daItemKantera_c : public daItemBase_c {
public:
    typedef int (daItemKantera_c::*actionFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int __CreateHeap();
    int create();
    void bg_check();
    int actionInit();
    int actionWaitInit();
    int actionWait();
    int initActionOrderGetDemo();
    int actionOrderGetDemo();
    int actionGetDemo();
    int execute();
    int draw();
    int _delete();

    void setStatus(u8 i_status) { mStatus = i_status; }

private:
    /* 0x92C */ u32 field_0x92c;
    /* 0x930 */ s32 field_0x930;
    /* 0x934 */ u8 mStatus;
    /* 0x935 */ u8 field_0x935;
    /* 0x936 */ u8 field_0x936;
    /* 0x937 */ u8 field_0x937;
    /* 0x938 */ s16 field_0x938;
    /* 0x93A */ s16 field_0x93a;
    /* 0x93C */ cXyz field_0x93c;
};

STATIC_ASSERT(sizeof(daItemKantera_c) == 0x948);

#endif /* D_A_OBJ_KANTERA_H */
