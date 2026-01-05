#ifndef JUTRESFONT_H
#define JUTRESFONT_H

#include "JSystem/JUtility/JUTFont.h"

class JKRHeap;

typedef bool (*IsLeadByte_func)(int);

/**
* @ingroup jsystem-jutility
* 
*/
struct BlockHeader {
    const BlockHeader* getNext() const { return reinterpret_cast<const BlockHeader*>(reinterpret_cast<const u8*>(this) + size); }
    u32 magic;
    u32 size;
};

/**
* @ingroup jsystem-jutility
* 
*/
class JUTResFont : public JUTFont {
public:
    virtual ~JUTResFont();
    virtual void setGX();
    virtual void setGX(JUtility::TColor, JUtility::TColor);
    virtual f32 drawChar_scale(f32, f32, f32, f32, int, bool);
    virtual int getLeading() const;
    virtual s32 getAscent() const;
    virtual s32 getDescent() const;
    virtual s32 getHeight() const;
    virtual s32 getWidth() const;
    virtual void getWidthEntry(int, JUTFont::TWidth*) const;
    virtual s32 getCellWidth() const;
    virtual s32 getCellHeight() const;
    virtual int getFontType() const;
    virtual ResFONT* getResFont() const;
    virtual bool isLeadByte(int) const;
    virtual void loadImage(int, _GXTexMapID);
    virtual void setBlock();

    JUTResFont(ResFONT const*, JKRHeap*);
    JUTResFont();
    void deleteMemBlocks_ResFont();
    void initialize_state();
    bool initiate(ResFONT const*, JKRHeap*);
    bool protected_initiate(ResFONT const*, JKRHeap*);
    void countBlock();
    void loadFont(int, _GXTexMapID, JUTFont::TWidth*);
    int getFontCode(int) const;
    int convertSjis(int, u16*) const;

    inline void delete_and_initialize() {
        deleteMemBlocks_ResFont();
        initialize_state();
    }

    static const int suAboutEncoding_ = 3;
    static IsLeadByte_func const saoAboutEncoding_[suAboutEncoding_];

    // some types uncertain, may need to be fixed
    /* 0x1C */ int mWidth;
    /* 0x20 */ int mHeight;
    /* 0x24 */ GXTexObj mTexObj;
    /* 0x44 */ int mTexPageIdx;
    /* 0x48 */ const ResFONT* mResFont;
    /* 0x4C */ ResFONT::INF1* mInf1Ptr;
    /* 0x50 */ void** mMemBlocks;
    /* 0x54 */ ResFONT::WID1** mpWidthBlocks;
    /* 0x58 */ ResFONT::GLY1** mpGlyphBlocks;
    /* 0x5C */ ResFONT::MAP1** mpMapBlocks;
    /* 0x60 */ u16 mWid1BlockNum;
    /* 0x62 */ u16 mGly1BlockNum;
    /* 0x64 */ u16 mMap1BlockNum;
    /* 0x66 */ u16 field_0x66;
    /* 0x68 */ u16 mMaxCode;
    /* 0x6C */ const IsLeadByte_func* mIsLeadByte;
};

extern u8 const JUTResFONT_Ascfont_fix12[];
extern u8 const JUTResFONT_Ascfont_fix16[];

#endif /* JUTRESFONT_H */
