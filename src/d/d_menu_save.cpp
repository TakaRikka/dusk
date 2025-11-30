#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_save.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "stdio.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_select_cursor.h"
#include "d/d_file_sel_info.h"
#include "d/d_file_sel_warning.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_MemCard.h"
#include "m_Do/m_Do_MemCardRWmng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_msg_scrn_explain.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "f_op/f_op_msg_mng.h"

static int SelStartFrameTbl[3] = {
    59,
    99,
    139,
};

static int SelEndFrameTbl[3] = {
    69,
    109,
    149,
};

static int SelOpenStartFrameTbl[3] = {
    249,
    448,
    648,
};

static int SelOpenEndFrameTbl[3] = {
    259,
    460,
    659,
};

static int YnSelStartFrameTbl[2][2] = {
    {2150, 3181},
    {2139, 3171},
};

static int YnSelEndFrameTbl[2][2] = {{2138, 3171}, {2150, 3181}};

static dMs_HIO_c g_msHIO;

dMs_HIO_c::dMs_HIO_c() {
    mDisplayWaitFrames = 15;
    mCardWaitFrames = 90;
    mEffectDispFrames = 5;
    mCharSwitchFrames = 5;
    mSelectIcon = 5;
    mSelectFrames = 5;
    mFadeOutTimer = 15;
    mTitleMsgCheck = false;
    field_0xd = 0;
    mErrorMsgCheck = false;
    field_0xf = 0;
}

dMenu_save_c::dMenu_save_c() {
    mpScrnExplain = NULL;
    mWarning = NULL;
    mSelIcon = NULL;

    for (int i = 0; i < 3; i++) {
        mpSelWakuMoyo[i] = NULL;
        mpSelWakuGold[i] = NULL;
        mpSelWakuGold2[i] = NULL;
        mpBookWaku[i] = NULL;
        mpSelData[i] = NULL;
        mFileInfo[i] = NULL;
    }
    mpSelectMoveBase = NULL;

    for (int i = 0; i < 2; i++) {
        mpNoYes[i] = NULL;
        mNoYesBase[i] = NULL;
        mNoYesGold[i] = NULL;
        mNoYesGold2[i] = NULL;
        mpHeaderTxtPane[i] = NULL;
        mpErrTxtPane[i] = NULL;
        mpNoYesTxt[i] = NULL;
    }

    field_0x40 = NULL;
    field_0x44 = NULL;
    field_0x48 = NULL;
    mpFileWakuAnm = NULL;
    mpFileWakuRotAnm = NULL;
    field_0x150 = NULL;
    field_0x158 = NULL;
    field_0x160 = NULL;
    field_0x4c = NULL;
    mpBBtnIcon = NULL;
    mpABtnIcon = NULL;
    mpBackTxt = NULL;
    mpConfirmTxt = NULL;
    mpArchive = NULL;

    mDisplayMenu = false;
    mpMount = NULL;
    field_0x21a2 = 0;
    field_0x21a1 = 0;
}

void dMenu_save_c::_create() {
    stick = new STControl(2, 2, 1, 1, 0.9f, 0.5f, 0, 0x2000);

    if (mUseType == TYPE_DEFAULT || mUseType == TYPE_WHITE_EVENT || mUseType == TYPE_BLACK_EVENT) {
        mpMount =
            mDoDvdThd_mountArchive_c::create("/res/Layout/saveres.arc", 0, mDoExt_getJ2dHeap());
    }

    if (mUseType == 2 || mUseType == TYPE_WHITE_EVENT || mUseType == TYPE_BLACK_EVENT) {
        u8 var_r31;
        switch (mUseType) {
        case 2:
            var_r31 = 2;
            break;
        case TYPE_BLACK_EVENT:
            var_r31 = 3;
            break;
        default:
        case TYPE_WHITE_EVENT:
            var_r31 = 1;
            break;
        }

        mpScrnExplain = new dMsgScrnExplain_c(stick, var_r31, false, 0);
        field_0x219d = 0;
        field_0x2190 = 0;
        field_0x2192 = 0;
        field_0x2194 = 0;
        mMenuSaveExplain.setScrnExplain(mpScrnExplain);
    }

    displayInit();
    g_msHIO.field_0x4 = -1;
}

void dMenu_save_c::screenSet() {
    static u64 l_tagName0[3] = {'w_sel_00', 'w_sel_01', 'w_sel_02'};
    static u64 l_tagName000[2] = {'w_no_t', 'w_yes_t'};
    static u64 l_tagName000U[2] = {'f_no_t', 'f_yes_t'};
    static u64 l_tagName00[2] = {'w_modo', 'w_kete'};
    static u64 l_tagName00U[2] = {'f_modo', 'f_kete'};
    static u64 l_tagName3[3] = {'w_moyo00', 'w_moyo01', 'w_moyo02'};
    static u64 l_tagName4[3] = {'w_gold00', 'w_gold01', 'w_gold02'};
    static u64 l_tagName5[3] = {'w_go2_00', 'w_go2_01', 'w_go2_02'};
    static u64 l_tagName9[2] = {'w_no_m', 'w_yes_m'};
    static u64 l_tagName10[2] = {'w_no_g', 'w_yes_g'};
    static u64 l_tagName11[2] = {'w_no_gr', 'w_yes_gr'};
    static u64 l_tagName12[3] = {'w_bk_l00', 'w_bk_l01', 'w_bk_l02'};
#if VERSION == VERSION_GCN_JPN
    static u64 l_tagName21[2] = {'w_tabi_s', 'w_tabi_x'};
    static u64 l_tagName20[2] = {'w_er_msg', 'w_er_msR'};
#else
    static u64 l_tagName21[2] = {'t_for', 't_for1'};
    static u64 l_tagName211[10] = {'tmoyou00', 'tmoyou01', 'tmoyou02', 'tmoyou03', 'tmoyou04',
                                'tmoyou05', 'tmoyou06', 'tmoyou07', 'tmoyou08', 'tmoyou09'};
    static u64 l_tagName20[2] = {'er_for0', 'er_for1'};
#endif
    static u64 l_tagName13[3] = {'w_dat_i0', 'w_dat_i1', 'w_dat_i2'};
    static u8 l_msgNum0[2] = {0x08, 0x07};
    static u8 l_msgNum[2] = {0x54, 0x55};

    mSaveSel.Scr = new J2DScreen();
    mSaveSel.Scr->setPriority("zelda_file_select2.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(mSaveSel.Scr);

    mSaveSel.font[0] = mDoExt_getMesgFont();
    mSaveSel.font[1] = mDoExt_getSubFont();
    mSaveSel.mMsgString = new dMsgString_c();

    void* bck = JKRGetNameResource("zelda_file_select2.bck", mpArchive);
    field_0x40 = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(bck);
    field_0x44 = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(bck);
    field_0x48 = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(bck);
    field_0x4c = (J2DAnmTransformKey*)J2DAnmLoaderDataBase::load(bck);
    field_0x40->searchUpdateMaterialID(mSaveSel.Scr);
    field_0x44->searchUpdateMaterialID(mSaveSel.Scr);
    field_0x48->searchUpdateMaterialID(mSaveSel.Scr);
    field_0x4c->searchUpdateMaterialID(mSaveSel.Scr);

    mpSelectMoveBase = new CPaneMgr(mSaveSel.Scr, 'w_move_n', 0, NULL);
    field_0xa0 = mSaveSel.Scr->search('wmenu_n');

    for (int i = 0; i < 3; i++) {
        mpSelData[i] = new CPaneMgr(mSaveSel.Scr, l_tagName0[i], 0, NULL);
    }

    mpNoYes[0] = new CPaneMgr(mSaveSel.Scr, 'w_no_n', 0, NULL);
    mpNoYes[1] = new CPaneMgr(mSaveSel.Scr, 'w_yes_n', 0, NULL);

    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        mpNoYesTxt[i] = new CPaneMgr(mSaveSel.Scr, l_tagName000[i], 0, NULL);
        mSaveSel.Scr->search(l_tagName000U[i])->hide();
#else
        mpNoYesTxt[i] = new CPaneMgr(mSaveSel.Scr, l_tagName000U[i], 0, NULL);
        mSaveSel.Scr->search(l_tagName000[i])->hide();
#endif
        ((J2DTextBox*)mpNoYesTxt[i]->getPanePtr())->setFont(mSaveSel.font[0]);

        char message[32];
        fopMsgM_messageGet(message, l_msgNum0[i]);
        ((J2DTextBox*)mpNoYesTxt[i]->getPanePtr())->setString(message);
    }

    mSaveSel.Scr->search('Wi_btn_n')->hide();
    mSaveSel.Scr->search('GC_btn_n')->show();

    mpBBtnIcon = new CPaneMgrAlpha(mSaveSel.Scr, 'w_nbbtn', 2, NULL);
    mpABtnIcon = new CPaneMgrAlpha(mSaveSel.Scr, 'w_nabtn', 2, NULL);
#if VERSION == VERSION_GCN_JPN
    mpBackTxt = new CPaneMgrAlpha(mSaveSel.Scr, 'w_modo', 2, NULL);
    mpConfirmTxt = new CPaneMgrAlpha(mSaveSel.Scr, 'w_kete', 2, NULL);
#else
    mpBackTxt = new CPaneMgrAlpha(mSaveSel.Scr, 'f_modo', 2, NULL);
    mpConfirmTxt = new CPaneMgrAlpha(mSaveSel.Scr, 'f_kete', 2, NULL);
#endif

    mpBackTxt->setAlpha(0);
    mpConfirmTxt->setAlpha(0);
    mpBBtnIcon->setAlpha(128);
    mpABtnIcon->setAlpha(128);

    for (int i = 0; i < 2; i++) {
        J2DTextBox* tbox[2];
#if VERSION == VERSION_GCN_JPN
        tbox[i] = (J2DTextBox*)mSaveSel.Scr->search(l_tagName00[i]);
        mSaveSel.Scr->search(l_tagName00U[i])->hide();
#else
        tbox[i] = (J2DTextBox*)mSaveSel.Scr->search(l_tagName00U[i]);
        mSaveSel.Scr->search(l_tagName00[i])->hide();
#endif
        tbox[i]->setFont(mSaveSel.font[0]);

        char message[32];
        fopMsgM_messageGet(message, l_msgNum[i]);
        tbox[i]->setString(message);
    }

    const void* bpk = JKRFileLoader::getGlbResource("zelda_file_select2.bpk", mpArchive);
    mpFileWakuAnm = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(bpk);
    mpFileWakuAnm->searchUpdateMaterialID(mSaveSel.Scr);
    mFileWakuAnmFrame = 0;

    const void* btk = JKRFileLoader::getGlbResource("zelda_file_select2_02.btk", mpArchive);
    mpFileWakuRotAnm = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mpFileWakuRotAnm->searchUpdateMaterialID(mSaveSel.Scr);
    mFileWakuRotAnmFrame = 0;

    for (int i = 0; i < 3; i++) {
        mpSelWakuMoyo[i] = new CPaneMgr(mSaveSel.Scr, l_tagName3[i], 0, NULL);
        mpSelWakuGold[i] = new CPaneMgr(mSaveSel.Scr, l_tagName4[i], 0, NULL);
        mpSelWakuGold2[i] = new CPaneMgr(mSaveSel.Scr, l_tagName5[i], 0, NULL);

        mpSelWakuMoyo[i]->getPanePtr()->setAnimation(mpFileWakuAnm);
        mpSelWakuGold[i]->getPanePtr()->setAnimation(mpFileWakuAnm);
        mpSelWakuGold2[i]->getPanePtr()->setAnimation(mpFileWakuAnm);
        mpSelWakuGold[i]->getPanePtr()->setAnimation(mpFileWakuRotAnm);
        mpSelWakuGold2[i]->getPanePtr()->setAnimation(mpFileWakuRotAnm);
    }

    for (int i = 0; i < 2; i++) {
        mNoYesBase[i] = new CPaneMgr(mSaveSel.Scr, l_tagName9[i], 0, NULL);
        mNoYesGold[i] = new CPaneMgr(mSaveSel.Scr, l_tagName10[i], 0, NULL);
        mNoYesGold2[i] = new CPaneMgr(mSaveSel.Scr, l_tagName11[i], 0, NULL);

        mNoYesBase[i]->getPanePtr()->setAnimation(mpFileWakuAnm);
        mNoYesGold[i]->getPanePtr()->setAnimation(mpFileWakuAnm);
        mNoYesGold2[i]->getPanePtr()->setAnimation(mpFileWakuAnm);
        mNoYesGold[i]->getPanePtr()->setAnimation(mpFileWakuRotAnm);
        mNoYesGold2[i]->getPanePtr()->setAnimation(mpFileWakuRotAnm);
        mNoYesBase[i]->setAlpha(0);
        mNoYesGold[i]->setAlpha(0);
        mNoYesGold2[i]->setAlpha(0);
    }

    field_0x150 = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(bpk);
    field_0x150->searchUpdateMaterialID(mSaveSel.Scr);

    const void* btk2 = JKRFileLoader::getGlbResource("zelda_file_select2.btk", mpArchive);
    field_0x158 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk2);
    field_0x158->searchUpdateMaterialID(mSaveSel.Scr);

    const void* brk = JKRFileLoader::getGlbResource("zelda_file_select2.brk", mpArchive);
    field_0x160 = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    field_0x160->searchUpdateMaterialID(mSaveSel.Scr);

    field_0x154 = 0;
    field_0x15c = 0;
    field_0x164 = 0;

    for (int i = 0; i < 3; i++) {
        mpBookWaku[i] = new CPaneMgr(mSaveSel.Scr, l_tagName12[i], 0, NULL);

        mpBookWaku[i]->getPanePtr()->setAnimation(field_0x150);
        mpBookWaku[i]->getPanePtr()->setAnimation(field_0x158);
        mpBookWaku[i]->getPanePtr()->setAnimation(field_0x160);
        mpBookWaku[i]->setAlpha(0);
    }

#if VERSION == VERSION_GCN_JPN
    mSaveSel.Scr->search('t_for')->hide();
    mSaveSel.Scr->search('t_for1')->hide();
#else
    mSaveSel.Scr->search('w_tabi_s')->hide();
    mSaveSel.Scr->search('w_tabi_x')->hide();

    for (int i = 0; i < 10; i++) {
        mSaveSel.Scr->search(l_tagName211[i])->hide();
    }
#endif

    for (int i = 0; i < 2; i++) {
        mpHeaderTxtPane[i] = new CPaneMgrAlpha(mSaveSel.Scr, l_tagName21[i], 0, NULL);

        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setFont(mSaveSel.font[0]);
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setString(0x100, "");
#if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setFontSize(21.0f, 21.0f);
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setLineSpace(22.0f);
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setCharSpace(2.0f);
#else
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setFontSize(19.0f, 19.0f);
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setLineSpace(20.0f);
        ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->setCharSpace(0.0f);
#endif
        mpHeaderTxt[i] = ((J2DTextBox*)mpHeaderTxtPane[i]->getPanePtr())->getStringPtr();
    }

    mpHeaderTxtPane[0]->setAlpha(255);
    mpHeaderTxtPane[1]->setAlpha(0);
    mHeaderTxtType = 0;

    field_0xb4 = mSaveSel.Scr->search('w_er_n');
#if VERSION == VERSION_GCN_JPN
    mSaveSel.Scr->search('er_for0')->hide();
    mSaveSel.Scr->search('er_for1')->hide();
#else
    mSaveSel.Scr->search('w_er_msg')->hide();
    mSaveSel.Scr->search('w_er_msR')->hide();
#endif

    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        mpErrTxtPane[i] = new CPaneMgrAlpha(mSaveSel.Scr, l_tagName20[i], 0, NULL);
#else
        mpErrTxtPane[i] = new CPaneMgrAlpha(mSaveSel.Scr, l_tagName20[i], 0, NULL);
#endif

        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setFont(mSaveSel.font[0]);
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setString(0x200, "");
#if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setFontSize(21.0f, 21.0f);
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setLineSpace(22.0f);
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setCharSpace(2.0f);
#else
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->resize(440.0f, 198.0f);
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setFontSize(21.0f, 21.0f);
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setLineSpace(21.0f);
        ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->setCharSpace(1.0f);
#endif
        mpErrTxt[i] = ((J2DTextBox*)mpErrTxtPane[i]->getPanePtr())->getStringPtr();
        mpErrTxtPane[i]->show();
    }

    mErrTxtType = 0;
    mpErrTxtPane[mErrTxtType]->setAlpha(255);
    mpErrTxtPane[mErrTxtType ^ 1]->setAlpha(0);

    mSelectedFile = dComIfGs_getDataNum();
    mSelIcon = new dSelect_cursor_c(0, 1.0f, NULL);
    mSelIcon->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);

    Vec pos;
    pos = mpSelData[mSelectedFile]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mpSelData[mSelectedFile]->getPanePtr(), true);
    mSelIcon->setAlphaRate(0.0f);

    J2DPane* pane = mSaveSel.Scr->search('Nm_02');
    pane->setAnimation(field_0x40);
    field_0x64 = 0;
    field_0x50 = 1;
    field_0x40->setFrame(field_0x50);
    pane->animationTransform();
    pane->setAnimation((J2DAnmTransformKey*)NULL);

    mpHeaderTxtPane[0]->getPanePtr()->scale(1.0f, 1.0f);
    mpHeaderTxtPane[1]->getPanePtr()->scale(1.0f, 1.0f);

    for (int i = 0; i < 3; i++) {
        mFileInfo[i] = new dFile_info_c(mpArchive, 1);
        mFileInfo[i]->setBasePane(mSaveSel.Scr->search(l_tagName13[i]));
        mpDataBase[i] = mFileInfo[i]->getDatBase();
        mpNoDataBase[i] = mFileInfo[i]->getNoDatBase();
    }

    mWarning = new dFile_warning_c(mpArchive, 0);
}


void dMenu_save_c::initialize() {
    displayInit();
}

void dMenu_save_c::displayInit() {
    field_0x1bf = 1;
    field_0x1c0 = 0;
    field_0x64 = 0;
    field_0xd2 = 0;
    field_0x9c = 0;
    field_0x17a = 0;
    field_0x65 = 0;
    field_0x9d = 0;
    field_0xd3 = 0;
    field_0x19c = 0;
    field_0x19d = 0;
    field_0x1a8 = 0;
    field_0x1a9 = 0;
    mMenuProc = PROC_SAVE_WAIT;
    mSaveStatus = 1;
    mEndStatus = 2;
}

bool dMenu_save_c::_open() {
    field_0x1bf = 1;

    if (mUseType == TYPE_DEFAULT || mUseType == TYPE_WHITE_EVENT || mUseType == TYPE_BLACK_EVENT) {
        if (!mpMount->sync()) {
            return false;
        }

        if (mpArchive == NULL) {
            mpArchive = mpMount->getArchive();
            screenSet();

            if (mUseType == TYPE_DEFAULT) {
                mDisplayMenu = true;
                field_0x1bf = 0;
            }

            g_mDoMemCd_control.command_attach();
            mpMount->destroy();
            mpMount = NULL;
        }
    }

    mYesNoCursor = CURSOR_NO;
    field_0x2190 = 1;

    switch (mUseType) {
    case TYPE_DEFAULT:
        mMenuProc = PROC_MEMCARD_CHECK;
        break;
    case TYPE_WHITE_EVENT:
    case TYPE_BLACK_EVENT:
        mYesNoCursor = CURSOR_YES;
        msgTxtSet(0x3CE, true);  // "Do you want to save?"
        mMenuProc = PROC_SAVE_QUESTION;
        break;
    case 2:
        mMenuProc = PROC_RETRY_QUESTION0;
        break;
    }

    mSaveStatus = 2;

    if (dMeter2Info_getMiniGameItemSetFlag() != 0) {
        dMeter2Info_resetMiniGameItem(true);
    }

    return true;
}

int dMenu_save_c::_close() {
    mMenuProc = PROC_SAVE_WAIT;
    mSaveStatus = 0;
    return 1;
}

void dMenu_save_c::_delete() {
    delete stick;

    if (mpScrnExplain != NULL) {
        delete mpScrnExplain;
        mpScrnExplain = NULL;
    }

    if (mSaveSel.mMsgString != NULL) {
        delete mSaveSel.mMsgString;
    }

    if (mSaveSel.Scr != NULL) {
        delete mSaveSel.Scr;
    }

    for (int i = 0; i < 3; i++) {
        if (mpSelWakuMoyo[i] != NULL) {
            delete mpSelWakuMoyo[i];
        }

        if (mpSelWakuGold[i] != NULL) {
            delete mpSelWakuGold[i];
        }

        if (mpSelWakuGold2[i] != NULL) {
            delete mpSelWakuGold2[i];
        }

        if (mpBookWaku[i] != NULL) {
            delete mpBookWaku[i];
        }

        if (mpSelData[i] != NULL) {
            delete mpSelData[i];
        }

        if (mFileInfo[i] != NULL) {
            delete mFileInfo[i];
        }
    }

    if (mpSelectMoveBase != NULL) {
        delete mpSelectMoveBase;
    }

    for (int i = 0; i < 2; i++) {
        if (mpNoYes[i] != NULL) {
            delete mpNoYes[i];
        }

        if (mNoYesBase[i] != NULL) {
            delete mNoYesBase[i];
        }

        if (mNoYesGold[i] != NULL) {
            delete mNoYesGold[i];
        }

        if (mNoYesGold2[i] != NULL) {
            delete mNoYesGold2[i];
        }

        if (mpHeaderTxtPane[i] != NULL) {
            delete mpHeaderTxtPane[i];
        }

        if (mpErrTxtPane[i] != NULL) {
            delete mpErrTxtPane[i];
        }

        if (mpNoYesTxt[i] != NULL) {
            delete mpNoYesTxt[i];
        }
    }

    if (field_0x40 != NULL) {
        delete field_0x40;
    }

    if (field_0x44 != NULL) {
        delete field_0x44;
    }

    if (field_0x48 != NULL) {
        delete field_0x48;
    }

    if (mpFileWakuAnm != NULL) {
        delete mpFileWakuAnm;
    }

    if (mpFileWakuRotAnm != NULL) {
        delete mpFileWakuRotAnm;
    }

    if (field_0x150 != NULL) {
        delete field_0x150;
    }

    if (field_0x158 != NULL) {
        delete field_0x158;
    }

    if (field_0x160 != NULL) {
        delete field_0x160;
    }

    if (field_0x4c != NULL) {
        delete field_0x4c;
    }

    if (mpBBtnIcon != NULL) {
        delete mpBBtnIcon;
    }

    if (mpABtnIcon != NULL) {
        delete mpABtnIcon;
    }

    if (mpBackTxt != NULL) {
        delete mpBackTxt;
    }

    if (mpConfirmTxt != NULL) {
        delete mpConfirmTxt;
    }

    if (mSelIcon != NULL) {
        delete mSelIcon;
    }

    if (mWarning != NULL) {
        delete mWarning;
    }

    mDoExt_removeMesgFont();
    mDoExt_removeSubFont();

    if (mpArchive != NULL) {
        mpArchive->removeResourceAll();
        mpArchive->unmount();
    }
}

typedef void (dMenu_save_c::*menuProcFunc)();
menuProcFunc MenuSaveProc[62] = {
    &dMenu_save_c::saveQuestion,
    &dMenu_save_c::saveQuestion2,
    &dMenu_save_c::saveQuestion21,
    &dMenu_save_c::saveQuestion3,
    &dMenu_save_c::saveQuestion4,
    &dMenu_save_c::saveGuide,
    &dMenu_save_c::memCardCheck,
    &dMenu_save_c::backSaveQuestion,
    &dMenu_save_c::backSaveQuestion2,
    &dMenu_save_c::memCardErrMsgWaitKey,
    &dMenu_save_c::IPLSelectDisp1,
    &dMenu_save_c::IPLSelectDisp2,
    &dMenu_save_c::memCardErrGoIPLSel,
    &dMenu_save_c::IPLSelect2Disp,
    &dMenu_save_c::memCardErrGoIPLSel2,
    &dMenu_save_c::memCardErrGotoIPL,
    &dMenu_save_c::memCardErrMsgWaitFormatSel,
    &dMenu_save_c::cardFormatYesSelDisp,
    &dMenu_save_c::cardFormatNoSelDisp,
    &dMenu_save_c::memCardErrMsgWaitFormatSel2,
    &dMenu_save_c::cardFormatYesSel2Disp,
    &dMenu_save_c::memCardFormat,
    &dMenu_save_c::memCardFormatWait,
    &dMenu_save_c::makeGameFileSelDisp,
    &dMenu_save_c::memCardMakeGameFileSel,
    &dMenu_save_c::makeGameFileDisp,
    &dMenu_save_c::memCardMakeGameFile,
    &dMenu_save_c::memCardMakeGameFileWait,
    &dMenu_save_c::memCardCommandEnd,
    &dMenu_save_c::memCardCommandEnd2,
    &dMenu_save_c::memCardDataLoadWait,
    &dMenu_save_c::memCardDataSaveWait,
    &dMenu_save_c::memCardDataSaveWait2,
    &dMenu_save_c::gameContinueDisp,
    &dMenu_save_c::gameContinue,
    &dMenu_save_c::gameContinue2,
    &dMenu_save_c::gameContinue3,
    &dMenu_save_c::saveEnd,
    &dMenu_save_c::endingNoSave,
    &dMenu_save_c::endingNoSave2,
    &dMenu_save_c::endingDataCheck,
    &dMenu_save_c::saveWait,
    &dMenu_save_c::messageChange,
    &dMenu_save_c::retryQuestion0,
    &dMenu_save_c::retryQuestion1,
    &dMenu_save_c::retryQuestion2,
    &dMenu_save_c::openSaveSelect,
    &dMenu_save_c::openSaveSelect2,
    &dMenu_save_c::openSaveSelect3,
    &dMenu_save_c::saveSelect,
    &dMenu_save_c::saveSelectMoveAnime,
    &dMenu_save_c::selectDataOpenMove,
    &dMenu_save_c::saveYesNoSelect,
    &dMenu_save_c::yesNoCursorMoveAnm,
    &dMenu_save_c::saveYesNoCancelMove,
    &dMenu_save_c::saveMoveDisp,
    &dMenu_save_c::saveMoveDisp2,
    &dMenu_save_c::msgWindowInitOpen,
    &dMenu_save_c::msgWindowOpen,
    &dMenu_save_c::msgWindowClose,
    &dMenu_save_c::errYesNoCursorMoveAnm,
};

void dMenu_save_c::_move() {
    if (!mDoRst::isReset() || field_0x21a0 != 0) {
        memCardWatch();
        if (mMenuProc != PROC_MESSAGE_CHANGE && mpScrnExplain != NULL) {
            field_0x219d = mpScrnExplain->getStatus();
            if (mpScrnExplain->getStatus() != 0) {
                mpScrnExplain->move();
            }
        }

        (this->*MenuSaveProc[mMenuProc])();
        saveSelAnm();

        if (mWarning != NULL) {
            mWarning->_move();
        }
    }
}

void dMenu_save_c::saveSelAnm() {
    if (mDisplayMenu) {
        selFileWakuAnm();
        bookIconAnm();
        mSaveSel.Scr->animation();
    }
}

void dMenu_save_c::selFileWakuAnm() {
    mFileWakuAnmFrame += 2;
    if (mFileWakuAnmFrame >= mpFileWakuAnm->getFrameMax()) {
        mFileWakuAnmFrame -= mpFileWakuAnm->getFrameMax();
    }
    mpFileWakuAnm->setFrame(mFileWakuAnmFrame);

    mFileWakuRotAnmFrame += 2;
    if (mFileWakuRotAnmFrame >= mpFileWakuRotAnm->getFrameMax()) {
        mFileWakuRotAnmFrame -= mpFileWakuRotAnm->getFrameMax();
    }
    mpFileWakuRotAnm->setFrame(mFileWakuRotAnmFrame);
}

void dMenu_save_c::bookIconAnm() {
    field_0x154 += 2;
    if (field_0x154 >= field_0x150->getFrameMax()) {
        field_0x154 -= field_0x150->getFrameMax();
    }
    field_0x150->setFrame(field_0x154);

    field_0x15c += 2;
    if (field_0x15c >= field_0x158->getFrameMax()) {
        field_0x15c -= field_0x158->getFrameMax();
    }
    field_0x158->setFrame(field_0x15c);

    field_0x164 += 2;
    if (field_0x164 >= field_0x160->getFrameMax()) {
        field_0x164 -= field_0x160->getFrameMax();
    }
    field_0x160->setFrame(field_0x164);
}

void dMenu_save_c::memCardWatch() {
    if (mUseType == 2) {
        return;
    }

    if (g_mDoMemCd_control.mProbeStat != 0 && g_mDoMemCd_control.mProbeStat != 1) {
        return;
    }

    if (mDoRst::isReset()) {
        return;
    }

    if (field_0x1bf == 0) {
        if (mpScrnExplain != NULL) {
            u8 status = mpScrnExplain->getStatus();
            if (status != 0 && (status == 3 || status == 4)) {
                mpScrnExplain->onForceSelect();
            }
        }
        mMenuProc = PROC_MEMCARD_CHECK;
    }

    g_mDoMemCd_control.mProbeStat = 2;
}

void dMenu_save_c::saveQuestion() {
    if (YesNoSelect()) {
        if (mYesNoCursor == CURSOR_YES) {
            field_0x1c0 = 1;

            J2DPane* pane = mSaveSel.Scr->search('Nm_02');
            pane->setAnimation(field_0x40);

            field_0xd2 = 0;
            field_0x9c = 0;
            field_0x17a = 0;

            strcpy(mpHeaderTxt[mHeaderTxtType], "");
            strcpy(mpHeaderTxt[mHeaderTxtType ^ 1], "");
            field_0x64 = 0;
            field_0x50 = 1;
            field_0x40->setFrame(field_0x50);
            pane->animationTransform();
            pane->setAnimation((J2DAnmTransformKey*)NULL);
            mpHeaderTxtPane[0]->getPanePtr()->scale(1.0f, 1.0f);
            mpHeaderTxtPane[1]->getPanePtr()->scale(1.0f, 1.0f);
            mMenuProc = PROC_SAVE_QUESTION2;
        } else {
            switch (mUseType) {
            case TYPE_WHITE_EVENT:
                mEndStatus = 1;
                mSaveStatus = 3;
                mMenuProc = PROC_SAVE_WAIT;
                break;
            case TYPE_BLACK_EVENT:
                field_0x2190 = 0;
                msgTxtSet(0x4E4, true);  // To save your progress, press START then choose Save
                mMenuProc = PROC_SAVE_GUIDE;
                break;
            }
        }
    }
}

void dMenu_save_c::saveQuestion2() {
    if (mpScrnExplain != NULL && mpScrnExplain->getStatus() == 0) {
        if (mUseType == TYPE_BLACK_EVENT) {
            mWaitTimer = g_msHIO.mFadeOutTimer;
            mDoGph_gInf_c::startFadeOut(mWaitTimer);
            field_0x21a0 = 1;
            mMenuProc = PROC_SAVE_QUESTION21;
        } else {
            mDoGph_gInf_c::startFadeOut(1);
            field_0x21a0 = 1;
            mMenuProc = PROC_SAVE_QUESTION3;
        }
    }
}

void dMenu_save_c::saveQuestion21() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    mMenuProc = PROC_SAVE_QUESTION3;
}

void dMenu_save_c::saveQuestion3() {
    mDisplayMenu = true;
    mWaitTimer = g_msHIO.mFadeOutTimer;
    mDoGph_gInf_c::startFadeIn(mWaitTimer);
    field_0x21a0 = 0;
    mMenuProc = PROC_SAVE_QUESTION4;
}

void dMenu_save_c::saveQuestion4() {
    if (mDoRst::isReset()) {
        return;
    }

    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    field_0x1bf = 0;
    mMenuProc = PROC_MEMCARD_CHECK;
}

void dMenu_save_c::saveGuide() {
    if (mpScrnExplain->getStatus() == 0) {
        mEndStatus = 1;
        mSaveStatus = 3;
        mMenuProc = PROC_SAVE_WAIT;
    }
}

void dMenu_save_c::memCardCheck() {
    u32 cardStatus = g_mDoMemCd_control.getStatus(0);

    if (cardStatus != 14) {
        switch (cardStatus) {
        case 0:
            field_0x1c0 = 0;
            errDispInitSet(0x3AB);  // There is no Memory Card in Slot A.
            field_0x9e = 0;
            field_0x1b4 = 9;
            mErrProc = 0x3D;
            break;
        case 8:
            field_0x1c0 = 0;
            errDispInitSet(0x3AC);  // The Memory Card in Slot A is damaged
            field_0x9e = 0;
            field_0x1b4 = 9;
            mErrProc = 0x3D;
            break;
        case 9:
            field_0x1c0 = 0;
            errDispInitSet(0x3AD);  // Wrong Device in Slot A.
            field_0x9e = 0;
            field_0x1b4 = 9;
            mErrProc = 0x3D;
            break;
        case 10:
            field_0x1c0 = 0;
            errDispInitSet(0x3AE);  // The Memory Card in Slot A is not compatible.
            field_0x9e = 0;
            field_0x1b4 = 9;
            mErrProc = 0x3D;
            break;
        case 6:
        case 7:
            field_0x1c0 = 0;
            errDispInitSet(0x3B0);  // The Memory Card in Slot A is corrupted
            field_0x9e = 1;
            field_0x1b4 = 0x10;
            break;
        case 11:
        case 12:
            field_0x1c0 = 0;
            errDispInitSet(0x3B4);  // There is Insufficient space on the Memory Card in Slot A.
            field_0x9e = 0;
            mpErrFunc = &iplSelMsgInitSet;
            mErrProc = PROC_IPL_SELECT_DISP1;
            field_0x1b4 = 9;
            break;
        case 2:
            g_mDoMemCd_control.load();
            mMenuProc = PROC_MEMCARD_DATA_LOAD_WAIT;
            break;
        case 1:
            field_0x1c0 = 0;
            errDispInitSet(0x3C4);  // There is no save for this game on the Memory Card in Slot A.
            field_0x9e = 0;
            mpErrFunc = &gameFileMakeSelInitSet;
            mErrProc = PROC_MAKE_GAME_FILE_SEL_DISP;
            field_0x1b4 = 9;
            break;
        case 3:
        case 4:
        case 5:
            break;
        }
    }
}

void dMenu_save_c::memCardErrMsgWaitKey() {
    if (cAPICPad_ANY_BUTTON(PAD_1) && dMeter2Info_getMsgKeyWaitTimer() == 0) {
        if (mErrProc == 0x3D) {
            closeSelect();
        } else {
            if (mpErrFunc != NULL) {
                (this->*mpErrFunc)();
            }
            mMenuProc = mErrProc;
        }
    }
}

void dMenu_save_c::backSaveQuestionInitSet() {
    field_0x2190 = 1;
    mYesNoCursor = CURSOR_YES;
    msgTxtSet(0x3CE, true);  // Do you want to save?
}

void dMenu_save_c::backSaveQuestion() {
    mWaitTimer = g_msHIO.mFadeOutTimer;
    mDoGph_gInf_c::startFadeOut(mWaitTimer);
    field_0x21a0 = 1;
    mMenuProc = PROC_BACK_SAVE_QUESTION2;
}

void dMenu_save_c::backSaveQuestion2() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    if (mUseType == TYPE_BLACK_EVENT) {
        if (field_0x21a2 == 1) {
            field_0x2190 = 0;
            mYesNoCursor = CURSOR_YES;
            msgTxtSet(0x4E4, true);  // To save your progress, press START then choose Save
        } else {
            backSaveQuestionInitSet();
        }
    } else {
        backSaveQuestionInitSet();
    }

    mDoGph_gInf_c::startFadeIn(1);
    field_0x21a0 = 0;
    field_0x1c0 = 0;
    field_0x1bf = 1;
    mDisplayMenu = false;

    if (mUseType == TYPE_BLACK_EVENT) {
        if (field_0x21a2 == 1) {
            mMenuProc = PROC_SAVE_GUIDE;
        } else {
            mMenuProc = PROC_SAVE_QUESTION;
        }
    } else {
        mMenuProc = PROC_SAVE_QUESTION;
    }
}

void dMenu_save_c::closeSelect() {
    mDoAud_seStart(Z2SE_SY_MENU_BACK, NULL, 0, 0);
    mSelIcon->setAlphaRate(0.0f);

    if (mUseType != TYPE_WHITE_EVENT && mUseType != TYPE_BLACK_EVENT) {
        mSaveStatus = 3;
        if (dMeter2Info_getMiniGameItemSetFlag() != 0) {
            dMeter2Info_setMiniGameItem(dMeter2Info_getMiniGameItemSetFlag());
        }
        mMenuProc = PROC_SAVE_WAIT;
    } else {
        field_0x1bf = 1;
        field_0x21a2 = 0;
        mMenuProc = PROC_BACK_SAVE_QUESTION;
    }
}

void dMenu_save_c::closeSelect2() {
    mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
    mSelIcon->setAlphaRate(0.0f);
    field_0x21a2 = 1;
    mMenuProc = PROC_BACK_SAVE_QUESTION;
}

void dMenu_save_c::iplSelMsgInitSet() {
    errorTxtSet(0x3B5);  // Either insert a MemCard with free space or manage card contents
}

void dMenu_save_c::iplSelInitSet() {
    errorTxtSet(0x3C0);  // Would you like to manage the contents of your Memory Card?
    mYesNoCursor = CURSOR_NO;
    ketteiTxtDispAnmInit(1);
    yesnoMenuMoveAnmInitSet(2999, 3009, 0);
}

void dMenu_save_c::IPLSelectDisp1() {
    if (errorTxtChangeAnm() == true) {
        mpErrFunc = &iplSelInitSet;
        mErrProc = PROC_IPL_SELECT_DISP2;
        mMenuProc = PROC_MEMCARD_ERRMSG_WAIT_KEY;
    }
}

void dMenu_save_c::IPLSelectDisp2() {
    bool txtAnmComplete = errorTxtChangeAnm();
    bool moveAnmComplete = yesnoMenuMoveAnm();
    bool ketteiAnmComplete = ketteiTxtDispAnm();

    if (txtAnmComplete == true && moveAnmComplete == true && ketteiAnmComplete == true) {
        yesnoCursorShow();
        mMenuProc = PROC_MEMCARD_ERR_GO_IPL_SEL;
    }
}

void dMenu_save_c::memCardErrGoIPLSel() {
    if (errYesNoSelect(1, 0)) {
        if (mYesNoCursor != CURSOR_NO) {
            errorTxtSet(0x3BD);  // The current game data will not be saved
            mYesNoPrevCursor = mYesNoCursor;
            mYesNoCursor = CURSOR_NO;
            yesnoSelectAnmSet(1);
            mMenuProc = PROC_IPL_SELECT2_DISP;
        } else {
            closeSelect();
        }
    }
}

void dMenu_save_c::IPLSelect2Disp() {
    bool txtAnmComplete = errorTxtChangeAnm();
    bool moveAnmComplete = yesnoSelectMoveAnm(1);
    bool wakuAnmComplete = yesnoWakuAlpahAnm(mYesNoPrevCursor);

    if (txtAnmComplete == true && moveAnmComplete == true && wakuAnmComplete == true) {
        yesnoCursorShow();
        mMenuProc = PROC_MEMCARD_ERR_GO_IPL_SEL2;
    }
}

void dMenu_save_c::memCardErrGoIPLSel2() {
    if (errYesNoSelect(1, 0)) {
        if (mYesNoCursor != CURSOR_NO) {
            field_0x1bf = 1;
            mWaitTimer = g_msHIO.mFadeOutTimer;
            mDoGph_gInf_c::startFadeOut(mWaitTimer);
            field_0x21a0 = 1;
            mMenuProc = PROC_MEMCARD_ERR_GOTO_IPL;
        } else {
            closeSelect();
        }
    }
}

void dMenu_save_c::memCardErrGotoIPL() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    field_0x21a1 = 1;
    mDoGph_gInf_c::startFadeIn(0);
    field_0x21a0 = 0;
    mDoRst::onShutdown();
}

void dMenu_save_c::memCardErrMsgWaitFormatSel() {
    if (errYesNoSelect(1, 0)) {
        if (mYesNoCursor != CURSOR_NO) {
            errorTxtSet(0x3B9);  // Formatting the Card in Slot A will erase all save data
            mYesNoPrevCursor = mYesNoCursor;
            mYesNoCursor = CURSOR_NO;
            yesnoSelectAnmSet(1);
            mMenuProc = PROC_CARD_FORMAT_YES_SEL_DISP;
        } else {
            errorTxtSet(0x3BC);  // Unable to save. The Card in Slot A cannot be used
            field_0x9e = 0;
            yesnoMenuMoveAnmInitSet(3009, 2999, 0);
            mMenuProc = PROC_CARD_FORMAT_NO_SEL_DISP;
        }
    }
}

void dMenu_save_c::cardFormatYesSelDisp() {
    bool txtChangeAnm = errorTxtChangeAnm();
    bool moveAnm = yesnoSelectMoveAnm(1);
    bool wakuAlphaAnm = yesnoWakuAlpahAnm(mYesNoPrevCursor);

    if (txtChangeAnm == true && moveAnm == true && wakuAlphaAnm == true) {
        yesnoCursorShow();
        mMenuProc = PROC_MEMCARD_ERRMSG_WAIT_FORMAT_SEL2;
    }
}

void dMenu_save_c::cardFormatNoSelDisp() {
    bool txtChangeAnm = errorTxtChangeAnm();
    bool moveAnm = yesnoMenuMoveAnm();

    if (txtChangeAnm == true && moveAnm == true) {
        mMenuProc = PROC_MEMCARD_ERRMSG_WAIT_KEY;
        mErrProc = 0x3D;
    }
}

void dMenu_save_c::memCardErrMsgWaitFormatSel2() {
    if (errYesNoSelect(1, 0)) {
        if (mYesNoCursor != CURSOR_NO) {
            field_0x1bf = 1;
            errorTxtSet(0x3B8);  // Formatting...
            field_0x9e = 0;
            yesnoMenuMoveAnmInitSet(3009, 2999, 0);
            mMenuProc = PROC_CARD_FORMAT_YES_SEL2_DISP;
        } else {
            errorTxtSet(0x3BC);  // Unable to save. The Card in Slot A cannot be used
            field_0x9e = 0;
            yesnoMenuMoveAnmInitSet(3009, 2999, 0);
            mMenuProc = PROC_CARD_FORMAT_NO_SEL_DISP;
        }
    }
}

void dMenu_save_c::cardFormatYesSel2Disp() {
    bool txtChangeAnm = errorTxtChangeAnm();
    bool moveAnm = yesnoMenuMoveAnm();

    if (txtChangeAnm == true && moveAnm == true) {
        mWaitTimer = g_msHIO.mCardWaitFrames;
        g_mDoMemCd_control.command_format();
        mMenuProc = PROC_MEMCARD_FORMAT;
    }
}

void dMenu_save_c::memCardFormat() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    mCmdState = g_mDoMemCd_control.FormatSync();

    if (mCmdState != 0) {
        mMenuProc = PROC_MEMCARD_FORMAT_WAIT;
    }
}

void dMenu_save_c::memCardFormatWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    if (mCmdState == 2) {
        errorTxtSet(0x399);  // An error might have occurred in formatting.
        mMenuProc = PROC_MEMCARD_COMMAND_END;
    } else if (mCmdState == 1) {
        errorTxtSet(0x39A);  // Formatted.
        mMenuProc = PROC_MEMCARD_COMMAND_END;
    }
}

void dMenu_save_c::gameFileMakeSelInitSet() {
    errorTxtSet(0x3C1);  // Create a save file on the Memory Card in Slot A?
    ketteiTxtDispAnmInit(1);
    mYesNoCursor = CURSOR_NO;
    yesnoMenuMoveAnmInitSet(2999, 3009, 0);
}

void dMenu_save_c::makeGameFileSelDisp() {
    bool txtChangeAnm = errorTxtChangeAnm();
    bool moveAnm = yesnoMenuMoveAnm();
    bool ketteiDispAnm = ketteiTxtDispAnm();

    if (txtChangeAnm == true && moveAnm == true && ketteiDispAnm == true) {
        yesnoCursorShow();
        mMenuProc = PROC_MEMCARD_MAKE_GAME_FILE_SEL;
    }
}

void dMenu_save_c::memCardMakeGameFileSel() {
    if (errYesNoSelect(1, 0)) {
        if (mYesNoCursor != CURSOR_NO) {
            field_0x1bf = 1;
            errorTxtSet(0x3C5);  // Creating a file...
            ketteiTxtDispAnmInit(0);
            field_0x9e = 0;
            yesnoMenuMoveAnmInitSet(3009, 2999, 0);
            mMenuProc = PROC_MAKE_GAME_FILE_DISP;
        } else {
            closeSelect();
        }
    }
}

void dMenu_save_c::makeGameFileDisp() {
    bool txtChangeAnm = errorTxtChangeAnm();
    bool moveAnm = yesnoMenuMoveAnm();
    bool ketteiDispAnm = ketteiTxtDispAnm();

    if (txtChangeAnm == true && moveAnm == true && ketteiDispAnm == true) {
        mWaitTimer = g_msHIO.mCardWaitFrames;
        setInitSaveData();
        dataSave();
        mMenuProc = PROC_MEMCARD_MAKE_GAME_FILE;
    }
}

void dMenu_save_c::memCardMakeGameFile() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    mCmdState = g_mDoMemCd_control.SaveSync();

    if (mCmdState != 0) {
        mMenuProc = PROC_MEMCARD_MAKE_GAME_FILE_WAIT;
    }
}

void dMenu_save_c::memCardMakeGameFileWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    if (mCmdState == 2) {
        errorTxtSet(0x3C6);  // An error might have occurred when creating a save file.
        mMenuProc = PROC_MEMCARD_COMMAND_END;
    } else if (mCmdState == 1) {
        dComIfGs_setNewFile(dComIfGs_getNewFile() | 1);
        errorTxtSet(0x3C7);  // A save file has been created.
        mMenuProc = PROC_MEMCARD_COMMAND_END;
    }
}

void dMenu_save_c::memCardCommandEnd() {
    if (errorTxtChangeAnm() == true) {
        mpErrFunc = NULL;
        mErrProc = PROC_MEMCARD_CHECK;
        mMenuProc = PROC_MEMCARD_ERRMSG_WAIT_KEY;
        field_0x1bf = 0;
    }
}

void dMenu_save_c::memCardCommandEnd2() {
    bool headerTxtChanged = headerTxtChangeAnm();
    bool ketteiDispAnm = ketteiTxtDispAnm();
    bool modoruDispAnm = modoruTxtDispAnm();
    u32 check = mWarning->getStatus() != 0;

    if (headerTxtChanged == true && ketteiDispAnm == true && modoruDispAnm == true && check == 1) {
        mpErrFunc = NULL;
        mErrProc = PROC_MEMCARD_CHECK;
        mMenuProc = PROC_MEMCARD_ERRMSG_WAIT_KEY;
        field_0x1bf = 0;
    }
}

void dMenu_save_c::memCardDataLoadWait() {
    int cardState = g_mDoMemCd_control.LoadSync(mSaveBuffer, sizeof(mSaveBuffer), 0);
    if (cardState != 0) {
        if (cardState == 2) {
            field_0x1c0 = 0;
            mMenuProc = PROC_MEMCARD_CHECK;
        } else if (cardState == 1) {
            saveSelectOpenInit();
        }
    }
}

void dMenu_save_c::dataWrite() {
    int stageNo = dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo());

    dComIfGs_putSave(stageNo);
    dComIfGs_setMemoryToCard(mSaveBuffer, mSelectedFile);
    mDoMemCdRWm_SetCheckSumGameData(mSaveBuffer, mSelectedFile);

    u8* save = mSaveBuffer;
    for (int i = 0; i < 3; i++) {
        mDoMemCdRWm_TestCheckSumGameData(save);
        save += QUEST_LOG_SIZE;
    }

    dataSave();
}

void dMenu_save_c::memCardDataSaveWait() {
    mDoAud_seStartLevel(Z2SE_SY_FILE_SAVE_LEVEL, NULL, 0, 0);

    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    mCmdState = g_mDoMemCd_control.SaveSync();
    if (mCmdState != 0) {
        printf("save cmdState %d\n", mCmdState);
        mMenuProc = PROC_MEMCARD_DATA_SAVE_WAIT2;
    }
}

void dMenu_save_c::memCardDataSaveWait2() {
    if (mWaitTimer != 0) {
        mDoAud_seStartLevel(Z2SE_SY_FILE_SAVE_LEVEL, NULL, 0, 0);
        mWaitTimer--;
        return;
    }

    if (mCmdState == 1) {
        mDoAud_seStart(Z2SE_SY_FILE_SAVE_OK, NULL, 0, 0);
        dComIfGs_setDataNum(mSelectedFile);
        dComIfGs_setNoFile(0);

        if (mUseType == TYPE_WHITE_EVENT || mUseType == TYPE_BLACK_EVENT) {
            headerTxtSet(0x530);  // Saved.
            mWarning->closeInit();
            ketteiTxtDispAnmInit(1);
            mMenuProc = PROC_SAVE_END;
        } else {
            headerTxtSet(0x3CA);  // Saved. Continue Playing?
            mWarning->closeInit();
            mYesNoCursor = CURSOR_YES;
            yesnoMenuMoveAnmInitSet(2259, 2249, 1);
            ketteiTxtDispAnmInit(1);
            mMenuProc = PROC_GAME_CONTINUE_DISP;
        }
        setSaveData();
    } else if (mCmdState == 2) {
        mDoAud_seStart(Z2SE_SY_FILE_ERROR, NULL, 0, 0);
        headerTxtSet(0x3CD);  // An error might have occurred when saving.
        mWarning->closeInit();
        ketteiTxtDispAnmInit(1);
        modoruTxtDispAnmInit(1);
        mMenuProc = PROC_MEMCARD_COMMAND_END2;
    }
}

void dMenu_save_c::gameContinueDisp() {
    bool headerTxtChanged = headerTxtChangeAnm();
    bool moveAnm = yesnoMenuMoveAnm();
    bool ketteiDispAnm = ketteiTxtDispAnm();
    u32 check = mWarning->getStatus() != 0;

    if (headerTxtChanged == true && moveAnm == true && ketteiDispAnm == true && check == 1) {
        yesnoCursorShow();
        mMenuProc = PROC_GAME_CONTINUE;
    }
}

void dMenu_save_c::gameContinue() {
    if (errYesNoSelect(0, 1)) {
        if (mYesNoCursor == CURSOR_YES) {
            mDoAud_seStart(Z2SE_SY_CONTINUE_OK, NULL, 0, 0);

            switch (mUseType) {
            case TYPE_WHITE_EVENT:
                break;
            case TYPE_DEFAULT:
            case 2:
            case TYPE_BLACK_EVENT:
                mEndStatus = 1;
                if (dMeter2Info_getMiniGameItemSetFlag()) {
                    dMeter2Info_setMiniGameItem(dMeter2Info_getMiniGameItemSetFlag());
                }
                break;
            }
            mSelIcon->setAlphaRate(0.0f);
            mSaveStatus = 4;

            if (mUseType == TYPE_BLACK_EVENT) {
                closeSelect2();
            } else {
                mMenuProc = PROC_SAVE_WAIT;
            }
            return;
        }

        mDoAud_seStart(Z2SE_QUIT_GAME, NULL, 0, 0);

        if (mUseType == 2 || mUseType == TYPE_WHITE_EVENT) {
            mWaitTimer = 15;
            mMenuProc = PROC_GAME_CONTINUE3;
            return;
        }

        if (mUseType == TYPE_BLACK_EVENT) {
            mSaveStatus = 4;
            closeSelect2();
            return;
        }

        mWaitTimer = 30;
        mMenuProc = PROC_GAME_CONTINUE2;
    }
}

void dMenu_save_c::gameContinue2() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    mDoRst::onReset();
}

void dMenu_save_c::gameContinue3() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    mMenuProc = PROC_SAVE_WAIT;
    mSaveStatus = 3;
    mEndStatus = 0;
}

void dMenu_save_c::saveEnd() {
    bool headerTxtChanged = headerTxtChangeAnm();
    bool ketteiDispAnm = ketteiTxtDispAnm();
    u32 check = mWarning->getStatus() != 0;

    if (headerTxtChanged == true && ketteiDispAnm == true && check == 1) {
        if (mUseType == TYPE_BLACK_EVENT) {
            mSaveStatus = 4;
            closeSelect2();
        } else {
            mEndStatus = 1;
            restartInit();
            mSaveStatus = 3;
            mMenuProc = PROC_SAVE_WAIT;
        }
    }
}

void dMenu_save_c::endingNoSave() {
    if (YesNoSelect()) {
        if (mYesNoCursor != CURSOR_NO) {
            mEndStatus = 0;
            mMenuProc = PROC_SAVE_WAIT;
            mSaveStatus = 3;
        } else {
            field_0x2190 = 1;
            mYesNoCursor = CURSOR_YES;
            msgTxtSet(0x3CF, false);  // Do you want to continue playing?
            mMenuProc = PROC_ENDING_NO_SAVE2;
        }
    }
}

void dMenu_save_c::endingNoSave2() {
    if (mpScrnExplain != NULL && mpScrnExplain->getStatus() == 0) {
        field_0x2190 = 0;
    }

    mYesNoCursor = CURSOR_NO;
    mMenuProc = PROC_SAVE_QUESTION;
}

void dMenu_save_c::endingDataCheck() {
    mMenuProc = PROC_SAVE_WAIT;
    mSaveStatus = 3;
}

void dMenu_save_c::retryQuestion0() {
    if (Z2GetAudioMgr()->checkBgmIDPlaying(Z2BGM_GAME_OVER) != true) {
        mYesNoCursor = CURSOR_YES;
        msgTxtSet(0x4BC, false);  // Retry?
        mMenuProc = PROC_RETRY_QUESTION1;
    }
}

void dMenu_save_c::retryQuestion1() {
    if (YesNoSelect()) {
        if (mYesNoCursor == CURSOR_YES) {
            mEndStatus = 1;
            restartInit();
            mSaveStatus = 3;
            mMenuProc = PROC_SAVE_WAIT;
        } else {
            field_0x2190 = 1;
            mYesNoCursor = CURSOR_NO;
            msgTxtSet(0x4BF, false);  // Return to the title screen?
            mMenuProc = PROC_RETRY_QUESTION2;
        }
    }
}

void dMenu_save_c::retryQuestion2() {
    if (YesNoSelect()) {
        if (mYesNoCursor == CURSOR_YES) {
            mWaitTimer = 15;
            mMenuProc = PROC_GAME_CONTINUE3;
        } else {
            field_0x2190 = 1;
            mYesNoCursor = CURSOR_YES;
            msgTxtSet(0x4BC, false);  // Retry?
            mMenuProc = PROC_RETRY_QUESTION1;
        }
    }
}

void dMenu_save_c::restartInit() {
    // if monkey lantern sequence not completed
    if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[226])) {
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[224]);  // Lantern stolen
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[225]);  // Monkey dropped lantern
    }

    if (dComIfGs_isItemFirstBit(fpcNm_ITEM_KANTERA) && dComIfGs_getItem(SLOT_1, true) == fpcNm_ITEM_NONE) {
        dComIfGs_setItem(SLOT_1, fpcNm_ITEM_KANTERA);
        dComIfGs_setOil(dMeter2Info_getOilGaugeBackUp());
    }

    if (dMeter2Info_getMiniGameItemSetFlag()) {
        dMeter2Info_setMiniGameItem(dMeter2Info_getMiniGameItemSetFlag());
    }
}

void dMenu_save_c::saveWait() {
    /* empty function */
}

void dMenu_save_c::messageChange() {
    if (mpScrnExplain != NULL) {
        field_0x219d = mpScrnExplain->getStatus();

        u8 explainStatus = mpScrnExplain->getStatus();
        if (explainStatus != 0) {
            if (explainStatus == 3 || explainStatus == 4) {
                mpScrnExplain->onForceSelect();
            }
            mpScrnExplain->move();
            return;
        }

        bool var_r3;
        if (field_0x2190 == 1) {
            var_r3 = mpScrnExplain->openExplain(field_0x2192, 1, mYesNoCursor == CURSOR_NO, -1,
                                                field_0x219c);
            mpScrnExplain->setKeyWaitTimer(30);
        } else {
            var_r3 = mpScrnExplain->openExplain(field_0x2192, 0, 0, -1, true);
        }

        if (var_r3 == true) {
            mMenuProc = field_0x2194;
        }
    }
}

int dMenu_save_c::YesNoSelect() {
    int ret = 0;

    if (mDoRst::isReset()) {
        return 0;
    }

    if (mpScrnExplain == NULL) {
        return 0;
    }

    if (field_0x219d != 5 && mpScrnExplain->getStatus() == 5) {
        switch (mpScrnExplain->getSelectCursor()) {
        case 0:
            if (mMenuProc == PROC_RETRY_QUESTION2) {
                mDoAud_seStart(Z2SE_QUIT_GAME, NULL, 0, 0);
            } else if (mMenuProc == PROC_RETRY_QUESTION1) {
                mDoAud_seStart(Z2SE_SY_CONTINUE_OK, NULL, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
            }
            break;
        case 1:
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                            -1.0f, 0);
            break;
        case 255:
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                            -1.0f, 0);
            break;
        }
    }

    if (mpScrnExplain->getStatus() == 0) {
        field_0x2190 = 0;
        switch (mpScrnExplain->getSelectCursor()) {
        case 0:
            mYesNoCursor = CURSOR_YES;
            break;
        case 1:
            mYesNoCursor = CURSOR_NO;
            break;
        case 255:
            mYesNoCursor = 2;
            break;
        }
        ret = 1;
    }

    return ret;
}

void dMenu_save_c::msgTxtSet(u16 msgID, bool param_1) {
    if (msgID != 0xFFFF) {
        bool check;

        if (mpScrnExplain == NULL) {
            check = false;
        } else if (field_0x2190 == 1) {
            check = mpScrnExplain->openExplain(msgID, 1, mYesNoCursor == CURSOR_NO, 0xFF, param_1);
            mpScrnExplain->setKeyWaitTimer(30);
        } else {
            check = mpScrnExplain->openExplain(msgID, 0, 0, 0xFF, true);
        }

        if (!check) {
            field_0x219c = param_1;
            field_0x2192 = msgID;
            field_0x2194 = mMenuProc;
            mMenuProc = PROC_MESSAGE_CHANGE;
        }
    }

    field_0x1b9 = 0;
}

void dMenu_save_c::openSaveSelect() {
    bool headerTxtChanged = headerTxtChangeAnm();

    bool dataMoveAnmComplete = true;
    if (field_0x64 != 0 || field_0x65 != 0) {
        dataMoveAnmComplete = selectDataBaseMoveAnm();
    }

    bool errMoveAnmComplete = true;
    if (field_0xd2 != 0 || field_0xd3 != 0) {
        errMoveAnmComplete = errorMoveAnm();
    }

    bool yesnoMoveAnmComplete = true;
    if (field_0x9c != 0 || field_0x9d != 0) {
        yesnoMoveAnmComplete = yesnoMenuMoveAnm();
    }

    if (headerTxtChanged == true && dataMoveAnmComplete == true && errMoveAnmComplete == true &&
        yesnoMoveAnmComplete == true) {
        headerTxtSet(0x4CA);  // Save to which log?
        saveSelectOpenAnmSet();
        mDoAud_seStart(Z2SE_SY_FILE_MENU_SLIDE_IN, NULL, 0, 0);
        mMenuProc = PROC_OPEN_SAVE_SELECT2;
    }
}

void dMenu_save_c::openSaveSelect2() {
    bool headerTxtChanged = headerTxtChangeAnm();
    bool dataMoveAnm = selectDataBaseMoveAnm();

    if (headerTxtChanged == true && dataMoveAnm == true) {
        modoruTxtDispAnmInit(1);
        mSelectedFile = dComIfGs_getDataNum();
        mLastSelFile = mSelectedFile;
        mpBookWaku[mSelectedFile]->alphaAnimeStart(0);
        mpSelData[mSelectedFile]->getPanePtr()->setAnimation(field_0x40);
        field_0x74[mSelectedFile] = SelStartFrameTbl[mSelectedFile];
        ketteiTxtDispAnmInit(1);
        mMenuProc = PROC_OPEN_SAVE_SELECT3;
    }
}

void dMenu_save_c::openSaveSelect3() {
    bool modoruTxtDisp = modoruTxtDispAnm();
    bool var_r30 = true;
    bool var_r29 = true;
    bool var_r28 = true;

    if (mSelectedFile != 0xFF) {
        var_r30 = mpBookWaku[mSelectedFile]->alphaAnime(g_msHIO.mEffectDispFrames, 0, 0xFF, 1);
        var_r29 = ketteiTxtDispAnm();

        if (field_0x74[mSelectedFile] != SelEndFrameTbl[mSelectedFile]) {
            field_0x74[mSelectedFile] += 2;

            if (field_0x74[mSelectedFile] > SelEndFrameTbl[mSelectedFile]) {
                field_0x74[mSelectedFile] = SelEndFrameTbl[mSelectedFile];
            }

            field_0x40->setFrame(field_0x74[mSelectedFile]);
            mpSelData[mSelectedFile]->getPanePtr()->animationTransform();
            var_r28 = false;
        }
    }

    if (modoruTxtDisp == true && var_r30 == true && var_r29 == true && var_r28 == true) {
        if (mSelectedFile != 0xFF) {
            mpSelData[mSelectedFile]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
            selFileCursorShow();
        }
        mMenuProc = PROC_SAVE_SELECT;
    }
}

void dMenu_save_c::saveSelect() {
    if (!mDoRst::isReset()) {
        stick->checkTrigger();

        if (mDoCPd_c::getTrigA(PAD_1)) {
            saveSelectStart();
        } else if (mDoCPd_c::getTrigB(PAD_1)) {
            closeSelect();
        } else if (stick->checkUpTrigger()) {
            if (mSelectedFile != 0) {
                mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, NULL, 0, 0);
                mLastSelFile = mSelectedFile;
                mSelectedFile--;
                dataSelectAnmSet();
                mMenuProc = PROC_SAVE_SELECT_MOVE_ANM;
            }
        } else if (stick->checkDownTrigger()) {
            if (mSelectedFile != 2) {
                mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, NULL, 0, 0);
                mLastSelFile = mSelectedFile;
                mSelectedFile++;
                dataSelectAnmSet();
                mMenuProc = PROC_SAVE_SELECT_MOVE_ANM;
            }
        }
    }
}

void dMenu_save_c::saveSelectStart() {
    mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
    selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectedFile],
                             SelOpenEndFrameTbl[mSelectedFile]);
    mSelIcon->setAlphaRate(0.0f);
    selectWakuAlpahAnmInit(mSelectedFile, 255, 0, g_msHIO.mSelectFrames);

    if (field_0x1aa[mSelectedFile] != 0) {
        u8* saveData = mSaveBuffer;
        for (int i = 0; i < 3; i++) {
            mDoMemCdRWm_TestCheckSumGameData(saveData);
            saveData += QUEST_LOG_SIZE;
        }
        headerTxtSet(0x3A);  // Saving...
        mWarning->openInit();
        ketteiTxtDispAnmInit(0);
        modoruTxtDispAnmInit(0);
        mMenuProc = PROC_SAVE_MOVE_DISP2;
    } else {
        yesnoMenuMoveAnmInitSet(2089, 2099, 0);
        headerTxtSet(0x4CB);  // Overwrite this log?
        mMenuProc = PROC_SELECT_DATA_OPEN_MOVE;
    }
}

void dMenu_save_c::selectDataOpenMove() {
    bool headerTxtChanged = headerTxtChangeAnm();
    bool selDataAnmComplete = selectDataMoveAnm();
    bool yesnoAnmComplete = yesnoMenuMoveAnm();
    bool wakuAnmComplete = selectWakuAlpahAnm(mSelectedFile);

    if (headerTxtChanged == true && selDataAnmComplete == true && yesnoAnmComplete == true &&
        wakuAnmComplete == true) {
        yesnoCursorShow();
        mMenuProc = PROC_SAVE_YES_NO_SELECT;
    }
}

void dMenu_save_c::dataSelectAnmSet() {
    if (mSelectedFile != 0xFF) {
        mpSelData[mSelectedFile]->getPanePtr()->setAnimation(field_0x40);
        field_0x74[mSelectedFile] = SelStartFrameTbl[mSelectedFile];
        field_0x40->setFrame(field_0x74[mSelectedFile]);
        mpSelData[mSelectedFile]->getPanePtr()->animationTransform();
        mpBookWaku[mSelectedFile]->alphaAnimeStart(0);
    }

    if (mLastSelFile != 0xFF) {
        selectWakuAlpahAnmInit(mLastSelFile, 255, 0, g_msHIO.mSelectFrames);
        mpSelData[mLastSelFile]->getPanePtr()->setAnimation(field_0x44);
        field_0x74[mLastSelFile] = SelEndFrameTbl[mLastSelFile];
        field_0x44->setFrame(field_0x74[mLastSelFile]);
        mpSelData[mLastSelFile]->getPanePtr()->animationTransform();
        mpBookWaku[mLastSelFile]->alphaAnimeStart(0);
        mSelIcon->setAlphaRate(0.0f);
    }
}

void dMenu_save_c::saveSelectMoveAnime() {
    bool bookWakuAnmComplete = true;
    bool selWakuAnmComplete = true;
    bool var_r29 = true;

    if (mLastSelFile != 0xFF) {
        bookWakuAnmComplete = mpBookWaku[mLastSelFile]->alphaAnime(g_msHIO.mEffectDispFrames, 255, 0, 1);
        selWakuAnmComplete = selectWakuAlpahAnm(mLastSelFile);

        if (field_0x74[mLastSelFile] != SelStartFrameTbl[mLastSelFile]) {
            field_0x74[mLastSelFile] -= 2;

            if (field_0x74[mLastSelFile] < SelStartFrameTbl[mLastSelFile]) {
                field_0x74[mLastSelFile] = SelStartFrameTbl[mLastSelFile];
            }

            field_0x44->setFrame(field_0x74[mLastSelFile]);
            mpSelData[mLastSelFile]->getPanePtr()->animationTransform();
            var_r29 = false;
        }
    }

    bool var_r28 = true;
    bool var_r27 = true;
    if (mSelectedFile != 0xFF) {
        var_r28 = mpBookWaku[mSelectedFile]->alphaAnime(g_msHIO.mEffectDispFrames, 0, 0xFF, 1);

        if (field_0x74[mSelectedFile] != SelEndFrameTbl[mSelectedFile]) {
            field_0x74[mSelectedFile] += 2;

            if (field_0x74[mSelectedFile] > SelEndFrameTbl[mSelectedFile]) {
                field_0x74[mSelectedFile] = SelEndFrameTbl[mSelectedFile];
            }

            field_0x40->setFrame(field_0x74[mSelectedFile]);
            mpSelData[mSelectedFile]->getPanePtr()->animationTransform();
            var_r27 = false;
        }
    }

    if (bookWakuAnmComplete == true && selWakuAnmComplete == true && var_r29 == true && var_r28 == true &&
        var_r27 == true) {
        if (mSelectedFile != 0xFF) {
            mpSelData[mSelectedFile]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
            selFileCursorShow();
        }

        if (mLastSelFile != 0xFF) {
            mpSelData[mLastSelFile]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
        }
        mMenuProc = PROC_SAVE_SELECT;
    }
}

void dMenu_save_c::saveYesNoSelect() {
    if (!mDoRst::isReset()) {
        stick->checkTrigger();

        if (mDoCPd_c::getTrigA(PAD_1)) {
            yesnoSelectStart();
        } else if (mDoCPd_c::getTrigB(PAD_1)) {
            yesnoCancelAnmSet();
        } else if (stick->checkRightTrigger()) {
            if (mYesNoCursor != CURSOR_NO) {
                mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
                mYesNoPrevCursor = mYesNoCursor;
                mYesNoCursor = CURSOR_NO;
                yesnoSelectAnmSet(0);
                mMenuProc = PROC_YES_NO_CURSOR_MOVE_ANM;
            }
        } else if (stick->checkLeftTrigger()) {
            if (mYesNoCursor != CURSOR_YES) {
                mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
                mYesNoPrevCursor = mYesNoCursor;
                mYesNoCursor = CURSOR_YES;
                yesnoSelectAnmSet(0);
                mMenuProc = PROC_YES_NO_CURSOR_MOVE_ANM;
            }
        }
    }
}

void dMenu_save_c::yesnoSelectStart() {
    if (mYesNoCursor != CURSOR_NO) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
        mSelIcon->setAlphaRate(0.0f);
        yesnoMenuMoveAnmInitSet(2099, 2089, 0);
        headerTxtSet(0x3A);  // Saving...
        mWarning->openInit();
        ketteiTxtDispAnmInit(0);
        modoruTxtDispAnmInit(0);
        mMenuProc = PROC_SAVE_MOVE_DISP;
    } else {
        yesnoCancelAnmSet();
    }
}

void dMenu_save_c::saveMoveDisp() {
    mDoAud_seStartLevel(Z2SE_SY_FILE_SAVE_LEVEL, NULL, 0, 0);

    bool headerTxtChanged = headerTxtChangeAnm();
    bool yesnoAnmComplete = yesnoMenuMoveAnm();
    bool ketteiAnmComplete = ketteiTxtDispAnm();
    bool modoruAnmComplete = modoruTxtDispAnm();
    u32 check = mWarning->getStatus() != 0;

    if (headerTxtChanged == true && yesnoAnmComplete == true && ketteiAnmComplete == true &&
        modoruAnmComplete == 1 && check == 1) {
        mWaitTimer = g_msHIO.mCardWaitFrames;
        dataWrite();
        mMenuProc = PROC_MEMCARD_DATA_SAVE_WAIT;
    }
}

void dMenu_save_c::saveMoveDisp2() {
    mDoAud_seStartLevel(Z2SE_SY_FILE_SAVE_LEVEL, NULL, 0, 0);

    bool headerTxtChanged = headerTxtChangeAnm();
    bool dataMoveAnm = selectDataMoveAnm();
    bool wakuAnmComplete = selectWakuAlpahAnm(mSelectedFile);
    bool ketteiAnmComplete = ketteiTxtDispAnm();
    bool modoruAnmComplete = modoruTxtDispAnm();
    u32 check = mWarning->getStatus() != 0;

    if (headerTxtChanged == true && dataMoveAnm == true && wakuAnmComplete == true &&
        ketteiAnmComplete == true && modoruAnmComplete == 1 && check == 1) {
        mWaitTimer = g_msHIO.mCardWaitFrames;
        dataWrite();
        mMenuProc = PROC_MEMCARD_DATA_SAVE_WAIT;
    }
}

void dMenu_save_c::yesnoSelectAnmSet(u8 param_0) {
    if (mYesNoPrevCursor != 0xFF) {
        yesnoWakuAlpahAnmInit(mYesNoPrevCursor, 255, 0, g_msHIO.mSelectFrames);
        mpNoYes[mYesNoPrevCursor]->getPanePtr()->setAnimation(field_0x40);
        field_0x8c[mYesNoPrevCursor] = YnSelEndFrameTbl[mYesNoPrevCursor][param_0];
        field_0x40->setFrame(field_0x8c[mYesNoPrevCursor]);
        mpNoYes[mYesNoPrevCursor]->getPanePtr()->animationTransform();
        mSelIcon->setAlphaRate(0.0f);
    }

    if (mYesNoCursor != 0xFF) {
        mpNoYes[mYesNoCursor]->getPanePtr()->setAnimation(field_0x44);
        field_0x8c[mYesNoCursor] = YnSelStartFrameTbl[mYesNoCursor][param_0];
        field_0x44->setFrame(field_0x8c[mYesNoCursor]);
        mpNoYes[mYesNoCursor]->getPanePtr()->animationTransform();
    }
}

void dMenu_save_c::yesNoCursorMoveAnm() {
    bool selAnmComplete = yesnoSelectMoveAnm(0);
    bool wakuAnmComplete = yesnoWakuAlpahAnm(mYesNoPrevCursor);

    if (selAnmComplete == true && wakuAnmComplete == true) {
        yesnoCursorShow();
        mMenuProc = PROC_SAVE_YES_NO_SELECT;
    }
}

void dMenu_save_c::yesnoCancelAnmSet() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0);
    mSelIcon->setAlphaRate(0.0f);
    headerTxtSet(0x4CA);
    selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectedFile],
                             SelOpenStartFrameTbl[mSelectedFile]);
    yesnoMenuMoveAnmInitSet(2099, 2089, 0);
    mMenuProc = PROC_SAVE_YES_NO_CANCEL_MOVE;
}

void dMenu_save_c::saveYesNoCancelMove() {
    bool headerTxtChanged = headerTxtChangeAnm();
    bool yesnoAnmComplete = yesnoMenuMoveAnm();
    bool dataAnmComplete = selectDataMoveAnm();

    if (headerTxtChanged == true && yesnoAnmComplete == true && dataAnmComplete == true) {
        selFileCursorShow();
        mMenuProc = PROC_SAVE_SELECT;
    }
}

void dMenu_save_c::headerTxtSet(u16 msgID) {
    if (msgID == 0xFFFF) {
        strcpy(mpHeaderTxt[mHeaderTxtType ^ 1], "");
    } else {
        mSaveSel.mMsgString->getString(
            msgID, (J2DTextBox*)mpHeaderTxtPane[mHeaderTxtType ^ 1]->getPanePtr(), NULL,
            mSaveSel.font[0], NULL, 0);
    }

    mpHeaderTxtPane[mHeaderTxtType]->alphaAnimeStart(0);
    mpHeaderTxtPane[mHeaderTxtType ^ 1]->alphaAnimeStart(0);
    mHeaderAnmComplete = false;
}

bool dMenu_save_c::headerTxtChangeAnm() {
    if (mHeaderAnmComplete) {
        return true;
    }

    bool changed = false;
    bool curHeaderAnmComplete =
        mpHeaderTxtPane[mHeaderTxtType]->alphaAnime(g_msHIO.mCharSwitchFrames, 255, 0, 0);
    bool newHeaderAnmComplete =
        mpHeaderTxtPane[mHeaderTxtType ^ 1]->alphaAnime(g_msHIO.mCharSwitchFrames, 0, 255, 0);
    int msgWaitTimer = dMeter2Info_getMsgKeyWaitTimer();

    if (curHeaderAnmComplete == true && newHeaderAnmComplete == true && msgWaitTimer == 0) {
        mHeaderTxtType ^= 1;
        mHeaderAnmComplete = true;
        changed = true;
    }

    return changed;
}

void dMenu_save_c::errDispInitSet(int param_0) {
    headerTxtSet(1);
    mpErrTxtPane[mErrTxtType]->setAlpha(255);
    mpErrTxtPane[mErrTxtType ^ 1]->setAlpha(0);

    mSaveSel.mMsgString->getString(param_0, (J2DTextBox*)mpErrTxtPane[mErrTxtType]->getPanePtr(),
                                   NULL, mSaveSel.font[0], NULL, 0);

    if (field_0xd2 != 0 || field_0xd3 != 0) {
        errorMoveAnmInitSet(2859, 2849);
    }

    if (field_0x64 != 0 || field_0x65 != 0) {
        selectDataBaseMoveAnmInitSet(33, 1);
    }

    if (field_0x9c != 0 || field_0x9d != 0) {
        yesnoMenuMoveAnmInitSet(2099, 2089, 0);
    }

    modoruTxtDispAnmInit(0);
    ketteiTxtDispAnmInit(0);
    mWarning->init();
    mSelIcon->setAlphaRate(0.0f);
    mMenuProc = PROC_MSG_WINDOW_INIT_OPEN;
}

void dMenu_save_c::msgWindowInitOpen() {
    bool headerTxtChanged = true;

    if (field_0x17a == 0) {
        headerTxtChanged = headerTxtChangeAnm();
    }

    bool dataAnmComplete = true;
    bool yesnoAnmComplete = true;
    bool errorAnmComplete = true;

    if (field_0x64 != 0 || field_0x65 != 0) {
        dataAnmComplete = selectDataBaseMoveAnm();
    }

    if (field_0x9c != 0 || field_0x9d != 0) {
        yesnoAnmComplete = yesnoMenuMoveAnm();
    }

    if (field_0xd2 != 0 || field_0xd3 != 0) {
        errorAnmComplete = errorMoveAnm();
    }

    bool modoruAnmComplete = modoruTxtDispAnm();
    bool ketteiAnmComplete = ketteiTxtDispAnm();

    if (headerTxtChanged == true && dataAnmComplete == true && yesnoAnmComplete == true && errorAnmComplete == true &&
        modoruAnmComplete == true && ketteiAnmComplete == true) {
        mSaveSel.Scr->clearAnmTransform();
        errorMoveAnmInitSet(2849, 2859);

        if (field_0x9e != 0) {
            yesnoMenuMoveAnmInitSet(2999, 3009, 0);
            ketteiTxtDispAnmInit(1);
        } else {
            ketteiTxtDispAnmInit(0);
        }

        field_0x17a = 1;
        mMenuProc = PROC_MSG_WINDOW_OPEN;
    }
}

void dMenu_save_c::msgWindowOpen() {
    bool errorAnmComplete = errorMoveAnm();
    bool yesnoAnmComplete = true;

    if (field_0x9e != 0) {
        yesnoAnmComplete = yesnoMenuMoveAnm();
    }

    bool ketteiAnmComplete = ketteiTxtDispAnm();

    if (errorAnmComplete == true && yesnoAnmComplete == true && ketteiAnmComplete == true) {
        if (field_0x9e != 0) {
            yesnoCursorShow();
        }

        mMenuProc = field_0x1b4;
    }
}

void dMenu_save_c::msgWindowClose() {
    bool errorAnmComplete = errorMoveAnm();
    bool yenoAnmComplete = true;

    if (field_0x9c != 0) {
        yenoAnmComplete = yesnoMenuMoveAnm();
    }

    if (errorAnmComplete == true && yenoAnmComplete == true) {
        field_0x9e = 0;

        if (mpErrFunc != NULL) {
            (this->*mpErrFunc)();
        } else {
            mMenuProc = field_0x1b4;
        }
    }
}

bool dMenu_save_c::errYesNoSelect(u8 param_0, u8 param_1) {
    bool yesnoSelected = false;

    if (mDoRst::isReset()) {
        return false;
    }

    stick->checkTrigger();

    if (mDoCPd_c::getTrigA(PAD_1)) {
        if (mYesNoCursor != CURSOR_NO) {
            if (param_1 == 0) {
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
            }
        } else if (param_1 == 0) {
            mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0);
        }

        mSelIcon->setAlphaRate(0.0f);
        yesnoSelected = true;
    } else if (stick->checkRightTrigger()) {
        if (mYesNoCursor != CURSOR_NO) {
            errCurMove(param_0, 0);
        }
    } else if (stick->checkLeftTrigger()) {
        if (mYesNoCursor != CURSOR_YES) {
            errCurMove(param_0, 0);
        }
    }

    return yesnoSelected;
}

void dMenu_save_c::errCurMove(u8 param_0, u8 param_1) {
    mYesNoPrevCursor = mYesNoCursor;
    mYesNoCursor ^= 1;
    field_0x2195 = param_0;

    mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, param_1, 0);
    yesnoSelectAnmSet(param_0);
    field_0x1b3 = mMenuProc;
    mMenuProc = PROC_ERR_YES_NO_CURSOR_MOVE_ANM;
}

void dMenu_save_c::errYesNoCursorMoveAnm() {
    bool moveAnmComplete = yesnoSelectMoveAnm(field_0x2195);
    bool wakuAnmComplete = yesnoWakuAlpahAnm(mYesNoPrevCursor);

    if (moveAnmComplete == true && wakuAnmComplete == true) {
        yesnoCursorShow();
        mMenuProc = field_0x1b3;
    }
}

void dMenu_save_c::errorTxtSet(u16 msgID) {
    if (msgID == 0xFFFF) {
        strcpy(mpErrTxt[mErrTxtType ^ 1], "");
    } else {
        J2DTextBox* tbox = (J2DTextBox*)mpErrTxtPane[mErrTxtType ^ 1]->getPanePtr();
        mSaveSel.mMsgString->getString(msgID, tbox, NULL, mSaveSel.font[0], NULL, 0);
    }

    mpErrTxtPane[mErrTxtType]->alphaAnimeStart(0);
    mpErrTxtPane[mErrTxtType ^ 1]->alphaAnimeStart(0);
    mErrTxtAnmComplete = false;
}

bool dMenu_save_c::errorTxtChangeAnm() {
    if (mErrTxtAnmComplete) {
        return true;
    }

    bool anmComplete = false;
    bool errTxtAnm1Complete =
        mpErrTxtPane[mErrTxtType]->alphaAnime(g_msHIO.mCharSwitchFrames, 255, 0, 0);
    bool errTxtAnm2Complete =
        mpErrTxtPane[mErrTxtType ^ 1]->alphaAnime(g_msHIO.mCharSwitchFrames, 0, 255, 0);
    int msgWaitTimer = dMeter2Info_getMsgKeyWaitTimer();

    if (errTxtAnm1Complete == true && errTxtAnm2Complete == true && msgWaitTimer == 0) {
        mErrTxtType ^= 1;
        mErrTxtAnmComplete = true;
        anmComplete = true;
    }

    return anmComplete;
}

void dMenu_save_c::saveSelectOpenInit() {
    if (field_0x64 != 0 || field_0x65 != 0) {
        selectDataBaseMoveAnmInitSet(0x21, 1);
    }

    if (field_0xd2 != 0 || field_0xd3 != 0) {
        errorMoveAnmInitSet(0xB2B, 0xB21);
    }

    if (field_0x9c != 0 || field_0x9c != 0) {
        yesnoMenuMoveAnmInitSet(2099, 2089, 0);
    }

    mWarning->init();
    headerTxtSet(0xFFFF);
    field_0x17a = 0;
    mMenuProc = PROC_OPEN_SAVE_SELECT;
}

void dMenu_save_c::selectDataBaseMoveAnmInitSet(int param_0, int param_1) {
    mpSelectMoveBase->getPanePtr()->setAnimation(field_0x40);
    mDataBaseMoveAnmFrame = param_0;
    mDataBaseMoveFrameMax = param_1;

    field_0x40->setFrame(mDataBaseMoveAnmFrame);
    mpSelectMoveBase->getPanePtr()->animationTransform();
    field_0x65 = 1;
}

bool dMenu_save_c::selectDataBaseMoveAnm() {
    if (mDataBaseMoveAnmFrame != mDataBaseMoveFrameMax) {
        if (mDataBaseMoveAnmFrame < mDataBaseMoveFrameMax) {
            mDataBaseMoveAnmFrame += 2;

            if (mDataBaseMoveAnmFrame > mDataBaseMoveFrameMax) {
                mDataBaseMoveAnmFrame = mDataBaseMoveFrameMax;
            }
        } else {
            mDataBaseMoveAnmFrame -= 2;

            if (mDataBaseMoveAnmFrame < mDataBaseMoveFrameMax) {
                mDataBaseMoveAnmFrame = mDataBaseMoveFrameMax;
            }
        }

        field_0x40->setFrame(mDataBaseMoveAnmFrame);
        mpSelectMoveBase->getPanePtr()->animationTransform();
        return false;
    } else {
#if VERSION == VERSION_GCN_JPN
        if (mDataBaseMoveAnmFrame == 33) {
            field_0x64 = 1;
        } else {
            field_0x64 = 0;
        }
        field_0x65 = 0;
#else
        if (mDataBaseMoveAnmFrame == 33) {
            field_0x64 = 1;
            field_0x65 = 0;
        } else {
            field_0x64 = 0;
        }
#endif

        mpSelectMoveBase->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
        return true;
    }
}

void dMenu_save_c::saveSelectOpenAnmSet() {
    setSaveData();

    if (field_0x64 == 0) {
        selectDataBaseMoveAnmInitSet(1, 0x21);
    }
}

void dMenu_save_c::selectDataMoveAnmInitSet(int param_0, int param_1) {
    mpSelData[0]->getPanePtr()->setAnimation(field_0x40);
    mpSelData[1]->getPanePtr()->setAnimation(field_0x40);
    mpSelData[2]->getPanePtr()->setAnimation(field_0x40);

    field_0x74[mSelectedFile] = param_0;
    field_0x80 = param_1;
    field_0x40->setFrame(field_0x74[mSelectedFile]);

    mpSelData[0]->getPanePtr()->animationTransform();
    mpSelData[1]->getPanePtr()->animationTransform();
    mpSelData[2]->getPanePtr()->animationTransform();
}

bool dMenu_save_c::selectDataMoveAnm() {
    if (field_0x74[mSelectedFile] != field_0x80) {
        if (field_0x74[mSelectedFile] < field_0x80) {
            field_0x74[mSelectedFile] += 2;

            if (field_0x74[mSelectedFile] > field_0x80) {
                field_0x74[mSelectedFile] = field_0x80;
            }
        } else {
            field_0x74[mSelectedFile] -= 2;

            if (field_0x74[mSelectedFile] < field_0x80) {
                field_0x74[mSelectedFile] = field_0x80;
            }
        }

        field_0x40->setFrame(field_0x74[mSelectedFile]);
        for (int i = 0; i < 3; i++) {
            mpSelData[i]->getPanePtr()->animationTransform();
        }

        return false;
    } else {
        for (int i = 0; i < 3; i++) {
            mpSelData[i]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
        }
        return true;
    }
}

void dMenu_save_c::yesnoMenuMoveAnmInitSet(int anmFrame, int frameMax, u8 param_2) {
    if (param_2 == 0 && field_0x9c == 0) {
        mYesNoCursor = CURSOR_NO;
        mYesNoPrevCursor = 1;
    }

    if (anmFrame == 2089) {
        ((J2DTextBox*)mpNoYesTxt[mYesNoCursor]->getPanePtr())
            ->setWhite(JUtility::TColor(255, 255, 255, 255));
        ((J2DTextBox*)mpNoYesTxt[mYesNoPrevCursor]->getPanePtr())
            ->setWhite(JUtility::TColor(150, 150, 150, 255));
    }

    mSelIcon->setAlphaRate(0.0f);
    mpNoYes[0]->getPanePtr()->setAnimation(field_0x48);
    mpNoYes[1]->getPanePtr()->setAnimation(field_0x48);
    mYesNoMoveAnmFrame = anmFrame;
    mYesNoMoveAnmMax = frameMax;

    field_0x48->setFrame(mYesNoMoveAnmFrame);
    mpNoYes[0]->getPanePtr()->animationTransform();
    mpNoYes[1]->getPanePtr()->animationTransform();
    field_0x9d = 1;
}

bool dMenu_save_c::yesnoMenuMoveAnm() {
    if (mYesNoMoveAnmFrame != mYesNoMoveAnmMax) {
        if (mYesNoMoveAnmFrame < mYesNoMoveAnmMax) {
            mYesNoMoveAnmFrame += 2;

            if (mYesNoMoveAnmFrame > mYesNoMoveAnmMax) {
                mYesNoMoveAnmFrame = mYesNoMoveAnmMax;
            }
        } else {
            mYesNoMoveAnmFrame -= 2;

            if (mYesNoMoveAnmFrame < mYesNoMoveAnmMax) {
                mYesNoMoveAnmFrame = mYesNoMoveAnmMax;
            }
        }

        field_0x48->setFrame(mYesNoMoveAnmFrame);
        mpNoYes[0]->getPanePtr()->animationTransform();
        mpNoYes[1]->getPanePtr()->animationTransform();
        return false;
    } else {
        mpNoYes[0]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
        mpNoYes[1]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);

        if (mYesNoMoveAnmFrame == 0x833 || mYesNoMoveAnmFrame == 0x8C9 ||
            mYesNoMoveAnmFrame == 0xBC1) {
            field_0x9c = 1;

            if (mYesNoCursor != 0xFF) {
                mNoYesBase[mYesNoCursor]->setAlpha(255);
                mNoYesGold[mYesNoCursor]->setAlpha(255);
                mNoYesGold2[mYesNoCursor]->setAlpha(255);
            }
        } else {
            field_0x9c = 0;

            if (mYesNoCursor != 0xFF) {
                mNoYesBase[mYesNoCursor]->setAlpha(0);
                mNoYesGold[mYesNoCursor]->setAlpha(0);
                mNoYesGold2[mYesNoCursor]->setAlpha(0);
            }
        }
        field_0x9d = 0;

        return true;
    }
}

bool dMenu_save_c::yesnoSelectMoveAnm(u8 param_0) {
    bool ret = false;
    bool var_r30 = true;
    if (mYesNoPrevCursor != 0xFF) {
        if (field_0x8c[mYesNoPrevCursor] != YnSelStartFrameTbl[mYesNoPrevCursor][param_0]) {
            if (field_0x8c[mYesNoPrevCursor] < YnSelStartFrameTbl[mYesNoPrevCursor][param_0]) {
                field_0x8c[mYesNoPrevCursor] += 2;

                if (field_0x8c[mYesNoPrevCursor] > YnSelStartFrameTbl[mYesNoPrevCursor][param_0]) {
                    field_0x8c[mYesNoPrevCursor] = YnSelStartFrameTbl[mYesNoPrevCursor][param_0];
                }
            } else {
                field_0x8c[mYesNoPrevCursor] -= 2;

                if (field_0x8c[mYesNoPrevCursor] < YnSelStartFrameTbl[mYesNoPrevCursor][param_0]) {
                    field_0x8c[mYesNoPrevCursor] = YnSelStartFrameTbl[mYesNoPrevCursor][param_0];
                }
            }

            field_0x40->setFrame(field_0x8c[mYesNoPrevCursor]);
            mpNoYes[mYesNoPrevCursor]->getPanePtr()->animationTransform();
            var_r30 = false;
        }
    }

    bool var_r7 = true;
    if (mYesNoCursor != 0xFF) {
        if (field_0x8c[mYesNoCursor] != YnSelEndFrameTbl[mYesNoCursor][param_0]) {
            if (field_0x8c[mYesNoCursor] < YnSelEndFrameTbl[mYesNoCursor][param_0]) {
                field_0x8c[mYesNoCursor] += 2;

                if (field_0x8c[mYesNoCursor] > YnSelEndFrameTbl[mYesNoCursor][param_0]) {
                    field_0x8c[mYesNoCursor] = YnSelEndFrameTbl[mYesNoCursor][param_0];
                }
            } else {
                field_0x8c[mYesNoCursor] -= 2;

                if (field_0x8c[mYesNoCursor] < YnSelEndFrameTbl[mYesNoCursor][param_0]) {
                    field_0x8c[mYesNoCursor] = YnSelEndFrameTbl[mYesNoCursor][param_0];
                }
            }

            field_0x44->setFrame(field_0x8c[mYesNoCursor]);
            mpNoYes[mYesNoCursor]->getPanePtr()->animationTransform();
            var_r7 = false;
        }
    }

    if (var_r30 == true && var_r7 == true) {
        if (mYesNoPrevCursor != 0xFF) {
            mpNoYes[mYesNoPrevCursor]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
        }

        if (mYesNoCursor != 0xFF) {
            mpNoYes[mYesNoCursor]->getPanePtr()->setAnimation((J2DAnmTransformKey*)NULL);
        }
        ret = true;
    }

    return ret;
}

void dMenu_save_c::yesnoCursorShow() {
    if (mYesNoCursor != 0xFF) {
        ((J2DTextBox*)mpNoYesTxt[mYesNoCursor]->getPanePtr())
            ->setWhite(JUtility::TColor(255, 255, 255, 255));

        mNoYesBase[mYesNoCursor]->setAlpha(255);
        mNoYesGold[mYesNoCursor]->setAlpha(255);
        mNoYesGold2[mYesNoCursor]->setAlpha(255);

        Vec pos = mpNoYes[mYesNoCursor]->getGlobalVtxCenter(false, 0);
        mSelIcon->setPos(pos.x, pos.y, mpNoYes[mYesNoCursor]->getPanePtr(), true);
        mSelIcon->setAlphaRate(1.0f);
        mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    }
}

void dMenu_save_c::errorMoveAnmInitSet(int param_0, int param_1) {
    field_0xb4->setAnimation(field_0x4c);
    field_0xb8 = param_0;
    field_0xbc = param_1;
    field_0x4c->setFrame(field_0xb8);
    field_0xb4->animationTransform();
    field_0xd3 = 1;
}

bool dMenu_save_c::errorMoveAnm() {
    if (field_0xb8 != field_0xbc) {
        if (field_0xb8 < field_0xbc) {
            field_0xb8 += 2;

            if (field_0xb8 > field_0xbc) {
                field_0xb8 = field_0xbc;
            }
        } else {
            field_0xb8 -= 2;

            if (field_0xb8 < field_0xbc) {
                field_0xb8 = field_0xbc;
            }
        }

        field_0x4c->setFrame(field_0xb8);
        field_0xb4->animationTransform();
        return 0;
    } else {
        field_0x4c->setFrame(field_0xbc);
        field_0xb4->animationTransform();
        field_0xb4->setAnimation((J2DAnmTransformKey*)NULL);

        if (field_0xbc == 2859) {
            field_0xd2 = 1;
        } else {
            field_0xd2 = 0;
        }
        field_0xd3 = 0;
        return 1;
    }
}

void dMenu_save_c::modoruTxtDispAnmInit(u8 param_0) {
    field_0x1a8 = param_0;
    if ((param_0 != 0 || mpBackTxt->getAlpha() != 0) &&
        (param_0 != 1 || mpBackTxt->getAlpha() != 255)) {
        mpBackTxt->alphaAnimeStart(0);
        mpBBtnIcon->alphaAnimeStart(0);
        return;
    }

    mpBackTxt->alphaAnimeStart(g_msHIO.mCharSwitchFrames);
    mpBBtnIcon->alphaAnimeStart(g_msHIO.mCharSwitchFrames);
}

bool dMenu_save_c::modoruTxtDispAnm() {
    bool txtAnmComplete;
    bool btnAnmComplete;

    if (field_0x1a8 != 0) {
        txtAnmComplete = mpBackTxt->alphaAnime(g_msHIO.mCharSwitchFrames, 0, 255, 0);
        btnAnmComplete = mpBBtnIcon->alphaAnime(g_msHIO.mCharSwitchFrames, 128, 255, 0);
    } else {
        txtAnmComplete = mpBackTxt->alphaAnime(g_msHIO.mCharSwitchFrames, 255, 0, 0);
        btnAnmComplete = mpBBtnIcon->alphaAnime(g_msHIO.mCharSwitchFrames, 255, 128, 0);
    }

    if (txtAnmComplete == true && btnAnmComplete == true) {
        return true;
    }

    return false;
}

void dMenu_save_c::ketteiTxtDispAnmInit(u8 param_0) {
    field_0x1a9 = param_0;
    if ((param_0 != 0 || mpConfirmTxt->getAlpha() != 0) &&
        (param_0 != 1 || mpConfirmTxt->getAlpha() != 255)) {
        mpConfirmTxt->alphaAnimeStart(0);
        mpABtnIcon->alphaAnimeStart(0);
        return;
    }

    mpConfirmTxt->alphaAnimeStart(g_msHIO.mCharSwitchFrames);
    mpABtnIcon->alphaAnimeStart(g_msHIO.mCharSwitchFrames);
}

bool dMenu_save_c::ketteiTxtDispAnm() {
    bool txtAnmComplete;
    bool btnAnmComplete;

    if (field_0x1a9 != 0) {
        txtAnmComplete = mpConfirmTxt->alphaAnime(g_msHIO.mCharSwitchFrames, 0, 255, 0);
        btnAnmComplete = mpABtnIcon->alphaAnime(g_msHIO.mCharSwitchFrames, 128, 255, 0);
    } else {
        txtAnmComplete = mpConfirmTxt->alphaAnime(g_msHIO.mCharSwitchFrames, 255, 0, 0);
        btnAnmComplete = mpABtnIcon->alphaAnime(g_msHIO.mCharSwitchFrames, 255, 128, 0);
    }

    if (txtAnmComplete == true && btnAnmComplete == true) {
        return true;
    }

    return false;
}

void dMenu_save_c::selectWakuAlpahAnmInit(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    mpSelWakuMoyo[param_0]->alphaAnimeStart(0);
    mpSelWakuGold[param_0]->alphaAnimeStart(0);
    mpSelWakuGold2[param_0]->alphaAnimeStart(0);
    field_0x104[param_0] = param_1;
    field_0x107[param_0] = param_2;
    field_0x10a[param_0] = param_3;
}

bool dMenu_save_c::selectWakuAlpahAnm(u8 param_0) {
    bool anmComplete = false;
    bool moyoAnmComplete = mpSelWakuMoyo[param_0]->alphaAnime(
        field_0x10a[param_0], field_0x104[param_0], field_0x107[param_0], 0);
    bool goldAnmComplete = mpSelWakuGold[param_0]->alphaAnime(
        field_0x10a[param_0], field_0x104[param_0], field_0x107[param_0], 0);
    bool gold2AnmComplete = mpSelWakuGold2[param_0]->alphaAnime(
        field_0x10a[param_0], field_0x104[param_0], field_0x107[param_0], 0);

    if (moyoAnmComplete == true && goldAnmComplete == true && gold2AnmComplete == true) {
        anmComplete = true;
    }

    return anmComplete;
}

void dMenu_save_c::selFileCursorShow() {
    mpSelWakuMoyo[mSelectedFile]->getPanePtr()->scale(1.0f, 1.0f);
    mpSelWakuGold[mSelectedFile]->getPanePtr()->scale(1.0f, 1.0f);
    mpSelWakuGold2[mSelectedFile]->getPanePtr()->scale(1.0f, 1.0f);

    mpSelWakuMoyo[mSelectedFile]->setAlpha(255);
    mpSelWakuGold[mSelectedFile]->setAlpha(255);
    mpSelWakuGold2[mSelectedFile]->setAlpha(255);

    Vec pos = mpSelData[mSelectedFile]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mpSelData[mSelectedFile]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
    mSelIcon->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
}

void dMenu_save_c::yesnoWakuAlpahAnmInit(u8 yesnoIdx, u8 startAlpha, u8 endAlpha, u8 anmTimer) {
    if (yesnoIdx != 0xFF) {
        mNoYesBase[yesnoIdx]->alphaAnimeStart(0);
        mNoYesGold[yesnoIdx]->alphaAnimeStart(0);
        mNoYesGold2[yesnoIdx]->alphaAnimeStart(0);
        mNoYesWakuStartAlpha[yesnoIdx] = startAlpha;
        mNoYesWakuEndAlpha[yesnoIdx] = endAlpha;
        mNoYesWakuAnmTimer[yesnoIdx] = anmTimer;
        mpNoYesTxt[yesnoIdx]->colorAnimeStart(0);
    }
}

bool dMenu_save_c::yesnoWakuAlpahAnm(u8 yesnoIdx) {
    if (yesnoIdx == 0xFF) {
        return true;
    }

    bool anmComplete = false;
    bool baseAnmComplete = mNoYesBase[yesnoIdx]->alphaAnime(mNoYesWakuAnmTimer[yesnoIdx],
                                                            mNoYesWakuStartAlpha[yesnoIdx],
                                                            mNoYesWakuEndAlpha[yesnoIdx], 0);

    bool goldAnmComplete = mNoYesGold[yesnoIdx]->alphaAnime(mNoYesWakuAnmTimer[yesnoIdx],
                                                            mNoYesWakuStartAlpha[yesnoIdx],
                                                            mNoYesWakuEndAlpha[yesnoIdx], 0);

    bool gold2AnmComplete = mNoYesGold2[yesnoIdx]->alphaAnime(mNoYesWakuAnmTimer[yesnoIdx],
                                                              mNoYesWakuStartAlpha[yesnoIdx],
                                                              mNoYesWakuEndAlpha[yesnoIdx], 0);

    bool txtAnmComplete = mpNoYesTxt[yesnoIdx]->colorAnime(
        mNoYesWakuAnmTimer[yesnoIdx], mpNoYesTxt[yesnoIdx]->getInitBlack(),
        mpNoYesTxt[yesnoIdx]->getInitBlack(), JUtility::TColor(255, 255, 255, 255),
        JUtility::TColor(150, 150, 150, 255), 0);

    if (baseAnmComplete == true && goldAnmComplete == true && gold2AnmComplete == true &&
        txtAnmComplete == true) {
        anmComplete = true;
    }

    return anmComplete;
}

void dMenu_save_c::dataSave() {
    g_mDoMemCd_control.save(mSaveBuffer, sizeof(mSaveBuffer), 0);
}

void dMenu_save_c::setSaveData() {
    u8* save = mSaveBuffer;

    for (int i = 0; i < 3; i++) {
        BOOL checksumTest = mDoMemCdRWm_TestCheckSumGameData(save);

        int saveStatus = mFileInfo[i]->setSaveData((dSv_save_c*)save, checksumTest, i);
        if (saveStatus == -1) {
            field_0x1ad[i] = 1;
            field_0x1aa[i] = 0;
        } else {
            field_0x1aa[i] = saveStatus;
            field_0x1ad[i] = 0;

            if ((dComIfGs_getNewFile() & 1 || !(dComIfGs_getNewFile() & 0x80)) &&
                field_0x1aa[i] == 2) {
                field_0x1aa[i] = 1;
            }
        }

        if (field_0x1ad[i] != 0 || field_0x1aa[i] == 1) {
            mpDataBase[i]->setAlpha(0);
            mpNoDataBase[i]->setAlpha(255);
        } else {
            mpDataBase[i]->setAlpha(255);
            mpNoDataBase[i]->setAlpha(0);
        }

        if (field_0x1aa[i] == 2) {
            field_0x1aa[i] = 1;
        }

        save += QUEST_LOG_SIZE;
    }
}

void dMenu_save_c::setInitSaveData() {
    for (int i = 0; i < 3; i++) {
        dComIfGs_setInitDataToCard(mSaveBuffer, i);
        mDoMemCdRWm_SetCheckSumGameData(mSaveBuffer, i);
    }
}

void dMenu_save_c::_draw() {
    if (field_0x21a1 == 0 && mpScrnExplain != NULL) {
        mpScrnExplain->draw((J2DOrthoGraph*)dComIfGp_getCurrentGrafPort());
    }
}

void dMenu_save_c::_draw2() {
    if (field_0x21a1 == 0) {
        if (mpScrnExplain != NULL) {
            dComIfGd_set2DOpa(&mMenuSaveExplain);
        }

        if (mDisplayMenu) {
            dComIfGd_set2DOpa(&mSaveSel);

            for (int i = 0; i < 3; i++) {
                mFileInfo[i]->_draw();
            }
            dComIfGd_set2DOpa(mSelIcon);
        }

        if (mWarning != NULL) {
            mWarning->_draw();
        }
    }
}

void dDlst_MenuSaveExplain_c::draw() {
    mpScrn->draw((J2DOrthoGraph*)dComIfGp_getCurrentGrafPort());
}


void dDlst_MenuSave_c::draw() {
    Scr->draw(0.0f, 0.0f, dComIfGp_getCurrentGrafPort());
}
