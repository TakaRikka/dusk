#ifndef D_MENU_OPTION_H
#define D_MENU_OPTION_H

#include "d/d_msg_scrn_explain.h"

class dMq_HIO_c : public JORReflexible {
public:
    dMq_HIO_c();
    virtual ~dMq_HIO_c();

    virtual void genMessage(JORMContext*);

    s8 id;
};

class dDlst_MenuQuitExplain_c : public dDlst_base_c {
public:
    virtual void draw();
    virtual ~dDlst_MenuQuitExplain_c() {}

    /* 0x4 */ dMsgScrnExplain_c* mpExplain;
};

class dMenu_Quit_c {
public:
    dMenu_Quit_c();
    virtual ~dMenu_Quit_c();

    void select1_init();
    void select1_proc();
    void select2_init();
    void select2_proc();
    void messageChange();
    bool YesNoSelect();
    void msgTxtSet(u16, bool);

    /* 0x04 */ STControl* mStick;
    /* 0x08 */ dDlst_MenuQuitExplain_c mMenuQuitExplain;
    /* 0x10 */ u8 field_0x10;
    /* 0x11 */ u8 field_0x11;
    /* 0x12 */ u8 field_0x12;
    /* 0x13 */ u8 field_0x13;
    /* 0x14 */ u8 field_0x14;
    /* 0x18 */ dMsgScrnExplain_c* mpScrnExplain;
    /* 0x1C */ u8 field_0x1c;
    /* 0x1E */ u16 mMsgId;
    /* 0x20 */ bool field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 field_0x22;
};

#endif /* D_MENU_OPTION_H */
