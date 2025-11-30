#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTTexture.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_fmap_map.h"
#include "m_Do/m_Do_graphic.h"

static u8 twoValueLineInterpolation(u8 i_value1, u8 i_value2, f32 i_param) {
    return (u8)((f32)i_value1 + i_param * ((f32)i_value2 - (f32)i_value1));
}

static void twoColorLineInterporation(GXColor const& i_color1, GXColor const& i_color2,
                                      f32 i_param, GXColor& o_color) {
    o_color.r = twoValueLineInterpolation(i_color1.r, i_color2.r, i_param);
    o_color.g = twoValueLineInterpolation(i_color1.g, i_color2.g, i_param);
    o_color.b = twoValueLineInterpolation(i_color1.b, i_color2.b, i_param);
    o_color.a = twoValueLineInterpolation(i_color1.a, i_color2.a, i_param);
}

void renderingFmap_c::init(u8* param_0, u16 i_texWidth, u16 i_texHeight, u16 param_3, u16 param_4) {
    field_0x4 = param_0;
    mTexWidth = i_texWidth;
    mTexHeight = i_texHeight;
    field_0x20 = param_3;
    field_0x22 = param_4;
    mPosX = 0.0f;
    mPosZ = 0.0f;
    mDrawEnable = false;
}

void renderingFmap_c::entry(dMenu_Fmap_world_data_c* i_worldData, int i_startStageNo,
                            f32 i_posX, f32 i_posY, f32 i_scale) {
    mpWorldData = i_worldData;
    mStartStageNo = i_startStageNo;
    mPosX = i_posX;
    mPosZ = i_posY;
    mCmPerTexel = i_scale;
    field_0x8 = mCmPerTexel * field_0x20 * mDoGph_gInf_c::getScale();
    field_0xc = mCmPerTexel * field_0x22;
    dComIfGd_setCopy2D(this);
}

bool renderingFmap_c::isSwitchSpecialOff(int i_swbit) {
    return !strcmp(dComIfGp_getStartStageName(), "F_SP121") && i_swbit == 0xb2;
}

bool renderingFmap_c::isSwitch(dDrawPath_c::group_class const* i_group) {
    if (i_group->mSwbit == 0xff) {
        return true;
    }

    if (i_group->field_0x1 == 0) {
        if (isSwitchSpecialOff(i_group->mSwbit)) {
            return true;
        }
    } else {
        if (isSwitchSpecialOff(i_group->mSwbit)) {
            return false;
        }
    }

    if (mRegionNo == dComIfGp_getNowLevel() && mStageNo == mStartStageNo) {
        if (i_group->field_0x1 == 0) {
            return !dComIfGs_isSwitch(i_group->mSwbit, mRoomNo);
        } else {
            return dComIfGs_isSwitch(i_group->mSwbit, mRoomNo) ? true : false;
        }
    } else {
        if (i_group->mSwbit < 0x80) {
            if (i_group->field_0x1 != 0) {
                return dComIfGs_isStageSwitch(mSaveTableNo, i_group->mSwbit) ? true : false;
            } else {
                return !dComIfGs_isStageSwitch(mSaveTableNo, i_group->mSwbit);
            }
        } else {
            return i_group->field_0x1 == 0;
        }
    }
}

int renderingFmap_c::getPointStagePathInnerNo(dMenu_Fmap_region_data_c* i_regionData,
                                               f32 i_offsetX, f32 i_offsetY, int i_stageNo,
                                               int* o_stageNo, int* o_roomNo) {
    return i_regionData->getPointStagePathInnerNo(mPosX + i_offsetX * mCmPerTexel,
                                                  mPosZ + i_offsetY * mCmPerTexel,
                                                  i_stageNo, o_stageNo, o_roomNo);
}

void renderingFmap_c::preDrawPath() {
    mEye.x = mPosX;
    mEye.y = mPosZ;
    mEye.z = -5000.0f;
    mCenter.x = mPosX;
    mCenter.y = mPosZ;
    mCenter.z = 5000.0f;
    mUp.x = 0.0f;
    mUp.y = -1.0f;
    mUp.z = 0.0f;
    mDoMtx_lookAt(mViewMtx, &mEye, &mCenter, &mUp, 0);
    
    GXLoadPosMtxImm(mViewMtx, GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_F32, 0);
    GXSetMisc(GX_MT_XF_FLUSH, 8);
}

bool renderingFmap_c::isDrawPath() {
    return true;
}

bool renderingFmap_c::isDrawRoom() {
    return (bool)((mRegionNo == dComIfGp_getNowLevel() && mStageNo == mStartStageNo
            && mRoomNo == dComIfGp_roomControl_getStayNo())
        || mpRoomData->isArrival());
}

void renderingFmap_c::postDrawPath() {
    GXSetMisc(GX_MT_XF_FLUSH, 0);
    mDoMtx_lookAt(mViewMtx, &mEye, &mCenter, &mUp, 0);
    GXLoadPosMtxImm(mViewMtx, GX_PNMTX0);
}

void renderingFmap_c::postRenderingMap() {
    dRenderingFDAmap_c::postRenderingMap();
    mDrawEnable = true;
}

void renderingFmap_c::roomSetteing() {
    mRegionOffsetX = mpRegionData->getRegionOffsetX();
    mRegionOffsetZ = mpRegionData->getRegionOffsetZ();
    mStageOffsetX = mRegionOffsetX + mpStageData->getOffsetX();
    mStageOffsetZ = mRegionOffsetZ + mpStageData->getOffsetZ();
    f32 offset_z = mStageOffsetZ;

    cXyz eye, center;
    eye.x = mPosX - mStageOffsetX;
    eye.y = mPosZ - offset_z;
    eye.z = mEye.z;
    center.x = eye.x;
    center.y = eye.y;
    center.z = mCenter.z;

    Mtx viewMtx;
    mDoMtx_lookAt(viewMtx, &eye, &center, &mUp, 0);
    GXLoadPosMtxImm(viewMtx, GX_PNMTX0);
}

dDrawPath_c::room_class* renderingFmap_c::getFirstRoomPointer() {
    dDrawPath_c::room_class* room = NULL;
    getFirstRegion();

    if (mpRoomData != NULL) {
        if (mpFmapData != NULL) {
            room = mpFmapData->getMapPath();
        }

        while (mpRoomData != NULL && (room == NULL || !isDrawRoom())) {
            if (mpFmapData != NULL) {
                room = getNextRoomPointer();
            }
        }

        if (room != NULL) {
            roomSetteing();
        }
    }

    return room;
}

void renderingFmap_c::getFirstRegion() {
    mpStageData = NULL;
    mpRegionData = mpWorldData->getMenuFmapRegionTop();
    if (mpRegionData != NULL) {
        mRegionNo = mpRegionData->getRegionNo();
        mRegionIndex = 0;
        getFirstStage();
    }
}

void renderingFmap_c::getFirstStage() {
    mpRoomData = NULL;
    mpStageData = mpRegionData->getMenuFmapStageDataTop();
    if (mpStageData != NULL) {
        mpStageArc = mpStageData->getStageArc();
        mSaveTableNo = mpStageArc->getSaveTableNo();
        mVisitedRoomSaveTableNo = mpStageArc->getVisitedRoomSaveTableNo();
        mStageNo = 0;
        getFirstRoom();
    }
}

void renderingFmap_c::getFirstRoom() {
    mpFmapData = NULL;
    mpRoomData = mpStageData->getFmapRoomDataTop();
    if (mpRoomData != NULL) {
        mRoomIndex = 0;
        mRoomNo = mpRoomData->getRoomNo();
        mpFmapData = mpRoomData->getFmapData();
    }
}

bool renderingFmap_c::getNextRoom() {
    bool ret = false;
    mpRoomData = mpRoomData->getNextData();
    if (mpRoomData == NULL) {
        ret = getNextStage();
    } else {
        mRoomIndex++;
        mpFmapData = mpRoomData->getFmapData();
        mRoomNo = mpRoomData->getRoomNo();
    }
    return ret;
}

bool renderingFmap_c::getNextStage() {
    bool ret = false;
    mpStageData = mpStageData->getNextData();
    if (mpStageData == NULL) {
        ret = getNextRegion();
    } else {
        mStageNo++;
        mpStageArc = mpStageData->getStageArc();
        mSaveTableNo = mpStageArc->getSaveTableNo();
        mVisitedRoomSaveTableNo = mpStageArc->getVisitedRoomSaveTableNo();
        getFirstRoom();
    }
    return ret;
}

bool renderingFmap_c::getNextRegion() {
    bool ret = false;
    mpRegionData = mpRegionData->getNextData();
    if (mpRegionData == NULL) {
        ret = true;
    } else {
        mRegionIndex++;
        mRegionNo = mpRegionData->getRegionNo();
        getFirstStage();
    }
    return ret;
}

dDrawPath_c::room_class* renderingFmap_c::getNextRoomPointer() {
    room_class* room;

    do {
        room = NULL;
        if (getNextRoom()) {
            return NULL;
        }
        if (mpRoomData != NULL) {
            room = mpFmapData->getMapPath();
        }
    } while (mpRoomData != NULL && (room == NULL || !isDrawRoom()));

    if (room != NULL) {
        roomSetteing();
    }

    return room;
}

void dMenu_FmapMap_c::setFmapPaletteColor(renderingFmap_c::palette_e i_palette, u8 i_r,
                                          u8 i_g, u8 i_b, u8 i_a) {
    int color;
    if (i_a >= 0xe0) {
        color = ((i_r & 0xf8) << 7) | ((i_g & 0xf8) << 2) | (i_b >> 3) | (1 << 15);
    } else {
        color = ((i_r & 0xf0) << 4) | (i_g & 0xf0) | (i_b >> 4) | ((i_a & 0xe0) << 7);
    }

    dMpath_RGB5A3_palDt_s* palette_entry = &m_palette[i_palette];
    palette_entry->field_0x0.color = color;
    palette_entry->field_0x2.color = color;
    palette_entry->field_0x4.color = color;
    palette_entry->field_0x6.color = color;
    DCStoreRange(&m_palette[i_palette], 8);
}

void dMenu_FmapMap_c::setFmapPaletteColor(renderingFmap_c::palette_e i_palette,
                                          GXColor const& i_color) {
    setFmapPaletteColor(i_palette, i_color.r, i_color.g, i_color.b, i_color.a);
}

bool dMenu_FmapMap_c::isFlashRoomNoCheck(int i_roomNo) const {
    bool ret = false;
    u8* ptr = mFlashRooms;
    for (int i = 0; i < mFlashRoomCount; ptr++, i++) {
        if (*ptr == i_roomNo) {
            ret = true;
            break;
        }
    }
    return ret;
}

void dMenu_FmapMap_c::setPointColor(f32 i_param) {
    static const palette_e palNo[8] = {
        PALETTE_11,
        PALETTE_12,
        PALETTE_13,
        PALETTE_14,
        PALETTE_15,
        PALETTE_16,
        PALETTE_17,
        PALETTE_18,
    };
    
    GXColor color;
    if (mFlash && mRegionCursor == dComIfGp_getNowLevel() && mStartStageNo == mStageCursor) {
        for (int i = 0; i < 8; i++) {
            twoColorLineInterporation(m_res->field_0xe8[i], m_res->field_0x108[i], i_param, color);
            setFmapPaletteColor(palNo[i], color);
        }
    } else {
        for (int i = 0; i < 8; i++) {
            twoColorLineInterporation(m_res->field_0x128[i], m_res->field_0x148[i], i_param, color);
            setFmapPaletteColor(palNo[i], color);
        }
    }
}

dMenu_FmapMap_c::dMenu_FmapMap_c() {
    mResTIMG = NULL;
    mMapImage_p = NULL;
    m_res = NULL;
    m_palette = NULL;
    field_0xcc = 0;
    mZoomRate = 0.0f;
    mLineNo = 0;
    mFlash = false;
    mLastFlash = false;
    mStageCursor = 0;
    mLastStageCursor = 0;
    mRegionCursor = 0;
    mFlashTimer = 0;
    mFlashRooms = NULL;
    mFlashRoomCount = 0;
}


dMenu_FmapMap_c::~dMenu_FmapMap_c() {
    _delete();
}

void dMenu_FmapMap_c::_create(u16 i_texWidth, u16 i_texHeight, u16 param_2, u16 param_3, void* i_res) {
    m_res = (dMfm_prm_res_s*)i_res;
    m_palette = m_res->palette_data;
    field_0xcc = 0x6c;
    mFlash = false;
    mStageCursor = 0;
    mLastStageCursor = -1;
    mLastFlash = mFlash;
    mFlashTimer = 0;
    setTexture(i_texWidth, i_texHeight, param_2, param_3);
}

void dMenu_FmapMap_c::_delete() {
    if (mResTIMG != NULL) {
        delete mResTIMG;
        mResTIMG = NULL;
    }
    if (mMapImage_p != NULL) {
        delete[] mMapImage_p;
        mMapImage_p = NULL;
    }
}

const dMfm_HIO_prm_res_src_s dMfm_HIO_prm_res_src_s::m_other = {30};

void dMenu_FmapMap_c::draw() {
    { int unused; }

    f32 f30 = 0.0f;
    if (mFlashTimer < dMfm_HIO_prm_res_src_s::m_other.mFlashDuration / 2) {
        f30 =
            (int)(dMfm_HIO_prm_res_src_s::m_other.mFlashDuration / 2 - mFlashTimer) /
            (f32)(int)(dMfm_HIO_prm_res_src_s::m_other.mFlashDuration / 2);
    } else {
        f30 =
            (int)(mFlashTimer - dMfm_HIO_prm_res_src_s::m_other.mFlashDuration / 2) /
            (f32)(int)(dMfm_HIO_prm_res_src_s::m_other.mFlashDuration / 2);
    }
    setPointColor(1.0f - f30);

    GXColor color;
    f32 f31;
    f31 = getRateWithFrameCount(m_res->field_0x168);
    f31 = cM_ssin(f31 * 0x10000 - 0x8000) * 0.5f + 0.5f;
    twoColorLineInterporation(m_res->field_0xd8, m_res->field_0xdc, f31, color);
    setFmapPaletteColor(PALETTE_19, color);

    f31 = getRateWithFrameCount(m_res->field_0x16a);
    f31 = cM_ssin(f31 * 0x10000 - 0x8000) * 0.5f + 0.5f;
    twoColorLineInterporation(m_res->field_0xe0, m_res->field_0xe4, f31, color);
    setFmapPaletteColor(PALETTE_1A, color);

    renderingMap();
}

void dMenu_FmapMap_c::rendering(dDrawPath_c::line_class const* i_line) {
    if (i_line->field_0x1 == 4) {
        for (mLineNo = 0; mLineNo < 2; mLineNo++) {
            renderingDecoration(i_line);
        }
    } else {
        dDrawPath_c::rendering(i_line);
    }
}

int dMenu_FmapMap_c::getLineWidth(int param_0) {
    static u8 const l_lineWidthPatData[5] = {0, 6, 12, 0, 0};
    static u8 const l_lineWidthPatData2[5] = {0, 0, 6, 0, 0};

    int iVar2 = 0;
    if (param_0 == 4) {
        switch (mLineNo) {
        case 0:
        case 1:
            iVar2 = m_res->field_0x16d;
        }

        if (mZoomRate > 1.0f) {
            return (int)((f32)iVar2 * (mZoomRate - 1.0f));
        } else {
            return 0;
        }
    } else if (mZoomRate > 1.5f) {
        return l_lineWidthPatData[param_0];
    } else {
        return l_lineWidthPatData2[param_0];
    }
}

 bool dMenu_FmapMap_c::isDrawType(int param_0) {
    return (param_0 >> 6) & 1 ? false : true;
}

void dMenu_FmapMap_c::setFlashOn(int i_regionNo, int i_stageNo, u8* i_rooms, int i_roomCount) {
    mLastFlash = mFlash;
    mFlash = true;
    mRegionCursor = i_regionNo;
    mStageCursor = (u8)i_stageNo;
    mFlashRooms = i_rooms;
    mFlashRoomCount = i_roomCount;
}

const GXColor* dMenu_FmapMap_c::getLineColor(int param_0, int param_1) {
    static GXColor black = {0, 0, 0, 0};
    static const GXColor borderColor0 = {0x64, 0, 0, 0};
    static const GXColor borderColor1 = {0x68, 0, 0, 0};

    GXColor* color = &black;
    if (param_1 == 4) {
        switch (mLineNo) {
        case 0:
            return &borderColor0;
        case 1:
            return &borderColor1;
        }
    } else {
        *color = *getColor(param_0);
    }

    return color;
}

const GXColor* dMenu_FmapMap_c::getBackColor() const {
    static const GXColor l_mapBaseColor = {0, 0, 0, 0};
    return &l_mapBaseColor;
}

const GXColor* dMenu_FmapMap_c::getColor(int param_0) {
    static const GXColor l_dungeon_onColor[9] = {
        {0x04, 0, 0, 0},
        {0x08, 0, 0, 0},
        {0x0C, 0, 0, 0},
        {0x00, 0, 0, 0},
        {0x10, 0, 0, 0},
        {0x14, 0, 0, 0},
        {0x18, 0, 0, 0},
        {0x1C, 0, 0, 0},
        {0x20, 0, 0, 0},
    };
    static const GXColor l_dungeon_stayColor[9] = {
        {0x24, 0, 0, 0},
        {0x28, 0, 0, 0},
        {0x2C, 0, 0, 0},
        {0x00, 0, 0, 0},
        {0x30, 0, 0, 0},
        {0x34, 0, 0, 0},
        {0x38, 0, 0, 0},
        {0x3C, 0, 0, 0},
        {0x40, 0, 0, 0},
    };
    static const GXColor l_dungeon_pointColor[9] = {
        {0x44, 0, 0, 0},
        {0x48, 0, 0, 0},
        {0x4C, 0, 0, 0},
        {0x00, 0, 0, 0},
        {0x50, 0, 0, 0},
        {0x54, 0, 0, 0},
        {0x58, 0, 0, 0},
        {0x5C, 0, 0, 0},
        {0x60, 0, 0, 0},
    };

    const GXColor* list = l_dungeon_onColor;
    if (mFlash && mRegionCursor == getNowDrawRegionNo() && mStageNo == mStageCursor) {
        if (mFlashRooms == NULL) {
            list = l_dungeon_pointColor;
        } else if (isFlashRoomNoCheck(mRoomNo)) {
            list = l_dungeon_pointColor;
        } else if (getNowDrawRegionNo() == dComIfGp_getNowLevel() && mStageNo == mStartStageNo) {
            list = l_dungeon_stayColor;
        }
    } else {
        if (getNowDrawRegionNo() == dComIfGp_getNowLevel() && mStageNo == mStartStageNo) {
            list = l_dungeon_stayColor;
        }
    }

    return &list[param_0];
}

void dMenu_FmapMap_c::setTexture(u16 i_width, u16 i_height, u16 param_2, u16 param_3) {
    mMapImage_p = NULL;
    mResTIMG = NULL;
    int size = GXGetTexBufferSize(i_width, i_height, GX_TF_C8, 0, 0);
    mMapImage_p = new (0x20) u8[size];
    init(mMapImage_p, i_width, i_height, param_2, param_3);
    mResTIMG = new (0x20) ResTIMG();
    makeResTIMG(mResTIMG, i_width, i_height, mMapImage_p, (u8*)m_palette, 0x1b);
}

void dMenu_FmapMap_c::setRendering(dMenu_Fmap_world_data_c* i_worldData, int i_startStageNo,
                                   f32 i_posX, f32 i_posY, f32 i_scale, f32 i_zoomRate) {
    mZoomRate = i_zoomRate;
    if (mFlash != mLastFlash || mStageCursor != mLastStageCursor) {
        mFlashTimer = dMfm_HIO_prm_res_src_s::m_other.mFlashDuration;
        mLastFlash = mFlash;
        mLastStageCursor = mStageCursor;
    } else {
        if (mFlashTimer != 0) {
            mFlashTimer--;
        } else {
            mFlashTimer = dMfm_HIO_prm_res_src_s::m_other.mFlashDuration;
        }
    }
    entry(i_worldData, i_startStageNo, i_posX, i_posY, i_scale);
}
