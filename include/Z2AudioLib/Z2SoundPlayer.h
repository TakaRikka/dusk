#ifndef Z2SOUNDPLAYER_H
#define Z2SOUNDPLAYER_H

#include "JSystem/JAWExtSystem/JAWWindow.h"
#include "JSystem/JAudio2/JAISoundHandles.h"

class Z2SoundPlayer : public JAWWindow {
public:
    Z2SoundPlayer(const char*);
    virtual ~Z2SoundPlayer() {}
    virtual void onDraw(JAWGraphContext*);
    virtual void frameWork();
    virtual void onPadProc(const JUTGamePad&);
    virtual void onTrigA(const JUTGamePad&);
    virtual void onTrigB(const JUTGamePad&);
    virtual void onTrigX(const JUTGamePad&);
    virtual void onTrigY(const JUTGamePad&);
    virtual void onTrigL(const JUTGamePad&);
    virtual void onTrigZ(const JUTGamePad&);
    virtual void onKeyMenu(const JUTGamePad&);
    virtual void onKeyUp(const JUTGamePad&);
    virtual void onKeyDown(const JUTGamePad&);
    virtual void onKeyLeft(const JUTGamePad&);
    virtual void onKeyRight(const JUTGamePad&);

    u32 getCursorMoveMax(const JUTGamePad&);

    /* 0x3ED */ u8 field_0x3ed;
    /* 0x3EE */ u8 field_0x3ee;
    /* 0x3EF */ u8 field_0x3ef;
    /* 0x3F0 */ u8 field_0x3f0;
    /* 0x3F4 */ const char* field_0x3f4;
    /* 0x3F8 */ short field_0x3f8;
    /* 0x3FA */ short field_0x3fa;
    /* 0x3FC */ short field_0x3fc;
    /* 0x3FE */ short field_0x3fe;
    /* 0x400 */ short field_0x400;
    /* 0x402 */ short field_0x402;
    /* 0x404 */ const char* field_0x404;
    /* 0x408 */ const char* field_0x408;
    /* 0x40C */ const char* field_0x40c;
    /* 0x410 */ const char* field_0x410;
    /* 0x414 */ const char* field_0x414;
    /* 0x418 */ const char* field_0x418;
    /* 0x41C */ const char* field_0x41c;
    /* 0x420 */ const char* field_0x420;
    /* 0x424 */ const char* field_0x424;
    /* 0x428 */ const char* field_0x428;
    /* 0x42C */ const char* field_0x42c;
    /* 0x430 */ const char* field_0x430;
    /* 0x434 */ const char* field_0x434;
    /* 0x438 */ const char* field_0x438;
    /* 0x43C */ const char* field_0x43c;
    /* 0x440 */ const char* field_0x440;
    /* 0x444 */ short field_0x444;
    /* 0x446 */ u8 field_0x446;
    /* 0x448 */ short* field_0x448[7];
    /* 0x464 */ int field_0x464;
    /* 0x468 */ int field_0x468;
    /* 0x46C */ JAISoundHandle field_0x46c[8];
    /* 0x48C */ JAISoundHandles field_0x48c;
    /* 0x494 */ JAISoundHandle field_0x494;
    /* 0x498 */ JAISoundHandle* field_0x498;
    /* 0x49C */ JAISoundHandle field_0x49c;
    /* 0x4A0 */ int field_0x4a0;
    /* 0x4A4 */ f32 field_0x4a4;
    /* 0x4A8 */ f32 field_0x4a8;
    /* 0x4AC */ f32 field_0x4ac;
    /* 0x4B0 */ f32 field_0x4b0;
    /* 0x4B4 */ f32 field_0x4b4;
    /* 0x4B8 */ f32 field_0x4b8;
};

#endif /* Z2SOUNDPLAYER_H */
