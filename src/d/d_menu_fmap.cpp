#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_fmap.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "stdio.h"
#include "f_op/f_op_msg_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_menu_fmap2D.h"
#include "d/d_menu_fmap_map.h"
#include "d/d_menu_window.h"
#include "d/d_meter_HIO.h"
#include "d/d_meter_map.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_msg_scrn_explain.h"
#include "d/d_stage.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_midna.h"

static dMf_HIO_c g_fmHIO;

static dMenu_Fmap_c::process init_process[30] = {
    &dMenu_Fmap_c::all_map_init,
    &dMenu_Fmap_c::portal_demo2_init,
    &dMenu_Fmap_c::portal_demo3_init,
    &dMenu_Fmap_c::table_demo1_init,
    &dMenu_Fmap_c::zoom_all_to_region_init,
    &dMenu_Fmap_c::zoom_region_to_all_init,
    &dMenu_Fmap_c::region_map_init,
    &dMenu_Fmap_c::to_portal_warp_map_init,
    &dMenu_Fmap_c::portal_warp_map_init,
    &dMenu_Fmap_c::portal_warp_select_init,
    &dMenu_Fmap_c::portal_warp_forbid_init,
    &dMenu_Fmap_c::zoom_region_to_spot_init,
    &dMenu_Fmap_c::zoom_spot_to_region_init,
    &dMenu_Fmap_c::spot_map_init,
    &dMenu_Fmap_c::portal_demo1_init,
    &dMenu_Fmap_c::portal_demo4_init,
    &dMenu_Fmap_c::portal_demo5_init,
    &dMenu_Fmap_c::portal_demo6_init,
    &dMenu_Fmap_c::yamiboss_demo1_init,
    &dMenu_Fmap_c::yamiboss_demo2_init,
    &dMenu_Fmap_c::yamiboss_demo3_init,
    &dMenu_Fmap_c::yamiboss_demo4_init,
    &dMenu_Fmap_c::yamiboss_demo5_init,
    &dMenu_Fmap_c::light_demo1_init,
    &dMenu_Fmap_c::light_demo2_init,
    &dMenu_Fmap_c::table_demo2_init,
    &dMenu_Fmap_c::table_demo3_init,
    &dMenu_Fmap_c::howl_demo1_init,
    &dMenu_Fmap_c::howl_demo2_init,
    &dMenu_Fmap_c::howl_demo3_init,
};

static dMenu_Fmap_c::process move_process[30] = {
    &dMenu_Fmap_c::all_map_proc,
    &dMenu_Fmap_c::portal_demo2_move,
    &dMenu_Fmap_c::portal_demo3_move,
    &dMenu_Fmap_c::table_demo1_move,
    &dMenu_Fmap_c::zoom_all_to_region_proc,
    &dMenu_Fmap_c::zoom_region_to_all_proc,
    &dMenu_Fmap_c::region_map_proc,
    &dMenu_Fmap_c::to_portal_warp_map_proc,
    &dMenu_Fmap_c::portal_warp_map_proc,
    &dMenu_Fmap_c::portal_warp_select_proc,
    &dMenu_Fmap_c::portal_warp_forbid_proc,
    &dMenu_Fmap_c::zoom_region_to_spot_proc,
    &dMenu_Fmap_c::zoom_spot_to_region_proc,
    &dMenu_Fmap_c::spot_map_proc,
    &dMenu_Fmap_c::portal_demo1_move,
    &dMenu_Fmap_c::portal_demo4_move,
    &dMenu_Fmap_c::portal_demo5_move,
    &dMenu_Fmap_c::portal_demo6_move,
    &dMenu_Fmap_c::yamiboss_demo1_move,
    &dMenu_Fmap_c::yamiboss_demo2_move,
    &dMenu_Fmap_c::yamiboss_demo3_move,
    &dMenu_Fmap_c::yamiboss_demo4_move,
    &dMenu_Fmap_c::yamiboss_demo5_move,
    &dMenu_Fmap_c::light_demo1_move,
    &dMenu_Fmap_c::light_demo2_move,
    &dMenu_Fmap_c::table_demo2_move,
    &dMenu_Fmap_c::table_demo3_move,
    &dMenu_Fmap_c::howl_demo1_move,
    &dMenu_Fmap_c::howl_demo2_move,
    &dMenu_Fmap_c::howl_demo3_move,
};

dMf_HIO_c* dMf_HIO_c::mMySelfPointer;

dMf_HIO_c::dMf_HIO_c() {
    mMySelfPointer = this;
    mBackAnimeStep = 0.2f;
    mBaseBackAlpha = 60;
    mTopAnimeStep = 1.0f;
    mMoyaAlpha = 95;
}

const char* dMenuFmap_getStartStageName(void* i_fieldData) {
    static char virtual_stage[8];
    stage_stag_info_class* stag_info = dComIfGp_getStage()->getStagInfo();
    if (dStage_stagInfo_GetUpButton(stag_info) == 5) {
        dMenu_Fmap_virtual_stage_data_c* data_list = (dMenu_Fmap_virtual_stage_data_c*)
                ((int)i_fieldData + ((dMenu_Fmap_field_data_c*)i_fieldData)->mVirtualStageOffset);
        dMenu_Fmap_virtual_stage_data_c::data* data = data_list->mData;
        for (int i = 0; i < data_list->mCount; i++) {
            if (!strcmp(dComIfGp_getStartStageName(), data[i].mStageName)) {
                // !@bug: probably supposed to be data[i].mVirtualStageName, but doesn't matter
                // because the two entries have the same virtual stage name
                strcpy(virtual_stage, data->mVirtualStageName);
                return virtual_stage;
            }
        }
    }
    return dComIfGp_getStartStageName();
}

dMenu_Fmap_c* dMenu_Fmap_c::MyClass;

dMenu_Fmap_c::dMenu_Fmap_c(JKRExpHeap* i_heap, STControl* i_stick, CSTControl* i_cstick,
                           u8 i_process, u8 i_regionCursor, u8 i_stageCursor, f32 i_stageTransX,
                           f32 i_stageTransZ, u8 i_panDirection) {
    MyClass = this;
    mpHeap = i_heap;
    mpStick = i_stick;
    mpCStick = i_cstick;
    mPortalNo = -1;
    mErrorSound = false;
    mStartStageNo = -1;
    mStageCursor = -1;
    mRoomCursor = -1;
    mFlashRooms = NULL;
    mFlashRoomCount = 0;
    field_0x310 = 0;
    mResetAreaName = false;
    mAreaNameSet = false;
    mRegionNo = -1;
    mStayStageNo = -1;
    field_0x30b = false;
    field_0x30f = 0;
    mOldRegionCursor = i_regionCursor;
    mOldStageCursor = i_stageCursor;
    mOldStageTransX = i_stageTransX;
    mOldStageTransZ = i_stageTransZ;
    mpMountArchive = NULL;
    mpResArchive = NULL;
    mpDraw2DBack = NULL;
    mpDraw2DTop = NULL;
    mpMenuFmapMap = NULL;
    mpFieldDat = NULL;
    mSpotTextureFadeAlpha = 0.0f;
    mpFmapMapRes = NULL;
    mpPortalDat = NULL;
    mpWorldData = NULL;
    field_0xb4 = 0;
    mDataNumMax = 0;

    for (int i = 0; i < 8; i++) {
        mpRegionData[i] = NULL;
        mpStageData[i] = NULL;
        mpRoomData[i] = NULL;
    }

    field_0x305 = false;

    for (int i = 0; i < 8; i++) {
        field_0x168[i] = 0.0f;
        field_0x188[i] = 0.0f;
        field_0x128[i] = 0.0f;
        field_0x148[i] = 0.0f;
    }

    mpTalkHeap = JKRCreateExpHeap(0x32000, mpHeap, false);
    field_0x200 = 0;
    mIsWarpMap = false;

    mProcess = PROC_REGION_MAP;
    strcpy(mMarkedStageName, "");
    if (g_fmapHIO.mpArcData != NULL && g_fmapHIO.mpArcData->isMounted()) {
        mProcess = i_process;
        mpMapArchive = g_fmapHIO.mpArcData;
    } else {
        mpMapArchive = dComIfGp_getFieldMapArchive2();
    }
    mPrevProcess = mProcess;
    mPrevProcessAlt = mProcess;

    if (dMeter2Info_getMapStatus() == 4) {
        mIsWarpMap = false;
        mProcess = PROC_PORTAL_DEMO1;
    } else if (dMeter2Info_getMapStatus() == 5) {
        mIsWarpMap = false;
        mProcess = PROC_YAMIBOSS_DEMO1;
    } else if (dMeter2Info_getMapStatus() == 8) {
        mIsWarpMap = false;
        mProcess = PROC_LIGHT_DEMO1;
    } else if (dMeter2Info_getMapStatus() == 3) {
        mIsWarpMap = true;
        mProcess = PROC_ALL_MAP;
    } else if (dMeter2Info_getMapStatus() == 7) {
        i_panDirection = 0xff;
        mIsWarpMap = false;
        mProcess = PROC_TABLE_DEMO1;
    } else if (dMeter2Info_getMapStatus() == 9) {
        mIsWarpMap = false;
        mProcess = PROC_HOWL_DEMO1;
    }

    if (i_panDirection == 1) {
        mTransX = -608.0f;
        mTransY = 0.0f;
    } else if (i_panDirection == 3) {
        mTransX = 608.0f;
        mTransY = 0.0f;
    } else if (i_panDirection == 2) {
        mTransX = 0.0f;
        mTransY = -448.0f;
    } else if (i_panDirection == 0) {
        mTransX = 0.0f;
        mTransY = 448.0f;
    } else {
        mTransX = 0.0f;
        mTransY = 0.0f;
    }

    mAlphaRatio = 0.0f;
    mPanDirection = i_panDirection;
    field_0x210 = 0;
    mDisplayFrame = 0;
    mZoomLevel = 0;

    for (int i = 0; i < 8; i++) {
        mRegionOffsetX[i] = 0.0f;
        mRegionOffsetZ[i] = 0.0f;
        mRegionTexture[i] = NULL;
        mTitleName[i] = 0;
    }

    for (int i = 0; i < 20; i++) {
        mAreaName[i] = 0;
    }

    field_0x1ec = 0.0f;
    mSpotNum = 0;
    mAllTitleName = 0;

    resetRoomDataBit();
}

dMenu_Fmap_c::~dMenu_Fmap_c() {
    mpHeap->getTotalFreeSize();
    if (mpFieldDat != NULL) {
        mpHeap->free(mpFieldDat);
        mpFieldDat = NULL;
    }
    if (mpPortalDat != NULL) {
        mpHeap->free(mpPortalDat);
        mpPortalDat = NULL;
    }

    for (int i = 0; i < 8; i++) {
        if (i == 7) {
            if (mRegionTexture[i] != NULL) {
                mpResArchive->removeResource(mRegionTexture[i]);
            }
            mRegionTexture[i] = NULL;
        } else {
            if (mRegionTexture[i] != NULL) {
                mpMapArchive->removeResource(mRegionTexture[i]);
                mRegionTexture[i] = NULL;
                mRegionTexture[i] = NULL;
            }
        }
    }

    removeAreaData();

    if (mpDraw2DBack != NULL) {
        delete mpDraw2DBack;
        mpDraw2DBack = NULL;
    }
    if (mpDraw2DTop != NULL) {
        delete mpDraw2DTop;
        mpDraw2DTop = NULL;
    }
    if (mpMenuFmapMap != NULL) {
        mpMenuFmapMap->_delete();
        delete mpMenuFmapMap;
        mpMenuFmapMap = NULL;
    }

    dComIfGp_getFmapResArchive()->removeResourceAll();
    if (mpMapArchive != NULL) {
        mpMapArchive->removeResourceAll();
    }

    MyClass = NULL;

    if (mpTalkHeap != NULL) {
        if (dMsgObject_getTalkHeap() == mpTalkHeap) {
            dMsgObject_setTalkHeap(NULL);
        }
        mDoExt_destroyExpHeap(mpTalkHeap);
        mpTalkHeap = NULL;
    }

    if (mpMountArchive != NULL) {
        JKRUnmountArchive(mpMountArchive->getArchive());
        mpMountArchive->destroy();
        mpMountArchive = NULL;
    }

    if (mpResArchive != NULL) {
        JKRUnmountArchive(mpResArchive);
        mpResArchive = NULL;
    }
}

void dMenu_Fmap_c::_create() {
    mpHeap->getTotalFreeSize();
    mpDraw2DBack = new dMenu_Fmap2DBack_c();
    mpDraw2DBack->setRegionCursor(dComIfGp_getNowLevel() - 1);
    mpDraw2DTop = new dMenu_Fmap2DTop_c(mpHeap, mpStick);
    mpMenuFmapMap = new dMenu_FmapMap_c();
    mpMenuFmapMap->_create(dMeter2Info_get2DWidth(), dMeter2Info_get2DHeight(),
                           dMeter2Info_get2DWidth(), dMeter2Info_get2DHeight(), mpFmapMapRes);
    
    readFieldMapData((void**)&mpFieldDat, "dat/field.dat", false, false);
    decodeFieldMapData();
    readFieldMapData((void**)&mpPortalDat, "dat/portal.dat", false, false);
    decodePortalData();
    mpDraw2DTop->setPortalBin(mpPortalDat);

    mpWorldData = NULL;
    for (int i = 0; i < 8; i++) {
        mpRegionData[i] = NULL;
        mpStageData[i] = NULL;
        mpRoomData[i] = NULL;
    }

    field_0x305 = false;

    for (int i = 0; i < 8; i++) {
        field_0x168[i] = 0.0f;
        field_0x188[i] = 0.0f;
        field_0x128[i] = 0.0f;
        field_0x148[i] = 0.0f;
    }

    if (mIsWarpMap == true) {
        mZoomLevel = 0;
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_LEFTRIGHT, 0.0f);
        mpDraw2DTop->setCrossLRString(0);
    } else {
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_LEFTRIGHT, 1.0f);
        mpDraw2DTop->setCrossLRString(0x3f9);
    }
    mpDraw2DBack->setArrowAlpha(1.0f);

    u8 process = mProcess;
    if (process == PROC_REGION_MAP || process == PROC_PORTAL_WARP_MAP
        || process == PROC_PORTAL_DEMO1 || process == PROC_YAMIBOSS_DEMO1
        || process == PROC_LIGHT_DEMO1 || process == PROC_HOWL_DEMO1)
    {
        if (g_fmapHIO.field_0x2f9) {
            mpDraw2DBack->setRegionCursor(mOldRegionCursor);
        }

        process = mProcess;
        if (process == PROC_HOWL_DEMO1) {
            mProcess = PROC_ZOOM_REGION_TO_ALL;
            readWorldData(mpDraw2DBack->getRegionCursor() + 1);
            mZoomLevel = 0;
            mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
            mProcess = process;
            getPlayerPos2D();
            mpDraw2DBack->regionMapMove(mpStick);
            drawIcon(field_0x1ec, true);
            mpDraw2DBack->setRegionCursor(getHowlRegionID() - 1);
        }

        readWorldData(mpDraw2DBack->getRegionCursor() + 1);
        mZoomLevel = 10;
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);

        if (mIsWarpMap == true) {
            mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
            mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
            mpDraw2DTop->setBButtonString(0x522, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            mpDraw2DTop->setZButtonString(0x52a, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            mpDraw2DTop->set3DStickString(0x526);
        } else {
            mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UPDOWN, 0.0f);
            mpDraw2DTop->setBButtonString(0x522, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            mpDraw2DTop->setAButtonString(0x527, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            mpDraw2DTop->set3DStickString(0x528);
            mpDraw2DBack->iconScale(0, 1.0f, 1.0f, 0.0f);
            if (mProcess != PROC_YAMIBOSS_DEMO1) {
                arrowPosInit();
            } else {
                yamibossArrowPosInit();
            }
        }
    } else if (process == PROC_SPOT_MAP) {
        if (g_fmapHIO.field_0x2f9) {
            mpDraw2DBack->setRegionCursor(mOldRegionCursor);
        }
        readWorldData(mpDraw2DBack->getRegionCursor() + 1);
        mZoomLevel = 10;
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
        if (g_fmapHIO.field_0x2f9) {
            mpDraw2DBack->setSpotCursor(mOldStageCursor);
        }
        mpDraw2DBack->zoomMapCalc2((f32)mZoomLevel / 10.0f);
        mpDraw2DBack->setStageTransForceXZ(mOldStageTransX, mOldStageTransZ);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
        mpDraw2DTop->setBButtonString(0x522, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        mpDraw2DTop->set3DStickString(0x528);
        mpDraw2DBack->iconScale(0, 0.0f, 0.0f, 1.0f);
        arrowPosInit();
    } else {
        mProcess = PROC_ZOOM_REGION_TO_ALL;
        readWorldData(mpDraw2DBack->getRegionCursor() + 1);
        mZoomLevel = 0;
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
        mProcess = process;
        getPlayerPos2D();
        mpDraw2DBack->regionMapMove(mpStick);
        drawIcon(field_0x1ec, true);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
        mpDraw2DTop->setBButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        mpDraw2DTop->setAButtonString(0x527, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        mpDraw2DTop->set3DStickString(0x524);
        mpDraw2DBack->iconScale(0, 1.0f, 1.0f, 0.0f);
        arrowPosInit();
    }

    if (mProcess == PROC_PORTAL_DEMO1 || mProcess == PROC_YAMIBOSS_DEMO1
        || mProcess == PROC_TABLE_DEMO1 || mProcess == PROC_LIGHT_DEMO1
        || mProcess == PROC_HOWL_DEMO1)
    {
        mpDraw2DBack->offArrowDrawFlag();
    }

    (this->*init_process[mProcess])();

    if (mProcess != PROC_HOWL_DEMO1) {
        getPlayerPos2D();
    }

    g_fmapHIO.field_0x2f9 = false;
}

void dMenu_Fmap_c::_delete() {
    /* empty function */
}

void dMenu_Fmap_c::_move() {
    u8 process = mProcess;
    field_0x310 = 0;
    mpDraw2DBack->clearIconInfo();

    (this->*move_process[mProcess])();
    
    mpDraw2DTop->_execute();
    mpDraw2DBack->setBaseBackAlpha(g_fmHIO.mBaseBackAlpha);
    mpDraw2DBack->btkAnimeLoop(g_fmHIO.mBackAnimeStep);
    mpDraw2DTop->setMoyaAlpha(g_fmHIO.mMoyaAlpha);
    mpDraw2DTop->btkAnimeLoop(g_fmHIO.mTopAnimeStep);

    if (mProcess != process) {
        (this->*init_process[mProcess])();
    }

    if (mProcess == PROC_PORTAL_WARP_FORBID) {
        mpDraw2DBack->calcBackAlpha(true);
    } else {
        mpDraw2DBack->calcBackAlpha(false);
    }

    mpDraw2DBack->setAllTrans(mTransX, mTransY);
    mpDraw2DTop->setAllTrans(mTransX, mTransY);

    if (g_fmapHIO.mRangeCheck) {
        drawDebugStage();
    }

    if (mSpotTextureFadeAlpha != 1.0f) {
        cLib_addCalc2(&mSpotTextureFadeAlpha, 1.0f, 0.4f, 0.5f);
        if (fabsf(mSpotTextureFadeAlpha - 1.0f) < 0.1f) {
            mSpotTextureFadeAlpha = 1.0f;
        }
        mpDraw2DBack->setSpotTextureFadeAlpha(mSpotTextureFadeAlpha);
    }
}

void dMenu_Fmap_c::_draw() {
    if (mpDraw2DBack != NULL && mpDraw2DTop != NULL) {
        if (field_0x305) {
            mpMenuFmapMap->setRendering(mpWorldData, mStartStageNo,
                                        mpDraw2DBack->getRenderingPosX(),
                                        mpDraw2DBack->getRenderingPosZ(),
                                        mpDraw2DBack->getRenderingScale(),
                                        mpDraw2DBack->getMapZoomRate());
            mpDraw2DBack->setStageInfo(mSpotNum, mpMenuFmapMap);
            drawIcon(field_0x1ec, false);
            if (mProcess == PROC_ZOOM_REGION_TO_SPOT || mProcess == PROC_ZOOM_SPOT_TO_REGION
                || mProcess == PROC_YAMIBOSS_DEMO4 || mProcess == PROC_LIGHT_DEMO1
                || mProcess == PROC_TABLE_DEMO2 || mProcess == PROC_HOWL_DEMO1)
            {
                f32 scale = 1.0f - field_0x1ec;
                mpDraw2DBack->iconScale(0, scale, scale, 1.0f - scale);
            }
        } else {
            drawPortalIcon();
            if (mProcess == PROC_ZOOM_ALL_TO_REGION || mProcess == PROC_ZOOM_REGION_TO_ALL) {
                f32 scale = 1.0f - (f32)mZoomLevel / 10.0f;
                mpDraw2DBack->iconScale(0, scale, scale, 1.0f - scale);
            }
        }

        mpDraw2DBack->setMapDrawFlag(mpMenuFmapMap->isDrawEnable());
        dComIfGd_set2DOpa(mpDraw2DBack);
        dComIfGd_set2DOpaTop(mpDraw2DTop);
    }
}

u8 dMenu_Fmap_c::getNextStatus(u8* param_0) {
    u8 ret = 8;
    *param_0 = 0;

    if (mProcess == PROC_ALL_MAP && dMw_B_TRIGGER()) {
        if (mPanDirection == 3) {
            mPanDirection = 1;
            dMeter2Info_setMapStatus(0);
            dMeter2Info_setMapKeyDirection(0x400);
        } else {
            mPanDirection = 3;
            if (dMeterMap_c::isEnableDispMapAndMapDispSizeTypeNo()) {
                dMeter2Info_setMapStatus(1);
            } else {
                dMeter2Info_setMapStatus(0);
            }
            dMeter2Info_setMapKeyDirection(0x200);
        }

        ret = 9;
        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_CLOSE_L, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_set2DVibrationM();
    }

    if (mProcess == PROC_PORTAL_WARP_MAP && dMeter2Info_getWarpStatus() == 3) {
        if (mPanDirection == 3) {
            mPanDirection = 1;
            dMeter2Info_setMapStatus(0);
            dMeter2Info_setMapKeyDirection(0x400);
        } else {
            mPanDirection = 3;
            if (dMeterMap_c::isEnableDispMapAndMapDispSizeTypeNo()) {
                dMeter2Info_setMapStatus(1);
            } else {
                dMeter2Info_setMapStatus(0);
            }
            dMeter2Info_setMapKeyDirection(0x200);
        }

        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_CLOSE_L, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_set2DVibrationM();
        return 9;
    } else if ((mProcess == PROC_ALL_MAP || mProcess == PROC_REGION_MAP
                || mProcess == PROC_PORTAL_WARP_MAP || mProcess == PROC_SPOT_MAP) && isSync()) {
        if (dMw_LEFT_TRIGGER()) {
            if (dMeterMap_c::isEnableDispMapAndMapDispSizeTypeNo()) {
                dMeter2Info_setMapStatus(1);
            } else {
                dMeter2Info_setMapStatus(0);
            }
            mPanDirection = 3;
            dMeter2Info_setMapKeyDirection(0x200);
            Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_CLOSE_L, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibrationM();
            return 9;
        } else if (dMw_RIGHT_TRIGGER()) {
            dMeter2Info_setMapStatus(0);
            mPanDirection = 1;
            dMeter2Info_setMapKeyDirection(0x400);
            Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_CLOSE_L, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibrationM();
            return 9;
        }
    }

    return ret;
}

bool dMenu_Fmap_c::isSync() {
    if (mpMountArchive != NULL && !mpMountArchive->sync()) {
        return false;
    }
    return true;
}

void dMenu_Fmap_c::all_map_init() {
    if (mIsWarpMap == true) {
        setTitleName(0x525);
        mpDraw2DTop->setZButtonString(0x52a, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    } else {
        setTitleName(mAllTitleName);
        mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    }

    mResetAreaName = true;

    if (!mAreaNameSet) {
        setAreaNameZero();
    } else {
        setAreaName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    }

    mRegionNo = mpDraw2DBack->getRegionCursor();
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    mpDraw2DTop->setBButtonString(0x3f9, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0x527, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0x524);
    mpDraw2DTop->setCrossLRString(0x3f9);
}

void dMenu_Fmap_c::all_map_proc() {
    mpStick->checkTrigger();

    u8 region = mpDraw2DBack->getSelectRegion();
    bool region_change = false;
    if (mRegionNo != region) {
        mRegionNo = region;
        region_change = true;
    }

    if (region != 0xff && mpDraw2DBack->isShowRegion(region)) {
        if (getRegionStageNum(region + 1) == 0) {
            mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        } else {
            mpDraw2DTop->setAButtonString(0x527, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        }
        if (region_change) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_AREA_SELECT, NULL, 0, 0,
                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else {
        mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    }

    if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        if (region == 0xff || !mpDraw2DBack->isShowRegion(region)) {
            if (!mErrorSound) {
                Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                mErrorSound = true;
            }
        } else {
            mpDraw2DBack->setRegionCursor(region);
            if (getRegionStageNum(mpDraw2DBack->getRegionCursor() + 1) == 0) {
                if (!mErrorSound) {
                    Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0,
                                             1.0f, 1.0f, -1.0f, -1.0f, 0);
                    mErrorSound = true;
                }
            } else {
                mpDraw2DBack->calcDrawPriority();
                setProcess(PROC_ZOOM_ALL_TO_REGION);
                if (mErrorSound == true) {
                    mErrorSound = false;
                }
            }
        }
    } else if (dMw_B_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        if (mErrorSound == true) {
            mErrorSound = false;
        }
    } else if (dMw_Z_TRIGGER() && mpDraw2DTop->isWarpAccept()) {
            /* dSv_event_flag_c::F_0265 - Arbiter's Grounds - Arbiter's Grounds clear */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x109])
                /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
            && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x169]))
        {
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
            mpDraw2DTop->getScrnExplainPtr()->openExplain(0x8b4, 0, 0, 0xff, true);
            mPrevProcessAlt = mProcess;
            setProcess(PROC_PORTAL_WARP_FORBID);
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else if (mpDraw2DTop->checkPlayerWarpAccept()) {
            if (mIsWarpMap == true) {
                mIsWarpMap = false;
                mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
                Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_OFF, NULL, 0, 0,
                                         1.0f, 1.0f, -1.0f, -1.0f, 0);
            } else {
                mIsWarpMap = true;
                mpDraw2DTop->setZButtonString(0x52a, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
                Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_ON, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        } else {
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
                /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x68])
                    /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0xFA]))
            {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x85d, 0, 0, 0xff, true);
            } else if (!mpDraw2DTop->checkWarpAcceptCannon()) {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7de, 0, 0, 0xff, true);
                /* dSv_event_flag_c::F_0787 - Kakariko Village - Stopped by Midna when trying to warp the sky cannon */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x313]);
            } else if (!mpDraw2DTop->checkWarpAcceptRegion4()) {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7dd, 0, 0, 0xff, true);
            } else {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51d, 0, 0, 0xff, true);
            }
            mPrevProcessAlt = mProcess;
            setProcess(PROC_PORTAL_WARP_FORBID);
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else {
        u8 region1 = mpDraw2DBack->getSelectRegion();
        mpDraw2DBack->allmap_move2(mpStick);
        u8 region2 = mpDraw2DBack->getSelectRegion();
        if (region1 != region2 || mResetAreaName) {
            mResetAreaName = false;
            if (mpDraw2DBack->getSelectRegion() != 0xff
                && mpDraw2DBack->isShowRegion(mpDraw2DBack->getSelectRegion()))
            {
                setAreaName(mTitleName[mpDraw2DBack->getSelectRegion()]);
                mpDraw2DBack->setSpotCursor(0);

            } else {
                setAreaNameZero();
            }
        }
        if (mErrorSound == true) {
            mErrorSound = false;
        }
    }
}

void dMenu_Fmap_c::zoom_all_to_region_init() {
    if (!readWorldData(mpDraw2DBack->getRegionCursor() + 1)) {
        setProcess(PROC_ALL_MAP);
        if (!mErrorSound) {
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            mErrorSound = true;
        }
    } else {
        mZoomLevel = 0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
}

void dMenu_Fmap_c::zoom_all_to_region_proc() {
    mZoomLevel++;
    mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);

    if (mIsWarpMap == true) {
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    } else {
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    }

    if (mZoomLevel >= 10) {
        if (mIsWarpMap == true) {
            setProcess(PROC_PORTAL_WARP_MAP);
        } else {
            setProcess(PROC_REGION_MAP);
        }
    }
}

void dMenu_Fmap_c::zoom_region_to_all_init() {
    mZoomLevel = 10;
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMOUT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Fmap_c::zoom_region_to_all_proc() {
    mZoomLevel--;
    mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);

    if (mIsWarpMap == true) {
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    } else {
        mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    }

    if (mZoomLevel <= 0) {
        if (mpDraw2DBack->getAllPathShowFlag()) {
            removeAreaData();
            readWorldData(0xff);
        }
        setProcess(PROC_ALL_MAP);
    }
}

void dMenu_Fmap_c::region_map_init() {
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    mResetAreaName = true;

    if (!mAreaNameSet) {
        setAreaNameZero();
    } else {
        setAreaName(mAreaName[mpDraw2DBack->getSpotCursor()]);
    }

    mStageCursor = mpDraw2DBack->getSpotCursor();
    mRoomCursor = -1;
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UPDOWN, 0.0f);
    mpDraw2DTop->setBButtonString(0x522, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0x527, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0x528);
    mpDraw2DTop->setCrossLRString(0x3f9);
}

void dMenu_Fmap_c::region_map_proc() {
    bool r29 = false;
    mpDraw2DTop->setAButtonString(0x527, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    r29 = true;

    if (dMw_B_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        setProcess(PROC_ZOOM_REGION_TO_ALL);
        mpMenuFmapMap->setFlashOff();
    } else if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        if (r29) {
            setProcess(PROC_ZOOM_REGION_TO_SPOT);
        } else if (!mErrorSound) {
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            mErrorSound = true;
        }
    } else if (dMw_Z_TRIGGER() && mpDraw2DTop->isWarpAccept()) {
            /* dSv_event_flag_c::F_0265 - Arbiter's Grounds - Arbiter's Grounds clear */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x109])
                /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
            && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x169]))
        {
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
            mpDraw2DTop->getScrnExplainPtr()->openExplain(0x8b4, 0, 0, 0xff, true);
            mPrevProcessAlt = mProcess;
            setProcess(PROC_PORTAL_WARP_FORBID);
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else if (mpDraw2DTop->checkPlayerWarpAccept()) {
            mIsWarpMap = true;
            setProcess(PROC_TO_PORTAL_WARP_MAP);
            mpMenuFmapMap->setFlashOff();
            mpDraw2DTop->setZButtonString(0x52a, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_ON, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else {
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
                /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x68])
                    /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0xFA]))
            {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x85d, 0, 0, 0xff, true);
            } else if (!mpDraw2DTop->checkWarpAcceptCannon()) {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7de, 0, 0, 0xff, true);
                /* dSv_event_flag_c::F_0787 - Kakariko Village - Stopped by Midna when trying to warp the sky cannon */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x313]);
            } else if (!mpDraw2DTop->checkWarpAcceptRegion4()) {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7dd, 0, 0, 0xff, true);
            } else {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51d, 0, 0, 0xff, true);
            }
            mPrevProcessAlt = mProcess;
            setProcess(PROC_PORTAL_WARP_FORBID);
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else {
        if (g_fmapHIO.mDisplayReferenceArea) {
            mpDraw2DBack->zoomMapCalc(1.0f);
        }
        mpDraw2DBack->regionMapMove(mpStick);
        int stage_no, room_no;
        f32 pos_x = mpDraw2DBack->getArrowPos2DX() - mDoGph_gInf_c::getMinXF()
                                                    - mDoGph_gInf_c::getWidthF() * 0.5f;
        f32 pos_y = mpDraw2DBack->getArrowPos2DY() - mDoGph_gInf_c::getHeightF() * 0.5f;
        mpMenuFmapMap->getPointStagePathInnerNo(getNowFmapRegionData(), pos_x, pos_y,
                                                mStayStageNo, &stage_no, &room_no);
        if (mStageCursor != stage_no || mRoomCursor != room_no || mResetAreaName) {
            mStageCursor = stage_no;
            mRoomCursor = room_no;
            u16 area_name = checkStRoomData();
            if (mStageCursor >= 0) {
                mpDraw2DBack->setSpotCursor(mStageCursor);
                if (area_name != 0xffff) {
                    setAreaName(area_name);
                } else {
                    setAreaName(mAreaName[mStageCursor]);
                }
            } else {
                setAreaNameZero();
            }
        }
        if (mErrorSound == true) {
            mErrorSound = false;
        }
    }

    if (mStageCursor >= 0) {
        setFlash(mStageCursor, true);
    } else {
        mpMenuFmapMap->setFlashOff();
    }
}

void dMenu_Fmap_c::to_portal_warp_map_init() {
    mZoomLevel = 10;
}

void dMenu_Fmap_c::to_portal_warp_map_proc() {
    mZoomLevel--;
    if (mZoomLevel <= 0) {
        setProcess(PROC_PORTAL_WARP_MAP);
    }
}

void dMenu_Fmap_c::portal_warp_map_init() {
    if (mPrevProcess != PROC_PORTAL_WARP_SELECT) {
        mPortalNo = 0xff;
        setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
        if (mPortalNo == 0xff) {
            setAreaNameZero();
        }
    }

    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    mpDraw2DTop->setBButtonString(0x522, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0x52a, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0x526);
}

void dMenu_Fmap_c::portal_warp_map_proc() {
    mpStick->checkTrigger();

    if (dMw_B_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        setProcess(PROC_ZOOM_REGION_TO_ALL);
        if (mErrorSound == true) {
            mErrorSound = false;
        }
    } else if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        if (mPortalNo != 0xff) {
            dMenu_Fmap_portal_data_c::data* portals = mpPortalDat->mData;
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
            dMsgObject_setPortalMessageID(portals[mPortalNo].mMessageID);

                /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x68])
                    /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0xFA]))
            {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x85d, 0, 0, 0xff, true);
                       /* dSv_event_flag_c::F_0786 - Kakariko Village - Forced conversation with Shad when he sees the sky cannon */
            } else if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x312])
                    /* dSv_event_flag_c::F_0785 - Kakariko Village - Shad leaves after attempting to warp sky cannon */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x311]))
            {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7de, 0, 0, 0xff, true);
                /* dSv_event_flag_c::F_0787 - Kakariko Village - Stopped by Midna when trying to warp the sky cannon */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x313]);
            } else if (mPortalNo == 6) {
                if ((!strcmp(dComIfGp_getStartStageName(), "F_SP124")
                                && ((daMidna_c*)daPy_py_c::getMidnaActor())->checkPortalObjRide())
                       /* dSv_event_flag_c::M_092 - Hyrule Field - Warped Eldin Bridge */
                    || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x7D]))
                {
                    mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51e, 2, 0, 0xff, true);
                } else {
                    mpDraw2DTop->getScrnExplainPtr()->openExplain(0x53a, 0, 0, 0xff, true);
                }
            } else if (mPortalNo == 3) {
                if ((dMeter2Info_getMapStatus() == 3
                                && dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 2
                                && dComIfGs_isOneZoneSwitch(0xf, -1))
                       /* dSv_event_flag_c::M_018 - Main Event - Brought Kakariko bridge back to original location */
                    || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x33]))
                {
                    mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51e, 2, 0, 0xff, true);
                } else {
                    mpDraw2DTop->getScrnExplainPtr()->openExplain(0x53b, 0, 0, 0xff, true);
                }
            } else {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51e, 2, 0, 0xff, true);
            }

            setProcess(PROC_PORTAL_WARP_SELECT);
            if (mErrorSound == true) {
                mErrorSound = false;
            }
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else {
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else if (dMw_Z_TRIGGER()) {
        mIsWarpMap = false;
        setProcess(PROC_REGION_MAP);
        mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
        Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_OFF, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else {
        portalWarpMapMove(mpStick);
        if (mErrorSound == true) {
            mErrorSound = false;
        }
    }

    setFlash(mpDraw2DBack->getSpotCursor(), false);
}

void dMenu_Fmap_c::portal_warp_select_init() {
    /* empty function */
}

void dMenu_Fmap_c::portal_warp_select_proc() {
    mpDraw2DTop->getScrnExplainPtr()->move();

    if (mpDraw2DTop->getScrnExplainPtr()->getStatus() == 0) {
        mpDraw2DTop->deleteExplain();

        if (dMeter2Info_getWarpStatus() == 3) {
            dMenu_Fmap_portal_data_c::data* portals = mpPortalDat->mData;
            u8 region = mpDraw2DBack->getRegionCursor();

            dMeter2Info_setWarpInfo(portals[mPortalNo].mStageName,
                cXyz(portals[mPortalNo].mPosition.x + mpDraw2DBack->getRegionOriginX(region),
                     portals[mPortalNo].mPosition.y,
                     portals[mPortalNo].mPosition.z + mpDraw2DBack->getRegionOriginZ(region)),
                0, portals[mPortalNo].mRoomNo, 0, portals[mPortalNo].mWarpPlayerNo);

            dComIfGp_SelectWarpPt_set(portals[mPortalNo].mSelectWarpPt);
            setProcess(PROC_PORTAL_WARP_MAP);
        } else if (mPrevProcess == PROC_PORTAL_DEMO5) {
            setProcess(PROC_PORTAL_DEMO5);
        } else {
            setProcess(PROC_PORTAL_WARP_MAP);
        }
    }

    setFlash(mpDraw2DBack->getSpotCursor(), false);
}

void dMenu_Fmap_c::portal_warp_forbid_init() {
    mpDraw2DTop->setBButtonString(0x3f9, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0);
    mpDraw2DTop->setCrossLRString(0);
}

void dMenu_Fmap_c::portal_warp_forbid_proc() {
    mpDraw2DTop->getScrnExplainPtr()->move();
    if (mpDraw2DTop->getScrnExplainPtr()->getStatus() == 0) {
        mpDraw2DTop->deleteExplain();
        setProcess(mPrevProcessAlt);
    }
    setFlash(mpDraw2DBack->getSpotCursor(), false);
}

void dMenu_Fmap_c::zoom_region_to_spot_init() {
    mZoomLevel = 0;
    field_0x1ec = 0.0f;
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Fmap_c::zoom_region_to_spot_proc() {
    mZoomLevel++;
    field_0x1ec = (f32)mZoomLevel / 10.0f;
    mpDraw2DBack->zoomMapCalc2(field_0x1ec);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);

    if (mZoomLevel >= 10) {
        setProcess(PROC_SPOT_MAP);
    }

    if (mStageCursor >= 0) {
        setFlash(mStageCursor, true);
    } else {
        mpMenuFmapMap->setFlashOff();
    }
}

void dMenu_Fmap_c::zoom_spot_to_region_init() {
    mZoomLevel = 10;
    field_0x1ec = 1.0f;
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMOUT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Fmap_c::zoom_spot_to_region_proc() {
    mZoomLevel--;
    field_0x1ec = (f32)mZoomLevel / 10.0f;
    mpDraw2DBack->zoomMapCalc2(field_0x1ec);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);

    if (mZoomLevel <= 0) {
        if (mIsWarpMap == true) {
            setProcess(PROC_PORTAL_WARP_MAP);
        } else {
            setProcess(PROC_REGION_MAP);
        }
    }

    if (mStageCursor >= 0) {
        setFlash(mStageCursor, true);
    } else {
        mpMenuFmapMap->setFlashOff();
    }
}

void dMenu_Fmap_c::spot_map_init() {
    field_0x1ec = 1.0f;
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    mResetAreaName = true;

    if (!mAreaNameSet) {
        setAreaNameZero();
    } else {
        setAreaName(mAreaName[mpDraw2DBack->getSpotCursor()]);
    }

    mStageCursor = mpDraw2DBack->getSpotCursor();
    mRoomCursor = -1;

    mpDraw2DBack->resetDrug();
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    mpDraw2DTop->setBButtonString(0x522, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0x529, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0x528);
    mpDraw2DTop->setCrossLRString(0x3f9);
}

void dMenu_Fmap_c::spot_map_proc() {
    if (dMw_B_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        setProcess(PROC_ZOOM_SPOT_TO_REGION);
        if (mErrorSound == true) {
            mErrorSound = false;
        }
    } else if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)
        && dMeter2Info_getMeterClass()->getMeterDrawPtr()->getInsideObjCheck() != 1)
    {
        mpDraw2DBack->stageMapMove(mpStick, 1, true);
    } else if (dMw_Z_TRIGGER() && mpDraw2DTop->isWarpAccept()) {
#if VERSION >= VERSION_GCN_JPN
        //! JPN version added a check to make sure if Arbiter's Grounds is cleared that
        //! the Mirror Chamber Statue has been spun before allowing portal warping from the map screen.
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[265]) && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])) {
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
            mpDraw2DTop->getScrnExplainPtr()->openExplain(0x8B4, 0, 0, 0xff, true);

            mPrevProcessAlt = mProcess;
            setProcess(PROC_PORTAL_WARP_FORBID);
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else 
#endif
        if (mpDraw2DTop->checkPlayerWarpAccept()) {
            mIsWarpMap = true;
            setProcess(PROC_ZOOM_SPOT_TO_REGION);
            mpDraw2DTop->setZButtonString(0x52a, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
            Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_ON, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else {
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);

                /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x68])
                    /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0xFA]))
            {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x85d, 0, 0, 0xff, true);
            } else if (!mpDraw2DTop->checkWarpAcceptCannon()) {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7de, 0, 0, 0xff, true);
                /* dSv_event_flag_c::F_0787 - Kakariko Village - Stopped by Midna when trying to warp the sky cannon */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x313]);
            } else if (!mpDraw2DTop->checkWarpAcceptRegion4()) {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x7dd, 0, 0, 0xff, true);
            } else {
                mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51d, 0, 0, 0xff, true);
            }

            mPrevProcessAlt = mProcess;
            setProcess(PROC_PORTAL_WARP_FORBID);
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else {
        if (g_fmapHIO.mDisplayReferenceArea) {
            mpDraw2DBack->zoomMapCalc(1.0f);
            mpDraw2DBack->zoomMapCalc2(field_0x1ec);
        }

        mpDraw2DBack->stageMapMove(mpStick, 1, true);

        int stage_no, room_no;
        f32 pos_x = mpDraw2DBack->getMapAreaGlobalCenterPosX() - mDoGph_gInf_c::getMinXF()
                                                                - mDoGph_gInf_c::getWidthF() * 0.5f;
        f32 pos_y = mpDraw2DBack->getMapAreaGlobalCenterPosY() - mDoGph_gInf_c::getHeightF() * 0.5f;
        mpMenuFmapMap->getPointStagePathInnerNo(getNowFmapRegionData(), pos_x, pos_y,
                                                mStayStageNo, &stage_no, &room_no);
        
        if (mStageCursor != stage_no || mResetAreaName) {
            mStageCursor = stage_no;
            mRoomCursor = room_no;
            u16 area_name = checkStRoomData();
            if (mStageCursor >= 0) {
                if (area_name != 0xffff) {
                    setAreaName(area_name);
                } else {
                    setAreaName(mAreaName[mStageCursor]);
                }
            } else {
                setAreaNameZero();
            }
        }

        if (mErrorSound == true) {
            mErrorSound = false;
        }
    }

    mpDraw2DBack->zoomMapCalcHIO();

    if (mStageCursor >= 0) {
        setFlash(mStageCursor, true);
    } else {
        mpMenuFmapMap->setFlashOff();
    }
}

void dMenu_Fmap_c::portal_demo1_init() {
    mpDraw2DBack->setRegionCursor(2);
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    setAreaName(mAreaName[mpDraw2DBack->getSpotCursor()]);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_RIGHT, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_LEFT, 0.0f);
    mpDraw2DTop->setBButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0);
    mpDraw2DTop->setCrossLRString(0);
    dMsgObject_setTalkHeap(mpTalkHeap);
    mMsgFlow.init(NULL, 3005, 0, NULL);
    mZoomLevel = 10;
}

void dMenu_Fmap_c::portal_demo1_move() {
    talkButton();
    mMsgFlow.doFlow(NULL, NULL, 0);

    if (dMsgObject_getMessageID() >= 2008 || mZoomLevel < 10) {
        if (mZoomLevel == 10) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMOUT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        mZoomLevel--;
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
        if (mZoomLevel <= 0) {
            if (mpDraw2DBack->getAllPathShowFlag()) {
                removeAreaData();
                readWorldData(0xff);
            }
            mIsWarpMap = true;
            setProcess(PROC_PORTAL_DEMO2);
        }
    }
}

void dMenu_Fmap_c::portal_demo2_init() {
    setTitleName(mAllTitleName);
    setAreaName(mTitleName[mpDraw2DBack->getRegionCursor()]);
}

void dMenu_Fmap_c::portal_demo2_move() {
    talkButton();
    mMsgFlow.doFlow(NULL, NULL, 0);

    if (dMsgObject_getMessageID() >= 2009) {
        mpDraw2DBack->setRegionCursor(1);
        setProcess(PROC_PORTAL_DEMO3);
        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_AREA_SELECT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    mpDraw2DBack->mapBlink();
}

void dMenu_Fmap_c::portal_demo3_init() {
    setTitleName(mAllTitleName);
    setAreaName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    mZoomLevel = 0;
}

void dMenu_Fmap_c::portal_demo3_move() {
    talkButton();
    mMsgFlow.doFlow(NULL, NULL, 0);

    if (dMsgObject_getMessageID() >= 2010 || mZoomLevel > 0) {
        if (mZoomLevel == 0) {
            readWorldData(mpDraw2DBack->getRegionCursor() + 1);
            Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        mZoomLevel++;
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
        if (mZoomLevel >= 10) {
            setProcess(PROC_PORTAL_DEMO4);
        }
    }

    if (mZoomLevel == 0) {
        mpDraw2DBack->mapBlink();
    }
}

void dMenu_Fmap_c::portal_demo4_init() {
    mPortalNo = 0xff;
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    if (mPortalNo == 0xff) {
        setAreaNameZero();
    }
}

void dMenu_Fmap_c::portal_demo4_move() {
    talkButton();
    if (mMsgFlow.doFlow(NULL, NULL, 0)) {
        setProcess(PROC_PORTAL_DEMO5);
    }
    setFlash(mpDraw2DBack->getSpotCursor(), false);
}

void dMenu_Fmap_c::portal_demo5_init() {
    mpDraw2DTop->set3DStickString(0x526);
    mpDraw2DBack->onArrowDrawFlag();
}

void dMenu_Fmap_c::portal_demo5_move() {
    mpStick->checkTrigger();

    // fakematch: the call to the fake inline function dMeter2Info_isTouchKeyCheck_alt
    // should actually call dMeter2Info_isTouchKeyCheck, but for some reason the generated asm
    // is different from every other call to dMeter2Info_isTouchKeyCheck
    if (dMw_B_TRIGGER() || dMw_Z_TRIGGER() || dMw_START_TRIGGER() || dMw_UP_TRIGGER()
        || dMw_DOWN_TRIGGER() || dMw_LEFT_TRIGGER() || dMw_RIGHT_TRIGGER()
        || dMeter2Info_isTouchKeyCheck_alt(0xc))
    {
        dMsgObject_setTalkHeap(mpTalkHeap);
        fopMsgM_messageSet(2015, 1000);
        setProcess(PROC_PORTAL_DEMO6);
    } else if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xc)) {
        if (mPortalNo != 0xff) {
            dMenu_Fmap_portal_data_c::data* portals = mpPortalDat->mData;
            mpDraw2DTop->createExplain(mpTalkHeap, mpStick);
            dMsgObject_setPortalMessageID(portals[mPortalNo].mMessageID);
            mpDraw2DTop->getScrnExplainPtr()->openExplain(0x51e, 2, 0, 0xff, true);
            setProcess(PROC_PORTAL_WARP_SELECT);
        } else {
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else {
        portalWarpMapMove(mpStick);
    }

    setFlash(mpDraw2DBack->getSpotCursor(), false);
}

void dMenu_Fmap_c::portal_demo6_init() {
    /* empty function */
}

void dMenu_Fmap_c::portal_demo6_move() {
    if (dMsgObject_getMsgObjectClass()->getStatus() == 0xe) {
        dMsgObject_getMsgObjectClass()->setStatus(0x10);
        talkButton();
    } else if (dMsgObject_getMsgObjectClass()->getStatus() == 0x12) {
        dMsgObject_getMsgObjectClass()->setStatus(0x13);
        setProcess(PROC_PORTAL_DEMO5);
        talkButton();
    }

    setFlash(mpDraw2DBack->getSpotCursor(), false);
}

void dMenu_Fmap_c::yamiboss_demo1_init() {
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    setAreaName(mAreaName[mpDraw2DBack->getSpotCursor()]);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_RIGHT, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_LEFT, 0.0f);
    mpDraw2DTop->setBButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0);
    mpDraw2DTop->setCrossLRString(0);
    dMsgObject_setTalkHeap(mpTalkHeap);
    mMsgFlow.init(NULL, 3012, 0, NULL);
    mZoomLevel = 10;
}

void dMenu_Fmap_c::yamiboss_demo1_move() {
    talkButton();
    mMsgFlow.doFlow(NULL, NULL, 0);
    if (dMsgObject_getMessageID() >= 4015) {
        setProcess(PROC_YAMIBOSS_DEMO2);
    }
}

void dMenu_Fmap_c::yamiboss_demo2_init() {
    mZoomLevel = 0;
    field_0x30b = false;
}

void dMenu_Fmap_c::yamiboss_demo2_move() {
    if (dMsgObject_getMsgObjectClass()->getStatus() == 0xe) {
        mZoomLevel++;
            /* dSv_event_flag_c::M_086 - Twilight Hyrule Field - Show Boss Bug's Tear of Light on the map */
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x77])) {
            if (mZoomLevel > 90) {
                setProcess(PROC_YAMIBOSS_DEMO3);
            }
        } else {
            if (mZoomLevel > 30) {
                field_0x30b = true;
                /* dSv_event_flag_c::M_086 - Twilight Hyrule Field - Show Boss Bug's Tear of Light on the map */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[0x77]);
            }
        }
    }
}

void dMenu_Fmap_c::yamiboss_demo3_init() {
    /* empty function */
}

void dMenu_Fmap_c::yamiboss_demo3_move() {
    talkButton();
    mMsgFlow.doFlow(NULL, NULL, 0);
    if (dMsgObject_getMessageID() >= 4016) {
        setProcess(PROC_YAMIBOSS_DEMO4);
    }
}

void dMenu_Fmap_c::yamiboss_demo4_init() {
    mZoomLevel = 0;
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Fmap_c::yamiboss_demo4_move() {
    mZoomLevel++;
    field_0x1ec = (f32)mZoomLevel / 10.0f;
    mpDraw2DBack->zoomMapCalc2(field_0x1ec);
    if (mZoomLevel >= 10) {
        setProcess(PROC_YAMIBOSS_DEMO5);
    }
}

void dMenu_Fmap_c::yamiboss_demo5_init() {
    /* empty function */
}

void dMenu_Fmap_c::yamiboss_demo5_move() {
    if (field_0x30b && dMsgObject_getMessageID() >= 4018) {
        field_0x30b = false;
    }
    talkButton();
    if (mMsgFlow.doFlow(NULL, NULL, 0)) {
        setProcess(PROC_SPOT_MAP);
        mpDraw2DBack->onArrowDrawFlag();
    }
}

void dMenu_Fmap_c::light_demo1_init() {
    mpDraw2DBack->zoomMapCalc2(1.0f);
    mZoomLevel = -10;
    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    setAreaName(mAreaName[mpDraw2DBack->getSpotCursor()]);
    mpDraw2DTop->setBButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0);
    mpDraw2DTop->setCrossLRString(0);
    dMsgObject_setTalkHeap(mpTalkHeap);
    field_0x1ec = 1.0f;
    setTitleName(mTitleName[mpDraw2DBack->getRegionCursor()]);
    setAreaName(mAreaName[mpDraw2DBack->getSpotCursor()]);
    mStageCursor = mpDraw2DBack->getSpotCursor();
    mpDraw2DBack->resetDrug();
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_UP, 0.0f);
    mpDraw2DTop->setArrowAlphaRatio(dMenu_Fmap2DTop_c::ARROW_DOWN, 0.0f);
    mpDraw2DTop->setBButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->setZButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DTop->set3DStickString(0);
    mpDraw2DTop->setCrossLRString(0);
    dMsgObject_setTalkHeap(mpTalkHeap);
    mMsgFlow.init(NULL, 125, 0, NULL);
}

void dMenu_Fmap_c::light_demo1_move() {
    mZoomLevel++;
    if (mZoomLevel > 0) {
        setProcess(PROC_LIGHT_DEMO2);
    }
}

void dMenu_Fmap_c::light_demo2_init() {
    mMsgFlow.init(NULL, 125, 0, NULL);
}

void dMenu_Fmap_c::light_demo2_move() {
    talkButton();
    if (mMsgFlow.doFlow(NULL, NULL, 0)) {
        setProcess(PROC_SPOT_MAP);
        mpDraw2DBack->onArrowDrawFlag();
    }
}

void dMenu_Fmap_c::table_demo1_init() {
    mZoomLevel = -10;
    mpDraw2DBack->setRegionCursor(dMeter2Info_getTableMapRegionNo());
    readWorldData(mpDraw2DBack->getRegionCursor() + 1);
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    dMenuFmapIconDisp_c icon;

    for (int i = 0; i < 8; i++) {
        if (mpRegionData[i] != NULL && mpStageData[i] != NULL) {
            int stay_no = dComIfGp_roomControl_getStayNo();
            icon.init(mpRegionData[i], mpStageData[i], 5, mStayStageNo, stay_no);
            
            while (!icon.getValidData()) {
                if (icon.isDrawDisp()) {
                    f32 pos_x, pos_z;
                    int stage_no, room_no;
                    icon.getPosition(&stage_no, &room_no, &pos_x, &pos_z, NULL);
                    if (isRoomCheck(stage_no, room_no)) {
                        mpDraw2DBack->setArrowPosAxis(pos_x, pos_z);
                    }
                }
                if (icon.nextData()) break;
            }
        }
    }
}

void dMenu_Fmap_c::table_demo1_move() {
    mZoomLevel++;
    if (mZoomLevel == 1) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
    if (mZoomLevel > 0) {
        mpDraw2DBack->zoomMapCalc((f32)mZoomLevel / 10.0f);
        if (mZoomLevel >= 10) {
            setProcess(PROC_TABLE_DEMO2);
        }
    }
}

void dMenu_Fmap_c::table_demo2_init() {
    mZoomLevel = -10;
    tableArrowPosInit(true);
    mpDraw2DBack->zoomMapCalc2(0.0f);
}

void dMenu_Fmap_c::table_demo2_move() {
    mZoomLevel++;
    if (mZoomLevel == 1) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
    if (mZoomLevel > 0) {
        field_0x1ec = (f32)mZoomLevel / 10.0f;
        mpDraw2DBack->zoomMapCalc2(field_0x1ec);
        if (mZoomLevel >= 10) {
            setProcess(PROC_TABLE_DEMO3);
        }
    }
}

void dMenu_Fmap_c::table_demo3_init() {
    dMsgObject_setTalkHeap(mpTalkHeap);
    mMsgFlow.init(NULL, 1089, 0, NULL);
}

void dMenu_Fmap_c::table_demo3_move() {
    talkButton();
    if (mMsgFlow.doFlow(NULL, NULL, 0)) {
        mpDraw2DBack->onArrowDrawFlag();
        setProcess(PROC_SPOT_MAP);
    }
}

void dMenu_Fmap_c::howl_demo1_init() {
    mZoomLevel = -10;
    mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    mpDraw2DBack->zoomMapCalc2(0.0f);
}

void dMenu_Fmap_c::howl_demo1_move() {
    mZoomLevel++;
    if (mZoomLevel == 1) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_ZOOMIN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
    if (mZoomLevel > 0) {
        field_0x1ec = (f32)mZoomLevel / 10.0f;
        mpDraw2DBack->zoomMapCalc2(field_0x1ec);
        if (mZoomLevel >= 10) {
            setProcess(PROC_HOWL_DEMO2);
        }
    }
}

void dMenu_Fmap_c::howl_demo2_init() {
    mZoomLevel = -10;
}

void dMenu_Fmap_c::howl_demo2_move() {
    mZoomLevel++;
    if (mZoomLevel > 0) {
        mpDraw2DBack->onArrowDrawFlag();
        setProcess(PROC_SPOT_MAP);
    }
}

void dMenu_Fmap_c::howl_demo3_init() {
    /* empty function */
}

void dMenu_Fmap_c::howl_demo3_move() {
    setProcess(PROC_REGION_MAP);
}

bool dMenu_Fmap_c::isOpen() {
    bool ret = false;
    bool init = false;

    if (!mpResArchive) {
        if (!mpMountArchive) {
            mpMountArchive = mDoDvdThd_mountArchive_c::create("/res/FieldMap/res-f.arc", 2, mpHeap);
        }

        if (mpMountArchive->sync()) {
            mpResArchive = mpMountArchive->getArchive();
            mpMountArchive->destroy();
            mpMountArchive = NULL;
            mpFmapMapRes = mpResArchive->getResource("dat/data.dat");
            _create();
        } else {
            return false;
        }
    }

    s16 display_frame_num = (s16)g_fmapHIO.mDisplayFrameNum;
    s16 undisplay_frame_num = (s16)g_fmapHIO.mUndisplayFrameNum;
    if (mDisplayFrame == 0) {
        init = true;
    }
    mDisplayFrame++;
    f32 ratio = (f32)mDisplayFrame / (f32)display_frame_num;

    if (mPanDirection == 1) {
        mTransX = (1.0f - ratio) * -608.0f;
        mTransY = 0.0f;
    } else if (mPanDirection == 3) {
        mTransX = (1.0f - ratio) * 608.0f;
        mTransY = 0.0f;
    } else if (mPanDirection == 2) {
        mTransX = 0.0f;
        mTransY = (1.0f - ratio) * -448.0f;
    } else if (mPanDirection == 0) {
        mTransX = 0.0f;
        mTransY = (1.0f - ratio) * 448.0f;
    }

    mAlphaRatio = ratio;

    if (mDisplayFrame >= display_frame_num) {
        mDisplayFrame = undisplay_frame_num;
        mTransX = 0.0f;
        mTransY = 0.0f;
        mAlphaRatio = 1.0f;
        ret = true;
    }

    mpDraw2DBack->setAllTrans(mTransX, mTransY);
    mpDraw2DBack->setAllAlphaRate(mAlphaRatio, init);
    mpDraw2DTop->setAllTrans(mTransX, mTransY);
    mpDraw2DTop->setAllAlphaRate(mAlphaRatio, init);
    mpDraw2DBack->setSpotTextureFadeAlpha(mSpotTextureFadeAlpha);

    return ret;
}

bool dMenu_Fmap_c::isClose() {
    bool ret = true;
    bool bVar2 = false;
    
    s16 undisplay_frame_num = (s16)g_fmapHIO.mUndisplayFrameNum;
    if (mDisplayFrame == undisplay_frame_num) {
        bVar2 = true;
    }
    mDisplayFrame--;
    f32 ratio = (f32)mDisplayFrame / (f32)undisplay_frame_num;

    if (mDisplayFrame <= 0) {
        mDisplayFrame = 0;
    } else {
        ret = false;
    }

    if (mPanDirection == 1) {
        mTransX = (1.0f - ratio) * 608.0f;
        mTransY = 0.0f;
    } else if (mPanDirection == 3) {
        mTransX = (1.0f - ratio) * -608.0f;
        mTransY = 0.0f;
    } else if (mPanDirection == 2) {
        mTransX = 0.0f;
        mTransY = (1.0f - ratio) * 448.0f;
    } else if (mPanDirection == 0) {
        mTransX = 0.0f;
        mTransY = (1.0f - ratio) * -448.0f;
    }

    mAlphaRatio = ratio;

    mpDraw2DBack->setAllTrans(mTransX, mTransY);
    mpDraw2DBack->setAllAlphaRate(mAlphaRatio, bVar2);
    mpDraw2DTop->setAllTrans(mTransX, mTransY);
    mpDraw2DTop->setAllAlphaRate(mAlphaRatio, bVar2);

    return ret;
}

u8 dMenu_Fmap_c::getProcess() {
    if (g_fmapHIO.field_0x2f9) {
        return mProcess;
    }
    return PROC_REGION_MAP;
}

u8 dMenu_Fmap_c::getRegionCursor() {
    if (mpDraw2DBack != NULL && g_fmapHIO.field_0x2f9) {
        return mpDraw2DBack->getRegionCursor();
    }
    return 0;
}

u8 dMenu_Fmap_c::getStageCursor() {
    if (mpDraw2DBack != NULL && g_fmapHIO.field_0x2f9) {
        return mpDraw2DBack->getSpotCursor();
    }
    return 0;
}

f32 dMenu_Fmap_c::getStageTransX() {
    if (mpDraw2DBack != NULL && g_fmapHIO.field_0x2f9) {
        return mpDraw2DBack->getStageTransX();
    }
    return 0.0f;
}

f32 dMenu_Fmap_c::getStageTransZ() {
    if (mpDraw2DBack != NULL && g_fmapHIO.field_0x2f9) {
        return mpDraw2DBack->getStageTransZ();
    }
    return 0.0f;
}

bool dMenu_Fmap_c::isRoomCheck(int i_stageNo, int i_roomNo) {
    if (!field_0x305) {
        return false;
    }

    dMenuMapCommon_c::Stage_c* stage_data
        = (dMenuMapCommon_c::Stage_c*)((int)mpFieldDat + mpFieldDat->mStageDataOffset);
    dMenuMapCommon_c::Stage_c::data* stages = stage_data->mData;
    u8 stage_no = 0;
    int i = 0;

    bool checked_data[100];
    for (int j = 0; j < mDataNumMax; j++) {
        checked_data[j] = false;
    }

    bool local_e0[64];
    bool room_check[64];
    char stage_name[8];

    for (; i < mDataNumMax; i++) {
        if (checked_data[i]) continue;

        strcpy(stage_name, stages[i].mName);

        for (int k = 0; k < 64; k++) {
            local_e0[k] = false;
            room_check[k] = false;
        }

        bool bVar1 = false;

        for (int j = i; j < mDataNumMax; j++) {
            if (strcmp(stages[j].mName, stage_name)) continue;

            checked_data[j] = true;

            if (stages[j].mRegionNo == mpDraw2DBack->getRegionCursor() + 1) {
                if (stages[j].mRoomNo != 0xff) {
                    local_e0[stages[j].mRoomNo] = true;
                    room_check[stages[j].mRoomNo] = true;
                    if (!bVar1) {
                        bVar1 = true;
                    }
                } else {
                    for (int k = 0; k < 64; k++) {
                        if (!local_e0[k]) {
                            room_check[k] = true;
                        }
                    }
                    if (!bVar1) {
                        bVar1 = true;
                    }
                }
            } else {
                if (stages[j].mRoomNo != 0xff) {
                    local_e0[stages[j].mRoomNo] = true;
                }
            }
        }

        if (bVar1) {
            if (i_stageNo == stage_no && room_check[i_roomNo]) break;
            stage_no++;
            if (stage_no > i_stageNo) {
                return false;
            }
        }
    }

    u8 region = mpDraw2DBack->getRegionCursor();
    i = 0;
    dMenuMapCommon_c::RoomData_c* room_data = mpRoomData[region];
    for (; i < i_stageNo; i++) {
        room_data = room_data->getNextData();
        if (room_data == NULL) break;
    }

    if (room_data == NULL || i != i_stageNo) {
        return false;
    }

    dMenuMapCommon_c::RoomData_c::data* room_data_data = room_data->getRoomData();
    u8* rooms = room_data_data->mData;
    for (int j = 0; j < room_data_data->mSize; j++) {
        if (i_roomNo == rooms[j] && room_check[rooms[j]]) {
            return true;
        }
    }

    return false;
}

u16 dMenu_Fmap_c::checkStRoomData() {
    dMenu_Fmap_field_room_data_c* room_data
        = (dMenu_Fmap_field_room_data_c*)((int)mpFieldDat + mpFieldDat->mRoomDataOffset);
    dMenu_Fmap_field_room_data_c::data* data = room_data->mData;
    int count = room_data->mCount;
    for (int i = 0; i < count; i++) {
        int offset = data->mCount + sizeof(dMenu_Fmap_field_room_data_c::data) - 1;
        if (data->mCount % 2 == 0) {
            offset += 1;
        }

        if (!strcmp(mStageName[mStageCursor], data->mStageName)) {
            for (int j = 0; j < data->mCount; j++) {
                if (mRoomCursor == data->mRoomNos[j]) {
                    mFlashRoomCount = data->mCount;
                    mFlashRooms = data->mRoomNos;
                    return data->mAreaName;
                }
            }
        }

        data = (dMenu_Fmap_field_room_data_c::data*)((int)data + offset);
    }

    mFlashRoomCount = 0;
    mFlashRooms = NULL;
    return -1;
}

void dMenu_Fmap_c::talkButton() {
    u16 status = dMsgObject_c::getStatus();
    if (status == 5 || status == 0xf || status == 0x10) {
        mpDraw2DTop->setAButtonString(0x408, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    } else {
        mpDraw2DTop->setAButtonString(0, dMenu_Fmap2DTop_c::ALPHA_DEFAULT);
    }
}

void dMenu_Fmap_c::setProcess(u8 i_process) {
    mPrevProcess = mProcess;
    mProcess = i_process;
}

void dMenu_Fmap_c::setFlash(u8 i_stageNo, bool param_1) {
    mpDraw2DBack->mapBlink(&field_0x210);
    if (param_1) {
        mpMenuFmapMap->setFlashOn(mpDraw2DBack->getRegionCursor() + 1, i_stageNo,
                                  mFlashRooms, mFlashRoomCount);
    } else {
        mpMenuFmapMap->setFlashOn(mpDraw2DBack->getRegionCursor() + 1, i_stageNo, NULL, 0);
    }
}

bool dMenu_Fmap_c::readWorldData(u8 i_regionNo) {
    removeAreaData();
    bool ret = false;
    bool bVar1 = false;

    if (mpDraw2DBack->getAllPathShowFlag()) {
        for (int i = 0; i < 8; i++) {
            if (i == i_regionNo - 1) {
                if (readAreaData(i + 1, true)) {
                    ret = true;
                    bVar1 = true;
                }
            } else {
                if (readAreaData(i + 1, false)) {
                    bVar1 = true;
                }
            }
        }
    } else {
        if (readAreaData(i_regionNo, true)) {
            ret = true;
            bVar1 = true;
        }
    }

    dMenu_Fmap_region_data_c* region_data = NULL;
    int head = -1;
    for (int i = 0; i < 8; i++) {
        if (mpRegionData[i] != NULL) {
            if (region_data == NULL) {
                head = i;
                region_data = mpRegionData[i];
                region_data->setNextData(NULL);
            } else {
                region_data->setNextData(mpRegionData[i]);
                region_data = mpRegionData[i];
                region_data->setNextData(NULL);
            }
        }
    }

    if (head != -1 && mpWorldData == NULL) {
        mpWorldData = new dMenu_Fmap_world_data_c(mpRegionData[head]);
        mpDraw2DBack->setWorldPosMinMax(mpWorldData->getWorldMinX(), mpWorldData->getWorldMinZ(),
                                        mpWorldData->getWorldMaxX(), mpWorldData->getWorldMaxZ());
    }

    if (bVar1) {
        ResTIMG* img = mpMenuFmapMap->getResTIMGPointer();
        if (img != NULL) {
            mpDraw2DBack->setSpotPane(img);
        }
        field_0x305 = true;
    }

    return ret;
}

bool dMenu_Fmap_c::readAreaData(u8 i_regionNo, bool i_isSelectedRegion) {
    if (field_0x305) {
        return false;
    }

    bool checked_data[100];
    bool local_e0[64];
    char stage_name[8];
    char tmp_stage_name[8];

    dMenuMapCommon_c::Stage_c::data* stages
        = ((dMenuMapCommon_c::Stage_c*)((int)mpFieldDat + mpFieldDat->mStageDataOffset))->mData;
    u8 stage_no = 0;
    dMenu_Fmap_stage_data_c* prev_stage_data = NULL;
    dMenuMapCommon_c::RoomData_c* prev_room_data = NULL;
    stage_name[7] = 0;

    if (i_isSelectedRegion) {
        mSpotNum = 0;
    }

    int i = 0;
    int stage_index = 0;

    for (int j = 0; j < mDataNumMax; j++) {
        checked_data[j] = false;
    }

    if (i_isSelectedRegion) {
        mStartStageNo = 0xff;
        mStayStageNo = -1;
    }

    field_0x200 = 0;

    for (; i < mDataNumMax; i++) {
        if (checked_data[i]) continue;

        strcpy(tmp_stage_name, mTmpStageName);
        resetRoomDataBit();
        if (i_isSelectedRegion) {
            strcpy(mTmpStageName, stages[i].mName);
        } else {
            strcpy(mTmpStageName, tmp_stage_name);
        }
        strcpy(tmp_stage_name, stages[i].mName);

        for (int k = 0; k < 64; k++) {
            local_e0[k] = false;
        }

        bool bVar2 = false;

        for (int j = i; j < mDataNumMax; j++) {
            if (strcmp(stages[j].mName, tmp_stage_name)) continue;

            checked_data[j] = true;

            if (i_regionNo == stages[j].mRegionNo) {
                if (stages[j].mRoomNo != 0xff) {
                    onRoomDataBit(stages[j].mRoomNo);
                    if (!bVar2) {
                        stage_index = j;
                        bVar2 = true;
                    }
                } else {
                    for (int k = 0; k < 64; k++) {
                        if (!local_e0[k]) {
                            onRoomDataBit(k);
                        }
                    }
                    if (!bVar2) {
                        stage_index = j;
                        bVar2 = true;
                    }
                }
            } else {
                if (stages[j].mRoomNo != 0xff) {
                    local_e0[stages[j].mRoomNo] = true;
                }
            }
        }

        if (bVar2) {
            dMenu_Fmap_stage_data_c* stage_data = new dMenu_Fmap_stage_data_c();
            if (mpStageData[i_regionNo - 1] == NULL) {
                mpStageData[i_regionNo - 1] = stage_data;
            } else {
                prev_stage_data->setNextData(stage_data);
            }
            prev_stage_data = stage_data;
            stage_data->setNextData(NULL);
            stage_data->setStageCntNo(stage_no);
            stage_data->setName(stages[stage_index].mName);

            for (int j = 0; j < 8; j++) {
                stage_name[j] = stages[stage_index].mName[j];
            }

            if (i_regionNo == dComIfGp_getNowLevel()) {
                if (dComIfGs_isPlayerFieldLastStayFieldDataExistFlag()) {
                    if (!strcmp(dMenuFmap_getStartStageName(mpFieldDat), stage_name)) {
                        mpDraw2DBack->setSpotCursor(stage_no);
                        mStartStageNo = stage_no;
                        mStayStageNo = stage_no;
                    }
                } else {
                    if (!strcmp(dComIfGs_getPlayerFieldLastStayName(), stage_name)) {
                        mpDraw2DBack->setSpotCursor(stage_no);
                        mStayStageNo = stage_no;
                    }
                }
            }

            stage_data->setOffsetX(stages[stage_index].mOffsetX);
            stage_data->setOffsetZ(stages[stage_index].mOffsetZ);

            if (i_isSelectedRegion) {
                mpDraw2DBack->setStageOriginXZ(mSpotNum, stages[stage_index].mOffsetX,
                                               stages[stage_index].mOffsetZ);
                mAreaName[mSpotNum] = stages[stage_index].mAreaName;
                strcpy(mStageName[mSpotNum], stage_name);
                mSpotNum++;
            }

            dMenuMapCommon_c::RoomData_c* room_data = new dMenuMapCommon_c::RoomData_c();
            if (mpRoomData[i_regionNo - 1] == NULL) {
                mpRoomData[i_regionNo - 1] = room_data;
            } else {
                prev_room_data->setNextData(room_data);
            }
            prev_room_data = room_data;
            room_data->setNextData(NULL);
            readRoomData(stage_name, stage_data, room_data, i_regionNo - 1,
                         stages[stage_index].mRoomNo);

            stage_no++;
        }
    }

    if (mpStageData[i_regionNo - 1] == NULL) {
        return false;
    }

    if (mpRegionData[i_regionNo - 1] == NULL) {
        mpRegionData[i_regionNo - 1] = new dMenu_Fmap_region_data_c(i_regionNo,
                                                                    mpStageData[i_regionNo - 1],
                                                                    mRegionOffsetX[i_regionNo - 1],
                                                                    mRegionOffsetZ[i_regionNo - 1]);
    }

    if (i_regionNo - 1 == mpDraw2DBack->getRegionCursor()) {
        for (int j = 0; j < mSpotNum; j++) {
            mpDraw2DBack->setStageOriginPathXZ(j,
                                getNowFmapRegionData()->getStageCenterX_CoordWorld(j),
                                getNowFmapRegionData()->getStageCenterZ_CoordWorld(j));
        }
    }

    return true;
}

bool dMenu_Fmap_c::readRoomData(char const* i_stageName, dMenu_Fmap_stage_data_c* i_stageData,
                                void* o_roomData, int i_regionNo, u8 param_4) {
    dMenu_Fmap_room_data_c* prev_room_data = NULL;
    i_stageData->setFmapRoomDataTop(NULL);
    dMenu_Fmap_stage_arc_data_c* room_data = NULL;
    
    char stage_path[20];
    sprintf(stage_path, "%s/stage.dat", i_stageName);
    if (readFieldMapData((void**)&room_data, stage_path, false, false)) {
        ((dMenuMapCommon_c::RoomData_c*)o_roomData)->setRoomData(room_data);
    }

    dMenu_Fmap_stage_arc_data_c* stage_arc = room_data;
    u8* room_nos = room_data->mRoomNos;
    i_stageData->setStageArc(stage_arc);

    for (int i = 0; i < stage_arc->mSize; i++) {
        if (isRoomDataBit(room_nos[i])) {
            void* dzs_data = NULL;

            char room_path[20];
            sprintf(room_path, "%s/room%d.dzs", i_stageName, room_nos[i]);

            if (readRoomDzsData(&dzs_data, 0x1500, room_path)) {
                dMenu_Fmap_data_c* map_data = new dMenu_Fmap_data_c();
                dMenu_Fmap_room_data_c* room_data
                    = new dMenu_Fmap_room_data_c(room_nos[i], i_stageData, map_data);
                if (prev_room_data == NULL) {
                    i_stageData->setFmapRoomDataTop(room_data);
                } else {
                    prev_room_data->setNextData(room_data);
                }
                prev_room_data = room_data;
                map_data->setDzsData(dzs_data);
                // this cast is nonsense but it ends up getting cast back anyway
                dStage_dt_c_fieldMapLoader(dzs_data, (dStage_dt_c*)map_data);
            }
        }
    }

    return true;
}

bool dMenu_Fmap_c::readFieldMapData(void** o_data, char const* i_path, bool param_2,
                                    bool param_3) {
    u32 size;
    if (mpMapArchive != dComIfGp_getFieldMapArchive2()) {
        if (param_3) {
            void* res = mpResArchive->getResource(i_path);
            mpResArchive->getExpandedResSize(res);
            *o_data = res;
            return true;
        } else {
            void* res = mpMapArchive->getResource(i_path);
            size = mpMapArchive->getExpandedResSize(res);
            mpMapArchive->removeResource(res);
        }
    } else {
        if (param_3) {
            void* res = mpResArchive->getResource(i_path);
            mpResArchive->getExpandedResSize(res);
            *o_data = res;
            return true;
        } else {
            size = dLib_getExpandSizeFromAramArchive(mpMapArchive, i_path);
        }
    }

    if (param_2) {
        if (mpMapArchive != dComIfGp_getFieldMapArchive2()) {
            *o_data = mpHeap->alloc(size, 0x20);
        } else {
            *o_data = mpMapArchive->getResource(i_path);
            return true;
        }
    } else {
        *o_data = mpHeap->alloc(size, 0x20);
    }

    u32 read_size = mpMapArchive->readResource(*o_data, size, i_path);

    if (param_2) {
        DCStoreRangeNoSync(*o_data, size);
    }
    
    if (read_size == 0) {
        return false;
    } else {
        return true;
    }
}

void dMenu_Fmap_c::decodeFieldMapData() {
    int field_data = (int)mpFieldDat;
    dMenu_Fmap_field_region_data_c* region_data
        = (dMenu_Fmap_field_region_data_c*)(field_data + mpFieldDat->mRegionDataOffset);
    dMenuMapCommon_c::Stage_c* stage_data
        = (dMenuMapCommon_c::Stage_c*)(field_data + mpFieldDat->mStageDataOffset);
    mDataNumMax = stage_data->mCount;
    mAllTitleName = *(u16*)(field_data + 0x16);
    setTitleName(mAllTitleName);
    dMenu_Fmap_field_region_data_c::data* regions = region_data->mData;
    char tex_path[20];

    for (int i = 0; i < region_data->mCount; i++) {
        mpDraw2DBack->setRegionTextureReadNum(i, regions[i].mTextureReadNum);
        
        for (int j = 0; j < 8; j++) {
            if (j + 1 == regions[i].mTextureReadNum) {
                bool local_3f = false;
                if (j == 7) {
                    local_3f = true;
                    sprintf(tex_path, "tex/region8.bti");
                } else {
                    sprintf(tex_path, "tex/region%d.bti", regions[i].mTextureReadNum);
                }

                if (readFieldMapData((void**)&mRegionTexture[j], tex_path, true, local_3f)) {
                    if (!g_fmapHIO.mRegionImageDebug) {
                        mpDraw2DBack->setRegionTexData(j, mRegionTexture[j],
                                        regions[i].mOriginX, regions[i].mOriginZ,
                                        regions[i].field_0xc, regions[i].field_0x10,
                                        regions[i].field_0x14, regions[i].field_0x18,
                                        regions[i].field_0x20, regions[i].field_0x1c,
                                        regions[i].field_0x24);
                    } else {
                        mpDraw2DBack->setRegionTexData(j, mRegionTexture[j],
                                        regions[i].mOriginX, regions[i].mOriginZ,
                                        g_fmapHIO.mRegionImagePosX[i] + regions[i].field_0xc,
                                        g_fmapHIO.mRegionImagePosZ[i] + regions[i].field_0x10,
                                        g_fmapHIO.mRegionImageScale[i] + regions[i].field_0x14,
                                        regions[i].field_0x18, regions[i].field_0x20,
                                        regions[i].field_0x1c, regions[i].field_0x24);
                    }

                    mTitleName[j] = regions[i].mTitleName;
                    mRegionOffsetX[j] = regions[i].mOriginX;
                    mRegionOffsetZ[j] = regions[i].mOriginZ;
                }
            }
        }
    }

    mpDraw2DBack->calcDrawPriority();
    mpDraw2DBack->calcAllMapPos();
    mpDraw2DBack->setStageData(stage_data, mDataNumMax);
    setAreaName(mTitleName[mpDraw2DBack->getRegionCursor()]);
}

void dMenu_Fmap_c::decodePortalData() {
    dMenu_Fmap_portal_data_c* portal_dat = mpPortalDat;
    if (g_fmapHIO.mPortalDataTerminalOutput) {
        for (int i = 0; i < portal_dat->mCount; i++) {}
    }
}

bool dMenu_Fmap_c::readRoomDzsData(void** i_data, u32 param_1, char const* i_path) {
    void* res;
    char* path = const_cast<char*>(i_path);
    u32 size;
    if (mpMapArchive != dComIfGp_getFieldMapArchive2()) {
        res = mpMapArchive->getResource(path);
        size = mpMapArchive->getExpandedResSize(res);
        mpMapArchive->removeResource(res);
    } else {
        size = dLib_getExpandSizeFromAramArchive(mpMapArchive, path);
    }

    *i_data = mpHeap->alloc(size, 0x20);
    u32 read_size = mpMapArchive->readResource(*i_data, size, path);
    if (read_size == 0) {
        return false;
    } else {
        field_0x200 += read_size;
        return true;
    }
}

bool dMenu_Fmap_c::removeAreaData() {
    if (!field_0x305) {
        return true;
    }

    if (mpWorldData != NULL) {
        if (mpWorldData != NULL) {
            delete mpWorldData;
        }
        mpWorldData = NULL;
    }

    for (int i = 0; i < 8; i++) {
        if (mpRegionData[i] != NULL) {
            if (mpRegionData[i] != NULL) {
                delete mpRegionData[i];
            }
            mpRegionData[i] = NULL;
        }
    }

    for (int i = 0; i < 8; i++) {
        dMenu_Fmap_stage_data_c* stage_data = mpStageData[i];
        while (stage_data != NULL) {
            dMenu_Fmap_stage_data_c* next_data = stage_data->getNextData();
            removeRoomData(stage_data);
            if (stage_data != NULL) {
                delete stage_data;
            }
            stage_data = next_data;
        }

        dMenuMapCommon_c::RoomData_c* room_data = mpRoomData[i];
        while (room_data != NULL) {
            dMenuMapCommon_c::RoomData_c* next_data = room_data->getNextData();
            dMenuMapCommon_c::RoomData_c::data* data = room_data->getRoomData();
            if (data != NULL) {
                mpHeap->free(data);
            }
            delete room_data;
            room_data = next_data;
        }
    }

    mpDraw2DBack->deleteSpotPane();

    mpWorldData = NULL;
    for (int i = 0; i < 8; i++) {
        mpRegionData[i] = NULL;
        mpStageData[i] = NULL;
        mpRoomData[i] = NULL;
    }

    for (int i = 0; i < 20; i++) {
        strcpy(mStageName[i], "");
    }

    field_0x305 = false;
    return true;
}

bool dMenu_Fmap_c::removeRoomData(dMenu_Fmap_stage_data_c* i_stageData) {
    dMenu_Fmap_room_data_c* room_data = i_stageData->getFmapRoomDataTop();
    while (room_data != NULL) {
        dMenu_Fmap_room_data_c* next_data = room_data->getNextData();
        dMenu_Fmap_data_c* fmap_data = room_data->getFmapData();
        void* dzs_data = fmap_data->getDzsData();
        if (dzs_data != NULL) {
            mpHeap->free(dzs_data);
        }
        if (fmap_data != NULL) {
            delete fmap_data;
        }
        delete room_data;
        room_data = next_data;
    }
    return true;
}

void dMenu_Fmap_c::setTitleName(u32 i_msgIndex) {
    mpDraw2DTop->setTitleNameString(i_msgIndex);
}

void dMenu_Fmap_c::setAreaName(u32 i_msgIndex) {
    mAreaNameSet = true;
    mpDraw2DTop->setAreaNameString(i_msgIndex);
}

void dMenu_Fmap_c::setAreaNameZero() {
    mAreaNameSet = false;
    mpDraw2DTop->setAreaNameString(0);
}

void dMenu_Fmap_c::portalWarpMapMove(STControl* i_stick) {
    mpDraw2DBack->regionMapMove(i_stick);
    dMenu_Fmap_portal_data_c* portal_dat = mpPortalDat;
    dMenu_Fmap_portal_data_c::data* portals = portal_dat->mData;
    f32 arrow_x = mpDraw2DBack->getArrowPos2DX();
    f32 arrow_y = mpDraw2DBack->getArrowPos2DY();
    u8 uVar6 = 0xff;

    for (int i = 0; i < portal_dat->mCount; i++) {
        if (portals[i].mRegionNo == mpDraw2DBack->getRegionCursor() + 1
            && checkDrawPortalIcon(portals[i].mStageNo, portals[i].mSwitchNo))
        {
            f32 pos_x, pos_y;
            mpDraw2DBack->calcAllMapPos2D(portals[i].mPosition.x, portals[i].mPosition.z,
                                          &pos_x, &pos_y);
            if ((pos_x - arrow_x) * (pos_x - arrow_x)
                                    + (pos_y - arrow_y) * (pos_y - arrow_y) <= 500.0f) {
                uVar6 = i;
                setAreaName(portals[uVar6].mMessageID);
                u8 stage = searchPortalStageID(const_cast<char*>(portals[uVar6].mStageName));
                if (stage != 0xff) {
                    mpDraw2DBack->setSpotCursor(stage);
                }
            }
        }
    }

    if (uVar6 != 0xff) {
        if (uVar6 != mPortalNo) {
            Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        mPortalNo = uVar6;
    }
}

void dMenu_Fmap_c::onRoomDataBit(int i_bit) {
    mRoomDataBits[i_bit >> 5] |= 1 << (i_bit & 0x1f);
}

BOOL dMenu_Fmap_c::isRoomDataBit(int i_bit) {
    return (mRoomDataBits[i_bit >> 5] & 1 << (i_bit & 0x1f)) ? TRUE : FALSE;
}

void dMenu_Fmap_c::resetRoomDataBit() {
    for (int i = 0; i < 2; i++) {
        mRoomDataBits[i] = 0;
    }

    for (int i = 0; i < 8; i++) {
        mTmpStageName[i] = '\0';
    }
}

void dMenu_Fmap_c::drawIcon(f32 param_0, bool param_1) {
    if (mpDraw2DBack->getRegionCursor() + 1 == dComIfGp_getNowLevel()) {
        drawPlayEnterIcon();
    }

    cXyz pos;
    s16 angle;
    char stage_name[8];
    if (dComIfGs_isPlayerFieldLastStayFieldDataExistFlag()) {
        fopAc_ac_c* player = daPy_getPlayerActorClass();
        pos.set(dMapInfo_n::getMapPlayerPos());
        angle = player->shape_angle.y;
        strcpy(stage_name, dMenuFmap_getStartStageName(mpFieldDat));
    } else {
        pos = dComIfGs_getPlayerFieldLastStayPos();
        angle = dComIfGs_getPlayerFieldLastStayAngleY();
        strcpy(stage_name, dComIfGs_getPlayerFieldLastStayName());
    }

    u8 is_portal_demo1 = 0;
    if (mProcess == PROC_PORTAL_DEMO1) {
        is_portal_demo1 = 1;
    }
    mpDraw2DBack->setIcon2DPos(0x11, stage_name, pos.x, pos.z, cM_sht2d(angle),
                               is_portal_demo1, param_1);
    
    if (!param_1) {
        if (mIsWarpMap == true && mpPortalDat != NULL) {
            dMenu_Fmap_portal_data_c* portal_dat = mpPortalDat;
            dMenu_Fmap_portal_data_c::data* portals = portal_dat->mData;
            for (int i = 0; i < portal_dat->mCount; i++) {
                if (checkDrawPortalIcon(portals[i].mStageNo, portals[i].mSwitchNo)) {
                    u8 region = mpDraw2DBack->getRegionCursor();
                    u8 cVar2 = portals[i].mRegionNo - 1;
                    if ((mProcess != PROC_PORTAL_WARP_FORBID && mProcess >= PROC_REGION_MAP)
                        || (mProcess == PROC_PORTAL_WARP_FORBID && mPrevProcess != PROC_ALL_MAP))
                    {
                        if (cVar2 == region) {
                            mpDraw2DBack->setIcon2DPos(1, portals[i].mPosition.x,
                                                       portals[i].mPosition.z, 0.0f, 0, false);
                            if (mPortalNo == i) {
                                mpDraw2DBack->setIcon2DPos(0, portals[i].mPosition.x,
                                                           portals[i].mPosition.z, 0.0f, 0, false);
                            }
                        }
                    } else {
                        mpDraw2DBack->setIcon2DPos(1, portals[i].mPosition.x,
                                                   portals[i].mPosition.z, 0.0f, 0, false);
                    }
                }
            }
        }

        if ((mProcess != PROC_PORTAL_WARP_FORBID && mProcess >= PROC_REGION_MAP)
            || (mProcess == PROC_PORTAL_WARP_FORBID && mPrevProcess != PROC_ALL_MAP))
        {
            drawLightDropIcon();
            drawBatsumarkIcon();
            drawEnterIcon();
            drawSnowmanIcon();
            drawGoldWolfIcon();
            drawCoachIcon();
        }
    }
}

void dMenu_Fmap_c::drawLightDropIcon() {
    if (isLightVesselGet()) {
        drawIcon(4, 5);
    }
}

void dMenu_Fmap_c::drawBatsumarkIcon() {
    drawIcon(5, 0x12);
}

void dMenu_Fmap_c::drawSnowmanIcon() {
    drawIcon(0xd, 9);
    drawIcon(0xe, 0xa);
}

void dMenu_Fmap_c::drawGoldWolfIcon() {
    drawIcon(6, 0xb);
}

void dMenu_Fmap_c::drawCoachIcon() {
    drawIcon(0xa, 0xd);
}

void dMenu_Fmap_c::drawPlayEnterIcon() {
    cXyz pos;
    s16 angle;
    char stage_name[8];

    if (isRoomCheck(mStayStageNo, dComIfGp_getStartStageRoomNo())) {
        if (dComIfGs_isPlayerFieldLastStayFieldDataExistFlag()) {
            pos.set(dMapInfo_n::getMapRestartPos());
            angle = dComIfGs_getRestartRoomAngleY();
            strcpy(stage_name, dMenuFmap_getStartStageName(mpFieldDat));
        } else {
            pos = dComIfGs_getPlayerFieldLastStayPos();
            angle = dComIfGs_getPlayerFieldLastStayAngleY();
            strcpy(stage_name, dComIfGs_getPlayerFieldLastStayName());
        }
        mpDraw2DBack->setIcon2DPos(0x15, stage_name, pos.x, pos.z, cM_sht2d(angle), 0, false);
    }
}

bool dMenu_Fmap_c::checkDrawPortalIcon(int i_stageNo, int i_switchNo) {
    if (g_fmapHIO.mAllRegionsUnlocked) {
        return true;
    }
    return dComIfGs_isStageSwitch(i_stageNo, i_switchNo);
}

bool dMenu_Fmap_c::searchIcon(u8 i_typeGroupNo, int i_swBit, f32* o_posX, f32* o_posZ) {
    dMenuFmapIconDisp_c icon;
    dMenu_Fmap_stage_data_c* stage_data;
    int tmp_r26;
    s32 stay_no;
    bool ret = false;
    stay_no = dComIfGp_roomControl_getStayNo();
    tmp_r26 = mStayStageNo;
    stage_data = getNowFmapStageData();
    dMenu_Fmap_region_data_c* region_data = getNowFmapRegionData();
    icon.init(region_data, stage_data, i_typeGroupNo, tmp_r26, stay_no);
    // icon.init(getNowFmapRegionData(), getNowFmapStageData(), i_typeGroupNo,
    //           mStayStageNo, dComIfGp_roomControl_getStayNo());
    
    while (!icon.getValidData()) {
        if (icon.isDrawDisp()) {
            int stage_no, room_no;
            const dTres_c::data_s* tres_data;
            icon.getPosition(&stage_no, &room_no, o_posX, o_posZ, &tres_data);
            if (isRoomCheck(stage_no, room_no) && tres_data->mSwBit == i_swBit) {
                ret = true;
                break;
            }
        }

        if (icon.nextData()) {
            break;
        }
    }

    return ret;
}

void dMenu_Fmap_c::drawIcon(u8 i_typeGroupNo, int param_1) {
    dMenuFmapIconDisp_c icon;
    dMenu_Fmap_stage_data_c* stage_data;
    int tmp_r30;
    s32 stay_no = dComIfGp_roomControl_getStayNo();
    tmp_r30 = mStayStageNo;
    stage_data = getNowFmapStageData();
    dMenu_Fmap_region_data_c* region_data = getNowFmapRegionData();
    icon.init(region_data, stage_data, i_typeGroupNo, tmp_r30, stay_no);
    // icon.init(getNowFmapRegionData(), getNowFmapStageData(), i_typeGroupNo,
    //           mStayStageNo, dComIfGp_roomControl_getStayNo());
    
    while (!icon.getValidData()) {
        if (icon.isDrawDisp()) {
            f32 pos_x, pos_z;
            int stage_no, room_no;
            icon.getPosition(&stage_no, &room_no, &pos_x, &pos_z, NULL);
            if (isRoomCheck(stage_no, room_no)) {
                mpDraw2DBack->setIcon2DPos(param_1, pos_x, pos_z, 0.0f, 0, false);
            }
        }

        if (icon.nextData()) {
            break;
        }
    }
}

void dMenu_Fmap_c::drawEnterIcon() {
    drawIcon(1, 0x14);
    drawIcon(8, 0x16);
}

void dMenu_Fmap_c::drawPortalIcon() {
    if (mIsWarpMap == true && mpPortalDat != NULL) {
        dMenu_Fmap_portal_data_c* portal_dat = mpPortalDat;
        dMenu_Fmap_portal_data_c::data* portals = portal_dat->mData;
        for (int i = 0; i < portal_dat->mCount; i++) {
            if (checkDrawPortalIcon(portals[i].mStageNo, portals[i].mSwitchNo)) {
                mpDraw2DBack->setIcon2DPos(1, portals[i].mPosition.x, portals[i].mPosition.z,
                                           0.0f, 0, false);
            }
        }
    }
}

u8 dMenu_Fmap_c::getRegionStageNum(int param_0) {
    if (mpFieldDat != NULL) {
        dMenu_Fmap_field_region_data_c* region_data
            = (dMenu_Fmap_field_region_data_c*)((int)mpFieldDat + mpFieldDat->mRegionDataOffset);
        dMenu_Fmap_field_region_data_c::data* regions = region_data->mData;
        for (int i = 0; i < region_data->mCount; i++) {
            if (param_0 == regions[i].mTextureReadNum) {
                return regions[i].field_0x1;
            }
        }
    }

    return 0;
}

dMenu_Fmap_region_data_c* dMenu_Fmap_c::getNowFmapRegionData() {
    return mpRegionData[mpDraw2DBack->getRegionCursor()];
}

dMenu_Fmap_stage_data_c* dMenu_Fmap_c::getNowFmapStageData() {
    return mpStageData[mpDraw2DBack->getRegionCursor()];
}

u8 dMenu_Fmap_c::searchPortalStageID(char* param_0) {
    for (int i = 0; i < 20; i++) {
        if (!strcmp(param_0, mStageName[i])) {
            return i;
        }
    }
    return 0xff;
}

void dMenu_Fmap_c::drawDebugStage() {
    if (mProcess != PROC_ALL_MAP) {
        mpDraw2DBack->setDebugData(mpMenuFmapMap, getNowFmapRegionData(), mStayStageNo);
    } else {
        mpDraw2DBack->setDebugData(NULL, NULL, 0);
    }
}

void dMenu_Fmap_c::arrowPosInit() {
    if (mProcess == PROC_TABLE_DEMO1) {
        tableArrowPosInit(false);
    } else if (mProcess == PROC_HOWL_DEMO1) {
        howlArrowPosInit();
    } else if (mpDraw2DBack->getRegionCursor() + 1 == dComIfGp_getNowLevel()) {
        cXyz pos;
        char stage_name[8];
        if (dComIfGs_isPlayerFieldLastStayFieldDataExistFlag()) {
            pos.set(dMapInfo_n::getMapPlayerPos());
            strcpy(stage_name, dMenuFmap_getStartStageName(mpFieldDat));
        } else {
            pos = dComIfGs_getPlayerFieldLastStayPos();
            strcpy(stage_name, dComIfGs_getPlayerFieldLastStayName());
        }
        
        f32 fVar1 = 0.0f;
        if (mProcess == PROC_LIGHT_DEMO1) {
            f32 pos1_x, pos2_x, pos1_z, pos2_z;
            mpDraw2DBack->calcAllMapPosWorld(0.0f, 0.0f, &pos2_x, &pos2_z);
            mpDraw2DBack->calcAllMapPosWorld(0.0f, -50.0f, &pos1_x, &pos1_z);
            fVar1 = pos1_z - pos2_z;
        }

        mpDraw2DBack->setArrowPos3D(mpDraw2DBack->getRegionCursor(), stage_name,
                                    pos.x, pos.z + fVar1);
    } else {
        mpDraw2DBack->setArrowPos3D(mpDraw2DBack->getRegionCursor(), NULL,
                                mpDraw2DBack->getRegionOriginX(mpDraw2DBack->getRegionCursor()),
                                mpDraw2DBack->getRegionOriginZ(mpDraw2DBack->getRegionCursor()));
    }
}

void dMenu_Fmap_c::tableArrowPosInit(bool param_0) {
    int iVar5 = -1;
    int swbit;
    switch (dMeter2Info_getTableMapRegionNo()) {
    case dMeter_fmapHIO_c::REGION_LANAYRU:
             /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x169])
                /* dSv_event_flag_c::M_070 - Cutscene - [cutscene: 18] Lanayru spirit restored */
            || !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x67]))
        {
            iVar5 = 0;
                 /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x108])) {
                swbit = 0x4f;
            } else {
                swbit = 0x33;
            }
        } else {
            iVar5 = 1;
            swbit = 0x2a;
        }
        break;
    case dMeter_fmapHIO_c::REGION_ELDIN:
        iVar5 = 2;
        swbit = 0x55;
        break;
    case dMeter_fmapHIO_c::REGION_FARON:
        iVar5 = 3;
        swbit = 0x5f;
        break;
    }

    static char* stage_name[4] = {"F_SP115", "F_SP113", "F_SP109", "F_SP108"};
    strcpy(mMarkedStageName, stage_name[iVar5]);

    if (param_0) {
        f32 pos1_x, pos2_x, pos1_z, pos2_z, icon_x, icon_z;
        searchIcon(5, swbit, &icon_x, &icon_z);
        mpDraw2DBack->calcAllMapPosWorld(0.0f, 0.0f, &pos2_x, &pos2_z);
        mpDraw2DBack->calcAllMapPosWorld(0.0f, -20.0f, &pos1_x, &pos1_z);
        mpDraw2DBack->setArrowPosAxis(icon_x, icon_z + (pos1_z - pos2_z));
    } else {
        mpDraw2DBack->setArrowPos3D(mpDraw2DBack->getRegionCursor(), mMarkedStageName,
                                mpDraw2DBack->getRegionOriginX(mpDraw2DBack->getRegionCursor()),
                                mpDraw2DBack->getRegionOriginZ(mpDraw2DBack->getRegionCursor()));
    }
}

void dMenu_Fmap_c::yamibossArrowPosInit() {
    f32 pos1_x, pos2_x, pos1_z, pos2_z, icon_x, icon_z;
    if (!searchIcon(4, 0x75, &icon_x, &icon_z)) {
        icon_x = -38552.47f;
        icon_z = 38647.59f;
    }
    mpDraw2DBack->calcAllMapPosWorld(0.0f, 0.0f, &pos2_x, &pos2_z);
    mpDraw2DBack->calcAllMapPosWorld(0.0f, -20.0f, &pos1_x, &pos1_z);
    mpDraw2DBack->setArrowPosAxis(icon_x, icon_z + (pos1_z - pos2_z));
}

void dMenu_Fmap_c::howlArrowPosInit() {
    u8 type = dMeter2Info_getGoldWolfMapType();
    static char* stage_name[6] = {"F_SP104", "F_SP122", "F_SP122", "F_SP124", "F_SP111", "F_SP116"};
    strcpy(mMarkedStageName, stage_name[type - 2]);
    static const int i_swBit[6] = {0x41, 0x29, 0x2a, 0x32, 0x79, 0x32};
    f32 icon_x, icon_z;
    if (searchIcon(6, i_swBit[type - 2], &icon_x, &icon_z)) {
        mpDraw2DBack->setArrowPos3DOffset(mpDraw2DBack->getRegionCursor(), mMarkedStageName,
                                          icon_x, icon_z, 0.0f);
    }
}

u8 dMenu_Fmap_c::getHowlRegionID() {
    switch (dMeter2Info_getGoldWolfMapType()) {
    case 2:
        return 1;
    case 3:
        return 4;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 3;
    case 7:
        return 4;
    default:
        return 1;
    }
}

bool dMenu_Fmap_c::isLightVesselGet() {
    return (bool)dComIfGp_isLightDropMapVisible();
}

cXyz* dMenu_Fmap_c::getPlayerPos2D() {
    cXyz pos;
    char stage_name[8];
    if (dComIfGs_isPlayerFieldLastStayFieldDataExistFlag()) {
        pos.set(dMapInfo_n::getMapPlayerPos());
        strcpy(stage_name, dMenuFmap_getStartStageName(mpFieldDat));
    } else {
        pos = dComIfGs_getPlayerFieldLastStayPos();
        strcpy(stage_name, dComIfGs_getPlayerFieldLastStayName());
    }
    mpDraw2DBack->calcAllMapPos2D(stage_name, pos.x - mpDraw2DBack->getStageTransX(),
                                  pos.z - mpDraw2DBack->getStageTransZ(),
                                  &mPlayerPos.x, &mPlayerPos.y);
    mPlayerPos.z = 0.0f;
    return &mPlayerPos;
}
