/**
 * d_file_select.cpp
 * dolzel2 - Quest Log Management (File Select Menu)
 */

#include "d/dolzel.h" // IWYU pragma: keep

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
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_MemCard.h"
#include "m_Do/m_Do_MemCardRWmng.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

static s32 SelStartFrameTbl[3] = {
    0x0000003B, 
    0x00000063, 
    0x0000008B,
};

static s32 SelEndFrameTbl[3] = {
    0x00000045,
    0x0000006D,
    0x00000095,
};

static s32 SelOpenStartFrameTbl[3] = {
    0x000000F8,
    0x000001C0, 
    0x00000288,
};

static s32 SelOpenEndFrameTbl[3] = {
    0x00000103,
    0x000001CC, 
    0x00000293,
};

static s32 MenuSelStartFrameTbl[3] = {
    0x000007CD, 
    0x00000382, 
    0x00000390,
};

static s32 MenuSelEndFrameTbl[3] = {
    0x000007DB,
    0x0000038E,
    0x00000382,
};

static u64 l_tagName13[3] = {'w_dat_i0', 'w_dat_i1', 'w_dat_i2'};

dFs_HIO_c::dFs_HIO_c() {
    field_0x0006 = 5;
    field_0x0007 = 5;
    field_0x000a = 60;
    field_0x0008 = 5;
    field_0x0009 = 15;
    field_0x0005 = 5;
    field_0x000b = 15;
    field_0x000d = 15;
    field_0x000c = 90;
    field_0x0010[0] = 1.11f;
    field_0x0010[1] = 1.11f;
    field_0x0010[2] = 1.11f;
    field_0x001c = 0;
    field_0x001d = 0;
    field_0x001e = 0;
    field_0x001f = 0;
    field_0x0020 = 0;
    field_0x0021 = 0;
    field_0x0022 = 0;
}

dFile_select_c::dFile_select_c(JKRArchive* i_archiveP) {
    mpArchive = i_archiveP;
    mpFileSelect3d = new dFile_select3D_c();
}

dFile_select_c::~dFile_select_c() {
    for (int i = 0; i < 3; i = i + 1) {
        delete field_0x0164[i];
        delete field_0x0170[i];
        delete field_0x017c[i];
        delete mpPaneAlpha1[i];
        delete mpPaneAlpha2[i];
        delete mpPaneAlpha3[i];
        delete mpPaneMgr2[i];
        delete field_0x0188[i];
        delete field_0x014c[i];
        delete field_0x0158[i];
        delete field_0x00bc[i];
        delete field_0x0340[i];
        delete field_0x0070[i];
    }
    delete field_0x00a4;
    for (int i = 0; i < 2; i = i + 1) {
        delete field_0x00f0[i];
        delete field_0x01a0[i];
        delete field_0x01a8[i];
        delete field_0x01b0[i];
        delete field_0x020c[i];
        delete field_0x0138[i];
        delete field_0x01c0[i];
        delete mpFileInfo[i];
        delete field_0x02a4[i];
        delete field_0x02c0[i];
        delete field_0x02c8[i];
        delete field_0x02d0[i];
        delete field_0x02d8[i];
    }
    delete field_0x0238;
    delete field_0x0240;
    delete field_0x023c;
    delete field_0x0244;
    delete field_0x0084;
    delete field_0x0088;
    delete field_0x008c;
    delete field_0x01c8;
    delete field_0x01d0;
    delete field_0x01d8;
    delete field_0x01e0;
    delete field_0x01e8;
    delete field_0x01f0;
    delete field_0x01f8;
    delete field_0x0200;
    delete field_0x0090;
    delete field_0x0094;
    delete field_0x0098;
    delete field_0x009c;
    mDoExt_removeMesgFont();
    mDoExt_removeSubFont();
    delete mStick;
    delete mSelIcon;
    delete mSelIcon2;
    delete mpName;
    delete mpFileWarning;
    delete field_0x029c;
    delete field_0x02a0;
    delete field_0x02e8;
    delete field_0x02f0;
    delete field_0x02f8;
    delete field_0x0300;
    delete field_0x0308;
    delete field_0x0310;
    delete field_0x0314;
    delete field_0x0318;
    delete field_0x0320;
    delete field_0x0328;
    delete field_0x032c;
    delete field_0x0330;
    delete field_0x0338;
    delete field_0x03a0;
    delete field_0x03a8;
    mpFileSelect3d->_delete();
    delete mpFileSelect3d;
    delete field_0x2378;

    dComIfGp_getMain2DArchive()->removeResourceAll();
    dComIfGp_getCollectResArchive()->removeResourceAll();
}

static dFs_HIO_c g_fsHIO;

void dFile_select_c::_create() {
    mDoGph_gInf_c::setFadeColor(static_cast<JUtility::TColor&>(g_blackColor));
    mStick = new STControl(2, 2, 1, 1, 0.9f, 0.5f, 0, 0x2000);
    g_fsHIO.field_0x0004 = -1;

    for (int i = 0; i < 3; i++) {
        mDataNew[i] = 0;
        field_0x025b[i] = 0;
    }

    mSelectNum = 0;
    
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
static DataSelProcFunc DataSelProc[47] = {
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
};

void dFile_select_c::_move() {
    dMeter2Info_decMsgKeyWaitTimer();

    s32 drive_status = DVDGetDriveStatus();

    if (drive_status != DVD_STATE_END && drive_status != DVD_STATE_BUSY && field_0x03b1 != 0) {
        field_0x03b1 = 0;
    }

    if ((mDoMemCd_getProbeStat() == 0 || mDoMemCd_getProbeStat() == 1) && (mDoRst::isReset() == 0 && field_0x03b1 == 0)) {
        if (field_0x2378->getAlpha() != 0) {
            field_0x2378->setAlpha(0);
        }
        
        field_0x026f = 0;
        field_0x0271 = 0;
        
    }

    (this->*DataSelProc[field_0x026f])();

    selFileWakuAnm();
    bookIconAnm();
    dataDelEffAnm();
    dataCopyEffAnm();

    fileSel.Scr->animation();
    mYnSel.ScrYn->animation();
    m3mSel.Scr3m->animation();
    mSelDt.ScrDt->animation();

    if (mCpSel.field_0x08) {
        selCopyFileWakuAnm();
        copyBookIconAnm();
        mCpSel.Scr->animation();
    }

    mpFileWarning->_move();
    mpFileSelect3d->_move();

    mDoMemCd_clearProbeStat();
}

void dFile_select_c::selFileWakuAnm() {
    field_0x01cc += 2;

    if (field_0x01cc >= field_0x01c8->getFrameMax())
        field_0x01cc -= field_0x01c8->getFrameMax();

    field_0x01c8->setFrame(field_0x01cc);
    field_0x01d4 += 2;

    if (field_0x01d4 >= field_0x01d0->getFrameMax())
        field_0x01d4 -= field_0x01d0->getFrameMax();

    field_0x01d0->setFrame(field_0x01d4);
    field_0x031c += 2;

    if (field_0x031c >= field_0x0318->getFrameMax())
        field_0x031c -= field_0x0318->getFrameMax();

    field_0x0318->setFrame(field_0x031c);
    field_0x0324 += 2;

    if (field_0x0324 >= field_0x0320->getFrameMax())
        field_0x0324 -= field_0x0320->getFrameMax();

    field_0x0320->setFrame(field_0x0324);
    field_0x0334 += 2;

    if (field_0x0334 >= field_0x0330->getFrameMax())
        field_0x0334 -= field_0x0330->getFrameMax();

    field_0x0330->setFrame(field_0x0334);
    field_0x033c += 2;

    if (field_0x033c >= field_0x0338->getFrameMax())
        field_0x033c -= field_0x0338->getFrameMax();

    field_0x0338->setFrame(field_0x033c);
    field_0x03a4 += 2;

    if (field_0x03a4 >= field_0x03a0->getFrameMax())
        field_0x03a4 -= field_0x03a0->getFrameMax();

    field_0x03a0->setFrame(field_0x03a4);
}

void dFile_select_c::bookIconAnm() {
    field_0x01dc += 2;

    if (field_0x01dc >= field_0x01d8->getFrameMax())
        field_0x01dc -= field_0x01d8->getFrameMax();

    field_0x01d8->setFrame(field_0x01dc);
    field_0x01e4 += 2;

    if (field_0x01e4 >= field_0x01e0->getFrameMax())
        field_0x01e4 -= field_0x01e0->getFrameMax();

    field_0x01e0->setFrame(field_0x01e4);
    field_0x01ec += 2;

    if (field_0x01ec >= field_0x01e8->getFrameMax())
        field_0x01ec -= field_0x01e8->getFrameMax();

    field_0x01e8->setFrame(field_0x01ec);
}

void dFile_select_c::selCopyFileWakuAnm() {
    field_0x02ec += 2;

    if (field_0x02ec >= field_0x02e8->getFrameMax())
        field_0x02ec -= field_0x02e8->getFrameMax();

    field_0x02e8->setFrame(field_0x02ec);
    field_0x02f4 += 2;

    if (field_0x02f4 >= field_0x02f0->getFrameMax())
        field_0x02f4 -= field_0x02f0->getFrameMax();

    field_0x02f0->setFrame(field_0x02f4);
}

void dFile_select_c::copyBookIconAnm() {
    field_0x02fc += 2;

    if (field_0x02fc >= field_0x02f8->getFrameMax())
        field_0x02fc -= field_0x02f8->getFrameMax();

    field_0x02f8->setFrame(field_0x02fc);
    field_0x0304 += 2;

    if (field_0x0304 >= field_0x0300->getFrameMax())
        field_0x0304 -= field_0x0300->getFrameMax();

    field_0x0300->setFrame(field_0x0304);
    field_0x030c += 2;

    if (field_0x030c >= field_0x0308->getFrameMax())
        field_0x030c -= field_0x0308->getFrameMax();
    
    field_0x0308->setFrame(field_0x030c);
}

void dFile_select_c::dataDelEffAnm() {
    if (field_0x0208 != 0) {
        field_0x0204 += 2;

        if (field_0x0204 >= field_0x0200->getFrameMax())
            field_0x0204 -= field_0x0200->getFrameMax();
        
        field_0x0200->setFrame(field_0x0204);
        field_0x01f4 += 2;

        if (field_0x01f4 >= field_0x01f0->getFrameMax())
            field_0x01f4 -= field_0x01f0->getFrameMax();

        field_0x01f0->setFrame(field_0x01f4);
    }
}

void dFile_select_c::dataCopyEffAnm() {
    if (field_0x0209 != 0) {
        field_0x0204 += 2;

        if (field_0x0204 >= field_0x0200->getFrameMax())
            field_0x0204 -= field_0x0200->getFrameMax();

        field_0x0200->setFrame(field_0x0204);
        field_0x01fc += 2;

        if (field_0x01fc >= field_0x01f8->getFrameMax())
            field_0x01fc -= field_0x01f8->getFrameMax();

        field_0x01f8->setFrame(field_0x01fc);
    }
}

void dFile_select_c::selectDataBaseMoveAnmInitSet(int param_0, int param_1) {
    field_0x00a4->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0084);
    field_0x00b0 = param_0;
    field_0x00b4 = param_1;
    field_0x0084->setFrame(field_0x00b0);
    field_0x00a4->getPanePtr()->animationTransform();
    field_0x00b9 = 1;
}

bool dFile_select_c::selectDataBaseMoveAnm() {
    if (field_0x00b0 != field_0x00b4) {
        if (field_0x00b0 < field_0x00b4) {
            field_0x00b0 += 2;

            if (field_0x00b0 > field_0x00b4)
                field_0x00b0 = field_0x00b4;

        } else {
            field_0x00b0 -= 2;

            if (field_0x00b0 < field_0x00b4)
                field_0x00b0 = field_0x00b4;

        }

        field_0x0084->setFrame(field_0x00b0);
        field_0x00a4->getPanePtr()->animationTransform();
        return false;
    } else {
        field_0x00b0 == 0x21 ? field_0x00b8 = 1 : field_0x00b8 = 0;
        field_0x00b9 = 0;

        field_0x00a4->getPanePtr()->setAnimation((J2DAnmTransform*)0);
        return true;
    }
}

void dFile_select_c::dataSelectInAnmSet() {
    setSaveData();
    dSv_save_c* pSave = (dSv_save_c*)mSave;
    OSTime date_ipl = pSave->getPlayer().getPlayerStatusB().getDateIpl();
    mSelectNum = 0;
    for (int i = 0; i < 3; i++) {
        field_0x0164[i]->setAlpha(0);
        field_0x0170[i]->setAlpha(0);
        field_0x017c[i]->setAlpha(0);
        if (field_0x025b[i] == 0) {
            OSTime date_ipl2 = pSave->getPlayer().getPlayerStatusB().getDateIpl();
            OSTime date2Secs = OSTicksToSeconds(date_ipl2);
            OSTime dateSec = OSTicksToSeconds(date_ipl);

            if (dateSec < date2Secs) {
                date_ipl = date_ipl2;
                mSelectNum = i;
            }
        }

        pSave = (dSv_save_c*)((u8*)pSave + 0xa94);

        if (field_0x025b[i] != 0 || mDataNew[i] != 0) {
            field_0x0220[i]->setAlpha(0);
            field_0x022c[i]->setAlpha(0xFF);
        } else {
            field_0x0220[i]->setAlpha(0xFF);
            field_0x022c[i]->setAlpha(0);
        }

        field_0x0188[i]->setAlpha(0);
    }
    mpFileWarning->init();
    selectDataBaseMoveAnmInitSet(1,0x21);
    mDoAud_seStart(Z2SE_SY_FILE_MENU_SLIDE_IN,0,0,0);
}

void dFile_select_c::dataSelectIn() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool selectDataBaseMove = selectDataBaseMoveAnm();

    bool errorMove = true;
    if (field_0x014a || field_0x014b) {
        errorMove = errorMoveAnm();
    }

    bool yesnoMenuMove = true;
    if (field_0x0108 || field_0x0281) {
        yesnoMenuMove = yesnoMenuMoveAnm();
    }

    bool nameMove = true;
    if (field_0x0128 || mCpSel.field_0x08) {
        nameMove = nameMoveAnm();
    }

    bool menuMove = true;
    if (field_0x0360 || field_0x0283) {
        menuMove = menuMoveAnm();
    }

    bool modoruTxtDisp = modoruTxtDispAnm();

    if (headerTxtChange == true && selectDataBaseMove == true && 
                                   errorMove == true && yesnoMenuMove == true && 
                                   nameMove == true && menuMove == true && 
                                   modoruTxtDisp == true) {
        for (int i = 0; i < 3; i++) {
            field_0x00bc[i]->reinit();
        }

        headerTxtSet(0x43,1,0);
        mLastSelectNum = mSelectNum;
        field_0x00bc[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0084);
        field_0x00e0[mSelectNum] = SelStartFrameTbl[mSelectNum];
        field_0x0188[mSelectNum]->alphaAnimeStart(0);
        ketteiTxtDispAnmInit(1);
        field_0x026f = 2;
    }
}

void dFile_select_c::dataSelectInit() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool alphaAnime = true;
    bool ketteiTxtDisp = true;
    bool check = true;

    if (mSelectNum != 0xFF) {
        alphaAnime = field_0x0188[mSelectNum]->alphaAnime(g_fsHIO.field_0x0006,0,0xFF,1);
        ketteiTxtDisp = ketteiTxtDispAnm();

        if (field_0x00e0[mSelectNum] != SelEndFrameTbl[mSelectNum]) {
            field_0x00e0[mSelectNum] += 2;

            if (field_0x00e0[mSelectNum] > SelEndFrameTbl[mSelectNum])
                field_0x00e0[mSelectNum] = SelEndFrameTbl[mSelectNum];
             
            field_0x0084->setFrame(field_0x00e0[mSelectNum]);
            field_0x00bc[mSelectNum]->getPanePtr()->animationTransform();
            check = false;
        }
    }

    if (headerTxtChange == true && alphaAnime == true && ketteiTxtDisp == true && check == true) {
        if (mSelectNum != 0xFF) {
            field_0x00bc[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
            selFileCursorShow();    
        }

        field_0x026f = 3;
    }
}

// handles switching between quest logs
void dFile_select_c::dataSelect() {
    mStick->checkTrigger();

    // If A or Start was pressed
    if (mDoCPd_c::getTrigA(PAD_1) || mDoCPd_c::getTrigStart(PAD_1)) {
        dataSelectStart(); // run the quest log open process
    } else {
        if (mStick->checkUpTrigger()) {
            // if we're not on the top quest log
            if (mSelectNum != 0) {
                mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR,0,0,0);
                mLastSelectNum = mSelectNum;
                mSelectNum--;
                dataSelectAnmSet(); // run the quest log selection animation
                field_0x026f = 4;
            }
        } else if (mStick->checkDownTrigger()) {
            // if we're not on the bottom quest log
            if (mSelectNum != 2) {
                mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR,0,0,0);
                mLastSelectNum = mSelectNum;
                mSelectNum++;
                dataSelectAnmSet(); // run the quest log selection animation
                field_0x026f = 4;
            }
        } 
    }
}

static s32 YnSelStartFrameTbl[2] = {
    0x000004E3, 
    0x000004D4,
};

static s32 YnSelEndFrameTbl[2] = {
    0x000004D4, 
    0x000004E3,
};

static u16 msgTbl[3] = {
    0x0040,
    0x0041,
    0x0042,
};

void dFile_select_c::dataSelectStart() {
    mSelIcon->setAlphaRate(0.0f);
    if (field_0x025b[mSelectNum] != 0) {
        headerTxtSet(0x52, 0, 0);
        selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
        yesnoMenuMoveAnmInitSet(0x473, 0x47d);
        selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.field_0x0008);
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
        field_0x026f = 7;
    } else if (mDataNew[mSelectNum] != 0) {
        dComIfGs_setNewFile(0x80);
        dComIfGs_setDataNum(mSelectNum);
        mDoAud_seStart(Z2SE_SY_NEW_FILE, 0, 0, 0);
        headerTxtSet(0x385, 1, 0);
        fileRecScaleAnmInitSet2(1.0f, 0.0f);
        nameMoveAnmInitSet(3359, 3369);
        field_0x0164[mSelectNum]->setAlpha(0);
        field_0x0170[mSelectNum]->setAlpha(0);
        field_0x017c[mSelectNum]->setAlpha(0);
        char auStack_28[32];
        dMeter2Info_getString(0x382, auStack_28, 0);
        dComIfGs_setPlayerName(auStack_28);
        mpName->setNextNameStr(dComIfGs_getPlayerName());
        mpName->initial();
        modoruTxtChange(1);
        field_0x026f = 6;
    } else {
        dComIfGs_setNewFile(0);
        mDoAud_seStart(Z2SE_SY_CURSOR_OK, 0, 0, 0);
        mSelectMenuNum = 1;
        mLastSelectMenuNum = mSelectMenuNum;
        headerTxtSet(msgTbl[mSelectNum], 1, 0);
        selectDataMoveAnmInitSet(
            SelOpenStartFrameTbl[mSelectNum],
            SelOpenEndFrameTbl[mSelectNum]);
        menuMoveAnmInitSet(799, 809);
        selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.field_0x0008);
        makeRecInfo(mSelectNum);
        field_0x026f = 5;
    }
    modoruTxtDispAnmInit(1);
}

void dFile_select_c::selectDataMoveAnmInitSet(int param_0, int param_1) {
    field_0x00bc[0]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0084);
    field_0x00bc[1]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0084);
    field_0x00bc[2]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0084);
    mpPane->setAnimation((J2DAnmTransform*)field_0x0084);
    field_0x00e0[mSelectNum] = param_0;
    field_0x00ec = param_1;
    field_0x0084->setFrame(field_0x00e0[mSelectNum]);
    field_0x00bc[0]->getPanePtr()->animationTransform();
    field_0x00bc[1]->getPanePtr()->animationTransform();
    field_0x00bc[2]->getPanePtr()->animationTransform();
    mpPane->animationTransform();
}

bool dFile_select_c::selectDataMoveAnm() {
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

        field_0x0084->setFrame(field_0x00e0[mSelectNum]);

        for (int i = 0; i < 3; i++) {
            field_0x00bc[i]->getPanePtr()->animationTransform();
        }
        mpPane->animationTransform();
    }

    if (field_0x00e0[mSelectNum] == field_0x00ec) {
        for (int i = 0; i < 3; i++) {
            field_0x00bc[i]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
        }
        mpPane->setAnimation((J2DAnmTransform*)0);
        return true;
    } else {
        return false;
    }
}

void dFile_select_c::dataSelectAnmSet() {
    if (mSelectNum != 0xff) {
        field_0x00bc[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0084);
        field_0x00e0[mSelectNum] = SelStartFrameTbl[mSelectNum];
        field_0x0084->setFrame(field_0x00e0[mSelectNum]);
        field_0x00bc[mSelectNum]->getPanePtr()->animationTransform();
        field_0x0188[mSelectNum]->alphaAnimeStart(0);
    }

    if (mLastSelectNum != 0xff) {
        selectWakuAlpahAnmInit(mLastSelectNum,0xff,0,g_fsHIO.field_0x0008);
        field_0x00bc[mLastSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)field_0x0088); // wrong?
        field_0x00e0[mLastSelectNum] = SelEndFrameTbl[mLastSelectNum];
        field_0x0088->setFrame(field_0x00e0[mLastSelectNum]);
        field_0x00bc[mLastSelectNum]->getPanePtr()->animationTransform();
        field_0x0188[mLastSelectNum]->alphaAnimeStart(0);
        mSelIcon->setAlphaRate(0.0f);
    }
}

void dFile_select_c::dataSelectMoveAnime() {
    bool iVar7 = true;
    bool iVar6 = true;
    bool bVar1 = true;
    if (mLastSelectNum != 0xff) {
        iVar7 = field_0x0188[mLastSelectNum]->alphaAnime(g_fsHIO.field_0x0006,0xff,0,1);
        iVar6 = selectWakuAlpahAnm(mLastSelectNum);

        if (field_0x00e0[mLastSelectNum] != SelStartFrameTbl[mLastSelectNum]) {
            field_0x00e0[mLastSelectNum] = field_0x00e0[mLastSelectNum] - 2;
            
            if (field_0x00e0[mLastSelectNum] < SelStartFrameTbl[mLastSelectNum])
                field_0x00e0[mLastSelectNum] = SelStartFrameTbl[mLastSelectNum];
        
            field_0x0088->setFrame(field_0x00e0[mLastSelectNum]);
            field_0x00bc[mLastSelectNum]->getPanePtr()->animationTransform();
            bVar1 = false;
        }
    }

    bool iVar5 = true;
    bool bVar2 = true;

    if (mSelectNum != 0xff) {
        iVar5 = field_0x0188[mSelectNum]->alphaAnime(g_fsHIO.field_0x0006,0,0xff,1);

        if (field_0x00e0[mSelectNum] != SelEndFrameTbl[mSelectNum]) {
            field_0x00e0[mSelectNum] = field_0x00e0[mSelectNum] + 2;

            if (field_0x00e0[mSelectNum] > SelEndFrameTbl[mSelectNum])
                field_0x00e0[mSelectNum] = SelEndFrameTbl[mSelectNum];

            field_0x0084->setFrame(field_0x00e0[mSelectNum]);
            field_0x00bc[mSelectNum]->getPanePtr()->animationTransform();
            bVar2 = false;
        }
    }

    if (iVar7 == true && iVar6 == true && bVar1 == true && iVar5 == true && bVar2 == true) {
        if (mSelectNum != 0xff) {
            field_0x00bc[mSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
            selFileCursorShow();
        }

        if (mLastSelectNum != 0xff) {
            field_0x00bc[mLastSelectNum]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
        }

        field_0x026f = 3;
    }
}

void dFile_select_c::makeRecInfo(u8 param_1) {
    dSv_save_c* pSave = (dSv_save_c*)&mSave[param_1];
    J2DPane* ken0 = mSelDt.ScrDt->search('ken_00');
    J2DPane* ken1 = mSelDt.ScrDt->search('ken_01');
    ken0->hide();
    ken1->hide();
    if (pSave->getPlayer().getGetItem().isFirstBit(40)) {
        ken0->hide();
        ken1->show();
    } else if (pSave->getPlayer().getGetItem().isFirstBit(63) && !pSave->getEvent().isEventBit(0x302)) {
        ken0->show();
        ken1->hide();
    }

    J2DPane* ken2 = mSelDt.ScrDt->search('ken_02');
    if (pSave->getPlayer().getGetItem().isFirstBit(41) || pSave->getPlayer().getGetItem().isFirstBit(73)) {
        ken2->show();
    } else {
        ken2->hide();
    }

    J2DPane* tate0 = mSelDt.ScrDt->search('tate_00');
    J2DPane* tate1 = mSelDt.ScrDt->search('tate_01');
    tate0->hide();
    tate1->hide();

    if (pSave->getPlayer().getGetItem().isFirstBit(43)) {
        tate0->show();
        tate1->hide();
    } else if (pSave->getPlayer().getGetItem().isFirstBit(42)) {
        tate0->hide();
        tate1->show();
    }

    J2DPane* tate2 = mSelDt.ScrDt->search('tate_02');
    if (pSave->getPlayer().getGetItem().isFirstBit(44)) {
        tate2->show();
    } else {
        tate2->hide();
    }

    J2DPane* fuku0 = mSelDt.ScrDt->search('fuku_00');
    if (pSave->getPlayer().getGetItem().isFirstBit(47)) {
        fuku0->show();
    } else {
        fuku0->hide();
    }

    J2DPane* fuku1 = mSelDt.ScrDt->search('fuku_01');
    if (pSave->getPlayer().getGetItem().isFirstBit(49)) {
        fuku1->show();
    } else {
        fuku1->hide();
    }

    J2DPane* fuku2 = mSelDt.ScrDt->search('fuku_02');
    if (pSave->getPlayer().getGetItem().isFirstBit(48)) {
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

    if ((crystalCollected == 0 && mirrorsCollected == 0) || (pSave->getEvent().isEventBit(0xc01) && !pSave->getPlayer().getCollect().isCollectMirror(0)) ||
        (pSave->getEvent().isEventBit(0x2b08) && !pSave->getPlayer().getCollect().isCollectCrystal(3))) {
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
    bool headerTxtChange = headerTxtChangeAnm();
    bool selectDataMove = selectDataMoveAnm();
    bool menuMove = menuMoveAnm();
    bool modoruTxtDisp = modoruTxtDispAnm();
    bool selectWakuAlpah = selectWakuAlpahAnm(mSelectNum);

    if (headerTxtChange == true && selectDataMove == true && menuMove == true && 
        modoruTxtDisp == true && selectWakuAlpah == true) {
        mSelectMenuNum = 1;
        field_0x0340[mSelectMenuNum]->getPanePtr()->setAnimation(field_0x0328);
        field_0x0328->setFrame(MenuSelStartFrameTbl[mSelectMenuNum]);
        field_0x0340[mSelectMenuNum]->getPanePtr()->animationTransform();
        field_0x0340[mSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
        menuCursorShow();
        field_0x026f = 8;
    }
}

void dFile_select_c::selectDataNameMove() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool fileRecScale = fileRecScaleAnm2();
    bool nameMove = nameMoveAnm();
    bool modoruTxtDisp = modoruTxtDispAnm();

    if (headerTxtChange == true && fileRecScale == true && nameMove == true && modoruTxtDisp == true) {
        field_0x026f = 15;
    }
}

void dFile_select_c::selectDataOpenEraseMove() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool selectDataMove = selectDataMoveAnm();
    yesnoMenuMoveAnm();
    bool modoruTxtDisp = modoruTxtDispAnm();
    bool selectWakuAlpah = selectWakuAlpahAnm(mSelectNum);

    if (headerTxtChange == true && selectDataMove == true && modoruTxtDisp == true && selectWakuAlpah == true) {
        field_0x026e = 2;
        yesnoCursorShow();
        field_0x026f = 26;
    }
}

// Handles selecting between copy / start / delete menus in quest log
void dFile_select_c::menuSelect() {
    mStick->checkTrigger();

    // if a was pressed, do the menu selection process
    if (mDoCPd_c::getTrigA(PAD_1)) {
        menuSelectStart();
    } else {
        // if b was pressed, do the menu cancel process
        if (mDoCPd_c::getTrigB(PAD_1)) {
            menuSelectCansel();
        } else {
            if (mStick->checkRightTrigger()) {
                if (mDataNew[mSelectNum] == 0 && mSelectMenuNum != 0) {
                    mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON,0,0,0);
                    mLastSelectMenuNum = mSelectMenuNum;
                    mSelectMenuNum--;
                    menuSelectAnmSet(); // run the menu selection animation
                    field_0x026f = 9;
                }
            } else if (mStick->checkLeftTrigger()) {
                if (mDataNew[mSelectNum] == 0 && mSelectMenuNum != 2) {
                    mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON,0,0,0);
                    mLastSelectMenuNum = mSelectMenuNum;
                    mSelectMenuNum++;
                    menuSelectAnmSet(); // run the menu selection animation
                    field_0x026f = 9;
                }
            } 
        }
    }
}

// Handles copy / start / delete actions depending on which menu is selected from menuSelect
void dFile_select_c::menuSelectStart() {
    mDoAud_seStart(Z2SE_SY_CURSOR_OK,0,0,0);

    if (mSelectMenuNum == 1) {
        dComIfGs_setCardToMemory((char*)mSave,mSelectNum);
        mSelectEnd = 1;
        field_0x026f = 46;
        dComIfGs_setDataNum(mSelectNum);
    }
    else if (mSelectMenuNum == 0) {
        mSelIcon->setAlphaRate(0.0f);
        yesnoMenuMoveAnmInitSet(0x473,0x47d);
        headerTxtSet(0x49, 0,0);
        field_0x026e = 1;
        field_0x026f = 12;
    }
    else if (mSelectMenuNum == 2) {
        field_0x026a = mSelectNum;
        menuMoveAnmInitSet(0x329,799);
        headerTxtSet(0x44,0,0);
        mSelIcon->setAlphaRate(0.0f);
        selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectNum],SelOpenStartFrameTbl[mSelectNum]);
        selectWakuAlpahAnmInit(mSelectNum,0,255,g_fsHIO.field_0x0008);
        copySelMoveAnmInitSet(0xd1f,0xd29);
        field_0x026e = 0;
        field_0x026f = 11;
    }
}

void dFile_select_c::menuSelectCansel() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL,0,0,0);
    int idx = mSelectNum;
    selectDataMoveAnmInitSet(SelOpenEndFrameTbl[idx],SelOpenStartFrameTbl[idx]);
    menuMoveAnmInitSet(0x329,799);
    modoruTxtDispAnmInit(0);
    headerTxtSet(0x43,1,0);
    mSelIcon->setAlphaRate(0.0);
    field_0x026f = 0xd;
}

void dFile_select_c::menuMoveAnmInitSet(int param_0, int param_1) {
    if (param_0 == 799) {
        field_0x0283 = true;

        for (int i = 0; i < 3; i++) {
            if (i == mSelectMenuNum) {
                static_cast<J2DTextBox*>(mpPaneMgr2[i]->getPanePtr())->setWhite(JUtility::TColor(255,255,255,255));
            } else {
                static_cast<J2DTextBox*>(mpPaneMgr2[i]->getPanePtr())->setWhite(JUtility::TColor(150,150,150,255));
            }
        }
    }

    field_0x0118->setAnimation(field_0x0328);
    field_0x0358 = param_0;
    field_0x035c = param_1;
    field_0x0328->setFrame(field_0x0358);
    field_0x0118->animationTransform();
}

bool dFile_select_c::menuMoveAnm() {
    if (field_0x0358 != field_0x035c) {
        if (field_0x0358 < field_0x035c){
            field_0x0358 += 2;

            if (field_0x0358 > field_0x035c)
                field_0x0358 = field_0x035c;
        } else {
            field_0x0358 -= 2;

            if (field_0x0358 < field_0x035c)
                field_0x0358 = field_0x035c;
        }    

        field_0x0328->setFrame(field_0x0358);
        field_0x0118->animationTransform();
        return false;
    } else {
        field_0x0118->setAnimation((J2DAnmTransform*)0);
        if (field_0x035c == 0x0329) {
            field_0x0360 = true;
            field_0x0283 = false;
        } else {
            field_0x0360 = false;

            if (mSelectMenuNum != 0xFF) {
                mpPaneAlpha1[mSelectMenuNum]->setAlpha(0);
                mpPaneAlpha2[mSelectMenuNum]->setAlpha(0);
                mpPaneAlpha3[mSelectMenuNum]->setAlpha(0);
            }
        }

        return true;
    }
}

void dFile_select_c::menuSelectAnmSet() {
    if (mSelectMenuNum != 0xFF) {
        field_0x0340[mSelectMenuNum]->getPanePtr()->setAnimation(field_0x0328);
        field_0x034c[mSelectMenuNum] = MenuSelEndFrameTbl[mSelectMenuNum];
        field_0x0328->setFrame(field_0x034c[mSelectMenuNum]);
        field_0x0340[mSelectMenuNum]->getPanePtr()->animationTransform();
    }

    if (mLastSelectMenuNum != 0xFF) {
        field_0x0340[mLastSelectMenuNum]->getPanePtr()->setAnimation(field_0x032c);
        field_0x034c[mLastSelectMenuNum] = MenuSelStartFrameTbl[mLastSelectMenuNum];
        field_0x032c->setFrame(field_0x034c[mLastSelectMenuNum]);
        field_0x0340[mLastSelectMenuNum]->getPanePtr()->animationTransform();
        menuWakuAlpahAnmInit(mLastSelectMenuNum,0xFF,0,g_fsHIO.field_0x0008);
        mSelIcon->setAlphaRate(0.0f);
    }
}

void dFile_select_c::menuSelectMoveAnm() {
    bool tmp1 = true;
    if (mSelectMenuNum != 0xFF && field_0x034c[mSelectMenuNum] != MenuSelStartFrameTbl[mSelectMenuNum]) {
        if (field_0x034c[mSelectMenuNum] > MenuSelStartFrameTbl[mSelectMenuNum]) {
            field_0x034c[mSelectMenuNum] -= 2;
            
            if (field_0x034c[mSelectMenuNum] < MenuSelStartFrameTbl[mSelectMenuNum])
                field_0x034c[mSelectMenuNum] = MenuSelStartFrameTbl[mSelectMenuNum];
        } else {
            field_0x034c[mSelectMenuNum] += 2;

            if (field_0x034c[mSelectMenuNum] > MenuSelStartFrameTbl[mSelectMenuNum])
                field_0x034c[mSelectMenuNum] = MenuSelStartFrameTbl[mSelectMenuNum];
        }

        field_0x0328->setFrame(field_0x034c[mSelectMenuNum]);
        field_0x0340[mSelectMenuNum]->getPanePtr()->animationTransform();
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
      field_0x032c->setFrame(field_0x034c[mLastSelectMenuNum]);
      field_0x0340[mLastSelectMenuNum]->getPanePtr()->animationTransform();
      tmp2 = false;
    }

    tmp3 = menuWakuAlpahAnm(mLastSelectMenuNum);
  }

  if (tmp1 == true && tmp2 == true && tmp3 == true) {
    if (mSelectMenuNum != 0xFF) {
      field_0x0340[mSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
      menuCursorShow();
    }

    if (mLastSelectMenuNum != 0xFF) {
      field_0x0340[mLastSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
    }

    field_0x026f = 8;
  }
}

void dFile_select_c::ToNameMove() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool nameMove = nameMoveAnm();
    bool menuMove = menuMoveAnm();
    bool fileRecScale = fileRecScaleAnm();
    bool fileInfoScale = fileInfoScaleAnm();

    if (headerTxtChange == true && nameMove == true && menuMove == true && 
        (fileRecScale == true && fileInfoScale == true)) {
        field_0x026f = 15;
    }
}

void dFile_select_c::ToNameMove2() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool nameMove = nameMoveAnm();
    bool errorMove = errorMoveAnm();
    bool yesnoMenuMove = yesnoMenuMoveAnm();
    bool ketteiTxtDisp = ketteiTxtDispAnm();
    bool returnTxtDisp = modoruTxtDispAnm();

    if (headerTxtChange == true && nameMove == true && errorMove == true && 
        (yesnoMenuMove == true && ketteiTxtDisp == true && returnTxtDisp == true)) {
        field_0x026f = 15;
    }
}

void dFile_select_c::nameInputWait() {
    mpName->showIcon();
    field_0x026f = 16;
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

    int inputEnd = mpName->isInputEnd();

    switch(mpName->isInputEnd()) {
    case 1:
        if (dComIfGs_getNoFile() == 1) {
            field_0x021e = 0;
            field_0x026f = 0;
            field_0x0271 = 0;
        } else {
            headerTxtSet(0x43,1,0);
            fileRecScaleAnmInitSet2(0.0f,1.0f);
            nameMoveAnmInitSet(0xd29,0xd1f);
            modoruTxtDispAnmInit(0);
            field_0x026f = 14;
        }
        break;
    case 2:
        dComIfGs_setPlayerName(mpName->getInputStrPtr());
        mpName->hideIcon();
        field_0x020a = 15;
        field_0x026f = 17;
            
    }
}

void dFile_select_c::nameToDataSelectMove() {
    bool headerTxtChange =  headerTxtChangeAnm();
    bool fileResScal = fileRecScaleAnm2();
    bool nameMove = nameMoveAnm();
    bool modoruTxtDisp = modoruTxtDispAnm();

    if (headerTxtChange == true && fileResScal == true && nameMove == true && modoruTxtDisp == true) {
        modoruTxtChange(0);
        selFileCursorShow();
        field_0x026f = 3;
    }
}

void dFile_select_c::nameInputFade() {
    char name[32];
    field_0x020a--;
    u8 alpha = (1.0f - (field_0x020a / 15.0f)) * 255.0f;
    field_0x2378->setAlpha(alpha);

    if (field_0x020a == 0) {
        headerTxtSet(900,1,1);
        dMeter2Info_getString(899,name,0);
        dComIfGs_setHorseName(name);
        mpName->setNextNameStr(dComIfGs_getHorseName());
        mpName->initial();
        field_0x020a = 15;
        field_0x026f = 18;
    }
}

void dFile_select_c::nameInput2Move() {
    if (mDoRst::isReset() == 0) {
        field_0x020a--;

        u8 alpha = (field_0x020a / 15.0f) * 255.0f;
        field_0x2378->setAlpha(alpha);

        if (field_0x020a == 0) {
            mpName->showIcon();
            field_0x026f = 19;
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

    switch(mpName->isInputEnd()) {
    case 1:
        mpName->hideIcon();
        field_0x020a = 0xf;
        field_0x026f = 20;
        break;
    case 2:
        dComIfGs_setHorseName(mpName->getInputStrPtr());
        mSelectEnd = true;
        field_0x026f = 46;
    }
}

void dFile_select_c::backNameInputMove0() {
    field_0x020a--;
    u8 alpha = (1.0f - (field_0x020a / 15.0f)) * 255.0f;
    field_0x2378->setAlpha(alpha);

    if (field_0x020a == 0) {
        headerTxtSet(901,1,1);
        mpName->setNextNameStr(dComIfGs_getPlayerName());
        mpName->initial();
        modoruTxtChange(1);
        field_0x020a = 15;
        field_0x026f = 21;
    }
}

void dFile_select_c::backNameInputMove() {
    if (mDoRst::isReset() == 0) {
        field_0x020a--;

        u8 alpha = (field_0x020a / 15.0f) * 255.0f;
        field_0x2378->setAlpha(alpha);

        if (field_0x020a == 0) {
            modoruTxtChange(1);
            field_0x026f = 15;
        }
    }
}

void dFile_select_c::ToCopyPaneMove() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool menuMove = menuMoveAnm();
    bool selectDataMove = selectDataMoveAnm();
    bool selectWakuAlpha = selectWakuAlpahAnm(mSelectNum);
    bool nameMove = nameMoveAnm();

    if (headerTxtChange == true && menuMove == true && selectDataMove == true &&
        selectWakuAlpha == true && nameMove == true)
    {
        field_0x026b = 0;
        Vec local_2c = field_0x02a4[field_0x026b]->getGlobalVtxCenter(false, 0);
        mSelIcon2->setPos(local_2c.x, local_2c.y, field_0x02a4[field_0x026b]->getPanePtr(), true);
        mSelIcon2->setAlphaRate(1.0f);
        field_0x026f = 22;
    }
}

void dFile_select_c::ToErasePaneMove() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool yesnoMenuMove = yesnoMenuMoveAnm();

    if (headerTxtChange == true && yesnoMenuMove == true) {
        yesnoCursorShow();
        field_0x026f = 26;
    }
}

void dFile_select_c::backSelectMove() {
    bool headerTxtChange = headerTxtChangeAnm();
    bool selectDataMove = selectDataMoveAnm();
    bool menuMove = menuMoveAnm();
    bool modoruTxtDisp = modoruTxtDispAnm();

    if (headerTxtChange == true && selectDataMove == true && menuMove == true && modoruTxtDisp == true) {
        selFileCursorShow();
        mpFileSelect3d->drawOff();
        field_0x026f = 3;
    }
}

void dFile_select_c::copySelMoveAnmInitSet(int param_0, int param_1) {
    if (param_0 == 0xd1f) {
        field_0x02c0[0]->setAlpha(0xff);
        field_0x02c8[0]->setAlpha(0xff);
        field_0x02d0[0]->setAlpha(0xff);
        field_0x02c0[1]->setAlpha(0);
        field_0x02c8[1]->setAlpha(0);
        field_0x02d0[1]->setAlpha(0);

        field_0x02a4[0]->getPanePtr()->setAnimation(field_0x029c);
        field_0x029c->setFrame(99.0f);
        field_0x02a4[0]->getPanePtr()->animationTransform();
        field_0x02a4[0]->getPanePtr()->setAnimation((J2DAnmTransform*)0);

        field_0x02a4[1]->getPanePtr()->setAnimation(field_0x02a0);
        field_0x02a0->setFrame(109.0f);
        field_0x02a4[1]->getPanePtr()->animationTransform();
        field_0x02a4[1]->getPanePtr()->setAnimation((J2DAnmTransform*)0);
        
        setSaveDataForCopySel();
        mCpSel.field_0x08 = true;
    }

    field_0x011c->setAnimation(field_0x0094);
    field_0x0120 = param_0;
    field_0x0124 = param_1;
    field_0x0094->setFrame(field_0x0120);
    field_0x011c->animationTransform();
}

void dFile_select_c::setSaveDataForCopySel() {
    static u64 l_tagName101[2] = {'w_nun01', 'w_nun02'};
    static char* l_numTex[3] = {
        "tt_1_metal_40x40.bti", "tt_2_metal_40x40.bti", "tt_3_metal_40x40.bti"
    };
    SaveData* pSave = mSave;
    int notSelectedIndex = 0;
    for (int i = 0; i < 3; i++) {
        if (i == mSelectNum) {
            pSave++;
            continue;
        }
        J2DPicture* tagName = (J2DPicture*) mCpSel.Scr->search(l_tagName101[notSelectedIndex]);
        tagName->changeTexture(l_numTex[i], 0);
        CPaneMgrAlpha* datBase = mpFileInfo[notSelectedIndex]->getDatBase();
        CPaneMgrAlpha* noDatBase = mpFileInfo[notSelectedIndex]->getNoDatBase();
        int saveResult =
            mpFileInfo[notSelectedIndex]->setSaveData((dSv_save_c*)*pSave, field_0x025b[i] == 0, i);
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
    mStick->checkTrigger();
    if (mDoCPd_c::getTrigA(0) != 0) {
        copyDataToSelectStart();
        return;
    }
    if (mDoCPd_c::getTrigB(0) != 0) {
        copyDataToSelectCansel();
        return;
    }
    if (mStick->checkUpTrigger()) {
        if (field_0x026b != 0) {
            mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, 0, 0, 0);
            field_0x026c = field_0x026b;
            field_0x026b = 0;
            copyDataToSelectMoveAnmSet();
            field_0x026f = 23;
        }
    } else if (mStick->checkDownTrigger() && field_0x026b != 1) {
        mDoAud_seStart(Z2SE_FILE_SELECT_CURSOR, 0, 0, 0);
        field_0x026c = field_0x026b;
        field_0x026b = 1;
        copyDataToSelectMoveAnmSet();
        field_0x026f = 23;
    }
}

void dFile_select_c::copyDataToSelectStart() {
    mDoAud_seStart(Z2SE_SY_CURSOR_OK, 0, 0, 0);
    field_0x026d = getCptoNum(field_0x026b);
    mSelIcon2->setAlphaRate(0.0f);
    if (mDataNew[field_0x026d] != 0) {
        headerTxtSet(70, 0, 0);
        mpFileWarning->openInit();
        field_0x0158[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
        field_0x0158[mSelectNum]->alphaAnimeStart(0);
        field_0x0158[mSelectNum]->setAlpha(0);
        field_0x0158[field_0x026d]->getPanePtr()->scale(1.0f, 1.0f);
        field_0x0158[field_0x026d]->alphaAnimeStart(0);
        field_0x0158[field_0x026d]->setAlpha(0);
        selectWakuAlpahAnmInit(field_0x026d, 0, 0xff, g_fsHIO.field_0x0008);
        copySelMoveAnmInitSet(3369, 3359);
        field_0x01fc = 0;
        field_0x0204 = 0;
        field_0x0209 = 1;
        field_0x03b1 = 1;
        modoruTxtDispAnmInit(0);
        ketteiTxtDispAnmInit(0);
        field_0x026f = 29;
    } else {
        yesnoMenuMoveAnmInitSet(1139, 1149);
        headerTxtSet(69, 0, 0);
        field_0x026f = 25;
    }
}

void dFile_select_c::copyDataToSelectCansel() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, 0, 0, 0);
    selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
    menuMoveAnmInitSet(799, 809);
    copySelMoveAnmInitSet(3369, 3359);
    headerTxtSet(msgTbl[mSelectNum], 1, 0);
    mSelIcon2->setAlphaRate(0.0f);
    selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.field_0x0008);
    field_0x026f = 24;
}

void dFile_select_c::copyDataToSelectMoveAnmSet() {
    if (field_0x026b != 0xff) {
        field_0x02d8[field_0x026b]->alphaAnimeStart(0);
        field_0x02a4[field_0x026b]->getPanePtr()->setAnimation(field_0x029c);
        field_0x02b4[field_0x026b] = 109;
        field_0x029c->setFrame(field_0x02b4[field_0x026b]);
        field_0x02a4[field_0x026b]->getPanePtr()->animationTransform();
    }
    if (field_0x026c != 0xff) {
        copySelectWakuAlpahAnmInit(field_0x026c, 0xff, 0, g_fsHIO.field_0x0008);
        field_0x02d8[field_0x026c]->alphaAnimeStart(0);
        field_0x02a4[field_0x026c]->getPanePtr()->setAnimation(field_0x02a0);
        field_0x02b4[field_0x026c] = 99;
        field_0x02a0->setFrame(field_0x02b4[field_0x026c]);
        field_0x02a4[field_0x026c]->getPanePtr()->animationTransform();
        mSelIcon2->setAlphaRate(0.0f);
    }
}

void dFile_select_c::copyDataToSelectMoveAnm() {
    bool iVar7 = true;
    bool iVar6 = true;
    bool bVar1 = true;
    if (field_0x026c != 0xff) {
        iVar7 = field_0x02d8[field_0x026c]->alphaAnime(g_fsHIO.field_0x0006, 0xff, 0, 1);
        iVar6 = copySelectWakuAlpahAnm(field_0x026c);
        if (field_0x02b4[field_0x026c] != 109) {
            field_0x02b4[field_0x026c] += 2;
            if (field_0x02b4[field_0x026c] > 109) {
                field_0x02b4[field_0x026c] = 109;
            }
            field_0x02a0->setFrame(field_0x02b4[field_0x026c]);
            field_0x02a4[field_0x026c]->getPanePtr()->animationTransform();
            bVar1 = false;
        }
    }
    bool iVar5 = true;
    bool bVar2 = true;
    if (field_0x026b != 0xff) {
        iVar5 = field_0x02d8[field_0x026b]->alphaAnime(g_fsHIO.field_0x0006, 0, 0xff, 1);
        if (field_0x02b4[field_0x026b] != 99) {
                field_0x02b4[field_0x026b] -= 2;
                if (field_0x02b4[field_0x026b] < 99) {
                    field_0x02b4[field_0x026b] = 99;
                }
                field_0x029c->setFrame(field_0x02b4[field_0x026b]);
                field_0x02a4[field_0x026b]->getPanePtr()->animationTransform();
                bVar2 = false;
            }
    }

    if (iVar7 == true && iVar6 == true && bVar1 == true && iVar5 == true && bVar2 == true) {
        if (field_0x026c != 0xff) {
            field_0x02a4[field_0x026c]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        if (field_0x026b != 0xff) {
            field_0x02a4[field_0x026b]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            field_0x02c0[field_0x026b]->setAlpha(0xff);
            field_0x02c8[field_0x026b]->setAlpha(0xff);
            field_0x02d0[field_0x026b]->setAlpha(0xff);
            Vec local_3c = field_0x02a4[field_0x026b]->getGlobalVtxCenter(false, 0);
            mSelIcon2->setPos(local_3c.x, local_3c.y, field_0x02a4[field_0x026b]->getPanePtr(),
                                true);
            mSelIcon2->setAlphaRate(1.0f);
        }
        field_0x026f = 22;
    }
}

void dFile_select_c::copySelectWakuAlpahAnmInit(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    if (param_1 != 0xff) {
        field_0x02c0[param_1]->alphaAnimeStart(0);
        field_0x02c8[param_1]->alphaAnimeStart(0);
        field_0x02d0[param_1]->alphaAnimeStart(0);
        field_0x02e0[param_1] = param_2;
        field_0x02e2[param_1] = param_3;
        field_0x02e4[param_1] = param_4;
    }
}

bool dFile_select_c::copySelectWakuAlpahAnm(u8 param_1) {
    if (param_1 == 0xff) {
        return true;
    }
    bool rv = false;
    bool iVar2 = field_0x02c0[param_1]->alphaAnime(field_0x02e4[param_1], field_0x02e0[param_1], field_0x02e2[param_1], 0);
    bool iVar3 = field_0x02c8[param_1]->alphaAnime(field_0x02e4[param_1], field_0x02e0[param_1], field_0x02e2[param_1], 0);
    bool iVar4 = field_0x02d0[param_1]->alphaAnime(field_0x02e4[param_1], field_0x02e0[param_1], field_0x02e2[param_1], 0);
    if (iVar2 == true && iVar3 == true && iVar4 == true) {
        rv = true;
    }
    return rv;
}

u8 dFile_select_c::getCptoNum(u8 param_0) {
    u8 cptoNum;
    switch (field_0x026a) {
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
    bool txtChanged = headerTxtChangeAnm();
    bool selectDataMoved = selectDataMoveAnm();
    bool menuMoved = menuMoveAnm();
    bool selectWakuAnimated = selectWakuAlpahAnm(mSelectNum);
    bool nameMoved = nameMoveAnm();
    if (txtChanged == true && selectDataMoved == true && menuMoved == true &&
        selectWakuAnimated == true && nameMoved == true)
    {
        for (int i = 0; i < 3; i++) {
            field_0x0340[i]->getPanePtr()->setAnimation(field_0x0328);
            if (i == mSelectMenuNum) {
                field_0x0328->setFrame(MenuSelStartFrameTbl[i]);
            } else {
                field_0x0328->setFrame(MenuSelEndFrameTbl[i]);
            }
            field_0x0340[i]->getPanePtr()->animationTransform();
            field_0x0340[i]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            ((J2DTextBox*)mpPaneMgr2[i]->getPanePtr())->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xff));
        }
        menuCursorShow();
        field_0x026f = 8;
    }
}

void dFile_select_c::copyToSelPaneMove() {
    bool txtChanged = headerTxtChangeAnm();
    bool menuMoved = yesnoMenuMoveAnm();
    if (txtChanged == true && menuMoved == true) {
        yesnoCursorShow();
        field_0x026f = 26;
    }
}

bool dFile_select_c::yesnoMenuMoveAnmInitSet(int param_1, int param_2) {
    if (!field_0x0108) {
        field_0x0268 = false;
        field_0x0269 = true;
    }
    if (param_1 == 1139) {
        field_0x0281 = true;
        ((J2DTextBox*)field_0x01c0[field_0x0268]->getPanePtr())->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
        ((J2DTextBox*)field_0x01c0[field_0x0269]->getPanePtr())->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xff));
    }
    mSelIcon->setAlphaRate(0.0f);
    field_0x00f0[0]->getPanePtr()->setAnimation(field_0x008c);
    field_0x00f0[1]->getPanePtr()->setAnimation(field_0x008c);
    field_0x0100 = param_1;
    field_0x0104 = param_2;
    field_0x008c->setFrame(field_0x0100);
    field_0x00f0[0]->getPanePtr()->animationTransform();
    field_0x00f0[1]->getPanePtr()->animationTransform();
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
        field_0x008c->setFrame(field_0x0100);
        field_0x00f0[0]->getPanePtr()->animationTransform();
        field_0x00f0[1]->getPanePtr()->animationTransform();
        rv = false;
    } else {
        field_0x00f0[0]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        field_0x00f0[1]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        if (field_0x0100 == 1149) {
            field_0x0108 = true;
            field_0x0281 = false;
        } else {
            field_0x0108 = false;
            if (field_0x0268 != 0xff) {
                field_0x01a0[field_0x0268]->setAlpha(0);
                field_0x01a8[field_0x0268]->setAlpha(0);
                field_0x01b0[field_0x0268]->setAlpha(0);
            }
        }
        rv = true;
    }
    return rv;
}

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
        field_0x0310->setFrame(field_0x00f8[field_0x0269]);
        field_0x00f0[field_0x0269]->getPanePtr()->animationTransform();
        bVar1 = false;
    }
    bool bVar2 = true;
    if (field_0x0268 != 0xff && YnSelEndFrameTbl[field_0x0268] != field_0x00f8[field_0x0268])
    {
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
        field_0x0314->setFrame(field_0x00f8[field_0x0268]);
        field_0x00f0[field_0x0268]->getPanePtr()->animationTransform();
        bVar2 = false;
    }
    if (bVar1 == true && bVar2 == true) {
        if (field_0x0269 != 0xff) {
            field_0x00f0[field_0x0269]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        if (field_0x0268 != 0xff) {
            field_0x00f0[field_0x0268]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        rv = true;
    }
    return rv;
}

void dFile_select_c::yesnoCursorShow() {
    if (field_0x0268 == 0xff) {
        return;
    }
    ((J2DTextBox*)field_0x01c0[field_0x0268]->getPanePtr())->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
    field_0x01a0[field_0x0268]->setAlpha(0xff);
    field_0x01a8[field_0x0268]->setAlpha(0xff);
    field_0x01b0[field_0x0268]->setAlpha(0xff);
    Vec local_24 = field_0x00f0[field_0x0268]->getGlobalVtxCenter(0, 0);
    mSelIcon->setPos(local_24.x, local_24.y, field_0x00f0[field_0x0268]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
    mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
}

void dFile_select_c::YesNoSelect() {
    mStick->checkTrigger();
    if (mDoCPd_c::getTrigA(0) != 0) {
        yesNoSelectStart();
        return;  
    }

    if (mDoCPd_c::getTrigB(0) != 0) {
        yesnoCancelAnmSet();
        return;
    }

    if (mStick->checkRightTrigger()) {
        if (field_0x0268 != 0) {
            mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, 0, 0, 0);
            field_0x0269 = field_0x0268;
            field_0x0268 = 0;
            yesnoSelectAnmSet();
            field_0x026f = 28;
        }
    } else if (mStick->checkLeftTrigger() && field_0x0268 != 1) {
        mDoAud_seStart(Z2SE_SY_MENU_CURSOR_COMMON, 0, 0, 0);
        field_0x0269 = field_0x0268;
        field_0x0268 = 1;
        yesnoSelectAnmSet();
        field_0x026f = 28;
    }
}

void dFile_select_c::yesNoSelectStart() {
    if (field_0x0268 != 0) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OK, 0, 0, 0);
        field_0x03b1 = 1;
        switch(field_0x026e) {
        case 1:
            headerTxtSet(74, 0, 0);
            mpFileWarning->openInit();
            field_0x014c[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
            field_0x014c[mSelectNum]->alphaAnimeStart(0);
            field_0x014c[mSelectNum]->setAlpha(0);
            menuMoveAnmInitSet(809, 799);
            yesnoMenuMoveAnmInitSet(1149, 1139);
            mSelIcon->setAlphaRate(0.0f);
            field_0x01f4 = 0;
            field_0x0204 = 0;
            field_0x0208 = 1;
            break;
        case 2:
            headerTxtSet(0x4a, 0, 0);
            mpFileWarning->openInit();
            field_0x014c[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
            field_0x014c[mSelectNum]->alphaAnimeStart(0);
            field_0x014c[mSelectNum]->setAlpha(0);
            yesnoMenuMoveAnmInitSet(1149, 1139);
            mSelIcon->setAlphaRate(0.0f);
            field_0x01f4 = 0;
            field_0x0204 = 0;
            field_0x0208 = 1;
            break;
        case 0:
            headerTxtSet(0x46, 0, 0);
            mpFileWarning->openInit();
            field_0x0158[mSelectNum]->getPanePtr()->scale(1.0f, 1.0f);
            field_0x0158[mSelectNum]->alphaAnimeStart(0);
            field_0x0158[mSelectNum]->setAlpha(0);
            field_0x0158[field_0x026d]->getPanePtr()->scale(1.0f, 1.0f);
            field_0x0158[field_0x026d]->alphaAnimeStart(0);
            field_0x0158[field_0x026d]->setAlpha(0);
            yesnoMenuMoveAnmInitSet(1149, 1139);
            mSelIcon->setAlphaRate(0.0f);
            mSelIcon2->setAlphaRate(0.0f);
            selectWakuAlpahAnmInit(field_0x026d, 0, 0xff, g_fsHIO.field_0x0008);
            copySelMoveAnmInitSet(0xd29, 0xd1f);
            field_0x01fc = 0;
            field_0x0204 = 0;
            field_0x0209 = 1;
            break;
        }
        modoruTxtDispAnmInit(0);
        ketteiTxtDispAnmInit(0);
        field_0x026f = 29;
    } else {
        yesnoCancelAnmSet();
    }
}

void dFile_select_c::yesnoSelectAnmSet() {
    if (field_0x0269 != 0xff) {
        yesnoWakuAlpahAnmInit(field_0x0269, 0xff, 0, g_fsHIO.field_0x0008);
        field_0x00f0[field_0x0269]->getPanePtr()->setAnimation(field_0x0310);
        field_0x00f8[field_0x0269] = YnSelEndFrameTbl[field_0x0269];
        field_0x0310->setFrame(field_0x00f8[field_0x0269]);
        field_0x00f0[field_0x0269]->getPanePtr()->animationTransform();
        mSelIcon->setAlphaRate(0.0f);
    }
    if (field_0x0268 != 0xff) {
        field_0x00f0[field_0x0268]->getPanePtr()->setAnimation(field_0x0314);
        field_0x00f8[field_0x0268] = YnSelStartFrameTbl[field_0x0268];
        field_0x0314->setFrame(field_0x00f8[field_0x0268]);
        field_0x00f0[field_0x0268]->getPanePtr()->animationTransform();
    }
}

void dFile_select_c::yesnoCancelAnmSet() {
    mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, 0, 0, 0);
    mSelIcon->setAlphaRate(0.0f);
    switch (field_0x026e) {
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
    field_0x026f = 27;
}

void dFile_select_c::YesNoCancelMove() {
    bool txtChanged = headerTxtChangeAnm();
    bool moveAnimated = yesnoMenuMoveAnm();
    bool selectMoveAnimated;
    switch (field_0x026e) {
    case 1:
        selectMoveAnimated = true;
        break;
    case 2:
        selectMoveAnimated = selectDataMoveAnm();
        break;
    case 0:
        selectMoveAnimated = true;
        break;
    }
    if (txtChanged == true && moveAnimated == true && selectMoveAnimated == true) {
        switch (field_0x026e) {
        case 1:
            if (mSelectMenuNum != 0xff) {
                Vec vtxCenter;
                vtxCenter = field_0x0340[mSelectMenuNum]->getGlobalVtxCenter(false, 0);
                mSelIcon->setPos(vtxCenter.x, vtxCenter.y, field_0x0340[mSelectMenuNum]->getPanePtr(),
                                 true);
                mSelIcon->setAlphaRate(1.0f);
                mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
            }
            field_0x026f = 8;
            break;
        case 2:
            selFileCursorShow();
            field_0x026f = 3;
            break;
        case 0:
            mSelIcon2->setAlphaRate(1.0f);
            field_0x026f = 22;
            break;
        }
    }
}

void dFile_select_c::yesNoCursorMoveAnm() {
    bool moveAnimated = yesnoSelectMoveAnm();
    bool wakuAnimated = yesnoWakuAlpahAnm(field_0x0269);
    if (moveAnimated == true && wakuAnimated == true) {
        yesnoCursorShow();
        field_0x026f = 26;
    }
}

void dFile_select_c::CmdExecPaneMove0() {
    int txtChanged = headerTxtChangeAnm();
    int unaff_r29;
    int unaff_r28;
    int unaff_r30;
    int unaff_r27;
    int nameMoved;
    switch (field_0x026e) {
    case 1:
        unaff_r29 = field_0x014c[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0, 0xff, 0);
        unaff_r28 = yesnoMenuMoveAnm();
        unaff_r30 = menuMoveAnm();
        unaff_r27 = true;
        nameMoved = true;
        break;
    case 2:
        unaff_r29 = field_0x014c[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0, 0xff, 0);
        unaff_r28 = true;
        unaff_r30 = yesnoMenuMoveAnm();
        unaff_r27 = true;
        nameMoved = true;
        break;
    case 0:
        unaff_r29 = field_0x0158[field_0x026d]->alphaAnime(g_fsHIO.field_0x0009, 0, 0xff, 0);
        unaff_r28 = field_0x0158[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0, 0xff, 0);
        if (field_0x0108) {
            unaff_r30 = yesnoMenuMoveAnm();
        } else {
            unaff_r30 = true;
        }
        unaff_r27 = selectWakuAlpahAnm(field_0x026d);
        nameMoved = nameMoveAnm();
        break;
    }
    int modoruTxt = modoruTxtDispAnm();
    int kettaiTxt = ketteiTxtDispAnm();
    int warningStatus = mpFileWarning->getStatus() != 0;
    if (txtChanged == true && unaff_r29 == true && unaff_r28 == true && unaff_r30 == true &&
        unaff_r27 == true && nameMoved == true && modoruTxt == true && kettaiTxt == true && warningStatus == true)
    {
        field_0x026f = 30;
    }
}

void dFile_select_c::CommandExec() {
    switch (field_0x026e) {
    case 1:
    case 2:
        mDoAud_seStartLevel(Z2SE_SY_FILE_DELETE_LEVEL, 0, 0, 0);
        dComIfGs_setInitDataToCard((u8*)mSave, mSelectNum);
        mDoMemCdRWm_SetCheckSumGameData((u8*)mSave, mSelectNum);
        dataSave();
        field_0x026f = 31;
        break;
    case 0:
        mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, 0, 0, 0);
        memcpy(&mSave[field_0x026d], &mSave[field_0x026a], sizeof(SaveData));
        mDoMemCdRWm_SetCheckSumGameData((u8*)mSave, field_0x026d);
        mDoMemCd_setCopyToPos(field_0x026d);
        dataSave();
        field_0x026f = 33;
        break;
    }
    field_0x03ac = g_fsHIO.field_0x000c;
}

void dFile_select_c::DataEraseWait() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    }
    mDoAud_seStartLevel(Z2SE_SY_FILE_DELETE_LEVEL, 0, 0, 0);
    field_0x03b4 = mDoMemCd_SaveSync();
    if (field_0x03b4 != 0) {
        field_0x026f = 32;
    }
}

void dFile_select_c::DataEraseWait2() {
    if (field_0x03ac != 0) {
        mDoAud_seStartLevel(Z2SE_SY_FILE_DELETE_LEVEL, 0, 0, 0);
        field_0x03ac--;
    } else if (field_0x03b4 == 2) {
        mDoAud_seStart(Z2SE_SY_FILE_ERROR, 0, 0, 0);
        headerTxtSet(76, 0, 0);
        mpFileWarning->closeInit();
        modoruTxtDispAnmInit(1);
        ketteiTxtDispAnmInit(1);
        field_0x014c[mSelectNum]->alphaAnimeStart(0);
        field_0x026f = 39;
    } else if (field_0x03b4 == 1) {
        mDoAud_seStart(Z2SE_SY_FILE_DELETE_OK, 0, 0, 0);
        field_0x03b1 = 0;
        field_0x014c[mSelectNum]->alphaAnimeStart(0);
        field_0x022c[mSelectNum]->alphaAnimeStart(0);
        field_0x0220[mSelectNum]->alphaAnimeStart(0);
        field_0x026f = 37;
    }
}

void dFile_select_c::ErasePaneMoveOk() {
    int iVar1 = field_0x014c[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    int iVar2;
    if (field_0x025b[mSelectNum] != 0) {
        iVar2 = field_0x022c[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    } else {
        iVar2 = field_0x0220[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    }
    if (iVar1 == 1 && iVar2 == 1) {
        field_0x0208 = 0;
        setSaveData();
        makeRecInfo(mSelectNum);
        headerTxtSet(0x4b, 0, 0);
        mpFileWarning->closeInit();
        field_0x022c[mSelectNum]->alphaAnimeStart(0);
        selectWakuAlpahAnmInit(mSelectNum, 0, 0xff, g_fsHIO.field_0x0007);
        field_0x026f = 38;
    }
}

void dFile_select_c::ErasePaneMoveOk2() {
    bool txtChanged = headerTxtChangeAnm();
    bool iVar2 =
        field_0x022c[mSelectNum]->alphaAnime(
                                  g_fsHIO.field_0x0007, 0, 0xff, 0);
    bool wakuAnm = selectWakuAlpahAnm(mSelectNum);
    bool warningStatus = mpFileWarning->getStatus();
    if (txtChanged == true && iVar2 == true && wakuAnm == true && warningStatus == true) {
        field_0x03ac = g_fsHIO.field_0x000a;
        mSelectMenuNum = 0;
        field_0x026f = 40;
    }
}

void dFile_select_c::eraseEndBackSelectWait() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    } else if (field_0x03ac == 0) {
        selectDataMoveAnmInitSet(SelOpenEndFrameTbl[mSelectNum], SelOpenStartFrameTbl[mSelectNum]);
        headerTxtSet(67, 1, 0);
        ketteiTxtDispAnmInit(1);
        field_0x026f = 41;
    }
}

void dFile_select_c::eraseEndBackSelect() {
    bool txtChanged = headerTxtChangeAnm();
    bool dataMoved = selectDataMoveAnm();
    bool ketteiTxtAnm = ketteiTxtDispAnm();
    if (txtChanged == true && dataMoved == true && ketteiTxtAnm == true) {
        selFileCursorShow();
        mpFileSelect3d->drawOff();
        field_0x026f = 3;
    }
}

void dFile_select_c::DataCopyWait() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    }
    mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, 0, 0, 0);
    field_0x03b4 = mDoMemCd_SaveSync();
    if (field_0x03b4 != 0) {
        field_0x026f = 34;
    }
}

void dFile_select_c::DataCopyWait2() {
    if (field_0x03ac != 0) {
        mDoAud_seStartLevel(Z2SE_SY_FILE_COPY_LEVEL, 0, 0, 0);
        field_0x03ac--;
    } else {
        if (field_0x03b4 == 2) {
            mDoAud_seStart(Z2SE_SY_FILE_ERROR, 0, 0, 0);
            headerTxtSet(0x48, 0, 0);
            mpFileWarning->closeInit();
            modoruTxtDispAnmInit(1);
            ketteiTxtDispAnmInit(1);
            field_0x026f = 39;
        } else if (field_0x03b4 == 1) {
            mDoAud_seStart(Z2SE_SY_FILE_COPY_OK, 0, 0, 0);
            field_0x03b1 = 0;
            field_0x0158[mSelectNum]->alphaAnimeStart(0);
            field_0x0158[field_0x026d]->alphaAnimeStart(0);
            field_0x022c[field_0x026d]->alphaAnimeStart(0);
            field_0x0220[field_0x026d]->alphaAnimeStart(0);
            field_0x026f = 35;
        }
        
    }
}

void dFile_select_c::copyPaneMoveOk() {
    bool iVar1 = field_0x0158[field_0x026d]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    bool iVar2 = field_0x0158[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    bool iVar3;
    if (field_0x025b[field_0x026d] != 0 || mDataNew[field_0x026d] != 0) {
        iVar3 = field_0x022c[field_0x026d]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    } else {
        iVar3 = field_0x0220[field_0x026d]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
    }
    if (iVar1 == true && iVar2 == true && iVar3 == true) {
        field_0x0209 = 0;
        headerTxtSet(71, 0, 0);
        mpFileWarning->closeInit();
        setSaveData();
        field_0x022c[field_0x026d]->alphaAnimeStart(0);
        field_0x0220[field_0x026d]->alphaAnimeStart(0);
        field_0x026f = 36;
    }
}

void dFile_select_c::copyPaneMoveOk2() {
    bool iVar1 = headerTxtChangeAnm();
    bool iVar2;
    if (field_0x025b[field_0x026d] != 0 || mDataNew[field_0x026d] != 0) {
        iVar2 = field_0x022c[field_0x026d]->alphaAnime(g_fsHIO.field_0x0007, 0, 0xff, 0);
    } else {
        iVar2 = field_0x0220[field_0x026d]->alphaAnime(g_fsHIO.field_0x0007, 0, 0xff, 0);
    }
    bool iVar3 = mpFileWarning->getStatus();
    if (iVar1 == true && iVar2 == true && iVar3 == true) {
        field_0x03ac = g_fsHIO.field_0x000a;
        field_0x026f = 42;
    }
}

void dFile_select_c::ErrorMsgPaneMove() {
    int txtChanged = headerTxtChangeAnm();
    int unaff_r30;
    int unaff_r29;
    switch (field_0x026e) {
    case 1:
    case 2:
        unaff_r30 = field_0x014c[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
        unaff_r29 = 1;
        break;

    case 0:
        unaff_r30 = field_0x0158[field_0x026d]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
        unaff_r29 = field_0x0158[mSelectNum]->alphaAnime(g_fsHIO.field_0x0009, 0xff, 0, 0);
        break;
    }
    int modoruTxtAnm = modoruTxtDispAnm();
    int ketteiTxtAnm = ketteiTxtDispAnm();
    int warningStatus = mpFileWarning->getStatus() != 0;
    if (txtChanged == 1 && unaff_r30 == 1 && unaff_r29 == 1 && modoruTxtAnm == 1 && ketteiTxtAnm == 1 &&
        warningStatus == 1)
    {
        field_0x0208 = 0;
        field_0x0209 = 0;
        field_0x03b1 = 0;
        field_0x03ac = g_fsHIO.field_0x000a;
        field_0x026f = 43;
    }
}

void dFile_select_c::backDatSelPaneMove() {
    int headerTxtAnm = headerTxtChangeAnm();
    int menuMoved = menuMoveAnm();
    int selectDataMoved = selectDataMoveAnm();
    int wakuAlpha1 = selectWakuAlpahAnm(mSelectNum);
    int wakuAlpha2 = selectWakuAlpahAnm(field_0x026d);
    int modoruTxtAnm = modoruTxtDispAnm();
    int ketteiTxtAnm = ketteiTxtDispAnm();
    if (headerTxtAnm == 1 && menuMoved == 1 && selectDataMoved == 1 && wakuAlpha1 == 1 &&
        wakuAlpha2 == 1 && modoruTxtAnm == 1 && ketteiTxtAnm == 1)
    {
        if (mSelectMenuNum != 0xff) {
            field_0x0340[mSelectMenuNum]->getPanePtr()->setAnimation(field_0x0328);
            field_0x0328->setFrame(MenuSelStartFrameTbl[mSelectMenuNum]);
            field_0x0340[mSelectMenuNum]->getPanePtr()->animationTransform();
            field_0x0340[mSelectMenuNum]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
            menuCursorShow();
        }
        field_0x026f = 8;
    }
}

void dFile_select_c::backDatSelWait() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    } else if (field_0x03ac == 0) {
        headerTxtSet(msgTbl[mSelectNum], 1, 0);
        selectWakuAlpahAnmInit(field_0x026d, 0xff, 0, g_fsHIO.field_0x0008);
        selectWakuAlpahAnmInit(mSelectNum, 0xff, 0, g_fsHIO.field_0x0008);
        menuMoveAnmInitSet(799, 809);
        selectDataMoveAnmInitSet(SelOpenStartFrameTbl[mSelectNum], SelOpenEndFrameTbl[mSelectNum]);
        modoruTxtDispAnmInit(1);
        ketteiTxtDispAnmInit(1);
        field_0x026f = 44;
    }
}

void dFile_select_c::backDatSelWait2() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    }
    if (cAPICPad_ANY_BUTTON(0) != 0 || field_0x03ac == 0) {
        field_0x026f = 0;
        field_0x0271 = 0;
    }
}

void dFile_select_c::nextModeWait() {
    /* empty function */
}

void dFile_select_c::screenSet() {
    static u64 l_tagName0[3] = {'w_sel_00', 'w_sel_01', 'w_sel_02'};
    static u64 l_tagName3[3] = {'w_moyo00', 'w_moyo01', 'w_moyo02'};
    static u64 l_tagName4[3] = {'w_gold00', 'w_gold01', 'w_gold02'};
    static u64 l_tagName5[3] = {'w_go2_00', 'w_go2_01', 'w_go2_02'};
    static u64 l_tagName12[3] = {'w_bk_l00', 'w_bk_l01', 'w_bk_l02'};
    static u64 l_nouseTag[15] = {
        'w_mcheck', 'w_tabi1', 'w_tabi2',  'w_tabi3',  'w_doko_c', 'w_uwa_c', 'w_cp_chu', 'w_cpsita',
        'w_cp_x',   'w_de',    'w_de_chu', 'w_desita', 'w_de_x',   'w_name',  'w_h_name'};
    
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
    void* uVar14 = JKRGetNameResource("zelda_file_select.bck", mpArchive);
    field_0x0084 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(uVar14);
    field_0x0088 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(uVar14);
    field_0x0090 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(uVar14);
    field_0x0094 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(uVar14);
    field_0x0098 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(uVar14);
    field_0x009c = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(uVar14);
    field_0x0084->searchUpdateMaterialID(fileSel.Scr);
    field_0x0088->searchUpdateMaterialID(fileSel.Scr);
    field_0x0090->searchUpdateMaterialID(fileSel.Scr);
    field_0x009c->searchUpdateMaterialID(fileSel.Scr);
    field_0x00a4 = new CPaneMgr(fileSel.Scr, 'w_move_n', 0, NULL);
    mpPane = fileSel.Scr->search('w_sub_n');
    for (int i = 0; i < 3; i++) {
        field_0x00bc[i] = new CPaneMgr(fileSel.Scr, l_tagName0[i], 1, NULL);
    }
    fileSel.Scr->search('Wi_btn_n')->hide();
    field_0x0238 = new CPaneMgrAlpha(fileSel.Scr, 'w_n_bbtn', 2, NULL);
    field_0x023c = new CPaneMgrAlpha(fileSel.Scr, 'w_n_abtn', 2, NULL);

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    field_0x0240 = new CPaneMgrAlpha(fileSel.Scr, 'w_modo', 2, NULL);
    field_0x0244 = new CPaneMgrAlpha(fileSel.Scr, 'w_kete', 2, NULL);
    fileSel.Scr->search('f_modo')->hide();
    fileSel.Scr->search('f_kete')->hide();
    #else
    field_0x0240 = new CPaneMgrAlpha(fileSel.Scr, 'f_modo', 2, NULL);
    field_0x0244 = new CPaneMgrAlpha(fileSel.Scr, 'f_kete', 2, NULL);
    fileSel.Scr->search('w_modo')->hide();
    fileSel.Scr->search('w_kete')->hide();
    #endif

    ((J2DTextBox*)field_0x0240->getPanePtr())->setFont(fileSel.mpMessageFont[0]);
    ((J2DTextBox*)field_0x0244->getPanePtr())->setFont(fileSel.mpMessageFont[0]);
    ((J2DTextBox*)field_0x0240->getPanePtr())->setString(32, "");
    field_0x0250 = ((J2DTextBox*)field_0x0240->getPanePtr())->getStringPtr();
    fopMsgM_messageGet(field_0x0250, 0x54);
    field_0x024c = 0;
    char acStack_38[32];
    fopMsgM_messageGet(acStack_38, 0x55);
    ((J2DTextBox*)field_0x0244->getPanePtr())->setString(acStack_38);
    void* bpk = JKRGetNameResource("zelda_file_select.bpk", mpArchive);
    JUT_ASSERT(4994, bpk != NULL);
    field_0x01c8 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    field_0x01c8->searchUpdateMaterialID(fileSel.Scr);
    field_0x01cc = 0;
    void* btk = JKRGetNameResource("zelda_file_select_05.btk", mpArchive);
    JUT_ASSERT(5004, btk != NULL);
    field_0x01d0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    field_0x01d0->searchUpdateMaterialID(fileSel.Scr);
    field_0x01d4 = 0;
    for (int i = 0; i < 3; i++) {
        field_0x0164[i] = new CPaneMgr(fileSel.Scr, l_tagName3[i], 0, NULL);
        field_0x0170[i] = new CPaneMgr(fileSel.Scr, l_tagName4[i], 0, NULL);
        field_0x017c[i] = new CPaneMgr(fileSel.Scr, l_tagName5[i], 0, NULL);
        field_0x0164[i]->getPanePtr()->setAnimation(field_0x01c8);
        field_0x0170[i]->getPanePtr()->setAnimation(field_0x01c8);
        field_0x017c[i]->getPanePtr()->setAnimation(field_0x01c8);
        field_0x0170[i]->getPanePtr()->setAnimation(field_0x01d0);
        field_0x017c[i]->getPanePtr()->setAnimation(field_0x01d0);
    }
    field_0x01d8 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    field_0x01d8->searchUpdateMaterialID(fileSel.Scr);
    btk = JKRGetNameResource("zelda_file_select.btk", mpArchive);
    JUT_ASSERT(5039, btk != NULL);
    field_0x01e0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    field_0x01e0->searchUpdateMaterialID(fileSel.Scr);
    void* brk = JKRGetNameResource("zelda_file_select.brk", mpArchive);
    JUT_ASSERT(5048, brk != NULL);
    field_0x01e8 = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    field_0x0200 = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    field_0x01e8->searchUpdateMaterialID(fileSel.Scr);
    field_0x0200->searchUpdateMaterialID(fileSel.Scr);
    field_0x01dc = 0;
    field_0x01e4 = 0;
    field_0x01ec = 0;
    for (int i = 0; i < 3; i++) {
        field_0x0188[i] = new CPaneMgr(fileSel.Scr, l_tagName12[i], 0, NULL);
        field_0x0188[i]->getPanePtr()->setAnimation(field_0x01d8);
        field_0x0188[i]->getPanePtr()->setAnimation(field_0x01e0);
        field_0x0188[i]->getPanePtr()->setAnimation(field_0x01e8);
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
        field_0x020c[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName21[i], 0, NULL);
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setFont(fileSel.mpMessageFont[0]);
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setString(512, "");
#if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setFontSize(21.0f, 21.0f);
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setLineSpace(22.0f);
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setCharSpace(2.0f);
#else
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setFontSize(24.0f, 24.0f);
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setLineSpace(20.0f);
        ((J2DTextBox*)field_0x020c[i]->getPanePtr())->setCharSpace(0.0f);
#endif
        field_0x0214[i] = ((J2DTextBox*)field_0x020c[i]->getPanePtr())->getStringPtr();
    }
    field_0x020c[0]->setAlpha(0xff);
    field_0x020c[1]->setAlpha(0);
    field_0x021c = 0;
    bpk = JKRGetNameResource("zelda_file_select_02.btk", mpArchive);
    JUT_ASSERT(5124, bpk != NULL);
    field_0x01f8 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(bpk);
    field_0x01f8->searchUpdateMaterialID(fileSel.Scr);
    bpk = JKRGetNameResource("zelda_file_select_03.btk", mpArchive);
    JUT_ASSERT(5133, bpk != NULL);
    field_0x01f0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(bpk);
    field_0x01f0->searchUpdateMaterialID(fileSel.Scr);
    field_0x01f4 = 0;
    field_0x01fc = 0;
    field_0x0204 = 0;
    for (int i = 0; i < 3; i++) {
        field_0x014c[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName18[i], 0, NULL);
        field_0x0158[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName19[i], 0, NULL);
        field_0x014c[i]->getPanePtr()->setAnimation(field_0x01f0);
        field_0x014c[i]->getPanePtr()->setAnimation(field_0x0200);
        field_0x0158[i]->getPanePtr()->setAnimation(field_0x01f8);
        field_0x0158[i]->getPanePtr()->setAnimation(field_0x0200);
    }
    field_0x0208 = 0;
    field_0x0209 = 0;
    field_0x012c = fileSel.Scr->search('w_er_n');

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    fileSel.Scr->search('er_for0')->hide();
    fileSel.Scr->search('er_for1')->hide();
    #else
    fileSel.Scr->search('w_er_msg')->hide();
    fileSel.Scr->search('w_er_msR')->hide();
    #endif

    fileSel.Scr->search('w_er_msE')->hide();
    for (int i = 0; i < 2; i++) {
        field_0x0138[i] = new CPaneMgrAlpha(fileSel.Scr, l_tagName20[i], 0, NULL);
        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setFont(fileSel.mpMessageFont[0]);
        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setString(512, "");

        #if (VERSION != VERSION_GCN_JPN) && (VERSION != VERSION_WII_JPN)
        field_0x0138[i]->getPanePtr()->resize(440.0f, 198.0f);
        #endif

        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setFontSize(21.0f, 21.0f);

        #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setLineSpace(22.0f);
        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setCharSpace(2.0f);
        #else
        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setLineSpace(21.0f);
        ((J2DTextBox*)field_0x0138[i]->getPanePtr())->setCharSpace(1.0f);
        #endif

        field_0x0140[i] = ((J2DTextBox*)field_0x0138[i]->getPanePtr())->getStringPtr();
        field_0x0138[i]->show();
    }
    field_0x0148 = 0;
    field_0x0138[field_0x0148]->setAlpha(0xff);
    field_0x0138[field_0x0148 ^ 1]->setAlpha(0);
    mSelIcon = new dSelect_cursor_c(0, 1.0f, NULL);
    JUT_ASSERT(5209, mSelIcon != NULL);
    mSelIcon->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
    Vec vtxCenter;
    vtxCenter = field_0x00bc[mSelectNum]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(vtxCenter.x, vtxCenter.y, field_0x00bc[mSelectNum]->getPanePtr(), true);
    mSelIcon->setAlphaRate(0.0f);
    J2DPane* piVar5 = fileSel.Scr->search('Nm_02');
    piVar5->setAnimation(field_0x0084);
    setWakuAnm();
    field_0x00a0 = 1;
    field_0x0084->setFrame(field_0x00a0);
    piVar5->animationTransform();
    piVar5->setAnimation((J2DAnmTransform*)NULL);
    field_0x020c[0]->getPanePtr()->scale(1.0f, 1.0f);
    field_0x020c[1]->getPanePtr()->scale(1.0f, 1.0f);
    J2DPane* apJStack_54[3];
    for (int i = 0; i < 3; i++) {
        apJStack_54[i] = new J2DPane(l_tagName13[i], JGeometry::TBox2<f32>(238.0f, 43.0f, 712.0f, 130.0f));
        apJStack_54[i]->setUserInfo('n_43');
        field_0x00bc[i]->getPanePtr()->insertChild(fileSel.Scr->search(l_tagName131[i]), apJStack_54[i]);
    }
    for (int i = 0; i < 3; i++) {
        field_0x0070[i] = new dFile_info_c(mpArchive, 0);
        field_0x0070[i]->setBasePane(apJStack_54[i]);
        field_0x0220[i] = field_0x0070[i]->getDatBase();
        field_0x022c[i] = field_0x0070[i]->getNoDatBase();
    }

    field_0x011c = fileSel.Scr->search('name_n');
    mpName = new dName_c(field_0x011c);
    field_0x0128 = false;
    mpFileWarning = new dFile_warning_c(mpArchive, 0);
    mSelDt.mpPane = fileSel.Scr->search('w_moyo03');
    JUtility::TColor black = mDoGph_gInf_c::getFadeColor();
    JUtility::TColor white = mDoGph_gInf_c::getFadeColor();
    black.a = 0;
    white.a = 0xff;
    ResTIMG* uVar20 = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    field_0x2378 = new J2DPicture('PICT01', JGeometry::TBox2<f32>(0.0f, FB_WIDTH, 0.0f, FB_HEIGHT), uVar20, NULL);
    field_0x2378->setBlackWhite(black, white);
    field_0x2378->setAlpha(0);
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
    void* bck = JKRGetNameResource("zelda_file_select_copy_select.bck",
                                          mpArchive);
    field_0x029c = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x02a0 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x029c->searchUpdateMaterialID(mCpSel.Scr);
    field_0x02a0->searchUpdateMaterialID(mCpSel.Scr);
    void* bpk = JKRGetNameResource("zelda_file_select_copy_select.bpk", mpArchive);
    JUT_ASSERT(5315, bpk != NULL);
    field_0x02e8 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    field_0x02e8->searchUpdateMaterialID(mCpSel.Scr);
    field_0x02ec = 0;
    void* btk = JKRGetNameResource("zelda_file_select_copy_select_03.btk", mpArchive);
    JUT_ASSERT(5325, btk != NULL);
    field_0x02f0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    field_0x02f0->searchUpdateMaterialID(mCpSel.Scr);
    field_0x02f4 = 0;
    field_0x02f8 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    field_0x02f8->searchUpdateMaterialID(mCpSel.Scr);
    btk = JKRGetNameResource("zelda_file_select_copy_select.btk", mpArchive);
    JUT_ASSERT(5343, btk != NULL);
    field_0x0300 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    field_0x0300->searchUpdateMaterialID(mCpSel.Scr);
    void* brk = JKRGetNameResource("zelda_file_select_copy_select.brk", mpArchive);
    JUT_ASSERT(5352, brk != NULL);
    field_0x0308 = (J2DAnmTevRegKey*)J2DAnmLoaderDataBase::load(brk);
    field_0x0308->searchUpdateMaterialID(mCpSel.Scr);
    field_0x02fc = 0;
    field_0x0304 = 0;
    field_0x030c = 0;
    for (int i = 0; i < 2; i++) {
        field_0x02a4[i] = new CPaneMgr(mCpSel.Scr, l_tagName000[i], 0, NULL);
        field_0x02c0[i] = new CPaneMgr(mCpSel.Scr, l_tagName001[i], 0, NULL);
        field_0x02c8[i] = new CPaneMgr(mCpSel.Scr, l_tagName002[i], 0, NULL);
        field_0x02d0[i] = new CPaneMgr(mCpSel.Scr, l_tagName003[i], 0, NULL);
        field_0x02d8[i] = new CPaneMgr(mCpSel.Scr, l_tagName004[i], 0, NULL);

        field_0x02c0[i]->getPanePtr()->setAnimation(field_0x02e8);
        field_0x02c8[i]->getPanePtr()->setAnimation(field_0x02e8);
        field_0x02d0[i]->getPanePtr()->setAnimation(field_0x02e8);
        field_0x02c8[i]->getPanePtr()->setAnimation(field_0x02f0);
        field_0x02d0[i]->getPanePtr()->setAnimation(field_0x02f0);
        field_0x02d8[i]->getPanePtr()->setAnimation(field_0x02f8);
        field_0x02d8[i]->getPanePtr()->setAnimation(field_0x0300);
        field_0x02d8[i]->getPanePtr()->setAnimation(field_0x0308);

        mCpSel.Scr->search(l_tagName005[i])->hide();
    }
    J2DPane* namePane = mCpSel.Scr->search('name_n');
    namePane->setAnimation(field_0x029c);
    field_0x029c->setFrame(0.0f);
    namePane->animationTransform();
    namePane->setAnimation((J2DAnmTransform*)NULL);
    mSelIcon2 = new dSelect_cursor_c(0, 1.0f, NULL);
    JUT_ASSERT(5406, mSelIcon2 != NULL);
    mSelIcon2->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
    Vec local_24 = field_0x02a4[0]->getGlobalVtxCenter(false, 0);
    mSelIcon2->setPos(local_24.x, local_24.y, field_0x02a4[0]->getPanePtr(), true);
    mSelIcon2->setAlphaRate(0.0f);
    for (int i = 0; i < 2; i++) {
        mpFileInfo[i] = new dFile_info_c(mpArchive, 0);
        mpFileInfo[i]->setBasePane(mCpSel.Scr->search(l_tagName13[i + 1]));
    }
    mCpSel.field_0x08 = false;
}

void dFile_select_c::screenSetYesNo() {
    static u64 l_tagName012[2] = {'w_no_n', 'w_yes_n'};
    static u64 l_tagName013[2] = {'w_no_t', 'w_yes_t'};
    static u64 l_tagName013U[2] = {'f_no_t', 'f_yes_t'};
    static u64 l_tagName9[2] = {'w_no_m', 'w_yes_m'};
    static u64 l_tagName10[2] = {'w_no_g', 'w_yes_g',};
    static u64 l_tagName11[2] = {'w_no_gr', 'w_yes_gr'};
    static u8 l_msgNum2[2] = {0x08, 0x07};

    mYnSel.ScrYn = new J2DScreen();
    JUT_ASSERT(5435, mYnSel.ScrYn != NULL);
    mYnSel.ScrYn->setPriority("zelda_file_select_yes_no_window.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(mYnSel.ScrYn);
    void* bck = JKRGetNameResource("zelda_file_select_yes_no_window.bck", mpArchive);
    field_0x0310 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0314 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x008c = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0310->searchUpdateMaterialID(mYnSel.ScrYn);
    field_0x0314->searchUpdateMaterialID(mYnSel.ScrYn);
    field_0x008c->searchUpdateMaterialID(mYnSel.ScrYn);
    for (int i = 0; i < 2; i++) {
        field_0x00f0[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName012[i], 0, NULL);
#if VERSION == VERSION_GCN_JPN
        field_0x01c0[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName013[i], 0, NULL);
        mYnSel.ScrYn->search(l_tagName013U[i])->hide();
#else
        field_0x01c0[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName013U[i], 0, NULL);
        mYnSel.ScrYn->search(l_tagName013[i])->hide();
#endif
        ((J2DTextBox*)field_0x01c0[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        char acStack_30[16];
        fopMsgM_messageGet(acStack_30, l_msgNum2[i]);
        ((J2DTextBox*)field_0x01c0[i]->getPanePtr())->setString(acStack_30);
    }
    void* bpk = JKRGetNameResource("zelda_file_select_yes_no_window.bpk", mpArchive);
    JUT_ASSERT(5483, bpk != NULL);
    field_0x0318 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    field_0x0318->searchUpdateMaterialID(mYnSel.ScrYn);
    field_0x031c = 0;
    void* btk = JKRGetNameResource("zelda_file_select_yes_no_window.btk", mpArchive);
    JUT_ASSERT(5493, btk != NULL);
    field_0x0320 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    field_0x0320->searchUpdateMaterialID(mYnSel.ScrYn);
    field_0x0324 = 0;
    for (int i = 0; i < 2; i++) {
        field_0x01a0[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName9[i], 0, NULL);
        field_0x01a8[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName10[i], 0, NULL);
        field_0x01b0[i] = new CPaneMgr(mYnSel.ScrYn, l_tagName11[i], 0, NULL);

        field_0x01a0[i]->getPanePtr()->setAnimation(field_0x0318);
        field_0x01a8[i]->getPanePtr()->setAnimation(field_0x0318);
        field_0x01b0[i]->getPanePtr()->setAnimation(field_0x0318);
        field_0x01a8[i]->getPanePtr()->setAnimation(field_0x0320);
        field_0x01b0[i]->getPanePtr()->setAnimation(field_0x0320);
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
    field_0x0328 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x032c = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(bck);
    field_0x0328->searchUpdateMaterialID(m3mSel.Scr3m);
    field_0x032c->searchUpdateMaterialID(m3mSel.Scr3m);
    void* bpk = JKRGetNameResource("zelda_file_select_3menu_window.bpk", mpArchive);
    JUT_ASSERT(5552, bpk != NULL);
    field_0x0330 = (J2DAnmColor*)J2DAnmLoaderDataBase::load(bpk);
    field_0x0330->searchUpdateMaterialID(m3mSel.Scr3m);
    field_0x0334 = 0;
    void* btk = JKRGetNameResource("zelda_file_select_3menu_window.btk", mpArchive);
    JUT_ASSERT(5562, btk != NULL);
    field_0x0338 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    field_0x0338->searchUpdateMaterialID(m3mSel.Scr3m);
    field_0x033c = 0;
    field_0x0118 = m3mSel.Scr3m->search('wmenu_n');
    field_0x0118->setAnimation(field_0x0328);
    field_0x0328->setFrame(799.0f);
    field_0x0118->animationTransform();
    for (int i = 0; i < 3; i++) {
        field_0x0340[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName1[i], 0, NULL);
#if VERSION == VERSION_GCN_JPN
        mpPaneMgr2[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName011[i], 0, NULL);
        m3mSel.Scr3m->search(l_tagName011U[i])->hide();
#else
        mpPaneMgr2[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName011U[i], 0, NULL);
        m3mSel.Scr3m->search(l_tagName011[i])->hide();
#endif

        ((J2DTextBox*)mpPaneMgr2[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        char acStack_30[32];
        fopMsgM_messageGet(acStack_30, l_msgNum[i]);
        ((J2DTextBox*)mpPaneMgr2[i]->getPanePtr())->setString(acStack_30);

        mpPaneAlpha1[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName6[i], 0, NULL);
        mpPaneAlpha2[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName7[i], 0, NULL);
        mpPaneAlpha3[i] = new CPaneMgr(m3mSel.Scr3m, l_tagName8[i], 0, NULL);

        mpPaneAlpha1[i]->getPanePtr()->setAnimation(field_0x0330);
        mpPaneAlpha2[i]->getPanePtr()->setAnimation(field_0x0330);
        mpPaneAlpha3[i]->getPanePtr()->setAnimation(field_0x0330);
        mpPaneAlpha2[i]->getPanePtr()->setAnimation(field_0x0338);
        mpPaneAlpha3[i]->getPanePtr()->setAnimation(field_0x0338);
    }
}

void dFile_select_c::screenSetDetail() {
    mSelDt.ScrDt = new J2DScreen();
    JUT_ASSERT(5622, mSelDt.ScrDt != NULL);
    mSelDt.ScrDt->setPriority("zelda_file_select_details.blo", 0x1100000, mpArchive);
    void* btk = JKRGetNameResource("zelda_file_select_details.btk", mpArchive);
    JUT_ASSERT(5628, btk != NULL);
    field_0x03a0 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mSelDt.ScrDt->setAnimation(field_0x03a0);
    field_0x03a4 = 0;
    mSelDt.mpPane2 = mSelDt.ScrDt->search('n_all');
    field_0x03a8 = new CPaneMgr(mSelDt.ScrDt, 'mset_p_n', 0, NULL);
    mpFileSelect3d->setBasePane(field_0x03a8);
    mpFileSelect3d->setBase2Pane(fileSel.Scr->search('w_sub_n'));
    mSelDt.ScrDt->search('d_win_n')->setUserInfo('n_43');
}

void dFile_select_c::setWakuAnm() {
    field_0x0098->setFrame(3000.0f);
    for (int i = 0; i < 3; i++) {
        field_0x0164[i]->getPanePtr()->setAnimation(field_0x0098);
        field_0x0170[i]->getPanePtr()->setAnimation(field_0x0098);
        field_0x017c[i]->getPanePtr()->setAnimation(field_0x0098);
        field_0x0164[i]->getPanePtr()->animationTransform();
        field_0x0170[i]->getPanePtr()->animationTransform();
        field_0x017c[i]->getPanePtr()->animationTransform();
        field_0x0164[i]->setAlpha(0);
        field_0x0170[i]->setAlpha(0);
        field_0x017c[i]->setAlpha(0);
    }
}

void dFile_select_c::displayInit() {
    field_0x03b1 = 0;
    mSelectEnd = false;
    field_0x03ac = g_fsHIO.field_0x000b;
    field_0x00b8 = 0;
    field_0x014a = false;
    field_0x0360 = false;
    field_0x0108 = false;
    field_0x021e = 0;
    field_0x00b9 = 0;
    field_0x014b = false;
    field_0x0281 = false;
    field_0x0283 = false;
    mDoMemCd_clearProbeStat();
    field_0x0248 = false;
    field_0x024a = false;
    field_0x0249 = false;
    field_0x024b = false;
    field_0x0240->setAlpha(0);
    field_0x0244->setAlpha(0);
    field_0x0238->setAlpha(0x80);
    field_0x023c->setAlpha(0x80);
    field_0x026f = 0;
    field_0x0271 = 0;
}

void dFile_select_c::setSaveData() {
    dSv_save_c* pSave = (dSv_save_c*)mSave;
    for (int i = 0; i < 3; i++) {
        int res = field_0x0070[i]->setSaveData(pSave, mDoMemCdRWm_TestCheckSumGameData(pSave), i);
        if (res == -1) {
            field_0x025b[i] = 1;
            mDataNew[i] = 0;
        } else {
            mDataNew[i] = res;
            field_0x025b[i] = 0;
        }
        pSave = (dSv_save_c*)((u8*)pSave + 0xa94);
    }
}

void dFile_select_c::headerTxtSet(u16 param_1, u8 param_2, u8 param_3) {
    static f32 fontsize[2] = {21.0f, 27.0f};
#if VERSION == VERSION_GCN_JPN
    static f32 linespace[2] = {22.0f, 20.0f};
    static f32 charspace[2] = {2.0f, 3.0f};
#else
    static f32 linespace[2] = {21.0f, 20.0f};
    static f32 charspace[2] = {0.0f, 0.0f};
#endif

    u8 uVar1 = field_0x021c ^ 1;
    if (param_3 != 0) {
        uVar1 = field_0x021c;
    }
    if (param_1 == 0xffff) {
        strcpy(field_0x0214[uVar1], "");
    } else {
        ((J2DTextBox*)field_0x020c[uVar1]->getPanePtr())->setFont(fileSel.mpMessageFont[param_2]);
        ((J2DTextBox*)field_0x020c[uVar1]->getPanePtr())->setFontSize(fontsize[param_2], fontsize[param_2]);
        ((J2DTextBox*)field_0x020c[uVar1]->getPanePtr())->setLineSpace(linespace[param_2]);
        ((J2DTextBox*)field_0x020c[uVar1]->getPanePtr())->setCharSpace(charspace[param_2]);
        fileSel.mpMessageString->getString(param_1, ((J2DTextBox*)field_0x020c[uVar1]->getPanePtr()), NULL, fileSel.mpMessageFont[param_2], NULL, 0);
    }
    if (param_3 == 0) {
        field_0x020c[field_0x021c]->alphaAnimeStart(0);
        field_0x020c[field_0x021c ^ 1]->alphaAnimeStart(0);
        field_0x021d = 0;
    }
}

bool dFile_select_c::headerTxtChangeAnm() {
    if (field_0x021d != 0) {
        return true;
    } else {
        bool ret = false;

        bool alphaAnime1 = field_0x020c[field_0x021c]->alphaAnime(g_fsHIO.field_0x0007, 0xFF, 0, 0);
        bool alphaAnime2 =
            field_0x020c[field_0x021c ^ 1]->alphaAnime(g_fsHIO.field_0x0007, 0, 0xFF, 0);
        int msgKeyWaitTimer = dMeter2Info_getMsgKeyWaitTimer();

        if (alphaAnime1 == true && alphaAnime2 == true && msgKeyWaitTimer == 0) {
            field_0x021c ^= 1;
            field_0x021d = 1;
            ret = true;
        }

        return ret;
    }
}

void dFile_select_c::modoruTxtChange(u8 param_1) {
    if (param_1 != 0) {
        fopMsgM_messageGet(field_0x0250, 981);
        field_0x024c = 1;
    } else {
        fopMsgM_messageGet(field_0x0250, 84);
        field_0x024c = 0;
    }
}

void dFile_select_c::modoruTxtDispAnmInit(u8 param_1) {
    field_0x024a = param_1;
    if ((param_1 != 0 || field_0x0240->getAlpha() != 0) &&
        (param_1 != 1 || field_0x0240->getAlpha() != 0xff))
    {
        field_0x0240->alphaAnimeStart(0);
        field_0x0238->alphaAnimeStart(0);
    } else {
        field_0x0240->alphaAnimeStart(g_fsHIO.field_0x0007);
        field_0x0238->alphaAnimeStart(g_fsHIO.field_0x0007);
    }
}

bool dFile_select_c::modoruTxtDispAnm() {
    bool iVar1;
    bool iVar2;
    if (field_0x024a != 0) {
        iVar1 = field_0x0240->alphaAnime(g_fsHIO.field_0x0007, 0, 0xff, 0);
        iVar2 = field_0x0238->alphaAnime(g_fsHIO.field_0x0007, 0x80, 0xff, 0);
    } else {
        iVar1 = field_0x0240->alphaAnime(g_fsHIO.field_0x0007, 0xff, 0, 0);
        iVar2 = field_0x0238->alphaAnime(g_fsHIO.field_0x0007, 0xff, 0x80, 0);
    }

    if (iVar1 == true && iVar2 == true) {
        return true;
    } 
    return false;
}

void dFile_select_c::ketteiTxtDispAnmInit(u8 param_1) {
    field_0x024b = param_1;
    if ((param_1 != 0 || field_0x0244->getAlpha() != 0) &&
        (param_1 != 1 || field_0x0244->getAlpha() != 0xff))
    {
        field_0x0244->alphaAnimeStart(0);
        field_0x023c->alphaAnimeStart(0);
    } else {
        field_0x0244->alphaAnimeStart(g_fsHIO.field_0x0007);
        field_0x023c->alphaAnimeStart(g_fsHIO.field_0x0007);
    }
}

bool dFile_select_c::ketteiTxtDispAnm() {
    bool iVar1;
    bool iVar2;
    if (field_0x024b != 0) {
        iVar1 = field_0x0244->alphaAnime(g_fsHIO.field_0x0007, 0, 0xff, 0);
        iVar2 = field_0x023c->alphaAnime(g_fsHIO.field_0x0007, 0x80, 0xff, 0);
    } else {
        iVar1 = field_0x0244->alphaAnime(g_fsHIO.field_0x0007, 0xff, 0, 0);
        iVar2 = field_0x023c->alphaAnime(g_fsHIO.field_0x0007, 0xff, 0x80, 0);
    }

    if (iVar1 == true && iVar2 == true) {
        return true;
    } 
    return false;
}

void dFile_select_c::selectWakuAlpahAnmInit(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    field_0x0164[param_1]->alphaAnimeStart(0);
    field_0x0170[param_1]->alphaAnimeStart(0);
    field_0x017c[param_1]->alphaAnimeStart(0);
    field_0x0194[param_1] = param_2;
    field_0x0197[param_1] = param_3;
    field_0x019a[param_1] = param_4;
}

bool dFile_select_c::selectWakuAlpahAnm(u8 param_1) {
    if (param_1 == 0xff) {
        return true;
    }
    bool rv = false;
    bool iVar2 = field_0x0164[param_1]->alphaAnime(field_0x019a[param_1], field_0x0194[param_1], field_0x0197[param_1], 0);
    bool iVar3 = field_0x0170[param_1]->alphaAnime(field_0x019a[param_1], field_0x0194[param_1], field_0x0197[param_1], 0);
    bool iVar4 = field_0x017c[param_1]->alphaAnime(field_0x019a[param_1], field_0x0194[param_1], field_0x0197[param_1], 0);
    if (iVar2 == true && iVar3 == true && iVar4 == true) {
        rv = true;
    }
    return rv;
}

void dFile_select_c::selFileCursorShow() {
    field_0x0164[mSelectNum]->setAlpha(0xff);
    field_0x0170[mSelectNum]->setAlpha(0xff);
    field_0x017c[mSelectNum]->setAlpha(0xff);

    Vec local_1c = field_0x00bc[mSelectNum]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(local_1c.x, local_1c.y, field_0x00bc[mSelectNum]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
    mSelIcon->setParam(0.96f, 0.94f, 0.03f, 0.7f, 0.7f);
}

void dFile_select_c::menuWakuAlpahAnmInit(u8 i_idx, u8 param_1, u8 param_2, u8 param_3) {
    mpPaneAlpha1[i_idx]->alphaAnimeStart(0);
    mpPaneAlpha2[i_idx]->alphaAnimeStart(0);
    mpPaneAlpha3[i_idx]->alphaAnimeStart(0);
    field_0x0388[i_idx] = param_1;
    field_0x038b[i_idx] = param_2;
    field_0x038e[i_idx] = param_3;
    mpPaneMgr2[i_idx]->colorAnimeStart(0);
}

bool dFile_select_c::menuWakuAlpahAnm(u8 param_1) {
    bool rv = false;
    bool iVar1 = mpPaneAlpha1[param_1]->alphaAnime(field_0x038e[param_1], field_0x0388[param_1],
                                                   field_0x038b[param_1], 0);
    bool iVar2 = mpPaneAlpha2[param_1]->alphaAnime(field_0x038e[param_1], field_0x0388[param_1],
                                                   field_0x038b[param_1], 0);
    bool iVar3 = mpPaneAlpha3[param_1]->alphaAnime(field_0x038e[param_1], field_0x0388[param_1],
                                                   field_0x038b[param_1], 0);
    mpPaneMgr2[param_1]->colorAnime(field_0x038e[param_1], mpPaneMgr2[param_1]->getInitBlack(),
                                    mpPaneMgr2[param_1]->getInitBlack(),
                                    JUtility::TColor(0xff, 0xff, 0xff, 0xff),
                                    JUtility::TColor(0x96, 0x96, 0x96, 0xff), 0);
    if (((iVar1 == 1) && (iVar2 == 1)) && (iVar3 == 1)) {
        rv = true;
    }
    return rv;
}

void dFile_select_c::menuCursorShow() {
    if (mSelectMenuNum == 0xff) {
        return;
    }
    ((J2DTextBox*)mpPaneMgr2[mSelectMenuNum]->getPanePtr())
        ->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
    mpPaneAlpha1[mSelectMenuNum]->setAlpha(0xff);
    mpPaneAlpha2[mSelectMenuNum]->setAlpha(0xff);
    mpPaneAlpha3[mSelectMenuNum]->setAlpha(0xff);
    Vec local_24 = field_0x0340[mSelectMenuNum]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(local_24.x, local_24.y, field_0x0340[mSelectMenuNum]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
    mSelIcon->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
}


void dFile_select_c::yesnoWakuAlpahAnmInit(u8 param_1, u8 param_2, u8 param_3, u8 param_4) {
    if (param_1 != 0xff) {
        field_0x01a0[param_1]->alphaAnimeStart(0);
        field_0x01a8[param_1]->alphaAnimeStart(0);
        field_0x01b0[param_1]->alphaAnimeStart(0);
        field_0x01b8[param_1] = param_2;
        field_0x01ba[param_1] = param_3;
        field_0x01bc[param_1] = param_4;
        field_0x01c0[param_1]->colorAnimeStart(0);
    }
}

bool dFile_select_c::yesnoWakuAlpahAnm(u8 param_1) {
    bool rv = false;
    bool iVar5 = true;
    bool iVar4 = true;
    bool iVar3 = true;
    bool iVar2 = true;
    if (param_1 != 0xff) {
        iVar5 = field_0x01a0[param_1]->alphaAnime(field_0x01bc[param_1], field_0x01b8[param_1],
                                                  field_0x01ba[param_1], 0);
        iVar4 = field_0x01a8[param_1]->alphaAnime(field_0x01bc[param_1], field_0x01b8[param_1],
                                                  field_0x01ba[param_1], 0);
        iVar3 = field_0x01b0[param_1]->alphaAnime(field_0x01bc[param_1], field_0x01b8[param_1],
                                                  field_0x01ba[param_1], 0);
        iVar2 = field_0x01c0[param_1]->colorAnime(
            field_0x01bc[param_1], field_0x01c0[param_1]->getInitBlack(),
            field_0x01c0[param_1]->getInitBlack(), JUtility::TColor(0xff, 0xff, 0xff, 0xff),
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
            field_0x0070[i]->draw();
        }

        dComIfGd_set2DOpa(&mSelDt);
        mpFileSelect3d->draw();

        if (mCpSel.field_0x08 != false) {
            dComIfGd_set2DOpa(&mCpSel);

            for (int i = 0; i < 2; i++) {
                mpFileInfo[i]->draw();
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
        field_0x2378->draw(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                           mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(),
                           false, false, false);
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
    MTXTrans(auStack_60, mpPane->getWidth() / 2, mpPane->getHeight() / 2,
                     0.0f);
    MTXConcat(local_98, auStack_60, local_98);
    MTXScale(auStack_90, mpPane->getWidth() / mpPane2->getWidth(), mpPane->getHeight() / mpPane2->getHeight(), 1.0f);
    MTXConcat(local_98, auStack_90, local_98);
    mpPane2->setMtx(local_98);
    ScrDt->draw(0.0f, 0.0f, grafContext);
}

void dDlst_FileSelCp_c::draw() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    MtxP local_98 = (MtxP)&mpPane1->getGlbMtx()[0][0];
    Mtx auStack_90;
    MTXScale(auStack_90, mpPane1->getWidth() / mpPane2->getWidth(), mpPane1->getHeight() / mpPane2->getHeight(), 1.0f);
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
    field_0x012c->setAnimation(field_0x0090);
    field_0x0130 = param_1;
    field_0x0134 = param_2;
    field_0x0090->setFrame(field_0x0130);
    field_0x012c->animationTransform();
    field_0x014b = true;
}

bool dFile_select_c::errorMoveAnm() {
    if (field_0x0130 != field_0x0134) {
        if (field_0x0130 < field_0x0134) {
            field_0x0130 +=2;

            if (field_0x0130 > field_0x0134)
                field_0x0130 = field_0x0134;
        } else {
            field_0x0130 -= 2;

            if (field_0x0130 < field_0x0134)
                field_0x0130 = field_0x0134;
        }

        field_0x0090->setFrame(field_0x0130);
        field_0x012c->animationTransform();
        return false;
    } else {
        field_0x012c->setAnimation((J2DAnmTransform*)0);
        field_0x0134 == 0xb2b ? field_0x014a = true : field_0x014a = false;
        field_0x014b = false;
        return true;
    }
}

void dFile_select_c::errDispInitSet(int param_1, int param_2) {
    if (param_2 != 0) {
        headerTxtSet(0xffff, 0, 0);
    } else {
        headerTxtSet(1, 0, 0);
    }
    field_0x021e = 0;
    field_0x0138[field_0x0148]->setAlpha(0xff);
    field_0x0138[field_0x0148 ^ 1]->setAlpha(0);
    fileSel.mpMessageString->getString(param_1, (J2DTextBox*)field_0x0138[field_0x0148]->getPanePtr(), NULL,
                                       fileSel.mpMessageFont[0], NULL, 0);
    if (field_0x014a || field_0x014b) {
        errorMoveAnmInitSet(2859, 2849);
    }
    if (field_0x00b8 != 0 || field_0x00b9 != 0) {
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
    if (mCpSel.field_0x08) {
        copySelMoveAnmInitSet(3369, 3359);
    }
    modoruTxtDispAnmInit(0);
    ketteiTxtDispAnmInit(0);
    mpFileWarning->init();
    mSelIcon->setAlphaRate(0.0f);
    mSelIcon2->setAlphaRate(0.0f);
    field_0x0271 = 22;
}

typedef void (dFile_select_c::*MemCardCheckFuncT)();
static MemCardCheckFuncT MemCardCheckProc[27] = {
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
};

void dFile_select_c::MemCardCheckMain() {
    (this->*MemCardCheckProc[field_0x0271])();
}

void dFile_select_c::MemCardStatCheck() {
    u32 status = mDoMemCd_getStatus(0);
    if (status == 14) {
        return;
    }
    switch(status) {
    case 0:
        errDispInitSet(2, 0);
        field_0x0280 = false;
        field_0x0284 = NULL;
        field_0x0273 = 2;
        field_0x0290 = &dFile_select_c::noSaveSelDispInit;
        field_0x0274 = 3;
        break;
    case 8:
        errDispInitSet(3, 0);
        field_0x0280 = false;
        field_0x0284 = NULL;
        field_0x0273 = 2;
        field_0x0290 = &dFile_select_c::noSaveSelDispInit;
        field_0x0274 = 3;
        break;
    case 9:
        errDispInitSet(4, 0);
        field_0x0280 = false;
        field_0x0284 = NULL;
        field_0x0273 = 2;
        field_0x0290 = &dFile_select_c::noSaveSelDispInit;
        field_0x0274 = 3;
        break;
    case 10:
        errDispInitSet(5, 0);
        field_0x0280 = false;
        field_0x0284 = NULL;
        field_0x0273 = 2;
        field_0x0290 = &dFile_select_c::noSaveSelDispInit;
        field_0x0274 = 3;
        break;
    case 6:
    case 7:
        errDispInitSet(6, 0);
        field_0x0280 = true;
        field_0x0273 = 5;
        break;
    case 11:
    case 12:
        errDispInitSet(9, 0);
        field_0x0280 = false;
        field_0x0284 = NULL;
        field_0x0273 = 2;
        field_0x0290 = &dFile_select_c::noFileSpaceDispInit;
        field_0x0274 = 18;
        break;
    case 2:
        mDoMemCd_Load();
        field_0x0271 = 1;
        break;
    case 1:
        errDispInitSet(22, 0);
        field_0x0280 = true;
        field_0x0273 = 13;
        break;
    }
}

void dFile_select_c::MemCardLoadWait() {
    int loadRes = mDoMemCd_LoadSync(mSave, sizeof(mSave), 0);
    if (loadRes == 0) {
        return;
    }
    if (loadRes == 1) {
        if (dComIfGs_getNoFile() != 0) {
            dComIfGs_setNoFile(0);
        }
        dataSelectInAnmSet();
        if (field_0x014a || field_0x014b) {
            errorMoveAnmInitSet(2859, 2849);
        }
        if (field_0x0108 || field_0x0281) {
            yesnoMenuMoveAnmInitSet(1149, 1139);
        }
        if (field_0x0128) {
            nameMoveAnmInitSet(3369, 3359);
        }
        if (mCpSel.field_0x08) {
            mSelIcon2->setAlphaRate(0.0f);
            copySelMoveAnmInitSet(3369, 3359);
        }

        if (field_0x0360 || field_0x0283) {
            menuMoveAnmInitSet(809, 799);
        }
        modoruTxtDispAnmInit(0);
        headerTxtSet(0xffff, 0, 0);
        field_0x021e = 0;
        field_0x026f = 1;
    } else if (loadRes == 2) {
        field_0x0271 = 0;
    }
}

void dFile_select_c::MemCardErrMsgWaitKey() {
    if (cAPICPad_ANY_BUTTON(0) != 0 && dMeter2Info_getMsgKeyWaitTimer() == 0) {
        if (field_0x0290 != NULL) {
            (this->*field_0x0290)();
        }
        field_0x0271 = field_0x0274;
    }
}

void dFile_select_c::noFileSpaceDispInit() {
    errorTxtSet(10);
}

void dFile_select_c::MemCardNoFileSpaceDisp() {
    if (errorTxtChangeAnm() == true) {
        field_0x0290 = &dFile_select_c::iplSelDispInit;
        field_0x0274 = 19;
        field_0x0271 = 2;
    }
}

void dFile_select_c::iplSelDispInit() {
    errorTxtSet(18);
    ketteiTxtDispAnmInit(1);
    field_0x0280 = true;
    yesnoMenuMoveAnmInitSet(0x473, 0x47d);
}

void dFile_select_c::MemCardGotoIPLSelectDisp() {
    bool txhChanged = errorTxtChangeAnm();
    bool yesNoMenuMoved = yesnoMenuMoveAnm();
    bool ketteiTxtDisplayed = ketteiTxtDispAnm();
    if (txhChanged == true && yesNoMenuMoved == true && ketteiTxtDisplayed == true) {
        yesnoCursorShow();
        field_0x0271 = 20;
    }
}

void dFile_select_c::MemCardGotoIPLSelect() {
    if (errYesNoSelect()) {
        if (field_0x0268) {
            field_0x03b1 = 1;
            field_0x03ac = g_fsHIO.field_0x000d;
            mDoGph_gInf_c::startFadeOut(field_0x03ac);
            mFadeFlag = true;
            field_0x0271 = 21;
        } else {
            yesnoWakuAlpahAnmInit(field_0x0268, 0xff, 0, g_fsHIO.field_0x0008);
            noSaveSelDispInit();
            field_0x0271 = 3;
        }
    }
}

void dFile_select_c::MemCardGotoIPL() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    } else {
        mHasDrawn = true;
        mDoGph_gInf_c::startFadeIn(0);
        mFadeFlag = false;
        mDoRst::onShutdown();
    }
}

void dFile_select_c::noSaveSelDispInit() {
    errorTxtSet(19);
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
        field_0x0271 = 4;
    }
}

void dFile_select_c::MemCardErrMsgWaitNoSaveSel() {
    if (!errYesNoSelect()) {
        return;
    }

    if (field_0x0268 != 0) {
        setInitSaveData();
        dComIfGs_setCardToMemory((char*)mSave, 0);
        dComIfGs_setNoFile(1);
        dComIfGs_setDataNum(0);
        for (int i = 0; i < 3; i++) {
            mDataNew[i] = 1;
        }
        headerTxtSet(901, 1, 0);
        mSelIcon->setAlphaRate(1.0f);
        char auStack_38[32];
        dMeter2Info_getString(0x382, auStack_38, 0);
        dComIfGs_setPlayerName(auStack_38);
        dMeter2Info_getString(899, auStack_38, 0);
        dComIfGs_setHorseName(auStack_38);
        mpName->setNextNameStr(dComIfGs_getPlayerName());
        mpName->initial();
        modoruTxtChange(1);
        nameMoveAnmInitSet(3359, 3369);
        yesnoMenuMoveAnmInitSet(1149, 1139);
        errorMoveAnmInitSet(2859, 2849);
        modoruTxtDispAnmInit(1);
        ketteiTxtDispAnmInit(1);
        field_0x026f = 45;
    } else {
        field_0x0284 = NULL;
        field_0x0273 = 0;
        ketteiTxtDispAnmInit(0);
        errorMoveAnmInitSet(2859, 2849);
        yesnoMenuMoveAnmInitSet(1149, 1139);
        field_0x0271 = 24;
    }
}

void dFile_select_c::formatYesSelDispInitSet() {
    errorTxtSet(14);
    field_0x0268 = 0;
    field_0x0269 = 1;
    yesnoSelectAnmSet();
    field_0x0271 = 6;
}

void dFile_select_c::formatNoSelDispInitSet() {
    errorTxtSet(17);
    field_0x0280 = false;
    yesnoMenuMoveAnmInitSet(0x47d, 0x473);
    field_0x0271 = 7;
}

void dFile_select_c::MemCardFormatYesSelDisp() {
    bool iVar1 = errorTxtChangeAnm();
    bool iVar2 = yesnoSelectMoveAnm();
    bool iVar3 = yesnoWakuAlpahAnm(field_0x0269);
    if (iVar1 == true && iVar2 == true && iVar3 == true) {
        yesnoCursorShow();
        field_0x0271 = 8;
    }
}

void dFile_select_c::MemCardFormatNoSelDisp() {
    bool iVar1 = errorTxtChangeAnm();
    bool iVar2 = yesnoMenuMoveAnm();
    if (iVar1 == true && iVar2 == true) {
        field_0x0284 = NULL;
        field_0x0290 = &dFile_select_c::noSaveSelDispInit;
        field_0x0274 = 3;
        field_0x0271 = 2;
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
    field_0x0271 = 9;
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
    bool iVar1 = errorTxtChangeAnm();
    bool iVar2 = yesnoMenuMoveAnm();
    if (iVar1 == true && iVar2 == true) {
        field_0x03ac = g_fsHIO.field_0x000c;
        mDoMemCd_Format();
        field_0x0271 = 10;
    }
}

void dFile_select_c::MemCardFormat() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    }
    field_0x03b4 = mDoMemCd_FormatSync();
    if (field_0x03b4 != 0) {
        field_0x0271 = 11;
    }
}

void dFile_select_c::MemCardFormatWait() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    } else {
        if (field_0x03b4 == 2) {
            errorTxtSet(11);
        } else if (field_0x03b4 == 1) {
            errorTxtSet(12);
        }
        field_0x0271 = 12;
    }
}

void dFile_select_c::MemCardFormatCheck() {
    if (errorTxtChangeAnm() == true) {
        field_0x0284 = NULL;
        field_0x0290 = NULL;
        field_0x0273 = 0;
        field_0x0274 = 24;
        field_0x0271 = 2;
        field_0x03b1 = 0;
    }
}

void dFile_select_c::MemCardMakeGameFileSel() {
    if (errYesNoSelect()) {
        if (field_0x0268 != 0) {
            errorTxtSet(27);
            field_0x03b1 = 1;
        } else {
            errorTxtSet(25);
        }
        ketteiTxtDispAnmInit(0);
        yesnoMenuMoveAnmInitSet(0x47d, 0x473);
        field_0x0271 = 14;
    }
}

void dFile_select_c::MemCardMakeGameFileSelDisp() {
    bool iVar1 = errorTxtChangeAnm();
    bool iVar2 = yesnoMenuMoveAnm();
    bool iVar3 = ketteiTxtDispAnm();
    if (iVar1 == true && iVar2 == true && iVar3 == true) {
        if (field_0x0268 != 0) {
            field_0x03ac = g_fsHIO.field_0x000c;
            setInitSaveData();
            dataSave();
            field_0x0271 = 15;
        } else {
            field_0x0284 = NULL;
            field_0x0290 = &dFile_select_c::noSaveSelDispInit;
            field_0x0274 = 3;
            field_0x0271 = 2;
        }
    }
}

void dFile_select_c::MemCardMakeGameFile() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    }
    field_0x03b4 = mDoMemCd_SaveSync();
    if (field_0x03b4 != 0) {
        field_0x0271 = 16;
    }
}

void dFile_select_c::MemCardMakeGameFileWait() {
    if (field_0x03ac != 0) {
        field_0x03ac--;
    } else {
        if (field_0x03b4 == 1) {
            errorTxtSet(0x1c);
        } else if (field_0x03b4 == 2) {
            errorTxtSet(0x1a);
        }
        field_0x0271 = 17;
    }
}

void dFile_select_c::MemCardMakeGameFileCheck() {
    if (errorTxtChangeAnm() == 1) {
        field_0x0284 = NULL;
        field_0x0290 = NULL;
        field_0x0273 = 0;
        field_0x0274 = 24;
        field_0x0271 = 2;
        field_0x03b1 = 0;
    }
}

void dFile_select_c::MemCardMsgWindowInitOpen() {
    bool iVar1;
    if (field_0x021e == 0x0) {
        iVar1 = headerTxtChangeAnm();
    } else {
        iVar1 = true;
    }
    bool iVar8 = true;
    bool iVar7 = true;
    bool iVar6 = true;
    bool iVar5 = true;
    if (field_0x00b8 || field_0x00b9) {
        iVar8 = selectDataBaseMoveAnm();
    }
    if (field_0x0108 || field_0x0281) {
        iVar7 = yesnoMenuMoveAnm();
    }
    if (field_0x0360 || field_0x0283) {
        iVar6 = menuMoveAnm();
    }
    if (field_0x014a || field_0x014b) {
        iVar5 = errorMoveAnm();
    }
    bool iVar2 = modoruTxtDispAnm();
    bool iVar3 = ketteiTxtDispAnm();
    bool iVar4 = true;
    if (field_0x0128 || mCpSel.field_0x08) {
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
        field_0x0271 = 23;
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
        field_0x0271 = field_0x0273;
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
        if (field_0x0284 != NULL) {
            (this->*field_0x0284)();
        } else {
            field_0x0271 = field_0x0273;
        }
    }
}

bool dFile_select_c::errYesNoSelect() {
    bool rv = false;
    mStick->checkTrigger();
    if (mDoCPd_c::getTrigA(0) != 0) {
        if (field_0x0268 != 0) {
            mDoAud_seStart(Z2SE_SY_CURSOR_OK, 0, 0, 0);
        } else {
            mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, 0, 0, 0);
        }
        mSelIcon->setAlphaRate(0.0f);
        rv = true;
    } else if (mStick->checkRightTrigger()) {
        if (field_0x0268) {
            field_0x0269 = field_0x0268;
            field_0x0268 = field_0x0268 ^ 1;
            errCurMove(0);
        }
    } else if (mStick->checkLeftTrigger()) {
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
    field_0x0272 = field_0x0271;
    field_0x0271 = 25;
}

void dFile_select_c::MemCardErrYesNoCursorMoveAnm() {
    bool moveRes = yesnoSelectMoveAnm();
    bool anmRes = yesnoWakuAlpahAnm(field_0x0269);
    if (moveRes == true && anmRes == true) {
        yesnoCursorShow();
        field_0x0271 = field_0x0272;
    }
}

void dFile_select_c::errorTxtSet(u16 param_1) {
    if (param_1 == 0xffff) {
        strcpy(field_0x0140[field_0x0148 ^ 1], "");
    } else {
        fileSel.mpMessageString
            ->getString(param_1, (J2DTextBox*)field_0x0138[field_0x0148 ^ 1]->getPanePtr(),
                        NULL, fileSel.mpMessageFont[0], NULL, 0);
    }
    field_0x0138[field_0x0148]->alphaAnimeStart(0);
    field_0x0138[field_0x0148 ^ 1]->alphaAnimeStart(0);
    field_0x0149 = 0;
}

bool dFile_select_c::errorTxtChangeAnm() {
    if (field_0x0149) {
        return true;
    }
    bool rv = false;
    bool animeRes1 = field_0x0138[field_0x0148]->alphaAnime(g_fsHIO.field_0x0007, 0xff, 0, 0);
    bool animeRes2 = field_0x0138[field_0x0148 ^ 1]->alphaAnime(g_fsHIO.field_0x0007, 0, 0xff, 0);
    s32 timer = dMeter2Info_getMsgKeyWaitTimer();
    if (animeRes1 == true && animeRes2 == true && timer == 0) {
        field_0x0148 ^= 1;
        field_0x0149 = 1;
        rv = true;
    }

    return rv;
}

bool dFile_select_c::fileRecScaleAnm() {
    return field_0x00bc[mSelectNum]->scaleAnime(g_fsHIO.field_0x0005, field_0x00c8[0],
                                                field_0x00d4[0], 0);
}

void dFile_select_c::fileRecScaleAnmInitSet2(f32 param_1, f32 param_2) {
    for (int i = 0; (int)i < 3; i++) {
        field_0x00c8[i] = param_1;
        field_0x00d4[i] = param_2;
        if (i == mSelectNum) {
            if (param_1 > 0.0f) {
                field_0x00c8[i] = g_fsHIO.field_0x0010[mSelectNum];
            }
            if (param_2 > 0.0f) {
                field_0x00d4[i] = g_fsHIO.field_0x0010[mSelectNum];
            }
        }
        field_0x00bc[i]->scaleAnimeStart(0);
    }
}

bool dFile_select_c::fileRecScaleAnm2() {
    bool scaleRes[3];
    for (int i = 0; i < 3; i++) {
        scaleRes[i] =
            field_0x00bc[i]->scaleAnime(g_fsHIO.field_0x0005, field_0x00c8[i], field_0x00d4[i], 0);
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
        mpPane->animationTransform();
        ret = false;
    }

    if (field_0x0110 == field_0x0114) {
        mpPane->setAnimation((J2DAnmTransform*)0);
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
    field_0x011c->setAnimation(field_0x0094);
    field_0x0120 = param_1;
    field_0x0124 = param_2;
    field_0x0094->setFrame(field_0x0120);
    field_0x011c->animationTransform();
}

bool dFile_select_c::nameMoveAnm() {
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
        field_0x011c->animationTransform();
        return false;
    } else {
        field_0x011c->setAnimation((J2DAnmTransform*)0);

        if (field_0x0124 == 0xd1f) {
            field_0x0128 = false;
            mCpSel.field_0x08 = false;
        }
        return true;
    }
}

void dFile_select_c::MemCardSaveDataClear() {
    field_0x03b4 = mDoMemCd_SaveSync();
    if (field_0x03b4 != 0) {
        field_0x0271 = 0;
    }
}

void dFile_select_c::setInitSaveData() {
    for (int i = 0; i < 3; i++) {
        dComIfGs_setInitDataToCard((u8*)mSave, i);
        mDoMemCdRWm_SetCheckSumGameData((u8*)mSave, i);
    }
}

void dFile_select_c::dataSave() {
    mDoMemCd_save(mSave, sizeof(mSave), 0);
}

dFile_select3D_c::dFile_select3D_c() {
    mpHeap = NULL;
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

    mpHeap = mDoExt_createSolidHeapFromGameToCurrent(&ppHeap,0x25800,32);
    field_0x03c4 = 0.0f;
    field_0x03c8 = 0.0f;
    mMirrorIdx = i_mirrorIdx;
    mMaskIdx = i_maskIdx;
    if (mMirrorIdx != 0) {
        createMirrorModel();
    }

    if (mMaskIdx != 0) {
        createMaskModel();
    }

    mpHeap->adjustSize();
    mDoExt_setCurrentHeap(ppHeap);

    if (mpModel) {
        dKy_tevstr_init(&mTevstr,0xFF,0xFF);
        set_mtx();
    }
}

void dFile_select3D_c::_delete() {
    /* empty function */
}

void dFile_select3D_c::freeHeap() {
    if (mpHeap) {
        mDoExt_destroySolidHeap(mpHeap);
        mpHeap = NULL;
        mpModel = NULL;
    }
}

void dFile_select3D_c::_move() {
    if (mpModel) {
        cXyz stack_20;
        Vec stack_8 = mPaneMgr->getGlobalVtxCenter(false, 0);
        toItem3Dpos(stack_8.x + field_0x03b8.x, stack_8.y + field_0x03b8.y, 720.0f, &stack_20);
        field_0x03a4.set(stack_20);
        field_0x03b0.set(0, 0, 0);
        animePlay();
        set_mtx();
    }
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
        if (mBckAnm == NULL || !mBckAnm->init((J3DAnmTransform*)anmBase, 1, 2, 1.0f, 0, -1, false)) {
            return;
        }
    }

    if (param_2) {
        brkRes = archive->getResource('BRK ', param_2);
        anmBase = (J3DAnmTevRegKey*)J3DAnmLoaderDataBase::load(brkRes);
        JUT_ASSERT(8859, anmBase != NULL);
        ((J3DAnmTevRegKey*)anmBase)->searchUpdateMaterialID(modelData);

        mBrkAnm = new mDoExt_brkAnm();
        if (mBrkAnm == NULL || !mBrkAnm->init(modelData, (J3DAnmTevRegKey*)anmBase, -1, 2, 1.0f, 0, -1)) {
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
    stack_8.x = stack_8.y = stack_8.z = tmp * field_0x03b8.z;
    mDoMtx_stack_c::transS(field_0x03a4.x, field_0x03a4.y, field_0x03a4.z);
    mDoMtx_stack_c::XYZrotM(field_0x03b0.x, field_0x03b0.y, field_0x03b0.z);
    mpModel->setBaseScale(stack_8);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void dFile_select3D_c::animePlay() {
    if (mBrkAnm) {
        field_0x03c4 += 1.0f;
        if (field_0x03c4 >= mBrkAnm->getEndFrame()) {
            field_0x03c4 -= mBrkAnm->getEndFrame();
        }
        mBrkAnm->setFrame(field_0x03c4);
        mBrkAnm->play();
    }
    if (mBckAnm) {
        field_0x03c8 += 1.0f;
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
    param_0 =
        (2.0f * ((param_0 - mDoGph_gInf_c::getMinXF()) / mDoGph_gInf_c::getWidthF()) - 1.0f);
    param_1 = (2.0f * ((param_1 - -100.0f) / 448.0f) - 1.0f);
    calcViewMtx(adStack_98);
    cMtx_inverse(adStack_98, auStack_c8);
    f32 tangent = std::tan(0.39269909262657166);
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
