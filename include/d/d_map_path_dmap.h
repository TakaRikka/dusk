#ifndef D_MAP_D_MAP_PATH_DMAP_H
#define D_MAP_D_MAP_PATH_DMAP_H

#include "d/d_tresure.h"
#include "d/d_map_path.h"

class dStage_FileList2_dt_c;
struct stage_tgsc_data_class;

class dMpath_c {
public:
    static bool isExistMapPathData();
    static int getTopBottomFloorNo(s8*, s8*);
    static void createWork();
    static void setPointer(s8, void*, int);
    static int setPointer(dDrawPath_c::room_class*, s8*, s8*);
    static void create();
    static void reset();
    static void remove();

    static dDrawPath_c::room_class* getRoomPointer(int layerNo, int roomNo) {
        return mLayerList->mRooms[layerNo][roomNo];
    }

    static f32 getMinX() { return mMinX; }
    static f32 getMinZ() { return mMinZ; }
    static f32 getMaxX() { return mMaxX; }
    static f32 getMaxZ() { return mMaxZ; }

    static f32 getSizeX() { return mAllSizeX; }
    static f32 getSizeZ() { return mAllSizeZ; }

    static f32 getCenterX() { return mAllCenterX; }
    static f32 getCenterZ() { return mAllCenterZ; }

    static dDrawPath_c::layer_data* mLayerList;  // this doesn't seem right, but can't figure it out atm
    static f32 mMinX;
    static f32 mMaxX;
    static f32 mMinZ;
    static f32 mMaxZ;
    static f32 mAllCenterX;
    static f32 mAllCenterZ;
    static f32 mAllSizeX;
    static f32 mAllSizeZ;
    static s8 mBottomFloorNo;
    static s8 mTopFloorNo;
};

struct dMapInfo_n {
    static bool chkGetCompass();
    static bool chkGetMap();
    static bool isVisitedRoom(int);
    static void correctionOriginPos(s8, Vec*);
    static void offsetPlus(dStage_FileList2_dt_c const*, Vec*);
    static void rotAngle(dStage_FileList2_dt_c const*, Vec*);
    static Vec getMapPlayerPos();
    static s16 getMapPlayerAngleY();
    static const dTres_c::typeGroupData_c* getConstRestartIconPointer();
    static Vec getMapRestartPos();
    static s16 getMapRestartAngleY();
    static void getRoomCenter(int, f32*, f32*);
    static void getRoomMinMaxXZ(int, f32*, f32*, f32*, f32*);
    static void getFloorParameter(f32, s8*, f32*, f32*, f32*, f32*);
};

class dMapInfo_c {
public:
    static s8 calcFloorNo(f32, bool, int);
    static s8 calcNowStayFloorNo(f32, bool);
    static void move(int, f32);
    static void init();
    static void reset();
    static void create();
    static void remove();

    static s8 getNowStayFloorNo() {
        JUT_ASSERT(0, mNowStayFloorNoDecisionFlg);

        s8 floor_no = 0;
        if (mNowStayFloorNoDecisionFlg) {
            floor_no = mNowStayFloorNo;
        }
        return floor_no;
    }

    static u8 getNowStayFloorNoDecisionFlg() {
        return mNowStayFloorNoDecisionFlg;
    }

    static void setNextRoomNoForMapPat0(int i_roomNo) { mNextRoomNo = i_roomNo; }
    static int getNextRoomNoForMapPat0() { return mNextRoomNo; }

    static int mNextRoomNo;
    static int mNowStayRoomNo;
    static s8 mNowStayFloorNo;
    static u8 mNowStayFloorNoDecisionFlg;
};

class renderingDAmap_c : public dRenderingFDAmap_c {
public:
    renderingDAmap_c() {
        mRoomNo = 0;
        field_0x28 = 0;
        mRoomNoSingle = 0;
        mRenderedFloor = 0;
        mIsDraw = false;
    }

    s8 calcFloorNoForObjectByMapPathRend(f32, int) const;
    void init(u8*, u16, u16, u16, u16);
    void entry(f32, f32, f32, int, s8);
    void setSingleRoomSetting();
    int getFirstDrawRoomNo();
    int getNextDrawRoomNo(int);

    virtual void draw();
    virtual ~renderingDAmap_c() {}
    virtual bool isSwitch(dDrawPath_c::group_class const*);
    virtual bool isRenderingFloor(int);
    virtual dDrawPath_c::room_class* getFirstRoomPointer();
    virtual dDrawPath_c::room_class* getNextRoomPointer();
    virtual void preDrawPath();
    virtual void postDrawPath();
    virtual bool isDrawPath();
    virtual const GXColor* getBackColor() const;
    virtual int getFirstDrawLayerNo();
    virtual int getNextDrawLayerNo(int);

    virtual bool isDrawIconSingle(dTres_c::data_s const*, int, int, bool, bool,
                                                 Vec const*) const;
    virtual int getIconGroupNumber(u8) const;
    virtual bool hasMap() const = 0;
    virtual bool isRendAllRoom() const = 0;
    virtual bool isRendDoor() const = 0;
    virtual bool isCheckFloor() const = 0;
    virtual bool isDrawIconSingle2(dTres_c::data_s const*, bool, bool, int) const = 0;
    virtual int getRoomNoSingle();
    virtual bool isDrawRoom(int, int) const;
    virtual bool isDrawRoomIcon(int, int) const;

    bool isDraw() const { return mIsDraw; }

    /* 0x24 */ int mRoomNo;
    /* 0x28 */ int field_0x28;
    /* 0x2C */ int mRoomNoSingle;
    /* 0x30 */ s8 mRenderedFloor;
    /* 0x31 */ bool mIsDraw;
};  // Size: 0x34

class stage_tgsc_data_class;

class renderingPlusDoor_c : public renderingDAmap_c {
public:
    renderingPlusDoor_c() {}
    void drawDoor1();
    void drawDoor2();
    void drawDoorCommon(stage_tgsc_data_class const*, int, bool);
    bool checkDispDoorS(int, int, f32);
    void drawNormalDoorS(stage_tgsc_data_class const*, int, int, bool);

    virtual ~renderingPlusDoor_c() {}
    virtual void beforeDrawPath();
    virtual void afterDrawPath();
    virtual bool isRendRestart() const = 0;
    virtual bool isRendCursor() const = 0;
    virtual bool isRendIcon() const = 0;
};

class renderingPlusDoorAndCursor_c : public renderingPlusDoor_c {
public:
    void drawTreasure();
    void drawTreasureAfterPlayer();
    void drawIconSingle(Vec const&, f32, f32);
    void drawCursor(Vec const&, s16, int, f32);
    renderingPlusDoorAndCursor_c() {}

    virtual ~renderingPlusDoorAndCursor_c() {}
    virtual void afterDrawPath();
    virtual f32 getIconSize(u8) const = 0;
    virtual const Vec* getIconPosition(dTres_c::typeGroupData_c*) const;
    virtual dTres_c::typeGroupData_c* getFirstData(u8);
    virtual dTres_c::typeGroupData_c* getNextData(dTres_c::typeGroupData_c*);
    virtual f32 getPlayerCursorSize() = 0;
    virtual f32 getRestartCursorSize() = 0;
};

#endif /* D_MAP_D_MAP_PATH_DMAP_H */
