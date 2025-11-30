#ifndef JUTCACHEFONT_H
#define JUTCACHEFONT_H

#include "JSystem/JUtility/JUTResFont.h"
#include "global.h"

class JKRAramBlock;

/**
* @ingroup jsystem-jutility
* 
*/
class JUTCacheFont : public JUTResFont {
public:
    struct TGlyphCacheInfo {
        /* 0x00 */ TGlyphCacheInfo* mPrev;
        /* 0x04 */ TGlyphCacheInfo* mNext;
        /* 0x08 */ u16 field_0x8;
        /* 0x0A */ u16 field_0xa;
        /* 0x0C */ u16 field_0xc;
        /* 0x0E */ u16 field_0xe;
        /* 0x10 */ u32 field_0x10;
        /* 0x14 */ u16 mTexFormat;
        /* 0x16 */ u16 field_0x16;
        /* 0x18 */ u16 field_0x18;
        /* 0x1A */ u16 mWidth;
        /* 0x1C */ u16 mHeight;
        /* 0x1E */ u16 field_0x1e;
    };

    struct TCachePage : TGlyphCacheInfo {
        /* 0x20 */ GXTexObj mTexObj;
        /* 0x40 */ u8 mImage[];
    };  // Size: 0x40

    enum EPagingType {
        PAGE_TYPE_0,
        PAGE_TYPE_1,
    };

    JUTCacheFont(ResFONT const*, u32, JKRHeap*);
    void deleteMemBlocks_CacheFont();
    void initialize_state();
    int getMemorySize(ResFONT const*, u16*, u32*, u16*, u32*, u16*, u32*, u32*);
    int initiate(ResFONT const*, void*, u32, JKRHeap*);
    bool internal_initiate(ResFONT const*, void*, u32, JKRHeap*);
    bool allocArea(void*, u32, JKRHeap*);
    bool allocArray(JKRHeap*);
    TGlyphCacheInfo* determineBlankPage();
    void getGlyphFromAram(JUTCacheFont::TGlyphCacheInfo*, JUTCacheFont::TCachePage*,
                                         int*, int*);
    TCachePage* loadCache_char_subroutine(int*, bool);
    void invalidiateAllCache();
    void unlink(JUTCacheFont::TGlyphCacheInfo*);
    void prepend(JUTCacheFont::TGlyphCacheInfo*);

    virtual ~JUTCacheFont();
    virtual void loadImage(int, _GXTexMapID);
    virtual void setBlock();

    void setPagingType(EPagingType type) { mPagingType = type; }

    static u32 calcCacheSize(u32 param_0, int param_1) { return (ALIGN_NEXT(param_0, 0x20) + 0x40) * param_1; }
    GXTexObj* getTexObj(void* buffer) { return &((TCachePage*)buffer)->mTexObj; }

private:
    /* 0x70 */ u32 mTotalWidSize;
    /* 0x74 */ u32 mTotalGlySize;
    /* 0x78 */ u32 mTotalMapSize;
    /* 0x7C */ void* field_0x7c;
    /* 0x80 */ void* field_0x80;
    /* 0x84 */ void* field_0x84;
    /* 0x88 */ u32 mMaxSheetSize;
    /* 0x8C */ EPagingType mPagingType;
    /* 0x90 */ void* mCacheBuffer;
    /* 0x94 */ int field_0x94;
    /* 0x98 */ u32 mCachePage;
    /* 0x9C */ TGlyphCacheInfo* field_0x9c;
    /* 0xA0 */ TGlyphCacheInfo* field_0xa0;
    /* 0xA4 */ TGlyphCacheInfo* field_0xa4;
    /* 0xA8 */ u32 field_0xa8;
    /* 0xAC */ JKRAramBlock* field_0xac;
    /* 0xB0 */ u8 field_0xb0;
    /* 0xB4 */ int field_0xb4;
};  // Size: 0xB8

#endif /* JUTCACHEFONT_H */
