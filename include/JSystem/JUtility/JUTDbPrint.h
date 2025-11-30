#ifndef JUTDBPRINT_H
#define JUTDBPRINT_H

#include "JSystem/JUtility/JUTFont.h"

class JKRHeap;

// TODO: fix struct
/**
* @ingroup jsystem-jutility
* 
*/
struct unk_print {
    /* 0x00 */ unk_print* mNext;
    /* 0x04 */ s16 unk_0x04;
    /* 0x06 */ s16 unk_0x06;
    /* 0x08 */ s16 unk_0x08;
    /* 0x0A */ s16 unk_0x0A;
    /* 0x0C */ char unk_0x0C[0];
};

/**
* @ingroup jsystem-jutility
* 
*/
class JUTDbPrint {
public:
    JUTDbPrint(JUTFont*, JKRHeap*);
    static JUTDbPrint* start(JUTFont*, JKRHeap*);
    JUTFont* changeFont(JUTFont*);
    void enter(int, int, int, char const*, int);
    void flush(int, int, int, int);
    void flush();
    void drawString(int, int, int, u8 const*);

    static JUTDbPrint* getManager() { return sDebugPrint; }

    void setVisible(bool visible) { mVisible = visible; }
    JUTFont* getFont() const { return mFont; }

    void setCharColor(JUtility::TColor color) { mColor = color; };

    static JUTDbPrint* sDebugPrint;

private:
    /* 0x00 */ unk_print* mFirst;
    /* 0x04 */ JUTFont* mFont;
    /* 0x08 */ JUtility::TColor mColor;
    /* 0x0C */ bool mVisible;
    /* 0x10 */ JKRHeap* mHeap;
};

#endif /* JUTDBPRINT_H */
