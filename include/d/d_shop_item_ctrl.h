#ifndef D_SHOP_D_SHOP_ITEM_CTRL_H
#define D_SHOP_D_SHOP_ITEM_CTRL_H

#include "SSystem/SComponent/c_xyz.h"

class dShopItemCtrl_c {
public:
    dShopItemCtrl_c();
    cXyz getCurrentPos(int);
    bool isHomePos(int);
    void setRotateAnime(int);
    void setZoomAnime(int, cXyz*, s16, bool);

    virtual ~dShopItemCtrl_c();

    void setItemIndex(int i, u32 index) { mItemIndex[i] = index; }
    u32 getItemIndex(int i) { return mItemIndex[i]; }
    void setMessageIndex(int i, u16 messageIndex) { mMessageIndex[i] = messageIndex; }
    u16 getMessageIndex(int i) { return mMessageIndex[i]; }

private:
    /* 0x04 */ u32 mItemIndex[7];
    /* 0x20 */ u16 mMessageIndex[7];
};

#endif /* D_SHOP_D_SHOP_ITEM_CTRL_H */
