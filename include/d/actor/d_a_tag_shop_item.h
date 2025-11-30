#ifndef D_A_TAG_SHOP_ITEM_H
#define D_A_TAG_SHOP_ITEM_H

#include "d/d_com_inf_game.h"

class daTag_ShopItem_c : public fopAc_ac_c {
public:
    int create();
    int Delete();
    int Execute();
    int Draw();
    u8 getType();
    u8 getGroupID();
    u8 getSwitchBit1();
    u8 getSwitchBit2();
    void initialize();

    virtual ~daTag_ShopItem_c();

    u32 getProcessID() { return mProcessID; }

    /* 0x56C */ u32 mProcessID;
    /* 0x570 */ s16 mCreateTimer;
    /* 0x572 */ u8 field_0x572;
};  // Size: 0x574

#endif /* D_A_TAG_SHOP_ITEM_H */
