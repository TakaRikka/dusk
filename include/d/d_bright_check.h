#ifndef D_D_BRIGHT_CHECK_H
#define D_D_BRIGHT_CHECK_H

#include "d/d_com_inf_game.h"

class dMsgString_c;

class dDlst_BrightCheck_c : public dDlst_base_c {
public:
    virtual void draw();
    virtual ~dDlst_BrightCheck_c() {}

    /* 0x04 */ J2DScreen* Scr;
    /* 0x08 */ u8 field_0x8[0xC - 0x8];
    /* 0x0C */ dMsgString_c* mMsgString;
};

class dBrightCheck_c {
public:
    enum Mode_e {
        MODE_WAIT_e,
        MODE_MOVE_e,
    };

    dBrightCheck_c(JKRArchive*);
    virtual ~dBrightCheck_c();

    void screenSet();
    void _move();
    void modeWait();
    void modeMove();
    void _draw();

    /* 0x04 */ JKRArchive* mArchive;
    /* 0x08 */ dDlst_BrightCheck_c mBrightCheck;
    /* 0x18 */ u8 mMode;
    /* 0x19 */ u8 mCompleteCheck;
};

#endif /* D_D_BRIGHT_CHECK_H */
