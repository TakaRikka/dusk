#ifndef D_SAVE_D_SAVE_H
#define D_SAVE_D_SAVE_H

#include <string>
#include "SSystem/SComponent/c_xyz.h"
#include <dolphin/os.h>
#include "global.h"
#include "f_pc/f_pc_name.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JHostIO/JORReflexible.h"


static const int DEFAULT_SELECT_ITEM_INDEX = 0;
static const int MAX_SELECT_ITEM = 4;
static const int SELECT_ITEM_NUM = 2;
static const int MAX_EQUIPMENT = 6;
static const int MAX_EVENTS = 256;
static const int MAX_ITEM_SLOTS = 24;
static const int LIGHT_DROP_STAGE = 4;
static const int LETTER_INFO_BIT = 64;
static const int MAX_INSECT_NUM = 24;
static const int MAX_VISIBLE_HEARTPIECES = 4;
static const int MAX_POH_NUM = 100;
static const int TBOX_MAX = 64;
static const int DSV_MEMBIT_ENUM_MAX = 8;
static const int ITEM_MAX_DAN = 128;
static const int SWITCH_ZONE_MAX = 0x20;
static const int SWITCH_ONE_ZONE_MAX = 0x10;
static const int ITEM_ZONE_MAX = 0x20;
static const int ITEM_ONE_ZONE_MAX = 0x10;
static const int QUEST_LOG_SIZE = 0xA94;
static const int QUIVER_MAX = 30;
static const int BIG_QUIVER_MAX = 60;
static const int GIANT_QUIVER_MAX = 100;
static const int WALLET_MAX = 300;
static const int BIG_WALLET_MAX = 600;
static const int GIANT_WALLET_MAX = 1000;
static const int MAX_FINDABLE_FISHES = 6;

static const int ITEM_BIT_MAX = 0x100;
static const int ITEM_XY_MAX_DUMMY = 8;

enum ButtonIndexes {
    /* 0 */ A_BUTTON,
    /* 1 */ B_BUTTON,
};

enum Wallets {
    /* 0x0 */ WALLET,
    /* 0x1 */ BIG_WALLET,
    /* 0x2 */ GIANT_WALLET
};

enum ItemSlots {
    /* 0x00 */ SLOT_0,
    /* 0x01 */ SLOT_1,
    /* 0x02 */ SLOT_2,
    /* 0x03 */ SLOT_3,
    /* 0x04 */ SLOT_4, // Bow Slot
    /* 0x05 */ SLOT_5,
    /* 0x06 */ SLOT_6,
    /* 0x07 */ SLOT_7,
    /* 0x08 */ SLOT_8,
    /* 0x09 */ SLOT_9,
    /* 0x0A */ SLOT_10,
    /* 0x0B */ SLOT_11,
    /* 0x0C */ SLOT_12,
    /* 0x0D */ SLOT_13,
    /* 0x0E */ SLOT_14,
    /* 0x0F */ SLOT_15,
    /* 0x10 */ SLOT_16,
    /* 0x11 */ SLOT_17,
    /* 0x12 */ SLOT_18,
    /* 0x13 */ SLOT_19,
    /* 0x14 */ SLOT_20,
    /* 0x15 */ SLOT_21,
    /* 0x16 */ SLOT_22,
    /* 0x17 */ SLOT_23
};

enum CollectItem {
    /* 0x0 */ COLLECT_CLOTHING,
    /* 0x1 */ COLLECT_SWORD,
    /* 0x2 */ COLLECT_SHIELD,
    /* 0x3 */ COLLECT_SMELL,
    /* 0x4 */ B_BUTTON_ITEM
};

enum Swords {
    /* 0x0 */ COLLECT_ORDON_SWORD,
    /* 0x1 */ COLLECT_MASTER_SWORD,
    /* 0x2 */ COLLECT_WOODEN_SWORD,
    /* 0x3 */ COLLECT_LIGHT_SWORD
};

enum Shields {
    /* 0x0 */ COLLECT_WOODEN_SHIELD,
    /* 0x1 */ COLLECT_ORDON_SHIELD,
    /* 0x2 */ COLLECT_HYLIAN_SHIELD
};

enum Clothes {
    /* 0x0 */ KOKIRI_CLOTHES_FLAG
};

enum LightDropStages {
    /* 0x0 */ FARON_VESSEL,
    /* 0x1 */ ELDIN_VESSEL,
    /* 0x2 */ LANAYRU_VESSEL
};

enum ItemMax {
    /* 0x0 */ ARROW_MAX,
    /* 0x1 */ NORMAL_BOMB_MAX,
    /* 0x2 */ WATER_BOMB_MAX,
    /* 0x6 */ POKE_BOMB_MAX = 6
};

enum {
    /* 0x0 */ SELECT_ITEM_LEFT,
    /* 0x1 */ SELECT_ITEM_RIGHT,
    /* 0x2 */ SELECT_ITEM_DOWN,
    /* 0x3 */ SELECT_ITEM_B,
    /* 0x0 */ SELECT_ITEM_X = SELECT_ITEM_LEFT,
    /* 0x1 */ SELECT_ITEM_Y = SELECT_ITEM_RIGHT,
};

enum {
    /* 0x0 */ TF_STATUS_HUMAN,
    /* 0x1 */ TF_STATUS_WOLF,
};

class dSv_player_status_a_c {
public:
    void init();
    void setSelectItemIndex(int i_no, u8 i_slotNo);
    u8 getSelectItemIndex(int i_no) const;
    void setMixItemIndex(int i_no, u8 i_slotNo);
    u8 getMixItemIndex(int i_no) const;
    u16 getRupeeMax() const;
    void onMagicFlag(u8 i_magic);
    void offMagicFlag(u8 i_magic);
    int isMagicFlag(u8 i_magic) const;

    u16 getMaxLife() const { return mMaxLife; }
    u16 getLife() const { return mLife; }
    u16 getRupee() const { return mRupee; }
    u16 getOil() const { return mOil; }
    u16 getMaxOil() const { return mMaxOil; }
    u8 getMagic() const { return mMagic; }
    u8 getMaxMagic() const { return mMaxMagic; }
    u8 getSelectEquip(int item) const { return mSelectEquip[item]; }
    u8 getTransformStatus() const { return mTransformStatus; }
    u8 getWalletSize() const { return mWalletSize; }
    void setOil(u16 i_oil) { mOil = i_oil; }
    void setMaxOil(u16 i_maxOil) { mMaxOil = i_maxOil; }
    void setWalletSize(u8 i_size) { mWalletSize = i_size; }
    void setMagic(u8 i_magic) { mMagic = i_magic; }
    void setMaxMagic(u8 i_maxMagic) { mMaxMagic = i_maxMagic; }
    void setRupee(u16 i_rupees) { mRupee = i_rupees; }
    void setLife(u16 i_life) { mLife = i_life; }
    void setMaxLife(u8 i_maxLife) { mMaxLife = i_maxLife; }
    void setSelectEquip(int i_equipId, u8 i_itemNo) { mSelectEquip[i_equipId] = i_itemNo; }
    void setTransformStatus(u8 i_status) { mTransformStatus = i_status; }

private:
    /* 0x00 */ u16 mMaxLife;
    /* 0x02 */ u16 mLife;
    /* 0x04 */ u16 mRupee;
    /* 0x06 */ u16 mMaxOil;
    /* 0x08 */ u16 mOil;
    /* 0x0A */ u8 unk10;
    /* 0x0B */ u8 mSelectItem[MAX_SELECT_ITEM];  // For GC: first 2 are X & Y, others unused; For
                                                 // Wii (in order): Left, Right, Down, B
    /* 0x0F */ u8 mMixItem[MAX_SELECT_ITEM];     // Combo items; For GC: first 2 are X & Y, others
                                                 // unused; For Wii (in order): Left, Right, Down, B
    /* 0x13 */ u8 mSelectEquip[MAX_EQUIPMENT];
    /* 0x19 */ u8 mWalletSize;
    /* 0x1A */ u8 mMaxMagic;
    /* 0x1B */ u8 mMagic;
    /* 0x1C */ u8 mMagicFlag;
    /* 0x1D */ u8 unk29;
    /* 0x1E */ u8 mTransformStatus;
    /* 0x1F */ u8 unk31[3];
};  // Size: 0x28

class dSv_player_status_b_c {
public:
    void init();
    void onDarkClearLV(int i_no);
    void offDarkClearLV(int i_no);
    BOOL isDarkClearLV(int i_no) const;
    void onTransformLV(int i_no);
    void offTransformLV(int i_no);
    BOOL isTransformLV(int i_no) const;

    void setDateIpl(s64 i_time) { mDateIpl = i_time; }
    u16 getDate() const { return mDate; }
    f32 getTime() const { return mTime; }
    void setDate(u16 i_date) { mDate = i_date; }
    void setTime(f32 i_time) { mTime = i_time; }
    OSTime getDateIpl() const { return mDateIpl; }

private:
    /* 0x00 */ OSTime mDateIpl;
    /* 0x08 */ u8 mTransformLevelFlag;
    /* 0x09 */ u8 mDarkClearLevelFlag;
    /* 0x0A */ u8 unk10;
    /* 0x0B */ u8 unk11;
    /* 0x0C */ f32 mTime;
    /* 0x10 */ u16 mDate;
    /* 0x12 */ u8 unk18[3];
};  // Size: 0x18

class dSv_horse_place_c {
public:
    dSv_horse_place_c() {}

    void init();
    void set(const char* i_name, const cXyz& i_pos, s16 i_angle, s8 i_roomNo);

    cXyz& getPos() { return mPos; }
    s16 getAngleY() const { return mAngleY; }
    const char* getStageName() const { return mName; }
    s8 getRoomNo() const { return mRoomNo; }

private:
    /* 0x00 */ cXyz mPos;
    /* 0x0C */ s16 mAngleY;
    /* 0x0E */ char mName[8];
    /* 0x16 */ u8 mSpawnId;
    /* 0x17 */ s8 mRoomNo;
};  // Size: 0x18

class dSv_player_return_place_c {
public:
    void init();
    void set(const char* i_name, s8 i_roomNo, u8 i_status);
    s8 getRoomNo() const { return mRoomNo; }
    u8 getPlayerStatus() const { return mPlayerStatus; }
    char* getName() { return mName; }

private:
    /* 0x00 */ char mName[8];
    /* 0x08 */ u8 mPlayerStatus;
    /* 0x09 */ s8 mRoomNo;
    /* 0x0A */ u8 unk10;
    /* 0x0B */ u8 unk11;
};  // Size: 0xC

class dSv_player_field_last_stay_info_c {
public:
    dSv_player_field_last_stay_info_c() {}

    void init();
    BOOL isRegionBit(int i_region) const;
    void onRegionBit(int i_region);
    void set(const char* i_name, const cXyz& i_pos, s16 i_angle, s8 i_spawn, u8 i_regionNo);

    bool isFieldDataExistFlag() const { return mFieldDataExistFlag; }
    void offFieldDataExistFlag() { mFieldDataExistFlag = false; }
    void onFieldDataExistFlag() { mFieldDataExistFlag = true; }
    cXyz& getPos() { return mPos; }
    u8 getRegionNo() const { return mRegionNo; }
    char* getName() { return mName; }
    s16 getAngleY() { return mAngleY; }

private:
    /* 0x00 */ cXyz mPos;
    /* 0x0C */ s16 mAngleY;
    /* 0x0E */ char mName[8];
    /* 0x16 */ s8 mLastSpawnId;
    /* 0x17 */ u8 mRegionNo;
    /* 0x18 */ bool mFieldDataExistFlag;
    /* 0x19 */ u8 mRegion;
    /* 0x1A */ u8 unk26[2];
};  // Size: 0x1C

class dSv_player_last_mark_info_c {
public:
    dSv_player_last_mark_info_c() {}

    void init();
    void setWarpItemData(const char* i_name, const cXyz& i_pos, s16 i_angle, s8 i_roomNo, u8, u8);

    const char* getName() { return mName; }
    cXyz& getPos() { return mPos; }
    s16 getAngleY() { return mAngleY; }
    s8 getRoomNo() { return mRoomNo; }
    s8 getWarpAcceptStage() { return mWarpAcceptStage; }
    void resetWarpAcceptStage() { mWarpAcceptStage = -1; }
    void setWarpAcceptStage(s8 accept) { mWarpAcceptStage = accept; }

private:
    /* 0x00 */ cXyz mPos;
    /* 0x0C */ s16 mAngleY;
    /* 0x0E */ char mName[8];
    /* 0x16 */ u8 mSpawnId;
    /* 0x17 */ s8 mRoomNo;
    /* 0x18 */ s8 mWarpAcceptStage;
    /* 0x19 */ u8 unk25[3];
};  // Size: 0x1C

class dSv_player_item_c {
public:
    void init();
    void setItem(int i_slotNo, u8 i_itemNo);
    u8 getItem(int i_slotNo, bool i_checkCombo) const;
    void setLineUpItem();
    u8 getLineUpItem(int i_slotNo) const;
    void setBottleItemIn(u8 curItemIn, u8 newItemIn);
    void setEmptyBottleItemIn(u8 i_itemNo);
    void setEmptyBottle();
    void setEmptyBottle(u8 i_itemNo);
    void setEquipBottleItemIn(u8 curItemIn, u8 newItemIn);
    void setEquipBottleItemEmpty(u8 curItemIn);
    u8 checkBottle(u8 i_itemNo);
    int checkInsectBottle();
    u8 checkEmptyBottle();
    void setBombBagItemIn(u8 i_curBomb, u8 i_newBomb, bool i_setNum);
    void setBombBagItemIn(u8 i_curBomb, u8 i_newBomb, u8 i_bombNum, bool i_setNum);
    void setEmptyBombBagItemIn(u8 i_newBomb, bool i_setNum);
    void setEmptyBombBagItemIn(u8 i_newBomb, u8 i_bombNum, bool i_setNum);
    void setEmptyBombBag();
    void setEmptyBombBag(u8 i_newBomb, u8 i_bombNum);
    u8 checkBombBag(u8 i_itemNo);
    void setWarashibeItem(u8 i_itemNo);
    void setRodTypeLevelUp();
    void setBaitItem(u8 i_itemNo);

    static const int BOMB_BAG_MAX = 3;
    static const int BOTTLE_MAX = 4;

private:
    /* 0x00 */ u8 mItems[24];
    /* 0x18 */ u8 mItemSlots[24];
};  // Size: 0x30

class dSv_player_get_item_c {
public:
    void init();
    void onFirstBit(u8 i_itemNo);
    void offFirstBit(u8 i_itemNo);
    int isFirstBit(u8 i_itemNo) const;

private:
    /* 0x0 */ u32 mItemFlags[8];
};  // Size: 0x20

class dSv_player_item_record_c {
public:
    void init();
    void setBombNum(u8 i_bagIdx, u8 i_bombNum);
    u8 getBombNum(u8 i_bagIdx) const;
    void setBottleNum(u8 i_bottleIdx, u8 i_bottleNum);
    u8 addBottleNum(u8 i_bottleIdx, s16 i_num);
    u8 getBottleNum(u8 i_bottleIdx) const;

    u8 getArrowNum() const { return mArrowNum; }
    void setArrowNum(u8 i_num) { mArrowNum = i_num; }
    u8 getPachinkoNum() const { return mPachinkoNum; }
    void setPachinkoNum(u8 i_num) { mPachinkoNum = i_num; }

private:
    /* 0x0 */ u8 mArrowNum;
    /* 0x1 */ u8 mBombNum[3];
    /* 0x4 */ u8 mBottleNum[4];
    /* 0x8 */ u8 mPachinkoNum;
    /* 0x9 */ u8 unk5[3];
};  // Size: 0xC

class dSv_player_item_max_c {
public:
    void init();
    void setBombNum(u8 i_bombType, u8 i_maxNum);
    u8 getBombNum(u8 i_bombType) const;

    void setArrowNum(u8 i_maxNum) { mItemMax[ARROW_MAX] = i_maxNum; }
    u8 getArrowNum() const { return mItemMax[ARROW_MAX]; }

private:
    /* 0x0 */ u8 mItemMax[8];
};  // Size: 0x8

class dSv_player_collect_c {
public:
    void init();
    void setCollect(int i_item_type, u8 i_item);
    void offCollect(int i_item_type, u8 i_item);
    BOOL isCollect(int i_item_type, u8 i_item) const;
    void onCollectCrystal(u8 i_item);
    void offCollectCrystal(u8 i_item);
    BOOL isCollectCrystal(u8 i_item) const;
    void onCollectMirror(u8 i_item);
    void offCollectMirror(u8 i_item);
    BOOL isCollectMirror(u8 i_item) const;

    u8 getPohNum() const { return mPohNum; }
    void addPohNum() {
        if (mPohNum < 0xFF) {
            mPohNum += 1;
        }
    };

private:
    /* 0x0 */ u8 mItem[8];
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 mCrystal;
    /* 0xA */ u8 mMirror;
    /* 0xB */ u8 unk11;
    /* 0xC */ u8 mPohNum;
    /* 0xD */ u8 padding[3];
};  // Size: 0x10

class dSv_player_wolf_c {
public:
    void init();

private:
    /* 0x0 */ u8 unk0[3];
    /* 0x3 */ u8 unk3;
};  // Size: 0x4

class dSv_light_drop_c {
public:
    void init();
    void setLightDropNum(u8 i_nowLevel, u8 i_dropNum);
    u8 getLightDropNum(u8 i_nowLevel) const;
    void onLightDropGetFlag(u8 i_nowLevel);
    void offLightDropGetFlag(u8 i_nowLevel);
    BOOL isLightDropGetFlag(u8 i_nowLevel) const;

private:
    /* 0x0 */ u8 mLightDropNum[4];
    /* 0x4 */ u8 mLightDropGetFlag;
    /* 0x5 */ u8 unk5[3];
};  // Size: 0x8

class dSv_letter_info_c {
public:
    void init();
    void onLetterGetFlag(int i_no);
    BOOL isLetterGetFlag(int i_no) const;
    void onLetterReadFlag(int i_no);
    int isLetterReadFlag(int i_no) const;
    u8 getGetNumber(int i_no) { return mGetNumber[i_no]; }
    void setGetNumber(int i_no, u8 i_value) { mGetNumber[i_no] = i_value; }

private:
    /* 0x00 */ u32 mLetterGetFlags[2];
    /* 0x08 */ u32 mLetterReadFlags[2];
    /* 0x10 */ u8 mGetNumber[64];
};  // Size: 0x50

class dSv_fishing_info_c {
public:
    void init();
    void addFishCount(u8 i_fishIndex);
    u16 getFishCount(u8 i_fishIndex) const { return mFishCount[i_fishIndex]; }
    u8 getMaxSize(int i_sizeIndex) const { return mMaxSize[i_sizeIndex]; }
    void setMaxSize(int i_sizeIndex, u8 i_size) { mMaxSize[i_sizeIndex] = i_size; }

private:
    /* 0x00 */ u16 mFishCount[16];
    /* 0x20 */ u8 mMaxSize[16];
};  // Size: 0x34

class dSv_player_info_c {
public:
    void init();
    char* getLinkName() { return mPlayerName; }
    const char* getPlayerName() const { return mPlayerName; }
    char* getHorseName() { return mHorseName; }
    void setPlayerName(const char* i_name) { strcpy((char*)mPlayerName, i_name); }
    void setHorseName(const char* i_name) { strcpy((char*)mHorseName, i_name); }
    void setTotalTime(s64 i_time) { mTotalTime = i_time; }
    s64 getTotalTime() const { return mTotalTime; }
    u8 getClearCount() const { return mClearCount; }

    void addDeathCount() {
        if (mDeathCount < 0xFFFF) {
            mDeathCount++;
        }
    }

private:
    /* 0x00 */ u64 unk0;
    /* 0x08 */ s64 mTotalTime;
    /* 0x10 */ u16 unk16;
    /* 0x12 */ u16 mDeathCount;
    /* 0x14 */ char mPlayerName[16];
    /* 0x24 */ u8 unk36;
    /* 0x25 */ char mHorseName[16];
    /* 0x35 */ u8 unk53;
    /* 0x36 */ u8 mClearCount;
    /* 0x37 */ u8 unk55[5];
};  // Size: 0x40

class dSv_player_config_c {
public:
    void init();
    u32 checkVibration() const;
    u8 getSound();
    void setSound(u8 i_mode);
    u8 getVibration();
    void setVibration(u8 i_status);
    u8 getPalLanguage() const;

    // Ruby inline names are from TWW debug.
    u8 getRuby() { return mRuby; }
    void setRuby(u8 i_ruby) { mRuby = i_ruby; }
    u8 getAttentionType() { return mAttentionType; }
    void setAttentionType(u8 i_mAttentionType) { mAttentionType = i_mAttentionType; }
    u16 getCalibrateDist() { return mCalibrateDist; }
    void setCalibrateDist(u16 i_mCalibrateDist) { mCalibrateDist = i_mCalibrateDist; }
    u8 getCalValue() { return mCalValue; }
    void setCalValue(u8 i_mCalValue) { mCalValue = i_mCalValue; }
    bool getShortCut() { return mShortCut; }
    void setShortCut(bool i_mShortCut) { mShortCut = i_mShortCut; }
    u8 getCameraControl() { return mCameraControl; }
    void setCameraControl(u8 i_mCameraControl) { mCameraControl = i_mCameraControl; }
    bool getPointer() { return mPointer; }
    void setPointer(bool i_mPointer) { mPointer = i_mPointer; }

    enum dSv_config_language {
        LANGUAGE_ENGLISH,
        LANGUAGE_GERMAN,
        LANGUAGE_FRENCH,
        LANGUAGE_SPANISH,
        LANGUAGE_ITALIAN,
    };

private:
    /* 0x0 */ u8 mRuby;
    /* 0x1 */ u8 mSoundMode;
    /* 0x2 */ u8 mAttentionType;  // Lock-On Type; 0 : hold, 1 : switch
    /* 0x3 */ u8 mVibration;      // Rumble status
    /* 0x4 */ u8 mLanguage;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u16 mCalibrateDist;  // Wii pointer horizontal calibration. Default is 0x015E
    /* 0x8 */ u8 mCalValue;        // Wii pointer vertical calibration. Default is 0x00
    /* 0x9 */ bool mShortCut;      // Wii icon shortcut enabled/disabled.
    /* 0xA */ u8 mCameraControl;   // 0 : normal, 1 : inverted
    /* 0xB */ bool mPointer;       // Wii pointer enabled/disabled.
};                                 // Size: 0xC

class dSv_player_c {
public:
    dSv_player_c() {}
    void init();

    dSv_player_info_c& getPlayerInfo() { return mPlayerInfo; }
    dSv_player_status_a_c& getPlayerStatusA() { return mPlayerStatusA; }
    dSv_player_status_b_c& getPlayerStatusB() { return mPlayerStatusB; }
    dSv_horse_place_c& getHorsePlace() { return mHorsePlace; }
    dSv_player_item_c& getItem() { return mItem; }
    dSv_player_collect_c& getCollect() { return mCollect; }
    dSv_player_item_record_c& getItemRecord() { return mItemRecord; }
    dSv_player_item_max_c& getItemMax() { return mItemMax; }
    dSv_player_last_mark_info_c& getPlayerLastMarkInfo() { return mPlayerLastMarkInfo; }
    dSv_player_return_place_c& getPlayerReturnPlace() { return mPlayerReturnPlace; }
    dSv_light_drop_c& getLightDrop() { return mLightDrop; }
    dSv_player_get_item_c& getGetItem() { return mGetItem; }
    dSv_player_config_c& getConfig() { return mConfig; }
    dSv_letter_info_c& getLetterInfo() { return mLetterInfo; }
    dSv_fishing_info_c& getFishingInfo() { return mFishingInfo; }
    dSv_player_field_last_stay_info_c& getPlayerFieldLastStayInfo() {
        return mPlayerFieldLastStayInfo;
    }

private:
    /* 0x000 */ dSv_player_status_a_c mPlayerStatusA;
    /* 0x028 */ dSv_player_status_b_c mPlayerStatusB;
    /* 0x040 */ dSv_horse_place_c mHorsePlace;
    /* 0x058 */ dSv_player_return_place_c mPlayerReturnPlace;
    /* 0x064 */ dSv_player_field_last_stay_info_c mPlayerFieldLastStayInfo;
    /* 0x080 */ dSv_player_last_mark_info_c mPlayerLastMarkInfo;
    /* 0x09C */ dSv_player_item_c mItem;
    /* 0x0CC */ dSv_player_get_item_c mGetItem;
    /* 0x0EC */ dSv_player_item_record_c mItemRecord;
    /* 0x0F8 */ dSv_player_item_max_c mItemMax;
    /* 0x100 */ dSv_player_collect_c mCollect;
    /* 0x110 */ dSv_player_wolf_c mWolf;
    /* 0x114 */ dSv_light_drop_c mLightDrop;
    /* 0x11C */ dSv_letter_info_c mLetterInfo;
    /* 0x16C */ dSv_fishing_info_c mFishingInfo;
    /* 0x1A0 */ dSv_player_info_c mPlayerInfo;
    /* 0x1E0 */ dSv_player_config_c mConfig;
};  // Size: 0x1EC

class dSv_memBit_c {
public:
    class WarpItemData_c {
    public:
        /* 0x04F94 */ char mWarpItemStage[8];
        /* 0x04F9C */ cXyz mWarpItemPos;
        /* 0x04FA8 */ s16 mWarpItemAngle;
        /* 0x04FAA */ s8 mWarpItemRoom;
        /* 0x04FAB */ u8 field_0x4fab;  // related to setWarpItemData
        /* 0x04FAC */ u8 field_0x4fac;  // related to setWarpItemData
        /* 0x04FAD */ u8 field_0x4fad[3];
    };

    enum {
        /* 0x0 */ MAP,
        /* 0x1 */ COMPASS,
        /* 0x2 */ BOSS_KEY,
        /* 0x3 */ STAGE_BOSS_ENEMY,
        /* 0x4 */ STAGE_LIFE,
        /* 0x5 */ STAGE_BOSS_DEMO,
        /* 0x6 */ OOCCOO_NOTE,
        /* 0x7 */ STAGE_BOSS_ENEMY_2
    };

    void init();
    void onTbox(int i_no);
    void offTbox(int i_no);
    BOOL isTbox(int i_no) const;
    void onSwitch(int i_no);
    void offSwitch(int i_no);
    BOOL isSwitch(int i_no) const;
    BOOL revSwitch(int i_no);
    void onItem(int i_no);
    void offItem(int i_no);
    BOOL isItem(int i_no) const;
    void onDungeonItem(int i_no);
    void offDungeonItem(int i_no);
    s32 isDungeonItem(int i_no) const;

    u8 getKeyNum() { return mKeyNum; }
    void setKeyNum(u8 i_keyNum) { mKeyNum = i_keyNum; }
    void onDungeonItemMap() { onDungeonItem(MAP); }
    s32 isDungeonItemMap() const { return isDungeonItem(MAP); }
    void onDungeonItemCompass() { onDungeonItem(COMPASS); }
    s32 isDungeonItemCompass() const { return isDungeonItem(COMPASS); }
    void onDungeonItemWarp() { onDungeonItem(OOCCOO_NOTE); }
    void onDungeonItemBossKey() { onDungeonItem(BOSS_KEY); }
    s32 isDungeonItemBossKey() const { return isDungeonItem(BOSS_KEY); }
    void onStageBossEnemy() {
        onDungeonItem(STAGE_BOSS_ENEMY);
        onDungeonItem(OOCCOO_NOTE);
    }
    void onStageBossEnemy2() { onDungeonItem(STAGE_BOSS_ENEMY_2); }
    s32 isStageBossEnemy() const { return isDungeonItem(STAGE_BOSS_ENEMY); }
    s32 isStageBossEnemy2() const { return isDungeonItem(STAGE_BOSS_ENEMY_2); }
    s32 isStageLife() const { return isDungeonItem(STAGE_LIFE); }
    s32 isDungeonItemWarp() const { return isDungeonItem(OOCCOO_NOTE); }
    void onStageLife() { onDungeonItem(STAGE_LIFE); }

private:
    /* 0x00 */ u32 mTbox[2];
    /* 0x08 */ u32 mSwitch[4];
    /* 0x18 */ u32 mItem[1];
    /* 0x1C */ u8 mKeyNum;
    /* 0x1D */ u8 mDungeonItem;
};  // Size: 0x20

class dSv_event_c {
public:
    void init();
    void onEventBit(const u16 i_no);
    void offEventBit(const u16 i_no);
    int isEventBit(const u16 i_no) const;
    void setEventReg(u16 i_reg, u8 i_no);
    u8 getEventReg(u16 i_reg) const;

    void* getPEventBit() { return (void*)mEvent; }

private:
    /* 0x0 */ u8 mEvent[256];
};  // Size: 0x100

class dSv_MiniGame_c {
public:
    void init();

    void setBalloonScore(u32 i_score) { mBalloonScore = i_score; }
    void setRaceGameTime(u32 i_time) { mRaceGameTime = i_time; }
    void setHookGameTime(u32 i_time) { mHookGameTime = i_time; }

    u32 getRaceGameTime() const { return mRaceGameTime; }
    u32 getBalloonScore() const { return mBalloonScore; }
    u32 getHookGameTime() const { return mHookGameTime; }

private:
    /* 0x00 */ u8 unk0[1][4];
    /* 0x04 */ u32 mHookGameTime;
    /* 0x08 */ u32 mBalloonScore;
    /* 0x0C */ u32 mRaceGameTime;
    /* 0x10 */ u32 unk16;
    /* 0x14 */ u32 unk20;
};  // Size: 0x18

class dSv_memory_c {
public:
    dSv_memory_c() { this->init(); }
    void init();
    dSv_memBit_c& getBit() { return mBit; }
    const dSv_memBit_c& getBit() const { return mBit; }

private:
    /* 0x0 */ dSv_memBit_c mBit;
};  // Size: 0x20

STATIC_ASSERT(sizeof(dSv_memory_c) == 0x20);

class dSv_memory2_c {
public:
    dSv_memory2_c() { this->init(); }
    void init();
    void onVisitedRoom(int i_no);
    void offVisitedRoom(int i_no);
    BOOL isVisitedRoom(int i_no);

private:
    /* 0x0 */ u32 mVisitedRoom[2];
};  // Size: 0x8

STATIC_ASSERT(sizeof(dSv_memory2_c) == 8);

class dSv_danBit_c {
public:
    bool init(s8 i_stageNo);
    void onSwitch(int i_no);
    void offSwitch(int i_no);
    BOOL isSwitch(int i_no) const;
    BOOL revSwitch(int i_no);
    void onItem(int i_no);
    void offItem(int i_no);
    BOOL isItem(int i_no) const;

    void reset() { mStageNo = -1; }

private:
    /* 0x00 */ s8 mStageNo;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2[2];
    /* 0x04 */ u32 mSwitch[2];
    /* 0x0C */ u32 mItem[4];
    /* 0x1C */ s16 unk28[16];
};  // Size: 0x3C

class dSv_zoneBit_c {
public:
    void init();
    void clearRoomSwitch();
    void clearRoomItem();
    void onSwitch(int i_no);
    void offSwitch(int i_no);
    BOOL isSwitch(int i_no) const;
    BOOL revSwitch(int i_no);
    void onOneSwitch(int i_no);
    void offOneSwitch(int i_no);
    BOOL isOneSwitch(int i_no) const;
    BOOL revOneSwitch(int i_no);
    void onItem(int i_no);
    void offItem(int i_no);
    BOOL isItem(int i_no) const;
    void onOneItem(int i_no);
    void offOneItem(int i_no);
    BOOL isOneItem(int i_no) const;

private:
    /* 0x00 */ u16 mSwitch[2];
    /* 0x04 */ u16 mRoomSwitch;
    /* 0x06 */ u16 mItem[2];
    /* 0x0A */ u16 mRoomItem;
    /* 0x0C */ u16 unk12;
};  // Size: 0xE

class dSv_zoneActor_c {
public:
    void init();
    void on(int i_id);
    void off(int i_id);
    BOOL is(int i_id) const;

    static const int ACTOR_MAX = 0x80;

private:
    /* 0x00 */ u32 mActorFlags[4];
};  // Size: 0x10

class dSv_zone_c {
public:
    dSv_zone_c() { mRoomNo = -1; }
    void init(int i_roomNo);

    dSv_zoneBit_c& getBit() { return mBit; }
    const dSv_zoneBit_c& getBit() const { return mBit; }
    dSv_zoneActor_c& getActor() { return mActor; }
    const dSv_zoneActor_c& getActor() const { return mActor; }

    int getRoomNo() const { return mRoomNo; }
    void reset() { mRoomNo = -1; }

private:
    /* 0x00 */ s8 mRoomNo;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ dSv_zoneBit_c mBit;
    /* 0x10 */ dSv_zoneActor_c mActor;
};  // Size: 0x20

STATIC_ASSERT(sizeof(dSv_zone_c) == 0x20);

class dSv_restart_c {
public:
    void setRoom(const cXyz& i_position, s16 i_angleY, s8 i_roomNo);

    void setRoomParam(u32 param) { mRoomParam = param; }
    void setStartPoint(s16 point) { mStartPoint = point; }
    void setLastSceneInfo(f32 speed, u32 mode, s16 angle) {
        mLastSpeedF = speed;
        mLastMode = mode;
        mLastAngleY = angle;
    }

    s16 getStartPoint() const { return mStartPoint; }
    u32 getLastMode() const { return mLastMode; }
    s8 getRoomNo() const { return mRoomNo; }
    u32 getRoomParam() const { return mRoomParam; }
    const cXyz& getRoomPos() const { return mRoomPos; }
    s16 getRoomAngleY() const { return mRoomAngleY; }
    s16 getLastAngleY() const { return mLastAngleY; }
    f32 getLastSpeedF() const { return mLastSpeedF; }

private:
    /* 0x00 */ s8 mRoomNo;
    /* 0x01 */ u8 field_0x01[3];
    /* 0x04 */ s16 mStartPoint;
    /* 0x06 */ s16 mRoomAngleY;
    /* 0x08 */ cXyz mRoomPos;
    /* 0x14 */ u32 mRoomParam;
    /* 0x18 */ f32 mLastSpeedF;
    /* 0x1C */ u32 mLastMode;
    /* 0x20 */ s16 mLastAngleY;
};  // Size: 0x24

class dSv_turnRestart_c {
public:
    class dSv_turnRestart_camera_c {
    public:
        /* 0x00 */ cXyz mCameraCtr;
        /* 0x0C */ cXyz mCameraEye;
        /* 0x18 */ cXyz mCameraUp;
    };  // Size: 0x24

    void set(const cXyz& i_position, s16 i_angleY, s8, u32 i_param);

    u32 getParam() const { return mParam; }
    const cXyz& getPos() const { return mPosition; }
    s16 getAngleY() const { return mAngleY; }
    s8 getRoomNo() const { return unk18; }

    f32 getCameraFvy() const { return mCameraFvy; }
    void setCameraFvy(f32 i_fvy) { mCameraFvy = i_fvy; }

    const cXyz& getCameraUp() const { return mCamera.mCameraUp; }
    const cXyz& getCameraEye() const { return mCamera.mCameraEye; }
    const cXyz& getCameraCtr() const { return mCamera.mCameraCtr; }

    void setCameraUp(const cXyz& i_up) { mCamera.mCameraUp = i_up; }
    void setCameraEye(const cXyz& i_eye) { mCamera.mCameraEye = i_eye; }
    void setCameraCtr(const cXyz& i_ctr) { mCamera.mCameraCtr = i_ctr; }

private:
    /* 0x00 */ cXyz mPosition;
    /* 0x0C */ u32 mParam;
    /* 0x10 */ s16 mAngleY;
    /* 0x12 */ s8 unk18;
    /* 0x14 */ dSv_turnRestart_camera_c mCamera;
    /* 0x38 */ f32 mCameraFvy;
};  // Size: 0x3C

class dSv_reserve_c {
public:
private:
    u8 unk[80];
};

class dSv_save_c {
public:
    dSv_save_c() {}

    void init();
    dSv_memory2_c* getSave2(int i_stage2No);

    dSv_player_c& getPlayer() { return mPlayer; }
    dSv_event_c& getEvent() { return mEvent; }
    dSv_MiniGame_c& getMiniGame() { return mMiniGame; }

    dSv_memory_c& getSave(int i_stageNo) {
        JUT_ASSERT(1412, 0 <= i_stageNo && i_stageNo < STAGE_MAX);
        return mSave[i_stageNo];
    }

    void putSave(int i_stageNo, dSv_memory_c mem) {
        JUT_ASSERT(1417, 0 <= i_stageNo && i_stageNo < dSv_save_c::STAGE_MAX);
        mSave[i_stageNo] = mem;
    }

    static const int STAGE_MAX = 32;
    static const int STAGE2_MAX = 64;

    // private:
    /* 0x000 */ dSv_player_c mPlayer;
    /* 0x1F0 */ dSv_memory_c mSave[STAGE_MAX];
    /* 0x5F0 */ dSv_memory2_c mSave2[STAGE2_MAX];
    /* 0x7F0 */ dSv_event_c mEvent;
    /* 0x8F0 */ dSv_reserve_c reserve;
    /* 0x940 */ dSv_MiniGame_c mMiniGame;
};  // Size: 0x958

class flagFile_c : public JORReflexible {
public:
    enum Flag_e {
        FLAG_SCENE_e = 0x1,
        FLAG_SAVE_e  = 0x2,
        FLAG_MEM_e   = 0x4,
        FLAG_DAN_e   = 0x8,
        FLAG_ALL_e   = 0xF,
    };

    flagFile_c();
    ~flagFile_c();
    BOOL check_flag(u16);

    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void genMessage(JORMContext*);

    /* 0x4 */ u8 unk_0x4[0x6 - 0x4];
    /* 0x6 */ u16 m_flags;
    /* 0x8 */ s8 m_no;
};

class dSv_info_c {
public:
    void init();
    void getSave(int i_stageNo);
    void putSave(int i_stageNo);
    void initZone();
    u32 createZone(int i_roomNo);
    void onSwitch(int i_no, int i_roomNo);
    void offSwitch(int i_no, int i_roomNo);
    BOOL isSwitch(int i_no, int i_roomNo) const;
    BOOL revSwitch(int i_no, int i_roomNo);
    void onItem(int i_no, int i_roomNo);
    BOOL isItem(int i_no, int i_roomNo) const;
    void onActor(int i_no, int i_roomNo);
    void offActor(int i_no, int i_roomNo);
    BOOL isActor(int i_no, int i_roomNo) const;
    int memory_to_card(char* card_ptr, int dataNum);
    int card_to_memory(char* card_ptr, int dataNum);
    int initdata_to_card(char* card_ptr, int dataNum);

    dSv_save_c& getSavedata() { return mSavedata; }
    dSv_memory_c& getMemory() { return mMemory; }
    dSv_zone_c* getZones() { return mZone; }
    dSv_zone_c& getZone(int id) { return mZone[id]; }
    dSv_player_c& getPlayer() { return mSavedata.getPlayer(); }
    dSv_event_c& getTmp() { return mTmp; }
    dSv_restart_c& getRestart() { return mRestart; }
    dSv_turnRestart_c& getTurnRestart() { return mTurnRestart; }
    dSv_event_c& getEvent() { return mSavedata.getEvent(); }
    dSv_danBit_c& getDan() { return mDan; }
    dSv_MiniGame_c& getMiniGame() { return mSavedata.getMiniGame(); }
    s64 getStartTime() const { return mStartTime; }
    s64 getSaveTotalTime() const { return mSaveTotalTime; }
    void setStartTime(s64 time) { mStartTime = time; }
    void setSaveTotalTime(s64 time) { mSaveTotalTime = time; }
    void initDan(s8 i_stage) { mDan.init(i_stage); }
    void resetDan() { mDan.reset(); }
    u8 getDataNum() const { return mDataNum; }
    void setDataNum(u8 num) { mDataNum = num; }
    void removeZone(int zoneNo) { mZone[zoneNo].reset(); }
    u8 getNoFile() const { return mNoFile; }
    void setNoFile(u8 file) { mNoFile = file; }
    u8 getNewFile() const { return mNewFile; }
    void setNewFile(u8 file) { mNewFile = file; }

    void setSavedata(dSv_save_c& i_save) { mSavedata = i_save; }
    void setMemory(dSv_memory_c& i_memory) { mMemory = i_memory; }
    void setDan(dSv_danBit_c& i_dan) { mDan = i_dan; }

    static const int MEMORY_SWITCH = 0x80;
    static const int DAN_SWITCH = 0x40;
    static const int ZONE_SWITCH = 0x20;
    static const int ONEZONE_SWITCH = 0x10;

    static const int MEMORY_ITEM = 0x80;
    static const int DAN_ITEM = 0x20;
    static const int ZONE_ITEM = 0x20;
    static const int ONEZONE_ITEM = 0x10;

    static const int ZONE_MAX = 0x20;

#if DEBUG
    /* 0x000 */ u8 unk_0x0;
    /* 0x001 */ char unk_0x1;
    /* 0x000 */ u8 unk_0x2[0x48 - 0x2];
#endif
    /* 0x000 */ dSv_save_c mSavedata;
    /* 0x958 */ dSv_memory_c mMemory;
    /* 0x978 */ dSv_danBit_c mDan;
    /* 0x9B4 */ dSv_zone_c mZone[ZONE_MAX];
    /* 0xDB4 */ dSv_restart_c mRestart;
    /* 0xDD8 */ dSv_event_c mTmp;
    /* 0xED8 */ dSv_turnRestart_c mTurnRestart;
    /* 0xF14 */ u8 field_0xf14[4];
    /* 0xF18 */ u8 mDataNum;
    /* 0xF19 */ u8 mNewFile;
    /* 0xF1A */ u8 mNoFile;
    /* 0xF1B */ u8 field_0xf1b[13];
    /* 0xF28 */ s64 mStartTime;
    /* 0xF30 */ s64 mSaveTotalTime;
#if DEBUG
    /* 0xF80 */ flagFile_c mFlagFile;
#endif
};  // Size: 0xF38

class dSv_event_flag_c {
public:
    enum {
        #include "d/d_save_bit_labels.inc"
    };

#if VERSION > VERSION_GCN_JPN
    const
#endif
#if PLATFORM_SHIELD
    s16
#else
    u16
#endif
    static saveBitLabels[822];
};

class dSv_event_tmp_flag_c {
public:
    enum {
        #include "d/d_save_temp_bit_labels.inc"
    };

    static u16 const tempBitLabels[185];
};

#endif /* D_SAVE_D_SAVE_H */
