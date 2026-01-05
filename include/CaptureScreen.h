#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include "JSystem/JFramework/JFWDisplay.h"

class CProjectionDivider {
public:
    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ s32 field_0x18;
    /* 0x1C */ s32 field_0x1c;

    CProjectionDivider(s32, s32);
    virtual ~CProjectionDivider() {}
    f32 calcDivRatioH(s32) const;
    f32 calcDivRatioV(s32) const;
};

class CPerspDivider : public CProjectionDivider {
public:
    CPerspDivider(const Mtx44&, s32, s32);
    virtual ~CPerspDivider() {}
    void divide(Mtx44&, s32, s32) const;
};

class COrthoDivider : public CProjectionDivider {
public:
    COrthoDivider(const Mtx44&, s32, s32);
    virtual ~COrthoDivider() {}
    void divide(Mtx44&, s32, s32) const;
};

class CaptureScreen {
public:
    CaptureScreen(const JFWDisplay*);

    static int getAlignedWidthBytes(int);
    static int getBmpBufferSize(int, int);
    void makeBmpFromEfb(void*, int, int, int, int, int, int);
    void openBmp(int, int);
    void closeBmp();
    void saveBmp(void*, u32);

    /* 0x0 */ const JFWDisplay* mpDisplay;
    /* 0x4 */ int field_0x4;
};

#endif /* CAPTURESCREEN_H */
