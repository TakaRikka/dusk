#ifndef D_S_D_S_LOGO_H
#define D_S_D_S_LOGO_H

#include "f_op/f_op_scene.h"
#include "m_Do/m_Do_dvd_thread.h"

class JKRExpHeap;
class JKRHeap;
struct ResTIMG;
class dDlst_2D_c;
class mDoDvdThd_mountXArchive_c;
class mDoDvdThd_mountArchive_c;
class mDoDvdThd_toMainRam_c;

class dScnLogo_c : public scene_class {
public:
    enum {
        /*  0 */ EXEC_WARNING_IN,
        /*  1 */ EXEC_WARNING_DISP,
        /*  2 */ EXEC_WARNING_OUT,
        /*  3 */ EXEC_NINTENDO_IN,
        /*  4 */ EXEC_NINTENDO_OUT,
        /*  5 */ EXEC_DOLBY_IN,
        /*  6 */ EXEC_DOLBY_OUT,
        /*  7 */ EXEC_DOLBY_OUT2,
        /*  8 */ EXEC_PROG_IN,
        /*  9 */ EXEC_PROG_SEL,
        /* 10 */ EXEC_PROG_OUT,
        /* 11 */ EXEC_PROG_SET,
        /* 12 */ EXEC_PROG_SET2,
        /* 13 */ EXEC_PROG_CHANGE,
        /* 14 */ EXEC_DVD_WAIT,
        /* 15 */ EXEC_SCENE_CHANGE,

        #if PLATFORM_WII || PLATFORM_SHIELD
        /* 16 */ EXEC_STRAP_IN,
        /* 17 */ EXEC_STRAP_DISP,
        /* 18 */ EXEC_STRAP_OUT,
        /* 19 */ EXEC_STRAP_OUT2,
        #endif

        #if VERSION == VERSION_SHIELD
        /* 20 */ EXEC_MOC_IN,
        /* 21 */ EXEC_MOC_DISP,
        /* 22 */ EXEC_MOC_OUT,
        /* 23 */ EXEC_NVLOGO_IN,
        /* 24 */ EXEC_NVLOGO_DISP,
        /* 25 */ EXEC_NVLOGO_OUT,
        #endif
    };

    dScnLogo_c() {}
    void preLoad_dyl_create();
    void preLoad_dyl_remove();
    bool preLoad_dyl();
    void checkProgSelect();
    int draw();
    void progInDraw();
    void progSelDraw();
    void progOutDraw();
    void progSetDraw();
    void progSet2Draw();
    void progChangeDraw();
    void warningInDraw();
    void warningDispDraw();
    void warningOutDraw();
    void nintendoInDraw();
    void nintendoOutDraw();
    void dolbyInDraw();
    void dolbyOutDraw();
    void dolbyOutDraw2();
    void dvdWaitDraw();
    void nextSceneChange();
    ~dScnLogo_c();
    int create();
    void dvdDataLoad();
    void setProgressiveMode(u8);
    u8 getProgressiveMode();
    bool isProgressiveMode();
    void setRenderMode();

    #if VERSION == VERSION_GCN_PAL || PLATFORM_WII || PLATFORM_SHIELD
    u8 getPalLanguage();
    #endif

    #if PLATFORM_WII || PLATFORM_SHIELD
    void logoInitWii();
    void strapInDraw();
    void strapDispDraw();
    void strapOutDraw();
    void strapOut2Draw();
    #else
    void logoInitGC();
    #endif

    #if VERSION == VERSION_SHIELD
    void mocInDraw();
    void mocDispDraw();
    void mocOutDraw();
    void nvLogoInDraw();
    void nvLogoDispDraw();
    void nvLogoOutDraw();
    #endif

    mDoDvdThd_mountXArchive_c* aramMount(const char* i_arcPath, JKRHeap* i_heap) {
        return mDoDvdThd_mountXArchive_c::create(i_arcPath, 0, JKRArchive::MOUNT_ARAM, i_heap);
    }

    mDoDvdThd_mountXArchive_c* onMemMount(const char* i_arcPath) {
        return mDoDvdThd_mountXArchive_c::create(i_arcPath, 0, JKRArchive::MOUNT_MEM, NULL);
    }
    
    static void onOpeningCut() {
        #if DEBUG
        mOpeningCut = true;
        #endif
    }

    static u8 isOpeningCut() {
        #if DEBUG
        return mOpeningCut;
        #else
        return 0;
        #endif
    }
    
    #if DEBUG
    static u8 mOpeningCut;
    #endif

public:
    /* 0x1C4 */ request_of_phase_process_class field_0x1c4;
    /* 0x1CC */ mDoDvdThd_toMainRam_c* sceneCommand;
    /* 0x1D0 */ JKRExpHeap* mLogoHeap;
    /* 0x1D4 */ JKRExpHeap* mLogo01Heap;
    /* 0x1D8 */ JKRHeap* mpHeap;
    /* 0x1DC */ dDlst_2D_c* mWarning;
    /* 0x1E0 */ dDlst_2D_c* mWarningStart;
    /* 0x1E4 */ dDlst_2D_c* mNintendoLogo;
    /* 0x1E8 */ dDlst_2D_c* mDolbyLogo;
    /* 0x1EC */ dDlst_2D_c* mProgressiveChoice;
    /* 0x1F0 */ dDlst_2D_c* mProgressiveYes;
    /* 0x1F4 */ dDlst_2D_c* mProgressiveNo;
    /* 0x1F8 */ dDlst_2D_c* mProgressiveSel;
#if PLATFORM_WII || PLATFORM_SHIELD
    /* 0x1FC */ dDlst_2D_c* mStrapImg;
#endif
#if VERSION == VERSION_SHIELD
    /* 0x200 */ dDlst_2D_c* mNvLogo;
    /* 0x204 */ dDlst_2D_c* mMocImg;
#endif
#if VERSION == VERSION_GCN_PAL
    /* 0x1FC */ mDoDvdThd_mountArchive_c* mpPalLogoResCommand;
#endif
    /* 0x1FC */ request_of_phase_process_class* m_preLoad_dylPhase;
    /* 0x200 */ ResTIMG* mProgressivePro;
    /* 0x204 */ ResTIMG* mProgressiveInter;
    /* 0x208 */ u8 mExecCommand;
    /* 0x209 */ u8 field_0x209;
    /* 0x20A */ u8 field_0x20a;
    /* 0x20B */ u8 field_0x20b;
    /* 0x20C */ u16 mTimer;
    /* 0x20E */ u16 field_0x20e;
    /* 0x210 */ u16 field_0x210;
    /* 0x212 */ u16 field_0x212;
    /* 0x214 */ u16 field_0x214;
    /* 0x218 */ u32 field_0x218;
    /* 0x21C */ void* dummyGameAlloc;
#if PLATFORM_WII || VERSION == VERSION_SHIELD
    /* 0x224 */ mDoDvdThd_toMainRam_c* mpHomeBtnCommand;
    /* 0x228 */ int mHomeBtnRegion;
#endif
    /* 0x220 */ mDoDvdThd_mountXArchive_c* mpField0Command;
    /* 0x224 */ mDoDvdThd_mountXArchive_c* mpAlAnmCommand;
    /* 0x228 */ u8 field_0x228[4];
    /* 0x22C */ mDoDvdThd_mountXArchive_c* mpFmapResCommand;
    /* 0x230 */ mDoDvdThd_mountXArchive_c* mpDmapResCommand;
    /* 0x234 */ mDoDvdThd_mountXArchive_c* mpCollectResCommand;
    /* 0x238 */ u8 field_0x238[4];
    /* 0x23C */ mDoDvdThd_mountXArchive_c* mpItemIconCommand;
    /* 0x240 */ mDoDvdThd_mountXArchive_c* mpRingResCommand;
    /* 0x244 */ u8 field_0x244[4];
    /* 0x248 */ mDoDvdThd_mountXArchive_c* mpPlayerNameCommand;
    /* 0x24C */ mDoDvdThd_mountXArchive_c* mpItemInfResCommand;
    /* 0x250 */ mDoDvdThd_mountXArchive_c* mpButtonCommand;
    /* 0x254 */ u8 field_0x254[4];
    /* 0x258 */ mDoDvdThd_mountXArchive_c* mpCardIconCommand;
    /* 0x25C */ mDoDvdThd_mountXArchive_c* mpBmgResCommand;
    /* 0x260 */ mDoDvdThd_mountXArchive_c* mpMsgComCommand;
    /* 0x264 */ mDoDvdThd_mountXArchive_c* mpMsgResCommand[7];
    /* 0x280 */ u8 field_0x280[0x10];
    /* 0x290 */ mDoDvdThd_mountXArchive_c* mpFontResCommand;
    /* 0x294 */ mDoDvdThd_mountXArchive_c* mpMain2DCommand;
    /* 0x298 */ mDoDvdThd_mountXArchive_c* mpRubyResCommand;
    /* 0x29C */ mDoDvdThd_toMainRam_c* mParticleCommand;
    /* 0x2A0 */ mDoDvdThd_toMainRam_c* mItemTableCommand;
    /* 0x2A4 */ mDoDvdThd_toMainRam_c* mEnemyItemCommand;
};

static int phase_0(dScnLogo_c* logo);
static int phase_1(dScnLogo_c* logo);
static int phase_2(dScnLogo_c* logo);

typedef int (*dScnLogo_Method)(dScnLogo_c*);;

#endif /* D_S_D_S_LOGO_H */
