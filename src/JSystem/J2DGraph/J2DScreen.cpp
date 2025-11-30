#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DMaterialFactory.h"
#include "JSystem/J2DGraph/J2DPictureEx.h"
#include "JSystem/J2DGraph/J2DTextBoxEx.h"
#include "JSystem/J2DGraph/J2DWindowEx.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSUMemoryStream.h"
#include "dolphin/types.h"

J2DScreen::J2DScreen()
    : J2DPane(NULL, true, 'root', JGeometry::TBox2<f32>(JGeometry::TVec2<f32>(0, 0), JGeometry::TVec2<f32>(640, 480))), mColor() {
    field_0x4 = -1;
    mScissor = false;
    mMaterialNum = 0;
    mMaterials = NULL;
    mTexRes = NULL;
    mFontRes = NULL;
    mNameTable = NULL;
}

J2DScreen::~J2DScreen() {
    clean();
}

void J2DScreen::clean() {
    delete[] mMaterials;
    mMaterialNum = 0;
    mMaterials = NULL;

    delete[] mTexRes;
    mTexRes = NULL;

    delete[] mFontRes;
    mFontRes = NULL;

    if (mNameTable != NULL) {
        delete[] mNameTable->getResNameTable();
        delete mNameTable;
        mNameTable = NULL;
    }
}

bool J2DScreen::setPriority(char const* resName, u32 param_1, JKRArchive* p_archive) {
    if (p_archive == NULL) {
        return false;
    }

    void* res = JKRGetNameResource(resName, p_archive);
    if (res != NULL) {
        JSUMemoryInputStream stream(res, p_archive->getExpandedResSize(res));
        return setPriority(&stream, param_1, p_archive);
    }

    return false;
}

bool J2DScreen::setPriority(JSURandomInputStream* p_stream, u32 param_1, JKRArchive* p_archive) {
    if (p_archive == NULL) {
        return false;
    } else {
        return private_set(p_stream, param_1, p_archive);
    }
}

bool J2DScreen::private_set(JSURandomInputStream* p_stream, u32 param_1, JKRArchive* p_archive) {
    if (!checkSignature(p_stream)) {
        return false;
    }

    if (!getScreenInformation(p_stream)) {
        return false;
    }

    bool make_end = makeHierarchyPanes(this, p_stream, param_1, p_archive) != 2;

    if (!(param_1 & 0x1F0000)) {
        clean();
    }

    return make_end ? p_stream->isGood() : false;
}

bool J2DScreen::checkSignature(JSURandomInputStream* p_stream) {
    J2DScrnHeader header;
    p_stream->read(&header, sizeof(J2DScrnHeader));

    if (header.mTag != 'SCRN' || (header.mType != 'blo1' && header.mType != 'blo2')) {
        JUT_WARN(257, "%s", "SCRN resource is broken.\n")
        return false;
    } else {
        return true;
    }
}

bool J2DScreen::getScreenInformation(JSURandomInputStream* p_stream) {
    J2DScrnInfoHeader info;
    p_stream->read(&info, sizeof(J2DScrnInfoHeader));

    if (info.mTag != 'INF1') {
        JUT_WARN(282, "%s", "SCRN resource is broken.\n")
        return false;
    }

    place(JGeometry::TBox2<f32>(0.0f, 0.0f, info.mWidth, info.mHeight));

    mColor = info.mColor;

    if (info.mSize > 0x10) {
        p_stream->skip(info.mSize - 0x10);
    }

    return true;
}

s32 J2DScreen::makeHierarchyPanes(J2DPane* p_basePane, JSURandomInputStream* p_stream, u32 param_2,
                                  JKRArchive* p_archive) {
    J2DPane* next_pane = p_basePane;

    while (true) {
        J2DScrnBlockHeader header;
        p_stream->peek(&header, sizeof(J2DScrnBlockHeader));

        switch (header.mTag) {
        case 'EXT1':
            p_stream->seek(header.mSize, JSUStreamSeekFrom_CUR);
            return 1;
        case 'BGN1': {
            p_stream->seek(header.mSize, JSUStreamSeekFrom_CUR);

            s32 ret = makeHierarchyPanes(next_pane, p_stream, param_2, p_archive);
            if (ret != 0) {
                return ret;
            }
            break;
        }
        case 'END1':
            p_stream->seek(header.mSize, JSUStreamSeekFrom_CUR);
            return 0;
        case 'TEX1':
            if ((mTexRes = getResReference(p_stream, param_2)) == NULL) {
                return 2;
            }
            break;
        case 'FNT1':
            if ((mFontRes = getResReference(p_stream, param_2)) == NULL) {
                return 2;
            }
            break;
        case 'MAT1':
            if (!createMaterial(p_stream, param_2, p_archive)) {
                return 2;
            }
            break;
        default:
            if (p_archive == NULL) {
                next_pane = createPane(header, p_stream, p_basePane, param_2);
            } else {
                next_pane = createPane(header, p_stream, p_basePane, param_2, p_archive);
            }

            if (next_pane == NULL) {
                return 2;
            }
            break;
        }
    }
}

J2DPane* J2DScreen::createPane(J2DScrnBlockHeader const& header, JSURandomInputStream* p_stream,
                               J2DPane* p_basePane, u32 param_3, JKRArchive* p_archive) {
    J2DPane* newPane;

    switch (header.mTag) {
    case 'PAN1':
        newPane = new J2DPane(p_basePane, p_stream, 0);
        break;
    case 'WIN1':
        newPane = new J2DWindow(p_basePane, p_stream, p_archive);
        break;
    case 'PIC1':
        newPane = new J2DPicture(p_basePane, p_stream, p_archive);
        break;
    case 'TBX1':
        newPane = new J2DTextBox(p_basePane, p_stream, p_archive);
        break;
    case 'PAN2':
        newPane = new J2DPane(p_basePane, p_stream, 1);
        break;
    case 'WIN2':
        if (param_3 & 0x1F0000) {
            newPane = new J2DWindowEx(p_basePane, p_stream, param_3, mMaterials);
            break;
        }
        newPane = new J2DWindow(p_basePane, p_stream, mMaterials);
        break;
    case 'PIC2':
        if (param_3 & 0x1F0000) {
            newPane = new J2DPictureEx(p_basePane, p_stream, param_3, mMaterials);
            break;
        }
        newPane = new J2DPicture(p_basePane, p_stream, mMaterials);
        break;
    case 'TBX2':
        if (param_3 & 0x1F0000) {
            newPane = new J2DTextBoxEx(p_basePane, p_stream, param_3, mMaterials);
            break;
        }
        newPane = new J2DTextBox(p_basePane, p_stream, param_3, mMaterials);
        break;
    default:
        s32 position = p_stream->getPosition();
        s32 size = header.mSize;
        s32 start = size + position;

        newPane = new J2DPane(p_basePane, p_stream, 0);
        p_stream->seek(start, JSUStreamSeekFrom_SET);
        break;
    }

    return newPane;
}

void J2DScreen::draw(f32 x, f32 y, J2DGrafContext const* grafCtx) {
    u32 l_x, l_y, width, height;

    if (mScissor) {
        GXGetScissor(&l_x, &l_y, &width, &height);
    }

    if (grafCtx != NULL) {
        J2DPane::draw(x, y, grafCtx, mScissor, true);
    } else {
        J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
        ortho.setPort();
        J2DPane::draw(x, y, &ortho, mScissor, true);
    }

    if (mScissor) {
        GXSetScissor(l_x, l_y, width, height);
    }

    GXSetNumIndStages(0);
    for (int i = 0; i < 0x10; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumTexGens(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
}

J2DPane* J2DScreen::search(u64 tag) {
    if (tag == 0) {
        return NULL;
    }

    return J2DPane::search(tag);
}

J2DPane* J2DScreen::searchUserInfo(u64 tag) {
    if (tag == 0) {
        return NULL;
    }

    return J2DPane::searchUserInfo(tag);
}

void J2DScreen::drawSelf(f32 param_0, f32 param_1, Mtx* param_2) {
    JUtility::TColor color(mColor);
    u8 alpha = (color.a * mAlpha) / 255;

    if (alpha != 0) {
        JUtility::TColor sp8(alpha | ((u32)color & 0xFFFFFF00));
        color = sp8;

        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        GXPosition3f32(0.0f, 0.0f, 0.0f);
        GXColor1u32(color);

        GXPosition3f32(getWidth(), 0.0f, 0.0f);
        GXColor1u32(color);

        GXPosition3f32(getWidth(), getHeight(), 0.0f);
        GXColor1u32(color);

        GXPosition3f32(0.0f, getHeight(), 0.0f);
        GXColor1u32(color);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXEnd();
    }
}

J2DResReference* J2DScreen::getResReference(JSURandomInputStream* p_stream, u32 param_1) {
    s32 position = p_stream->getPosition();
    p_stream->skip(4);

    s32 size1, size2;
    size1 = p_stream->readS32();
    p_stream->skip(4);
    size2 = p_stream->readS32();
    p_stream->seek(position + size2, JSUStreamSeekFrom_SET);

    size1 = size1 - size2;

    char* buffer;
    if (param_1 & 0x1F0000) {
        buffer = new char[size1];
    } else {
        buffer = new (-4) char[size1];
    }

    if (buffer != NULL) {
        p_stream->read(buffer, size1);
    }

    return (J2DResReference*)buffer;
}

bool J2DScreen::createMaterial(JSURandomInputStream* p_stream, u32 param_1, JKRArchive* p_archive) {
    s32 position = p_stream->getPosition();

    J2DScrnBlockHeader header;
    p_stream->read(&header, 8);
    mMaterialNum = p_stream->readU16();

    p_stream->skip(2);

    if (param_1 & 0x1F0000) {
        mMaterials = new J2DMaterial[mMaterialNum];
    } else {
        mMaterials = new (-4) J2DMaterial[mMaterialNum];
    }

    u8* buffer = new (-4) u8[header.mSize];
    if (mMaterials != NULL && buffer != NULL) {
        J2DMaterialBlock* pBlock = (J2DMaterialBlock*)buffer;
        p_stream->seek(position, JSUStreamSeekFrom_SET);
        p_stream->read(buffer, header.mSize);
        J2DMaterialFactory factory(*pBlock);

        for (u16 i = 0; i < mMaterialNum; i++) {
            factory.create(&mMaterials[i], i, param_1, mTexRes, mFontRes, p_archive);
        }

        if (param_1 & 0x1F0000) {
            u32 offset =
                buffer[0x14] << 0x18 | buffer[0x15] << 0x10 | buffer[0x16] << 8 | buffer[0x17];
            ResNTAB* sec_s = (ResNTAB*)(buffer + offset);
            u16 entryNum = sec_s->mEntryNum;
            u16 lastOffset = sec_s->mEntries[entryNum - 1].mOffs;
            char* ptr = (char*)sec_s;
            u16 size = lastOffset;
            while (ptr[size] != 0) {
                size++;  
            }
            size++; 

            u8* nametab = new u8[size];
            if (nametab == NULL) {
                goto failure;
            }
            for (u16 i = 0; i < size; i++) {
                nametab[i] = (buffer + offset)[i];
            }

            mNameTable = new JUTNameTab((ResNTAB*)nametab);
            if (mNameTable == NULL) {
                delete[] nametab;
                goto failure;
            }
        }

    success:
        delete[] buffer;
        return true;
    }

failure:
    delete[] buffer;
    clean();
    return false;
}

bool J2DScreen::isUsed(ResTIMG const* p_timg) {
    return J2DPane::isUsed(p_timg);
}

bool J2DScreen::isUsed(ResFONT const* p_font) {
    return J2DPane::isUsed(p_font);
}

J2DDataManage* J2DScreen::mDataManage;

void* J2DScreen::getNameResource(char const* resName) {
    void* res = JKRFileLoader::getGlbResource(resName, NULL);

    if (res == NULL && mDataManage != NULL) {
        res = mDataManage->get(resName);
    }

    return res;
}

void J2DScreen::animation() {
    animationPane(mTransform);

    for (u16 i = 0; i < mMaterialNum; i++) {
        mMaterials[i].animation();
    }
}

void J2DScreen::setAnimation(J2DAnmColor* p_anmColor) {
    p_anmColor->searchUpdateMaterialID(this);
    u16 matNum = p_anmColor->getUpdateMaterialNum();

    for (u16 i = 0; i < matNum; i++) {
        u16 matID = p_anmColor->getUpdateMaterialID(i);
        if (matID < mMaterialNum) {
            mMaterials[matID].setAnimation(p_anmColor);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmTextureSRTKey* p_anmSRTKey) {
    p_anmSRTKey->searchUpdateMaterialID(this);
    u16 matNum = p_anmSRTKey->getUpdateMaterialNum();

    for (u16 i = 0; i < matNum; i++) {
        u16 matID = p_anmSRTKey->getUpdateMaterialID(i);
        if (matID < mMaterialNum) {
            mMaterials[matID].setAnimation(p_anmSRTKey);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmTexPattern* p_anmPattern) {
    p_anmPattern->searchUpdateMaterialID(this);
    u16 matNum = p_anmPattern->getUpdateMaterialNum();

    for (u16 i = 0; i < matNum; i++) {
        u16 matID = p_anmPattern->getUpdateMaterialID(i);
        if (matID < mMaterialNum) {
            mMaterials[matID].setAnimation(p_anmPattern);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmTevRegKey* p_anmRegKey) {
    p_anmRegKey->searchUpdateMaterialID(this);

    u16 cMatNum = p_anmRegKey->getCRegUpdateMaterialNum();
    for (u16 i = 0; i < cMatNum; i++) {
        u16 matID = p_anmRegKey->getCRegUpdateMaterialID(i);
        if (matID < mMaterialNum) {
            mMaterials[matID].setAnimation(p_anmRegKey);
        }
    }

    u16 kMatNum = p_anmRegKey->getKRegUpdateMaterialNum();
    for (u16 i = 0; i < kMatNum; i++) {
        u16 matID = p_anmRegKey->getKRegUpdateMaterialID(i);
        if (matID < mMaterialNum) {
            mMaterials[matID].setAnimation(p_anmRegKey);
        }
    }
}

void J2DScreen::setAnimation(J2DAnmVtxColor* p_anmVtxColor) {
    J2DPane::setVtxColorAnimation(p_anmVtxColor);
}

void J2DScreen::setAnimation(J2DAnmVisibilityFull* p_anmVisibility) {
    J2DPane::setVisibileAnimation(p_anmVisibility);
}

J2DPane* J2DScreen::createPane(J2DScrnBlockHeader const& header, JSURandomInputStream* p_stream,
                               J2DPane* p_basePane, u32 param_3) {
    return createPane(header, p_stream, p_basePane, param_3, NULL);
}

u16 J2DScreen::getTypeID() const {
    return 8;
}

void J2DScreen::calcMtx() {
    makeMatrix(mTranslateX, mTranslateY);
}

void J2DScreen::setAnimation(J2DAnmBase* p_anm) {
    J2DPane::setAnimation(p_anm);
}

void J2DScreen::setAnimationVF(J2DAnmVisibilityFull*) {
    /* empty function */
}

void J2DScreen::setAnimationVC(J2DAnmVtxColor*) {
    /* empty function */
}
