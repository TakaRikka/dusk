#ifndef D_METER_D_METER_MAP_H
#define D_METER_D_METER_MAP_H

#include "d/d_map_path_dmap.h"

class J2DPicture;
class dMap_c;
class dMeterMap_c : public dDlst_base_c {
public:
    static bool isEnableDispMap();
    static int getMapDispSizeTypeNo();
    static bool isEnableDispMapAndMapDispSizeTypeNo();
    f32 getMapDispEdgeBottomY_Layout();
    bool isEventRunCheck();
    f32 getMapDispEdgeLeftX_Layout();
    f32 getMapDispEdgeTop();
    s16 getDispPosInside_OffsetX();
    s16 getDispPosOutSide_OffsetX();
    void setDispPosInsideFlg_SE_On();
    void setDispPosOutsideFlg_SE_On();
    void setMapAlpha(u8);
    static bool isMapOpenCheck();
    dMeterMap_c(J2DScreen*);
    void _create(J2DScreen*);
    void _delete();
    void _move(u32);
    void _draw();
    void ctrlShowMap();
    u8 checkMoveStatus();
    static bool isShow(u32);
    static bool isFmapScreen();
    static bool isDmapScreen();
    static void meter_map_move(u32);
    void keyCheck();

    virtual void draw();
    virtual ~dMeterMap_c();

    bool isDispPosInsideFlg() { return field_0x2d != 0; }

    enum { MAP_SIZE_TYPE_BIG = 2, MAP_SIZE_TYPE_SMALL = 3, MAP_SIZE_TYPE_STAGE = 7 };

private:
    /* 0x04 */ J2DPicture* mMapJ2DPicture;
    /* 0x08 */ dMap_c* mMap;
    /* 0x0C */ s32 mIsCompass;
    /* 0x10 */ s32 mIsMap;
    /* 0x14 */ u32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 mSizeW;
    /* 0x24 */ f32 mSizeH;
    /* 0x28 */ s16 field_0x28;
    /* 0x2A */ u8 field_0x2a;
    /* 0x2B */ u8 field_0x2b;
    /* 0x2C */ u8 mMapAlpha;
    /* 0x2D */ u8 field_0x2d;
    /* 0x2E */ u8 field_0x2e;
    /* 0x30 */ int field_0x30;
};

#endif /* D_METER_D_METER_MAP_H */
