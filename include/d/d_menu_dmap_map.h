#ifndef D_MENU_D_MENU_DMAP_MAP_H
#define D_MENU_D_MENU_DMAP_MAP_H

#include "d/d_menu_dmap.h"
#include <cmath.h>

class dMenu_DmapMap_c {
public:
    virtual ~dMenu_DmapMap_c() {}
    void _create(u16, u16, u16, u16, void*);
    void _delete();
    void setTexture(u16, u16, u16, u16);
    void setPos(int, int, f32, f32, f32, bool, f32);

    const renderingDmap_c* getRendPointer(int i_no) const { return &mRend[i_no]; }
    ResTIMG* getResTIMGPointer(int i_no) const { return mResTIMG[i_no]; }

    /* 0x04 */ renderingDmap_c mRend[2];
    /* 0x74 */ ResTIMG* mResTIMG[2];
    /* 0x7C */ u8* mMapImage_p[2];
    /* 0x84 */ int field_0x84;
};

class dMenu_StageMapCtrl_c : public dMenu_DmapMap_c {
public:
    dMenu_StageMapCtrl_c() {
        field_0x8c = NULL;
        field_0x90 = 0.0f;
        field_0x94 = 0.0f;
        field_0x98 = 0.0f;
        field_0x9c = 0.0f;
        field_0xa0 = 0.0f;
        field_0xa4 = 0.0f;
        field_0xa8 = 0.0f;
        field_0xac = 0.0f;
        field_0xb0 = 0.0f;
        field_0xb4 = 0.0f;
        field_0xb8 = 0.0f;
        field_0xbc = 0.0f;
        field_0xc0 = 0.0f;
        field_0xc4 = 0.0f;
        field_0xc8 = 0.0f;
        field_0xcc = 0.0f;
        field_0xd0 = 0.0f;
        field_0xd4 = 0.0f;
        field_0xd8 = 0.0f;
        field_0xe4 = 0;
        field_0xe6 = 0;
        field_0xe7 = 0;
        field_0xe8 = 0;
        field_0xe9 = 0;
        field_0xea = 0;
        field_0xeb = 0;
        field_0xec = 0;
        field_0xed = 0;
        field_0xee = 0;
        field_0xef = 0;
        field_0xf0 = 0;
        field_0xf1 = 0;
        field_0xf2 = 0;
        field_0xf3 = 0;
        field_0xf4 = 0;
        field_0xf5 = 0;
        field_0xf6 = 0;
        field_0xf7 = 0;
        field_0xf8 = false;
        field_0xf9 = false;
    }

    virtual ~dMenu_StageMapCtrl_c() {}
    virtual bool isEnableZoomMove() const = 0;
    virtual void calcZoomCenter(f32*, f32*) = 0;
    virtual void getInitWholeMapScale(f32*, f32, f32, f32, f32) = 0;
    virtual void getInitDispCenter(f32*, f32*) = 0;
    virtual void getZoomMinMaxCheck(f32*, f32*, f32*, f32*, bool*, bool*) = 0;
    virtual f32 getZoomCmPerPixel() = 0;
    void draw() {}

    f32 getMapBlendPer() const;
    f32 getPixelStageSizeX() const;
    f32 getPixelStageSizeZ() const;
    f32 getPixelCenterX() const;
    f32 getPixelCenterZ() const;
    void initGetTreasureList(u8, s8);
    bool getTreasureList(f32*, f32*, s8*, u8*, s8*);
    void cnvPosTo2Dpos(f32, f32, f32*, f32*) const;
    void getPlayerDrawInfo(f32*, f32*, s16*) const;
    s8 getRestartDrawInfo(f32*, f32*, s16*) const;
    void setPlusNowStayFloorNo(s8, u8);
    void moveFloor();
    void wait_init_proc();
    void wait_proc();
    void zoomIn_init_proc();
    void initZoomIn(u8);
    void initZoomIn(u8, f32, f32);
    void initZoomInCenterHold(u8);
    void zoomCalcSet(f32);
    void zoomIn_proc();
    void initZoomOut(u8);
    void initZoomWait(f32, f32);
    void zoomWait_init_proc();
    void zoomWait_proc();
    void zoomOut_init_proc();
    void zoomOut_proc();
    void move();
    s8 getPlayerStayFloorNo() const;
    void _create(u16, u16, u16, u16, void*);
    void _create(u16, u16, u16, u16, s8, void*);
    void _delete();
    bool isEnableZoomIn();
    bool isEnableZoomOut();
    void setPlusZoomCenterX(f32);
    void setPlusZoomCenterZ(f32);

    u8 getDisableZoomMoveFlgX() const { return field_0xf8; }
    u8 getDisableZoomMoveFlgZ() const { return field_0xf9; }

    s8 getNowStayFloorNo() const { return field_0xe7; }
    s8 getTopFloorNo() const { return field_0xed; }
    s8 getBottomFloorNo() const { return field_0xee; }
    s8 getDispFloorNo() const { return field_0xeb; }
    s8 getDispFloor2No() const { return field_0xec; }

    f32 getDispCenterX() const { return field_0x9c; }
    f32 getDispCenterZ() const { return field_0xa0; }

    f32 getStageMapSizeX() const { return dMpath_c::getSizeX(); }
    f32 getStageMapSizeZ() const { return dMpath_c::getSizeZ(); }

    f32 getZoomCenterMaxX() { return field_0xcc; }
    f32 getZoomCenterMinX() { return field_0xc8; }
    f32 getZoomCenterMaxZ() { return field_0xd4; }
    f32 getZoomCenterMinZ() { return field_0xd0; }

    f32 getPixelPerCm() const { return 1.0f / field_0xbc; }

    f32 makeStayFloorBlendPer(int param_0, f32 param_1) {
        return param_0 + param_1;
    }

    int getFloorNo(f32 param_0) const {
        return std::ceil(param_0);
    }

    f32 getBlendPer(f32 param_0) const {
        return std::fmod(param_0, 1.0f);
    }

    bool isEndZoomIn() const { return field_0xf5 == 2; }
    bool isEndZoomOut() const { return field_0xf5 == 0; }

    ResTIMG* getResTIMGPointer(int i_no) const { return dMenu_DmapMap_c::getResTIMGPointer(i_no); }

    static f32 m_zoomCenterMinX;
    static f32 m_zoomCenterMaxX;
    static f32 m_zoomCenterMinZ;
    static f32 m_zoomCenterMaxZ;

    /* 0x88 */ u8 field_0x88[0x8C - 0x88];
    /* 0x8C */ dTres_c::typeGroupData_c* field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ f32 field_0x9c;
    /* 0xA0 */ f32 field_0xa0;
    /* 0xA4 */ f32 field_0xa4;
    /* 0xA8 */ f32 field_0xa8;
    /* 0xAC */ f32 field_0xac;
    /* 0xB0 */ f32 field_0xb0;
    /* 0xB4 */ f32 field_0xb4;
    /* 0xB8 */ f32 field_0xb8;
    /* 0xBC */ f32 field_0xbc;
    /* 0xC0 */ f32 field_0xc0;
    /* 0xC4 */ f32 field_0xc4;
    /* 0xC8 */ f32 field_0xc8;
    /* 0xCC */ f32 field_0xcc;
    /* 0xD0 */ f32 field_0xd0;
    /* 0xD4 */ f32 field_0xd4;
    /* 0xD8 */ f32 field_0xd8;
    /* 0xDC */ f32 field_0xdc;
    /* 0xE0 */ f32 field_0xe0;
    /* 0xE4 */ u16 field_0xe4;
    /* 0xE6 */ s8 field_0xe6;
    /* 0xE7 */ s8 field_0xe7;
    /* 0xE8 */ s8 field_0xe8;
    /* 0xE9 */ u8 field_0xe9;
    /* 0xEA */ s8 field_0xea;
    /* 0xEB */ s8 field_0xeb;
    /* 0xEC */ s8 field_0xec;
    /* 0xED */ s8 field_0xed;
    /* 0xEE */ s8 field_0xee;
    /* 0xEF */ u8 field_0xef;
    /* 0xF0 */ u8 field_0xf0;
    /* 0xF1 */ u8 field_0xf1;
    /* 0xF2 */ u8 field_0xf2;
    /* 0xF3 */ u8 field_0xf3;
    /* 0xF4 */ s8 field_0xf4;
    /* 0xF5 */ u8 field_0xf5;
    /* 0xF6 */ u8 field_0xf6;
    /* 0xF7 */ s8 field_0xf7;
    /* 0xF8 */ bool field_0xf8;
    /* 0xF9 */ bool field_0xf9;
};

class dMenu_DmapMapCtrl_c : public dMenu_StageMapCtrl_c {
public:
    virtual bool isEnableZoomMove() const;
    virtual void calcZoomCenter(f32*, f32*);
    virtual void getInitWholeMapScale(f32*, f32, f32, f32, f32);
    virtual void getInitDispCenter(f32*, f32*);
    virtual void getZoomMinMaxCheck(f32*, f32*, f32*, f32*, bool*, bool*);
    virtual f32 getZoomCmPerPixel();
    virtual void draw();
};

#endif /* D_MENU_D_MENU_DMAP_MAP_H */
