#ifndef D_MENU_D_MENU_WINDOW_HIO_H
#define D_MENU_D_MENU_WINDOW_HIO_H

#include "JSystem/JHostIO/JORReflexible.h"

class dMw_DHIO_c : public JORReflexible {
public:
    dMw_DHIO_c();

    virtual ~dMw_DHIO_c() {}
    void genMessage(JORMContext*);

    void bitCheck();

    /* 0x004 */ s8 field_0x4;
    /* 0x005 */ u8 mMap[32];
    /* 0x025 */ u8 mCompass[32];
    /* 0x045 */ u8 mBossKey[32];
    /* 0x065 */ u8 mBossClear[32];
    /* 0x085 */ u8 mHeartContainer[32];
    /* 0x0A5 */ u8 mBossDemo[32];
    /* 0x0C5 */ u8 mPortal[32];
    /* 0x0E5 */ u8 mMidBossClear[32];
    /* 0x105 */ u8 mMapCopy[32];
    /* 0x125 */ u8 mCompassCopy[32];
    /* 0x145 */ u8 mBossKeyCopy[32];
    /* 0x165 */ u8 mBossClearCopy[32];
    /* 0x185 */ u8 mHeartContainerCopy[32];
    /* 0x1A5 */ u8 mBossDemoCopy[32];
    /* 0x1C5 */ u8 mPortalCopy[32];
    /* 0x1E5 */ u8 mMidBossClearCopy[32];
};

extern dMw_DHIO_c g_mwDHIO;

class dMw_HIO_c : public JORReflexible {
public:
    dMw_HIO_c();
    void init();
    u8 getBombFlag();
    u8 getArrowFlag();
    u8 getPachinkoFlag();
    void update();
    void playerStatusHIO();
    void itemBitCheck();
    void itemNumHIO();
    void itemHIO();

    void setArrowFlag(u8 i_flag) { mArrowFlag = i_flag; }
    void setPachinkoFlag(u8 i_flag) { mPachinkoFlag = i_flag; }
    void setBombFlag(u8 i_flag) { mBombFlag = i_flag; }

    virtual ~dMw_HIO_c() {}
    void genMessage(JORMContext*);

    /* 0x004 */ s8 field_0x4;
    /* 0x006 */ s16 mLife;
    /* 0x008 */ s16 mMaxLife;
    /* 0x00A */ u8 mMagic;
    /* 0x00B */ u8 mMaxMagic;
    /* 0x00C */ s32 mOil;
    /* 0x010 */ s32 mMaxOil;
    /* 0x014 */ s32 mOxygen;
    /* 0x018 */ s32 mMaxOxygen;
    /* 0x01C */ u8 mEnableMagic;
    /* 0x01D */ u8 mEnableArrowDrops;
    /* 0x01E */ s16 mRupees;
    /* 0x020 */ s16 mWalletSize;
    /* 0x022 */ u8 mKeys;
    /* 0x023 */ u8 field_0x23;
    /* 0x024 */ s16 mLifeCopy;
    /* 0x026 */ s16 mMaxLifeCopy;
    /* 0x028 */ u8 mMagicCopy;
    /* 0x029 */ u8 mMaxMagicCopy;
    /* 0x02C */ s32 mOilCopy;
    /* 0x030 */ s32 mMaxOilCopy;
    /* 0x034 */ s32 mOxygenCopy;
    /* 0x038 */ s32 mMaxOxygenCopy;
    /* 0x03C */ u8 mEnableMagicCopy;
    /* 0x03D */ u8 mEnableArrowDropsCopy;
    /* 0x03E */ s16 mRupeesCopy;
    /* 0x040 */ s16 mWalletSizeCopy;
    /* 0x042 */ s16 mDonationAmount;
    /* 0x044 */ s16 mDonationAmountCopy;
    /* 0x046 */ u8 mKeysCopy;
    /* 0x047 */ u8 field_0x47;
    /* 0x048 */ u8 mPoes;
    /* 0x049 */ u8 mPoesCopy;
    /* 0x04A */ u8 mItems[24];
    /* 0x062 */ u8 mItemsCopy[24];
    /* 0x07A */ u8 field_0x7a[24];
    /* 0x092 */ u8 mSwords[4];
    /* 0x096 */ u8 mShields[3];
    /* 0x099 */ u8 mClothes[3];
    /* 0x09C */ u8 mSwordsCopy[4];
    /* 0x0A0 */ u8 mShieldsCopy[3];
    /* 0x0A3 */ u8 mClothesCopy[3];
    /* 0x0A6 */ u8 mScent;
    /* 0x0A7 */ u8 mScentCopy;
    /* 0x0A8 */ u8 field_0xa8;
    /* 0x0A9 */ u8 mBombBagLV2;
    /* 0x0AA */ u8 mBombBagLV2Copy;
    /* 0x0AB */ u8 mLV2KeyShards[3];
    /* 0x0AE */ u8 mLV2KeyShardsCopy[3];
    /* 0x0B1 */ u8 mLV5Item[2];
    /* 0x0B3 */ u8 mLV5ItemCopy[2];
    /* 0x0B5 */ u8 mGoldBug[24];
    /* 0x0CD */ u8 mGoldBugCopy[24];
    /* 0x0E5 */ u8 mArrowNum;
    /* 0x0E6 */ u8 mSeedNum;
    /* 0x0E7 */ u8 mBombNum[3];
    /* 0x0EA */ u8 field_0xea;
    /* 0x0EB */ u8 mBottleNum[4];
    /* 0x0EF */ u8 mArrowNumCopy;
    /* 0x0F0 */ u8 mSeedNumCopy;
    /* 0x0F1 */ u8 mBombNumCopy[3];
    /* 0x0F4 */ u8 field_0xf4;
    /* 0x0F5 */ u8 mBottleNumCopy[4];
    /* 0x0F9 */ u8 field_0xf9[8];
    /* 0x101 */ u8 field_0x101[8];
    /* 0x109 */ u8 field_0x109[8];
    /* 0x111 */ u8 field_0x111[8];
    /* 0x119 */ u8 mEnableOxygenMeter;
    /* 0x11A */ u8 mEnableOxygenMeterCopy;
    /* 0x11B */ u8 mArrowFlag;
    /* 0x11C */ u8 mPachinkoFlag;
    /* 0x11D */ u8 mBombFlag;
    /* 0x11E */ u8 mFusedShadow[4];
    /* 0x122 */ u8 mFusedShadowCopy[4];
    /* 0x126 */ u8 mMirrorShard[4];
    /* 0x12A */ u8 mMirrorShardCopy[4];
};  // Size: 0x130

extern dMw_HIO_c g_mwHIO;

#endif /* D_MENU_D_MENU_WINDOW_HIO_H */
