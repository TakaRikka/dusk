#ifndef D_METER_D_METER_STRING_H
#define D_METER_D_METER_STRING_H

#include "d/d_meter2.h"

class dMeterString_c : public dMeterSub_c {
public:
    dMeterString_c(int);
    int createString(int);
    void playBckAnimation(f32);
    void drawPikari();
    bool isLeadByte(int);

    virtual void draw();
    virtual ~dMeterString_c();
    virtual int _create();
    virtual int _execute(u32);
    virtual int _delete();

    f32 acc(s16 param_0, s16 param_1, s16 param_2) {
        return ((f32)(param_1 - param_2) * (f32)(param_1 - param_2)) / ((f32)(param_0 - param_2) * (f32)(param_0 - param_2));
    }

    /* 0x04 */ J2DScreen* mpScreen;
    /* 0x08 */ J2DAnmTransformKey* mpGetInBck;
    /* 0x0C */ CPaneMgr* mpParentPane;
    /* 0x10 */ CPaneMgr* mpRootPane;
    /* 0x14 */ CPaneMgr* mpTextPane;
    /* 0x18 */ JKRArchive* mpMapArchive;
    /* 0x1C */ u8 field_0x1C[0x24 - 0x1C];
    /* 0x24 */ int mStringID;
    /* 0x28 */ u8 field_0x28;
    /* 0x2C */ f32 mAnimFrame;
    /* 0x30 */ f32 mPikariAnimFrame;
    /* 0x34 */ int mStringID2;
    /* 0x38 */ s16 mOffsetX;
    /* 0x3A */ s16 mOffsetY;
    /* 0x3C */ u8 field_0x3c;
};

#endif /* D_METER_D_METER_STRING_H */
