#ifndef D_COM_D_COM_INF_GAME_H
#define D_COM_D_COM_INF_GAME_H

#include "d/d_attention.h"
#include "d/d_cc_s.h"
#include "d/d_event.h"
#include "d/d_event_manager.h"
#include "d/d_particle.h"
#include "d/d_resorce.h"
#include "d/d_vibration.h"
#include "d/d_drawlist.h"
#include "d/d_stage.h"
#include "d/d_save.h"
#include "f_op/f_op_actor.h"
#include "global.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include <cstring>

#include "tracy/Tracy.hpp"

enum dComIfG_ButtonStatus {
    /* 0x00 */ BUTTON_STATUS_NONE,
    /* 0x01 */ BUTTON_STATUS_LET_GO,
    /* 0x02 */ BUTTON_STATUS_PEEK,
    /* 0x03 */ BUTTON_STATUS_ATTACK,
    /* 0x04 */ BUTTON_STATUS_PUT_AWAY,
    /* 0x05 */ BUTTON_STATUS_HOWL,
    /* 0x06 */ BUTTON_STATUS_OPEN,
    /* 0x07 */ BUTTON_STATUS_ENTER,
    /* 0x08 */ BUTTON_STATUS_CHECK,
    /* 0x09 */ BUTTON_STATUS_DASH,
    /* 0x0A */ BUTTON_STATUS_CROUCH,
    /* 0x0B */ BUTTON_STATUS_DEFEND,
    /* 0x0C */ BUTTON_STATUS_PICK_UP,
    /* 0x0D */ BUTTON_STATUS_DIG,
    /* 0x0E */ BUTTON_STATUS_EAT,
    /* 0x0F */ BUTTON_STATUS_SELECT,
    /* 0x10 */ BUTTON_STATUS_LOCK,
    /* 0x11 */ BUTTON_STATUS_SWITCH,
    /* 0x12 */ BUTTON_STATUS_BACK,
    /* 0x13 */ BUTTON_STATUS_THROW,
    /* 0x14 */ BUTTON_STATUS_PLACE,
    /* 0x15 */ BUTTON_STATUS_GRAB,
    /* 0x16 */ BUTTON_STATUS_GET_OFF,
    /* 0x17 */ BUTTON_STATUS_GET_ON,
    /* 0x18 */ BUTTON_STATUS_PADDLE,
    /* 0x19 */ BUTTON_STATUS_JUMP,
    /* 0x1A */ BUTTON_STATUS_READ,
    /* 0x1B */ BUTTON_STATUS_LOOK,
    /* 0x1C */ BUTTON_STATUS_SPEAK,
    /* 0x1D */ BUTTON_STATUS_LIFT,
    /* 0x1E */ BUTTON_STATUS_SWING,
    /* 0x1F */ BUTTON_STATUS_UNK_31,
    /* 0x20 */ BUTTON_STATUS_UNK_32,
    /* 0x21 */ BUTTON_STATUS_UNK_33,
    /* 0x22 */ BUTTON_STATUS_CONFIRM,
    /* 0x23 */ BUTTON_STATUS_NEXT,
    /* 0x24 */ BUTTON_STATUS_INFO,
    /* 0x25 */ BUTTON_STATUS_UNK_37,
    /* 0x26 */ BUTTON_STATUS_UNK_38,
    /* 0x27 */ BUTTON_STATUS_UNK_39,
    /* 0x28 */ BUTTON_STATUS_WHOOP,
    /* 0x29 */ BUTTON_STATUS_ZOOM,
    /* 0x2A */ BUTTON_STATUS_QUIT,
    /* 0x2B */ BUTTON_STATUS_PICK,
    /* 0x2C */ BUTTON_STATUS_BLOW,
    /* 0x2D */ BUTTON_STATUS_DODGE,
    /* 0x2E */ BUTTON_STATUS_CUT,
    /* 0x2F */ BUTTON_STATUS_HINT,
    /* 0x30 */ BUTTON_STATUS_FINISH,
    /* 0x31 */ BUTTON_STATUS_SET_FREE,
    /* 0x32 */ BUTTON_STATUS_DISMOUNT,
    /* 0x33 */ BUTTON_STATUS_DROP_DOWN,
    /* 0x34 */ BUTTON_STATUS_UNK_52,
    /* 0x35 */ BUTTON_STATUS_TAKE,
    /* 0x36 */ BUTTON_STATUS_HURRY,
    /* 0x37 */ BUTTON_STATUS_PULL_DOWN,
    /* 0x38 */ BUTTON_STATUS_PET,
    /* 0x39 */ BUTTON_STATUS_UNK_57,
    /* 0x3A */ BUTTON_STATUS_SHIELD_ATTACK,
    /* 0x3B */ BUTTON_STATUS_LISTEN,
    /* 0x3C */ BUTTON_STATUS_DRINK,
    /* 0x3D */ BUTTON_STATUS_UNK_61,
    /* 0x3E */ BUTTON_STATUS_COVER,
    /* 0x3F */ BUTTON_STATUS_PUSH,
    /* 0x40 */ BUTTON_STATUS_RESIST,
    /* 0x41 */ BUTTON_STATUS_DIVE,
    /* 0x42 */ BUTTON_STATUS_PUT_TOGETHER,
    /* 0x43 */ BUTTON_STATUS_SKIP,
    /* 0x44 */ BUTTON_STATUS_SLAP,
    /* 0x45 */ BUTTON_STATUS_SNIFF,
    /* 0x46 */ BUTTON_STATUS_BITE,
    /* 0x47 */ BUTTON_STATUS_ROLL,
    /* 0x48 */ BUTTON_STATUS_FASTEN,
    /* 0x49 */ BUTTON_STATUS_GET_DOWN,
    /* 0x4A */ BUTTON_STATUS_HAWKEYE_OFF,
    /* 0x4B */ BUTTON_STATUS_UNK_75,
    /* 0x4C */ BUTTON_STATUS_SWIM,
    /* 0x4D */ BUTTON_STATUS_CANT_SKIP,
    /* 0x4E */ BUTTON_STATUS_SENSE,
    /* 0x4F */ BUTTON_STATUS_UNK_79,
    /* 0x50 */ BUTTON_STATUS_UNK_80,
    /* 0x51 */ BUTTON_STATUS_LAND,
    /* 0x52 */ BUTTON_STATUS_HOOK,
    /* 0x53 */ BUTTON_STATUS_CHANGE_LOCKS,
    /* 0x54 */ BUTTON_STATUS_EQUIP,
    /* 0x55 */ BUTTON_STATUS_PULL,
    /* 0x56 */ BUTTON_STATUS_REEL,
    /* 0x57 */ BUTTON_STATUS_EXTRACT,
    /* 0x58 */ BUTTON_STATUS_SPIN,
    /* 0x59 */ BUTTON_STATUS_UNK_89,
    /* 0x5A */ BUTTON_STATUS_SPIN_ATTACK,
    /* 0x5B */ BUTTON_STATUS_REEL_FAST,
    /* 0x5C */ BUTTON_STATUS_RAISE,
    /* 0x5D */ BUTTON_STATUS_RELEASE,
    /* 0x5E */ BUTTON_STATUS_UNK_94,
    /* 0x5F */ BUTTON_STATUS_MAP,
    /* 0x60 */ BUTTON_STATUS_ITEMS,
    /* 0x61 */ BUTTON_STATUS_INSERT,
    /* 0x62 */ BUTTON_STATUS_DRAW,
    /* 0x63 */ BUTTON_STATUS_STRIKE,
    /* 0x64 */ BUTTON_STATUS_BLANK,
    /* 0x65 */ BUTTON_STATUS_UNK_101,
    /* 0x66 */ BUTTON_STATUS_POINT_WII_REMOTE,
    /* 0x67 */ BUTTON_STATUS_FLIP,
    /* 0x68 */ BUTTON_STATUS_CHANGE_VIEW,
    /* 0x69 */ BUTTON_STATUS_UNK_105,
    /* 0x6A */ BUTTON_STATUS_UNK_106,
    /* 0x6B */ BUTTON_STATUS_CHANCE,
    /* 0x6C */ BUTTON_STATUS_SCOOP,
    /* 0x6D */ BUTTON_STATUS_SURVEY,
    /* 0x6E */ BUTTON_STATUS_CANCEL,
    /* 0x6F */ BUTTON_STATUS_SEIZE,
    /* 0x70 */ BUTTON_STATUS_COLLECTION,
    /* 0x71 */ BUTTON_STATUS_AREA_MAP,
    /* 0x72 */ BUTTON_STATUS_ACTION,
    /* 0x73 */ BUTTON_STATUS_SET_HOOK,
    /* 0x74 */ BUTTON_STATUS_TILT,
    /* 0x75 */ BUTTON_STATUS_THRUST,
    /* 0x76 */ BUTTON_STATUS_ROTATE,
    /* 0x77 */ BUTTON_STATUS_HELM_SPLITTER,
    /* 0x78 */ BUTTON_STATUS_MOVE,
    /* 0x79 */ BUTTON_STATUS_UNK_121,
    /* 0x7A */ BUTTON_STATUS_HOLD_ON,
    /* 0x7B */ BUTTON_STATUS_UNK_123,
    /* 0x7C */ BUTTON_STATUS_HELP,
    /* 0x7D */ BUTTON_STATUS_ZOOM_IN,
    /* 0x7E */ BUTTON_STATUS_ZOOM_OUT,
    /* 0x7F */ BUTTON_STATUS_MOVE_CLOSER_TV,
    /* 0x80 */ BUTTON_STATUS_UNK_128,
    /* 0x81 */ BUTTON_STATUS_UNK_129,
    /* 0x82 */ BUTTON_STATUS_UNK_130,
    /* 0x83 */ BUTTON_STATUS_UNK_131,
    /* 0x84 */ BUTTON_STATUS_UNK_132,
    /* 0x85 */ BUTTON_STATUS_UNK_133,
    /* 0x86 */ BUTTON_STATUS_UNK_134,
    /* 0x87 */ BUTTON_STATUS_UNK_135,
    /* 0x88 */ BUTTON_STATUS_UNK_136,
    /* 0x89 */ BUTTON_STATUS_UNK_137,
    /* 0x8A */ BUTTON_STATUS_UNK_138,
    /* 0x8B */ BUTTON_STATUS_UNK_139,
    /* 0x8C */ BUTTON_STATUS_UNK_140,
    /* 0x8D */ BUTTON_STATUS_UNK_141,
    /* 0x8E */ BUTTON_STATUS_UNK_142,
    /* 0x8F */ BUTTON_STATUS_UNK_143,
    /* 0x90 */ BUTTON_STATUS_UNK_144,
    /* 0x91 */ BUTTON_STATUS_UNK_145,
    /* 0x92 */ BUTTON_STATUS_UNK_146,
    /* 0x93 */ BUTTON_STATUS_UNK_147,
    /* 0x94 */ BUTTON_STATUS_UNK_148,
    /* 0x95 */ BUTTON_STATUS_UNK_149,
    /* 0x96 */ BUTTON_STATUS_UNK_150,
    /* 0x97 */ BUTTON_STATUS_UNK_151,
    /* 0x98 */ BUTTON_STATUS_UNK_152,
    /* 0x99 */ BUTTON_STATUS_UNK_153,
};

enum dComIfG_ButtonStatusFlag {
    /* 0x0 */ BUTTON_STATUS_FLAG_NONE,
    /* 0x2 */ BUTTON_STATUS_FLAG_EMPHASIS = 2,
    /* 0x4 */ BUTTON_STATUS_FLAG_CONTINUATION = 4,
};

class JKRAramArchive;
class dSmplMdl_draw_c;
class dTimer_c;
class daPy_py_c;
class scene_class;

class __d_timer_info_c {
public:
    __d_timer_info_c() {
        mTimerMode = -1;
        mTimerLimitTimeMs = 0;
        mTimerNowTimeMs = 0;
        mTimerPtr = NULL;
    }
    dTimer_c* mTimerPtr;
    s32 mTimerNowTimeMs;
    s32 mTimerLimitTimeMs;
    s32 mTimerMode;
    u8 mTimerType;
};

class phaseParam_c {
public:
    phaseParam_c(const char* name, JKRHeap* heap) {
        mResName = name;
        mpHeap = heap;
    }

    /* 0x0 */ const char* mResName;
    /* 0x4 */ JKRHeap* mpHeap;
};

class camera_class;
class camera_process_class;
class dComIfG_camera_info_class {
public:
    dComIfG_camera_info_class() {}
    ~dComIfG_camera_info_class() {}

    /* 0x00 */ camera_class* mCamera;
    /* 0x04 */ s8 field_0x4;
    /* 0x05 */ s8 field_0x5;
    /* 0x06 */ s8 field_0x6;
    /* 0x07 */ u8 field_0x7;
    /* 0x08 */ u32 mCameraAttentionStatus;
    /* 0x0C */ f32 mCameraZoomScale;
    /* 0x10 */ f32 mCameraZoomForcus;
    /* 0x14 */ char* mCameraParamFileName;
    class {
    public:
        /* 0x18 */ cXyz mCameraPos;
        /* 0x24 */ cXyz mCameraTarget;
        /* 0x30 */ f32 mCameraFovy;
        /* 0x34 */ s16 mCameraBank;
    } mCamInfo;
};
STATIC_ASSERT(sizeof(dComIfG_camera_info_class) == 0x38);

class dComIfG_resLoader_c {
public:
    dComIfG_resLoader_c();
    ~dComIfG_resLoader_c();
    int load(char const**, JKRHeap*);

private:
    /* 0x0 */ const char** mResNameTable;
    /* 0x4 */ request_of_phase_process_class mPhase;
    /* 0xC */ u8 mLoadIndex;
};

enum PlayerPtr { LINK_PTR, HORSE_PTR };

class daHorse_c;
class dPa_control_c;
class daAlink_c;
class dMsgObject_c;
class J2DOrthoGraph;

class dComIfG_MesgCamInfo_c {
public:
    /* 0x00 */ int mID;
    /* 0x04 */ int mBasicID;
    /* 0x08 */ fopAc_ac_c* mActor[10];
};

class dComIfG_item_info_class {
public:
    /* 0x04E84 */ dMsgObject_c* mMsgObjectClass;
    /* 0x04E88 */ f32 mItemLifeCount;
    /* 0x04E8C */ s32 mItemRupeeCount;
    /* 0x04E90 */ s16 mItemKeyNumCount;
    /* 0x04E92 */ s16 mItemMaxLifeCount;
    /* 0x04E94 */ s16 mItemMagicCount;
    /* 0x04E96 */ s16 mItemNowMagicCount;
    /* 0x04E98 */ s16 mItemMaxMagicCount;
    /* 0x04E9A */ s16 field_0x4e9a;
    /* 0x04E9C */ s32 mItemOilCount;
    /* 0x04EA0 */ s32 mItemNowOil;
    /* 0x04EA4 */ int mItemMaxOilCount;
    /* 0x04EA8 */ int mOxygen;
    /* 0x04EAC */ int mNowOxygen;
    /* 0x04EB0 */ int mMaxOxygen;
    /* 0x04EB4 */ int mOxygenCount;
    /* 0x04EB8 */ int mMaxOxygenCount;
    /* 0x04EBC */ s16 mItemArrowNumCount;
    /* 0x04EBE */ s16 mItemPachinkoNumCount;
    /* 0x04EC0 */ s16 mItemMaxArrowNumCount;
    /* 0x04EC2 */ s16 mItemBombNumCount[3];
    /* 0x04EC8 */ s16 field_0x4ec8;
    /* 0x04ECA */ u8 field_0x4eca[2];
    /* 0x04ECC */ s16 mItemMaxBombNumCount1;
    /* 0x04ECE */ s16 mItemMaxBombNumCount2;
    /* 0x04ED0 */ u8 field_0x4ed0[4];
    /* 0x04ED4 */ int mMessageCountNum;
    /* 0x04ED8 */ s16 field_0x4ed8;
    /* 0x04EDA */ u8 field_0x4eda[0x4EDE - 0x4EDA];
    /* 0x04EDE */ u16 mItemNowLife;
    /* 0x04EE0 */ u8 field_0x4ee0[2];
    /* 0x04EE2 */ u8 mMesgStatus;
    /* 0x04EE3 */ u8 field_0x4ee3;
    /* 0x04EE4 */ u8 mRStatus;
    /* 0x04EE5 */ u8 mAStatus;  // B button
    /* 0x04EE6 */ u8 field_0x4ee6;
    /* 0x04EE7 */ u8 mNunStatus;
    /* 0x04EE8 */ u8 mBottleStatus;
    /* 0x04EE9 */ u8 mRemoConStatus;
    /* 0x04EEA */ u8 field_0x4eea[2];
    /* 0x04EEC */ u8 mDoStatus;  // A button
    /* 0x04EED */ u8 field_0x4eed;
    /* 0x04EEE */ u8 m3DStatus;
    /* 0x04EEF */ u8 field_0x4eef;  // related to NunStatusForce
    /* 0x04EF0 */ u8 field_0x4ef0;  // related to NunStatus
    /* 0x04EF1 */ u8 field_0x4ef1;  // related to RemoConStatusForce
    /* 0x04EF2 */ u8 field_0x4ef2;  // related to RemoConStatus
    /* 0x04EF3 */ u8 field_0x4ef3[2];
    /* 0x04EF5 */ u8 m3DDirection;
    /* 0x04EF6 */ u8 m3DDirectionForce;
    /* 0x04EF7 */ u8 mCStickStatus;
    /* 0x04EF8 */ u8 mCStickDirection;
    /* 0x04EF9 */ u8 mCStickDirectionForce;
    /* 0x04EFA */ u8 mSButtonStatus;
    /* 0x04EFB */ u8 mZStatus;
    /* 0x04EFC */ u8 mRStatusForce;
    /* 0x04EFD */ u8 mAStatusForce;
    /* 0x04EFE */ u8 field_0x4efe;
    /* 0x04EFF */ u8 field_0x4eff;  // related to NunStatusForce
    /* 0x04F00 */ u8 mBottleStatusForce;
    /* 0x04F01 */ u8 field_0x4f01;  // related to RemoConStatusForce
    /* 0x04F02 */ u8 field_0x4f02[2];
    /* 0x04F04 */ u8 mDoStatusForce;
    /* 0x04F05 */ u8 mTouchStatusForce;
    /* 0x04F06 */ u8 m3DStatusForce;
    /* 0x04F07 */ u8 mCStickStatusForce;
    /* 0x04F08 */ u8 mSButtonStatusForce;
    /* 0x04F09 */ u8 mZStatusForce;
    /* 0x04F0A */ u8 mRSetFlag;
    /* 0x04F0B */ u8 mASetFlag;
    /* 0x04F0C */ u8 field_0x4f0c;
    /* 0x04F0D */ u8 mNunSetFlag;
    /* 0x04F0E */ u8 mBottleSetFlag;
    /* 0x04F0F */ u8 mRemoConSetFlag;
    /* 0x04F10 */ u8 field_0x4f10[2];
    /* 0x04F12 */ u8 mDoSetFlag;
    /* 0x04F13 */ u8 m3DSetFlag;
    /* 0x04F14 */ u8 mCStickSetFlag;
    /* 0x04F15 */ u8 mSButtonSetFlag;
    /* 0x04F16 */ u8 mZSetFlag;
    /* 0x04F17 */ u8 mRSetFlagForce;
    /* 0x04F18 */ u8 mASetFlagForce;
    /* 0x04F19 */ u8 field_0x4f19;
    /* 0x04F1A */ u8 field_0x4f1a;  // related to NunStatusForce
    /* 0x04F1B */ u8 mBottleSetFlagForce;
    /* 0x04F1C */ u8 field_0x4f1c;  // related to RemoConStatusForce
    /* 0x04F1D */ u8 field_0x4f1d[2];
    /* 0x04F1F */ u8 mDoSetFlagForce;
    /* 0x04F20 */ u8 m3DSetFlagForce;
    /* 0x04F21 */ u8 mCStickSetFlagForce;
    /* 0x04F22 */ u8 mSButtonSetFlagForce;
    /* 0x04F23 */ u8 mZSetFlagForce;
    /* 0x04F24 */ u8 mXStatus;
    /* 0x04F25 */ u8 mXStatusForce;
    /* 0x04F26 */ u8 mXSetFlag;
    /* 0x04F27 */ u8 mXSetFlagForce;
    /* 0x04F28 */ u8 mYStatus;
    /* 0x04F29 */ u8 mYStatusForce;
    /* 0x04F2A */ u8 mYSetFlag;
    /* 0x04F2B */ u8 mYSetFlagForce;
    /* 0x04F2C */ u8 mNunZStatus;
    /* 0x04F2D */ u8 mNunZSetFlag;
    /* 0x04F2E */ u8 field_0x4fc6;  // related to NunZStatusForce
    /* 0x04F2F */ u8 field_0x4fc7;  // related to NunZStatusForce
    /* 0x04F30 */ u8 mNunCStatus;
    /* 0x04F31 */ u8 mNunCSetFlag;
    /* 0x04F32 */ u8 field_0x4fca;  // related to NunCStatusForce
    /* 0x04F33 */ u8 field_0x4fcb;  // related to NunCStatusForce
    /* 0x04F34 */ u8 mSelectItem[8];
    /* 0x04F3C */ u8 mSelectEquip[6];
    /* 0x04F42 */ u8 mBaseAnimeID;
    /* 0x04F43 */ u8 mFaceAnimeID;
    /* 0x04F44 */ u8 mNowAnimeID;
    /* 0x04F45 */ u8 mItemSlot;
    /* 0x04F46 */ u8 mItemID;
    /* 0x04F47 */ u8 field_0x4f47[2];
    /* 0x04F49 */ u8 mDirection;
    /* 0x04F4A */ u8 field_0x4f4a;
#if PLATFORM_WII || PLATFORM_SHIELD
    /* 0x04FE3 */ u8 unk_0x4fe3;
#endif
    /* 0x04F4B */ u8 field_0x4f4b;  // related to itemInit
    /* 0x04F4C */ u8 field_0x4f4c;  // related to itemInit
    /* 0x04F4D */ u8 field_0x4f4d;
    /* 0x04F4E */ u8 mMesgCancelButton;
    /* 0x04F4F */ u8 field_0x4f4f[2];
    /* 0x04F51 */ u8 mGameoverStatus;
    /* 0x04F52 */ u8 field_0x4f52[5];
    /* 0x04F57 */ u8 mHeapLockFlag;
    /* 0x04F58 */ u8 mSubHeapLockFlag[2];
    /* 0x04F5A */ u8 mNowVibration;
    /* 0x04F5B */ u8 field_0x4f5b[2];
    /* 0x04F5D */ u8 mWolfAbility[12];
    /* 0x04F69 */ u8 mNeedLightDropNum;
    /* 0x04F6A */ u8 field_0x4f6a[18];
    /* 0x04F7C */ u8 mMesgBgm;
    /* 0x04F7D */ u8 mPauseFlag;
    /* 0x04F7E */ u8 mItemLifeCountType;
    /* 0x04F7F */ u8 mOxygenShowFlag;
    /* 0x04F80 */ u8 mShow2D;
    /* 0x04F84 */ JKRExpHeap* mExpHeap2D;
    /* 0x04F88 */ JKRExpHeap* mSubExpHeap2D[2];
    /* 0x04F90 */ JKRExpHeap* mMsgExpHeap;
    /* 0x04F94 */ dSv_memBit_c::WarpItemData_c mWarpItemData;
};

class dComIfG_play_c {
public:
    dComIfG_play_c() { this->ct(); }
    void ct();
    void init();
    void itemInit();
    void setItemBombNumCount(u8 i_item, s16 count);
    s16 getItemBombNumCount(u8 i_item);
    void clearItemBombNumCount(u8 i_item);
    s16 getItemMaxBombNumCount(u8 i_bombType);
    void setNowVibration(u8 vib_status);
    u32 getNowVibration();
    void setStartStage(dStage_startStage_c* p_startStage);
    static int getLayerNo_common_common(char const* stageName, int roomId, int layer);
    static int getLayerNo_common(char const* stageName, int roomId, int layer);
    static int getLayerNo(int);
    void createParticle();
    dSmplMdl_draw_c* createSimpleModel();
    void deleteSimpleModel();
    void drawSimpleModel();
    int addSimpleModel(J3DModelData* i_modelData, int roomNo, u8 drawBG);
    int removeSimpleModel(J3DModelData* i_modelData, int roomNo);
    void entrySimpleModel(J3DModel* i_model, int roomNo);
    void setTimerNowTimeMs(int time);
    int getTimerNowTimeMs();
    void setTimerLimitTimeMs(int time);
    int getTimerLimitTimeMs();
    void setTimerMode(int mode);
    int getTimerMode();
    void setTimerType(u8 type);
    u8 getTimerType();
    void setTimerPtr(dTimer_c* ptr);
    dTimer_c* getTimerPtr();
    void setWarpItemData(const char* stage, cXyz pos, s16 angle, s8 roomNo, u8, u8);
    ~dComIfG_play_c() {}

    dStage_startStage_c* getStartStage() { return &mStartStage; }
    const char* getStartStageName() { return mStartStage.getName(); }
    s16 getStartStagePoint() { return mStartStage.getPoint(); }
    s8 getStartStageRoomNo() { return mStartStage.getRoomNo(); }
    s8 getStartStageLayer() { return mStartStage.getLayer(); }
    void setStartStageLayer(s8 layer) { mStartStage.setLayer(layer); }
    s8 getStartStageDarkArea() { return mStartStage.getDarkArea(); }
    void setStartStageDarkArea(s8 darkArea) { mStartStage.setDarkArea(darkArea); }

    dStage_startStage_c* getNextStartStage() { return mNextStage.getStartStage(); }
    const char* getNextStageName() { return mNextStage.getName(); }
    s16 getNextStagePoint() { return mNextStage.getPoint(); }
    s8 getNextStageRoomNo() { return mNextStage.getRoomNo(); }
    s8 getNextStageLayer() { return mNextStage.getLayer(); }
    BOOL isEnableNextStage() { return mNextStage.isEnable(); }
    void offEnableNextStage() { mNextStage.offEnable(); }
    s8 getNextStageWipe() { return mNextStage.getWipe(); }
    u8 getNextStageWipeSpeed() { return mNextStage.getWipeSpeed(); }
    void setNextStage(const char* stage, s8 roomNo, s16 point, s8 layer, s8 wipe, u8 wipe_speed) {
        mNextStage.set(stage, roomNo, point, layer, wipe, wipe_speed);
    }

    dStage_stageDt_c& getStage() { return mStageData; }
    dStage_roomControl_c* getRoomControl() { return &mRoomControl; }
    dEvt_control_c* getEvent() { return &mEvent; }
    dEvent_manager_c& getEvtManager() { return mEvtManager; }
    dAttention_c* getAttention() { return &mAttention; }
    dVibration_c& getVibration() { return mVibration; }

    JKRAramArchive* getFieldMapArchive2() { return (JKRAramArchive*)mFieldMapArchive2; }
    void setFieldMapArchive2(JKRArchive* arc) { mFieldMapArchive2 = arc; }
    JKRArchive* getMsgArchive(int idx) { return mMsgArchive[idx]; }
    void setMsgArchive(int i, JKRArchive* arc) { mMsgArchive[i] = arc; }
    JKRArchive* getDemoMsgArchive() { return mDemoMsgArchive; }
    void setDemoMsgArchive(JKRArchive* arc) { mDemoMsgArchive = arc; }
    JKRArchive* getMeterButtonArchive() { return mMeterButtonArchive; }
    void setMeterButtonArchive(JKRArchive* arc) { mMeterButtonArchive = arc; }
    JKRArchive* getFontArchive() { return mFontArchive; }
    void setFontArchive(JKRArchive* arc) { mFontArchive = arc; }
    JKRArchive* getRubyArchive() { return mRubyArchive; }
    void setRubyArchive(JKRArchive* arc) { mRubyArchive = arc; }
    JKRArchive* getAnmArchive() { return mAnmArchive; }
    void setAnmArchive(JKRArchive* arc) { mAnmArchive = arc; }
    JKRArchive* getCollectResArchive() { return mCollectResArchive; }
    void setCollectResArchive(JKRArchive* arc) { mCollectResArchive = arc; }
    JKRArchive* getFmapResArchive() { return mFmapResArchive; }
    void setFmapResArchive(JKRArchive* arc) { mFmapResArchive = arc; }
    JKRArchive* getDmapResArchive() { return mDmapResArchive; }
    void setDmapResArchive(JKRArchive* arc) { mDmapResArchive = arc; }
    JKRArchive* getOptionResArchive() { return mOptionResArchive; }
    void setOptionResArchive(JKRArchive* arc) { mOptionResArchive = arc; }
    JKRArchive* getItemIconArchive() { return mItemIconArchive; }
    void setItemIconArchive(JKRArchive* arc) { mItemIconArchive = arc; }
    JKRArchive* getNameResArchive() { return mNameResArchive; }
    void setNameResArchive(JKRArchive* arc) { mNameResArchive = arc; }
    void setErrorResArchive(JKRArchive* arc) { mErrorResArchive = arc; }
    JKRArchive* getAllMapArchive() { return mAllMapArchive; }
    void setAllMapArchive(JKRArchive* arc) { mAllMapArchive = arc; }
    JKRArchive* getMsgCommonArchive() { return mMsgCommonArchive; }
    void setMsgCommonArchive(JKRArchive* arc) { mMsgCommonArchive = arc; }
    JKRArchive* getRingResArchive() { return mRingResArchive; }
    void setRingResArchive(JKRArchive* arc) { mRingResArchive = arc; }
    JKRArchive* getCardIconResArchive() { return mCardIconResArchive; }
    void setCardIconResArchive(JKRArchive* arc) { mCardIconResArchive = arc; }
    JKRArchive* getMsgDtArchive() { return mMsgDtArchive[0]; }
    JKRArchive* getMsgDtArchive(int idx) { return mMsgDtArchive[idx]; }
    void setMsgDtArchive(int i, JKRArchive* arc) { mMsgDtArchive[i] = arc; }
    JKRArchive* getMain2DArchive() { return mMain2DArchive; }
    void setMain2DArchive(JKRArchive* arc) { mMain2DArchive = arc; }

    dPa_control_c* getParticle() { return mParticle; }
    dSmplMdl_draw_c* getSimpleModel() { return mSimpleModel; }
    int getWindowNum() { return mWindowNum; }
    void setWindowNum(u8 num) { mWindowNum = num; }
    s8 getLayerOld() { return mLayerOld; }
    s32 checkStatus(u16 flags) { return flags & mStatus; }
    void setStatus(u16 status) { mStatus = status; }
    void onStatus(u16 i_status) { mStatus |= i_status; }
    dDlst_window_c* getWindow(int i) { return &mWindow[i]; }
    void setWindow(int i, f32 param_1, f32 param_2, f32 param_3, f32 param_4, f32 param_5,
                   f32 param_6, int camID, int mode) {
        mWindow[i].setViewPort(param_1, param_2, param_3, param_4, param_5, param_6);
        mWindow[i].setScissor(param_1, param_2, param_3, param_4);
        mWindow[i].setCameraID(camID);
        mWindow[i].setMode(mode);
    }

    camera_class* getCamera(int idx) { return mCameraInfo[idx].mCamera; }
    void setCamera(int i, camera_class* cam) { mCameraInfo[i].mCamera = cam; }
    int getCameraWinID(int i) { return mCameraInfo[i].field_0x4; }
    int getCameraPlayer1ID(int i) { return mCameraInfo[i].field_0x5; }
    int getCameraPlayer2ID(int i) { return mCameraInfo[i].field_0x6; }
    u32 getCameraAttentionStatus(int i) { return mCameraInfo[i].mCameraAttentionStatus; }
    BOOL checkCameraAttentionStatus(int i, u32 flag) {
        return mCameraInfo[i].mCameraAttentionStatus & flag;
    }
    void setCameraAttentionStatus(int i, u32 flag) { mCameraInfo[i].mCameraAttentionStatus = flag; }
    void onCameraAttentionStatus(int i, u32 flag) { mCameraInfo[i].mCameraAttentionStatus |= flag; }
    void offCameraAttentionStatus(int i, u32 flag) {
        mCameraInfo[i].mCameraAttentionStatus &= ~flag;
    }
    void setCameraInfo(int camIdx, camera_class* p_cam, int param_2, int param_3, int param_4) {
        mCameraInfo[camIdx].mCamera = p_cam;
        mCameraInfo[camIdx].field_0x4 = param_2;
        mCameraInfo[camIdx].field_0x5 = param_3;
        mCameraInfo[camIdx].field_0x6 = param_4;
        setCameraAttentionStatus(camIdx, 0);
    }
    f32 getCameraZoomScale(int i_no) { return mCameraInfo[i_no].mCameraZoomScale; }
    void setCameraZoomScale(int i_no, f32 i_scale) { mCameraInfo[i_no].mCameraZoomScale = i_scale; }
    f32 getCameraZoomForcus(int i_no) { return mCameraInfo[i_no].mCameraZoomForcus; }
    void setCameraZoomForcus(int i_no, f32 i_focus) {
        mCameraInfo[i_no].mCameraZoomForcus = i_focus;
    }
    const char* getCameraParamFileName(int i) { return mCameraInfo[i].mCameraParamFileName; }
    void setCameraParamFileName(int i, char* name) { mCameraInfo[i].mCameraParamFileName = name; }
    void saveCameraPosition(int i, cXyz* i_pos, cXyz* i_target, f32 i_fovy, s16 i_bank) {
        mCameraInfo[i].mCamInfo.mCameraPos = *i_pos;
        mCameraInfo[i].mCamInfo.mCameraTarget = *i_target;
        mCameraInfo[i].mCamInfo.mCameraFovy = i_fovy;
        mCameraInfo[i].mCamInfo.mCameraBank = i_bank;
    }
    void loadCameraPosition(int i, cXyz* o_pos, cXyz* o_target, f32* o_fovy, s16* o_bank) {
        *o_pos = mCameraInfo[i].mCamInfo.mCameraPos;
        *o_target = mCameraInfo[i].mCamInfo.mCameraTarget;
        *o_fovy = mCameraInfo[i].mCamInfo.mCameraFovy;
        *o_bank = mCameraInfo[i].mCamInfo.mCameraBank;
    }

    fopAc_ac_c* getPlayer(int idx) { return mPlayerInfo[idx].mpPlayer; }
    void setPlayer(int i, fopAc_ac_c* player) { mPlayerInfo[i].mpPlayer = player; }
    int getPlayerCameraID(int i) { return mPlayerInfo[i].mCameraID; }
    void setPlayerInfo(int i, fopAc_ac_c* player, int cam) {
        mPlayerInfo[i].mpPlayer = player;
        mPlayerInfo[i].mCameraID = cam;
    }

    void* getPlayerPtr(int ptrIdx) { return mPlayerPtr[ptrIdx]; }
    void setPlayerPtr(int i, fopAc_ac_c* ptr) { mPlayerPtr[i] = ptr; }
    daHorse_c* getHorseActor() { return (daHorse_c*)mPlayerPtr[1]; }

    dMsgObject_c* getMsgObjectClass() { return mItemInfo.mMsgObjectClass; }
    void setMsgObjectClass(dMsgObject_c* obj) { mItemInfo.mMsgObjectClass = obj; }
    f32 getItemLifeCount() { return mItemInfo.mItemLifeCount; }
    u8& getItemLifeCountType() { return mItemInfo.mItemLifeCountType; }
    void setItemLifeCount(f32 hearts, u8 type) {
        mItemInfo.mItemLifeCount += hearts;
        mItemInfo.mItemLifeCountType = type;
    }
    void clearItemLifeCount() {
        mItemInfo.mItemLifeCount = 0.0f;
        mItemInfo.mItemLifeCountType = 0;
    }
    s32 getItemRupeeCount() { return mItemInfo.mItemRupeeCount; }
    void setItemRupeeCount(s32 rupees) { mItemInfo.mItemRupeeCount += rupees; }
    void clearItemRupeeCount() { mItemInfo.mItemRupeeCount = 0; }
    s16 getItemKeyNumCount() { return mItemInfo.mItemKeyNumCount; }
    void setItemKeyNumCount(s16 keys) { mItemInfo.mItemKeyNumCount += keys; }
    void clearItemKeyNumCount() { mItemInfo.mItemKeyNumCount = 0; }
    s16 getItemMaxLifeCount() { return mItemInfo.mItemMaxLifeCount; }
    void setItemMaxLifeCount(s16 max) { mItemInfo.mItemMaxLifeCount += max; }
    void clearItemMaxLifeCount() { mItemInfo.mItemMaxLifeCount = 0; }
    void setItemMagicCount(s16 magic) { mItemInfo.mItemMagicCount += magic; }
    void setItemNowMagic(s16 magic) { mItemInfo.mItemNowMagicCount = magic; }
    void setItemMaxMagicCount(s16 max) { mItemInfo.mItemMaxMagicCount += max; }
    s32 getItemOilCount() { return mItemInfo.mItemOilCount; }
    void setItemOilCount(s32 oil) { mItemInfo.mItemOilCount += oil; }
    void setItemMaxOilCount(s32 oil) { mItemInfo.mItemMaxOilCount += oil; }
    void clearItemOilCount() { mItemInfo.mItemOilCount = 0; }
    s32 getItemNowOil() { return mItemInfo.mItemNowOil; }
    void setItemNowOil(s32 oil) { mItemInfo.mItemNowOil = oil; }
    s32 getItemMaxOilCount() { return mItemInfo.mItemMaxOilCount; }
    void clearItemMaxOilCount() { mItemInfo.mItemMaxOilCount = 0; }
    int getOxygen() { return mItemInfo.mOxygen; }
    void setOxygen(s32 oxygen) { mItemInfo.mOxygen = oxygen; }
    int getNowOxygen() { return mItemInfo.mNowOxygen; }
    void setNowOxygen(s32 oxygen) { mItemInfo.mNowOxygen = oxygen; }
    int getMaxOxygen() { return mItemInfo.mMaxOxygen; }
    void setMaxOxygen(s32 max) { mItemInfo.mMaxOxygen = max; }
    s32 getOxygenCount() { return mItemInfo.mOxygenCount; }
    void setOxygenCount(s32 oxygen) { mItemInfo.mOxygenCount += oxygen; }
    void clearOxygenCount() { mItemInfo.mOxygenCount = 0; }
    void setMaxOxygenCount(s32 oxygen) { mItemInfo.mMaxOxygenCount += oxygen; }
    s32 getMaxOxygenCount() { return mItemInfo.mMaxOxygenCount; }
    void clearMaxOxygenCount() { mItemInfo.mMaxOxygenCount = 0; }
    s16 getItemArrowNumCount() { return mItemInfo.mItemArrowNumCount; }
    void setItemArrowNumCount(s16 arrows) { mItemInfo.mItemArrowNumCount += arrows; }
    void clearItemArrowNumCount() { mItemInfo.mItemArrowNumCount = 0; }
    s16 getItemPachinkoNumCount() { return mItemInfo.mItemPachinkoNumCount; }
    void setItemPachinkoNumCount(s16 seeds) { mItemInfo.mItemPachinkoNumCount += seeds; }
    void clearItemPachinkoNumCount() { mItemInfo.mItemPachinkoNumCount = 0; }
    s16 getItemMaxArrowNumCount() { return mItemInfo.mItemMaxArrowNumCount; }
    int getMessageCountNumber() { return mItemInfo.mMessageCountNum; }
    void setMessageCountNumber(u32 number) { mItemInfo.mMessageCountNum = number; }
    u16 getItemNowLife() { return mItemInfo.mItemNowLife; }
    void setItemNowLife(u16 life) { mItemInfo.mItemNowLife = life; }
    u8 getMesgStatus() { return mItemInfo.mMesgStatus; }
    u8 getRStatus() { return mItemInfo.mRStatus; }
    bool isRSetFlag(u8 flag) { return (mItemInfo.mRSetFlag & flag) ? true : false; }
    void setRStatus(u8 status, u8 flag) {
        mItemInfo.mRStatus = status;
        mItemInfo.mRSetFlag = flag;
    }
    u8 getRStatusForce() { return mItemInfo.mRStatusForce; }
    u8 getRSetFlagForce() { return mItemInfo.mRSetFlagForce; }
    void setRStatusForce(u8 status, u8 flag) {
        mItemInfo.mRStatusForce = status;
        mItemInfo.mRSetFlagForce = flag;
    }
    u8 getAStatus() { return mItemInfo.mAStatus; }
    bool isASetFlag(u8 flag) { return (mItemInfo.mASetFlag & flag) ? true : false; }
    void setAStatus(u8 status, u8 flag) {
        mItemInfo.mAStatus = status;
        mItemInfo.mASetFlag = flag;
    }
    u8 getAStatusForce() { return mItemInfo.mAStatusForce; }
    u8 getASetFlagForce() { return mItemInfo.mASetFlagForce; }
    void setAStatusForce(u8 status, u8 flag) {
        mItemInfo.mAStatusForce = status;
        mItemInfo.mASetFlagForce = flag;
    }
    u8 getNunStatus() { return mItemInfo.mNunStatus; }
    bool isNunSetFlag(u8 flag) { return (mItemInfo.mNunSetFlag & flag) ? true : false; }
    void setNunStatus(u8 status, u8 param_1, u8 flag) {
        mItemInfo.mNunStatus = status;
        mItemInfo.field_0x4ef0 = param_1;
        mItemInfo.mNunSetFlag = flag;
    }
    u8 getBottleStatus() { return mItemInfo.mBottleStatus; }
    bool isBottleSetFlag(u8 flag) { return (mItemInfo.mBottleSetFlag & flag) ? true : false; }
    void setBottleStatus(u8 status, u8 flag) {
        mItemInfo.mBottleStatus = status;
        mItemInfo.mBottleSetFlag = flag;
    }
    u8 getBottleStatusForce() { return mItemInfo.mBottleStatusForce; }
    u8 getBottleSetFlagForce() { return mItemInfo.mBottleSetFlagForce; }
    void setBottleStatusForce(u8 status, u8 flag) {
        mItemInfo.mBottleStatusForce = status;
        mItemInfo.mBottleSetFlagForce = flag;
    }
    u8 getRemoConStatus() { return mItemInfo.mRemoConStatus; }
    bool isRemoConSetFlag(u8 flag) { return (mItemInfo.mRemoConSetFlag & flag) ? true : false; }
    void setRemoConStatus(u8 status, u8 param_1, u8 flag) {
        mItemInfo.mRemoConStatus = status;
        mItemInfo.field_0x4ef2 = param_1;
        mItemInfo.mRemoConSetFlag = flag;
    }
    u8 getDoStatus() { return mItemInfo.mDoStatus; }
    bool isDoSetFlag(u8 flag) { return (mItemInfo.mDoSetFlag & flag) ? true : false; }
    void setDoStatus(u8 status, u8 flag) {
        mItemInfo.mDoStatus = status;
        mItemInfo.mDoSetFlag = flag;
    }
    u8 getDoStatusForce() { return mItemInfo.mDoStatusForce; }
    u8 getDoSetFlagForce() { return mItemInfo.mDoSetFlagForce; }
    void setDoStatusForce(u8 status, u8 flag) {
        mItemInfo.mDoStatusForce = status;
        mItemInfo.mDoSetFlagForce = flag;
    }
    u8 get3DStatus() { return mItemInfo.m3DStatus; }
    u8 get3DDirection() { return mItemInfo.m3DDirection; }
    bool is3DSetFlag(u8 flag) { return (mItemInfo.m3DSetFlag & flag) ? true : false; }
    void set3DStatus(u8 status, u8 direction, u8 flag) {
        mItemInfo.m3DStatus = status;
        mItemInfo.m3DDirection = direction;
        mItemInfo.m3DSetFlag = flag;
    }
    u8 get3DStatusForce() { return mItemInfo.m3DStatusForce; }
    u8 get3DDirectionForce() { return mItemInfo.m3DDirectionForce; }
    u8 get3DSetFlagForce() { return mItemInfo.m3DSetFlagForce; }
    void set3DStatusForce(u8 status, u8 direction, u8 flag) {
        mItemInfo.m3DStatusForce = status;
        mItemInfo.m3DDirectionForce = direction;
        mItemInfo.m3DSetFlagForce = flag;
    }
    u8 getCStickStatus() { return mItemInfo.mCStickStatus; }
    u8 getCStickDirection() { return mItemInfo.mCStickDirection; }
    bool isCStickSetFlag(u8 flag) { return (mItemInfo.mCStickSetFlag & flag) ? true : false; }
    void setCStickStatus(u8 status, u8 param_1, u8 flag) {
        mItemInfo.mCStickStatus = status;
        mItemInfo.mCStickDirection = param_1;
        mItemInfo.mCStickSetFlag = flag;
    }
    u8 getCStickStatusForce() { return mItemInfo.mCStickStatusForce; }
    u8 getCStickDirectionForce() { return mItemInfo.mCStickDirectionForce; }
    u8 getCStickSetFlagForce() { return mItemInfo.mCStickSetFlagForce; }
    void setCStickStatusForce(u8 status, u8 param_1, u8 flag) {
        mItemInfo.mCStickStatusForce = status;
        mItemInfo.mCStickDirectionForce = param_1;
        mItemInfo.mCStickSetFlagForce = flag;
    }
    u8 getSButtonStatus() { return mItemInfo.mSButtonStatus; }
    bool isSButtonSetFlag(u8 flag) { return (mItemInfo.mSButtonSetFlag & flag) ? true : false; }
    void setSButtonStatus(u8 status, u8 flag) {
        mItemInfo.mSButtonStatus = status;
        mItemInfo.mSButtonSetFlag = flag;
    }
    u8 getSButtonStatusForce() { return mItemInfo.mSButtonStatusForce; }
    u8 getSButtonSetFlagForce() { return mItemInfo.mSButtonSetFlagForce; }
    void setSButtonStatusForce(u8 status, u8 flag) {
        mItemInfo.mSButtonStatusForce = status;
        mItemInfo.mSButtonSetFlagForce = flag;
    }
    u8 getZStatus() { return mItemInfo.mZStatus; }
    bool isZSetFlag(u8 flag) { return (mItemInfo.mZSetFlag & flag) ? true : false; }
    void setZStatus(u8 status, u8 flag) {
        mItemInfo.mZStatus = status;
        mItemInfo.mZSetFlag = flag;
    }
    u8 getZStatusForce() { return mItemInfo.mZStatusForce; }
    u8 getZSetFlagForce() { return mItemInfo.mZSetFlagForce; }
    void setZStatusForce(u8 status, u8 flag) {
        mItemInfo.mZStatusForce = status;
        mItemInfo.mZSetFlagForce = flag;
    }
    u8 getXStatus() { return mItemInfo.mXStatus; }
    bool isXSetFlag(u8 flag) { return (mItemInfo.mXSetFlag & flag) ? true : false; }
    void setXStatus(u8 status, u8 flag) {
        mItemInfo.mXStatus = status;
        mItemInfo.mXSetFlag = flag;
    }
    u8 getXStatusForce() { return mItemInfo.mXStatusForce; }
    u8 getXSetFlagForce() { return mItemInfo.mXSetFlagForce; }
    void setXStatusForce(u8 status, u8 flag) {
        mItemInfo.mXStatusForce = status;
        mItemInfo.mXSetFlagForce = flag;
    }
    u8 getYStatus() { return mItemInfo.mYStatus; }
    bool isYSetFlag(u8 flag) { return (mItemInfo.mYSetFlag & flag) ? true : false; }
    void setYStatus(u8 status, u8 flag) {
        mItemInfo.mYStatus = status;
        mItemInfo.mYSetFlag = flag;
    }
    u8 getYStatusForce() { return mItemInfo.mYStatusForce; }
    u8 getYSetFlagForce() { return mItemInfo.mYSetFlagForce; }
    void setYStatusForce(u8 status, u8 flag) {
        mItemInfo.mYStatusForce = status;
        mItemInfo.mYSetFlagForce = flag;
    }
    u8 getNunZStatus() { return mItemInfo.mNunZStatus; }
    bool isNunZSetFlag(u8 flag) { return (mItemInfo.mNunZSetFlag & flag) ? true : false; }
    void setNunZStatus(u8 status, u8 flag) {
        mItemInfo.mNunZStatus = status;
        mItemInfo.mNunZSetFlag = flag;
    }
    u8 getNunCStatus() { return mItemInfo.mNunCStatus; }
    bool isNunCSetFlag(u8 flag) { return (mItemInfo.mNunCSetFlag & flag) ? true : false; }
    void setNunCStatus(u8 status, u8 flag) {
        mItemInfo.mNunCStatus = status;
        mItemInfo.mNunCSetFlag = flag;
    }
    u8 getSelectItem(int idx) { return mItemInfo.mSelectItem[idx]; }
    void setSelectItem(int idx, u8 i_itemNo) { mItemInfo.mSelectItem[idx] = i_itemNo; }
    void setSelectEquip(int idx, u8 item_id) { mItemInfo.mSelectEquip[idx] = item_id; }
    u8 getBaseAnimeID() { return mItemInfo.mBaseAnimeID; }
    void setBaseAnimeID(u8 id) { mItemInfo.mBaseAnimeID = id; }
    void clearBaseAnimeID() { mItemInfo.mBaseAnimeID = 0; }
    u8 getFaceAnimeID() { return mItemInfo.mFaceAnimeID; }
    void setFaceAnimeID(u8 id) { mItemInfo.mFaceAnimeID = id; }
    void clearFaceAnimeID() { mItemInfo.mFaceAnimeID = 0; }
    void clearNowAnimeID() { mItemInfo.mNowAnimeID = -1; }
    void setItem(u8 slot, u8 i_no) {
        mItemInfo.mItemSlot = slot;
        mItemInfo.mItemID = i_no;
    }
    u8 getDirection() { return mItemInfo.mDirection; }
    void setDirection(u8 i_dir) { mItemInfo.mDirection = i_dir; }
    u8 checkMesgCancelButton() { return mItemInfo.mMesgCancelButton; }
    void setMesgCancelButton(u8 button) { mItemInfo.mMesgCancelButton = button; }
    u8 getGameoverStatus() { return mItemInfo.mGameoverStatus; }
    void setGameoverStatus(u8 status) { mItemInfo.mGameoverStatus = status; }
    u8 isHeapLockFlag() { return mItemInfo.mHeapLockFlag; }
    void setHeapLockFlag(u8 status) { mItemInfo.mHeapLockFlag = status; }
    void offHeapLockFlag() { mItemInfo.mHeapLockFlag = 0; }
    u8 getSubHeapLockFlag(int idx) { return mItemInfo.mSubHeapLockFlag[idx]; }
    void setSubHeapLockFlag(int idx, u8 status) { mItemInfo.mSubHeapLockFlag[idx] = status; }
    u8 getWolfAbility(int i_idx) { return mItemInfo.mWolfAbility[i_idx]; }
    u8 getNeedLightDropNum() { return mItemInfo.mNeedLightDropNum; }
    void setNeedLightDropNum(u8 i_num) { mItemInfo.mNeedLightDropNum = i_num; }
    u8 checkMesgBgm() { return mItemInfo.mMesgBgm; }
    void setMesgBgm(u8 param_0) { mItemInfo.mMesgBgm = param_0; }
    u8 isPauseFlag() { return mItemInfo.mPauseFlag; }
    void offPauseFlag() { mItemInfo.mPauseFlag = false; }
    void onPauseFlag() { mItemInfo.mPauseFlag = true; }
    u8 getOxygenShowFlag() { return mItemInfo.mOxygenShowFlag; }
    void setOxygenShowFlag(u8 flag) { mItemInfo.mOxygenShowFlag = flag; }
    u8 show2dCheck() { return mItemInfo.mShow2D; }
    void show2dOn() { mItemInfo.mShow2D = 1; }
    void show2dOff() { mItemInfo.mShow2D = 0; }
    JKRExpHeap* getExpHeap2D() { return mItemInfo.mExpHeap2D; }
    void setExpHeap2D(void* heap) { mItemInfo.mExpHeap2D = (JKRExpHeap*)heap; }
    JKRExpHeap* getSubExpHeap2D(int idx) { return mItemInfo.mSubExpHeap2D[idx]; }
    void setSubExpHeap2D(int idx, void* heap) { mItemInfo.mSubExpHeap2D[idx] = (JKRExpHeap*)heap; }
    JKRExpHeap* getMsgExpHeap() { return mItemInfo.mMsgExpHeap; }
    void setMsgExpHeap(void* heap) { mItemInfo.mMsgExpHeap = (JKRExpHeap*)heap; }

    dComIfG_MesgCamInfo_c* getMesgCamInfo() { return &mMesgCamInfo; }
    void setMesgCamInfoID(int param_0) { mMesgCamInfo.mID = param_0; }
    void clearMesgCamInfoID() { mMesgCamInfo.mID = -1; }
    void setMesgCamInfoBasicID(int id) { mMesgCamInfo.mBasicID = id; }
    void clearMesgCamInfoBasicID() { mMesgCamInfo.mBasicID = 0; }
    void setMesgCamInfoActor(fopAc_ac_c* param_1, fopAc_ac_c* param_2, fopAc_ac_c* param_3,
                             fopAc_ac_c* param_4, fopAc_ac_c* param_5, fopAc_ac_c* param_6,
                             fopAc_ac_c* param_7, fopAc_ac_c* param_8, fopAc_ac_c* param_9,
                             fopAc_ac_c* param_10) {
        mMesgCamInfo.mActor[0] = param_1;
        mMesgCamInfo.mActor[1] = param_2;
        mMesgCamInfo.mActor[2] = param_3;
        mMesgCamInfo.mActor[3] = param_4;
        mMesgCamInfo.mActor[4] = param_5;
        mMesgCamInfo.mActor[5] = param_6;
        mMesgCamInfo.mActor[6] = param_7;
        mMesgCamInfo.mActor[7] = param_8;
        mMesgCamInfo.mActor[8] = param_9;
        mMesgCamInfo.mActor[9] = param_10;
    }

    u32 checkPlayerStatus(int param_0, int i, u32 flag) { return mPlayerStatus[param_0][i] & flag; }
    void setPlayerStatus(int param_0, int i, u32 flag) { mPlayerStatus[param_0][i] |= flag; }
    void clearPlayerStatus(int param_0, int i, u32 flag) { mPlayerStatus[param_0][i] &= ~flag; }

    void setCurrentWindow(dDlst_window_c* i_window) { mCurrentWindow = i_window; }
    void setCurrentView(view_class* i_view) { mCurrentView = i_view; }
    void setCurrentViewport(view_port_class* i_viewport) { mCurrentViewport = i_viewport; }
    J2DGrafContext* getCurrentGrafPort() { return (J2DGrafContext*)mCurrentGrafPort; }
    void setCurrentGrafPort(J2DOrthoGraph* i_graf) { mCurrentGrafPort = i_graf; }
    void* getItemTable() { return mItemTable; }
    void setItemTable(void* data) { mItemTable = data; }
    char* getLastPlayStageName() { return mLastPlayStageName; }
    void setLastPlayStageName(char* name) {
        strncpy(mLastPlayStageName, name, 7);
        mLastPlayStageName[7] = 0;
    }

public:
    /* 0x00000 */ dBgS mBgs;
    /* 0x01404 */ dCcS mCcs;
    /* 0x03EC8 */ dStage_startStage_c mStartStage;
    /* 0x03ED6 */ dStage_nextStage_c mNextStage;
    /* 0x03EE8 */ dStage_stageDt_c mStageData;
    /* 0x03F8C */ dStage_roomControl_c mRoomControl;
    /* 0x03F90 */ dEvt_control_c mEvent;
    /* 0x040C0 */ dEvent_manager_c mEvtManager;
    /* 0x04780 */ dAttention_c mAttention;
    #if PLATFORM_WII || VERSION == VERSION_SHIELD
    /* 0x04C9C */ u8 unk_0x4c9c[8];
    #endif
    /* 0x04C9C */ dVibration_c mVibration;
    /* 0x04D2C */ u8 field_0x4d2c[4];
    /* 0x04D30 */ JKRArchive* mFieldMapArchive2;
    /* 0x04D34 */ JKRArchive* mMsgArchive[11];
    /* 0x04D60 */ JKRArchive* mDemoMsgArchive;
    /* 0x04D64 */ JKRArchive* mMeterButtonArchive;
    /* 0x04D68 */ void* field_0x4d68;
    /* 0x04D6C */ JKRArchive* mFontArchive;
    /* 0x04D70 */ JKRArchive* mRubyArchive;
    /* 0x04D74 */ JKRArchive* mAnmArchive;
    /* 0x04D78 */ void* field_0x4d78[2];
    /* 0x04D80 */ JKRArchive* mCollectResArchive;
    /* 0x04D84 */ JKRArchive* mFmapResArchive;
    /* 0x04D88 */ JKRArchive* mDmapResArchive;
    /* 0x04D8C */ JKRArchive* mOptionResArchive;
    /* 0x04D90 */ void* field_0x4d90[2];
    /* 0x04D98 */ JKRArchive* mItemIconArchive;
    /* 0x04D9C */ JKRArchive* mNameResArchive;
    /* 0x04DA0 */ JKRArchive* mErrorResArchive;
    /* 0x04DA4 */ void* field_0x4da4;
    /* 0x04DA8 */ JKRArchive* mAllMapArchive;
    /* 0x04DAC */ JKRArchive* mMsgCommonArchive;
    /* 0x04DB0 */ JKRArchive* mRingResArchive;
    /* 0x04DB4 */ void* field_0x4db4;
    /* 0x04DB8 */ JKRArchive* mCardIconResArchive;
    /* 0x04DBC */ JKRArchive* mMsgDtArchive[15];
    /* 0x04DF8 */ JKRArchive* mMain2DArchive;
    /* 0x04DFC */ void* field_0x4dfc[2];
    /* 0x04E04 */ dPa_control_c* mParticle;
    /* 0x04E08 */ dSmplMdl_draw_c* mSimpleModel;
    /* 0x04E0C */ u8 mWindowNum;
    /* 0x04E0D */ s8 mLayerOld;
    /* 0x04E0E */ u16 mStatus;
    /* 0x04E10 */ dDlst_window_c mWindow[1];
    /* 0x04E3C */ dComIfG_camera_info_class mCameraInfo[1];
    /* 0x04E74 */ struct {
        /* 0x0 */ fopAc_ac_c* mpPlayer;
        /* 0x4 */ s8 mCameraID;
    } mPlayerInfo[1];
    /* 0x04E7C */ fopAc_ac_c* mPlayerPtr[2];  // 0: Player, 1: Horse ; type may be wrong
    /* 0x04E84 */ dComIfG_item_info_class mItemInfo;
    /* 0x04FB0 */ dComIfG_MesgCamInfo_c mMesgCamInfo;
    /* 0x04FE0 */ u32 mPlayerStatus[1][4];
    /* 0x04FF0 */ u8 field_0x4ff0[0x8];
    /* 0x04FF8 */ __d_timer_info_c mTimerInfo;
    /* 0x0500C */ dDlst_window_c* mCurrentWindow;
    /* 0x05010 */ view_class* mCurrentView;
    /* 0x05014 */ view_port_class* mCurrentViewport;
    /* 0x05018 */ J2DOrthoGraph* mCurrentGrafPort;
    /* 0x0501C */ void* mItemTable;
    /* 0x0501D */ u8 field_0x501d[4];
    /* 0x05024 */ char mLastPlayStageName[8];
};

class dComIfG_inf_c {
public:
#if PLATFORM_WII || PLATFORM_SHIELD
    class baseCsr_c : public mDoGph_gInf_c::csr_c {
    public:
        class navi_c {
        public:
            navi_c() : m_heap(NULL), m_model(NULL), field_0x40(0.0f, 0.0f, 0.0f), field_0x4c(0, 0, 0) {
                field_0x54 = 0.0f;
                mParticleId = 0;
            }
            virtual ~navi_c() {}
            void create();
            bool draw(f32, f32, u8);
            u32 getParticleId() { return mParticleId; }

            JKRSolidHeap* m_heap;
            J3DModel* m_model;
            mDoExt_bckAnm m_bck;
            mDoExt_brkAnm m_brk;
            cXyz field_0x40;
            csXyz field_0x4c;
            f32 field_0x54;
            f32 field_0x58;
            f32 field_0x5c;
            u32 mParticleId;
        };

        virtual ~baseCsr_c() {}
        baseCsr_c(u8);
        void draw(f32, f32);
        void create();
        static void particleExecute();
        static navi_c* getNavi() { return m_navi; }

        dDlst_blo_c* getCsr() { return &field_0x8; }
        void onNavi() { field_0x13d = 1; }

        /* 0x008 */ dDlst_blo_c field_0x8;
        /* 0x130 */ dDlst_blo_c::anm_c anm;
        /* 0x13C */ u8 field_0x13c;
        /* 0x13D */ u8 field_0x13d;
        /* 0x13E */ u8 field_0x13e;

        static dPa_hermiteEcallBack_c m_blurCB;
        static u32 _m_blurID;
        static navi_c* m_navi;
    };

    class anmCsr_c : public mDoGph_gInf_c::csr_c {
    public:
        virtual ~anmCsr_c() {}
        void draw(f32, f32);

        dDlst_blo_c field_0x8;
    };
#endif

    dComIfG_inf_c() { this->ct(); }
    ~dComIfG_inf_c() {}
    void ct();

#if PLATFORM_WII || PLATFORM_SHIELD
    static void createBaseCsr();

    static baseCsr_c* getBaseCsr() {
        return m_baseCsr;
    }
#endif

    /* 0x00000 */ dSv_info_c info;
    /* 0x00F38 */ dComIfG_play_c play;
    /* 0x05F64 */ dDlst_list_c drawlist;
    /* 0x1C2F8 */ dRes_control_c mResControl;
    /* 0x1DDF8 */ u8 mFadeBrightness;
    /* 0x1DDF9 */ u8 mWorldDark;
    /* 0x1DDFA */ s8 field_0x1ddfa;
    /* 0x1DDFB */ s8 mPolyDamageOff;
    /* 0x1DDFC */ s8 field_0x1ddfc;
    /* 0x1DE00 */ u32 field_0x1de00;
    /* 0x1DE04 */ u32 field_0x1de04;
    /* 0x1DE08 */ u8 field_0x1de08;
    /* 0x1DE09 */ u8 field_0x1de09;
    /* 0x1DE0A */ u8 field_0x1de0a;
    /* 0x1DE0B */ u8 mIsDebugMode;
    #if DEBUG
    /* 0x1DE0C */ OSStopwatch mStopwatch;
    #endif

    static __d_timer_info_c dComIfG_mTimerInfo;
    #if PLATFORM_WII || PLATFORM_SHIELD
    static baseCsr_c* m_baseCsr;
    #endif
};  // Size: 0x1DE10

STATIC_ASSERT(122384 == sizeof(dComIfG_inf_c));

extern dComIfG_inf_c g_dComIfG_gameInfo;
extern GXColor g_blackColor;
extern GXColor g_clearColor;
extern GXColor g_whiteColor;
extern GXColor g_saftyWhiteColor;

int dComLbG_PhaseHandler(request_of_phase_process_class*, request_of_phase_process_fn*,
                         void*);
BOOL dComIfG_resetToOpening(scene_class* scene);
char* dComIfG_getRoomArcName(int roomNo);
void* dComIfG_getStageRes(char const* i_resName);
void* dComIfG_getOldStageRes(char const* i_resName);
void dComIfG_get_timelayer(int* layer);
int dComIfG_resDelete(request_of_phase_process_class* i_phase, char const* i_resName);
int dComIfG_changeOpeningScene(scene_class* scene, s16 procName);
int dComIfG_resLoad(request_of_phase_process_class* i_phase, char const* i_arcName);
int dComIfG_resLoad(request_of_phase_process_class* i_phase, char const* i_resName, JKRHeap* heap);
int dComIfG_TimerStop(int);
int dComIfG_TimerDeleteCheck(int);
int dComIfG_TimerDeleteRequest(int i_mode);
int dComIfG_TimerStart(int i_mode, s16 i_time);
u32 dComIfG_getTrigA(u32 i_padNo);

bool dComIfGp_isLightDropMapVisible();
int dComIfG_TimerReStart(int);
int dComIfG_TimerEnd(int i_mode, int param_1);
void dComIfGs_onStageBossEnemy(int i_stageNo);

u8 dComIfGs_getMixItemIndex(int i_no);
void dComIfGs_setSelectItemIndex(int i_no, u8 item_index);
void dComIfGs_setMixItemIndex(int i_no, u8 item_index);
u8 dComIfGs_getBottleMax();
u8 dComIfGs_checkGetItem(u8 i_itemNo);
void dComIfGs_setSelectEquipClothes(u8 i_itemNo);
void dComIfGs_setKeyNum(int i_stageNo, u8 i_keyNum);
s32 dComIfGs_isDungeonItemWarp(int i_stageNo);
void dComIfGs_BossLife_public_Set(s8);
s8 dComIfGs_sense_type_change_Get();
void dComIfGs_sense_type_change_Set(s8);
#if TARGET_PC
cXyz dComIfGs_getWarpPlayerPos();
#else
cXyz& dComIfGs_getWarpPlayerPos();
#endif
const char* dComIfGs_getWarpStageName();
s16 dComIfGs_getWarpPlayerAngleY();
s8 dComIfGs_getWarpRoomNo();
// dComIfGs_getWarpMarkFlag declared below
BOOL dComIfGs_isOneZoneSwitch(int i_swBit, int i_roomNo);
BOOL dComIfGs_isOneZoneItem(int i_swBit, int i_roomNo);
BOOL dComIfGs_isZoneSwitch(int, int);
BOOL dComIfGs_isZoneItem(int, int);
void dComIfGs_setLastWarpMarkItemData(const char* stage, cXyz pos, s16 angle, s8 roomNo, u8, u8);
u16 dComIfGs_getMaxLifeGauge();
void dComIfGs_setWarpMarkFlag(u8);
void dComIfGs_setSelectEquipSword(u8 i_itemNo);
void dComIfGs_setSelectEquipShield(u8 i_itemNo);
BOOL dComIfGs_isVisitedRoom(int i_roomNo);
void dComIfGs_onZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_onOneZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_offZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_offOneZoneSwitch(int i_swBit, int i_roomNo);
s8 dComIfGp_getReverb(int i_roomNo);
void dComIfGs_gameStart();
BOOL dComIfGs_wolfeye_effect_check();
BOOL dComIfGs_Wolf_Change_Check();
void dComIfGs_onVisitedRoom(int param_0);
void dComIfGs_offVisitedRoom(int param_0);
void dComIfGs_setWarpItemData(char const* stage, cXyz pos, s16 angle, s8 roomNo, u8 param_4,
                              u8 param_5);
BOOL dComIfGs_isStageSwitch(int i_stageNo, int i_no);
BOOL dComIfGs_isStageTbox(int i_stageNo, int i_no);
void dComIfGs_onStageSwitch(int i_stageNo, int i_no);
void dComIfGs_offStageSwitch(int i_stageNo, int i_no);
BOOL dComIfGs_isStageSwitch(int i_stageNo, int i_no);
void dComIfGs_onDungeonItemMap(int i_stageNo);
void dComIfGs_offDungeonItemMap(int i_stageNo);
s32 dComIfGs_isDungeonItemMap(int i_stageNo);
void dComIfGs_onDungeonItemCompass(int i_stageNo);
void dComIfGs_offDungeonItemCompass(int i_stageNo);
s32 dComIfGs_isDungeonItemCompass(int i_stageNo);
void dComIfGs_onDungeonItemBossKey(int i_stageNo);
void dComIfGs_offDungeonItemBossKey(int i_stageNo);
s32 dComIfGs_isDungeonItemBossKey(int i_stageNo);
void dComIfGs_onStageBossEnemy(int i_stageNo);
void dComIfGs_offStageBossEnemy(int i_stageNo);
s32 dComIfGs_isStageBossEnemy(int i_stageNo);
void dComIfGs_onStageMiddleBoss(int i_stageNo);
void dComIfGs_offStageMiddleBoss(int i_stageNo);
s32 dComIfGs_isStageMiddleBoss(int i_stageNo);
void dComIfGs_onStageLife(int i_stageNo);
void dComIfGs_offStageLife(int i_stageNo);
s32 dComIfGs_isStageLife(int i_stageNo);
void dComIfGs_onStageBossDemo(int i_stageNo);
void dComIfGs_offStageBossDemo(int i_stageNo);
s32 dComIfGs_isStageBossDemo(int i_stageNo);
void dComIfGs_onDungeonItemWarp(int i_stageNo);
void dComIfGs_offDungeonItemWarp(int i_stageNo);
s32 dComIfGs_isDungeonItemWarp(int i_stageNo);
void dComIfGs_BossLife_public_Set(s8);
s8 dComIfGs_BossLife_public_Get();
void dComIfGs_sense_type_change_Set(s8);
s8 dComIfGs_sense_type_change_Get();
void dComIfGs_onZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_offZoneSwitch(int i_swBit, int i_roomNo);
BOOL dComIfGs_isZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_onOneZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_offOneZoneSwitch(int i_swBit, int i_roomNo);
BOOL dComIfGs_isOneZoneSwitch(int i_swBit, int i_roomNo);
void dComIfGs_onZoneItem(int i_swBit, int i_roomNo);
void dComIfGs_offZoneItem(int i_swBit, int i_roomNo);
BOOL dComIfGs_isZoneItem(int i_swBit, int i_roomNo);
void dComIfGs_onOneZoneItem(int i_swBit, int i_roomNo);
void dComIfGs_offOneZoneItem(int i_swBit, int i_roomNo);
BOOL dComIfGs_isOneZoneItem(int i_swBit, int i_roomNo);
u16 dComIfGs_getMaxLifeGauge();
void dComIfGs_onGetMagicUseFlag();
void dComIfGs_setSelectItemIndex(int i_no, u8 item_index);
void dComIfGs_setMixItemIndex(int i_no, u8 item_index);
u8 dComIfGs_getSelectMixItemNoArrowIndex(int i_selmixItemIdx);
u8 dComIfGs_getMixItemIndex(int i_no);
u8 dComIfGs_checkGetInsectNum();
u8 dComIfGs_checkGetItem(u8 i_itemNo);
u8 dComIfGs_getBottleMax();
void dComIfGs_gameStart();
void dComIfGs_setSelectEquipClothes(u8 i_itemNo);
void dComIfGs_setSelectEquipSword(u8 i_itemNo);
void dComIfGs_setSelectEquipShield(u8 i_itemNo);
void dComIfGs_setKeyNum(int i_stageNo, u8 i_keyNum);
void dComIfGs_setWarpItemData(char const* stage, cXyz pos, s16 angle, s8 roomNo, u8 param_4,
                              u8 param_5);
void dComIfGs_setLastWarpMarkItemData(const char* stage, cXyz pos, s16 angle, s8 roomNo, u8, u8);
const char* dComIfGs_getWarpStageName();
#if TARGET_PC
cXyz dComIfGs_getWarpPlayerPos();
#else
cXyz& dComIfGs_getWarpPlayerPos();
#endif
s16 dComIfGs_getWarpPlayerAngleY();
s8 dComIfGs_getWarpRoomNo();
u8 dComIfGs_getWarpMarkFlag();
void dComIfGs_setWarpMarkFlag(u8);
BOOL dComIfGs_Wolf_Change_Check();
void dComIfGs_PolyDamageOff_Set(s8 i_setting);
s8 dComIfGs_PolyDamageOff_Check();
void dComIfGs_shake_kandelaar();
int dComIfGs_shake_kandelaar_check();
BOOL dComIfGs_wolfeye_effect_check();
void dComIfGs_Grass_hide_Set(s8);
void dComIfGs_onVisitedRoom(int i_roomNo);
void dComIfGs_offVisitedRoom(int i_roomNo);
BOOL dComIfGs_isVisitedRoom(int i_roomNo);
void dComIfGs_staffroll_next_go();
u8 dComIfGs_staffroll_next_go_check();

BOOL dComIfGp_checkItemGet(u8 i_itemNo, int param_1);
s8 dComIfGp_getReverb(int i_roomNo);
bool dComIfGp_getMapTrans(int i_roomNo, f32* o_transX, f32* o_transY, s16* o_angle);
stage_camera_class* dComIfGp_getRoomCamera(int i_roomNo);
stage_arrow_class* dComIfGp_getRoomArrow(int i_roomNo);
void dComIfGp_setNextStage(char const* i_stage, s16 i_point, s8 i_roomNo, s8 i_layer, f32 i_lastSpeed,
                           u32 i_lastMode, int, s8 i_wipe, s16 i_lastAngle, int param_9, int i_wipeSpeedT);
void dComIfGp_setNextStage(char const* i_stage, s16 i_point, s8 i_roomNo, s8 i_layer);
void dComIfGp_setSelectItem(int index);
u8 dComIfGp_getSelectItem(int index);
void dComIfGp_mapShow();
void dComIfGp_mapHide();
bool dComIfGp_checkMapShow();
s32 dComIfGp_setHeapLockFlag(u8 flag);
s32 dComIfGp_offHeapLockFlag(int flag);
void dComIfGp_createSubExpHeap2D();
void dComIfGp_destroySubExpHeap2D();
int dComIfGp_checkEmptySubHeap2D();
int dComIfGp_searchUseSubHeap2D(int flag);
JKRExpHeap* dComIfGp_getSubHeap2D(int flag);
s16 dComIfGp_getSelectItemNum(int index);
int dComIfGp_getSelectItemMaxNum(int index);
void dComIfGp_setSelectItemNum(int i_selItemIdx, s16 i_num);
void dComIfGp_addSelectItemNum(int index, s16 num);
void dComIfGp_calcNowRegion();
bool dComIfGp_isLightDropMapVisible();
u8 dComIfGp_getNowLevel();
void dComIfGp_ret_wp_set(s8);
void dComIfGp_world_dark_set(u8);
u8 dComIfGp_world_dark_get();
void dComIfGp_TargetWarpPt_set(u8);
void dComIfGp_SelectWarpPt_set(u8);
u8 dComIfGp_TargetWarpPt_get();
u8 dComIfGp_SelectWarpPt_get();
BOOL dComIfGp_TransportWarp_check();

void dComIfG_get_timelayer(int* o_layer);
int dComIfG_changeOpeningScene(scene_class* i_scene, s16 i_procName);
BOOL dComIfG_resetToOpening(scene_class* i_scene);
int dComIfG_resLoad(request_of_phase_process_class* i_phase, char const* i_arcName);
int dComIfG_resLoad(request_of_phase_process_class* i_phase, char const* i_resName, JKRHeap* i_heap);
int dComIfG_resDelete(request_of_phase_process_class* i_phase, char const* i_resName);
void dComIfG_playerStatusD();
void dComIfG_playerStatusD_pre_clear();
u32 dComIfG_getTrigA(u32 i_padNo);
u8 dComIfG_getNowCalcRegion();
void* dComIfG_getStageRes(char const* i_resName);
void* dComIfG_getOldStageRes(char const* i_resName);
char* dComIfG_getRoomArcName(int roomNo);
int dComIfG_TimerStart(int i_mode, s16 i_time);
int dComIfG_TimerStop(int);
int dComIfG_TimerReStart(int);
int dComIfG_TimerEnd(int i_mode, int param_1);
int dComIfG_TimerDeleteCheck(int);
int dComIfG_TimerDeleteRequest(int i_mode);
int dComLbG_PhaseHandler(request_of_phase_process_class*, request_of_phase_process_fn*, void*);
BOOL dComIfG_isSceneResetButton();

int dComIfGd_setSimpleShadow(cXyz* i_pos, f32 param_1, f32 param_2, cBgS_PolyInfo& param_3, s16 i_angle,
                             f32 param_5, TGXTexObj* i_tex);
int dComIfGd_setShadow(u32 param_0, s8 param_1, J3DModel* param_2, cXyz* param_3, f32 param_4,
                       f32 param_5, f32 param_6, f32 param_7, cBgS_PolyInfo& param_8,
                       dKy_tevstr_c* param_9, s16 param_10, f32 param_11, TGXTexObj* param_12);

inline dSv_info_c* dComIfGs_getSaveInfo() {
    return &g_dComIfG_gameInfo.info;
}

inline dSv_save_c* dComIfGs_getSaveData() {
    return &g_dComIfG_gameInfo.info.getSavedata();
}

inline u16 dComIfGs_getMaxLife() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMaxLife();
}

inline void dComIfGs_setMaxLife(u8 i_maxLife) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMaxLife(i_maxLife);
}

inline u16 dComIfGs_getLife() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getLife();
}

inline void dComIfGs_setLife(u16 i_life) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setLife(i_life);
}

inline u16 dComIfGs_getRupee() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getRupee();
}

inline void dComIfGs_setRupee(u16 i_rupees) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setRupee(i_rupees);
}

inline u16 dComIfGs_getMaxOil() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMaxOil();
}

inline void dComIfGs_setMaxOil(u16 i_maxOil) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMaxOil(i_maxOil);
}

inline u16 dComIfGs_getOil() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getOil();
}

inline void dComIfGs_setOil(u16 i_oil) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setOil(i_oil);
}

inline u8 dComIfGs_getSelectEquipClothes() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_CLOTHING);
}

inline u8 dComIfGs_getSelectEquipSword() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_SWORD);
}

inline u8 dComIfGs_getSelectEquipShield() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_SHIELD);
}

inline u8 dComIfGs_getCollectSmell() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(COLLECT_SMELL);
}

inline void dComIfGs_setCollectSmell(u8 smell) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(COLLECT_SMELL, smell);
}

inline u8 dComIfGs_getBButtonItemKey() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectEquip(B_BUTTON_ITEM);
}

inline void dComIfGs_setBButtonItemKey(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setSelectEquip(B_BUTTON_ITEM, i_itemNo);
}

inline u8 dComIfGs_getWalletSize() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getWalletSize();
}

inline void dComIfGs_setWalletSize(u8 i_size) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setWalletSize(i_size);
}

inline u8 dComIfGs_getMaxMagic() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMaxMagic();
}

inline void dComIfGs_setMaxMagic(u8 i_maxMagic) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMaxMagic(i_maxMagic);
}

inline u8 dComIfGs_getMagic() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getMagic();
}

inline void dComIfGs_setMagic(u8 i_magic) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setMagic(i_magic);
}

inline u8 dComIfGs_getTransformStatus() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getTransformStatus();
}

inline void dComIfGs_setTransformStatus(u8 i_status) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().setTransformStatus(i_status);
}

inline u8 dComIfGs_getSelectItemIndex(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getSelectItemIndex(i_no);
}

inline u16 dComIfGs_getRupeeMax() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().getRupeeMax();
}

inline void dComIfGs_offGetMagicUseFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().offMagicFlag(0);
}

inline s32 dComIfGs_isGetMagicUseFlag() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusA().isMagicFlag(0);
}

inline f32 dComIfGs_getTime() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().getTime();
}

inline void dComIfGs_setTime(f32 i_time) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().setTime(i_time);
}

inline u16 dComIfGs_getDate() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().getDate();
}

inline void dComIfGs_setDate(u16 i_date) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().setDate(i_date);
}

inline void dComIfGs_onDarkClearLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().onDarkClearLV(i_no);
}

inline void dComIfGs_offDarkClearLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().offDarkClearLV(i_no);
}

inline BOOL dComIfGs_isDarkClearLV(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().isDarkClearLV(i_no);
}

inline void dComIfGs_onTransformLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().onTransformLV(i_no);
}

inline void dComIfGs_offTransformLV(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().offTransformLV(i_no);
}

inline BOOL dComIfGs_isTransformLV(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerStatusB().isTransformLV(i_no);
}

#if TARGET_PC
inline cXyz dComIfGs_getHorseRestartPos() {
#else
inline cXyz& dComIfGs_getHorseRestartPos() {
#endif
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getPos();
}

inline s16 dComIfGs_getHorseRestartAngleY() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getAngleY();
}

inline const char* dComIfGs_getHorseRestartStageName() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getStageName();
}

inline s8 dComIfGs_getHorseRestartRoomNo() {
    return g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().getRoomNo();
}

inline void dComIfGs_setHorseRestart(const char* i_stageName, cXyz& i_pos, s16 i_angle,
                                     s8 i_roomNo) {
    g_dComIfG_gameInfo.info.getPlayer().getHorsePlace().set(i_stageName, i_pos, i_angle, i_roomNo);
}

#if TARGET_PC
inline cXyz dComIfGs_getPlayerFieldLastStayPos() {
#else
inline cXyz& dComIfGs_getPlayerFieldLastStayPos() {
#endif
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getPos();
}

inline s16 dComIfGs_getPlayerFieldLastStayAngleY() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getAngleY();
}

inline char* dComIfGs_getPlayerFieldLastStayName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getName();
}

inline u8 dComIfGs_getPlayerFieldLastStayRegionNo() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getRegionNo();
}

inline bool dComIfGs_isPlayerFieldLastStayFieldDataExistFlag() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().isFieldDataExistFlag();
}

inline void dComIfGs_offPlayerFieldLastStayFieldDataExistFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().offFieldDataExistFlag();
}

inline void dComIfGs_onPlayerFieldLastStayFieldDataExistFlag() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().onFieldDataExistFlag();
}

inline BOOL dComIfGs_isRegionBit(int i_region) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().isRegionBit(i_region);
}

inline void dComIfGs_onRegionBit(int i_region) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().onRegionBit(i_region);
}

inline void dComIfGs_setPlayerFieldLastStayInfo(const char* i_stage, cXyz& i_pos, s16 i_angle,
                                                s8 i_point, u8 i_region) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().set(i_stage, i_pos, i_angle,
                                                                         i_point, i_region);
}

#if TARGET_PC
inline cXyz dComIfGs_getLastWarpMarkPlayerPos() {
#else
inline cXyz& dComIfGs_getLastWarpMarkPlayerPos() {
#endif
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getPos();
}

inline s16 dComIfGs_getLastWarpMarkPlayerAngleY() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getAngleY();
}

inline const char* dComIfGs_getLastWarpMarkStageName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getName();
}

inline u8 dComIfGs_getLastWarpMarkRoomNo() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getRoomNo();
}

inline s8 dComIfGs_getLastWarpAcceptStage() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().getWarpAcceptStage();
}

inline void dComIfGs_setLastWarpAcceptStage(s8 param_0) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().setWarpAcceptStage(param_0);
}

inline void dComIfGs_resetLastWarpAcceptStage() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerLastMarkInfo().resetWarpAcceptStage();
}

inline void dComIfGs_setItem(int i_slotNo, u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setItem(i_slotNo, i_itemNo);
}

inline u8 dComIfGs_getItem(int i_slotNo, bool i_checkCombo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().getItem(i_slotNo, i_checkCombo);
}

inline void dComIfGs_setLineUpItem() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setLineUpItem();
}

inline u8 dComIfGs_getLineUpItem(int i_slotNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().getLineUpItem(i_slotNo);
}

inline void dComIfGs_setBottleItemIn(u8 curItem, u8 newItem) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setBottleItemIn(curItem, newItem);
}

inline void dComIfGs_setEmptyBottleItemIn(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBottleItemIn(i_itemNo);
}

inline void dComIfGs_setEmptyBottle() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBottle();
}

inline void dComIfGs_setEmptyBottle(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBottle(i_itemNo);
}

inline void dComIfGs_setEquipBottleItemIn(u8 i_curItem, u8 i_newItem) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEquipBottleItemIn(i_curItem, i_newItem);
}

inline void dComIfGs_setEquipBottleItemEmpty(u8 i_curItem) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEquipBottleItemEmpty(i_curItem);
}

inline u8 dComIfGs_checkBottle(u8 i_itemNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkBottle(i_itemNo);
}

inline u8 dComIfGs_checkInsectBottle() {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkInsectBottle();
}

inline u8 dComIfGs_checkEmptyBottle() {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkEmptyBottle();
}

inline void dComIfGs_setEmptyBombBagItemIn(u8 i_newBomb, bool i_setNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBagItemIn(i_newBomb, i_setNum);
}

inline void dComIfGs_setEmptyBombBagItemIn(u8 i_newBomb, u8 i_bombNum, bool i_setNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBagItemIn(i_newBomb, i_bombNum,
                                                                        i_setNum);
}

inline void dComIfGs_setEmptyBombBag() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBag();
}

inline void dComIfGs_setEmptyBombBag(u8 i_newBomb, u8 i_bombNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setEmptyBombBag(i_newBomb, i_bombNum);
}

inline u8 dComIfGs_checkBombBag(u8 i_itemNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getItem().checkBombBag(i_itemNo);
}

inline void dComIfGs_setWarashibeItem(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setWarashibeItem(i_itemNo);
}

inline void dComIfGs_setRodTypeLevelUp() {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setRodTypeLevelUp();
}

inline void dComIfGs_setBaitItem(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getItem().setBaitItem(i_item);
}

inline void dComIfGs_onItemFirstBit(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getGetItem().onFirstBit(i_itemNo);
}

inline void dComIfGs_offItemFirstBit(u8 i_itemNo) {
    g_dComIfG_gameInfo.info.getPlayer().getGetItem().offFirstBit(i_itemNo);
}

inline int dComIfGs_isItemFirstBit(u8 i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getGetItem().isFirstBit(i_no);
}

inline u8 dComIfGs_getArrowNum() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getArrowNum();
}

inline void dComIfGs_setArrowNum(u8 i_arrowNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setArrowNum(i_arrowNum);
}

inline u8 dComIfGs_getPachinkoNum() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getPachinkoNum();
}

inline void dComIfGs_setPachinkoNum(u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setPachinkoNum(i_num);
}

inline u8 dComIfGs_getPachinkoMax() {
    return 50;
}

inline void dComIfGs_setBombNum(u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setBombNum(0, i_num);
}

inline void dComIfGs_setBombNum(u8 i_bagIdx, u8 i_bombNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setBombNum(i_bagIdx, i_bombNum);
}

inline u8 dComIfGs_getBombNum(u8 i_bagIdx) {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getBombNum(i_bagIdx);
}

inline void dComIfGs_setBottleNum(u8 i_bottleIdx, u8 i_bottleNum) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().setBottleNum(i_bottleIdx, i_bottleNum);
}

inline void dComIfGs_addBottleNum(u8 i_bottleIdx, s16 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getItemRecord().addBottleNum(i_bottleIdx, i_num);
}

inline u8 dComIfGs_getBottleNum(u8 i_bottleIdx) {
    return g_dComIfG_gameInfo.info.getPlayer().getItemRecord().getBottleNum(i_bottleIdx);
}

inline u8 dComIfGs_getArrowMax() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemMax().getArrowNum();
}

inline void dComIfGs_setArrowMax(u8 i_arrowMax) {
    g_dComIfG_gameInfo.info.getPlayer().getItemMax().setArrowNum(i_arrowMax);
}

inline void dComIfGs_setBombMax(u8 i_max) {
    g_dComIfG_gameInfo.info.getPlayer().getItemMax().setBombNum(0, i_max);
}

inline void dComIfGs_setBombMax(u8 i_type, u8 i_max) {
    g_dComIfG_gameInfo.info.getPlayer().getItemMax().setBombNum(i_type, i_max);
}

inline u8 dComIfGs_getBombMax() {
    return g_dComIfG_gameInfo.info.getPlayer().getItemMax().getBombNum(0);
}

inline u8 dComIfGs_getBombMax(u8 i_bombType) {
    return g_dComIfG_gameInfo.info.getPlayer().getItemMax().getBombNum(i_bombType);
}

inline void dComIfGs_setPohSpiritNum(u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setPohNum(i_num);
}

inline u8 dComIfGs_getPohSpiritNum() {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().getPohNum();
}

inline void dComIfGs_addPohSpiritNum() {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().addPohNum();
}

inline BOOL dComIfGs_isCollectClothes(u8 i_clothesNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(COLLECT_CLOTHING, i_clothesNo);
}

inline void dComIfGs_setCollectClothes(u8 i_clothesNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setCollect(COLLECT_CLOTHING, i_clothesNo);
}

inline void dComIfGs_setCollectSword(u8 i_swordNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setCollect(COLLECT_SWORD, i_swordNo);
}

inline void dComIfGs_setCollectShield(u8 i_shieldNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().setCollect(COLLECT_SHIELD, i_shieldNo);
}

inline BOOL dComIfGs_isCollectClothing(u8 i_clothesNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(COLLECT_CLOTHING,
                                                                      i_clothesNo);
}

inline void dComIfGs_offCollectClothes(u8 i_clothesNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollect(COLLECT_CLOTHING, i_clothesNo);
}

inline BOOL dComIfGs_isCollectSword(u8 i_swordNo) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(COLLECT_SWORD, i_swordNo);
}

inline void dComIfGs_offCollectSword(u8 i_swordNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollect(COLLECT_SWORD, i_swordNo);
}

inline BOOL dComIfGs_isCollectShield(u8 i_item) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollect(COLLECT_SHIELD, i_item);
}

inline void dComIfGs_offCollectShield(u8 i_shieldNo) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollect(COLLECT_SHIELD, i_shieldNo);
}

inline void dComIfGs_onCollectCrystal(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().onCollectCrystal(i_item);
}

inline void dComIfGs_offCollectCrystal(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollectCrystal(i_item);
}

inline bool dComIfGs_isCollectCrystal(u8 i_item) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollectCrystal(i_item);
}

inline void dComIfGs_onCollectMirror(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().onCollectMirror(i_item);
}

inline void dComIfGs_offCollectMirror(u8 i_item) {
    g_dComIfG_gameInfo.info.getPlayer().getCollect().offCollectMirror(i_item);
}

inline bool dComIfGs_isCollectMirror(u8 i_item) {
    return g_dComIfG_gameInfo.info.getPlayer().getCollect().isCollectMirror(i_item);
}

inline void dComIfGs_setLightDropNum(u8 i_level, u8 i_num) {
    g_dComIfG_gameInfo.info.getPlayer().getLightDrop().setLightDropNum(i_level, i_num);
}

inline u8 dComIfGs_getLightDropNum(u8 i_nowLevel) {
    return g_dComIfG_gameInfo.info.getPlayer().getLightDrop().getLightDropNum(i_nowLevel);
}

inline void dComIfGs_onLightDropGetFlag(u8 i_nowLevel) {
    g_dComIfG_gameInfo.info.getPlayer().getLightDrop().onLightDropGetFlag(i_nowLevel);
}

inline void dComIfGs_offLightDropGetFlag(u8 i_level) {
    g_dComIfG_gameInfo.info.getPlayer().getLightDrop().offLightDropGetFlag(i_level);
}

inline BOOL dComIfGs_isLightDropGetFlag(u8 i_nowLevel) {
    return g_dComIfG_gameInfo.info.getPlayer().getLightDrop().isLightDropGetFlag(i_nowLevel);
}

inline void dComIfGs_onLetterGetFlag(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().onLetterGetFlag(i_no);
}

inline BOOL dComIfGs_isLetterGetFlag(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().isLetterGetFlag(i_no);
}

inline void dComIfGs_onLetterReadFlag(int i_no) {
    g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().onLetterReadFlag(i_no);
}

inline int dComIfGs_isLetterReadFlag(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().isLetterReadFlag(i_no);
}

inline u8 dComIfGs_getGetNumber(int i_no) {
    return g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().getGetNumber(i_no);
}

inline void dComIfGs_setGetNumber(int i_no, u8 i_value) {
    g_dComIfG_gameInfo.info.getPlayer().getLetterInfo().setGetNumber(i_no, i_value);
}

inline void dComIfGs_addFishNum(u8 param_0) {
    g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().addFishCount(param_0);
}

inline u16 dComIfGs_getFishNum(u8 param_0) {
    return g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().getFishCount(param_0);
}

inline u8 dComIfGs_getFishSize(u8 param_0) {
    return g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().getMaxSize(param_0);
}

inline void dComIfGs_setFishSize(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.info.getPlayer().getFishingInfo().setMaxSize(param_0, param_1);
}

inline OSTime dComIfGs_getTotalTime() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getTotalTime();
}

inline void dComIfGs_addDeathCount() {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().addDeathCount();
}

inline char* dComIfGs_getPlayerName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getPlayerName();
}

inline void dComIfGs_setPlayerName(const char* i_name) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().setPlayerName(i_name);
}

inline char* dComIfGs_getHorseName() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getHorseName();
}

inline void dComIfGs_setHorseName(const char* i_name) {
    g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().setHorseName(i_name);
}

inline u8 dComIfGs_getClearCount() {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerInfo().getClearCount();
}

// Ruby inline names are from TWW debug.
inline u8 dComIfGs_getOptRuby() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getRuby();
}

inline void dComIfGs_setOptRuby(u8 i_ruby) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setRuby(i_ruby);
}

inline u8 dComIfGs_getOptAttentionType() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getAttentionType();
}

inline void dComIfGs_setOptAttentionType(u8 i_attentionType) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setAttentionType(i_attentionType);
}

inline void dComIfGs_setOptCalibrateDist(u16 i_calibrateDist) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setCalibrateDist(i_calibrateDist);
}

inline void dComIfGs_setOptCalValue(s8 i_calValue) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setCalValue(i_calValue);
}

inline u8 dComIfGs_getOptCameraControl() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getCameraControl();
}

inline void dComIfGs_setOptCameraControl(u8 i_cameraControl) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setCameraControl(i_cameraControl);
}

inline u8 dComIfGs_getOptPointer() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getPointer();
}

inline void dComIfGs_setOptPointer(u8 i_pointer) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setPointer(i_pointer);
}

inline u8 dComIfGs_checkOptVibration() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().checkVibration();
}

inline u8 dComIfGs_getOptSound() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getSound();
}

inline void dComIfGs_setOptSound(u8 i_mode) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setSound(i_mode);
}

inline u8 dComIfGs_getOptVibration() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getVibration();
}

inline void dComIfGs_setOptVibration(u8 i_status) {
    g_dComIfG_gameInfo.info.getPlayer().getConfig().setVibration(i_status);
}

inline u8 dComIfGs_getPalLanguage() {
    return g_dComIfG_gameInfo.info.getPlayer().getConfig().getPalLanguage();
}

inline BOOL dComIfGs_isSaveTbox(int i_stageNo, int i_no) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isTbox(i_no);
}

inline void dComIfGs_onSaveSwitch(int i_stageNo, int i_no) {
    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().onSwitch(i_no);
}

inline void dComIfGs_offSaveSwitch(int i_stageNo, int i_no) {
    g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().offSwitch(i_no);
}

inline BOOL dComIfGs_isSaveSwitch(int i_stageNo, int i_no) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave(i_stageNo).getBit().isSwitch(i_no);
}

inline void dComIfGs_onSaveVisitedRoom(int param_0, int i_roomNo) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave2(param_0)->onVisitedRoom(i_roomNo);
}

inline void dComIfGs_offSaveVisitedRoom(int param_0, int i_roomNo) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave2(param_0)->offVisitedRoom(i_roomNo);
}

inline BOOL dComIfGs_isSaveVisitedRoom(int param_0, int i_roomNo) {
    return g_dComIfG_gameInfo.info.getSavedata().getSave2(param_0)->isVisitedRoom(i_roomNo);
}

inline void* dComIfGs_getPEventBit() {
    return g_dComIfG_gameInfo.info.getEvent().getPEventBit();
}

inline void dComIfGs_onEventBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getEvent().onEventBit(i_flag);
}

inline void dComIfGs_offEventBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getEvent().offEventBit(i_flag);
}

inline BOOL dComIfGs_isEventBit(const u16 i_flag) {
    return g_dComIfG_gameInfo.info.getEvent().isEventBit(i_flag);
}

inline void dComIfGs_setEventReg(u16 reg, u8 value) {
    g_dComIfG_gameInfo.info.getEvent().setEventReg(reg, value);
}

inline u8 dComIfGs_getEventReg(u16 reg) {
    return g_dComIfG_gameInfo.info.getEvent().getEventReg(reg);
}

inline int dComIfGs_getHookGameTime() {
    return g_dComIfG_gameInfo.info.getMiniGame().getHookGameTime();
}

inline void dComIfGs_setHookGameTime(u32 i_time) {
    g_dComIfG_gameInfo.info.getMiniGame().setHookGameTime(i_time);
}

inline u32 dComIfGs_getBalloonScore() {
    return g_dComIfG_gameInfo.info.getMiniGame().getBalloonScore();
}

inline void dComIfGs_setBalloonScore(u32 i_score) {
    g_dComIfG_gameInfo.info.getMiniGame().setBalloonScore(i_score);
}

inline int dComIfGs_getRaceGameTime() {
    return g_dComIfG_gameInfo.info.getMiniGame().getRaceGameTime();
}

inline void dComIfGs_setRaceGameTime(int i_time) {
    g_dComIfG_gameInfo.info.getMiniGame().setRaceGameTime(i_time);
}

inline u8 dComIfGs_getKeyNum() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().getKeyNum();
}

inline void dComIfGs_setKeyNum(u8 i_keyNum) {
    g_dComIfG_gameInfo.info.getMemory().getBit().setKeyNum(i_keyNum);
}

inline void dComIfGs_onDungeonItemMap() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemMap();
}

inline void dComIfGs_offDungeonItemMap() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemMap();
}

inline s32 dComIfGs_isDungeonItemMap() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemMap();
}

inline void dComIfGs_onDungeonItemCompass() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemCompass();
}

inline void dComIfGs_offDungeonItemCompass() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemCompass();
}

inline s32 dComIfGs_isDungeonItemCompass() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemCompass();
}

inline void dComIfGs_onDungeonItemBossKey() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemBossKey();
}

inline void dComIfGs_offDungeonItemBossKey() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemBossKey();
}

inline s32 dComIfGs_isDungeonItemBossKey() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemBossKey();
}

inline void dComIfGs_onStageBossEnemy() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageBossEnemy();
}

inline void dComIfGs_offStageBossEnemy() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageBossEnemy();
}

inline s32 dComIfGs_isStageBossEnemy() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageBossEnemy();
}

inline void dComIfGs_onStageLife() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageLife();
}

inline void dComIfGs_offStageLife() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageLife();
}

inline s32 dComIfGs_isStageLife() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageLife();
}

inline void dComIfGs_onStageBossDemo() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageBossDemo();
}

inline void dComIfGs_offStageBossDemo() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageBossDemo();
}

inline s32 dComIfGs_isStageBossDemo() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageBossDemo();
}

inline void dComIfGs_onDungeonItemWarp() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onDungeonItemWarp();
}

inline void dComIfGs_offDungeonItemWarp() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offDungeonItemWarp();
}

inline s32 dComIfGs_isDungeonItemWarp() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isDungeonItemWarp();
}

inline void dComIfGs_onStageMiddleBoss() {
    g_dComIfG_gameInfo.info.getMemory().getBit().onStageBossEnemy2();
}

inline void dComIfGs_offStageMiddleBoss() {
    g_dComIfG_gameInfo.info.getMemory().getBit().offStageBossEnemy2();
}

inline BOOL dComIfGs_isStageMiddleBoss() {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isStageBossEnemy2();
}

inline void dComIfGs_onTbox(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().onTbox(i_no);
}

inline void dComIfGs_offTbox(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().offTbox(i_no);
}

inline BOOL dComIfGs_isTbox(int i_no) {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isTbox(i_no);
}

inline void dComIfGs_onSaveSwitch(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().onSwitch(i_no);
}

inline void dComIfGs_offSaveSwitch(int i_no) {
    g_dComIfG_gameInfo.info.getMemory().getBit().offSwitch(i_no);
}

inline BOOL dComIfGs_isSaveSwitch(int i_no) {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isSwitch(i_no);
}

inline BOOL dComIfGs_isSaveItem(int i_no) {
    return g_dComIfG_gameInfo.info.getMemory().getBit().isItem(i_no);
}

inline void dComIfGs_onSaveDunSwitch(int i_swNo) {
    g_dComIfG_gameInfo.info.getDan().onSwitch(i_swNo);
}

inline void dComIfGs_offSaveDunSwitch(int i_swNo) {
    g_dComIfG_gameInfo.info.getDan().offSwitch(i_swNo);
}

inline BOOL dComIfGs_isSaveDunSwitch(int i_no) {
    return g_dComIfG_gameInfo.info.getDan().isSwitch(i_no);
}

inline BOOL dComIfGs_isSaveDunItem(int i_no) {
    return g_dComIfG_gameInfo.info.getDan().isItem(i_no);
}

inline void dComIfGs_resetDan() {
    g_dComIfG_gameInfo.info.resetDan();
}

inline void dComIfGs_initDan(s8 i_stageNo) {
    g_dComIfG_gameInfo.info.initDan(i_stageNo);
}

inline void dComIfGs_clearRoomSwitch(int i_zoneNo) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().clearRoomSwitch();
}

inline void dComIfGs_clearRoomItem(int i_zoneNo) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().clearRoomItem();
}

inline void dComIfGs_onSvZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onSwitch(i_swBit);
}

inline void dComIfGs_offSvZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offSwitch(i_swBit);
}

inline BOOL dComIfGs_isSvZoneSwitch(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isSwitch(i_swBit);
}

inline void dComIfGs_onSvOneZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onOneSwitch(i_swBit);
}

inline void dComIfGs_offSvOneZoneSwitch(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offOneSwitch(i_swBit);
}

inline BOOL dComIfGs_isSvOneZoneSwitch(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isOneSwitch(i_swBit);
}

inline void dComIfGs_onSvZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onItem(i_swBit);
}

inline void dComIfGs_offSvZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offItem(i_swBit);
}

inline BOOL dComIfGs_isSvZoneItem(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isItem(i_swBit);
}

inline void dComIfGs_onSvOneZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().onOneItem(i_swBit);
}

inline void dComIfGs_offSvOneZoneItem(int i_zoneNo, int i_swBit) {
    g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().offOneItem(i_swBit);
}

inline BOOL dComIfGs_isSvOneZoneItem(int i_zoneNo, int i_swBit) {
    return g_dComIfG_gameInfo.info.getZone(i_zoneNo).getBit().isOneItem(i_swBit);
}

inline void dComIfGs_removeZone(int i_zoneNo) {
    g_dComIfG_gameInfo.info.removeZone(i_zoneNo);
}

inline s8 dComIfGs_getRestartRoomNo() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomNo();
}

inline s16 dComIfGs_getStartPoint() {
    return g_dComIfG_gameInfo.info.getRestart().getStartPoint();
}

inline void dComIfGs_setStartPoint(s16 i_point) {
    g_dComIfG_gameInfo.info.getRestart().setStartPoint(i_point);
}

inline s16 dComIfGs_getRestartRoomAngleY() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomAngleY();
}

inline const cXyz& dComIfGs_getRestartRoomPos() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomPos();
}

inline u32 dComIfGs_getRestartRoomParam() {
    return g_dComIfG_gameInfo.info.getRestart().getRoomParam();
}

inline void dComIfGs_setRestartRoomParam(u32 i_param) {
    g_dComIfG_gameInfo.info.getRestart().setRoomParam(i_param);
}

inline f32 dComIfGs_getLastSceneSpeedF() {
    return g_dComIfG_gameInfo.info.getRestart().getLastSpeedF();
}

inline u32 dComIfGs_getLastSceneMode() {
    return g_dComIfG_gameInfo.info.getRestart().getLastMode();
}

inline s16 dComIfGs_getLastSceneAngleY() {
    return g_dComIfG_gameInfo.info.getRestart().getLastAngleY();
}

inline void dComIfGs_setRestartRoom(const cXyz& i_position, s16 i_angle, s8 i_roomNo) {
    g_dComIfG_gameInfo.info.getRestart().setRoom(i_position, i_angle, i_roomNo);
}

inline void dComIfGs_onTmpBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getTmp().onEventBit(i_flag);
}

inline void dComIfGs_offTmpBit(const u16 i_flag) {
    g_dComIfG_gameInfo.info.getTmp().offEventBit(i_flag);
}

inline BOOL dComIfGs_isTmpBit(const u16 i_flag) {
    return g_dComIfG_gameInfo.info.getTmp().isEventBit(i_flag);
}

inline void dComIfGs_setTmpReg(u16 i_reg, u8 i_no) {
    g_dComIfG_gameInfo.info.getTmp().setEventReg(i_reg, i_no);
}

inline u8 dComIfGs_getTmpReg(const u16 i_reg) {
    return g_dComIfG_gameInfo.info.getTmp().getEventReg(i_reg);
}

inline dSv_turnRestart_c& dComIfGs_getTurnRestart() {
    return g_dComIfG_gameInfo.info.getTurnRestart();
}

inline const cXyz& dComIfGs_getTurnRestartPos() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getPos();
}

inline u32 dComIfGs_getTurnRestartParam() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getParam();
}

inline s16 dComIfGs_getTurnRestartAngleY() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getAngleY();
}

inline s8 dComIfGs_getTurnRestartRoomNo() {
    return g_dComIfG_gameInfo.info.getTurnRestart().getRoomNo();
}

inline void dComIfGs_setTurnRestart(const cXyz& i_position, s16 i_angle, s8 param_2, u32 i_param) {
    g_dComIfG_gameInfo.info.getTurnRestart().set(i_position, i_angle, param_2, i_param);
}

inline u8 dComIfGs_getDataNum() {
    return g_dComIfG_gameInfo.info.getDataNum();
}

inline void dComIfGs_setDataNum(u8 i_num) {
    return g_dComIfG_gameInfo.info.setDataNum(i_num);
}

inline u8 dComIfGs_getNewFile() {
    return g_dComIfG_gameInfo.info.getNewFile();
}

inline void dComIfGs_setNewFile(u8 i_fileNo) {
    return g_dComIfG_gameInfo.info.setNewFile(i_fileNo);
}

inline u8 dComIfGs_getNoFile() {
    return g_dComIfG_gameInfo.info.getNoFile();
}

inline void dComIfGs_setNoFile(u8 i_file) {
    g_dComIfG_gameInfo.info.setNoFile(i_file);
}

inline s64 dComIfGs_getSaveStartTime() {
    return g_dComIfG_gameInfo.info.getStartTime();
}

inline void dComIfGs_setSaveStartTime(s64 i_time) {
    g_dComIfG_gameInfo.info.setStartTime(i_time);
}

inline s64 dComIfGs_getSaveTotalTime() {
    return g_dComIfG_gameInfo.info.getSaveTotalTime();
}

inline void dComIfGs_setSaveTotalTime(s64 i_time) {
    g_dComIfG_gameInfo.info.setSaveTotalTime(i_time);
}

inline void dComIfGs_init() {
    g_dComIfG_gameInfo.info.init();
}

inline void dComIfGs_getSave(int i_stageNo) {
    g_dComIfG_gameInfo.info.getSave(i_stageNo);
}

inline void dComIfGs_putSave(int i_stageNo) {
    g_dComIfG_gameInfo.info.putSave(i_stageNo);
}

inline void dComIfGs_initZone() {
    g_dComIfG_gameInfo.info.initZone();
}

inline int dComIfGs_createZone(int roomNo) {
    return g_dComIfG_gameInfo.info.createZone(roomNo);
}

inline void dComIfGs_onSwitch(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.onSwitch(i_no, i_roomNo);
}

inline void dComIfGs_offSwitch(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.offSwitch(i_no, i_roomNo);
}

inline BOOL dComIfGs_isSwitch(int i_no, int i_roomNo) {
    return g_dComIfG_gameInfo.info.isSwitch(i_no, i_roomNo);
}

inline void dComIfGs_revSwitch(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.revSwitch(i_no, i_roomNo);
}

inline void dComIfGs_onItem(int i_bitNo, int i_roomNo) {
    g_dComIfG_gameInfo.info.onItem(i_bitNo, i_roomNo);
}

inline bool dComIfGs_isItem(int i_bitNo, int i_roomNo) {
    return g_dComIfG_gameInfo.info.isItem(i_bitNo, i_roomNo);
}

inline void dComIfGs_onActor(int i_bitNo, int i_roomNo) {
    g_dComIfG_gameInfo.info.onActor(i_bitNo, i_roomNo);
}

inline void dComIfGs_offActor(int i_no, int i_roomNo) {
    g_dComIfG_gameInfo.info.offActor(i_no, i_roomNo);
}

inline BOOL dComIfGs_isActor(int i_no, int i_roomNo) {
    return g_dComIfG_gameInfo.info.isActor(i_no, i_roomNo);
}

inline void dComIfGs_setMemoryToCard(u8* i_saveData, int i_dataNum) {
    g_dComIfG_gameInfo.info.memory_to_card((char*)i_saveData, i_dataNum);
}

inline void dComIfGs_setCardToMemory(u8* card_ptr, int dataNum) {
    g_dComIfG_gameInfo.info.card_to_memory((char*)card_ptr, dataNum);
}

inline void dComIfGs_setInitDataToCard(u8* i_saveData, int i_dataNum) {
    g_dComIfG_gameInfo.info.initdata_to_card((char*)i_saveData, i_dataNum);
}

inline u8 dComIfGs_getWolfAbility(int i_idx) {
    return g_dComIfG_gameInfo.play.getWolfAbility(i_idx);
}

inline s8 dComIfGs_Grass_hide_Check() {
    return g_dComIfG_gameInfo.field_0x1ddfc;
}

inline dStage_startStage_c* dComIfGp_getStartStage() {
    return g_dComIfG_gameInfo.play.getStartStage();
}

inline void dComIfGp_setStartStage(dStage_startStage_c* p_startStage) {
    g_dComIfG_gameInfo.play.setStartStage(p_startStage);
}

inline const char* dComIfGp_getStartStageName() {
    return g_dComIfG_gameInfo.play.getStartStageName();
}

inline s16 dComIfGp_getStartStagePoint() {
    return g_dComIfG_gameInfo.play.getStartStagePoint();
}

inline s8 dComIfGp_getStartStageRoomNo() {
    return g_dComIfG_gameInfo.play.getStartStageRoomNo();
}

inline s8 dComIfGp_getStartStageLayer() {
    return g_dComIfG_gameInfo.play.getStartStageLayer();
}

inline void dComIfGp_setStartStageLayer(s8 layer) {
    g_dComIfG_gameInfo.play.setStartStageLayer(layer);
}

inline s8 dComIfGp_getStartStageDarkArea() {
    return g_dComIfG_gameInfo.play.getStartStageDarkArea();
}

inline void dComIfGp_setStartStageDarkArea(s8 darkArea) {
    g_dComIfG_gameInfo.play.setStartStageDarkArea(darkArea);
}

inline dStage_startStage_c* dComIfGp_getNextStartStage() {
    return g_dComIfG_gameInfo.play.getNextStartStage();
}

inline const char* dComIfGp_getNextStageName() {
    return g_dComIfG_gameInfo.play.getNextStageName();
}

inline s16 dComIfGp_getNextStagePoint() {
    return g_dComIfG_gameInfo.play.getNextStagePoint();
}

inline s16 dComIfGp_getNextStageRoomNo() {
    return g_dComIfG_gameInfo.play.getNextStageRoomNo();
}

inline s16 dComIfGp_getNextStageLayer() {
    return g_dComIfG_gameInfo.play.getNextStageLayer();
}

inline BOOL dComIfGp_isEnableNextStage() {
    return g_dComIfG_gameInfo.play.isEnableNextStage();
}

inline void dComIfGp_offEnableNextStage() {
    g_dComIfG_gameInfo.play.offEnableNextStage();
}

inline s8 dComIfGp_getNextStageWipe() {
    return g_dComIfG_gameInfo.play.getNextStageWipe();
}

inline u8 dComIfGp_getNextStageWipeSpeed() {
    return g_dComIfG_gameInfo.play.getNextStageWipeSpeed();
}

inline dStage_stageDt_c* dComIfGp_getStage() {
    return &g_dComIfG_gameInfo.play.getStage();
}

inline roomRead_class* dComIfGp_getStageRoom() {
    return g_dComIfG_gameInfo.play.getStage().getRoom();
}

inline stage_palette_info_class* dComIfGp_getStagePaletteInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPaletteInfo();
}

inline stage_pselect_info_class* dComIfGp_getStagePselectInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPselectInfo();
}

inline stage_envr_info_class* dComIfGp_getStageEnvrInfo() {
    return g_dComIfG_gameInfo.play.getStage().getEnvrInfo();
}

inline stage_vrboxcol_info_class* dComIfGp_getStageVrboxcolInfo() {
    return g_dComIfG_gameInfo.play.getStage().getVrboxcolInfo();
}

inline stage_plight_info_class* dComIfGp_getStagePlightInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPlightInfo();
}

inline int dComIfGp_getStagePaletteNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPaletteNumInfo();
}

inline int dComIfGp_getStagePselectNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPselectNumInfo();
}

inline int dComIfGp_getStageEnvrNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getEnvrNumInfo();
}

inline int dComIfGp_getStageVrboxcolNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getVrboxcolNumInfo();
}

inline int dComIfGp_getStagePlightNumInfo() {
    return g_dComIfG_gameInfo.play.getStage().getPlightNumInfo();
}

inline stage_stag_info_class* dComIfGp_getStageStagInfo() {
    return g_dComIfG_gameInfo.play.getStage().getStagInfo();
}

inline stage_scls_info_dummy_class* dComIfGp_getStageSclsInfo() {
    return g_dComIfG_gameInfo.play.getStage().getSclsInfo();
}

inline dStage_Multi_c* dComIfGp_getMulti() {
    return g_dComIfG_gameInfo.play.getStage().getMulti();
}

inline void dComIfGp_setOldMulti() {
    g_dComIfG_gameInfo.play.getStage().setOldMulti();
}

inline void dComIfGp_resetOldMulti() {
    g_dComIfG_gameInfo.play.getStage().resetOldMulti();
}

inline s16 dComIfGp_getStageWorldRollAngleX() {
    return g_dComIfG_gameInfo.play.getStage().getWorldRollAngleX();
}

inline s16 dComIfGp_getStageWorldRollDirAngleY() {
    return g_dComIfG_gameInfo.play.getStage().getWorldRollDirAngleY();
}

inline int dComIfGp_roomControl_getStayNo() {
    return dStage_roomControl_c::getStayNo();
}

inline BOOL dComIfGp_roomControl_getTimePass() {
    return g_dComIfG_gameInfo.play.getRoomControl()->GetTimePass();
}

inline void dComIfGp_roomControl_setTimePass(int isPassing) {
    g_dComIfG_gameInfo.play.getRoomControl()->SetTimePass(isPassing);
}

inline dKy_tevstr_c* dComIfGp_roomControl_getTevStr(int i_roomNo) {
    return g_dComIfG_gameInfo.play.getRoomControl()->getTevStr(i_roomNo);
}

inline void dComIfGp_roomControl_setStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->setStatusFlag(i_roomNo, flag);
}

inline void dComIfGp_roomControl_onStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->onStatusFlag(i_roomNo, flag);
}

inline void dComIfGp_roomControl_offStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->offStatusFlag(i_roomNo, flag);
}

inline u8 dComIfGp_roomControl_checkStatusFlag(int i_roomNo, u8 flag) {
    return g_dComIfG_gameInfo.play.getRoomControl()->checkStatusFlag(i_roomNo, flag);
}

inline s32 dComIfGp_roomControl_getZoneNo(int i_roomNo) {
    return g_dComIfG_gameInfo.play.getRoomControl()->getZoneNo(i_roomNo);
}

inline void dComIfGp_roomControl_setZoneNo(int roomNo, int zoneNo) {
    g_dComIfG_gameInfo.play.getRoomControl()->setZoneNo(roomNo, zoneNo);
}

inline void dComIfGp_roomControl_init() {
    g_dComIfG_gameInfo.play.getRoomControl()->init();
}

inline void dComIfGp_roomControl_initZone() {
    g_dComIfG_gameInfo.play.getRoomControl()->initZone();
}

inline dStage_roomDt_c* dComIfGp_roomControl_getStatusRoomDt(int room_no) {
    return g_dComIfG_gameInfo.play.getRoomControl()->getStatusRoomDt(room_no);
}

inline void dComIfGp_roomControl_setStayNo(int stayNo) {
    g_dComIfG_gameInfo.play.mRoomControl.setStayNo(stayNo);
}

inline BOOL dComIfGp_roomControl_checkRoomDisp(int i_roomNo) {
    return g_dComIfG_gameInfo.play.getRoomControl()->checkRoomDisp(i_roomNo);
}

inline int dComIfGp_roomControl_loadRoom(int param_0, u8* param_1, bool param_2) {
    return g_dComIfG_gameInfo.play.getRoomControl()->loadRoom(param_0, param_1, param_2);
}

inline void dComIfGp_roomControl_zoneCountCheck(int i_roomNo) {
    g_dComIfG_gameInfo.play.getRoomControl()->zoneCountCheck(i_roomNo);
}

inline dEvt_control_c* dComIfGp_getEvent() {
    return g_dComIfG_gameInfo.play.getEvent();
}

inline u16 dComIfGp_event_chkEventFlag(u16 i_flag) {
    return g_dComIfG_gameInfo.play.getEvent()->chkEventFlag(i_flag);
}

inline void dComIfGp_event_onEventFlag(u16 i_flag) {
    g_dComIfG_gameInfo.play.getEvent()->onEventFlag(i_flag);
}

inline void dComIfGp_event_onHindFlag(u16 i_flag) {
    g_dComIfG_gameInfo.play.getEvent()->onHindFlag(i_flag);
}

inline void dComIfGp_event_offHindFlag(u16 i_flag) {
    g_dComIfG_gameInfo.play.getEvent()->offHindFlag(i_flag);
}

inline u8 dComIfGp_event_getMode() {
    return g_dComIfG_gameInfo.play.getEvent()->getMode();
}

inline BOOL dComIfGp_event_runCheck() {
    return g_dComIfG_gameInfo.play.getEvent()->runCheck();
}

inline u16 dComIfGp_event_checkHind(u16 i_hindFlag) {
    if (!dComIfGp_event_runCheck()) {
        return false;
    }
    return g_dComIfG_gameInfo.play.getEvent()->checkHind(i_hindFlag);
}

inline BOOL dComIfGp_event_chkTalkXY() {
    return g_dComIfG_gameInfo.play.getEvent()->chkTalkXY();
}

inline u8 dComIfGp_event_getPreItemNo() {
    return g_dComIfG_gameInfo.play.getEvent()->getPreItemNo();
}

inline f32 dComIfGp_event_getCullRate() {
    return g_dComIfG_gameInfo.play.getEvent()->getCullRate();
}

inline void dComIfGp_event_setCullRate(f32 i_rate) {
    g_dComIfG_gameInfo.play.getEvent()->setCullRate(i_rate);
}

inline int dComIfGp_event_order(u16 i_type, u16 i_prio, u16 i_flags, u16 i_hindFlags, void* i_requestActor,
                                void* i_targetActor, s16 i_eventID, u8 i_mapToolId) {
    return g_dComIfG_gameInfo.play.getEvent()->order(i_type, i_prio, i_flags, i_hindFlags, i_requestActor,
                                                    i_targetActor, i_eventID, i_mapToolId);
}

inline void dComIfGp_event_reset() {
    g_dComIfG_gameInfo.play.getEvent()->reset();
}

inline int dComIfGp_event_moveApproval(void* i_actor) {
    return g_dComIfG_gameInfo.play.getEvent()->moveApproval(i_actor);
}

inline BOOL dComIfGp_event_compulsory(void* i_actor, const char* i_eventname, u16 i_hindFlag) {
    return g_dComIfG_gameInfo.play.getEvent()->compulsory(i_actor, i_eventname, i_hindFlag);
}

inline void dComIfGp_event_remove() {
    g_dComIfG_gameInfo.play.getEvent()->remove();
}

inline fopAc_ac_c* dComIfGp_event_getPt1() {
    return g_dComIfG_gameInfo.play.getEvent()->getPt1();
}

inline fopAc_ac_c* dComIfGp_event_getPt2() {
    return g_dComIfG_gameInfo.play.getEvent()->getPt2();
}

inline fopAc_ac_c* dComIfGp_event_getTalkPartner() {
    return g_dComIfG_gameInfo.play.getEvent()->getPtT();
}

inline void dComIfGp_event_setTalkPartner(void* i_actor) {
    g_dComIfG_gameInfo.play.getEvent()->setPtT(i_actor);
}

inline fopAc_ac_c* dComIfGp_event_getItemPartner() {
    return g_dComIfG_gameInfo.play.getEvent()->getPtI();
}

inline void dComIfGp_event_setItemPartner(void* i_actor) {
    g_dComIfG_gameInfo.play.getEvent()->setPtI(i_actor);
}

inline void dComIfGp_event_setItemPartnerId(fpc_ProcID i_id) {
    g_dComIfG_gameInfo.play.getEvent()->setPtI_Id(i_id);
}

inline fopAc_ac_c* dComIfGp_event_getDoorPartner() {
    return g_dComIfG_gameInfo.play.getEvent()->getPtD();
}

inline void dComIfGp_event_setDoorPartner(void* i_partner) {
    g_dComIfG_gameInfo.play.getEvent()->setPtD(i_partner);
}

inline u8 dComIfGp_event_getGtItm() {
    return g_dComIfG_gameInfo.play.getEvent()->getGtItm();
}

inline void dComIfGp_event_setGtItm(u8 i_itemNo) {
    g_dComIfG_gameInfo.play.getEvent()->setGtItm(i_itemNo);
}

inline dEvent_manager_c& dComIfGp_getEventManager() {
    return g_dComIfG_gameInfo.play.getEvtManager();
}

inline dEvent_manager_c* dComIfGp_getPEvtManager() {
    return &g_dComIfG_gameInfo.play.getEvtManager();
}

inline int dComIfGp_evmng_cameraPlay() {
    return dComIfGp_getPEvtManager()->cameraPlay();
}

inline int dComIfGp_evmng_startDemo(int i_mapToolId) {
    return dComIfGp_getPEvtManager()->setStartDemo(i_mapToolId);
}

inline void dComIfGp_evmng_create() {
    g_dComIfG_gameInfo.play.getEvtManager().create();
}

inline void dComIfGp_evmng_remove() {
    g_dComIfG_gameInfo.play.getEvtManager().remove();
}

inline bool dComIfGp_evmng_existence(s16 eventId) {
    return g_dComIfG_gameInfo.play.getEvtManager().getEventData(eventId) != NULL;
}

inline s16 dComIfGp_evmng_getEventIdx(const char* eventName, u8 mapToolID) {
    return g_dComIfG_gameInfo.play.getEvtManager().getEventIdx(eventName, mapToolID, -1);
}

inline bool dComIfGp_evmng_existence(const char* eventname) {
     return g_dComIfG_gameInfo.play.getEvtManager().getEventData(
         dComIfGp_evmng_getEventIdx(eventname, 0xFF)) != NULL;
}

inline BOOL dComIfGp_evmng_isMapToolCamera(u8 mapToolID) {
    return g_dComIfG_gameInfo.play.getEvtManager().isMapToolCamera(mapToolID, -1);
}

inline BOOL dComIfGp_evmng_startCheck(s16 i_eventId) {
    return g_dComIfG_gameInfo.play.getEvtManager().startCheck(i_eventId);
}

inline BOOL dComIfGp_evmng_startCheck(char const* i_eventname) {
    return g_dComIfG_gameInfo.play.getEvtManager().startCheckOld(i_eventname);
}

inline BOOL dComIfGp_evmng_endCheck(s16 i_eventID) {
    return g_dComIfG_gameInfo.play.getEvtManager().endCheck(i_eventID);
}

inline BOOL dComIfGp_evmng_endCheck(const char* i_eventname) {
    return g_dComIfG_gameInfo.play.getEvtManager().endCheckOld(i_eventname);
}

inline int dComIfGp_evmng_getMyStaffId(const char* i_staffname, fopAc_ac_c* i_actor, int i_tagId) {
    return dComIfGp_getPEvtManager()->getMyStaffId(i_staffname, i_actor, i_tagId);
}

inline int dComIfGp_evmng_getIsAddvance(int i_staffId) {
    return dComIfGp_getPEvtManager()->getIsAddvance(i_staffId);
}

inline int dComIfGp_evmng_getMyActIdx(int i_staffId, char** i_actions, int i_actionNum, BOOL param_3, BOOL param_4) {
    return dComIfGp_getPEvtManager()->getMyActIdx(i_staffId, i_actions, i_actionNum, param_3, param_4);
}

inline f32* dComIfGp_evmng_getMyFloatP(int i_staffId, char* i_dataname) {
    return (f32*)dComIfGp_getPEvtManager()->getMySubstanceP(i_staffId, i_dataname, dEvDtData_c::TYPE_FLOAT);
}

inline cXyz* dComIfGp_evmng_getMyXyzP(int i_staffId, char* i_dataname) {
    return (cXyz*)dComIfGp_getPEvtManager()->getMySubstanceP(i_staffId, i_dataname, dEvDtData_c::TYPE_VEC);
}

inline int* dComIfGp_evmng_getMyIntegerP(int i_staffId, char* i_dataname) {
    return (int*)dComIfGp_getPEvtManager()->getMySubstanceP(i_staffId, i_dataname, dEvDtData_c::TYPE_INT);
}

inline char* dComIfGp_evmng_getMyStringP(int i_staffId, char* i_dataname) {
    return (char*)dComIfGp_getPEvtManager()->getMySubstanceP(i_staffId, i_dataname, dEvDtData_c::TYPE_STRING);
}

inline int dComIfGp_evmng_getMySubstanceNum(int i_staffId, char* i_dataname) {
    return dComIfGp_getPEvtManager()->getMySubstanceNum(i_staffId, i_dataname);
}

inline void dComIfGp_evmng_cutEnd(int i_staffId) {
    dComIfGp_getPEvtManager()->cutEnd(i_staffId);
}

inline void dComIfGp_evmng_setGoal(cXyz* i_pos) {
    dComIfGp_getPEvtManager()->setGoal(i_pos);
}

inline cXyz* dComIfGp_evmng_getGoal() {
    return dComIfGp_getPEvtManager()->getGoal();
}

inline BOOL dComIfGp_evmng_ChkPresentEnd() {
    return dComIfGp_getPEvtManager()->ChkPresentEnd();
}

inline int dComIfGp_evmng_checkStartDemo() {
    return dComIfGp_getPEvtManager()->checkStartDemo();
}

inline dAttention_c* dComIfGp_getAttention() {
    return g_dComIfG_gameInfo.play.getAttention();
}

inline fopAc_ac_c* dComIfGp_att_getZHint() {
    return dComIfGp_getAttention()->getZHintTarget();
}

inline int dComIfGp_att_ZHintRequest(fopAc_ac_c* param_1, int param_2) {
    return dComIfGp_getAttention()->ZHintRequest(param_1, param_2);
}

inline fopAc_ac_c* dComIfGp_att_getCatghTarget() {
    return dComIfGp_getAttention()->getCatghTarget();
}

inline u8 dComIfGp_att_getCatchChgItem() {
    return dComIfGp_getAttention()->getCatchChgItem();
}

inline int dComIfGp_att_CatchRequest(fopAc_ac_c* param_0, u8 param_1, f32 i_horizontalDist,
                                      f32 i_upDist, f32 i_downDist, s16 i_angle, int param_5) {
    return dComIfGp_getAttention()->CatchRequest(param_0, param_1, i_horizontalDist, i_upDist,
                                                 i_downDist, i_angle, param_5);
}

inline fopAc_ac_c* dComIfGp_att_getLookTarget() {
    return dComIfGp_getAttention()->getLookTarget();
}

inline void dComIfGp_att_LookRequest(fopAc_ac_c* param_0, f32 i_horizontalDist, f32 i_upDist,
                                     f32 i_downDist, s16 i_angle, int param_5) {
    dComIfGp_getAttention()->LookRequest(param_0, i_horizontalDist, i_upDist, i_downDist, i_angle,
                                        param_5);
}

inline dVibration_c& dComIfGp_getVibration() {
    return g_dComIfG_gameInfo.play.getVibration();
}

inline JKRAramArchive* dComIfGp_getFieldMapArchive2() {
    return g_dComIfG_gameInfo.play.getFieldMapArchive2();
}

inline void dComIfGp_setFieldMapArchive2(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setFieldMapArchive2(arc);
}

inline JKRArchive* dComIfGp_getMsgArchive(int idx) {
    return g_dComIfG_gameInfo.play.getMsgArchive(idx);
}

inline void dComIfGp_setMsgArchive(int i, JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMsgArchive(i, arc);
}

inline JKRArchive* dComIfGp_getDemoMsgArchive() {
    return g_dComIfG_gameInfo.play.getDemoMsgArchive();
}

inline void dComIfGp_setDemoMsgArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setDemoMsgArchive(arc);
}

inline JKRArchive* dComIfGp_getMeterButtonArchive() {
    return g_dComIfG_gameInfo.play.getMeterButtonArchive();
}

inline void dComIfGp_setMeterButtonArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMeterButtonArchive(arc);
}

inline JKRArchive* dComIfGp_getFontArchive() {
    return g_dComIfG_gameInfo.play.getFontArchive();
}

inline void dComIfGp_setFontArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setFontArchive(arc);
}

inline JKRArchive* dComIfGp_getRubyArchive() {
    return g_dComIfG_gameInfo.play.getRubyArchive();
}

inline void dComIfGp_setRubyArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setRubyArchive(arc);
}

inline JKRArchive* dComIfGp_getAnmArchive() {
    return g_dComIfG_gameInfo.play.getAnmArchive();
}

inline void dComIfGp_setAnmArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setAnmArchive(arc);
}

inline JKRArchive* dComIfGp_getCollectResArchive() {
    return g_dComIfG_gameInfo.play.getCollectResArchive();
}

inline void dComIfGp_setCollectResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setCollectResArchive(arc);
}

inline JKRArchive* dComIfGp_getFmapResArchive() {
    return g_dComIfG_gameInfo.play.getFmapResArchive();
}

inline void dComIfGp_setFmapResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setFmapResArchive(arc);
}

inline JKRArchive* dComIfGp_getDmapResArchive() {
    return g_dComIfG_gameInfo.play.getDmapResArchive();
}

inline void dComIfGp_setDmapResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setDmapResArchive(arc);
}

inline JKRArchive* dComIfGp_getOptionResArchive() {
    return g_dComIfG_gameInfo.play.getOptionResArchive();
}

inline void dComIfGp_setOptionResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setOptionResArchive(arc);
}

inline JKRArchive* dComIfGp_getItemIconArchive() {
    return g_dComIfG_gameInfo.play.getItemIconArchive();
}

inline void dComIfGp_setItemIconArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setItemIconArchive(arc);
}

inline JKRArchive* dComIfGp_getNameResArchive() {
    return g_dComIfG_gameInfo.play.getNameResArchive();
}

inline void dComIfGp_setNameResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setNameResArchive(arc);
}

inline void dComIfGp_setErrorResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setErrorResArchive(arc);
}

inline JKRArchive* dComIfGp_getAllMapArchive() {
    return g_dComIfG_gameInfo.play.getAllMapArchive();
}

inline void dComIfGp_setAllMapArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setAllMapArchive(arc);
}

inline JKRArchive* dComIfGp_getMsgCommonArchive() {
    return g_dComIfG_gameInfo.play.getMsgCommonArchive();
}

inline void dComIfGp_setMsgCommonArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMsgCommonArchive(arc);
}

inline JKRArchive* dComIfGp_getRingResArchive() {
    return g_dComIfG_gameInfo.play.getRingResArchive();
}

inline void dComIfGp_setRingResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setRingResArchive(arc);
}

inline JKRArchive* dComIfGp_getCardIconResArchive() {
    return g_dComIfG_gameInfo.play.getCardIconResArchive();
}

inline void dComIfGp_setCardIconResArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setCardIconResArchive(arc);
}

inline JKRArchive* dComIfGp_getMsgDtArchive() {
    return g_dComIfG_gameInfo.play.getMsgDtArchive();
}

inline JKRArchive* dComIfGp_getMsgDtArchive(int idx) {
    return g_dComIfG_gameInfo.play.getMsgDtArchive(idx);
}

inline void dComIfGp_setMsgDtArchive(int i, JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMsgDtArchive(i, arc);
}

inline JKRArchive* dComIfGp_getMain2DArchive() {
    return g_dComIfG_gameInfo.play.getMain2DArchive();
}

inline void dComIfGp_setMain2DArchive(JKRArchive* arc) {
    g_dComIfG_gameInfo.play.setMain2DArchive(arc);
}

inline JKRExpHeap* dComIfGp_particle_getResHeap() {
    return g_dComIfG_gameInfo.play.getParticle()->getResHeap();
}

inline void dComIfGp_particle_levelExecute(u32 param_0) {
    g_dComIfG_gameInfo.play.getParticle()->levelExecute(param_0);
}

inline void dComIfGp_particle_createCommon(const void* data) {
    g_dComIfG_gameInfo.play.getParticle()->createCommon(data);
}

inline void dComIfGp_particle_readScene(u8 particle_no, mDoDvdThd_toMainRam_c** param_1) {
    g_dComIfG_gameInfo.play.getParticle()->readScene(particle_no, param_1);
}

inline void dComIfGp_particle_createScene(const void* param_0) {
    g_dComIfG_gameInfo.play.getParticle()->createScene(param_0);
}

inline void dComIfGp_particle_removeScene(bool param_0) {
    g_dComIfG_gameInfo.play.getParticle()->removeScene(param_0);
}

inline void dComIfGp_particle_cleanup() {
    g_dComIfG_gameInfo.play.getParticle()->cleanup();
}

inline void dComIfGp_particle_calc3D() {
    g_dComIfG_gameInfo.play.getParticle()->calc3D();
}

inline void dComIfGp_particle_calc2D() {
    g_dComIfG_gameInfo.play.getParticle()->calc2D();
}

inline void dComIfGp_particle_calcMenu() {
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->calcMenu();
    }
}

inline void dComIfGp_particle_draw(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormal(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFog(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalFog(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawP1(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalP1(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawProjection(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawProjection(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawNormalPri0_A(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalPri0_A(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawNormalPri0_B(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawNormalPri0_B(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFogPri0_A(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri0_A(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFogPri0_B(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri0_B(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFogPri1(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri1(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFogPri2(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri2(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFogPri3(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri3(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawFogPri4(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogPri4(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawDarkworld(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawDarkworld(i_drawInfo);
    }
}

inline void dComIfGp_particle_drawScreen(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->drawFogScreen(i_drawInfo);
    }
}

inline void dComIfGp_particle_draw2Dgame(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2Dgame(i_drawInfo);
    }
}

inline void dComIfGp_particle_draw2Dfore(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2Dfore(i_drawInfo);
    }
}

inline void dComIfGp_particle_draw2Dback(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2Dback(i_drawInfo);
    }
}

inline void dComIfGp_particle_draw2DmenuFore(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2DmenuFore(i_drawInfo);
    }
}

inline void dComIfGp_particle_draw2DmenuBack(JPADrawInfo* i_drawInfo) {
    ZoneScoped;
    if (g_dComIfG_gameInfo.play.getParticle() != NULL) {
        g_dComIfG_gameInfo.play.getParticle()->draw2DmenuBack(i_drawInfo);
    }
}

inline void dComIfGp_setHitMark(u16 i_hitmark, fopAc_ac_c* param_1, const cXyz* param_2,
                                const csXyz* param_3, const cXyz* param_4, u32 i_atType) {
    g_dComIfG_gameInfo.play.getParticle()->setHitMark(i_hitmark, param_1, param_2, param_3, param_4,
                                                      i_atType);
}

inline void dComIfGp_particle_setWaterRipple(u32* param_0, cBgS_PolyInfo& param_1,
                                             const cXyz* param_2, f32 param_3,
                                             const dKy_tevstr_c* param_4, const cXyz* param_5,
                                             s8 param_6) {
    g_dComIfG_gameInfo.play.getParticle()->setWaterRipple(param_0, param_1, param_2, param_3,
                                                          param_4, param_5, param_6);
}

inline u32 dComIfGp_particle_setPolyColor(u32 param_0, u16 param_1, cBgS_PolyInfo& param_2,
                                          const cXyz* param_3, const dKy_tevstr_c* param_4,
                                          const csXyz* param_5, const cXyz* param_6, int param_7,
                                          dPa_levelEcallBack* param_8, s8 param_9,
                                          const cXyz* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setPoly(param_0, param_1, param_2, param_3,
                                                          param_4, param_5, param_6, param_7,
                                                          param_8, param_9, param_10);
}

inline JPABaseEmitter* dComIfGp_particle_setPolyColor(u16 param_1, cBgS_PolyInfo& param_2,
                                                      const cXyz* param_3,
                                                      const dKy_tevstr_c* param_4,
                                                      const csXyz* param_5, const cXyz* param_6,
                                                      int param_7, dPa_levelEcallBack* param_8,
                                                      s8 param_9, const cXyz* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setPoly(
        param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9, param_10);
}

inline void dComIfGp_particle_setSimple(u16 param_0, cXyz* i_pos, u8 param_2, GXColor& param_3,
                                        GXColor& param_4, int param_5, float param_6) {
    g_dComIfG_gameInfo.play.getParticle()->setSimple(param_0, i_pos, 0, param_2, param_3, param_4,
                                                     param_5, param_6);
}

inline u32 dComIfGp_particle_setStopContinue(u32 param_0) {
    return g_dComIfG_gameInfo.play.getParticle()->setStopContinue(param_0);
}

inline u32 dComIfGp_particle_setSimpleFoot(u32 param_0, u32* param_1, cBgS_PolyInfo& param_2,
                                           cXyz const* param_3, dKy_tevstr_c const* param_4,
                                           int param_5, csXyz const* param_6, cXyz const* param_7,
                                           dPa_levelEcallBack* param_8, s8 param_9,
                                           cXyz const* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setSimpleFoot(param_0, param_1, param_2, param_3,
                                                                param_4, param_5, param_6, param_7,
                                                                param_8, param_9, param_10);
}

inline u16 dComIfGp_particle_setCommonPoly(u32* param_0, cBgS_PolyInfo* param_1,
                                           const cXyz* param_2, const cXyz* param_3,
                                           const dKy_tevstr_c* param_4, u32 param_5, u32 param_6,
                                           const csXyz* param_7, const cXyz* param_8, s8 param_9) {
    return g_dComIfG_gameInfo.play.getParticle()->setCommonPoly(
        param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8, param_9);
}

inline void dComIfGp_particle_levelEmitterOnEventMove(u32 param_0) {
    g_dComIfG_gameInfo.play.getParticle()->forceOnEventMove(param_0);
}

inline JPABaseEmitter* dComIfGp_particle_getEmitter(u32 param_0) {
    return g_dComIfG_gameInfo.play.getParticle()->getEmitter(param_0);
}

inline u32 dComIfGp_particle_set(u32 param_0, u16 param_1, const cXyz* i_pos,
                                 const dKy_tevstr_c* param_3, const csXyz* i_rotation,
                                 const cXyz* i_scale, u8 i_alpha, dPa_levelEcallBack* param_7,
                                 s8 param_8, const GXColor* param_9, const GXColor* param_10,
                                 const cXyz* param_11) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(
        param_0, param_1, i_pos, param_3, i_rotation, i_scale, i_alpha, param_7, param_8, param_9,
        param_10, param_11, 1.0f);
}

inline u32 dComIfGp_particle_set(u32 param_0, u16 param_1, const cXyz* i_pos,
                                 const csXyz* i_rotation, const cXyz* i_scale, u8 param_5,
                                 dPa_levelEcallBack* param_6, s8 param_7, const GXColor* param_8,
                                 const GXColor* param_9, const cXyz* param_10) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(
        param_0, param_1, i_pos, NULL, i_rotation, i_scale, param_5, param_6, param_7, param_8,
        param_9, param_10, 1.0f);
}

inline JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos,
                                             const dKy_tevstr_c* param_3, const csXyz* i_rotation,
                                             const cXyz* i_scale, u8 i_alpha,
                                             dPa_levelEcallBack* i_callback, s8 param_8,
                                             const GXColor* i_prmColor, const GXColor* i_envColor,
                                             const cXyz* i_particleScale) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(
        i_resID, i_pos, param_3, i_rotation, i_scale, i_alpha, i_callback, param_8, i_prmColor,
        i_envColor, i_particleScale, 1.0f);
}

inline JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos,
                                             const csXyz* i_rotation, const cXyz* i_scale,
                                             u8 i_alpha, dPa_levelEcallBack* i_callback, s8 param_7,
                                             const GXColor* i_prmColor, const GXColor* i_envColor,
                                             const cXyz* i_particleScale) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(
        i_resID, i_pos, NULL, i_rotation, i_scale, i_alpha, i_callback, param_7, i_prmColor,
        i_envColor, i_particleScale, 1.0f);
}

inline u32 dComIfGp_particle_set(u32 param_0, u16 param_1, const cXyz* i_pos,
                                 const dKy_tevstr_c* param_3) {
    return dComIfGp_particle_set(param_0, param_1, i_pos, param_3, NULL, NULL, 0xFF, NULL, -1, NULL,
                                 NULL, NULL);
}

inline JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos,
                                             const csXyz* i_rotation, const cXyz* i_scale) {
    return dComIfGp_particle_set(i_resID, i_pos, NULL, i_rotation, i_scale, 255, NULL, -1, NULL,
                                 NULL, NULL);
}

inline JPABaseEmitter* dComIfGp_particle_set(u16 i_resID, const cXyz* i_pos,
                                             const dKy_tevstr_c* param_2, const csXyz* i_rotation,
                                             const cXyz* i_scale) {
    return dComIfGp_particle_set(i_resID, i_pos, param_2, i_rotation, i_scale, 255, NULL, -1, NULL,
                                 NULL, NULL);
}

inline u32 dComIfGp_particle_set(u32 param_0, u16 param_1, const cXyz* i_pos,
                                 const csXyz* param_3, const cXyz* param_4) {
    return dComIfGp_particle_set(param_0, param_1, i_pos, param_3, param_4, 0xFF, NULL, -1, NULL,
                                 NULL, NULL);
}

inline u32 dComIfGp_particle_setColor(u32 param_0, u16 param_1, const cXyz* i_pos,
                                      const dKy_tevstr_c* param_3, const GXColor* param_4,
                                      const GXColor* param_5, f32 param_6, u8 param_7,
                                      const csXyz* param_8, const cXyz* param_9,
                                      dPa_levelEcallBack* param_10, s8 param_11,
                                      const cXyz* param_12) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(
        param_0, param_1, i_pos, param_3, param_8, param_9, param_7, param_10, param_11, param_4,
        param_5, param_12, param_6);
}

inline JPABaseEmitter* dComIfGp_particle_setColor(u16 param_0, const cXyz* i_pos,
                                                  const dKy_tevstr_c* param_2,
                                                  const GXColor* param_3, const GXColor* param_4,
                                                  f32 param_5, u8 param_6, const csXyz* param_7,
                                                  const cXyz* param_8, dPa_levelEcallBack* param_9,
                                                  s8 param_10, const cXyz* param_11) {
    return g_dComIfG_gameInfo.play.getParticle()->setNormal(param_0, i_pos, param_2, param_7,
                                                            param_8, param_6, param_9, param_10,
                                                            param_3, param_4, param_11, param_5);
}

inline JPABaseEmitter* dComIfGp_particle_setColor(u16 param_0, const cXyz* i_pos,
                                                  const dKy_tevstr_c* param_2,
                                                  const GXColor* param_3, const GXColor* param_4,
                                                  f32 param_5, u8 param_6) {
    return dComIfGp_particle_setColor(param_0, i_pos, param_2, param_3, param_4, param_5, param_6,
                                      NULL, NULL, NULL, -1, NULL);
}

inline u32 dComIfGp_particle_getHeapSize() {
    return g_dComIfG_gameInfo.play.getParticle()->getHeapSize();
}

inline u32 dComIfGp_particle_getSceneHeapSize() {
    return g_dComIfG_gameInfo.play.getParticle()->getSceneHeapSize();
}

inline int dComIfGp_particle_getEmitterNum() {
    return g_dComIfG_gameInfo.play.getParticle()->getEmitterNum();
}

inline int dComIfGp_particle_getParticleNum() {
    return g_dComIfG_gameInfo.play.getParticle()->getParticleNum();
}

inline dSmplMdl_draw_c* dComIfGp_getSimpleModel() {
    return g_dComIfG_gameInfo.play.getSimpleModel();
}

inline int dComIfGp_getWindowNum() {
    return g_dComIfG_gameInfo.play.getWindowNum();
}

inline void dComIfGp_setWindowNum(int num) {
    g_dComIfG_gameInfo.play.setWindowNum(num);
}

inline s8 dComIfGp_getLayerOld() {
    return g_dComIfG_gameInfo.play.getLayerOld();
}

inline s32 dComIfGp_checkStatus(u16 flags) {
    return g_dComIfG_gameInfo.play.checkStatus(flags);
}

inline void dComIfGp_setStatus(u16 status) {
    g_dComIfG_gameInfo.play.setStatus(status);
}

inline void dComIfGp_onStatus(u16 i_status) {
    g_dComIfG_gameInfo.play.onStatus(i_status);
}

inline dDlst_window_c* dComIfGp_getWindow(int i) {
    return g_dComIfG_gameInfo.play.getWindow(i);
}

inline void dComIfGp_setWindow(u8 i, f32 param_1, f32 param_2, f32 param_3, f32 param_4,
                               f32 param_5, f32 param_6, int camID, int mode) {
    g_dComIfG_gameInfo.play.setWindow(i, param_1, param_2, param_3, param_4, param_5, param_6,
                                      camID, mode);
}

inline camera_process_class* dComIfGp_getCamera(int idx) {
    return (camera_process_class*)g_dComIfG_gameInfo.play.getCamera(idx);
}

inline void dComIfGp_setCamera(int i, camera_class* cam) {
    g_dComIfG_gameInfo.play.setCamera(i, cam);
}

inline int dComIfGp_getCameraWinID(int idx) {
    return g_dComIfG_gameInfo.play.getCameraWinID(idx);
}

inline int dComIfGp_getCameraPlayer1ID(int idx) {
    return g_dComIfG_gameInfo.play.getCameraPlayer1ID(idx);
}

inline int dComIfGp_getCameraPlayer2ID(int idx) {
    return g_dComIfG_gameInfo.play.getCameraPlayer2ID(idx);
}

inline u32 dComIfGp_getCameraAttentionStatus(int i_no) {
    return g_dComIfG_gameInfo.play.getCameraAttentionStatus(i_no);
}

inline BOOL dComIfGp_checkCameraAttentionStatus(int i, u32 flag) {
    return g_dComIfG_gameInfo.play.checkCameraAttentionStatus(i, flag);
}

inline void dComIfGp_onCameraAttentionStatus(int i, u32 flag) {
    g_dComIfG_gameInfo.play.onCameraAttentionStatus(i, flag);
}

inline void dComIfGp_offCameraAttentionStatus(int i, u32 flag) {
    g_dComIfG_gameInfo.play.offCameraAttentionStatus(i, flag);
}

inline void dComIfGp_setCameraInfo(int camIdx, camera_class* p_cam, int param_2, int param_3,
                                   int param_4) {
    g_dComIfG_gameInfo.play.setCameraInfo(camIdx, p_cam, param_2, param_3, param_4);
}

inline f32 dComIfGp_getCameraZoomScale(int i_no) {
    return g_dComIfG_gameInfo.play.getCameraZoomScale(i_no);
}

inline void dComIfGp_setCameraZoomScale(int i_no, f32 i_scale) {
    g_dComIfG_gameInfo.play.setCameraZoomScale(i_no, i_scale);
}

inline f32 dComIfGp_getCameraZoomForcus(int i_no) {
    return g_dComIfG_gameInfo.play.getCameraZoomForcus(i_no);
}

inline void dComIfGp_setCameraZoomForcus(int i_no, f32 i_focus) {
    g_dComIfG_gameInfo.play.setCameraZoomForcus(i_no, i_focus);
}

inline const char* dComIfGp_getCameraParamFileName(int i) {
    return g_dComIfG_gameInfo.play.getCameraParamFileName(i);
}

inline void dComIfGp_setCameraParamFileName(int i, char* name) {
    g_dComIfG_gameInfo.play.setCameraParamFileName(i, name);
}

inline void dComIfGp_saveCameraPosition(int i, cXyz* i_pos, cXyz* i_target, f32 i_fovy,
                                        s16 i_bank) {
    g_dComIfG_gameInfo.play.saveCameraPosition(i, i_pos, i_target, i_fovy, i_bank);
}

inline void dComIfGp_loadCameraPosition(int i, cXyz* o_pos, cXyz* o_target, f32* o_fovy,
                                        s16* o_bank) {
    g_dComIfG_gameInfo.play.loadCameraPosition(i, o_pos, o_target, o_fovy, o_bank);
}

inline fopAc_ac_c* dComIfGp_getPlayer(int idx) {
    return g_dComIfG_gameInfo.play.getPlayer(idx);
}

inline void dComIfGp_setPlayer(int i, fopAc_ac_c* player) {
    g_dComIfG_gameInfo.play.setPlayer(i, player);
}

inline int dComIfGp_getPlayerCameraID(int idx) {
    return g_dComIfG_gameInfo.play.getPlayerCameraID(idx);
}

inline void dComIfGp_setPlayerInfo(int plyrIdx, fopAc_ac_c* ptr, int camIdx) {
    g_dComIfG_gameInfo.play.setPlayerInfo(plyrIdx, ptr, camIdx);
}

inline daPy_py_c* dComIfGp_getLinkPlayer() {
    return (daPy_py_c*)g_dComIfG_gameInfo.play.getPlayerPtr(LINK_PTR);
}

inline daHorse_c* dComIfGp_getHorseActor() {
    return (daHorse_c*)g_dComIfG_gameInfo.play.getPlayerPtr(HORSE_PTR);
}

inline void dComIfGp_setLinkPlayer(fopAc_ac_c* ptr) {
    g_dComIfG_gameInfo.play.setPlayerPtr(0, ptr);
}

inline void dComIfGp_setHorseActor(fopAc_ac_c* i_horse) {
    g_dComIfG_gameInfo.play.setPlayerPtr(1, i_horse);
}

inline void dComIfGp_setPlayerPtr(int i, fopAc_ac_c* ptr) {
    g_dComIfG_gameInfo.play.setPlayerPtr(i, ptr);
}

inline dMsgObject_c* dComIfGp_getMsgObjectClass() {
    return g_dComIfG_gameInfo.play.getMsgObjectClass();
}

inline void dComIfGp_setMsgObjectClass(dMsgObject_c* obj) {
    return g_dComIfG_gameInfo.play.setMsgObjectClass(obj);
}

inline f32 dComIfGp_getItemLifeCount() {
    return g_dComIfG_gameInfo.play.getItemLifeCount();
}

inline u8 dComIfGp_getItemLifeCountType() {
    return g_dComIfG_gameInfo.play.getItemLifeCountType();
}

inline void dComIfGp_setItemLifeCount(f32 amount, u8 type) {
    g_dComIfG_gameInfo.play.setItemLifeCount(amount, type);
}

inline void dComIfGp_clearItemLifeCount() {
    g_dComIfG_gameInfo.play.clearItemLifeCount();
}

inline s32 dComIfGp_getItemRupeeCount() {
    return g_dComIfG_gameInfo.play.getItemRupeeCount();
}

inline void dComIfGp_setItemRupeeCount(s32 amount) {
    g_dComIfG_gameInfo.play.setItemRupeeCount(amount);
}

inline void dComIfGp_clearItemRupeeCount() {
    g_dComIfG_gameInfo.play.clearItemRupeeCount();
}

inline s16 dComIfGp_getItemKeyNumCount() {
    return g_dComIfG_gameInfo.play.getItemKeyNumCount();
}

inline void dComIfGp_setItemKeyNumCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemKeyNumCount(count);
}

inline void dComIfGp_clearItemKeyNumCount() {
    g_dComIfG_gameInfo.play.clearItemKeyNumCount();
}

inline s16 dComIfGp_getItemMaxLifeCount() {
    return g_dComIfG_gameInfo.play.getItemMaxLifeCount();
}

inline void dComIfGp_setItemMaxLifeCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemMaxLifeCount(count);
}

inline void dComIfGp_clearItemMaxLifeCount() {
    g_dComIfG_gameInfo.play.clearItemMaxLifeCount();
}

inline void dComIfGp_setItemMagicCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemMagicCount(count);
}

inline void dComIfGp_setItemNowMagic(s16 magic) {
    g_dComIfG_gameInfo.play.setItemNowMagic(magic);
}

inline void dComIfGp_setItemMaxMagicCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemMaxMagicCount(count);
}

inline s32 dComIfGp_getItemOilCount() {
    return g_dComIfG_gameInfo.play.getItemOilCount();
}

inline void dComIfGp_setItemOilCount(s32 oil) {
    g_dComIfG_gameInfo.play.setItemOilCount(oil);
}

inline void dComIfGp_setItemMaxOilCount(s32 oil) {
    g_dComIfG_gameInfo.play.setItemMaxOilCount(oil);
}

inline void dComIfGp_clearItemOilCount() {
    g_dComIfG_gameInfo.play.clearItemOilCount();
}

inline s32 dComIfGp_getItemNowOil() {
    return g_dComIfG_gameInfo.play.getItemNowOil();
}

inline void dComIfGp_setItemNowOil(s32 oil) {
    g_dComIfG_gameInfo.play.setItemNowOil(oil);
}

inline s32 dComIfGp_getItemMaxOilCount() {
    return g_dComIfG_gameInfo.play.getItemMaxOilCount();
}

inline void dComIfGp_clearItemMaxOilCount() {
    g_dComIfG_gameInfo.play.clearItemMaxOilCount();
}

inline int dComIfGp_getOxygen() {
    return g_dComIfG_gameInfo.play.getOxygen();
}

inline void dComIfGp_setOxygen(s32 oxygen) {
    g_dComIfG_gameInfo.play.setOxygen(oxygen);
}

inline int dComIfGp_getNowOxygen() {
    return g_dComIfG_gameInfo.play.getNowOxygen();
}

inline void dComIfGp_setNowOxygen(s32 oxygen) {
    g_dComIfG_gameInfo.play.setNowOxygen(oxygen);
}

inline s32 dComIfGp_getMaxOxygen() {
    return g_dComIfG_gameInfo.play.getMaxOxygen();
}

inline void dComIfGp_setMaxOxygen(s32 i_oxygen) {
    g_dComIfG_gameInfo.play.setMaxOxygen(i_oxygen);
}

inline s32 dComIfGp_getOxygenCount() {
    return g_dComIfG_gameInfo.play.getOxygenCount();
}

inline void dComIfGp_setOxygenCount(s32 oxygen) {
    g_dComIfG_gameInfo.play.setOxygenCount(oxygen);
}

inline void dComIfGp_clearOxygenCount() {
    g_dComIfG_gameInfo.play.clearOxygenCount();
}

inline void dComIfGp_setMaxOxygenCount(s32 oxygen) {
    g_dComIfG_gameInfo.play.setMaxOxygenCount(oxygen);
}

inline s32 dComIfGp_getMaxOxygenCount() {
    return g_dComIfG_gameInfo.play.getMaxOxygenCount();
}

inline void dComIfGp_clearMaxOxygenCount() {
    g_dComIfG_gameInfo.play.clearMaxOxygenCount();
}

inline s16 dComIfGp_getItemArrowNumCount() {
    return g_dComIfG_gameInfo.play.getItemArrowNumCount();
}

inline void dComIfGp_setItemArrowNumCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemArrowNumCount(count);
}

inline void dComIfGp_clearItemArrowNumCount() {
    g_dComIfG_gameInfo.play.clearItemArrowNumCount();
}

inline s16 dComIfGp_getItemPachinkoNumCount() {
    return g_dComIfG_gameInfo.play.getItemPachinkoNumCount();
}

inline void dComIfGp_setItemPachinkoNumCount(s16 count) {
    g_dComIfG_gameInfo.play.setItemPachinkoNumCount(count);
}

inline void dComIfGp_clearItemPachinkoNumCount() {
    g_dComIfG_gameInfo.play.clearItemPachinkoNumCount();
}

inline s16 dComIfGp_getItemMaxArrowNumCount() {
    return g_dComIfG_gameInfo.play.getItemMaxArrowNumCount();
}

inline int dComIfGp_getMessageCountNumber() {
    return g_dComIfG_gameInfo.play.getMessageCountNumber();
}

inline void dComIfGp_setMessageCountNumber(s32 number) {
    g_dComIfG_gameInfo.play.setMessageCountNumber(number);
}

inline u16 dComIfGp_getItemNowLife() {
    return g_dComIfG_gameInfo.play.getItemNowLife();
}

inline void dComIfGp_setItemNowLife(u16 life) {
    g_dComIfG_gameInfo.play.setItemNowLife(life);
}

inline u8 dComIfGp_getMesgStatus() {
    return g_dComIfG_gameInfo.play.getMesgStatus();
}

inline u8 dComIfGp_getRStatus() {
    return g_dComIfG_gameInfo.play.getRStatus();
}

inline bool dComIfGp_isRSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isRSetFlag(flag);
}

inline void dComIfGp_setRStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setRStatus(status, flag);
}

inline u8 dComIfGp_getRStatusForce() {
    return g_dComIfG_gameInfo.play.getRStatusForce();
}

inline u8 dComIfGp_getRSetFlagForce() {
    return g_dComIfG_gameInfo.play.getRSetFlagForce();
}

inline void dComIfGp_setRStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setRStatusForce(status, flag);
}

inline u8 dComIfGp_getAStatus() {
    return g_dComIfG_gameInfo.play.getAStatus();
}

inline bool dComIfGp_isASetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isASetFlag(flag);
}

inline void dComIfGp_setAStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setAStatus(status, flag);
}

inline u8 dComIfGp_getAStatusForce() {
    return g_dComIfG_gameInfo.play.getAStatusForce();
}

inline u8 dComIfGp_getASetFlagForce() {
    return g_dComIfG_gameInfo.play.getASetFlagForce();
}

inline void dComIfGp_setAStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setAStatusForce(status, flag);
}

inline u8 dComIfGp_getNunStatus() {
    return g_dComIfG_gameInfo.play.getNunStatus();
}

inline bool dComIfGp_isNunSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isNunSetFlag(flag);
}

inline void dComIfGp_setNunStatus(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setNunStatus(status, param_1, flag);
}

inline u8 dComIfGp_getBottleStatus() {
    return g_dComIfG_gameInfo.play.getBottleStatus();
}

inline bool dComIfGp_isBottleSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isBottleSetFlag(flag);
}

inline void dComIfGp_setBottleStatus(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setBottleStatus(param_0, param_1);
}

inline u8 dComIfGp_getBottleStatusForce() {
    return g_dComIfG_gameInfo.play.getBottleStatusForce();
}

inline u8 dComIfGp_getBottleSetFlagForce() {
    return g_dComIfG_gameInfo.play.getBottleSetFlagForce();
}

inline void dComIfGp_setBottleStatusForce(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setBottleStatusForce(param_0, param_1);
}

inline u8 dComIfGp_getRemoConStatus() {
    return g_dComIfG_gameInfo.play.getRemoConStatus();
}

inline bool dComIfGp_isRemoConSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isRemoConSetFlag(flag);
}

inline void dComIfGp_setRemoConStatus(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setRemoConStatus(status, param_1, flag);
}

inline u8 dComIfGp_getDoStatus() {
    return g_dComIfG_gameInfo.play.getDoStatus();
}

inline bool dComIfGp_isDoSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isDoSetFlag(flag);
}

inline void dComIfGp_setDoStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setDoStatus(status, flag);
}

inline u8 dComIfGp_getDoStatusForce() {
    return g_dComIfG_gameInfo.play.getDoStatusForce();
}

inline u8 dComIfGp_getDoSetFlagForce() {
    return g_dComIfG_gameInfo.play.getDoSetFlagForce();
}

inline void dComIfGp_setDoStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setDoStatusForce(status, flag);
}

inline u8 dComIfGp_get3DStatus() {
    return g_dComIfG_gameInfo.play.get3DStatus();
}

inline u8 dComIfGp_get3DDirection() {
    return g_dComIfG_gameInfo.play.get3DDirection();
}

inline bool dComIfGp_is3DSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.is3DSetFlag(flag);
}

inline void dComIfGp_set3DStatus(u8 status, u8 direction, u8 flag) {
    g_dComIfG_gameInfo.play.set3DStatus(status, direction, flag);
}

inline u8 dComIfGp_get3DStatusForce() {
    return g_dComIfG_gameInfo.play.get3DStatusForce();
}

inline u8 dComIfGp_get3DDirectionForce() {
    return g_dComIfG_gameInfo.play.get3DDirectionForce();
}

inline u8 dComIfGp_get3DSetFlagForce() {
    return g_dComIfG_gameInfo.play.get3DSetFlagForce();
}

inline void dComIfGp_set3DStatusForce(u8 status, u8 direction, u8 flag) {
    g_dComIfG_gameInfo.play.set3DStatusForce(status, direction, flag);
}

inline u8 dComIfGp_getCStickStatus() {
    return g_dComIfG_gameInfo.play.getCStickStatus();
}

inline u8 dComIfGp_getCStickDirection() {
    return g_dComIfG_gameInfo.play.getCStickDirection();
}

inline bool dComIfGp_isCStickSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isCStickSetFlag(flag);
}

inline void dComIfGp_setCStickStatus(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setCStickStatus(status, param_1, flag);
}

inline u8 dComIfGp_getCStickStatusForce() {
    return g_dComIfG_gameInfo.play.getCStickStatusForce();
}

inline u8 dComIfGp_getCStickDirectionForce() {
    return g_dComIfG_gameInfo.play.getCStickDirectionForce();
}

inline u8 dComIfGp_getCStickSetFlagForce() {
    return g_dComIfG_gameInfo.play.getCStickSetFlagForce();
}

inline void dComIfGp_setCStickStatusForce(u8 status, u8 param_1, u8 flag) {
    g_dComIfG_gameInfo.play.setCStickStatusForce(status, param_1, flag);
}

inline u8 dComIfGp_getSButtonStatus() {
    return g_dComIfG_gameInfo.play.getSButtonStatus();
}

inline bool dComIfGp_isSButtonSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isSButtonSetFlag(flag);
}

inline void dComIfGp_setSButtonStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setSButtonStatus(status, flag);
}

inline u8 dComIfGp_getSButtonStatusForce() {
    return g_dComIfG_gameInfo.play.getSButtonStatusForce();
}

inline u8 dComIfGp_getSButtonSetFlagForce() {
    return g_dComIfG_gameInfo.play.getSButtonSetFlagForce();
}

inline void dComIfGp_setSButtonStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setSButtonStatusForce(status, flag);
}

inline u8 dComIfGp_getZStatus() {
    return g_dComIfG_gameInfo.play.getZStatus();
}

inline bool dComIfGp_isZSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isZSetFlag(flag);
}

inline void dComIfGp_setZStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setZStatus(status, flag);
}

inline u8 dComIfGp_getZStatusForce() {
    return g_dComIfG_gameInfo.play.getZStatusForce();
}

inline u8 dComIfGp_getZSetFlagForce() {
    return g_dComIfG_gameInfo.play.getZSetFlagForce();
}

inline void dComIfGp_setZStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setZStatusForce(status, flag);
}

inline u8 dComIfGp_getXStatus() {
    return g_dComIfG_gameInfo.play.getXStatus();
}

inline bool dComIfGp_isXSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isXSetFlag(flag);
}

inline void dComIfGp_setXStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setXStatus(status, flag);
}

inline u8 dComIfGp_getXStatusForce() {
    return g_dComIfG_gameInfo.play.getXStatusForce();
}

inline u8 dComIfGp_getXSetFlagForce() {
    return g_dComIfG_gameInfo.play.getXSetFlagForce();
}

inline void dComIfGp_setXStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setXStatusForce(status, flag);
}

inline u8 dComIfGp_getYStatus() {
    return g_dComIfG_gameInfo.play.getYStatus();
}

inline bool dComIfGp_isYSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isYSetFlag(flag);
}

inline void dComIfGp_setYStatus(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setYStatus(status, flag);
}

inline u8 dComIfGp_getYStatusForce() {
    return g_dComIfG_gameInfo.play.getYStatusForce();
}

inline u8 dComIfGp_getYSetFlagForce() {
    return g_dComIfG_gameInfo.play.getYSetFlagForce();
}

inline void dComIfGp_setYStatusForce(u8 status, u8 flag) {
    g_dComIfG_gameInfo.play.setYStatusForce(status, flag);
}

inline u8 dComIfGp_getNunZStatus() {
    return g_dComIfG_gameInfo.play.getNunZStatus();
}

inline bool dComIfGp_isNunZSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isNunZSetFlag(flag);
}

inline void dComIfGp_setNunZStatus(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setNunZStatus(param_0, param_1);
}

inline u8 dComIfGp_getNunCStatus() {
    return g_dComIfG_gameInfo.play.getNunCStatus();
}

inline bool dComIfGp_isNunCSetFlag(u8 flag) {
    return g_dComIfG_gameInfo.play.isNunCSetFlag(flag);
}

inline void dComIfGp_setNunCStatus(u8 param_0, u8 param_1) {
    g_dComIfG_gameInfo.play.setNunCStatus(param_0, param_1);
}

inline void dComIfGp_setSelectEquipClothes(u8 i_clothNo) {
    g_dComIfG_gameInfo.play.setSelectEquip(COLLECT_CLOTHING, i_clothNo);
}

inline void dComIfGp_setSelectEquipSword(u8 i_swordNo) {
    g_dComIfG_gameInfo.play.setSelectEquip(COLLECT_SWORD, i_swordNo);
}

inline void dComIfGp_setSelectEquipShield(u8 i_shieldNo) {
    g_dComIfG_gameInfo.play.setSelectEquip(COLLECT_SHIELD, i_shieldNo);
}

inline u8 dComIfGp_getMesgAnimeAttrInfo() {
    return g_dComIfG_gameInfo.play.getBaseAnimeID();
}

inline void dComIfGp_setMesgAnimeAttrInfo(u8 param_1) {
    g_dComIfG_gameInfo.play.setBaseAnimeID(param_1);
}

inline void dComIfGp_clearMesgAnimeAttrInfo() {
    g_dComIfG_gameInfo.play.clearBaseAnimeID();
}

inline u8 dComIfGp_getMesgFaceAnimeAttrInfo() {
    return g_dComIfG_gameInfo.play.getFaceAnimeID();
}

inline void dComIfGp_setMesgFaceAnimeAttrInfo(u8 id) {
    g_dComIfG_gameInfo.play.setFaceAnimeID(id);
}

inline void dComIfGp_clearMesgFaceAnimeAttrInfo() {
    g_dComIfG_gameInfo.play.clearFaceAnimeID();
}

inline void dComIfGp_clearMesgAnimeTagInfo() {
    g_dComIfG_gameInfo.play.clearNowAnimeID();
}

inline void dComIfGp_setItem(u8 slot, u8 i_no) {
    g_dComIfG_gameInfo.play.setItem(slot, i_no);
}

inline u8 dComIfGp_getAdvanceDirection() {
    return g_dComIfG_gameInfo.play.getDirection();
}

inline void dComIfGp_setAdvanceDirection(u8 i_dir) {
    g_dComIfG_gameInfo.play.setDirection(i_dir);
}

inline u8 dComIfGp_checkMesgCancelButton() {
    return g_dComIfG_gameInfo.play.checkMesgCancelButton();
}

inline void dComIfGp_setMesgCancelButton(u8 button) {
    g_dComIfG_gameInfo.play.setMesgCancelButton(button);
}

inline u8 dComIfGp_getGameoverStatus() {
    return g_dComIfG_gameInfo.play.getGameoverStatus();
}

inline void dComIfGp_setGameoverStatus(u8 i_status) {
    return g_dComIfG_gameInfo.play.setGameoverStatus(i_status);
}

inline u8 dComIfGp_isHeapLockFlag() {
    return g_dComIfG_gameInfo.play.isHeapLockFlag();
}

inline u8 dComIfGp_getSubHeapLockFlag(int idx) {
    return g_dComIfG_gameInfo.play.getSubHeapLockFlag(idx);
}

inline void dComIfGp_setSubHeapLockFlag(int idx, u8 status) {
    g_dComIfG_gameInfo.play.setSubHeapLockFlag(idx, status);
}

inline u8 dComIfGp_getNeedLightDropNum() {
    return g_dComIfG_gameInfo.play.getNeedLightDropNum();
}

inline void dComIfGp_setNeedLightDropNum(u8 i_num) {
    g_dComIfG_gameInfo.play.setNeedLightDropNum(i_num);
}

inline u8 dComIfGp_checkMesgBgm() {
    return g_dComIfG_gameInfo.play.checkMesgBgm();
}

inline void dComIfGp_setMesgBgmOff() {
    g_dComIfG_gameInfo.play.setMesgBgm(0);
}

inline void dComIfGp_setMesgBgmOn() {
    g_dComIfG_gameInfo.play.setMesgBgm(1);
}

inline u8 dComIfGp_isPauseFlag() {
    return g_dComIfG_gameInfo.play.isPauseFlag();
}

inline void dComIfGp_offPauseFlag() {
    g_dComIfG_gameInfo.play.offPauseFlag();
}

inline void dComIfGp_onPauseFlag() {
    g_dComIfG_gameInfo.play.onPauseFlag();
}

inline u8 dComIfGp_getOxygenShowFlag() {
    return g_dComIfG_gameInfo.play.getOxygenShowFlag();
}

inline void dComIfGp_offOxygenShowFlag() {
    g_dComIfG_gameInfo.play.setOxygenShowFlag(0);
}

inline void dComIfGp_onOxygenShowFlag() {
    g_dComIfG_gameInfo.play.setOxygenShowFlag(1);
}

inline u8 dComIfGp_2dShowCheck() {
    return g_dComIfG_gameInfo.play.show2dCheck();
}

inline void dComIfGp_2dShowOn() {
    g_dComIfG_gameInfo.play.show2dOn();
}

inline void dComIfGp_2dShowOff() {
    g_dComIfG_gameInfo.play.show2dOff();
}

inline JKRExpHeap* dComIfGp_getExpHeap2D() {
    return g_dComIfG_gameInfo.play.getExpHeap2D();
}

inline void dComIfGp_setExpHeap2D(void* heap) {
    g_dComIfG_gameInfo.play.setExpHeap2D(heap);
}

inline JKRExpHeap* dComIfGp_getSubExpHeap2D(int idx) {
    return g_dComIfG_gameInfo.play.getSubExpHeap2D(idx);
}

inline void dComIfGp_setSubExpHeap2D(int idx, void* heap) {
    g_dComIfG_gameInfo.play.setSubExpHeap2D(idx, heap);
}

inline JKRExpHeap* dComIfGp_getMsgExpHeap() {
    return g_dComIfG_gameInfo.play.getMsgExpHeap();
}

inline void dComIfGp_setMsgExpHeap(void* heap) {
    g_dComIfG_gameInfo.play.setMsgExpHeap(heap);
}

inline dComIfG_MesgCamInfo_c* dComIfGp_getMesgCameraInfo() {
    return g_dComIfG_gameInfo.play.getMesgCamInfo();
}

inline void dComIfGp_setMesgCameraTagInfo(int id) {
    g_dComIfG_gameInfo.play.setMesgCamInfoID(id);
}

inline void dComIfGp_clearMesgCameraTagInfo() {
    g_dComIfG_gameInfo.play.clearMesgCamInfoID();
}

inline void dComIfGp_setMesgCameraAttrInfo(int param_1) {
    g_dComIfG_gameInfo.play.setMesgCamInfoBasicID(param_1);
}

inline void dComIfGp_clearMesgCameraAttrInfo() {
    g_dComIfG_gameInfo.play.clearMesgCamInfoBasicID();
}

inline void dComIfGp_setMesgCameraInfoActor(fopAc_ac_c* param_1, fopAc_ac_c* param_2,
                                            fopAc_ac_c* param_3, fopAc_ac_c* param_4,
                                            fopAc_ac_c* param_5, fopAc_ac_c* param_6,
                                            fopAc_ac_c* param_7, fopAc_ac_c* param_8,
                                            fopAc_ac_c* param_9, fopAc_ac_c* param_10)

{
    g_dComIfG_gameInfo.play.setMesgCamInfoActor(param_1, param_2, param_3, param_4, param_5,
                                                param_6, param_7, param_8, param_9, param_10);
}

inline u32 dComIfGp_checkPlayerStatus0(int param_0, u32 flag) {
    return g_dComIfG_gameInfo.play.checkPlayerStatus(param_0, 0, flag);
}

inline u32 dComIfGp_checkPlayerStatus1(int param_0, u32 flag) {
    return g_dComIfG_gameInfo.play.checkPlayerStatus(param_0, 1, flag);
}

inline void dComIfGp_setPlayerStatus0(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.setPlayerStatus(param_0, 0, flag);
}

inline void dComIfGp_setPlayerStatus1(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.setPlayerStatus(param_0, 1, flag);
}

inline void dComIfGp_clearPlayerStatus0(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.clearPlayerStatus(param_0, 0, flag);
}

inline void dComIfGp_clearPlayerStatus1(int param_0, u32 flag) {
    g_dComIfG_gameInfo.play.clearPlayerStatus(param_0, 1, flag);
}

inline void dComIfGp_setCurrentWindow(dDlst_window_c* i_window) {
    g_dComIfG_gameInfo.play.setCurrentWindow(i_window);
}

inline void dComIfGp_setCurrentView(view_class* i_view) {
    g_dComIfG_gameInfo.play.setCurrentView(i_view);
}

inline void dComIfGp_setCurrentViewport(view_port_class* i_viewport) {
    g_dComIfG_gameInfo.play.setCurrentViewport(i_viewport);
}

inline J2DGrafContext* dComIfGp_getCurrentGrafPort() {
    return g_dComIfG_gameInfo.play.getCurrentGrafPort();
}

inline void dComIfGp_setCurrentGrafPort(J2DOrthoGraph* i_graf) {
    g_dComIfG_gameInfo.play.setCurrentGrafPort(i_graf);
}

inline void* dComIfGp_getItemTable() {
    return g_dComIfG_gameInfo.play.getItemTable();
}

inline void dComIfGp_setItemTable(void* data) {
    g_dComIfG_gameInfo.play.setItemTable(data);
}

inline char* dComIfGp_getLastPlayStageName() {
    return g_dComIfG_gameInfo.play.getLastPlayStageName();
}

inline void dComIfGp_setLastPlayStageName(char* name) {
    g_dComIfG_gameInfo.play.setLastPlayStageName(name);
}

inline void dComIfGp_init() {
    g_dComIfG_gameInfo.play.init();
}

inline void dComIfGp_itemDataInit() {
    g_dComIfG_gameInfo.play.itemInit();
}

inline void dComIfGp_setItemBombNumCount(u8 i_item, s16 count) {
    g_dComIfG_gameInfo.play.setItemBombNumCount(i_item, count);
}

inline s16 dComIfGp_getItemBombNumCount(u8 i_no) {
    return g_dComIfG_gameInfo.play.getItemBombNumCount(i_no);
}

inline void dComIfGp_clearItemBombNumCount(u8 i_no) {
    g_dComIfG_gameInfo.play.clearItemBombNumCount(i_no);
}

inline s16 dComIfGp_getItemMaxBombNumCount() {
    return g_dComIfG_gameInfo.play.getItemMaxBombNumCount(dItemNo_NORMAL_BOMB_e);
}

inline void dComIfGp_setNowVibration(u8 status) {
    g_dComIfG_gameInfo.play.setNowVibration(status);
}

inline u32 dComIfGp_getNowVibration() {
    return g_dComIfG_gameInfo.play.getNowVibration();
}

inline void dComIfGp_particle_create() {
    g_dComIfG_gameInfo.play.createParticle();
}

inline void dComIfGp_createSimpleModel() {
    g_dComIfG_gameInfo.play.createSimpleModel();
}

inline void dComIfGp_deleteSimpleModel() {
    g_dComIfG_gameInfo.play.deleteSimpleModel();
}

inline void dComIfGp_drawSimpleModel() {
    g_dComIfG_gameInfo.play.drawSimpleModel();
}

inline int dComIfGp_addSimpleModel(J3DModelData* i_modelData, int roomNo, u8 i_drawBG) {
    return g_dComIfG_gameInfo.play.addSimpleModel(i_modelData, roomNo, i_drawBG);
}

inline void dComIfGp_removeSimpleModel(J3DModelData* i_modelData, int roomNo) {
    g_dComIfG_gameInfo.play.removeSimpleModel(i_modelData, roomNo);
}

inline void dComIfGp_entrySimpleModel(J3DModel* model, int roomNo) {
    g_dComIfG_gameInfo.play.entrySimpleModel(model, roomNo);
}

inline void dComIfG_ct() {
    g_dComIfG_gameInfo.ct();
}

inline dBgS& dComIfG_Bgsp() {
    return g_dComIfG_gameInfo.play.mBgs;
}

inline dCcS* dComIfG_Ccsp() {
    return &g_dComIfG_gameInfo.play.mCcs;
}

inline dCcS& dComIfG_Ccsp2() {
    return g_dComIfG_gameInfo.play.mCcs;
}

inline void dComIfG_setTimerNowTimeMs(int time) {
    g_dComIfG_gameInfo.play.setTimerNowTimeMs(time);
}

inline int dComIfG_getTimerNowTimeMs() {
    return g_dComIfG_gameInfo.play.getTimerNowTimeMs();
}

inline void dComIfG_setTimerLimitTimeMs(int i_time) {
    g_dComIfG_gameInfo.play.setTimerLimitTimeMs(i_time);
}

inline int dComIfG_getTimerLimitTimeMs() {
    return g_dComIfG_gameInfo.play.getTimerLimitTimeMs();
}

inline void dComIfG_setTimerMode(int mode) {
    return g_dComIfG_gameInfo.play.setTimerMode(mode);
}

inline int dComIfG_getTimerMode() {
    return g_dComIfG_gameInfo.play.getTimerMode();
}

inline void dComIfG_setTimerType(u8 i_type) {
    g_dComIfG_gameInfo.play.setTimerType(i_type);
}

inline u8 dComIfG_getTimerType() {
    return g_dComIfG_gameInfo.play.getTimerType();
}

inline void dComIfG_setTimerPtr(dTimer_c* i_ptr) {
    g_dComIfG_gameInfo.play.setTimerPtr(i_ptr);
}

inline dTimer_c* dComIfG_getTimerPtr() {
    return g_dComIfG_gameInfo.play.getTimerPtr();
}

/**
 * Attempts to add a new Object Resource Archive (*.arc) into the Resource Control.
 * @param i_arcName Name of archive to be added
 * @param i_mountDirection The direction to mount the archive. mDoDvd_MOUNT_DIRECTION_HEAD or
 * mDoDvd_MOUNT_DIRECTION_TAIL
 * @param i_heap Pointer to heap to load resources into
 * @return TRUE if successful, FALSE otherwise
 */
inline int dComIfG_setObjectRes(const char* i_arcName, u8 i_mountDirection, JKRHeap* i_heap) {
    return g_dComIfG_gameInfo.mResControl.setObjectRes(i_arcName, i_mountDirection, i_heap);
}

inline int dComIfG_setObjectRes(const char* i_arcName, void* i_archiveRes, u32 i_bufferSize, JKRHeap* i_heap) {
    UNUSED(i_heap);
    return g_dComIfG_gameInfo.mResControl.setObjectRes(i_arcName, i_archiveRes, i_bufferSize, NULL);
}

/**
 * Attempts to add a new Stage Resource Archive (*.arc) into the Resource Control.
 * @param i_arcName Name of archive to be added
 * @param i_heap Pointer to heap to load resources into
 * @return TRUE if successful, FALSE otherwise
 */
inline int dComIfG_setStageRes(const char* i_arcName, JKRHeap* i_heap) {
    return g_dComIfG_gameInfo.mResControl.setStageRes(i_arcName, i_heap);
}

inline int dComIfG_syncObjectRes(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.syncObjectRes(i_arcName);
}

inline int dComIfG_syncStageRes(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.syncStageRes(i_arcName);
}

inline int dComIfG_deleteObjectResMain(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.deleteObjectRes(i_arcName);
}

inline int dComIfG_deleteStageRes(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.deleteStageRes(i_arcName);
}

inline void* dComIfG_getStageRes(const char* i_arcName, const char* i_resName) {
    return g_dComIfG_gameInfo.mResControl.getStageRes(i_arcName, i_resName);
}

inline void* dComIfG_getObjectRes(const char* i_arcName, const char* i_resName) {
    return g_dComIfG_gameInfo.mResControl.getObjectRes(i_arcName, i_resName);
}

inline void* dComIfG_getObjectRes(const char* i_arcName, int i_index) {
    return g_dComIfG_gameInfo.mResControl.getObjectRes(i_arcName, i_index);
}

inline void dComIfG_dumpResControl() {
    g_dComIfG_gameInfo.mResControl.dump();
}

inline dRes_info_c* dComIfG_getObjectResInfo(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getObjectResInfo(i_arcName);
}

inline dRes_info_c* dComIfG_getStageResInfo(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getStageResInfo(i_arcName);
}

inline int dComIfG_syncAllObjectRes() {
    return g_dComIfG_gameInfo.mResControl.syncAllObjectRes();
}

inline void* dComIfG_getObjectIDRes(const char* i_arcName, u16 i_resID) {
    return g_dComIfG_gameInfo.mResControl.getObjectIDRes(i_arcName, i_resID);
}

inline int dComIfG_getObjctResName2Index(const char* i_arcName, const char* i_resName) {
    return g_dComIfG_gameInfo.mResControl.getObjectResName2Index(i_arcName, i_resName);
}

inline u8 dComIfG_getBrightness() {
    return g_dComIfG_gameInfo.mFadeBrightness;
}

inline void dComIfG_setBrightness(u8 brightness) {
    g_dComIfG_gameInfo.mFadeBrightness = brightness;
}

inline BOOL dComIfG_isDebugMode() {
    return g_dComIfG_gameInfo.mIsDebugMode;
}

inline u32 dComIfG_getTrigB(u32 i_padNo) {
    return mDoCPd_c::getTrig(i_padNo) & PAD_BUTTON_B;
}

inline u32 dComIfG_getObjectAllSize() {
    return g_dComIfG_gameInfo.mResControl.getObjectAllSize();
}

inline u32 dComIfG_getStageAllSize() {
    return g_dComIfG_gameInfo.mResControl.getStageAllSize();
}

inline u32 dComIfG_getObjectSize(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getObjectSize(i_arcName);
}

inline u32 dComIfG_getStageSize(const char* i_arcName) {
    return g_dComIfG_gameInfo.mResControl.getStageSize(i_arcName);
}

#if DEBUG
inline void dComIfG_initStopwatch() {
    OSInitStopwatch(&g_dComIfG_gameInfo.mStopwatch, "dComIfG");
}
#endif

inline int dComIfGd_setRealShadow(u32 param_0, s8 param_1, J3DModel* param_2, cXyz* param_3,
                                  f32 param_4, f32 param_5, dKy_tevstr_c* param_6) {
    return g_dComIfG_gameInfo.drawlist.setRealShadow(param_0, param_1, param_2, param_3, param_4,
                                                     param_5, param_6);
}

inline int dComIfGd_setSimpleShadow(cXyz* pos, f32 param_1, f32 param_2, cXyz* param_3, s16 angle,
                                    f32 param_5, TGXTexObj* tex) {
    return g_dComIfG_gameInfo.drawlist.setSimpleShadow(pos, param_1, param_2, param_3, angle,
                                                       param_5, tex);
}

inline bool dComIfGd_addRealShadow(u32 key, J3DModel* model) {
    return g_dComIfG_gameInfo.drawlist.addRealShadow(key, model);
}

inline void dComIfGd_drawListItem3d() {
    g_dComIfG_gameInfo.drawlist.drawOpaListItem3d();
    g_dComIfG_gameInfo.drawlist.drawXluListItem3d();
}

# if VERSION > VERSION_GCN_JPN
inline void dComIfGd_drawListCursor() {
    g_dComIfG_gameInfo.drawlist.drawOpaListCursor();
    g_dComIfG_gameInfo.drawlist.drawXluListCursor();
}
#endif

inline void dComIfGd_reset() {
    g_dComIfG_gameInfo.drawlist.reset();
}

inline void dComIfGd_set2DOpa(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.set2DOpa(dlst);
}

inline void dComIfGd_set2DXlu(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.set2DXlu(dlst);
}

inline void dComIfGd_set2DOpaTop(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.set2DOpaTop(dlst);
}

inline void dComIfGd_setCopy2D(dDlst_base_c* dlst) {
    g_dComIfG_gameInfo.drawlist.setCopy2D(dlst);
}

inline view_class* dComIfGd_getView() {
    return g_dComIfG_gameInfo.drawlist.getView();
}

inline Mtx44* dComIfGd_getProjViewMtx() {
    return &(g_dComIfG_gameInfo.drawlist.getView()->projViewMtx);
}

inline MtxP dComIfGd_getInvViewMtx() {
    return g_dComIfG_gameInfo.drawlist.getView()->invViewMtx;
}

inline view_port_class* dComIfGd_getViewport() {
    return g_dComIfG_gameInfo.drawlist.getViewport();
}

inline MtxP dComIfGd_getViewRotMtx() {
    return g_dComIfG_gameInfo.drawlist.getView()->viewMtxNoTrans;
}
inline MtxP dComIfGd_getViewMtx() {
    return g_dComIfG_gameInfo.drawlist.getView()->viewMtx;
}

inline J3DDrawBuffer* dComIfGd_getListFilter() {
    return g_dComIfG_gameInfo.drawlist.getOpaListFilter();
}

inline J3DDrawBuffer* dComIfGd_getOpaListIndScreen() {
    return g_dComIfG_gameInfo.drawlist.getOpaListP0();
}

inline J3DDrawBuffer* dComIfGd_getListPacket() {
    return g_dComIfG_gameInfo.drawlist.getOpaListPacket();
}

inline void dComIfGd_setListSky() {
    g_dComIfG_gameInfo.drawlist.setOpaListSky();
    g_dComIfG_gameInfo.drawlist.setXluListSky();
}

inline void dComIfGd_setListDark() {
    g_dComIfG_gameInfo.drawlist.setOpaListDark();
    g_dComIfG_gameInfo.drawlist.setXluListDark();
}

inline void dComIfGd_setListInvisisble() {
    g_dComIfG_gameInfo.drawlist.setOpaListInvisible();
    g_dComIfG_gameInfo.drawlist.setXluListInvisible();
}

inline void dComIfGd_setListDarkBG() {
    g_dComIfG_gameInfo.drawlist.setOpaListDarkBG();
    g_dComIfG_gameInfo.drawlist.setXluListDarkBG();
}

inline void dComIfGd_setXluListDarkBG() {
    g_dComIfG_gameInfo.drawlist.setXluListDarkBG();
}

inline void dComIfGd_setList() {
    g_dComIfG_gameInfo.drawlist.setOpaList();
    g_dComIfG_gameInfo.drawlist.setXluList();
}

inline void dComIfGd_setListItem3D() {
    g_dComIfG_gameInfo.drawlist.setOpaListItem3D();
    g_dComIfG_gameInfo.drawlist.setXluListItem3D();
}

inline void dComIfGd_setList3Dlast() {
    g_dComIfG_gameInfo.drawlist.setOpaList3Dlast();
    g_dComIfG_gameInfo.drawlist.setXluList3Dlast();
}

inline void dComIfGd_setXluList2DScreen() {
    g_dComIfG_gameInfo.drawlist.setXluList2DScreen();
}

inline void dComIfGd_setXluListBG() {
    g_dComIfG_gameInfo.drawlist.setXluListBG();
}

inline void dComIfGd_setListBG() {
    g_dComIfG_gameInfo.drawlist.setOpaListBG();
    g_dComIfG_gameInfo.drawlist.setXluListBG();
}

inline void dComIfGd_setListIndScreen() {
    g_dComIfG_gameInfo.drawlist.setOpaListP0();
    g_dComIfG_gameInfo.drawlist.setXluListP0();
}

inline void dComIfGd_setListMiddle() {
    g_dComIfG_gameInfo.drawlist.setOpaListMiddle();
    g_dComIfG_gameInfo.drawlist.setXluListMiddle();
}

inline void dComIfGd_setListZxlu() {
    g_dComIfG_gameInfo.drawlist.setOpaListZxlu();
    g_dComIfG_gameInfo.drawlist.setXluListZxlu();
}

inline J3DDrawBuffer* dComIfGd_getOpaList() {
    return g_dComIfG_gameInfo.drawlist.getOpaList();
}

inline J3DDrawBuffer* dComIfGd_getOpaListBG() {
    return g_dComIfG_gameInfo.drawlist.getOpaListBG();
}

inline J3DDrawBuffer* dComIfGd_getOpaListDark() {
    return g_dComIfG_gameInfo.drawlist.getOpaListDark();
}

inline J3DDrawBuffer* dComIfGd_getXluListBG() {
    return g_dComIfG_gameInfo.drawlist.getXluListBG();
}

inline void dComIfGd_setListFilter() {
    g_dComIfG_gameInfo.drawlist.setOpaListFilter();
    g_dComIfG_gameInfo.drawlist.setXluListFilter();
}

inline void dComIfGd_init() {
    g_dComIfG_gameInfo.drawlist.init();
}

inline void dComIfGd_peekZ(s16 param_0, s16 param_1, u32* param_2) {
    g_dComIfG_gameInfo.drawlist.newPeekZdata(param_0, param_1, param_2);
}

inline void dComIfGd_peekZdata() {
    g_dComIfG_gameInfo.drawlist.peekZdata();
}

inline void dComIfGd_setView(view_class* view) {
    g_dComIfG_gameInfo.drawlist.setView(view);
}

inline void dComIfGd_setWindow(dDlst_window_c* window) {
    g_dComIfG_gameInfo.drawlist.setWindow(window);
}

inline void dComIfGd_setViewport(view_port_class* port) {
    g_dComIfG_gameInfo.drawlist.setViewport(port);
}

inline void dComIfGd_entryZSortListZxlu(J3DPacket* i_packet, cXyz& param_1) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.entryZSortListZxlu(i_packet, param_1);
}

inline void dComIfGd_entryZSortXluList(J3DPacket* i_packet, cXyz& param_1) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.entryZSortXluList(i_packet, param_1);
}

inline void dComIfGd_drawCopy2D() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawCopy2D();
}

inline void dComIfGd_drawOpaListSky() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListSky();
}

inline void dComIfGd_drawXluListSky() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListSky();
}

inline void dComIfGd_drawOpaListBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListBG();
}

inline void dComIfGd_drawOpaListDarkBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListDarkBG();
}

inline void dComIfGd_drawOpaListMiddle() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListMiddle();
}

inline void dComIfGd_drawOpaList() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaList();
}

inline void dComIfGd_drawOpaListDark() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListDark();
}

inline void dComIfGd_drawOpaListPacket() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListPacket();
}

inline void dComIfGd_drawXluListBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListBG();
}

inline void dComIfGd_drawXluListDarkBG() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListDarkBG();
}

inline void dComIfGd_drawXluList() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluList();
}

inline void dComIfGd_drawXluListDark() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListDark();
}

inline void dComIfGd_drawXluListInvisible() {
    ZoneScoped;
#ifdef TARGET_PC
    if (!dusk::getSettings().game.disableWaterRefraction) {
#endif
        g_dComIfG_gameInfo.drawlist.drawXluListInvisible();
#ifdef TARGET_PC
    }
#endif
}

inline void dComIfGd_drawOpaListInvisible() {
    ZoneScoped;
#ifdef TARGET_PC
    if (!dusk::getSettings().game.disableWaterRefraction) {
#endif
        g_dComIfG_gameInfo.drawlist.drawOpaListInvisible();
#ifdef TARGET_PC
        }
#endif
}

inline void dComIfGd_drawXluListZxlu() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListZxlu();
}

inline void dComIfGd_drawXluList2DScreen() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluList2DScreen();
}

inline void dComIfGd_drawOpaList3Dlast() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaList3Dlast();
}

inline void dComIfGd_draw2DOpa() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.draw2DOpa();
}

inline void dComIfGd_draw2DOpaTop() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.draw2DOpaTop();
}

inline void dComIfGd_draw2DXlu() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.draw2DXlu();
}

inline void dComIfGd_drawOpaListFilter() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListFilter();
}

inline void dComIfGd_drawIndScreen() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawOpaListP0();
}

inline void dComIfGd_drawListZxlu() {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawXluListZxlu();
}

inline void dComIfGd_drawShadow(Mtx param_0) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.drawShadow(param_0);
}

inline void dComIfGd_imageDrawShadow(Mtx param_0) {
    ZoneScoped;
    g_dComIfG_gameInfo.drawlist.imageDrawShadow(param_0);
}

inline void dComIfGd_set3DlineMat(mDoExt_3DlineMat_c* param_0) {
    g_dComIfG_gameInfo.drawlist.set3DlineMat(param_0);
}

inline void dComIfGd_set3DlineMatDark(mDoExt_3DlineMat_c* param_0) {
    g_dComIfG_gameInfo.drawlist.set3DlineMatDark(param_0);
}

#if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
inline void dComIfGd_setListCursor() {
    g_dComIfG_gameInfo.drawlist.setOpaListCursor();
    g_dComIfG_gameInfo.drawlist.setXluListCursor();
}
#endif

#endif /* D_COM_D_COM_INF_GAME_H */
