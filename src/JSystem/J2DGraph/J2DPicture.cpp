#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JUtility/JUTResource.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include "dolphin/gx.h"

J2DPicture::J2DPicture() : mPalette(NULL) {
    for (int i = 0; i < 2; i++) {
        mTexture[i] = NULL;
    }

    field_0x109 = 0;
    mTextureNum = 0;
    initinfo();
}


J2DPicture::J2DPicture(J2DPane* p_pane, JSURandomInputStream* p_stream, JKRArchive* p_archive)
    : mPalette(NULL) {
    private_readStream(p_pane, p_stream, p_archive);
}

J2DPicture::J2DPicture(J2DPane* p_pane, JSURandomInputStream* p_stream, J2DMaterial* p_material)
    : mPalette(NULL) {
    int position = p_stream->getPosition();

    J2DPicHeader header;
    p_stream->read(&header, sizeof(header));
    mKind = 'PIC1';

    int position2 = p_stream->getPosition();

    J2DPaneHeader panHeader;
    p_stream->peek(&panHeader, sizeof(panHeader));
    makePaneExStream(p_pane, p_stream);
    p_stream->seek(position2 + panHeader.mSize, JSUStreamSeekFrom_SET);

    J2DScrnBlockPictureParameter picInfo;
    p_stream->read(&picInfo, sizeof(picInfo));
    u16 matNum = picInfo.field_0x4;

    for (int i = 0; i < 4; i++) {
        field_0x10a[i] = picInfo.field_0x10[i];
        mCornerColor[i] = picInfo.mCornerColor[i];
    }

    p_stream->seek(position + header.mSize, JSUStreamSeekFrom_SET);

    J2DMaterial* material = NULL;
    if (matNum != 0xFFFF) {
        material = &p_material[matNum];
    }

    mAlpha = 255;
    if (material != NULL) {
        mAlpha = material->getColorBlock()->getMatColor(0)->a;
    }

    mBlack = JUtility::TColor(0);
    mWhite = JUtility::TColor(0xFFFFFFFF);
    mTextureNum = 0;

    if (material != NULL && material->getTevBlock() != NULL) {
        u8 texgenNum = material->getTexGenBlock()->getTexGenNum();
        u32 stageNum = material->getTevBlock()->getTevStageNum();

        if ((texgenNum == 1 && stageNum != 1) || (texgenNum != 1 && (int)stageNum != texgenNum + 1))
        {
            J2DGXColorS10* color0p = material->getTevBlock()->getTevColor(0);
            GXColorS10 color0;
            color0.r = color0p->r;
            color0.g = color0p->g;
            color0.b = color0p->b;
            color0.a = color0p->a;

            J2DGXColorS10* color1p = material->getTevBlock()->getTevColor(1);
            GXColorS10 color1;
            color1.r = color1p->r;
            color1.g = color1p->g;
            color1.b = color1p->b;
            color1.a = color1p->a;

            mBlack = JUtility::TColor(((u8)color0.r << 0x18) | ((u8)color0.g << 0x10) |
                                      ((u8)color0.b << 8) | (u8)color0.a);
            mWhite = JUtility::TColor(((u8)color1.r << 0x18) | ((u8)color1.g << 0x10) |
                                      ((u8)color1.b << 8) | (u8)color1.a);
        }

        mTextureNum = texgenNum <= 2 ? texgenNum : 2;
    }

    field_0x109 = 0;

    for (u32 i = 0; i < 2; i++) {
        mTexture[i] = NULL;

        if (material != NULL && material->getTevBlock() != NULL) {
            JUTTexture* tex = material->getTevBlock()->getTexture(i);

            if (tex != NULL) {
                mTexture[i] = tex;
                field_0x109 |= (1 << i);
            }
        }
    }

    if (material != NULL && material->getTevBlock() != NULL) {
        material->getTevBlock()->setUndeleteFlag(0xF0);
    }

    mPalette = NULL;

    JUtility::TColor color(0xFFFFFFFF);
    JUtility::TColor alpha(0xFFFFFFFF);
    if (material != NULL && material->getTevBlock() != NULL) {
        color = *material->getTevBlock()->getTevKColor(3);
        alpha = *material->getTevBlock()->getTevKColor(1);
    }

    setBlendColorRatio(color.a, color.b);
    setBlendAlphaRatio(alpha.a, alpha.b);
}

J2DPicture::J2DPicture(ResTIMG const* p_timg) {
    for (int i = 0; i < 2; i++) {
        mTexture[i] = NULL;
    }

    field_0x109 = 0;
    mTextureNum = 0;

    if (p_timg != NULL) {
        append(p_timg, 1.0f);
    }
    mPalette = NULL;
    initinfo();
}


J2DPicture::J2DPicture(u64 tag, JGeometry::TBox2<f32> const& bounds, ResTIMG const* p_timg,
                       ResTLUT const* p_tlut)
    : J2DPane(tag, bounds), mPalette(NULL) {
    for (int i = 0; i < 2; i++) {
        mTexture[i] = NULL;
    }

    field_0x109 = 0;
    mTextureNum = 0;

    private_initiate(p_timg, p_tlut);
    initinfo();
}

void J2DPicture::private_readStream(J2DPane* parent, JSURandomInputStream* stream,
                                    JKRArchive* archive) {
    J2DScrnBlockHeader header;
    int headerPosition = stream->getPosition();

    stream->read(&header, sizeof(J2DScrnBlockHeader));
    mKind = header.mTag;
    makePaneStream(parent, stream);

    JUTResReference ref;

    ResTIMG* img;
    ResTLUT* lut;
    u32 var_r27 = 0;
    u8 var_r26 = stream->readU8();

    img = (ResTIMG*)getPointer(stream, 'TIMG', archive);
    lut = (ResTLUT*)getPointer(stream, 'TLUT', archive);

    u8 spA = stream->read8b();

    var_r26 -= 3;
    if (var_r26 != 0) {
        var_r27 = stream->read8b();
        var_r26--;
    }

    if (var_r26 != 0) {
        stream->read8b();
        var_r26--;
    }

    mBlack = 0;
    mWhite = 0xFFFFFFFF;

    if (var_r26 != 0) {
        mBlack = stream->readU32();
        var_r26--;
    }

    if (var_r26 != 0) {
        mWhite = stream->readU32();
        var_r26--;
    }

    setCornerColor(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    for (int i = 0; var_r26 != 0 && i < 4; i++) {
        mCornerColor[i] = stream->readU32();
        var_r26--;
    }

    stream->seek(headerPosition + header.mSize, JSUStreamSeekFrom_SET);

    for (int i = 0; i < 2; i++) {
        mTexture[i] = NULL;
    }

    mTextureNum = 0;
    field_0x109 = 1;

    if (img != NULL) {
        mTexture[0] = new JUTTexture(img, 0);
        mTextureNum++;
    }

    if (lut != NULL) {
        mPalette = new JUTPalette(GX_TLUT0, lut);
        mTexture[0]->attachPalette(mPalette);
    }

    setTexCoord(mTexture[0], (J2DBinding)spA, (J2DMirror)(var_r27 & 3), (bool)((var_r27 >> 2) & 1));
    setBlendRatio(1.0f, 1.0f);
}

void J2DPicture::initiate(const ResTIMG* img, const ResTLUT* lut) {
    private_initiate(img, lut);
    if (mTexture[0] == NULL) {
        return;
    }

    place(JGeometry::TBox2<f32>(0.0f, 0.0f, mTexture[0]->getWidth(), mTexture[0]->getHeight()));
}

void J2DPicture::private_initiate(const ResTIMG* timg, const ResTLUT* tlut) {
    if (mTextureNum != 0) {
        return;
    }

    if (timg != NULL) {
        if (mTexture[0] == NULL) {
            mTexture[0] = new JUTTexture(timg, 0);
            if (mTexture[0] != NULL) {
                mTextureNum++;
                field_0x109 = (field_0x109 & 2) | 1;
            }
        } else {
            mTexture[0]->storeTIMG(timg, (u8)0);
            mTextureNum++;
            field_0x109 = (field_0x109 & 2) | 1;
        }
    }

    mPalette = NULL;
    if (tlut && mPalette == NULL) {
        mPalette = new JUTPalette(GX_TLUT0, const_cast<ResTLUT*>(tlut));
        if (mTexture[0] != NULL) {
            mTexture[0]->attachPalette(mPalette);
        }
    }
}

void J2DPicture::initinfo() {
    mKind = 'PIC1';
    setTexCoord(NULL, BIND15, MIRROR0, false);
    setBlendRatio(1.0f, 1.0f);

    mBlack = 0;
    mWhite = -1;
    setCornerColor(-1, -1, -1, -1);
}


J2DPicture::~J2DPicture() {
    for (int i = 0; i < 2; i++) {
        if ((int)(field_0x109 & 1 << i) != 0) {
            delete mTexture[i];
        }
    }

    delete mPalette;
}

bool J2DPicture::prepareTexture(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i >= 2) {
            return 0;
        }

        if (mTexture[i] == NULL) {
            JUTTexture* tmp = new JUTTexture();
            mTexture[i] = tmp;

            if (mTexture[i] == NULL) {
                return 0;
            }
            field_0x109 |= (1 << i);
        }
    }
    return 1;
}

bool J2DPicture::insert(ResTIMG const* img, JUTPalette* palette, u8 param_2, f32 param_3) {
    if (img == NULL || mTextureNum >= 2 || param_2 >= 2 || param_2 > mTextureNum) {
        return false;
    }

    u8 var_r26 = 0;
    if (img->indexTexture != 0 && palette == NULL) {
        var_r26 = getUsableTlut(mTextureNum);
    }

    JUTTexture* var_r31;
    if (mTexture[mTextureNum] == NULL) {
        var_r31 = new JUTTexture(img, var_r26);

        if (palette != NULL) {
            var_r31->storeTIMG(img, palette);
        }

        for (u8 i = 1; i > param_2; i--) {
            mTexture[i] = mTexture[i - 1];
            field_0x11c[i] = field_0x11c[i - 1];
            field_0x124[i] = field_0x124[i - 1];
        }

        field_0x109 =
            (field_0x109 & ((1 << param_2) - 1)) | ((field_0x109 & ~((1 << param_2) - 1)) * 2);
        field_0x109 |= (1 << param_2);
    } else {
        var_r31 = mTexture[mTextureNum];
        if (palette == NULL) {
            mTexture[mTextureNum]->storeTIMG(img, var_r26);
        } else {
            mTexture[mTextureNum]->storeTIMG(img, palette);
        }

        u8 sp8[2];
        for (u8 i = 0; i < 2; i++) {
            sp8[i] = (field_0x109 & (1 << i)) != 0;
        }

        for (u8 i = mTextureNum; i > param_2; i--) {
            mTexture[i] = mTexture[i - 1];
            sp8[i] = sp8[i - 1];
            field_0x11c[i] = field_0x11c[i - 1];
            field_0x124[i] = field_0x124[i - 1];
        }

        field_0x109 = 0;

        for (u8 i = 0; i < 2; i++) {
            if (sp8[i] != 0) {
                field_0x109 |= (1 << i);
            }
        }

        field_0x109 |= (1 << param_2);
    }

    mTexture[param_2] = var_r31;
    field_0x11c[param_2] = param_3;
    field_0x124[param_2] = param_3;

    if (mTextureNum == 0 && mTexture[0] != NULL) {
        place(JGeometry::TBox2<f32>(0.0f, 0.0f, mTexture[0]->getWidth(), mTexture[0]->getHeight()));
        setTexCoord(NULL, BIND15, MIRROR0, false);
    }

    mTextureNum++;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}


bool J2DPicture::insert(char const* resName, JUTPalette* palette, u8 param_2, f32 param_3) {
    void* resource = J2DScreen::getNameResource(resName);
    return insert((ResTIMG*)resource, palette, param_2, param_3);
}

bool J2DPicture::insert(JUTTexture* texture, u8 param_1, f32 param_2) {
    if (texture == NULL || mTextureNum >= 2 || param_1 >= 2 || param_1 > mTextureNum) {
        return false;
    }

    if (mTexture[1] != NULL && field_0x109 & 2) {
        delete mTexture[1];
        field_0x109 &= 1;
    }

    for (u8 i = 1; i > param_1; i--) {
        mTexture[i] = mTexture[i - 1];
        field_0x11c[i] = field_0x11c[i - 1];
        field_0x124[i] = field_0x124[i - 1];
    }

    field_0x109 =
        (field_0x109 & ((1 << param_1) - 1)) | ((field_0x109 & ~((1 << param_1) - 1)) * 2);
    mTexture[param_1] = texture;
    field_0x109 &= ~(1 << param_1);

    field_0x11c[param_1] = param_2;
    field_0x124[param_1] = param_2;

    if (mTextureNum == 0 && &mTexture[0] != NULL) {
        place(JGeometry::TBox2<f32>(0.0f, 0.0f, mTexture[0]->getWidth(), mTexture[0]->getHeight()));
        setTexCoord(NULL, BIND15, MIRROR0, false);
    }

    mTextureNum++;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}


bool J2DPicture::remove(u8 param_0) {
    if (mTextureNum <= param_0 || mTextureNum == 1) {
        return false;
    }

    if (field_0x109 & (1 << param_0)) {
        delete mTexture[param_0];
    }

    for (u8 i = param_0; i < mTextureNum - 1; i++) {
        mTexture[i] = mTexture[i + 1];
        field_0x11c[i] = field_0x11c[i + 1];
        field_0x124[i] = field_0x124[i + 1];
    }

    mTexture[mTextureNum - 1] = NULL;
    field_0x109 =
        (field_0x109 & ((1 << param_0) - 1)) | ((field_0x109 & ~((1 << (param_0 + 1)) - 1)) >> 1);

    mTextureNum--;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}

bool J2DPicture::remove(JUTTexture* param_0) {
    u8 i = 0;
    for (; i < mTextureNum; i++) {
        if (mTexture[i] == param_0) {
            break;
        }
    }
    return remove(i);
}

const ResTIMG* J2DPicture::changeTexture(ResTIMG const* img, u8 texIndex) {
    if (texIndex > mTextureNum || 2 <= texIndex || img == NULL) {
        return NULL;
    }

    const ResTIMG* result;
    if (texIndex < mTextureNum) {
        JUTTexture* tex = getTexture(texIndex);
        result = tex->getTexInfo();

        u8 var_r30 = 0;
        if (img->indexTexture != 0) {
            var_r30 = getUsableTlut(texIndex);
        }
        getTexture(texIndex)->storeTIMG(img, var_r30);
        return result;
    }

    append(img, 1.0f);
    return NULL;
}


const ResTIMG* J2DPicture::changeTexture(char const* resName, u8 param_1) {
    void* resource = J2DScreen::getNameResource(resName);
    return changeTexture((ResTIMG*)resource, param_1);
}

static bool dummy_weak_order(JUTTexture* tex) {
    J2DPicture* picture;
    return picture->append(tex, 1.0f);
}

const ResTIMG* J2DPicture::changeTexture(ResTIMG const* img, u8 texIndex, JUTPalette* palette) {
    if (texIndex > mTextureNum || 2 <= texIndex || img == NULL) {
        return NULL;
    }

    const ResTIMG* result;
    if (texIndex < mTextureNum) {
        JUTTexture* tex = getTexture(texIndex);
        result = tex->getTexInfo();

        GXTlut var_r30 = GX_TLUT0;
        if (img->indexTexture != 0) {
            var_r30 = getTlutID(img, getUsableTlut(texIndex));
        }
        getTexture(texIndex)->storeTIMG(img, palette, var_r30);
        return result;
    }

    append(img, palette, 1.0f);
    return NULL;
}


const ResTIMG* J2DPicture::changeTexture(char const* param_0, u8 param_1, JUTPalette* param_2) {
    void* resource = J2DScreen::getNameResource(param_0);
    return changeTexture((ResTIMG*)resource, param_1, param_2);
}

void J2DPicture::drawSelf(f32 param_0, f32 param_1) {
    Mtx tmp;
    MTXIdentity(tmp);

    drawSelf(param_0, param_1, &tmp);
}

void J2DPicture::drawSelf(f32 param_0, f32 param_1, Mtx* param_2) {
    if (mTexture[0] != NULL && mTextureNum != 0) {
        drawFullSet(mGlobalBounds.i.x + param_0, mGlobalBounds.i.y + param_1,
                    mBounds.f.x - mBounds.i.x, mBounds.f.y - mBounds.i.y, param_2);
    }
}

void J2DPicture::drawFullSet(f32 param_0, f32 param_1, f32 param_2, f32 param_3, Mtx* param_4) {
    if (mTexture[0] != NULL && mTextureNum != 0) {
        drawTexCoord(mBounds.i.x, mBounds.i.y, param_2, param_3, field_0x10a[0].x, field_0x10a[0].y,
                     field_0x10a[1].x, field_0x10a[1].y, field_0x10a[2].x, field_0x10a[2].y,
                     field_0x10a[3].x, field_0x10a[3].y, param_4);
    }
}

void J2DPicture::draw(f32 x, f32 y, f32 width, f32 height, bool mirrorX, bool mirrorY,
                      bool rotate90) {
    if (isVisible() && mTextureNum != 0 && mTexture[0] != NULL) {
        for (u8 i = 0; i < mTextureNum; i++) {
            load(i);
        }

        GXSetNumTexGens(mTextureNum);
        mColorAlpha = mAlpha;
        JUtility::TColor color[4];
        getNewColor(color);

        setTevMode();
        makeMatrix(x, y, 0.0f, 0.0f);
        GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

        JGeometry::TVec2<s16> coords[4];
        setTexCoord(coords, mTexture[0], BIND15, (J2DMirror)((mirrorX << 1) | mirrorY), rotate90);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S16, 8);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXColor1u32(color[0]);
        GXTexCoord2s16(coords[0].x, coords[0].y);

        GXPosition3f32(width, 0.0f, 0.0f);
        GXColor1u32(color[1]);
        GXTexCoord2s16(coords[1].x, coords[1].y);

        GXPosition3f32(width, height, 0.0f);
        GXColor1u32(color[3]);
        GXTexCoord2s16(coords[3].x, coords[3].y);

        GXPosition3f32(0.0f, height, 0.0f);
        GXColor1u32(color[2]);
        GXTexCoord2s16(coords[2].x, coords[2].y);
        GXEnd();

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U16, 15);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

        Mtx m;
        MTXIdentity(m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    }
}


void J2DPicture::drawOut(JGeometry::TBox2<f32> const& posBox,
                         JGeometry::TBox2<f32> const& texRect) {
    if (isVisible() && mTextureNum != 0 && mTexture[0] != NULL) {
        for (u8 i = 0; i < mTextureNum; i++) {
            load(i);
        }

        GXSetNumTexGens(mTextureNum);

        f32 s0 = (posBox.i.x - texRect.i.x) / texRect.getWidth();
        f32 s1 = (posBox.f.x - texRect.f.x) / texRect.getWidth() + 1.0f;

        f32 t0 = (posBox.i.y - texRect.i.y) / texRect.getHeight();
        f32 t1 = (posBox.f.y - texRect.f.y) / texRect.getHeight() + 1.0f;

        mColorAlpha = mAlpha;
        JUtility::TColor color[4];
        getNewColor(color);
        setTevMode();

        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32((s16)posBox.i.x, (s16)posBox.i.y, 0.0f);
        GXColor1u32(color[0]);
        GXTexCoord2f32(s0, t0);

        GXPosition3f32((s16)posBox.f.x, (s16)posBox.i.y, 0.0f);
        GXColor1u32(color[1]);
        GXTexCoord2f32(s1, t0);

        GXPosition3f32((s16)posBox.f.x, (s16)posBox.f.y, 0.0f);
        GXColor1u32(color[3]);
        GXTexCoord2f32(s1, t1);

        GXPosition3f32((s16)posBox.i.x, (s16)posBox.f.y, 0.0f);
        GXColor1u32(color[2]);
        GXTexCoord2f32(s0, t1);
        GXEnd();

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 15);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
        GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    }
}


void J2DPicture::drawTexCoord(f32 param_0, f32 param_1, f32 param_2, f32 param_3, s16 param_4,
                              s16 param_5, s16 param_6, s16 param_7, s16 param_8, s16 param_9,
                              s16 param_10, s16 param_11, Mtx* param_12) {
    if (mTextureNum != 0) {
        for (u8 i = 0; i < mTextureNum; i++) {
            load(i);
        }

        f32 tmp1 = param_0 + param_2;
        f32 tmp2 = param_1 + param_3;

        GXSetNumTexGens(mTextureNum);

        JUtility::TColor black[4];

        Mtx outputMtx;

        getNewColor(black);
        setTevMode();
        MTXConcat((MtxP)param_12, mGlobalMtx, outputMtx);

        GXLoadPosMtxImm(outputMtx, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        GXPosition3f32(param_0, param_1, 0.0f);
        GXColor1u32(black[0]);
        GXTexCoord2s16(param_4, param_5);

        GXPosition3f32(tmp1, param_1, 0.0f);
        GXColor1u32(black[1]);
        GXTexCoord2s16(param_6, param_7);

        GXPosition3f32(tmp1, tmp2, 0.0f);
        GXColor1u32(black[3]);
        GXTexCoord2s16(param_10, param_11);

        GXPosition3f32(param_0, tmp2, 0.0f);
        GXColor1u32(black[2]);
        GXTexCoord2s16(param_8, param_9);

        GXEnd();

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    }
}


void J2DPicture::setTevMode() {
    u8 i;
    for (i = 0; i < mTextureNum; i++) {
        GXSetTevOrder(GXTevStageID(i), GXTexCoordID(i), GXTexMapID(i), GX_COLOR_NULL);
    }

    GXSetTevColor(GX_TEVREG2, JUtility::TColor(0xFFFFFFFF));
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);

    if (mTexture[0] == NULL) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    } else if (mTexture[0]->getTransparency()) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    } else {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    }

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKColor(GX_KCOLOR0, mBlendKonstColor);
    GXSetTevKColor(GX_KCOLOR2, mBlendKonstAlpha);

    for (i = 1; i < mTextureNum; i++) {
        GXSetTevKColorSel(GXTevStageID(i), GXTevKColorSel(0x20 - i * 4));
        GXSetTevKAlphaSel(GXTevStageID(i), GXTevKAlphaSel(0x22 - i * 4));
        GXSetTevColorIn(GXTevStageID(i), GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);

        if (mTexture[i] == NULL) {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_A2, GX_CA_KONST, GX_CA_ZERO);
        } else if (mTexture[i]->getTransparency()) {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
        } else {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_A2, GX_CA_KONST, GX_CA_ZERO);
        }

        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
    }

    if (mBlack != 0 || mWhite != 0xFFFFFFFF) {
        GXSetTevOrder(GXTevStageID(i), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, mBlack);
        GXSetTevColor(GX_TEVREG1, mWhite);
        GXSetTevColorIn(GXTevStageID(i), GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GXTevStageID(i), GX_CA_A0, GX_CA_A1, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        i++;
    }

    if (mColorAlpha != 0xFF || mCornerColor[0] != 0xFFFFFFFF || mCornerColor[1] != 0xFFFFFFFF ||
        mCornerColor[2] != 0xFFFFFFFF || mCornerColor[3] != 0xFFFFFFFF)
    {
        GXSetTevOrder(GXTevStageID(i), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GXTevStageID(i), GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GXTevStageID(i), GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                        GX_TEVPREV);
        i++;
    }

    GXSetNumTevStages(u8(i));
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);

    for (i = 0; i < mTextureNum; i++) {
        GXSetTexCoordGen(GXTexCoordID(i), GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    }

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumIndStages(0);

    for (int i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
}

void J2DPicture::swap(f32& lhs, f32& rhs) {
    f32 tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

void J2DPicture::setBlendColorRatio(f32 param_0, f32 param_1) {
    field_0x11c[0] = param_0;
    field_0x11c[1] = param_1;
    setBlendKonstColor();
}

void J2DPicture::setBlendAlphaRatio(f32 param_0, f32 param_1) {
    field_0x124[0] = param_0;
    field_0x124[1] = param_1;
    setBlendKonstAlpha();
}

void J2DPicture::setBlendKonstColor() {
    int uvar3 = 0;
    for (u8 i = 1; i < mTextureNum; i++) {
        f32 tmp = 0.0f;
        for (u8 j = 0; j < i; j++) {
            tmp += field_0x11c[j];
        }

        f32 tmp2 = tmp + field_0x11c[i];
        if (tmp2 != 0.0f) {
            // probably fake match but idk whats happening here
            uvar3 |= (u8)(255.0f * (1.0f - tmp / tmp2)) << (i - 1) * 8;
        }
    }
    mBlendKonstColor = uvar3;
}

void J2DPicture::setBlendKonstAlpha() {
    int uvar3 = 0;
    for (u8 i = 1; i < mTextureNum; i++) {
        f32 tmp = 0.0f;
        for (u8 j = 0; j < i; j++) {
            tmp += field_0x124[j];
        }

        f32 tmp2 = tmp + field_0x124[i];
        if (tmp2 != 0.0f) {
            // probably fake match but idk whats happening here
            uvar3 |= (u8)(255.0f * (1.0f - tmp / tmp2)) << (i - 1) * 8;
        }
    }
    mBlendKonstAlpha = uvar3;
}

void J2DPicture::getNewColor(JUtility::TColor* param_0) {
    param_0[0] = mCornerColor[0];
    param_0[1] = mCornerColor[1];
    param_0[2] = mCornerColor[2];
    param_0[3] = mCornerColor[3];

    if (mColorAlpha != 0xFF) {
        param_0[0].a = (param_0[0].a * mColorAlpha) / 0xFF;
        param_0[1].a = (param_0[1].a * mColorAlpha) / 0xFF;
        param_0[2].a = (param_0[2].a * mColorAlpha) / 0xFF;
        param_0[3].a = (param_0[3].a * mColorAlpha) / 0xFF;
    }
}

void J2DPicture::setTexCoord(JUTTexture const* param_0, J2DBinding param_1, J2DMirror param_2,
                             bool param_3) {
    setTexCoord(&field_0x10a[0], param_0, param_1, param_2, param_3);
}

void J2DPicture::setTexCoord(JGeometry::TVec2<s16>* param_0, JUTTexture const* param_1,
                             J2DBinding binding, J2DMirror mirror, bool rotate90) {
    bool bindLeft;
    bool bindRight;
    bool bindTop;
    bool bindBottom;

    if (!rotate90) {
        if (mirror & J2DMirror_X)
            bindLeft = binding & J2DBind_Right;
        else
            bindLeft = binding & J2DBind_Left;

        if (mirror & J2DMirror_X)
            bindRight = binding & J2DBind_Left;
        else
            bindRight = binding & J2DBind_Right;

        if (mirror & J2DMirror_Y)
            bindTop = binding & J2DBind_Bottom;
        else
            bindTop = binding & J2DBind_Top;

        if (mirror & J2DMirror_Y)
            bindBottom = binding & J2DBind_Top;
        else
            bindBottom = binding & J2DBind_Bottom;
    } else {
        if (mirror & J2DMirror_X)
            bindLeft = binding & J2DBind_Bottom;
        else
            bindLeft = binding & J2DBind_Top;

        if (mirror & J2DMirror_X)
            bindRight = binding & J2DBind_Top;
        else
            bindRight = binding & J2DBind_Bottom;

        if (mirror & J2DMirror_Y)
            bindTop = binding & J2DBind_Left;
        else
            bindTop = binding & J2DBind_Right;

        if (mirror & J2DMirror_Y)
            bindBottom = binding & J2DBind_Right;
        else
            bindBottom = binding & J2DBind_Left;
    }

    f32 rectWidth;
    f32 rectHeight;

    rectWidth = !rotate90 ? getWidth() : getHeight();
    rectHeight = !rotate90 ? getHeight() : getWidth();

    f32 texWidth;
    f32 texHeight;
    if (param_1 == NULL) {
        texWidth = rectWidth;
        texHeight = rectHeight;
    } else {
        texWidth = param_1->getWidth();
        texHeight = param_1->getHeight();
    }

    f32 s0, t0, s1, t1;
    if (bindLeft) {
        s0 = 0.0f;
        s1 = bindRight ? 1.0f : (rectWidth / texWidth);
    } else if (bindRight) {
        s0 = 1.0f - (rectWidth / texWidth);
        s1 = 1.0f;
    } else {
        s0 = 0.5f - (rectWidth / texWidth) / 2.0f;
        s1 = 0.5f + (rectWidth / texWidth) / 2.0f;
    }

    if (bindTop) {
        t0 = 0.0f;
        t1 = bindBottom ? 1.0f : (rectHeight / texHeight);
    } else if (bindBottom) {
        t0 = 1.0f - (rectHeight / texHeight);
        t1 = 1.0f;
    } else {
        t0 = 0.5f - (rectHeight / texHeight) / 2.0f;
        t1 = 0.5f + (rectHeight / texHeight) / 2.0f;
    }

    if (mirror & J2DMirror_X) {
        swap(s0, s1);
    }
    if (mirror & J2DMirror_Y) {
        swap(t0, t1);
    }

    s16 temp_r27 = J2DCast_F32_to_S16(s0, 8);
    s16 temp_r28 = J2DCast_F32_to_S16(s1, 8);
    s16 temp_r30 = J2DCast_F32_to_S16(t0, 8);
    s16 temp_r31 = J2DCast_F32_to_S16(t1, 8);

    if (!rotate90) {
        param_0[0].set(temp_r27, temp_r30);
        param_0[1].set(temp_r28, temp_r30);
        param_0[2].set(temp_r27, temp_r31);
        param_0[3].set(temp_r28, temp_r31);
    } else {
        param_0[0].set(temp_r27, temp_r31);
        param_0[1].set(temp_r27, temp_r30);
        param_0[2].set(temp_r28, temp_r31);
        param_0[3].set(temp_r28, temp_r30);
    }
}


bool J2DPicture::isUsed(ResTIMG const* param_0) {
    for (u8 i = 0; i < mTextureNum; i++) {
        if (mTexture[i] != NULL && mTexture[i]->getTexInfo() == param_0) {
            return true;
        }
    }

    return J2DPane::isUsed(param_0);
}

u8 J2DPicture::getUsableTlut(u8 param_0) {
    u8 var_r8 = 0;

    for (u8 i = 0; i < mTextureNum; i++) {
        if (i != param_0 && mTexture[i] != NULL) {
            const ResTIMG* img = mTexture[i]->getTexInfo();

            if (img != NULL && img->indexTexture != 0) {
                int name = mTexture[i]->getTlutName();
                int var_r0 = name >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0;

                u8 temp_r0 = name - var_r0;
                if (temp_r0 < 2) {
                    var_r8 |= (1 << temp_r0);
                }
            }
        }
    }

    u8 var_r26 = 0;
    for (u8 i = 0; i < 2; i++) {
        if (!(var_r8 & (1 << i))) {
            var_r26 = i;
            break;
        }
    }

    return var_r26;
}

GXTlut J2DPicture::getTlutID(ResTIMG const* img, u8 param_1) {
    if (img->numColors > 256) {
        return (GXTlut)((param_1 % 4) + 0x10);
    }

    return (GXTlut)param_1;
}
