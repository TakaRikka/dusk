#ifndef D_D_SELECT_ICON_H
#define D_D_SELECT_ICON_H

#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_drawlist.h"
#include "d/d_pane_class.h"

class dSi_HIO_c {
public:
    virtual ~dSi_HIO_c() {}
    dSi_HIO_c();

    u8 field_0x4[4];
};

class dSelect_icon_c {
public:
    void animation();
    void setAlpha(u8);
    void setPos(J2DPane*, f32, f32);
    virtual ~dSelect_icon_c(); // must be declared but not defined to match?

    void drawSelf() {
        field_0x4.draw();
    }

    /* 0x04 */ dDlst_base_c field_0x4;  // type is a guess
    /* 0x08 */ J2DScreen* field_0x8;
    /* 0x0C */ CPaneMgrAlpha* field_0xc;
    /* 0x10 */ CPaneMgr* field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ J2DAnmBase* field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ J2DAnmBase* field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
};

#endif /* D_D_SELECT_ICON_H */
