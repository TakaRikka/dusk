#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTCacheFont.h"
#include "JSystem/JUtility/JUTException.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTConsole.h"
#include "JSystem/JKernel/JKRAram.h"
#include "dolphin/gx.h"

JUTCacheFont::JUTCacheFont(ResFONT const* p_fontRes, u32 cacheSize, JKRHeap* p_heap) {
    initialize_state();
    JUTResFont::initialize_state();
    JUTFont::initialize_state();
    initiate(p_fontRes, NULL, cacheSize, p_heap);
}

JUTCacheFont::~JUTCacheFont() {
    if (isValid()) {
        deleteMemBlocks_CacheFont();
        initialize_state();

        deleteMemBlocks_ResFont();
        JUTResFont::initialize_state();

        JUTFont::initialize_state();
    }
}

void JUTCacheFont::deleteMemBlocks_CacheFont() {
    if (field_0xb0 != 0) {
        delete[] mCacheBuffer;
    }

    delete field_0xac;
    delete mInf1Ptr;
    delete mMemBlocks;
    delete field_0x7c;
    delete field_0x80;
    delete field_0x84;
}

void JUTCacheFont::initialize_state() {
    field_0xb0 = 0;
    mCacheBuffer = NULL;

    field_0xac = NULL;
    mInf1Ptr = NULL;
    field_0x7c = NULL;
    field_0x80 = NULL;
    field_0x84 = NULL;
    mMemBlocks = NULL;

    mPagingType = PAGE_TYPE_0;
    mMaxSheetSize = 0;

    mCacheBuffer = NULL;
    field_0x9c = NULL;
    field_0xa0 = NULL;
}

int JUTCacheFont::getMemorySize(ResFONT const* p_font, u16* o_widCount, u32* o_widSize,
                                u16* o_glyCount, u32* o_glySize, u16* o_mapCount, u32* o_mapSize,
                                u32* o_glyTexSize) {
    if (p_font == NULL) {
        return 0;
    }

    u16 widBlockCount = 0;
    u16 glyBlockCount = 0;
    u16 mapBlockCount = 0;
    u32 totalWidSize = 0;
    u32 totalGlySize = 0;
    u32 totalMapSize = 0;
    u32 maxGlyTexSize = 0;
    u32 glyTexSize;

    u8* fontInf = (u8*)p_font->data;
    for (int i = 0; i < p_font->numBlocks; i++) {
        switch (((BlockHeader*)fontInf)->magic) {
        case 'INF1':
            break;
        case 'WID1':
            totalWidSize += ((BlockHeader*)fontInf)->size;
            widBlockCount++;
            break;
        case 'GLY1':
            totalGlySize += ((BlockHeader*)fontInf)->size;
            glyTexSize = ((ResFONT::GLY1*)fontInf)->textureSize;
            glyBlockCount++;
            if (glyTexSize > maxGlyTexSize) {
                maxGlyTexSize = glyTexSize;
            }
            break;
        case 'MAP1':
            totalMapSize += ((BlockHeader*)fontInf)->size;
            mapBlockCount++;
            break;
        default:
            JUTReportConsole("JUTCacheFont: Unknown data block\n");
            break;
        }

        fontInf += ((BlockHeader*)fontInf)->size;
    }

    if (o_widCount != NULL) {
        *o_widCount = widBlockCount;
    }

    if (o_glyCount != NULL) {
        *o_glyCount = glyBlockCount;
    }

    if (o_mapCount != NULL) {
        *o_mapCount = mapBlockCount;
    }

    if (o_widSize != NULL) {
        *o_widSize = totalWidSize;
    }

    if (o_glySize != NULL) {
        *o_glySize = totalGlySize;
    }

    if (o_mapSize != NULL) {
        *o_mapSize = totalMapSize;
    }

    if (o_glyTexSize != NULL) {
        *o_glyTexSize = maxGlyTexSize;
    }

    return 1;
}

int JUTCacheFont::initiate(ResFONT const* p_fontRes, void* param_1, u32 param_2, JKRHeap* p_heap) {
    if (!internal_initiate(p_fontRes, param_1, param_2, p_heap)) {
        deleteMemBlocks_CacheFont();
        deleteMemBlocks_ResFont();
        JUTFont::initialize_state();
        mValid = false;
        return 0;
    }

    return 1;
}

bool JUTCacheFont::internal_initiate(ResFONT const* p_fontRes, void* param_1, u32 param_2,
                                     JKRHeap* param_3) {
    deleteMemBlocks_CacheFont();
    initialize_state();
    deleteMemBlocks_ResFont();
    JUTResFont::initialize_state();
    JUTFont::initialize_state();

    if (p_fontRes == NULL) {
        return false;
    }

    mResFont = p_fontRes;
    mValid = true;
    getMemorySize(p_fontRes, &mWid1BlockNum, &mTotalWidSize, &mGly1BlockNum, &mTotalGlySize,
                  &mMap1BlockNum, &mTotalMapSize, &mMaxSheetSize);

    if (!allocArea(param_1, param_2, param_3)) {
        return false;
    } else if (!allocArray(param_3)) {
        return false;
    }

    setBlock();
    return true;
}

bool JUTCacheFont::allocArea(void* param_0, u32 param_1, JKRHeap* heap) {
    mInf1Ptr = (ResFONT::INF1*)new (heap, 0) ResFONT();
    if (mInf1Ptr == NULL) {
        return false;
    }

    if (mTotalWidSize != 0) {
        field_0x7c = new (heap, 0) u8[mTotalWidSize];
        if (field_0x7c == NULL) {
            return false;
        }
    }

    if (mGly1BlockNum != 0) {
        field_0x80 = new (heap, 0) ResFONT::GLY1[mGly1BlockNum];
        if (field_0x80 == NULL) {
            return false;
        }

        field_0xac = JKRAram::getManager()->mAramHeap->alloc(
            mTotalGlySize - (mGly1BlockNum * sizeof(ResFONT::GLY1)), JKRAramHeap::HEAD);
        if (field_0xac == NULL) {
            return false;
        }
    }

    if (mTotalMapSize != 0) {
        field_0x84 = new (heap, 0) u8[mTotalMapSize];
        if (field_0x84 == NULL) {
            return false;
        }
    }

    field_0x94 = mMaxSheetSize + 0x40;
    mCachePage = param_1 / field_0x94;
    u32 v1 = field_0x94 * mCachePage;
    if (mCachePage == 0) {
        return false;
    }

    if (param_0 != NULL) {
        mCacheBuffer = param_0;
        field_0xb0 = 0;
    } else {
        mCacheBuffer = new (heap, 0x20) u8[v1];
        if (mCacheBuffer == NULL) {
            return false;
        }
        field_0xb0 = 1;
    }

    invalidiateAllCache();
    return true;
}

bool JUTCacheFont::allocArray(JKRHeap* param_0) {
    mMemBlocks = (void**)new (param_0, 0) u32[mWid1BlockNum + mGly1BlockNum + mMap1BlockNum];
    if (mMemBlocks == NULL) {
        return false;
    }

    void** blocks = mMemBlocks;
    if (mWid1BlockNum) {
        mpWidthBlocks = (ResFONT::WID1**)blocks;
        blocks = blocks + mWid1BlockNum;
    }
    if (mGly1BlockNum) {
        mpGlyphBlocks = (ResFONT::GLY1**)blocks;
        blocks = blocks + mGly1BlockNum;
        for (int i = 0; i < mGly1BlockNum; i++) {
            mpGlyphBlocks[i] = (ResFONT::GLY1*)((u8*)mCacheBuffer + (field_0x94 * i));
        }
    } 
    if (mMap1BlockNum) {
        mpMapBlocks = (ResFONT::MAP1**)blocks;
    }
    return true;
}

void JUTCacheFont::setBlock() {
    int widthNum = 0;
    int gylphNum = 0;
    int mapNum = 0;
    u8* pWidth = (u8*)field_0x7c;
    ResFONT::GLY1* piVar5 = (ResFONT::GLY1*)field_0x80;
    ResFONT::MAP1* pMap = (ResFONT::MAP1*)field_0x84;
    u32 aramAddress = field_0xac->getAddress();
    mMaxCode = 0xffff;
    const int* pData = (int*)mResFont->data;

    for (int i = 0; i < mResFont->numBlocks; i++) {
        switch (*pData) {
        case 'INF1':
            memcpy(mInf1Ptr, pData, 0x20);
            mIsLeadByte = &JUTResFont::saoAboutEncoding_[mInf1Ptr->fontType];
            break;
        case 'WID1':
            memcpy(pWidth, pData, pData[1]);
            mpWidthBlocks[widthNum] = (ResFONT::WID1*)pWidth;
            pWidth += pData[1];
            widthNum++;
            break;
        case 'GLY1':
            memcpy(piVar5, pData, 0x20);
            JKRAramBlock* iVar1 =
                JKRMainRamToAram((u8*)pData + 0x20, aramAddress, pData[1] - 0x20,
                                 EXPAND_SWITCH_UNKNOWN0, 0, NULL, 0xffffffff, NULL);
            if (iVar1 == NULL) {
                JUTException::panic("JUTCacheFont.cpp", 0x1dd,
                                    "trouble occurred in JKRMainRamToAram.");
            }
            piVar5->magic = aramAddress;
            if (piVar5->textureSize > mMaxSheetSize) {
                mMaxSheetSize = piVar5->textureSize;
            }
            mpGlyphBlocks[gylphNum] = piVar5;
            gylphNum++;
            piVar5++;
            aramAddress = pData[1] + aramAddress - 0x20;
            break;
        case 'MAP1':
            memcpy(pMap, pData, pData[1]);
            mpMapBlocks[mapNum] = pMap;
            if (mMaxCode > mpMapBlocks[mapNum]->startCode) {
                mMaxCode = mpMapBlocks[mapNum]->startCode;
            }
            mapNum++;
            pMap = (ResFONT::MAP1*)((u8*)pMap + pData[1]);
            break;
        default:
            JUTReportConsole("Unknown data block\n");
            break;
        }

        pData = (int*)((u8*)pData + pData[1]);
    }
}

JUTCacheFont::TGlyphCacheInfo* JUTCacheFont::determineBlankPage() {
    TGlyphCacheInfo* pVar1 = field_0xa4;
    if (pVar1 != NULL) {
        field_0xa4 = pVar1->mNext;
        TGlyphCacheInfo* pVar2 = pVar1->mNext;
        if (pVar2 == NULL) {
            field_0xa8 = 0;
        } else {
            pVar2->mPrev = NULL;
        }
        return pVar1;
    }

    TGlyphCacheInfo* puVar1 = field_0xa0;
    while (puVar1 != NULL) {
        TGlyphCacheInfo* prev = puVar1->mPrev;
        if (puVar1->field_0x1e == 0) {
            unlink(puVar1);
            field_0xb4++;
            return puVar1;
        }
        puVar1 = prev;
    }

    return NULL;
}

/* 802DDBBC-802DDCE4 2D84FC 0128+00 1/1 0/0 0/0 .text
 * getGlyphFromAram__12JUTCacheFontFPQ212JUTCacheFont15TGlyphCacheInfoPQ212JUTCacheFont10TCachePagePiPi
 */
void JUTCacheFont::getGlyphFromAram(JUTCacheFont::TGlyphCacheInfo* param_0,
                                    JUTCacheFont::TCachePage* pCachePage, int* param_2, int* param_3) {
    TGlyphCacheInfo* pGylphCacheInfo = pCachePage;
    memcpy(pGylphCacheInfo, param_0, sizeof(TGlyphCacheInfo));
    prepend(pGylphCacheInfo);
    int iVar3 = pGylphCacheInfo->field_0x16 * pGylphCacheInfo->field_0x18;
    int iVar2 = *param_2 / iVar3;
    pGylphCacheInfo->field_0x8 += iVar2 * iVar3;
    u16 local_30 = pGylphCacheInfo->field_0x8 + iVar3 - 1;
    pGylphCacheInfo->field_0xa = pGylphCacheInfo->field_0xa < local_30 ? pGylphCacheInfo->field_0xa : local_30;
    *param_3 = iVar2;
    *param_2 -= iVar2 * iVar3;
    u8* addr =
        JKRAramToMainRam((u32)param_0->mPrev + pGylphCacheInfo->field_0x10 * iVar2, pCachePage->mImage,
                         pGylphCacheInfo->field_0x10, EXPAND_SWITCH_UNKNOWN0, 0, NULL, 0xffffffff, NULL);
    JUT_ASSERT(624, addr != NULL);
    GXInitTexObj(&pCachePage->mTexObj, pCachePage->mImage, pGylphCacheInfo->mWidth, pGylphCacheInfo->mHeight,
                 (GXTexFmt)pGylphCacheInfo->mTexFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&pCachePage->mTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE,
                    GX_ANISO_1);
}

void JUTCacheFont::loadImage(int param_0, _GXTexMapID texMapId) {
    TCachePage* cachePage = loadCache_char_subroutine(&param_0, false);
    if (cachePage != NULL) {
        mWidth = cachePage->field_0xc * (param_0 % (int)cachePage->field_0x16);
        mHeight = cachePage->field_0xe * (param_0 / cachePage->field_0x16);
        GXLoadTexObj(getTexObj(cachePage), texMapId);
        if (mPagingType == PAGE_TYPE_1) {
            unlink(cachePage);
            prepend(cachePage);
        }
    }
}

JUTCacheFont::TCachePage* JUTCacheFont::loadCache_char_subroutine(int* param_0, bool param_1) {
    TCachePage* rv = NULL;
    for (TCachePage* pCachePage = (TCachePage*)field_0x9c; pCachePage != NULL;
         pCachePage = (TCachePage*)pCachePage->mNext)
    {
        if (pCachePage->field_0x8 <= *param_0 && *param_0 <= pCachePage->field_0xa) {
            rv = pCachePage;
            *param_0 -= pCachePage->field_0x8;
            break;
        }
    }

    if (rv == NULL) {
        int i = 0;
        for (; i < mGly1BlockNum; i++) {
            if (mpGlyphBlocks[i]->startCode <= *param_0 && *param_0 <= mpGlyphBlocks[i]->endCode) {
                *param_0 -= mpGlyphBlocks[i]->startCode;
                break;
            }
        }
        if (i < mGly1BlockNum) {
            TCachePage* pBlankPage = (TCachePage*)determineBlankPage();
            if (pBlankPage == NULL) {
                return NULL;
            }
            int texPageIdx;
            getGlyphFromAram((JUTCacheFont::TGlyphCacheInfo*)mpGlyphBlocks[i], pBlankPage, param_0,
                             &texPageIdx);
            mTexPageIdx = texPageIdx;
            field_0x66 = i;
            rv = pBlankPage;
        } else {
            return NULL;
        }
    }
    if (param_1) {
        rv->field_0x1e = 1;
    }
    return rv;
}

void JUTCacheFont::invalidiateAllCache() {
    int* piVar3 = (int*)mCacheBuffer;
    for (int uVar2 = 0; uVar2 < mCachePage; uVar2++) {
        int iVar1;
        if (uVar2 == 0) {
            iVar1 = 0;
        } else {
            iVar1 = (int)piVar3 - field_0x94;
        }
        *piVar3 = iVar1;
        if (uVar2 == mCachePage - 1) {
            iVar1 = 0;
        } else {
            iVar1 = (int)piVar3 + field_0x94;
        }
        piVar3[1] = iVar1;
        piVar3 = (int*)((int)piVar3 + field_0x94);
    }
    field_0xa8 = (int)piVar3 - field_0x94;
    field_0xa4 = (TGlyphCacheInfo*)mCacheBuffer;
    field_0x9c = NULL;
    field_0xa0 = NULL;
}

void JUTCacheFont::unlink(JUTCacheFont::TGlyphCacheInfo* cacheInfo) {
    if (cacheInfo->mPrev == NULL) {
        field_0x9c = cacheInfo->mNext;
    } else {
        cacheInfo->mPrev->mNext = cacheInfo->mNext;
    }

    if (cacheInfo->mNext == NULL) {
        field_0xa0 = cacheInfo->mPrev;
    } else {
        cacheInfo->mNext->mPrev = cacheInfo->mPrev;
    }
}

void JUTCacheFont::prepend(JUTCacheFont::TGlyphCacheInfo* cacheInfo) {
    TGlyphCacheInfo* oldHead = field_0x9c;
    field_0x9c = cacheInfo;
    cacheInfo->mPrev = NULL;
    cacheInfo->mNext = oldHead;

    if (oldHead == NULL) {
        field_0xa0 = cacheInfo;
    } else {
        oldHead->mPrev = cacheInfo;
    }
}

ResFONT* JUTResFont::getResFont() const {
    return (ResFONT*)mResFont;
}

int JUTResFont::getFontType() const {
    return mInf1Ptr->fontType;
}

int JUTResFont::getLeading() const {
    return mInf1Ptr->leading;
}

s32 JUTResFont::getWidth() const {
    return mInf1Ptr->width;
}

s32 JUTResFont::getAscent() const {
    return mInf1Ptr->ascent;
}

s32 JUTResFont::getDescent() const {
    return mInf1Ptr->descent;
}

s32 JUTResFont::getHeight() const {
    return getAscent() + getDescent();
}
