/**
 * d_s_logo.cpp
 * Game Boot Logo's Display
 */

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

#if PLATFORM_WII
#include "d/d_cursor_mng.h"
#endif

#if VERSION == VERSION_GCN_JPN
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

static dLog_HIO_c g_LogHIO;

typedef void (dScnLogo_c::*execFunc)();
static execFunc l_execFunc[16] = {
    &dScnLogo_c::warningInDraw,   &dScnLogo_c::warningDispDraw, &dScnLogo_c::warningOutDraw,
    &dScnLogo_c::nintendoInDraw,  &dScnLogo_c::nintendoOutDraw, &dScnLogo_c::dolbyInDraw,
    &dScnLogo_c::dolbyOutDraw,    &dScnLogo_c::dolbyOutDraw2,   &dScnLogo_c::progInDraw,
    &dScnLogo_c::progSelDraw,     &dScnLogo_c::progOutDraw,     &dScnLogo_c::progSetDraw,
    &dScnLogo_c::progSet2Draw,    &dScnLogo_c::progChangeDraw,  &dScnLogo_c::dvdWaitDraw,
    &dScnLogo_c::nextSceneChange,
};

dLog_HIO_c::dLog_HIO_c() {}

void dScnLogo_c::preLoad_dyl_create() {
    m_preLoad_dylPhase = new request_of_phase_process_class[14];
    memset(m_preLoad_dylPhase, 0, sizeof(request_of_phase_process_class) * 14);
}

void dScnLogo_c::preLoad_dyl_remove() {
    delete[] m_preLoad_dylPhase;
}

static s16 const l_preLoad_dylKeyTbl[14] = {
    0x02DC, 0x02CE, 0x0221, 0x00F2, 0x021B, 0x02F4, 0x0139,
    0x015A, 0x02E4, 0x00FE, 0x0308, 0x030F, 0x00FF, 0x013F,
};

bool dScnLogo_c::preLoad_dyl() {
    bool ret = true;

    for (int i = 0; i < 14; i++) {
        int phase_state = cDylPhs::Link(&m_preLoad_dylPhase[i], l_preLoad_dylKeyTbl[i]);

        if (phase_state != cPhs_COMPLEATE_e) {
            ret = false;
        }
    }

    return ret;
}

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
    cLib_calcTimer<u16>(&mTimer);
    (this->*l_execFunc[mExecCommand])();
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
            if (mDoCPd_c::getHoldRight(PAD_1) || mDoCPd_c::getStickX(PAD_1) > 0.5f) {
                mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
                field_0x209 = 1;
                field_0x20e = 30;
                field_0x210 = field_0x20e;
                field_0x212 = 0;
            }
        } else {
            if (mDoCPd_c::getHoldLeft(PAD_1) || mDoCPd_c::getStickX(PAD_1) < -0.5f) {
                mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
                field_0x209 = 0;
                field_0x20e = 30;
                field_0x210 = field_0x20e;
                field_0x212 = 0;
            }
        }

        if (mDoCPd_c::getTrigA(PAD_1) || mTimer == 0) {
            if (field_0x209 == 0) {
                mProgressiveSel->getPicture()->changeTexture(mProgressivePro, 0);
                setProgressiveMode(PROGRESSIVE_MODE_ON);
                mDoRst::setProgChgFlag(1);
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
            } else {
                mProgressiveSel->getPicture()->changeTexture(mProgressiveInter, 0);
                setProgressiveMode(PROGRESSIVE_MODE_OFF);
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
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
            mDoRst::setProgSeqFlag(1);
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
        if (field_0x218 == 1 && field_0x209 == 0) {
        #else
        if (field_0x218 != 0 && field_0x209 == 0) {
        #endif
            mExecCommand = EXEC_PROG_CHANGE;
            mTimer = 150;
        } else if (field_0x218 == 0 && field_0x209 != 0) {
            if (mDoRst::getWarningDispFlag() != 0) {
                mTimer = 90;
                mExecCommand = EXEC_NINTENDO_IN;
            } else {
                mTimer = 120;
                mExecCommand = EXEC_WARNING_IN;
            }

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
    if (getProgressiveMode() != 0 && mTimer == 90 && field_0x209 == 0) {
        setRenderMode();
    }

    if (mTimer == 0) {
        if (mDoRst::getWarningDispFlag() != 0) {
            mTimer = 90;
            mExecCommand = EXEC_NINTENDO_IN;
        } else {
            mTimer = 120;
            mExecCommand = EXEC_WARNING_IN;
        }

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

    f32 alpha = (f32)field_0x210 / (f32)field_0x20e;
    if (field_0x212 != 0) {
        alpha = 1.0f - alpha;
    }

    mWarningStart->setAlpha(255.0f * alpha);
    if (field_0x210 == 0) {
        field_0x210 = field_0x20e;
        field_0x212 ^= 1;
    } else {
        field_0x210--;
    }

    if (mTimer == 0 || mDoCPd_c::getTrig(PAD_1) &
                           (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_START |
                            PAD_TRIGGER_Z | PAD_TRIGGER_L | PAD_TRIGGER_R | PAD_BUTTON_LEFT |
                            PAD_BUTTON_RIGHT | PAD_BUTTON_DOWN | PAD_BUTTON_UP))
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

void dScnLogo_c::dvdWaitDraw() {
    if (!dComIfG_syncAllObjectRes()) {
        if (mpField0Command->sync() && mpAlAnmCommand->sync() && mpFmapResCommand->sync() &&
            mpDmapResCommand->sync() && mpCollectResCommand->sync() && mpItemIconCommand->sync() &&
            mpRingResCommand->sync() && mpPlayerNameCommand->sync() &&
            mpItemInfResCommand->sync() && mpButtonCommand->sync() && mpCardIconCommand->sync() &&
            mpBmgResCommand->sync() && mpMsgComCommand->sync() && mpMsgResCommand[0]->sync() &&
            mpMsgResCommand[1]->sync() && mpMsgResCommand[2]->sync() &&
            mpMsgResCommand[3]->sync() && mpMsgResCommand[4]->sync() &&
            mpMsgResCommand[5]->sync() && mpMsgResCommand[6]->sync() && mpFontResCommand->sync() &&
            mpMain2DCommand->sync() && mpRubyResCommand->sync() && mParticleCommand->sync() &&
            mItemTableCommand->sync() && mEnemyItemCommand->sync() && preLoad_dyl())
        {
            mDoRst::setLogoScnFlag(0);
            mDoRst::setProgChgFlag(0);
            mExecCommand = EXEC_SCENE_CHANGE;
        }
    }
}

void dScnLogo_c::nextSceneChange() {
    if (!mDoRst::isReset()) {
        dComIfG_changeOpeningScene(this, PROC_OPENING_SCENE);
    }
}

dScnLogo_c::~dScnLogo_c() {
    if (mDoRst::isReset()) {
        if (mDoAud_zelAudio_c::isInitFlag()) {
            Z2AudioMgr::getInterface()->resetProcess(5, true);
        }
        mDoRst_reset(0, 0x80000000, 0);
    }

    delete mNintendoLogo;
    delete mWarning;
    delete mWarningStart;
    delete mDolbyLogo;
    delete mProgressiveChoice;
    delete mProgressiveYes;
    delete mProgressiveNo;
    delete mProgressiveSel;

    #if VERSION == VERSION_GCN_PAL
    mpPalLogoResCommand->getArchive()->removeResourceAll();
    mpPalLogoResCommand->getArchive()->unmount();
    mpPalLogoResCommand->destroy();
    #endif

    preLoad_dyl_remove();
    dComIfG_deleteObjectResMain(LOGO_ARC);

    field_0x1d4->destroy();
    field_0x1d0->destroy();
    JKRFree(dummyGameAlloc);

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
    mParticleCommand->destroy();

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
    mItemTableCommand->destroy();

    dEnemyItem_c::setItemData((u8*)mEnemyItemCommand->getMemAddress());
    mEnemyItemCommand->destroy();

    dDlst_shadowControl_c::setSimpleTex((ResTIMG*)dComIfG_getObjectRes("Always", 0x4A));
    dTres_c::createWork();
    dMpath_c::createWork();

    #if PLATFORM_WII
    data_8053a730 = 0;
    #endif
}

static int phase_0(dScnLogo_c* i_this) {
    mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
    dComIfGp_particle_create();

    i_this->dummyGameAlloc = mDoExt_getGameHeap()->alloc(0x340000, -0x10);
    JUT_ASSERT(1523, i_this->dummyGameAlloc != NULL);
    i_this->field_0x1d0 = JKRExpHeap::create(i_this->dummyGameAlloc, 0x340000, NULL, false);
    i_this->field_0x1d4 = JKRExpHeap::create(0x130000, i_this->field_0x1d0, false);

    #if VERSION == VERSION_GCN_PAL
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
    #endif

    return cPhs_NEXT_e;
}

static int phase_1(dScnLogo_c* i_this) {
    if (!cDyl_InitAsyncIsDone()) {
        return cPhs_INIT_e;
    }

    if (!mDoAud_zelAudio_c::isInitFlag() || Z2AudioMgr::getInterface()->checkFirstWaves()) {
        return cPhs_INIT_e;
    }

    #if VERSION == VERSION_GCN_PAL
    if (!mDoDvdThd::SyncWidthSound) {
        return cPhs_INIT_e;
    }

    if (!i_this->mpPalLogoResCommand->sync()) {
        return cPhs_INIT_e;
    }
    #endif

    dComIfG_setObjectRes(LOGO_ARC, (u8)0, i_this->field_0x1d0);
    mDoRst::setLogoScnFlag(1);
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
    int phase_state = resLoad(&field_0x1c4, this);
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    #if PLATFORM_WII
    data_8053a730 = 1;
    #endif

    mpHeap = mDoExt_setCurrentHeap(field_0x1d4);
    logoInitGC();
    mpHeap->becomeCurrentHeap();

    dvdDataLoad();
    Z2AudioMgr::getInterface()->loadStaticWaves();
    mDoGph_gInf_c::setTickRate((OS_BUS_CLOCK / 4) / 60);
    mDoGph_gInf_c::waitBlanking(0);
    field_0x20a = 0;
    mDoGph_gInf_c::startFadeIn(30);

    checkProgSelect();
    if (field_0x20a != 0) {
        mExecCommand = EXEC_PROG_IN;
        mTimer = 30;
        field_0x218 = getProgressiveMode();
    } else {
        if (mDoRst::getWarningDispFlag()) {
            mTimer = 90;
            mExecCommand = EXEC_NINTENDO_IN;
        } else {
            mTimer = 120;
            mExecCommand = EXEC_WARNING_IN;
        }
        mDoRst::setProgSeqFlag(1);
    }

    JUTGamePad::clearResetOccurred();
    JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
    mDoRst::offReset();
    mDoRst::offResetPrepare();

    return phase_state;
}

void dScnLogo_c::logoInitGC() {
    ResTIMG* nintendoImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 4);
    mNintendoLogo = new dDlst_2D_c(nintendoImg, 117, 154, 376, 104, 255);
#if VERSION == VERSION_GCN_JPN
    mNintendoLogo->getPicture()->setWhite(JUtility::TColor(0, 70, 255, 255));
#else
    mNintendoLogo->getPicture()->setWhite(JUtility::TColor(220, 0, 0, 255));
#endif

    ResTIMG* dolbyImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 3);
    mDolbyLogo = new dDlst_2D_c(dolbyImg, 189, 150, 232, 112, 255);

#if VERSION == VERSION_GCN_PAL
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
    mWarning = new dDlst_2D_c(warningImg, 0, 0, FB_WIDTH, FB_HEIGHT, 255);

    ResTIMG* warnStartImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', warningPs[language]);
    mWarningStart = new dDlst_2D_c(warnStartImg, 0, 359, FB_WIDTH, 48, 255);

    ResTIMG* progChoiceImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', choice[language]);
    mProgressiveChoice = new dDlst_2D_c(progChoiceImg, 113, 143, 416, 210, 255);

    ResTIMG* progYesImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', yes[language]);
    mProgressiveYes = new dDlst_2D_c(progYesImg, 121, 352, 200, 72, 255);
    mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    ResTIMG* progNoImg = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', no[language]);
    mProgressiveNo = new dDlst_2D_c(progNoImg, 320, 352, 200, 72, 255);
    mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    mProgressivePro = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', prog[language]);
    mProgressiveInter = (ResTIMG*)mpPalLogoResCommand->getArchive()->getResource('DAT ', intr[language]);
    mProgressiveSel = new dDlst_2D_c(mProgressivePro, 153, 309, 336, 88, 255);
#else
    ResTIMG* warningImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 10);
    mWarning = new dDlst_2D_c(warningImg, 0, 0, FB_WIDTH, FB_HEIGHT, 255);

    ResTIMG* warnStartImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 11);
    mWarningStart = new dDlst_2D_c(warnStartImg, 0, 359, FB_WIDTH, 48, 255);

    ResTIMG* progChoiceImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 5);
    mProgressiveChoice = new dDlst_2D_c(progChoiceImg, 113, 281, 416, 72, 255);

    ResTIMG* progYesImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 9);
    mProgressiveYes = new dDlst_2D_c(progYesImg, 211, 372, 80, 32, 255);
    mProgressiveYes->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    ResTIMG* progNoImg = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 7);
    mProgressiveNo = new dDlst_2D_c(progNoImg, 350, 372, 80, 32, 255);
    mProgressiveNo->getPicture()->setWhite(JUtility::TColor(160, 160, 160, 255));

    mProgressivePro = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 8);
    mProgressiveInter = (ResTIMG*)dComIfG_getObjectRes(LOGO_ARC, 6);
    mProgressiveSel = new dDlst_2D_c(mProgressivePro, 153, 309, 336, 88, 255);
#endif
}

void dScnLogo_c::dvdDataLoad() {
    dComIfG_setObjectRes("Always", (u8)0, NULL);
    archiveHeap->dump_sort();

    dComIfG_setObjectRes("Alink", (u8)0, NULL);

    mpField0Command = mDoDvdThd_mountXArchive_c::create(
        "/res/FieldMap/Field0.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpAlAnmCommand =
        mDoDvdThd_mountXArchive_c::create("/res/Object/AlAnm.arc", 0, JKRArchive::MOUNT_ARAM, NULL);
    mpFmapResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/fmapres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpDmapResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/dmapres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpCollectResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/clctres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpItemIconCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/itemicon.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpRingResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/ringres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpPlayerNameCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/playerName.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpItemInfResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/itmInfRes.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpButtonCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/button.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpCardIconCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/CardIcon/cardicon.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());

    #if VERSION == VERSION_GCN_PAL
    switch (getPalLanguage()) {
    case 1:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgde/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 2:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgfr/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 3:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgsp/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 4:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgit/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 0:
    default:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msguk/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    }
    #else
    mpBmgResCommand = mDoDvdThd_mountXArchive_c::create(MSG_PATH, 0, JKRArchive::MOUNT_MEM, NULL);
    #endif

    mpMsgComCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgcom.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[0] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres00.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[1] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres01.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[2] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres02.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[3] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres03.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
#if VERSION == VERSION_GCN_JPN
    mpMsgResCommand[4] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres04.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
#else
    mpMsgResCommand[4] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres04F.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
#endif
    mpMsgResCommand[5] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres05.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[6] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres06.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMain2DCommand =
        mDoDvdThd_mountXArchive_c::create("/res/Layout/main2D.arc", 0, JKRArchive::MOUNT_MEM, NULL);

#if VERSION == VERSION_GCN_JPN
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontjp/fontres.arc", 1,
                                                         JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontjp/rubyres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
#elif VERSION == VERSION_GCN_PAL
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fonteu/fontres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fonteu/rubyres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
#else
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontus/fontres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontus/rubyres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
#endif
    mParticleCommand = mDoDvdThd_toMainRam_c::create("/res/Particle/common.jpc", 0,
                                                     dComIfGp_particle_getResHeap());
    mItemTableCommand = mDoDvdThd_toMainRam_c::create("/res/ItemTable/item_table.bin", 0, NULL);
    mEnemyItemCommand = mDoDvdThd_toMainRam_c::create("/res/ItemTable/enemy_table.bin", 0, NULL);

    preLoad_dyl_create();
    preLoad_dyl();
}

static int dScnLogo_Create(scene_class* i_this) {
    return (new (i_this) dScnLogo_c())->create();
}

static int dScnLogo_Execute(dScnLogo_c* i_this) {
    if (mDoRst::isReset()) {
        fopScnM_ChangeReq(i_this, PROC_LOGO_SCENE, 0, 5);
    }
    return 1;
}

static int dScnLogo_Draw(dScnLogo_c* i_this) {
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

#if VERSION == VERSION_GCN_PAL
u8 dScnLogo_c::getPalLanguage() {
    u8 language;
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

    return language;
}
#endif

void dScnLogo_c::setProgressiveMode(u8 mode) {
    #if VERSION == VERSION_GCN_PAL
    OSSetEuRgb60Mode(mode);
    #else
    OSSetProgressiveMode(mode);
    #endif
}

u8 dScnLogo_c::getProgressiveMode() {
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

dLog_HIO_c::~dLog_HIO_c() {}

static dScnLogo_Method l_dScnLogo_Method[5] = {
    (dScnLogo_Method)dScnLogo_Create,
    dScnLogo_Delete,
    dScnLogo_Execute,
    dScnLogo_IsDelete,
    dScnLogo_Draw,
};

extern scene_process_profile_definition g_profile_LOGO_SCENE = {
    fpcLy_ROOT_e,
    1,
    fpcPi_CURRENT_e,
    PROC_LOGO_SCENE,
    &g_fpcNd_Method.base,
    sizeof(dScnLogo_c),
    0,
    0,
    &g_fopScn_Method.base,
    (process_method_class*)&l_dScnLogo_Method,
    NULL,
};
