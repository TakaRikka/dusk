#ifndef D_KANKYO_D_KANKYO_H
#define D_KANKYO_D_KANKYO_H

#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_kankyo_data.h"
#include "d/d_kankyo_tev_str.h"
#include "d/d_kankyo_wether.h"
#include "f_pc/f_pc_base.h"
#include "m_Do/m_Do_ext.h"

class JPABaseEmitter;
class cBgS_PolyInfo;
class color_RGB_class;

struct LIGHT_INFLUENCE {
    /* 0x00 */ cXyz mPosition;
    /* 0x0C */ GXColorS10 mColor;
    /* 0x14 */ f32 mPow;
    /* 0x18 */ f32 mFluctuation;
    /* 0x1C */ int mIndex;
};

struct WIND_INFLUENCE {
    /* 0x00 */ cXyz position;
    /* 0x0C */ cXyz mDirection;
    /* 0x18 */ f32 mRadius;
    /* 0x1C */ f32 mStrength;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ int field_0x24;
    /* 0x28 */ u8 mConstant;
    /* 0x29 */ u8 field_0x29;
};

struct WIND_INF_ENTITY {
    /* 0x00 */ u8 mInUse;
    /* 0x04 */ f32 mMinRadius;
    /* 0x08 */ f32 mSpeed;
    /* 0x0C */ f32 mStrengthMaxVel;
    /* 0x10 */ WIND_INFLUENCE mInfluence;
};  // Size: 0x3C

struct EFLIGHT_PROC {
    /* 0x00 */ u8 mState;
    /* 0x01 */ u8 mFrame;
    /* 0x04 */ int mLightType;
    /* 0x08 */ LIGHT_INFLUENCE field_0x8;
};

struct SND_INFLUENCE {
    /* 0x00 */ cXyz position;
    /* 0x0C */ int field_0xc;
    /* 0x10 */ int timer;
    /* 0x14 */ fpc_ProcID actor_id;
};

struct DALKMIST_INFLUENCE {
    /* 0x00 */ cXyz mPos;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ s8 mIndex;
};

struct DUNGEON_LIGHT {
    /* 0x00 */ cXyz mPosition;
    /* 0x0C */ GXColor mColor;
    /* 0x10 */ f32 mRefDistance;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 mCutoffAngle;
    /* 0x1C */ f32 mAngleX;
    /* 0x20 */ f32 mAngleY;
    /* 0x24 */ u8 mAngleAttenuation;
    /* 0x25 */ u8 mDistAttenuation;
    /* 0x26 */ u8 field_0x26[6];
    /* 0x2C */ LIGHT_INFLUENCE mInfluence;
};  // Size: 0x4C

struct BOSS_LIGHT {
    /* 0x00 */ cXyz mPos;
    /* 0x0C */ GXColor mColor;
    /* 0x10 */ f32 mRefDistance;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 mCutoffAngle;
    /* 0x1C */ f32 mAngleX;
    /* 0x20 */ f32 mAngleY;
    /* 0x24 */ u8 mAngleAttenuation;
    /* 0x25 */ u8 mDistAttenuation;
    /* 0x26 */ u8 field_0x26;
};  // Size: 0x28

struct GB_WIND_INFLUENCE {
    /* 0x00 */ cXyz vec;
    /* 0x0C */ cXyz* vec_override;
    /* 0x10 */ f32 pow;
};

struct EF_THUNDER {
    /* 0x00 */ u8 mStatus;
    /* 0x01 */ u8 mState;
    /* 0x02 */ u8 field_0x2;
    /* 0x04 */ int mMode;
    /* 0x08 */ f32 mFlashTimer;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ u8 field_0x20[4];
    /* 0x24 */ LIGHT_INFLUENCE mLightInfluence;
};

struct GB_MAPLE_COL_CHANGE {
    /* 0x00 */ J3DLightObj light_obj;
};  // Size: 0x74

struct NAVYCHAN {
    /* 0x0 */ u8 field_0x0[0x4 - 0x0];
    /* 0x4 */ cXyz field_0x4;
};

struct LightStatus {
    /* 0x00 */ Vec position;
    /* 0x0C */ Vec field_0xc;
    /* 0x18 */ GXColor color;
    /* 0x1C */ int field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ Vec mLightDir;
    /* 0x44 */ f32 mCutoff;
    /* 0x48 */ GXSpotFn mSpotFn;
    /* 0x4C */ f32 mRefDist;
    /* 0x50 */ f32 mRefBrightness;
    /* 0x54 */ GXDistAttnFn mDistFn;
    /* 0x58 */ u32 field_0x58[2][6];  //?
    /* 0x88 */ f32 field_0x88;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ u32 field_0x98[2][8];  //?
    /* 0xD8 */ f32 field_0xd8;
    /* 0xDC */ f32 field_0xdc;
    /* 0xE0 */ f32 field_0xe0;
    /* 0xE4 */ f32 field_0xe4;
};  // Size: 0xE8

class fopAc_ac_c;
class dKyd_lightSchejule;
class stage_palette_info_class;
class stage_pselect_info_class;
class stage_envr_info_class;
class stage_vrboxcol_info_class;
class stage_plight_info_class;
class kytag08_class;

struct dKy_pol_arg_struct {
    u8 data[8];
};

struct dKy_color_data_struct {
    u8 data[8];
};

struct dKy_pol_efftbl_struct {
    u8 data[8];
};

struct dKy_pol_effcol_struct {
    u8 data[8];
};

struct dKy_pol_sound_struct {
    u8 data[8];
};

enum dKy_LIGHT_SIZE {
    LIGHT_SIZE_S,
    LIGHT_SIZE_M,
    LIGHT_SIZE_L,
    LIGHT_SIZE_LL,
};

struct dKy_Odour_Data {
    /* 0x0 */ u8 mOdourPacketStatus;
    /* 0x1 */ u8 field_0xf21;
    /* 0x4 */ int field_0xf24;
    /* 0x8 */ dKankyo_odour_Packet* mpOdourPacket;
};

struct dKy_flush_info {
    u8 start_frame;
    u8 r;
    u8 g;
    u8 b;
};

class dScnKy_env_light_c {
public:
    dScnKy_env_light_c();
    void setDaytime();
    void setSunpos();
    f32 getDaytime();
    f32 getDarkDaytime();
    void setLight_palno_get(u8*, u8*, u8*, u8*, u8*, u8*, u8*, u8*, f32*, int*, int*,
                                           f32*, u8*);
    void setLight();
    void setLight_bg(dKy_tevstr_c*, GXColorS10*, GXColorS10*, f32*, f32*);
    void setLight_actor(dKy_tevstr_c*, GXColorS10*, f32*, f32*);
    void settingTevStruct_colget_actor(cXyz*, dKy_tevstr_c*, GXColorS10*,
                                                      GXColorS10*, f32*, f32*);
    void settingTevStruct_colget_player(dKy_tevstr_c*);
    void settingTevStruct_plightcol_plus(cXyz*, dKy_tevstr_c*, GXColorS10,
                                                        GXColorS10, u8);
    void settingTevStruct(int, cXyz*, dKy_tevstr_c*);
    void setLightTevColorType(J3DModelData*, dKy_tevstr_c*);
    void setLightTevColorType_MAJI(J3DModelData*, dKy_tevstr_c*);
    void CalcTevColor();
    void Sndpos();
    void Eflight_flush_proc();
    void SetBaseLight();
    void exeKankyo();
    void drawKankyo();

    virtual ~dScnKy_env_light_c() {}

    void setLightTevColorType(J3DModel* model_p, dKy_tevstr_c* tevstr_p) {
        setLightTevColorType(model_p->getModelData(), tevstr_p);
    }
    void setLightTevColorType_MAJI(J3DModel* model_p, dKy_tevstr_c* tevstr_p) {
        setLightTevColorType_MAJI(model_p->getModelData(), tevstr_p);
    }

public:
    /* 0x0000 */  // vtable
    /* 0x0004 */ stage_palette_info_class* stage_palette_info;
    /* 0x0008 */ stage_pselect_info_class* stage_pselect_info;
    /* 0x000C */ stage_envr_info_class* stage_envr_info;
    /* 0x0010 */ stage_vrboxcol_info_class* stage_vrboxcol_info;
    /* 0x0014 */ dKyd_lightSchejule* light_schedule;
    /* 0x0018 */ LIGHT_INFLUENCE base_light;
    /* 0x0038 */ LIGHT_INFLUENCE mLightInfluence[30];
    /* 0x03F8 */ LIGHT_INFLUENCE* pointlight[100];
    /* 0x0588 */ LIGHT_INFLUENCE* efplight[5];
    /* 0x059C */ u8 unk_0x59c[0x72C - 0x59C];
    /* 0x072C */ LIGHT_INFLUENCE* unk_0x72c[5];  // only ever set to NULL in plight_init
    /* 0x0740 */ LIGHT_INFLUENCE unk_0x740;
    /* 0x0760 */ u8 unk_0x760[0x764 - 0x760];
    /* 0x0764 */ LIGHT_INFLUENCE bgparts_active_light[2];
    /* 0x07A4 */ EFLIGHT_PROC eflight;
    /* 0x07CC */ stage_plight_info_class* stage_plight_info;
    /* 0x07D0 */ WIND_INFLUENCE* pntwind[30];
    /* 0x0848 */ WIND_INF_ENTITY wind_inf_entity[5];
    /* 0x0974 */ SND_INFLUENCE sound_influence;
    /* 0x098C */ u8 player_room_no;
    /* 0x0990 */ DALKMIST_INFLUENCE* dalkmist_influence[10];
    /* 0x09B8 */ DUNGEON_LIGHT dungeonlight[8];
    /* 0x0C18 */ BOSS_LIGHT field_0x0c18[8];
    /* 0x0D58 */ BOSS_LIGHT field_0x0d58[6];
#if DEBUG
    /* 0x0E48 */ NAVYCHAN navy;
    /* 0x0E58 */ u8 field_0xe58[0xE68 - 0xE58];  // part of NAVYCHAN?
#endif
    /* 0x0E48 */ GB_WIND_INFLUENCE global_wind_influence;
    /* 0x0E5C */ f32 custom_windpower;
    /* 0x0E60 */ f32 unk_0xe60;
    /* 0x0E64 */ u16 unk_0xe64;
    /* 0x0E66 */ u16 unk_0xe66;
    /* 0x0E68 */ s16 evt_wind_angle_x;
    /* 0x0E6A */ s16 evt_wind_angle_y;
    /* 0x0E6C */ u8 unk_0xe6c;
    /* 0x0E6D */ u8 evt_wind_go;
    /* 0x0E6E */ u8 TeachWind_existence;
    /* 0x0E6F */ u8 unk_0xe6f;
    /* 0x0E70 */ bool mSunInitialized;
    /* 0x0E74 */ dKankyo_sun_Packet* mpSunPacket;
    /* 0x0E78 */ dKankyo_sunlenz_Packet* mpSunLenzPacket;
    /* 0x0E7C */ bool mRainInitialized;
    /* 0x0E80 */ int raincnt;
    /* 0x0E84 */ dKankyo_rain_Packet* mpRainPacket;
    /* 0x0E88 */ bool mSnowInitialized;
    /* 0x0E8C */ int mSnowCount;
    /* 0x0E90 */ u8 field_0xe90;
    /* 0x0E91 */ u8 field_0xe91;
    /* 0x0E92 */ bool field_0xe92;
    /* 0x0E94 */ dKankyo_snow_Packet* mpSnowPacket;
    /* 0x0E98 */ bool mStarInitialized;
    /* 0x0E9C */ int mStarCount;
    /* 0x0EA0 */ f32 mStarDensity;
    /* 0x0EA4 */ dKankyo_star_Packet* mpStarPacket;
    /* 0x0EA8 */ bool mHousiInitialized;
    /* 0x0EA9 */ u8 field_0xea9;
    /* 0x0EAC */ int mHousiCount;
    /* 0x0EB0 */ dKankyo_housi_Packet* mpHousiPacket;
    /* 0x0EB4 */ u8 mCloudInitialized;
    /* 0x0EB5 */ u8 mMoyaMode;
    /* 0x0EB8 */ int mMoyaCount;
    /* 0x0EBC */ f32 field_0xebc;
    /* 0x0EC0 */ dKankyo_cloud_Packet* mpCloudPacket;
    /* 0x0EC4 */ u8 mVrkumoStatus;
    /* 0x0EC8 */ int mVrkumoCount;
    /* 0x0ECC */ f32 mVrkumoStrength;
    /* 0x0ED0 */ dKankyo_vrkumo_Packet* mpVrkumoPacket;
    /* 0x0ED4 */ EF_THUNDER mThunderEff;
    /* 0x0F18 */ u8 field_0xf18[4];
    /* 0x0F1C */ dKankyo_shstar_Packet* mpShstarPacket;
    /* 0x0F20 */ dKy_Odour_Data mOdourData;
    /* 0x0F2C */ u8 field_0xf2c;
    /* 0x0F30 */ J3DModel* undwater_ef_model;
    /* 0x0F34 */ f32 field_0xf34;
    /* 0x0F38 */ JKRSolidHeap* undwater_ef_heap;
    /* 0x0F3C */ u8 senses_mode;
    /* 0x0F3D */ u8 now_senses_effect;
    /* 0x0F40 */ f32 senses_effect_strength;
    /* 0x0F44 */ JPABaseEmitter* senses_ef_emitter0;
    /* 0x0F48 */ JPABaseEmitter* senses_ef_emitter1;
    /* 0x0F4C */ JPABaseEmitter* senses_ef_emitter2;
    /* 0x0F50 */ GB_MAPLE_COL_CHANGE global_maple_col_change[2];
    /* 0x1038 */ u8 field_0x1038;
    /* 0x103C */ int field_0x103c;
    /* 0x1040 */ u8 field_0x1040[4];
    /* 0x1044 */ u8 mMudInitialized;
    /* 0x1048 */ int field_0x1048;
    /* 0x104C */ dKankyo_mud_Packet* mpMudPacket;
    /* 0x1050 */ u8 mEvilInitialized;
    /* 0x1051 */ u8 field_0x1051;
    /* 0x1054 */ int field_0x1054;
    /* 0x1058 */ dKankyo_evil_Packet* mpEvilPacket;
    /* 0x105C */ mDoExt_btkAnm* undwater_btk;
    /* 0x1060 */ kytag08_class* fog_avoid_tag;
    /* 0x1064 */ Vec field_0x1064;  // some light direction
    /* 0x1070 */ cXyz sun_light_pos;
    /* 0x107C */ cXyz plight_near_pos;
    /* 0x1088 */ cXyz sun_pos;
    /* 0x1094 */ cXyz moon_pos;
    /* 0x10A0 */ cXyz field_0x10a0;
    /* 0x10AC */ GXFogAdjTable mXFogTbl;
    /* 0x10C0 */ GXColorS10 vrbox_sky_col;
    /* 0x10C8 */ GXColorS10 vrbox_kumo_top_col;
    /* 0x10D0 */ GXColorS10 vrbox_kumo_bottom_col;
    /* 0x10D8 */ GXColorS10 vrbox_kumo_shadow_col;
    /* 0x10E0 */ GXColorS10 vrbox_kasumi_outer_col;
    /* 0x10E8 */ GXColorS10 vrbox_kasumi_inner_col;
    /* 0x10F0 */ GXColorS10 field_0x10f0;
    /* 0x10F8 */ GXColorS10 field_0x10f8;
    /* 0x1100 */ GXColorS10 actor_amb_col;
    /* 0x1108 */ GXColorS10 bg_amb_col[4];
    /* 0x1128 */ GXColorS10 dungeonlight_col[6];
    /* 0x1158 */ GXColorS10 fog_col;
    /* 0x1160 */ GXColorS10 actor_addcol_amb;
    /* 0x1168 */ u8 unk_0x1168[8];  // probably unused GXColorS10
    /* 0x1170 */ GXColorS10 bg_addcol_amb;
    /* 0x1178 */ u8 unk_0x1178[8];  // probably unused GXColorS10
    /* 0x1180 */ GXColorS10 bg1_addcol_amb;
    /* 0x1188 */ u8 unk_0x1188[8];  // probably unused GXColorS10
    /* 0x1190 */ GXColorS10 bg2_addcol_amb;
    /* 0x1198 */ u8 unk_0x1198[8];  // probably unused GXColorS10
    /* 0x11A0 */ GXColorS10 bg3_addcol_amb;
    /* 0x11A8 */ u8 unk_0x11a8[8];  // probably unused GXColorS10
    /* 0x11B0 */ GXColorS10 addcol_fog;
    /* 0x11B8 */ GXColorS10 vrbox_addcol_sky0;
    /* 0x11C0 */ GXColorS10 vrbox_addcol_kasumi;
    /* 0x11C8 */ f32 unk_0x11c8;  // used as a ratio, but only ever set to 1.0
    /* 0x11CC */ f32 unk_0x11cc;  // used as a ratio, but only ever set to 1.0
    /* 0x11D0 */ f32 unk_0x11d0;  // used as a ratio, but only ever set to 1.0
    /* 0x11D4 */ f32 dice_wether_change_time;
    /* 0x11D8 */ f32 dice_wether_time;
    /* 0x11DC */ f32 pat_ratio;
    /* 0x11E0 */ f32 mColPatBlendGather;
    /* 0x11E4 */ f32 mFogNear;
    /* 0x11E8 */ f32 mFogFar;
    /* 0x11EC */ f32 field_0x11ec;
    /* 0x11F0 */ f32 field_0x11f0;
    /* 0x11F4 */ f32 field_0x11f4;
    /* 0x11F8 */ f32 now_allcol_ratio;
    /* 0x11FC */ f32 now_actcol_ratio;
    /* 0x1200 */ f32 now_bgcol_ratio;
    /* 0x1204 */ f32 now_fogcol_ratio;
    /* 0x1208 */ f32 now_vrboxsoracol_ratio;
    /* 0x120C */ f32 now_vrboxkumocol_ratio;
    /* 0x1210 */ f32 field_0x1210;
    /* 0x1214 */ f32 allcol_ratio;
    /* 0x1218 */ f32 actcol_ratio;
    /* 0x121C */ f32 bgcol_ratio;
    /* 0x1220 */ f32 fogcol_ratio;
    /* 0x1224 */ f32 vrboxsoracol_ratio;
    /* 0x1228 */ f32 vrboxkumocol_ratio;
    /* 0x122C */ f32 field_0x122c;
    /* 0x1230 */ f32 field_0x1230;
    /* 0x1234 */ f32 field_0x1234;
    /* 0x1238 */ f32 field_0x1238;
    /* 0x123C */ f32 field_0x123c;
    /* 0x1240 */ f32 field_0x1240;
    /* 0x1244 */ f32 daytime;
    /* 0x1248 */ f32 nexttime;
    /* 0x124C */ f32 time_change_rate;
    /* 0x1250 */ f32 dark_daytime;
    /* 0x1254 */ f32 field_0x1254;
    /* 0x1258 */ f32 field_0x1258;
    /* 0x125C */ f32 bg_light_influence;
    /* 0x1260 */ f32 grass_light_inf_rate;
    /* 0x1264 */ f32 field_0x1264;
    /* 0x1268 */ f32 field_0x1268;
    /* 0x126C */ f32 field_0x126c;
    /* 0x1270 */ f32 mDemoAttentionPoint;
    /* 0x1274 */ f32 old_time;  // saved time of day before entering twilight. time set back to this when leaving twilight
    /* 0x1278 */ f32 field_0x1278;
    /* 0x127C */ f32 field_0x127c;
    /* 0x1280 */ int field_0x1280;
    /* 0x1284 */ u8 field_0x1284[4];
    /* 0x1288 */ u32 moya_se;
    /* 0x128C */ u8 unk_0x128c[0x1298 - 0x128C];
    /* 0x1298 */ int dice_wether_counter;
    /* 0x129C */ int base_raincnt;
    /* 0x12A0 */ int field_0x12a0;
    /* 0x12A4 */ int mPlayerPLightIdx;
    /* 0x12A8 */ int mPlayerEfLightIdx;
    /* 0x12AC */ u8 field_0x12ac[0x12B0 - 0x12AC];
    /* 0x12B0 */ int field_0x12b0;
    /* 0x12B4 */ int schbit_timer;
    /* 0x12B8 */ u8 field_0x12b8[0x12BC - 0x12B8];
    /* 0x12BC */ u16 mFogAdjCenter;
    /* 0x12BE */ u16 mDate;
    /* 0x12C0 */ s16 mActorLightEffect;
    /* 0x12C2 */ u8 wether_pat0;
    /* 0x12C3 */ u8 wether_pat1;
    /* 0x12C4 */ u8 mColpatPrevGather;
    /* 0x12C5 */ u8 mColpatCurrGather;
    /* 0x12C6 */ u8 PrevCol;
    /* 0x12C7 */ u8 UseCol;
    /* 0x12C8 */ u8 mColpatWeather;
    /* 0x12C9 */ u8 dice_wether_mode;
    /* 0x12CA */ u8 dice_wether_state;
    /* 0x12CB */ u8 dice_wether_pat;
    /* 0x12CC */ u8 wether;
    /* 0x12CD */ u8 mColPatMode;
    /* 0x12CE */ u8 mColPatModeGather;
    /* 0x12CF */ u8 mFogAdjEnable;
    /* 0x12D0 */ u8 mFogAdjTableType;
    /* 0x12D1 */ u8 Itemgetcol_chg;
    /* 0x12D2 */ u8 light_init_timer;
    /* 0x12D3 */ u8 schbit;
    /* 0x12D4 */ bool hide_vrbox;
    /* 0x12D5 */ u8 mContrastFlag;
    /* 0x12D6 */ u8 field_0x12d6;
    /* 0x12D7 */ u8 shadow_mode;
    /* 0x12D8 */ u8 camera_water_in_status;
    /* 0x12DC */ dKy_pol_efftbl_struct* pol_efftbl;
    /* 0x12E0 */ dKy_pol_effcol_struct* pol_effcol;
    /* 0x12E4 */ dKy_pol_efftbl_struct* pol_efftbl2;
    /* 0x12E8 */ dKy_pol_effcol_struct* pol_effcol2;
    /* 0x12EC */ dKy_pol_sound_struct* pol_sound;
    /* 0x12F0 */ dKy_pol_arg_struct* pol_arg;
    /* 0x12F4 */ dKy_color_data_struct* colordata_tbl;
    /* 0x12F8 */ u8 mFogDensity;
    /* 0x12F9 */ u8 field_0x12f9;
    /* 0x12FA */ u8 is_blure;
    /* 0x12FB */ u8 using_time_control_tag;
    /* 0x12FC */ s8 field_0x12fc;
    /* 0x12FD */ u8 darktime_week;
    /* 0x12FE */ u8 fishing_hole_season;
    /* 0x1300 */ s16 field_0x1300;
    /* 0x1302 */ s16 field_0x1302;
    /* 0x1304 */ f32 mWaterSurfaceShineRate;
    /* 0x1308 */ u8 light_mask_type;
    /* 0x1309 */ u8 light_size;
    /* 0x130A */ u8 field_0x130a;
    /* 0x130B */ u8 field_0x130b;
    /* 0x130C */ u8 staffroll_next_timer;
};  // Size: 0x1310

extern dScnKy_env_light_c g_env_light;

STATIC_ASSERT(sizeof(dScnKy_env_light_c) == 4880);

inline dScnKy_env_light_c* dKy_getEnvlight() {
    return &g_env_light;
}

class dKankyo_ParticlelightHIO_c : public JORReflexible {
public:
    dKankyo_ParticlelightHIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);
    virtual ~dKankyo_ParticlelightHIO_c() {}

    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 field_0x5;
    /* 0x06 */ GXColor prim_col;
    /* 0x0A */ GXColor env_col;
    /* 0x10 */ f32 blend_ratio;
    /* 0x14 */ int field_0x14;
    /* 0x18 */ u8 type;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1a;
    /* 0x1B */ u8 field_0x1B[0x20 - 0x1B];
};

class dKankyo_lightHIO_c : public JORReflexible {
public:
    dKankyo_lightHIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);
    virtual ~dKankyo_lightHIO_c() {}

    void dKankyo_lightHIOInfoUpDateF();

    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 field_0x5;
    /* 0x08 */ int field_0x8;
    /* 0x0C */ int field_0xc[10];
    /* 0x34 */ u8 m_fogtype;
    /* 0x35 */ u8 field_0x35[0x50 - 0x35];
    /* 0x50 */ u8 m_displayTVColorSettings;
    /* 0x51 */ u8 m_HOSTIO_setting;
    /* 0x52 */ u8 field_0x52;
    /* 0x53 */ u8 m_forcedPalette;
    /* 0x54 */ u8 m_displayColorPaletteCheckInfo;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ s16 field_0x5c;
    /* 0x5E */ s16 field_0x5e;
    /* 0x60 */ u8 field_0x60;
    /* 0x61 */ u8 field_0x61;
    /* 0x62 */ u8 field_0x62;
    /* 0x63 */ u8 field_0x63;
    /* 0x64 */ u8 field_0x64;
    /* 0x65 */ u8 field_0x65;
    /* 0x66 */ u8 field_0x66;
    /* 0x67 */ u8 field_0x67;
    /* 0x68 */ u8 field_0x68;
    /* 0x69 */ u8 field_0x69;
    /* 0x6A */ u8 field_0x6a;
    /* 0x6B */ u8 field_0x6b;
    /* 0x6C */ cXyz m_BG_fakelight_pos;
    /* 0x78 */ u8 m_BG_fakelight_R;
    /* 0x79 */ u8 m_BG_fakelight_G;
    /* 0x7A */ u8 m_BG_fakelight_B;
    /* 0x7C */ f32 m_BG_fakelight_power;
    /* 0x80 */ u8 field_0x80;
};

class dKankyo_vrboxHIO_c : public JORReflexible {
public:
    dKankyo_vrboxHIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);
    virtual ~dKankyo_vrboxHIO_c() {}

    void dKankyo_vrboxHIOInfoUpDateF();

    /* 0x04 */ u8 m_VrboxSetting;
    /* 0x05 */ u8 field_0x5;
    /* 0x06 */ u8 m_displayVrboxTVColorSettings;
    /* 0x07 */ u8 field_0x7;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ u8 field_0xa;
    /* 0x0B */ u8 field_0xb;
    /* 0x0C */ u8 field_0xc;
    /* 0x0D */ u8 field_0xd;
    /* 0x0E */ s8 field_0xe;
    /* 0x0F */ s8 field_0xf;
    /* 0x10 */ s8 field_0x10;
    /* 0x11 */ s8 field_0x11;
    /* 0x12 */ s8 field_0x12;
    /* 0x13 */ s8 field_0x13;
    /* 0x14 */ u8 field_0x14;
    /* 0x18 */ f32 m_horizonHeight;
    /* 0x1C */ u8 field_0x1c[0x20 - 0x1C];
};

class dKankyo_bloomHIO_c : public JORReflexible {
public:
    dKankyo_bloomHIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);
    virtual ~dKankyo_bloomHIO_c() {}

    /* 0x004 */ u8 field_0x4;
    /* 0x005 */ u8 field_0x5;
    /* 0x006 */ u8 m_saturationPattern;
    /* 0x007 */ dkydata_bloomInfo_info_class bloom_info[64];
};

class dKankyo_navyHIO_c : public JORReflexible {
public:
    dKankyo_navyHIO_c();

    void genMessage(JORMContext*);
    virtual ~dKankyo_navyHIO_c() {}

    /* 0x004 */ u8 field_0x4;
    /* 0x005 */ u8 field_0x5;
    /* 0x006 */ u8 field_0x6;
    /* 0x008 */ s16 field_0x8;
    /* 0x00C */ f32 cloud_sunny_wind_influence_rate;
    /* 0x010 */ f32 cloud_sunny_bottom_height;
    /* 0x014 */ f32 cloud_sunny_top_height;
    /* 0x018 */ f32 cloud_cloudy_wind_influence_rate;
    /* 0x01C */ f32 cloud_cloudy_bottom_height;
    /* 0x020 */ f32 cloud_cloudy_top_height;
    /* 0x024 */ f32 cloud_cloudy_height_shrink_rate;
    /* 0x028 */ f32 cloud_cloudy_alpha;
    /* 0x02C */ f32 cloud_sunny_size;
    /* 0x030 */ f32 cloud_cloudy_size;
    /* 0x034 */ f32 cloud_sunny_height_shrink_rate;
    /* 0x038 */ f32 cloud_sunny_alpha;
    /* 0x03C */ f32 field_0x3c;
    /* 0x040 */ f32 field_0x40;
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 field_0x48;
    /* 0x04C */ f32 field_0x4c;
    /* 0x050 */ f32 field_0x50;
    /* 0x054 */ f32 field_0x54;
    /* 0x058 */ f32 field_0x58;
    /* 0x05C */ f32 field_0x5c;
    /* 0x060 */ f32 field_0x60;
    /* 0x064 */ f32 field_0x64;
    /* 0x068 */ u8 field_0x68;
    /* 0x069 */ u8 field_0x69;
    /* 0x06A */ s16 housi_max_number;
    /* 0x06C */ f32 housi_max_alpha;
    /* 0x070 */ f32 housi_max_scale;
    /* 0x074 */ u8 field_0x74;
    /* 0x075 */ u8 field_0x75;
    /* 0x076 */ u8 field_0x76;
    /* 0x077 */ u8 field_0x77;
    /* 0x078 */ u8 field_0x78;
    /* 0x079 */ u8 field_0x79;
    /* 0x07A */ u8 field_0x7a;
    /* 0x07C */ f32 field_0x7c;
    /* 0x080 */ f32 field_0x80;
    /* 0x084 */ f32 field_0x84;
    /* 0x088 */ s16 field_0x88;
    /* 0x08A */ u8 field_0x8a;
    /* 0x08C */ cXyz field_0x8c;
    /* 0x098 */ cXyz field_0x98;
    /* 0x0A4 */ u8 field_0xA4[0x0A8 - 0x0A4];
    /* 0x0A8 */ GXColor moon_col;
    /* 0x0AC */ f32 moon_scale;
    /* 0x0B0 */ cXyz field_0xb0;
    /* 0x0BC */ f32 field_0xbc;
    /* 0x0C0 */ f32 field_0xc0;
    /* 0x0C4 */ s16 field_0xc4;
    /* 0x0C8 */ f32 field_0xc8;
    /* 0x0CC */ f32 field_0xcc;
    /* 0x0D0 */ u8 field_0xd0;
    /* 0x0D1 */ u8 field_0xd1;
    /* 0x0D2 */ u8 field_0xd2;
    /* 0x0D3 */ u8 field_0xd3;
    /* 0x0D4 */ u8 field_0xd4;
    /* 0x0D5 */ u8 field_0xd5;
    /* 0x0D6 */ u8 field_0xd6;
    /* 0x0D7 */ u8 field_0xd7;
    /* 0x0D8 */ u8 field_0xd8;
    /* 0x0D9 */ u8 field_0xd9;
    /* 0x0DA */ u8 field_0xda;
    /* 0x0DB */ u8 field_0xdb;
    /* 0x0DC */ u8 field_0xdc;
    /* 0x0DD */ u8 field_0xdd;
    /* 0x0DE */ u8 field_0xde;
    /* 0x0E0 */ s16 field_0xe0;
    /* 0x0E4 */ f32 field_0xe4;
    /* 0x0E8 */ color_RGB_class sun_col;
    /* 0x0EB */ u8 field_0xeb;
    /* 0x0EC */ color_RGB_class sun_col2;
    /* 0x0EF */ u8 field_0xef;
    /* 0x0F0 */ u8 field_0xf0;
    /* 0x0F1 */ u8 field_0xf1;
    /* 0x0F2 */ u8 field_0xf2;
    /* 0x0F3 */ u8 field_0xf3;
    /* 0x0F4 */ u8 field_0xf4;
    /* 0x0F5 */ u8 field_0xf5;
    /* 0x0F6 */ u8 field_0xf6;
    /* 0x0F7 */ u8 field_0xf7;
    /* 0x0F8 */ u8 field_0xf8;
    /* 0x0F9 */ u8 field_0xf9;
    /* 0x0FA */ u8 field_0xfa;
    /* 0x0FB */ u8 field_0xfb;
    /* 0x0FC */ u8 field_0xfc;
    /* 0x0FD */ u8 field_0xfd;
    /* 0x0FE */ u8 field_0xfe;
    /* 0x0FF */ u8 field_0xFF[0x108 - 0x0FF];
    /* 0x108 */ u8 field_0x108;
    /* 0x109 */ u8 field_0x109;
    /* 0x10A */ u8 field_0x10a;
    /* 0x10B */ u8 field_0x10b;
    /* 0x10C */ u8 field_0x10c;
    /* 0x10D */ u8 field_0x10d;
    /* 0x10E */ u8 field_0x10e;
    /* 0x10F */ u8 field_0x10f;
    /* 0x110 */ u8 field_0x110;
    /* 0x111 */ u8 field_0x111;
    /* 0x112 */ u8 field_0x112;
    /* 0x113 */ u8 field_0x113;
    /* 0x114 */ u8 field_0x114;
    /* 0x115 */ u8 field_0x115;
    /* 0x116 */ u8 field_0x116;
    /* 0x117 */ u8 field_0x117[0x120 - 0x117];
    /* 0x120 */ f32 field_0x120;
    /* 0x124 */ f32 field_0x124;
    /* 0x128 */ u8 constellation_maker_ON;
    /* 0x12C */ cXyz constellation_maker_pos[10];
    /* 0x1A4 */ f32 lightning_scale_x_min;
    /* 0x1A8 */ f32 lightning_scale_x_max;
    /* 0x1AC */ f32 lightning_scale_y_min;
    /* 0x1B0 */ f32 lightning_scale_y_max;
    /* 0x1B4 */ s16 lightning_tilt_angle;
    /* 0x1B6 */ s16 field_0x1b6;
    /* 0x1B8 */ cXyz collect_light_reflect_pos;
    /* 0x1C4 */ u8 lightning_debug_mode;
    /* 0x1C5 */ u8 field_0x1c5;
    /* 0x1C6 */ u8 sun_adjust_ON;
    /* 0x1C8 */ f32 moya_alpha;
    /* 0x1CC */ color_RGB_class thunder_col;
    /* 0x1D0 */ f32 thunder_height;
    /* 0x1D4 */ f32 thunder_blacken_rate;
    /* 0x1D8 */ f32 water_in_col_ratio_R;
    /* 0x1DC */ f32 water_in_col_ratio_G;
    /* 0x1E0 */ f32 water_in_col_ratio_B;
    /* 0x1E4 */ u8 field_0x1e4;
    /* 0x1E5 */ u8 field_0x1e5;
    /* 0x1E6 */ u8 field_0x1e6;
    /* 0x1E8 */ f32 field_0x1e8;
    /* 0x1EC */ f32 field_0x1ec;
    /* 0x1F0 */ f32 field_0x1f0;
    /* 0x1F4 */ f32 field_0x1f4;
    /* 0x1F8 */ f32 field_0x1f8;
    /* 0x1FC */ u8 field_0x1fc;
    /* 0x1FD */ u8 field_0x1fd;
    /* 0x1FE */ u8 field_0x1fe;
    /* 0x1FF */ u8 field_0x1ff;
    /* 0x200 */ u8 field_0x200;
    /* 0x204 */ f32 mist_tag_fog_near;
    /* 0x208 */ f32 mist_tag_fog_far;
    /* 0x20C */ u8 demo_adjust_SW;
    /* 0x20D */ u8 wipe_test_ON;
    /* 0x210 */ f32 field_0x210;
    /* 0x214 */ u8 fade_test_speed;
    /* 0x215 */ u8 field_0x215;
    /* 0x218 */ f32 smell_railtag_space;
    /* 0x21C */ color_RGB_class smell_col;
    /* 0x21F */ u8 field_0x21f;
    /* 0x220 */ color_RGB_class smell_col2;
    /* 0x224 */ f32 smell_alpha;
    /* 0x228 */ u8 smell_adjust_ON;
    /* 0x22A */ u16 field_0x22a;
    /* 0x22C */ u8 field_0x22c;
    /* 0x22D */ u8 field_0x22d;
    /* 0x22E */ u8 field_0x22E[0x231 - 0x22E];
    /* 0x231 */ u8 light_adjust_ON;
    /* 0x232 */ u8 field_0x232[0x23C - 0x232];
    /* 0x23C */ color_RGB_class adjust_light_ambcol;
    /* 0x240 */ s16 adjust_light_dif0_col_R;
    /* 0x242 */ s16 adjust_light_dif0_col_G;
    /* 0x244 */ s16 adjust_light_dif0_col_B;
    /* 0x246 */ color_RGB_class adjust_light_dif1_col;
    /* 0x24C */ Vec adjust_light_main_pos;
    /* 0x258 */ GXColorS10 mist_twilight_c1_col;
    /* 0x260 */ GXColor mist_twilight_c2_col;
    /* 0x264 */ color_RGB_class field_0x264;
    /* 0x267 */ u8 field_0x267;
    /* 0x268 */ u8 field_0x268;
    /* 0x26A */ s16 adjust_custom_R;
    /* 0x26C */ s16 adjust_custom_G;
    /* 0x26E */ s16 adjust_custom_B;
    /* 0x270 */ u8 adjust_light_mode;
    /* 0x274 */ f32 adjust_height;
    /* 0x278 */ f32 field_0x278;
    /* 0x27C */ f32 field_0x27c;
    /* 0x280 */ f32 field_0x280;
    /* 0x284 */ f32 field_0x284;
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c;
    /* 0x290 */ u8 shadow_adjust_ON;
    /* 0x294 */ f32 shadow_normal_alpha;
    /* 0x298 */ f32 shadow_max_alpha;
    /* 0x29C */ u8 field_0x29c;
    /* 0x29D */ u8 field_0x29d;
    /* 0x29E */ u8 field_0x29e;
    /* 0x29F */ color_RGB_class unk_color_1;
    /* 0x2A2 */ u8 unk_alpha_1;
    /* 0x2A3 */ color_RGB_class unk_color_2;
    /* 0x2A6 */ u8 unk_alpha_2;
    /* 0x2A7 */ color_RGB_class unk_color_3;
    /* 0x2AA */ u8 unk_alpha_3;
    /* 0x2AB */ color_RGB_class camera_light_col;
    /* 0x2AE */ u8 camera_light_alpha;
    /* 0x2B0 */ f32 camera_light_y_shift;
    /* 0x2B4 */ f32 camera_light_power;
    /* 0x2B8 */ f32 camera_light_cutoff;
    /* 0x2BC */ u8 camera_light_sp;
    /* 0x2BD */ u8 camera_light_da;
    /* 0x2BE */ u8 room_light_type;
    /* 0x2C0 */ s16 demo_focus_pos;
    /* 0x2C4 */ f32 demo_focus_offset_x;
    /* 0x2C8 */ f32 demo_focus_offset_y;
    /* 0x2CC */ color_RGB_class grass_ambcol;
    /* 0x2CF */ u8 field_0x2cf;
    /* 0x2D0 */ u8 grass_adjust_ON;
    /* 0x2D4 */ f32 grass_shine_value;
    /* 0x2D8 */ s16 stars_max_number;
    /* 0x2DA */ u8 display_save_location;
    /* 0x2DB */ u8 door_light_influence_ratio;
    /* 0x2DC */ s16 unk_light_influence_ratio;
    /* 0x2DE */ u8 fish_pond_colreg_adjust_ON;
    /* 0x2E0 */ GXColorS10 fish_pond_colreg_c0;
    /* 0x2E8 */ u8 water_mud_adjust_ON;
    /* 0x2EA */ s16 field_0x2ea;
    /* 0x2EC */ s16 field_0x2ec;
    /* 0x2EE */ u8 fish_pond_tree_adjust_ON;
    /* 0x2EF */ color_RGB_class fish_pond_tree_ambcol;
    /* 0x2F2 */ u8 field_0x2f2;
    /* 0x2F3 */ color_RGB_class fish_pond_tree_dif0_col;
    /* 0x2F6 */ u8 field_0x2f6;
    /* 0x2F7 */ color_RGB_class fish_pond_tree_dif1_col;
    /* 0x2FA */ u8 field_0x2fa;
    /* 0x2FB */ u8 rainbow_adjust_ON;
    /* 0x2FC */ s16 rainbow_separation_dist;
    /* 0x2FE */ u8 rainbow_max_alpha;
    /* 0x2FF */ u8 field_0x2ff;
    /* 0x300 */ u8 grass_light_debug;
    /* 0x301 */ u8 grass_light_influence_ratio;
    /* 0x302 */ s16 field_0x302;
    /* 0x304 */ f32 field_0x304;
    /* 0x308 */ u8 field_0x308;
    /* 0x309 */ color_RGB_class moya_col;
    /* 0x30C */ u8 field_0x30c;
    /* 0x30D */ u8 field_0x30d;
    /* 0x30E */ u8 twilight_sense_saturation_mode;
    /* 0x30F */ u8 twilight_sense_pat;
    /* 0x310 */ u8 twilight_sense_pat_tv_display_ON;
    /* 0x311 */ u8 use_debug;
    /* 0x312 */ u8 field_0x312;
    /* 0x313 */ u8 camera_light_adjust_ON;
    /* 0x314 */ f32 field_0x314;
    /* 0x318 */ f32 terrain_height_crr;
    /* 0x31C */ f32 influence_multiplier;
    /* 0x320 */ f32 cutoff_multiplier;
    /* 0x324 */ color_RGB_class possessed_zelda_light_col;
    /* 0x327 */ u8 possessed_zelda_light_alpha;
    /* 0x328 */ f32 possessed_zelda_light_height;
    /* 0x32C */ f32 possessed_zelda_light_power;
    /* 0x330 */ color_RGB_class beast_ganon_light_col;
    /* 0x333 */ u8 beast_ganon_light_alpha;
    /* 0x334 */ f32 beast_ganon_light_height;
    /* 0x338 */ f32 beast_ganon_light_power;
    /* 0x33C */ color_RGB_class water_in_light_col;
};

class dKankyo_efflightHIO_c : public JORReflexible {
public:
    dKankyo_efflightHIO_c();

    void genMessage(JORMContext*);
    virtual ~dKankyo_efflightHIO_c() {}

    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 adjust_ON;
    /* 0x08 */ f32 power;
    /* 0x0C */ f32 fluctuation;
    /* 0x10 */ dKy_flush_info step1;
    /* 0x14 */ dKy_flush_info step2;
    /* 0x18 */ dKy_flush_info step3;
    /* 0x1C */ dKy_flush_info step4;
};

class dKankyo_dungeonlightHIO_c : public JORReflexible {
public:
    dKankyo_dungeonlightHIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);
    virtual ~dKankyo_dungeonlightHIO_c() {}

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 displayDebugSphere;
    /* 0x7 */ u8 usedLights;
    /* 0x8 */ u8 field_0x8;
    /* 0x9 */ u8 field_0x9;
};

class dKankyo_demolightHIO_c : public JORReflexible {
public:
    dKankyo_demolightHIO_c();

    void genMessage(JORMContext*);
    virtual ~dKankyo_demolightHIO_c() {}

    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 adjust_ON;
    /* 0x08 */ LIGHT_INFLUENCE light;
};

class dKankyo_windHIO_c : public JORReflexible {
public:
    dKankyo_windHIO_c();

    void genMessage(JORMContext*);
    virtual ~dKankyo_windHIO_c() {}

    /* 0x04 */ u8 field_0x04[0x05 - 0x04];
    /* 0x05 */ u8 display_wind_dir;
    /* 0x06 */ u8 use_HOSTIO_adjustment;
    /* 0x08 */ int field_0x8;
    /* 0x0C */ s16 global_x_angle;
    /* 0x0E */ s16 global_y_angle;
    /* 0x10 */ f32 global_wind_power;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ u8 display_wind_trajectory;
    /* 0x22 */ s16 lightsword_x_angle;
    /* 0x24 */ f32 lightsword_init_scale;
    /* 0x28 */ f32 lightsword_end_scale;
    /* 0x2C */ f32 influence;
    /* 0x30 */ f32 influence_attenuation;
    /* 0x34 */ f32 lightsword_move_speed;
    /* 0x38 */ f32 wind_change_speed;
    /* 0x3C */ s16 minigame_no_wind_duration;
    /* 0x3E */ s16 minigame_low_wind_duration;
    /* 0x40 */ s16 minigame_high_wind_duration;
};

class dKankyo_HIO_c : public JORReflexible {
public:
    dKankyo_HIO_c();

    void genMessage(JORMContext*);
    virtual ~dKankyo_HIO_c() {}

    /* 0x004 */ u8 wether_effect;
    /* 0x005 */ u8 wether_palette;
    /* 0x006 */ u8 time_change;
    /* 0x007 */ u8 display_env_tag_debug;
    /* 0x008 */ u8 display_wether_debug;
    /* 0x009 */ u8 field_0x9;
    /* 0x00A */ u8 field_0xa;
    /* 0x00B */ u8 display_schedule_bit;
    /* 0x00C */ f32 field_0xc;
    /* 0x010 */ u8 field_0x10;
    /* 0x011 */ u8 start_rain;
    /* 0x012 */ u8 effect_light_monitor;
    /* 0x013 */ u8 point_light_monitor;
    /* 0x014 */ u8 field_0x14;
    /* 0x015 */ u8 field_0x15;
    /* 0x016 */ u8 field_0x16;
    /* 0x017 */ u8 field_0x17;
    /* 0x018 */ u8 no_setting_tevstr;
    /* 0x019 */ u8 no_color_type;
    /* 0x01A */ s8 field_0x1a;
    /* 0x01C */ dKankyo_lightHIO_c light;
    /* 0x0A0 */ dKankyo_vrboxHIO_c vrbox;
    /* 0x0C0 */ dKankyo_bloomHIO_c bloom;
    /* 0x3C8 */ dKankyo_navyHIO_c navy;
    /* 0x708 */ dKankyo_efflightHIO_c effLight;
    /* 0x728 */ dKankyo_demolightHIO_c demoLight;
    /* 0x750 */ dKankyo_windHIO_c wind;
    /* 0x794 */ dKankyo_dungeonlightHIO_c dungeonLight;
    /* 0x7A0 */ dKankyo_ParticlelightHIO_c particleLight;
};

extern dKankyo_HIO_c g_kankyoHIO;

enum dKy_dice_wether_mode {
    DICE_MODE_SUNNY_e,
    DICE_MODE_CLOUDY_e,
    DICE_MODE_RAIN_LIGHT_e,
    DICE_MODE_RAIN_HEAVY_e,
    DICE_MODE_THUNDER_LIGHT_e,
    DICE_MODE_THUNDER_HEAVY_e,
    DICE_MODE_UNK6_e,
    DICE_MODE_DONE_e = 0xFF,
};

enum dKy_dice_wether_state {
    DICE_STATE_RESET_e,
    DICE_STATE_INIT_e,
    DICE_STATE_EXEC_e,
    DICE_STATE_NEXT_e,
};

void dKy_set_nexttime(f32 nexttime);
void dKy_itudemo_se();
BOOL dKy_darkworld_spot_check(char const* stageName, int roomNo);
void dKy_darkworld_Area_set(char const* stageName, int roomNo);
void dKy_FiveSenses_fullthrottle_dark();
BOOL dKy_daynight_check();
void dKy_clear_game_init();
void dKy_setLight_init();
u8 dKy_pol_sound_get(const cBgS_PolyInfo* polyinfo_p);
void dKy_depth_dist_set(void* process_p);
void dKy_instant_rainchg();
void dKy_instant_timechg(f32 time);
void dKy_set_vrboxsoracol_ratio(f32 ratio);
void dKy_set_vrboxkumocol_ratio(f32 ratio);
void dKy_actor_addcol_set(s16 r, s16 g, s16 b, f32 ratio);
void dKy_fog_startendz_set(f32 param_0, f32 param_1, f32 ratio);
void dKy_vrbox_addcol_set(s16 r, s16 g, s16 b, f32 ratio);
void dKy_GxFog_set();
u8 dKy_pol_argument_get(const cBgS_PolyInfo* polyinfo_p);
void dKy_Sound_set(cXyz pos, int param_1, fpc_ProcID actor_id, int timer);
void dKy_bg_MAxx_proc(void* bg_model_p);
void dKy_change_colpat(u8 colpat);
int dKy_BossLight_set(cXyz* pos_p, GXColor* color_p, f32 ref_dist, u8 param_3);
void dKy_custom_colset(u8 prevGather, u8 curGather, f32 blend);
void dKy_setLight();
cXyz dKy_plight_near_pos();
int dKy_BossSpotLight_set(cXyz* pos_p, f32 angle_x, f32 angle_y, f32 cutoff, GXColor* color_p,
                          f32 ref_dist, u8 spot_type, u8 distattn_type);
void dKy_calc_color_set(GXColorS10* out_color_p, color_RGB_class* color_a_start_p,
                        color_RGB_class* color_b_start_p, color_RGB_class* color_a_end_p,
                        color_RGB_class* color_b_end_p, f32 color_ratio, f32 blend_ratio,
                        GXColorS10 add_col, f32 scale);
void dKy_twilight_camelight_set();
u8 dKy_shadow_mode_check(u8 mode);
void dKy_shadow_mode_set(u8 mode);
void dKy_shadow_mode_reset(u8 mode);
void dKy_Itemgetcol_chg_on();
void dKy_actor_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio);
void dKy_bg_addcol_amb_set(s16 r, s16 g, s16 b, f32 ratio);
int dKy_WolfEyeLight_set(cXyz* pos_p, f32 angle_x, f32 angle_y, f32 cutoff, GXColor* color_p,
                         f32 ref_dist, u8 spot_type, u8 distattn_type);
BOOL dKy_TeachWind_existence_chk();
u8 dKy_darkworld_stage_check(char const* stageName, int roomNo);
BOOL dKy_withwarp_capture_check();
u8 dKy_darkworld_check();
void dKy_undwater_filter_draw();
BOOL dKy_camera_water_in_status_check();
int dKy_Outdoor_check();
int dKy_getdaytime_hour();
int dKy_getdaytime_minute();
int dKy_get_dayofweek();
int dKy_getDarktime_minute();
int dKy_getDarktime_hour();
u8 dKy_getDarktime_week();
void dKy_Sound_init();
void dKy_setLight_nowroom(char room_no);
void dKy_setLight_nowroom_actor(dKy_tevstr_c* tevstr_p);
void dKy_setLight_again();
void dKy_GxFog_tevstr_set(dKy_tevstr_c* tevstr_p);
void dKy_plight_set(LIGHT_INFLUENCE* light_inf_p);
void dKy_plight_priority_set(LIGHT_INFLUENCE* light_inf_p);
void dKy_efplight_set(LIGHT_INFLUENCE* light_inf_p);
void dKy_efplight_cut(LIGHT_INFLUENCE* light_inf_p);
void dKy_tevstr_init(dKy_tevstr_c* tevstr_p, s8 room_no, u8 floorCol);
SND_INFLUENCE* dKy_Sound_get();
void dKy_plight_cut(LIGHT_INFLUENCE* light_inf_p);
void dKy_dalkmist_inf_set(DALKMIST_INFLUENCE* dalkmist_inf_p);
void dKy_dalkmist_inf_cut(DALKMIST_INFLUENCE* dalkmist_inf_p);
int dKy_rain_check();
void dKy_set_allcol_ratio(f32 ratio);
void dKy_set_actcol_ratio(f32 ratio);
void dKy_set_bgcol_ratio(f32 ratio);
void dKy_set_fogcol_ratio(f32 ratio);
void dKy_set_vrboxcol_ratio(f32 ratio);
f32 dKy_get_parcent(f32 max, f32 min, f32 value);
void dKy_setLight_nowroom_grass(char room_no, f32 light_ratio);
void dKy_Global_amb_set(dKy_tevstr_c* tevstr_p);
u8 dKy_pol_eff_prim_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p);
u8 dKy_pol_eff_env_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p);
u8 dKy_pol_eff_alpha_get(const cBgS_PolyInfo* polyinfo_p);
f32 dKy_pol_eff_ratio_get(const cBgS_PolyInfo* polyinfo_p);
u8 dKy_pol_eff2_prim_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p);
u8 dKy_pol_eff2_env_get(const cBgS_PolyInfo* polyinfo_p, GXColor* out_color_p);
u8 dKy_pol_eff2_alpha_get(const cBgS_PolyInfo* polyinfo_p);
f32 dKy_pol_eff2_ratio_get(const cBgS_PolyInfo* polyinfo_p);
u8 dKy_pol_efftype_get(const cBgS_PolyInfo* polyinfo_p);
u8 dKy_pol_efftype2_get(const cBgS_PolyInfo* polyinfo_p);
void dKy_ParticleColor_get_actor(cXyz* param_0, dKy_tevstr_c* tevstr_p, GXColor* param_2,
                                 GXColor* param_3, GXColor* param_4, GXColor* param_5, f32 param_6);
void dKy_ParticleColor_get_bg(cXyz* param_0, dKy_tevstr_c* tevstr_p, GXColor* param_2,
                              GXColor* param_3, GXColor* param_4, GXColor* param_5, f32 param_6);
GXColor dKy_light_influence_col(GXColor* param_0, f32 param_1);
void dKy_WaterIn_Light_set();
void dKy_SordFlush_set(cXyz light_pos, int light_type);
void dKy_camera_water_in_status_set(u8 status);
void dKy_mock_light_every_set(LIGHT_INFLUENCE* light_inf_p);
f32 dKy_move_room_ratio(dKy_tevstr_c* tevstr_p, s8* room_no_p);
void dKy_bg1_addcol_amb_set(s16 r, s16 g, s16 b, f32 factor);
void dKy_vrbox_addcol_sky0_set(s16 r, s16 g, s16 b, f32 factor);
void dKy_vrbox_addcol_kasumi_set(s16 r, s16 g, s16 b, f32 factor);
void dKy_addcol_fog_set(s16 r, s16 g, s16 b, f32 factor);
cXyz dKy_light_influence_pos(int light_id);
BOOL dKy_SunMoon_Light_Check();
void dKy_bgparts_activelight_set(LIGHT_INFLUENCE* light_inf_p, int bgpart_id);
void dKy_bgparts_activelight_cut(int bgpart_id);
int dKy_Indoor_check();
void dKy_GfFog_tevstr_set(dKy_tevstr_c* tevstr_p);
void dKy_setLight_mine(dKy_tevstr_c* tevstr_p);
u8 dKy_get_schbit();
int dKy_get_schbit_timer();

#endif /* D_KANKYO_D_KANKYO_H */
