/**
 * d_menu_window_HIO.cpp
 * Menu HostIO functionality
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_window_HIO.h"
#include "d/d_com_inf_game.h"

dMw_HIO_c::dMw_HIO_c() {
    init();
}

void dMw_HIO_c::init() {
    static u8 m_cloth_item[3] = {fpcNm_ITEM_WEAR_KOKIRI, fpcNm_ITEM_ARMOR, fpcNm_ITEM_WEAR_ZORA};
    static u8 m_sword_item[4] = {fpcNm_ITEM_SWORD, fpcNm_ITEM_MASTER_SWORD, fpcNm_ITEM_LIGHT_SWORD, fpcNm_ITEM_WOOD_STICK};
    static u8 m_shield_item[3] = {fpcNm_ITEM_SHIELD, fpcNm_ITEM_HYLIA_SHIELD, fpcNm_ITEM_WOOD_SHIELD};

    mArrowFlag = 0;
    mBombFlag = 0;
    mPachinkoFlag = 0;

    mLifeCopy = 0;
    mLife = 0;
    mMaxLifeCopy = 0;
    mMaxLife = 0;
    mMagicCopy = 0;
    mMagic = 0;
    mMaxMagicCopy = 0;
    mMaxMagic = 0;
    mOilCopy = 0;
    mOil = 0;
    mMaxOilCopy = 0;
    mMaxOil = 0;
    mOxygenCopy = 0;
    mOxygen = 0;
    mMaxOxygenCopy = 0;
    mMaxOxygen = 0;
    mEnableMagicCopy = false;
    mEnableMagic = false;
    mEnableArrowDropsCopy = false;
    mEnableArrowDrops = false;
    mRupeesCopy = 0;
    mRupees = 0;
    mWalletSizeCopy = WALLET;
    mWalletSize = WALLET;
    mKeysCopy = 0;
    mKeys = 0;
    field_0x47 = 0;
    field_0x23 = 0;
    mEnableOxygenMeterCopy = false;
    mEnableOxygenMeter = false;
    mPoesCopy = 0;
    mPoes = 0;
    mDonationAmountCopy = 0;
    mDonationAmount = 0;

    for (int i = 0; i < 24; i++) {
        mItemsCopy[i] = fpcNm_ITEM_NONE;
        mItems[i] = fpcNm_ITEM_NONE;
        field_0x7a[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        mBottleNumCopy[i] = 0;
        mBottleNum[i] = 0;
    }

    for (int i = 0; i < 3; i++) {
        mBombNumCopy[i] = 0;
        mBombNum[i] = 0;
    }

    mScentCopy = fpcNm_ITEM_NONE;
    mScent = fpcNm_ITEM_NONE;
    field_0xa8 = 0;
    field_0xf4 = 0;
    field_0xea = 0;

    for (int i = 0; i < 4; i++) {
        bool swordBit = dComIfGs_isItemFirstBit(m_sword_item[i]);
        mSwordsCopy[i] = swordBit;
        mSwords[i] = swordBit;
    }

    for (int i = 0; i < 3; i++) {
        bool shieldBit = dComIfGs_isItemFirstBit(m_shield_item[i]);
        mShieldsCopy[i] = shieldBit;
        mShields[i] = shieldBit;

        bool clothesBit = dComIfGs_isItemFirstBit(m_cloth_item[i]);
        mClothesCopy[i] = clothesBit;
        mClothes[i] = clothesBit;
    }

    mBombBagLV2Copy = false;
    mBombBagLV2 = false;

    for (int i = 0; i < 3; i++) {
        mLV2KeyShardsCopy[i] = false;
        mLV2KeyShards[i] = false;
    }

    for (int i = 0; i < 2; i++) {
        mLV5ItemCopy[i] = false;
        mLV5Item[i] = false;
    }

    for (int i = 0; i < 24; i++) {
        mGoldBugCopy[i] = false;
        mGoldBug[i] = false;
    }

    for (int i = 0; i < 8; i++) {
        field_0x101[i] = 0;
        field_0xf9[i] = 0;
        field_0x111[i] = 0;
        field_0x109[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        mFusedShadowCopy[i] = false;
        mFusedShadow[i] = false;
    }

    for (int i = 0; i < 4; i++) {
        mMirrorShardCopy[i] = false;
        mMirrorShard[i] = false;
    }
}

bool dMw_HIO_c::getBombFlag() {
    return false;
}

bool dMw_HIO_c::getArrowFlag() {
    return false;
}

bool dMw_HIO_c::getPachinkoFlag() {
    return false;
}

dMw_DHIO_c::dMw_DHIO_c() {
    for (int i = 0; i < 32; i++) {
        mMap[i] = false;
        mCompass[i] = false;
        mBossKey[i] = false;
        mBossClear[i] = false;
        mHeartContainer[i] = false;
        mBossDemo[i] = false;
        mPortal[i] = false;
        mMidBossClear[i] = false;
        mMapCopy[i] = false;
        mCompassCopy[i] = false;
        mBossKeyCopy[i] = false;
        mBossClearCopy[i] = false;
        mHeartContainerCopy[i] = false;
        mBossDemoCopy[i] = false;
        mPortalCopy[i] = false;
        mMidBossClearCopy[i] = false;
    }
}

dMw_HIO_c g_mwHIO;

dMw_DHIO_c g_mwDHIO;
