/**
 * d_meter_map.cpp
 * 
*/

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_meter_map.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/JHostIO/JORServer.h"
#include "d/actor/d_a_player.h"
#include "d/d_map.h"
#include "d/d_menu_window.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_overlap_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "d/d_camera.h"

#if (PLATFORM_WII || PLATFORM_SHIELD)
dMeter_map_HIO_c g_meter_mapHIO;
#endif

#if DEBUG
dMeter_map_HIO_c* dMeter_map_HIO_c::mMySelfPointer = NULL;
dMeterMap_c* dMeterMap_c::mMySelfPointer = NULL;

dMeter_map_HIO_c::dMeter_map_HIO_c() {
    dMeter_map_HIO_c::mMySelfPointer = this;
    mAreaMap = dMap_HIO_c::mMySelfPointer;
    mDebugMapExpansion = 0;
    mMapDisplayLocation = 0;
    mWideBottomLeftX = -38;
    mWideY = 380;
    mNormalBottomLeftX = 35;
    mNormalY = 380;
    mGcBottomLeftX = 35;
    mGcY = 421;
    mHiddenRightX = -36;
    field_0x16 = 0;
    mSizeTypeChange = 0;
    field_0x18 = 1;
    mMediumHorizontal = 144;
    mMediumVertical = 144;
    mBigHorizontal = 216;
    mBigVertical = 216;
    mSmallHorizontal = 96;
    mSmallVertical = 96;
    mMapDisplaySize = 0;
    field_0x2a = 144;
    mMapAlphaTest = 0;
    mAlpha = 180;
    mTextureResolutionHalved = 0;
    mForcedMapTopEdgeChange = 0;
    field_0x30 = 144;
    mMapDisplayProhibited = 0;
    mMapRenderingProhibited = 0;
}

void dMeter_map_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("■デバック用マップ拡大■", 0);
    ctx->genCheckBox("ONで拡大", &mDebugMapExpansion, 0x1);
    ctx->genLabel("■マップ表示位置■", 0);
    ctx->genCheckBox("ONで調整有効", &mMapDisplayLocation, 0x1);
    ctx->genLabel("*Wide(16:9)", 0);
    ctx->genSlider("左下X", &mWideBottomLeftX, -200, 640);
    ctx->genSlider("    Y", &mWideY, 0, 480);
    ctx->genLabel("*Normal(4:3)", 0);
    ctx->genSlider("左下X", &mNormalBottomLeftX, -200, 640);
    ctx->genSlider("    Y", &mNormalY, 0, 480);
    ctx->genLabel("*GC(4:3) WiiでMAPアイコン無しの場合のY座標にも使用します", 0);
    ctx->genSlider("左下X", &mGcBottomLeftX, -200, 640);
    ctx->genSlider("    Y", &mGcY, 0, 480);
    ctx->genSlider("隠右X", &mHiddenRightX, -600, 800);
    ctx->genLabel("■マップ表示サイズ■", 0);
    ctx->genCheckBox("・ON変更", &mMapDisplaySize, 0x1);
    ctx->genSlider("", &field_0x2a, 1, 400);
    ctx->genLabel("■マップサイズ■", 0);
    ctx->genCheckBox("・サイズタイプの変更<サイズ変更後はテクスチャーサイズ変更をして下さい>",
                     &mSizeTypeChange, 0x1);
    ctx->startRadioButton("", &field_0x18, 1, NULL, 0xffff, 0xffff, 0x100, 0xffff);
    ctx->genRadioButtonItem("中", 1, 0, 0xffff, 0xffff, 0x80, 0x10);
    ctx->genRadioButtonItem("大", 2, 0, 0xffff, 0xffff, 0x80, 0x10);
    ctx->genRadioButtonItem("小", 3, 0, 0xffff, 0xffff, 0x80, 0x10);
    ctx->endRadioButton();
    ctx->genLabel("・各サイズ<サイズ変更後はテクスチャーサイズ変更をして下さい>", 0);
    ctx->genLabel("・中", 0);
    ctx->genSlider("横", &mMediumHorizontal, 1, 0x90);
    ctx->genSlider("縦", &mMediumVertical, 1, 0x90);
    ctx->genLabel("・大", 0);
    ctx->genSlider("横", &mBigHorizontal, 1, 0xd8);
    ctx->genSlider("縦", &mBigVertical, 1, 0xd8);
    ctx->genLabel("・小", 0);
    ctx->genSlider("横", &mSmallHorizontal, 1, 0x90);
    ctx->genSlider("縦", &mSmallVertical, 1, 0x90);
    ctx->genCheckBox("・テクスチャー解像度半分", &mTextureResolutionHalved, 0x1);
    ctx->genButton("テクスチャーサイズ変更", 0x4000001);
    ctx->genLabel("■マップアルファ■", 0);
    ctx->genCheckBox("マップアルファテストONで有効", &mMapAlphaTest, 0x1);
    ctx->genSlider("アルファ", &mAlpha, 0, 0xff);

    if (dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 1 && field_0x18 != 1) {
        field_0x18 = 1;
        update();
    }

    ctx->genLabel("■強制マップ上端変更■", 0);
    ctx->genCheckBox("ONで有効", &mForcedMapTopEdgeChange, 0x1);
    ctx->genSlider("", &field_0x30, -1, 480);
    ctx->genLabel("ONからOFFにする場合は-1にしてからOFFにして下さい", 0);

    mAreaMap = dMap_HIO_c::mMySelfPointer;
    if (mAreaMap != NULL) {
        ctx->genNode("エリアマップ", this->mAreaMap, 0, 0);
    }

    ctx->genLabel("■マップ表示禁止■", 0);
    ctx->genCheckBox("ONで有効", &mMapDisplayProhibited, 0x1);
    ctx->genLabel("■マップレンダリング禁止■", 0);
    ctx->genCheckBox("ONで有効", &mMapRenderingProhibited, 0x1);
}

void dMeter_map_HIO_c::listenPropertyEvent(const JORPropertyEvent* evt) {
    // DEBUG NONMATCHING
    JORReflexible::listenPropertyEvent(evt);

    switch ((u32)evt->id) {
        case 0x4000001:
            int mapDispType = dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo());
            if (dMeterMap_c::mMySelfPointer != NULL) {
                dMeterMap_c* map_p = dMeterMap_c::mMySelfPointer->getMapPointer();
                if (map_p != NULL) {
                    int sizeType = map_p->getMapDispSizeTypeNo();
                    s16 width;
                    s16 height;

                    switch (sizeType) {                     
                        case dMeterMap_c::MAP_SIZE_TYPE_BIG:
                            width = g_meter_mapHIO.mBigHorizontal;
                            height = g_meter_mapHIO.mBigVertical;
                            break;
                            
                        case 4:
                            if (mapDispType != dStage_stagInfo_c::UB_D_MAP) {
                                width = g_meter_mapHIO.mMediumHorizontal;
                                height = g_meter_mapHIO.mMediumVertical;
                                break;
                            }

                            // You cannot specify a large size for the dungeon map
                            OS_REPORT("ダンジョンマップは大サイズ指定できません\n");
                            break;

                        case 1:
                        case dMeterMap_c::MAP_SIZE_TYPE_SMALL:
                            width = g_meter_mapHIO.mSmallHorizontal;
                            height = g_meter_mapHIO.mSmallVertical;
                            break;
                    }

                    if (g_meter_mapHIO.mTextureResolutionHalved) {
                        dMeterMap_c::mMySelfPointer->field_0x8_debug = 1;
                    } else {
                        dMeterMap_c::mMySelfPointer->field_0x8_debug = 0;
                    }

                    field_0x28 = (s16)width;
                    dMeterMap_c::mMySelfPointer->setSizeW(width);
                    dMeterMap_c::mMySelfPointer->setSizeH(height);
                    dMeterMap_c::mMySelfPointer->field_0x4_debug = 1;
                    return;
                }
            }
    }
}

void dMeter_map_HIO_c::update() {
    JORMContext* mctx = attachJORMContext(8);
    mctx->startUpdateNode(this);
    mctx->updateRadioButton(2, &field_0x18, 1);
    mctx->endUpdateNode();
    releaseJORMContext(mctx);
}
#endif

bool dMeterMap_c::isEnableDispMap() {
    bool minimapEnable = false;
    stage_stag_info_class* pstag = dComIfGp_getStage()->getStagInfo();

    if (pstag != NULL && getMapDispSizeTypeNo() != 0) {
        minimapEnable = dStage_stagInfo_GetMiniMap(pstag) != 0;
    }

    bool enableMap = false;
    if (minimapEnable && dMpath_c::isExistMapPathData()) {
        enableMap = true;
    }
    
    return enableMap;
}

int dMeterMap_c::getMapDispSizeTypeNo() {
    int uvar6 = 7;

    dStage_FileList_dt_c* fList =
        dComIfGp_roomControl_getStatusRoomDt(dComIfGp_roomControl_getStayNo())->getFileListInfo();
    if (fList != NULL) {
        uvar6 = dStage_FileList_dt_getMiniMap(fList);
    }

    u32 stageMapSizeTypeNo = dStage_stagInfo_GetMiniMap(dComIfGp_getStage()->getStagInfo());
    JUT_ASSERT(536, stageMapSizeTypeNo != MAP_SIZE_TYPE_STAGE);
    s32 mapDispType = dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo());

    if (mapDispType == 1) {
        uvar6 = 1;
        JUT_ASSERT(544, stageMapSizeTypeNo != MAP_SIZE_TYPE_SMALL);
        JUT_ASSERT(545, stageMapSizeTypeNo != MAP_SIZE_TYPE_BIG);
    } else if (uvar6 == 7) {
        uvar6 = stageMapSizeTypeNo;
    }

    int mapDispSizeType = 0;

    #if DEBUG
    if (g_meter_mapHIO.mSizeTypeChange) {
        uvar6 = g_meter_mapHIO.field_0x18;
    }
    #endif
    
    switch (uvar6) {
    case 0:
        break;
    case 1:
        if (mapDispType == 1) {
            mapDispSizeType = 4;
        } else {
            mapDispSizeType = 1;
        }
        break;
    case 2:
        if (mapDispType == 1) {
            JUT_ASSERT(573, mapDispType != dStage_stagInfo_c::UB_D_MAP);
            break;
        }

        mapDispSizeType = 2;
        break;
    case 3:
        if (mapDispType == 1) {
            mapDispSizeType = 5;
        } else {
            mapDispSizeType = 3;
        }
        break;
    }

    return mapDispSizeType;
}

bool dMeterMap_c::isEnableDispMapAndMapDispSizeTypeNo() {
    if (isEnableDispMap() && getMapDispSizeTypeNo()) {
        return true;
    }

    return false;
}

f32 dMeterMap_c::getMapDispEdgeBottomY_Layout() {
    #if (PLATFORM_WII || PLATFORM_SHIELD)
    if (dComIfGs_getOptPointer() == 0) {
        return g_meter_mapHIO.mGcY;
    }

    if (mDoGph_gInf_c::isWide()) {
        return g_meter_mapHIO.mWideY;
    }

    return g_meter_mapHIO.mNormalY;
    #else
    return 421.0f;
    #endif
}

bool dMeterMap_c::isEventRunCheck() {
    if (dComIfGp_event_runCheck()) {
        return true;
    }
    
    if (dMsgObject_isTalkNowCheck()) {
        return true;
    }

    if (dComIfGp_event_runCheck() != field_0x30) {
        return true;
    }

    return false;
}

f32 dMeterMap_c::getMapDispEdgeLeftX_Layout() {
    #if (PLATFORM_WII || PLATFORM_SHIELD)
    if (mDoGph_gInf_c::isWide()) {
        return g_meter_mapHIO.mWideBottomLeftX + field_0x28;
    }

    return g_meter_mapHIO.mNormalBottomLeftX + field_0x28;
    #else
    return field_0x28 + 35;
    #endif
}

f32 dMeterMap_c::getMapDispEdgeTop() {
    f32 tmp = 0.0f;
    if (mMap != NULL) {
        tmp = mMap->getTexSizeY() -
              mMap->getTexelPerCm() * (mMap->getPackZ() + -mMap->getPackPlusZ()) -
              mMap->getTopEdgePlus();
    }
    f32 rv = getMapDispEdgeBottomY_Layout() - tmp;
    return rv;
}

s16 dMeterMap_c::getDispPosInside_OffsetX() {
    return 0;
}

s16 dMeterMap_c::getDispPosOutSide_OffsetX() {
    #if DEBUG
    f32 fVar1 = g_meter_mapHIO.mHiddenRightX + mDoGph_gInf_c::getMinXF();
    #endif
    f32 dvar3 =
        #if DEBUG
        fVar1
        #else
        -36.0f 
        #endif
        - mSizeW;
    f32 offset = 0.0f;
    if (mMap != NULL) {
        offset = mMap->getTexelPerCm() * mMap->getPackX();
        offset += (mMap->getRightEdgePlus() + (mMap->getTexelPerCm() * mMap->getPackPlusZ()));
    }
    dvar3 += offset;
    return dvar3 - getMapDispEdgeLeftX_Layout();
}

void dMeterMap_c::setDispPosInsideFlg_SE_On() {
    if (isEnableDispMapAndMapDispSizeTypeNo()) {
        dComIfGp_mapShow();
        field_0x2d = 1;
        field_0x2e = 7;
    }
}

void dMeterMap_c::setDispPosOutsideFlg_SE_On() {
    dComIfGp_mapHide();
    field_0x2d = 0;
    field_0x2e = 7;
}

void dMeterMap_c::setMapAlpha(u8 i_alpha) {
    mMapAlpha = i_alpha;
}

bool dMeterMap_c::isMapOpenCheck() {
    #if DEBUG
    if (dCam_getBody()->Gear() == 1) {
        return false;
    }
    #endif
    
    if (dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 7) {
        return false;
    }

    return true;
}

dMeterMap_c::dMeterMap_c(J2DScreen* i_scrn) {
#if DEBUG
    i_scrn = i_scrn;
    field_0x4_debug = 0;
    field_0x8_debug = 0;
    dMeterMap_c::mMySelfPointer = this;
#endif
    _create(i_scrn);
}

dMeterMap_c::~dMeterMap_c() {
#if DEBUG
    field_0x4_debug = 0;
    field_0x8_debug = 0;
#endif
    
    _delete();
}

void dMeterMap_c::_create(J2DScreen* unused) {
    s32 sizeX;
    s32 sizeY;
    s32 dispSizeW;
    s32 dispSizeH;

    getMapDispSizeTypeNo();
    switch (getMapDispSizeTypeNo()) {
    case 1:
        dispSizeW = 216;
        dispSizeH = 216;
        sizeX = 144;
        sizeY = 144;
        break;
    case 2:
        dispSizeW = 216;
        dispSizeH = 216;
        sizeX = 216;
        sizeY = 216;
        break;
    case 3:
        dispSizeW = 216;
        dispSizeH = 216;
        sizeX = 96;
        sizeY = 96;
        break;
    case 0:
    case 4:
        dispSizeW = 144;
        dispSizeH = 144;
        sizeX = 144;
        sizeY = 144;
        break;
    case 5:
        dispSizeW = 144;
        dispSizeH = 144;
        sizeX = 96;
        sizeY = 96;
        break;
    default:
        JUT_ASSERT(988, FALSE);
        break;
    }

    mSizeW = (s16)sizeX;
    mSizeH = (s16)sizeY;

    mMap = new dMap_c(sizeX, sizeY, dispSizeW, dispSizeH);
    JUT_ASSERT(999, mMap != NULL);

    mMapJ2DPicture = new J2DPicture(mMap->getResTIMGPointer());
    JUT_ASSERT(1002, mMapJ2DPicture != NULL);

    mIsCompass = dComIfGs_isDungeonItemCompass();
    mIsMap = dComIfGs_isDungeonItemMap();
    field_0x14 = 0;
    field_0x2a = 0;

    if (dComIfGp_checkMapShow()) {
        field_0x2d = 1;

        if (!isEnableDispMapAndMapDispSizeTypeNo()) {
            field_0x2d = 0;
        }

        if (!isMapOpenCheck()) {
            field_0x2d = 0;
        }
    } else {
        field_0x2d = 0;
    }

    if (field_0x2d != 0) {
        field_0x2d = 1;
        field_0x28 = getDispPosInside_OffsetX();
        dMeter2Info_setMapStatus(1);
    } else {
        field_0x2d = 0;
        field_0x28 = getDispPosOutSide_OffsetX();
        dMeter2Info_setMapStatus(0);
    }

    field_0x2e = 0;
    field_0x28 = 0;
    field_0x30 = 0;
                 /* dSv_event_flag_c::M_085 - Twilight Hyrule Field - Midna dialogue right before Boss Bug's Tear of Light appears */
    field_0x2b = dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[118]);
}

void dMeterMap_c::setDispPosOutSide() {
    field_0x2d = 0;
    field_0x28 = getDispPosOutSide_OffsetX();
}

void dMeterMap_c::setDispPosInSide() {
    field_0x2d = 1;
    field_0x28 = getDispPosInside_OffsetX();
}

void dMeterMap_c::_delete() {
    if (mMapJ2DPicture != NULL) {
        delete mMapJ2DPicture;
    }

    if (mMap != NULL) {
        delete mMap;
    }

    if (isEnableDispMapAndMapDispSizeTypeNo()) {
        if (field_0x2d != 0) {
            dComIfGp_mapShow();
        } else {
            dComIfGp_mapHide();
        }
    }
}

void dMeterMap_c::_move(u32 param_0) {
        /* dSv_event_flag_c::M_085 - Twilight Hyrule Field - Midna dialogue right before Boss Bug's Tear of Light appears */
    if (!field_0x2b && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[118])) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0) {
            /* dSv_event_flag_c::M_086 - Twilight Hyrule Field - Show Boss Bug's Tear of Light on the map */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[119]);
        } else {
            /* dSv_event_flag_c::M_085 - Twilight Hyrule Field - Midna dialogue right before Boss Bug's Tear of Light appears */
            field_0x2b = dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[118]);
            dMeter2Info_setPauseStatus(2);
        }
    }

    #if DEBUG
    if (field_0x4_debug) {
        mMap->changeTextureSize(mSizeW, mSizeH, field_0x8_debug);
        mMapJ2DPicture->changeTexture(mMap->getResTIMGPointer(), 0);
        field_0x4_debug = 0;
    }
    #endif

    int stayNo = dComIfGp_roomControl_getStayNo();

    field_0x14 = param_0;
    field_0x2a = checkMoveStatus();

    if (field_0x2a == 1 || field_0x2a == 2 || field_0x2a == 3 ||
        dMeter2Info_getPauseStatus() == 7 || dMeter2Info_getPauseStatus() == 2 ||
        dMeter2Info_getPauseStatus() == 6 || dMeter2Info_getPauseStatus() == 4 ||
        dMeter2Info_getPauseStatus() == 3 || dMeter2Info_getPauseStatus() == 5)
    {
        ctrlShowMap();
    }

    if (field_0x2d != 0) {
        if (field_0x28 != getDispPosInside_OffsetX()) {
            if (!cLib_addCalcAngleS(&field_0x28, getDispPosInside_OffsetX(), 2, 60, 10)) {
                #if DEBUG
                cLib_checkBit<u8>((int)field_0x2e, 4);
                #endif
            }
        }
    } else {
        cLib_addCalcAngleS(&field_0x28, getDispPosOutSide_OffsetX(), 2, 60, 10);
    }

    Vec map_pos = dMapInfo_n::getMapPlayerPos();

    #if DEBUG
    f32 sizeW = g_meter_mapHIO.field_0x28;
    f32 sizeH = g_meter_mapHIO.field_0x28;
    if (g_meter_mapHIO.mDebugMapExpansion) {
        sizeW = 400.0f;
        sizeH = 400.0f;
    }

    if (g_meter_mapHIO.mMapDisplaySize && mMap != NULL) {
        sizeW = g_meter_mapHIO.field_0x2a;
        sizeH = g_meter_mapHIO.field_0x2a;
    }

    mSizeW = (s16)sizeW;
    mSizeH = (s16)sizeH;
    #endif

    field_0x18 = field_0x28 + getMapDispEdgeLeftX_Layout();
    field_0x1c = getMapDispEdgeBottomY_Layout() - mSizeH;

    mMap->_move(map_pos.x, map_pos.z, stayNo, map_pos.y);
    field_0x30 = dComIfGp_event_runCheck();
}

void dMeterMap_c::_draw() {
    #if DEBUG
    if (!g_meter_mapHIO.mMapRenderingProhibited) {
        mMap->_draw();
        dComIfGd_set2DOpa(this);
    }
    #else
    mMap->_draw();
    dComIfGd_set2DOpa(this);
    #endif
}

void dMeterMap_c::draw() {
    if (
        #if DEBUG
        !g_meter_mapHIO.mMapDisplayProhibited &&
        #endif
        mMap != NULL && mMap->isDraw()
    ) {
        J2DGrafContext* graf = dComIfGp_getCurrentGrafPort();
        graf->setup2D();
        f32 sizeX = mSizeW;
        f32 sizeY = mSizeH;
        f32 tmp2 = field_0x18;
        f32 tmp3 = field_0x1c;

        u8 alpha = mMapAlpha;
        #if DEBUG
        if (g_meter_mapHIO.mMapAlphaTest) {
            alpha = g_meter_mapHIO.mAlpha;
        }
        #endif
        mMapJ2DPicture->setAlpha(alpha);

        mMapJ2DPicture->draw(tmp2, tmp3, sizeX, sizeY, false, false, false);
        mMapJ2DPicture->calcMtx();
    }
}

void dMeterMap_c::ctrlShowMap() {
    int unused = 0;
    if (!dMeter2Info_isGameStatus(1)) {
        if (!isMapOpenCheck() && !isEventRunCheck()) {
            if (dMw_LEFT_TRIGGER() || dMw_RIGHT_TRIGGER()) {
                Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
            return;
        }

        if (dMeter2Info_getPauseStatus() == 1 || dMeter2Info_getPauseStatus() == 7 ||
            dMeter2Info_getPauseStatus() == 3 || dMeter2Info_getPauseStatus() == 5 ||
            dMeter2Info_getPauseStatus() == 2 || dMeter2Info_getPauseStatus() == 6)
        {
            #if !DEBUG
            if (dMeter2Info_getMapStatus() == 0 && field_0x2d == 0) {
                setDispPosInsideFlg_SE_On();
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                            -1.0f, 0);
                dMeter2Info_set2DVibration();
                dMeter2Info_setMapStatus(1);
            }
            #endif

            if (
            #if DEBUG
                dMeter2Info_getMapStatus() == 0 &&
            #else
                dMeter2Info_getMapStatus() == 1 && field_0x2d != 0 &&
            #endif
                isFmapScreen()
            ) {
                switch (dMeter2Info_getPauseStatus()) {
                case 1:
                    dMeter2Info_setMapStatus(4);
                    break;
                case 7:
                    dMeter2Info_setMapStatus(7);
                    break;
                case 3:
                    dMeter2Info_setMapStatus(8);
                    break;
                case 5:
                    dMeter2Info_setMapStatus(9);
                    break;
                case 2:
                    dMeter2Info_setMapStatus(5);
                    break;
                case 6:
                    dMeter2Info_setMapStatus(3);
                    break;
                }

                dMeter2Info_setMapKeyDirection(0x400);
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                            -1.0f, 0);
                #if !DEBUG
                dMeter2Info_set2DVibration();
                #endif
            }
            dMeter2Info_resetPauseStatus();
        } else if (dMeter2Info_getPauseStatus() == 4) {
            dMeter2Info_resetPauseStatus();
            if (isDmapScreen()) {
                #if !DEBUG
                if (dMeter2Info_getMapStatus() == 0 && field_0x2d == 0) {
                    setDispPosInsideFlg_SE_On();
                    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f,
                                                -1.0f, -1.0f, 0);
                    dMeter2Info_set2DVibration();
                    dMeter2Info_setMapStatus(1);
                }
                #endif

                if (
                #if DEBUG
                    dMeter2Info_getMapStatus() == 0
                #else
                    dMeter2Info_getMapStatus() == 1 && field_0x2d != 0
                #endif
                ) {
                    dMeter2Info_setMapStatus(6);
                    dMeter2Info_setMapKeyDirection(0x400);
                    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f,
                                                -1.0f, -1.0f, 0);
                    dMeter2Info_set2DVibration();
                }
            }

        } else if (!mDoCPd_c::getTrigUp(PAD_1) && !mDoCPd_c::getTrigDown(PAD_1)) {
            keyCheck();
        }

        if (mIsCompass != dComIfGs_isDungeonItemCompass()) {
            if (!isDispPosInsideFlg()) {
                setDispPosInsideFlg_SE_On();
                #if !DEBUG
                dMeter2Info_setMapStatus(1);
                #endif
            }

            mIsCompass = dComIfGs_isDungeonItemCompass();
            unused = 0;
        }

        if (mIsMap != dComIfGs_isDungeonItemMap()) {
            if (!isDispPosInsideFlg()) {
                setDispPosInsideFlg_SE_On();
                #if !DEBUG
                dMeter2Info_setMapStatus(1);
                #endif
            }

            mIsMap = dComIfGs_isDungeonItemMap();
            unused = 0;
        }
    }
}

u8 dMeterMap_c::checkMoveStatus() {
    u8 rv;
    if (isShow(field_0x14)) {
        if (isEnableDispMapAndMapDispSizeTypeNo()) {
            if (isDispPosInsideFlg()) {
                rv = 1;
            } else {
                rv = 2;
            }
        } else {
            rv = 3;
        }
    } else {
        rv = 0;
    }
    return rv;
}

bool dMeterMap_c::isShow(u32 param_0) {
    if ((param_0 & 0x4000) || fopOvlpM_IsDoingReq() ||
        ((param_0 & 0x40) && dComIfGp_event_checkHind(0x100) &&
         !dComIfGp_event_chkEventFlag(0x40)) ||
        daPy_getPlayerActorClass()->getSumouMode() || (param_0 & 0x200000) || (param_0 & 0x100) ||
        (param_0 & 0x80) || (param_0 & 0x40000000) || (param_0 & 0x1000) || (param_0 & 0x100000) ||
        (param_0 & 0x20000) || (param_0 & 8) || (param_0 & 0x10) || (param_0 & 0x1000000) ||
        (param_0 & 0x800000) || (param_0 & 0x20) || (param_0 & 0x4000000) ||
        (param_0 & 0x10000000) || (param_0 & 0x800) || (param_0 & 0x20000000) ||
        daPy_getPlayerActorClass()->checkEnemyAttentionLock() || dMeter2Info_isGameStatus(2))
    {
        return false;
    }
    return true;
}

bool dMeterMap_c::isFmapScreen() {
    if (dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 0 ||
        dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 5 ||
        dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 3 ||
        dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 4 ||
        dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 6)
    {
        return true;
    }
    return false;
}

bool dMeterMap_c::isDmapScreen() {
    if (dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo()) == 1) {
        return true;
    }

    return false;
}

void dMeterMap_c::meter_map_move(u32 param_0) {
    if (isShow(param_0) && isMapOpenCheck()) {
        if (dMeter2Info_getPauseStatus() == 6) {
            if (!dMeter2Info_isSub2DStatus(1) && isFmapScreen()) {
                dMeter2Info_setMapStatus(3);
                dMeter2Info_setMapKeyDirection(0x400);
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            }
            dMeter2Info_resetPauseStatus();
        } else if (
            #if DEBUG
            dMw_RIGHT_TRIGGER() &&
            #else
            dMw_LEFT_TRIGGER() && 
            #endif
            !dComIfGp_event_runCheck() &&
            !dMsgObject_isTalkNowCheck() &&
            (dMeter2Info_getMapStatus() == 0 || dMeter2Info_getMapStatus() == 1))
        {
            #if !DEBUG
            if (dMeter2Info_getMapStatus() == 0 && !dMeter2Info_isSub2DStatus(1)) {
                if (isFmapScreen() || isDmapScreen()) {
                    dMeter2Info_setMapStatus(2);
                    dMeter2Info_setMapKeyDirection(0x200);
                    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                                -1.0f, 0);
                    dMeter2Info_set2DVibration();
                }
            }
        } else if (dMw_RIGHT_TRIGGER() && !dComIfGp_event_runCheck() &&
                    !dMsgObject_isTalkNowCheck() &&
                    (dMeter2Info_getMapStatus() == 0 || dMeter2Info_getMapStatus() == 1))
        {
            #endif
            if (!dMeter2Info_isSub2DStatus(1)) {
                if (isFmapScreen() || isDmapScreen()) {
                    dMeter2Info_setMapStatus(2);
                    dMeter2Info_setMapKeyDirection(0x400);
                    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                                -1.0f, 0);
                    dMeter2Info_set2DVibration();
                }
            }
        }
    }
}

void dMeterMap_c::keyCheck() {
    if (dMw_LEFT_TRIGGER() && !isEventRunCheck() &&
        (dMeter2Info_getMapStatus() == 0 || dMeter2Info_getMapStatus() == 1))
    {
        if (dMeter2Info_getMapStatus() == 1) {
            if (isDispPosInsideFlg()) {
                setDispPosOutsideFlg_SE_On();
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_CLOSE_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_setMapStatus(0);
            }
        } else if (dMeter2Info_getMapStatus() == 0) {
            if (!dMeter2Info_isSub2DStatus(1) && (isFmapScreen() || isDmapScreen())) {
                dMeter2Info_setMapStatus(2);
                dMeter2Info_setMapKeyDirection(0x200);
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            } else {
                setDispPosInsideFlg_SE_On();
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
                dMeter2Info_setMapStatus(1);
            }
        }
    } else if (dMw_RIGHT_TRIGGER() && !isEventRunCheck() &&
               (dMeter2Info_getMapStatus() == 0 || dMeter2Info_getMapStatus() == 1))
    {
        if (dMeter2Info_getMapStatus() == 0) {
            if (isEnableDispMapAndMapDispSizeTypeNo()) {
                if (!isDispPosInsideFlg()) {
                    setDispPosInsideFlg_SE_On();
                    Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                             -1.0f, 0);
                    dMeter2Info_set2DVibration();
                    dMeter2Info_setMapStatus(1);
                }
            } else if (!dMeter2Info_isSub2DStatus(1) && (isFmapScreen() || isDmapScreen())) {
                dMeter2Info_setMapStatus(2);
                dMeter2Info_setMapKeyDirection(0x400);
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            }
        } else if (dMeter2Info_getMapStatus() == 1 && isDispPosInsideFlg()) {
            if (!dMeter2Info_isSub2DStatus(1) && (isFmapScreen() || isDmapScreen())) {
                dMeter2Info_setMapStatus(2);
                dMeter2Info_setMapKeyDirection(0x400);
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_OPEN_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            } else {
                setDispPosOutsideFlg_SE_On();
                Z2GetAudioMgr()->seStart(Z2SE_SY_MAP_CLOSE_S, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_setMapStatus(0);
            }
        }
    }
}
