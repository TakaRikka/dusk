#ifndef D_D_STAGE_H
#define D_D_STAGE_H

#include "SSystem/SComponent/c_lib.h"
#include "d/d_bg_parts.h"
#include "d/d_kankyo.h"
#include "d/d_kankyo_data.h"
#include "f_op/f_op_actor_mng.h"
#include "global.h"

enum StageType {
    /* 0x0 */ ST_FIELD,
    /* 0x1 */ ST_DUNGEON,
    /* 0x2 */ ST_ROOM,
    /* 0x3 */ ST_BOSS_ROOM,
    /* 0x4 */ ST_CASTLE_TOWN,
    /* 0x5 */ ST_CLOUD_SEA,
};

// made up name
struct dStage_nodeHeader {
    /* 0x0 */ u32 m_tag;
    /* 0x4 */ int m_entryNum;
    /* 0x8 */ u32 m_offset;
};

// made up name
struct dStage_fileHeader {
    /* 0x0 */ int m_chunkCount;
    /* 0x4 */ dStage_nodeHeader m_nodes[1]; // Variable length
};

struct stage_vrboxcol_info_class {
    /* 0x00 */ color_RGB_class sky_col;
    /* 0x03 */ color_RGB_class kumo_top_col;
    /* 0x06 */ color_RGB_class kumo_bottom_col;
    /* 0x09 */ GXColor kumo_shadow_col;
    /* 0x0D */ GXColor kasumi_outer_col;
    /* 0x11 */ GXColor kasumi_inner_col;
};  // Size: 0x18

// Virt
struct stage_vrbox_info_class {
    /* 0x00 */ color_RGB_class field_0x0;
    /* 0x03 */ color_RGB_class field_0x3;
    /* 0x06 */ color_RGB_class field_0x6;
    /* 0x09 */ GXColor field_0x9;
    /* 0x0D */ GXColor field_0xd;
    /* 0x11 */ GXColor field_0x11;
};  // Size: 0x15

// TRES
struct stage_tresure_data_class {
    /* 0x00 */ char name[8];
    /* 0x08 */ fopAcM_prmBase_class base;
};  // Size: 0x20

struct stage_tresure_class {
    /* 0x00 */ int num;
    /* 0x04 */ stage_tresure_data_class* m_entries;
};

// STAG
struct stage_stag_info_class {
    /* 0x00 */ f32 mNear;
    /* 0x04 */ f32 mFar;
    /* 0x08 */ u8 mCameraType;
    /* 0x09 */ u8 field_0x09;
    /* 0x0A */ u16 field_0x0a;
    /* 0x0C */ u32 field_0x0c;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u8 field_0x14[6];  // usually all 0xFF
    /* 0x1A */ s16 mGapLevel;
    /* 0x1C */ s16 mRangeUp;
    /* 0x1E */ s16 mRangeDown;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ u8 mMsgGroup;
    /* 0x2A */ u16 mStageTitleNo;
    /* 0x2C */ u8 mParticleNo[16];
};  // Size: 0x3C

// SCLS
struct stage_scls_info_class {
    /* 0x0 */ char mStage[8];
    /* 0x8 */ u8 mStart;
    /* 0x9 */ s8 mRoom;
    /* 0xA */ u8 field_0xa;
    /* 0xB */ u8 field_0xb;
    /* 0xC */ u8 mWipe;

    char* getStage() { return mStage; }

    int getRoom() { return mRoom; }
};  // Size: 0xD

struct stage_scls_info_dummy_class {
    /* 0x00 */ int num;
    /* 0x04 */ stage_scls_info_class* m_entries;
};

// LGT
struct stage_pure_lightvec_info_class {
    /* 0x00 */ Vec position;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 directionX;
    /* 0x14 */ f32 directionY;
    /* 0x18 */ f32 spotCutoff;
    /* 0x1C */ u8 spot_type;
    /* 0x1D */ u8 dist_atten_type;
    /* 0x1E */ u8 flags;
    /* 0x1F */ u8 switch_no;
};  // Size: 0x20

// COLO
struct stage_pselect_info_class {
    /* 0x0 */ u8 palette_id[8];
    /* 0x8 */ f32 change_rate;
};  // Size: 0xC

// LGHT
struct stage_plight_info_class {
    /* 0x00 */ Vec position;
    /* 0x0C */ f32 power;
    /* 0x10 */ u8 field_0x10[0x18 - 0x10];
    /* 0x18 */ color_RGB_class color;
    /* 0x1B */ u8 fluctuation;
};  // Size: 0x1C

// PAL
struct stage_palette_info_class {
    /* 0x00 */ color_RGB_class actor_amb_col;
    /* 0x03 */ color_RGB_class bg_amb_col[4];
    /* 0x0F */ color_RGB_class plight_col[6];
    /* 0x21 */ color_RGB_class fog_col;
    /* 0x24 */ f32 fog_start_z;
    /* 0x28 */ f32 fog_end_z;
    /* 0x2C */ u8 vrboxcol_id;
    /* 0x2D */ u8 bg_light_influence;
    /* 0x2E */ u8 cloud_shadow_density;
    /* 0x2F */ u8 unk_0x2f;
    /* 0x30 */ u8 bloom_tbl_id;
    /* 0x31 */ u8 BG1_amb_alpha;
    /* 0x32 */ u8 BG2_amb_alpha;
    /* 0x33 */ u8 BG3_amb_alpha;
};  // Size: 0x34

struct stage_map_info_class {
    /* 0x00 */ u8 field_0x0[0x35];
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ u8 field_0x36[2];
};  // Size: 0x38

struct stage_map_info_dummy_class {
    /* 0x0 */ int num;
    /* 0x4 */ stage_map_info_class* m_entries;
};

// Env
struct stage_envr_info_class {
    /* 0x0 */ u8 pselect_id[65];
};  // Size: 0x41

// CAMR / RCAM
struct stage_camera2_data_class {
    /* 0x00 */ char m_cam_type[16];
    /* 0x10 */ u8 m_arrow_idx;
    /* 0x11 */ u8 field_0x11;
    /* 0x12 */ u8 field_0x12;
    /* 0x13 */ u8 field_0x13;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
};  // Size: 0x18

struct stage_camera_class {
    /* 0x0 */ int num;
    /* 0x4 */ stage_camera2_data_class* m_entries;
};

// AROB / RARO
struct stage_arrow_data_class {
    /* 0x00 */ cXyz position;
    /* 0x0C */ csXyz angle;
    /* 0x12 */ s16 field_0x12;
};  // Size: 0x14

struct stage_arrow_class {
    /* 0x00 */ int num;
    /* 0x04 */ stage_arrow_data_class* m_entries;
};

// ACT
struct stage_actor_data_class {
    /* 0x00 */ char name[8];
    /* 0x08 */ fopAcM_prmBase_class base;
};  // Size: 0x20

struct stage_actor_class {
    /* 0x0 */ int num;
    /* 0x4 */ stage_actor_data_class* m_entries;
};

// TGSC / SCOB / TGDR / Door
struct stage_tgsc_data_class {
    /* 0x00 */ char name[8];
    /* 0x08 */ fopAcM_prmBase_class base;
    /* 0x20 */ fopAcM_prmScale_class scale;
};  // Size: 0x24

STATIC_ASSERT(sizeof(stage_tgsc_data_class) == 0x24);

struct stage_tgsc_class {
    /* 0x00 */ int num;
    /* 0x04 */ stage_tgsc_data_class* m_entries;
};

// MPAT
struct map_path_class {
    /* 0x0 */ int num;
    /* 0x4 */ void* m_entries;
};

// RTBL
struct roomRead_data_class {
    /* 0x0 */ u8 num;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 field_0x2;
    /* 0x4 */ u8* m_rooms;
};  // Size: 0x8

struct roomRead_class {
    /* 0x0 */ int num;
    /* 0x4 */ roomRead_data_class** m_entries;
};

// MEM
struct dStage_MemoryMap_c {
    /* 0x0 */ int m_num;
    /* 0x4 */ u32* field_0x4;
};

// MEC
struct dStage_MemoryConfig_data {
    /* 0x0 */ u8 m_roomNo;
    /* 0x1 */ u8 m_blockID;
};  // Size: 0x2

struct dStage_MemoryConfig_c {
    /* 0x0 */ int m_num;
    /* 0x4 */ dStage_MemoryConfig_data* field_0x4;
};

// PATH / RPAT
struct dPath;
struct dStage_dPath_c {
    /* 0x0 */ int m_num;
    /* 0x4 */ dPath* m_path;
};

// PPNT / RPPN
struct dStage_dPnt_c {
    /* 0x0 */ int num;
    /* 0x4 */ u32 m_pnt_offset;
};  // Size: 0x8

// MULT
struct dStage_Mult_info {
    /* 0x0 */ f32 mTransX;
    /* 0x4 */ f32 mTransY;
    /* 0x8 */ s16 mAngle;
    /* 0xA */ u8 mRoomNo;
};  // Size: 0xC

class dStage_Multi_c {
public:
    /* 0x0 */ int num;
    /* 0x4 */ dStage_Mult_info* m_entries;
};

// SOND
struct stage_sound_data {
    /* 0x00 */ char field_0x0[8];
    /* 0x08 */ Vec field_0x8;
    /* 0x14 */ u8 field_0x14;
    /* 0x15 */ u8 field_0x15;
    /* 0x16 */ u8 field_0x16;
    /* 0x17 */ u8 field_0x17;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1a;
};  // Size: 0x1C

struct dStage_SoundInfo_c {
    /* 0x0 */ int num;
    /* 0x4 */ stage_sound_data* entries;
};

// FILI
class dStage_FileList_dt_c {
public:
    /* 0x00 */ u32 mParameters;
    /* 0x04 */ f32 mSeaLevel;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ u8 field_0x10[10];
    /* 0x1A */ u8 mDefaultCamera;
    /* 0x1B */ u8 mBitSw;
    /* 0x1C */ u16 mMsg;
};  // Size: 0x20

// FILI
class dStage_FileList2_dt_c {
public:
    /* 0x00 */ f32 mLeftRmX;
    /* 0x04 */ f32 mInnerRmZ;
    /* 0x08 */ f32 mRightRmX;
    /* 0x0C */ f32 mFrontRmZ;
    /* 0x10 */ u8 mMinFloorNo;
    /* 0x11 */ u8 mMaxFloorNo;
    /* 0x12 */ u8 field_0x12;
    /* 0x13 */ u8 field_0x13;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ s16 field_0x1c;
};  // Size: 0x20

struct dStage_FileList2_c {
    /* 0x0 */ int num;
    /* 0x4 */ dStage_FileList2_dt_c* entries;
};

// FLOR
struct dStage_FloorInfo_dt_c {
    // Copied from TWW, may not be right
    /* 0x00 */ int field_0x00;
    /* 0x04 */ u8 floorNo;
    /* 0x05 */ s8 field_0x05[14];
}; // Size: 0x14

struct dStage_FloorInfo_c {
    /* 0x00 */ int num;
    /* 0x04 */ dStage_FloorInfo_dt_c* m_entries;
};

// LBNK
class dStage_Lbnk_dt_c {
public:
    /* 0x0 */ u8 bank;
    /* 0x1 */ u8 bank2;
    /* 0x2 */ u8 field_0x2;
};

struct dStage_Lbnk_c {
    /* 0x0 */ int num;
    /* 0x4 */ dStage_Lbnk_dt_c* entries;
};

struct dStage_Elst_dt_c {
    /* 0x0 */ u8 m_layerTable[15];
};  // Size: 0xF

struct dStage_Elst_c {
    /* 0x0 */ int m_entryNum;
    /* 0x4 */ dStage_Elst_dt_c* m_entries;
};

// DMAP
struct dStage_DMap_dt_c {
    // Copied from TWW, may not be right
    /* 0x00 */ int field_0x00;
    /* 0x04 */ int field_0x04;
    /* 0x08 */ int field_0x08;
    /* 0x0C */ f32 offsetY;
};  // Size: 0x10

struct dStage_DMap_c {
    /* 0x00 */ int num;
    /* 0x04 */ dStage_DMap_dt_c* entries;
};

/**
 * dStage_MapEvent
 * Section Magic: "REVT"
 *
 * For STB/ZEV events, an event name is specified for data
 * For MapTool events, other data is specified instead
 */
// using packing to make sure event_name doesn't get aligned
// field_0x1a / switch_no might be part of maptool data? unsure
#pragma push
#pragma pack(1)
struct dStage_MapEvent_dt_c {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x03 */ u8 field_0x3;
    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 field_0x5;
    /* 0x06 */ u8 priority;
    /* 0x07 */ u8 field_0x7;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ u8 field_0xA;
    /* 0x0B */ u8 field_0xB;
    /* 0x0C */ u8 field_0xC;
    union {
        /* 0x0D */ char event_name[13];
        struct {
            /* 0x0D */ u8 field_0xd[0x14 - 0xD];
            /* 0x14 */ u16 field_0x14;
            /* 0x16 */ u8 field_0x16;
            /* 0x17 */ u8 field_0x17;
            /* 0x18 */ u8 sound_type;
            /* 0x19 */ u8 field_0x19;
        } maptool;
    } data;
    /* 0x1A */ u8 field_0x1a;
    /* 0x1B */ u8 switch_no;
};  // SIZE = 0x1C
#pragma pack()
#pragma pop

enum dStage_MapEvent_dt_type {
    dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA,
    dStage_MapEvent_dt_TYPE_ZEV,
    dStage_MapEvent_dt_TYPE_STB,
};

struct dStage_MapEventInfo_c {
    /* 0x0 */ int num;
    /* 0x4 */ dStage_MapEvent_dt_c* m_entries;
};

class dStage_dt_c {
public:
    dStage_dt_c() {}
    void setRoomNo(s8 roomNo) { mRoomNo = roomNo; }
    s8 getRoomNo() { return mRoomNo; }

    /* vt[ 2] */ virtual void init(void) = 0;
    /* vt[ 3] */ virtual void setCamera(stage_camera_class*) = 0;
    /* vt[ 4] */ virtual stage_camera_class* getCamera(void) const = 0;
    /* vt[ 5] */ virtual void setArrow(stage_arrow_class*) = 0;
    /* vt[ 6] */ virtual stage_arrow_class* getArrow(void) const = 0;
    /* vt[ 7] */ virtual void setPlayer(stage_actor_class*) = 0;
    /* vt[ 8] */ virtual stage_actor_class* getPlayer(void) const = 0;
    /* vt[ 9] */ virtual void setPlayerNum(u16) = 0;
    /* vt[10] */ virtual u16 getPlayerNum(void) const = 0;
    /* vt[11] */ virtual void setRoom(roomRead_class*) = 0;
    /* vt[12] */ virtual roomRead_class* getRoom(void) const = 0;
    /* vt[13] */ virtual void setMapInfo(stage_map_info_class*) = 0;
    /* vt[14] */ virtual stage_map_info_class* getMapInfo(void) const = 0;
    /* vt[15] */ virtual stage_map_info_class* getMapInfo2(int) const = 0;
    /* vt[16] */ virtual void setMapInfoBase(stage_map_info_dummy_class*) = 0;
    /* vt[17] */ virtual stage_map_info_dummy_class* getMapInfoBase(void) const = 0;
    /* vt[18] */ virtual void setPaletteInfo(stage_palette_info_class*) = 0;
    /* vt[19] */ virtual stage_palette_info_class* getPaletteInfo(void) const = 0;
    /* vt[20] */ virtual void setPselectInfo(stage_pselect_info_class*) = 0;
    /* vt[21] */ virtual stage_pselect_info_class* getPselectInfo(void) const = 0;
    /* vt[22] */ virtual void setEnvrInfo(stage_envr_info_class*) = 0;
    /* vt[23] */ virtual stage_envr_info_class* getEnvrInfo(void) const = 0;
    /* vt[24] */ virtual void setVrboxInfo(stage_vrbox_info_class*) = 0;
    /* vt[25] */ virtual stage_vrbox_info_class* getVrboxInfo(void) const = 0;
    /* vt[26] */ virtual void setVrboxcolInfo(stage_vrboxcol_info_class*) = 0;
    /* vt[27] */ virtual stage_vrboxcol_info_class* getVrboxcolInfo(void) const = 0;
    /* vt[28] */ virtual void setPlightInfo(stage_plight_info_class*) = 0;
    /* vt[29] */ virtual stage_plight_info_class* getPlightInfo(void) const = 0;
    /* vt[30] */ virtual void setPaletteNumInfo(int) = 0;
    /* vt[31] */ virtual int getPaletteNumInfo(void) const = 0;
    /* vt[32] */ virtual void setPselectNumInfo(int) = 0;
    /* vt[33] */ virtual int getPselectNumInfo(void) const = 0;
    /* vt[34] */ virtual void setEnvrNumInfo(int) = 0;
    /* vt[35] */ virtual int getEnvrNumInfo(void) const = 0;
    /* vt[36] */ virtual void setVrboxNumInfo(int) = 0;
    /* vt[37] */ virtual int getVrboxNumInfo(void) const = 0;
    /* vt[38] */ virtual void setVrboxcolNumInfo(int) = 0;
    /* vt[39] */ virtual int getVrboxcolNumInfo(void) const = 0;
    /* vt[40] */ virtual void setPlightNumInfo(int) = 0;
    /* vt[41] */ virtual int getPlightNumInfo(void) const = 0;
    /* vt[42] */ virtual void setLightVecInfo(stage_pure_lightvec_info_class*) = 0;
    /* vt[43] */ virtual stage_pure_lightvec_info_class* getLightVecInfo(void) const = 0;
    /* vt[44] */ virtual void setLightVecInfoNum(int) = 0;
    /* vt[45] */ virtual int getLightVecInfoNum(void) const = 0;
    /* vt[46] */ virtual void setStagInfo(stage_stag_info_class*) = 0;
    /* vt[47] */ virtual stage_stag_info_class* getStagInfo(void) const = 0;
    /* vt[48] */ virtual void setSclsInfo(stage_scls_info_dummy_class*) = 0;
    /* vt[49] */ virtual stage_scls_info_dummy_class* getSclsInfo(void) const = 0;
    /* vt[50] */ virtual void setPntInfo(dStage_dPnt_c*) = 0;
    /* vt[51] */ virtual dStage_dPnt_c* getPntInf(void) const = 0;
    /* vt[52] */ virtual void setPathInfo(dStage_dPath_c*) = 0;
    /* vt[53] */ virtual dStage_dPath_c* getPathInf(void) const = 0;
    /* vt[54] */ virtual void setPnt2Info(dStage_dPnt_c*) = 0;
    /* vt[55] */ virtual dStage_dPnt_c* getPnt2Inf(void) const = 0;
    /* vt[56] */ virtual void setPath2Info(dStage_dPath_c*) = 0;
    /* vt[57] */ virtual dStage_dPath_c* getPath2Inf(void) const = 0;
    /* vt[58] */ virtual void setSoundInf(dStage_SoundInfo_c*) = 0;
    /* vt[59] */ virtual dStage_SoundInfo_c* getSoundInf(void) const = 0;
    /* vt[60] */ virtual void setSoundInfCL(dStage_SoundInfo_c*) = 0;
    /* vt[61] */ virtual dStage_SoundInfo_c* getSoundInfCL(void) const = 0;
    /* vt[62] */ virtual void setMapEventInfo(dStage_MapEventInfo_c*) = 0;
    /* vt[63] */ virtual dStage_MapEventInfo_c* getMapEventInfo(void) const = 0;
    /* vt[64] */ virtual void setFileList2Info(dStage_FileList2_dt_c*) = 0;
    /* vt[65] */ virtual dStage_FileList2_dt_c* getFileList2Info(void) const = 0;
    /* vt[66] */ virtual void setFileListInfo(dStage_FileList_dt_c*) = 0;
    /* vt[67] */ virtual dStage_FileList_dt_c* getFileListInfo(void) const = 0;
    /* vt[68] */ virtual void setFloorInfo(dStage_FloorInfo_c*) = 0;
    /* vt[69] */ virtual dStage_FloorInfo_c* getFloorInfo(void) const = 0;
    /* vt[70] */ virtual void setMemoryConfig(dStage_MemoryConfig_c*) = 0;
    /* vt[71] */ virtual dStage_MemoryConfig_c* getMemoryConfig(void) const = 0;
    /* vt[72] */ virtual void setMemoryMap(dStage_MemoryMap_c*) = 0;
    /* vt[73] */ virtual dStage_MemoryMap_c* getMemoryMap(void) const = 0;
    /* vt[74] */ virtual void setMulti(dStage_Multi_c*) = 0;
    /* vt[75] */ virtual dStage_Multi_c* getMulti(void) const = 0;
    /* vt[76] */ virtual void setOldMulti(void) = 0;
    /* vt[77] */ virtual void resetOldMulti(void) = 0;
    /* vt[78] */ virtual dStage_Multi_c* getOldMulti(void) const = 0;
    /* vt[79] */ virtual void setLbnk(dStage_Lbnk_c*) = 0;
    /* vt[80] */ virtual dStage_Lbnk_c* getLbnk(void) const = 0;
    /* vt[81] */ virtual void setTresure(stage_tresure_class*) = 0;
    /* vt[82] */ virtual stage_tresure_class* getTresure(void) const = 0;
    /* vt[83] */ virtual void setDMap(dStage_DMap_c*) = 0;
    /* vt[84] */ virtual dStage_DMap_c* getDMap(void) const = 0;
    /* vt[85] */ virtual void setDrTg(stage_tgsc_class*) = 0;
    /* vt[86] */ virtual stage_tgsc_class* getDrTg(void) const = 0;
    /* vt[87] */ virtual void setDoor(stage_tgsc_class*) = 0;
    /* vt[88] */ virtual stage_tgsc_class* getDoor(void) const = 0;
    /* vt[89] */ virtual void setMapPath(void*) = 0;
    /* vt[90] */ virtual void* getMapPath(void) = 0;
    /* vt[91] */ virtual void setElst(dStage_Elst_c*) = 0;
    /* vt[92] */ virtual dStage_Elst_c* getElst(void) = 0;

private:
    /* 0x04 */ s8 mRoomNo;
};

void dStage_SetErrorStage();
void dStage_SetErrorRoom();

class dStage_stageDt_c : public dStage_dt_c {
public:
    dStage_stageDt_c() {}

    /* vt[ 2] */ virtual void init(void);
    /* vt[ 3] */ virtual void setCamera(stage_camera_class* i_Camera) { mCamera = i_Camera; }
    /* vt[ 4] */ virtual stage_camera_class* getCamera(void) const { return mCamera; }
    /* vt[ 5] */ virtual void setArrow(stage_arrow_class* i_Arrow) { mArrow = i_Arrow; }
    /* vt[ 6] */ virtual stage_arrow_class* getArrow(void) const { return mArrow; }
    /* vt[ 7] */ virtual void setPlayer(stage_actor_class* i_Player) { mPlayer = i_Player; }
    /* vt[ 8] */ virtual stage_actor_class* getPlayer(void) const { return mPlayer; }
    /* vt[ 9] */ virtual void setPlayerNum(u16 i_PlayerNum) { mPlayerNum = i_PlayerNum; }
    /* vt[10] */ virtual u16 getPlayerNum(void) const { return mPlayerNum; }
    /* vt[11] */ virtual void setRoom(roomRead_class* i_Room) { mRoom = i_Room; }
    /* vt[12] */ virtual roomRead_class* getRoom(void) const { return mRoom; }
    /* vt[13] */ virtual void setMapInfo(stage_map_info_class* i_MapInfo) { mMapInfo = i_MapInfo; }
    /* vt[14] */ virtual stage_map_info_class* getMapInfo(void) const { return mMapInfo; }
    /* vt[15] */ virtual stage_map_info_class* getMapInfo2(int) const;
    /* vt[16] */ virtual void setMapInfoBase(stage_map_info_dummy_class* i_MapInfoBase) { mMapInfoBase = i_MapInfoBase; }
    /* vt[17] */ virtual stage_map_info_dummy_class* getMapInfoBase(void) const { return mMapInfoBase; }
    /* vt[18] */ virtual void setPaletteInfo(stage_palette_info_class* i_PaletteInfo) { mPaletteInfo = i_PaletteInfo; }
    /* vt[19] */ virtual stage_palette_info_class* getPaletteInfo(void) const { return mPaletteInfo; }
    /* vt[20] */ virtual void setPselectInfo(stage_pselect_info_class* i_PselectInfo) { mPselectInfo = i_PselectInfo; }
    /* vt[21] */ virtual stage_pselect_info_class* getPselectInfo(void) const { return mPselectInfo; }
    /* vt[22] */ virtual void setEnvrInfo(stage_envr_info_class* i_EnvrInfo) { mEnvrInfo = i_EnvrInfo; }
    /* vt[23] */ virtual stage_envr_info_class* getEnvrInfo(void) const { return mEnvrInfo; }
    /* vt[24] */ virtual void setVrboxInfo(stage_vrbox_info_class* i_VrboxInfo) { mVrboxInfo = i_VrboxInfo; }
    /* vt[25] */ virtual stage_vrbox_info_class* getVrboxInfo(void) const { return mVrboxInfo; }
    /* vt[26] */ virtual void setVrboxcolInfo(stage_vrboxcol_info_class* i_VrboxcolInfo) { mVrboxcolInfo = i_VrboxcolInfo; }
    /* vt[27] */ virtual stage_vrboxcol_info_class* getVrboxcolInfo(void) const { return mVrboxcolInfo; }
    /* vt[28] */ virtual void setPlightInfo(stage_plight_info_class* i_PlightInfo) { mPlightInfo = i_PlightInfo; }
    /* vt[29] */ virtual stage_plight_info_class* getPlightInfo(void) const { return mPlightInfo; }
    /* vt[30] */ virtual void setPaletteNumInfo(int i_PaletteNumInfo) { mPaletteNumInfo = i_PaletteNumInfo; }
    /* vt[31] */ virtual int getPaletteNumInfo(void) const { return mPaletteNumInfo; }
    /* vt[32] */ virtual void setPselectNumInfo(int i_PselectNumInfo) { mPselectNumInfo = i_PselectNumInfo; }
    /* vt[33] */ virtual int getPselectNumInfo(void) const { return mPselectNumInfo; }
    /* vt[34] */ virtual void setEnvrNumInfo(int i_EnvrNumInfo) { mEnvrNumInfo = i_EnvrNumInfo; }
    /* vt[35] */ virtual int getEnvrNumInfo(void) const { return mEnvrNumInfo; }
    /* vt[36] */ virtual void setVrboxNumInfo(int i_VrboxNumInfo) { mVrboxNumInfo = i_VrboxNumInfo; }
    /* vt[37] */ virtual int getVrboxNumInfo(void) const { return mVrboxNumInfo; }
    /* vt[38] */ virtual void setVrboxcolNumInfo(int i_VrboxcolNumInfo) { mVrboxcolNumInfo = i_VrboxcolNumInfo; }
    /* vt[39] */ virtual int getVrboxcolNumInfo(void) const { return mVrboxcolNumInfo; }
    /* vt[42] */ virtual void setLightVecInfo(stage_pure_lightvec_info_class* i_LightVecInfo) {
        (void)i_LightVecInfo;
        dStage_SetErrorStage();
        OSReport("stage non LightVec data !!\n");
        JUT_ASSERT(3003, FALSE);
    }
    /* vt[43] */ virtual stage_pure_lightvec_info_class* getLightVecInfo(void) const {
        OSReport("stage non LightVec data !!\n");
        JUT_ASSERT(3007, FALSE);
        return NULL;
    }
    /* vt[44] */ virtual void setLightVecInfoNum(int i_LightVecInfoNum) { 
        (void)i_LightVecInfoNum;
        dStage_SetErrorStage();
        OSReport("stage non LightVecNum data !!\n");
        JUT_ASSERT(3014, FALSE);
    }
    /* vt[45] */ virtual int getLightVecInfoNum(void) const {
        OSReport("stage non LightVecNum data !!\n");
        JUT_ASSERT(3018, FALSE);
        return NULL;
    }
    /* vt[40] */ virtual void setPlightNumInfo(int i_PlightNumInfo) { mPlightNumInfo = i_PlightNumInfo; }
    /* vt[41] */ virtual int getPlightNumInfo(void) const { return mPlightNumInfo; }
    /* vt[46] */ virtual void setStagInfo(stage_stag_info_class* i_StagInfo) { mStagInfo = i_StagInfo; }
    /* vt[47] */ virtual stage_stag_info_class* getStagInfo(void) const { return mStagInfo; }
    /* vt[48] */ virtual void setSclsInfo(stage_scls_info_dummy_class* i_SclsInfo) { mSclsInfo = i_SclsInfo; }
    /* vt[49] */ virtual stage_scls_info_dummy_class* getSclsInfo(void) const { return mSclsInfo; }
    /* vt[50] */ virtual void setPntInfo(dStage_dPnt_c* i_PntInfo) { mPntInfo = i_PntInfo; }
    /* vt[51] */ virtual dStage_dPnt_c* getPntInf(void) const { return mPntInfo; }
    /* vt[52] */ virtual void setPathInfo(dStage_dPath_c* i_PathInfo) { mPathInfo = i_PathInfo; }
    /* vt[53] */ virtual dStage_dPath_c* getPathInf(void) const { return mPathInfo; }
    /* vt[54] */ virtual void setPnt2Info(dStage_dPnt_c* i_Pnt2Info) { mPnt2Info = i_Pnt2Info; }
    /* vt[55] */ virtual dStage_dPnt_c* getPnt2Inf(void) const { return mPnt2Info; }
    /* vt[56] */ virtual void setPath2Info(dStage_dPath_c* i_Path2Info) { mPath2Info = i_Path2Info; }
    /* vt[57] */ virtual dStage_dPath_c* getPath2Inf(void) const { return mPath2Info; }
    /* vt[58] */ virtual void setSoundInf(dStage_SoundInfo_c* i_SoundInf) { mSoundInf = i_SoundInf; }
    /* vt[59] */ virtual dStage_SoundInfo_c* getSoundInf(void) const { return mSoundInf; }
    /* vt[60] */ virtual void setSoundInfCL(dStage_SoundInfo_c* i_SoundInfCL) { mSoundInfCL = i_SoundInfCL; }
    /* vt[61] */ virtual dStage_SoundInfo_c* getSoundInfCL(void) const { return mSoundInfCL; }
    /* vt[62] */ virtual void setMapEventInfo(dStage_MapEventInfo_c* i_MapEventInfo) { mMapEventInfo = i_MapEventInfo; }
    /* vt[63] */ virtual dStage_MapEventInfo_c* getMapEventInfo(void) const { return mMapEventInfo; }
    /* vt[64] */ virtual void setFileList2Info(dStage_FileList2_dt_c* list) {
        (void)list;
        dStage_SetErrorStage();
        OSReport("stage non filelist2 data!\n");
        JUT_ASSERT(3123, FALSE);
    }
    /* vt[65] */ virtual dStage_FileList2_dt_c* getFileList2Info(void) const {
        OSReport("stage non filelist2 data!\n");
        JUT_ASSERT(3127, FALSE);
        return NULL;
    }
    /* vt[66] */ virtual void setFileListInfo(dStage_FileList_dt_c* list) {
        (void)list;
        dStage_SetErrorStage();
        OSReport("stage non filelist data!\n");
        JUT_ASSERT(3138, FALSE);
    }
    /* vt[67] */ virtual dStage_FileList_dt_c* getFileListInfo(void) const {
        OSReport("stage non filelist data!\n");
        JUT_ASSERT(3142, FALSE);
        return NULL;
    }
    /* vt[68] */ virtual void setFloorInfo(dStage_FloorInfo_c* i_FloorInfo) { mFloorInfo = i_FloorInfo; }
    /* vt[69] */ virtual dStage_FloorInfo_c* getFloorInfo(void) const { return mFloorInfo; }
    /* vt[70] */ virtual void setMemoryConfig(dStage_MemoryConfig_c* i_MemoryConfig) { mMemoryConfig = i_MemoryConfig; }
    /* vt[71] */ virtual dStage_MemoryConfig_c* getMemoryConfig(void) const { return mMemoryConfig; }
    /* vt[72] */ virtual void setMemoryMap(dStage_MemoryMap_c* i_MemoryMap) { mMemoryMap = i_MemoryMap; }
    /* vt[73] */ virtual dStage_MemoryMap_c* getMemoryMap(void) const { return mMemoryMap; }
    /* vt[74] */ virtual void setMulti(dStage_Multi_c* i_Multi) { mMulti = i_Multi; }
    /* vt[75] */ virtual dStage_Multi_c* getMulti(void) const { return mMulti; }
    /* vt[76] */ virtual void setOldMulti(void);
    /* vt[77] */ virtual void resetOldMulti(void);
    /* vt[78] */ virtual dStage_Multi_c* getOldMulti(void) const { return mOldMulti; }
    /* vt[79] */ virtual void setLbnk(dStage_Lbnk_c* lbnk) {
        (void)lbnk;
        dStage_SetErrorStage();
        OSReport("stage non Lbnk data!\n");
        JUT_ASSERT(3231, FALSE);
    }
    /* vt[80] */ virtual dStage_Lbnk_c* getLbnk(void) const {
        OSReport("stage non Lbnk data!\n");
        JUT_ASSERT(3238, FALSE);
        return NULL;
    }
    /* vt[81] */ virtual void setTresure(stage_tresure_class* i_Tresure) { mTresure = i_Tresure; }
    /* vt[82] */ virtual stage_tresure_class* getTresure(void) const { return mTresure; }
    /* vt[83] */ virtual void setDMap(dStage_DMap_c* i_DMap) { mDMap = i_DMap; }
    /* vt[84] */ virtual dStage_DMap_c* getDMap(void) const { return mDMap; }
    /* vt[85] */ virtual void setDrTg(stage_tgsc_class* i_DrTg) { mDrTg = i_DrTg; }
    /* vt[86] */ virtual stage_tgsc_class* getDrTg(void) const { return mDrTg; }
    /* vt[87] */ virtual void setDoor(stage_tgsc_class* i_Door) { mDoor = i_Door; }
    /* vt[88] */ virtual stage_tgsc_class* getDoor(void) const { return mDoor; }
    /* vt[89] */ virtual void setMapPath(void* i_MapPath) { return; }
    /* vt[90] */ virtual void* getMapPath(void) { return NULL; }
    /* vt[91] */ virtual void setElst(dStage_Elst_c* i_Elst) { mElst = i_Elst; }
    /* vt[92] */ virtual dStage_Elst_c* getElst(void);

    s16 getWorldRollAngleX() { return (s16)mWorldRollAngleX; }
    s16 getWorldRollDirAngleY() { return mWorldRollDirAngleY; }

public:
    /* 0x08 */ stage_camera_class* mCamera;
    /* 0x0C */ stage_arrow_class* mArrow;
    /* 0x10 */ stage_actor_class* mPlayer;
    /* 0x14 */ roomRead_class* mRoom;
    /* 0x18 */ stage_map_info_class* mMapInfo;
    /* 0x1C */ stage_map_info_dummy_class* mMapInfoBase;
    /* 0x20 */ stage_palette_info_class* mPaletteInfo;
    /* 0x24 */ stage_pselect_info_class* mPselectInfo;
    /* 0x28 */ stage_envr_info_class* mEnvrInfo;
    /* 0x2C */ stage_vrbox_info_class* mVrboxInfo;
    /* 0x30 */ stage_vrboxcol_info_class* mVrboxcolInfo;
    /* 0x34 */ stage_plight_info_class* mPlightInfo;
    /* 0x38 */ dStage_MapEventInfo_c* mMapEventInfo;
    /* 0x3C */ u32 mPaletteNumInfo;
    /* 0x40 */ u32 mPselectNumInfo;
    /* 0x44 */ u32 mEnvrNumInfo;
    /* 0x48 */ u32 mVrboxNumInfo;
    /* 0x4C */ int mVrboxcolNumInfo;
    /* 0x50 */ u32 mPlightNumInfo;
    /* 0x54 */ u16 mPlayerNum;
    /* 0x56 */ s16 mWorldRollAngleX;
    /* 0x58 */ s16 mWorldRollDirAngleY;
    /* 0x5A */ u16 field_0x5a;
    /* 0x5C */ stage_stag_info_class* mStagInfo;
    /* 0x60 */ stage_scls_info_dummy_class* mSclsInfo;
    /* 0x64 */ dStage_dPnt_c* mPntInfo;
    /* 0x68 */ dStage_dPath_c* mPathInfo;
    /* 0x6C */ dStage_dPnt_c* mPnt2Info;
    /* 0x70 */ dStage_dPath_c* mPath2Info;
    /* 0x74 */ dStage_SoundInfo_c* mSoundInf;
    /* 0x78 */ dStage_SoundInfo_c* mSoundInfCL;
    /* 0x7C */ dStage_FloorInfo_c* mFloorInfo;
    /* 0x80 */ dStage_MemoryConfig_c* mMemoryConfig;
    /* 0x84 */ dStage_MemoryMap_c* mMemoryMap;
    /* 0x88 */ dStage_Multi_c* mMulti;
    /* 0x8C */ dStage_Multi_c* mOldMulti;
    /* 0x90 */ stage_tresure_class* mTresure;
    /* 0x94 */ dStage_DMap_c* mDMap;
    /* 0x98 */ stage_tgsc_class* mDrTg;
    /* 0x9C */ stage_tgsc_class* mDoor;
    /* 0xA0 */ dStage_Elst_c* mElst;
};

class dStage_roomDt_c : public dStage_dt_c {
public:
    dStage_roomDt_c() {}
    void initFileList2(void);

    virtual void init(void);
    virtual void setCamera(stage_camera_class* i_Camera) { mCamera = i_Camera; }
    virtual stage_camera_class* getCamera(void) const { return mCamera; }
    virtual void setArrow(stage_arrow_class* i_Arrow) { mArrow = i_Arrow; }
    virtual stage_arrow_class* getArrow(void) const { return mArrow; }
    virtual void setPlayer(stage_actor_class* i_Player) { mPlayer = i_Player; }
    virtual stage_actor_class* getPlayer(void) const { return mPlayer; }
    virtual void setPlayerNum(u16 i_PlayerNum) { mPlayerNum = i_PlayerNum; }
    virtual u16 getPlayerNum(void) const { return mPlayerNum; }
    virtual void setRoom(roomRead_class* i_Room) {
        (void)i_Room;
        dStage_SetErrorRoom();
        OSReport("Room non room data !!\n");
        JUT_ASSERT(2095, FALSE);
    }
    virtual roomRead_class* getRoom(void) const {
        OSReport("Room non room data !!\n");
        JUT_ASSERT(2100, FALSE);
        return NULL;
    }
    virtual void setMapInfo(stage_map_info_class* i_MapInfo) { mMapInfo = i_MapInfo; }
    virtual stage_map_info_class* getMapInfo(void) const { return mMapInfo; }
    virtual stage_map_info_class* getMapInfo2(int) const;
    virtual void setMapInfoBase(stage_map_info_dummy_class* i_MapInfoBase) { mMapInfoBase = i_MapInfoBase; }
    virtual stage_map_info_dummy_class* getMapInfoBase(void) const { return mMapInfoBase; }
    virtual void setPaletteInfo(stage_palette_info_class* i_PaletteInfo) {
        (void)i_PaletteInfo;
        dStage_SetErrorRoom();
        OSReport("Room non palet data !!\n");
        JUT_ASSERT(2126, FALSE);
    }
    virtual stage_palette_info_class* getPaletteInfo(void) const {
        OSReport("Room non palet data !!\n");
        JUT_ASSERT(2130, FALSE);
        return NULL;
    }
    virtual void setPselectInfo(stage_pselect_info_class* i_PselectInfo) {
        (void)i_PselectInfo;
        dStage_SetErrorRoom();
        OSReport("Room non pselect data !!\n");
        JUT_ASSERT(2137, FALSE);
    }
    virtual stage_pselect_info_class* getPselectInfo(void) const {
        OSReport("Room non pselect data !!\n");
        JUT_ASSERT(2141, FALSE);
        return NULL;
    }
    virtual void setEnvrInfo(stage_envr_info_class* i_EnvrInfo) {
        (void)i_EnvrInfo;
        dStage_SetErrorRoom();
        OSReport("Room non envr data !!\n");
        JUT_ASSERT(2148, FALSE);
    }
    virtual stage_envr_info_class* getEnvrInfo(void) const {
        OSReport("Room non envr data !!\n");
        JUT_ASSERT(2152, FALSE);
        return NULL;
    }
    virtual void setVrboxInfo(stage_vrbox_info_class* i_VrboxInfo) { mVrboxInfo = i_VrboxInfo; }
    virtual stage_vrbox_info_class* getVrboxInfo(void) const { return mVrboxInfo; }
    virtual void setVrboxcolInfo(stage_vrboxcol_info_class* i_VrboxcolInfo) { mVrboxcolInfo = i_VrboxcolInfo; }
    virtual stage_vrboxcol_info_class* getVrboxcolInfo(void) const { return mVrboxcolInfo; }
    virtual void setPlightInfo(stage_plight_info_class* i_PlightInfo) {
        (void)i_PlightInfo;
        dStage_SetErrorRoom();
        OSReport("Room non plight data !!\n");
        JUT_ASSERT(2174, FALSE);
    }
    virtual stage_plight_info_class* getPlightInfo(void) const {
        OSReport("Room non plight data !!\n");
        JUT_ASSERT(2178, FALSE);
        return NULL;
    }
    virtual void setPaletteNumInfo(int i_PaletteNumInfo) {
        (void)i_PaletteNumInfo;
        dStage_SetErrorRoom();
        OSReport("Room non palette num data !!\n");
        JUT_ASSERT(2186, FALSE);
    }
    virtual int getPaletteNumInfo(void) const {
        OSReport("Room non palette num data !!\n");
        JUT_ASSERT(2190, FALSE);
        return NULL;
    }
    virtual void setPselectNumInfo(int i_PselectNumInfo) {
        (void)i_PselectNumInfo;
        dStage_SetErrorRoom();
        OSReport("Room non pselect num data !!\n");
        JUT_ASSERT(2197, FALSE);
    }
    virtual int getPselectNumInfo(void) const {
        OSReport("Room non pselect num data !!\n");
        JUT_ASSERT(2201, FALSE);
        return NULL;
    }
    virtual void setEnvrNumInfo(int i_EnvrNumInfo) {
        (void)i_EnvrNumInfo;
        dStage_SetErrorRoom();
        OSReport("Room non envr num data !!\n");
        JUT_ASSERT(2208, FALSE);
    }
    virtual int getEnvrNumInfo(void) const {
        OSReport("Room non envr num data !!\n");
        JUT_ASSERT(2212, FALSE);
        return NULL;
    }
    virtual void setVrboxNumInfo(int i_VrboxNumInfo) { mVrboxNumInfo = i_VrboxNumInfo; }
    virtual int getVrboxNumInfo(void) const { return mVrboxNumInfo; }
    virtual void setVrboxcolNumInfo(int i_VrboxcolNumInfo) { mVrboxcolNumInfo = i_VrboxcolNumInfo; }
    virtual int getVrboxcolNumInfo(void) const { return mVrboxcolNumInfo; }
    virtual void setPlightNumInfo(int i_PlightNumInfo) {
        (void)i_PlightNumInfo;
        dStage_SetErrorRoom();
        OSReport("Room non plight num data !!\n");
        JUT_ASSERT(2223, FALSE);
    }
    virtual int getPlightNumInfo(void) const {
        OSReport("Room non plight num data !!\n");
        JUT_ASSERT(2227, FALSE);
        return NULL;
    }
    virtual void setLightVecInfo(stage_pure_lightvec_info_class* i_LightVecInfo) { mLightVecInfo = i_LightVecInfo; }
    virtual stage_pure_lightvec_info_class* getLightVecInfo(void) const { return mLightVecInfo; }
    virtual void setLightVecInfoNum(int i_LightVecInfoNum) { mLightVecInfoNum = i_LightVecInfoNum; }
    virtual int getLightVecInfoNum(void) const { return mLightVecInfoNum; }
    virtual void setStagInfo(stage_stag_info_class* i_StagInfo) {
        (void)i_StagInfo;
        dStage_SetErrorRoom();
        OSReport("Room non stag data !!\n");
        JUT_ASSERT(2256, FALSE);
    }
    virtual stage_stag_info_class* getStagInfo(void) const {
        OSReport("Room non stag data !!\n");
        JUT_ASSERT(2260, FALSE);
        return NULL;
    }
    virtual void setSclsInfo(stage_scls_info_dummy_class* i_SclsInfo) { mSclsInfo = i_SclsInfo; }
    virtual stage_scls_info_dummy_class* getSclsInfo(void) const { return mSclsInfo; }
    virtual void setPntInfo(dStage_dPnt_c* i_PntInfo) {
        (void)i_PntInfo;
        dStage_SetErrorRoom();
        OSReport("Room non Pnt data !\n");
        JUT_ASSERT(2281, FALSE);
    }
    virtual dStage_dPnt_c* getPntInf(void) const {
        OSReport("Room non Pnts data !\n");
        JUT_ASSERT(2285, FALSE);
        return NULL;
    }
    virtual void setPathInfo(dStage_dPath_c* i_PathInfo) {
        (void)i_PathInfo;
        dStage_SetErrorRoom();
        OSReport("Room non Path data !\n");
        JUT_ASSERT(2292, FALSE);
    }
    virtual dStage_dPath_c* getPathInf(void) const {
        OSReport("Room non Path data !\n");
        JUT_ASSERT(2296, FALSE);
        return NULL;
    }
    virtual void setPnt2Info(dStage_dPnt_c* i_Pnt2Info) { mPnt2Info = i_Pnt2Info; }
    virtual dStage_dPnt_c* getPnt2Inf(void) const { return mPnt2Info; }
    virtual void setPath2Info(dStage_dPath_c* i_Path2Info) { mPath2Info = i_Path2Info; }
    virtual dStage_dPath_c* getPath2Inf(void) const { return mPath2Info; }
    virtual void setSoundInf(dStage_SoundInfo_c* i_SoundInf) { mSoundInf = i_SoundInf; }
    virtual dStage_SoundInfo_c* getSoundInf(void) const { return mSoundInf; }
    virtual void setSoundInfCL(dStage_SoundInfo_c* i_SoundInfCL) { mSoundInfCL = i_SoundInfCL; }
    virtual dStage_SoundInfo_c* getSoundInfCL(void) const { return mSoundInfCL; }
    virtual void setMapEventInfo(dStage_MapEventInfo_c* i_MapEventInfo) { mMapEventInfo = i_MapEventInfo; }
    virtual dStage_MapEventInfo_c* getMapEventInfo(void) const { return mMapEventInfo; }
    virtual void setFileList2Info(dStage_FileList2_dt_c* i_FileList2Info) { mFileList2Info = i_FileList2Info; }
    virtual dStage_FileList2_dt_c* getFileList2Info(void) const { return mFileList2Info; }
    virtual void setFileListInfo(dStage_FileList_dt_c* i_FileListInfo) { mFileListInfo = i_FileListInfo; }
    virtual dStage_FileList_dt_c* getFileListInfo(void) const { return mFileListInfo; }
    virtual void setFloorInfo(dStage_FloorInfo_c* i_FloorInfo) { mFloorInfo = i_FloorInfo; }
    virtual dStage_FloorInfo_c* getFloorInfo(void) const { return mFloorInfo; }
    virtual void setMemoryConfig(dStage_MemoryConfig_c* i_MemoryConfig) {
        (void)i_MemoryConfig;
        OSReport("Room non memory config data!\n");
        JUT_ASSERT(2414, FALSE);
    }
    virtual dStage_MemoryConfig_c* getMemoryConfig(void) const {
        OSReport("Room non memory config data!\n");
        JUT_ASSERT(2423, FALSE);
        return NULL;
    }
    virtual void setMemoryMap(dStage_MemoryMap_c* i_MemoryMap) {
        (void)i_MemoryMap;
        OSReport("Room non memory map data!\n");
        JUT_ASSERT(2433, FALSE);
    }
    virtual dStage_MemoryMap_c* getMemoryMap(void) const {
        OSReport("Room non memory map data!\n");
        JUT_ASSERT(2442, FALSE);
        return NULL;
    }
    virtual void setMulti(dStage_Multi_c* i_Multi) {
        (void)i_Multi;
        OSReport("Room non multi data!\n");
        JUT_ASSERT(2452, FALSE);
    }
    virtual dStage_Multi_c* getMulti(void) const {
        OSReport("Room non multi data!\n");
        JUT_ASSERT(2457, FALSE);
        return NULL;
    }
    virtual void setOldMulti(void) {
        OSReport("Room non old multi data!\n");
        JUT_ASSERT(2462, FALSE);
    }
    virtual void resetOldMulti(void) {
        OSReport("Room non old multi data!\n");
        JUT_ASSERT(2467, FALSE);
    }
    virtual dStage_Multi_c* getOldMulti(void) const {
        OSReport("Room non old multi data!\n");
        JUT_ASSERT(2472, FALSE);
        return NULL;
    }
    virtual void setLbnk(dStage_Lbnk_c* i_Lbnk) { mLbnk = i_Lbnk; }
    virtual dStage_Lbnk_c* getLbnk(void) const { return mLbnk; }
    virtual void setTresure(stage_tresure_class* i_Tresure) { mTresure = i_Tresure; }
    virtual stage_tresure_class* getTresure(void) const { return mTresure; }
    virtual void setDMap(dStage_DMap_c* i_DMap) {
        (void)i_DMap;
        OS_REPORT("Room non DMap data\n");
        JUT_ASSERT(2508, FALSE);
    }
    virtual dStage_DMap_c* getDMap(void) const {
        OS_REPORT("Room non DMap data\n");
        JUT_ASSERT(2513, FALSE);
        return NULL;
    }
    virtual void setDrTg(stage_tgsc_class* i_DrTg) { mDrTg = i_DrTg; }
    virtual stage_tgsc_class* getDrTg(void) const { return mDrTg; }
    virtual void setDoor(stage_tgsc_class* i_Door) { mDoor = i_Door; }
    virtual stage_tgsc_class* getDoor(void) const { return mDoor; }
    virtual void setMapPath(void* i_MapPath) {
        (void)i_MapPath;
        OSReport("stage non 2d map path data !!\n");
        JUT_ASSERT(2557, FALSE);
    }
    virtual void* getMapPath(void) {
        OSReport("stage non 2d map path data !!\n");
        JUT_ASSERT(2561, FALSE);
        return NULL;
    }
    virtual void setElst(dStage_Elst_c* i_Elst) {
        (void)i_Elst;
        dStage_SetErrorRoom();
        OSReport("Room non envLayserSet data\n");
        JUT_ASSERT(2572, FALSE);
    }
    virtual dStage_Elst_c* getElst(void) {
        dStage_SetErrorRoom();
        OSReport("Room no Elst Data!!\n");
        return NULL;
    }

public:
    /* 0x08 */ stage_pure_lightvec_info_class* mLightVecInfo;
    /* 0x0C */ int mLightVecInfoNum;
    /* 0x10 */ stage_map_info_class* mMapInfo;
    /* 0x14 */ stage_map_info_dummy_class* mMapInfoBase;
    /* 0x18 */ stage_vrbox_info_class* mVrboxInfo;
    /* 0x1C */ stage_vrboxcol_info_class* mVrboxcolInfo;
    /* 0x20 */ dStage_FileList2_dt_c* mFileList2Info;
    /* 0x24 */ dStage_FileList_dt_c* mFileListInfo;
    /* 0x28 */ stage_actor_class* mPlayer;
    /* 0x2C */ dStage_dPnt_c* mPnt2Info;
    /* 0x30 */ dStage_dPath_c* mPath2Info;
    /* 0x34 */ stage_camera_class* mCamera;
    /* 0x38 */ stage_arrow_class* mArrow;
    /* 0x3C */ dStage_MapEventInfo_c* mMapEventInfo;
    /* 0x40 */ dStage_SoundInfo_c* mSoundInf;
    /* 0x44 */ dStage_SoundInfo_c* mSoundInfCL;
    /* 0x48 */ stage_scls_info_dummy_class* mSclsInfo;
    /* 0x4C */ dStage_Lbnk_c* mLbnk;
    /* 0x50 */ stage_tresure_class* mTresure;
    /* 0x54 */ stage_tgsc_class* mDrTg;
    /* 0x58 */ stage_tgsc_class* mDoor;
    /* 0x5C */ dStage_FloorInfo_c* mFloorInfo;
#if DEBUG
    /* 0x60 */ int field_0x60;
#endif
    /* 0x60 */ u16 mPlayerNum;
    /* 0x62 */ u16 field_0x62;
    /* 0x64 */ int mVrboxNumInfo;
    /* 0x68 */ int mVrboxcolNumInfo;
};  // Size: 0x6C

class dBgW_Base;

class dStage_roomStatus_c {
public:
    /* 0x000 */ dStage_roomDt_c mRoomDt;
    /* 0x06C */ dKy_tevstr_c mKyTevStr;
    /* 0x3F4 */ u8 mFlag;
    /* 0x3F5 */ bool mDraw;
    /* 0x3F6 */ s8 mZoneCount;
    /* 0x3F7 */ s8 mZoneNo;
    /* 0x3F8 */ s8 mMemBlockID;
    /* 0x3F9 */ u8 mRegionNo;
    /* 0x3FC */ int mProcID;
    /* 0x400 */ dBgW_Base* mpBgW;

    int getZoneNo() const { return mZoneNo; }
};  // Size: 0x404

STATIC_ASSERT(sizeof(dStage_roomStatus_c) == 0x404);

class dStage_roomControl_c {
public:
    class roomDzs_c {
    public:
        roomDzs_c() { m_num = 0; }
        void create(u8);
        void remove();
        void* add(u8, u8);

        /* 0x00 */ u8 m_num;
        /* 0x04 */ void** m_dzs;
    };

    struct nameData {
        /* 0x0 */ s8 m_num;
        /* 0x1 */ char m_names[32][10];  // ?
    };

    struct bankDataEntry {
        u8 field_0x0;
        u8 mLayerNo;
        u8 field_0x2[0x20];
    };

    struct bankData {
        /* 0x0 */ u8 m_num;
        /* 0x1 */ bankDataEntry m_entries[32];  // ?
    };

    static void createRoomDzs(u8 i_num) { m_roomDzs.create(i_num); }
    static void* addRoomDzs(u8 i_num, u8 roomNo) { return m_roomDzs.add(i_num, roomNo); }
    static void removeRoomDzs() { m_roomDzs.remove(); }

    dStage_roomControl_c() {}
    void init(void);
    static int getZoneNo(int i_roomNo) { return mStatus[i_roomNo].getZoneNo(); }
    void initZone();
    dStage_roomDt_c* getStatusRoomDt(int);
    static JKRExpHeap* getMemoryBlock(int);
    static void setStayNo(int);
    static void setNextStayNo(int);
    BOOL checkRoomDisp(int) const;
    int loadRoom(int, u8*, bool);
    void zoneCountCheck(int) const;
    static JKRExpHeap* createMemoryBlock(int, u32);
    static void destroyMemoryBlock();
    static void setArcBank(int, char const*);
    static char* getArcBank(int);
    static bool resetArchiveBank(int);
    static void SetTimePass(int i_TimePass) { m_time_pass = i_TimePass; }
    static void setZoneNo(int, int);
    static dBgp_c* getBgp(int i_roomNo) {
#if DEBUG
        // NONMATCHING
#else
        return NULL;
#endif
    }
    static BOOL GetTimePass() { return m_time_pass; }

    static s8 getStayNo() { return mStayNo; }
    static u8 getRegionNo(int i_roomNo) { return mStatus[i_roomNo].mRegionNo; }
    static s8 getMemoryBlockID(int i_roomNo) { return mStatus[i_roomNo].mMemBlockID; }
    dKy_tevstr_c* getTevStr(int i_roomNo) {
        JUT_ASSERT(2675, 0 <= i_roomNo && i_roomNo < 64);
        return &mStatus[i_roomNo].mKyTevStr;
    }
    void setStatusFlag(int i_roomNo, u8 flag) {
        JUT_ASSERT(2679, 0 <= i_roomNo && i_roomNo < 64);
        mStatus[i_roomNo].mFlag = flag;
    }
    static void onStatusDraw(int i_roomNo) { mStatus[i_roomNo].mDraw = true; }
    static dStage_FileList2_dt_c* getFileList2(int i_roomNo) {
        return mStatus[i_roomNo].mRoomDt.mFileList2Info;
    }
    static char* getDemoArcName() { return mDemoArcName; }
    static nameData* getArcBankName() { return mArcBankName; }
    static bankData* getArcBankData() { return mArcBankData; }
    static void setRoomReadId(s8 id) { mRoomReadId = id; }
    static s8 getRoomReadId() { return mRoomReadId; }
    static void offNoChangeRoom() { mNoChangeRoom = false; }
    static void onNoChangeRoom() { mNoChangeRoom = true; }
    static int getNextStayNo() { return mNextStayNo; }
    static void setProcID(u32 id) { mProcID = id; }
    static u32 getProcID() { return mProcID; }
    static void setStatusProcID(int i_roomNo, fpc_ProcID i_id) { mStatus[i_roomNo].mProcID = i_id; }
    static int getStatusProcID(int i_roomNo) { return mStatus[i_roomNo].mProcID; }
    static void setRegionNo(int i_roomNo, u8 i_regionNo) { mStatus[i_roomNo].mRegionNo = i_regionNo; }

    u8 checkStatusFlag(int i_roomNo, u8 flag) const {
        return cLib_checkBit(mStatus[i_roomNo].mFlag, flag);
    }

    void onStatusFlag(int i_roomNo, u8 flag) {
        JUT_ASSERT(2691, 0 <= i_roomNo && i_roomNo < 64);
        return cLib_onBit(mStatus[i_roomNo].mFlag, flag);
    }

    void offStatusFlag(int i_roomNo, u8 flag) {
        JUT_ASSERT(2695, 0 <= i_roomNo && i_roomNo < 64);
        return cLib_offBit(mStatus[i_roomNo].mFlag, flag);
    }

    static void setFileList2(int i_roomNo, dStage_FileList2_dt_c* list) {
        JUT_ASSERT(2711, 0 <= i_roomNo && i_roomNo < 64);
        mStatus[i_roomNo].mRoomDt.setFileList2Info(list);
    }

    static void setZoneCount(int i_roomNo, int count) {
        JUT_ASSERT(2737, 0 <= i_roomNo && i_roomNo < 64);
        mStatus[i_roomNo].mZoneCount = count;
    }

    static void setMemoryBlockID(int i_roomNo, int i_blockID) {
        JUT_ASSERT(2753, 0 <= i_roomNo && i_roomNo < 64);
        mStatus[i_roomNo].mMemBlockID = i_blockID;
    }

    static void setBgW(int i_roomNo, dBgW_Base* i_bgw) {
        JUT_ASSERT(2778, 0 <= i_roomNo && i_roomNo < 64);
        mStatus[i_roomNo].mpBgW = i_bgw;
    }

    static JKRExpHeap* getMemoryBlockHeap(int i_no) { return mMemoryBlock[i_no]; }

    static const int MEMORY_BLOCK_MAX = 19;

    static JKRExpHeap* mMemoryBlock[MEMORY_BLOCK_MAX];
    static char mArcBank[32][10];
    static dStage_roomStatus_c mStatus[0x40];
    static char mDemoArcName[10];
    static u32 mProcID;
    static nameData* mArcBankName;
    static bankData* mArcBankData;
    static roomDzs_c m_roomDzs;
    static s8 mStayNo;
    static s8 mOldStayNo;
    static s8 mNextStayNo;
    static u8 m_time_pass;
    static u8 mNoChangeRoom;
    static s8 mRoomReadId;

    #if DEBUG
    static void onNoArcBank() {
        mNoArcBank = true;
    } 

    static u8 mNoArcBank;
    #endif

private:
    /* 0x0 */ u8 field_0x0[4];
};

class dStage_startStage_c {
public:
    void set(const char*, s8, s16, s8);
    const char* getName() const { return mName; }
    s16 getPoint() const { return mPoint; }
    s8 getRoomNo() const { return mRoomNo; }
    s8 getLayer() const { return mLayer; }
    void setLayer(s8 layer) { mLayer = layer; }
    s8 getDarkArea() const { return mDarkArea; }
    void setDarkArea(s8 darkArea) { mDarkArea = darkArea; }

private:
    /* 0x0 */ char mName[8];
    /* 0x8 */ s16 mPoint;
    /* 0xA */ s8 mRoomNo;
    /* 0xB */ s8 mLayer;
    /* 0xC */ s8 mDarkArea;
};

class dStage_nextStage_c : public dStage_startStage_c {
public:
    dStage_nextStage_c() {
        enabled = 0;  // TODO: maybe wrong
    }
    void set(const char*, s8, s16, s8, s8, u8);
    void offEnable() { enabled = 0; }
    BOOL isEnable() const { return enabled; }
    s8 getWipe() const { return wipe; }
    u8 getWipeSpeed() const { return wipe_speed; }

private:
    s8 enabled;
    s8 wipe;
    u8 wipe_speed;
};

// unknown name
struct dStage_objectNameInf {
    /* 0x00 */ char name[8];
    /* 0x08 */ s16 procname;
    /* 0x0A */ s8 argument;
};  // Size: 0xC

class dStage_KeepDoorInfo {
public:
    /* 0x000 */ int mNum;
    /* 0x004 */ stage_tgsc_data_class mDrTgData[0x40];
};  // Size = 0x904

typedef int (*dStage_Func)(dStage_dt_c*, void*, int, void*);

struct FuncTable {
    char identifier[5];
    dStage_Func function;
};

const char* dStage_getName2(s16, s8);
dStage_objectNameInf* dStage_searchName(const char*);
static int dStage_stageKeepTresureInit(dStage_dt_c*, void*, int, void*);
static int dStage_filiInfo2Init(dStage_dt_c*, void*, int, void*);
static int dStage_mapPathInitCommonLayer(dStage_dt_c*, void*, int, void*);
static int dStage_mapPathInit(dStage_dt_c*, void*, int, void*);
static int dStage_memaInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_mecoInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_stagInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_soundInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_lgtvInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_envrInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_pselectInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_paletteInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_vrboxcolInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_roomDrtgInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_tgscInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_actorInit(dStage_dt_c*, void*, int, void*);
static int dStage_layerTresureInit(dStage_dt_c*, void*, int, void*);
static int dStage_elstInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_rppnInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_rpatInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_multInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_playerInit(dStage_dt_c*, void*, int, void*);
static int dStage_cameraInit(dStage_dt_c*, void*, int, void*);
static int dStage_actorInit_always(dStage_dt_c*, void*, int, void*);
static int dStage_roomReadInit(dStage_dt_c*, void*, int, void*);
static int dStage_arrowInit(dStage_dt_c*, void*, int, void*);
static int dStage_vrboxInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_sclsInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_tgscInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_plightInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_ppntInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_pathInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_filiInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_stageDrtgInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_floorInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_dmapInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_stEventInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_soundInfoInitCL(dStage_dt_c*, void*, int, void*);
static int dStage_RoomCameraInit(dStage_dt_c*, void*, int, void*);
static int dStage_lbnkInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_actorCommonLayerInit(dStage_dt_c*, void*, int, void*);
static int dStage_roomTresureInit(dStage_dt_c*, void*, int, void*);
static int dStage_tgscCommonLayerInit(dStage_dt_c*, void*, int, void*);
static int dStage_mapEventInfoInit(dStage_dt_c*, void*, int, void*);
static int dStage_fieldMapTresureInit(dStage_dt_c*, void*, int, void*);
static int dStage_fieldMapFiliInfo2Init(dStage_dt_c*, void*, int, void*);
static int dStage_fieldMapMapPathInit(dStage_dt_c*, void*, int, void*);

s8 dStage_roomRead_dt_c_GetReverbStage(roomRead_class&, int);
int dStage_changeScene(int i_exitId, f32 i_speed, u32 i_mode, s8 i_roomNo, s16 i_angle, int i_layerOverride);
void dStage_infoCreate();
u32 dStage_stagInfo_GetParticleNo(stage_stag_info_class* p_info, int layer);
int dStage_changeSceneExitId(cBgS_PolyInfo& param_0, f32 speed, u32 mode, s8 roomNo, s16 angle);
int dStage_changeScene4Event(int i_exitId, s8 room_no, int i_wipe, bool param_3, f32 speed,
                             u32 mode, s16 angle, int param_7);
void dStage_Create();
void dStage_Delete();
void dStage_restartRoom(u32 roomParam, u32 mode, int param_2);
int dStage_RoomCheck(cBgS_GndChk* gndChk);
void dStage_dt_c_roomReLoader(void* i_data, dStage_dt_c* stageDt, int param_2);
void dStage_dt_c_roomLoader(void* i_data, dStage_dt_c* stageDt, int param_2);
dStage_KeepDoorInfo* dStage_GetKeepDoorInfo();
dStage_KeepDoorInfo* dStage_GetRoomKeepDoorInfo();
void dStage_dt_c_fieldMapLoader(void* i_data, dStage_dt_c* i_stage);
const char* dStage_getName(s16 procName, s8 argument);

#if VERSION == VERSION_WII_USA_R0
void dStage_escapeRestart();
#endif

inline s32 dStage_roomRead_dt_c_ChkBg(u8 param_0) {
    return param_0 & 0x80;
}

inline s32 dStage_roomRead_dt_c_GetVrboxswitch(roomRead_data_class& data) {
    return data.field_0x2 & 8;
}

inline s32 dStage_roomRead_dt_c_GetTimePass(roomRead_data_class& data) {
    return data.field_0x2 & 3;
}

inline u8 dStage_roomRead_dt_c_GetLoadRoomIndex(u8 param_0) {
    return param_0 & 0x3f;
}

inline u32 dStage_roomRead_dt_c_GetReverb(roomRead_data_class& room) {
    return room.field_0x1 & 0x7F;
}

inline u32 dStage_stagInfo_GetSTType(stage_stag_info_class* pstag) {
    return (pstag->field_0x0c >> 16) & 7;
}

inline int dStage_stagInfo_GetEscapeWarp(stage_stag_info_class* pstag) {
    return (pstag->field_0x10 >> 24);
}

inline u32 dStage_stagInfo_GetMiniMap(stage_stag_info_class* pstag) {
    return (pstag->field_0x0a >> 0xD) & 7;
}

inline u32 dStage_stagInfo_GetParticleNo(stage_stag_info_class* p_info) {
    return (p_info->field_0x0a >> 0x3) & 0xFF;
}

inline s16 dStage_stagInfo_GetUpButton(stage_stag_info_class* p_info) {
    return p_info->field_0x0a & 7;
}

inline u32 dStage_stagInfo_GetArg0(stage_stag_info_class* p_info) {
    return (p_info->field_0x0c >> 0x14) & 0xFF;
}

inline int dStage_stagInfo_GetMsgGroup(stage_stag_info_class* p_info) {
    return p_info->mMsgGroup;
}

inline s32 dStage_stagInfo_GetSaveTbl(stage_stag_info_class* param_0) {
    return param_0->field_0x09 >> 1 & 0x1f;
}

inline int dStage_stagInfo_GetTimeH(stage_stag_info_class* p_info) {
    s8 time = (p_info->field_0x0c >> 8) & 0xFF;
    return time;
}

inline BOOL dStage_staginfo_GetArchiveHeap(stage_stag_info_class* p_info) {
    return p_info->field_0x0a & 0x1000;
}

inline int dStage_stagInfo_GetGapLevel(stage_stag_info_class* pstag) {
    return pstag->mGapLevel;
}

inline int dStage_stagInfo_GetRangeUp(stage_stag_info_class* pstag) {
    return pstag->mRangeUp;
}

inline int dStage_stagInfo_GetRangeDown(stage_stag_info_class* pstag) {
    return pstag->mRangeDown;
}

inline u32 dStage_stagInfo_ChkKeyDisp(stage_stag_info_class* pstag) {
    return pstag->field_0x09 & 1;
}

inline u8 dStage_stagInfo_GetWolfDashType(stage_stag_info_class* pstag) {
    return (pstag->field_0x09 >> 6) & 3;
}

inline u16 dStage_stagInfo_GetStageTitleNo(stage_stag_info_class* pstag) {
    return pstag->mStageTitleNo;
}

inline int dStage_stagInfo_DefaultCameraType(stage_stag_info_class* pstag) {
    return pstag->mCameraType;
}

inline u16 dStage_stagInfo_GetCullPoint(stage_stag_info_class* pstag) {
    return pstag->field_0x10 & 0xFFFF;
}

inline u8 dStage_sclsInfo_getSceneLayer(stage_scls_info_class* p_info) {
    return p_info->field_0xb & 0xF;
}

inline s32 dStage_sclsInfo_getWipe(stage_scls_info_class* p_info) {
    return p_info->mWipe;
}

inline s32 dStage_sclsInfo_getWipeTime(stage_scls_info_class* p_info) {
    return (p_info->field_0xb >> 5) & 7;
}

inline int dStage_sclsInfo_getTimeH(stage_scls_info_class* p_info) {
    return ((p_info->field_0xa >> 4) & 0xF) | (p_info->field_0xb & 0x10);
}

inline u32 dStage_FileList_dt_getMiniMap(dStage_FileList_dt_c* p_fList) {
    return p_fList->mParameters >> 3 & 7;
}

inline u32 dStage_FileList_dt_GetEnemyAppear1Flag(dStage_FileList_dt_c* p_fList) {
    return p_fList->mParameters & 0x20000000;
}

inline u8 dStage_FileList_dt_GetBitSw(dStage_FileList_dt_c* p_fList) {
    return p_fList->mBitSw;
}

inline f32 dStage_FileList_dt_SeaLevel(dStage_FileList_dt_c* p_fList) {
    return p_fList->mSeaLevel;
}

inline int dStage_FileList_dt_GlobalWindLevel(dStage_FileList_dt_c* i_fili) {
    return (i_fili->mParameters >> 0x12) & 3;
}

inline int dStage_FileList_dt_GlobalWindDir(dStage_FileList_dt_c* i_fili) {
    return (i_fili->mParameters >> 0xF) & 7;
}

inline u8 dStage_FileList_dt_GetDefaultCamera(dStage_FileList_dt_c* p_fList) {
    return p_fList->mDefaultCamera;
}

inline int dStage_FileList_dt_GRASSLIGHT(dStage_FileList_dt_c* p_fList) {
    return (p_fList->mParameters >> 7) & 0xFF;
}

inline u16 dStage_FileList_dt_GetMsg(dStage_FileList_dt_c* p_fList) {
    return p_fList->mMsg;
}

inline f32 dStage_FileList2_dt_GetLeftRmX(dStage_FileList2_dt_c* p_fList2) {
    return p_fList2->mLeftRmX;
}

inline f32 dStage_FileList2_dt_GetRightRmX(dStage_FileList2_dt_c* p_fList2) {
    return p_fList2->mRightRmX;
}

inline f32 dStage_FileList2_dt_GetInnerRmZ(dStage_FileList2_dt_c* p_fList2) {
    return p_fList2->mInnerRmZ;
}

inline f32 dStage_FileList2_dt_GetFrontRmZ(dStage_FileList2_dt_c* p_fList2) {
    return p_fList2->mFrontRmZ;
}

inline s8 dStage_FileList2_dt_GetMinFloorNo(dStage_FileList2_dt_c* p_fList2) {
    return p_fList2->mMinFloorNo;
}

inline s8 dStage_FileList2_dt_GetMaxFloorNo(dStage_FileList2_dt_c* p_fList2) {
    return p_fList2->mMaxFloorNo;
}

inline int dStage_MapEvent_dt_c_getEventSCutSW(dStage_MapEvent_dt_c* event) {
    return event->field_0x8 & 1;
}

inline int dStage_MapEvent_dt_c_getEventSCutType(dStage_MapEvent_dt_c* event) {
    return event->field_0x8 >> 1 & 3;
}

inline int dStage_lbnkWIND(dStage_Lbnk_dt_c* i_data) {
    return (i_data->field_0x2 >> 3) & 7;
}

inline int dStage_lbnkWlevel(dStage_Lbnk_dt_c* i_data) {
    return (i_data->field_0x2 >> 6) & 3;
}

#endif /* D_D_STAGE_H */
