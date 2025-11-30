#ifndef D_A_D_A_SHOP_ITEM_STATIC_H
#define D_A_D_A_SHOP_ITEM_STATIC_H

#include "d/actor/d_a_itembase.h"

struct ResourceData {
    const char* get_arcName() const { return mArcName; }
    s16 get_bmdName() const { return mBmdName; }
    s16 get_btk1Name() const { return mBtkName; }
    s16 get_bpk1Name() const { return mBpkName; }
    s16 get_bck1Name() const { return mBckName; }
    s16 get_bxa1Name() const { return mBxaName; }
    s16 get_brk1Name() const { return mBrkName; }
    s16 get_btp1Name() const { return mBtpName; }
    f32 get_scale() const { return mScale; }
    f32 get_offsetY() const { return mOffsetY; }
    s16 get_angleX() const { return mAngle.x; }
    s16 get_angleY() const { return mAngle.y; }
    s16 get_angleZ() const { return mAngle.z; }
    s16 get_angleOffsetX() const { return mAngleOffsetX; }
    u32 get_flag() const { return mFlag; }
    s8 get_tevfrm() const { return mTevFrm; }
    s8 get_btpfrm() const { return mBtpFrm; }
    u8 get_shadowSize() const { return mShadowSize; }
    u8 get_collisionH() const { return mCollisionH; }
    u8 get_collisionR() const { return mCollisionR; }

    /* 0x00 */ const char* mArcName;
    /* 0x04 */ s16 mBmdName;
    /* 0x06 */ s16 mBtkName;
    /* 0x08 */ s16 mBpkName;
    /* 0x0A */ s16 mBckName;
    /* 0x0C */ s16 mBxaName;
    /* 0x0E */ s16 mBrkName;
    /* 0x10 */ s16 mBtpName;
    /* 0x14 */ f32 mOffsetY;
    /* 0x18 */ f32 mScale;
    /* 0x1C */ s16 mAngleOffsetX;
    /* 0x1E */ SVec mAngle;
    /* 0x24 */ u32 mFlag;
    /* 0x28 */ u8 mShadowSize;
    /* 0x29 */ u8 mCollisionH;
    /* 0x2A */ u8 mCollisionR;
    /* 0x2B */ s8 mTevFrm;
    /* 0x2C */ s8 mBtpFrm;
};  // Size: 0x30

class daShopItem_c : public daItemBase_c {
public:
    csXyz* getRotateP();
    cXyz* getPosP();
    const char* getShopArcname();
    u16 getHeapSize();
    void CreateInit();
    void set_mtx();
    bool _execute();
    bool _draw();

    inline int _create();
    inline int _delete();
    
    virtual void setListStart();
    virtual void setShadow();
    virtual BOOL chkFlag(int);
    virtual s8 getTevFrm();
    virtual s8 getBtpFrm();
    virtual u8 getShadowSize();
    virtual u8 getCollisionH();
    virtual u8 getCollisionR();

    u8 getShopItemID() const { return mShopItemID; }
    s16 getAngleX() const { return mAngleX; }
    void setAngleX(s16 angle) { mAngleX = angle;}
    s16 getAngleY() const { return mAngleY; }
    void setAngleY(s16 angle) { mAngleY = angle;}

    static ResourceData const mData[23];
    static f32 const m_cullfar_max;

    enum {
        SHOP_ITEMNO_SOLD,
        SHOP_ITEMNO_OIL,
        SHOP_ITEMNO_RED,
        SHOP_ITEMNO_MILK,
        SHOP_ITEMNO_HALF_MILK,
        SHOP_ITEMNO_ARROW,
        SHOP_ITEMNO_SHIELD_B,
        SHOP_ITEMNO_SHIELD_A,
        SHOP_ITEMNO_BOMB,
        SHOP_ITEMNO_WATER_BOMB,
        SHOP_ITEMNO_POKE_BOMB,
        SHOP_ITEMNO_BOMB_BAG,
        SHOP_ITEMNO_BOMB_BAG_LV2,
        SHOP_ITEMNO_LAND_MINE,
        SHOP_ITEMNO_BOTTLE2,
        SHOP_ITEMNO_BEE_CHILD,
        SHOP_ITEMNO_PACHINKO,
        SHOP_ITEMNO_BLUE,
        SHOP_ITEMNO_HAWKEYE,
        SHOP_ITEMNO_ARMOR,
        SHOP_ITEMNO_ARMOR_SOLD,
        SHOP_ITEMNO_GREEN,
        SHOP_ITEMNO_DROP,
        SHOP_ITEMNO_MAX,
    };

private:
    /* 0x92C */ Mtx mMtx;
    /* 0x95C */ int field_0x95c;
    /* 0x960 */ s16 mAngleX;
    /* 0x962 */ s16 mAngleY;
    /* 0x964 */ u8 mShopItemID;
};

int CheckShopItemCreateHeap(fopAc_ac_c* i_this);

#endif /* D_A_D_A_SHOP_ITEM_STATIC_H */
