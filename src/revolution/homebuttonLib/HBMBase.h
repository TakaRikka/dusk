#ifndef HOMEBUTTON_BASE_H
#define HOMEBUTTON_BASE_H

#include <revolution/ax.h>
#include <revolution/axfx.h>
#include <revolution/wpad.h>
#include "HBMCommon.h"
#include "HBMController.h"
#include "HBMGUIManager.h"
#include "nw4hbm/lyt/drawInfo.h"
#include "nw4hbm/lyt/layout.h"
#include "nw4hbm/snd/DvdSoundArchive.h"
#include "nw4hbm/snd/MemorySoundArchive.h"
#include "nw4hbm/snd/NandSoundArchive.h"
#include "nw4hbm/snd/SoundArchivePlayer.h"
#include "nw4hbm/snd/SoundPlayer.h"
#include "nw4hbm/snd/SoundSystem.h"


#include <new>

namespace homebutton {
    static void initgx();
    static void drawBlackPlate(f32 left, f32 top, f32 right, f32 bottom);
    static u32 get_comma_length(char* pBuf);
    static void SpeakerCallback(OSAlarm* alm, OSContext* ctx);
    static void MotorCallback(OSAlarm* alm, OSContext* ctx);
    static void RetrySimpleSyncCallback(OSAlarm* alm, OSContext* ctx);
    static void SimpleSyncCallback(s32 result, s32 num);
}  // namespace homebutton

namespace nw4hbm {

    namespace lyt {
        class ArcResourceAccessor;
        class ArcResourceLink;
        class Layout;
        class MultiArcResourceAccessor;
        class Pane;
    }  // namespace lyt

    namespace ut {
        class ResFont;
    }

}  // namespace nw4hbm

namespace homebutton {

    class HomeButton;
    class Controller;
    class GroupAnmController;
    class RemoteSpk;

    class HomeButtonEventHandler : public gui::EventHandler {
    public:
        HomeButtonEventHandler(homebutton::HomeButton* pHomeButton) : mpHomeButton(pHomeButton) {}

        /* 0x08 */ virtual void onEvent(u32 uID, u32 uEvent, void* pData);

        homebutton::HomeButton* getHomeButton() { return mpHomeButton; }

    private:
        /* 0x00 (base) */
        /* 0x08 */ HomeButton* mpHomeButton;
    };  // size = 0x0C

    class HomeButton {
    private:
        typedef enum {
            /* 0 */ eSeq_Normal,
            /* 1 */ eSeq_Control,
            /* 2 */ eSeq_Cmn,
        } eSeq;

        class BlackFader {
        public:
            BlackFader(int maxFrame) {
                init(maxFrame);
                setColor(0, 0, 0);
            }

            void setColor(u8 r, u8 g, u8 b) {
                red_ = r;
                green_ = g;
                blue_ = b;
            }

            int getFrame() const { return frame_; }
            int getMaxFrame() const { return maxFrame_; }
            void start() { state_ = 1; }
            GXColor GetColor(u8 alpha) { return (GXColor){red_, green_, blue_, alpha}; }

            bool isDone();
            void init(int maxFrame);
            void calc();
            void draw();

        private:
            /* 0x00 */ int frame_;
            /* 0x04 */ int maxFrame_;
            /* 0x08 */ int state_;
            /* 0x0D */ u8 red_;
            /* 0x0E */ u8 green_;
            /* 0x0F */ u8 blue_;
        };  // size = 0x10

    public:
        HomeButton(const HBMDataInfo* dataInfo);
        ~HomeButton();

        int getVolume();
        HBMSelectBtnNum getSelectBtnNum();
        bool isActive() const;
        bool isUpBarActive() const;
        bool isDownBarActive();
        void setAdjustFlag(int flag);
        void setForcusSE();
        void setSimpleSyncAlarm(int type);
        void setSpeakerAlarm(int chan, int msec);
        void setVolume(int vol);
        bool getVibFlag();
        int getPaneNo(const char*);
        void setVibFlag(bool flag);
        void create();
        void init();
        void calc(const HBMControllerData* pController);
        void draw();
        void update(const HBMControllerData* pController);
        void updateTrigPane();
        void startPointEvent(const char* pPane, void* pData);
        void startLeftEvent(const char* pPane);
        void startTrigEvent(const char* pPane);
        int findAnimator(int pane, int anm);
        int findGroupAnimator(int pane, int anm);
        void callSimpleSyncCallback(s32 result, s32 num);
        void startBlackOut();

        const HBMDataInfo* getHBMDataInfo() { return mpHBInfo; }
        Controller* getController(int chan) { return mpController[chan]; }
        const char* getFuncPaneName(int no) { return scFuncTouchPaneName[no]; }
        const char* getPaneName(int no) { return scBtnName[no]; }
        bool getReassignedFlag() const { return mReassignedFlag; }
        HomeButtonEventHandler* getEventHandler() const { return mpHomeButtonEventHandler; }
        nw4hbm::snd::SoundArchivePlayer* GetSoundArchivePlayer() { return mpSoundArchivePlayer; }
        void setEndSimpleSyncFlag(bool flag) { mEndSimpleSyncFlag = flag; }
        void setReassignedFlag(bool flag) { mReassignedFlag = flag; }
        void setSimpleSyncFlag(bool flag) { mSimpleSyncFlag = flag; }

        static void createInstance(const HBMDataInfo* dataInfo);
        static HomeButton* getInstance() { return spHomeButtonObj; }
        static void deleteInstance();

    private:
        void init_battery(const HBMControllerData* pController);
        void calc_battery(int chan);
        void reset_battery();

        void init_sound();
        void fadeout_sound(f32 gain);

        void init_msg();
        void init_vib();
        void init_volume();

        void set_config();
        void set_text();

        void calc_fadeoutAnm();

        void update_controller(int id);
        void update_posController(int id);

        void reset_btn();
        void reset_control();
        void reset_guiManager(int num);
        void reset_window();

    public:
        void play_sound(int id);
        void createSound(nw4hbm::snd::NandSoundArchive* pNandSoundArchive, bool bCreateSoundHeap);
        void deleteSound();
        void draw_impl();
        void updateSoundArchivePlayer();
        void setSoundVolume(f32 volume);
        inline void stopSound(bool checkFlag);
        void initSound(const char* path);
        void updateSound();

        void PlaySeq(int num) {
            if (mpSoundArchivePlayer != NULL && mpSoundHandle != NULL) {
                mpSoundHandle->DetachSound();
                mpSoundArchivePlayer->StartSound(mpSoundHandle, num);
            }
        }

    private:
        /* 0x000 */ eSeq mSequence;
        /* 0x004 */ const HBMDataInfo* mpHBInfo;
        /* 0x008 */ int mButtonNum;
        /* 0x00C */ int mAnmNum;
        /* 0x010 */ int mState;
        /* 0x014 */ int mSelectAnmNum;
        /* 0x018 */ int mMsgCount;
        /* 0x01C */ int mPaneCounter[14];
        /* 0x054 */ int mPadDrawTime[WPAD_MAX_CONTROLLERS];
        /* 0x064 */ int mForcusSEWaitTime;
        /* 0x068 */ int mBar0AnmRev;
        /* 0x06C */ int mBar1AnmRev;
        /* 0x070 */ int mBar0AnmRevHold;
        /* 0x074 */ int mBar1AnmRevHold;
        /* 0x078 */ int mGetPadInfoTime;
        /* 0x07C */ bool mControllerFlag[WPAD_MAX_CONTROLLERS];
        /* 0x080 */ int mVolumeNum;
        /* 0x084 */ bool mVibFlag;
        /* 0x085 */ bool mControlFlag;
        /* 0x086 */ bool mLetterFlag;
        /* 0x087 */ bool mAdjustFlag;
        /* 0x088 */ bool mReassignedFlag;
        /* 0x089 */ bool mSimpleSyncFlag;
        /* 0x08A */ bool mEndSimpleSyncFlag;
        /* 0x08B */ bool mInitFlag;
        /* 0x08C */ bool mForceSttInitProcFlag;
        /* 0x08D */ bool mForceSttFadeInProcFlag;
        /* 0x08E */ bool mEndInitSoundFlag;
        /* 0x08F */ bool mForceStopSyncFlag;
        /* 0x090 */ bool mForceEndMsgAnmFlag;
#if HBM_REVISION > 1
        /* 0x094 */ int mSoundRetryCnt;
#endif
        /* 0x098 */ int mDialogFlag[4];
        /* 0x0A8 */ char* mpLayoutName;
        /* 0x0AC */ char* mpAnmName;
        /* 0x0B0 */ HBMSelectBtnNum mSelectBtnNum;
        /* 0x0B4 */ wchar_t* mpText[7][6];
        /* 0x15C */ WPADInfo mWpadInfo[WPAD_MAX_CONTROLLERS];
        /* 0x1BC */ WPADSimpleSyncCallback mSimpleSyncCallback;
        /* 0x1CC */ f32 mOnPaneVibFrame[4];
        /* 0x1DC */ f32 mOnPaneVibWaitFrame[4];
        /* 0x1E0 */ int mWaitStopMotorCount;
        /* 0x1E4 */ nw4hbm::lyt::Layout* mpLayout;
        /* 0x1E8 */ nw4hbm::lyt::Layout* mpCursorLayout[WPAD_MAX_CONTROLLERS];
        /* 0x1F8 */ nw4hbm::lyt::ArcResourceAccessor* mpResAccessor;
        /* 0x1FC */ gui::PaneManager* mpPaneManager;
        /* 0x200 */ HomeButtonEventHandler* mpHomeButtonEventHandler;
        /* 0x204 */ nw4hbm::lyt::DrawInfo mDrawInfo;
        /* 0x258 */ Controller* mpController[WPAD_MAX_CONTROLLERS];
        /* 0x268 */ RemoteSpk* mpRemoteSpk;
        /* 0x26C */ GroupAnmController* mpAnmController[12];
        /* 0x29C */ GroupAnmController* mpGroupAnmController[74];
        /* 0x3C4 */ GroupAnmController* mpPairGroupAnmController[15];
        /* 0x400 */ BlackFader mFader;
        /* 0x410 */ OSAlarm mAlarm[WPAD_MAX_CONTROLLERS];
        /* 0x4D0 */ OSAlarm mSpeakerAlarm[WPAD_MAX_CONTROLLERS];
        /* 0x590 */ OSAlarm mSimpleSyncAlarm;
        /* 0x5C0 */ nw4hbm::snd::SoundArchivePlayer* mpSoundArchivePlayer;
        /* 0x5C4 */ nw4hbm::snd::DvdSoundArchive* mpDvdSoundArchive;
        /* 0x5C8 */ nw4hbm::snd::MemorySoundArchive* mpMemorySoundArchive;
        /* 0x5CC */ nw4hbm::snd::NandSoundArchive* mpNandSoundArchive;
        /* 0x5D0 */ nw4hbm::snd::SoundHeap* mpSoundHeap;
        /* 0x5D4 */ nw4hbm::snd::SoundHandle* mpSoundHandle;
        /* 0x5D8 */ u16 mAppVolume[3];
        /* 0x5E0 */ AXFXAllocFunc mAxFxAlloc;
        /* 0x5E4 */ AXFXFreeFunc mAxFxFree;
        /* 0x5E8 */ AXFX_REVERBHI mAxFxReverb;
        /* 0x748 */ AXAuxCallback mAuxCallback;
        /* 0x74C */ void* mpAuxContext;
        /* 0x750 */ f32 mFadeOutSeTime;

        // static members
    private:
        static HomeButton* spHomeButtonObj;

        static const char* scCursorLytName[WPAD_MAX_CONTROLLERS];
        static const char* scCursorPaneName;
        static const char* scCursorRotPaneName;
        static const char* scCursorSRotPaneName;

        static const char* scBtnName[4];
        static const char* scTxtName[4];
        static const char* scGrName[8];
        static const char* scAnimName[3];
        static const char* scPairGroupAnimName[15];
        static const char* scPairGroupName[15];
        static const char* scGroupAnimName[22];
        static const char* scGroupName[35];
        static const char* scFuncPaneName[5];
        static const char* scFuncTouchPaneName[10];
        static const char* scFuncTextPaneName[3];
        static const char* scBatteryPaneName[WPAD_MAX_CONTROLLERS][4];
    };  // size = 0x740

}  // namespace homebutton

#endif
