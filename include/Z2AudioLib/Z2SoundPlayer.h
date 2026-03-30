#ifndef Z2SOUNDPLAYER_H
#define Z2SOUNDPLAYER_H

#include "JSystem/JAWExtSystem/JAWWindow.h"
#include "JSystem/JAudio2/JAISoundHandles.h"
#include "JSystem/JAudio2/JAUSoundTable.h"

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
    int getMenuNumberMax();
    void correctSeNumber();

    void onDrawSoundItem(JAWGraphContext*, JAUSoundNameTable*, int, const JUtility::TColor&, const JUtility::TColor&, const char*, u32, u32, u32);

    /* 0x3ED */ bool field_0x3ed;
    /* 0x3EE */ bool field_0x3ee;
    /* 0x3EF */ bool field_0x3ef;
    /* 0x3F0 */ bool field_0x3f0;
    /* 0x3F4 */ const char* m_name;
    /* 0x3F8 */ s16 field_0x3f8;
    /* 0x3FA */ s16 field_0x3fa;
    /* 0x3FC */ s16 field_0x3fc;
    /* 0x3FE */ s16 field_0x3fe;
    /* 0x400 */ s16 field_0x400;
    /* 0x402 */ s16 m_portNum;
    /* 0x404 */ const char* m_portNames[16];
    /* 0x444 */ s16 m_portVal;
    /* 0x446 */ u8 field_0x446;
    /* 0x448 */ s16* field_0x448[7];
    /* 0x464 */ u32 m_cursorY;
    /* 0x468 */ int m_cursorMax;
    /* 0x46C */ JAISoundHandle field_0x46c[8];
    /* 0x48C */ JAISoundHandles field_0x48c;
    /* 0x494 */ JAISoundHandle field_0x494;
    /* 0x498 */ JAISoundHandle* mp_subBgmHandle;
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
