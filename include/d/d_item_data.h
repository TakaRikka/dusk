#ifndef D_D_ITEM_DATA_H
#define D_D_ITEM_DATA_H

#include <dolphin/types.h>

struct dItem_itemInfo {
    /* 0x0 */ u8 mShadowSize;
    /* 0x1 */ u8 mH;
    /* 0x2 */ u8 mR;
    /* 0x3 */ u8 mFlag;
};

struct dItem_itemResource {
    f32 getTexScale() { return mTexScale; }

    /* 0x00 */ const char* mArcName;
    /* 0x04 */ s16 mBmdName;
    /* 0x06 */ s16 mBtkName;
    /* 0x08 */ s16 mBckName;
    /* 0x0A */ s16 mBrkName;
    /* 0x0C */ s16 mBtpName;
    /* 0x0E */ s8 mTevFrm;
    /* 0x0F */ s8 mBtpFrm;
    /* 0x10 */ s16 mTexture;
    /* 0x12 */ u8 mTexScale;
    /* 0x14 */ s16 field_0x14;
};  // Size: 0x18

struct dItem_fieldItemResource {
    /* 0x0 */ const char* mFieldArc;
    /* 0x4 */ s16 mItemBmdName;
    /* 0x6 */ s16 mItemBckName;
    /* 0x8 */ s16 mItemBrkName;
    /* 0xA */ u8 field_0xa;
    /* 0xC */ u16 mHeapSize;
};  // Size: 0x10

struct dItem_data {
    static const char* getArcName(u8 index) { return item_resource[index].mArcName; }

    static s16 getBtpName(u8 index) { return item_resource[index].mBtpName; }

    static s16 getBrkName(u8 index) { return item_resource[index].mBrkName; }

    static s16 getBxaName(u8 index) { return -1; }

    static s16 getBckName(u8 index) { return item_resource[index].mBckName; }

    static s16 getBpkName(u8 index) { return -1; }

    static s16 getBtkName(u8 index) { return item_resource[index].mBtkName; }

    static s16 getBmdName(u8 index) { return item_resource[index].mBmdName; }

    static s8 getTevFrm(u8 index) { return item_resource[index].mTevFrm; }

    static s8 getBtpFrm(u8 index) { return item_resource[index].mBtpFrm; }

    static s16 getTexture(u8 index) { return item_resource[index].mTexture; }

    static u8 getTexScale (u8 index) { return item_resource[index].mTexScale; }

    static const char* getFieldArc(u8 index) { return field_item_res[index].mFieldArc; }

    static s16 getItemBmdName(u8 index) { return field_item_res[index].mItemBmdName; }

    static s16 getItemBtkName(u8 index) { return -1; }

    static s16 getItemBpkName(u8 index) { return -1; }

    static s16 getItemBckName(u8 index) { return field_item_res[index].mItemBckName; }

    static s16 getItemBxaName(u8 index) { return -1; }

    static s16 getItemBrkName(u8 index) { return field_item_res[index].mItemBrkName; }

    static s16 getItemBtpName(u8 index) { return -1; }

    static BOOL chkFlag(u8 index, int flag) { return item_info[index].mFlag & flag; }

    static u8 getShadowSize(u8 index) { return item_info[index].mShadowSize; }

    static const u8 getH(u8 index) { return item_info[index].mH; }

    static const u8 getR(u8 index) { return item_info[index].mR; }

    static u16 getFieldHeapSize(u8 index) { return field_item_res[index].mHeapSize; } 

    static dItem_itemResource item_resource[255];
    static dItem_fieldItemResource field_item_res[255];
    static dItem_itemInfo item_info[255];
};

#endif /* D_D_ITEM_DATA_H */
