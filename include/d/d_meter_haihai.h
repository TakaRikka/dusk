#ifndef D_METER_D_METER_HAIHAI_H
#define D_METER_D_METER_HAIHAI_H

#include "d/d_meter2.h"

class J2DScreen;
class CPaneMgr;
class J2DAnmTransformKey;
class J2DAnmTextureSRTKey;
class J2DAnmColor;

class dMeterHaihai_c : public dMeterSub_c {
public:
    enum {
        DIR_LEFT_e = (1 << 0),
        DIR_DOWN_e = (1 << 1),
        DIR_RIGHT_e = (1 << 2),
        DIR_UP_e = (1 << 3),
    };

    dMeterHaihai_c(u8);
    void drawHaihai(u8, f32, f32, f32, f32);
    void drawHaihai(u8);
    void drawHaihaiLeft(f32, f32);
    void drawHaihaiRight(f32, f32);
    void drawHaihaiTop(f32, f32);
    void drawHaihaiBottom(f32, f32);
    void setScale(f32);
    void alphaAnimeHaihai(u32);
    void updateHaihai();
    void playBckAnime(J2DAnmTransformKey*);
    void playBtkAnime(J2DAnmTextureSRTKey*);
    void playBpkAnime(J2DAnmColor*);

    virtual void draw();
    virtual ~dMeterHaihai_c();
    virtual int _create();
    virtual int _execute(u32);
    virtual int _delete();

    void onPlayAllAnime() { mPlayAnimFlags = 0xFF; }
    bool checkPlayAnime(int i_flag) { return mPlayAnimFlags & (1 << i_flag); }

    /* 0x04 */ J2DScreen* mpHaihaiScreen;
    /* 0x08 */ CPaneMgr* mpParent;
    /* 0x0C */ J2DAnmTransformKey* mpCursorBck;
    /* 0x10 */ J2DAnmTextureSRTKey* mpCursorBtk;
    /* 0x14 */ J2DAnmColor* mpCursorBpk;
    /* 0x18 */ f32 mBckFrame;
    /* 0x1C */ f32 mBtkFrame;
    /* 0x20 */ f32 mBpkFrame;
    /* 0x24 */ u32 mFlags;
    /* 0x28 */ u8 mType;
    /* 0x29 */ u8 mPlayAnimFlags;
};

#endif /* D_METER_D_METER_HAIHAI_H */
