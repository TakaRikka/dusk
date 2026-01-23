/**
 * d_file_select.cpp
 * dolzel2 - Quest Log Management (File Select Menu)
 */

#include "d/dolzel.h"  // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/d_file_sel_info.h"
#include "d/d_file_select.h"
#include "d/d_lib.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_string.h"
#include "d/d_s_play.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_MemCardRWmng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

static s32 SelStartFrameTbl[3] = {
    59,
    99,
    139,
};

static s32 SelEndFrameTbl[3] = {
    69,
    109,
    149,
};

static s32 SelOpenStartFrameTbl[3] = {
    248,
    448,
    648,
};

static s32 SelOpenEndFrameTbl[3] = {
    259,
    460,
    659,
};

static s32 MenuSelStartFrameTbl[3] = {
    1997,
    898,
    912,
};

static s32 MenuSelEndFrameTbl[3] = {
    2011,
    910,
    898,
};

static u64 l_tagName13[3] = {'w_dat_i0', 'w_dat_i1', 'w_dat_i2'};

dFs_HIO_c::dFs_HIO_c() {
    base_effect_appear_frames = 5;
    char_switch_frames = 5;
    field_0x000a = 60;
    select_box_appear_frames = 5;
    copy_erase_frames = 15;
    select_icon_appear_frames = 5;
    appear_display_wait_frames = 15;
    field_0x000d = 15;
    card_wait_frames = 90;
    test_frame_counts[0] = 1.11f;
    test_frame_counts[1] = 1.11f;
    test_frame_counts[2] = 1.11f;
    title_mesg_check = FALSE;
    title_msg_check_sel = 0;
    error_mesg_check = FALSE;
    error_msg_check_sel = 0;
    mask_mirror_test_display = FALSE;
    test_mask_display = 0;
    test_mirror_display = 0;
}

#if DEBUG
void dFs_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("\n*****タイトルメッセージチェック*****\n", 0);
    mctx->genCheckBox("ＯＮ", &title_mesg_check, 0x1);

    mctx->genLabel("\n*****エラーメッセージチェック*****\n", 0);
    mctx->genCheckBox("ＯＮ", &error_mesg_check, 0x1);

    mctx->genLabel("\n*****出現　表示調整*****\n", 0);
    mctx->genSlider("表示待ちフレーム", &appear_display_wait_frames, 0, 120);
    mctx->genSlider("カード待ちフレーム", &card_wait_frames, 0, 120);
    mctx->genSlider("本エフェクト出現フレーム", &base_effect_appear_frames, 0, 120);
    mctx->genSlider("文字切り替えフレーム", &char_switch_frames, 0, 120);
    mctx->genSlider("選択アイコン", &select_icon_appear_frames, 0, 120);
    mctx->genSlider("選択枠", &select_box_appear_frames, 0, 120);

    mctx->genLabel("\nコピー、消去エフェクト出現、消去\n", 0);
    mctx->genSlider("フレーム数", &copy_erase_frames, 0, 120);

    mctx->genLabel("\n***** 鏡、仮面表示テスト *****\n", 0);
    mctx->genCheckBox("ＯＮ", &mask_mirror_test_display, 0x1);
    mctx->startComboBox("仮面", &test_mask_display);
    mctx->genComboBoxItem("０", 0);
    mctx->genComboBoxItem("１", 1);
    mctx->genComboBoxItem("２", 2);
    mctx->genComboBoxItem("３", 3);
    mctx->genComboBoxItem("４", 4);
    mctx->endComboBox();

    mctx->startComboBox("鏡", &test_mirror_display);
    mctx->genComboBoxItem("０", 0);
    mctx->genComboBoxItem("１", 1);
    mctx->genComboBoxItem("２", 2);
    mctx->genComboBoxItem("３", 3);
    mctx->genComboBoxItem("４", 4);
    mctx->endComboBox();

    mctx->genLabel("\n*****テスト調整*****\n", 0);
    mctx->genSlider("フレーム１", &test_frame_counts[0], 1.0f, 5.0f);
    mctx->genSlider("フレーム２", &test_frame_counts[1], 1.0f, 5.0f);
    mctx->genSlider("フレーム３", &test_frame_counts[2], 1.0f, 5.0f);
}
#endif

static dFs_HIO_c g_fsHIO;

dFile_select_c::dFile_select_c(JKRArchive* i_archiveP) {
    mpArchive = i_archiveP;
    mpFileSelect3d = new dFile_select3D_c();
}

dFile_select_c::~dFile_select_c() {
    int i;

    for (i = 0; i < 3; i++) {
        delete mSelFileMoyoPane[i];
        delete mSelFileGoldPane[i];
        delete mSelFileGold2Pane[i];
        delete m3mSelPane_mo[i];
        delete m3mSelPane_g[i];
        delete m3mSelPane_gr[i];
        delete m3mSelTextPane[i];
        delete mSelFilePane_Book_l[i];
        delete mDeleteEfPane[i];
        delete mCopyEfPane[i];
        delete mSelFilePanes[i];
        delete m3mSelPane[i];
        delete mFileInfo[i];
    }

    delete mBaseMovePane;

    for (i = 0; i < 2; i++) {
        delete mYnSelPane[i];
        delete mYnSelPane_m[i];
        delete mYnSelPane_g[i];
        delete mYnSelPane_gr[i];
        delete mHeaderTxtPane[i];
        delete mErrorMsgTxtPane[i];
        delete mYnSelTxtPane[i];
        delete mCpFileInfo[i];
        delete mCpSelPane[i];
        delete mCpSelPane_moyo[i];
        delete mCpSelPane_gold[i];
        delete mCpSelPane_gold2[i];
        delete mCpSelPane_book[i];
    }

    delete mBbtnPane;
    delete mModoruTxtPane;
    delete mAbtnPane;
    delete mKetteiTxtPane;
    delete mBaseMoveAnm;
    delete field_0x0088;
    delete mYnSelBck3;
    delete mFileSelBpk;
    delete mFileSel05Btk;
    delete mSelFileBookBpk;
    delete mSelFileBookBtk;
    delete mSelFileBookBrk;
    delete mDtEffBtk;
    delete mCpEffBtk;
    delete mCpDtEffBrk;
    delete field_0x0090;
    delete field_0x0094;
    delete field_0x0098;
    delete field_0x009c;

    mDoExt_removeMesgFont();
    mDoExt_removeSubFont();

    delete stick;
    delete mSelIcon;
    delete mSelIcon2;
    delete mpName;
    delete mpFileWarning;

    delete mCpSelBck;
    delete mCpSelBck2;
    delete mCpSelBpk;
    delete mCpSel03Btk;
    delete mCpSelBookBpk;
    delete mCpSelBookBtk;
    delete mCpSelBookBrk;

    delete mYnSelBck;
    delete mYnSelBck2;
    delete mYnSelBpk;
    delete mYnSelBtk;

    delete m3mBck;
    delete m3mBck2;
    delete m3mBpk;
    delete m3mBtk;

    delete mSelDtBtk;
    delete mSelDtPane_mset;

    mpFileSelect3d->_delete();
    delete mpFileSelect3d;
    
    #if PLATFORM_GCN
    delete mpFadePict;
    dComIfGp_getMain2DArchive()->removeResourceAll();
    #endif

    dComIfGp_getCollectResArchive()->removeResourceAll();

    mDoHIO_DELETE_CHILD(g_fsHIO.no);
}

void dFile_select_c::_create() {
    int i;

    mDoGph_gInf_c::setFadeColor(static_cast<JUtility::TColor&>(g_blackColor));
    
    stick = new STControl(2, 2, 1, 1, 0.9f, 0.5f, 0, 0x2000);
    JUT_ASSERT(355, stick != NULL);

    g_fsHIO.no = mDoHIO_CREATE_CHILD("ファイルセレクト画面", &g_fsHIO);

    for (i = 0; i < 3; i++) {
        mIsDataNew[i] = false;
        mIsNoData[i] = false;
    }

    mSelectNum = 0;
    #if PLATFORM_WII || PLATFORM_SHIELD
    field_0x4332 = 0;
    #endif

    dComIfGs_init();
    dComIfGp_itemDataInit();
    dComIfGs_setNewFile(0);

    mHasDrawn = false;

    screenSet();
    screenSetCopySel();
    screenSet3Menu();
    screenSetYesNo();
    screenSetDetail();
    displayInit();
}

typedef void (dFile_select_c::*DataSelProcFunc)(void);
static DataSelProcFunc DataSelProc[] = {
    &dFile_select_c::MemCardCheckMain,
    &dFile_select_c::dataSelectIn,
    &dFile_select_c::dataSelectInit,
    &dFile_select_c::dataSelect,
    &dFile_select_c::dataSelectMoveAnime,
    &dFile_select_c::selectDataOpenMove,
    &dFile_select_c::selectDataNameMove,
    &dFile_select_c::selectDataOpenEraseMove,
    &dFile_select_c::menuSelect,
    &dFile_select_c::menuSelectMoveAnm,
    &dFile_select_c::ToNameMove,
    &dFile_select_c::ToCopyPaneMove,
    &dFile_select_c::ToErasePaneMove,
    &dFile_select_c::backSelectMove,
    &dFile_select_c::nameToDataSelectMove,
    &dFile_select_c::nameInputWait,
    &dFile_select_c::nameInput,
    &dFile_select_c::nameInputFade,
    &dFile_select_c::nameInput2Move,
    &dFile_select_c::nameInput2,
    &dFile_select_c::backNameInputMove0,
    &dFile_select_c::backNameInputMove,
    &dFile_select_c::copyDataToSelect,
    &dFile_select_c::copyDataToSelectMoveAnm,
    &dFile_select_c::copyToSelBack,
    &dFile_select_c::copyToSelPaneMove,
    &dFile_select_c::YesNoSelect,
    &dFile_select_c::YesNoCancelMove,
    &dFile_select_c::yesNoCursorMoveAnm,
    &dFile_select_c::CmdExecPaneMove0,
    &dFile_select_c::CommandExec,
    &dFile_select_c::DataEraseWait,
    &dFile_select_c::DataEraseWait2,
    &dFile_select_c::DataCopyWait,
    &dFile_select_c::DataCopyWait2,
    &dFile_select_c::copyPaneMoveOk,
    &dFile_select_c::copyPaneMoveOk2,
    &dFile_select_c::ErasePaneMoveOk,
    &dFile_select_c::ErasePaneMoveOk2,
    &dFile_select_c::ErrorMsgPaneMove,
    &dFile_select_c::eraseEndBackSelectWait,
    &dFile_select_c::eraseEndBackSelect,
    &dFile_select_c::backDatSelWait,
    &dFile_select_c::backDatSelWait2,
    &dFile_select_c::backDatSelPaneMove,
    &dFile_select_c::ToNameMove2,
    &dFile_select_c::nextModeWait,

    #if PLATFORM_WII || PLATFORM_SHIELD
    &dFile_select_c::dataSelectInCopy,
    &dFile_select_c::cardToNandDataCopy,
    &dFile_select_c::cardToNandDataCopyWait,
    &dFile_select_c::cardToNandDataCopyWait2,
    &dFile_select_c::cardToNandDataCopyOkDisp,
    &dFile_select_c::cardToNandDataCopyOkDisp2,
    &dFile_select_c::cardToNandDataCopyOkDisp3,
    &dFile_select_c::cardToNandDataCopyErrDisp,
    &dFile_select_c::cardToNandDataCopyErrDisp2,
    &dFile_select_c::cardToNandDataCopyErrDisp3,
    #endif
};

void dFile_select_c::_move() {
    #if DEBUG
    if (g_fsHIO.title_mesg_check) {
        titleMsgCheck();
    }

    if (g_fsHIO.error_mesg_check) {
        errorMsgCheck();
    }
    #endif

    dMeter2Info_decMsgKeyWaitTimer();

    #if PLATFORM_GCN
    s32 drive_status = DVDGetDriveStatus();
    if (drive_status != DVD_STATE_END && drive_status != DVD_STATE_BUSY && field_0x03b1) {
        field_0x03b1 = 0;
    }
    #endif

    #if !PLATFORM_WII
    if ((mDoMemCd_getProbeStat() == 0 || mDoMemCd_getProbeStat() == 1) &&
        (!mDoRst::isReset() && !field_0x03b1))
    {
        #if PLATFORM_GCN
        if (mpFadePict->getAlpha() != 0) {
            mpFadePict->setAlpha(0);
        }
        #endif

        mDataSelProc = DATASELPROC_MEMCARD_CHECK_MAIN;
        mCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
    }
    #endif

    (this->*DataSelProc[mDataSelProc])();

    selFileWakuAnm();
    bookIconAnm();
    dataDelEffAnm();
    dataCopyEffAnm();

    fileSel.Scr->animation();
    mYnSel.ScrYn->animation();
    m3mSel.Scr3m->animation();
    mSelDt.ScrDt->animation();

    if (mCpSel.isShow) {
        selCopyFileWakuAnm();
        copyBookIconAnm();
        mCpSel.Scr->animation();
    }

    mpFileWarning->_move();
    mpFileSelect3d->_move();

    mDoMemCd_clearProbeStat();
}

#if DEBUG
void dFile_select_c::titleMsgCheck() {
    if (mDoCPd_c::getTrigRight(PAD_1)) {
        g_fsHIO.title_msg_check_sel++;
        if (g_fsHIO.title_msg_check_sel > 16) {
            g_fsHIO.title_msg_check_sel = 0;
        }
    } else if (mDoCPd_c::getTrigLeft(PAD_1)) {
        if (g_fsHIO.title_msg_check_sel == 0) {
            g_fsHIO.title_msg_check_sel = 16;
        } else {
            g_fsHIO.title_msg_check_sel--;
        }
    }

    static u16 msg[] = {
        0x0001, 0x0040, 0x0041,
        0x0042, 0x0052, 0x0043,
        0x0044, 0x0045, 0x0046,
        0x0047, 0x0048, 0x0049,
        0x004a, 0x004b, 0x004c,
        0x0384, 0x0385,
    };

    static u8 font[] = {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
    static f32 fontsize[] = {21.0f, 27.0f};
    static f32 linespaceU[] = {22.0f, 20.0f};
    static f32 charspaceU[] = {0.0f, 0.0f};

    ((J2DTextBox*)mHeaderTxtPane[mHeaderTxtDispIdx]->getPanePtr())->setFont(fileSel.font[font[g_fsHIO.title_msg_check_sel]]);
    ((J2DTextBox*)mHeaderTxtPane[mHeaderTxtDispIdx]->getPanePtr())->setFontSize(fontsize[font[g_fsHIO.title_msg_check_sel]], fontsize[font[g_fsHIO.title_msg_check_sel]]);
    ((J2DTextBox*)mHeaderTxtPane[mHeaderTxtDispIdx]->getPanePtr())->setLineSpace(linespaceU[font[g_fsHIO.title_msg_check_sel]]);
    ((J2DTextBox*)mHeaderTxtPane[mHeaderTxtDispIdx]->getPanePtr())->setCharSpace(charspaceU[font[g_fsHIO.title_msg_check_sel]]);

    fileSel.mMessageString->getString(msg[g_fsHIO.title_msg_check_sel], (J2DTextBox*)mHeaderTxtPane[mHeaderTxtDispIdx]->getPanePtr(), NULL, fileSel.font[font[g_fsHIO.title_msg_check_sel]], NULL, 0);
}

void dFile_select_c::errorMsgCheck() {
    if (mDoCPd_c::getTrigRight(PAD_1)) {
        g_fsHIO.error_msg_check_sel++;
        if (g_fsHIO.error_msg_check_sel > 17) {
            g_fsHIO.error_msg_check_sel = 0;
        }
    } else if (mDoCPd_c::getTrigLeft(PAD_1)) {
        if (g_fsHIO.error_msg_check_sel == 0) {
            g_fsHIO.error_msg_check_sel = 17;
        } else {
            g_fsHIO.error_msg_check_sel--;
        }
    }

    static u16 er_msg[] = {
        0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0009,
        0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x0011,
        0x0012, 0x0013, 0x0019, 0x001b, 0x001a, 0x001c,
    };

    fileSel.mMessageString->getString(er_msg[g_fsHIO.error_msg_check_sel], (J2DTextBox*)mErrorMsgTxtPane[mErrorTxtDispIdx]->getPanePtr(), NULL, fileSel.font[0], NULL, 0);
}
#endif

void dFile_select_c::selFileWakuAnm() {
    mSelFileBpkFrame += 2;
    if (mSelFileBpkFrame >= mFileSelBpk->getFrameMax())
        mSelFileBpkFrame -= mFileSelBpk->getFrameMax();
    mFileSelBpk->setFrame(mSelFileBpkFrame);

    mSelFileBtk05Frame += 2;
    if (mSelFileBtk05Frame >= mFileSel05Btk->getFrameMax())
        mSelFileBtk05Frame -= mFileSel05Btk->getFrameMax();
    mFileSel05Btk->setFrame(mSelFileBtk05Frame);

    mYnSelBpkFrame += 2;
    if (mYnSelBpkFrame >= mYnSelBpk->getFrameMax())
        mYnSelBpkFrame -= mYnSelBpk->getFrameMax();
    mYnSelBpk->setFrame(mYnSelBpkFrame);

    mYnSelBtkFrame += 2;
    if (mYnSelBtkFrame >= mYnSelBtk->getFrameMax())
        mYnSelBtkFrame -= mYnSelBtk->getFrameMax();
    mYnSelBtk->setFrame(mYnSelBtkFrame);

    m3mBpkFrame += 2;
    if (m3mBpkFrame >= m3mBpk->getFrameMax())
        m3mBpkFrame -= m3mBpk->getFrameMax();
    m3mBpk->setFrame(m3mBpkFrame);

    m3mBtkFrame += 2;
    if (m3mBtkFrame >= m3mBtk->getFrameMax())
        m3mBtkFrame -= m3mBtk->getFrameMax();
    m3mBtk->setFrame(m3mBtkFrame);

    mSelDtBtkFrame += 2;
    if (mSelDtBtkFrame >= mSelDtBtk->getFrameMax())
        mSelDtBtkFrame -= mSelDtBtk->getFrameMax();
    mSelDtBtk->setFrame(mSelDtBtkFrame);
}

void dFile_select_c::bookIconAnm() {
    mSelFileBookBpkFrame += 2;
    if (mSelFileBookBpkFrame >= mSelFileBookBpk->getFrameMax())
        mSelFileBookBpkFrame -= mSelFileBookBpk->getFrameMax();
    mSelFileBookBpk->setFrame(mSelFileBookBpkFrame);

    mSelFileBookBtkFrame += 2;
    if (mSelFileBookBtkFrame >= mSelFileBookBtk->getFrameMax())
        mSelFileBookBtkFrame -= mSelFileBookBtk->getFrameMax();
    mSelFileBookBtk->setFrame(mSelFileBookBtkFrame);

    mSelFileBookBrkFrame += 2;
    if (mSelFileBookBrkFrame >= mSelFileBookBrk->getFrameMax())
        mSelFileBookBrkFrame -= mSelFileBookBrk->getFrameMax();
    mSelFileBookBrk->setFrame(mSelFileBookBrkFrame);
}

void dFile_select_c::selCopyFileWakuAnm() {
    mCpSelBpkFrame += 2;
    if (mCpSelBpkFrame >= mCpSelBpk->getFrameMax())
        mCpSelBpkFrame -= mCpSelBpk->getFrameMax();
    mCpSelBpk->setFrame(mCpSelBpkFrame);

    mCpSel03BtkFrame += 2;
    if (mCpSel03BtkFrame >= mCpSel03Btk->getFrameMax())
        mCpSel03BtkFrame -= mCpSel03Btk->getFrameMax();
    mCpSel03Btk->setFrame(mCpSel03BtkFrame);
}

void dFile_select_c::copyBookIconAnm() {
    mCpSelBookBpkFrame += 2;
    if (mCpSelBookBpkFrame >= mCpSelBookBpk->getFrameMax())
        mCpSelBookBpkFrame -= mCpSelBookBpk->getFrameMax();
    mCpSelBookBpk->setFrame(mCpSelBookBpkFrame);

    mCpSelBookBtkFrame += 2;
    if (mCpSelBookBtkFrame >= mCpSelBookBtk->getFrameMax())
        mCpSelBookBtkFrame -= mCpSelBookBtk->getFrameMax();
    mCpSelBookBtk->setFrame(mCpSelBookBtkFrame);

    mCpSelBookBrkFrame += 2;
    if (mCpSelBookBrkFrame >= mCpSelBookBrk->getFrameMax())
        mCpSelBookBrkFrame -= mCpSelBookBrk->getFrameMax();
    mCpSelBookBrk->setFrame(mCpSelBookBrkFrame);
}

void dFile_select_c::dataDelEffAnm() {
    if (field_0x0208 != 0) {
        mCpDtEffBrkFrame += 2;
        if (mCpDtEffBrkFrame >= mCpDtEffBrk->getFrameMax())
            mCpDtEffBrkFrame -= mCpDtEffBrk->getFrameMax();
        mCpDtEffBrk->setFrame(mCpDtEffBrkFrame);

        mDtEffBtkFrame += 2;
        if (mDtEffBtkFrame >= mDtEffBtk->getFrameMax())
            mDtEffBtkFrame -= mDtEffBtk->getFrameMax();
        mDtEffBtk->setFrame(mDtEffBtkFrame);
    }
}

void dFile_select_c::dataCopyEffAnm() {
    if (field_0x0209 != 0) {
        mCpDtEffBrkFrame += 2;
        if (mCpDtEffBrkFrame >= mCpDtEffBrk->getFrameMax())
            mCpDtEffBrkFrame -= mCpDtEffBrk->getFrameMax();
        mCpDtEffBrk->setFrame(mCpDtEffBrkFrame);

        mCpEffBtkFrame += 2;
        if (mCpEffBtkFrame >= mCpEffBtk->getFrameMax())
            mCpEffBtkFrame -= mCpEffBtk->getFrameMax();
        mCpEffBtk->setFrame(mCpEffBtkFrame);
    }
}

void dFile_select_c::selectDataBaseMoveAnmInitSet(int i_frame, int i_frameMax) {
    mBaseMovePane->getPanePtr()->setAnimation(mBaseMoveAnm);
    mBaseMoveAnmFrame = i_frame;
    mBaseMoveAnmFrameMax = i_frameMax;
    mBaseMoveAnm->setFrame(mBaseMoveAnmFrame);
    mBaseMovePane->getPanePtr()->animationTransform();

    #if PLATFORM_GCN
    field_0x00b9 = 1;
    #endif
}

bool dFile_select_c::selectDataBaseMoveAnm() {
    bool ret;
    if (mBaseMoveAnmFrame != mBaseMoveAnmFrameMax) {
        if (mBaseMoveAnmFrame < mBaseMoveAnmFrameMax) {
            mBaseMoveAnmFrame += 2;
            if (mBaseMoveAnmFrame > mBaseMoveAnmFrameMax)
                mBaseMoveAnmFrame = mBaseMoveAnmFrameMax;
        } else {
            mBaseMoveAnmFrame -= 2;
            if (mBaseMoveAnmFrame < mBaseMoveAnmFrameMax)
                mBaseMoveAnmFrame = mBaseMoveAnmFrameMax;
        }

        mBaseMoveAnm->setFrame(mBaseMoveAnmFrame);
        mBaseMovePane->getPanePtr()->animationTransform();
        ret = false;
    } else {
        if (mBaseMoveAnmFrame == 33) {
            field_0x00b8 = 1;
        } else {
            field_0x00b8 = 0;
        }

        #if PLATFORM_GCN
        field_0x00b9 = 0;
        #endif

        mBaseMovePane->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        ret = true;
    }

    return ret;
}

void dFile_select_c::dataSelectInAnmSet() {
    setSaveData();

    dSv_save_c* pSave = (dSv_save_c*)mSaveData;
    OSTime date_ipl = pSave->getPlayer().getPlayerStatusB().getDateIpl();
    mSelectNum = 0;

    for (int i = 0; i < 3; i++) {
        #if PLATFORM_GCN
        mSelFileMoyoPane[i]->setAlpha(0);
        mSelFileGoldPane[i]->setAlpha(0);
        mSelFileGold2Pane[i]->setAlpha(0);
        #endif

        if (!mIsNoData[i]) {
            OSTime date_ipl2 = pSave->getPlayer().getPlayerStatusB().getDateIpl();
            OSTime date2Secs = OSTicksToSeconds(date_ipl2);
            OSTime dateSec = OSTicksToSeconds(date_ipl);

            if (dateSec < date2Secs) {
                date_ipl = date_ipl2;
                mSelectNum = i;
            }
        }

        pSave = (dSv_save_c*)((u8*)pSave + SAVEDATA_SIZE);

        if (mIsNoData[i] || mIsDataNew[i] != 0) {
            mFileInfoDatBasePane[i]->setAlpha(0);
            mFileInfoNoDatBasePane[i]->setAlpha(0xFF);
        } else {
            mFileInfoDatBasePane[i]->setAlpha(0xFF);
            mFileInfoNoDatBasePane[i]->setAlpha(0);
        }

        mSelFilePane_Book_l[i]->setAlpha(0);
    }

    #if PLATFORM_GCN
    mpFileWarning->init();
    #endif

    selectDataBaseMoveAnmInitSet(1, 33);
    mDoAud_seStart(Z2SE_SY_FILE_MENU_SLIDE_IN, NULL, 0, 0);
}

void dFile_select_c::dataSelectIn() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelDataBaseMove = selectDataBaseMoveAnm();
    bool isErrorMove = true;
    bool isYnMenuMove = true;

    #if PLATFORM_GCN
    if (field_0x014a || field_0x014b)
    #else
    if (field_0x014a)
    #endif
    {
        isErrorMove = errorMoveAnm();
    }

    isYnMenuMove = true;
    if (field_0x0108 || field_0x0281) {
        isYnMenuMove = yesnoMenuMoveAnm();
    }

    bool isNameMove = true;
    #if PLATFORM_GCN
    if (field_0x0128 || mCpSel.isShow)
    #else
    if (field_0x0128)
    #endif
    {
        isNameMove = nameMoveAnm();
    }

    #if PLATFORM_GCN
    bool isMenuMove = true;
    if (field_0x0360 || field_0x0283) {
        isMenuMove = menuMoveAnm();
    }
    #endif

    bool isModoruTxtDisp = modoruTxtDispAnm();

    if (isHeaderTxtChange == true &&
        isSelDataBaseMove == true &&
        isErrorMove == true &&
        isYnMenuMove == true &&
        isNameMove == true &&
        #if PLATFORM_GCN
        isMenuMove == true &&
        #endif
        isModoruTxtDisp == true)
    {
        for (int i = 0; i < 3; i++) {
            mSelFilePanes[i]->reinit();
        }

        headerTxtSet(0x43, 1, 0);
        mLastSelectNum = mSelectNum;
        mSelFilePanes[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)mBaseMoveAnm);
        field_0x00e0[mSelectNum] = SelStartFrameTbl[mSelectNum];
        mSelFilePane_Book_l[mSelectNum]->alphaAnimeStart(0);
        ketteiTxtDispAnmInit(1);
        mDataSelProc = DATASELPROC_DATA_SELECT_INIT;
    }
}

void dFile_select_c::dataSelectInit() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isBookAlphaAnime = true;
    bool isKetteiTxtDisp = true;
    bool check = true;

    if (mSelectNum != 0xFF) {
        isBookAlphaAnime = mSelFilePane_Book_l[mSelectNum]->alphaAnime(g_fsHIO.base_effect_appear_frames, 0, 0xFF, 1);
        isKetteiTxtDisp = ketteiTxtDispAnm();

        if (field_0x00e0[mSelectNum] != SelEndFrameTbl[mSelectNum]) {
            field_0x00e0[mSelectNum] += 2;

            if (field_0x00e0[mSelectNum] > SelEndFrameTbl[mSelectNum])
                field_0x00e0[mSelectNum] = SelEndFrameTbl[mSelectNum];

            mBaseMoveAnm->setFrame(field_0x00e0[mSelectNum]);
            mSelFilePanes[mSelectNum]->getPanePtr()->animationTransform();
            check = false;
        }
    }

    if (isHeaderTxtChange == true && isBookAlphaAnime == true && isKetteiTxtDisp == true && check == true) {
        if (mSelectNum != 0xFF) {
            mSelFilePanes[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            selFileCursorShow();
        }

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectNum;
        #endif

        mDataSelProc = DATASELPROC_DATA_SELECT;
    }
}

// handles switching between quest logs
void dFile_select_c::dataSelect() {
    stick->checkTrigger();

    // If A or Start was pressed
    if (mDoCPd_c::getTrigA(PAD_1) || mDoCPd_c::getTrigStart(PAD_1)) {
        dataSelectStart();  // run the quest log open process
    } else if (stick->checkUpTrigger()) {
        // if we're not on the top quest log
        if (mSelectNum != 0) {
            mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, NULL, 0, 0);
            mLastSelectNum = mSelectNum;
            mSelectNum--;
            dataSelectAnmSet();  // run the quest log selection animation
            mDataSelProc = DATASELPROC_DATA_SELECT_MOVE_ANIME;
        }
    } else if (stick->checkDownTrigger()) {
        // if we're not on the bottom quest log
        if (mSelectNum != 2) {
            mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, NULL, 0, 0);
            mLastSelectNum = mSelectNum;
            mSelectNum++;
            dataSelectAnmSet();  // run the quest log selection animation
            mDataSelProc = DATASELPROC_DATA_SELECT_MOVE_ANIME;
        }
    }
}

static s32 YnSelStartFrameTbl[2] = {
    1251,
    1236,
};

static s32 YnSelEndFrameTbl[2] = {
    1236,
    1251,
};

static u16 msgTbl[3] = {
    0x0040,
    0x0041,
    0x0042,
};

void dFile_select_c::dataSelectStart() {
    mSelIcon->setAlphaRate(0.0f);

    if (mIsNoData[mSelectNum]) {
        headerTxtSet(0x52, 0, 0);
        selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
        yesnoMenuMoveAnmInitSet(0x473, 0x47d);
        selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.select_box_appear_frames);

        mSelDt.ScrDt->search('ken_00')->hide();
        mSelDt.ScrDt->search('ken_01')->hide();
        mSelDt.ScrDt->search('ken_02')->hide();
        mSelDt.ScrDt->search('tate_00')->hide();
        mSelDt.ScrDt->search('tate_01')->hide();
        mSelDt.ScrDt->search('tate_02')->hide();
        mSelDt.ScrDt->search('fuku_00')->hide();
        mSelDt.ScrDt->search('fuku_01')->hide();
        mSelDt.ScrDt->search('fuku_02')->hide();
        mpFileSelect3d->drawOff();

        mDataSelProc = DATASELPROC_SELECT_DATA_OPENERASE_MOVE;
    } else if (mIsDataNew[mSelectNum] != 0) {
        #if PLATFORM_GCN
        dComIfGs_setNewFile(128);
        #endif

        dComIfGs_setDataNum(mSelectNum);
        mDoAud_seStart(Z2SE_SY_NEW_FILE, NULL, 0, 0);
        headerTxtSet(0x385, 1, 0);
        fileRecScaleAnmInitSet2(1.0f, 0.0f);
        nameMoveAnmInitSet(3359, 3369);

        mSelFileMoyoPane[mSelectNum]->setAlpha(0);
        mSelFileGoldPane[mSelectNum]->setAlpha(0);
        mSelFileGold2Pane[mSelectNum]->setAlpha(0);

        char namebuf[32];
        dMeter2Info_getString(0x382, namebuf, NULL);
        dComIfGs_setPlayerName(namebuf);
        mpName->setNextNameStr(dComIfGs_getPlayerName());
        mpName->initial();
        modoruTxtChange(1);

        mDataSelProc = DATASELPROC_SELECT_DATA_NAME_MOVE;
    } else {
        #if PLATFORM_GCN
        dComIfGs_setNewFile(0);
        #endif

        mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
        #if PLATFORM_GCN
        mSelectMenuNum = 1;
        #else
        mSelectMenuNum = 0xFF;
        #endif

        mLastSelectMenuNum = mSelectMenuNum;

        #if PLATFORM_SHIELD
        ketteiTxtDispAnmInit(0);
        #endif

        headerTxtSet(msgTbl[mSelectNum], 1, 0);
        selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
        menuMoveAnmInitSet(799, 809);
        selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.select_box_appear_frames);
        makeRecInfo(mSelectNum);

        mDataSelProc = DATASELPROC_SELECT_DATA_OPEN_MOVE;
    }

    modoruTxtDispAnmInit(1);
}

void dFile_select_c::selectDataMoveAnmInitSet(int param_0, int param_1) {
    mSelFilePanes[0]->getPanePtr()->setAnimation(mBaseMoveAnm);
    mSelFilePanes[1]->getPanePtr()->setAnimation(mBaseMoveAnm);
    mSelFilePanes[2]->getPanePtr()->setAnimation(mBaseMoveAnm);
    mBaseSubPane->setAnimation(mBaseMoveAnm);

    field_0x00e0[mSelectNum] = param_0;
    field_0x00ec = param_1;
    mBaseMoveAnm->setFrame(field_0x00e0[mSelectNum]);
    mSelFilePanes[0]->getPanePtr()->animationTransform();
    mSelFilePanes[1]->getPanePtr()->animationTransform();
    mSelFilePanes[2]->getPanePtr()->animationTransform();

    mBaseSubPane->animationTransform();
}

bool dFile_select_c::selectDataMoveAnm() {
    bool ret;

    if (field_0x00e0[mSelectNum] != field_0x00ec) {
        if (field_0x00e0[mSelectNum] < field_0x00ec) {
            field_0x00e0[mSelectNum] += 2;

            if (field_0x00e0[mSelectNum] > field_0x00ec)
                field_0x00e0[mSelectNum] = field_0x00ec;

        } else {
            field_0x00e0[mSelectNum] -= 2;

            if (field_0x00e0[mSelectNum] < field_0x00ec)
                field_0x00e0[mSelectNum] = field_0x00ec;
        }

        mBaseMoveAnm->setFrame(field_0x00e0[mSelectNum]);

        for (int i = 0; i < 3; i++) {
            mSelFilePanes[i]->getPanePtr()->animationTransform();
        }
        mBaseSubPane->animationTransform();
    }

    if (field_0x00e0[mSelectNum] == field_0x00ec) {
        for (int i = 0; i < 3; i++) {
            mSelFilePanes[i]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        mBaseSubPane->setAnimation((J2DAnmTransform*)NULL);
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

void dFile_select_c::dataSelectAnmSet() {
    if (mSelectNum != 0xff) {
        mSelFilePanes[mSelectNum]->getPanePtr()->setAnimation(mBaseMoveAnm);
        field_0x00e0[mSelectNum] = SelStartFrameTbl[mSelectNum];
        mBaseMoveAnm->setFrame(field_0x00e0[mSelectNum]);
        mSelFilePanes[mSelectNum]->getPanePtr()->animationTransform();
        mSelFilePane_Book_l[mSelectNum]->alphaAnimeStart(0);
    }

    if (mLastSelectNum != 0xff) {
        selectWakuAlpahAnmInit(mLastSelectNum, 0xff, 0, g_fsHIO.select_box_appear_frames);
        mSelFilePanes[mLastSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0088);  // wrong?
        field_0x00e0[mLastSelectNum] = SelEndFrameTbl[mLastSelectNum];
        field_0x0088->setFrame(field_0x00e0[mLastSelectNum]);
        mSelFilePanes[mLastSelectNum]->getPanePtr()->animationTransform();
        mSelFilePane_Book_l[mLastSelectNum]->alphaAnimeStart(0);
        mSelIcon->setAlphaRate(0.0f);
    }
}

void dFile_select_c::dataSelectMoveAnime() {
    bool iVar7 = true;
    bool iVar6 = true;
    bool bVar1 = true;

    if (mLastSelectNum != 0xff) {
        iVar7 = mSelFilePane_Book_l[mLastSelectNum]->alphaAnime(g_fsHIO.base_effect_appear_frames, 0xff, 0, 1);
        iVar6 = selectWakuAlpahAnm(mLastSelectNum);

        if (field_0x00e0[mLastSelectNum] != SelStartFrameTbl[mLastSelectNum]) {
            field_0x00e0[mLastSelectNum] -= 2;

            if (field_0x00e0[mLastSelectNum] < SelStartFrameTbl[mLastSelectNum])
                field_0x00e0[mLastSelectNum] = SelStartFrameTbl[mLastSelectNum];

            field_0x0088->setFrame(field_0x00e0[mLastSelectNum]);
            mSelFilePanes[mLastSelectNum]->getPanePtr()->animationTransform();
            bVar1 = false;
        }
    }

    bool iVar5 = true;
    bool bVar2 = true;

    if (mSelectNum != 0xff) {
        iVar5 = mSelFilePane_Book_l[mSelectNum]->alphaAnime(g_fsHIO.base_effect_appear_frames, 0, 0xff, 1);

        if (field_0x00e0[mSelectNum] != SelEndFrameTbl[mSelectNum]) {
            field_0x00e0[mSelectNum] += 2;

            if (field_0x00e0[mSelectNum] > SelEndFrameTbl[mSelectNum])
                field_0x00e0[mSelectNum] = SelEndFrameTbl[mSelectNum];

            mBaseMoveAnm->setFrame(field_0x00e0[mSelectNum]);
            mSelFilePanes[mSelectNum]->getPanePtr()->animationTransform();
            bVar2 = false;
        }
    }

    if (iVar7 == true && iVar6 == true && bVar1 == true && iVar5 == true && bVar2 == true) {
        if (mSelectNum != 0xff) {
            mSelFilePanes[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            selFileCursorShow();
        }

        if (mLastSelectNum != 0xff) {
            mSelFilePanes[mLastSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }

        mDataSelProc = DATASELPROC_DATA_SELECT;
    }
}

void dFile_select_c::makeRecInfo(u8 i_dataNo) {
    dSv_save_c* pSave = (dSv_save_c*)&mSaveData[i_dataNo];

    J2DPane* ken0 = mSelDt.ScrDt->search('ken_00');
    J2DPane* ken1 = mSelDt.ScrDt->search('ken_01');
    ken0->hide();
    ken1->hide();

    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_SWORD)) {
        ken0->hide();
        ken1->show();
    } else if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_WOOD_STICK) &&
               !pSave->getEvent().isEventBit(dSv_event_flag_c::F_0026))
    {
        ken0->show();
        ken1->hide();
    }

    J2DPane* ken2 = mSelDt.ScrDt->search('ken_02');
    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_MASTER_SWORD) ||
        pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_LIGHT_SWORD))
    {
        ken2->show();
    } else {
        ken2->hide();
    }

    J2DPane* tate0 = mSelDt.ScrDt->search('tate_00');
    J2DPane* tate1 = mSelDt.ScrDt->search('tate_01');
    tate0->hide();
    tate1->hide();

    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_SHIELD)) {
        tate0->show();
        tate1->hide();
    } else if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_WOOD_SHIELD)) {
        tate0->hide();
        tate1->show();
    }

    J2DPane* tate2 = mSelDt.ScrDt->search('tate_02');
    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_HYLIA_SHIELD)) {
        tate2->show();
    } else {
        tate2->hide();
    }

    J2DPane* fuku0 = mSelDt.ScrDt->search('fuku_00');
    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_WEAR_KOKIRI)) {
        fuku0->show();
    } else {
        fuku0->hide();
    }

    J2DPane* fuku1 = mSelDt.ScrDt->search('fuku_01');
    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_WEAR_ZORA)) {
        fuku1->show();
    } else {
        fuku1->hide();
    }

    J2DPane* fuku2 = mSelDt.ScrDt->search('fuku_02');
    if (pSave->getPlayer().getGetItem().isFirstBit(fpcNm_ITEM_ARMOR)) {
        fuku2->show();
    } else {
        fuku2->hide();
    }

    u8 mirrorsCollected = 0;
    for (int i = 0; i < 4; i++) {
        if (!pSave->getPlayer().getCollect().isCollectMirror(i)) {
            break;
        }
        mirrorsCollected++;
    }

    u8 crystalCollected = 0;
    for (int i = 0; i < 4; i++) {
        if (!pSave->getPlayer().getCollect().isCollectCrystal(i)) {
            break;
        }
        crystalCollected++;
    }

    if ((crystalCollected == 0 && mirrorsCollected == 0) ||
        (pSave->getEvent().isEventBit(dSv_event_flag_c::M_071) && !pSave->getPlayer().getCollect().isCollectMirror(0)) ||
        (pSave->getEvent().isEventBit(dSv_event_flag_c::F_0354) && !pSave->getPlayer().getCollect().isCollectCrystal(3)))
    {
        mpFileSelect3d->drawOff();
    } else {
        mpFileSelect3d->freeHeap();
        if (pSave->getPlayer().getCollect().isCollectCrystal(3) || mirrorsCollected == 0) {
            mpFileSelect3d->_create(0, crystalCollected);
        } else {
            mpFileSelect3d->_create(mirrorsCollected, 0);
        }
    }
}

void dFile_select_c::selectDataOpenMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelDataMove = selectDataMoveAnm();
    bool isMenuMove = menuMoveAnm();
    bool isModoruTxtDisp = modoruTxtDispAnm();
    bool isSelWakuAlpha = selectWakuAlpahAnm(mSelectNum);

    if (isHeaderTxtChange == true && isSelDataMove == true && isMenuMove == true &&
        isModoruTxtDisp == true && isSelWakuAlpha == true)
    {
        mSelectMenuNum = 1;
        m3mSelPane[mSelectMenuNum]->getPanePtr()->setAnimation(m3mBck);
        m3mBck->setFrame(MenuSelStartFrameTbl[mSelectMenuNum]);
        m3mSelPane[mSelectMenuNum]->getPanePtr()->animationTransform();
        m3mSelPane[mSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        menuCursorShow();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectMenuNum;
        #endif

        mDataSelProc = DATASELPROC_MENU_SELECT;
    }
}

void dFile_select_c::selectDataNameMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isFileRecScale = fileRecScaleAnm2();
    bool isNameMove = nameMoveAnm();
    bool isModoruTxtDisp = modoruTxtDispAnm();

    if (isHeaderTxtChange == true && isFileRecScale == true && isNameMove == true &&
        isModoruTxtDisp == true)
    {
        mDataSelProc = DATASELPROC_NAME_INPUT_WAIT;
    }
}

void dFile_select_c::selectDataOpenEraseMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelDataMove = selectDataMoveAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    bool isModoruTxtDisp = modoruTxtDispAnm();
    bool isSelWakuAlpha = selectWakuAlpahAnm(mSelectNum);

    if (isHeaderTxtChange == true && isSelDataMove == true && isModoruTxtDisp == true &&
        isSelWakuAlpha == true)
    {
        mCommand = 2;
        yesnoCursorShow();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = field_0x0268;
        #endif

        mDataSelProc = DATASELPROC_YES_NO_SELECT;
    }
}

// Handles selecting between copy / start / delete menus in quest log
void dFile_select_c::menuSelect() {
    stick->checkTrigger();

    // if a was pressed, do the menu selection process
    if (mDoCPd_c::getTrigA(PAD_1)) {
        menuSelectStart();
    } 
    // if b was pressed, do the menu cancel process
    else if (mDoCPd_c::getTrigB(PAD_1)) {
        menuSelectCansel();
    } else if (stick->checkRightTrigger()) {
        if (!mIsDataNew[mSelectNum] && mSelectMenuNum != 0) {
            mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
            mLastSelectMenuNum = mSelectMenuNum;
            mSelectMenuNum--;
            menuSelectAnmSet();  // run the menu selection animation
            mDataSelProc = DATASELPROC_MENU_SELECT_MOVE_ANM;
        }
    } else if (stick->checkLeftTrigger()) {
        if (!mIsDataNew[mSelectNum] && mSelectMenuNum != 2) {
            mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
            mLastSelectMenuNum = mSelectMenuNum;
            mSelectMenuNum++;
            menuSelectAnmSet();  // run the menu selection animation
            mDataSelProc = DATASELPROC_MENU_SELECT_MOVE_ANM;
        }
    }
}

// Handles copy / start / delete actions depending on which menu is selected from menuSelect
void dFile_select_c::menuSelectStart() {
    mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);

    if (mSelectMenuNum == 1) {
        dComIfGs_setCardToMemory((u8*)mSaveData, mSelectNum);
        mIsSelectEnd = true;
        mDataSelProc = DATASELPROC_NEXT_MODE_WAIT;
        dComIfGs_setDataNum(mSelectNum);
    } else if (mSelectMenuNum == 0) {
        mSelIcon->setAlphaRate(0.0f);
        yesnoMenuMoveAnmInitSet(0x473, 0x47d);
        headerTxtSet(0x49, 0, 0);
        mCommand = 1;
        mDataSelProc = DATASELPROC_TO_ERASE_PANE_MOVE;
    } else if (mSelectMenuNum == 2) {
        mCpDataNum = mSelectNum;
        menuMoveAnmInitSet(0x329, 799);
        headerTxtSet(0x44, 0, 0);
        mSelIcon->setAlphaRate(0.0f);
        selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectNum], SelOpenStartFrameTbl[mSelectNum]);
        selectWakuAlpahAnmInit(mSelectNum, 0, 255, g_fsHIO.select_box_appear_frames);
        copySelMoveAnmInitSet(0xd1f, 0xd29);
        mCommand = 0;
        mDataSelProc = DATASELPROC_TO_COPY_PANE_MOVE;
    }
}

void dFile_select_c::menuSelectCansel() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0);
    selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectNum], SelOpenStartFrameTbl[mSelectNum]);
    menuMoveAnmInitSet(0x329, 799);
    modoruTxtDispAnmInit(0);
    headerTxtSet(0x43, 1, 0);
    mSelIcon->setAlphaRate(0.0f);
    mDataSelProc = DATASELPROC_BACK_SELECT_MOVE;
}

void dFile_select_c::menuMoveAnmInitSet(int param_0, int param_1) {
    if (param_0 == 799) {
        field_0x0283 = true;

        for (int i = 0; i < 3; i++) {
            if (i == mSelectMenuNum) {
                static_cast<J2DTextBox*>(m3mSelTextPane[i]->getPanePtr())->setWhite(JUtility::TColor(255, 255, 255, 255));
            } else {
                static_cast<J2DTextBox*>(m3mSelTextPane[i]->getPanePtr())->setWhite(JUtility::TColor(150, 150, 150, 255));
            }
        }
    }

    m3mMenuPane->setAnimation(m3mBck);
    field_0x0358 = param_0;
    field_0x035c = param_1;
    m3mBck->setFrame(field_0x0358);
    m3mMenuPane->animationTransform();
}

bool dFile_select_c::menuMoveAnm() {
    bool ret;

    if (field_0x0358 != field_0x035c) {
        if (field_0x0358 < field_0x035c) {
            field_0x0358 += 2;

            if (field_0x0358 > field_0x035c)
                field_0x0358 = field_0x035c;
        } else {
            field_0x0358 -= 2;

            if (field_0x0358 < field_0x035c)
                field_0x0358 = field_0x035c;
        }

        m3mBck->setFrame(field_0x0358);
        m3mMenuPane->animationTransform();
        ret = false;
    } else {
        m3mMenuPane->setAnimation((J2DAnmTransform*)NULL);
        if (field_0x035c == 0x0329) {
            field_0x0360 = true;
            field_0x0283 = false;
        } else {
            field_0x0360 = false;

            if (mSelectMenuNum != 0xFF) {
                m3mSelPane_mo[mSelectMenuNum]->setAlpha(0);
                m3mSelPane_g[mSelectMenuNum]->setAlpha(0);
                m3mSelPane_gr[mSelectMenuNum]->setAlpha(0);
            }
        }

        ret = true;
    }

    return ret;
}

void dFile_select_c::menuSelectAnmSet() {
    if (mSelectMenuNum != 0xFF) {
        m3mSelPane[mSelectMenuNum]->getPanePtr()->setAnimation(m3mBck);
        field_0x034c[mSelectMenuNum] = MenuSelEndFrameTbl[mSelectMenuNum];
        m3mBck->setFrame(field_0x034c[mSelectMenuNum]);
        m3mSelPane[mSelectMenuNum]->getPanePtr()->animationTransform();
    }

    if (mLastSelectMenuNum != 0xFF) {
        m3mSelPane[mLastSelectMenuNum]->getPanePtr()->setAnimation(m3mBck2);
        field_0x034c[mLastSelectMenuNum] = MenuSelStartFrameTbl[mLastSelectMenuNum];
        m3mBck2->setFrame(field_0x034c[mLastSelectMenuNum]);
        m3mSelPane[mLastSelectMenuNum]->getPanePtr()->animationTransform();
        menuWakuAlpahAnmInit(mLastSelectMenuNum, 0xFF, 0, g_fsHIO.select_box_appear_frames);
        mSelIcon->setAlphaRate(0.0f);
    }
}

void dFile_select_c::menuSelectMoveAnm() {
    bool tmp1 = true;

    if (mSelectMenuNum != 0xFF &&
        field_0x034c[mSelectMenuNum] != MenuSelStartFrameTbl[mSelectMenuNum])
    {
        if (field_0x034c[mSelectMenuNum] > MenuSelStartFrameTbl[mSelectMenuNum]) {
            field_0x034c[mSelectMenuNum] -= 2;

            if (field_0x034c[mSelectMenuNum] < MenuSelStartFrameTbl[mSelectMenuNum])
                field_0x034c[mSelectMenuNum] = MenuSelStartFrameTbl[mSelectMenuNum];
        } else {
            field_0x034c[mSelectMenuNum] += 2;

            if (field_0x034c[mSelectMenuNum] > MenuSelStartFrameTbl[mSelectMenuNum])
                field_0x034c[mSelectMenuNum] = MenuSelStartFrameTbl[mSelectMenuNum];
        }

        m3mBck->setFrame(field_0x034c[mSelectMenuNum]);
        m3mSelPane[mSelectMenuNum]->getPanePtr()->animationTransform();
        tmp1 = false;
    }

    bool tmp2 = true;
    bool tmp3 = true;

    if (mLastSelectMenuNum != 0xFF) {
        if (field_0x034c[mLastSelectMenuNum] != MenuSelEndFrameTbl[mLastSelectMenuNum]) {
            if (field_0x034c[mLastSelectMenuNum] < MenuSelEndFrameTbl[mLastSelectMenuNum]) {
                field_0x034c[mLastSelectMenuNum] += 2;

                if (field_0x034c[mLastSelectMenuNum] > MenuSelEndFrameTbl[mLastSelectMenuNum])
                    field_0x034c[mLastSelectMenuNum] = MenuSelEndFrameTbl[mLastSelectMenuNum];

            } else {
                field_0x034c[mLastSelectMenuNum] -= 2;

                if (field_0x034c[mLastSelectMenuNum] < MenuSelEndFrameTbl[mLastSelectMenuNum])
                    field_0x034c[mLastSelectMenuNum] = MenuSelEndFrameTbl[mLastSelectMenuNum];
            }
            m3mBck2->setFrame(field_0x034c[mLastSelectMenuNum]);
            m3mSelPane[mLastSelectMenuNum]->getPanePtr()->animationTransform();
            tmp2 = false;
        }

        tmp3 = menuWakuAlpahAnm(mLastSelectMenuNum);
    }

    if (tmp1 == true && tmp2 == true && tmp3 == true) {
        if (mSelectMenuNum != 0xFF) {
            m3mSelPane[mSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            menuCursorShow();
        }

        if (mLastSelectMenuNum != 0xFF) {
            m3mSelPane[mLastSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }

        mDataSelProc = DATASELPROC_MENU_SELECT;
    }
}

void dFile_select_c::ToNameMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isNameMove = nameMoveAnm();
    bool isMenuMove = menuMoveAnm();
    bool isFileRecScale = fileRecScaleAnm();
    bool isFileInfoScale = fileInfoScaleAnm();

    if (isHeaderTxtChange == true && isNameMove == true && isMenuMove == true &&
        (isFileRecScale == true && isFileInfoScale == true))
    {
        mDataSelProc = DATASELPROC_NAME_INPUT_WAIT;
    }
}

void dFile_select_c::ToNameMove2() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isNameMove = nameMoveAnm();
    bool isErrorMove = errorMoveAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    bool isKetteiTxtDisp = ketteiTxtDispAnm();
    bool isModoruTxtDisp = modoruTxtDispAnm();

    if (isHeaderTxtChange == true && isNameMove == true && isErrorMove == true &&
        (isYnMenuMove == true && isKetteiTxtDisp == true && isModoruTxtDisp == true))
    {
        mDataSelProc = DATASELPROC_NAME_INPUT_WAIT;
    }
}

void dFile_select_c::nameInputWait() {
    mpName->showIcon();
    mDataSelProc = DATASELPROC_NAME_INPUT;
}

void dFile_select_c::nameInput() {
    mpName->_move();

    if (mpName->getCurPos() == 0) {
        if (field_0x024c != 0) {
            modoruTxtChange(0);
        }
    } else if (field_0x024c == 0) {
        modoruTxtChange(1);
    }

    switch (mpName->isInputEnd()) {
    case 1:
        if (dComIfGs_getNoFile() == 1) {
            field_0x021e = 0;
            mDataSelProc = DATASELPROC_MEMCARD_CHECK_MAIN;
            mCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
        } else {
            headerTxtSet(0x43, 1, 0);
            fileRecScaleAnmInitSet2(0.0f, 1.0f);
            nameMoveAnmInitSet(0xd29, 0xd1f);
            modoruTxtDispAnmInit(0);
            mDataSelProc = DATASELPROC_NAME_TO_DATA_SELECT_MOVE;
        }
        break;
    case 2:
        dComIfGs_setPlayerName(mpName->getInputStrPtr());
        mpName->hideIcon();
        mFadeTimer = 15;

        #if PLATFORM_WII || PLATFORM_SHIELD
        mDoGph_gInf_c::startFadeOut(15);
        mFadeFlag = true;
        #endif

        mDataSelProc = DATASELPROC_NAME_INPUT_FADE;
    }
}

void dFile_select_c::nameToDataSelectMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isFileRecScale = fileRecScaleAnm2();
    bool isNameMove = nameMoveAnm();
    bool isModoruTxtDisp = modoruTxtDispAnm();

    if (isHeaderTxtChange == true && isFileRecScale == true && isNameMove == true && isModoruTxtDisp == true)
    {
        modoruTxtChange(0);
        selFileCursorShow();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectNum;
        #endif

        mDataSelProc = DATASELPROC_DATA_SELECT;
    }
}

void dFile_select_c::nameInputFade() {
    mFadeTimer--;

    #if PLATFORM_GCN
    u8 alpha = (1.0f - (mFadeTimer / 15.0f)) * 255.0f;
    mpFadePict->setAlpha(alpha);
    #endif

    if (mFadeTimer == 0) {
        char name[32];
        headerTxtSet(900, 1, 1);
        dMeter2Info_getString(899, name, NULL);
        dComIfGs_setHorseName(name);
        mpName->setNextNameStr(dComIfGs_getHorseName());
        mpName->initial();

        mFadeTimer = 15;

        #if PLATFORM_WII || PLATFORM_SHIELD
        mDoGph_gInf_c::startFadeIn(15);
        mFadeFlag = false;
        #endif

        mDataSelProc = DATASELPROC_NAME_INPUT2_MOVE;
    }
}

void dFile_select_c::nameInput2Move() {
    if (!mDoRst::isReset()) {
        mFadeTimer--;

        #if PLATFORM_GCN
        u8 alpha = (mFadeTimer / 15.0f) * 255.0f;
        mpFadePict->setAlpha(alpha);
        #endif

        if (mFadeTimer == 0) {
            mpName->showIcon();
            mDataSelProc = DATASELPROC_NAME_INPUT2;
        }
    }
}

void dFile_select_c::nameInput2() {
    mpName->_move();
    if (mpName->getCurPos() == 0) {
        if (field_0x024c != 0) {
            modoruTxtChange(0);
        }
    } else if (field_0x024c == 0) {
        modoruTxtChange(1);
    }

    switch (mpName->isInputEnd()) {
    case 1:
        mpName->hideIcon();
        mFadeTimer = 15;

        #if PLATFORM_WII || PLATFORM_SHIELD
        mDoGph_gInf_c::startFadeOut(15);
        mFadeFlag = true;
        #endif

        mDataSelProc = DATASELPROC_BACK_NAME_INPUT_MOVE0;
        break;
    case 2:
        dComIfGs_setHorseName(mpName->getInputStrPtr());
        mIsSelectEnd = true;
        mDataSelProc = DATASELPROC_NEXT_MODE_WAIT;
    }
}

void dFile_select_c::backNameInputMove0() {
    mFadeTimer--;

    #if PLATFORM_GCN
    u8 alpha = (1.0f - (mFadeTimer / 15.0f)) * 255.0f;
    mpFadePict->setAlpha(alpha);
    #endif

    if (mFadeTimer == 0) {
        headerTxtSet(901, 1, 1);
        mpName->setNextNameStr(dComIfGs_getPlayerName());
        mpName->initial();
        modoruTxtChange(1);

        mFadeTimer = 15;

        #if PLATFORM_WII || PLATFORM_SHIELD
        mDoGph_gInf_c::startFadeIn(15);
        mFadeFlag = false;
        #endif

        mDataSelProc = DATASELPROC_BACK_NAME_INPUT_MOVE;
    }
}

void dFile_select_c::backNameInputMove() {
    if (!mDoRst::isReset()) {
        mFadeTimer--;

        #if PLATFORM_GCN
        u8 alpha = (mFadeTimer / 15.0f) * 255.0f;
        mpFadePict->setAlpha(alpha);
        #endif

        if (mFadeTimer == 0) {
            modoruTxtChange(1);
            mDataSelProc = DATASELPROC_NAME_INPUT_WAIT;
        }
    }
}

void dFile_select_c::ToCopyPaneMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isMenuMove = menuMoveAnm();
    bool isSelDataMove = selectDataMoveAnm();
    bool isSelWakuAlpha = selectWakuAlpahAnm(mSelectNum);
    bool isNameMove = nameMoveAnm();

    if (isHeaderTxtChange == true && isMenuMove == true && isSelDataMove == true &&
        isSelWakuAlpha == true && isNameMove == true)
    {
        field_0x026b = 0;
        Vec pos = mCpSelPane[field_0x026b]->getGlobalVtxCenter(false, 0);
        mSelIcon2->setPos(pos.x, pos.y, mCpSelPane[field_0x026b]->getPanePtr(), true);
        mSelIcon2->setAlphaRate(1.0f);

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = field_0x026b;
        #endif

        mDataSelProc = DATASELPROC_COPY_DATA_TO_SELECT;
    }
}

void dFile_select_c::ToErasePaneMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();

    if (isHeaderTxtChange == true && isYnMenuMove == true) {
        yesnoCursorShow();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = field_0x0268;
        #endif

        mDataSelProc = DATASELPROC_YES_NO_SELECT;
    }
}

void dFile_select_c::backSelectMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelDataMove = selectDataMoveAnm();
    bool isMenuMove = menuMoveAnm();
    bool isModoruTxtDisp = modoruTxtDispAnm();

    if (isHeaderTxtChange == true && isSelDataMove == true && isMenuMove == true &&
        isModoruTxtDisp == true)
    {
        selFileCursorShow();
        mpFileSelect3d->drawOff();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectNum;
        #endif

        mDataSelProc = DATASELPROC_DATA_SELECT;
    }
}

void dFile_select_c::copySelMoveAnmInitSet(int param_0, int param_1) {
    if (param_0 == 3359) {
        mCpSelPane_moyo[0]->setAlpha(0xff);
        mCpSelPane_gold[0]->setAlpha(0xff);
        mCpSelPane_gold2[0]->setAlpha(0xff);
        mCpSelPane_moyo[1]->setAlpha(0);
        mCpSelPane_gold[1]->setAlpha(0);
        mCpSelPane_gold2[1]->setAlpha(0);

        mCpSelPane[0]->getPanePtr()->setAnimation(mCpSelBck);
        mCpSelBck->setFrame(99.0f);
        mCpSelPane[0]->getPanePtr()->animationTransform();
        mCpSelPane[0]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);

        mCpSelPane[1]->getPanePtr()->setAnimation(mCpSelBck2);
        mCpSelBck2->setFrame(109.0f);
        mCpSelPane[1]->getPanePtr()->animationTransform();
        mCpSelPane[1]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);

        setSaveDataForCopySel();
        mCpSel.isShow = true;
    }

    mNameBasePane->setAnimation(field_0x0094);
    field_0x0120 = param_0;
    field_0x0124 = param_1;
    field_0x0094->setFrame(field_0x0120);
    mNameBasePane->animationTransform();
}

void dFile_select_c::setSaveDataForCopySel() {
    static u64 l_tagName101[2] = {'w_nun01', 'w_nun02'};
    static char* l_numTex[3] = {"tt_1_metal_40x40.bti", "tt_2_metal_40x40.bti",
                                "tt_3_metal_40x40.bti"};
    SaveDataBuf* pSave = mSaveData;
    int notSelectedIndex = 0;
    for (int i = 0; i < 3; i++) {
        if (i == mSelectNum) {
            pSave++;
            continue;
        }

        J2DPicture* numPict = (J2DPicture*)mCpSel.Scr->search(l_tagName101[notSelectedIndex]);
        numPict->changeTexture(l_numTex[i], 0);
        CPaneMgrAlpha* datBase = mCpFileInfo[notSelectedIndex]->getDatBase();
        CPaneMgrAlpha* noDatBase = mCpFileInfo[notSelectedIndex]->getNoDatBase();

        int saveResult = mCpFileInfo[notSelectedIndex]->setSaveData((dSv_save_c*)*pSave, mIsNoData[i] == false, i);
        if (saveResult == -1 || saveResult == 1) {
            datBase->hide();
            noDatBase->show();
        } else {
            datBase->show();
            noDatBase->hide();
        }
 
        pSave++;
        notSelectedIndex++;
    }
}

void dFile_select_c::copyDataToSelect() {
    stick->checkTrigger();

    if (mDoCPd_c::getTrigA(PAD_1)) {
        copyDataToSelectStart();
    } else if (mDoCPd_c::getTrigB(PAD_1)) {
        copyDataToSelectCansel();
    } else if (stick->checkUpTrigger()) {
        if (field_0x026b != 0) {
            mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, NULL, 0, 0);
            field_0x026c = field_0x026b;
            field_0x026b = 0;
            copyDataToSelectMoveAnmSet();
            mDataSelProc = DATASELPROC_COPY_DATA_TO_SELECT_MOVE_ANM;
        }
    } else if (stick->checkDownTrigger() && field_0x026b != 1) {
        mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, NULL, 0, 0);
        field_0x026c = field_0x026b;
        field_0x026b = 1;
        copyDataToSelectMoveAnmSet();
        mDataSelProc = DATASELPROC_COPY_DATA_TO_SELECT_MOVE_ANM;
    }
}

void dFile_select_c::copyDataToSelectStart() {
    mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);

    mCpDataToNum = getCptoNum(field_0x026b);
    mSelIcon2->setAlphaRate(0.0f);

    if (mIsDataNew[mCpDataToNum] != 0) {
        headerTxtSet(70, 0, 0);
        mpFileWarning->openInit();
        mCopyEfPane[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
        mCopyEfPane[mSelectNum]->alphaAnimeStart(0);
        mCopyEfPane[mSelectNum]->setAlpha(0);
        mCopyEfPane[mCpDataToNum]->getPanePtr()->scale(1.0f, 1.0f);
        mCopyEfPane[mCpDataToNum]->alphaAnimeStart(0);
        mCopyEfPane[mCpDataToNum]->setAlpha(0);

        selectWakuAlpahAnmInit(mCpDataToNum, 0, 0xff, g_fsHIO.select_box_appear_frames);
        copySelMoveAnmInitSet(3369, 3359);

        mCpEffBtkFrame = 0;
        mCpDtEffBrkFrame = 0;
        field_0x0209 = 1;
        field_0x03b1 = 1;
        modoruTxtDispAnmInit(0);
        ketteiTxtDispAnmInit(0);

        mDataSelProc = DATASELPROC_CMD_EXEC_PANE_MOVE0;
    } else {
        yesnoMenuMoveAnmInitSet(1139, 1149);
        headerTxtSet(69, 0, 0);
        mDataSelProc = DATASELPROC_COPY_TO_SEL_PANE_MOVE;
    }
}

void dFile_select_c::copyDataToSelectCansel() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, 0, 0, 0);
    selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
    menuMoveAnmInitSet(799, 809);
    copySelMoveAnmInitSet(3369, 3359);
    headerTxtSet(msgTbl[mSelectNum], 1, 0);
    mSelIcon2->setAlphaRate(0.0f);
    selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.select_box_appear_frames);
    mDataSelProc = DATASELPROC_COPY_TO_SEL_BACK;
}

void dFile_select_c::copyDataToSelectMoveAnmSet() {
    if (field_0x026b != 0xff) {
        mCpSelPane_book[field_0x026b]->alphaAnimeStart(0);
        mCpSelPane[field_0x026b]->getPanePtr()->setAnimation(mCpSelBck);
        field_0x02b4[field_0x026b] = 109;
        mCpSelBck->setFrame(field_0x02b4[field_0x026b]);
        mCpSelPane[field_0x026b]->getPanePtr()->animationTransform();
    }

    if (field_0x026c != 0xff) {
        copySelectWakuAlpahAnmInit(field_0x026c, 0xff, 0, g_fsHIO.select_box_appear_frames);
        mCpSelPane_book[field_0x026c]->alphaAnimeStart(0);
        mCpSelPane[field_0x026c]->getPanePtr()->setAnimation(mCpSelBck2);
        field_0x02b4[field_0x026c] = 99;
        mCpSelBck2->setFrame(field_0x02b4[field_0x026c]);
        mCpSelPane[field_0x026c]->getPanePtr()->animationTransform();
        mSelIcon2->setAlphaRate(0.0f);
    }
}

void dFile_select_c::copyDataToSelectMoveAnm() {
    bool iVar7 = true;
    bool iVar6 = true;
    bool bVar1 = true;

    if (field_0x026c != 0xff) {
        iVar7 = mCpSelPane_book[field_0x026c]->alphaAnime(g_fsHIO.base_effect_appear_frames, 0xff, 0, 1);
        iVar6 = copySelectWakuAlpahAnm(field_0x026c);
        if (field_0x02b4[field_0x026c] != 109) {
            field_0x02b4[field_0x026c] += 2;
            if (field_0x02b4[field_0x026c] > 109) {
                field_0x02b4[field_0x026c] = 109;
            }
            mCpSelBck2->setFrame(field_0x02b4[field_0x026c]);
            mCpSelPane[field_0x026c]->getPanePtr()->animationTransform();
            bVar1 = false;
        }
    }

    bool iVar5 = true;
    bool bVar2 = true;
    if (field_0x026b != 0xff) {
        iVar5 = mCpSelPane_book[field_0x026b]->alphaAnime(g_fsHIO.base_effect_appear_frames, 0, 0xff, 1);
        if (field_0x02b4[field_0x026b] != 99) {
            field_0x02b4[field_0x026b] -= 2;
            if (field_0x02b4[field_0x026b] < 99) {
                field_0x02b4[field_0x026b] = 99;
            }
            mCpSelBck->setFrame(field_0x02b4[field_0x026b]);
            mCpSelPane[field_0x026b]->getPanePtr()->animationTransform();
            bVar2 = false;
        }
    }

    if (iVar7 == true && iVar6 == true && bVar1 == true && iVar5 == true && bVar2 == true) {
        if (field_0x026c != 0xff) {
            mCpSelPane[field_0x026c]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }

        if (field_0x026b != 0xff) {
            mCpSelPane[field_0x026b]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            mCpSelPane_moyo[field_0x026b]->setAlpha(0xff);
            mCpSelPane_gold[field_0x026b]->setAlpha(0xff);
            mCpSelPane_gold2[field_0x026b]->setAlpha(0xff);

            Vec pos = mCpSelPane[field_0x026b]->getGlobalVtxCenter(false, 0);
            mSelIcon2->setPos(pos.x, pos.y, mCpSelPane[field_0x026b]->getPanePtr(), true);
            mSelIcon2->setAlphaRate(1.0f);
        }

        mDataSelProc = DATASELPROC_COPY_DATA_TO_SELECT;
    }
}

void dFile_select_c::copySelectWakuAlpahAnmInit(u8 i_idx, u8 param_2, u8 param_3, u8 param_4) {
    if (i_idx != 0xff) {
        mCpSelPane_moyo[i_idx]->alphaAnimeStart(0);
        mCpSelPane_gold[i_idx]->alphaAnimeStart(0);
        mCpSelPane_gold2[i_idx]->alphaAnimeStart(0);
        field_0x02e0[i_idx] = param_2;
        field_0x02e2[i_idx] = param_3;
        field_0x02e4[i_idx] = param_4;
    }
}

bool dFile_select_c::copySelectWakuAlpahAnm(u8 i_idx) {
    if (i_idx == 0xff) {
        return true;
    }

    bool rv = false;
    bool iVar2 = mCpSelPane_moyo[i_idx]->alphaAnime(field_0x02e4[i_idx], field_0x02e0[i_idx],
                                                   field_0x02e2[i_idx], 0);
    bool iVar3 = mCpSelPane_gold[i_idx]->alphaAnime(field_0x02e4[i_idx], field_0x02e0[i_idx],
                                                   field_0x02e2[i_idx], 0);
    bool iVar4 = mCpSelPane_gold2[i_idx]->alphaAnime(field_0x02e4[i_idx], field_0x02e0[i_idx],
                                                   field_0x02e2[i_idx], 0);
    if (iVar2 == true && iVar3 == true && iVar4 == true) {
        rv = true;
    }

    return rv;
}

u8 dFile_select_c::getCptoNum(u8 param_0) {
    u8 cptoNum;
    switch (mCpDataNum) {
    case 0:
        cptoNum = param_0 + 1;
        break;
    case 1:
        cptoNum = param_0 * 2;
        break;
    case 2:
        cptoNum = param_0;
        break;
    }

    return cptoNum;
}

void dFile_select_c::copyToSelBack() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelDataMove = selectDataMoveAnm();
    bool isMenuMove = menuMoveAnm();
    bool isSelWakuAlpha = selectWakuAlpahAnm(mSelectNum);
    bool isNameMove = nameMoveAnm();

    if (isHeaderTxtChange == true && isSelDataMove == true && isMenuMove == true &&
        isSelWakuAlpha == true && isNameMove == true)
    {
        for (int i = 0; i < 3; i++) {
            m3mSelPane[i]->getPanePtr()->setAnimation(m3mBck);
            if (i == mSelectMenuNum) {
                m3mBck->setFrame(MenuSelStartFrameTbl[i]);
            } else {
                m3mBck->setFrame(MenuSelEndFrameTbl[i]);
            }
            m3mSelPane[i]->getPanePtr()->animationTransform();
            m3mSelPane[i]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            ((J2DTextBox*)m3mSelTextPane[i]->getPanePtr())->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xff));
        }

        menuCursorShow();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectMenuNum;
        #endif

        mDataSelProc = DATASELPROC_MENU_SELECT;
    }
}

void dFile_select_c::copyToSelPaneMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    if (isHeaderTxtChange == true && isYnMenuMove == true) {
        yesnoCursorShow();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = field_0x0268;
        #endif

        mDataSelProc = DATASELPROC_YES_NO_SELECT;
    }
}

void dFile_select_c::yesnoMenuMoveAnmInitSet(int param_1, int param_2) {
    if (!field_0x0108) {
        field_0x0268 = false;
        field_0x0269 = true;
    }

    #if PLATFORM_WII || PLATFORM_SHIELD
    field_0x4333 = field_0x0268;
    #endif

    if (param_1 == 1139) {
        field_0x0281 = true;
        ((J2DTextBox*)mYnSelTxtPane[field_0x0268]->getPanePtr())->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
        ((J2DTextBox*)mYnSelTxtPane[field_0x0269]->getPanePtr())->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xff));
    }

    mSelIcon->setAlphaRate(0.0f);
    mYnSelPane[0]->getPanePtr()->setAnimation(mYnSelBck3);
    mYnSelPane[1]->getPanePtr()->setAnimation(mYnSelBck3);
    field_0x0100 = param_1;
    field_0x0104 = param_2;
    mYnSelBck3->setFrame(field_0x0100);
    mYnSelPane[0]->getPanePtr()->animationTransform();
    mYnSelPane[1]->getPanePtr()->animationTransform();
}

bool dFile_select_c::yesnoMenuMoveAnm() {
    bool rv;
    if (field_0x0100 != field_0x0104) {
        if (field_0x0100 < field_0x0104) {
            field_0x0100 += 2;
            if (field_0x0100 > field_0x0104) {
                field_0x0100 = field_0x0104;
            }
        } else {
            field_0x0100 -= 2;
            if (field_0x0100 < field_0x0104) {
                field_0x0100 = field_0x0104;
            }
        }

        mYnSelBck3->setFrame(field_0x0100);
        mYnSelPane[0]->getPanePtr()->animationTransform();
        mYnSelPane[1]->getPanePtr()->animationTransform();
        rv = false;
    } else {
        mYnSelPane[0]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        mYnSelPane[1]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);

        if (field_0x0100 == 1149) {
            field_0x0108 = true;
            field_0x0281 = false;
        } else {
            field_0x0108 = false;
            if (field_0x0268 != 0xff) {
                mYnSelPane_m[field_0x0268]->setAlpha(0);
                mYnSelPane_g[field_0x0268]->setAlpha(0);
                mYnSelPane_gr[field_0x0268]->setAlpha(0);
            }
        }
        rv = true;
    }

    return rv;
}

#if PLATFORM_WII || PLATFORM_SHIELD
bool dFile_select_c::GCtoWiiTimeConvert() {
    u8 err_num = 0;
    SaveDataBuf* save_raw_p = mSaveData;

    for (int i = 0; i < 3; i++) {
        dSv_save_c* save_p = (dSv_save_c*)save_raw_p;

        BOOL is_valid_checksum = mDoMemCdRWm_TestCheckSumGameData(save_raw_p);
        if (is_valid_checksum) {
            const char* playerName = save_p->getPlayer().getPlayerInfo().getPlayerName();
            if (*playerName != 0) {
                OSTime iplDate = save_p->getPlayer().getPlayerStatusB().getDateIpl();
                save_p->getPlayer().getPlayerStatusB().setDateIpl(1.5 * iplDate);

                OSTime totalTime = save_p->getPlayer().getPlayerInfo().getTotalTime();
                save_p->getPlayer().getPlayerInfo().setTotalTime(1.5 * totalTime);
                mDoMemCdRWm_SetCheckSumGameData((u8*)&mSaveData[0], i);
            }
        } else {
            dComIfGs_setInitDataToCard((u8*)&mSaveData[0], i);
            mDoMemCdRWm_SetCheckSumGameData((u8*)&mSaveData[0], i);
            err_num++;
        }

        save_raw_p++;
    }

    if (err_num == 3) {
        return false;
    }

    return true;
}
#endif

bool dFile_select_c::yesnoSelectMoveAnm() {
    bool rv = false;
    bool bVar1 = true;

    if (field_0x0269 != 0xff && field_0x00f8[field_0x0269] != YnSelStartFrameTbl[field_0x0269]) {
        if (field_0x00f8[field_0x0269] < YnSelStartFrameTbl[field_0x0269]) {
            field_0x00f8[field_0x0269] += 2;
            if (field_0x00f8[field_0x0269] > YnSelStartFrameTbl[field_0x0269]) {
                field_0x00f8[field_0x0269] = YnSelStartFrameTbl[field_0x0269];
            }
        } else {
            field_0x00f8[field_0x0269] -= 2;
            if (field_0x00f8[field_0x0269] < YnSelStartFrameTbl[field_0x0269]) {
                field_0x00f8[field_0x0269] = YnSelStartFrameTbl[field_0x0269];
            }
        }

        mYnSelBck->setFrame(field_0x00f8[field_0x0269]);
        mYnSelPane[field_0x0269]->getPanePtr()->animationTransform();
        bVar1 = false;
    }

    bool bVar2 = true;
    if (field_0x0268 != 0xff && field_0x00f8[field_0x0268] != YnSelEndFrameTbl[field_0x0268]) {
        if (field_0x00f8[field_0x0268] < YnSelEndFrameTbl[field_0x0268]) {
            field_0x00f8[field_0x0268] += 2;
            if (field_0x00f8[field_0x0268] > YnSelEndFrameTbl[field_0x0268]) {
                field_0x00f8[field_0x0268] = YnSelEndFrameTbl[field_0x0268];
            }
        } else {
            field_0x00f8[field_0x0268] -= 2;
            if (field_0x00f8[field_0x0268] < YnSelEndFrameTbl[field_0x0268]) {
                field_0x00f8[field_0x0268] = YnSelEndFrameTbl[field_0x0268];
            }
        }

        mYnSelBck2->setFrame(field_0x00f8[field_0x0268]);
        mYnSelPane[field_0x0268]->getPanePtr()->animationTransform();
        bVar2 = false;
    }

    if (bVar1 == true && bVar2 == true) {
        if (field_0x0269 != 0xff) {
            mYnSelPane[field_0x0269]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        if (field_0x0268 != 0xff) {
            mYnSelPane[field_0x0268]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        rv = true;
    }

    return rv;
}

void dFile_select_c::yesnoCursorShow() {
    if (field_0x0268 != 0xff) {
        ((J2DTextBox*)mYnSelTxtPane[field_0x0268]->getPanePtr())->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
        mYnSelPane_m[field_0x0268]->setAlpha(0xff);
        mYnSelPane_g[field_0x0268]->setAlpha(0xff);
        mYnSelPane_gr[field_0x0268]->setAlpha(0xff);

        Vec pos = mYnSelPane[field_0x0268]->getGlobalVtxCenter(0, 0);
        mSelIcon->setPos(pos.x, pos.y, mYnSelPane[field_0x0268]->getPanePtr(), true);
        mSelIcon->setAlphaRate(1.0f);
        mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    }
}

void dFile_select_c::YesNoSelect() {
    stick->checkTrigger();

    if (mDoCPd_c::getTrigA(PAD_1)) {
        yesNoSelectStart();
    } else if (mDoCPd_c::getTrigB(PAD_1)) {
        yesnoCancelAnmSet();
    } else if (stick->checkRightTrigger()) {
        if (field_0x0268 != 0) {
            mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
            field_0x0269 = field_0x0268;
            field_0x0268 = 0;
            yesnoSelectAnmSet();
            mDataSelProc = DATASELPROC_YES_NO_CURSOR_MOVE_ANM;
        }
    } else if (stick->checkLeftTrigger() && field_0x0268 != 1) {
        mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0);
        field_0x0269 = field_0x0268;
        field_0x0268 = 1;
        yesnoSelectAnmSet();
        mDataSelProc = DATASELPROC_YES_NO_CURSOR_MOVE_ANM;
    }
}

void dFile_select_c::yesNoSelectStart() {
    if (field_0x0268 != 0) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
        field_0x03b1 = 1;

        switch (mCommand) {
        case 1:
            headerTxtSet(74, 0, 0);
            mpFileWarning->openInit();
            mDeleteEfPane[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
            mDeleteEfPane[mSelectNum]->alphaAnimeStart(0);
            mDeleteEfPane[mSelectNum]->setAlpha(0);
            menuMoveAnmInitSet(809, 799);
            yesnoMenuMoveAnmInitSet(1149, 1139);
            mSelIcon->setAlphaRate(0.0f);
            mDtEffBtkFrame = 0;
            mCpDtEffBrkFrame = 0;
            field_0x0208 = 1;
            break;
        case 2:
            headerTxtSet(0x4a, 0, 0);
            mpFileWarning->openInit();
            mDeleteEfPane[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
            mDeleteEfPane[mSelectNum]->alphaAnimeStart(0);
            mDeleteEfPane[mSelectNum]->setAlpha(0);
            yesnoMenuMoveAnmInitSet(1149, 1139);
            mSelIcon->setAlphaRate(0.0f);
            mDtEffBtkFrame = 0;
            mCpDtEffBrkFrame = 0;
            field_0x0208 = 1;
            break;
        case 0:
            headerTxtSet(0x46, 0, 0);
            mpFileWarning->openInit();
            mCopyEfPane[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
            mCopyEfPane[mSelectNum]->alphaAnimeStart(0);
            mCopyEfPane[mSelectNum]->setAlpha(0);
            mCopyEfPane[mCpDataToNum]->getPanePtr()->scale(1.0f, 1.0f);
            mCopyEfPane[mCpDataToNum]->alphaAnimeStart(0);
            mCopyEfPane[mCpDataToNum]->setAlpha(0);
            yesnoMenuMoveAnmInitSet(1149, 1139);
            mSelIcon->setAlphaRate(0.0f);
            mSelIcon2->setAlphaRate(0.0f);
            selectWakuAlpahAnmInit(mCpDataToNum, 0, 0xff, g_fsHIO.select_box_appear_frames);
            copySelMoveAnmInitSet(0xd29, 0xd1f);
            mCpEffBtkFrame = 0;
            mCpDtEffBrkFrame = 0;
            field_0x0209 = 1;
            break;
        }
    
        modoruTxtDispAnmInit(0);
        ketteiTxtDispAnmInit(0);
        mDataSelProc = DATASELPROC_CMD_EXEC_PANE_MOVE0;
    } else {
        yesnoCancelAnmSet();
    }
}

void dFile_select_c::yesnoSelectAnmSet() {
    if (field_0x0269 != 0xff) {
        yesnoWakuAlpahAnmInit(field_0x0269, 0xff, 0, g_fsHIO.select_box_appear_frames);
        mYnSelPane[field_0x0269]->getPanePtr()->setAnimation(mYnSelBck);
        field_0x00f8[field_0x0269] = YnSelEndFrameTbl[field_0x0269];
        mYnSelBck->setFrame(field_0x00f8[field_0x0269]);
        mYnSelPane[field_0x0269]->getPanePtr()->animationTransform();
        mSelIcon->setAlphaRate(0.0f);
    }

    if (field_0x0268 != 0xff) {
        mYnSelPane[field_0x0268]->getPanePtr()->setAnimation(mYnSelBck2);
        field_0x00f8[field_0x0268] = YnSelStartFrameTbl[field_0x0268];
        mYnSelBck2->setFrame(field_0x00f8[field_0x0268]);
        mYnSelPane[field_0x0268]->getPanePtr()->animationTransform();
    }
}

void dFile_select_c::yesnoCancelAnmSet() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0);
    mSelIcon->setAlphaRate(0.0f);

    switch (mCommand) {
    case 1:
        headerTxtSet(msgTbl[mSelectNum], 1, 0);
        break;
    case 2:
        headerTxtSet(67, 1, 0);
        selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectNum], SelOpenStartFrameTbl[mSelectNum]);
        break;
    case 0:
        headerTxtSet(68, 0, 0);
        break;
    }

    yesnoMenuMoveAnmInitSet(1149, 1139);
    mDataSelProc = DATASELPROC_YES_NO_CANCEL_MOVE;
}

void dFile_select_c::YesNoCancelMove() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    bool isSelDataMove;

    switch (mCommand) {
    case 1:
        isSelDataMove = true;
        break;
    case 2:
        isSelDataMove = selectDataMoveAnm();
        break;
    case 0:
        isSelDataMove = true;
        break;
    }

    if (isHeaderTxtChange == true && isYnMenuMove == true && isSelDataMove == true) {
        switch (mCommand) {
        case 1:
            if (mSelectMenuNum != 0xff) {
                Vec vtxCenter;
                vtxCenter = m3mSelPane[mSelectMenuNum]->getGlobalVtxCenter(false, 0);
                mSelIcon->setPos(vtxCenter.x, vtxCenter.y,
                                 m3mSelPane[mSelectMenuNum]->getPanePtr(), true);
                mSelIcon->setAlphaRate(1.0f);
                mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);

                #if PLATFORM_WII || PLATFORM_SHIELD
                field_0x4333 = mSelectMenuNum;
                #endif
            }
            mDataSelProc = DATASELPROC_MENU_SELECT;
            break;
        case 2:
            selFileCursorShow();

            #if PLATFORM_WII || PLATFORM_SHIELD
            field_0x4333 = mSelectNum;
            #endif

            mDataSelProc = DATASELPROC_DATA_SELECT;
            break;
        case 0:
            mSelIcon2->setAlphaRate(1.0f);

            #if PLATFORM_WII || PLATFORM_SHIELD
            field_0x4333 = field_0x026b;
            #endif

            mDataSelProc = DATASELPROC_COPY_DATA_TO_SELECT;
            break;
        }
    }
}

void dFile_select_c::yesNoCursorMoveAnm() {
    bool isYnSelMove = yesnoSelectMoveAnm();
    bool isYnWakuAlpha = yesnoWakuAlpahAnm(field_0x0269);
    if (isYnSelMove == true && isYnWakuAlpha == true) {
        yesnoCursorShow();
        mDataSelProc = DATASELPROC_YES_NO_SELECT;
    }
}

void dFile_select_c::CmdExecPaneMove0() {
    int isHeaderTxtChange = headerTxtChangeAnm();
    int isEffAlphaAnm;
    int isYnMoveAnm;
    int isMenuMove;
    int isSelWakuAlpha;
    int isNameMove;

    switch (mCommand) {
    case 1:
        isEffAlphaAnm = mDeleteEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xff, 0);
        isYnMoveAnm = yesnoMenuMoveAnm();
        isMenuMove = menuMoveAnm();
        isSelWakuAlpha = true;
        isNameMove = true;
        break;
    case 2:
        isEffAlphaAnm = mDeleteEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xff, 0);
        isYnMoveAnm = true;
        isMenuMove = yesnoMenuMoveAnm();
        isSelWakuAlpha = true;
        isNameMove = true;
        break;
    case 0:
        isEffAlphaAnm = mCopyEfPane[mCpDataToNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xff, 0);
        isYnMoveAnm = mCopyEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xff, 0);
        if (field_0x0108) {
            isMenuMove = yesnoMenuMoveAnm();
        } else {
            isMenuMove = true;
        }
        isSelWakuAlpha = selectWakuAlpahAnm(mCpDataToNum);
        isNameMove = nameMoveAnm();
        break;
    }

    BOOL isModoruTxtDisp = modoruTxtDispAnm();
    BOOL isKetteiTxtDisp = ketteiTxtDispAnm();
    BOOL warningStatus = mpFileWarning->getStatus();
    if (isHeaderTxtChange == true && isEffAlphaAnm == true && isYnMoveAnm == true && isMenuMove == true &&
        isSelWakuAlpha == true && isNameMove == true && isModoruTxtDisp == true && isKetteiTxtDisp == true &&
        warningStatus == true)
    {
        mDataSelProc = DATASELPROC_COMMAND_EXEC;
    }
}

void dFile_select_c::CommandExec() {
    u8* srcData;
    u8* dstData;

    switch (mCommand) {
    case 1:
    case 2:
        mDoAud_seStartLevel(Z2SE_SY_FILE_DELETE_LEVEL, NULL, 0, 0);
        dComIfGs_setInitDataToCard((u8*)mSaveData, mSelectNum);
        mDoMemCdRWm_SetCheckSumGameData((u8*)mSaveData, mSelectNum);
        dataSave();
        mDataSelProc = DATASELPROC_DATA_ERASE_WAIT;
        break;
    case 0:
        mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, NULL, 0, 0);

        srcData = (u8*)&mSaveData[mCpDataNum];
        dstData = (u8*)&mSaveData[mCpDataToNum];
        memcpy(dstData, srcData, sizeof(SaveDataBuf));
        mDoMemCdRWm_SetCheckSumGameData((u8*)mSaveData, mCpDataToNum);
        mDoMemCd_setCopyToPos(mCpDataToNum);
        dataSave();
        mDataSelProc = DATASELPROC_DATA_COPY_WAIT;
        break;
    }

    mWaitTimer = g_fsHIO.card_wait_frames;
}

void dFile_select_c::DataEraseWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }
    mDoAud_seStartLevel(Z2SE_SY_FILE_DELETE_LEVEL, NULL, 0, 0);

    #if PLATFORM_GCN
    field_0x03b4 = mDoMemCd_SaveSync();
    #else
    field_0x03b4 = mDoMemCd_SaveSyncNAND();
    #endif

    if (field_0x03b4 != 0) {
        mDataSelProc = DATASELPROC_DATA_ERASE_WAIT2;
    }
}

void dFile_select_c::DataEraseWait2() {
    if (mWaitTimer != 0) {
        mDoAud_seStartLevel(Z2SE_SY_FILE_DELETE_LEVEL, NULL, 0, 0);
        mWaitTimer--;
    } else if (field_0x03b4 == 2) {
        mDoAud_seStart(Z2SE_SY_FILE_ERROR, NULL, 0, 0);
        headerTxtSet(76, 0, 0);
        mpFileWarning->closeInit();
        modoruTxtDispAnmInit(1);
        ketteiTxtDispAnmInit(1);
        mDeleteEfPane[mSelectNum]->alphaAnimeStart(0);
        mDataSelProc = DATASELPROC_ERROR_MSG_PANE_MOVE;
    } else if (field_0x03b4 == 1) {
        mDoAud_seStart(Z2SE_SY_FILE_DELETE_OK, NULL, 0, 0);
        field_0x03b1 = 0;
        mDeleteEfPane[mSelectNum]->alphaAnimeStart(0);
        mFileInfoNoDatBasePane[mSelectNum]->alphaAnimeStart(0);
        mFileInfoDatBasePane[mSelectNum]->alphaAnimeStart(0);
        mDataSelProc = DATASELPROC_ERASE_PANE_MOVE_OK;
    }
}

void dFile_select_c::ErasePaneMoveOk() {
    int iVar1 = mDeleteEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);

    int iVar2;
    if (mIsNoData[mSelectNum]) {
        iVar2 = mFileInfoNoDatBasePane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
    } else {
        iVar2 = mFileInfoDatBasePane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
    }

    if (iVar1 == 1 && iVar2 == 1) {
        field_0x0208 = 0;
        setSaveData();
        makeRecInfo(mSelectNum);
        headerTxtSet(0x4b, 0, 0);
        mpFileWarning->closeInit();
        mFileInfoNoDatBasePane[mSelectNum]->alphaAnimeStart(0);
        selectWakuAlpahAnmInit(mSelectNum, 0, 0xff, g_fsHIO.char_switch_frames);
        mDataSelProc = DATASELPROC_ERASE_PANE_MOVE_OK2;
    }
}

void dFile_select_c::ErasePaneMoveOk2() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool iVar2 = mFileInfoNoDatBasePane[mSelectNum]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xff, 0);
    bool wakuAnm = selectWakuAlpahAnm(mSelectNum);
    u32 warningStatus = mpFileWarning->getStatus();

    if (isHeaderTxtChange == true && iVar2 == true && wakuAnm == true && warningStatus == true) {
        mWaitTimer = g_fsHIO.field_0x000a;
        mSelectMenuNum = 0;
        mDataSelProc = DATASELPROC_ERASE_END_BACK_SELECT_WAIT;
    }
}

void dFile_select_c::eraseEndBackSelectWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    } else if (mWaitTimer == 0) {
        selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectNum], SelOpenStartFrameTbl[mSelectNum]);
        headerTxtSet(67, 1, 0);
        ketteiTxtDispAnmInit(1);
        mDataSelProc = DATASELPROC_ERASE_END_BACK_SELECT;
    }
}

void dFile_select_c::eraseEndBackSelect() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelDataMove = selectDataMoveAnm();
    bool isKetteiTxtDisp = ketteiTxtDispAnm();

    if (isHeaderTxtChange == true && isSelDataMove == true && isKetteiTxtDisp == true) {
        selFileCursorShow();
        mpFileSelect3d->drawOff();

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectNum;
        #endif

        mDataSelProc = DATASELPROC_DATA_SELECT;
    }
}

void dFile_select_c::DataCopyWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, NULL, 0, 0);

    #if PLATFORM_GCN
    field_0x03b4 = mDoMemCd_SaveSync();
    #else
    field_0x03b4 = mDoMemCd_SaveSyncNAND();
    #endif

    if (field_0x03b4 != 0) {
        mDataSelProc = DATASELPROC_DATA_COPY_WAIT2;
    }
}

void dFile_select_c::DataCopyWait2() {
    if (mWaitTimer != 0) {
        mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, NULL, 0, 0);
        mWaitTimer--;
    } else {
        if (field_0x03b4 == 2) {
            mDoAud_seStart(Z2SE_SY_FILE_ERROR, NULL, 0, 0);
            headerTxtSet(0x48, 0, 0);
            mpFileWarning->closeInit();
            modoruTxtDispAnmInit(1);
            ketteiTxtDispAnmInit(1);
            mDataSelProc = DATASELPROC_ERROR_MSG_PANE_MOVE;
        } else if (field_0x03b4 == 1) {
            mDoAud_seStart(Z2SE_SY_FILE_COPY_OK, NULL, 0, 0);
            field_0x03b1 = 0;
            mCopyEfPane[mSelectNum]->alphaAnimeStart(0);
            mCopyEfPane[mCpDataToNum]->alphaAnimeStart(0);
            mFileInfoNoDatBasePane[mCpDataToNum]->alphaAnimeStart(0);
            mFileInfoDatBasePane[mCpDataToNum]->alphaAnimeStart(0);
            mDataSelProc = DATASELPROC_COPY_PANE_MOVE_OK;
        }
    }
}

void dFile_select_c::copyPaneMoveOk() {
    bool iVar1 = mCopyEfPane[mCpDataToNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
    bool iVar2 = mCopyEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);

    bool iVar3;
    if (mIsNoData[mCpDataToNum] || mIsDataNew[mCpDataToNum] != 0) {
        iVar3 = mFileInfoNoDatBasePane[mCpDataToNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
    } else {
        iVar3 = mFileInfoDatBasePane[mCpDataToNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
    }

    if (iVar1 == true && iVar2 == true && iVar3 == true) {
        field_0x0209 = 0;
        headerTxtSet(71, 0, 0);
        mpFileWarning->closeInit();
        setSaveData();
        mFileInfoNoDatBasePane[mCpDataToNum]->alphaAnimeStart(0);
        mFileInfoDatBasePane[mCpDataToNum]->alphaAnimeStart(0);
        mDataSelProc = DATASELPROC_COPY_PANE_MOVE_OK2;
    }
}

void dFile_select_c::copyPaneMoveOk2() {
    bool isHeaderTxtChange = headerTxtChangeAnm();

    bool iVar2;
    if (mIsNoData[mCpDataToNum] || mIsDataNew[mCpDataToNum] != 0) {
        iVar2 = mFileInfoNoDatBasePane[mCpDataToNum]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xff, 0);
    } else {
        iVar2 = mFileInfoDatBasePane[mCpDataToNum]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xff, 0);
    }

    u32 iVar3 = mpFileWarning->getStatus();
    if (isHeaderTxtChange == true && iVar2 == true && iVar3 == true) {
        mWaitTimer = g_fsHIO.field_0x000a;
        mDataSelProc = DATASELPROC_BACK_DAT_SEL_WAIT;
    }
}

void dFile_select_c::ErrorMsgPaneMove() {
    int isHeaderTxtChange = headerTxtChangeAnm();
    int unaff_r30;
    int unaff_r29;

    switch (mCommand) {
    case 1:
    case 2:
        unaff_r30 = mDeleteEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
        unaff_r29 = 1;
        break;

    case 0:
        unaff_r30 = mCopyEfPane[mCpDataToNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
        unaff_r29 = mCopyEfPane[mSelectNum]->alphaAnime(g_fsHIO.copy_erase_frames, 0xff, 0, 0);
        break;
    }

    int isModoruTxtDisp = modoruTxtDispAnm();
    int isKetteiTxtDisp = ketteiTxtDispAnm();
    BOOL warningStatus = mpFileWarning->getStatus();

    if (isHeaderTxtChange == true && unaff_r30 == true && unaff_r29 == true && isModoruTxtDisp == true &&
        isKetteiTxtDisp == true && warningStatus == true)
    {
        field_0x0208 = 0;
        field_0x0209 = 0;
        field_0x03b1 = 0;
        mWaitTimer = g_fsHIO.field_0x000a;
        mDataSelProc = DATASELPROC_BACK_DAT_SEL_WAIT2;
    }
}

void dFile_select_c::backDatSelPaneMove() {
    int isHeaderTxtChange = headerTxtChangeAnm();
    int isMenuMove = menuMoveAnm();
    int isSelDataMove = selectDataMoveAnm();
    int isWaku1Alpha = selectWakuAlpahAnm(mSelectNum);
    int isWaku2Alpha = selectWakuAlpahAnm(mCpDataToNum);
    int isModoruTxtAnm = modoruTxtDispAnm();
    int isKetteiTxtDisp = ketteiTxtDispAnm();

    if (isHeaderTxtChange == true && isMenuMove == true && isSelDataMove == true && isWaku1Alpha == true &&
        isWaku2Alpha == true && isModoruTxtAnm == true && isKetteiTxtDisp == true)
    {
        if (mSelectMenuNum != 0xff) {
            m3mSelPane[mSelectMenuNum]->getPanePtr()->setAnimation(m3mBck);
            m3mBck->setFrame(MenuSelStartFrameTbl[mSelectMenuNum]);
            m3mSelPane[mSelectMenuNum]->getPanePtr()->animationTransform();
            m3mSelPane[mSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            menuCursorShow();
        }

        #if PLATFORM_WII || PLATFORM_SHIELD
        field_0x4333 = mSelectMenuNum;
        #endif

        mDataSelProc = DATASELPROC_MENU_SELECT;
    }
}

void dFile_select_c::backDatSelWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    } else if (mWaitTimer == 0) {
        headerTxtSet(msgTbl[mSelectNum], 1, 0);
        selectWakuAlpahAnmInit(mCpDataToNum, 0xff, 0, g_fsHIO.select_box_appear_frames);
        selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.select_box_appear_frames);
        menuMoveAnmInitSet(799, 809);
        selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
        modoruTxtDispAnmInit(1);
        ketteiTxtDispAnmInit(1);
        mDataSelProc = DATASELPROC_BACK_DAT_SEL_PANE_MOVE;
    }
}

void dFile_select_c::backDatSelWait2() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    if (cAPICPad_ANY_BUTTON(0) != 0 || mWaitTimer == 0) {
        mDataSelProc = DATASELPROC_MEMCARD_CHECK_MAIN;
        mCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
    }
}

void dFile_select_c::nextModeWait() {}

#if PLATFORM_WII || PLATFORM_SHIELD
void dFile_select_c::dataSelectInCopy() {
    bool isHeaderTxtChange = headerTxtChangeAnm();
    bool isSelectDataBaseMove = selectDataBaseMoveAnm();

    bool isErrorMove = true;
    if (field_0x014a) {
        isErrorMove = errorMoveAnm();
    }

    bool isYnMenuMove = true;
    if (field_0x0108) {
        isYnMenuMove = yesnoMenuMoveAnm();
    }

    if (isHeaderTxtChange == true && isSelectDataBaseMove == true && isErrorMove == true && isYnMenuMove == true) {
        for (int i = 0; i < 3; i++) {
            mSelFilePanes[i]->reinit();
        }

        headerTxtSet(0xECC, 0, 0);
        mpFileWarning->setText(0xECD);
        mpFileWarning->openInit();

        for (int i = 0; i < 3; i++) {
            mCopyEfPane[i]->getPanePtr()->scale(1.0f, 1.0f);
            mCopyEfPane[i]->alphaAnimeStart(0);
            mCopyEfPane[i]->setAlpha(0);
            selectWakuAlpahAnmInit(i, 0, 0xFF, g_fsHIO.select_box_appear_frames);
        }

        ketteiTxtDispAnmInit(0);

        mCpEffBtkFrame = 0;
        mCpDtEffBrkFrame = 0;
        field_0x0209 = 1;
        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY;
    }
}

void dFile_select_c::cardToNandDataCopy() {
    BOOL isHeaderTxtChange = headerTxtChangeAnm();
    BOOL temp_r29 = mCopyEfPane[0]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xFF, 0);
    BOOL temp_r28 = mCopyEfPane[1]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xFF, 0);
    BOOL temp_r27 = mCopyEfPane[2]->alphaAnime(g_fsHIO.copy_erase_frames, 0, 0xFF, 0);
    BOOL temp_r26 = selectWakuAlpahAnm(0);
    BOOL temp_r25 = selectWakuAlpahAnm(1);
    BOOL temp_r24 = selectWakuAlpahAnm(2);
    BOOL isKetteiTxtDisp = ketteiTxtDispAnm();

    if (isHeaderTxtChange == TRUE && temp_r29 == TRUE && temp_r28 == TRUE && temp_r27 == TRUE && temp_r26 == TRUE && temp_r25 == TRUE && temp_r24 == TRUE && isKetteiTxtDisp == TRUE) {
        memcpy(&mSaveData, &field_0x2376, SAVEFILE_SIZE);
        mDoMemCdRWm_SetCheckSumGameData((u8*)&mSaveData[0], 0);
        mDoMemCdRWm_SetCheckSumGameData((u8*)&mSaveData[0], 1);
        mDoMemCdRWm_SetCheckSumGameData((u8*)&mSaveData[0], 2);
        mDoMemCd_saveNAND(&mSaveData, SAVEFILE_SIZE, 0);
        mWaitTimer = g_fsHIO.card_wait_frames;
        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_WAIT;
    }
}

void dFile_select_c::cardToNandDataCopyWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, NULL, 0, 0);

    field_0x03b4 = mDoMemCd_SaveSyncNAND();
    if (field_0x03b4 != 0) {
        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_WAIT2;
    }
}

void dFile_select_c::cardToNandDataCopyWait2() {
    if (mWaitTimer != 0) {
        mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, NULL, 0, 0);
        mWaitTimer--;
        return;
    }

    if (field_0x03b4 == 2) {
        mDoAud_seStart(Z2SE_SY_FILE_ERROR, NULL, 0, 0);
        headerTxtSet(0xECF, 0, 0);
        mpFileWarning->closeInit();

        for (int i = 0; i < 3; i++) {
            mCopyEfPane[i]->alphaAnimeStart(0);
            selectWakuAlpahAnmInit(i, 0xFF, 0, g_fsHIO.select_box_appear_frames);
        }

        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_ERR_DISP;
    } else if (field_0x03b4 == 1) {
        mDoAud_seStart(Z2SE_SY_FILE_COPY_OK, NULL, 0, 0);

        for (int i = 0; i < 3; i++) {
            mCopyEfPane[i]->alphaAnimeStart(0);
            mFileInfoNoDatBasePane[i]->alphaAnimeStart(0);
            mFileInfoDatBasePane[i]->alphaAnimeStart(0);
            selectWakuAlpahAnmInit(i, 0xFF, 0, g_fsHIO.select_box_appear_frames);
        }

        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_OK_DISP;
    }
}

void dFile_select_c::cardToNandDataCopyOkDisp() {
    bool temp_r25 = mCopyEfPane[0]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    bool temp_r24 = mCopyEfPane[1]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    bool temp_r23 = mCopyEfPane[2]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);

    bool var_r28;
    if (mIsNoData[0] || mIsDataNew[0] != 0) {
        var_r28 = mFileInfoNoDatBasePane[0]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    } else {
        var_r28 = mFileInfoDatBasePane[0]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    }

    bool var_r27;
    if (mIsNoData[1] || mIsDataNew[1] != 0) {
        var_r27 = mFileInfoNoDatBasePane[1]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    } else {
        var_r27 = mFileInfoDatBasePane[1]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    }

    bool var_r26;
    if (mIsNoData[2] || mIsDataNew[2] != 0) {
        var_r26 = mFileInfoNoDatBasePane[2]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    } else {
        var_r26 = mFileInfoDatBasePane[2]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    }

    bool temp_r22 = selectWakuAlpahAnm(0);
    bool temp_r21 = selectWakuAlpahAnm(1);
    bool temp_r20 = selectWakuAlpahAnm(2);

    if (temp_r25 == true && temp_r24 == true && temp_r23 == true && var_r28 == true && var_r27 == true && var_r26 == true && temp_r22 == true && temp_r21 == true && temp_r20 == true) {
        field_0x0209 = 0;
        headerTxtSet(0xECE, 0, 0);
        mpFileWarning->closeInit();
        setSaveData();

        for (int i = 0; i < 3; i++) {
            mFileInfoNoDatBasePane[i]->alphaAnimeStart(0);
            mFileInfoDatBasePane[i]->alphaAnimeStart(0);
        }

        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_OK_DISP2;
    }
}

void dFile_select_c::cardToNandDataCopyOkDisp2() {
    bool isHeaderTxtChange = headerTxtChangeAnm();

    bool var_r28;
    if (mIsNoData[0] || mIsDataNew[0] != 0) {
        var_r28 = mFileInfoNoDatBasePane[0]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    } else {
        var_r28 = mFileInfoDatBasePane[0]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    }

    bool var_r27;
    if (mIsNoData[1] || mIsDataNew[1] != 0) {
        var_r27 = mFileInfoNoDatBasePane[1]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    } else {
        var_r27 = mFileInfoDatBasePane[1]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    }

    bool var_r26;
    if (mIsNoData[2] || mIsDataNew[2] != 0) {
        var_r26 = mFileInfoNoDatBasePane[2]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    } else {
        var_r26 = mFileInfoDatBasePane[2]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    }

    u32 temp_r22 = mpFileWarning->getStatus();

    if (isHeaderTxtChange == true && var_r28 == true && var_r27 == true && var_r26 == true && temp_r22 == true) {
        mWaitTimer = g_fsHIO.field_0x000a;
        mpFileWarning->setText(0xED6);

        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_OK_DISP3;
    }
}

void dFile_select_c::cardToNandDataCopyOkDisp3() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    headerTxtSet(0x43, 1, 0);

    SaveDataBuf* savebuf_p = mSaveData;
    dSv_save_c* save_p = (dSv_save_c*)savebuf_p;
    OSTime date_ipl = save_p->getPlayer().getPlayerStatusB().getDateIpl();
    mSelectNum = 0;

    for (int i = 0; i < 3; i++) {
        save_p = (dSv_save_c*)savebuf_p;
        if (!mIsNoData[i]) {
            OSTime date_ipl2 = save_p->getPlayer().getPlayerStatusB().getDateIpl();
            if (OSTicksToSeconds(date_ipl) < OSTicksToSeconds(date_ipl2)) {
                date_ipl = date_ipl2;
                mSelectNum = i;
            }
        }

        savebuf_p++;
    }

    mLastSelectNum = mSelectNum;
    ketteiTxtDispAnmInit(1);

    mSelFilePanes[mSelectNum]->getPanePtr()->setAnimation(mBaseMoveAnm);
    field_0x00e0[mSelectNum] = SelStartFrameTbl[mSelectNum];
    mSelFilePane_Book_l[mSelectNum]->alphaAnimeStart(0);
    ketteiTxtDispAnmInit(1);

    mDataSelProc = DATASELPROC_DATA_SELECT_INIT;
}

void dFile_select_c::cardToNandDataCopyErrDisp() {
    BOOL isHeaderTxtChange = headerTxtChangeAnm();
    BOOL temp_r29 = mCopyEfPane[0]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    BOOL temp_r28 = mCopyEfPane[1]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    BOOL temp_r27 = mCopyEfPane[2]->alphaAnime(g_fsHIO.copy_erase_frames, 0xFF, 0, 0);
    BOOL temp_r26 = selectWakuAlpahAnm(0);
    BOOL temp_r25 = selectWakuAlpahAnm(1);
    BOOL temp_r24 = selectWakuAlpahAnm(2);
    BOOL temp_r23 = mpFileWarning->getStatus();

    if (isHeaderTxtChange == true && temp_r29 == true && temp_r28 == true && temp_r27 == true && temp_r26 == true && temp_r25 == true && temp_r24 == true && temp_r23 == true) {
        field_0x0208 = 0;
        field_0x0209 = 0;
        mWaitTimer = g_fsHIO.field_0x000a;
        mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_ERR_DISP2;
    }
}

void dFile_select_c::cardToNandDataCopyErrDisp2() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
        return;
    }

    headerTxtSet(0xFFFF, 0, 0);

    if (field_0x00b8 != 0) {
        selectDataBaseMoveAnmInitSet(0x21, 1);
    }

    mDataSelProc = DATASELPROC_CARD_TO_NAND_DATA_COPY_ERR_DISP3;
}

void dFile_select_c::cardToNandDataCopyErrDisp3() {
    bool isHeaderTxtChange = headerTxtChangeAnm();

    bool isSelectDataBaseMove;
    if (field_0x00b8 != 0) {
        isSelectDataBaseMove = selectDataBaseMoveAnm();
    }

    if (isHeaderTxtChange == true && isSelectDataBaseMove == true) {
        mDataSelProc = DATASELPROC_MEMCARD_CHECK_MAIN;
        mCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
    }
}
#endif

void dFile_select_c::screenSet() {
    static u64 l_tagName0[3] = {'w_sel_00', 'w_sel_01', 'w_sel_02'};
    static u64 l_tagName3[3] = {'w_moyo00', 'w_moyo01', 'w_moyo02'};
    static u64 l_tagName4[3] = {'w_gold00', 'w_gold01', 'w_gold02'};
    static u64 l_tagName5[3] = {'w_go2_00', 'w_go2_01', 'w_go2_02'};
    static u64 l_tagName12[3] = {'w_bk_l00', 'w_bk_l01', 'w_bk_l02'};
    static u64 l_nouseTag[15] = {'w_mcheck', 'w_tabi1',  'w_tabi2',  'w_tabi3', 'w_doko_c',
                                 'w_uwa_c',  'w_cp_chu', 'w_cpsita', 'w_cp_x',  'w_de',
                                 'w_de_chu', 'w_desita', 'w_de_x',   'w_name',  'w_h_name'};

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    static u64 l_tagName21[2] = {'w_tabi_s', 'w_tabi_x'};
#else
    static u64 l_tagName21[2] = {'t_for', 't_for1'};
#endif

    static u64 l_tagName18[3] = {'w_de_ef0', 'w_de_ef1', 'w_de_ef2'};
    static u64 l_tagName19[3] = {'w_cp_ef0', 'w_cp_ef1', 'w_cp_ef2'};

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    static u64 l_tagName20[2] = {'w_er_msg', 'w_er_msR'};
#else
    static u64 l_tagName20[2] = {'er_for0', 'er_for1'};
#endif

    static u64 l_tagName131[3] = {'N_sel_00', 'N_sel_01', 'N_sel_02'};

    fileSel.Scr = new J2DScreen();
    JUT_ASSERT(4917, fileSel.Scr != NULL);
    fileSel.Scr->setPriority("zelda_file_select.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(fileSel.Scr);

    void* bck = JKRGetNameResource("zelda_file_select.bck", mpArchive);
    mBaseMoveAnm = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0088 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0090 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0094 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0098 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x009c = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mBaseMoveAnm->searchUpdateMaterialID(fileSel.Scr);
    field_0x0088->searchUpdateMaterialID(fileSel.Scr);
    field_0x0090->searchUpdateMaterialID(fileSel.Scr);
    field_0x009c->searchUpdateMaterialID(fileSel.Scr);

    mBaseMovePane = new CPaneMgr(fileSel.Scr, 'w_move_n', 0, NULL);
    mBaseSubPane = fileSel.Scr->search('w_sub_n');

    for (int i = 0; i < 3; i++) {
        mSelFilePanes[i] = new CPaneMgr(fileSel.Scr, l_tagName0[i], 1, NULL);
    }

    fileSel.Scr->search('Wi_btn_n')->hide();
    mBbtnPane = new CPaneMgrAlpha(fileSel.Scr, 'w_n_bbtn', 2, NULL);
    mAbtnPane = new CPaneMgrAlpha(fileSel.Scr, 'w_n_abtn', 2, NULL);

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    mModoruTxtPane = new CPaneMgrAlpha(fileSel.Scr, 'w_modo', 2, NULL);
    mKetteiTxtPane = new CPaneMgrAlpha(fileSel.Scr, 'w_kete', 2, NULL);
    fileSel.Scr->search('f_modo')->hide();
    fileSel.Scr->search('f_kete')->hide();
#else
    mModoruTxtPane = new CPaneMgrAlpha(fileSel.Scr, 'f_modo', 2, NULL);
    mKetteiTxtPane = new CPaneMgrAlpha(fileSel.Scr, 'f_kete', 2, NULL);
    fileSel.Scr->search('w_modo')->hide();
    fileSel.Scr->search('w_kete')->hide();
#endif

    ((J2DTextBox*)mModoruTxtPane->getPanePtr())->setFont(fileSel.font[0]);
    ((J2DTextBox*)mKetteiTxtPane->getPanePtr())->setFont(fileSel.font[0]);
    ((J2DTextBox*)mModoruTxtPane->getPanePtr())->setString(32, "");

    mModoruStringPtr = ((J2DTextBox*)mModoruTxtPane->getPanePtr())->getStringPtr();
    fopMsgM_messageGet(mModoruStringPtr, 0x54);
    field_0x024c = 0;

    char ketteiTxtBuf[32];
    fopMsgM_messageGet(ketteiTxtBuf, 0x55);
    ((J2DTextBox*)mKetteiTxtPane->getPanePtr())->setString(ketteiTxtBuf);

    void* bpk = JKRGetNameResource("zelda_file_select.bpk", mpArchive);
    JUT_ASSERT(4994, bpk != NULL);
    mFileSelBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    mFileSelBpk->searchUpdateMaterialID(fileSel.Scr);
    mSelFileBpkFrame = 0;

    void* btk = JKRGetNameResource("zelda_file_select_05.btk", mpArchive);
    JUT_ASSERT(5004, btk != NULL);
    mFileSel05Btk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mFileSel05Btk->searchUpdateMaterialID(fileSel.Scr);
    mSelFileBtk05Frame = 0;

    for (int i = 0; i < 3; i++) {
        mSelFileMoyoPane[i] = new CPaneMgr(fileSel.Scr, l_tagName3[i], 0, NULL);
        mSelFileGoldPane[i] = new CPaneMgr(fileSel.Scr, l_tagName4[i], 0, NULL);
        mSelFileGold2Pane[i] = new CPaneMgr(fileSel.Scr, l_tagName5[i], 0, NULL);
        mSelFileMoyoPane[i]->getPanePtr()->setAnimation(mFileSelBpk);
        mSelFileGoldPane[i]->getPanePtr()->setAnimation(mFileSelBpk);
        mSelFileGold2Pane[i]->getPanePtr()->setAnimation(mFileSelBpk);
        mSelFileGoldPane[i]->getPanePtr()->setAnimation(mFileSel05Btk);
        mSelFileGold2Pane[i]->getPanePtr()->setAnimation(mFileSel05Btk);
    }

    mSelFileBookBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    mSelFileBookBpk->searchUpdateMaterialID(fileSel.Scr);

    btk = JKRGetNameResource("zelda_file_select.btk", mpArchive);
    JUT_ASSERT(5039, btk != NULL);
    mSelFileBookBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mSelFileBookBtk->searchUpdateMaterialID(fileSel.Scr);

    void* brk = JKRGetNameResource("zelda_file_select.brk", mpArchive);
    JUT_ASSERT(5048, brk != NULL);
    mSelFileBookBrk = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    mCpDtEffBrk = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    mSelFileBookBrk->searchUpdateMaterialID(fileSel.Scr);
    mCpDtEffBrk->searchUpdateMaterialID(fileSel.Scr);

    mSelFileBookBpkFrame = 0;
    mSelFileBookBtkFrame = 0;
    mSelFileBookBrkFrame = 0;
    for (int i = 0; i < 3; i++) {
        mSelFilePane_Book_l[i] = new CPaneMgr(fileSel.Scr, l_tagName12[i], 0, NULL);
        mSelFilePane_Book_l[i]->getPanePtr()->setAnimation(mSelFileBookBpk);
        mSelFilePane_Book_l[i]->getPanePtr()->setAnimation(mSelFileBookBtk);
        mSelFilePane_Book_l[i]->getPanePtr()->setAnimation(mSelFileBookBrk);
    }

    for (int i = 0; i < 15; i++) {
        fileSel.Scr->search(l_nouseTag[i])->hide();
    }

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    fileSel.Scr->search('t_for')->hide();
    fileSel.Scr->search('t_for1')->hide();
#else
    fileSel.Scr->search('w_tabi_s')->hide();
    fileSel.Scr->search('w_tabi_x')->hide();
    fileSel.Scr->search('w_mgn1')->hide();
    fileSel.Scr->search('w_mgn2')->hide();
#endif

    for (int i = 0; i < 2; i++) {
        mHeaderTxtPane[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName21[i], 0, NULL);
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setFont(fileSel.font[0]);
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setString(512, "");
#if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setFontSize(21.0f, 21.0f);
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setLineSpace(22.0f);
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setCharSpace(2.0f);
#else
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setFontSize(24.0f, 24.0f);
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setLineSpace(20.0f);
        ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->setCharSpace(0.0f);
#endif
        mHeaderStringPtr[i] = ((J2DTextBox*)mHeaderTxtPane[i]->getPanePtr())->getStringPtr();
    }

    mHeaderTxtPane[0]->setAlpha(0xff);
    mHeaderTxtPane[1]->setAlpha(0);
    mHeaderTxtDispIdx = 0;

    bpk = JKRGetNameResource("zelda_file_select_02.btk", mpArchive);
    JUT_ASSERT(5124, bpk != NULL);
    mCpEffBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(bpk);
    mCpEffBtk->searchUpdateMaterialID(fileSel.Scr);

    bpk = JKRGetNameResource("zelda_file_select_03.btk", mpArchive);
    JUT_ASSERT(5133, bpk != NULL);
    mDtEffBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(bpk);
    mDtEffBtk->searchUpdateMaterialID(fileSel.Scr);

    mDtEffBtkFrame = 0;
    mCpEffBtkFrame = 0;
    mCpDtEffBrkFrame = 0;
    for (int i = 0; i < 3; i++) {
        mDeleteEfPane[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName18[i], 0, NULL);
        mCopyEfPane[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName19[i], 0, NULL);
        mDeleteEfPane[i]->getPanePtr()->setAnimation(mDtEffBtk);
        mDeleteEfPane[i]->getPanePtr()->setAnimation(mCpDtEffBrk);
        mCopyEfPane[i]->getPanePtr()->setAnimation(mCpEffBtk);
        mCopyEfPane[i]->getPanePtr()->setAnimation(mCpDtEffBrk);
    }

    field_0x0208 = 0;
    field_0x0209 = 0;
    mErrorMsgPane = fileSel.Scr->search('w_er_n');

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    fileSel.Scr->search('er_for0')->hide();
    fileSel.Scr->search('er_for1')->hide();
#else
    fileSel.Scr->search('w_er_msg')->hide();
    fileSel.Scr->search('w_er_msR')->hide();
#endif

    fileSel.Scr->search('w_er_msE')->hide();

    for (int i = 0; i < 2; i++) {
        mErrorMsgTxtPane[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName20[i], 0, NULL);
        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setFont(fileSel.font[0]);
        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setString(512, "");

#if (VERSION != VERSION_GCN_JPN) && (VERSION != VERSION_WII_JPN)
        mErrorMsgTxtPane[i]->getPanePtr()->resize(440.0f, 198.0f);
#endif

        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setFontSize(21.0f, 21.0f);

#if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setLineSpace(22.0f);
        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setCharSpace(2.0f);
#else
        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setLineSpace(21.0f);
        ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->setCharSpace(1.0f);
#endif

        mErrorMsgStringPtr[i] = ((J2DTextBox*)mErrorMsgTxtPane[i]->getPanePtr())->getStringPtr();
        mErrorMsgTxtPane[i]->show();
    }

    mErrorTxtDispIdx = 0;
    mErrorMsgTxtPane[mErrorTxtDispIdx]->setAlpha(0xff);
    mErrorMsgTxtPane[mErrorTxtDispIdx ^ 1]->setAlpha(0);

    mSelIcon = new dSelect_cursor_c(0, 1.0f, NULL);
    JUT_ASSERT(5209, mSelIcon != NULL);
    mSelIcon->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
    Vec vtxCenter;
    vtxCenter = mSelFilePanes[mSelectNum]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(vtxCenter.x, vtxCenter.y, mSelFilePanes[mSelectNum]->getPanePtr(), true);
    mSelIcon->setAlphaRate(0.0f);

    J2DPane* basePane = fileSel.Scr->search('Nm_02');
    basePane->setAnimation(mBaseMoveAnm);

    setWakuAnm();

    field_0x00a0 = 1;
    mBaseMoveAnm->setFrame(field_0x00a0);

    basePane->animationTransform();
    basePane->setAnimation((J2DAnmTransform*)NULL);

    mHeaderTxtPane[0]->getPanePtr()->scale(1.0f, 1.0f);
    mHeaderTxtPane[1]->getPanePtr()->scale(1.0f, 1.0f);

    J2DPane* selChildPanes[3];
    for (int i = 0; i < 3; i++) {
        selChildPanes[i] = new J2DPane(l_tagName13[i], JGeometry::TBox2<f32>(238.0f, 43.0f, 712.0f, 130.0f));
        selChildPanes[i]->setUserInfo('n_43');
        mSelFilePanes[i]->getPanePtr()->insertChild(fileSel.Scr->search(l_tagName131[i]),
                                                   selChildPanes[i]);
    }

    for (int i = 0; i < 3; i++) {
        mFileInfo[i] = new dFile_info_c(mpArchive, 0);
        mFileInfo[i]->setBasePane(selChildPanes[i]);
        mFileInfoDatBasePane[i] = mFileInfo[i]->getDatBase();
        mFileInfoNoDatBasePane[i] = mFileInfo[i]->getNoDatBase();
    }

    mNameBasePane = fileSel.Scr->search('name_n');
    mpName = new dName_c(mNameBasePane);
    field_0x0128 = false;
    mpFileWarning = new dFile_warning_c(mpArchive, 0);
    mSelDt.mpPane = fileSel.Scr->search('w_moyo03');

    #if PLATFORM_GCN
    JUtility::TColor black = mDoGph_gInf_c::getFadeColor();
    JUtility::TColor white = mDoGph_gInf_c::getFadeColor();
    black.a = 0;
    white.a = 0xff;

    ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpFadePict = new J2DPicture('PICT01', JGeometry::TBox2<f32>(0.0f, FB_WIDTH, 0.0f, FB_HEIGHT),
                                timg, NULL);
    mpFadePict->setBlackWhite(black, white);
    mpFadePict->setAlpha(0);
    #endif
}

void dFile_select_c::screenSetCopySel() {
    static u64 l_tagName000[2] = {'w_sel_01', 'w_sel_02'};
    static u64 l_tagName001[2] = {'w_moyo01', 'w_moyo02'};
    static u64 l_tagName002[2] = {'w_gold01', 'w_gold02'};
    static u64 l_tagName003[2] = {'w_go2_01', 'w_go2_02'};
    static u64 l_tagName004[2] = {'w_bk_l01', 'w_bk_l02'};
    static u64 l_tagName005[2] = {'w_cp_ef1', 'w_cp_ef2'};

    mCpSel.Scr = new J2DScreen();

    JUT_ASSERT(5286, mCpSel.Scr != NULL);
    mCpSel.Scr->setPriority("zelda_file_select_copy_select.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(mCpSel.Scr);
    mCpSel.mpPane2 = mCpSel.Scr->search('name_n');
    mCpSel.mpPane1 = fileSel.Scr->search('name_n');

    void* bck = JKRGetNameResource("zelda_file_select_copy_select.bck", mpArchive);
    mCpSelBck = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mCpSelBck2 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mCpSelBck->searchUpdateMaterialID(mCpSel.Scr);
    mCpSelBck2->searchUpdateMaterialID(mCpSel.Scr);

    void* bpk = JKRGetNameResource("zelda_file_select_copy_select.bpk", mpArchive);
    JUT_ASSERT(5315, bpk != NULL);
    mCpSelBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    mCpSelBpk->searchUpdateMaterialID(mCpSel.Scr);
    mCpSelBpkFrame = 0;

    void* btk = JKRGetNameResource("zelda_file_select_copy_select_03.btk", mpArchive);
    JUT_ASSERT(5325, btk != NULL);
    mCpSel03Btk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mCpSel03Btk->searchUpdateMaterialID(mCpSel.Scr);
    mCpSel03BtkFrame = 0;
    mCpSelBookBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    mCpSelBookBpk->searchUpdateMaterialID(mCpSel.Scr);

    btk = JKRGetNameResource("zelda_file_select_copy_select.btk", mpArchive);
    JUT_ASSERT(5343, btk != NULL);
    mCpSelBookBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mCpSelBookBtk->searchUpdateMaterialID(mCpSel.Scr);

    void* brk = JKRGetNameResource("zelda_file_select_copy_select.brk", mpArchive);
    JUT_ASSERT(5352, brk != NULL);
    mCpSelBookBrk = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    mCpSelBookBrk->searchUpdateMaterialID(mCpSel.Scr);
    mCpSelBookBpkFrame = 0;
    mCpSelBookBtkFrame = 0;
    mCpSelBookBrkFrame = 0;

    for (int i = 0; i < 2; i++) {
        mCpSelPane[i] = new CPaneMgr(mCpSel.Scr, l_tagName000[i], 0, NULL);
        mCpSelPane_moyo[i] = new CPaneMgr(mCpSel.Scr, l_tagName001[i], 0, NULL);
        mCpSelPane_gold[i] = new CPaneMgr(mCpSel.Scr, l_tagName002[i], 0, NULL);
        mCpSelPane_gold2[i] = new CPaneMgr(mCpSel.Scr, l_tagName003[i], 0, NULL);
        mCpSelPane_book[i] = new CPaneMgr(mCpSel.Scr, l_tagName004[i], 0, NULL);

        mCpSelPane_moyo[i]->getPanePtr()->setAnimation(mCpSelBpk);
        mCpSelPane_gold[i]->getPanePtr()->setAnimation(mCpSelBpk);
        mCpSelPane_gold2[i]->getPanePtr()->setAnimation(mCpSelBpk);
        mCpSelPane_gold[i]->getPanePtr()->setAnimation(mCpSel03Btk);
        mCpSelPane_gold2[i]->getPanePtr()->setAnimation(mCpSel03Btk);
        mCpSelPane_book[i]->getPanePtr()->setAnimation(mCpSelBookBpk);
        mCpSelPane_book[i]->getPanePtr()->setAnimation(mCpSelBookBtk);
        mCpSelPane_book[i]->getPanePtr()->setAnimation(mCpSelBookBrk);

        mCpSel.Scr->search(l_tagName005[i])->hide();
    }

    J2DPane* namePane = mCpSel.Scr->search('name_n');
    namePane->setAnimation(mCpSelBck);
    mCpSelBck->setFrame(0.0f);
    namePane->animationTransform();
    namePane->setAnimation((J2DAnmTransform*)NULL);

    mSelIcon2 = new dSelect_cursor_c(0, 1.0f, NULL);
    JUT_ASSERT(5406, mSelIcon2 != NULL);
    mSelIcon2->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
    Vec center = mCpSelPane[0]->getGlobalVtxCenter(false, 0);
    mSelIcon2->setPos(center.x, center.y, mCpSelPane[0]->getPanePtr(), true);
    mSelIcon2->setAlphaRate(0.0f);

    for (int i = 0; i < 2; i++) {
        mCpFileInfo[i] = new dFile_info_c(mpArchive, 0);
        mCpFileInfo[i]->setBasePane(mCpSel.Scr->search(l_tagName13[i + 1]));
    }

    mCpSel.isShow = false;
}

void dFile_select_c::screenSetYesNo() {
    static u64 l_tagName012[2] = {'w_no_n', 'w_yes_n'};
    static u64 l_tagName013[2] = {'w_no_t', 'w_yes_t'};
    static u64 l_tagName013U[2] = {'f_no_t', 'f_yes_t'};
    static u64 l_tagName9[2] = {'w_no_m', 'w_yes_m'};
    static u64 l_tagName10[2] = {
        'w_no_g',
        'w_yes_g',
    };
    static u64 l_tagName11[2] = {'w_no_gr', 'w_yes_gr'};
    static u8 l_msgNum2[2] = {0x08, 0x07};

    mYnSel.ScrYn = new J2DScreen();
    JUT_ASSERT(5435, mYnSel.ScrYn != NULL);
    mYnSel.ScrYn->setPriority("zelda_file_select_yes_no_window.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(mYnSel.ScrYn);

    void* bck = JKRGetNameResource("zelda_file_select_yes_no_window.bck", mpArchive);
    mYnSelBck = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mYnSelBck2 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mYnSelBck3 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    mYnSelBck->searchUpdateMaterialID(mYnSel.ScrYn);
    mYnSelBck2->searchUpdateMaterialID(mYnSel.ScrYn);
    mYnSelBck3->searchUpdateMaterialID(mYnSel.ScrYn);

    for (int i = 0; i < 2; i++) {
        mYnSelPane[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName012[i], 0, NULL);
#if VERSION == VERSION_GCN_JPN
        mYnSelTxtPane[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName013[i], 0, NULL);
        mYnSel.ScrYn->search(l_tagName013U[i])->hide();
#else
        mYnSelTxtPane[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName013U[i], 0, NULL);
        mYnSel.ScrYn->search(l_tagName013[i])->hide();
#endif
        ((J2DTextBox*)mYnSelTxtPane[i]->getPanePtr())->setFont(mDoExt_getMesgFont());

        char textBuf[16];
        fopMsgM_messageGet(textBuf, l_msgNum2[i]);
        ((J2DTextBox*)mYnSelTxtPane[i]->getPanePtr())->setString(textBuf);
    }

    void* bpk = JKRGetNameResource("zelda_file_select_yes_no_window.bpk", mpArchive);
    JUT_ASSERT(5483, bpk != NULL);
    mYnSelBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    mYnSelBpk->searchUpdateMaterialID(mYnSel.ScrYn);
    mYnSelBpkFrame = 0;

    void* btk = JKRGetNameResource("zelda_file_select_yes_no_window.btk", mpArchive);
    JUT_ASSERT(5493, btk != NULL);
    mYnSelBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mYnSelBtk->searchUpdateMaterialID(mYnSel.ScrYn);
    mYnSelBtkFrame = 0;

    for (int i = 0; i < 2; i++) {
        mYnSelPane_m[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName9[i], 0, NULL);
        mYnSelPane_g[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName10[i], 0, NULL);
        mYnSelPane_gr[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName11[i], 0, NULL);

        mYnSelPane_m[i]->getPanePtr()->setAnimation(mYnSelBpk);
        mYnSelPane_g[i]->getPanePtr()->setAnimation(mYnSelBpk);
        mYnSelPane_gr[i]->getPanePtr()->setAnimation(mYnSelBpk);
        mYnSelPane_g[i]->getPanePtr()->setAnimation(mYnSelBtk);
        mYnSelPane_gr[i]->getPanePtr()->setAnimation(mYnSelBtk);
    }
}

void dFile_select_c::screenSet3Menu() {
    static u64 l_tagName6[3] = {'w_sat_mo', 'w_del_mo', 'w_cop_mo'};
    static u64 l_tagName7[3] = {'w_sat_g', 'w_del_g', 'w_cop_g'};
    static u64 l_tagName8[3] = {'w_sat_gr', 'w_del_gr', 'w_cop_gr'};
    static u64 l_tagName1[3] = {'w_sta_n', 'w_del_n', 'w_cop_n'};
    static u64 l_tagName011[3] = {'w_sta', 'w_del', 'w_cop_t'};
    static u64 l_tagName011U[3] = {'f_sta', 'f_del', 'f_cop_t'};
    static u8 l_msgNum[3] = {0x57, 0x58, 0x56};

    m3mSel.Scr3m = new J2DScreen();
    JUT_ASSERT(5530, m3mSel.Scr3m != NULL);
    m3mSel.Scr3m->setPriority("zelda_file_select_3menu_window.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(m3mSel.Scr3m);

    void* bck = JKRGetNameResource("zelda_file_select_3menu_window.bck", mpArchive);
    m3mBck = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    m3mBck2 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    m3mBck->searchUpdateMaterialID(m3mSel.Scr3m);
    m3mBck2->searchUpdateMaterialID(m3mSel.Scr3m);

    void* bpk = JKRGetNameResource("zelda_file_select_3menu_window.bpk", mpArchive);
    JUT_ASSERT(5552, bpk != NULL);
    m3mBpk = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    m3mBpk->searchUpdateMaterialID(m3mSel.Scr3m);
    m3mBpkFrame = 0;

    void* btk = JKRGetNameResource("zelda_file_select_3menu_window.btk", mpArchive);
    JUT_ASSERT(5562, btk != NULL);
    m3mBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    m3mBtk->searchUpdateMaterialID(m3mSel.Scr3m);
    m3mBtkFrame = 0;

    m3mMenuPane = m3mSel.Scr3m->search('wmenu_n');
    m3mMenuPane->setAnimation(m3mBck);
    m3mBck->setFrame(799.0f);
    m3mMenuPane->animationTransform();

    for (int i = 0; i < 3; i++) {
        m3mSelPane[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName1[i], 0, NULL);
#if VERSION == VERSION_GCN_JPN
        m3mSelTextPane[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName011[i], 0, NULL);
        m3mSel.Scr3m->search(l_tagName011U[i])->hide();
#else
        m3mSelTextPane[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName011U[i], 0, NULL);
        m3mSel.Scr3m->search(l_tagName011[i])->hide();
#endif

        ((J2DTextBox*)m3mSelTextPane[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        char textbuf[32];
        fopMsgM_messageGet(textbuf, l_msgNum[i]);
        ((J2DTextBox*)m3mSelTextPane[i]->getPanePtr())->setString(textbuf);

        m3mSelPane_mo[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName6[i], 0, NULL);
        m3mSelPane_g[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName7[i], 0, NULL);
        m3mSelPane_gr[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName8[i], 0, NULL);

        m3mSelPane_mo[i]->getPanePtr()->setAnimation(m3mBpk);
        m3mSelPane_g[i]->getPanePtr()->setAnimation(m3mBpk);
        m3mSelPane_gr[i]->getPanePtr()->setAnimation(m3mBpk);

        m3mSelPane_g[i]->getPanePtr()->setAnimation(m3mBtk);
        m3mSelPane_gr[i]->getPanePtr()->setAnimation(m3mBtk);
    }
}

void dFile_select_c::screenSetDetail() {
    mSelDt.ScrDt = new J2DScreen();
    JUT_ASSERT(5622, mSelDt.ScrDt != NULL);
    mSelDt.ScrDt->setPriority("zelda_file_select_details.blo", 0x1100000, mpArchive);

    void* btk = JKRGetNameResource("zelda_file_select_details.btk", mpArchive);
    JUT_ASSERT(5628, btk != NULL);
    mSelDtBtk = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mSelDt.ScrDt->setAnimation(mSelDtBtk);
    mSelDtBtkFrame = 0;

    mSelDt.mpPane2 = mSelDt.ScrDt->search('n_all');
    mSelDtPane_mset = new CPaneMgr(mSelDt.ScrDt, 'mset_p_n', 0, NULL);

    mpFileSelect3d->setBasePane(mSelDtPane_mset);
    mpFileSelect3d->setBase2Pane(fileSel.Scr->search('w_sub_n'));

    mSelDt.ScrDt->search('d_win_n')->setUserInfo('n_43');
}

void dFile_select_c::setWakuAnm() {
    field_0x0098->setFrame(3000.0f);

    for (int i = 0; i < 3; i++) {
        mSelFileMoyoPane[i]->getPanePtr()->setAnimation(field_0x0098);
        mSelFileGoldPane[i]->getPanePtr()->setAnimation(field_0x0098);
        mSelFileGold2Pane[i]->getPanePtr()->setAnimation(field_0x0098);
        mSelFileMoyoPane[i]->getPanePtr()->animationTransform();
        mSelFileGoldPane[i]->getPanePtr()->animationTransform();
        mSelFileGold2Pane[i]->getPanePtr()->animationTransform();
        mSelFileMoyoPane[i]->setAlpha(0);
        mSelFileGoldPane[i]->setAlpha(0);
        mSelFileGold2Pane[i]->setAlpha(0);
    }
}

void dFile_select_c::displayInit() {
    field_0x03b1 = 0;
    mIsSelectEnd = false;
    mWaitTimer = g_fsHIO.appear_display_wait_frames;
    field_0x00b8 = 0;
    field_0x014a = false;
    field_0x0360 = false;
    field_0x0108 = false;
    field_0x021e = 0;

    #if PLATFORM_GCN
    field_0x00b9 = 0;
    field_0x014b = false;
    #endif

    field_0x0281 = false;
    field_0x0283 = false;
    mDoMemCd_clearProbeStat();

    field_0x0248 = false;
    field_0x024a = false;
    field_0x0249 = false;
    field_0x024b = false;

    mModoruTxtPane->setAlpha(0);
    mKetteiTxtPane->setAlpha(0);
    mBbtnPane->setAlpha(0x80);
    mAbtnPane->setAlpha(0x80);

    mDataSelProc = DATASELPROC_MEMCARD_CHECK_MAIN;
    mCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
}

void dFile_select_c::setSaveData() {
    dSv_save_c* pSave = (dSv_save_c*)mSaveData;
    for (int i = 0; i < SAVEDATA_NUM; i++) {
        int res = mFileInfo[i]->setSaveData(pSave, mDoMemCdRWm_TestCheckSumGameData(pSave), i);
        if (res == -1) {
            mIsNoData[i] = true;
            mIsDataNew[i] = false;
        } else {
            mIsDataNew[i] = res;
            mIsNoData[i] = false;
        }
        pSave = (dSv_save_c*)((u8*)pSave + SAVEDATA_SIZE);
    }
}

void dFile_select_c::headerTxtSet(u16 i_msgId, u8 i_type, u8 param_3) {
    u8 dispIdx = mHeaderTxtDispIdx ^ 1;
    if (param_3 != 0) {
        dispIdx = mHeaderTxtDispIdx;
    }

    if (i_msgId == 0xFFFF) {
        strcpy(mHeaderStringPtr[dispIdx], "");
    } else {
        static f32 fontsize[2] = {21.0f, 27.0f};
        #if VERSION == VERSION_GCN_JPN
            static f32 linespace[2] = {22.0f, 20.0f};
            static f32 charspace[2] = {2.0f, 3.0f};
        #else
            static f32 linespace[2] = {21.0f, 20.0f};
            static f32 charspace[2] = {0.0f, 0.0f};
        #endif

        ((J2DTextBox*)mHeaderTxtPane[dispIdx]->getPanePtr())->setFont(fileSel.font[i_type]);
        ((J2DTextBox*)mHeaderTxtPane[dispIdx]->getPanePtr())->setFontSize(fontsize[i_type], fontsize[i_type]);
        ((J2DTextBox*)mHeaderTxtPane[dispIdx]->getPanePtr())->setLineSpace(linespace[i_type]);
        ((J2DTextBox*)mHeaderTxtPane[dispIdx]->getPanePtr())->setCharSpace(charspace[i_type]);
        fileSel.mMessageString->getString(i_msgId,
                                          ((J2DTextBox*)mHeaderTxtPane[dispIdx]->getPanePtr()), NULL,
                                          fileSel.font[i_type], NULL, 0);
    }

    if (param_3 == 0) {
        mHeaderTxtPane[mHeaderTxtDispIdx]->alphaAnimeStart(0);
        mHeaderTxtPane[mHeaderTxtDispIdx ^ 1]->alphaAnimeStart(0);
        field_0x021d = 0;
    }
}

bool dFile_select_c::headerTxtChangeAnm() {
    if (field_0x021d != 0) {
        return true;
    }

    bool ret = false;

    bool alphaAnime1 = mHeaderTxtPane[mHeaderTxtDispIdx]->alphaAnime(g_fsHIO.char_switch_frames, 0xFF, 0, 0);
    bool alphaAnime2 = mHeaderTxtPane[mHeaderTxtDispIdx ^ 1]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xFF, 0);
    int msgKeyWaitTimer = dMeter2Info_getMsgKeyWaitTimer();

    if (alphaAnime1 == true && alphaAnime2 == true && msgKeyWaitTimer == 0) {
        mHeaderTxtDispIdx ^= 1;
        field_0x021d = 1;
        ret = true;
    }

    return ret;
}

void dFile_select_c::modoruTxtChange(u8 param_1) {
    if (param_1 != 0) {
        fopMsgM_messageGet(mModoruStringPtr, 981);
        field_0x024c = 1;
    } else {
        fopMsgM_messageGet(mModoruStringPtr, 84);
        field_0x024c = 0;
    }
}

void dFile_select_c::modoruTxtDispAnmInit(u8 param_1) {
    field_0x024a = param_1;
    if ((param_1 != 0 || mModoruTxtPane->getAlpha() != 0) &&
        (param_1 != 1 || mModoruTxtPane->getAlpha() != 0xff))
    {
        mModoruTxtPane->alphaAnimeStart(0);
        mBbtnPane->alphaAnimeStart(0);
    } else {
        mModoruTxtPane->alphaAnimeStart(g_fsHIO.char_switch_frames);
        mBbtnPane->alphaAnimeStart(g_fsHIO.char_switch_frames);
    }
}

bool dFile_select_c::modoruTxtDispAnm() {
    bool iVar1;
    bool iVar2;
    if (field_0x024a != 0) {
        iVar1 = mModoruTxtPane->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xff, 0);
        iVar2 = mBbtnPane->alphaAnime(g_fsHIO.char_switch_frames, 0x80, 0xff, 0);
    } else {
        iVar1 = mModoruTxtPane->alphaAnime(g_fsHIO.char_switch_frames, 0xff, 0, 0);
        iVar2 = mBbtnPane->alphaAnime(g_fsHIO.char_switch_frames, 0xff, 0x80, 0);
    }

    if (iVar1 == true && iVar2 == true) {
        return true;
    }

    return false;
}

void dFile_select_c::ketteiTxtDispAnmInit(u8 param_1) {
    field_0x024b = param_1;
    if ((param_1 != 0 || mKetteiTxtPane->getAlpha() != 0) &&
        (param_1 != 1 || mKetteiTxtPane->getAlpha() != 0xff))
    {
        mKetteiTxtPane->alphaAnimeStart(0);
        mAbtnPane->alphaAnimeStart(0);
    } else {
        mKetteiTxtPane->alphaAnimeStart(g_fsHIO.char_switch_frames);
        mAbtnPane->alphaAnimeStart(g_fsHIO.char_switch_frames);
    }
}

bool dFile_select_c::ketteiTxtDispAnm() {
    bool iVar1;
    bool iVar2;
    if (field_0x024b != 0) {
        iVar1 = mKetteiTxtPane->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xff, 0);
        iVar2 = mAbtnPane->alphaAnime(g_fsHIO.char_switch_frames, 0x80, 0xff, 0);
    } else {
        iVar1 = mKetteiTxtPane->alphaAnime(g_fsHIO.char_switch_frames, 0xff, 0, 0);
        iVar2 = mAbtnPane->alphaAnime(g_fsHIO.char_switch_frames, 0xff, 0x80, 0);
    }

    if (iVar1 == true && iVar2 == true) {
        return true;
    }

    return false;
}

void dFile_select_c::selectWakuAlpahAnmInit(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    mSelFileMoyoPane[param_1]->alphaAnimeStart(0);
    mSelFileGoldPane[param_1]->alphaAnimeStart(0);
    mSelFileGold2Pane[param_1]->alphaAnimeStart(0);
    field_0x0194[param_1] = param_2;
    field_0x0197[param_1] = param_3;
    field_0x019a[param_1] = param_4;
}

bool dFile_select_c::selectWakuAlpahAnm(u8 param_1) {
    if (param_1 == 0xff) {
        return true;
    }

    bool rv = false;
    bool iVar2 = mSelFileMoyoPane[param_1]->alphaAnime(field_0x019a[param_1], field_0x0194[param_1],
                                                   field_0x0197[param_1], 0);
    bool iVar3 = mSelFileGoldPane[param_1]->alphaAnime(field_0x019a[param_1], field_0x0194[param_1],
                                                   field_0x0197[param_1], 0);
    bool iVar4 = mSelFileGold2Pane[param_1]->alphaAnime(field_0x019a[param_1], field_0x0194[param_1],
                                                   field_0x0197[param_1], 0);
    if (iVar2 == true && iVar3 == true && iVar4 == true) {
        rv = true;
    }

    return rv;
}

void dFile_select_c::selFileCursorShow() {
    mSelFileMoyoPane[mSelectNum]->setAlpha(0xff);
    mSelFileGoldPane[mSelectNum]->setAlpha(0xff);
    mSelFileGold2Pane[mSelectNum]->setAlpha(0xff);

    Vec local_1c = mSelFilePanes[mSelectNum]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(local_1c.x, local_1c.y, mSelFilePanes[mSelectNum]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
    mSelIcon->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
}

void dFile_select_c::menuWakuAlpahAnmInit(u8 i_idx, u8 param_1, u8 param_2, u8 param_3) {
    m3mSelPane_mo[i_idx]->alphaAnimeStart(0);
    m3mSelPane_g[i_idx]->alphaAnimeStart(0);
    m3mSelPane_gr[i_idx]->alphaAnimeStart(0);
    field_0x0388[i_idx] = param_1;
    field_0x038b[i_idx] = param_2;
    field_0x038e[i_idx] = param_3;
    m3mSelTextPane[i_idx]->colorAnimeStart(0);
}

bool dFile_select_c::menuWakuAlpahAnm(u8 param_1) {
    bool rv = false;
    bool iVar1 = m3mSelPane_mo[param_1]->alphaAnime(field_0x038e[param_1], field_0x0388[param_1],
                                                   field_0x038b[param_1], 0);
    bool iVar2 = m3mSelPane_g[param_1]->alphaAnime(field_0x038e[param_1], field_0x0388[param_1],
                                                   field_0x038b[param_1], 0);
    bool iVar3 = m3mSelPane_gr[param_1]->alphaAnime(field_0x038e[param_1], field_0x0388[param_1],
                                                   field_0x038b[param_1], 0);
    bool var_r25 = m3mSelTextPane[param_1]->colorAnime(field_0x038e[param_1], m3mSelTextPane[param_1]->getInitBlack(),
                                    m3mSelTextPane[param_1]->getInitBlack(),
                                    JUtility::TColor(0xff, 0xff, 0xff, 0xff),
                                    JUtility::TColor(0x96, 0x96, 0x96, 0xff), 0);
    if (iVar1 == 1 && iVar2 == 1 && iVar3 == 1) {
        rv = true;
    }

    return rv;
}

void dFile_select_c::menuCursorShow() {
    if (mSelectMenuNum != 0xff) {
        ((J2DTextBox*)m3mSelTextPane[mSelectMenuNum]->getPanePtr())->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
        m3mSelPane_mo[mSelectMenuNum]->setAlpha(0xff);
        m3mSelPane_g[mSelectMenuNum]->setAlpha(0xff);
        m3mSelPane_gr[mSelectMenuNum]->setAlpha(0xff);

        Vec local_24 = m3mSelPane[mSelectMenuNum]->getGlobalVtxCenter(false, 0);
        mSelIcon->setPos(local_24.x, local_24.y, m3mSelPane[mSelectMenuNum]->getPanePtr(), true);
        mSelIcon->setAlphaRate(1.0f);
        mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    }
}

void dFile_select_c::yesnoWakuAlpahAnmInit(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    if (param_1 != 0xff) {
        mYnSelPane_m[param_1]->alphaAnimeStart(0);
        mYnSelPane_g[param_1]->alphaAnimeStart(0);
        mYnSelPane_gr[param_1]->alphaAnimeStart(0);
        field_0x01b8[param_1] = param_2;
        field_0x01ba[param_1] = param_3;
        field_0x01bc[param_1] = param_4;
        mYnSelTxtPane[param_1]->colorAnimeStart(0);
    }
}

bool dFile_select_c::yesnoWakuAlpahAnm(u8 param_1) {
    bool rv = false;
    bool iVar5 = true;
    bool iVar4 = true;
    bool iVar3 = true;
    bool iVar2 = true;
    if (param_1 != 0xff) {
        iVar5 = mYnSelPane_m[param_1]->alphaAnime(field_0x01bc[param_1], field_0x01b8[param_1],
                                                  field_0x01ba[param_1], 0);
        iVar4 = mYnSelPane_g[param_1]->alphaAnime(field_0x01bc[param_1], field_0x01b8[param_1],
                                                  field_0x01ba[param_1], 0);
        iVar3 = mYnSelPane_gr[param_1]->alphaAnime(field_0x01bc[param_1], field_0x01b8[param_1],
                                                  field_0x01ba[param_1], 0);
        iVar2 = mYnSelTxtPane[param_1]->colorAnime(
            field_0x01bc[param_1], mYnSelTxtPane[param_1]->getInitBlack(),
            mYnSelTxtPane[param_1]->getInitBlack(), JUtility::TColor(0xff, 0xff, 0xff, 0xff),
            JUtility::TColor(0x96, 0x96, 0x96, 0xff), 0);
    }

    if (iVar5 == true && iVar4 == true && iVar3 == true && iVar2 == true) {
        rv = true;
    }

    return rv;
}

void dFile_select_c::_draw() {
    if (!mHasDrawn) {
        dComIfGd_set2DOpa(&fileSel);

        for (int i = 0; i < 3; i++) {
            mFileInfo[i]->draw();
        }

        dComIfGd_set2DOpa(&mSelDt);
        mpFileSelect3d->draw();

        if (mCpSel.isShow != false) {
            dComIfGd_set2DOpa(&mCpSel);

            for (int i = 0; i < 2; i++) {
                mCpFileInfo[i]->draw();
            }
        }

        if (field_0x0128 != false) {
            mpName->draw();
        }

        dComIfGd_set2DOpa(&m3mSel);
        dComIfGd_set2DOpa(&mYnSel);
        mpFileWarning->draw();
        dComIfGd_set2DOpa(mSelIcon);
        dComIfGd_set2DOpa(mSelIcon2);

        #if PLATFORM_GCN
        mpFadePict->draw(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                           mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(), false, false,
                           false);
        #endif
    }
}

void dDlst_FileSel_c::draw() {
    J2DGrafContext* graf = dComIfGp_getCurrentGrafPort();
    Scr->draw(0.0f, 0.0f, graf);
}

void dDlst_FileSelDt_c::draw() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    MtxP local_98 = (MtxP)&mpPane->getGlbMtx()[0][0];
    Mtx auStack_60;
    Mtx auStack_90;
    MTXTrans(auStack_60, mpPane->getWidth() / 2, mpPane->getHeight() / 2, 0.0f);
    MTXConcat(local_98, auStack_60, local_98);
    MTXScale(auStack_90, mpPane->getWidth() / mpPane2->getWidth(),
             mpPane->getHeight() / mpPane2->getHeight(), 1.0f);
    MTXConcat(local_98, auStack_90, local_98);
    mpPane2->setMtx(local_98);
    ScrDt->draw(0.0f, 0.0f, grafContext);
}

void dDlst_FileSelCp_c::draw() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    MtxP local_98 = (MtxP)&mpPane1->getGlbMtx()[0][0];
    Mtx auStack_90;
    MTXScale(auStack_90, mpPane1->getWidth() / mpPane2->getWidth(),
             mpPane1->getHeight() / mpPane2->getHeight(), 1.0f);
    MTXConcat(local_98, auStack_90, local_98);
    mpPane2->setMtx(local_98);
    Scr->draw(0.0f, 0.0f, grafContext);
}

void dDlst_FileSelYn_c::draw() {
    J2DGrafContext* graf = dComIfGp_getCurrentGrafPort();
    ScrYn->draw(0.0f, 0.0f, graf);
}

void dDlst_FileSel3m_c::draw() {
    J2DGrafContext* graf = dComIfGp_getCurrentGrafPort();
    Scr3m->draw(0.0f, 0.0f, graf);
}

void dFile_select_c::errorMoveAnmInitSet(int param_1, int param_2) {
    mErrorMsgPane->setAnimation(field_0x0090);
    field_0x0130 = param_1;
    field_0x0134 = param_2;
    field_0x0090->setFrame(field_0x0130);
    mErrorMsgPane->animationTransform();

    #if PLATFORM_GCN
    field_0x014b = true;
    #endif
}

bool dFile_select_c::errorMoveAnm() {
    bool ret;
    if (field_0x0130 != field_0x0134) {
        if (field_0x0130 < field_0x0134) {
            field_0x0130 += 2;

            if (field_0x0130 > field_0x0134)
                field_0x0130 = field_0x0134;
        } else {
            field_0x0130 -= 2;

            if (field_0x0130 < field_0x0134)
                field_0x0130 = field_0x0134;
        }

        field_0x0090->setFrame(field_0x0130);
        mErrorMsgPane->animationTransform();
        ret = false;
    } else {
        mErrorMsgPane->setAnimation((J2DAnmTransform*)NULL);

        if (field_0x0134 == 0xb2b) {
            field_0x014a = true;
        } else {
            field_0x014a = false;
        }

        #if PLATFORM_GCN
        field_0x014b = false;
        #endif

        ret = true;
    }

    return ret;
}

void dFile_select_c::errDispInitSet(int param_1, int param_2) {
    if (param_2 != 0) {
        headerTxtSet(0xffff, 0, 0);
    } else {
        #if PLATFORM_GCN
        headerTxtSet(1, 0, 0);
        #else
        headerTxtSet(1, 1, 0);
        #endif
    }

    #if PLATFORM_GCN
    field_0x021e = 0;
    #endif

    mErrorMsgTxtPane[mErrorTxtDispIdx]->setAlpha(0xff);
    mErrorMsgTxtPane[mErrorTxtDispIdx ^ 1]->setAlpha(0);
    fileSel.mMessageString->getString(param_1,
                                       (J2DTextBox*)mErrorMsgTxtPane[mErrorTxtDispIdx]->getPanePtr(), NULL,
                                       fileSel.font[0], NULL, 0);
    #if PLATFORM_GCN
    if (field_0x014a || field_0x014b)
    #else
    if (field_0x014a)
    #endif
    {
        errorMoveAnmInitSet(2859, 2849);
    }

    #if PLATFORM_GCN
    if (field_0x00b8 != 0 || field_0x00b9 != 0)
    #else
    if (field_0x00b8 != 0)
    #endif
    {
        mpFileSelect3d->drawOff();
        selectDataBaseMoveAnmInitSet(33, 1);
    }

    if (field_0x0108 || field_0x0281) {
        yesnoMenuMoveAnmInitSet(1149, 1139);
    }

    if (field_0x0360 || field_0x0283) {
        menuMoveAnmInitSet(809, 799);
    }

    if (field_0x0128 != 0) {
        nameMoveAnmInitSet(3369, 3359);
    }

    if (mCpSel.isShow) {
        copySelMoveAnmInitSet(3369, 3359);
    }

    modoruTxtDispAnmInit(0);
    ketteiTxtDispAnmInit(0);

    #if PLATFORM_GCN
    mpFileWarning->init();
    #endif

    mSelIcon->setAlphaRate(0.0f);
    mSelIcon2->setAlphaRate(0.0f);
    mCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_INIT_OPEN;
}

#if PLATFORM_WII || PLATFORM_SHIELD
void dFile_select_c::errDispInitSet(char* i_errMesg) {
    headerTxtSet(1, 1, 0);

    mErrorMsgTxtPane[mErrorTxtDispIdx]->setAlpha(0xFF);
    mErrorMsgTxtPane[mErrorTxtDispIdx ^ 1]->setAlpha(0);

    strcpy(mErrorMsgStringPtr[mErrorTxtDispIdx], i_errMesg);

    if (field_0x014a) {
        errorMoveAnmInitSet(2859, 2849);
    }

    if (field_0x00b8 != 0) {
        selectDataBaseMoveAnmInitSet(33, 1);
    }

    if (field_0x0108 || field_0x0281) {
        yesnoMenuMoveAnmInitSet(1149, 1139);
    }

    if (field_0x0360 || field_0x0283) {
        menuMoveAnmInitSet(809, 799);
    }

    if (field_0x0128 != 0) {
        nameMoveAnmInitSet(3369, 3359);
    }

    if (mCpSel.isShow) {
        copySelMoveAnmInitSet(3369, 3359);
    }

    modoruTxtDispAnmInit(0);
    ketteiTxtDispAnmInit(0);

    mSelIcon->setAlphaRate(0.0f);
    mSelIcon2->setAlphaRate(0.0f);
    mCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_INIT_OPEN;
}
#endif

typedef void (dFile_select_c::*MemCardCheckFuncT)();
static MemCardCheckFuncT MemCardCheckProc[] = {
    &dFile_select_c::MemCardStatCheck,
    &dFile_select_c::MemCardLoadWait,
    &dFile_select_c::MemCardErrMsgWaitKey,
    &dFile_select_c::MemCardNoSaveSelDisp,
    &dFile_select_c::MemCardErrMsgWaitNoSaveSel,
    &dFile_select_c::MemCardErrMsgWaitFormatSel,
    &dFile_select_c::MemCardFormatYesSelDisp,
    &dFile_select_c::MemCardFormatNoSelDisp,
    &dFile_select_c::MemCardErrMsgWaitFormatSel2,
    &dFile_select_c::MemCardFormatYesSel2Disp,
    &dFile_select_c::MemCardFormat,
    &dFile_select_c::MemCardFormatWait,
    &dFile_select_c::MemCardFormatCheck,
    &dFile_select_c::MemCardMakeGameFileSel,
    &dFile_select_c::MemCardMakeGameFileSelDisp,
    &dFile_select_c::MemCardMakeGameFile,
    &dFile_select_c::MemCardMakeGameFileWait,
    &dFile_select_c::MemCardMakeGameFileCheck,
    &dFile_select_c::MemCardNoFileSpaceDisp,
    &dFile_select_c::MemCardGotoIPLSelectDisp,
    &dFile_select_c::MemCardGotoIPLSelect,
    &dFile_select_c::MemCardGotoIPL,
    &dFile_select_c::MemCardMsgWindowInitOpen,
    &dFile_select_c::MemCardMsgWindowOpen,
    &dFile_select_c::MemCardMsgWindowClose,
    &dFile_select_c::MemCardErrYesNoCursorMoveAnm,
    &dFile_select_c::MemCardSaveDataClear,

    #if PLATFORM_WII || PLATFORM_SHIELD
    &dFile_select_c::nandStatCheck,
    &dFile_select_c::gameFileInitSel,
    &dFile_select_c::gameFileInitSelDisp,
    &dFile_select_c::gameFileInit,
    &dFile_select_c::gameFileInitCheck,
    &dFile_select_c::MemCardLoadWaitCard,
    &dFile_select_c::cardDataCopySel,
    &dFile_select_c::cardDataCopySel2Disp,
    &dFile_select_c::cardDataCopySel2,
    &dFile_select_c::loadWaitNand,
    &dFile_select_c::loadNandFile,
    #endif
};

void dFile_select_c::MemCardCheckMain() {
    (this->*MemCardCheckProc[mCardCheckProc])();
}

void dFile_select_c::MemCardStatCheck() {
#if PLATFORM_WII
    loadFileNAND();
    return;
#endif

    u32 status = mDoMemCd_getStatus(0);
    if (status == 14) {
        return;
    }

#if PLATFORM_GCN
    switch (status) {
    case 0:
        errDispInitSet(2, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        break;
    case 8:
        errDispInitSet(3, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        break;
    case 9:
        errDispInitSet(4, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        break;
    case 10:
        errDispInitSet(5, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        break;
    case 6:
    case 7:
        errDispInitSet(6, 0);
        field_0x0280 = true;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_FORMAT_SEL;
        break;
    case 11:
    case 12:
        errDispInitSet(9, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::noFileSpaceDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_FILESPACE_DISP;
        break;
    case 2:
        mDoMemCd_Load();
        mCardCheckProc = MEMCARDCHECKPROC_LOAD_WAIT;
        break;
    case 1:
        errDispInitSet(22, 0);
        field_0x0280 = true;
        mNextCardCheckProc = MEMCARDCHECKPROC_MAKE_GAMEFILE_SEL;
        break;
    }
#else
    switch (status) {
    case 8:
    case 9:
    case 10:
    case 6:
    case 7:
    case 11:
    case 12:
    case 1:
        errDispInitSet(0xEC8, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = NULL;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_LOAD_NAND_FILE;
        break;
    case 0:
    default:
        if (field_0x4332 == 1) {
            field_0x03b1 = 1;
            errDispInitSet(0xED0, 0);
            field_0x0280 = false;
            mWindowCloseMsgDispCb = NULL;
            mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
            mKeyWaitMsgDispCb = NULL;
            mKeyWaitCardCheckProc = MEMCARDCHECKPROC_LOAD_NAND_FILE;
            field_0x4332 = 0;
        } else {
            loadFileNAND();
        }
        break;
    case 2:
        mDoMemCd_Load();
        mCardCheckProc = MEMCARDCHECKPROC_LOAD_WAIT_CARD;
        break;
    }
#endif
}

void dFile_select_c::MemCardLoadWait() {
    #if PLATFORM_GCN
    int loadRes = mDoMemCd_LoadSync(mSaveData, sizeof(mSaveData), 0);
    #else
    int loadRes = mDoMemCd_LoadSyncNAND(mSaveData, sizeof(mSaveData), 0);
    #endif
    if (loadRes == 0) {
        return;
    }

    if (loadRes == 1) {
        #if DEBUG
        if (mDoMemCd_getDataVersion() != 6) {
            char errmsg[264];
            // "Savedata version is different\n\nVersion %d\n\nFormatting data."
            sprintf(errmsg, "セーブデータのバージョンが違います\n\nバージョン %d\n\nデータを初期化します。", mDoMemCd_getDataVersion());
            errDispInitSet(errmsg);
            field_0x0280 = false;
            mWindowCloseMsgDispCb = NULL;
            mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
            mKeyWaitMsgDispCb = &dFile_select_c::saveDataClearInit;
            mKeyWaitCardCheckProc = MEMCARDCHECKPROC_SAVEDATA_CLEAR;
        } else
        #endif
        {
            #if PLATFORM_GCN
            if (dComIfGs_getNoFile() != 0) {
                dComIfGs_setNoFile(0);
            }
            #endif

            dataSelectInAnmSet();
            
            #if PLATFORM_GCN
            if (field_0x014a || field_0x014b)
            #else
            if (field_0x014a)
            #endif
            {
                errorMoveAnmInitSet(2859, 2849);
            }

            if (field_0x0108 || field_0x0281) {
                yesnoMenuMoveAnmInitSet(1149, 1139);
            }

            if (field_0x0128) {
                nameMoveAnmInitSet(3369, 3359);
            }

            if (mCpSel.isShow) {
                #if PLATFORM_GCN
                mSelIcon2->setAlphaRate(0.0f);
                #endif

                copySelMoveAnmInitSet(3369, 3359);
            }

            #if PLATFORM_GCN
            if (field_0x0360 || field_0x0283) {
                menuMoveAnmInitSet(809, 799);
            }
            #endif

            modoruTxtDispAnmInit(0);
            headerTxtSet(0xffff, 0, 0);
            field_0x021e = 0;
            mDataSelProc = DATASELPROC_DATA_SELECT_IN;
        }
    } else if (loadRes == 2) {
        #if PLATFORM_WII || PLATFORM_SHIELD
        OS_REPORT("== NAND LOAD ERR %d ==\n", loadRes);
        mCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
        #else
        mCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
        #endif
    }
}

#if PLATFORM_WII || PLATFORM_SHIELD
void dFile_select_c::nandStatCheck() {
    u32 status = mDoMemCd_getStatusNAND();

    switch (status) {
    case 1:
        mDoMemCd_LoadNAND();
        mCardCheckProc = MEMCARDCHECKPROC_LOAD_WAIT;
        break;
    case 0:
        errDispInitSet(0xEE8, 0);
        field_0x0280 = true;
        mNextCardCheckProc = MEMCARDCHECKPROC_MAKE_GAMEFILE_SEL;
        break;
    case 4:
    case 6:
        errDispInitSet(0xEDD, 0);
        field_0x0280 = true;
        mNextCardCheckProc = MEMCARDCHECKPROC_GAMEFILE_INIT_SEL;
        break;
    case 5:
        errDispInitSet(0xEDC, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        break;
    case 7:
        errDispInitSet(0xEE3, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::iplSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_GOTO_IPL_SELECT_DISP;
        break;
    case 8:
        errDispInitSet(0xEE4, 0);
        field_0x0280 = false;
        mWindowCloseMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        mKeyWaitMsgDispCb = &dFile_select_c::iplSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_GOTO_IPL_SELECT_DISP;
        break;
    }
}

void dFile_select_c::loadFileNAND() {
    field_0x03b1 = 1;
    s32 ret = mDoMemCd_checkNANDFile();
    mCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
}

void dFile_select_c::MemCardLoadWaitCard() {
    s32 result = mDoMemCd_LoadSync(mSaveData, SAVEFILE_SIZE, 0);
    if (result != 0) {
        if (result == 1) {
            bool var_r28 = GCtoWiiTimeConvert();
            if (var_r28) {
                errDispInitSet(0xEC5, 0);  // copy memcard save to wii memory?
                field_0x0280 = 1;
                mNextCardCheckProc = MEMCARDCHECKPROC_CARD_DATA_COPY_SEL;
                field_0x4332 = 1;
            } else {
                errDispInitSet(0xEC8, 0);  // no savefile on memcard. save in wii memory will be loaded instead
                field_0x0280 = 0;
                mWindowCloseMsgDispCb = NULL;
                mNextCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
                mKeyWaitMsgDispCb = NULL;
                mKeyWaitCardCheckProc = MEMCARDCHECKPROC_LOAD_NAND_FILE;
            }
        } else if (result == 2) {
            // "\n=== failed to read from memory card ===\n\n"
            OS_REPORT("\n=== メモリーカードからの読み込み失敗 ===\n\n");
            loadFileNAND();
        }
    }
}

void dFile_select_c::cardDataCopySel() {
    if (errYesNoSelect()) {
        if (field_0x0268 != 0) {
            u32 var_r29 = mDoMemCd_checkNANDFile();
            if (var_r29 == 1) {
                errorTxtSet(0xEC9);
                field_0x0268 = 0;
                field_0x0269 = 1;
                yesnoSelectAnmSet();
                field_0x4333 = field_0x0268;
                mCardCheckProc = MEMCARDCHECKPROC_CARD_DATA_COPY_SEL2_DISP;
            } else {
                s32 var_r28 = mDoMemCd_getStatusNAND();
                if (var_r28 == 0) {
                    field_0x03b1 = 1;
                    memcpy(field_0x2376, mSaveData, SAVEFILE_SIZE);
                    setInitSaveData();
                    dataSelectInAnmSet();

                    if (field_0x014a) {
                        errorMoveAnmInitSet(0xB2B, 0xB21);
                    }

                    if (field_0x0108) {
                        yesnoMenuMoveAnmInitSet(0x47D, 0x473);
                    }

                    headerTxtSet(0xFFFF, 0, 0);
                    field_0x021e = 0;
                    setSaveData();
                    mDataSelProc = DATASELPROC_DATA_SELECT_IN_COPY;
                } else {
                    mCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
                }
            }
        } else {
            #if PLATFORM_SHIELD
            cardDataCopyNoSelect();
            #else
            field_0x4332 = 0;
            loadFileNAND();
            #endif
        }
    }
}

void dFile_select_c::cardDataCopySel2Disp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnSelMove = yesnoSelectMoveAnm();
    bool isYnWakuAlpha = yesnoWakuAlpahAnm(field_0x0269);

    if (isErrorTxtChange == true && isYnSelMove == true && isYnWakuAlpha == true) {
        yesnoCursorShow();
        mCardCheckProc = MEMCARDCHECKPROC_CARD_DATA_COPY_SEL2;
    }
}

void dFile_select_c::cardDataCopySel2() {
    if (errYesNoSelect()) {
        if (field_0x0268 != 0) {
            field_0x03b1 = 1;
            memcpy(field_0x2376, mSaveData, SAVEFILE_SIZE);
            mDoMemCd_LoadNAND();
            mCardCheckProc = MEMCARDCHECKPROC_LOAD_WAIT_NAND;
        } else {
            #if PLATFORM_SHIELD
            cardDataCopyNoSelect();
            #else
            field_0x4332 = 0;
            loadFileNAND();
            #endif
        }
    }
}

void dFile_select_c::cardDataCopyNoSelect() {
    field_0x4332 = 0;
    setSaveData();
    dataSelectInAnmSet();

    if (field_0x014a) {
        errorMoveAnmInitSet(0xB2B, 0xB21);
    }

    if (field_0x0108) {
        yesnoMenuMoveAnmInitSet(0x47D, 0x473);
    }

    if (field_0x0128) {
        nameMoveAnmInitSet(0xD29, 0xD1F);
    }

    if (mCpSel.isShow) {
        copySelMoveAnmInitSet(0xD29, 0xD1F);
    }

    modoruTxtDispAnmInit(0);
    headerTxtSet(0xFFFF, 0, 0);
    field_0x021e = 0;
    mDataSelProc = DATASELPROC_DATA_SELECT_IN;
}

void dFile_select_c::loadWaitNand() {
    s32 temp_r3 = mDoMemCd_LoadSyncNAND(mSaveData, SAVEFILE_SIZE, 0);
    if (temp_r3 != 0) {
        if (temp_r3 == 1) {
            dataSelectInAnmSet();

            if (field_0x014a) {
                errorMoveAnmInitSet(0xB2B, 0xB21);
            }

            if (field_0x0108) {
                yesnoMenuMoveAnmInitSet(0x47D, 0x473);
            }

            headerTxtSet(0xFFFF, 0, 0);
            field_0x021e = 0;
            setSaveData();
            mDataSelProc = DATASELPROC_DATA_SELECT_IN_COPY;
        } else if (temp_r3 == 2) {
            mCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
        }
    }
}

void dFile_select_c::loadNandFile() {
    loadFileNAND();
}
#endif

void dFile_select_c::MemCardErrMsgWaitKey() {
    if (cAPICPad_ANY_BUTTON(PAD_1) != 0 && dMeter2Info_getMsgKeyWaitTimer() == 0) {
        if (mKeyWaitMsgDispCb != NULL) {
            (this->*mKeyWaitMsgDispCb)();
        }
        mCardCheckProc = mKeyWaitCardCheckProc;
    }
}

void dFile_select_c::noFileSpaceDispInit() {
    errorTxtSet(10);
}

void dFile_select_c::MemCardNoFileSpaceDisp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    if (isErrorTxtChange == true) {
        mKeyWaitMsgDispCb = &dFile_select_c::iplSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_GOTO_IPL_SELECT_DISP;
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
    }
}

void dFile_select_c::iplSelDispInit() {
    #if PLATFORM_WII
    errorTxtSet(0xEE5);
    #else
    errorTxtSet(18);
    #endif

    ketteiTxtDispAnmInit(1);
    field_0x0280 = true;
    yesnoMenuMoveAnmInitSet(0x473, 0x47d);
}

void dFile_select_c::MemCardGotoIPLSelectDisp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    bool isKetteiTxtDisp = ketteiTxtDispAnm();
    if (isErrorTxtChange == true && isYnMenuMove == true && isKetteiTxtDisp == true) {
        yesnoCursorShow();
        mCardCheckProc = MEMCARDCHECKPROC_GOTO_IPL_SELECT;
    }
}

void dFile_select_c::MemCardGotoIPLSelect() {
    if (errYesNoSelect()) {
        if (field_0x0268) {
            #if PLATFORM_GCN
            field_0x03b1 = 1;
            #endif

            mWaitTimer = g_fsHIO.field_0x000d;
            mDoGph_gInf_c::startFadeOut(mWaitTimer);
            mFadeFlag = true;
            mCardCheckProc = MEMCARDCHECKPROC_GOTO_IPL;
        } else {
            yesnoWakuAlpahAnmInit(field_0x0268, 0xff, 0, g_fsHIO.select_box_appear_frames);
            noSaveSelDispInit();
            mCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        }
    }
}

void dFile_select_c::MemCardGotoIPL() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    } else {
        mHasDrawn = true;
        mDoGph_gInf_c::startFadeIn(0);
        mFadeFlag = false;

        #if PLATFORM_GCN
        mDoRst::onShutdown();
        #else
        mDoRst::onReturnToMenu();
        #endif
    }
}

void dFile_select_c::noSaveSelDispInit() {
    #if PLATFORM_WII
    errorTxtSet(0xED9);
    #else
    errorTxtSet(19);
    #endif

    field_0x0282 = false;
    if (!field_0x0108) {
        ketteiTxtDispAnmInit(1);
        field_0x0280 = true;
        yesnoMenuMoveAnmInitSet(0x473, 0x47d);
        field_0x0282 = true;
    }
}

void dFile_select_c::MemCardNoSaveSelDisp() {
    bool iVar1 = errorTxtChangeAnm();
    bool iVar3 = true;
    bool iVar2 = true;
    if (field_0x0282) {
        if (field_0x0280) {
            iVar3 = yesnoMenuMoveAnm();
            iVar2 = ketteiTxtDispAnm();
        }
    } else {
        iVar3 = yesnoWakuAlpahAnm(field_0x0268);
    }

    if (iVar1 == true && iVar3 == true && iVar2 == true) {
        if (field_0x0280) {
            yesnoCursorShow();
        }
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_NO_SAVE_SEL;
    }
}

void dFile_select_c::MemCardErrMsgWaitNoSaveSel() {
    if (!errYesNoSelect()) {
        return;
    }

    if (field_0x0268 != 0) {
        setInitSaveData();
        dComIfGs_setCardToMemory((u8*)mSaveData, 0);
        dComIfGs_setNoFile(1);
        dComIfGs_setDataNum(0);

        for (int i = 0; i < 3; i++) {
            mIsDataNew[i] = true;
        }

        headerTxtSet(901, 1, 0);
        mSelIcon->setAlphaRate(1.0f);
    
        char namebuf[32];
        dMeter2Info_getString(0x382, namebuf, 0);
        dComIfGs_setPlayerName(namebuf);
        dMeter2Info_getString(899, namebuf, 0);
        dComIfGs_setHorseName(namebuf);
        mpName->setNextNameStr(dComIfGs_getPlayerName());
        mpName->initial();
    
        modoruTxtChange(1);
        nameMoveAnmInitSet(3359, 3369);
        yesnoMenuMoveAnmInitSet(1149, 1139);
        errorMoveAnmInitSet(2859, 2849);
        modoruTxtDispAnmInit(1);
        ketteiTxtDispAnmInit(1);
        mDataSelProc = DATASELPROC_TO_NAME_MOVE2;
    } else {
        mWindowCloseMsgDispCb = NULL;
        #if PLATFORM_GCN
        mNextCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
        #else
        mNextCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
        #endif
        ketteiTxtDispAnmInit(0);
        errorMoveAnmInitSet(2859, 2849);
        yesnoMenuMoveAnmInitSet(1149, 1139);
        mCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_CLOSE;
    }
}

void dFile_select_c::formatYesSelDispInitSet() {
    errorTxtSet(14);
    field_0x0268 = 0;
    field_0x0269 = 1;

    #if PLATFORM_WII || PLATFORM_SHIELD
    field_0x4333 = field_0x0268;
    #endif

    yesnoSelectAnmSet();
    mCardCheckProc = MEMCARDCHECKPROC_FORMAT_YES_SEL_DISP;
}

void dFile_select_c::formatNoSelDispInitSet() {
    errorTxtSet(17);
    field_0x0280 = false;
    yesnoMenuMoveAnmInitSet(0x47d, 0x473);
    mCardCheckProc = MEMCARDCHECKPROC_FORMAT_NO_SEL_DISP;
}

void dFile_select_c::MemCardFormatYesSelDisp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnSelMove = yesnoSelectMoveAnm();
    bool isYnWakuAlpha = yesnoWakuAlpahAnm(field_0x0269);
    if (isErrorTxtChange == true && isYnSelMove == true && isYnWakuAlpha == true) {
        yesnoCursorShow();
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_FORMAT_SEL2;
    }
}

void dFile_select_c::MemCardFormatNoSelDisp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    if (isErrorTxtChange == true && isYnMenuMove == true) {
        mWindowCloseMsgDispCb = NULL;
        mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
    }
}

void dFile_select_c::MemCardErrMsgWaitFormatSel() {
    if (errYesNoSelect()) {
        if (field_0x0268) {
            formatYesSelDispInitSet();
        } else {
            formatNoSelDispInitSet();
        }
    }
}

void dFile_select_c::formatYesSel2DispInitSet() {
    errorTxtSet(13);
    field_0x0280 = false;
    yesnoMenuMoveAnmInitSet(0x47d, 0x473);
    mCardCheckProc = MEMCARDCHECKPROC_FORMAT_YES_SEL2_DISP;
}

void dFile_select_c::MemCardErrMsgWaitFormatSel2() {
    if (errYesNoSelect()) {
        if (field_0x0268) {
            field_0x03b1 = 1;
            formatYesSel2DispInitSet();
        } else {
            formatNoSelDispInitSet();
        }
    }
}

void dFile_select_c::MemCardFormatYesSel2Disp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    if (isErrorTxtChange == true && isYnMenuMove == true) {
        mWaitTimer = g_fsHIO.card_wait_frames;
        mDoMemCd_Format();
        mCardCheckProc = MEMCARDCHECKPROC_FORMAT;
    }
}

void dFile_select_c::MemCardFormat() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    field_0x03b4 = mDoMemCd_FormatSync();
    if (field_0x03b4 != 0) {
        mCardCheckProc = MEMCARDCHECKPROC_FORMAT_WAIT;
    }
}

void dFile_select_c::MemCardFormatWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    } else {
        if (field_0x03b4 == 2) {
            errorTxtSet(11);
        } else if (field_0x03b4 == 1) {
            errorTxtSet(12);
        }
        mCardCheckProc = MEMCARDCHECKPROC_FORMAT_CHECK;
    }
}

void dFile_select_c::MemCardFormatCheck() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    if (isErrorTxtChange == true) {
        mWindowCloseMsgDispCb = NULL;
        mKeyWaitMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_CLOSE;
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        field_0x03b1 = 0;
    }
}

void dFile_select_c::MemCardMakeGameFileSel() {
    if (errYesNoSelect()) {
        if (field_0x0268 != 0) {
            #if PLATFORM_WII
            errorTxtSet(0xEEC);
            #else
            errorTxtSet(27);
            field_0x03b1 = 1;
            #endif 
        } else {
            #if PLATFORM_WII
            errorTxtSet(0xEEB);
            #else
            errorTxtSet(25);
            #endif 
        }
        ketteiTxtDispAnmInit(0);
        yesnoMenuMoveAnmInitSet(0x47d, 0x473);
        mCardCheckProc = MEMCARDCHECKPROC_MAKE_GAMEFILE_SEL_DISP;
    }
}

void dFile_select_c::MemCardMakeGameFileSelDisp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    bool isKetteiTxtDisp = ketteiTxtDispAnm();

    if (isErrorTxtChange == true && isYnMenuMove == true && isKetteiTxtDisp == true) {
        if (field_0x0268 != 0) {
            mWaitTimer = g_fsHIO.card_wait_frames;
            setInitSaveData();
            dataSave();
            mCardCheckProc = MEMCARDCHECKPROC_MAKE_GAMEFILE;
        } else {
            mWindowCloseMsgDispCb = NULL;
            mKeyWaitMsgDispCb = &dFile_select_c::noSaveSelDispInit;
            mKeyWaitCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
            mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        }
    }
}

void dFile_select_c::MemCardMakeGameFile() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    #if PLATFORM_GCN
    field_0x03b4 = mDoMemCd_SaveSync();
    #else
    field_0x03b4 = mDoMemCd_SaveSyncNAND();
    #endif

    if (field_0x03b4 != 0) {
        mCardCheckProc = MEMCARDCHECKPROC_MAKE_GAMEFILE_WAIT;
    }
}

void dFile_select_c::MemCardMakeGameFileWait() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    } else {
        if (field_0x03b4 == 1) {
            #if PLATFORM_WII
            errorTxtSet(0xEEE);
            #else
            errorTxtSet(0x1C);
            #endif 
        } else if (field_0x03b4 == 2) {
            #if PLATFORM_WII
            errorTxtSet(0xEED);
            #else
            errorTxtSet(0x1A);
            #endif 
        }
        mCardCheckProc = MEMCARDCHECKPROC_MAKE_GAMEFILE_CHECK;
    }
}

void dFile_select_c::MemCardMakeGameFileCheck() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    if (isErrorTxtChange == true) {
        mWindowCloseMsgDispCb = NULL;
        mKeyWaitMsgDispCb = NULL;

        #if PLATFORM_GCN
        mNextCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
        #else
        mNextCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
        #endif

        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_CLOSE;
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
        field_0x03b1 = 0;
    }
}

#if PLATFORM_WII || PLATFORM_SHIELD
void dFile_select_c::gameFileInitSel() {
    if (errYesNoSelect() != 0) {
        if (field_0x0268 != 0) {
            errorTxtSet(0xEE0);
            yesnoMenuMoveAnmInitSet(0x47D, 0x473);
            mCardCheckProc = MEMCARDCHECKPROC_GAMEFILE_INIT_SEL_DISP;
        } else {
            yesnoWakuAlpahAnmInit(field_0x0268, 0xFF, 0, g_fsHIO.select_box_appear_frames);
            noSaveSelDispInit();
            mCardCheckProc = MEMCARDCHECKPROC_NO_SAVE_SEL_DISP;
        }
    }
}

void dFile_select_c::gameFileInitSelDisp() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    bool isYnMenuMove = yesnoMenuMoveAnm();
    
    if (isErrorTxtChange == true && isYnMenuMove == true) {
        mWaitTimer = g_fsHIO.card_wait_frames;
        setInitSaveData();
        dataSave();
        mCardCheckProc = MEMCARDCHECKPROC_GAMEFILE_INIT;
    }
}

void dFile_select_c::gameFileInit() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    field_0x03b4 = mDoMemCd_SaveSyncNAND();
    if (field_0x03b4 != 0 && mWaitTimer == 0) {
        if (field_0x03b4 == 1) {
            errorTxtSet(0xEE1);
        } else if (field_0x03b4 == 2) {
            errorTxtSet(0xEE2);
        }
        mCardCheckProc = MEMCARDCHECKPROC_GAMEFILE_INIT_CHECK;
    }
}

void dFile_select_c::gameFileInitCheck() {
    bool isErrorTxtChange = errorTxtChangeAnm();
    if (isErrorTxtChange == true) {
        mWindowCloseMsgDispCb = NULL;
        mKeyWaitMsgDispCb = NULL;
        mNextCardCheckProc = MEMCARDCHECKPROC_NAND_STAT_CHECK;
        mKeyWaitCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_CLOSE;
        mCardCheckProc = MEMCARDCHECKPROC_ERRMSG_WAIT_KEY;
    }
}
#endif

void dFile_select_c::MemCardMsgWindowInitOpen() {
    bool iVar1;
    if (field_0x021e == 0) {
        iVar1 = headerTxtChangeAnm();
    } else {
        iVar1 = true;
    }

    bool iVar8 = true;
    bool iVar7 = true;
    bool iVar6 = true;
    bool iVar5 = true;

    #if PLATFORM_GCN
    if (field_0x00b8 || field_0x00b9)
    #else
    if (field_0x00b8)
    #endif
    {
        iVar8 = selectDataBaseMoveAnm();
    }

    if (field_0x0108 || field_0x0281) {
        iVar7 = yesnoMenuMoveAnm();
    }

    if (field_0x0360 || field_0x0283) {
        iVar6 = menuMoveAnm();
    }

    #if PLATFORM_GCN
    if (field_0x014a || field_0x014b)
    #else
    if (field_0x014a)
    #endif
    {
        iVar5 = errorMoveAnm();
    }

    bool iVar2 = modoruTxtDispAnm();
    bool iVar3 = ketteiTxtDispAnm();
    bool iVar4 = true;
    if (field_0x0128 || mCpSel.isShow) {
        iVar4 = nameMoveAnm();
    }

    if (iVar1 == true && iVar8 == true && iVar7 == true && iVar6 == true && iVar5 == true &&
        iVar2 == true && iVar3 == true && iVar4 == true)
    {
        fileSel.Scr->clearAnmTransform();
        setWakuAnm();
        errorMoveAnmInitSet(0xb21, 0xb2b);
        if (field_0x0280) {
            yesnoMenuMoveAnmInitSet(0x473, 0x47d);
            ketteiTxtDispAnmInit(1);
        } else {
            ketteiTxtDispAnmInit(0);
        }
        field_0x021e = 1;
        mCardCheckProc = MEMCARDCHECKPROC_MSG_WINDOW_OPEN;
    }
}

void dFile_select_c::MemCardMsgWindowOpen() {
    bool iVar1 = errorMoveAnm();
    bool iVar3 = true;
    if (field_0x0280) {
        iVar3 = yesnoMenuMoveAnm();
    }
    bool iVar2 = ketteiTxtDispAnm();
    if (iVar1 == true && iVar3 == true && iVar2 == true) {
        if (field_0x0280) {
            yesnoCursorShow();
        }
        mCardCheckProc = mNextCardCheckProc;
    }
}

void dFile_select_c::MemCardMsgWindowClose() {
    bool errorRes = errorMoveAnm();
    bool menuMoveRes = true;
    bool textAnmRes = true;
    if (field_0x0108) {
        menuMoveRes = yesnoMenuMoveAnm();
        textAnmRes = ketteiTxtDispAnm();
    }
    if (errorRes == true && menuMoveRes == true && textAnmRes == true) {
        field_0x0280 = false;
        if (mWindowCloseMsgDispCb != NULL) {
            (this->*mWindowCloseMsgDispCb)();
        } else {
            mCardCheckProc = mNextCardCheckProc;
        }
    }
}

bool dFile_select_c::errYesNoSelect() {
    bool rv = false;
    stick->checkTrigger();

    if (mDoCPd_c::getTrigA(PAD_1)) {
        if (field_0x0268 != 0) {
            mDoAud_seStart(Z2SE_SY_CURSOR_OK, 0, 0, 0);
        } else {
            mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, 0, 0, 0);
        }
        mSelIcon->setAlphaRate(0.0f);
        rv = true;
    } else if (stick->checkRightTrigger()) {
        if (field_0x0268) {
            field_0x0269 = field_0x0268;
            field_0x0268 = field_0x0268 ^ 1;
            errCurMove(0);
        }
    } else if (stick->checkLeftTrigger()) {
        if (field_0x0268 != 1) {
            field_0x0269 = field_0x0268;
            field_0x0268 = field_0x0268 ^ 1;
            errCurMove(0);
        }
    }
    return rv;
}

void dFile_select_c::errCurMove(u8 param_1) {
    mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, 0, param_1, 0);
    yesnoSelectAnmSet();
    field_0x0272 = mCardCheckProc;
    mCardCheckProc = MEMCARDCHECKPROC_ERR_YESNO_CURSOR_MOVE_ANM;
}

void dFile_select_c::MemCardErrYesNoCursorMoveAnm() {
    bool moveRes = yesnoSelectMoveAnm();
    bool anmRes = yesnoWakuAlpahAnm(field_0x0269);
    if (moveRes == true && anmRes == true) {
        yesnoCursorShow();
        mCardCheckProc = field_0x0272;
    }
}

void dFile_select_c::errorTxtSet(u16 i_msgId) {
    if (i_msgId == 0xffff) {
        strcpy(mErrorMsgStringPtr[mErrorTxtDispIdx ^ 1], "");
    } else {
        fileSel.mMessageString->getString(
            i_msgId, (J2DTextBox*)mErrorMsgTxtPane[mErrorTxtDispIdx ^ 1]->getPanePtr(), NULL,
            fileSel.font[0], NULL, 0);
    }

    mErrorMsgTxtPane[mErrorTxtDispIdx]->alphaAnimeStart(0);
    mErrorMsgTxtPane[mErrorTxtDispIdx ^ 1]->alphaAnimeStart(0);
    field_0x0149 = 0;
}

bool dFile_select_c::errorTxtChangeAnm() {
    if (field_0x0149) {
        return true;
    }

    bool rv = false;
    bool animeRes1 = mErrorMsgTxtPane[mErrorTxtDispIdx]->alphaAnime(g_fsHIO.char_switch_frames, 0xff, 0, 0);
    bool animeRes2 = mErrorMsgTxtPane[mErrorTxtDispIdx ^ 1]->alphaAnime(g_fsHIO.char_switch_frames, 0, 0xff, 0);
    s32 timer = dMeter2Info_getMsgKeyWaitTimer();
    if (animeRes1 == true && animeRes2 == true && timer == 0) {
        mErrorTxtDispIdx ^= 1;
        field_0x0149 = 1;
        rv = true;
    }

    return rv;
}

bool dFile_select_c::fileRecScaleAnm() {
    bool var_r30 = mSelFilePanes[mSelectNum]->scaleAnime(g_fsHIO.select_icon_appear_frames, field_0x00c8[0], field_0x00d4[0], 0);
    return var_r30;
}

void dFile_select_c::fileRecScaleAnmInitSet2(f32 param_1, f32 param_2) {
    for (int i = 0; (int)i < 3; i++) {
        field_0x00c8[i] = param_1;
        field_0x00d4[i] = param_2;
        if (i == mSelectNum) {
            if (param_1 > 0.0f) {
                field_0x00c8[i] = g_fsHIO.test_frame_counts[mSelectNum];
            }
            if (param_2 > 0.0f) {
                field_0x00d4[i] = g_fsHIO.test_frame_counts[mSelectNum];
            }
        }
        mSelFilePanes[i]->scaleAnimeStart(0);
    }
}

bool dFile_select_c::fileRecScaleAnm2() {
    bool scaleRes[3];
    for (int i = 0; i < 3; i++) {
        scaleRes[i] =
            mSelFilePanes[i]->scaleAnime(g_fsHIO.select_icon_appear_frames, field_0x00c8[i], field_0x00d4[i], 0);
    }

    if (scaleRes[0] == true && scaleRes[1] == true && scaleRes[2] == true) {
        return true;
    }

    return false;
}

bool dFile_select_c::fileInfoScaleAnm() {
    bool ret;

    if (field_0x0110 != field_0x0114) {
        if (field_0x0110 < field_0x0114) {
            field_0x0110 += 2;

            if (field_0x0110 > field_0x0114)
                field_0x0110 = field_0x0114;
        } else {
            field_0x0110 -= 2;

            if (field_0x0110 < field_0x0114)
                field_0x0110 = field_0x0114;
        }

        field_0x009c->setFrame(field_0x0110);
        mBaseSubPane->animationTransform();
        ret = false;
    }

    if (field_0x0110 == field_0x0114) {
        mBaseSubPane->setAnimation((J2DAnmTransform*)0);
        ret = true;
    }

    return ret;
}

void dFile_select_c::nameMoveAnmInitSet(int param_1, int param_2) {
    if (param_1 == 3359) {
        field_0x0128 = true;
    }
    if (param_1 == 3369) {
        mpName->hideIcon();
    }
    mNameBasePane->setAnimation(field_0x0094);
    field_0x0120 = param_1;
    field_0x0124 = param_2;
    field_0x0094->setFrame(field_0x0120);
    mNameBasePane->animationTransform();
}

bool dFile_select_c::nameMoveAnm() {
    bool ret;
    if (field_0x0120 != field_0x0124) {
        if (field_0x0120 < field_0x0124) {
            field_0x0120 += 2;

            if (field_0x0120 > field_0x0124) {
                field_0x0120 = field_0x0124;
            }
        } else {
            field_0x0120 -= 2;

            if (field_0x0120 < field_0x0124) {
                field_0x0120 = field_0x0124;
            }
        }
        field_0x0094->setFrame(field_0x0120);
        mNameBasePane->animationTransform();
        ret = false;
    } else {
        mNameBasePane->setAnimation((J2DAnmTransform*)0);

        if (field_0x0124 == 0xd1f) {
            field_0x0128 = false;
            mCpSel.isShow = false;
        }
        ret = true;
    }

    return ret;
}

#if PLATFORM_WII || PLATFORM_SHIELD
void dFile_select_c::saveDataClearInit() {
    setInitSaveData();
    dataSave();
}
#endif

void dFile_select_c::MemCardSaveDataClear() {
    #if PLATFORM_GCN
    field_0x03b4 = mDoMemCd_SaveSync();
    #else
    field_0x03b4 = mDoMemCd_SaveSyncNAND();
    #endif

    if (field_0x03b4 != 0) {
        #if PLATFORM_GCN
        mCardCheckProc = MEMCARDCHECKPROC_STAT_CHECK;
        #else
        mDoMemCd_LoadNAND();
        mCardCheckProc = MEMCARDCHECKPROC_LOAD_WAIT;
        #endif
    }
}

void dFile_select_c::setInitSaveData() {
    for (int i = 0; i < 3; i++) {
        dComIfGs_setInitDataToCard((u8*)mSaveData, i);
        mDoMemCdRWm_SetCheckSumGameData((u8*)mSaveData, i);
    }
}

void dFile_select_c::dataSave() {
    #if PLATFORM_GCN
    mDoMemCd_save(mSaveData, sizeof(mSaveData), 0);
    #else
    mDoMemCd_saveNAND(mSaveData, sizeof(mSaveData), 0);
    #endif
}

dFile_select3D_c::dFile_select3D_c() {
    mpSolidHeap = NULL;
    mpModel = NULL;
    field_0x03b8.y = 0.0f;
    field_0x03b8.x = 0.0f;
    field_0x03b8.z = 1.0f;
}

dFile_select3D_c::~dFile_select3D_c() {
    freeHeap();
}

void dFile_select3D_c::_create(u8 i_mirrorIdx, u8 i_maskIdx) {
    JKRHeap* ppHeap;

    mpSolidHeap = mDoExt_createSolidHeapFromGameToCurrent(&ppHeap, 0x25800, 32);
    JUT_ASSERT(8680, mpSolidHeap != NULL);

    field_0x03c4 = 0.0f;
    field_0x03c8 = 0.0f;
    mMirrorIdx = i_mirrorIdx;
    mMaskIdx = i_maskIdx;

    #if DEBUG
    if (g_fsHIO.mask_mirror_test_display) {
        mMirrorIdx = g_fsHIO.test_mirror_display;
        mMaskIdx = g_fsHIO.test_mask_display;
    }
    #endif

    if (mMirrorIdx != 0) {
        createMirrorModel();
    }

    if (mMaskIdx != 0) {
        createMaskModel();
    }

    mpSolidHeap->adjustSize();
    mDoExt_setCurrentHeap(ppHeap);

    if (mpModel) {
        dKy_tevstr_init(&mTevstr, 0xFF, 0xFF);
        set_mtx();
    }
}

void dFile_select3D_c::_delete() {}

void dFile_select3D_c::freeHeap() {
    if (mpSolidHeap) {
        mDoExt_destroySolidHeap(mpSolidHeap);
        mpSolidHeap = NULL;
        mpModel = NULL;
    }
}

void dFile_select3D_c::_move() {
    if (!mpModel) {
        return;
    }

    cXyz stack_20;
    Vec stack_8 = mPaneMgr->getGlobalVtxCenter(false, 0);
    toItem3Dpos(oREG_F(0) + (stack_8.x + field_0x03b8.x), oREG_F(1) + (stack_8.y + field_0x03b8.y), oREG_F(2) + 720.0f, &stack_20);
    field_0x03a4.set(stack_20);
    field_0x03b0.set(0, 0, 0);
    animePlay();
    set_mtx();
}

void dFile_select3D_c::draw() {
    if (mpModel) {
        dComIfGd_setListItem3D();
        g_env_light.settingTevStruct(13, &field_0x03a4, &mTevstr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &mTevstr);
        animeEntry();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    }
}

void dFile_select3D_c::setJ3D(char const* param_0, char const* param_1, char const* param_2) {
    JKRArchive* archive;
    J3DAnmBase* anmBase;
    void* bmdRes;
    J3DMaterialAnm* material;
    void* bckRes;
    void* brkRes;
    J3DModelData* modelData;

    archive = dComIfGp_getCollectResArchive();

    bmdRes = archive->getResource('BMD ', param_0);
    modelData = J3DModelLoaderDataBase::load(bmdRes, 0x51020010);
    JUT_ASSERT(8823, modelData != NULL);

    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        material = new J3DMaterialAnm();
        modelData->getMaterialNodePointer(i)->change();
        modelData->getMaterialNodePointer(i)->setMaterialAnm(material);
    }

    mpModel = new J3DModel(modelData, 0, 1);
    JUT_ASSERT(8836, mpModel != NULL);

    if (param_1) {
        bckRes = archive->getResource('BCK ', param_1);
        anmBase = (J3DAnmTransform*)J3DAnmLoaderDataBase::load(bckRes);
        JUT_ASSERT(8846, anmBase != NULL);

        mBckAnm = new mDoExt_bckAnm();
        if (mBckAnm == NULL || !mBckAnm->init((J3DAnmTransform*)anmBase, 1, 2, 1.0f, 0, -1, false))
        {
            return;
        }
    }

    if (param_2) {
        brkRes = archive->getResource('BRK ', param_2);
        anmBase = (J3DAnmTevRegKey*)J3DAnmLoaderDataBase::load(brkRes);
        JUT_ASSERT(8859, anmBase != NULL);
        ((J3DAnmTevRegKey*)anmBase)->searchUpdateMaterialID(modelData);

        mBrkAnm = new mDoExt_brkAnm();
        if (mBrkAnm == NULL ||
            !mBrkAnm->init(modelData, (J3DAnmTevRegKey*)anmBase, -1, 2, 1.0f, 0, -1))
        {
            return;
        }
    }
}

void dFile_select3D_c::set_mtx() {
    cXyz stack_8;
    f32 tmp = mPane->getScaleX();
    if (tmp <= 0.1f) {
        tmp = 0.0f;
    }

    stack_8.x = stack_8.y = stack_8.z = tmp * (field_0x03b8.z + oREG_F(3));
    mDoMtx_stack_c::transS(field_0x03a4.x, field_0x03a4.y, field_0x03a4.z);
    mDoMtx_stack_c::XYZrotM(field_0x03b0.x, field_0x03b0.y, field_0x03b0.z);
    mpModel->setBaseScale(stack_8);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void dFile_select3D_c::animePlay() {
    if (mBrkAnm) {
        field_0x03c4 += oREG_S(0) + 1;
        if (field_0x03c4 >= mBrkAnm->getEndFrame()) {
            field_0x03c4 -= mBrkAnm->getEndFrame();
        }
        mBrkAnm->setFrame(field_0x03c4);
        mBrkAnm->play();
    }
    if (mBckAnm) {
        field_0x03c8 += oREG_S(1) + 1;
        if (field_0x03c8 >= mBckAnm->getEndFrame()) {
            field_0x03c8 -= mBckAnm->getEndFrame();
        }
        mBckAnm->setFrame(field_0x03c8);
        mBckAnm->play();
    }
}

void dFile_select3D_c::animeEntry() {
    if (mBrkAnm) {
        mBrkAnm->entry(mpModel->getModelData());
    }

    if (mBckAnm) {
        mBckAnm->entry(mpModel->getModelData());
    }
}

void dFile_select3D_c::createMaskModel() {
    const static f32 m_kamen_offset_x[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const static f32 m_kamen_offset_y[5] = {0.0f, 0.0f, 0.0f, 5.0f, 5.0f};
    const static f32 m_kamen_scale[5] = {2.0f, 2.0f, 2.0f, 1.6f, 1.6f};
    static const char* bck_name[4] = {
        "md_mask_parts_spin_1.bck",
        "md_mask_parts_spin_2.bck",
        "md_mask_parts_spin_3.bck",
        "md_mask_parts_spin_4.bck",
    };
    static const char* brk_name[4] = {
        "md_mask_parts_spin_1.brk",
        "md_mask_parts_spin_2_3.brk",
        "md_mask_parts_spin_2_3.brk",
        "md_mask_parts_spin_4.brk",
    };

    field_0x03b8.x = m_kamen_offset_x[mMaskIdx];
    field_0x03b8.y = m_kamen_offset_y[mMaskIdx];
    field_0x03b8.z = m_kamen_scale[mMaskIdx];
    field_0x03a4.set(0.0f, 0.0f, 0.0f);
    field_0x03b0.set(0, 0, 0);
    mpModel = NULL;
    mBckAnm = NULL;
    mBrkAnm = NULL;
    if (mMaskIdx == 0) {
        return;
    }
    setJ3D("md_mask_UI.bmd", bck_name[mMaskIdx - 1], brk_name[mMaskIdx - 1]);
    switch (mMaskIdx) {
    case 1:
        mpModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
    case 2:
        mpModel->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(3)->getShape()->hide();
    case 3:
        mpModel->getModelData()->getMaterialNodePointer(6)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(7)->getShape()->hide();
    }
}

void dFile_select3D_c::createMirrorModel() {
    const static f32 m_mirror_offset_x[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const static f32 m_mirror_offset_y[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const static f32 m_mirror_scale[5] = {0.6f, 0.6f, 0.6f, 0.6f, 0.6f};
    static const char* bck_name[4] = {
        "kageri_mirrer_spin_1.bck",
        "kageri_mirrer_spin_2.bck",
        "kageri_mirrer_spin_3.bck",
        "kageri_mirrer_spin_4.bck",
    };
    static const char* brk_name[4] = {
        "kageri_mirrer_spin_1.brk",
        "kageri_mirrer_spin_2_3_4.brk",
        "kageri_mirrer_spin_2_3_4.brk",
        "kageri_mirrer_spin_2_3_4.brk",
    };

    field_0x03b8.x = m_mirror_offset_x[mMirrorIdx];
    field_0x03b8.y = m_mirror_offset_y[mMirrorIdx];
    field_0x03b8.z = m_mirror_scale[mMirrorIdx];
    field_0x03a4.set(0.0f, 0.0f, 0.0f);
    field_0x03b0.set(0, 0, 0);
    mpModel = NULL;
    mBckAnm = NULL;
    mBrkAnm = NULL;
    if (mMirrorIdx == 0) {
        return;
    }
    setJ3D("kageri_mirrer_UI.bmd", bck_name[mMirrorIdx - 1], brk_name[mMirrorIdx - 1]);
    switch (mMirrorIdx) {
    case 1:
        mpModel->getModelData()->getMaterialNodePointer(4)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(5)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(6)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(7)->getShape()->hide();
    case 2:
        mpModel->getModelData()->getMaterialNodePointer(8)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(9)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(10)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(11)->getShape()->hide();
    case 3:
        mpModel->getModelData()->getMaterialNodePointer(12)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(13)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(14)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(15)->getShape()->hide();
    }
}

#pragma push
#pragma optimization_level 2
void dFile_select3D_c::toItem3Dpos(f32 param_0, f32 param_1, f32 param_2, cXyz* param_3) {
    Mtx adStack_98;
    Mtx auStack_c8;
    param_0 = (2.0f * ((param_0 - mDoGph_gInf_c::getMinXF()) / mDoGph_gInf_c::getWidthF()) - 1.0f);
    param_1 = (2.0f * ((param_1 - -100.0f) / 448.0f) - 1.0f);
    calcViewMtx(adStack_98);
    cMtx_inverse(adStack_98, auStack_c8);
    f32 tangent = std::tan(M_PI / 8.0f);
    f32 dVar12 = -param_2;
    cXyz cStack_d4((param_0 * param_2) * (mDoGph_gInf_c::getAspect() * tangent),
                   (tangent * (param_1 * dVar12)), dVar12);
    cMtx_multVec(auStack_c8, &cStack_d4, param_3);
}
#pragma pop

void dFile_select3D_c::calcViewMtx(Mtx param_0) {
    cXyz pos1(0.0f, 0.0f, -1000.0f);
    cXyz pos2(0.0f, 1.0f, 0.0f);
    cMtx_lookAt(param_0, &pos1, &cXyz::Zero, &pos2, 0);
}
