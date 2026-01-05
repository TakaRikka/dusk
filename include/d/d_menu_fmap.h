#ifndef D_MENU_D_MENU_FMAP_H
#define D_MENU_D_MENU_FMAP_H

#include "JSystem/JHostIO/JORReflexible.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/d_menu_map_common.h"
#include "d/d_msg_flow.h"

struct CSTControl;
class J2DAnmTextureSRTKey;
class JKRArchive;
class JKRAramArchive;
class JKRExpHeap;
class JKRHeap;
class STControl;
struct ResTIMG;
class dMenu_Fmap_stage_data_c;
class dMenu_Fmap_world_data_c;
class dMenu_Fmap_region_data_c;
class dMenu_Fmap2DBack_c;
class dMenu_Fmap2DTop_c;
class dMenu_FmapMap_c;
class mDoDvdThd_mountArchive_c;

// Unknown name
class dMenu_Fmap_field_data_c {
public:
    /* 0x00 */ u32 field_0x0;
    /* 0x04 */ u32 mStageDataOffset;
    /* 0x08 */ u32 mRegionDataOffset;
    /* 0x0C */ u32 mVirtualStageOffset;
    /* 0x10 */ u32 mRoomDataOffset;
};

// Unknown name
class dMenu_Fmap_field_region_data_c {
public:
    struct data {
        /* 0x00 */ u8 mTextureReadNum;
        /* 0x01 */ u8 field_0x1;
        /* 0x02 */ u16 mTitleName;
        /* 0x04 */ f32 mOriginX;
        /* 0x08 */ f32 mOriginZ;
        /* 0x0C */ f32 field_0xc;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ f32 field_0x18;
        /* 0x1C */ f32 field_0x1c;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
    };

    /* 0x0 */ u8 mCount;
    /* 0x4 */ data mData[0];
};

// Unknown name
class dMenu_Fmap_virtual_stage_data_c {
public:
    struct data {
        /* 0x0 */ char mStageName[8];
        /* 0x8 */ char mVirtualStageName[8];
    };

    /* 0x0 */ u8 mCount;
    /* 0x1 */ u8 pad[3];
    /* 0x4 */ data mData[0];
};

// Unknown name
class dMenu_Fmap_field_room_data_c {
public:
    struct data {
        /* 0x0 */ char mStageName[8];
        /* 0x8 */ u16 mAreaName;
        /* 0xA */ u8 mCount;
        /* 0xB */ u8 mRoomNos[0];
    };

    /* 0x0 */ u8 mCount;
    /* 0x1 */ u8 pad[3];
    /* 0x4 */ data mData[0];
};

// Unknown name
class dMenu_Fmap_portal_data_c {
public:
    struct data {
        /* 0x00 */ u8 mSelectWarpPt;
        /* 0x01 */ u8 mRegionNo;
        /* 0x02 */ const char mStageName[8];
        /* 0x0A */ u8 mRoomNo;
        /* 0x0B */ u8 mWarpPlayerNo;
        /* 0x0C */ u16 mMessageID;
        /* 0x0E */ u8 mStageNo;
        /* 0x0F */ u8 mSwitchNo;
        /* 0x10 */ cXyz mPosition;
    };

    /* 0x0 */ u8 field_0x0[8];
    /* 0x8 */ u8 mCount;
    /* 0xC */ data mData[0];
};

class dMenu_Fmap_c {
public:
    typedef void (dMenu_Fmap_c::*process)();
 
    dMenu_Fmap_c(JKRExpHeap*, STControl*, CSTControl*, u8, u8, u8, f32, f32, u8);
    void _create();
    void _delete();
    void _move();
    void _draw();
    u8 getNextStatus(u8*);
    bool isSync();
    void all_map_init();
    void all_map_proc();
    void zoom_all_to_region_init();
    void zoom_all_to_region_proc();
    void zoom_region_to_all_init();
    void zoom_region_to_all_proc();
    void region_map_init();
    void region_map_proc();
    void to_portal_warp_map_init();
    void to_portal_warp_map_proc();
    void portal_warp_map_init();
    void portal_warp_map_proc();
    void portal_warp_select_init();
    void portal_warp_select_proc();
    void portal_warp_forbid_init();
    void portal_warp_forbid_proc();
    void zoom_region_to_spot_init();
    void zoom_region_to_spot_proc();
    void zoom_spot_to_region_init();
    void zoom_spot_to_region_proc();
    void spot_map_init();
    void spot_map_proc();
    void portal_demo1_init();
    void portal_demo1_move();
    void portal_demo2_init();
    void portal_demo2_move();
    void portal_demo3_init();
    void portal_demo3_move();
    void portal_demo4_init();
    void portal_demo4_move();
    void portal_demo5_init();
    void portal_demo5_move();
    void portal_demo6_init();
    void portal_demo6_move();
    void yamiboss_demo1_init();
    void yamiboss_demo1_move();
    void yamiboss_demo2_init();
    void yamiboss_demo2_move();
    void yamiboss_demo3_init();
    void yamiboss_demo3_move();
    void yamiboss_demo4_init();
    void yamiboss_demo4_move();
    void yamiboss_demo5_init();
    void yamiboss_demo5_move();
    void light_demo1_init();
    void light_demo1_move();
    void light_demo2_init();
    void light_demo2_move();
    void table_demo1_init();
    void table_demo1_move();
    void table_demo2_init();
    void table_demo2_move();
    void table_demo3_init();
    void table_demo3_move();
    void howl_demo1_init();
    void howl_demo1_move();
    void howl_demo2_init();
    void howl_demo2_move();
    void howl_demo3_init();
    void howl_demo3_move();
    bool isOpen();
    bool isClose();
    u8 getProcess();
    u8 getRegionCursor();
    u8 getStageCursor();
    f32 getStageTransX();
    f32 getStageTransZ();
    bool isRoomCheck(int, int);
    u16 checkStRoomData();
    void talkButton();
    void setProcess(u8);
    void setFlash(u8, bool);
    bool readWorldData(u8);
    bool readAreaData(u8, bool);
    bool readRoomData(char const*, dMenu_Fmap_stage_data_c*, void*, int, u8);
    bool readFieldMapData(void**, char const*, bool, bool);
    void decodeFieldMapData();
    void decodePortalData();
    bool readRoomDzsData(void**, u32, char const*);
    bool removeAreaData();
    bool removeRoomData(dMenu_Fmap_stage_data_c*);
    void setTitleName(u32);
    void setAreaName(u32);
    void setAreaNameZero();
    void portalWarpMapMove(STControl*);
    void onRoomDataBit(int);
    BOOL isRoomDataBit(int);
    void resetRoomDataBit();
    void drawIcon(f32, bool);
    void drawIcon(u8, int);
    void drawLightDropIcon();
    void drawBatsumarkIcon();
    void drawSnowmanIcon();
    void drawGoldWolfIcon();
    void drawCoachIcon();
    void drawPlayEnterIcon();
    bool checkDrawPortalIcon(int, int);
    bool searchIcon(u8, int, f32*, f32*);
    void drawEnterIcon();
    void drawPortalIcon();
    u8 getRegionStageNum(int);
    dMenu_Fmap_region_data_c* getNowFmapRegionData();
    dMenu_Fmap_stage_data_c* getNowFmapStageData();
    u8 searchPortalStageID(char*);
    void drawDebugStage();
    void arrowPosInit();
    void tableArrowPosInit(bool);
    void yamibossArrowPosInit();
    void howlArrowPosInit();
    u8 getHowlRegionID();
    bool isLightVesselGet();
    cXyz* getPlayerPos2D();

    virtual ~dMenu_Fmap_c();

    enum process_e {
        /* 0x00 */ PROC_ALL_MAP,
        /* 0x01 */ PROC_PORTAL_DEMO2,
        /* 0x02 */ PROC_PORTAL_DEMO3,
        /* 0x03 */ PROC_TABLE_DEMO1,
        /* 0x04 */ PROC_ZOOM_ALL_TO_REGION,
        /* 0x05 */ PROC_ZOOM_REGION_TO_ALL,
        /* 0x06 */ PROC_REGION_MAP,
        /* 0x07 */ PROC_TO_PORTAL_WARP_MAP,
        /* 0x08 */ PROC_PORTAL_WARP_MAP,
        /* 0x09 */ PROC_PORTAL_WARP_SELECT,
        /* 0x0A */ PROC_PORTAL_WARP_FORBID,
        /* 0x0B */ PROC_ZOOM_REGION_TO_SPOT,
        /* 0x0C */ PROC_ZOOM_SPOT_TO_REGION,
        /* 0x0D */ PROC_SPOT_MAP,
        /* 0x0E */ PROC_PORTAL_DEMO1,
        /* 0x0F */ PROC_PORTAL_DEMO4,
        /* 0x10 */ PROC_PORTAL_DEMO5,
        /* 0x11 */ PROC_PORTAL_DEMO6,
        /* 0x12 */ PROC_YAMIBOSS_DEMO1,
        /* 0x13 */ PROC_YAMIBOSS_DEMO2,
        /* 0x14 */ PROC_YAMIBOSS_DEMO3,
        /* 0x15 */ PROC_YAMIBOSS_DEMO4,
        /* 0x16 */ PROC_YAMIBOSS_DEMO5,
        /* 0x17 */ PROC_LIGHT_DEMO1,
        /* 0x18 */ PROC_LIGHT_DEMO2,
        /* 0x19 */ PROC_TABLE_DEMO2,
        /* 0x1A */ PROC_TABLE_DEMO3,
        /* 0x1B */ PROC_HOWL_DEMO1,
        /* 0x1C */ PROC_HOWL_DEMO2,
        /* 0x1D */ PROC_HOWL_DEMO3,
     };

    static dMenu_Fmap_c* MyClass;

private:
    /* 0x004 */ JKRExpHeap* mpHeap;
    /* 0x008 */ JKRExpHeap* mpTalkHeap;
    /* 0x00C */ STControl* mpStick;
    /* 0x010 */ CSTControl* mpCStick;
    /* 0x014 */ dMenu_Fmap2DBack_c* mpDraw2DBack;
    /* 0x018 */ dMenu_Fmap2DTop_c* mpDraw2DTop;
    /* 0x01C */ mDoDvdThd_mountArchive_c* mpMountArchive;
    /* 0x020 */ dMenu_Fmap_world_data_c* mpWorldData;
    /* 0x024 */ dMenu_Fmap_region_data_c* mpRegionData[8];
    /* 0x044 */ dMenu_Fmap_stage_data_c* mpStageData[8];
    /* 0x064 */ JKRArchive* mpMapArchive;
    /* 0x068 */ JKRArchive* mpResArchive;
    /* 0x06C */ dMenuMapCommon_c::RoomData_c* mpRoomData[8];
    /* 0x08C */ dMenu_Fmap_field_data_c* mpFieldDat;
    /* 0x090 */ dMenu_Fmap_portal_data_c* mpPortalDat;
    /* 0x094 */ ResTIMG* mRegionTexture[8];
    /* 0x0B4 */ int field_0xb4;
    /* 0x0B8 */ dMenu_FmapMap_c* mpMenuFmapMap;
    /* 0x0BC */ void* mpFmapMapRes;
    /* 0x0C0 */ u8* mFlashRooms;
    /* 0x0C4 */ dMsgFlow_c mMsgFlow;
    /* 0x110 */ cXyz mPlayerPos;
    /* 0x11C */ f32 mTransX;
    /* 0x120 */ f32 mTransY;
    /* 0x124 */ f32 mAlphaRatio;
    /* 0x128 */ f32 field_0x128[8];
    /* 0x148 */ f32 field_0x148[8];
    /* 0x168 */ f32 field_0x168[8];
    /* 0x188 */ f32 field_0x188[8];
    /* 0x1A8 */ f32 mSpotTextureFadeAlpha;
    /* 0x1AC */ f32 mRegionOffsetX[8];
    /* 0x1CC */ f32 mRegionOffsetZ[8];
    /* 0x1EC */ f32 field_0x1ec;
    /* 0x1F0 */ f32 mOldStageTransX;
    /* 0x1F4 */ f32 mOldStageTransZ;
    /* 0x1F8 */ u32 mRoomDataBits[2];
    /* 0x200 */ u32 field_0x200;
    /* 0x204 */ int mStageCursor;
    /* 0x208 */ int mRoomCursor;
    /* 0x20C */ int mStayStageNo;
    /* 0x210 */ s16 field_0x210;
    /* 0x212 */ s16 mDisplayFrame;
    /* 0x214 */ s16 mZoomLevel;
    /* 0x216 */ u16 mAllTitleName;
    /* 0x218 */ u16 mTitleName[8];
    /* 0x228 */ u16 mAreaName[20];
    /* 0x250 */ char mMarkedStageName[8];
    /* 0x258 */ char mTmpStageName[8];
    /* 0x260 */ char mStageName[20][8];
    /* 0x300 */ u8 mProcess;
    /* 0x301 */ u8 mPrevProcess;
    /* 0x302 */ u8 mPrevProcessAlt;
    /* 0x303 */ u8 mDataNumMax;
    /* 0x304 */ u8 mPanDirection;
    /* 0x305 */ bool field_0x305;
    /* 0x306 */ u8 mSpotNum;
    /* 0x307 */ bool mIsWarpMap;
    /* 0x308 */ u8 mPortalNo;
    /* 0x309 */ bool mErrorSound;
    /* 0x30A */ u8 mStartStageNo;
    /* 0x30B */ bool field_0x30b;
    /* 0x30C */ u8 mOldRegionCursor;
    /* 0x30D */ u8 mOldStageCursor;
    /* 0x30E */ u8 mFlashRoomCount;
    /* 0x30F */ u8 field_0x30f;
    /* 0x310 */ u8 field_0x310;
    /* 0x311 */ u8 mRegionNo;
    /* 0x312 */ bool mResetAreaName;
    /* 0x313 */ bool mAreaNameSet;
};

class dMf_HIO_c : public JORReflexible {
public:
    dMf_HIO_c();
    virtual ~dMf_HIO_c() {}
#if DEBUG
    virtual void genMessage(JORMContext*);
#endif

    /* 0x04 */ u8 field_0x4[4];
    /* 0x08 */ f32 mBackAnimeStep;
    /* 0x0C */ f32 mTopAnimeStep;
    /* 0x10 */ u8 mBaseBackAlpha;
    /* 0x11 */ u8 mMoyaAlpha;

    static dMf_HIO_c* mMySelfPointer;
};

const char* dMenuFmap_getStartStageName(void* param_0);

#endif /* D_MENU_D_MENU_FMAP_H */
