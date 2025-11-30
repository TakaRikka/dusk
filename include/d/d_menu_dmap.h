#ifndef D_MENU_D_MENU_DMAP_H
#define D_MENU_D_MENU_DMAP_H

#include "d/d_select_cursor.h"
#include "d/d_msg_flow.h"
#include "d/d_menu_map_common.h"
#include "d/d_map_path_dmap.h"
#include "dolphin/types.h"

class CPaneMgr;
struct CSTControl;
class J2DOrthoGraph;
class J2DPicture;
class J2DScreen;
class JKRArchive;
class JKRExpHeap;
class STControl;
class mDoDvdThd_mountArchive_c;
class dMsgString_c;
class dMeterHaihai_c;
class dMenu_ItemExplain_c;
class J2DPictureEx;
struct dMenu_DmapMapCtrl_c;

class renderingDmap_c : public renderingPlusDoor_c {
public:
    virtual ~renderingDmap_c() {}

    virtual const GXColor* getColor(int);
    virtual int getLineWidth(int);
    virtual void beforeDrawPath();
    virtual void afterDrawPath();
    virtual bool hasMap() const;
    virtual bool isRendAllRoom() const;
    virtual bool isRendDoor() const;
    virtual bool isCheckFloor() const;
    virtual bool isDrawIconSingle2(dTres_c::data_s const*, bool, bool, int) const;
    virtual bool isRendRestart() const;
    virtual bool isRendCursor() const;
    virtual bool isRendIcon() const;
    virtual f32 getPlayerCursorSize();
    virtual f32 getRestartCursorSize();

    int getLineWidthZoomBig(int);
    int getLineWidthZoomSmall(int);

    void entry(f32 param_0, f32 param_1, f32 param_2, s8 param_3, s8 param_4, f32 param_5) {
        field_0x34 = param_5;
        renderingDAmap_c::entry(param_0, param_1, param_2, param_3, param_4);
    }

    /* 0x34 */ f32 field_0x34;
};

class dMenu_DmapBg_c : public dDlst_base_c, public dMenuMapCommon_c {
public:
    dMenu_DmapBg_c(JKRExpHeap*, STControl*);
    void mapScreenInit();
    void mapScreenAnime();
    void mapIconScaleSet(u8);
    void iconScaleAnmInit(f32, f32, u8);
    bool iconScaleAnm();
    void buttonIconScreenInit();
    void setAButtonString(u32);
    void setBButtonString(u32);
    void setCButtonString(u32);
    void setJButtonString(u32);
    void createExplain();
    void deleteExplain();
    void baseScreenInit();
    void setFloorMessage();
    bool dpdMove(s8, s8, s8, u8*, u8);
    ~dMenu_DmapBg_c();
    void setAllAlphaRate(f32, bool);
    void setGoldAnimation(bool);
    void setGoldFrameAlphaRate(f32);
    void addGoldFrameAlphaRate();
    void decGoldFrameAlphaRate();
    void draw();
    void update();
    void calcCursor();
    void drawCursor();

    void setDPDFloorSelCurPos(s8 i_pos) { field_0xdd6 = i_pos; }

    f32 getMapWidth() { return mMapWidth; }
    f32 getMapHeight() { return mMapHeight; }
    J2DPicture* getMapPane() { return mMapPane; }
    dMenu_ItemExplain_c* getItemExplainPtr() { return mpItemExplain; }

    void setAllTrans(f32 param_0, f32 param_1) {
        field_0xd94 = param_0;
        field_0xd98 = param_1;
    }

    /* 0xC98 */ JKRExpHeap* mpHeap;
    /* 0xC9C */ JKRExpHeap* mpTalkHeap;
    /* 0xCA0 */ STControl* mpStick;
    /* 0xCA4 */ J2DScreen* mBaseScreen;
    /* 0xCA8 */ J2DScreen* mMapScreen[2];
    /* 0xCB0 */ int field_0xcb0;
    /* 0xCB4 */ J2DScreen* mButtonScreen;
    /* 0xCB8 */ J2DScreen* mDecorateScreen;
    /* 0xCBC */ J2DScreen* mFloorScreen;
    /* 0xCC0 */ dSelect_cursor_c* mpDrawCursor;
    /* 0xCC4 */ J2DPicture* mMapPane;
    /* 0xCC8 */ dMsgString_c* mString;
    /* 0xCCC */ CPaneMgrAlphaMorf* mpBaseRoot;
    /* 0xCD0 */ CPaneMgrAlphaMorf* mpMapRoot[2];
    /* 0xCD8 */ CPaneMgrAlphaMorf* mpButtonRoot;
    /* 0xCDC */ CPaneMgrAlphaMorf* mpDecorateRoot;
    /* 0xCE0 */ CPaneMgrAlphaMorf* mpFloorRoot;
    /* 0xCE4 */ CPaneMgrAlpha* mpBlack;
    /* 0xCE8 */ CPaneMgrAlpha* mpCButton;
    /* 0xCEC */ CPaneMgrAlpha* mpJButton;
    /* 0xCF0 */ int field_0xcf0;
    /* 0xCF4 */ CPaneMgr* field_0xcf4;
    /* 0xCF8 */ CPaneMgr* field_0xcf8;
    /* 0xCFC */ u8 field_0xcfc[0xd00 - 0xcfc];
    /* 0xD00 */ CPaneMgr* mpMapSpace[2];
    /* 0xD08 */ CPaneMgr* mpButtonAB[2];
    /* 0xD10 */ CPaneMgr* mpButtonText[2];
    /* 0xD18 */ dMenu_ItemExplain_c* mpItemExplain;
    /* 0xD1C */ mDoDvdThd_mountArchive_c* mpArchiveMount;
    /* 0xD20 */ JKRArchive* mpArchive;
    /* 0xD24 */ J2DPicture* mpBackTexture;
    /* 0xD28 */ J2DAnmTextureSRTKey* field_0xd28[1];
    /* 0xD2C */ u8 field_0xd2c[0xd30 - 0xd2c];
    /* 0xD30 */ dMeterHaihai_c* mpMeterHaihai;
    /* 0xD34 */ dMsgFlow_c mMsgFlow;
    /* 0xD80 */ f32 field_0xd80;
    /* 0xD84 */ f32 field_0xd84;
    /* 0xD88 */ f32 field_0xd88;
    /* 0xD8C */ u8 field_0xd8c;
    /* 0xD8D */ u8 field_0xd8d;
    /* 0xD8E */ u8 field_0xd8e;
    /* 0xD8F */ u8 field_0xd8f;
    /* 0xD90 */ u8 field_0xd90;
    /* 0xD91 */ u8 field_0xd91;
    /* 0xD92 */ u8 field_0xd92;
    /* 0xD93 */ u8 field_0xd93;
    /* 0xD94 */ f32 field_0xd94;
    /* 0xD98 */ f32 field_0xd98;
    /* 0xD9C */ f32 field_0xd9c;
    /* 0xDA0 */ f32 mMapWidth;
    /* 0xDA4 */ f32 mMapHeight;
    /* 0xDA8 */ f32 field_0xda8;
    /* 0xDAC */ f32 field_0xdac;
    /* 0xDB0 */ f32 field_0xdb0;
    /* 0xDB4 */ f32 field_0xdb4;
    /* 0xDB8 */ f32 field_0xdb8;
    /* 0xDBC */ f32 field_0xdbc;
    /* 0xDC0 */ f32 field_0xdc0;
    /* 0xDC4 */ f32 field_0xdc4;
    /* 0xDC8 */ f32 field_0xdc8[1];
    /* 0xDCC */ f32 field_0xdcc;
    /* 0xDD0 */ u8 field_0xdd0;
    /* 0xDD1 */ u8 field_0xdd1;
    /* 0xDD2 */ u8 field_0xdd2;
    /* 0xDD3 */ u8 field_0xdd3;
    /* 0xDD4 */ s8 field_0xdd4;
    /* 0xDD5 */ u8 field_0xdd5;
    /* 0xDD6 */ s8 field_0xdd6;
    /* 0xDD7 */ u8 field_0xdd7;
    /* 0xDD8 */ u8 field_0xdd8;
    /* 0xDD9 */ u8 field_0xdd9;
    /* 0xDDA */ u8 field_0xdda;
};

class dMenu_Dmap_c {
public:
    dMenu_Dmap_c(JKRExpHeap*, STControl*, CSTControl*, u8, u8);
    void screenInit();
    void getPlayerIconPos(s8, f32);
    void getIconPos(s8, f32);
    s8 getCurFloorPos();
    s8 getDefaultCurFloorPos();
    void iconMoveCalc();
    void drawFloorScreenBack(J2DScreen*, f32, f32, J2DOrthoGraph*);
    void drawFloorScreenTop(J2DScreen*, f32, f32, J2DOrthoGraph*);
    bool isMapMoveState();
    void floorChangeMode();
    void _create();
    void _move();
    void setMapTexture();
    void mapBgAnime();
    void mapControl();
    bool isOpen();
    bool isClose();
    void _draw();
    void itemInfo_init_proc();
    void itemInfo_proc();
    void itemSelect();
    int getNextItem(int);
    void itemSelectAnmInit();
    void itemSelectAnm();
    void itemInfoOpenAnm();
    void itemInfoWait();
    void itemInfoCloseAnm();
    u8 getNextStatus();
    bool isSync();
    bool isKeyCheck();
    void infoModeChange_init_proc();
    void infoModeChange_proc();
    void mapModeChange_init_proc();
    void mapModeChange_proc();
    void lv5_talk_init_proc();
    void lv5_talk_proc();
    void mapMode_init_proc();
    void mapMode_proc();
    void floorSelect_init_proc();
    void floorSelect_proc();
    int itemCarryCheck();
    void floorChange_init_proc();
    void floorChange_proc();
    void zoomWait_init_proc();
    void zoomWait_proc();
    void zoomIn_init_proc();
    void zoomIn_proc();
    void zoomOut_init_proc();
    void zoomOut_proc();

    virtual ~dMenu_Dmap_c();

    void setInOutDir(u8 dir) { mInOutDir = dir; }
    u8 getZoomState() { return mZoomState; }
    u16 getCMessasgeNum() { return mCMessageNum; }
    s8 getBottomFloor() { return mBottomFloor; }
    s8 getFloorAll() { return (mFloorAll - mBottomFloor) + 1; }
    s8 getFloorPos(s8 param_0) { return param_0 - mBottomFloor; }
    u16 getCMessageNum() { return mCMessageNum; }

    static dMenu_Dmap_c* myclass;

private:
    /* 0x004 */ dMenu_DmapMapCtrl_c* mMapCtrl;
    /* 0x008 */ dMenu_DmapBg_c* mpDrawBg;
    /* 0x00C */ u8 field_0xc[0x10 - 0xC];
    /* 0x010 */ CPaneMgr* field_0x10;
    /* 0x014 */ CPaneMgr* mSelFloor[8];
    /* 0x034 */ CPaneMgr* mIconLinkPos[8];
    /* 0x054 */ CPaneMgr* mIconBossPos[8];
    /* 0x074 */ CPaneMgr* mStayIcon[2];
    /* 0x07C */ CPaneMgr* field_0x7c[3];
    /* 0x088 */ CPaneMgr* field_0x88[3];
    /* 0x094 */ CPaneMgr* field_0x94;
    /* 0x098 */ CPaneMgr* field_0x98;
    /* 0x09C */ u8 field_0x9c[0x44];
    /* 0x0E0 */ mDoDvdThd_mountArchive_c* mpDresArchiveMount;
    /* 0x0E4 */ JKRArchive* mpDresArchive;
    /* 0x0E8 */ JKRExpHeap* field_0xe8;
    /* 0x0EC */ JKRExpHeap* mDmapHeap;
    /* 0x0F0 */ STControl* mSelStick;
    /* 0x0F4 */ STControl* mpStick;
    /* 0x0F8 */ CSTControl* mpCStick;
    /* 0x0FC */ u8* mItemTexBuf;
    /* 0x100 */ void* mpBinData;
    /* 0x104 */ f32 field_0x104;
    /* 0x108 */ f32 field_0x108;
    /* 0x10C */ f32 field_0x10c;
    /* 0x110 */ f32 field_0x110;
    /* 0x114 */ f32 field_0x114[8];
    /* 0x134 */ f32 field_0x134;
    /* 0x138 */ f32 field_0x138;
    /* 0x13C */ f32 field_0x13c;
    /* 0x140 */ f32 field_0x140;
    /* 0x144 */ f32 field_0x144;
    /* 0x148 */ f32 field_0x148;
    /* 0x14C */ Vec field_0x14c[2];
    /* 0x164 */ s16 field_0x164;
    /* 0x166 */ u16 mCMessageNum;
    /* 0x168 */ u16 mJMessageNum;
    /* 0x16A */ u16 field_0x16a;
    /* 0x16C */ s8 field_0x16c;
    /* 0x16D */ s8 field_0x16d;
    /* 0x16E */ s8 field_0x16e;
    /* 0x16F */ u8 field_0x16f;
    /* 0x170 */ s8 mFloorAll;
    /* 0x171 */ s8 mBottomFloor;
    /* 0x172 */ s8 field_0x172;
    /* 0x173 */ s8 field_0x173;
    /* 0x174 */ u8 field_0x174[3];
    /* 0x177 */ u8 field_0x177;
    /* 0x178 */ u8 field_0x178;
    /* 0x179 */ u8 field_0x179;
    /* 0x17A */ u8 mInOutDir;
    /* 0x17B */ u8 m_process;
    /* 0x17C */ u8 m_itemSubProcess;
    /* 0x17D */ u8 field_0x17d;
    /* 0x17E */ u8 field_0x17e;
    /* 0x17F */ u8 mZoomState;
    /* 0x180 */ u8 field_0x180;
    /* 0x181 */ u8 field_0x181;
    /* 0x182 */ u8 field_0x182;
    /* 0x183 */ u8 field_0x183;
    /* 0x184 */ bool field_0x184;
    /* 0x185 */ bool field_0x185;
};

#endif /* D_MENU_D_MENU_DMAP_H */
