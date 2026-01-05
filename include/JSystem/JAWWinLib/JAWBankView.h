#ifndef JAWBANKVIEW_H
#define JAWBANKVIEW_H

#include "JSystem/JAWExtSystem/JAWWindow.h"

class JAWBankView : public JAWWindow {
public:
    JAWBankView();

    /* 0x3F0 */ int field_0x3f0;
    /* 0x3F4 */ int field_0x3f4;
    /* 0x3F8 */ int field_0x3f8;
    /* 0x3FC */ int field_0x3fc;
    /* 0x400 */ u8 field_0x400;
    /* 0x404 */ int field_0x404;
    /* 0x408 */ f32 field_0x408;
    /* 0x40C */ u8 field_0x40c[4];
};

#endif /* JAWBANKVIEW_H */
