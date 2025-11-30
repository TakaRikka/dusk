#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_dmap.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_item.h"
#include "d/d_menu_item_explain.h"
#include "d/d_msg_object.h"
#include "d/d_meter_HIO.h"
#include "d/d_meter2_info.h"
#include "d/d_lib.h"
#include "d/d_menu_dmap_map.h"
#include "d/d_msg_string.h"
#include "d/d_meter_haihai.h"
#include "d/d_menu_window.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_graphic.h"

#if (PLATFORM_WII || PLATFORM_SHIELD)
#define POINTER_OPT dComIfGs_getOptPointer()
#else
#define POINTER_OPT 0
#endif

typedef void (dMenu_Dmap_c::*ProcFunc)();

static ProcFunc init_process[5] = {
    &dMenu_Dmap_c::itemInfo_init_proc,
    &dMenu_Dmap_c::mapMode_init_proc,
    &dMenu_Dmap_c::infoModeChange_init_proc,
    &dMenu_Dmap_c::mapModeChange_init_proc,
    &dMenu_Dmap_c::lv5_talk_init_proc,
};

static ProcFunc move_process[5] = {
    &dMenu_Dmap_c::itemInfo_proc,
    &dMenu_Dmap_c::mapMode_proc,
    &dMenu_Dmap_c::infoModeChange_proc,
    &dMenu_Dmap_c::mapModeChange_proc,
    &dMenu_Dmap_c::lv5_talk_proc,
};

static ProcFunc itemInfo_subProcess[5] = {
    &dMenu_Dmap_c::itemSelect,
    &dMenu_Dmap_c::itemSelectAnm,
    &dMenu_Dmap_c::itemInfoOpenAnm,
    &dMenu_Dmap_c::itemInfoWait,
    &dMenu_Dmap_c::itemInfoCloseAnm,
};

static ProcFunc floor_init_process[2] = {
    &dMenu_Dmap_c::floorSelect_init_proc,
    &dMenu_Dmap_c::floorChange_init_proc,
};

static ProcFunc floor_move_process[2] = {
    &dMenu_Dmap_c::floorSelect_proc,
    &dMenu_Dmap_c::floorChange_proc,
};

static ProcFunc map_init_process[3] = {
    &dMenu_Dmap_c::zoomWait_init_proc,
    &dMenu_Dmap_c::zoomIn_init_proc,
    &dMenu_Dmap_c::zoomOut_init_proc,
};

static ProcFunc map_move_process[3] = {
    &dMenu_Dmap_c::zoomWait_proc,
    &dMenu_Dmap_c::zoomIn_proc,
    &dMenu_Dmap_c::zoomOut_proc,
};

dMenu_DmapBg_c::dMenu_DmapBg_c(JKRExpHeap* i_heap, STControl* i_stick) {
    mpHeap = i_heap;
    mpStick = i_stick;
    field_0xd94 = 0.0f;
    field_0xd98 = 0.0f;
    field_0xd9c = 0.0f;
    field_0xda8 = 1.0f;
    field_0xdb4 = 0.0f;
    field_0xdb8 = 0.0f;
    field_0xdd3 = 0xFF;
    field_0xdd4 = 0x9D;
    field_0xdd6 = 0x9D;
    field_0xdd5 = 0xFF;
    field_0xdd7 = 0;
    field_0xdd8 = 1;

    mString = new dMsgString_c();
    JUT_ASSERT(621, mString != NULL);

    mpTalkHeap = JKRCreateExpHeap(0x32000, mpHeap, false);
    JUT_ASSERT(624, mpTalkHeap != NULL);

    mpItemExplain = NULL;

    mpMeterHaihai = new dMeterHaihai_c(2);
    JUT_ASSERT(630, mpMeterHaihai != NULL);

    field_0xdda = 0;

    OS_REPORT("remain ===> %d\n", mpHeap->getTotalFreeSize());

    baseScreenInit();
    mapScreenInit();

    char archive_path[32];
    strcpy(archive_path, "/res/FieldMap/D_MN10.arc");
    char stage_name[8];
    strcpy(stage_name, dComIfGp_getStartStageName());
    archive_path[18] = stage_name[4];
    archive_path[19] = stage_name[5];

    if (dComIfGs_isDungeonItemMap() || dComIfGs_isDungeonItemCompass()) {
        mpArchiveMount = mDoDvdThd_mountArchive_c::create(archive_path, 2, mDoExt_getJ2dHeap());
    } else {
        mpArchiveMount = NULL;
    }

    mpArchive = NULL;
    mpBackTexture = NULL;
    field_0xdbc = 0.0f;

    memset(&field_0xd80, 0, 20);
    buttonIconScreenInit();
    field_0xdd0 = 0;
}

void dMenu_DmapBg_c::mapScreenInit() {
    for (int i = 0; i < 2; i++) {
        mMapScreen[i] = new J2DScreen();
        JUT_ASSERT(689, mMapScreen[i] != NULL);

        bool fg = mMapScreen[i]->setPriority("zelda_dungeon_map_map.blo", 0x20000, dComIfGp_getDmapResArchive());
        JUT_ASSERT(693, fg != false);

        dPaneClass_showNullPane(mMapScreen[i]);
    }
    
    #if (PLATFORM_WII || PLATFORM_SHIELD)
    mpBlack = new CPaneMgrAlpha(mMapScreen[0], 'm_black', 2, NULL);
    JUT_ASSERT(699, mpBlack != NULL);
    mpBlack->setAlphaRate(0.0f);
    #else
    mpBlack = NULL;
    #endif
    
    mMapScreen[0]->search('map_icon')->hide();
    mMapScreen[0]->search('map_aria')->hide();
    mMapScreen[0]->search('n_all')->hide();
    mMapScreen[1]->search('n_all')->hide();
    mMapScreen[0]->search('m_black')->hide();
    mMapScreen[1]->search('bs_00_0')->hide();
    mMapScreen[1]->search('bs_00_1')->hide();
    mMapScreen[1]->search('gold00_0')->hide();
    mMapScreen[1]->search('gold00_1')->hide();
    mMapScreen[1]->search('m_black')->hide();
    mMapScreen[1]->search('center_n')->hide();
    mMapScreen[1]->search('map_ai_n')->setBasePosition(J2DBasePosition_0);
    mMapScreen[0]->search('center_n')->setBasePosition(J2DBasePosition_4);

    OSInitFastCast();
    {
        Mtx m;
        CPaneMgr pane;
        Vec vtx = pane.getGlobalVtx(mMapScreen[1]->search('map_icon'), &m, 0, false, 0);
        field_0xdc0 = vtx.x;
        field_0xdc4 = vtx.y;
    }

    for (int i = 0; i < 2; i++) {
        mpMapRoot[i] = new CPaneMgrAlphaMorf(mMapScreen[i], 'ROOT', 2, NULL);
        JUT_ASSERT(751, mpMapRoot[i] != NULL);
    }

    void* btk0 = JKRGetNameResource("zelda_dungeon_map_map.btk", dComIfGp_getDmapResArchive());
    JUT_ASSERT(766, btk0 != NULL);

    field_0xd28[0] = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk0);
    field_0xd28[0]->searchUpdateMaterialID(mMapScreen[0]);
    field_0xdc8[0] = 0.0f;
    field_0xd28[0]->setFrame(field_0xdc8[0]);

    mMapScreen[0]->animation();
    setGoldAnimation(true);

    mMapPane = (J2DPicture*)mMapScreen[1]->search('map_aria');
    mMapPane->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
    mMapPane->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));

    CPaneMgr pane;
    Mtx m;
    Vec vtx0 = pane.getGlobalVtx(mMapPane, &m, 0, false, 0);
    Vec vtx3 = pane.getGlobalVtx(mMapPane, &m, 3, false, 0);
    mMapWidth = vtx3.x - vtx0.x;
    mMapHeight = vtx3.y - vtx0.y;

    J2DPicture* map_icon_pic = (J2DPicture*)mMapScreen[1]->search('map_icon');
    map_icon_pic->setBasePosition(J2DBasePosition_4);
    map_icon_pic->setCornerColor(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
    map_icon_pic->setAlpha(0);

    initiate(dComIfGp_getDmapResArchive());

    for (int i = 0; i < 2; i++) {
        mpMapSpace[i] = new CPaneMgr(mMapScreen[i], 'mapspace', 0, NULL);
        JUT_ASSERT(817, mpMapSpace[i] != NULL);

        mpMapSpace[i]->paneTrans(0.0f, -15.0f);
    }
}

void dMenu_DmapBg_c::mapScreenAnime() {
    if (0.0f == field_0xd94 && 0.0f == field_0xd98) {
        for (int i = 0; i < 1; i++) {
            field_0xdc8[i] += 0.4f;
            if (field_0xdc8[i] >= field_0xd28[i]->getFrameMax()) {
                field_0xdc8[i] -= field_0xd28[i]->getFrameMax();
            }
            field_0xd28[i]->setFrame(field_0xdc8[i]);
            mMapScreen[i]->animation();
        }
    }
}

void dMenu_DmapBg_c::mapIconScaleSet(u8 param_1) {
    f32 fVar1;
    f32 fVar2;
    if (param_1 != 0) {
        fVar1 = 0.0f;
        fVar2 = 1.0f;
    } else {
        fVar1 = 1.0f;
        fVar2 = 0.0f;
    }

    iconScale(1, fVar1, fVar1, fVar2);
}

void dMenu_DmapBg_c::iconScaleAnmInit(f32 param_0, f32 param_1, u8 param_2) {
    field_0xdac = param_0;
    field_0xdb0 = param_1;
    field_0xdd1 = param_2;
    field_0xdd2 = 0;
}

bool dMenu_DmapBg_c::iconScaleAnm() {
    bool rv = false;
    if (field_0xdd2 > field_0xdd1) {
        return true;
    } 

    f32 dVar7 = fopMsgM_valueIncrease(field_0xdd1, field_0xdd2, 4);
    f32 dVar8;
    f32 dVar1;
    if (field_0xdac > field_0xdb0) {
        dVar8 = 0.05f * dVar7;
        dVar1 = 1.0f - dVar7;
    } else {
        dVar7 = 1.0f - dVar7;
        dVar8 = dVar7;
        dVar1 = 1.0f - dVar7;
    }

    iconScale(1, dVar7, dVar8, dVar1);

    field_0xdd2++;
    if (field_0xdd2 >= field_0xdd1) {
        rv = true;
    }

    return rv;
}

void dMenu_DmapBg_c::buttonIconScreenInit() {
    static u64 const cont_at[5] = {'cont_at', 'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4'};
    static u64 const cont_bt[5] = {'cont_bt', 'cont_bt1', 'cont_bt2', 'cont_bt3', 'cont_bt4'};
    static u64 const font_at[5] = {'font_at', 'font_at1', 'font_at2', 'font_at3', 'font_at4'};
    static u64 const font_bt[5] = {'font_bt', 'font_bt1', 'font_bt2', 'font_bt3', 'font_bt4'};
    static u64 const c_tag[2] = {
        #if VERSION == VERSION_GCN_JPN
        'c_text_s', 'c_text'
        #else
        'f_text_s', 'f_text'
        #endif
    };

    mButtonScreen = new J2DScreen();
    JUT_ASSERT(916, mButtonScreen != NULL);

    bool fg = mButtonScreen->setPriority("zelda_dungeon_map_spot_button.blo", 0x20000, dComIfGp_getDmapResArchive());
    JUT_ASSERT(922, fg != false);
    dPaneClass_showNullPane(mButtonScreen);

    #if (PLATFORM_WII || PLATFORM_SHIELD)
    mDecorateScreen = new J2DScreen();
    JUT_ASSERT(926, mDecorateScreen != NULL);

    fg = mDecorateScreen->setPriority("zelda_dungeon_map_decoration_revo.blo", 0x20000, dComIfGp_getDmapResArchive());
    JUT_ASSERT(930, fg != false);
    dPaneClass_showNullPane(mDecorateScreen);

    mpDecorateRoot = new CPaneMgrAlphaMorf(mDecorateScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(934, mpDecorateRoot != NULL);
    #else
    mDecorateScreen = NULL;
    mpDecorateRoot = NULL;
    mButtonScreen->search('w_spot')->hide();
    mButtonScreen->search('spot00')->hide();
    mButtonScreen->search('spot01')->hide();
    #endif

    mpButtonRoot = new CPaneMgrAlphaMorf(mButtonScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(952, mpButtonRoot != NULL);

    for (int i = 0; i < 2; i++) {
        mpButtonAB[i] = NULL;
        mpButtonText[i] = NULL;
    }

    mpCButton = new CPaneMgrAlpha(mButtonScreen, 'c_btn', 2, NULL);
    JUT_ASSERT(978, mpCButton != NULL);
    mpJButton = NULL;

    for (int i = 0; i < 5; i++) {
        #if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)mButtonScreen->search(cont_at[i]))->setFont(mDoExt_getMesgFont());
        ((J2DTextBox*)mButtonScreen->search(cont_bt[i]))->setFont(mDoExt_getMesgFont());
        ((J2DTextBox*)mButtonScreen->search(cont_at[i]))->setString(32, "");
        ((J2DTextBox*)mButtonScreen->search(cont_bt[i]))->setString(32, "");
        ((J2DTextBox*)mButtonScreen->search(font_at[i]))->hide();
        ((J2DTextBox*)mButtonScreen->search(font_bt[i]))->hide();
        #else
        ((J2DTextBox*)mButtonScreen->search(font_at[i]))->setFont(mDoExt_getMesgFont());
        ((J2DTextBox*)mButtonScreen->search(font_bt[i]))->setFont(mDoExt_getMesgFont());
        ((J2DTextBox*)mButtonScreen->search(font_at[i]))->setString(32, "");
        ((J2DTextBox*)mButtonScreen->search(font_bt[i]))->setString(32, "");
        ((J2DTextBox*)mButtonScreen->search(cont_at[i]))->hide();
        ((J2DTextBox*)mButtonScreen->search(cont_bt[i]))->hide();
        #endif
    }

    field_0xcf4 = new CPaneMgr(mButtonScreen, 'c_n', 2, NULL);
    field_0xcf8 = NULL;

    J2DTextBox* textBox;
    for (int i = 0; i < 2; i++) {
        textBox = ((J2DTextBox*)mButtonScreen->search(c_tag[i]));
        textBox->setFont(mDoExt_getMesgFont());
        textBox->setString(32, "");
    }

    if (field_0xcf4 != NULL) {
        field_0xcf4->show();
    }

    if (field_0xcf8 != NULL) {
        field_0xcf8->paneTrans(160.0f, 0.0f);
    }

    field_0xcf4->paneTrans(321.0f, 0.0f);
}

void dMenu_DmapBg_c::setAButtonString(u32 i_msgNo) {
    static u64 const cont_at[5] = {
        #if VERSION == VERSION_GCN_JPN
        'cont_at', 'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4'
        #else
        'font_at', 'font_at1', 'font_at2', 'font_at3', 'font_at4'
        #endif
    };
    for (int i = 0; i < 5; i++) {
        if (i_msgNo == 0) {
            strcpy(((J2DTextBox*)mButtonScreen->search(cont_at[i]))->getStringPtr(), "");
        } else {
            dMeter2Info_getStringKanji(i_msgNo, ((J2DTextBox*)mButtonScreen->search(cont_at[i]))->getStringPtr(), NULL);
        }
    }
}

void dMenu_DmapBg_c::setBButtonString(u32 i_msgNo) {
    static u64 const cont_bt[5] = {
        #if VERSION == VERSION_GCN_JPN
        'cont_bt', 'cont_bt1', 'cont_bt2', 'cont_bt3', 'cont_bt4'
        #else
        'font_bt', 'font_bt1', 'font_bt2', 'font_bt3', 'font_bt4'
        #endif
    };
    for (int i = 0; i < 5; i++) {
        if (i_msgNo == 0) {
            strcpy(((J2DTextBox*)mButtonScreen->search(cont_bt[i]))->getStringPtr(), "");
        } else {
            dMeter2Info_getStringKanji(i_msgNo, ((J2DTextBox*)mButtonScreen->search(cont_bt[i]))->getStringPtr(), NULL);
        }
    }
}

static f32 player_px;

static f32 player_py;

dMenu_Dmap_c* dMenu_Dmap_c::myclass;

void dMenu_DmapBg_c::setCButtonString(u32 i_msgNo) {
    static u64 const c_tag[2] = {
        #if VERSION == VERSION_GCN_JPN
        'c_text_s', 'c_text'
        #else
        'f_text_s', 'f_text'
        #endif
    };
    int i;

    u32 msgNo;
    if (!dMenu_Dmap_c::myclass->isMapMoveState()) {
        msgNo = 0;
    } else {
        msgNo = dMenu_Dmap_c::myclass->getCMessasgeNum();
    }

    if (msgNo == 0) {
        for (i = 0; i < 2; i++) {
            strcpy(((J2DTextBox*)mButtonScreen->search(c_tag[i]))->getStringPtr(), "");
        }
        mpCButton->setAlphaRate(0.5f);
    } else {
        for (i = 0; i < 2; i++) {
            dMeter2Info_getStringKanji(msgNo, ((J2DTextBox*)mButtonScreen->search(c_tag[i]))->getStringPtr(), NULL);
        }
        mpCButton->setAlphaRate(1.0f);
    }
}

void dMenu_DmapBg_c::setJButtonString(u32 i_msgNo) {
    dMenu_Dmap_c::myclass->isMapMoveState();
}

void dMenu_DmapBg_c::createExplain() {
    if (mpItemExplain == NULL) {
        JKRHeap* prevHeap = mDoExt_setCurrentHeap(mpTalkHeap);
        mpItemExplain = new dMenu_ItemExplain_c(mpTalkHeap, dComIfGp_getDmapResArchive(), mpStick, true);
        JUT_ASSERT(1308, mpItemExplain != NULL);

        mDoExt_setCurrentHeap(prevHeap);
    }
}

void dMenu_DmapBg_c::deleteExplain() {
    if (mpItemExplain != NULL) {
        JKRHeap* prevHeap = mDoExt_setCurrentHeap(mpTalkHeap);
        delete mpItemExplain;
        mpItemExplain = NULL;
        mDoExt_setCurrentHeap(prevHeap);
    }
}

void dMenu_DmapBg_c::baseScreenInit() {
    mBaseScreen = new J2DScreen();
    JUT_ASSERT(1336, mBaseScreen != NULL);

    bool fg = mBaseScreen->setPriority("zelda_dungeon_map_base.blo", 0x20000, dComIfGp_getDmapResArchive());
    JUT_ASSERT(1347, fg != false);
    dPaneClass_showNullPane(mBaseScreen);

    mFloorScreen = new J2DScreen();
    JUT_ASSERT(1351, mFloorScreen != NULL);

    fg = mFloorScreen->setPriority("zelda_dungeon_map_floor_parts.blo", 0x20000,
                                   dComIfGp_getDmapResArchive());
    JUT_ASSERT(1362, fg != false);
    dPaneClass_showNullPane(mFloorScreen);

    mBaseScreen->search('w_btn_n')->hide();

    mpBaseRoot = new CPaneMgrAlphaMorf(mBaseScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(1396, mpBaseRoot != NULL);

    mpFloorRoot = new CPaneMgrAlphaMorf(mFloorScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(1399, mpFloorRoot != NULL);

    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    JUT_ASSERT(1403, mpDrawCursor != NULL);

    mpDrawCursor->setAlphaRate(1.0f);
    mpDrawCursor->setParam(0.95f, 0.9f, 0.1f, 0.6f, 0.5f);

    #if VERSION == VERSION_GCN_JPN
    J2DTextBox* uVar9 = (J2DTextBox*)mBaseScreen->search('t_t00');
    mBaseScreen->search('f_t_00')->hide();
    #else
    J2DTextBox* uVar9 = (J2DTextBox*)mBaseScreen->search('f_t_00');
    mBaseScreen->search('t_t00')->hide();
    #endif
    uVar9->setFont(mDoExt_getSubFont());
    uVar9->setString(32, "");

    u16 stageTitleNo = (u16)dStage_stagInfo_GetStageTitleNo(dComIfGp_getStage()->getStagInfo());
    if (stageTitleNo != 0) {
        mString->getString(stageTitleNo, uVar9, NULL, NULL, NULL, 0);
    } else {
        JUT_WARN(1426, "stage title message index zero!");
    }
}

void dMenu_DmapBg_c::setFloorMessage() {
    int var_r26 = -5;

    char floor_msg_buf[128];
    J2DTextBox* floor_textbox_1[8];
    J2DTextBox* floor_textbox_2[8];
    J2DTextBox* floor_textbox_3[8];
    s16 floor_msg_no[13] = {
        0,      0,      0x03DB, 0x03DA, 0x036B, 0x036C, 0x036D,
        0x036E, 0x036F, 0x03DC, 0x03DD, 0x03D9, 0x03D8,
    };

    #if VERSION == VERSION_GCN_JPN
    #define FLOOR_TAG(A, B) ('ffoor0_0' | (A<<16) | (B))
    #define FFOOR_TAG(A, B) ('floor0_0' | (A<<16) | (B))
    #else
    #define FLOOR_TAG(A, B) ('floor0_0' | (A<<16) | (B))
    #define FFOOR_TAG(A, B) ('ffoor0_0' | (A<<16) | (B))
    #endif

    floor_textbox_1[0] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(7, 1));
    floor_textbox_2[0] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(7, 2));
    floor_textbox_3[0] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(7, 3));
    mFloorScreen->search(FLOOR_TAG(7, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(7, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(7, 3))->hide();
    floor_textbox_1[0]->setFont(mDoExt_getSubFont());
    floor_textbox_2[0]->setFont(mDoExt_getSubFont());
    floor_textbox_3[0]->setFont(mDoExt_getSubFont());

    floor_textbox_1[1] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(0, 1));
    floor_textbox_2[1] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(0, 2));
    floor_textbox_3[1] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(0, 3));
    mFloorScreen->search(FLOOR_TAG(0, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(0, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(0, 3))->hide();
    floor_textbox_1[1]->setFont(mDoExt_getSubFont());
    floor_textbox_2[1]->setFont(mDoExt_getSubFont());
    floor_textbox_3[1]->setFont(mDoExt_getSubFont());

    floor_textbox_1[2] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(1, 1));
    floor_textbox_2[2] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(1, 2));
    floor_textbox_3[2] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(1, 3));
    mFloorScreen->search(FLOOR_TAG(1, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(1, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(1, 3))->hide();
    floor_textbox_1[2]->setFont(mDoExt_getSubFont());
    floor_textbox_2[2]->setFont(mDoExt_getSubFont());
    floor_textbox_3[2]->setFont(mDoExt_getSubFont());

    floor_textbox_1[3] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(2, 1));
    floor_textbox_2[3] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(2, 2));
    floor_textbox_3[3] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(2, 3));
    mFloorScreen->search(FLOOR_TAG(2, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(2, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(2, 3))->hide();
    floor_textbox_1[3]->setFont(mDoExt_getSubFont());
    floor_textbox_2[3]->setFont(mDoExt_getSubFont());
    floor_textbox_3[3]->setFont(mDoExt_getSubFont());

    floor_textbox_1[4] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(3, 1));
    floor_textbox_2[4] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(3, 2));
    floor_textbox_3[4] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(3, 3));
    mFloorScreen->search(FLOOR_TAG(3, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(3, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(3, 3))->hide();
    floor_textbox_1[4]->setFont(mDoExt_getSubFont());
    floor_textbox_2[4]->setFont(mDoExt_getSubFont());
    floor_textbox_3[4]->setFont(mDoExt_getSubFont());

    floor_textbox_1[5] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(4, 1));
    floor_textbox_2[5] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(4, 2));
    floor_textbox_3[5] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(4, 3));
    mFloorScreen->search(FLOOR_TAG(4, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(4, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(4, 3))->hide();
    floor_textbox_1[5]->setFont(mDoExt_getSubFont());
    floor_textbox_2[5]->setFont(mDoExt_getSubFont());
    floor_textbox_3[5]->setFont(mDoExt_getSubFont());

    floor_textbox_1[6] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(5, 1));
    floor_textbox_2[6] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(5, 2));
    floor_textbox_3[6] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(5, 3));
    mFloorScreen->search(FLOOR_TAG(5, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(5, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(5, 3))->hide();
    floor_textbox_1[6]->setFont(mDoExt_getSubFont());
    floor_textbox_2[6]->setFont(mDoExt_getSubFont());
    floor_textbox_3[6]->setFont(mDoExt_getSubFont());

    floor_textbox_1[7] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(6, 1));
    floor_textbox_2[7] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(6, 2));
    floor_textbox_3[7] = (J2DTextBox*)mFloorScreen->search(FFOOR_TAG(6, 3));
    mFloorScreen->search(FLOOR_TAG(6, 1))->hide();
    mFloorScreen->search(FLOOR_TAG(6, 2))->hide();
    mFloorScreen->search(FLOOR_TAG(6, 3))->hide();
    floor_textbox_1[7]->setFont(mDoExt_getSubFont());
    floor_textbox_2[7]->setFont(mDoExt_getSubFont());
    floor_textbox_3[7]->setFont(mDoExt_getSubFont());

    OS_REPORT("floor ===> %d\n", dMenu_Dmap_c::myclass->getFloorAll());

    for (int i = 0; i < 8; i++) {
        if (i < dMenu_Dmap_c::myclass->getFloorAll()) {
            int floor_msg_index = (dMenu_Dmap_c::myclass->getBottomFloor() - var_r26) + i;
            if (floor_msg_index < 0 || floor_msg_index >= 13) {
                JUT_WARN(1609, "floor message index zero! %d", floor_msg_index);
                floor_textbox_1[i]->setString(16, "");
                floor_textbox_2[i]->setString(16, "");
                floor_textbox_3[i]->setString(16, "");
            } else {
                dMeter2Info_getString(floor_msg_no[floor_msg_index], floor_msg_buf, NULL);
                floor_textbox_1[i]->setString(16, floor_msg_buf);
                floor_textbox_2[i]->setString(16, floor_msg_buf);
                floor_textbox_3[i]->setString(16, floor_msg_buf);
            }
        } else {
            floor_textbox_1[i]->setString(16, "");
            floor_textbox_2[i]->setString(16, "");
            floor_textbox_3[i]->setString(16, "");
        }
    }
}

bool dMenu_DmapBg_c::dpdMove(s8 param_0, s8 param_1, s8 param_2, u8* param_3, u8 param_4) {
    bool var_r31 = false;
    return var_r31;
}

dMenu_DmapBg_c::~dMenu_DmapBg_c() {
    dComIfGp_getDmapResArchive()->removeResourceAll();
    delete mString;

    if (mpMeterHaihai != NULL) {
        delete mpMeterHaihai;
        mpMeterHaihai = NULL;
    }

    delete mBaseScreen;
    mBaseScreen = NULL;

    delete mFloorScreen;
    mFloorScreen = NULL;

    delete mpBaseRoot;
    mpBaseRoot = NULL;

    delete mpFloorRoot;
    mpFloorRoot = NULL;

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    for (int i = 0; i < 2; i++) {
        delete mMapScreen[i];
        mMapScreen[i] = NULL;

        delete mpMapSpace[i];
        mpMapSpace[i] = NULL;

        delete mpMapRoot[i];
        mpMapRoot[i] = NULL;
    }

    if (mpBlack != NULL) {
        delete mpBlack;
        mpBlack = NULL;
    }

    for (int i = 0; i < 1; i++) {
        delete field_0xd28[i];
    }

    for (int i = 0; i < 2; i++) {
        delete mpMapSpace[i];
        mpMapSpace[i] = NULL;
    }

    if (field_0xcf4 != NULL) {
        delete field_0xcf4;
        field_0xcf4 = NULL;
    }

    if (field_0xcf8 != NULL) {
        delete field_0xcf8;
        field_0xcf8 = NULL;
    }

    deleteExplain();

    if (mpArchive != NULL) {
        JKRUnmountArchive(mpArchive);
        mpArchive = NULL;
    }

    if (mpArchiveMount != NULL) {
        JKRUnmountArchive(mpArchiveMount->getArchive());
        mpArchiveMount->destroy();
        mpArchiveMount = NULL;
    }

    if (mpBackTexture != NULL) {
        delete mpBackTexture;
        mpBackTexture = NULL;
    }

    delete mButtonScreen;
    mButtonScreen = NULL;

    if (mDecorateScreen != NULL) {
        delete mDecorateScreen;
        mDecorateScreen = NULL;
    }

    if (mpDecorateRoot != NULL) {
        delete mpDecorateRoot;
        mpDecorateRoot = NULL;
    }

    delete mpButtonRoot;
    mpButtonRoot = NULL;

    for (int i = 0; i < 2; i++) {
        if (mpButtonAB[i] != NULL) {
            delete mpButtonAB[i];
            mpButtonAB[i] = NULL;
        }

        if (mpButtonText[i] != NULL) {
            delete mpButtonText[i];
            mpButtonText[i] = NULL;
        }
    }

    delete mpCButton;
    mpCButton = NULL;

    if (mpJButton != NULL) {
        delete mpJButton;
        mpJButton = NULL;
    }

    if (mpTalkHeap != NULL) {
        if (dMsgObject_getTalkHeap() == mpTalkHeap) {
            dMsgObject_setTalkHeap(NULL);
        }
        mDoExt_destroyExpHeap(mpTalkHeap);
        mpTalkHeap = NULL;
    }
}

void dMenu_DmapBg_c::setAllAlphaRate(f32 i_rate, bool param_2) {
    field_0xd9c = i_rate;

    if (param_2) {
        mpBaseRoot->setBackupAlpha();
        mpFloorRoot->setBackupAlpha();

        for (int i = 0; i < 2; i++) {
            mpMapRoot[i]->setBackupAlpha();
        }

        mpButtonRoot->setBackupAlpha();

        if (mpDecorateRoot != NULL) {
            mpDecorateRoot->setBackupAlpha();
        }
    }

    mpBaseRoot->setAlphaMorfRate(i_rate);
    mpFloorRoot->setAlphaMorfRate(i_rate);

    for (int i = 0; i < 2; i++) {
        mpMapRoot[i]->setAlphaMorfRate(i_rate);
    }

    mpButtonRoot->setAlphaMorfRate(i_rate);

    if (mpDecorateRoot != NULL) {
        mpDecorateRoot->setAlphaMorfRate(i_rate);
    }
}

void dMenu_DmapBg_c::setGoldAnimation(bool param_1) {
    J2DPane* gold0 = mMapScreen[0]->search('gold00_0');
    J2DPane* gold1 = mMapScreen[0]->search('gold00_1');

    if (param_1) {
        gold0->setAnimation(field_0xd28[0]);
        gold1->setAnimation(field_0xd28[0]);
    } else {
        gold0->setAnimation((J2DAnmTextureSRTKey*)NULL);
        gold1->setAnimation((J2DAnmTextureSRTKey*)NULL);
    }
}

void dMenu_DmapBg_c::setGoldFrameAlphaRate(f32 i_rate) {
    if (0.0f == i_rate) {
        mMapScreen[0]->search('gold00_0')->hide();
        mMapScreen[0]->search('gold00_1')->hide();
    } else {
        mMapScreen[0]->search('gold00_0')->show();
        mMapScreen[0]->search('gold00_1')->show();
        mMapScreen[0]->search('gold00_0')->setAlpha(i_rate * 255.0f);
        mMapScreen[0]->search('gold00_1')->setAlpha(i_rate * 255.0f);
    }
}

void dMenu_DmapBg_c::addGoldFrameAlphaRate() {
    f32 rate;
    if (field_0xdd7 >= g_fmapHIO.mDisplayFrameNum) {
        rate = 1.0f;
        setGoldAnimation(true);
    } else {
        field_0xdd7++;
        if (field_0xdd7 == g_fmapHIO.mDisplayFrameNum) {
            setGoldAnimation(true);
        }
        rate = (f32)(field_0xdd7 * field_0xdd7) / (f32)(g_fmapHIO.mDisplayFrameNum * g_fmapHIO.mDisplayFrameNum);
    }

    setGoldFrameAlphaRate(rate);
}

void dMenu_DmapBg_c::decGoldFrameAlphaRate() {
    f32 rate;
    setGoldAnimation(false);

    if (field_0xdd7 >= g_fmapHIO.mUndisplayFrameNum) {
        field_0xdd7 = g_fmapHIO.mUndisplayFrameNum / 2;
    } 

    field_0xdd7 = 0;
    if (field_0xdd7 == 0) {
        rate = 0.0f;
    } else {
        field_0xdd7--;
        rate = (f32)(field_0xdd7 * field_0xdd7) / (f32)(g_fmapHIO.mUndisplayFrameNum * g_fmapHIO.mUndisplayFrameNum);
    }
    
    setGoldFrameAlphaRate(rate);
}

void dMenu_DmapBg_c::draw() {
    u32 scissor_left;
    u32 scissor_top;
    u32 scissor_width;
    u32 scissor_height;

    J2DOrthoGraph* grafContext = (J2DOrthoGraph*)dComIfGp_getCurrentGrafPort();
    grafContext->setup2D();

    GXGetScissor(&scissor_left, &scissor_top, &scissor_width, &scissor_height);
    grafContext->scissor(field_0xd94, 0.0f, FB_WIDTH, FB_HEIGHT);
    grafContext->setScissor();

    mBaseScreen->draw(field_0xd94, field_0xd98, grafContext);
    dMenu_Dmap_c::myclass->drawFloorScreenBack(mFloorScreen, field_0xd94, field_0xd98, grafContext);

    f32 dVar21 = mDoGph_gInf_c::getWidthF() / FB_WIDTH;
    f32 dVar16 = mDoGph_gInf_c::getHeightF() / FB_HEIGHT;
    mMapScreen[0]->draw(field_0xd94, field_0xd98, grafContext);

    if (mpBackTexture != NULL) {
        J2DPane* center_pane = mMapScreen[0]->search('center_n');
        CPaneMgr pane;
        Mtx mtx;
        Vec local_200 = pane.getGlobalVtx(center_pane, &mtx, 0, false, 0);
        Vec local_20c = pane.getGlobalVtx(center_pane, &mtx, 3, false, 0);
        grafContext->scissor(((local_200.x - mDoGph_gInf_c::getMinXF()) / dVar21), ((local_200.y / dVar16) / dVar16),
                             ((local_20c.x - local_200.x) / dVar21), 2.0f + (local_20c.y - local_200.y));
        grafContext->setScissor();

        f32 dVar17 = field_0xd8c / 255.0f;
        JUtility::TColor color_black;
        JUtility::TColor color_white;
        color_black.set(field_0xd91, field_0xd92, field_0xd93, 0xFF);
        color_white.set(field_0xd8d, field_0xd8e, field_0xd8f, 0xFF);
        mpBackTexture->setAlpha(dVar17 * (field_0xdbc * field_0xd9c));

        f32 local_28c = mpBackTexture->getBounds().i.x;
        mpBackTexture->setBlackWhite(color_black, color_white);
        mpBackTexture->draw(local_28c, field_0xd94 + mpBackTexture->getBounds().i.y, mpBackTexture->getWidth(),
                            mpBackTexture->getHeight(), false, false, false);

        grafContext->scissor(field_0xd94 + mDoGph_gInf_c::getMinXF(),
                             scissor_top, mDoGph_gInf_c::getWidthF(),
                             scissor_height);
        grafContext->setScissor();
    }

    mMapScreen[1]->draw(field_0xd94, field_0xd98, grafContext);

    J2DPane* center_pane = mMapScreen[1]->search('center_n');
    CPaneMgr pane;
    Mtx local_110;
    Vec local_218 = pane.getGlobalVtx(center_pane, &local_110, 0, false, 0);
    Vec local_224 = pane.getGlobalVtx(center_pane, &local_110, 3, false, 0);
    f32 local_294 = ((local_218.x - mDoGph_gInf_c::getMinXF()) / dVar21);
    f32 local_298 = ((local_218.y / dVar16) / dVar16);
    f32 local_29c = ((local_224.x - local_218.x) / dVar21);
    f32 local_2a0 = 2.0f + (local_224.y - local_218.y);
    grafContext->scissor(local_294, local_298, local_29c, local_2a0);
    grafContext->setScissor();

    Vec local_26c = pane.getGlobalVtx(mMapPane, &local_110, 0, false, 0);
    drawIcon(local_26c.x + field_0xd94, local_26c.y, field_0xda8, 1.0f);
    grafContext->scissor(field_0xd94 + mDoGph_gInf_c::getMinXF(), scissor_top, mDoGph_gInf_c::getWidthF(), scissor_height);
    grafContext->setScissor();
    grafContext->scissor(scissor_left, scissor_top, scissor_width, scissor_height);
    grafContext->setScissor();

    if (field_0xdda != 0 && mpMeterHaihai != NULL) {
        mpMeterHaihai->setScale(0.7f);
        f32 x1 = 0.0f;
        f32 y1 = 0.0f;
        mpMeterHaihai->drawHaihai(field_0xdda,
            x1 + (local_224.x + local_218.x) / 2,
            y1 + (local_224.y + local_218.y) / 2,
            -35.0f + (local_224.x - local_218.x), 
            -35.0f + (local_224.y - local_218.y));
        field_0xdda = 0;
    }

    dMenu_Dmap_c::myclass->drawFloorScreenTop(mFloorScreen, field_0xd94, field_0xd98, grafContext);

    if (0.0f == field_0xd94 && 0.0f == field_0xd98) {
        drawCursor();
    }

    if (mDecorateScreen != NULL) {
        mDecorateScreen->draw(field_0xd94, field_0xd98, grafContext);
    }

    if (field_0xdd0 != 0 && mpItemExplain != NULL) {
        mpItemExplain->draw(grafContext);
    }

    mButtonScreen->draw(field_0xd94, field_0xd98, grafContext);
    grafContext->scissor(scissor_left, scissor_top, scissor_width, scissor_height);
    grafContext->setScissor();
    grafContext->setup2D();
}

void dMenu_DmapBg_c::update() {
    if (mpArchiveMount != NULL) {
        if (mpArchiveMount->sync() && mpArchive == NULL) {
            mpArchive = mpArchiveMount->getArchive();
            mpArchiveMount->destroy();
            mpArchiveMount = NULL;

            ResTIMG* mp_image = (ResTIMG*)mpArchive->getResource("tex/bg.bti");
            JUT_ASSERT(2321, mp_image != NULL);

            mpBackTexture = new J2DPicture(mp_image);
            JUT_ASSERT(2323, mpBackTexture != NULL);

            void* spec = mpArchive->getResource("spec/spec.dat");
            memcpy(&field_0xd80, spec, 20);
        }
    }

    if (mpBackTexture != NULL && field_0xdbc < 255.0f) {
        field_0xdbc += 25.5f;
        if (field_0xdbc > 255.0f) {
            field_0xdbc = 255.0f;
        }
    }
}

void dMenu_DmapBg_c::calcCursor() {}

void dMenu_DmapBg_c::drawCursor() {
    mpDrawCursor->draw();
}

dMenu_Dmap_c::dMenu_Dmap_c(JKRExpHeap* param_1, STControl* param_2, CSTControl* param_3,
                           u8 param_4, u8 param_5) {
    dMeter2Info_setMapDrugFlag(0);
    dMenu_Dmap_c::myclass = this;
    field_0xe8 = param_1;
    mpStick = param_2;
    mpCStick = param_3;
    mZoomState = param_4;
    mpDrawBg = NULL;
    mDmapHeap = NULL;
    mItemTexBuf = NULL;
    mSelStick = NULL;
    field_0x94 = NULL;
    m_itemSubProcess = 0;

    for (int i = 0; i < 8; i++) {
        mSelFloor[i] = NULL;
    }

    for (int i = 0; i < 8; i++) {
        mSelFloor[i] = NULL;
        mIconLinkPos[i] = NULL;
        mIconBossPos[i] = NULL;
    }

    for (int i = 0; i < 2; i++) {
        mStayIcon[i] = NULL;
    }

    field_0x10 = NULL;
    for (int i = 0; i < 3; i++) {
        field_0x7c[i] = NULL;
        field_0x88[i]=  NULL;
    }

    field_0x98 = NULL;
    mMapCtrl = NULL;
    field_0x144 = 0.0f;
    field_0x148 = 0.0f;
    field_0x181 = 0;

    if (param_5 == 1) {
        field_0x104 = -mDoGph_gInf_c::getWidthF();
        field_0x108 = 0.0f;
    } else if (param_5 == 3) {
        field_0x104 = mDoGph_gInf_c::getWidthF();
        field_0x108 = 0.0f;
    } else if (param_5 == 2) {
        field_0x104 = 0.0f;
        field_0x108 = -mDoGph_gInf_c::getHeightF();
    } else if (param_5 == 0) {
        field_0x104 = 0.0f;
        field_0x108 = mDoGph_gInf_c::getHeightF();
    } else {
        field_0x104 = 0.0f;
        field_0x108 = 0.0f;
    }

    field_0x10c = 0.0f;
    field_0x110 = 0.0f;
    field_0x134 = 0.0f;
    field_0x138 = 0.0f;
    field_0x13c = 0.0f;
    field_0x140 = 0.0f;
    mInOutDir = param_5;
    field_0x164 = 0;
    field_0x185 = 0;
    field_0x180 = 0;
    mMapCtrl = 0;
    mpDresArchiveMount = 0;
    mpDresArchive = 0;
}

void dMenu_Dmap_c::screenInit() {
    static u64 const floor_tag[8] = {'floor7_n', 'floor0_n', 'floor1_n', 'floor2_n',
                                     'floor3_n', 'floor4_n', 'floor5_n', 'floor6_n'};
    static u64 const icon_tag[8] = {'ico_set7', 'ico_set0', 'ico_set1', 'ico_set2',
                                    'ico_set3', 'ico_set4', 'ico_set5', 'ico_set6'};
    static u64 const boss_tag[8] = {'ic_st_b7', 'ic_st_b0', 'ic_st_b1', 'ic_st_b2',
                                    'ic_st_b3', 'ic_st_b4', 'ic_st_b5', 'ic_st_b6'};
    static u64 const stay_tag[2] = {'rink_nul', 'boss_nul'};
    static u64 const waku_tag[3] = {'gray_map', 'gray_con', 'gray_key'};
    static u64 const key_tag[3] = {'key_00', 'key_01', 'key_02'};
    static u64 const item_tag[3] = {'map000', 'con000', 'i_key_n'};

    field_0x10 = new CPaneMgr(mpDrawBg->mBaseScreen, 'floo_s_n', 0, NULL);

    for (int i = 0; i < 8; i++) {
        field_0x114[i] = 0.0f;
        mSelFloor[i] = new CPaneMgr(mpDrawBg->mFloorScreen, floor_tag[i], 3, NULL);
        JUT_ASSERT(2654, mSelFloor[i] != NULL);
        mIconLinkPos[i] = new CPaneMgr(mpDrawBg->mFloorScreen, icon_tag[i], 0, NULL);
        JUT_ASSERT(2657, mIconLinkPos[i] != NULL);
        mIconBossPos[i] = new CPaneMgr(mpDrawBg->mFloorScreen, boss_tag[i], 0, NULL);
        JUT_ASSERT(2660, mIconBossPos[i] != NULL);
    }

    for (int i = 0; i < 2; i++) {
        mStayIcon[i] = new CPaneMgr(mpDrawBg->mFloorScreen, stay_tag[i], 0, NULL);
        JUT_ASSERT(2665, mStayIcon[i] != NULL);
        mStayIcon[i]->hide();

        field_0x14c[i] = mStayIcon[i]->getGlobalVtxCenter(false, 0);

        if (i == 0) {
            mpDrawBg->mFloorScreen->search('wolf')->move((f32)mpDrawBg->mFloorScreen->search('rink')->getBounds().i.x, (f32)mpDrawBg->mFloorScreen->search('rink')->getBounds().i.y);

            if (daPy_py_c::checkNowWolf()) {
                mpDrawBg->mFloorScreen->search('wolf')->show();
                mpDrawBg->mFloorScreen->search('rink')->hide();
            } else {
                mpDrawBg->mFloorScreen->search('wolf')->hide();
                mpDrawBg->mFloorScreen->search('rink')->show();
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        if (i >= (mFloorAll - mBottomFloor) + 1) {
            mSelFloor[i]->hide();
        } else  if (i == getCurFloorPos()) {
            mSelFloor[i]->getPanePtr()->scale(1.0f, 1.0f);
            mSelFloor[i]->setAlpha(0xFF);
        } else {
            mSelFloor[i]->getPanePtr()->scale(0.9f, 0.9f);
            mSelFloor[i]->setAlpha(0x80);
        }
    }

    f32 var_f29;
    f32 var_f28 = 7.0f;

    var_f29 = mpDrawBg->mFloorScreen->search('s_n_all')->getHeight();
    f32 var_f30 = mSelFloor[0]->getPanePtr()->getHeight();

    f32 var_f27 = mFloorAll - mBottomFloor + 1;

    f32 var_f26 = (var_f27 * var_f30) + ((var_f27 - 1.0f) * var_f28);

    f32 var_f31 = (var_f30 / 2) + ((var_f29 - var_f26) / 2);
    var_f31 -= var_f29 / 2;

    for (int i = mFloorAll - mBottomFloor; i >= 0; i--) {
        mSelFloor[i]->getPanePtr()->translate(mSelFloor[i]->getPanePtr()->getTranslateX(), var_f31);
        var_f31 += var_f30 + var_f28;
    }
    
    field_0x10->getPanePtr()->translate(mSelFloor[getCurFloorPos()]->getPanePtr()->getTranslateX(), mSelFloor[getCurFloorPos()]->getPanePtr()->getTranslateY());
    iconMoveCalc();
    Vec local_b0 = mSelFloor[getDefaultCurFloorPos()]->getGlobalVtxCenter(false, 0);
    mpDrawBg->mpDrawCursor->setPos(local_b0.x + field_0x104, local_b0.y, mSelFloor[getDefaultCurFloorPos()]->getPanePtr(), true);
    field_0x94 = new CPaneMgr(mpDrawBg->mBaseScreen, 'item_s_n', 0, NULL);
    field_0x7c[0] = new CPaneMgr(mpDrawBg->mBaseScreen, 'map_n', 3, NULL);
    field_0x7c[1] = new CPaneMgr(mpDrawBg->mBaseScreen, 'con_n', 3, NULL);
    field_0x7c[2] = new CPaneMgr(mpDrawBg->mBaseScreen, 'key_n', 3, NULL);
    field_0x88[0] = new CPaneMgr(mpDrawBg->mBaseScreen, 'map000', 3, NULL);
    field_0x88[1] = new CPaneMgr(mpDrawBg->mBaseScreen, 'con000', 3, NULL);
    
    if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x11) {
        field_0x88[2] = new CPaneMgr(mpDrawBg->mBaseScreen, 'i_key_n', 3, NULL);
    } else if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x14) {
        field_0x88[2] = new CPaneMgr(mpDrawBg->mBaseScreen, 'lv5_k_n', 3, NULL);
    } else {
        field_0x88[2] = new CPaneMgr(mpDrawBg->mBaseScreen, 'nor_k_n', 3, NULL);
    }

    for (int i = 0; i < 3; i++) {
        field_0x88[i]->scale(0.9f, 0.9f);
    }

    if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x11) {
        mpDrawBg->mBaseScreen->search('lv5_k_n')->hide();
        mpDrawBg->mBaseScreen->search('nor_k_n')->hide();

        u8 key_num = 0;
        if (checkItemGet(fpcNm_ITEM_L2_KEY_PIECES1, 1)) {
            key_num = 1;
        }
        if (checkItemGet(fpcNm_ITEM_L2_KEY_PIECES2, 1)) {
            key_num++;
        }
        if (checkItemGet(fpcNm_ITEM_L2_KEY_PIECES3, 1)) {
            key_num++;
        }

        for (int i = 0; i < 3; i++) {
            if (i == key_num - 1) {
                mpDrawBg->mBaseScreen->search(key_tag[i])->show();
            } else {
                mpDrawBg->mBaseScreen->search(key_tag[i])->hide();
            }
        }

        if (key_num != 0) {
            field_0x174[2] = 0x26;
        } else {
            field_0x174[2] = 0;
        }

        if (key_num == 0 || dMeter2Info_isTempBit(0)) {
            mpDrawBg->mBaseScreen->search('key_n_n')->hide();
        } else {
            ResTIMG* tex = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(key_num));
            ((J2DPicture*)mpDrawBg->mBaseScreen->search('c_n_2_s'))->changeTexture(tex, 0);
            ((J2DPicture*)mpDrawBg->mBaseScreen->search('c_n_2'))->changeTexture(tex, 0);
            tex = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', dMeter2Info_getNumberTextureName(3));
            ((J2DPicture*)mpDrawBg->mBaseScreen->search('c_n_1_s'))->changeTexture(tex, 0);
            ((J2DPicture*)mpDrawBg->mBaseScreen->search('c_n_1'))->changeTexture(tex, 0);
        }
    } else if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x14) {
        mpDrawBg->mBaseScreen->search('i_key_n')->hide();
        mpDrawBg->mBaseScreen->search('nor_k_n')->hide();
        mpDrawBg->mBaseScreen->search('key_n_n')->hide();

        u8 itemNo = fpcNm_ITEM_NONE;
        field_0x174[2] = 0;

        if (g_fmapHIO.mMapIconHIO.mLV5DungeonItemDebug) {
            itemNo = g_fmapHIO.mMapIconHIO.mLV5DungeonItem;
            if (itemNo != fpcNm_ITEM_NONE) {
                field_0x174[2] = itemNo;
            }
        } else {
            if (dComIfGs_isDungeonItemBossKey()) {
                itemNo = fpcNm_ITEM_LV5_BOSS_KEY;
                field_0x174[2] = (u8)dComIfGs_isDungeonItemBossKey() ? fpcNm_ITEM_LV5_BOSS_KEY : 0;
                                                                    /* dSv_event_flag_c::F_0003 - Snowpeak Ruins - Handed over 
                                                                                                  tomato puree and left room */
            } else if (checkItemGet(fpcNm_ITEM_TOMATO_PUREE, 1) && !dComIfGs_isEventBit(2)) {
                itemNo = fpcNm_ITEM_TOMATO_PUREE;
                field_0x174[2] = fpcNm_ITEM_TOMATO_PUREE;
                                                             /* dSv_event_flag_c::F_0004 - Snowpeak Ruins - Handed over secret 
                                                                                           ingredient and left room */
            } else if (checkItemGet(fpcNm_ITEM_TASTE, 1) && !dComIfGs_isEventBit(1)) {
                itemNo = fpcNm_ITEM_TASTE;
                field_0x174[2] = fpcNm_ITEM_TASTE;
            }
        }

        switch (itemNo) {
        case fpcNm_ITEM_TOMATO_PUREE:
        case fpcNm_ITEM_TASTE:
            dMeter2Info_readItemTexture(itemNo, mItemTexBuf,
                                        (J2DPicture*)mpDrawBg->mBaseScreen->search('lv5_boss'), NULL, NULL, NULL, NULL,
                                        NULL, NULL, -1);
            break;
        }
    } else {
        mpDrawBg->mBaseScreen->search('i_key_n')->hide();
        mpDrawBg->mBaseScreen->search('lv5_k_n')->hide();
        mpDrawBg->mBaseScreen->search('key_n_n')->hide();
        field_0x174[2] = (u8)dComIfGs_isDungeonItemBossKey() ? fpcNm_ITEM_BOSS_KEY : 0;
    }

    field_0x174[0] = (u8)dComIfGs_isDungeonItemMap() ? fpcNm_ITEM_MAP : 0;
    field_0x174[1] = (u8)dComIfGs_isDungeonItemCompass() ? fpcNm_ITEM_COMPUS : 0;
    field_0x177 = 0;

    int bVar6 = 0;
    for (int i = 0; i < 3; i++) {
        if (field_0x174[i] != 0) {
            if (!bVar6) {
                field_0x177 = i;
                bVar6 = 1;
            }
            field_0x88[i]->getPanePtr()->scale(0.9f, 0.9f);
        } else {
            mpDrawBg->mBaseScreen->search(item_tag[i])->hide();
            mpDrawBg->mBaseScreen->search(waku_tag[i])->hide();

            if (i == 2) {
                mpDrawBg->mBaseScreen->search('lv5_k_n')->hide();
                mpDrawBg->mBaseScreen->search('nor_k_n')->hide();
                mpDrawBg->mBaseScreen->search('key_n_n')->hide();
            }
        }
    }

    if (bVar6 == true) {
        field_0x94->getPanePtr()->translate(field_0x7c[field_0x177]->getPanePtr()->getTranslateX(), field_0x7c[field_0x177]->getPanePtr()->getTranslateY());
    }

    field_0x94->hide();
    field_0x98 = new CPaneMgr(mpDrawBg->mBaseScreen, 'so_s_n', 0, NULL);
    field_0x98->hide();
}

void dMenu_Dmap_c::getPlayerIconPos(s8 i_floorNo, f32 param_2) {
    if (i_floorNo == field_0x172) {
        f32 pos_x;
        f32 pos_y;
        s16 rotation;
        mMapCtrl->getPlayerDrawInfo(&pos_x, &pos_y, &rotation);
        mpDrawBg->setIconInfo(ICON_LINK_e, pos_x, pos_y, param_2, cM_sht2d(rotation), 1.0f, 1);
        player_px = pos_x;
        player_py = pos_y;
    }
}

void dMenu_Dmap_c::getIconPos(s8 i_floorNo, f32 param_2) {
    f32 pos_x;
    f32 pos_y;
    f32 map_width = mpDrawBg->getMapWidth();
    f32 map_height = mpDrawBg->getMapHeight();
    s8 cStack_45;

    mMapCtrl->initGetTreasureList(1, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, &cStack_45, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_DUNGEON_ENTER_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(8, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, &cStack_45, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_LV8_WARP_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(3, i_floorNo);
    if (mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL) == 1) {
        mpDrawBg->setIconInfo(ICON_BOSS_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(0, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_TREASURE_CHEST_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(5, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_DESTINATION_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    if (dComIfGp_isLightDropMapVisible()) {
        mMapCtrl->initGetTreasureList(4, i_floorNo);
        while (true) {
            if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
                break;
            }
            mpDrawBg->setIconInfo(ICON_LIGHT_DROP_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
        }
    }

    mMapCtrl->initGetTreasureList(12, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_LIGHT_BALL_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(11, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_CANNON_BALL_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(2, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_KEY_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(9, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_MONKEY_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(15, i_floorNo);
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, NULL, NULL)) {
            break;
        }
        mpDrawBg->setIconInfo(ICON_COPY_STATUE_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
    }

    mMapCtrl->initGetTreasureList(16, i_floorNo);
    s16 rotation;
    u8 swbit;
    s8 roomNo;
    while (true) {
        if (!mMapCtrl->getTreasureList(&pos_x, &pos_y, NULL, &swbit, &roomNo)) {
            break;
        }
        if (swbit == 0xFF || (swbit != 0xFF && dComIfGs_isSwitch(swbit, (int)roomNo))) {
            mpDrawBg->setIconInfo(ICON_OOCCOO_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
        } else {
            mpDrawBg->setIconInfo(ICON_OOCCOO_JR_e, pos_x, pos_y, param_2, 0.0f, 1.0f, 1);
        }
    }

    s32 drawInfo = mMapCtrl->getRestartDrawInfo(&pos_x, &pos_y, &rotation);
    if (drawInfo == i_floorNo) {
        mpDrawBg->setIconInfo(ICON_LINK_ENTER_e, pos_x, pos_y, param_2, cM_sht2d(rotation), 1.0f, 1);
    }

    getPlayerIconPos(i_floorNo, param_2);
}

dMenu_Dmap_c::~dMenu_Dmap_c() {
    s32 iVar2 = field_0xe8->getTotalFreeSize();
    s32 iVar4 = field_0xe8->getTotalFreeSize();

    if (mpDrawBg != NULL) {
        delete mpDrawBg;
        mpDrawBg = NULL;
    }

    if (mDmapHeap != NULL) {
        mDmapHeap->free(mItemTexBuf);
        mItemTexBuf = NULL;
    }

    OS_REPORT("mpDrawBg free ==================> %d bytes use,  %d bytes remain\n",
              field_0xe8->getTotalFreeSize() - iVar4, field_0xe8->getTotalFreeSize());

    if (mSelStick != NULL) {
        delete mSelStick;
    }

    if (field_0x94 != NULL) {
        delete field_0x94;
    }

    for (int i = 0; i < 8; i++) {
        if (mSelFloor[i] != NULL) {
            delete mSelFloor[i];
            mSelFloor[i] = NULL;
        }
        if (mIconLinkPos[i] != NULL) {
            delete mIconLinkPos[i];
            mIconLinkPos[i] = NULL;
        }
        if (mIconBossPos[i] != NULL) {
            delete mIconBossPos[i];
            mIconBossPos[i] = NULL;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (mStayIcon[i] != NULL) {
            delete mStayIcon[i];
            mStayIcon[i] = NULL;
        }
    }

    if (field_0x10 != NULL) {
        delete field_0x10;
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x7c[i] != NULL) {
            delete field_0x7c[i];
            field_0x7c[i] = NULL;
        }
        if (field_0x88[i] != NULL) {
            delete field_0x88[i];
            field_0x88[i] = NULL;
        }
    }

    if (field_0x98 != NULL) {
        delete field_0x98;
    }

    if (mMapCtrl != NULL) {
        mMapCtrl->_delete();
        delete mMapCtrl;
    }

    if (mpDresArchiveMount != NULL) {
        JKRUnmountArchive(mpDresArchiveMount->getArchive());
        mpDresArchiveMount->destroy();
        mpDresArchiveMount = NULL;
    }

    if (mpDresArchive != NULL) {
        JKRUnmountArchive(mpDresArchive);
        mpDresArchive = NULL;
    }

    if (mDmapHeap != NULL) {
        mDoExt_destroyExpHeap(mDmapHeap);
        mDmapHeap = NULL;
    }

    dMeter2Info_setMapDrugFlag(false);
    OS_REPORT("MenuDmap free ==================> %d bytes use,  %d bytes remain\n",
              field_0xe8->getTotalFreeSize() - iVar2, field_0xe8->getTotalFreeSize());

    dMenu_Dmap_c::myclass = NULL;
}

s8 dMenu_Dmap_c::getCurFloorPos() {
    s8 i_floor = field_0x16c - mBottomFloor;
    JUT_ASSERT(3393, i_floor >= 0 && i_floor < (8));
    return i_floor;
}

s8 dMenu_Dmap_c::getDefaultCurFloorPos() {
    return getCurFloorPos();
}

void dMenu_Dmap_c::iconMoveCalc() {
    s32 iVar7 = ((s16)field_0x172 - (s16)mBottomFloor);
    mStayIcon[0]->show();

    Vec local_3c = field_0x14c[0];
    Vec local_6c = mIconLinkPos[iVar7]->getGlobalVtxCenter(false, 0);
    mStayIcon[0]->paneTrans(local_6c.x - local_3c.x, local_6c.y - local_3c.y);

    // !@bug Uses ScaleX for y coordinate
    f32 yscale = mSelFloor[iVar7]->getPanePtr()->getScaleX();
    f32 xscale = mSelFloor[iVar7]->getPanePtr()->getScaleX();
    mStayIcon[0]->scale(xscale, yscale);

    if (field_0x173 != -99) {
        iVar7 = field_0x173 - (s16)mBottomFloor;
        mStayIcon[1]->show();

        Vec local_54 = field_0x14c[1];
        Vec local_78 = mIconBossPos[iVar7]->getGlobalVtxCenter(false, 0);
        mStayIcon[1]->paneTrans(local_78.x - local_54.x, local_78.y - local_54.y);
        mStayIcon[1]->scale(xscale, yscale);
    }
}

void dMenu_Dmap_c::drawFloorScreenBack(J2DScreen* i_screen, f32 i_transX, f32 i_transY,
                                       J2DOrthoGraph* i_ortho) {
    for (int i = 0; i < getFloorAll(); i++) {
        if (i == getCurFloorPos() && 0.0f == field_0x114[i]) {
            mSelFloor[i]->hide();
        } else {
            mSelFloor[i]->show();
        }
    }

    if (field_0x172 - mBottomFloor == getCurFloorPos()) {
        mStayIcon[0]->hide();
    } else {
        mStayIcon[0]->show();
    }

    if (field_0x173 != -99) {
        if (field_0x173 - mBottomFloor == getCurFloorPos()) {
            mStayIcon[1]->hide();
        } else {
            mStayIcon[1]->show();
        }
    }

    i_screen->draw(i_transX, i_transY, i_ortho);
}

void dMenu_Dmap_c::drawFloorScreenTop(J2DScreen* i_screen, f32 i_transX, f32 i_transY,
                                      J2DOrthoGraph* i_ortho) {
    for (int i = 0; i < getFloorAll(); i++) {
        if (i == getCurFloorPos() && 0.0f == field_0x114[i]) {
            mSelFloor[i]->show();
        } else {
            mSelFloor[i]->hide();
        }
    }

    if (field_0x172 - mBottomFloor == getCurFloorPos()) {
        mStayIcon[0]->show();
    } else {
        mStayIcon[0]->hide();
    }

    if (field_0x173 != -99) {
        if (field_0x173 - mBottomFloor == getCurFloorPos()) {
            mStayIcon[1]->show();
        } else {
            mStayIcon[1]->hide();
        }
    }

    i_screen->draw(i_transX, i_transY, i_ortho);
}

bool dMenu_Dmap_c::isMapMoveState() {
    field_0x184 = true;
    if (mMapCtrl->getDisableZoomMoveFlgX() == true && mMapCtrl->getDisableZoomMoveFlgZ() == true) {
        field_0x184 = false;
    }

    return field_0x184;
}

void dMenu_Dmap_c::floorChangeMode() {
    if (field_0x16d > field_0x16c) {
        field_0x16e = field_0x16c;
        field_0x179 = mSelStick->getYwaitTimer();
        s8 dVar1 = field_0x16d - field_0x16c;
        field_0x16c = field_0x16d;
        mMapCtrl->setPlusNowStayFloorNo(dVar1, field_0x179);
        field_0x17d = 1;
    } else {
        field_0x16e = field_0x16c;
        field_0x179 = mSelStick->getYwaitTimer();
        field_0x17d = 1;
        s8 dVar1 = field_0x16d - field_0x16c;
        field_0x16c = field_0x16d;
        mMapCtrl->setPlusNowStayFloorNo(dVar1, field_0x179);
    }

    field_0x185 = 0;

    if (mZoomState != 0) {
        mpDrawBg->setAButtonString(0);
        mpDrawBg->setBButtonString(0x522);
    } else {
        mpDrawBg->setAButtonString(0x527);
        mpDrawBg->setBButtonString(0x3f9);
    }
}

void dMenu_Dmap_c::_create() {
    u32 sp30 = field_0xe8->getTotalFreeSize();
    OS_REPORT("MenuDmap create前 最大===========> %d bytes max\n", field_0xe8->getFreeSize());
    OS_REPORT("MenuDmap create前================> %d bytes remain\n", sp30);

    mDmapHeap = JKRCreateExpHeap(field_0xe8->getFreeSize() - 0x10, field_0xe8, false);
    JUT_ASSERT(3732, mDmapHeap != NULL);
    JKRHeap* heap = mDoExt_setCurrentHeap(mDmapHeap);
    u32 sp28 = mDmapHeap->getTotalFreeSize();
    
    mMapCtrl = new dMenu_DmapMapCtrl_c();
    JUT_ASSERT(3739, mMapCtrl != NULL);

    mCMessageNum = 0;
    mJMessageNum = 0;

    field_0x184 = true;
    if (mMapCtrl->getDisableZoomMoveFlgX() == true && mMapCtrl->getDisableZoomMoveFlgZ() == true) {
        field_0x184 = false;
    }

    mSelStick = new STControl(5, 2, 1, 1, 0.9f, 0.5f, 0, 0x800);
    JUT_ASSERT(3753, mSelStick != NULL);
    mSelStick->setFirstWaitTime(8);

    u32 sp24 = field_0xe8->getTotalFreeSize();
    mpDrawBg = new dMenu_DmapBg_c(mDmapHeap, mpStick);
    JUT_ASSERT(3763, mpDrawBg != NULL);

    mItemTexBuf = (u8*)mDmapHeap->alloc(0xC00, 0x20);
    JUT_ASSERT(3766, mItemTexBuf != NULL);

    if (mZoomState) {
        if (POINTER_OPT == 0) {
            mpDrawBg->setAButtonString(0);
            mpDrawBg->setBButtonString(0x522);
        }

        mCMessageNum = 0x37B;
        mJMessageNum = 0x569;
    } else {
        if (POINTER_OPT == 0) {
            mpDrawBg->setAButtonString(0x527);
            mpDrawBg->setBButtonString(0x3F9);
        }

        mCMessageNum = 0;
        mJMessageNum = 0;
    }

    OS_REPORT("mpDrawBg create==================> %d bytes use,  %d bytes remain\n", sp24 - field_0xe8->getTotalFreeSize(), field_0xe8->getTotalFreeSize());

    if (dMeter2Info_getMapStatus() == 6) {
        s8 sp8 = 0;
        if (dComIfGs_isSaveSwitch(0x67)) {
            sp8++;
        } else if (dComIfGs_isSaveSwitch(0x66) || dComIfGs_isSaveSwitch(0x65)) {
            
        }

        mMapCtrl->_create(mpDrawBg->getMapWidth(), mpDrawBg->getMapHeight(), mpDrawBg->getMapWidth(), mpDrawBg->getMapHeight(), sp8, mpBinData);
        m_process = 4;
    } else {
        mMapCtrl->_create(mpDrawBg->getMapWidth(), mpDrawBg->getMapHeight(), mpDrawBg->getMapWidth(), mpDrawBg->getMapHeight(), mpBinData);
        m_process = 1;
    }

    field_0x16c = mMapCtrl->getNowStayFloorNo();
    field_0x172 = mMapCtrl->getPlayerStayFloorNo();
    field_0x16d = field_0x16c;
    field_0x16f = field_0x16c;
    field_0x16a = 0;

    mpDrawBg->setDPDFloorSelCurPos(field_0x172);
    mFloorAll = mMapCtrl->getTopFloorNo();
    mBottomFloor = mMapCtrl->getBottomFloorNo();
    field_0x173 = 0x9D;

    int floorNo;
    if (dTres_c::getBossIconFloorNo(&floorNo)) {
        OS_REPORT("BOSS floorNo<%d>\n", floorNo);
        field_0x173 = floorNo;
    } else {
        OS_REPORT("BOSS floor Nothing!!\n");
    }

    mpDrawBg->setFloorMessage();

    if (mZoomState) {
        f32 sp1C = 0.0f;
        f32 sp18 = 0.0f;
        mMapCtrl->calcZoomCenter(&sp1C, &sp18);
        mMapCtrl->initZoomWait(sp1C, sp18);

        if ((mMapCtrl->getDisableZoomMoveFlgX() != true || mMapCtrl->getDisableZoomMoveFlgZ() != true) && mpDrawBg->field_0xcf4 != NULL) {
            mpDrawBg->field_0xcf4->setAlpha(0xFF);
        }
    }

    field_0x182 = 1;
    field_0x183 = 1;

    mpDrawBg->mapIconScaleSet(mZoomState);

    screenInit();

    mpDrawBg->getMapPane()->changeTexture(mMapCtrl->getResTIMGPointer(0), NULL);

    if (mpDrawBg->getMapPane()->append(mMapCtrl->getResTIMGPointer(1), 1.0f)) {
        mpDrawBg->getMapPane()->setBlendRatio(1.0f, 0.0f);
    }

    mDoExt_setCurrentHeap(heap);

    getIconPos(mMapCtrl->getNowStayFloorNo(), 1.0f);
    
    field_0x17d = 0;
    (this->*init_process[m_process])();

    field_0x17e = 0;
    (this->*map_init_process[field_0x17e])();
}

void dMenu_Dmap_c::_move() {
    u8 prev_process = m_process;
    JKRHeap* prev_heap = mDoExt_setCurrentHeap(mDmapHeap);

    mpDrawBg->update();
    mpDrawBg->clearIconInfo();
    mapControl();

    if (field_0x17e == 0) {
        mpDrawBg->dpdMove(mFloorAll, mBottomFloor, field_0x172, field_0x174, field_0x181);

        if (mpDrawBg->mpBlack != NULL) {
            f32 var_f31;
            if (mpDrawBg->field_0xdd3 != 0xFF) {
                var_f31 = 0.0f;
            } else {
                var_f31 = 0.70588237f;
            }

            cLib_addCalc2(&field_0x110, var_f31, 0.4f, 0.5f);
            mpDrawBg->mpBlack->setAlphaRate(field_0x110);
        }

        (this->*move_process[m_process])();
        mpDrawBg->setCButtonString(0);
        mpDrawBg->setJButtonString(0);

        if (m_process != prev_process) {
            (this->*init_process[m_process])();
        }
    }

    setMapTexture();
    mapBgAnime();
    mpDrawBg->calcCursor();
    mpDrawBg->addGoldFrameAlphaRate();

    mDoExt_setCurrentHeap(prev_heap);
}

void dMenu_Dmap_c::setMapTexture() {
    f32 mapBlendPer = mMapCtrl->getMapBlendPer();
    mpDrawBg->getMapPane()->setBlendRatio(mapBlendPer, 1.0f - mapBlendPer);
}

void dMenu_Dmap_c::mapBgAnime() {
    mpDrawBg->mapScreenAnime();
}

void dMenu_Dmap_c::mapControl() {
    u8 temp_r27 = field_0x17e;
    if (m_process == 0 || field_0x183 == 0) {
        mMapCtrl->move();

        getIconPos(mMapCtrl->getDispFloorNo(), mMapCtrl->getMapBlendPer());
        getIconPos(mMapCtrl->getDispFloor2No(), 1.0f - mMapCtrl->getMapBlendPer());
        return;
    }

    (this->*map_move_process[field_0x17e])();

    if (field_0x17e != temp_r27) {
        (this->*map_init_process[field_0x17e])();
    }

    f32 sp28 = g_fmapHIO.mScrollSpeedSlowBound < g_fmapHIO.mScrollSpeedFastBound ? g_fmapHIO.mScrollSpeedSlowBound : g_fmapHIO.mScrollSpeedFastBound;
    f32 sp24 = g_fmapHIO.mScrollSpeedSlowBound > g_fmapHIO.mScrollSpeedFastBound ? g_fmapHIO.mScrollSpeedSlowBound : g_fmapHIO.mScrollSpeedFastBound;

    f32 stick_value = mpCStick->getValueStick();
    s16 stick_angle = mpCStick->getAngleStick();

    bool var_r28 = false;
    f32 disp_center_x = mMapCtrl->getDispCenterX();
    f32 disp_center_z = mMapCtrl->getDispCenterZ();

    if (stick_value >= sp28 && field_0x181 != 2) {
        var_r28 = true;
        
        f32 var_f31 = mMapCtrl->getStageMapSizeX();
        if (var_f31 < mMapCtrl->getStageMapSizeZ()) {
            var_f31 = mMapCtrl->getStageMapSizeZ();
        }

        f32 var_f29;
        if (stick_value < sp24) {
            var_f29 = g_fmapHIO.mScrollSpeedDungeonMapSlow;
        } else {
            var_f29 = g_fmapHIO.mScrollSpeedDungeonMapFast;
        }

        f32 temp_f28 = (var_f29 / 100.0f) * var_f31;
        f32 sp18 = temp_f28 * cM_ssin(stick_angle);
        f32 sp14 = temp_f28 * cM_scos(stick_angle);
        mMapCtrl->setPlusZoomCenterX(sp18);
        mMapCtrl->setPlusZoomCenterZ(sp14);
    }

    mMapCtrl->move();

    getIconPos(mMapCtrl->getDispFloorNo(), mMapCtrl->getMapBlendPer());
    getIconPos(mMapCtrl->getDispFloor2No(), 1.0f - mMapCtrl->getMapBlendPer());

    if (var_r28 && (disp_center_x != mMapCtrl->getDispCenterX() || disp_center_z != mMapCtrl->getDispCenterZ())) {
        Z2GetAudioMgr()->seStartLevel(Z2SE_SY_MAP_SCROLL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    if (mpDrawBg->mpMeterHaihai != NULL && isMapMoveState() && (getCMessageNum() == 0x37B || getCMessageNum() == 0x569)) {
        mpDrawBg->field_0xdda = 0;

        if (mMapCtrl->getDispCenterX() > mMapCtrl->getZoomCenterMinX()) {
            mpDrawBg->field_0xdda |= 1;
        }

        if (mMapCtrl->getDispCenterX() < mMapCtrl->getZoomCenterMaxX()) {
            mpDrawBg->field_0xdda |= 4;
        }

        if (mMapCtrl->getDispCenterZ() > mMapCtrl->getZoomCenterMinZ()) {
            mpDrawBg->field_0xdda |= 8;
        }

        if (mMapCtrl->getDispCenterZ() < mMapCtrl->getZoomCenterMaxZ()) {
            mpDrawBg->field_0xdda |= 2;
        }

        mpDrawBg->mpMeterHaihai->_execute(0);
    }
}

bool dMenu_Dmap_c::isOpen() {
    bool var_r28 = false;
    bool var_r27 = false;
    const s16 display_frame_num = g_fmapHIO.mDisplayFrameNum;
    const s16 undisplay_frame_num = g_fmapHIO.mUndisplayFrameNum;

    mpBinData = NULL;

    if (mpDresArchive == NULL) {
        if (mpDresArchiveMount == NULL) {
            mpDresArchiveMount = mDoDvdThd_mountArchive_c::create("/res/FieldMap/res-d.arc", 2, mDoExt_getJ2dHeap());
        }

        if (mpDresArchiveMount->sync()) {
            mpDresArchive = mpDresArchiveMount->getArchive();
            mpDresArchiveMount->destroy();
            mpDresArchiveMount = NULL;

            mpBinData = mpDresArchive->getResource("dat/data.dat");
            JUT_ASSERT(4377, mpBinData != NULL);

            _create();
        } else {
            return var_r28;
        }
    }

    if (field_0x164 == 0) {
        var_r27 = true;
    }

    field_0x164++;
    f32 temp_f31 = (f32)field_0x164 / (f32)display_frame_num;
    
    if (mInOutDir == 1) {
        field_0x104 = (1.0f - temp_f31) * -mDoGph_gInf_c::getWidthF();
        field_0x108 = 0.0f;
    } else if (mInOutDir == 3) {
        field_0x104 = (1.0f - temp_f31) * mDoGph_gInf_c::getWidthF();
        field_0x108 = 0.0f;
    } else if (mInOutDir == 2) {
        field_0x104 = 0.0f;
        field_0x108 = (1.0f - temp_f31) * -mDoGph_gInf_c::getHeightF();
    } else if (mInOutDir == 0) {
        field_0x104 = 0.0f;
        field_0x108 = (1.0f - temp_f31) * mDoGph_gInf_c::getHeightF();
    }
    
    field_0x10c = temp_f31;

    if (m_process == 0) {
        Vec sp2C = field_0x7c[field_0x177]->getGlobalVtxCenter(false, 0);
        mpDrawBg->mpDrawCursor->setPos(sp2C.x + field_0x104, sp2C.y, field_0x7c[field_0x177]->getPanePtr(), 0);
    } else {
        Vec sp20 = mSelFloor[getDefaultCurFloorPos()]->getGlobalVtxCenter(false, 0);
        mpDrawBg->mpDrawCursor->setPos(sp20.x + field_0x104, sp20.y, mSelFloor[getDefaultCurFloorPos()]->getPanePtr(), 1);
    }

    if (field_0x164 >= display_frame_num) {
        field_0x164 = undisplay_frame_num;
        field_0x104 = 0.0f;
        field_0x108 = 0.0f;
        field_0x10c = 1.0f;
        var_r28 = true;
    }

    mpDrawBg->setAllAlphaRate(field_0x10c, var_r27);
    mpDrawBg->setGoldFrameAlphaRate(0.0f);
    return var_r28;
}

bool dMenu_Dmap_c::isClose() {
    bool var_r30 = true;
    bool var_r29 = false;
    const s16 undisplay_frame_num = g_fmapHIO.mUndisplayFrameNum;

    if (field_0x164 == undisplay_frame_num) {
        var_r29 = true;
    }

    field_0x164--;

    f32 temp_f31 = (f32)field_0x164 / (f32)undisplay_frame_num;

    if (field_0x164 <= 0) {
        field_0x164 = 0;
    } else {
        var_r30 = false;
    }

    if (mInOutDir == 1) {
        field_0x104 = (1.0f - temp_f31) * mDoGph_gInf_c::getWidthF();
        field_0x108 = 0.0f;
    } else if (mInOutDir == 3) {
        field_0x104 = (1.0f - temp_f31) * -mDoGph_gInf_c::getWidthF();
        field_0x108 = 0.0f;
    } else if (mInOutDir == 2) {
        field_0x104 = 0.0f;
        field_0x108 = (1.0f - temp_f31) * mDoGph_gInf_c::getHeightF();
    } else if (mInOutDir == 0) {
        field_0x104 = 0.0f;
        field_0x108 = (1.0f - temp_f31) * -mDoGph_gInf_c::getHeightF();
    }

    field_0x10c = temp_f31;
    mpDrawBg->setAllAlphaRate(field_0x10c, var_r29);
    mpDrawBg->decGoldFrameAlphaRate();
    return var_r30;
}

void dMenu_Dmap_c::_draw() {
    if (mMapCtrl != NULL) {
        mMapCtrl->draw();

        if (mpDrawBg != NULL) {
            mpDrawBg->setAllTrans(field_0x104, field_0x108);

            if (mpDrawBg->mpBackTexture != NULL) {
                f32 var_f31 = 100.0f * mpDrawBg->field_0xd88;
                f32 sp24 = mpDrawBg->field_0xd80 / var_f31;
                f32 sp20 = mpDrawBg->field_0xd84 / var_f31;

                const ResTIMG* back_timg = mpDrawBg->mpBackTexture->getTexture(0)->getTexInfo();
                field_0x13c = mMapCtrl->getPixelStageSizeX();
                field_0x140 = mMapCtrl->getPixelStageSizeZ();
                field_0x134 = mMapCtrl->getPixelCenterX();
                field_0x138 = mMapCtrl->getPixelCenterZ();

                f32 temp_f30;
                f32 var_f29;

                f32 var_f27;
                if (field_0x13c > field_0x140) {
                    temp_f30 = back_timg->width * var_f31;
                    var_f29 = mMapCtrl->getStageMapSizeX();
                    var_f27 = field_0x13c * (temp_f30 / var_f29);
                } else {
                    temp_f30 = back_timg->height * var_f31;
                    var_f29 = mMapCtrl->getStageMapSizeZ();
                    var_f27 = field_0x140 * (temp_f30 / var_f29);
                }

                f32 sp1C[2];
                sp1C[0] = var_f27;
                sp1C[1] = var_f27;

                f32 sp14[2];
                sp14[0] = -(field_0x134 + (100.0f * (sp24 * mMapCtrl->getPixelPerCm())));
                sp14[1] = -(field_0x138 + 100.0f * (sp20 * mMapCtrl->getPixelPerCm()));

                Vec spC;
                spC.x = mpDrawBg->mMapScreen[0]->search('center_n')->getGlbBounds().i.x + (mpDrawBg->mMapScreen[0]->search('center_n')->getWidth() / 2);
                spC.y = mpDrawBg->mMapScreen[0]->search('center_n')->getGlbBounds().i.y + (mpDrawBg->mMapScreen[0]->search('center_n')->getHeight() / 2);
                
                CPaneMgr sp70;
                //!@bug It's unclear what this is supposed to be, but a stack pointer being converted to a bool is probably not intended.
                u8 sp40[0x30];
                Vec sp34 = sp70.getGlobalVtxCenter(mpDrawBg->getMapPane(), (bool)sp40, 0);

                spC.x = sp34.x;
                spC.y = sp34.y;
                mpDrawBg->mpBackTexture->move(sp14[0] + (spC.x - (sp1C[0] / 2)), sp14[1] + (spC.y - (sp1C[1] / 2)));
                mpDrawBg->mpBackTexture->resize(sp1C[0], sp1C[1]);
            }

            dComIfGd_set2DOpa(mpDrawBg);
        }
    }
}

void dMenu_Dmap_c::itemInfo_init_proc() {
    m_itemSubProcess = 0;
    field_0x185 = 0;
}

void dMenu_Dmap_c::itemInfo_proc() {
    (this->*itemInfo_subProcess[m_itemSubProcess])();
}

void dMenu_Dmap_c::itemSelect() {
    if (mpDrawBg->field_0xdd4 == -99 && mpDrawBg->field_0xdd5 == 0xFF) {
        mSelStick->checkTrigger();
    }

    if (POINTER_OPT == 0) {
        if (field_0x174[field_0x177] != 0) {
            mpDrawBg->setAButtonString(0x5D);
        } else {
            mpDrawBg->setAButtonString(0);
        }

        mpDrawBg->setBButtonString(0x3F9);
    }

    if (mSelStick->checkUpTrigger()) {
        int temp_r3 = getNextItem(-1);
        if (temp_r3 != -1) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            field_0x178 = field_0x177;
            field_0x177 = temp_r3;
            field_0x179 = mSelStick->getYwaitTimer();
            itemSelectAnmInit();
        }
        return;
    }

    if (mSelStick->checkDownTrigger()) {
        int temp_r3_2 = getNextItem(1);
        if (temp_r3_2 != -1) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            field_0x178 = field_0x177;
            field_0x177 = temp_r3_2;
            field_0x179 = mSelStick->getYwaitTimer();
            itemSelectAnmInit();
        }
        return;
    }

    if (mSelStick->checkRightTrigger()) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_FLOOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        field_0x179 = mSelStick->getXwaitTimer();
        m_process = 3;

        if (POINTER_OPT == 0) {
            if (mZoomState) {
                mpDrawBg->setAButtonString(0);
                mpDrawBg->setBButtonString(0x522);
            } else {
                mpDrawBg->setAButtonString(0x527);
                mpDrawBg->setBButtonString(0x3F9);
            }
        }
    } else if (mpDrawBg->field_0xdd4 == -99) {
        if (mpDrawBg->field_0xdd5 != 0xFF) {
            if (field_0x177 != mpDrawBg->field_0xdd5) {
                Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                field_0x178 = field_0x177;
                field_0x177 = mpDrawBg->field_0xdd5;
                field_0x179 = mSelStick->getYwaitTimer();
                itemSelectAnmInit();
                return;
            }
        }

        if (mDoCPd_c::getTrigA(PAD_1) && !dMeter2Info_isTouchKeyCheck(0xC)) {
            if (field_0x174[field_0x177] != 0) {
                field_0x185 = 1;
                field_0x183 = 0;

                mpDrawBg->createExplain();
                if (mpDrawBg->getItemExplainPtr()->openExplainDmap(field_0x174[field_0x177], 0, 0, 0, 0xFF)) {
                    mpDrawBg->field_0xdd0 = 1;
                    m_itemSubProcess = 2;
                    Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

                    if (POINTER_OPT == 0) {
                        mpDrawBg->setAButtonString(0);
                        mpDrawBg->setBButtonString(0);
                    }

                    mCMessageNum = 0;
                    mJMessageNum = 0;
                }
            } else {
                Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }
    }
}

int dMenu_Dmap_c::getNextItem(int param_0) {
    if (!itemCarryCheck()) {
        return -1;
    }

    if ((field_0x177 + param_0) < 0 || (field_0x177 + param_0) > 2) {
        return -1;
    }

    int var_r30 = field_0x177;
    int var_r28 = -1;

    while ((var_r30 + param_0) <= 2 && (var_r30 + param_0) >= 0) {
        if (field_0x174[var_r30 + param_0] != 0) {
            var_r28 = var_r30 + param_0;
            break;
        }

        var_r30 += param_0;
    }

    return var_r28;
}

void dMenu_Dmap_c::itemSelectAnmInit() {
    field_0x94->hide();
    field_0x88[field_0x178]->scaleAnimeStart(0);
    field_0x88[field_0x177]->scaleAnimeStart(0);
    m_itemSubProcess = 1;
}

void dMenu_Dmap_c::itemSelectAnm() {
    bool temp_r29 = field_0x88[field_0x178]->scaleAnime(field_0x179, 1.0f, 0.9f, 0);
    bool temp_r28 = field_0x88[field_0x177]->scaleAnime(field_0x179, 0.9f, 1.0f, 0);

    if (temp_r29 == true && temp_r28 == true) {
        field_0x94->getPanePtr()->translate(field_0x88[field_0x177]->getPanePtr()->getTranslateX(),
                                            field_0x88[field_0x177]->getPanePtr()->getTranslateY());
        field_0x94->show();

        Vec sp14 = field_0x7c[field_0x177]->getGlobalVtxCenter(false, 0);
        mpDrawBg->mpDrawCursor->setPos(sp14.x + field_0x104, sp14.y, field_0x7c[field_0x177]->getPanePtr(), false);
        m_itemSubProcess = 0;
    }
}

void dMenu_Dmap_c::itemInfoOpenAnm() {
    m_itemSubProcess = 3;
}

void dMenu_Dmap_c::itemInfoWait() {
    mpDrawBg->getItemExplainPtr()->move();

    if (mpDrawBg->getItemExplainPtr()->getStatus() == 2) {
        if (POINTER_OPT == 0) {
            mpDrawBg->setAButtonString(0);
            mpDrawBg->setBButtonString(0x3F9);
        }
    } else if (mpDrawBg->getItemExplainPtr()->getStatus() == 3) {
        if (POINTER_OPT == 0) {
            if (field_0x174[field_0x177] != 0) {
                mpDrawBg->setAButtonString(0x5D);
            } else {
                mpDrawBg->setAButtonString(0);
            }
        }
    } else if (mpDrawBg->getItemExplainPtr()->getStatus() == 5) {
        if (POINTER_OPT == 0) {
            mpDrawBg->setAButtonString(0);
        }

        Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        mpDrawBg->field_0xdd0 = 1;
        m_itemSubProcess = 4;
    }
}

void dMenu_Dmap_c::itemInfoCloseAnm() {
    bool var_r30 = false;
    mpDrawBg->getItemExplainPtr()->move();

    if (mpDrawBg->getItemExplainPtr()->getStatus() == 0) {
        var_r30 = true;
    }

    if (var_r30 == true) {
        mpDrawBg->deleteExplain();
        mpDrawBg->field_0xdd0 = 0;
        field_0x185 = 0;
        field_0x183 = 1;
        m_itemSubProcess = 0;

        if (POINTER_OPT == 0) {
            mpDrawBg->setBButtonString(0x3F9);
        }

        if (mZoomState) {
            mCMessageNum = 0x37B;
            mJMessageNum = 0x569;
        } else {
            mCMessageNum = 0;
            mJMessageNum = 0;
        }
    }
}

u8 dMenu_Dmap_c::getNextStatus() {
    if (POINTER_OPT != 0) {
        if ((dMw_LEFT_TRIGGER() || (dMw_B_TRIGGER() && (!getZoomState() || (m_process == 0 && m_itemSubProcess == 0)))) && !isKeyCheck()) {
            return 1;
        } else if (dMeter2Info_isTouchKeyCheck(0xE) && dMeter2Info_getMapStatus() != 6 && !isKeyCheck()) {
            return 3;
        }
    } else {
        if ((dMw_LEFT_TRIGGER() || (dMw_B_TRIGGER() && (!getZoomState() || (m_process == 0 && m_itemSubProcess == 0)))) && !isKeyCheck()) {
            return 1;
        }
        #if (PLATFORM_GCN)
        else if (dMw_RIGHT_TRIGGER() && !isKeyCheck()) {
            return 2;
        }
        #endif
        else if (dMeter2Info_isTouchKeyCheck(0xE) && dMeter2Info_getMapStatus() != 6 && !isKeyCheck()) {
            return 3;
        }
    }

    return 0;
}

bool dMenu_Dmap_c::isSync() {
    if (mpDrawBg != NULL && (mpDrawBg->mpArchiveMount != NULL) && !mpDrawBg->mpArchiveMount->sync()) {
        return false;
    }

    if (mpDresArchiveMount != NULL && !mpDresArchiveMount->sync()) {
        return false;
    }

    return true;
}

bool dMenu_Dmap_c::isKeyCheck() {
    if (!isSync()) {
        return true;
    }

    return field_0x185;
}

void dMenu_Dmap_c::infoModeChange_init_proc() {
    field_0x10->hide();
    mSelFloor[getCurFloorPos()]->scaleAnimeStart(0);
    mSelFloor[getCurFloorPos()]->alphaAnimeStart(0);
    field_0x88[field_0x177]->scaleAnimeStart(0);
}

void dMenu_Dmap_c::infoModeChange_proc() {
    bool temp_r29 = mSelFloor[getCurFloorPos()]->scaleAnime(field_0x179, 1.0f, 0.9f, 0);
    bool temp_r27 = mSelFloor[getCurFloorPos()]->alphaAnime(field_0x179, 0xFF, 0x80, 0);
    bool temp_r28 = field_0x88[field_0x177]->scaleAnime(field_0x179, 0.9f, 1.0f, 0);

    if (temp_r29 == true && temp_r28 == true && temp_r27 == true) {
        field_0x94->show();

        Vec sp14 = field_0x7c[field_0x177]->getGlobalVtxCenter(false, 0);
        mpDrawBg->mpDrawCursor->setPos(sp14.x + field_0x104, sp14.y, field_0x7c[field_0x177]->getPanePtr(), 0);
        m_process = 0;
        m_itemSubProcess = 0;
    }

    iconMoveCalc();
}

void dMenu_Dmap_c::mapModeChange_init_proc() {
    field_0x94->hide();
    mSelFloor[getCurFloorPos()]->scaleAnimeStart(0);
    mSelFloor[getCurFloorPos()]->alphaAnimeStart(0);
    field_0x88[field_0x177]->scaleAnimeStart(0);
}

void dMenu_Dmap_c::mapModeChange_proc() {
    bool temp_r29 = mSelFloor[getCurFloorPos()]->scaleAnime(field_0x179, 0.9f, 1.0f, 0);
    bool temp_r27 = mSelFloor[getCurFloorPos()]->alphaAnime(field_0x179, 0x80, 0xFF, 0);
    bool temp_r28 = field_0x88[field_0x177]->scaleAnime(field_0x179, 1.0f, 0.9f, 0);

    if (temp_r29 == true && temp_r28 == true && temp_r27 == true) {
        field_0x10->show();

        Vec sp14 = mSelFloor[getDefaultCurFloorPos()]->getGlobalVtxCenter(false, 0);
        mpDrawBg->mpDrawCursor->setPos(sp14.x + field_0x104, sp14.y, mSelFloor[getDefaultCurFloorPos()]->getPanePtr(), 1);
        m_process = 1;
        field_0x17d = 0;

        (this->*map_init_process[field_0x17d])();
    }

    iconMoveCalc();
}

void dMenu_Dmap_c::lv5_talk_init_proc() {
    field_0x185 = true;
    dMsgObject_setTalkHeap(mpDrawBg->mpTalkHeap);
    mpDrawBg->mMsgFlow.init(NULL, 0x202, 0, NULL);
}

void dMenu_Dmap_c::lv5_talk_proc() {
    u16 msg_status = dMsgObject_c::getStatus();
    if (msg_status == 15 || msg_status == 16) {
        mpDrawBg->setAButtonString(0x408);
        mpDrawBg->setBButtonString(0);
    } else {
        mpDrawBg->setAButtonString(0);
        mpDrawBg->setBButtonString(0);
    }

    if (mpDrawBg->mMsgFlow.doFlow(NULL, NULL, 0)) {
        if (POINTER_OPT == 0) {
            if (mZoomState) {
                mpDrawBg->setAButtonString(0);
                mpDrawBg->setBButtonString(0x522);
            } else {
                mpDrawBg->setAButtonString(0x527);
                mpDrawBg->setBButtonString(0x3F9);
            }
        }

        m_process = 1;
    }
}

void dMenu_Dmap_c::mapMode_init_proc() {
    field_0x185 = false;
}

void dMenu_Dmap_c::mapMode_proc() {
    u8 temp_r30 = field_0x17d;
    if (mpDrawBg->field_0xdd4 == -99 && mpDrawBg->field_0xdd5 == 0xFF) {
        mSelStick->checkTrigger();
    }

    (this->*floor_move_process[field_0x17d])();

    if (field_0x17d != temp_r30) {
        (this->*floor_init_process[field_0x17d])();
    }
}

void dMenu_Dmap_c::floorSelect_init_proc() {}

void dMenu_Dmap_c::floorSelect_proc() {
    if (field_0x16d != field_0x16c) {
        floorChangeMode();
        return;
    }

    if (mSelStick->checkUpTrigger()) {
        if (field_0x16c != mFloorAll) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_FLOOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            field_0x16e = field_0x16c;
            field_0x16c++;
            field_0x16d = field_0x16c;
            field_0x179 = mSelStick->getYwaitTimer();
            mMapCtrl->setPlusNowStayFloorNo(1, field_0x179);
            field_0x17d = 1;
        }
    } else if (mSelStick->checkDownTrigger()) {
        if (field_0x16c != mBottomFloor) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_FLOOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            field_0x16e = field_0x16c;
            field_0x16c -= 1;
            field_0x16d = field_0x16c;
            field_0x179 = mSelStick->getYwaitTimer();
            field_0x17d = 1;
            mMapCtrl->setPlusNowStayFloorNo(-1, field_0x179);
        }
    } else if (mSelStick->checkLeftTrigger() && itemCarryCheck()) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        field_0x179 = mSelStick->getXwaitTimer();
        m_process = 2;
    }
}

int dMenu_Dmap_c::itemCarryCheck() {
    int var_r29 = 0;
    for (int i = 0; i < 3; i++) {
        if (field_0x174[i] != 0) {
            var_r29++;
        }
    }

    return var_r29;
}

void dMenu_Dmap_c::floorChange_init_proc() {
    field_0x10->hide();
    mSelFloor[getCurFloorPos()]->scaleAnimeStart(0);
    mSelFloor[getFloorPos(field_0x16e)]->scaleAnimeStart(0);
    mSelFloor[getCurFloorPos()]->alphaAnimeStart(0);
    mSelFloor[getFloorPos(field_0x16e)]->alphaAnimeStart(0);
}

void dMenu_Dmap_c::floorChange_proc() {
    bool temp_r29 = mSelFloor[getCurFloorPos()]->scaleAnime(field_0x179, 0.9f, 1.0f, 0);
    bool temp_r28 = mSelFloor[getFloorPos(field_0x16e)]->scaleAnime(field_0x179, 1.0f, 0.9f, 0);
    bool temp_r27 = mSelFloor[getCurFloorPos()]->alphaAnime(field_0x179, 0x80, 0xFF, 0);
    bool temp_r26 = mSelFloor[getFloorPos(field_0x16e)]->alphaAnime(field_0x179, 0xFF, 0x80, 0);

    if (temp_r29 == true && temp_r28 == true && temp_r27 == true && temp_r26 == true) {
        field_0x10->show();
        field_0x10->getPanePtr()->translate(mSelFloor[getCurFloorPos()]->getPanePtr()->getTranslateX(),
                                            mSelFloor[getCurFloorPos()]->getPanePtr()->getTranslateY());
        
        Vec sp14 = mSelFloor[getDefaultCurFloorPos()]->getGlobalVtxCenter(false, 0);
        mpDrawBg->mpDrawCursor->setPos(sp14.x + field_0x104, sp14.y, mSelFloor[getDefaultCurFloorPos()]->getPanePtr(), 1);
        field_0x17d = 0;
    }

    iconMoveCalc();
}

void dMenu_Dmap_c::zoomWait_init_proc() {}

void dMenu_Dmap_c::zoomWait_proc() {
    if (m_process == 1) {
        if (mDoCPd_c::getTrigA(PAD_1) && (((POINTER_OPT == 1 && mpDrawBg->field_0xdd3 != 0xFF) || POINTER_OPT == 0) && !dMeter2Info_isTouchKeyCheck(0xC))) {
            if (!mZoomState && mMapCtrl->isEnableZoomIn()) {
                field_0x17e = 1;
                field_0x181 = 0;
                return;
            }
        } else if (mDoCPd_c::getTrigB(PAD_1) && (POINTER_OPT == 1 || POINTER_OPT == 0) && !dMeter2Info_isTouchKeyCheck(0xC)) {
            if (mZoomState && mMapCtrl->isEnableZoomOut()) {
                field_0x17e = 2;
                return;
            }

            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
}

void dMenu_Dmap_c::zoomIn_init_proc() {
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

    if (mpDrawBg->field_0xdd3 != 0xFF) {
        mMapCtrl->initZoomIn(10, mpDrawBg->field_0xdb4, mpDrawBg->field_0xdb8);

        if (field_0x182 != 0) {
            field_0x182 = 0;
        }
    } else if (field_0x182 != 0) {
        mMapCtrl->initZoomIn(10);
        field_0x182 = 0;
    } else {
        mMapCtrl->initZoomInCenterHold(10);
    }

    mpDrawBg->iconScaleAnmInit(0.0f, 1.0f, 10);

    field_0x184 = 1;
    if (mMapCtrl->getDisableZoomMoveFlgX() == true && mMapCtrl->getDisableZoomMoveFlgZ() == true) {
        field_0x184 = 0;
    }

    mCMessageNum = 0;
    mJMessageNum = 0;
}

void dMenu_Dmap_c::zoomIn_proc() {
    bool temp_r30 = mMapCtrl->isEndZoomIn();
    bool temp_r29 = mpDrawBg->iconScaleAnm();
    bool temp_r28 = true;

    if (temp_r30 == true && temp_r29 == true && temp_r28 == true) {
        mZoomState = 1;
        mpDrawBg->setAButtonString(0);
        mpDrawBg->setBButtonString(0x522);
        mCMessageNum = 0x37B;
        mJMessageNum = 0x569;
        field_0x17e = 0;
    }
}

void dMenu_Dmap_c::zoomOut_init_proc() {
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMOUT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    mMapCtrl->initZoomOut(10);
    mpDrawBg->iconScaleAnmInit(1.0f, 0.0f, 10);

    field_0x184 = 1;
    if (mMapCtrl->getDisableZoomMoveFlgX() == true && mMapCtrl->getDisableZoomMoveFlgZ() == true) {
        field_0x184 = 0;
    }

    mCMessageNum = 0;
    mJMessageNum = 0;
}

void dMenu_Dmap_c::zoomOut_proc() {
    bool temp_r30 = mMapCtrl->isEndZoomOut();
    bool temp_r29 = mpDrawBg->iconScaleAnm();
    bool temp_r28 = true;

    if (temp_r30 == true && temp_r29 == true && temp_r28 == true) {
        mZoomState = 0;
        mpDrawBg->setAButtonString(0x527);
        mpDrawBg->setBButtonString(0x3F9);
        mCMessageNum = 0;
        mJMessageNum = 0;
        field_0x17e = 0;
    }
}
