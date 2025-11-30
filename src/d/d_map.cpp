/**
 * d_map.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_map.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"

f32 renderingAmap_c::getIconSize(u8 i_icon) const {
    static f32 const l_iconSize[] = {
        0.12f, 0.12f, 0.08f, 0.12f, 0.08f, 0.16f, 0.12f, 0.0f,  0.12f,
        0.08f, 0.08f, 0.08f, 0.08f, 0.08f, 0.08f, 0.08f, 0.08f,
    };

    return l_iconSize[i_icon];
}

dMap_prm_res_s* dMap_HIO_prm_res_dst_s::m_res;

inline u8 twoValueLineInterpolation(u8 param_0, u8 param_1, f32 param_2) {
    return ((f32)param_0 + (param_2 * ((f32)param_1 - (f32)param_0)));
}

void renderingAmap_c::draw() {
    f32 tmp = ((f32)(g_Counter.mCounter0 % dMap_HIO_prm_res_dst_s::m_res->field_0x1aa) /
               (f32)dMap_HIO_prm_res_dst_s::m_res->field_0x1aa);
    tmp = tmp;

    f32 temp_f1 = 0.5f + ((0.5f * cM_ssin(65536.0f * tmp - 32768.0f)));
    u8 temp_r31 = twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x198,
                                            dMap_HIO_prm_res_dst_s::m_res->field_0x19c, temp_f1);
    u8 temp_r30 = twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x199,
                                            dMap_HIO_prm_res_dst_s::m_res->field_0x19d, temp_f1);
    u8 temp_r29 = twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x19a,
                                            dMap_HIO_prm_res_dst_s::m_res->field_0x19e, temp_f1);
    u8 temp_r28 = twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x19b,
                                            dMap_HIO_prm_res_dst_s::m_res->field_0x19f, temp_f1);
    setAmapPaletteColor(0x2D, temp_r31, temp_r30, temp_r29, temp_r28);

    f32 tmp2 = (f32)(g_Counter.mCounter0 % dMap_HIO_prm_res_dst_s::m_res->field_0x1ac) /
               (f32)dMap_HIO_prm_res_dst_s::m_res->field_0x1ac;
    tmp2 = tmp2;

    f32 temp_f1_2 = 0.5f + (0.5f * cM_ssin(65536.0f * tmp2 - 32768.0f));
    u8 temp_r31_2 =
        twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x1a0,
                                  dMap_HIO_prm_res_dst_s::m_res->field_0x1a4, temp_f1_2);
    u8 temp_r30_2 =
        twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x1a1,
                                  dMap_HIO_prm_res_dst_s::m_res->field_0x1a5, temp_f1_2);
    u8 temp_r29_2 =
        twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x1a2,
                                  dMap_HIO_prm_res_dst_s::m_res->field_0x1a6, temp_f1_2);
    u8 temp_r28_2 =
        twoValueLineInterpolation(dMap_HIO_prm_res_dst_s::m_res->field_0x1a3,
                                  dMap_HIO_prm_res_dst_s::m_res->field_0x1a7, temp_f1_2);
    setAmapPaletteColor(0x2E, temp_r31_2, temp_r30_2, temp_r29_2, temp_r28_2);

    renderingDAmap_c::draw();
}

int renderingAmap_c::getDispType() const {
    int disp_type;

    switch (dStage_stagInfo_GetUpButton(dComIfGp_getStage()->getStagInfo())) {
    case 7:
        disp_type = 1;
        break;
    case 0:
    case 5:
        disp_type = 0;
        break;
    case 1:
        disp_type = 1;
        break;
    case 2:
        disp_type = 2;
        break;
    case 3:
        disp_type = 3;
        break;
    case 4:
        disp_type = 4;
        break;
    case 6:
        disp_type = 5;
        break;
    default:
        JUT_ASSERT(0, FALSE);
        break;
    }

    return disp_type;
}

void renderingAmap_c::beforeDrawPath() {
    if (isDrawOutSideTrim()) {
        field_0x38 = 0;
        getOutSideBlackLineNumber();

        do {
            drawPath();
        } while (isOutSideBlackLine());

        field_0x38 = 2;
    }
}

void renderingAmap_c::afterDrawPath() {
    renderingPlusDoorAndCursor_c::afterDrawPath();
}

void renderingAmap_c::rendering(dDrawPath_c::room_class const* i_room) {
    dDrawPath_c::rendering(i_room);
}

void renderingAmap_c::drawPath() {
    dDrawPath_c::drawPath();
}

void renderingAmap_c::rendering(dDrawPath_c::line_class const* i_line) {
    if (isDrawType(i_line->field_0x0)) {
        if (isDrawOutSideTrim() && field_0x38 != 2) {
            if (i_line->field_0x1 == 2) {
                renderingDecoration(i_line);
            }
        } else if (i_line->field_0x1 == 4) {
            field_0x3c = 0;

            while (field_0x3c < 2) {
                renderingDecoration(i_line);
                field_0x3c++;
            }
        } else {
            dDrawPath_c::rendering(i_line);
        }
    }
}

f32 renderingAmap_c::getPlayerCursorSize() {
    return dMap_HIO_prm_res_dst_s::m_res->cursor_size;
}

f32 renderingAmap_c::getRestartCursorSize() {
    return getPlayerCursorSize();
}

void renderingAmap_c::rendering(dDrawPath_c::poly_class const* i_poly) {
    if (isDrawType(i_poly->field_0x0) && (!isDrawOutSideTrim() || field_0x38 == 2)) {
        dDrawPath_c::rendering(i_poly);
    }
}

bool renderingAmap_c::isDrawOutSideTrim() {
    bool rt = false;

    if (getDispType() == 0 || getDispType() == 4 || getDispType() == 3 || getDispType() == 2 ||
        getDispType() == 5)
    {
        rt = dMap_HIO_prm_res_dst_s::m_res->field_0x1ae > 0;
    }

    return rt;
}

int renderingAmap_c::getOutSideBlackLineNumber() {
    m_outSideBlackLineCnt = 0;
    return dMap_HIO_prm_res_dst_s::m_res->field_0x1ae;
}

bool renderingAmap_c::isOutSideBlackLine() {
    bool rt = false;
    m_outSideBlackLineCnt++;

    if (m_outSideBlackLineCnt < dMap_HIO_prm_res_dst_s::m_res->field_0x1ae) {
        rt = true;
    }

    return rt;
}

int renderingAmap_c::getLineWidthSub(int i_no) {
    static const u8 l_lineWidthPatOff[] = {0x00, 0x00, 0x06, 0x00, 0x00};
    static const u8 l_lineWidthPatOn[] = {0x06, 0x06, 0x06, 0x00, 0x00};
    static const u8 l_lineWidthPatStay[] = {0x06, 0x06, 0x0C, 0x00, 0x00};

    const u8* pat = l_lineWidthPatOff;

    if (mRoomNo == mRoomNoSingle) {
        pat = l_lineWidthPatStay;
    } else if (dMapInfo_n::isVisitedRoom(mRoomNo)) {
        pat = l_lineWidthPatOn;
    }

    return pat[i_no];
}

s32 renderingAmap_c::getDecorationLineWidth(int param_0) {
    int var_r31 = 0;

    if (param_0 == 4) {
        switch (field_0x3c) {
        case 0:
            var_r31 = dMap_HIO_prm_res_dst_s::m_res->field_0x1a8;
            break;
        case 1:
            var_r31 = dMap_HIO_prm_res_dst_s::m_res->field_0x1a9;
            break;
        }

        return var_r31;
    }

    switch (m_outSideBlackLineCnt) {
    case 0:
        var_r31 = dMap_HIO_prm_res_dst_s::m_res->field_0x1b0;
        break;
    case 1:
        var_r31 = dMap_HIO_prm_res_dst_s::m_res->field_0x1b2;
        break;
    case 2:
        var_r31 = dMap_HIO_prm_res_dst_s::m_res->field_0x1b4;
        break;
    case 3:
        var_r31 = dMap_HIO_prm_res_dst_s::m_res->field_0x1b6;
        break;
    }

    if (var_r31 > 0) {
        var_r31 += getLineWidthSub(param_0);

        if (var_r31 > 0xFF) {
            var_r31 = 0xFF;
        }
    }

    return var_r31;
}

int renderingAmap_c::getLineWidth(int i_no) {
    return getLineWidthSub(i_no);
}

const GXColor* dMap_c::getColor(int i_no) {
    static GXColor const l_dungeon_offColor[] = {
        {0x08, 0x00, 0x00, 0x00}, {0x0C, 0x00, 0x00, 0x00}, {0x10, 0x00, 0x00, 0x00},
        {0x04, 0x00, 0x00, 0x00}, {0x14, 0x00, 0x00, 0x00}, {0x18, 0x00, 0x00, 0x00},
        {0x1C, 0x00, 0x00, 0x00}, {0x20, 0x00, 0x00, 0x00}, {0x24, 0x00, 0x00, 0x00},
    };

    static GXColor const l_dungeon_onColor[] = {
        {0x28, 0x00, 0x00, 0x00}, {0x2C, 0x00, 0x00, 0x00}, {0x30, 0x00, 0x00, 0x00},
        {0x04, 0x00, 0x00, 0x00}, {0x34, 0x00, 0x00, 0x00}, {0x38, 0x00, 0x00, 0x00},
        {0x3C, 0x00, 0x00, 0x00}, {0x40, 0x00, 0x00, 0x00}, {0x44, 0x00, 0x00, 0x00},
    };

    static GXColor const l_dungeon_stayColor[] = {
        {0x48, 0x00, 0x00, 0x00}, {0x4C, 0x00, 0x00, 0x00}, {0x50, 0x00, 0x00, 0x00},
        {0x04, 0x00, 0x00, 0x00}, {0x54, 0x00, 0x00, 0x00}, {0x58, 0x00, 0x00, 0x00},
        {0x5C, 0x00, 0x00, 0x00}, {0x60, 0x00, 0x00, 0x00}, {0x64, 0x00, 0x00, 0x00},
    };

    const GXColor* color_tbl = l_dungeon_offColor;

    if (field_0x8e == 2 && field_0x8f != 0 && field_0x8f < 6) {
        if (mRoomNoSingle == field_0x88) {
            if (mRoomNo == field_0x88) {
                color_tbl = l_dungeon_stayColor;
            } else if (mRoomNo == field_0x84) {
                color_tbl = l_dungeon_onColor;
            }
        } else if (mRoomNo == field_0x88) {
            color_tbl = l_dungeon_onColor;
        } else if (mRoomNo == field_0x84) {
            color_tbl = l_dungeon_stayColor;
        }
    } else if (mRoomNo == mRoomNoSingle) {
        color_tbl = l_dungeon_stayColor;
    } else if (dMapInfo_n::isVisitedRoom(mRoomNo)) {
        color_tbl = l_dungeon_onColor;
    }

    return &color_tbl[i_no];
}

GXColor* renderingAmap_c::getLineColor(int param_0, int param_1) {
    static GXColor backColor = {0x04, 0x00, 0x00, 0x00};
    static const GXColor borderColor0 = {0xB4, 0x00, 0x00, 0x00};
    static const GXColor borderColor1 = {0xB8, 0x00, 0x00, 0x00};

    GXColor* color = &backColor;

    if (param_1 == 4) {
        switch (field_0x3c) {
        case 0:
            *color = borderColor0;
            break;
        case 1:
            *color = borderColor1;
            break;
        default:
            JUT_ASSERT(0, FALSE);
            break;
        }
    } else {
        *color = *getColor(param_0);
    }

    return color;
}

const GXColor* renderingAmap_c::getDecoLineColor(int param_0, int param_1) {
    static GXColor const colorTable[] = {
        {0xBC, 0x00, 0x00, 0x00},
        {0xC0, 0x00, 0x00, 0x00},
        {0xC4, 0x00, 0x00, 0x00},
        {0xC8, 0x00, 0x00, 0x00},
    };

    const GXColor* color;
    if (isDrawOutSideTrim() && field_0x38 != 2) {
        JUT_ASSERT(0, m_outSideBlackLineCnt < ARRAY_SIZEU(colorTable));
        color = &colorTable[m_outSideBlackLineCnt];
    } else {
        color = getLineColor(param_0, param_1);
    }

    return color;
}

const Vec* dMap_c::getIconPosition(dTres_c::typeGroupData_c* i_data) const {
    return renderingPlusDoorAndCursor_c::getIconPosition(i_data);
}

int renderingAmap_c::getIconGroupNumber(u8 param_0) const {
    return renderingDAmap_c::getIconGroupNumber(param_0);
}

bool renderingAmap_c::hasMap() const {
    int stay_type = getStayType();

    return stay_type == 1 && dMapInfo_n::chkGetMap();
}

bool renderingAmap_c::isRendDoor() const {
    int disp_type = getDispType();
    return disp_type == 1 || disp_type == 2;
}

bool renderingAmap_c::isCheckFloor() const {
    return true;
}

bool renderingAmap_c::isRendRestart() const {
    return true;
}

bool renderingAmap_c::isRendCursor() const {
    return true;
}

bool renderingAmap_c::isRendAllRoom() const {
    int disp_type = getDispType();
    return disp_type != 2 && disp_type != 3 && disp_type != 5;
}

int renderingAmap_c::getStayType() const {
    static int const l_stayType[] = {0, 1, 0, 0, 0, 0, 0};

    return l_stayType[getDispType()];
}

bool renderingAmap_c::isDrawIconSingle2(dTres_c::data_s const* i_data, bool param_1, bool param_2,
                                        int param_3) const {
    JUT_ASSERT(0, i_data != NULL);

    bool var_r31 = false;
    int disp_type = getDispType();
    int stay_type = getStayType();
    int group_no = dTres_c::getTypeToTypeGroupNo(i_data->mType);
    bool has_map = hasMap();

    bool var_r23 = false;
    if (stay_type == 1 && dMapInfo_n::chkGetCompass()) {
        var_r23 = true;
    }

    switch (group_no) {
    case 0:
        if (stay_type == 1) {
            JUT_ASSERT(0, i_data->mNo != 255);

            if (var_r23 && i_data->mNo != 255 && !dComIfGs_isTbox(i_data->mNo) && param_1) {
                var_r31 = true;
            }
        }
        break;
    case 1:
    case 8:
        if (stay_type == 1) {
            if ((has_map || param_2) && param_1) {
                var_r31 = true;
            }
        } else if (param_2) {
            if ((i_data->mSwBit == 255 ||
                 (i_data->mSwBit != 255 && dComIfGs_isSwitch(i_data->mSwBit, i_data->mRoomNo))) &&
                param_1)
            {
                var_r31 = true;
            }
        }
        break;
    case 2:
        if (stay_type == 1) {
            if (var_r23 && !dComIfGs_isTbox(i_data->mNo) && param_1) {
                var_r31 = true;
            }
        }
        break;
    case 4:
        if (stay_type == 0) {
            bool temp_r23_2 = false;

            if (dComIfGp_isLightDropMapVisible()) {
                if (dComIfGp_getStartStageDarkArea() == 2) {
                    int temp_r3 = i_data->mNo;
                    if (temp_r3 == 0x33 || temp_r3 == 0x34 || temp_r3 == 0x35) {
                            /* dSv_event_flag_c::M_086 - Twilight Hyrule Field - Show Boss Bug's Tear of Light on the map */
                        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[119])) {
                            temp_r23_2 = true;
                        }
                    } else {
                        temp_r23_2 = true;
                    }
                } else {
                    temp_r23_2 = true;
                }
            }

            if (temp_r23_2) {
                JUT_ASSERT(0, i_data->mNo != 255);
                bool temp_r23_3 = false;
                if (i_data->mNo != 255 && !dComIfGs_isTbox(i_data->mNo)) {
                    temp_r23_3 = true;
                }

                if (temp_r23_3) {
                    if (disp_type == 3) {
                        if (i_data->mRoomNo == param_3 && param_1) {
                            var_r31 = true;
                        }
                    } else if (disp_type == 4) {
                        if (param_1) {
                            var_r31 = true;
                        }
                    } else if (disp_type == 5) {
                        if (i_data->mRoomNo == param_3) {
                            var_r31 = true;
                        }
                    } else {
                        var_r31 = true;
                    }
                }
            }
        }
        break;
    case 5:
        if (((i_data->mNo == 255 || (i_data->mNo != 255 && !dComIfGs_isTbox(i_data->mNo))) &&
             (i_data->mSwBit == 255 ||
              (i_data->mSwBit != 255 && dComIfGs_isSwitch(i_data->mSwBit, i_data->mRoomNo)))) &&
            param_1)
        {
            var_r31 = true;
        }
        break;
    case 3:
        if (stay_type == 1 && var_r23) {
            if ((i_data->mSwBit == 255 ||
                 (i_data->mSwBit != 255 && dComIfGs_isSwitch(i_data->mSwBit, i_data->mRoomNo))) &&
                !dComIfGs_isStageBossEnemy() && param_1)
            {
                var_r31 = true;
            }
        }
        break;
    case 9:
        if (stay_type == 1 && var_r23) {
            if ((i_data->mNo == 255 || (i_data->mNo != 255 && !dComIfGs_isTbox(i_data->mNo))) &&
                param_1)
            {
                var_r31 = true;
            }
        }
        break;
    case 10:
        if (stay_type == 0 && i_data->mNo != 0xFF && dComIfGs_isTbox(i_data->mNo) && param_1) {
            var_r31 = true;
        }
        break;
    case 11:
        if (stay_type == 1 && var_r23 && param_1) {
            var_r31 = true;
        }
        break;
    case 12:
        if (stay_type == 1 && var_r23) {
            if ((i_data->mNo == 255 || (i_data->mNo != 255 && !dComIfGs_isTbox(i_data->mNo))) &&
                param_1)
            {
                var_r31 = true;
            }
        }
        break;
    case 13:
    case 14:
        if (stay_type == 0) {
            if (i_data->mSwBit == 255 ||
                (i_data->mSwBit != 255 && dComIfGs_isSwitch(i_data->mSwBit, i_data->mRoomNo)))
            {
                var_r31 = true;
            }
        }
        break;
    case 15:
        if (stay_type == 1 && var_r23) {
            if ((i_data->mNo == 255 || (i_data->mNo != 255 && dComIfGs_isTbox(i_data->mNo))) &&
                param_1)
            {
                var_r31 = true;
            }
        }
        break;
    case 16:
        if (stay_type == 1 && !dComIfGs_isStageBossEnemy() && i_data->mNo != 255 &&
            !dComIfGs_isTbox(i_data->mNo) && var_r23 && param_1)
        {
            var_r31 = true;
        }
        break;
    default:
        JUT_ASSERT(0, FALSE);
        break;
    }

    return var_r31;
}

bool dMap_c::isDrawType(int param_0) {
    return (param_0 & 0x80) == 0;
}

bool dMap_c::isRendAllRoom() const {
    bool rt = renderingAmap_c::isRendAllRoom();
    if (field_0x8e == 2 && field_0x8f >= 3 && field_0x8f <= 3) {
        rt = true;
    }

    return rt;
}

bool dMap_c::isSpecialOutline() {
    return false;
}

void dMap_c::copyPalette() {
    JUT_ASSERT(0, m_res != NULL);

    cLib_memCpy(m_res, m_res_src, sizeof(dMap_HIO_prm_res_dst_s::m_res->palette_data));
    if (getStayType() == 0) {
        copyPalette(1, 0x2C, 1.0f);
    }
}

void dMap_c::setMapPaletteColorAlphaPer(int param_0, int param_1, f32 param_2) {
    int temp_r28 = (param_1 - param_0) + 1;
    int temp_r30 = param_0;

    for (int i = 0; i < temp_r28; i++) {
        setMapPaletteColorAlphaPer(temp_r30, param_2);
        temp_r30++;
    }
}

void dMap_c::resCopy() {
    cLib_memCpy(m_res, m_res_src, sizeof(dMap_prm_res_s));
    if (getStayType() == 0) {
        copyPalette(1, 0x2C, 1.0f);
    }
}

dMap_c::dMap_c(int param_0, int param_1, int param_2, int param_3) {
    m_res_src = NULL;
    m_res = NULL;
    mResTIMG = NULL;
    mImage_p = NULL;

    mCenterX = 0.0f;
    mCenterZ = 0.0f;
    field_0x58 = 1.0f;
    mPackX = 0.0f;
    mPackZ = 0.0f;
    field_0x64 = 0.0f;
    mPackPlusZ = 0.0f;
    mRightEdgePlus = 0.0f;
    mTopEdgePlus = 0.0f;

    field_0x74 = 0;
    mTexSizeX = 0;
    mTexSizeY = 0;
    mStayRoomNo = -1;
    field_0x80 = -1;
    field_0x84 = -1;
    field_0x88 = -1;

    field_0x8c = 0;
    field_0x8d = 0;
    field_0x8e = 0;
    field_0x8f = 0;
    field_0x90 = 0;

    m_res = new (0x20) dMap_prm_res_s;
    JUT_ASSERT(0, m_res != NULL);

    dMap_HIO_prm_res_dst_s::m_res = m_res;

    m_res_src = (dMap_prm_res_s*)dComIfG_getObjectRes("Always", 0x45);
    resCopy();

    mTexSizeX = param_0;
    mTexSizeY = param_1;

    if (dMap_HIO_prm_res_dst_s::m_res->field_0x1ae > 0) {
        field_0x74 = dMap_HIO_prm_res_dst_s::m_res->field_0x1b0 / 6;
    }

    int buffer_size = GXGetTexBufferSize(param_2, param_3, 9, GX_FALSE, 0);
    mImage_p = new (0x20) u8[buffer_size];
    JUT_ASSERT(0, mImage_p != NULL);

    renderingDAmap_c::init(mImage_p, mTexSizeX, mTexSizeY, mTexSizeX, mTexSizeY);

    mResTIMG = new (0x20) ResTIMG;
    JUT_ASSERT(0, mResTIMG != NULL);

    makeResTIMG(mResTIMG, mTexSizeX, mTexSizeY, mImage_p, (u8*)m_res, 0x33);
}

void dMap_c::_remove() {
    if (mImage_p != NULL) {
        delete[] mImage_p;
        mImage_p = NULL;
    }

    if (mResTIMG != NULL) {
        delete mResTIMG;
        mResTIMG = NULL;
    }

    if (m_res != NULL) {
        delete m_res;
        m_res = NULL;
    }
}

void dMap_c::getMapMinMaxXZ(int i_roomNo, f32* param_1, f32* param_2, f32* param_3, f32* param_4) {
    f32 var_f0 = 0.0f;
    f32 var_f1 = 0.0f;
    f32 var_f2 = 0.0f;
    f32 var_f3 = 0.0f;

    JUT_ASSERT(0, i_roomNo >= 0);

    if (i_roomNo >= 0) {
        f32 temp_f31 = dMpath_c::getMinX();
        f32 temp_f30 = dMpath_c::getMinZ();
        f32 temp_f29 = dMpath_c::getMaxX();
        f32 temp_f28 = dMpath_c::getMaxZ();

        f32 sp14 = 0.0f;
        f32 sp10 = 0.0f;
        f32 spC = 0.0f;
        f32 sp8 = 0.0f;

        dStage_FileList2_dt_c* fileList2_p = dStage_roomControl_c::getFileList2(i_roomNo);
        JUT_ASSERT(0, fileList2_p != NULL);

        if (fileList2_p != NULL) {
            dMapInfo_n::getRoomMinMaxXZ(i_roomNo, &sp14, &sp10, &spC, &sp8);
        }

        switch (getDispType()) {
        default:
            JUT_ASSERT(0, FALSE);
        case 4:
        case 0:
            var_f0 = temp_f31;
            var_f1 = temp_f30;
            var_f2 = temp_f29;
            var_f3 = temp_f28;
            break;
        case 1:
            var_f0 = sp14;
            var_f1 = sp10;
            var_f2 = spC;
            var_f3 = sp8;
            break;
        case 5:
        case 2:
        case 3:
            var_f0 = sp14;
            var_f1 = sp10;
            var_f2 = spC;
            var_f3 = sp8;
        }
    }

    if (param_1 != NULL) {
        *param_1 = var_f0;
    }

    if (param_2 != NULL) {
        *param_2 = var_f1;
    }

    if (param_3 != NULL) {
        *param_3 = var_f2;
    }

    if (param_4 != NULL) {
        *param_4 = var_f3;
    }
}

void dMap_c::getPack(int param_0, f32* param_1, f32* param_2) {
    f32 var_f31 = 0.0f;
    f32 var_f30 = 0.0f;

    if (getStayType() == 0) {
        f32 sp14 = 0.0f;
        f32 sp10 = 0.0f;
        f32 spC = 0.0f;
        f32 sp8 = 0.0f;
        getMapMinMaxXZ(param_0, &sp14, &sp10, &spC, &sp8);

        f32 temp_f1 = (field_0x58 * (f32)(field_0x74 + 4)) * 0.5f;
        sp14 -= temp_f1;
        sp10 -= temp_f1;
        spC += temp_f1;
        sp8 += temp_f1;

        f32 temp_f6 = spC - sp14;
        f32 temp_f7 = sp8 - sp10;
        f32 temp_f8 = (spC + sp14) * 0.5f;
        f32 temp_f3_2 = (sp8 + sp10) * 0.5f;

        if (temp_f6 > temp_f7) {
            var_f31 = (temp_f6 * 0.5f) - (temp_f8 - sp14);
            var_f30 = (temp_f6 * 0.5f) - (sp8 - temp_f3_2);
        } else {
            var_f31 = (temp_f7 * 0.5f) - (temp_f8 - sp14);
            var_f30 = (temp_f7 * 0.5f) - (sp8 - temp_f3_2);
        }
    }

    if (param_1 != NULL) {
        *param_1 = var_f31;
    }

    if (param_2 != NULL) {
        *param_2 = var_f30;
    }
}

void dMap_c::calcMapCenterXZ(int i_roomNo, f32* param_1, f32* param_2) {
    f32 var_f31 = 0.0f;
    f32 var_f30 = 0.0f;

    JUT_ASSERT(0, i_roomNo >= 0);

    if (i_roomNo >= 0) {
        f32 sp14 = 0.0f;
        f32 sp10 = 0.0f;
        f32 spC = 0.0f;
        f32 sp8 = 0.0f;
        getMapMinMaxXZ(i_roomNo, &sp14, &sp10, &spC, &sp8);

        var_f31 = (spC + sp14) * 0.5f;
        var_f30 = (sp8 + sp10) * 0.5f;
    }

    if (param_1 != NULL) {
        *param_1 = var_f31;
    }

    if (param_2 != NULL) {
        *param_2 = var_f30;
    }
}

void dMap_c::calcMapCmPerTexel(int i_roomNo, f32* ip_cmPerTexel) {
    JUT_ASSERT(0, i_roomNo >= 0);
    JUT_ASSERT(0, ip_cmPerTexel != NULL);

    f32 cmPerTexel = 0.0f;
    JUT_ASSERT(0, mTexSizeY != 0);

    if (i_roomNo >= 0) {
        if (getStayType() == 0) {
            f32 sp14 = 0.0f;
            f32 sp10 = 0.0f;
            f32 spC = 0.0f;
            f32 sp8 = 0.0f;
            getMapMinMaxXZ(i_roomNo, &sp14, &sp10, &spC, &sp8);

            f32 temp_f2 = spC - sp14;
            f32 temp_f0 = sp8 - sp10;
            f32 var_f3;
            if (temp_f2 >= temp_f0) {
                var_f3 = temp_f2;
            } else {
                var_f3 = temp_f0;
            }

            cmPerTexel = var_f3 / ((f32)mTexSizeY - (f32)(field_0x74 + 4));
        } else {
            f32 var_f31 = 0.0f;

            stage_stag_info_class* pstag = dComIfGp_getStage()->getStagInfo();
            JUT_ASSERT(0, pstag != NULL);

            if (pstag != NULL) {
                var_f31 = pstag->field_0x20;
            }

            if (var_f31 == 0.0f) {
                var_f31 = 10800.0f;
            }

            cmPerTexel = var_f31 / (f32)mTexSizeY;
        }
    }

    JUT_ASSERT(0, cmPerTexel != 0.0f);

    if (ip_cmPerTexel != NULL) {
        *ip_cmPerTexel = cmPerTexel;
    }
}

inline void getRGBA_RGB5A3(const dMpath_RGB5A3_palDt_s& palette, u8& r, u8& g, u8& b, u8& a) {
    u16 temp_r0;
    int t_r;
    int t_g;
    int t_b;
    int t_a;

    temp_r0 = palette.field_0x0.color;
    if (temp_r0 & 0x8000) {
        t_r = ((temp_r0 >> 7) & 0xF8) | (r >> 5);
        t_g = ((temp_r0 >> 2) & 0xF8) | (g >> 5);
        t_b = ((temp_r0 << 3) & 0xF8) | (b >> 5);
        t_a = 0xFF;
    } else {
        t_r = ((temp_r0 >> 4) & 0xF0) | (r >> 4);
        t_g = ((temp_r0 >> 0) & 0xF0) | (g >> 4);
        t_b = ((temp_r0 << 4) & 0xF0) | (b >> 4);
        t_a = ((temp_r0 >> 7) & 0xE0) | (a >> 3) | (a >> 6);
    }

    r = t_r;
    g = t_g;
    b = t_b;
    a = t_a;
}

void dMap_c::setMapPaletteColorAlphaPer(int i_paletteNo, f32 param_1) {
    JUT_ASSERT(0, i_paletteNo < renderingAmap_c::PALETTE_NUMBER);

    int pixel;
    u8 scaled_a = 0;
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
    dMpath_RGB5A3_palDt_s& temp_r31 = dMap_HIO_prm_res_dst_s::m_res->palette_data[i_paletteNo];
    getRGBA_RGB5A3(m_res_src->palette_data[i_paletteNo], r, g, b, a);

    scaled_a = a * param_1;
    if (scaled_a >= 0xE0) {
        pixel = ((r & 0xF8) << 7) | ((g & 0xF8) << 2) | ((b & 0xF8) >> 3) | 0x8000;
    } else {
        pixel = ((r & 0xF0) << 4) | (g & 0xF0) | ((b & 0xF0) >> 4) | ((scaled_a & 0xE0) << 7);
    }

    temp_r31.field_0x0.color = pixel;
    temp_r31.field_0x2.color = pixel;
    temp_r31.field_0x4.color = pixel;
    temp_r31.field_0x6.color = pixel;
    DCStoreRange(&dMap_HIO_prm_res_dst_s::m_res->palette_data[i_paletteNo],
                 sizeof(dMpath_RGB5A3_palDt_s));
}

void dMap_c::copyPalette(int i_dstPaletteNo, int i_srcPaletteNo, f32 param_2) {
    JUT_ASSERT(0, i_dstPaletteNo < renderingAmap_c::PALETTE_NUMBER);
    JUT_ASSERT(0, i_srcPaletteNo < renderingAmap_c::PALETTE_NUMBER);

    int pixel;
    u8 scaled_a = 0;
    u8 r = 0;
    u8 g = 0;
    u8 b = 0;
    u8 a = 0;
    dMpath_RGB5A3_palDt_s& temp_r31 = dMap_HIO_prm_res_dst_s::m_res->palette_data[i_dstPaletteNo];
    getRGBA_RGB5A3(m_res_src->palette_data[i_srcPaletteNo], r, g, b, a);

    scaled_a = a * param_2;
    if (scaled_a >= 0xE0) {
        pixel = ((r & 0xF8) << 7) | ((g & 0xF8) << 2) | ((b & 0xF8) >> 3) | 0x8000;
    } else {
        pixel = ((r & 0xF0) << 4) | (g & 0xF0) | ((b & 0xF0) >> 4) | ((scaled_a & 0xE0) << 7);
    }

    temp_r31.field_0x0.color = pixel;
    temp_r31.field_0x2.color = pixel;
    temp_r31.field_0x4.color = pixel;
    temp_r31.field_0x6.color = pixel;
    DCStoreRange(&dMap_HIO_prm_res_dst_s::m_res->palette_data[i_dstPaletteNo],
                 sizeof(dMpath_RGB5A3_palDt_s));
}

void dMap_c::setAmapPaletteColor(int i_paletteNo, u8 i_r, u8 i_g, u8 i_b, u8 i_scaledA) {
    JUT_ASSERT(0, i_paletteNo < renderingAmap_c::PALETTE_NUMBER);

    int pixel;
    if (i_scaledA >= 0xE0) {
        pixel = ((i_r & 0xF8) << 7) | ((i_g & 0xF8) << 2) | ((i_b & 0xF8) >> 3) | 0x8000;
    } else {
        pixel =
            ((i_r & 0xF0) << 4) | (i_g & 0xF0) | ((i_b & 0xF0) >> 4) | ((i_scaledA & 0xE0) << 7);
    }

    dMpath_RGB5A3_palDt_s& temp_r31 = dMap_HIO_prm_res_dst_s::m_res->palette_data[i_paletteNo];
    temp_r31.field_0x0.color = pixel;
    temp_r31.field_0x2.color = pixel;
    temp_r31.field_0x4.color = pixel;
    temp_r31.field_0x6.color = pixel;
    DCStoreRange(&dMap_HIO_prm_res_dst_s::m_res->palette_data[i_paletteNo],
                 sizeof(dMpath_RGB5A3_palDt_s));
}

int dMap_c::getDispType() const {
    return renderingAmap_c::getDispType();
}

bool dMap_c::isDrawRoom(int param_0, int param_1) const {
    if (field_0x8e == 2) {
        if (field_0x8f >= 3 && field_0x8f <= 3) {
            return param_0 == field_0x84 || param_0 == field_0x88;
        } else {
            return param_0 == field_0x80;
        }
    } else {
        return renderingDAmap_c::isDrawRoom(param_0, param_1);
    }
}

int dMap_c::getRoomNoSingle() {
    if (field_0x8e == 2 && field_0x8f <= 6) {
        return field_0x80;
    } else {
        return renderingDAmap_c::getRoomNoSingle();
    }
}

bool dMap_c::isDrawRoomIcon(int param_0, int param_1) const {
    bool var_r30 = hasMap() || param_0 == param_1;
    if (renderingAmap_c::isRendAllRoom()) {
        var_r30 = var_r30 || dMapInfo_n::isVisitedRoom(param_0);
    }

    return var_r30;
}

void dMap_c::_move(f32 i_centerX, f32 i_centerZ, int i_roomNo, f32 param_3) {
    if (mStayRoomNo == -1) {
        mStayRoomNo = i_roomNo;
        field_0x80 = mStayRoomNo;
        field_0x84 = mStayRoomNo;
        dMapInfo_c::setNextRoomNoForMapPat0(mStayRoomNo);

        field_0x8e = 0;
        field_0x8f = 0;
        field_0x90 = 0;
        mCenterX = i_centerX;
        mCenterZ = i_centerZ;

        calcMapCenterXZ(field_0x80, &mCenterX, &mCenterZ);
        calcMapCmPerTexel(field_0x80, &field_0x58);
        getPack(field_0x80, &mPackX, &mPackZ);

        mCenterX += mPackX;
        mCenterZ -= mPackZ;
        mCenterX += field_0x64;
        mCenterZ += mPackPlusZ;
    }

    if (getStayType() == 0 && dComIfGs_isSaveDunSwitch(0x32) &&
        strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0)
    {
        field_0x64 = 0.0f;
        mPackPlusZ = -3710.0f;

        f32 temp = (field_0x58 * (f32)(field_0x74 + 4)) * 0.5f;
        mRightEdgePlus = 0.0f;
        mTopEdgePlus = ((-22397.0f - dMpath_c::getMinZ()) - temp) / field_0x58;
    } else {
        mRightEdgePlus = 0.0f;
        mTopEdgePlus = 0.0f;
    }

    int temp_r29 = mStayRoomNo;
    mStayRoomNo = i_roomNo;

    dMapInfo_c::move(mStayRoomNo, param_3);
    field_0x8e = 0;

    switch (getDispType()) {
    default:
    case 0:
        JUT_ASSERT(0, FALSE);
        break;
    case 5:
        field_0x8e = 2;
        break;
    case 1:
        field_0x8e = 1;
        break;
    }

    switch (field_0x8e) {
    case 0:
        if (mStayRoomNo != temp_r29) {
            field_0x80 = mStayRoomNo;
            field_0x84 = mStayRoomNo;
            field_0x88 = mStayRoomNo;

            calcMapCenterXZ(field_0x80, &mCenterX, &mCenterZ);
            calcMapCmPerTexel(field_0x80, &field_0x58);
            getPack(field_0x80, &mPackX, &mPackZ);

            mCenterX += mPackX;
            mCenterZ -= mPackZ;
        }
        break;
    case 1:
        switch (field_0x8f) {
        case 0:
            if (field_0x80 != dMapInfo_c::getNextRoomNoForMapPat0()) {
                field_0x8f = 1;
                field_0x90 = 60;
                field_0x84 = (s8)dMapInfo_c::getNextRoomNoForMapPat0();
                field_0x88 = field_0x80;
            }
            break;
        case 1:
            field_0x90--;
            if (field_0x90 == 0) {
                field_0x8f = 0;
                field_0x80 = field_0x84;
                calcMapCenterXZ(field_0x80, &mCenterX, &mCenterZ);
            }
            break;
        }
        break;
    case 2:
        switch (field_0x8f) {
        case 0:
            if (field_0x80 != dMapInfo_c::getNextRoomNoForMapPat0()) {
                field_0x84 = (s8)dMapInfo_c::getNextRoomNoForMapPat0();
                field_0x88 = field_0x80;
                field_0x90 = 30;
                field_0x8f = 1;
                field_0x90 = 30;
            }
            break;
        case 1:
            if (field_0x90 != 0) {
                field_0x90--;
            }

            if (field_0x90 == 0 && mStayRoomNo != field_0x88) {
                field_0x8f = 2;
                field_0x90 = 5;
            }
            break;
        case 2:
            field_0x90--;
            if (field_0x90 == 0) {
                field_0x8f = 3;
                field_0x90 = 20;
            }
            break;
        case 3:
            field_0x90--;
            if (field_0x90 == 0) {
                field_0x80 = field_0x84;

                calcMapCenterXZ(field_0x80, &mCenterX, &mCenterZ);
                calcMapCmPerTexel(field_0x80, &field_0x58);
                getPack(field_0x80, &mPackX, &mPackZ);

                mCenterX += mPackX;
                mCenterZ -= mPackZ;
                field_0x8f = 4;
                field_0x90 = 5;
            }
            break;
        case 4:
            field_0x90--;
            if (field_0x90 == 0) {
                field_0x8f = 5;
            }
            break;
        case 5:
            if (field_0x84 == mStayRoomNo) {
                field_0x8f = 6;
            }
            break;
        case 6:
            field_0x8f = 0;
            break;
        }
        break;
    }

    switch (field_0x8e) {
    case 0:
        break;
    case 1:
        switch (field_0x8f) {
        case 0:
            break;
        case 1:
            calcMapCenterXZ(field_0x88, &mCenterX, &mCenterZ);
            f32 sp2C, sp28;
            calcMapCenterXZ(field_0x84, &sp2C, &sp28);

            f32 temp_f2 = 1.0f - ((f32)field_0x90 / 60.0f);
            mCenterX += temp_f2 * (sp2C - mCenterX);
            mCenterZ += temp_f2 * (sp28 - mCenterZ);
            break;
        }
        break;
    case 2:
        switch (field_0x8f) {
        case 2: {
            f32 temp_f31 = (f32)field_0x90 / 5.0f;
            setMapPaletteColorAlphaPer(0x2F, 0x32, temp_f31);
            setMapPaletteColorAlphaPer(0x21, 0x2B, temp_f31);
            break;
        }
        case 3: {
            calcMapCenterXZ(field_0x88, &mCenterX, &mCenterZ);
            f32 sp24, sp20;
            calcMapCenterXZ(field_0x84, &sp24, &sp20);

            f32 temp_f31_2 = 1.0f - ((f32)field_0x90 / 20.0f);
            mCenterX += temp_f31_2 * (sp24 - mCenterX);
            mCenterZ += temp_f31_2 * (sp20 - mCenterZ);

            if (mStayRoomNo == field_0x88) {
                setMapPaletteColorAlphaPer(0xA, 0x11, temp_f31_2);
                setMapPaletteColorAlphaPer(0x12, 0x19, 1.0f - temp_f31_2);
            } else {
                setMapPaletteColorAlphaPer(0xA, 0x11, 1.0f - temp_f31_2);
                setMapPaletteColorAlphaPer(0x12, 0x19, temp_f31_2);
            }

            f32 sp1C, sp18;
            calcMapCmPerTexel(field_0x88, &sp1C);
            calcMapCmPerTexel(field_0x84, &sp18);
            field_0x58 = sp1C + (temp_f31_2 * (sp18 - sp1C));

            f32 sp14, sp10, spC, sp8;
            getPack(field_0x88, &sp14, &sp10);
            getPack(field_0x84, &spC, &sp8);

            sp14 += temp_f31_2 * (spC - sp14);
            sp10 += temp_f31_2 * (sp8 - sp10);
            mCenterX += sp14;
            mCenterZ -= sp10;
            break;
        }
        case 4: {
            f32 temp_f31_3 = 1.0f - ((f32)field_0x90 / 5.0f);
            setMapPaletteColorAlphaPer(0x2F, 0x32, temp_f31_3);
            setMapPaletteColorAlphaPer(0x21, 0x2B, temp_f31_3);
            break;
        }
        case 6:
            copyPalette();
            break;
        }
        break;
    }

    field_0x8d = 1;
}

void dMap_c::_draw() {
    if (mStayRoomNo >= 0 && field_0x8d != 0) {
        s8 floor_no = dMapInfo_c::getNowStayFloorNo();
        entry(mCenterX, mCenterZ, field_0x58, mStayRoomNo, floor_no);
    }
}

dTres_c::typeGroupData_c* dMap_c::getFirstData(u8 param_0) {
    return renderingPlusDoorAndCursor_c::getFirstData(param_0);
}

dTres_c::typeGroupData_c* dMap_c::getNextData(dTres_c::typeGroupData_c* param_0) {
    return renderingPlusDoorAndCursor_c::getNextData(param_0);
}
