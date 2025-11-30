#ifndef D_MAP_D_MAP_PATH_FMAP_H
#define D_MAP_D_MAP_PATH_FMAP_H

#include "d/d_map_path.h"
#include "d/d_tresure.h"
#include "d/d_stage.h"

class fmpTresTypeGroupData_c {
public:
    fmpTresTypeGroupData_c() {
        mpTresData = NULL;
        mpNext = NULL;
    }

    ~fmpTresTypeGroupData_c() {
        if (mpNext != NULL) {
            delete mpNext;
        }
    }

    void setTypeGroupNo(u8 i_no) { mTypeGroupNo = i_no; }
    void setNextData(fmpTresTypeGroupData_c* i_next) { mpNext = i_next; }
    void setTresData(const dTres_c::data_s* i_data) { mpTresData = i_data; }
    const dTres_c::data_s* getTresData() { return mpTresData; }
    fmpTresTypeGroupData_c* getNextData() { return mpNext; }

    /* 0x0 */ const dTres_c::data_s* mpTresData;
    /* 0x0 */ fmpTresTypeGroupData_c* mpNext;
    /* 0x8 */ u8 mTypeGroupNo;
};  // Size: 0xC

class fmpTresTypeGroupDataList_c {
public:
    void addTypeGroupData(u8, dTres_c::data_s const*);
    ~fmpTresTypeGroupDataList_c() {
        if (mpTypeGroupDataHead != NULL) {
            delete mpTypeGroupDataHead;
        }
    }
    fmpTresTypeGroupDataList_c() {
        mpTypeGroupDataHead = NULL;
        mpNextData = NULL;
    }

    fmpTresTypeGroupData_c* getTypeGroupDataHead() { return mpTypeGroupDataHead; }

    /* 0x0 */ fmpTresTypeGroupData_c* mpTypeGroupDataHead;
    /* 0x4 */ fmpTresTypeGroupData_c* mpNextData;
};

class fmpTresTypeGroupDataListAll_c {
public:
    void addTypeGroupData(u8 i_typeGroupNo, const dTres_c::data_s* i_data) {
        mpTypeGroupData[i_typeGroupNo].addTypeGroupData(i_typeGroupNo, i_data);
    }
    fmpTresTypeGroupDataList_c* getTypeGroupDataList(int i_typeGroupNo) {
        return &mpTypeGroupData[i_typeGroupNo];
    }

    /* 0x0 */ fmpTresTypeGroupDataList_c mpTypeGroupData[17];
};

class dMenu_Fmap_data_c {
public:
    dMenu_Fmap_data_c() {
        mp_tresure = NULL;
        m_fileList2 = NULL;
        mp_mapPath = NULL;
        mp_dzsData = NULL;
    }

    dTres_c::list_class* getTresure() { return mp_tresure; }
    f32 getFilelist2MinX() { return m_fileList2->mLeftRmX; }
    f32 getFilelist2MinZ() { return m_fileList2->mInnerRmZ; }
    f32 getFilelist2MaxX() { return m_fileList2->mRightRmX; }
    f32 getFilelist2MaxZ() { return m_fileList2->mFrontRmZ; }
    dDrawPath_c::room_class* getMapPath() { return mp_mapPath; }
    void* getDzsData() { return mp_dzsData; }

    void setFileList2(dStage_FileList2_dt_c* i_fileList) { m_fileList2 = i_fileList; }
    void setTresure(dTres_c::list_class* i_list) { mp_tresure = i_list; }
    void setMapPath(dDrawPath_c::room_class* i_path) { mp_mapPath = i_path; }
    void setDzsData(void* i_dzsData) { mp_dzsData = i_dzsData; }

    /* 0x0 */ dTres_c::list_class* mp_tresure;
    /* 0x4 */ dStage_FileList2_dt_c* m_fileList2;
    /* 0x8 */ dDrawPath_c::room_class* mp_mapPath;
    /* 0xC */ void* mp_dzsData;
};

class dMenu_Fmap_stage_data_c;
class dMenu_Fmap_room_data_c {
public:
    bool isArrival();
    void buildTresTypeGroup(int, int, int);
    void buildFmapRoomData(int, int, f32, f32, f32, f32);

    dMenu_Fmap_room_data_c(u8 i_roomNo, dMenu_Fmap_stage_data_c* i_stageData,
                           dMenu_Fmap_data_c* i_data) {
        mp_fmapData = i_data;
        mp_fmpTresTypeGroupDataListAll = NULL;
        mp_nextData = NULL;
        mp_parentStage = i_stageData;
        m_roomNo = i_roomNo;
    }

    ~dMenu_Fmap_room_data_c() {
        if (mp_fmpTresTypeGroupDataListAll != NULL) {
            delete mp_fmpTresTypeGroupDataListAll;
        }
    }

    fmpTresTypeGroupDataListAll_c* getTypeGroupDataListAll() { return mp_fmpTresTypeGroupDataListAll; }
    f32 getFileList2MinX() { return mp_fmapData->getFilelist2MinX(); }
    f32 getFileList2MinZ() { return mp_fmapData->getFilelist2MinZ(); }
    f32 getFileList2MaxX() { return mp_fmapData->getFilelist2MaxX(); }
    f32 getFileList2MaxZ() { return mp_fmapData->getFilelist2MaxZ(); }
    dMenu_Fmap_room_data_c* getNextData() { return mp_nextData; }
    dMenu_Fmap_data_c* getFmapData() { return mp_fmapData; }
    int getRoomNo() { return m_roomNo; }
    void setNextData(dMenu_Fmap_room_data_c* i_nextData) { mp_nextData = i_nextData; }

    /* 0x00 */ dMenu_Fmap_data_c* mp_fmapData;
    /* 0x04 */ fmpTresTypeGroupDataListAll_c* mp_fmpTresTypeGroupDataListAll;
    /* 0x08 */ dMenu_Fmap_room_data_c* mp_nextData;
    /* 0x0C */ dMenu_Fmap_stage_data_c* mp_parentStage;
    /* 0x10 */ u8 m_roomNo;
};

class dMenu_Fmap_stage_arc_data_c {
public:
    u8 getSaveTableNo() { return mSaveTableNo; }
    u8 getVisitedRoomSaveTableNo() { return mVisitedRoomSaveTableNo; }

    /* 0x0 */ u8 mSize;
    /* 0x1 */ u8 mSaveTableNo;
    /* 0x2 */ u8 mVisitedRoomSaveTableNo;
    /* 0x3 */ u8 field_0x3;
    /* 0x4 */ u8 mRoomNos[0];
};

class dMenu_Fmap_stage_data_c {
public:
    bool isArrival();
    int buildFmapStageData(int, f32, f32);

    dMenu_Fmap_stage_data_c() {
        mpStageArc = NULL;
        mp_roomTop = NULL;
        mpNextData = NULL;
        m_stageMinX = 0.0f;
        m_stageMinZ = 0.0f;
        m_stageMaxX = 0.0f;
        m_stageMaxZ = 0.0f;
        m_offsetX = 0.0f;
        m_offsetZ = 0.0f;
        for (int i = 0; i < 8; i++) {
            name[i] = '\0';
        }
    }

    f32 getStageCenterX() { return (m_stageMaxX + m_stageMinX) * 0.5f; }
    f32 getStageCenterZ() { return (m_stageMaxZ + m_stageMinZ) * 0.5f; }
    f32 getStageCenterX_CoordRegion() { return m_offsetX + getStageCenterX(); }
    f32 getStageCenterZ_CoordRegion() { return m_offsetZ + getStageCenterZ(); }
    char* getStageName() { return name; }
    dMenu_Fmap_stage_arc_data_c* getStageArc() { return mpStageArc; }
    dMenu_Fmap_stage_data_c* getNextData() { return mpNextData; }
    dMenu_Fmap_room_data_c* getFmapRoomDataTop() { return mp_roomTop; }
    f32 getOffsetX() { return m_offsetX; }
    f32 getOffsetZ() { return m_offsetZ; }
    f32 getStageMinX() { return m_stageMinX; }
    f32 getStageMinZ() { return m_stageMinZ; }
    f32 getStageMaxX() { return m_stageMaxX; }
    f32 getStageMaxZ() { return m_stageMaxZ; }
    void setStageArc(dMenu_Fmap_stage_arc_data_c* i_stageArc) { mpStageArc = i_stageArc; }
    void setFmapRoomDataTop(dMenu_Fmap_room_data_c* i_roomTop) { mp_roomTop = i_roomTop; }
    void setNextData(dMenu_Fmap_stage_data_c* i_nextData) { mpNextData = i_nextData; }
    void setStageCntNo(int i_no) { m_stageCntNo = i_no; }
    void setOffsetX(f32 i_offsetX) { m_offsetX = i_offsetX; }
    void setOffsetZ(f32 i_offsetZ) { m_offsetZ = i_offsetZ; }

    void setName(char* i_name) {
        for (int i = 0; i < 8; i++) {
            name[i] = i_name[i];
        }
    }

    /* 0x00 */ char name[8];
    /* 0x08 */ dMenu_Fmap_stage_arc_data_c* mpStageArc;
    /* 0x0C */ dMenu_Fmap_room_data_c* mp_roomTop;
    /* 0x10 */ dMenu_Fmap_stage_data_c* mpNextData;
    /* 0x14 */ f32 m_offsetX;
    /* 0x18 */ f32 m_offsetZ;
    /* 0x1C */ f32 m_stageMinX;
    /* 0x20 */ f32 m_stageMinZ;
    /* 0x24 */ f32 m_stageMaxX;
    /* 0x28 */ f32 m_stageMaxZ;
    /* 0x2C */ int m_stageCntNo;
};

class dMenu_Fmap_region_data_c {
public:
    dMenu_Fmap_stage_data_c* getMenuFmapStageData(int);
    int getPointStagePathInnerNo(f32, f32, int, int*, int*);
    int buildFmapRegionData(int);

    dMenu_Fmap_region_data_c(int i_no, dMenu_Fmap_stage_data_c* i_stageData,
                             f32 i_offsetX, f32 i_offsetZ) {
        mpMenuFmapStageDataTop = i_stageData;
        mpNextData = NULL;
        mRegionOffsetX = i_offsetX;
        mRegionOffsetZ = i_offsetZ;
        mRegionMinX = 0.0f;
        mRegionMaxX = 0.0f;
        mRegionMinZ = 0.0f;
        mRegionMaxZ = 0.0f;
        mRegionNo = i_no;
        buildFmapRegionData(0);
    }

    f32 getStageCenterX_CoordWorld(int i_stageNo) {
        return mRegionOffsetX + getMenuFmapStageData(i_stageNo)->getStageCenterX_CoordRegion();
    }

    f32 getStageCenterZ_CoordWorld(int i_stageNo) {
        return mRegionOffsetZ + getMenuFmapStageData(i_stageNo)->getStageCenterZ_CoordRegion();
    }

    dMenu_Fmap_stage_data_c* getMenuFmapStageDataTop() { return mpMenuFmapStageDataTop; }
    dMenu_Fmap_region_data_c* getNextData() { return mpNextData; }
    f32 getRegionOffsetX() { return mRegionOffsetX; }
    f32 getRegionOffsetZ() { return mRegionOffsetZ; }
    f32 getRegionMinX() { return mRegionMinX; }
    f32 getRegionMaxX() { return mRegionMaxX; }
    f32 getRegionMinZ() { return mRegionMinZ; }
    f32 getRegionMaxZ() { return mRegionMaxZ; }
    int getRegionNo() { return mRegionNo; }
    void setNextData(dMenu_Fmap_region_data_c* i_data) { mpNextData = i_data; }

    /* 0x00 */ dMenu_Fmap_stage_data_c* mpMenuFmapStageDataTop;
    /* 0x04 */ dMenu_Fmap_region_data_c* mpNextData;
    /* 0x08 */ f32 mRegionOffsetX;
    /* 0x0C */ f32 mRegionOffsetZ;
    /* 0x10 */ f32 mRegionMinX;
    /* 0x14 */ f32 mRegionMaxX;
    /* 0x18 */ f32 mRegionMinZ;
    /* 0x1C */ f32 mRegionMaxZ;
    /* 0x20 */ int mRegionNo;
};

class dMenu_Fmap_world_data_c {
public:
    void create(dMenu_Fmap_region_data_c*);
    int buildFmapWorldData();

    dMenu_Fmap_world_data_c(dMenu_Fmap_region_data_c* i_regionData) { create(i_regionData); }
    dMenu_Fmap_region_data_c* getMenuFmapRegionTop() { return mp_fmapRegionData; }
    f32 getWorldMinX() { return m_worldMinX; }
    f32 getWorldMinZ() { return m_worldMinZ; }
    f32 getWorldMaxX() { return m_worldMaxX; }
    f32 getWorldMaxZ() { return m_worldMaxZ; }

    /* 0x00 */ dMenu_Fmap_region_data_c* mp_fmapRegionData;
    /* 0x04 */ f32 m_worldMinX;
    /* 0x08 */ f32 m_worldMinZ;
    /* 0x0C */ f32 m_worldMaxX;
    /* 0x10 */ f32 m_worldMaxZ;
};

class dMenuFmapIconPointer_c {
public:
    bool init(dMenu_Fmap_region_data_c*, dMenu_Fmap_stage_data_c*, u8, int, int);
    bool getFirstData();
    bool getData();
    void getFirstRoomData();
    bool getNextRoomData();
    bool getNextStageData();
    bool getNextData();
    bool nextData();
    bool getValidData();

    /* 0x00 */ int mStageNo;
    /* 0x04 */ int mStayStageNo;
    /* 0x08 */ int mStayRoomNo;
    /* 0x0C */ int mSaveTbl;
    /* 0x10 */ dMenu_Fmap_region_data_c* mpRegionData;
    /* 0x14 */ dMenu_Fmap_stage_data_c* mpStageData;
    /* 0x18 */ dMenu_Fmap_room_data_c* mpRoomData;
    /* 0x1C */ fmpTresTypeGroupDataListAll_c* mpFmpTresTypeGroupDataListAll;
    /* 0x20 */ fmpTresTypeGroupDataList_c* mpFmpTresTypeGroupDataList;
    /* 0x24 */ fmpTresTypeGroupData_c* mpFmpTresTypeGroupData;
    /* 0x28 */ dTres_c::typeGroupData_c* mpTresTypeGroupData;
    /* 0x2C */ const dTres_c::data_s* mpTresData;
    /* 0x30 */ u8 mTypeGroupNo;
    /* 0x31 */ u8 mType;
};

class dMenuFmapIconDisp_c : public dMenuFmapIconPointer_c {
public:
    bool getPosition(int*, int*, f32*, f32*, dTres_c::data_s const**);
    virtual bool isDrawDisp();
};

#endif /* D_MAP_D_MAP_PATH_FMAP_H */
