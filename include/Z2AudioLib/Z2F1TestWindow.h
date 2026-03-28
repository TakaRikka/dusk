#ifndef Z2F1TESTWINDOW_H
#define Z2F1TESTWINDOW_H

#include "JSystem/JAHostIO/JAHFrameNode.h"
#include "JSystem/JAHostIO/JAHUTableEdit.h"
#include "JSystem/JAWExtSystem/JAWWindow.h"

class Z2F1TestWindow : public JAWWindow {
public:
    Z2F1TestWindow();

    virtual void onDraw(JAWGraphContext*);
    virtual void frameWork();
    virtual void onPadProc(const JUTGamePad&);
    virtual void onTrigY(const JUTGamePad&);
    virtual void onTrigMenu(const JUTGamePad&);

    void calcF1();
    f32 tau(f32);
    static f32 calcThreshold(int);
    f32 getRawX() const;
    f32 calcNoise();
    void setValue(f32, f32);
    f32 getValue2();
    f32 getValue();

    /* 0x3F0 */ f32 field_0x3f0;
    /* 0x3F4 */ f32 field_0x3f4;
    /* 0x3F8 */ f32 field_0x3f8[500];
    /* 0xBC8 */ int field_0xbc8;
    /* 0xBCC */ int field_0xbcc;
    /* 0xBD0 */ u8 field_0xbd0;
    /* 0xBD4 */ f32 field_0xbd4;
};

class Z2F1SwingTestNode : public JAHFrameNode, public Z2F1TestWindow {
public:
    Z2F1SwingTestNode();

    virtual void message(JAHControl&);
    virtual void onFrame();

    /* 0xC48 */ JAHUSeBox field_0xc48;
    /* 0xCD0 */ JAHUSeBox field_0xcd0;
    /* 0xD58 */ u8 mModulateVolume;
    /* 0xD59 */ u8 mModulatePitch;
    /* 0xD5A */ u8 mModulatePan;
    /* 0xD5B */ u8 mModulateDolby;
    /* 0xD5C */ f32 field_0xd5c;
    /* 0xD60 */ f32 field_0xd60;
    /* 0xD64 */ f32 field_0xd64;
    /* 0xD68 */ f32 field_0xd68;
    /* 0xD6C */ u8 field_0xd6c[0xD70 - 0xD6C];
};

class Z2AudSettingView : public JAWWindow {
public:
    Z2AudSettingView();

    virtual void onDraw(JAWGraphContext*);
};

class Z2DSPCheck : public JAWWindow {
public:
    Z2DSPCheck() : JAWWindow("Z2DSPCheck", 500, 450) {}

    virtual void onDraw(JAWGraphContext*);
};

#endif /* Z2F1TESTWINDOW_H */
