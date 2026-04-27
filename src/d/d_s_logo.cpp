/**
 * d_s_logo.cpp
 * Game Boot Logo's Display
 */

#include <cstdio>

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_logo.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "c/c_dylink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_map_path_dmap.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"
#include <cstring>
#include "m_Do/m_Do_main.h"
#include "JSystem/JUtility/JUTConsole.h"

#include "dusk/logging.h"
#include "dusk/version.hpp"

#if !PLATFORM_GCN
#include <revolution/os.h>
#include <revolution/sc.h>
#include "m_Do/m_Do_Reset.h"

#include "res/Object/LogoUsWii.h"
#endif

#if PLATFORM_WII || VERSION == VERSION_SHIELD
#include "m_Re/m_Re_controller_pad.h"
#include "d/d_cursor_mng.h"
#include "d/d_home_button.h"

struct homeBtnData {
    /* 0x0 */ int region;
    /* 0x4 */ const char* path;
};
#endif

#if TARGET_PC
using namespace dusk::version;

#define LOGO_ARC versionSelect<const char*>({{GameVersion::GcnJpn, "Logo"}, {GameVersion::GcnPal, "LogoPal"}}, "LogoUs")
#define MSG_PATH versionSelect<const char*>({{GameVersion::GcnJpn, "/res/Msgjp/bmgres.arc"}}, "/res/Msgus/bmgres.arc")
#elif VERSION == VERSION_SHIELD
#define LOGO_ARC "LogoUs"
#define MSG_PATH  "/res/Msgcn/bmgres.arc"
#elif VERSION == VERSION_GCN_JPN
#define LOGO_ARC  "Logo"
#define MSG_PATH  "/res/Msgjp/bmgres.arc"
#elif VERSION == VERSION_GCN_PAL
#define LOGO_ARC "LogoPal"
#define MSG_PATH  "/res/Msgus/bmgres.arc"
#else
#define LOGO_ARC "LogoUs"
#define MSG_PATH  "/res/Msgus/bmgres.arc"
#endif

#if VERSION == VERSION_GCN_PAL
#define PROGRESSIVE_MODE_OFF OS_EURGB60_OFF
#define PROGRESSIVE_MODE_ON  OS_EURGB60_ON
#else
#define PROGRESSIVE_MODE_OFF OS_PROGRESSIVE_MODE_OFF
#define PROGRESSIVE_MODE_ON  OS_PROGRESSIVE_MODE_ON
#endif

#if PLATFORM_WII || VERSION == VERSION_SHIELD_DEBUG
#define FMAP_RES_PATH "/res/LayoutRevo/fmapresR.arc"
#define DMAP_RES_PATH "/res/LayoutRevo/dmapresR.arc"
#define COLLECT_RES_PATH "/res/LayoutRevo/clctresR.arc"

#define MSG_COM_PATH "/res/LayoutRevo/msgcomR.arc"
#define MSG_RES0_PATH "/res/LayoutRevo/msgres00R.arc"
#define MSG_RES1_PATH "/res/LayoutRevo/msgres01R.arc"
#define MSG_RES2_PATH "/res/LayoutRevo/msgres02R.arc"
#define MSG_RES3_PATH "/res/LayoutRevo/msgres03R.arc"
#else
#define FMAP_RES_PATH "/res/Layout/fmapres.arc"
#define DMAP_RES_PATH "/res/Layout/dmapres.arc"
#define COLLECT_RES_PATH "/res/Layout/clctres.arc"

#define MSG_COM_PATH "/res/Layout/msgcom.arc"
#define MSG_RES0_PATH "/res/Layout/msgres00.arc"
#define MSG_RES1_PATH "/res/Layout/msgres01.arc"
#define MSG_RES2_PATH "/res/Layout/msgres02.arc"
#define MSG_RES3_PATH "/res/Layout/msgres03.arc"
#endif

#if PLATFORM_WII || PLATFORM_SHIELD
#define ICON_RES_PATH "/res/WiiBannerIcon/bannerIcon.arc"
#define PARTICLE_COM_PATH "/res/Particle/common-r.jpc"
#else
#define ICON_RES_PATH "/res/CardIcon/cardicon.arc"
#define PARTICLE_COM_PATH "/res/Particle/common.jpc"
#endif

#if PLATFORM_WII
#define RING_RES_PATH "/res/LayoutRevo/ringresR.arc"
#define ITEM_INF_RES_PATH "/res/LayoutRevo/itmInfResR.arc"
#define BUTTON_RES_PATH "/res/LayoutRevo/buttonR.arc"
#define MAIN2D_PATH "/res/LayoutRevo/main2DR.arc"
#else
#define RING_RES_PATH "/res/Layout/ringres.arc"
#define ITEM_INF_RES_PATH "/res/Layout/itmInfRes.arc"
#define BUTTON_RES_PATH "/res/Layout/button.arc"
#define MAIN2D_PATH "/res/Layout/main2D.arc"
#endif

class dLog_HIO_c : public JORReflexible {
public:
    dLog_HIO_c();
    virtual ~dLog_HIO_c() {}

    void genMessage(JORMContext*);

    u8 field_0x4[0x8 - 0x4];
};  // Size: 0x8

static dLog_HIO_c g_LogHIO;

dLog_HIO_c::dLog_HIO_c() {}

#if DEBUG
void dLog_HIO_c::genMessage(JORMContext*) {}
#endif

void dScnLogo_c::preLoad_dyl_create() {
    m_preLoad_dylPhase = JKR_NEW_ARRAY(request_of_phase_process_class, 14);
    JUT_ASSERT(194, m_preLoad_dylPhase != NULL);

    memset(m_preLoad_dylPhase, 0, sizeof(request_of_phase_process_class) * 14);
}

void dScnLogo_c::preLoad_dyl_remove() {
    JKR_DELETE_ARRAY(m_preLoad_dylPhase);
}

typedef void (dScnLogo_c::*execFunc)();
static execFunc l_execFunc[] = {
    &dScnLogo_c::warningInDraw,   &dScnLogo_c::warningDispDraw, &dScnLogo_c::warningOutDraw,
    &dScnLogo_c::nintendoInDraw,  &dScnLogo_c::nintendoOutDraw, &dScnLogo_c::dolbyInDraw,
    &dScnLogo_c::dolbyOutDraw,    &dScnLogo_c::dolbyOutDraw2,   &dScnLogo_c::progInDraw,
    &dScnLogo_c::progSelDraw,     &dScnLogo_c::progOutDraw,     &dScnLogo_c::progSetDraw,
    &dScnLogo_c::progSet2Draw,    &dScnLogo_c::progChangeDraw,  &dScnLogo_c::dvdWaitDraw,
    &dScnLogo_c::nextSceneChange,

    #if PLATFORM_WII || PLATFORM_SHIELD
    &dScnLogo_c::strapInDraw,
    &dScnLogo_c::strapDispDraw,
    &dScnLogo_c::strapOutDraw,
    &dScnLogo_c::strapOut2Draw,
    #endif

    #if VERSION == VERSION_SHIELD
    &dScnLogo_c::mocInDraw,
    &dScnLogo_c::mocDispDraw,
    &dScnLogo_c::mocOutDraw,
    &dScnLogo_c::nvLogoInDraw,
    &dScnLogo_c::nvLogoDispDraw,
    &dScnLogo_c::nvLogoOutDraw,
    #endif
};

static s16 const l_preLoad_dylKeyTbl[14] = {
    fpcNm_BG_e,
    fpcNm_DEMO00_e,
    fpcNm_NBOMB_e,
    fpcNm_SPINNER_e,
    fpcNm_Obj_LifeContainer_e,
    fpcNm_CROD_e,
    fpcNm_DISAPPEAR_e,
    fpcNm_Tag_Attp_e,
    fpcNm_MG_ROD_e,
    fpcNm_BOOMERANG_e,
    fpcNm_ARROW_e,
    fpcNm_SUSPEND_e,
    fpcNm_MIDNA_e,
    fpcNm_Obj_Yousei_e,
};

bool dScnLogo_c::preLoad_dyl() {
    bool ret = true;
    int var_r28 = 14;

    for (int i = 0; i < var_r28; i++) {
        int phase_state = cDylPhs::Link(&m_preLoad_dylPhase[i], l_preLoad_dylKeyTbl[i]);

        if (phase_state != cPhs_COMPLEATE_e) {
            ret = false;
        }
    }

    return ret;
}

#if DEBUG
u8 dScnLogo_c::mOpeningCut;
#endif

void dScnLogo_c::checkProgSelect() {
    #if VERSION == VERSION_GCN_PAL
    if (mDoRst::getProgSeqFlag() == 0) {
        field_0x20a = 1;

        if (isProgressiveMode()) {
            field_0x209 = 0;
        } else {
            field_0x209 = 1;
        }
    }
    #else
    if (mDoRst::getProgSeqFlag() == 0 && VIGetDTVStatus() != 0) {
        if (isProgressiveMode() || mDoCPd_c::getHoldB(PAD_1)) {
            field_0x20a = 1;
            field_0x209 = 0;
        }
    }
    #endif
}

int dScnLogo_c::draw() {
    static int sDrawLogCount = 0;
    if (sDrawLogCount < 10) {
        DuskLog.debug("dScnLogo_c::draw: mExecCommand={} mTimer={}", mExecCommand, mTimer);
    }
    cLib_calcTimer<u16>(&mTimer);
    (this->*l_execFunc[mExecCommand])();
    sDrawLogCount++;
    return 1;
}

void dScnLogo_c::progInDraw() {
    dComIfGd_set2DOpa(mProgressiveChoice);
    dComIfGd_set2DOpa(mProgressiveYes);
    dComIfGd_set2DOpa(mProgressiveNo);

    if (mTimer == 0) {
        mExecCommand = EXEC_PROG_SEL;
        mTimer = 600;
        field_0x20e = 30;
        field_0x210 = field_0x20e;
        field_0x212 = 0;
        field_0x20b = 0;
    }
}

void dScnLogo_c::progSelDraw() {
    dComIfGd_set2DOpa(mProgressiveChoice);
    dComIfGd_set2DOpa(mProgressiveYes);
    dComIfGd_set2DOpa(mProgressiveNo);

    if (field_0x20b == 0) {
        if (field_0x209 == 0) {
            #if PLATFORM_WII
            if (mReCPd::getTrigRight(PAD_1) || mReCPd::getStickX(PAD_1) > 0.5f)
            #else
            if (mDoCPd_c::getHoldRight(PAD_1) || mDoCPd_c::getStickX(PAD_1) > 0.5f)
            #endif
            {
                #if PLATFORM_WII || PLATFORM_SHIELD
                mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0);
                #else
                mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
                #endif
                field_0x209 = 1;
                field_0x20e = 30;
                field_0x210 = field_0x20e;
                field_0x212 = 0;
            }
        } else {
            #if PLATFORM_WII
            if (mReCPd::getTrigLeft(PAD_1) || mReCPd::getStickX(PAD_1) < -0.5f)
            #else
            if (mDoCPd_c::getHoldLeft(PAD_1) || mDoCPd_c::getStickX(PAD_1) < -0.5f)
            #endif
            {
                #if PLATFORM_WII || PLATFORM_SHIELD
                mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0);
                #else
                mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
                #endif
                field_0x209 = 0;
                field_0x20e = 30;
                field_0x210 = field_0x20e;
                field_0x212 = 0;
            }
        }

        #if PLATFORM_WII
        if (cAPICPad_A_TRIGGER(PAD_1) || mTimer == 0)
        #else
        if (mDoCPd_c::getTrigA(PAD_1) || mTimer == 0)
        #endif
        {
            if (field_0x209 == 0) {
                J2DPicture* pic = mProgressiveSel->getPicture();
                pic->changeTexture(mProgressivePro, 0);

                #if VERSION != VERSION_SHIELD
                setProgressiveMode(PROGRESSIVE_MODE_ON);
                #endif

                mDoRst::setProgChgFlag(1);

                #if PLATFORM_WII || PLATFORM_SHIELD
                mDoAud_seStart(Z2SE_SY_TALK_WIN_CLOSE, NULL, 0, 0);
                #else
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
                #endif
            } else {
                J2DPicture* pic = mProgressiveSel->getPicture();
                pic->changeTexture(mProgressiveInter, 0);

                #if VERSION != VERSION_SHIELD
                setProgressiveMode(PROGRESSIVE_MODE_OFF);
                #endif

                #if PLATFORM_WII || PLATFORM_SHIELD
                mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0);
                #else
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
                #endif
            }

            if (mTimer > 540) {
                field_0x20b = 1;
                field_0x214 = mTimer - 540;
            } else {
                mExecCommand = EXEC_PROG_OUT;
                mTimer = 30;
                mDoGph_gInf_c::startFadeOut(mTimer);
                field_0x20e = 30;
                field_0x210 = field_0x20e;
                field_0x212 = 0;
            }

            #if PLATFORM_WII || PLATFORM_SHIELD
            mDoRst::setProgSeqFlag(0);
            #else
            mDoRst::setProgSeqFlag(1);
            #endif
        }
    } else {
        if (field_0x214 == 0) {
            mExecCommand = EXEC_PROG_OUT;
            mTimer = 30;
            mDoGph_gInf_c::startFadeOut(30);
            field_0x20e = 30;
            field_0x210 = field_0x20e;
            field_0x212 = 0;
        } else {
            field_0x214--;
        }
    }

    f32 alpha = (f32)field_0x210 / (f32)field_0x20e;
    if (field_0x212 != 0) {
        alpha = 1.0f - alpha;
    }
    u8 r = alpha * 255.0f;
    u8 g = alpha * 200.0f;

    if (field_0x209 != 0) {
        mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));
        mProgressiveYes->getPicture()->setBlack(JUtility::TColor(0, 0, 0, 0));

        mProgressiveNo->getPicture()->setWhite(JUtility::TColor(255, 200, 0, 255));
        mProgressiveNo->getPicture()->setBlack(JUtility::TColor(r, g, 0, 0));
    } else {
        mProgressiveYes->getPicture()->setWhite(JUtility::TColor(255, 200, 0, 255));
        mProgressiveYes->getPicture()->setBlack(JUtility::TColor(r, g, 0, 0));

        mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));
        mProgressiveNo->getPicture()->setBlack(JUtility::TColor(0, 0, 0, 0));
    }

    if (field_0x210 == 0) {
        field_0x210 = field_0x20e;
        field_0x212 ^= 1;
    } else {
        field_0x210--;
    }
}

void dScnLogo_c::progOutDraw() {
    dComIfGd_set2DOpa(mProgressiveChoice);
    dComIfGd_set2DOpa(mProgressiveYes);
    dComIfGd_set2DOpa(mProgressiveNo);

    if (mTimer == 0) {
        #if VERSION == VERSION_GCN_PAL
        if (field_0x218 == 1 && field_0x209 == 0)
        #else
        if (field_0x218 != 0 && field_0x209 == 0)
        #endif
        {
            mExecCommand = EXEC_PROG_CHANGE;
            mTimer = 150;
        } else if (field_0x218 == 0 && field_0x209 != 0) {
            #if PLATFORM_WII || PLATFORM_SHIELD
            mTimer = 90;
            mExecCommand = EXEC_NINTENDO_IN;
            #else
            if (mDoRst::getWarningDispFlag() != 0) {
                mTimer = 90;
                mExecCommand = EXEC_NINTENDO_IN;
            } else {
                mTimer = 120;
                mExecCommand = EXEC_WARNING_IN;
            }
            #endif

            mDoGph_gInf_c::startFadeIn(30);
        } else {
            mExecCommand = EXEC_PROG_SET;
            mTimer = 150;
            mDoGph_gInf_c::startFadeIn(30);
        }
    }
}

void dScnLogo_c::progSetDraw() {
    dComIfGd_set2DOpa(mProgressiveSel);

    if (mTimer == 0) {
        mExecCommand = EXEC_PROG_SET2;
        mTimer = 30;
        mDoGph_gInf_c::startFadeOut(30);
    }
}

void dScnLogo_c::progSet2Draw() {
    dComIfGd_set2DOpa(mProgressiveSel);

    if (mTimer == 0) {
        if (getProgressiveMode() != 0) {
            mTimer = 150;
        } else {
            mTimer = 30;
        }

        mExecCommand = EXEC_PROG_CHANGE;
    }
}

void dScnLogo_c::progChangeDraw() {
    #if PLATFORM_SHIELD
    OSReport("progChangeDraw");
    #endif

    if (getProgressiveMode() != 0 && mTimer == 90 && field_0x209 == 0) {
        setRenderMode();
    }

    if (mTimer == 0) {
        #if VERSION == VERSION_SHIELD_DEBUG
        mExecCommand = EXEC_STRAP_IN;
        mTimer = 0;
        #elif PLATFORM_WII || PLATFORM_SHIELD
        mExecCommand = EXEC_STRAP_IN;
        mTimer = 90;
        #else
        if (mDoRst::getWarningDispFlag() != 0) {
            mTimer = 90;
            mExecCommand = EXEC_NINTENDO_IN;
        } else {
            mTimer = 120;
            mExecCommand = EXEC_WARNING_IN;
        }
        #endif

        mDoGph_gInf_c::startFadeIn(30);
    }
}

void dScnLogo_c::warningInDraw() {
    dComIfGd_set2DOpa(mWarning);

    if (mTimer == 0) {
        mExecCommand = EXEC_WARNING_DISP;
        mTimer = 3510;
        field_0x20e = 30;
        field_0x210 = field_0x20e;
        field_0x212 = 1;
    }
}

void dScnLogo_c::warningDispDraw() {
    dComIfGd_set2DOpa(mWarning);
    dComIfGd_set2DOpa(mWarningStart);

    f32 alphaf = (f32)field_0x210 / (f32)field_0x20e;
    if (field_0x212 != 0) {
        alphaf = 1.0f - alphaf;
    }

    u8 alpha = 255.0f * alphaf;
    mWarningStart->setAlpha(alpha);

    if (field_0x210 == 0) {
        field_0x210 = field_0x20e;
        field_0x212 ^= 1;
    } else {
        field_0x210--;
    }

    #if PLATFORM_WII
    if (mTimer == 0 || cAPICPad_A_TRIGGER(PAD_1) || cAPICPad_B_TRIGGER(PAD_1) || cAPICPad_START_TRIGGER(PAD_1))
    #else
    if (mTimer == 0 || mDoCPd_c::getTrig(PAD_1) &
                           (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_START |
                            PAD_TRIGGER_Z | PAD_TRIGGER_L | PAD_TRIGGER_R | PAD_BUTTON_LEFT |
                            PAD_BUTTON_RIGHT | PAD_BUTTON_DOWN | PAD_BUTTON_UP))
    #endif
    {
        mExecCommand = EXEC_WARNING_OUT;
        mTimer = 30;
        mDoGph_gInf_c::startFadeOut(30);
        mDoRst::setWarningDispFlag(1);
    }
}

void dScnLogo_c::warningOutDraw() {
    dComIfGd_set2DOpa(mWarning);

    if (mTimer == 0) {
        mTimer = 90;
        mExecCommand = EXEC_NINTENDO_IN;
        mDoGph_gInf_c::startFadeIn(30);
    }
}

void dScnLogo_c::nintendoInDraw() {
    dComIfGd_set2DOpa(mNintendoLogo);

    if (mTimer == 0) {
        mExecCommand = EXEC_NINTENDO_OUT;
        mTimer = 30;
        mDoGph_gInf_c::startFadeOut(30);
    }
}

void dScnLogo_c::nintendoOutDraw() {
    dComIfGd_set2DOpa(mNintendoLogo);

    if (mTimer == 0) {
        mExecCommand = EXEC_DOLBY_IN;
        mTimer = 90;
        mDoGph_gInf_c::startFadeIn(30);
    }
}

void dScnLogo_c::dolbyInDraw() {
    dComIfGd_set2DOpa(mDolbyLogo);

    if (mTimer == 0) {
        mExecCommand = EXEC_DOLBY_OUT;
        mTimer = 30;
        mDoGph_gInf_c::startFadeOut(30);
    }
}

void dScnLogo_c::dolbyOutDraw() {
    dComIfGd_set2DOpa(mDolbyLogo);

    if (mTimer == 0) {
        mExecCommand = EXEC_DOLBY_OUT2;
        mTimer = 30;
        mDoGph_gInf_c::startFadeIn(30);
    }
}

void dScnLogo_c::dolbyOutDraw2() {
    if (mTimer == 0) {
        mExecCommand = EXEC_DVD_WAIT;
    }
}

#if PLATFORM_WII || PLATFORM_SHIELD
void dScnLogo_c::strapInDraw() {
    dComIfGd_set2DOpa(mStrapImg);
    if (mTimer == 0) {
        mExecCommand = EXEC_STRAP_DISP;

        #if VERSION == VERSION_SHIELD_DEBUG
        mTimer = 0;
        #elif VERSION == VERSION_SHIELD
        mTimer = 60;
        #else
        mTimer = 840;
        #endif
    }
}

void dScnLogo_c::strapDispDraw() {
    dComIfGd_set2DOpa(mStrapImg);

    #if VERSION == VERSION_SHIELD_DEBUG
    if (mTimer == 0)
    #elif VERSION == VERSION_SHIELD
    if (mTimer == 0 || cAPICPad_ANY_BUTTON(PAD_1))
    #else
    if (mTimer == 0 || mReCPd::getTrig(PAD_1) & ~WPAD_BUTTON_HOME)
    #endif
    {
        mExecCommand = EXEC_DVD_WAIT;
    }
}

void dScnLogo_c::strapOutDraw() {
    dComIfGd_set2DOpa(mStrapImg);
    if (mTimer == 0) {
        #if VERSION == VERSION_SHIELD
        mExecCommand = EXEC_NVLOGO_IN;
        #else
        mExecCommand = EXEC_STRAP_OUT2;
        #endif

        mTimer = 30;
        mDoGph_gInf_c::startFadeIn(30);
    }
}

void dScnLogo_c::strapOut2Draw() {
    if (mTimer == 0) {
        mExecCommand = EXEC_SCENE_CHANGE;
    }
}
#endif

#if VERSION == VERSION_SHIELD
void dScnLogo_c::mocInDraw() {
    dComIfGd_set2DOpa(mMocImg);

    if (mTimer == 0) {
        mExecCommand = EXEC_MOC_DISP;
        mTimer = 120;
    }
}

void dScnLogo_c::mocDispDraw() {
    dComIfGd_set2DOpa(mMocImg);

    if (mTimer == 0 || cAPICPad_ANY_BUTTON(PAD_1)) {
        mExecCommand = EXEC_MOC_OUT;
        mTimer = 30;
        mDoGph_gInf_c::startFadeOut(30);
    }
}

void dScnLogo_c::mocOutDraw() {
    dComIfGd_set2DOpa(mMocImg);

    if (mTimer == 0) {
        mExecCommand = EXEC_STRAP_IN;
        mTimer = 90;
        mDoGph_gInf_c::startFadeIn(30);
    }
}

void dScnLogo_c::nvLogoInDraw() {
    dComIfGd_set2DOpa(mNvLogo);

    if (mTimer == 0) {
        mExecCommand = EXEC_NVLOGO_DISP;
        mTimer = 120;
    }
}

void dScnLogo_c::nvLogoDispDraw() {
    dComIfGd_set2DOpa(mNvLogo);

    if (mTimer == 0 || cAPICPad_ANY_BUTTON(PAD_1)) {
        mExecCommand = EXEC_NVLOGO_OUT;
        mTimer = 30;
        mDoGph_gInf_c::startFadeOut(30);
    }
}

void dScnLogo_c::nvLogoOutDraw() {
    dComIfGd_set2DOpa(mNvLogo);

    if (mTimer == 0) {
        mExecCommand = EXEC_STRAP_OUT2;
        mTimer = 30;
        mDoGph_gInf_c::startFadeIn(30);
    }
}
#endif

void dScnLogo_c::dvdWaitDraw() {
    #if PLATFORM_WII || PLATFORM_SHIELD
    dComIfGd_set2DOpa(mStrapImg);
    #endif

    if (!dComIfG_syncAllObjectRes()) {
        if (
            #if PLATFORM_WII || VERSION == VERSION_SHIELD
            mpHomeBtnCommand->sync() &&
            #endif
            mpField0Command->sync()
            && mpAlAnmCommand->sync()
            && mpFmapResCommand->sync()
            && mpDmapResCommand->sync()
            && mpCollectResCommand->sync()
            && mpItemIconCommand->sync()
            && mpRingResCommand->sync()
            && mpPlayerNameCommand->sync()
            && mpItemInfResCommand->sync()
            && mpButtonCommand->sync()
            && mpCardIconCommand->sync()
            && mpBmgResCommand->sync()
            && mpMsgComCommand->sync()
            && mpMsgResCommand[0]->sync()
            && mpMsgResCommand[1]->sync()
            && mpMsgResCommand[2]->sync()
            && mpMsgResCommand[3]->sync()
            && mpMsgResCommand[4]->sync()
            && mpMsgResCommand[5]->sync()
            && mpMsgResCommand[6]->sync()
            && mpFontResCommand->sync()
            && mpMain2DCommand->sync()
            && mpRubyResCommand->sync()
            && mParticleCommand->sync()
            && mItemTableCommand->sync()
            && mEnemyItemCommand->sync()
            && preLoad_dyl())
        {
            mDoRst::setLogoScnFlag(0);

            #if PLATFORM_WII || PLATFORM_SHIELD
            mDoRst::setProgSeqFlag(0);
            #endif

            mDoRst::setProgChgFlag(0);

            #if PLATFORM_WII || PLATFORM_SHIELD
            mExecCommand = EXEC_STRAP_OUT;
            mTimer = 30;
            mDoGph_gInf_c::startFadeOut(30);
            #else
            mExecCommand = EXEC_SCENE_CHANGE;
            #endif
        }
    }
}

void dScnLogo_c::nextSceneChange() {
    if (!mDoRst::isReset()) {
        if (!isOpeningCut())
        {
            dComIfG_changeOpeningScene(this, fpcNm_OPENING_SCENE_e);
        } else {
            #if DEBUG
            fopScnM_ChangeReq(this, fpcNm_MENU_SCENE_e, 0, 30);
            dComIfGs_init();
            dComIfG_playerStatusD();
            return;
            #endif
        }
    }
}

dScnLogo_c::~dScnLogo_c() {
    if (mDoRst::isReset()) {
        #if !(PLATFORM_WII || PLATFORM_SHIELD)
        if (mDoAud_zelAudio_c::isInitFlag())
        #endif
        {
            Z2AudioMgr::getInterface()->resetProcess(5, true);
        }

        mDoRst_reset(0, 0x80000000, 0);
    }

    #if PLATFORM_WII || PLATFORM_SHIELD
    JKR_DELETE(mStrapImg);
    #endif

    #if !(PLATFORM_WII || PLATFORM_SHIELD)
    JKR_DELETE(mNintendoLogo);
    JKR_DELETE(mWarning);
    JKR_DELETE(mWarningStart);
    JKR_DELETE(mDolbyLogo);
    JKR_DELETE(mProgressiveChoice);
    JKR_DELETE(mProgressiveYes);
    JKR_DELETE(mProgressiveNo);
    JKR_DELETE(mProgressiveSel);

    #if TARGET_PC
    if (getGameVersion() == GameVersion::GcnPal) {
        mpPalLogoResCommand->getArchive()->removeResourceAll();
        mpPalLogoResCommand->getArchive()->unmount();
        mpPalLogoResCommand->destroy();
    }
    #elif VERSION == VERSION_GCN_PAL
    mpPalLogoResCommand->getArchive()->removeResourceAll();
    mpPalLogoResCommand->getArchive()->unmount();
    mpPalLogoResCommand->destroy();
    #endif
    #endif

    #if VERSION == VERSION_SHIELD
    JKR_DELETE(mNvLogo);
    JKR_DELETE(mMocImg);
    #endif

    preLoad_dyl_remove();

    #if PLATFORM_WII || PLATFORM_SHIELD
    switch (getPalLanguage()) {
    case 2:
        dComIfG_deleteObjectResMain("LogoFrWii");
        break;
    case 3:
        dComIfG_deleteObjectResMain("LogoSpWii");
        break;
    case 0:
    default:
        dComIfG_deleteObjectResMain("LogoUsWii");
        break;
    }
    #else
    dComIfG_deleteObjectResMain(LOGO_ARC);
    #endif

    mLogo01Heap->destroy();
    mLogoHeap->destroy();
    JKRFree(dummyGameAlloc);

    #if PLATFORM_WII || VERSION == VERSION_SHIELD
    dHomeButton_c::create(mHomeBtnRegion, mpHomeBtnCommand->getMemAddress());
    mpHomeBtnCommand->destroy();
    #endif

    dComIfGp_particle_createCommon(mParticleCommand->getMemAddress());
    dComIfGp_setFieldMapArchive2(mpField0Command->getArchive());
    dComIfGp_setAnmArchive(mpAlAnmCommand->getArchive());
    dComIfGp_setFmapResArchive(mpFmapResCommand->getArchive());
    dComIfGp_setDmapResArchive(mpDmapResCommand->getArchive());
    dComIfGp_setCollectResArchive(mpCollectResCommand->getArchive());
    dComIfGp_setItemIconArchive(mpItemIconCommand->getArchive());
    dComIfGp_setAllMapArchive(NULL);
    dComIfGp_setRingResArchive(mpRingResCommand->getArchive());
    dComIfGp_setNameResArchive(mpPlayerNameCommand->getArchive());
    dComIfGp_setDemoMsgArchive(mpItemInfResCommand->getArchive());

    dComIfGp_setMeterButtonArchive(mpButtonCommand->getArchive());
    #if DEBUG
    JKRArchive* button = mpButtonCommand->getArchive();
    if (button != NULL) {
        OS_REPORT("button not nullptr\n");
    } else {
        OS_REPORT("button nullptr\n");
    }
    #endif

    dComIfGp_setErrorResArchive(NULL);
    dComIfGp_setCardIconResArchive(mpCardIconCommand->getArchive());
    dComIfGp_setMsgDtArchive(0, mpBmgResCommand->getArchive());
    dComIfGp_setMsgCommonArchive(mpMsgComCommand->getArchive());
    for (int i = 0; i < 7; i++) {
        dComIfGp_setMsgArchive(i, mpMsgResCommand[i]->getArchive());
    }
    dComIfGp_setFontArchive(mpFontResCommand->getArchive());
    dComIfGp_setRubyArchive(mpRubyResCommand->getArchive());
    dComIfGp_setMain2DArchive(mpMain2DCommand->getArchive());

    mpField0Command->destroy();
    mpAlAnmCommand->destroy();
    mpFmapResCommand->destroy();
    mpDmapResCommand->destroy();
    mpCollectResCommand->destroy();
    mpItemIconCommand->destroy();
    mpRingResCommand->destroy();
    mpPlayerNameCommand->destroy();
    mpItemInfResCommand->destroy();
    mpButtonCommand->destroy();
    mpCardIconCommand->destroy();
    mpBmgResCommand->destroy();
    mpMsgComCommand->destroy();
    for (int i = 0; i < 7; i++) {
        mpMsgResCommand[i]->destroy();
    }
    mpFontResCommand->destroy();
    mpMain2DCommand->destroy();
    mpRubyResCommand->destroy();

    #if !PLATFORM_SHIELD
    mParticleCommand->destroy();
    #else
    JKR_DELETE(mParticleCommand);
    #endif

    JKRAramHeap* aram_heap = JKRAram::getAramHeap();
    u32 free_size = aram_heap->getTotalFreeSize();
    mDoExt_getMesgFont();
    mDoExt_getSubFont();
    mDoExt_getRubyFont();
    mDoExt_setAraCacheSize(free_size - aram_heap->getTotalFreeSize());

#if VERSION == VERSION_GCN_JPN
    if (dComIfGp_getFontArchive() != NULL) {
        dComIfGp_getFontArchive()->unmount();
        dComIfGp_setFontArchive(NULL);
    }
#endif

    dComIfGp_setItemTable(mItemTableCommand->getMemAddress());
    #if !PLATFORM_SHIELD
    mItemTableCommand->destroy();
    #else
    JKR_DELETE(mItemTableCommand);
    #endif

    dEnemyItem_c::setItemData((u8*)mEnemyItemCommand->getMemAddress());
    #if !PLATFORM_SHIELD
    mEnemyItemCommand->destroy();
    #else
    JKR_DELETE(mEnemyItemCommand);
    #endif

    dDlst_shadowControl_c::setSimpleTex((ResTIMG*)dComIfG_getObjectRes("Always", 0x4A));

    #if PLATFORM_WII || PLATFORM_SHIELD
    dComIfG_inf_c::createBaseCsr();
    #endif

    dTres_c::createWork();
    dMpath_c::createWork();

    OS_REPORT("\x1b[31m%d gameHeap->getFreeSize %08x(%d)\n\x1b[m", 1479, mDoExt_getGameHeap()->getFreeSize(), mDoExt_getGameHeap()->getFreeSize());

    #if PLATFORM_WII
    data_8053a730 = 0;
    #endif
}

static int phase_0(dScnLogo_c* i_this) {
    mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
    dComIfGp_particle_create();

    OS_REPORT("\x1b[31m%d gameHeap->getFreeSize %08x(%d)\n\x1b[m", 1497, mDoExt_getGameHeap()->getFreeSize(), mDoExt_getGameHeap()->getFreeSize());
    u32 var_r29 = 0x340000;
    u32 var_r28 = 0x130000;

    i_this->dummyGameAlloc = mDoExt_getGameHeap()->alloc(var_r29, -0x10);
    JUT_ASSERT(1523, i_this->dummyGameAlloc != NULL);

    i_this->mLogoHeap = JKRExpHeap::create(i_this->dummyGameAlloc, var_r29, NULL, false);
    JUT_ASSERT(1525, i_this->mLogoHeap != NULL);
    JKRHEAP_NAME(i_this->mLogoHeap, "Logo");

    i_this->mLogo01Heap = JKRCreateExpHeap(var_r28, i_this->mLogoHeap, false);
    JUT_ASSERT(1528, i_this->mLogo01Heap != NULL);
    JKRHEAP_NAME(i_this->mLogo01Heap, "Logo01");

    #if TARGET_PC || VERSION == VERSION_GCN_PAL
    IF_DUSK_BLOCK(getGameVersion() == GameVersion::GcnPal)
    switch (i_this->getPalLanguage()) {
    case 1:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalGm.arc", 0, NULL);
        break;
    case 2:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalFr.arc", 0, NULL);
        break;
    case 3:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalSp.arc", 0, NULL);
        break;
    case 4:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalIt.arc", 0, NULL);
        break;
    case 0:
    default:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalUk.arc", 0, NULL);
        break;
    }
    IF_DUSK_BLOCK_END
    #endif

    #if PLATFORM_WII || PLATFORM_SHIELD
    OSSetPowerCallback(mDoRst_shutdownCallBack);
    #endif

    return cPhs_NEXT_e;
}

static int phase_1(dScnLogo_c* i_this) {
    if (!cDyl_InitAsyncIsDone()) {
        return cPhs_INIT_e;
    }

    #if !(PLATFORM_WII || PLATFORM_SHIELD)
    if (!mDoAud_zelAudio_c::isInitFlag() || Z2AudioMgr::getInterface()->checkFirstWaves()) {
        return cPhs_INIT_e;
    }
    #endif

    #if TARGET_PC || VERSION == VERSION_GCN_PAL
    IF_DUSK_BLOCK(getGameVersion() == GameVersion::GcnPal)
    if (!mDoDvdThd::SyncWidthSound) {
        return cPhs_INIT_e;
    }

    if (!i_this->mpPalLogoResCommand->sync()) {
        return cPhs_INIT_e;
    }
    IF_DUSK_BLOCK_END
    #endif

    int rt;
    #if PLATFORM_WII || PLATFORM_SHIELD
    switch (i_this->getPalLanguage()) {
    case 0:
    default:
        rt = dComIfG_setObjectRes("LogoUsWii", (u8)0, i_this->mLogoHeap);
        break;
    case 2:
        rt = dComIfG_setObjectRes("LogoFrWii", (u8)0, i_this->mLogoHeap);
        break;
    case 3:
        rt = dComIfG_setObjectRes("LogoSpWii", (u8)0, i_this->mLogoHeap);
        break;
    }
    #else
    rt = dComIfG_setObjectRes(LOGO_ARC, (u8)0, i_this->mLogoHeap);
    #endif

    JUT_ASSERT(1652, rt == 1);

    mDoRst::setLogoScnFlag(1);

    OS_REPORT("\x1b[32m%d archiveHeap->getTotalFreeSize %08x\n\x1b[m", 1667, archiveHeap->getTotalFreeSize());
    archiveHeap->dump_sort();
    return cPhs_NEXT_e;
}

static int phase_2(dScnLogo_c* i_this) {
    if (dComIfG_syncAllObjectRes()) {
        return cPhs_INIT_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

static int resLoad(request_of_phase_process_class* i_phase, dScnLogo_c* i_this) {
    static int (*l_method[3])(void*) = {(int (*)(void*))phase_0, (int (*)(void*))phase_1,
                                        (int (*)(void*))phase_2};

    return dComLbG_PhaseHandler(i_phase, l_method, i_this);
}

int dScnLogo_c::create() {
    static bool sDiagLogged = false;
    if (!sDiagLogged) {
        DuskLog.debug("dScnLogo_c::create START");
    }
    int phase_state = resLoad(&field_0x1c4, this);
    if (!sDiagLogged) {
        DuskLog.debug("dScnLogo_c::create resLoad={} (need {} for complete)", phase_state, fmt::underlying(cPhs_COMPLEATE_e));
        sDiagLogged = true;
    }
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    DuskLog.debug("dScnLogo_c::create resLoad COMPLETE, continuing init...");

    #if PLATFORM_WII
    data_8053a730 = 1;
    #endif

    mpHeap = mDoExt_setCurrentHeap(mLogo01Heap);

    #if PLATFORM_WII || PLATFORM_SHIELD
    logoInitWii();
    #else
    logoInitGC();
    #endif

    JKRSetCurrentHeap(mpHeap);

    OS_REPORT("\x1b[31m%d gameHeap->getFreeSize %08x(%d)\n\x1b[m", 1732, mDoExt_getGameHeap()->getFreeSize(), mDoExt_getGameHeap()->getFreeSize());

    dvdDataLoad();

    OS_REPORT("\x1b[31m%d gameHeap->getFreeSize %08x(%d)\n\x1b[m", 1738, mDoExt_getGameHeap()->getFreeSize(), mDoExt_getGameHeap()->getFreeSize());

    #if !(PLATFORM_WII || PLATFORM_SHIELD)
    Z2AudioMgr::getInterface()->loadStaticWaves();
    #endif

    mDoGph_gInf_c::setTickRate(OS_TIMER_CLOCK / 60);
    mDoGph_gInf_c::waitBlanking(0);

    field_0x20a = 0;

    #if VERSION == VERSION_SHIELD_DEBUG
    mExecCommand = EXEC_STRAP_IN;
    mTimer = 0;
    #elif VERSION == VERSION_SHIELD
    mExecCommand = EXEC_MOC_IN;
    mTimer = 30;
    #elif PLATFORM_WII
    mExecCommand = EXEC_STRAP_IN;
    mTimer = 90;
    #endif

    mDoGph_gInf_c::startFadeIn(30);

    #if !(PLATFORM_WII || PLATFORM_SHIELD)
    checkProgSelect();
    if (field_0x20a != 0) {
        mExecCommand = EXEC_PROG_IN;
        #if TARGET_PC
        mTimer = dusk::getSettings().game.skipWarningScreen ? 1 : 30;
        #else
        mTimer = 30;
        #endif
        field_0x218 = getProgressiveMode();
    } else {
        #if TARGET_PC
        if (dusk::getSettings().game.skipWarningScreen) {
            mTimer = 0;  // Possibly unnecessary but just in case
            mExecCommand = EXEC_DVD_WAIT;
        } else {
            if (mDoRst::getWarningDispFlag()) {
                mTimer = 90;
                mExecCommand = EXEC_NINTENDO_IN;
            } else {
                mTimer = 120;
                mExecCommand = EXEC_WARNING_IN;
            }
        }
        #else
        if (mDoRst::getWarningDispFlag()) {
            mTimer = 90;
            mExecCommand = EXEC_NINTENDO_IN;
        } else {
            mTimer = 120;
            mExecCommand = EXEC_WARNING_IN;
        }
        #endif
        mDoRst::setProgSeqFlag(1);
    }

    JUTGamePad::clearResetOccurred();
    JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
    #endif

    mDoRst::offReset();
    mDoRst::offResetPrepare();

    #if PLATFORM_WII || VERSION == VERSION_SHIELD
    dHomeButton_c::lbl_8053A724 = 0;
    #endif

    return phase_state;
}

#if PLATFORM_WII || PLATFORM_SHIELD
void dScnLogo_c::logoInitWii() {
    u8 language = getPalLanguage();
    if (language > 5) {
        language = 0;
    }

    ResTIMG* timg;
    s16 width;
    s16 height;
    if (mDoGph_gInf_c::isWide()) {
        switch (language) {
        case 0:
        default:
            timg = (ResTIMG*)dComIfG_getObjectRes("LogoUsWii", dRes_ID_LOGOUSWII_BTI_STRAP_16_9_832X456_US_e);
            break;
        case 2:
            timg = (ResTIMG*)dComIfG_getObjectRes("LogoFrWii", 3);
            break;
        case 3:
            timg = (ResTIMG*)dComIfG_getObjectRes("LogoSpWii", 3);
            break;
        }

        width = 832;
        height = 456;
    } else {
        switch (language) {
        case 0:
        default:
            timg = (ResTIMG*)dComIfG_getObjectRes("LogoUsWii", dRes_ID_LOGOUSWII_BTI_STRAP_608X456_US_e);
            break;
        case 2:
            timg = (ResTIMG*)dComIfG_getObjectRes("LogoFrWii", 4);
            break;
        case 3:
            timg = (ResTIMG*)dComIfG_getObjectRes("LogoSpWii", 4);
            break;
        }

        // this uses the standard width but the widescreen height?
        width = FB_WIDTH_BASE;
        height = FB_HEIGHT;
    }

    JUT_ASSERT(2309, timg != NULL);
    mStrapImg = JKR_NEW dDlst_2D_c(timg, (FB_WIDTH_BASE / 2) - (width / 2), (FB_HEIGHT_BASE / 2) - (height / 2), width, height, 255);

    #if VERSION == VERSION_SHIELD
    timg = (ResTIMG*)dComIfG_getObjectRes("LogoUsWii", 5);
    mNvLogo = JKR_NEW dDlst_2D_c(timg, (FB_WIDTH_BASE / 2) - (width / 2), (FB_HEIGHT_BASE / 2) - (height / 2), width, height, 255);

    timg = (ResTIMG*)dComIfG_getObjectRes("LogoUsWii", 4);
    mMocImg = JKR_NEW dDlst_2D_c(timg, (FB_WIDTH_BASE / 2) - (width / 2), (FB_HEIGHT_BASE / 2) - (height / 2), width, height, 255);
    #endif

    OS_REPORT("\x1b[32m%d archiveHeap->getTotalFreeSize %08x\n\x1b[m", 2316, archiveHeap->getTotalFreeSize());

    JUT_ASSERT(2319, mStrapImg != NULL);

    mNintendoLogo = NULL;
    mDolbyLogo = NULL;
    mWarning = NULL;
    mWarningStart = NULL;
    mProgressiveChoice = NULL;
    mProgressiveYes = NULL;
    mProgressiveNo = NULL;
    mProgressivePro = NULL;
    mProgressiveInter = NULL;
    mProgressiveSel = NULL;

    OS_REPORT("\x1b[32m%d archiveHeap->getTotalFreeSize %08x\n\x1b[m", 2341, archiveHeap->getTotalFreeSize());
    OS_REPORT("%d mLogo01Heap->getTotalFreeSize %08x\n\x1b[m", 2344, mLogo01Heap->getTotalFreeSize());
}
#else
void dScnLogo_c::logoInitGC() {
    ResTIMG* nintendoImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 4);
    mNintendoLogo = JKR_NEW dDlst_2D_c(nintendoImg, 117, 154, 376, 104, 255);
#if VERSION == VERSION_GCN_JPN
    mNintendoLogo->getPicture()->setWhite(JUtility::TColor(0, 70, 255, 255));
#else
    mNintendoLogo->getPicture()->setWhite(JUtility::TColor(220, 0, 0, 255));
#endif

    ResTIMG* dolbyImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 3);
    mDolbyLogo = JKR_NEW dDlst_2D_c(dolbyImg, 189, 150, 232, 112, 255);

#if TARGET_PC
    if (getGameVersion() == GameVersion::GcnPal) {
        u8 language = getPalLanguage();
        if (language >= 5) {
            language = 0;
        }

        static const char* choice[] = {
            "50_60_choice_eng.bti",
            "50_60_choice_ger.bti",
            "50_60_choice_fra.bti",
            "50_60_choice_spa.bti",
            "50_60_choice_ita.bti",
        };

        static const char* yes[] = {
            "60_set_eng.bti",
            "60_set_ger.bti",
            "60_set_fra.bti",
            "60_set_spa.bti",
            "60_set_ita.bti",
        };

        static const char* no[] = {
            "50_set_eng.bti",
            "50_set_ger.bti",
            "50_set_fra.bti",
            "50_set_spa.bti",
            "50_set_ita.bti",
        };

        static const char* prog[] = {
            "progressive_pro.bti",
            "progressive_pro_gm.bti",
            "progressive_pro_fr.bti",
            "progressive_pro_sp.bti",
            "progressive_pro_it.bti",
        };

        static const char* intr[] = {
            "progressive_inter.bti",
            "progressive_inter_gm.bti",
            "progressive_inter_fr.bti",
            "progressive_inter_sp.bti",
            "progressive_inter_it.bti",
        };

        static const char* warning[] = {
            "warning.bti",
            "warning_gm.bti",
            "warning_fr.bti",
            "warning_sp.bti",
            "warning_it.bti",
        };

        static const char* warningPs[] = {
            "warning_pstart.bti",
            "warning_pstart_gm.bti",
            "warning_pstart_fr.bti",
            "warning_pstart_sp.bti",
            "warning_pstart_it.bti",
        };

        ResTIMG* warningImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', warning[language]);
        mWarning = JKR_NEW dDlst_2D_c(warningImg, 0, 0, FB_WIDTH, FB_HEIGHT, 255);

        ResTIMG* warnStartImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', warningPs[language]);
        mWarningStart = JKR_NEW dDlst_2D_c(warnStartImg, 0, 359, FB_WIDTH, 48, 255);

        ResTIMG* progChoiceImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', choice[language]);
        mProgressiveChoice = JKR_NEW dDlst_2D_c(progChoiceImg, 113, 143, 416, 210, 255);

        ResTIMG* progYesImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', yes[language]);
        mProgressiveYes = JKR_NEW dDlst_2D_c(progYesImg, 121, 352, 200, 72, 255);
        mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

        ResTIMG* progNoImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', no[language]);
        mProgressiveNo = JKR_NEW dDlst_2D_c(progNoImg, 320, 352, 200, 72, 255);
        mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

        mProgressivePro = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', prog[language]);
        mProgressiveInter = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', intr[language]);
        mProgressiveSel = JKR_NEW dDlst_2D_c(mProgressivePro, 153, 309, 336, 88, 255);
    } else {
        ResTIMG* warningImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 10);
        mWarning = JKR_NEW dDlst_2D_c(warningImg, 0, 0, FB_WIDTH, FB_HEIGHT, 255);

        ResTIMG* warnStartImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 11);
        mWarningStart = JKR_NEW dDlst_2D_c(warnStartImg, 0, 359, FB_WIDTH, 48, 255);

        ResTIMG* progChoiceImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 5);
        mProgressiveChoice = JKR_NEW dDlst_2D_c(progChoiceImg, 113, 281, 416, 72, 255);

        ResTIMG* progYesImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 9);
        mProgressiveYes = JKR_NEW dDlst_2D_c(progYesImg, 211, 372, 80, 32, 255);
        mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

        ResTIMG* progNoImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 7);
        mProgressiveNo = JKR_NEW dDlst_2D_c(progNoImg, 350, 372, 80, 32, 255);
        mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

        mProgressivePro = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 8);
        mProgressiveInter = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 6);
        mProgressiveSel = JKR_NEW dDlst_2D_c(mProgressivePro, 153, 309, 336, 88, 255);
    }

#elif VERSION == VERSION_GCN_PAL
    u8 language = getPalLanguage();
    if (language >= 5) {
        language = 0;
    }

    static const char* choice[] = {
        "50_60_choice_eng.bti",
        "50_60_choice_ger.bti",
        "50_60_choice_fra.bti",
        "50_60_choice_spa.bti",
        "50_60_choice_ita.bti",
    };

    static const char* yes[] = {
        "60_set_eng.bti",
        "60_set_ger.bti",
        "60_set_fra.bti",
        "60_set_spa.bti",
        "60_set_ita.bti",
    };

    static const char* no[] = {
        "50_set_eng.bti",
        "50_set_ger.bti",
        "50_set_fra.bti",
        "50_set_spa.bti",
        "50_set_ita.bti",
    };

    static const char* prog[] = {
        "progressive_pro.bti",
        "progressive_pro_gm.bti",
        "progressive_pro_fr.bti",
        "progressive_pro_sp.bti",
        "progressive_pro_it.bti",
    };

    static const char* intr[] = {
        "progressive_inter.bti",
        "progressive_inter_gm.bti",
        "progressive_inter_fr.bti",
        "progressive_inter_sp.bti",
        "progressive_inter_it.bti",
    };

    static const char* warning[] = {
        "warning.bti",
        "warning_gm.bti",
        "warning_fr.bti",
        "warning_sp.bti",
        "warning_it.bti",
    };

    static const char* warningPs[] = {
        "warning_pstart.bti",
        "warning_pstart_gm.bti",
        "warning_pstart_fr.bti",
        "warning_pstart_sp.bti",
        "warning_pstart_it.bti",
    };

    ResTIMG* warningImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', warning[language]);
    mWarning = JKR_NEW dDlst_2D_c(warningImg, 0, 0, FB_WIDTH, FB_HEIGHT, 255);

    ResTIMG* warnStartImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', warningPs[language]);
    mWarningStart = JKR_NEW dDlst_2D_c(warnStartImg, 0, 359, FB_WIDTH, 48, 255);

    ResTIMG* progChoiceImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', choice[language]);
    mProgressiveChoice = JKR_NEW dDlst_2D_c(progChoiceImg, 113, 143, 416, 210, 255);

    ResTIMG* progYesImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', yes[language]);
    mProgressiveYes = JKR_NEW dDlst_2D_c(progYesImg, 121, 352, 200, 72, 255);
    mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    ResTIMG* progNoImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', no[language]);
    mProgressiveNo = JKR_NEW dDlst_2D_c(progNoImg, 320, 352, 200, 72, 255);
    mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    mProgressivePro = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', prog[language]);
    mProgressiveInter = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', intr[language]);
    mProgressiveSel = JKR_NEW dDlst_2D_c(mProgressivePro, 153, 309, 336, 88, 255);
#else
    ResTIMG* warningImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 10);
    mWarning = JKR_NEW dDlst_2D_c(warningImg, 0, 0, FB_WIDTH, FB_HEIGHT, 255);

    ResTIMG* warnStartImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 11);
    mWarningStart = JKR_NEW dDlst_2D_c(warnStartImg, 0, 359, FB_WIDTH, 48, 255);

    ResTIMG* progChoiceImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 5);
    mProgressiveChoice = JKR_NEW dDlst_2D_c(progChoiceImg, 113, 281, 416, 72, 255);

    ResTIMG* progYesImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 9);
    mProgressiveYes = JKR_NEW dDlst_2D_c(progYesImg, 211, 372, 80, 32, 255);
    mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    ResTIMG* progNoImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 7);
    mProgressiveNo = JKR_NEW dDlst_2D_c(progNoImg, 350, 372, 80, 32, 255);
    mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    mProgressivePro = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 8);
    mProgressiveInter = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 6);
    mProgressiveSel = JKR_NEW dDlst_2D_c(mProgressivePro, 153, 309, 336, 88, 255);
#endif
}
#endif

void dScnLogo_c::dvdDataLoad() {
    int rt;

    #if PLATFORM_WII || VERSION == VERSION_SHIELD
    rt = dComIfG_setObjectRes("HomeBtn", (u8)0, NULL);
    JUT_ASSERT(__LINE__, rt == 1);

    static const homeBtnData l_homeBtnData[] = {
        {SC_LANG_ENGLISH, "/res/HomeBtn/homeBtn_ENG.arc"},
        {SC_LANG_ENGLISH, "/res/HomeBtn/homeBtn_ENG.arc"},
        {SC_LANG_FRENCH, "/res/HomeBtn/homeBtn_FRA.arc"},
        {SC_LANG_SPANISH, "/res/HomeBtn/homeBtn_SPA.arc"},
        {SC_LANG_ENGLISH, "/res/HomeBtn/homeBtn_ENG.arc"},
        {SC_LANG_ENGLISH, "/res/HomeBtn/homeBtn_ENG.arc"},
        {SC_LANG_ENGLISH, "/res/HomeBtn/homeBtn_ENG.arc"},
    };

    u8 language = getPalLanguage();
    const homeBtnData* data = &l_homeBtnData[language];
    mpHomeBtnCommand = mDoDvdThd_toMainRam_c::create(data->path, 0, NULL);
    mHomeBtnRegion = data->region;
    #endif

    rt = dComIfG_setObjectRes("Always", (u8)0, NULL);
    JUT_ASSERT(2420, rt == 1);

    OS_REPORT("\x1b[32m%d archiveHeap->getTotalFreeSize %08x\n\x1b[m", 2421, archiveHeap->getTotalFreeSize());
    archiveHeap->dump_sort();

    rt = dComIfG_setObjectRes("Alink", (u8)0, NULL);
    JUT_ASSERT(2429, rt == 1);

    #if PLATFORM_WII || PLATFORM_SHIELD
    rt = dComIfG_setObjectRes("NNGC", (u8)0, NULL);
    JUT_ASSERT(2433, rt == 1);
    #endif

    OS_REPORT("\x1b[32m%d archiveHeap->getTotalFreeSize %08x\n\x1b[m", 2436, archiveHeap->getTotalFreeSize());

    mpField0Command = aramMount("/res/FieldMap/Field0.arc", mDoExt_getJ2dHeap());
    mpAlAnmCommand = aramMount("/res/Object/AlAnm.arc", NULL);
    mpFmapResCommand = aramMount(FMAP_RES_PATH, mDoExt_getJ2dHeap());
    mpDmapResCommand = aramMount(DMAP_RES_PATH, mDoExt_getJ2dHeap());
    mpCollectResCommand = aramMount(COLLECT_RES_PATH, mDoExt_getJ2dHeap());
    mpItemIconCommand = aramMount("/res/Layout/itemicon.arc", mDoExt_getJ2dHeap());
    mpRingResCommand = aramMount(RING_RES_PATH, mDoExt_getJ2dHeap());
    mpPlayerNameCommand = aramMount("/res/Layout/playerName.arc", mDoExt_getJ2dHeap());
    mpItemInfResCommand = aramMount(ITEM_INF_RES_PATH, mDoExt_getJ2dHeap());
    mpButtonCommand = aramMount(BUTTON_RES_PATH, mDoExt_getJ2dHeap());
    mpCardIconCommand = aramMount(ICON_RES_PATH, mDoExt_getJ2dHeap());

    #if TARGET_PC
    if (getGameVersion() == GameVersion::GcnPal) {
        switch (getPalLanguage()) {
        case 1:
            mpBmgResCommand = onMemMount("/res/Msgde/bmgres.arc");
            break;
        case 2:
            mpBmgResCommand = onMemMount("/res/Msgfr/bmgres.arc");
            break;
        case 3:
            mpBmgResCommand = onMemMount("/res/Msgsp/bmgres.arc");
            break;
        case 4:
            mpBmgResCommand = onMemMount("/res/Msgit/bmgres.arc");
            break;
        case 0:
        default:
            mpBmgResCommand = onMemMount("/res/Msguk/bmgres.arc");
            break;
        }
    } else {
        mpBmgResCommand = onMemMount(MSG_PATH);
    }
    #elif VERSION == VERSION_GCN_PAL
    switch (getPalLanguage()) {
    case 1:
        mpBmgResCommand = onMemMount("/res/Msgde/bmgres.arc");
        break;
    case 2:
        mpBmgResCommand = onMemMount("/res/Msgfr/bmgres.arc");
        break;
    case 3:
        mpBmgResCommand = onMemMount("/res/Msgsp/bmgres.arc");
        break;
    case 4:
        mpBmgResCommand = onMemMount("/res/Msgit/bmgres.arc");
        break;
    case 0:
    default:
        mpBmgResCommand = onMemMount("/res/Msguk/bmgres.arc");
        break;
    }
    #elif VERSION == VERSION_SHIELD_DEBUG
    switch (getPalLanguage()) {
    case 2:
        mpBmgResCommand = onMemMount("/res/Msgfr/bmgres.arc");
        break;
    case 3:
        mpBmgResCommand = onMemMount("/res/Msgsp/bmgres.arc");
        break;
    default:
        mpBmgResCommand = onMemMount("/res/Msgus/bmgres.arc");
        break;
    }
    #else
    mpBmgResCommand = onMemMount(MSG_PATH);
    #endif

    mpMsgComCommand = aramMount(MSG_COM_PATH, mDoExt_getJ2dHeap());
    mpMsgResCommand[0] = aramMount(MSG_RES0_PATH, mDoExt_getJ2dHeap());
    mpMsgResCommand[1] = aramMount(MSG_RES1_PATH, mDoExt_getJ2dHeap());
    mpMsgResCommand[2] = aramMount(MSG_RES2_PATH, mDoExt_getJ2dHeap());
    mpMsgResCommand[3] = aramMount(MSG_RES3_PATH, mDoExt_getJ2dHeap());
#if TARGET_PC
    const auto res4Path = versionSelect<const char*>({{GameVersion::GcnJpn, "/res/Layout/msgres04.arc"}}, "/res/Layout/msgres04F.arc");
    mpMsgResCommand[4] = aramMount( res4Path, mDoExt_getJ2dHeap());
#elif VERSION == VERSION_GCN_JPN
    mpMsgResCommand[4] = aramMount("/res/Layout/msgres04.arc", mDoExt_getJ2dHeap());
#else
    mpMsgResCommand[4] = aramMount("/res/Layout/msgres04F.arc", mDoExt_getJ2dHeap());
#endif
    mpMsgResCommand[5] = aramMount("/res/Layout/msgres05.arc", mDoExt_getJ2dHeap());
    mpMsgResCommand[6] = aramMount("/res/Layout/msgres06.arc", mDoExt_getJ2dHeap());

    mpMain2DCommand = onMemMount(MAIN2D_PATH);

#if TARGET_PC
    const auto fontResPath = versionSelect<const char*>(
        {
            {GameVersion::GcnJpn, "/res/Fontjp/fontres.arc"},
            {GameVersion::GcnPal, "/res/Fonteu/fontres.arc"},
        }, "/res/Fontus/fontres.arc");
    const auto fontRubyPath = versionSelect<const char*>(
        {
            {GameVersion::GcnJpn, "/res/Fontjp/rubyres.arc"},
            {GameVersion::GcnPal, "/res/Fonteu/rubyres.arc"},
        }, "/res/Fontus/rubyres.arc");

    // Note: GCN_JPN mounts this archive as tail instead of head.
    // I'm guessing this is fine since we have more RAM.
    mpFontResCommand = onMemMount(fontResPath);
    mpRubyResCommand = onMemMount(fontRubyPath);
#elif VERSION == VERSION_GCN_JPN
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontjp/fontres.arc", 1, JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = onMemMount("/res/Fontjp/rubyres.arc");
#elif VERSION == VERSION_GCN_PAL
    mpFontResCommand = onMemMount("/res/Fonteu/fontres.arc");
    mpRubyResCommand = onMemMount("/res/Fonteu/rubyres.arc");
#elif VERSION == VERSION_SHIELD_DEBUG
    mpFontResCommand = onMemMount("/res/Fonteu/fontres.arc");
    mpRubyResCommand = onMemMount("/res/Fontus/rubyres.arc");
#elif VERSION == VERSION_SHIELD
    mpFontResCommand = onMemMount("/res/Fontcn/fontres.arc");
    mpRubyResCommand = onMemMount("/res/Fontcn/rubyres.arc");
#else
    mpFontResCommand = onMemMount("/res/Fontus/fontres.arc");
    mpRubyResCommand = onMemMount("/res/Fontus/rubyres.arc");
#endif

    mParticleCommand = mDoDvdThd_toMainRam_c::create(PARTICLE_COM_PATH, 0, dComIfGp_particle_getResHeap());

    mItemTableCommand = mDoDvdThd_toMainRam_c::create("/res/ItemTable/item_table.bin", 0, NULL);
    JUT_ASSERT(2620, mItemTableCommand != NULL);

    mEnemyItemCommand = mDoDvdThd_toMainRam_c::create("/res/ItemTable/enemy_table.bin", 0, NULL);
    JUT_ASSERT(2624, mEnemyItemCommand != NULL);

    preLoad_dyl_create();
    preLoad_dyl();
}

static int dScnLogo_Create(scene_class* i_this) {
    DuskLog.debug("[DIAG] dScnLogo_Create: entry i_this={}", (void*)i_this);
    return (JKR_NEW_ARGS (i_this) dScnLogo_c)->create();
}

static int dScnLogo_Execute(dScnLogo_c* i_this) {
    fpc_ProcID id = fpcM_GetID(i_this);

    if (mDoRst::isReset()) {
        fopScnM_ChangeReq(i_this, fpcNm_LOGO_SCENE_e, 0, 5);
    }

    return 1;
}

#if DEBUG
static u8 lbl_8074CA49;
#endif

static int dScnLogo_Draw(dScnLogo_c* i_this) {
    #if DEBUG
    int x = 36;
    int y = 40;

    y += 12;
    JUTReport(x, y, "COPYDATE %s", mDoMain::COPYDATE_STRING);

    y += 12;
    JUTReport(x, y, "Build by %s", "Authorized User");

    y += 12;
    JUTReport(x, y, "_DEBUG/Debug version");

    if (!lbl_8074CA49) {
        lbl_8074CA49 = 1;
        JUTReportConsole_f("COPYDATE %s\n", mDoMain::COPYDATE_STRING);
        JUTReportConsole_f("Build by %s\n", "Authorized User");
        JUTReportConsole_f("_DEBUG/Debug version\n");
        JUTReportConsole_f("SDKVersion %s\n", "11Dec2009Patch02");

        #if PLATFORM_WII || PLATFORM_SHIELD
        JUTReportConsole_f("ConsoleSimMem  %08x\n", OSGetConsoleSimulatedMem1Size());
        JUTReportConsole_f("PhysicalMemory %08x\n", OSGetPhysicalMem1Size());
        JUTReportConsole_f("ConsoleType    %08x\n", OSGetConsoleType());
        JUTReportConsole_f("Language(PAL) %1x\n", SCGetLanguage());
        JUTReportConsole_f("Progressive   %1x\n", SCGetProgressiveMode());
        JUTReportConsole_f("SoundMode     %1x\n", SCGetSoundMode());
        #else
        JUTReportConsole_f("ConsoleSimMem  %08x\n", OSGetConsoleSimulatedMemSize());
        JUTReportConsole_f("PhysicalMemory %08x\n", OSGetPhysicalMemSize());
        JUTReportConsole_f("ConsoleType    %08x\n", OSGetConsoleType());
        JUTReportConsole_f("Language(PAL) %1x\n", OSGetLanguage());
        JUTReportConsole_f("Progressive   %1x\n", OSGetProgressiveMode());
        JUTReportConsole_f("SoundMode     %1x\n", OSGetSoundMode());
        #endif
    }
    #endif

    i_this->draw();
    return 1;
}

static int dScnLogo_Delete(dScnLogo_c* i_this) {
    i_this->~dScnLogo_c();
    return 1;
}

static int dScnLogo_IsDelete(dScnLogo_c* i_this) {
    return 1;
}

#if TARGET_PC || VERSION == VERSION_GCN_PAL || PLATFORM_WII || PLATFORM_SHIELD
u8 dScnLogo_c::getPalLanguage() {
    u8 language;

    #if PLATFORM_WII || PLATFORM_SHIELD
    switch (SCGetLanguage()) {
    case SC_LANG_JAPANESE:
        language = 6;
        break;
    case SC_LANG_ENGLISH:
        language = 0;
        break;
    case SC_LANG_GERMAN:
        language = 1;
        break;
    case SC_LANG_FRENCH:
        language = 2;
        break;
    case SC_LANG_SPANISH:
        language = 3;
        break;
    case SC_LANG_ITALIAN:
        language = 4;
        break;
    case SC_LANG_DUTCH:
        language = 5;
        break;
    }
    #else
    switch (OSGetLanguage()) {
    case OS_LANGUAGE_ENGLISH:
        language = 0;
        break;
    case OS_LANGUAGE_GERMAN:
        language = 1;
        break;
    case OS_LANGUAGE_FRENCH:
        language = 2;
        break;
    case OS_LANGUAGE_SPANISH:
        language = 3;
        break;
    case OS_LANGUAGE_ITALIAN:
        language = 4;
        break;
    case OS_LANGUAGE_DUTCH:
        language = 5;
        break;
    }
    #endif

    return language;
}
#endif

void dScnLogo_c::setProgressiveMode(u8 mode) {
    #if VERSION == VERSION_SHIELD_DEBUG
    return;
    #endif

    #if PLATFORM_WII
    SCSetProgressiveMode(mode);
    #elif VERSION == VERSION_GCN_PAL
    OSSetEuRgb60Mode(mode);
    #else
    OSSetProgressiveMode(mode);
    #endif
}

u8 dScnLogo_c::getProgressiveMode() {
    #if PLATFORM_WII || PLATFORM_SHIELD
    return SCGetProgressiveMode();
    #endif

    #if VERSION == VERSION_GCN_PAL
    return OSGetEuRgb60Mode();
    #else
    return OSGetProgressiveMode();
    #endif
}

bool dScnLogo_c::isProgressiveMode() {
    #if VERSION == VERSION_GCN_PAL
    return OSGetEuRgb60Mode() == OS_EURGB60_ON;
    #else
    return OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON;
    #endif
}

void dScnLogo_c::setRenderMode() {
    mDoMch_render_c::setProgressiveMode();
}

static scene_method_class l_dScnLogo_Method = {
    (process_method_func)dScnLogo_Create,
    (process_method_func)dScnLogo_Delete,
    (process_method_func)dScnLogo_Execute,
    (process_method_func)dScnLogo_IsDelete,
    (process_method_func)dScnLogo_Draw,
};

scene_process_profile_definition g_profile_LOGO_SCENE = {
    /* Layer ID     */ fpcLy_ROOT_e,
    /* List ID      */ 1,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_LOGO_SCENE_e,
    /* Proc SubMtd  */ &g_fpcNd_Method.base,
    /* Size         */ sizeof(dScnLogo_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopScn_Method.base,
    /* Scene SubMtd */ &l_dScnLogo_Method,
                       0,
};
