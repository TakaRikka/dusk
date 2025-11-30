#ifndef D_METER_D_METER_HAKUSHA_H
#define D_METER_D_METER_HAKUSHA_H

#include "d/d_meter2.h"

class dMeterHakusha_c : public dMeterSub_c {
public:
    struct hakusha_data {
        f32 pos_x;
        f32 pos_y;
        u8 flags;
    };

    dMeterHakusha_c(void*);
    void alphaAnimeHakusha(u32);
    void updateHakusha();
    void setAlphaHakushaAnimeMin();
    void setAlphaHakushaAnimeMax();
    void setAlphaButtonAnimeMin();
    void setAlphaButtonAnimeMax();
    int getHakushaNum();

    virtual void draw();
    virtual ~dMeterHakusha_c();
    virtual int _create();
    virtual int _execute(u32);
    virtual int _delete();

    /* 0x004 */ J2DScreen* field_0x004;
    /* 0x008 */ J2DScreen* mpHakushaScreen;
    /* 0x00C */ J2DScreen* mpButtonScreen;
    /* 0x010 */ CPaneMgr* mpHakushaParent;
    /* 0x014 */ CPaneMgr* mpHakushaPos[6];
    /* 0x02C */ CPaneMgr* mpHakushaOn;
    /* 0x030 */ CPaneMgr* mpHakushaOff;
    /* 0x034 */ CPaneMgr* mpButtonA;
    /* 0x038 */ hakusha_data mHakushaData[12];
    /* 0x0C8 */ f32 mHakushaAnimFrame[12];
    /* 0x0F8 */ f32 mButtonAPosX;
    /* 0x0FC */ f32 mButtonAPosY;
    /* 0x100 */ f32 field_0x100;
    /* 0x104 */ f32 field_0x104;
    /* 0x108 */ s16 mHakushaNum;
    /* 0x10A */ u8 mHakushaStatus[12];
};

#endif /* D_METER_D_METER_HAKUSHA_H */
