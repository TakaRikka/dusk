#include "HBMBase.h"

#include <revolution/mem.h>
#include <revolution/vi.h>
#include "HBMAnmController.h"
#include "HBMController.h"

#include "nw4hbm/lyt/arcResourceAccessor.h"
#include "nw4hbm/math/triangular.h"

#include <new>

#if HBM_REVISION == 1
#define LN(rev1, rev2) rev1
#else
#define LN(rev1, rev2) rev2
#endif

struct AnmControllerTable {
    /* 0x00 */ int pane;
    /* 0x04 */ int anm;
};  // size = 0x08

static MEMAllocator sAllocator;
static MEMAllocator sSoundAllocator;
MEMAllocator* spAllocator = &sAllocator;
homebutton::HomeButton* homebutton::HomeButton::spHomeButtonObj;
#define gpHomeButton (homebutton::HomeButton::getInstance())

/* DECOMP_FORCE(__FILE__);
DECOMP_FORCE(NW4HBMAssert_String(mpDvdSoundArchive));
DECOMP_FORCE("Cannot open \"%s\"");
DECOMP_FORCE(NW4HBMAssert_String(mpMemorySoundArchive));
DECOMP_FORCE("Cannot setup MemorySoundArchive"); */

void* HBMAllocMem(u32 size) {
    void* addr = MEMAllocFromAllocator(spAllocator, size);
    return addr;
}

void HBMFreeMem(void* mem) {
    MEMFreeToAllocator(spAllocator, mem);
}

void HBMCreate(const HBMDataInfo* pHBInfo) {
    MEMHeapHandle hExpHeap = MEMCreateExpHeap(pHBInfo->mem, pHBInfo->memSize);

    MEMInitAllocatorForExpHeap(&sAllocator, hExpHeap, 32);
    spAllocator = &sAllocator;
    nw4hbm::lyt::Layout::SetAllocator(&sAllocator);
    homebutton::HomeButton::createInstance(pHBInfo);
    gpHomeButton->create();
}

void HBMDelete() {
    homebutton::HomeButton::deleteInstance();
    MEMDestroyExpHeap((MEMHeapHandle)spAllocator->pHeap);
}

void HBMInit() {
    gpHomeButton->init();
}

HBMSelectBtnNum HBMCalc(const HBMControllerData* pController) {
    gpHomeButton->calc(pController);
    return HBMGetSelectBtnNum();
}

void HBMDraw() {
    gpHomeButton->draw_impl();
}

HBMSelectBtnNum HBMGetSelectBtnNum() {
    return gpHomeButton->getSelectBtnNum();
}

void HBMSetAdjustFlag(bool flag) {
    gpHomeButton->setAdjustFlag(flag);
}

void HBMStartBlackOut() {
    gpHomeButton->startBlackOut();
}

void HBMPlaySound(int num) {
    gpHomeButton->play_sound(num);
}

void HBMUpdateSoundArchivePlayer(void) {
    gpHomeButton->updateSoundArchivePlayer();
}

void HBMSetSoundVolume(f32 volume) {
    gpHomeButton->setSoundVolume(volume);
}

void HBMStopSound(void) {
    gpHomeButton->stopSound(false);
}

void HBMCreateSound(const char* path, void* memBuf, u32 memSize) {
    MEMInitAllocatorForFrmHeap(&sSoundAllocator, MEMCreateFrmHeapEx(memBuf, memSize, 0), 32);
    gpHomeButton->initSound(path);
}

void HBMDeleteSound(void) {
    gpHomeButton->deleteSound();
    MEMDestroyFrmHeap((MEMHeapHandle)sSoundAllocator.pHeap);
}

void HBMUpdateSound(void) {
    gpHomeButton->updateSound();
}

enum HBMAllocatorType {
    HBM_ALLOCATOR_APPLI, /* application */
    HBM_ALLOCATOR_LOCAL,
    HBM_ALLOCATOR_NW4HBM,
};

static HBMAllocatorType getAllocatorType(const HBMDataInfo* pHBInfo) {
    if (pHBInfo->pAllocator) {
        return HBM_ALLOCATOR_APPLI;
    } else if (pHBInfo->mem) {
        return HBM_ALLOCATOR_LOCAL;
    } else {
        return HBM_ALLOCATOR_NW4HBM;
    }
}

namespace homebutton {

    void HomeButton::initSound(const char* path) {
        if (!AICheckInit()) {
            AIInit(NULL);
            AXInit();
        }

        nw4hbm::snd::SoundSystem::InitSoundSystem();

        void* pvVar4 =
            MEMAllocFromAllocator(&sSoundAllocator, sizeof(nw4hbm::snd::NandSoundArchive));
        if (pvVar4 != NULL) {
            mpNandSoundArchive = new (pvVar4) nw4hbm::snd::NandSoundArchive();
        }

        NW4HBM_ASSERT(LN(3941, 3884), mpNandSoundArchive);
        NW4R_DB_ASSERTMSG(LN(3946, 3889), mpNandSoundArchive->Open(path), "Cannot open \"%s\"",
                          path);

        u32 size = mpNandSoundArchive->GetHeaderSize();
        mpNandSoundArchive->LoadHeader(MEMAllocFromAllocator(&sSoundAllocator, size), size);
        createSound(mpNandSoundArchive, 1);
    }

    void HomeButton::updateSound() {
        updateSoundArchivePlayer();

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (i < ARRAY_SIZE(mpController)) {
                mpController[i]->updateSound();
            }
        }
    }

    static const AnmControllerTable scAnmTable[12] = {
        {0, 0}, {0, 2}, {4, 1}, {1, 0}, {1, 2}, {5, 1},
        {2, 0}, {2, 2}, {6, 1}, {3, 0}, {3, 2}, {7, 1},
    };

    static const AnmControllerTable scGroupAnmTable[74] = {
        {0, 0},   {1, 1},   {2, 0},   {3, 1},   {4, 2},   {4, 19},  {5, 3},   {5, 20},  {6, 4},
        {6, 7},   {7, 4},   {7, 7},   {8, 4},   {8, 7},   {9, 4},   {9, 7},   {10, 4},  {10, 7},
        {11, 5},  {12, 5},  {13, 6},  {13, 8},  {14, 14}, {14, 6},  {14, 8},  {15, 5},  {16, 6},
        {16, 14}, {17, 11}, {17, 12}, {18, 11}, {18, 12}, {19, 13}, {20, 13}, {21, 9},  {21, 10},
        {22, 9},  {22, 10}, {23, 9},  {23, 10}, {24, 9},  {24, 10}, {25, 9},  {25, 10}, {26, 9},
        {26, 10}, {27, 9},  {27, 10}, {28, 9},  {28, 10}, {29, 9},  {29, 10}, {30, 9},  {30, 10},
        {31, 15}, {31, 16}, {31, 17}, {31, 18}, {31, 21}, {32, 15}, {32, 16}, {32, 17}, {32, 18},
        {32, 21}, {33, 15}, {33, 16}, {33, 17}, {33, 18}, {33, 21}, {34, 15}, {34, 16}, {34, 17},
        {34, 18}, {34, 21},
    };

    const char* HomeButton::scCursorLytName[WPAD_MAX_CONTROLLERS] = {
        "P1_Def.brlyt",
        "P2_Def.brlyt",
        "P3_Def.brlyt",
        "P4_Def.brlyt",
    };

    const char* HomeButton::scCursorPaneName = "N_Trans";
    const char* HomeButton::scCursorRotPaneName = "N_Rot";
    const char* HomeButton::scCursorSRotPaneName = "N_SRot";

    const char* HomeButton::scBtnName[4] = {"B_btnL_00", "B_btnL_01", "B_btnL_10", "B_btnL_11"};
    const char* HomeButton::scTxtName[4] = {"T_btnL_00", "T_btnL_01", "T_btnL_10", "T_btnL_11"};

    const char* HomeButton::scGrName[8] = {
        "btnL_00_inOut", "btnL_01_inOut", "btnL_10_inOut", "btnL_11_inOut",
        "btnL_00_psh",   "btnL_01_psh",   "btnL_10_psh",   "btnL_11_psh",
    };

    const char* HomeButton::scAnimName[3] = {
        "_cntBtn_in.brlan",
        "_cntBtn_psh.brlan",
        "_cntBtn_out.brlan",
    };

    const char* HomeButton::scPairGroupAnimName[15] = {
        "_ltrIcn_on.brlan",      "_optn_bar_psh.brlan",   "_close_bar_psh.brlan",
        "_hmMenu_bar_in.brlan",  "_hmMenu_bar_psh.brlan", "_link_msg_in.brlan",
        "_link_msg_out.brlan",   "_cmn_msg_in.brlan",     "_cmn_msg_out.brlan",
        "_cntrl_up.brlan",       "_cntrl_wndw_opn.brlan", "_cntrl_dwn.brlan",
        "_hmMenu_bar_out.brlan", "_cmn_msg_rtrn.brlan",   "_12btn_on.brlan",
    };

    const char* HomeButton::scPairGroupName[15] = {
        "ltrIcn_on",      "optn_bar_psh", "close_bar_psh",  "hmMenu_bar_in", "hmMenu_bar_psh",
        "link_msg_in",    "link_msg_out", "cmn_msg_in",     "cmn_msg_out",   "cntrl_up",
        "cntrl_wndw_opn", "cntrl_dwn",    "hmMenu_bar_out", "cmn_msg_rtrn",  "12btn_on",
    };

    const char* HomeButton::scGroupAnimName[22] = {
        "_hmMenu_strt.brlan",     "_hmMenu_fnsh.brlan",     "_optn_bar_in.brlan",
        "_optn_bar_out.brlan",    "_optn_btn_in.brlan",     "_optn_btn_psh.brlan",
        "_vb_btn_wht_psh.brlan",  "_optn_btn_out.brlan",    "_vb_btn_ylw_psh.brlan",
        "_sound_gry.brlan",       "_sound_ylw.brlan",       "_cmn_msg_btn_in.brlan",
        "_cmn_msg_btn_out.brlan", "_cmn_msg_btn_psh.brlan", "_vb_btn_ylw_ylw.brlan",
        "_btry_wink.brlan",       "_btry_gry.brlan",        "_btry_wht.brlan",
        "_btry_wink_gry.brlan",   "_close_bar_in.brlan",    "_close_bar_out.brlan",
        "_btry_red.brlan",
    };

    const char* HomeButton::scGroupName[35] = {
        "hmMenu_strt",      "hmMenu_fnsh",      "hmMenuBck_strt",   "hmMenuBck_fnsh",
        "optn_bar_in",      "optn_bar_out",     "optnBtn_00_inOut", "optnBtn_01_inOut",
        "optnBtn_10_inOut", "optnBtn_11_inOut", "optnBtn_20_inOut", "optnBtn_00_psh",
        "optnBtn_01_psh",   "optnBtn_10_psh",   "optnBtn_11_psh",   "optnBtn_20_psh",
        "optnBtn_10_cntrl", "msgBtn_00_inOut",  "msgBtn_01_inOut",  "msgBtn_00_psh",
        "msgBtn_01_psh",    "vol_00",           "vol_01",           "vol_02",
        "vol_03",           "vol_04",           "vol_05",           "vol_06",
        "vol_07",           "vol_08",           "vol_09",           "plyr_00",
        "plyr_01",          "plyr_02",          "plyr_03",
    };

    const char* HomeButton::scFuncPaneName[5] = {"let_icn_00", "N_plyr_00", "N_plyr_01",
                                                 "N_plyr_02", "N_plyr_03"};

    const char* HomeButton::scFuncTouchPaneName[10] = {
        "B_btn_00",     "B_bar_10",     "B_optnBtn_00", "B_optnBtn_01", "B_optnBtn_10",
        "B_optnBtn_11", "B_optnBtn_20", "B_BtnA",       "B_BtnB",       "cntrl_00",
    };

    const char* HomeButton::scFuncTextPaneName[3] = {"T_msg_00", "T_msg_01", "T_Dialog"};

    const char* HomeButton::scBatteryPaneName[WPAD_MAX_CONTROLLERS][4] = {
        {"btryPwr_00_0", "btryPwr_00_1", "btryPwr_00_2", "btryPwr_00_3"},
        {"btryPwr_01_0", "btryPwr_01_1", "btryPwr_01_2", "btryPwr_01_3"},
        {"btryPwr_02_0", "btryPwr_02_1", "btryPwr_02_2", "btryPwr_02_3"},
        {"btryPwr_03_0", "btryPwr_03_1", "btryPwr_03_2", "btryPwr_03_3"},
    };

    void HomeButton::createInstance(const HBMDataInfo* pHBInfo) {
        if (void* pMem = HBMAllocMem(sizeof(*spHomeButtonObj))) {
            spHomeButtonObj = new (pMem) HomeButton(pHBInfo);
        }
    }

    void HomeButton::deleteInstance() {
        spHomeButtonObj->~HomeButton();
        HBMFreeMem(spHomeButtonObj);
        spHomeButtonObj = NULL;
    }

    void HomeButton::BlackFader::init(int maxFrame) {
        frame_ = 0;
        maxFrame_ = maxFrame;
        state_ = 0;
    }

    void HomeButton::BlackFader::calc() {
        if (state_ == 1) {
            frame_++;
        } else if (state_ == 2) {
            frame_--;
        }

        if (frame_ < 0) {
            frame_ = 0;
        } else if (frame_ > maxFrame_) {
            frame_ = maxFrame_;
        }
    }

    bool HomeButton::BlackFader::isDone() {
        if (state_ == 1) {
            if (frame_ == maxFrame_) {
                return true;
            }
        }

        if (state_ == 2) {
            if (frame_ == 0) {
                return true;
            }
        }

        return false;
    }

    void HomeButton::draw_impl() {
        u8 alpha;
        BlackFader* pFader;
        HomeButton* pHBM;
        int i;

        mpLayout->Draw(mDrawInfo);

        if (mpHBInfo->cursor == 0) {
            for (i = WPAD_MAX_CONTROLLERS - 1; i >= WPAD_CHAN0; i--) {
                mpCursorLayout[i]->Draw(mDrawInfo);
            }
        }

        pFader = &mFader;
        alpha = pFader->getFrame() * 255 / pFader->getMaxFrame();
        initgx();
        GXSetTevColor(GX_TEVREG0, pFader->GetColor(alpha));
        drawBlackPlate(-1000.0f, -1000.0f, 1000.0f, 1000.0f);
    }

    int HomeButton::findGroupAnimator(int pane, int anm) {
        for (int i = 0; i < (int)ARRAY_SIZE(scGroupAnmTable); i++) {
            if (scGroupAnmTable[i].pane == pane && scGroupAnmTable[i].anm == anm) {
                return i;
            }
        }

        return -1;
    }

    HBMSelectBtnNum HomeButton::getSelectBtnNum() {
        if (mState != 18) {
            return HBM_SELECT_NULL;
        }

        return mSelectBtnNum;
    }

    void HomeButton::updateSoundArchivePlayer() {
        if (mpSoundArchivePlayer != NULL) {
            mpSoundArchivePlayer->Update();
        }
    }

    void HomeButton::fadeout_sound(f32 gain) {
        if (mSelectBtnNum == HBM_SELECT_BTN3) {
            return;
        }

        if (mEndInitSoundFlag) {
            AXSetMasterVolume(gain * 32768.0f);
        }

        if (mpSoundArchivePlayer != NULL) {
            for (int i = 0; i < mpSoundArchivePlayer->GetSoundPlayerCount(); i++) {
                mpSoundArchivePlayer->GetSoundPlayer(i).SetVolume(gain);
            }
        }
    }

    void HomeButton::setSoundVolume(f32 volume) {
        AXSetMasterVolume(volume * 32768.0f);

        if (mpSoundArchivePlayer != NULL) {
            for (int i = 0; i < mpSoundArchivePlayer->GetSoundPlayerCount(); i++) {
                mpSoundArchivePlayer->GetSoundPlayer(i).SetVolume(volume);
            }
        }
    }

    void HomeButton::play_sound(int id) {
        int ret = 0;

        if (mpHBInfo->sound_callback != NULL) {
            ret = mpHBInfo->sound_callback(5, id);
        }

        if (ret == 0) {
            PlaySeq(id);
        }
    }

    void HomeButton::stopSound(bool checkFlag) {
        if (mpSoundArchivePlayer != NULL) {
            for (int i = 0; i < mpSoundArchivePlayer->GetSoundPlayerCount(); i++) {
                mpSoundArchivePlayer->GetSoundPlayer(i).StopAllSound(0);
            }
        }

        if (checkFlag && !mEndInitSoundFlag) {
            return;
        }

        AXFXReverbHiShutdown(&mAxFxReverb);
        AXRegisterAuxACallback(mAuxCallback, mpAuxContext);
        AXFXSetHooks(mAxFxAlloc, mAxFxFree);
        AXSetMasterVolume(mAppVolume[0]);
        AXSetAuxAReturnVolume(mAppVolume[1]);
        AXSetAuxBReturnVolume(mAppVolume[2]);
    }

    HomeButton::HomeButton(const HBMDataInfo* pHBInfo)
        : mpHBInfo(pHBInfo), mpLayout(NULL), mpPaneManager(NULL), mFader(30) {
        mState = 2;
        mSelectBtnNum = HBM_SELECT_NULL;
        mSelectAnmNum = -1;
        mMsgCount = 0;
        mSequence = eSeq_Normal;
        mForcusSEWaitTime = 0;
        mLetterFlag = false;
        mBar0AnmRev = 0;
        mBar1AnmRev = 0;
        mBar0AnmRevHold = 0;
        mBar1AnmRevHold = 0;
        mAdjustFlag = false;
        mReassignedFlag = false;

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            OSCreateAlarm(&mAlarm[i]);
            OSCreateAlarm(&mSpeakerAlarm[i]);
        }

        OSCreateAlarm(&mSimpleSyncAlarm);

        mpSoundArchivePlayer = NULL;
        mpDvdSoundArchive = NULL;
        mpMemorySoundArchive = NULL;
        mpNandSoundArchive = NULL;
        mpSoundHeap = NULL;
        mpSoundHandle = NULL;
    }

    HomeButton::~HomeButton() {
        int i;

        mpResAccessor->~ArcResourceAccessor();
        HBMFreeMem(mpResAccessor);

        mpLayout->~Layout();
        HBMFreeMem(mpLayout);

        if (mpHBInfo->cursor == 0) {
            for (i = 0; i < (int)ARRAY_SIZE(mpCursorLayout); i++) {
                mpCursorLayout[i]->~Layout();
                HBMFreeMem(mpCursorLayout[i]);
            }
        }

        for (i = 0; i < mAnmNum; i++) {
            mpAnmController[i]->~GroupAnmController();
            HBMFreeMem(mpAnmController[i]);
        }

        for (i = 0; i < (int)ARRAY_SIZE(mpPairGroupAnmController); i++) {
            mpPairGroupAnmController[i]->~GroupAnmController();
            HBMFreeMem(mpPairGroupAnmController[i]);
        }

        for (i = 0; i < (int)ARRAY_SIZE(mpGroupAnmController); i++) {
            mpGroupAnmController[i]->~GroupAnmController();
            HBMFreeMem(mpGroupAnmController[i]);
        }

        mpHomeButtonEventHandler->HomeButtonEventHandler::~HomeButtonEventHandler();
        HBMFreeMem(mpHomeButtonEventHandler);

        mpPaneManager->~PaneManager();
        HBMFreeMem(mpPaneManager);

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            mpController[i]->~Controller();
            HBMFreeMem(mpController[i]);
        }

        mpRemoteSpk->~RemoteSpk();
        HBMFreeMem(mpRemoteSpk);
        mpRemoteSpk = NULL;

        HBMFreeMem(mpLayoutName);
        HBMFreeMem(mpAnmName);

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            OSCancelAlarm(&mAlarm[i]);
            OSCancelAlarm(&mSpeakerAlarm[i]);
        }

        OSCancelAlarm(&mSimpleSyncAlarm);
    }

    void HomeButton::create() {
        int i;
        char anmNameBuf[64];

        mInitFlag = false;
        mForceSttInitProcFlag = false;
        mForceSttFadeInProcFlag = false;

        set_config();
        set_text();

        if (void* pMem = HBMAllocMem(sizeof(*mpResAccessor))) {
            mpResAccessor = new (pMem) nw4hbm::lyt::ArcResourceAccessor();
        }

        NW4HBM_ASSERT(623, mpResAccessor);
        mpResAccessor->Attach(mpHBInfo->layoutBuf, "arc");

        if (!mpHBInfo->cursor) {
            for (i = 0; i < (int)ARRAY_SIZE(mpCursorLayout); i++) {
                if (void* pMem = HBMAllocMem(sizeof(*mpCursorLayout)[i])) {
                    mpCursorLayout[i] = new (pMem) nw4hbm::lyt::Layout();
                }

                NW4HBM_ASSERT(635, mpCursorLayout[i]);

                void* lytRes = mpResAccessor->GetResource(0, scCursorLytName[i], NULL);

                mpCursorLayout[i]->Build(lytRes, mpResAccessor);
            }
        }

        if (void* pMem = HBMAllocMem(sizeof(*mpLayout))) {
            mpLayout = new (pMem) nw4hbm::lyt::Layout();
        }

        {
            void* lytRes = mpResAccessor->GetResource(0, mpLayoutName, NULL);
            mpLayout->Build(lytRes, mpResAccessor);
        }

        for (i = 0; i < mAnmNum; i++) {
            std::strcpy(anmNameBuf, mpAnmName);
            std::strcat(anmNameBuf, scAnimName[scAnmTable[i].anm]);

            void* lpaRes = mpResAccessor->GetResource(0, anmNameBuf, NULL);
            NW4HBM_ASSERT_CHECK_NULL(665, lpaRes);

            if (void* pMem = HBMAllocMem(sizeof(*mpAnmController)[i])) {
                mpAnmController[i] = new (pMem) GroupAnmController();
            }

            NW4HBM_ASSERT(671, mpAnmController[i]);

            mpAnmController[i]->mpAnimGroup = mpLayout->CreateAnimTransform(lpaRes, mpResAccessor);

            mpAnmController[i]->mpGroup =
                mpLayout->GetGroupContainer()->FindGroupByName(scGrName[scAnmTable[i].pane]);

            nw4hbm::lyt::PaneLinkList& list = mpAnmController[i]->mpGroup->GetPaneList();

            for (nw4hbm::lyt::PaneLinkList::Iterator it = list.GetBeginIter();
                 it != list.GetEndIter(); it++)
            {
                it->mTarget->BindAnimation(mpAnmController[i]->mpAnimGroup, false);
            }

            mpAnmController[i]->init(ANIM_TYPE_FORWARD,
                                     mpAnmController[i]->mpAnimGroup->GetFrameMax(), 0,
                                     mpHBInfo->frameDelta);
        }

        for (i = 0; i < (int)ARRAY_SIZE(mpGroupAnmController); i++) {
            std::strcpy(anmNameBuf, mpAnmName);
            std::strcat(anmNameBuf, scGroupAnimName[scGroupAnmTable[i].anm]);

            void* lpaRes = mpResAccessor->GetResource(0, anmNameBuf, NULL);

            NW4HBM_ASSERT_CHECK_NULL(697, lpaRes);

            if (void* pMem = HBMAllocMem(sizeof(*mpGroupAnmController)[i])) {
                mpGroupAnmController[i] = new (pMem) GroupAnmController();
            }

            NW4HBM_ASSERT(703, mpGroupAnmController[i]);

            mpGroupAnmController[i]->mpAnimGroup =
                mpLayout->CreateAnimTransform(lpaRes, mpResAccessor);

            mpGroupAnmController[i]->mpGroup = mpLayout->GetGroupContainer()->FindGroupByName(
                scGroupName[scGroupAnmTable[i].pane]);

            nw4hbm::lyt::PaneLinkList& list = mpGroupAnmController[i]->mpGroup->GetPaneList();

            for (nw4hbm::lyt::PaneLinkList::Iterator it = list.GetBeginIter();
                 it != list.GetEndIter(); it++)
            {
                it->mTarget->BindAnimation(mpGroupAnmController[i]->mpAnimGroup, false);
            }

            mpGroupAnmController[i]->init(ANIM_TYPE_FORWARD,
                                          mpGroupAnmController[i]->mpAnimGroup->GetFrameMax(), 0,
                                          mpHBInfo->frameDelta);
        }

        for (i = 0; i < (int)ARRAY_SIZE(mpPairGroupAnmController); i++) {
            std::strcpy(anmNameBuf, mpAnmName);
            std::strcat(anmNameBuf, scPairGroupAnimName[i]);

            void* lpaRes = mpResAccessor->GetResource(0, anmNameBuf, NULL);

            NW4HBM_ASSERT_CHECK_NULL(729, lpaRes);

            if (void* pMem = HBMAllocMem(sizeof(*mpPairGroupAnmController)[i])) {
                mpPairGroupAnmController[i] = new (pMem) GroupAnmController();
            }

            NW4HBM_ASSERT(735, mpPairGroupAnmController[i]);

            mpPairGroupAnmController[i]->mpAnimGroup =
                mpLayout->CreateAnimTransform(lpaRes, mpResAccessor);

            mpPairGroupAnmController[i]->mpGroup =
                mpLayout->GetGroupContainer()->FindGroupByName(scPairGroupName[i]);

            nw4hbm::lyt::PaneLinkList& list = mpPairGroupAnmController[i]->mpGroup->GetPaneList();

            for (nw4hbm::lyt::PaneLinkList::Iterator it = list.GetBeginIter();
                 it != list.GetEndIter(); it++)
            {
                it->mTarget->BindAnimation(mpPairGroupAnmController[i]->mpAnimGroup, false);
            }

            mpPairGroupAnmController[i]->init(
                ANIM_TYPE_FORWARD, mpPairGroupAnmController[i]->mpAnimGroup->GetFrameMax(), 0,
                mpHBInfo->frameDelta);
        }

        if (void* pMem = HBMAllocMem(sizeof(*mpHomeButtonEventHandler))) {
            mpHomeButtonEventHandler = new (pMem) HomeButtonEventHandler(this);
        }

        NW4HBM_ASSERT(758, mpHomeButtonEventHandler);

        if (void* pMem = HBMAllocMem(sizeof(*mpPaneManager))) {
            mpPaneManager =
                new (pMem) gui::PaneManager(mpHomeButtonEventHandler, NULL, spAllocator);
        }

        NW4HBM_ASSERT(765, mpPaneManager);

        mpPaneManager->createLayoutScene(*mpLayout);
        mpPaneManager->setAllComponentTriggerTarget(false);

        for (i = 0; i < mButtonNum; i++) {
            nw4hbm::lyt::Pane* pTouchPane =
                mpLayout->GetRootPane()->FindPaneByName(scBtnName[i], true);
            mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(true);
        }

        if (void* pMem = HBMAllocMem(sizeof(*mpRemoteSpk))) {
            mpRemoteSpk = new (pMem) RemoteSpk(mpHBInfo->spkSeBuf);
        }

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (void* pMem = HBMAllocMem(sizeof(*mpController)[i])) {
                mpController[i] = new (pMem) Controller(i, mpRemoteSpk);
            }
        }

        mpPaneManager->setDrawInfo(&mDrawInfo);

        nw4hbm::math::VEC2 ad_v(1.0f / mpHBInfo->adjust.x, 1.0f);
        mDrawInfo.SetLocationAdjustScale(ad_v);
        mDrawInfo.SetLocationAdjust(mAdjustFlag);

        nw4hbm::math::MTX34 viewMtx;
        nw4hbm::math::MTX34Identity(&viewMtx);
        mDrawInfo.SetViewMtx(viewMtx);

        init_msg();
    }

    static u32 get_comma_length(char* pBuf) {
        u32 len;

        for (len = 0; pBuf[len]; len++) {
            if (pBuf[len] == ',') {
                break;
            }
        }

        return len;
    }

    void HomeButton::set_config() {
        int i = 0, j = 0;

        char* pConfig = static_cast<char*>(mpHBInfo->configBuf);
        char* pEnd = static_cast<char*>(mpHBInfo->configBuf) + mpHBInfo->memSize;
        u32 len = get_comma_length(pConfig);

        mpLayoutName = static_cast<char*>(HBMAllocMem(len + 1));

        NW4HBM_ASSERT(827, mpLayoutName);

        std::strncpy(mpLayoutName, pConfig, len);
        mpLayoutName[len] = '\0';

        pConfig += len + 1;

        len = get_comma_length(pConfig);
        mpAnmName = static_cast<char*>(HBMAllocMem(len + 1));

        NW4HBM_ASSERT(837, mpAnmName);

        std::strncpy(mpAnmName, pConfig, len);
        mpAnmName[len] = '\0';

        pConfig += len;

        // comma operator generates a temporary
        i = 0;
        j = 0;
        for (; pConfig[i]; i++) {
            if (pConfig[i] == ',') {
                if (pConfig[i + 1] == '1') {
                    mDialogFlag[j] = true;
                } else {
                    mDialogFlag[j] = false;
                }

                j++;
            }
        }

        mButtonNum = j;
        mAnmNum = mButtonNum * 3;
    }

    void HomeButton::set_text() {
        int i = 0, j = 0, k = 0;
        bool flag = false;  // more accurately insideStringFlag

        wchar_t* message = static_cast<wchar_t*>(mpHBInfo->msgBuf);
        for (; message[i]; i++) {
            if (message[i] == L'\"') {
                message[i] = '\0';

                if (!flag) {
                    flag = true;

                    mpText[j][k] = &message[i + 1];
                    j++;

                    if (j == 0x07) {
                        j = 0;
                        k++;
                    }
                } else {
                    flag = false;
                }
            }
        }
    }

    void HomeButton::init() {
        int i;

        if (mInitFlag) {
            return;
        }

        mInitFlag = true;

        mForceSttInitProcFlag = false;
        mForceSttFadeInProcFlag = false;
        mForceStopSyncFlag = false;

        if (mSelectBtnNum != HBM_SELECT_BTN3) {
            mEndInitSoundFlag = false;
        }

        GXSetCullMode(GX_CULL_NONE);

        for (i = 0; i < (int)ARRAY_SIZE(mPaneCounter); i++) {
            mPaneCounter[i] = 0;
        }

        mState = 0;
        mSequence = eSeq_Normal;
        mReassignedFlag = false;

        updateTrigPane();

        mpPaneManager->init();

        reset_guiManager(-1);

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (i < WPAD_MAX_CONTROLLERS) {
                mPadDrawTime[i] = 0;

                mpController[i]->setInValidPos();
                mpController[i]->clrKpadButton();
                mpController[i]->disconnect();
                mpController[i]->clrBatteryFlag();
                mpController[i]->initCallback();
                mpController[i]->initSound();

                mOnPaneVibFrame[i] = 0.0f;
                mOnPaneVibWaitFrame[i] = 0.0f;
            }
        }

        mDrawInfo.SetViewRect(mpLayout->GetLayoutRect());
        mpLayout->GetRootPane()->FindPaneByName(scFuncPaneName[0], true)->SetVisible(false);

        // 2-6: "B_optnBtn_XX" entries in scFuncTouchPaneName
        for (i = 2; i < 7; i++) {
            mpLayout->GetRootPane()
                ->FindPaneByName(scFuncTouchPaneName[i], true)
                ->SetVisible(false);
        }

        for (i = 0; i < (int)ARRAY_SIZE(scFuncTextPaneName); i++) {
            mpLayout->GetRootPane()->FindPaneByName(scFuncTextPaneName[i], true)->SetVisible(false);
        }

        mpRemoteSpk->Start();

        if (mpSoundArchivePlayer != NULL) {
            for (i = 0; i < mpSoundArchivePlayer->GetSoundPlayerCount(); i++) {
                mpSoundArchivePlayer->GetSoundPlayer(i).SetVolume(1.0f);
            }
        }

        calc(NULL);
        mFader.init(30);
    }

    void HomeButton::init_msg() {
        int i, len;

        for (i = 0; i < (int)ARRAY_SIZE(scFuncTextPaneName); i++) {
            nw4hbm::lyt::Pane* p_pane =
                mpLayout->GetRootPane()->FindPaneByName(scFuncTextPaneName[i], true);
            nw4hbm::lyt::TextBox* p_text =
                nw4hbm::ut::DynamicCast<nw4hbm::lyt::TextBox*, nw4hbm::lyt::Pane>(p_pane);

            p_text->SetString(mpText[mpHBInfo->region][i], 0);
        }
    }

    void HomeButton::init_volume() {
        int i, anm_no;

        mVolumeNum = getVolume();
        setVolume(10);

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
            getController(i)->connect();
        }

        for (i = 0; i < 10; i++) {
            if (i < mVolumeNum) {
                anm_no = findGroupAnimator(i + 0x15, 10);
                mpGroupAnmController[anm_no]->start();
            } else {
                anm_no = findGroupAnimator(i + 0x15, 9);
                mpGroupAnmController[anm_no]->start();
            }
        }
    }

    void HomeButton::init_vib() {
        int anm_no;

        mVibFlag = getVibFlag();
        if (mVibFlag) {
            anm_no = findGroupAnimator(13, 6);
            mpGroupAnmController[anm_no]->start();

            anm_no = findGroupAnimator(14, 8);
            mpGroupAnmController[anm_no]->start();
        } else {
            anm_no = findGroupAnimator(13, 8);
            mpGroupAnmController[anm_no]->start();

            anm_no = findGroupAnimator(14, 6);
            mpGroupAnmController[anm_no]->start();
        }
    }

    void HomeButton::init_sound() {
        if (mpHBInfo->sound_callback != NULL) {
            mpHBInfo->sound_callback(0, 0);
        }

        mAppVolume[0] = AXGetMasterVolume();
        mAppVolume[1] = AXGetAuxAReturnVolume();
        mAppVolume[2] = AXGetAuxBReturnVolume();

        AXFXGetHooks(&mAxFxAlloc, &mAxFxFree);
        AXGetAuxACallback(&mAuxCallback, &mpAuxContext);
        AXFXSetHooks(&HBMAllocMem, &HBMFreeMem);

        mAxFxReverb.preDelay = 0.0f;
        mAxFxReverb.time = 2.5f;
        mAxFxReverb.coloration = 0.5f;
        mAxFxReverb.damping = 0.0f;
        mAxFxReverb.crosstalk = 0.0f;
        mAxFxReverb.mix = 1.0f;

        AXFXReverbHiInit(&mAxFxReverb);
        AXRegisterAuxACallback(&AXFXReverbHiCallback, &mAxFxReverb);
        AXSetMasterVolume(0x8000);
        AXSetAuxAReturnVolume(0);
        AXSetAuxBReturnVolume(0);

        if (mpHBInfo->sound_callback != NULL) {
            mpHBInfo->sound_callback(1, 0);
        }

        mEndInitSoundFlag = true;
    }

    void HomeButton::init_battery(const HBMControllerData* pController) {
        int anm_no;

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (pController->wiiCon[i].kpad) {
                if (!mpHBInfo->cursor) {
                    mpCursorLayout[i]
                        ->GetRootPane()
                        ->FindPaneByName(scCursorPaneName, true)
                        ->SetVisible(true);
                }

                anm_no = findGroupAnimator(i + 31, 17);
                mpGroupAnmController[anm_no]->start();
                mControllerFlag[i] = true;

                getController(i)->getInfoAsync(&mWpadInfo[i]);
            } else {
                if (!mpHBInfo->cursor) {
                    mpCursorLayout[i]
                        ->GetRootPane()
                        ->FindPaneByName(scCursorPaneName, true)
                        ->SetVisible(false);
                }

                anm_no = findGroupAnimator(i + 31, 16);
                mpGroupAnmController[anm_no]->start();
                mControllerFlag[i] = false;
            }
        }

        reset_battery();
        mGetPadInfoTime = 0;
    }

    void HomeButton::calc(const HBMControllerData* pController) {
        int i;

        mpPaneManager->calc();

        for (i = 0; i < mAnmNum; i++) {
            mpAnmController[i]->do_calc();
        }

        for (i = 0; i < (int)ARRAY_SIZE(mpPairGroupAnmController); i++) {
            mpPairGroupAnmController[i]->do_calc();
        }

        for (i = 0; i < (int)ARRAY_SIZE(mpGroupAnmController); i++) {
            mpGroupAnmController[i]->do_calc();
        }

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (mOnPaneVibFrame[i] > 0.0f) {
                if (!mControllerFlag[i] || !getController(i)->isRumbling()) {
                    mOnPaneVibFrame[i] = 0.0f;
                    mOnPaneVibWaitFrame[i] = 0.0f;

                    if (getController(i)->isRumbling()) {
                        getController(i)->stopMotor();
                    }

                    continue;
                }

                mOnPaneVibFrame[i] -= mpHBInfo->frameDelta;
                if (mOnPaneVibFrame[i] <= 0.0f || mState == 17) {
                    getController(i)->stopMotor();
                    mOnPaneVibFrame[i] = 0.0f;
                    mOnPaneVibWaitFrame[i] = 9.0f;
                }
                continue;
            } else if (mOnPaneVibWaitFrame[i] > 0.0f) {
                mOnPaneVibWaitFrame[i] -= mpHBInfo->frameDelta;

                if (mOnPaneVibWaitFrame[i] <= 0.0f) {
                    mOnPaneVibWaitFrame[i] = 0.0f;
                }
            }
        }

        switch (mState) {
        case 0:
            if (mpHBInfo->backFlag) {
                mSelectAnmNum = findGroupAnimator(2, 0);

                mpLayout->GetRootPane()->FindPaneByName("back_00", true)->SetVisible(false);

                mpLayout->GetRootPane()->FindPaneByName("back_02", true)->SetVisible(true);
            } else {
                mSelectAnmNum = findGroupAnimator(0, 0);

                mpLayout->GetRootPane()->FindPaneByName("back_00", true)->SetVisible(true);

                mpLayout->GetRootPane()->FindPaneByName("back_02", true)->SetVisible(false);
            }

            mpGroupAnmController[mSelectAnmNum]->start();

            if (pController) {
                mState = 1;
                init_battery(pController);
            }

            break;

        case 1:
            if (!mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
                init_volume();
                init_vib();
                init_sound();
                play_sound(0);

                mState = 2;
            }

            break;

        case 2:
            if (mLetterFlag && !mpPairGroupAnmController[0]->isPlaying()) {
                mpLayout->GetRootPane()->FindPaneByName(scFuncPaneName[0], true)->SetVisible(true);
                mpPairGroupAnmController[0]->setAnimType(2);
                mpPairGroupAnmController[0]->start();
            } else if (!mLetterFlag) {
                mpLayout->GetRootPane()->FindPaneByName(scFuncPaneName[0], true)->SetVisible(false);
                mpPairGroupAnmController[0]->setState(0);
            }

#if HBM_REVISION == 1
            if (mpPairGroupAnmController[14]->isPlaying() &&
                mpPairGroupAnmController[14]->getCurrentFrame() <
                    mpPairGroupAnmController[14]->getDelta())
            {
                mpPairGroupAnmController[14]->setCurrentFrame(0.0f);
                mpPairGroupAnmController[14]->setState(0);
            }
#endif
            break;

        case 3:
            if (!mpGroupAnmController[mSelectAnmNum]->isPlaying() && mSelectAnmNum != 5) {
                reset_battery();
                mSelectAnmNum = 5;
                mpPairGroupAnmController[mSelectAnmNum]->start();
            }

            mWaitStopMotorCount--;
            if (mWaitStopMotorCount <= 0) {
                for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                    WPADDisconnect(i);
                }

                mState = 4;
            }

            break;

        case 4:
            if (mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                u32 type;
                if (WPADProbe(i, &type) != WPAD_ENODEV) {
                    break;
                }
            }

            if (i < WPAD_MAX_CONTROLLERS) {
                break;
            }

            mState = 5;
            mMsgCount = 0;

#if HBM_REVISION > 1
            mSoundRetryCnt = 0;
#endif

            mSimpleSyncCallback = WPADSetSimpleSyncCallback(&SimpleSyncCallback);
            mEndSimpleSyncFlag = false;

            mSimpleSyncFlag = WPADStartFastSimpleSync();

            if (!mSimpleSyncFlag) {
                setSimpleSyncAlarm(0);
            }

            break;

        case 5:
        case 6:
            if (!mSimpleSyncFlag || mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            if (mMsgCount == 0) {
                reset_control();
                reset_btn();
                mpPairGroupAnmController[14]->setAnimType(2);
                mpPairGroupAnmController[14]->start();
            }

            if (mControllerFlag[3]) {
                if (mState != 6) {
                    if (getController(3)->isPlayingSoundId(5)) {
                        mState = 6;
                        mMsgCount = 0xDF2;
                    }

#if HBM_REVISION > 1
                    mSoundRetryCnt++;
                    if (mSoundRetryCnt > 0xDF2) {
                        mState = 6;
                        mMsgCount = 0xDF2;
                    }
#endif
                } else {
                    mMsgCount++;
                    if (mMsgCount > 0xE10) {
                        mState = 7;
                    }
                }
            } else {
                mMsgCount++;
                if (mMsgCount > 3600) {
                    mState = 7;

                    if (!WPADStopSimpleSync()) {
                        setSimpleSyncAlarm(1);
                    }
                }
            }

            break;

        case 7:
            if (!mEndSimpleSyncFlag) {
                break;
            }

            WPADSetSimpleSyncCallback(mSimpleSyncCallback);
            mSimpleSyncCallback = NULL;

#if HBM_REVISION > 1
            mpRemoteSpk->ClearPcm();
#endif

            reset_guiManager(-1);

            mSelectAnmNum = 6;
            mpPairGroupAnmController[mSelectAnmNum]->start();

            mState = 8;

#if HBM_REVISION > 1
            mpPairGroupAnmController[14]->setAnimType(0);
#endif

            play_sound(21);

            break;

        case 8:
            if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            if (mSelectAnmNum == 13) {
                reset_window();

                mpLayout->GetRootPane()
                    ->FindPaneByName(scFuncTextPaneName[2], true)
                    ->SetVisible(false);
            } else if (mSelectAnmNum == 6) {
                mpLayout->GetRootPane()
                    ->FindPaneByName(scFuncTextPaneName[0], true)
                    ->SetVisible(false);

                mpLayout->GetRootPane()
                    ->FindPaneByName(scFuncTextPaneName[1], true)
                    ->SetVisible(false);
            }

            mState = 2;
            break;

        case 9:
            if (mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            if (mVibFlag) {
                for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                    getController(i)->stopMotor();
                }
            }

            mState = 2;

            break;

        case 10:
            if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            mBar0AnmRev = 0;
            mBar1AnmRev = 0;
            mBar0AnmRevHold = 0;
            mBar1AnmRevHold = 0;

            if (mSequence != eSeq_Control) {
                // 2-6: "B_optnBtn_XX" entries in scFuncTouchPaneName
                for (i = 2; i < 7; i++) {
                    mpLayout->GetRootPane()
                        ->FindPaneByName(scFuncTouchPaneName[i], true)
                        ->SetVisible(false);
                }

                mState = 2;
            } else if (mSequence == eSeq_Control)  // ? already true
            {
                // mpLayout->GetRootPane()->FindPaneByName("bar_00", true)->SetVisible(false);

                mSelectAnmNum = 10;
                mpPairGroupAnmController[mSelectAnmNum]->start();

                mSelectAnmNum = 1;
                play_sound(7);

                mState = 8;
            }

            updateTrigPane();

            break;

        case 11:
            if (mpAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            mSelectAnmNum = 7;
            mpPairGroupAnmController[mSelectAnmNum]->start();

            mState = 12;

            break;

        case 12:
            if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            updateTrigPane();
            reset_btn();
            mState = 2;

            break;

        case 13:
            if (mpGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            // only case is SELECT_NULL
            if (mSelectBtnNum >= 0) {
                mFader.start();
                mState = 19;
                mFadeOutSeTime = mFader.getMaxFrame();

                if (mpHBInfo->sound_callback != NULL) {
                    mpHBInfo->sound_callback(mSelectBtnNum != HBM_SELECT_BTN3 ? 3 : 6,
                                             mFadeOutSeTime);
                }
            } else {
                updateTrigPane();
                mSelectAnmNum = 13;
                mpPairGroupAnmController[mSelectAnmNum]->start();
                mState = 8;
            }

            reset_guiManager(-1);
            break;

        case 14:
            if (mpPairGroupAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            calc_fadeoutAnm();
            break;

        case 15:
            if (mpAnmController[mSelectAnmNum]->isPlaying()) {
                break;
            }

            mFader.start();
            mState = 19;
            mFadeOutSeTime = mFader.getMaxFrame();

            if (mpHBInfo->sound_callback != NULL) {
                mpHBInfo->sound_callback(mSelectBtnNum != HBM_SELECT_BTN3 ? 3 : 6, mFadeOutSeTime);
            }

            break;

        case 16:
            GroupAnmController* anim = mpGroupAnmController[mSelectAnmNum];

            if (!anim->isPlaying()) {
                mState = 17;

                fadeout_sound(0.0f);
            } else {
                f32 restFrame = anim->getMaxFrame() - anim->getCurrentFrame();
                fadeout_sound(restFrame / mFadeOutSeTime);
            }
            break;

        case 17:
            mState = 18;

            if (mSelectBtnNum != HBM_SELECT_BTN3) {
                stopSound(true);
            }

            setVolume(mVolumeNum);
            bool result = WPADSaveConfig(NULL);
            mpRemoteSpk->Stop();

            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                if (i < WPAD_MAX_CONTROLLERS) {
                    mpController[i]->clearCallback();
                }
            }

            NW4HBM_ASSERT(LN(1671, 1649), result);

            if (mSelectBtnNum != HBM_SELECT_BTN3 && mpHBInfo->sound_callback != NULL) {
                mpHBInfo->sound_callback(4, 0);
            }

            mInitFlag = false;
            break;

        case 18:
            mState = 2;
            break;

        case 19:
            if (mForceSttInitProcFlag) {
                init_battery(pController);
                mForceSttInitProcFlag = false;
            }

            if (mForceSttFadeInProcFlag) {
                init_volume();
                init_vib();
                mForceSttFadeInProcFlag = false;
            }

            if (mFader.isDone()) {
                if (mForceStopSyncFlag) {
                    if (!mEndSimpleSyncFlag) {
                        break;
                    }

                    WPADSetSimpleSyncCallback(mSimpleSyncCallback);
                    mSimpleSyncCallback = NULL;
                    mForceStopSyncFlag = false;
                }

                if (mForceEndMsgAnmFlag) {
                    int anm_no;

                    anm_no = 5;
                    mpPairGroupAnmController[anm_no]->initFrame();
                    mpPairGroupAnmController[anm_no]->stop();

                    mpLayout->GetRootPane()
                        ->FindPaneByName(scFuncTextPaneName[0], true)
                        ->SetVisible(false);
                    mpLayout->GetRootPane()
                        ->FindPaneByName(scFuncTextPaneName[1], true)
                        ->SetVisible(false);

                    anm_no = 14;
                    if (mpPairGroupAnmController[anm_no]->isPlaying()) {
                        mpPairGroupAnmController[anm_no]->initFrame();
                        mpPairGroupAnmController[anm_no]->stop();
                    }
                }

                mState = 17;

                VISetBlack(true);
                VIFlush();
                fadeout_sound(0.0f);
            } else {
                f32 restFrame = mFader.getMaxFrame() - mFader.getFrame();

                fadeout_sound(restFrame / mFadeOutSeTime);
            }

            break;
        default:
            break;
        }

        if (mBar0AnmRev && isUpBarActive()) {
            if (mBar0AnmRev && mBar0AnmRev != mBar0AnmRevHold) {
                mpPairGroupAnmController[mBar0AnmRev]->start();
                mBar0AnmRevHold = mBar0AnmRev;
            }

            mBar0AnmRev = 0;
        }

        if (mBar1AnmRev && isDownBarActive()) {
            if (mBar1AnmRev && mBar1AnmRev != mBar1AnmRevHold) {
                mpGroupAnmController[mBar1AnmRev]->start();
                mBar1AnmRevHold = mBar1AnmRev;
            }

            mBar1AnmRev = 0;
        }

        if (pController) {
            update(pController);
        }

        mpLayout->Animate(0);
        mpLayout->CalculateMtx(mDrawInfo);

        if (!mpHBInfo->cursor) {
            for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                mpCursorLayout[i]->CalculateMtx(mDrawInfo);
            }
        }

        if (mForcusSEWaitTime <= 2) {
            mForcusSEWaitTime++;
        }
    }

    void HomeButton::calc_fadeoutAnm() {
        mpLayout->GetRootPane()->FindPaneByName(scFuncTextPaneName[2], true)->SetVisible(false);

        if (mpHBInfo->backFlag) {
            mSelectAnmNum = findGroupAnimator(3, 1);
        } else {
            mSelectAnmNum = findGroupAnimator(1, 1);
        }

        mpGroupAnmController[mSelectAnmNum]->start();
        mState = 16;
        mFadeOutSeTime = mpGroupAnmController[mSelectAnmNum]->getMaxFrame();

        if (mpHBInfo->sound_callback != NULL) {
            mpHBInfo->sound_callback(2, mFadeOutSeTime);
        }
    }

    void HomeButton::calc_battery(int chan) {
        // presumably j because it is the second index
        for (int j = 0; j < (int)ARRAY_SIZE(scBatteryPaneName[chan]); j++) {
            if (j < mWpadInfo[chan].battery) {
                mpLayout->GetRootPane()
                    ->FindPaneByName(scBatteryPaneName[chan][j], true)
                    ->SetVisible(true);
            } else {
                mpLayout->GetRootPane()
                    ->FindPaneByName(scBatteryPaneName[chan][j], true)
                    ->SetVisible(false);
            }
        }

        if (mWpadInfo[chan].battery < 2) {
            int anm_no = findGroupAnimator(chan + 31, 21);
            mpGroupAnmController[anm_no]->start();
        } else {
            int anm_no = findGroupAnimator(chan + 31, 17);
            mpGroupAnmController[anm_no]->start();
        }

        if (mGetPadInfoTime < 100) {
            mGetPadInfoTime = 0;
        }

        getController(chan)->clrBatteryFlag();
    }

    static void SpeakerCallback(OSAlarm* alm, OSContext*) {
        u32 data = (u32)OSGetAlarmUserData(alm);
        int chan = (data >> 16) & 0xFFFF;
        int id = data & 0xFFFF;

        HomeButton* pHBObj = HomeButton::getInstance();

        if (!WPADIsSpeakerEnabled(chan) || !pHBObj->getController(chan)->isPlayReady()) {
            pHBObj->setSpeakerAlarm(chan, 50);
        } else {
            pHBObj->getController(chan)->playSound(pHBObj->GetSoundArchivePlayer(), id);
        }
    }

    static void MotorCallback(OSAlarm* alm, OSContext*) {
        Controller* pController = (Controller*)OSGetAlarmUserData(alm);

        pController->stopMotor();
    }

    void HomeButton::setSpeakerAlarm(int chan, int msec) {
        OSSetAlarmUserData(&mSpeakerAlarm[chan], (void*)((chan << 16) | (chan + 2)));
        OSCancelAlarm(&mSpeakerAlarm[chan]);
        OSSetAlarm(&mSpeakerAlarm[chan], OSMillisecondsToTicks(msec), &SpeakerCallback);
    }

    static void RetrySimpleSyncCallback(OSAlarm* alm, OSContext*) {
        HomeButton* pHBObj = HomeButton::getInstance();
        int type = (int)OSGetAlarmUserData(alm);
        bool retrySuccessFlag = false;

        if (type == 0) {
            if (WPADStartFastSimpleSync()) {
                pHBObj->setSimpleSyncFlag(true);
                retrySuccessFlag = true;
            }
        } else {
            if (WPADStopSimpleSync()) {
                retrySuccessFlag = true;
            }
        }

        if (!retrySuccessFlag) {
            pHBObj->setSimpleSyncAlarm(type);
        }
    }

    static void SimpleSyncCallback(s32 result, s32 num) {
        if (result == 1) {
            HomeButton::getInstance()->setEndSimpleSyncFlag(true);
        }

        HomeButton::getInstance()->callSimpleSyncCallback(result, num);
    }

    void HomeButton::setSimpleSyncAlarm(int type) {
        OSCancelAlarm(&mSimpleSyncAlarm);
        OSSetAlarmUserData(&mSimpleSyncAlarm, (void*)type);
        OSSetAlarm(&mSimpleSyncAlarm, OSMillisecondsToTicks(100), &RetrySimpleSyncCallback);
    }

    void HomeButton::callSimpleSyncCallback(s32 result, s32 num) {
        if (mSimpleSyncCallback) {
            (*mSimpleSyncCallback)(result, num);
        }
    }

    void HomeButton::update(const HBMControllerData* pController) {
#define IsValidDevType_(x)                                                                         \
    ((x)->use_devtype != WPAD_DEV_CLASSIC && (x)->kpad->dev_type != WPAD_DEV_CLASSIC)
#define IsValidDevType2_(x)                                                                        \
    ((x)->use_devtype == WPAD_DEV_CLASSIC && (x)->kpad->dev_type == WPAD_DEV_CLASSIC)

        int i, anm_no;

        mFader.calc();

        for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            if (pController->wiiCon[i].kpad) {
                if (pController->wiiCon[i].kpad->wpad_err != WPAD_ENODEV) {
                    if (mPadDrawTime[i] > 5) {
                        if (pController->wiiCon[i].kpad->wpad_err == WPAD_ESUCCESS) {
                            bool pointerEnableFlag;

                            if (IsValidDevType_(&pController->wiiCon[i])) {
                                if (pController->wiiCon[i].kpad->dpd_valid_fg > 0) {
                                    pointerEnableFlag = true;
                                } else {
                                    pointerEnableFlag = false;
                                }
                            } else {
                                pointerEnableFlag = true;
                            }

                            mpController[i]->setKpad(&pController->wiiCon[i], pointerEnableFlag);

                            if (!mpHBInfo->cursor) {
                                mpCursorLayout[i]
                                    ->GetRootPane()
                                    ->FindPaneByName(scCursorPaneName, true)
                                    ->SetVisible(true);
                            }
                        }
                    } else {
                        mPadDrawTime[i]++;
                    }

                    if (IsValidDevType_(&pController->wiiCon[i]) &&
                        pController->wiiCon[i].kpad->dpd_valid_fg <= 0)
                    {
                        s32 result;
                        u32 type;

                        result = WPADProbe(i, &type);

                        if (pController->wiiCon[i].kpad->wpad_err != WPAD_EBUSY &&
                            result != WPAD_EBUSY)
                        {
                            mpController[i]->setInValidPos();
                        }
                    }
                } else {
                    mpController[i]->setInValidPos();

                    if (!mpHBInfo->cursor) {
                        mpCursorLayout[i]
                            ->GetRootPane()
                            ->FindPaneByName(scCursorPaneName, true)
                            ->SetVisible(false);
                    }
                }

                if (!mControllerFlag[i]) {
                    mControllerFlag[i] = true;

                    getController(i)->getInfoAsync(&mWpadInfo[i]);

                    anm_no = findGroupAnimator(i + 31, 17);
                    mpGroupAnmController[anm_no]->start();

#if HBM_REVISION > 1
                    anm_no = findGroupAnimator(i + 31, 18);
                    mpGroupAnmController[anm_no]->stop();
#endif

                    anm_no = findGroupAnimator(i + 31, 15);
                    mpGroupAnmController[anm_no]->start();

                    play_sound(i + 17);
                    getController(i)->connect();
                    getController(i)->startMotor();

                    OSSetAlarmUserData(&mAlarm[i], getController(i));
                    OSCancelAlarm(&mAlarm[i]);
                    OSSetAlarm(&mAlarm[i], OSMillisecondsToTicks(300), &MotorCallback);

                    setSpeakerAlarm(i, 400);
                }

                if (pController->wiiCon[i].kpad->wpad_err == WPAD_ESUCCESS) {
                    nw4hbm::math::VEC3 vec;

                    if (IsValidDevType2_(&pController->wiiCon[i])) {
                        vec = nw4hbm::math::VEC3(0.0f, 0.0f, 15.0f);
                    } else {
                        Vec2 v = pController->wiiCon[i].kpad->horizon;
                        f32 mRad = nw4hbm::math::Atan2Deg(-v.y, v.x);

                        vec = nw4hbm::math::VEC3(0.0f, 0.0f, mRad);
                    }

                    if (!mpHBInfo->cursor) {
                        mpCursorLayout[i]
                            ->GetRootPane()
                            ->FindPaneByName(scCursorRotPaneName, true)
                            ->SetRotate(vec);

                        mpCursorLayout[i]
                            ->GetRootPane()
                            ->FindPaneByName(scCursorSRotPaneName, true)
                            ->SetRotate(vec);
                    }

                    if (mGetPadInfoTime > 100) {
                        getController(i)->getInfoAsync(&mWpadInfo[i]);
                    }

                    update_controller(i);

                    if (!mpHBInfo->cursor) {
                        update_posController(i);
                    }
                }

                if (getController(i)->getBatteryFlag()) {
                    calc_battery(i);
                }
            } else {
                if (mControllerFlag[i]) {
                    anm_no = findGroupAnimator(i + 31, 17);
                    mpGroupAnmController[anm_no]->start();

                    anm_no = findGroupAnimator(i + 31, 18);
                    mpGroupAnmController[anm_no]->start();

                    for (int j = 0; j < (int)ARRAY_SIZE(scBatteryPaneName[i]); j++) {
                        mpLayout->GetRootPane()
                            ->FindPaneByName(scBatteryPaneName[i][j], true)
                            ->SetVisible(false);
                    }

                    if (!mpHBInfo->cursor) {
                        mpCursorLayout[i]
                            ->GetRootPane()
                            ->FindPaneByName(scCursorPaneName, true)
                            ->SetVisible(false);
                    }

                    mPadDrawTime[i] = 0;
                    mControllerFlag[i] = false;

                    mpController[i]->setInValidPos();
                    mpController[i]->clrKpadButton();
                    mpController[i]->disconnect();
                }

                reset_guiManager(i);
            }
        }

        if (mGetPadInfoTime > 100) {
            mGetPadInfoTime = 0;
        } else {
            mGetPadInfoTime++;
        }
#undef IsValidDevType2_
#undef IsValidDevType_
    }

    void HomeButton::update_controller(int id) {
        int anm_no;

        if (isActive()) {
            HBController* pCon = mpController[id]->getController();

            f32 x = pCon->x * 608.0f / 2.0f;
            f32 y = pCon->y * 456.0f / 2.0f;

            if (mAdjustFlag) {
                x *= mpHBInfo->adjust.x;
                y *= mpHBInfo->adjust.y;
            }

            mpPaneManager->update(id, x, -y, pCon->trig, pCon->hold, pCon->release, pCon);

            //! @bug: probably meant to be the or operator instead of an OR?
            if (((pCon->trig & 0x10000000) | (pCon->trig & WPAD_BUTTON_HOME)) && isActive()) {
                if (mSequence == eSeq_Control) {
                    mpPaneManager->update(id, 0.0f, -180.0f, 0, 0, 0, 0);

#if HBM_REVISION > 1
                    mSelectAnmNum = 4;
                    mpPairGroupAnmController[mSelectAnmNum]->start();
#endif

                    mSelectAnmNum = 2;
                    mpPairGroupAnmController[mSelectAnmNum]->start();
                    int anm_no = 11;  // ?
                    mpPairGroupAnmController[anm_no]->start();

                    mState = 10;
                    mSequence = eSeq_Normal;
                    play_sound(8);
                } else if (mSequence == eSeq_Normal) {
                    if (mpPairGroupAnmController[3]->isPlaying()) {
                        mpPairGroupAnmController[3]->stop();
                    }

                    if (mpPairGroupAnmController[12]->isPlaying()) {
                        mpPairGroupAnmController[12]->stop();
                    }

                    mSelectBtnNum = HBM_SELECT_HOMEBTN;

                    mSelectAnmNum = 4;
                    mpPairGroupAnmController[mSelectAnmNum]->start();

                    mState = 14;
                    play_sound(1);
                }
            } else if (mSequence == eSeq_Control && isActive()) {
                if ((pCon->trig & WPAD_BUTTON_MINUS) || (pCon->trig & 0x10000)) {
                    if (mVolumeNum > 0) {
                        mVolumeNum--;

                        anm_no = findGroupAnimator(mVolumeNum + 21, 10);
                        mpGroupAnmController[anm_no]->stop();

                        anm_no = findGroupAnimator(mVolumeNum + 21, 9);
                        mpGroupAnmController[anm_no]->start();

                        anm_no = findGroupAnimator(11, 5);
                        mpGroupAnmController[anm_no]->start();

                        if (mVolumeNum == 0) {
                            play_sound(12);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        } else {
                            play_sound(10);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        }
                    } else {
                        play_sound(13);
                    }
                } else if ((pCon->trig & WPAD_BUTTON_PLUS) || (pCon->trig & 0x20000)) {
                    if (mVolumeNum < 10) {
                        anm_no = findGroupAnimator(mVolumeNum + 21, 9);
                        mpGroupAnmController[anm_no]->stop();

                        anm_no = findGroupAnimator(mVolumeNum + 21, 10);
                        mpGroupAnmController[anm_no]->start();

                        mVolumeNum++;

                        anm_no = findGroupAnimator(12, 5);
                        mpGroupAnmController[anm_no]->start();

                        if (mVolumeNum == 10) {
                            play_sound(11);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        } else {
                            play_sound(9);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        }
                    } else {
                        play_sound(13);
                    }
                }
            }
        } else if (mSequence == eSeq_Control && mState == 5 &&
                   !mpPairGroupAnmController[mSelectAnmNum]->isPlaying())
        {
            HBController* pCon = mpController[id]->getController();
            if (pCon->trig) {
                mMsgCount = 0xE10;
            }
        }
    }

    void HomeButton::update_posController(int id) {
        HBController* pCon = mpController[id]->getController();
        nw4hbm::ut::Rect layoutRect = mpLayout->GetLayoutRect();

        f32 x = pCon->x * layoutRect.right;
        f32 y = pCon->y * layoutRect.bottom;
        nw4hbm::math::VEC2 pos(x, y);

        mpCursorLayout[id]
            ->GetRootPane()
            ->FindPaneByName(scCursorPaneName, true)
            ->SetTranslate(pos);
    }

    void HomeButton::updateTrigPane() {
        int i;

        switch (mSequence) {
        case eSeq_Normal:
            for (i = 0; i < (int)ARRAY_SIZE(scFuncTouchPaneName); i++) {
                if (i < 2 || i == 9) {
                    nw4hbm::lyt::Pane* pTouchPane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTouchPaneName[i], true);

                    mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(true);
                } else {
                    nw4hbm::lyt::Pane* pTouchPane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTouchPaneName[i], true);

                    mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(false);
                }
            }

            break;

        case eSeq_Control:
            for (i = 0; i < (int)ARRAY_SIZE(scFuncTouchPaneName); i++) {
                if ((i > 0 && i < 7) || i == 9) {
                    nw4hbm::lyt::Pane* pTouchPane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTouchPaneName[i], true);

                    mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(true);
                } else {
                    nw4hbm::lyt::Pane* pTouchPane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTouchPaneName[i], true);

                    mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(false);
                }
            }

            break;

        case eSeq_Cmn:
            for (i = 0; i < (int)ARRAY_SIZE(scFuncTouchPaneName); i++) {
                if (i < 7 || i == 9) {
                    nw4hbm::lyt::Pane* pTouchPane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTouchPaneName[i], true);

                    mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(false);
                } else {
                    nw4hbm::lyt::Pane* pTouchPane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTouchPaneName[i], true);

                    mpPaneManager->getPaneComponentByPane(pTouchPane)->setTriggerTarget(true);
                }
            }

            break;
        }
    }

    void HomeButton::startPointEvent(const char* pPane, void* pData) {
        int anm_no;
        int btn_no = getPaneNo(pPane);
        HBController* pCon = static_cast<HBController*>(pData);
        bool onFlag = false;

        if (isActive() && btn_no != -1 && !mPaneCounter[btn_no]) {
            if (mSequence != eSeq_Cmn && btn_no < mButtonNum) {
                anm_no = findAnimator(btn_no, 0);
                mpAnmController[anm_no]->start();

                if (mSequence == eSeq_Normal) {
                    setForcusSE();
                    onFlag = true;
                }
            } else {
                switch (btn_no - mButtonNum) {
                case 0:
                    if (mSequence == eSeq_Normal) {
                        if (isUpBarActive()) {
                            mpPairGroupAnmController[3]->start();
                            mBar0AnmRevHold = 3;
                            mBar0AnmRev = 0;
                            setForcusSE();
                            onFlag = true;
                        } else {
                            mBar0AnmRev = 3;
                        }
                    }

                    break;

                case 1:
                case 9:
                    if (mSequence == eSeq_Normal) {
                        anm_no = findGroupAnimator(4, 2);

                        if (homebutton::HomeButton::isDownBarActive()) {
                            mpGroupAnmController[anm_no]->start();
                            mBar1AnmRevHold = anm_no;
                            mBar1AnmRev = 0;
                            setForcusSE();
                            onFlag = true;
                        } else {
                            mBar1AnmRev = anm_no;
                        }
                    } else if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(4, 19);

                        if (isDownBarActive()) {
                            mpGroupAnmController[anm_no]->start();
                            mBar1AnmRevHold = anm_no;
                            mBar1AnmRev = 0;
                            setForcusSE();
                            onFlag = true;
                        } else {
                            mBar1AnmRev = anm_no;
                        }
                    }

                    break;

                case 2:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(6, 4);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;

                case 3:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(7, 4);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;

                case 4:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(8, 4);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;

                case 5:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(9, 4);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;

                case 6:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(10, 4);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;

                case 7:
                    if (mSequence == eSeq_Cmn) {
                        anm_no = findGroupAnimator(17, 11);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;

                case 8:
                    if (mSequence == eSeq_Cmn) {
                        anm_no = findGroupAnimator(18, 11);
                        mpGroupAnmController[anm_no]->start();

                        setForcusSE();
                        onFlag = true;
                    }

                    break;
                }
            }
        }

        if (btn_no == mButtonNum + 1 || btn_no == mButtonNum + 9) {
            mPaneCounter[mButtonNum + 1]++;
            mPaneCounter[mButtonNum + 9]++;
        } else {
            mPaneCounter[btn_no]++;
        }

        if (onFlag && pCon) {
            if (!getController(pCon->chan)->isRumbling() && mOnPaneVibWaitFrame[pCon->chan] <= 0.0f)
            {
                mOnPaneVibFrame[pCon->chan] = 3.0f;
                getController(pCon->chan)->startMotor();
            }
        }
    }

    void HomeButton::startLeftEvent(const char* pPane) {
        int anm_no;
        int btn_no = getPaneNo(pPane);

        if (0 < mPaneCounter[btn_no]) {
            if (btn_no == mButtonNum + 1 || btn_no == mButtonNum + 9) {
                mPaneCounter[mButtonNum + 1]--;
                mPaneCounter[mButtonNum + 9]--;
            } else {
                mPaneCounter[btn_no]--;
            }
        }

        if (isActive() && btn_no != -1 && !mPaneCounter[btn_no]) {
            if (mSequence != eSeq_Cmn && btn_no < mButtonNum) {
                anm_no = findAnimator(btn_no, 2);
                mpAnmController[anm_no]->start();
            } else {
                switch (btn_no - mButtonNum) {
                case 0:
                    if (mSequence == eSeq_Normal) {
                        if (isUpBarActive()) {
                            mpPairGroupAnmController[12]->start();
                            mBar0AnmRevHold = 12;
                            mBar0AnmRev = 0;
                        } else {
                            mBar0AnmRev = 12;
                        }
                    }

                    break;

                case 1:
                case 9:
                    if (mSequence == eSeq_Normal) {
                        anm_no = findGroupAnimator(5, 3);

                        if (isDownBarActive()) {
                            mpGroupAnmController[anm_no]->start();
                            mBar1AnmRevHold = anm_no;
                            mBar1AnmRev = 0;
                        } else {
                            mBar1AnmRev = anm_no;
                        }
                    } else if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(5, 20);

                        if (isDownBarActive()) {
                            mpGroupAnmController[anm_no]->start();
                            mBar1AnmRevHold = anm_no;
                            mBar1AnmRev = 0;
                        } else {
                            mBar1AnmRev = anm_no;
                        }
                    }

                    break;

                case 2:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(6, 7);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;

                case 3:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(7, 7);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;

                case 4:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(8, 7);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;

                case 5:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(9, 7);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;

                case 6:
                    if (mSequence == eSeq_Control) {
                        anm_no = findGroupAnimator(10, 7);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;

                case 7:
                    if (mSequence == eSeq_Cmn) {
                        anm_no = findGroupAnimator(17, 12);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;

                case 8:
                    if (mSequence == eSeq_Cmn) {
                        anm_no = findGroupAnimator(18, 12);
                        mpGroupAnmController[anm_no]->start();
                    }

                    break;
                }
            }
        }
    }

    void HomeButton::startTrigEvent(const char* pPane) {
        int anm_no;
        int btn_no;

        btn_no = getPaneNo(pPane);

        if (isActive() && btn_no != -1) {
            if (mSequence == eSeq_Normal && btn_no < mButtonNum) {
                mSelectBtnNum = static_cast<HBMSelectBtnNum>(btn_no + 1);

                mSelectAnmNum = findAnimator(btn_no + 4, 1);
                mpAnmController[mSelectAnmNum]->start();
                play_sound(5);

                if (mDialogFlag[btn_no]) {
                    mState = 11;
                    mSequence = eSeq_Cmn;

                    nw4hbm::lyt::Pane* p_pane =
                        mpLayout->GetRootPane()->FindPaneByName(scFuncTextPaneName[2], true);
                    nw4hbm::lyt::TextBox* p_text =
                        nw4hbm::ut::DynamicCast<nw4hbm::lyt::TextBox*>(p_pane);

                    u16 len;
                    p_text->SetString(mpText[mpHBInfo->region][btn_no + 2], 0);

                    mpLayout->GetRootPane()
                        ->FindPaneByName(scFuncTextPaneName[2], true)
                        ->SetVisible(true);
                } else {
                    mState = 15;
                }
            } else {
                switch (btn_no - mButtonNum) {
                case 0:
                    if (mpPairGroupAnmController[12]->isPlaying()) {
                        mpPairGroupAnmController[12]->stop();
                    }

                    if (mpPairGroupAnmController[3]->isPlaying()) {
                        mpPairGroupAnmController[3]->stop();
                    }

                    mSelectBtnNum = HBM_SELECT_HOMEBTN;
                    mSelectAnmNum = 4;
                    mpPairGroupAnmController[mSelectAnmNum]->start();

                    mState = 14;
                    play_sound(1);

                    break;

                case 1:
                case 9:
                    if (mSequence == eSeq_Control) {
#if HBM_REVISION > 1
                        mSelectAnmNum = 4;
                        mpPairGroupAnmController[mSelectAnmNum]->start();
#endif

                        mSelectAnmNum = 2;
                        mpPairGroupAnmController[mSelectAnmNum]->start();

                        mpPairGroupAnmController[11]->start();

                        mState = 10;
                        mSequence = eSeq_Normal;
                        play_sound(8);
                    } else if (mSequence == eSeq_Normal) {
                        mpPairGroupAnmController[1]->start();

                        mSelectAnmNum = 9;
                        mpPairGroupAnmController[mSelectAnmNum]->start();
                        mState = 10;

                        for (int i = 2; i < 7; i++) {
                            mpLayout->GetRootPane()
                                ->FindPaneByName(scFuncTouchPaneName[i], true)
                                ->SetVisible(true);
                        }

                        mSequence = eSeq_Control;
                        play_sound(5);
                    }

                    break;

                case 2:
                    if (mVolumeNum > 0) {
                        mVolumeNum--;

                        anm_no = findGroupAnimator(mVolumeNum + 21, 10);
                        mpGroupAnmController[anm_no]->stop();

                        anm_no = findGroupAnimator(mVolumeNum + 21, 9);
                        mpGroupAnmController[anm_no]->start();

                        if (mVolumeNum == 0) {
                            play_sound(12);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        } else {
                            play_sound(10);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        }

                        anm_no = findGroupAnimator(11, 5);
                        mpGroupAnmController[anm_no]->start();
                    } else {
                        play_sound(13);
                    }

                    break;

                case 3:
                    if (mVolumeNum < 10) {
                        anm_no = findGroupAnimator(mVolumeNum + 21, 9);
                        mpGroupAnmController[anm_no]->stop();

                        anm_no = findGroupAnimator(mVolumeNum + 21, 10);
                        mpGroupAnmController[anm_no]->start();

                        mVolumeNum++;

                        if (mVolumeNum == 10) {
                            play_sound(11);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        } else {
                            play_sound(9);

                            for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                                getController(i)->setSpeakerVol(mVolumeNum / 10.0f);
                                getController(i)->playSound(mpSoundArchivePlayer, 1);
                            }
                        }

                        anm_no = findGroupAnimator(12, 5);
                        mpGroupAnmController[anm_no]->start();
                    } else {
                        play_sound(13);
                    }

                    break;

                case 4:
                    if (!mVibFlag) {
                        mVibFlag = true;

                        setVibFlag(mVibFlag);

                        anm_no = findGroupAnimator(14, 8);
                        mpGroupAnmController[anm_no]->start();

                        mSelectAnmNum = findGroupAnimator(16, 6);
                        mpGroupAnmController[mSelectAnmNum]->start();

                        play_sound(14);
                    } else {
                        mSelectAnmNum = findGroupAnimator(16, 14);
                        mpGroupAnmController[mSelectAnmNum]->start();

                        play_sound(13);
                    }

                    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                        getController(i)->startMotor();
                        mOnPaneVibFrame[i] = 0.0f;
                        mOnPaneVibWaitFrame[i] = 0.0f;
                    }

                    mState = 9;

                    break;

                case 5:
                    if (mVibFlag) {
                        mVibFlag = false;
                        setVibFlag(mVibFlag);

                        anm_no = findGroupAnimator(14, 6);
                        mpGroupAnmController[anm_no]->start();

                        mSelectAnmNum = findGroupAnimator(13, 8);
                        mpGroupAnmController[mSelectAnmNum]->start();

                        play_sound(15);
                    } else {
                        play_sound(13);
                    }

                    mState = 9;

                    break;

                case 6:
                    mSelectAnmNum = findGroupAnimator(15, 5);
                    mpGroupAnmController[mSelectAnmNum]->start();

                    mState = 3;
                    setReassignedFlag(true);

                    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                        mOnPaneVibFrame[i] = 0.0f;
                        mOnPaneVibWaitFrame[i] = 0.0f;
                        getController(i)->stopMotor();
                    }

                    mWaitStopMotorCount = 30;

                    mpLayout->GetRootPane()
                        ->FindPaneByName(scFuncTextPaneName[0], true)
                        ->SetVisible(true);

                    mpLayout->GetRootPane()
                        ->FindPaneByName(scFuncTextPaneName[1], true)
                        ->SetVisible(true);

                    play_sound(5);
                    play_sound(16);

                    break;

                case 7:
                    mSelectAnmNum = findGroupAnimator(19, 13);
                    mpGroupAnmController[mSelectAnmNum]->start();

                    mState = 13;
                    mSequence = eSeq_Normal;

                    if (mSelectBtnNum == HBM_SELECT_BTN1) {
                        play_sound(2);
                    } else if (mSelectBtnNum == HBM_SELECT_BTN2) {
                        play_sound(3);
                    }

                    break;

                case 8:
                    mSelectAnmNum = findGroupAnimator(20, 13);
                    mpGroupAnmController[mSelectAnmNum]->start();

                    mState = 13;
                    mSelectBtnNum = HBM_SELECT_NULL;

                    mSequence = eSeq_Normal;

                    play_sound(6);

                    break;
                }
            }
        }
    }

    void HomeButton::reset_btn() {
        int anm_no;

        for (int i = 0; i < mButtonNum; i++) {
            if (!mPaneCounter[i]) {
                continue;
            }

            anm_no = findAnimator(i, 2);
            mpAnmController[anm_no]->start();
            mPaneCounter[i] = 0;
        }

        if (mPaneCounter[mButtonNum]) {
            mpPairGroupAnmController[12]->start();
            mPaneCounter[mButtonNum + 1] = 0;
        }

        if (mPaneCounter[mButtonNum + 1]) {
            if (mSequence == eSeq_Control) {
                if (!mMsgCount) {
                    anm_no = findGroupAnimator(5, 20);
                    mpGroupAnmController[anm_no]->start();
                    mPaneCounter[mButtonNum] = 0;
                }
            } else {
                anm_no = findGroupAnimator(5, 3);
                mpGroupAnmController[anm_no]->start();
                mPaneCounter[mButtonNum] = 0;
            }
        }
    }

    void HomeButton::reset_control() {
        int anm_no;

        for (int i = 0; i < 5; i++) {
            anm_no = findGroupAnimator(i + 6, 7);
            mpGroupAnmController[anm_no]->start();
        }
    }

    void HomeButton::reset_window() {
        int anm_no;

        anm_no = findGroupAnimator(17, 12);
        mpGroupAnmController[anm_no]->start();

        anm_no = findGroupAnimator(18, 12);
        mpGroupAnmController[anm_no]->start();
    }

    void HomeButton::reset_battery() {
        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            for (int j = 0; j < 4; j++) {
                mpLayout->GetRootPane()
                    ->FindPaneByName(scBatteryPaneName[i][j], true)
                    ->SetVisible(false);
            }
        }
    }

    void HomeButton::reset_guiManager(int num) {
        if (num < 0)  // presumably a -1 = all case
        {
            for (int i = 0; i < 8; i++) {
                mpPaneManager->update(i, -10000.0f, -10000.0f, 0, 0, 0, NULL);
            }
        } else {
            mpPaneManager->update(num, -10000.0f, -10000.0f, 0, 0, 0, NULL);
        }
    }

    bool HomeButton::isActive() const {
        return mState == 2;
    }

    bool HomeButton::isUpBarActive() const {
        bool flag = true;

        if (!isActive() || mpPairGroupAnmController[12]->isPlaying() ||
            mpPairGroupAnmController[3]->isPlaying())
        {
            flag = false;
        }

        return flag;
    }

    bool HomeButton::isDownBarActive() {
        bool flag = true;

        int anm_no[4];
        anm_no[0] = findGroupAnimator(4, 2);
        anm_no[1] = findGroupAnimator(5, 3);
        anm_no[2] = findGroupAnimator(4, 19);
        anm_no[3] = findGroupAnimator(5, 20);

        if (!isActive() || mpGroupAnmController[anm_no[0]]->isPlaying() ||
            mpGroupAnmController[anm_no[1]]->isPlaying() ||
            mpGroupAnmController[anm_no[2]]->isPlaying() ||
            mpGroupAnmController[anm_no[3]]->isPlaying())
        {
            flag = false;
        }

        return flag;
    }

    int HomeButton::getPaneNo(const char* panename) {
        int buttonNum = mButtonNum;
        int ret = -1;

        for (int i = 0; i < buttonNum; i++) {
            if (!std::strcmp(panename, getPaneName(i))) {
                ret = i;
                break;
            }
        }

        for (int i = 0; i < 10; i++) {
            if (!std::strcmp(panename, getFuncPaneName(i))) {
                ret = i + buttonNum;
                break;
            }
        }

        return ret;
    }

    int HomeButton::findAnimator(int pane, int anm) {
        for (int i = 0; i < mAnmNum; i++) {
            if (scAnmTable[i].pane == pane && scAnmTable[i].anm == anm) {
                return i;
            }
        }

        return -1;
    }

    void HomeButton::setForcusSE() {
        if (mForcusSEWaitTime <= 2) {
            return;
        }

        play_sound(4);
        mForcusSEWaitTime = 0;
    }

    void HomeButton::setAdjustFlag(int flag) {
        nw4hbm::math::VEC2 sc_v;

        mAdjustFlag = static_cast<bool>(flag);
        mDrawInfo.SetLocationAdjust(mAdjustFlag);

        if (mAdjustFlag) {
            sc_v = nw4hbm::math::VEC2(mpHBInfo->adjust.x, mpHBInfo->adjust.y);
            mpLayout->GetRootPane()->SetScale(sc_v);

            if (!mpHBInfo->cursor) {
                for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                    mpCursorLayout[i]->GetRootPane()->SetScale(sc_v);
                }
            }
        } else {
            sc_v = nw4hbm::math::VEC2(1.0f, 1.0f);
            mpLayout->GetRootPane()->SetScale(sc_v);

            if (!mpHBInfo->cursor) {
                for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
                    mpCursorLayout[i]->GetRootPane()->SetScale(sc_v);
                }
            }
        }

        GXSetCullMode(GX_CULL_NONE);
    }

    void HomeButton::setVolume(int vol) {
        WPADSetSpeakerVolume(vol * 12.7f);
    }

    int HomeButton::getVolume() {
        return WPADGetSpeakerVolume() * (1.0f / 12.7f) + 0.9f;
    }

    void HomeButton::setVibFlag(bool flag) {
        WPADEnableMotor(flag);
    }

    bool HomeButton::getVibFlag() {
        return WPADIsMotorEnabled() ? true : false;
    }

    void HomeButtonEventHandler::onEvent(u32 uID, u32 uEvent, void* pData) {
        gui::PaneComponent* p_panecpt = (gui::PaneComponent*)mpManager->getComponent(uID);
        const char* panename = p_panecpt->getPane()->GetName();

        HomeButton* p_hbtn = getHomeButton();

        HBController* pCon = static_cast<HBController*>(pData);

        switch (uEvent) {
        case 1:
            p_hbtn->startPointEvent(panename, pData);
            break;

        case 2:
            p_hbtn->startLeftEvent(panename);
            break;

        case 0:
            if ((pCon->trig & WPAD_BUTTON_A) || (pCon->trig & 0x1000000)) {
                p_hbtn->startTrigEvent(panename);
            }
            break;
        }
    }

    void HomeButton::startBlackOut() {
        mForceSttInitProcFlag = false;
        mForceSttFadeInProcFlag = false;
        mForceStopSyncFlag = false;
        mForceEndMsgAnmFlag = false;

        switch (mState) {
        case 0:
            mForceSttInitProcFlag = true;

        case 1:
            mForceSttFadeInProcFlag = true;
            break;

        case 3:
        case 5:
        case 6:
        case 7:
            if ((mState == 3 && mSelectAnmNum == 5) || (mState == 5 && !mSimpleSyncFlag)) {
                OSCancelAlarm(&mSimpleSyncAlarm);
                WPADSetSimpleSyncCallback(mSimpleSyncCallback);
                mSimpleSyncCallback = NULL;
            } else if (!mEndSimpleSyncFlag && mState > 3) {
                mForceStopSyncFlag = true;

                if (!WPADStopSimpleSync()) {
                    OSCancelAlarm(&mSimpleSyncAlarm);

                    OSSetAlarmUserData(&mSimpleSyncAlarm, (void*)1);
                    OSSetAlarm(&mSimpleSyncAlarm, OSMillisecondsToTicks(100),
                               &RetrySimpleSyncCallback);
                }
            } else {
                WPADSetSimpleSyncCallback(mSimpleSyncCallback);
                mSimpleSyncCallback = NULL;
            }

            mForceEndMsgAnmFlag = true;
            break;
        }

        mState = 19;
        mFader.start();

        mSelectBtnNum = HBM_SELECT_BTN2;

        f32 maxFrame = mFader.getMaxFrame();
        mFadeOutSeTime = maxFrame;

        if (mpHBInfo->sound_callback != NULL) {
            mpHBInfo->sound_callback(3, maxFrame);
        }
    }

    static void drawBlackPlate(f32 left, f32 top, f32 right, f32 bottom) {
#if HBM_REVISION == 1
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(left, top);
        GXPosition2f32(right, top);
        GXPosition2f32(right, bottom);
        GXPosition2f32(left, bottom);
        GXEnd();
#else
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(left, top);
        GXPosition2f32(left, bottom);
        GXPosition2f32(right, bottom);
        GXPosition2f32(right, top);
        GXEnd();
#endif
    }

    static void initgx() {
        Mtx view_mtx;

        PSMTXIdentity(view_mtx);
        GXLoadPosMtxImm(view_mtx, 0);
        GXSetCurrentMtx(0);

        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_F32, 0);

        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);

        GXSetNumTexGens(0);

        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);

        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetAlphaUpdate(false);
        GXSetZMode(false, GX_ALWAYS, false);

#if HBM_REVISION > 1
        GXSetNumIndStages(0);
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
#endif
    }

    void HomeButton::createSound(nw4hbm::snd::NandSoundArchive* pNandSoundArchive,
                                 bool bCreateSoundHeap) {
        void* buffer =
            MEMAllocFromAllocator(&sSoundAllocator, sizeof(nw4hbm::snd::SoundArchivePlayer));
        if (buffer != NULL) {
            mpSoundArchivePlayer = new (buffer) nw4hbm::snd::SoundArchivePlayer();
        }
        NW4HBM_ASSERT(LN(3752, 3695), mpSoundArchivePlayer);

        void* memBuffer;
        void* strmBuffer;
        u32 memSize = mpSoundArchivePlayer->GetRequiredMemSize(pNandSoundArchive);
        u32 strmSize = mpSoundArchivePlayer->GetRequiredStrmBufferSize(pNandSoundArchive);
        strmBuffer = MEMAllocFromAllocator(&sSoundAllocator, strmSize);
        memBuffer = MEMAllocFromAllocator(&sSoundAllocator, memSize);
        bool result = mpSoundArchivePlayer->Setup(pNandSoundArchive, memBuffer, memSize, strmBuffer,
                                                  strmSize);
        NW4HBM_ASSERT(LN(3770, 3713), result);

        buffer = MEMAllocFromAllocator(&sSoundAllocator, sizeof(nw4hbm::snd::SoundHandle));
        if (buffer != NULL) {
            mpSoundHandle = new (buffer) nw4hbm::snd::SoundHandle();
        }
        NW4HBM_ASSERT(LN(3777, 3720), mpSoundHandle);

        if (bCreateSoundHeap) {
            buffer = MEMAllocFromAllocator(&sSoundAllocator, sizeof(nw4hbm::snd::SoundHeap));
            if (buffer != NULL) {
                mpSoundHeap = new (buffer) nw4hbm::snd::SoundHeap();
            }
            NW4HBM_ASSERT(LN(3786, 3729), mpSoundHeap);

            u32 size = mButtonNum == 2 ? 0x60000 : 0x6F800;
            mpSoundHeap->Create(MEMAllocFromAllocator(&sSoundAllocator, size), size);
            NW4HBM_ASSERT(LN(3794, 3737), mpSoundHeap->IsValid());

            bool result = mpSoundArchivePlayer->LoadGroup(0, mpSoundHeap, 0);
            NW4HBM_ASSERT(LN(3797, 3740), result);
        } else {
            mpSoundHeap = NULL;
        }
    }

    void HomeButton::deleteSound() {
        if (mpDvdSoundArchive != NULL) {
            mpDvdSoundArchive->Close();
            mpDvdSoundArchive->~DvdSoundArchive();
        }

        if (mpMemorySoundArchive != NULL) {
            mpMemorySoundArchive->Shutdown();
            mpMemorySoundArchive->~MemorySoundArchive();
        }

        if (mpNandSoundArchive != NULL) {
            mpNandSoundArchive->Close();
            mpNandSoundArchive->~NandSoundArchive();
        }

        if (mpSoundHeap != NULL) {
            mpSoundHeap->Destroy();
            mpSoundHeap->~SoundHeap();
        }

        if (mpSoundArchivePlayer != NULL) {
            mpSoundArchivePlayer->Shutdown();
            mpSoundArchivePlayer->~SoundArchivePlayer();
        }

        if (mpSoundHandle != NULL) {
            mpSoundHandle->~SoundHandle();
        }

        nw4hbm::snd::SoundSystem::ShutdownSoundSystem();
    }

}  // namespace homebutton
