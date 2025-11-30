#ifndef JUTPALETTE_H
#define JUTPALETTE_H

#include <dolphin/gx.h>

enum JUTTransparency { UNK0, UNK1 };

/**
* @ingroup jsystem-jutility
* 
*/
struct ResTLUT {
    u8 format;
    u8 transparency;
    u16 numColors;
};

/**
* @ingroup jsystem-jutility
* 
*/
class JUTPalette {
public:
    JUTPalette(_GXTlut p1, _GXTlutFmt p2, JUTTransparency p3, u16 p4, void* p5) {
        this->storeTLUT(p1, p2, p3, p4, p5);
    }

    JUTPalette(GXTlut tlutNo, ResTLUT* p_tlutRes) {
        storeTLUT(tlutNo, p_tlutRes);
    }

    void storeTLUT(_GXTlut, ResTLUT*);
    void storeTLUT(_GXTlut, _GXTlutFmt, JUTTransparency, u16, void*);
    bool load();

    u8 getTlutName() const { return mTlutName; }
    u8 getFormat() const { return mFormat; }
    u8 getTransparency() const { return mTransparency; }
    u16 getNumColors() const { return mNumColors; }
    ResTLUT* getColorTable() const { return mColorTable; }

private:
    /* 0x00 */ _GXTlutObj mTlutObj;
    /* 0x0C */ u8 mTlutName;
    /* 0x0D */ u8 mFormat;
    /* 0x10 */ ResTLUT* mColorTable;
    /* 0x14 */ u16 mNumColors;
    /* 0x16 */ u8 mTransparency;
};

#endif /* JUTPALETTE_H */
