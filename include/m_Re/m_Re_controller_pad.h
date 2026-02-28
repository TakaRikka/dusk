#ifndef M_RE_CONTROLLER_PAD_H
#define M_RE_CONTROLLER_PAD_H

#include <revolution/mtx.h>
#include <revolution/kpad.h>

#if PLATFORM_SHIELD
#define RECPD_SAMPLING_BUF_COUNT 16
#else
#define RECPD_SAMPLING_BUF_COUNT 10
#endif

class mReCPd {
public:
    struct Pad {
        struct Acc {
            void clean();
            void changeStts(u32, bool);
            void calcSwingVec();
            void calcSttsRem();
            void calcSttsFs();

            /* 0x000 */ Vec field_0x0[120];
            /* 0x5A0 */ Vec field_0x5a0[120];
            /* 0xB40 */ int field_0xb40;
            /* 0xB44 */ int m_swing;
            /* 0xB48 */ Vec2 m_swingVec;
            /* 0xB50 */ u8 field_0xb50;
            /* 0xB51 */ u8 field_0xb51;
            /* 0xB54 */ f32 field_0xb54;
            /* 0xB58 */ Vec field_0xb58;
            /* 0xB64 */ u32 field_0xb64;
            /* 0xB68 */ Mtx field_0xb68;
            /* 0xB98 */ f32 field_0xb98;
            /* 0xB9C */ int field_0xb9c;
            /* 0xBA0 */ f32 field_0xba0;
            /* 0xBA4 */ f32 field_0xba4;
            /* 0xBA8 */ u8 field_0xba8[0xBAC - 0xBA8];
            /* 0xBAC */ int field_0xbac;
            /* 0xBB0 */ f32 field_0xbb0;
            /* 0xBB4 */ f32 field_0xbb4;
            /* 0xBB8 */ u8 field_0xbb8[0xBBC - 0xBB8];
            /* 0xBBC */ int field_0xbbc;
            /* 0xBC0 */ int field_0xbc0;
        };

        void clean();
        void cleanOneTime();
        void copyRemAcc();
        void copyFSAcc();
        void cleanTrig();
        void cleanHold();
        void cleanRelease();
        void clearUpDown();
        void calcDpdBox(Vec2*, Vec2*, f32, f32);
        void calcDpdOld();
        void calcDpd2DPos();
        void calcDpdRatioPos();
        void calcDpdPlayCirPos();
        void calcDpdPlayBoxPos();

        /* 0x0000 */ int field_0x0;
#if PLATFORM_SHIELD
        /* 0x0004 */ u8 unk_shield_0x04[0x8 - 0x4];
#endif
        /* 0x0004 */ KPADStatus field_0x4[RECPD_SAMPLING_BUF_COUNT];
        /* 0x0554 */ int field_0x554;
#if PLATFORM_SHIELD
        /* 0x0558 */ u8 unk_shield_0x558[0x55c - 0x558];
#endif
        /* 0x0558 */ KPADStatus field_0x558;
        /* 0x05E0 */ Acc m_remAcc;
        /* 0x11A4 */ Acc m_FSAcc;
        /* 0x1D68 */ Vec2 m_stick3D;
        /* 0x1D70 */ f32 m_stickValue;
        /* 0x1D74 */ s16 m_stickAngle;
        /* 0x1D76 */ s16 m_stick3DAngle;
        /* 0x1D78 */ Vec2 field_0x1d78;
        /* 0x1D80 */ Vec2 field_0x1d80;
        /* 0x1D88 */ Vec2 m_dpd_2d_pos;
        /* 0x1D90 */ Vec2 m_dpd_ratio_pos;
        /* 0x1D98 */ f32 field_0x1d98;
        /* 0x1D9C */ f32 field_0x1d9c;
        /* 0x1DA0 */ Vec2 m_dpd_play_box_pos;
        /* 0x1DA8 */ Vec2 field_0x1da8;
        /* 0x1DB0 */ Vec2 field_0x1db0;
        /* 0x1DB8 */ f32 field_0x1db8;
        /* 0x1DBC */ f32 field_0x1dbc;
        /* 0x1DC0 */ f32 field_0x1dc0;
        /* 0x1DC4 */ int field_0x1dc4;
        /* 0x1DC8 */ int field_0x1dc8;
        /* 0x1DCC */ int m_swingBlock;
        /* 0x1DD0 */ int field_0x1dd0;
        /* 0x1DD4 */ u8 field_0x1dd4;
        /* 0x1DD8 */ int field_0x1dd8;
        /* 0x1DDC */ bool field_0x1ddc;
        /* 0x1DDD */ u8 field_0x1ddd;
        /* 0x1DE0 */ int field_0x1de0;
        /* 0x1DE4 */ int field_0x1de4;
        /* 0x1DE8 */ u8 field_0x1de8;
        /* 0x1DEC */ int field_0x1dec;
        /* 0x1DF0 */ u8 field_0x1df0;
        /* 0x1DF4 */ int field_0x1df4;
        /* 0x1DF8 */ u32 dev_type;
        /* 0x1DFC */ u32 field_0x1dfc;
        /* 0x1E00 */ int field_0x1e00;
        /* 0x1E04 */ u8 field_0x1e04;
        /* 0x1E05 */ u8 field_0x1e05;
    };

    struct motorWave_t {
        u8* field_0x0;
        u16 field_0x4;
        u16 field_0x6;
        u8 field_0x8;
    };

    static void enableDpd(u32);
    static void disableDpd(u32);
    static void onConnect(u32);
    static void setExtensionCallback();
    static void onGetInfoAsync(u32);
    static void create();
    static void startMoterWave(motorWave_t*, u8*, bool);
    static void startMoterWave(int, u8*, bool);
    static void stopMoterWave(motorWave_t*);
    static void stopMoterWave(int);
    static void updateMoterWave();
    static void stopMoter();
    static BOOL getLowBat(u32 chan);
    static void setLowBat(u32 chan, BOOL low_bat);
    static void onLowBatChk(u32 chan);
    static void procNoData(u32 chan);
    static void read();
    static f32 calibrateDist(int);
    static Vec2& getDpd2DPos(u32 chan);
    static Vec2& getDpdRatioPos(u32 chan);
    static Vec2& getDpdPlayBoxPos(u32 chan);
    static BOOL chkDpdOk(u32 chan);
    static BOOL chkDpdValid(u32 chan);
    static BOOL chkDpdPosIn(u32 chan);
    static void getKPADAcc(u32 chan, Vec*, int);
    static s16 getAngleXy(u32 chan);
    static s16 getAngleZy(u32 chan);
    static f32 getKPADAccValue(u32 chan, int);
    static f32 getKPADAccValueMax(u32 chan);
    static f32 getKPADAccSpeed(u32 chan, int);
    static f32 getKPADAccSpeedMax(u32 chan);
    static f32 getKPADFSAccValue(u32 chan, int);
    static f32 getKPADFSAccValueMax(u32 chan);
    static s16 getFSStickAngle3DRev(u32 chan);
    static void calcUnderVec(Pad::Acc* acc_p, f32);

    static u32 getHoldB(u32);
    static u32 getHoldMinus(u32);
    static u32 getHoldPlus(u32);
    static u32 getHoldZ1(u32);
    static u32 getTrigB(u32);
    static u32 getTrigMinus(u32);
    static u32 getTrigHome(u32);
    static u32 getTrigPlus(u32);
    static u32 getTrigZ1(u32);
    static u32 chkSwingBlock(u32);
    static u32 getTrigSwing(u32);
    static Vec2& getSwingVec(u32);
    static u32 chkSimpleProbe(u32);
    static BOOL chkDevTypeValid(u32);
    static u32 getFSTrigSwing(u32);
    
    static void plusCalValue();
    static void minusCalValue();

    // made up, copying concepts from m_Do_controller_pad
    static WPADInfo& getPadInfo(u32 i_pad) { return m_pad_info[i_pad]; }
    static Pad& getPad(u32 i_pad) { return m_pad[i_pad]; }
    static u32 getTrig(u32 i_pad) { return getPad(i_pad).field_0x4[0].trig; }
    static u32 getHold(u32 i_pad) { return getPad(i_pad).field_0x4[0].hold; }

    static u32 getHoldA(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_A; }
    static u32 getHoldRight(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_RIGHT; }
    static u32 getHoldLeft(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_LEFT; }
    static u32 getHoldUp(u32 i_pad) { return getHold(i_pad) & WPAD_BUTTON_UP; }
    static u32 getTrigA(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_A; }
    static u32 getTrigRight(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_RIGHT; }
    static u32 getTrigLeft(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_LEFT; }

    static u32 getTrigUp(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_UP; }
    static u32 getTrigStart(u32 i_pad) { return getTrig(i_pad) & WPAD_BUTTON_PLUS; }

    static WPADInfo m_pad_info[WPAD_MAX_CONTROLLERS];
    static Pad m_pad[WPAD_MAX_CONTROLLERS];
    static motorWave_t m_motorWave[WPAD_MAX_CONTROLLERS];
    static int m_cal_value;
};

#endif /* M_RE_CONTROLLER_PAD_H */
