/**
 * d_map_path.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_com_inf_game.h"
#include "d/d_map_path.h"
#include "m_Do/m_Do_lib.h"

void dMpath_n::dTexObjAggregate_c::create() {
    static int const data[7] = {
        79, 80, 77, 78, 76, 81, 82,
    };

    for (int lp1 = 0; lp1 < 7; lp1++) {
        mp_texObj[lp1] = new GXTexObj();
        JUT_ASSERT(70, mp_texObj[lp1] != NULL);
        ResTIMG* image = (ResTIMG*)dComIfG_getObjectRes("Always", data[lp1]);
        JUT_ASSERT(72, image != NULL);
        JUT_ASSERT(73, image->minFilter == GX_NEAR);
        JUT_ASSERT(74, image->magFilter == GX_NEAR);
        mDoLib_setResTimgObj(image, mp_texObj[lp1], 0, NULL);
    }
}

void dMpath_n::dTexObjAggregate_c::remove() {
    for (int i = 0; i < 7; i++) {
        delete mp_texObj[i];
        mp_texObj[i] = NULL;
    }
}

#if DEBUG
void dMpath_HIO_n::hioList_c::gen(JORMContext* mctx) {
    static const char* number[] = {
        "00", "01", "02", "03", "04", "05", "06", "07",
        "08", "09", "10", "11", "12", "13", "14", "15",
        "16", "17", "18", "19", "20", "21", "22", "23",
        "24", "25", "26", "27", "28", "29", "30", "31",
        "32", "33", "34", "35", "36", "37", "38", "39",
        "40", "41", "42", "43", "44", "45", "46", "47",
        "48", "49", "50", "51", "52", "53", "54", "55",
        "56", "57", "58", "59", "60", "61", "62", "63",
    };
    // DEBUG NONMATCHING
}

void dMpath_HIO_n::hioList_c::update(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

u32 dMpath_HIO_n::hioList_c::addString(char* param_1, u32 param_2, u32 param_3) const {
    // DEBUG NONMATCHING
}

u32 dMpath_HIO_n::hioList_c::addStringBinary(char* param_1, u32 param_2, u32 param_3) const {
    // DEBUG NONMATCHING
}

BOOL dMpath_HIO_file_base_c::writeHostioTextFile(const char* param_1) {
    JORFile file;
    BOOL result = 0;
    const char* r27 = "すべてのファイル(*.*)\0*.*\0";
    if (param_1) {
        r27 = param_1;
    }
    if (file.open(JORFile::EFlags_WRITE | JORFile::EFlags_UNK_0x4, r27, NULL, NULL, NULL)) {
        const u32 bufSize = 4000;
        u32 size = 0;
        char buffer[bufSize];
        memset(buffer, 0, bufSize);
        size = addString(buffer, size, bufSize);
        JUT_ASSERT(732, size < bufSize);
        file.writeData(buffer, s16(size));
        file.close();
        OSReport("write append success!::%6d\n", size);
        result = 1;
    } else {
        OSReport("write append failure!\n");
        result = 0;
    }
    return result;
}

BOOL dMpath_HIO_file_base_c::writeBinaryTextFile(const char* param_1) {
    JORFile file;
    BOOL result = 0;
    const char* r27 = "すべてのファイル(*.*)\0*.*\0";
    if (param_1) {
        r27 = param_1;
    }
    if (file.open(JORFile::EFlags_WRITE | JORFile::EFlags_UNK_0x4, r27, NULL, NULL, NULL)) {
        const u32 bufSize = 10000;
        u32 size = 0;
        char buffer[bufSize];
        memset(buffer, 0, bufSize);
        size = addStringBinary(buffer, size, bufSize);
        JUT_ASSERT(762, size < bufSize);
        file.writeData(buffer, s16(size));
        file.close();
        OSReport("write append success!::%6d\n", size);
        result = 1;
    } else {
        OSReport("write append failure!\n");
        result = 0;
    }
    return result;
}

BOOL dMpath_HIO_file_base_c::writeBinaryFile(const char* param_1) {
    JORFile file;
    BOOL result = 0;
    const char* r27 = "すべてのファイル(*.*)\0*.*\0";
    if (param_1) {
        r27 = param_1;
    }
    if (file.open(JORFile::EFlags_WRITE | JORFile::EFlags_UNK_0x4, r27, NULL, NULL, NULL)) {
        const u32 bufSize = 2000;
        u32 size = 0;
        char buffer[bufSize];
        memset(buffer, 0, bufSize);
        size = addData(buffer, size, bufSize);
        JUT_ASSERT(794, size < bufSize);
        file.writeData(buffer, s16(size));
        file.close();
        OSReport("write append success!::%6d\n", size);
        result = 1;
    } else {
        OSReport("write append failure!\n");
        result = 0;
    }
    return result;
}

void dMpath_HIO_file_base_c::binaryDump(const void* param_1, u32 param_2) {
    int r26 = 0;
    u8* r30 = (u8*)param_1;
    u8* r25 = r30;
    int r28;
    for (int i = 0; i < param_2; i++, r30++) {
        r28 = i % 8;
        if (r28 == 0) {
            OSReport("%04x : ",i);
        }
        OSReport("%02x", u8(*r30));
        if (r28 == 3) {
            OSReport(" - ");
        } else if (r28 == 7) {
            OSReport("\n");
        } else {
            OSReport(" ");
        }
    }
    if (r28 != 7) {
        OSReport("\n");
    }
    OSReport("startAdr<%08x>dataSize<%d><0x%04x>\n", param_1, param_2, param_2);
}

bool dMpath_HIO_file_base_c::readBinaryFile(const char* param_1)  {
    JORFile file;
    bool result = false;
    const char* r26 = "すべてのファイル(*.*)\0*.*\0";
    if (param_1) {
        r26 = param_1;
    }
    if (file.open(JORFile::EFlags_READ, r26, NULL, NULL, NULL)) {
        s32 r28 = file.getFileSize();
        char* buf = new char[r28];
        JUT_ASSERT(855, buf != NULL);
        file.readData(buf, r28);
        copyReadBufToData(buf, r28);
        OSReport("write read success!::%6d\n", r28);
        result = true;
        delete[] buf;
        buf = NULL;
        file.close();
    } else {
        OSReport("write append failure!\n");
        result = false;
    }
    return result;
}
#endif

void dDrawPath_c::rendering(dDrawPath_c::line_class const* p_line) {
    if (isDrawType(p_line->field_0x0)) {
        int width = getLineWidth(p_line->field_0x1);

        if (width > 0 && p_line->mDataNum >= 2) {
            GXSetLineWidth(width, GX_TO_ZERO);
            GXSetTevColor(GX_TEVREG0, *getLineColor(p_line->field_0x0 & 0x3F, p_line->field_0x1));
            GXBegin(GX_LINESTRIP, GX_VTXFMT0, p_line->mDataNum);

            u16* tmp = p_line->mpData;
            for (int i = 0; i < p_line->mDataNum; i++) {
                GXPosition1x16(*tmp);
                tmp++;
            }
            GXEnd();
        }
    }
}

void dDrawPath_c::rendering(dDrawPath_c::poly_class const* p_poly) {
    if (isDrawType(p_poly->field_0x0)) {
        GXSetTevColor(GX_TEVREG0, *getColor(p_poly->field_0x0 & 0x3F));

        if (p_poly->mDataNum >= 3) {
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, p_poly->mDataNum);

            u16* tmp = p_poly->mpData;
            for (int i = 0; i < p_poly->mDataNum; i++) {
                GXPosition1x16(*tmp);
                tmp++;
            }
            GXEnd();
        }
    }
}

void dDrawPath_c::rendering(dDrawPath_c::group_class const* p_group) {
    if (isSwitch(p_group)) {
        poly_class* poly = p_group->mpPoly;
        for (int i = 0; i < p_group->mPolyNum; i++) {
            rendering(poly);
            poly++;
        }

        line_class* line = p_group->mpLine;
        for (int i = 0; i < p_group->mLineNum; i++) {
            rendering(line);
            line++;
        }
    }
}

void dDrawPath_c::rendering(dDrawPath_c::floor_class const* p_floor) {
    if (p_floor->mpGroup != NULL) {
        group_class* group = p_floor->mpGroup;

        for (int i = 0; i < p_floor->mGroupNum; i++) {
            rendering(group);
            group++;
        }
    }
}

void dDrawPath_c::rendering(dDrawPath_c::room_class const* room) {
    JUT_ASSERT(1043, room != NULL);
    if (room != NULL) {
        GXSetArray(GX_VA_POS, room->mpFloatData, 8);
        floor_class* floor = room->mpFloor;

        if (floor != NULL) {
            for (int i = 0; i < room->mFloorNum; i++) {
                if (isRenderingFloor(floor->mFloorNo)) {
                    rendering(floor);
                }
                floor++;
            }
        }
    }
}

void dDrawPath_c::drawPath() {
    room_class* room = getFirstRoomPointer();
    while (room != NULL) {
        rendering(room);
        room = getNextRoomPointer();
    }
}

void dRenderingMap_c::makeResTIMG(ResTIMG* p_image, u16 width, u16 height, u8* p_data,
                                  u8* p_palette, u16 param_5) const {
    p_image->format = GX_TF_C8;
    p_image->alphaEnabled = 2;
    p_image->width = width;
    p_image->height = height;
    p_image->wrapS = GX_CLAMP;
    p_image->wrapT = GX_CLAMP;
    p_image->indexTexture = true;
    p_image->colorFormat = 2;
    p_image->numColors = param_5 * 4;
    p_image->paletteOffset = p_palette - (u8*)p_image;
    p_image->mipmapEnabled = false;
    p_image->doEdgeLOD = false;
    p_image->biasClamp = false;
    p_image->maxAnisotropy = 0;
    p_image->minFilter = GX_LINEAR;
    p_image->magFilter = GX_LINEAR;
    p_image->minLOD = 0;
    p_image->maxLOD = 0;
    p_image->mipmapCount = 1;
    p_image->LODBias = 0;
    p_image->imageOffset = p_data - (u8*)p_image;
}

void dRenderingMap_c::renderingMap() {
    preRenderingMap();
    if (isDrawPath()) {
        preDrawPath();
        beforeDrawPath();
        drawPath();
        afterDrawPath();
        postDrawPath();
    }
    postRenderingMap();
}

void dRenderingFDAmap_c::setTevSettingNonTextureDirectColor() const {
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
}

void dRenderingFDAmap_c::setTevSettingIntensityTextureToCI() const {
    GXSetNumTevStages(2);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC, GX_CC_C1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_C2, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                    GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_1_4);
}

void dRenderingFDAmap_c::drawBack() const {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetTevColor(GX_TEVREG0, *getBackColor());
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-field_0x8, -field_0xc, 0);
    GXPosition3f32(field_0x8, -field_0xc, 0);
    GXPosition3f32(field_0x8, field_0xc, 0);
    GXPosition3f32(-field_0x8, field_0xc, 0);
    GXEnd();
}

void dRenderingFDAmap_c::preRenderingMap() {
    GXSetViewport(0.0f, 0.0f, mTexWidth, mTexHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, mTexWidth, mTexHeight);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_FALSE);
    GXSetNumIndStages(0);
    GXSetClipMode(GX_CLIP_ENABLE);
    setTevSettingNonTextureDirectColor();
    f32 right = field_0x8 * 0.5f;
    f32 top = field_0xc * 0.5f;
    Mtx44 matrix;
    C_MTXOrtho(matrix, top, -top, -right, right, 0.0f, 10000.0f);
    GXSetProjection(matrix, GX_ORTHOGRAPHIC);
    GXLoadPosMtxImm(g_mDoMtx_identity, GX_PNMTX0);
    GXSetCurrentMtx(0);
    drawBack();
}

void dRenderingFDAmap_c::postRenderingMap() {
    GXSetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
    GXSetTexCopySrc(0, 0, mTexWidth, mTexHeight);
    GXSetTexCopyDst(mTexWidth, mTexHeight, GX_CTF_R8, GX_FALSE);
    GXCopyTex(field_0x4, GX_TRUE);
    GXPixModeSync();
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetDither(GX_TRUE);
    dComIfGp_getCurrentGrafPort()->setup2D();
}

dMpath_n::dTexObjAggregate_c dMpath_n::m_texObjAgg;

/* Enabling the following definition will modify the following function to
 * make the map look worse for extra speed in the emulator, especially in large
 * areas such as hyrule field.
 */
// #define HYRULE_FIELD_SPEEDHACK

void dRenderingFDAmap_c::renderingDecoration(dDrawPath_c::line_class const* p_line) {
    s32 width = getDecorationLineWidth(p_line->field_0x1);
    if (width <= 0) {
        return;
    }

    setTevSettingIntensityTextureToCI();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetNumTevStages(1);
    GXLoadTexObj(dMpath_n::m_texObjAgg.mp_texObj[6], GX_TEXMAP0);

    u16* data_p = p_line->mpData;
    s32 data_num = p_line->mDataNum;
    GXSetLineWidth(width, GX_TO_ONE);
    GXSetPointSize(width, GX_TO_ONE);
    GXColor lineColor = *getDecoLineColor(p_line->field_0x0 & 0x3f, p_line->field_0x1);
    GXSetTevColor(GX_TEVREG0, lineColor);
    lineColor.r = lineColor.r - 4;
    GXSetTevColor(GX_TEVREG1, lineColor);

    for (int i = 0; i < data_num; data_p++, i++) {
#ifndef HYRULE_FIELD_SPEEDHACK
        if (i < data_num - 1) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
            GXPosition1x16(data_p[0]);
            GXTexCoord2f32(0, 0);
            GXPosition1x16(data_p[1]);
            GXTexCoord2f32(0, 0);
        }
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_KONST, GX_CC_TEXC, GX_CC_C1);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
#endif
        GXBegin(GX_POINTS, GX_VTXFMT0, 1);
        GXPosition1x16(data_p[0]);
        GXTexCoord2f32(0, 0);
        GXEnd();
    }

    setTevSettingNonTextureDirectColor();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_F32, 0);
}

const GXColor* dRenderingFDAmap_c::getDecoLineColor(int param_0, int param_1) {
    return getLineColor(param_0, param_1);
}

s32 dRenderingFDAmap_c::getDecorationLineWidth(int param_0) {
    return getLineWidth(param_0);
}
