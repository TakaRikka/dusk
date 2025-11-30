#ifndef D_KANKYO_D_KANKYO_WETHER_H
#define D_KANKYO_D_KANKYO_WETHER_H

#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"

class WIND_INFLUENCE;

void dKyw_wether_draw();
void dKyw_wether_delete();
void dKyw_wether_move();
void dKyw_wether_move_draw();
void dKyw_wether_move_draw2();
void dKyw_wether_draw2();
void dKyw_wether_delete2();
void dKyw_wether_init();
void dKyw_wether_init2();
static void dKyw_wind_init();
static void dKyw_pntwind_init();
f32 dKyw_get_wind_pow();
static void squal_proc();
void dKyw_rain_set(int count);
void dKyw_wind_set();
cXyz* dKyw_get_wind_vec();
cXyz dKyw_get_wind_vecpow();
cXyz dKyw_get_AllWind_vecpow(cXyz* param_0);
void dKyw_evt_wind_set(s16 angleX, s16 angleY);
void dKyw_custom_windpower(f32 pow);
void dKyw_evt_wind_set_go();
void dKyw_wether_proc();
void dKyw_get_AllWind_vec(cXyz* i_position, cXyz* i_direction, f32* i_power);
void dKyw_pntwind_cut(WIND_INFLUENCE* i_pntwind);
cXyz dKyw_pntwind_get_vecpow(cXyz* param_0);
void dKyw_pntwind_set(WIND_INFLUENCE* i_pntwind);
void dKyw_pntlight_collision_get_info(cXyz* param_0, cXyz* param_1, f32* param_2);
void dKyw_plight_collision_set(cXyz* param_0, s16 param_1, s16 param_2, f32 param_3, f32 param_4,
                               f32 param_5, f32 param_6, f32 param_7);

class dKankyo_sun_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_sun_Packet();

    /* 0x10 */ cXyz mPos[2];
    /* 0x28 */ u8 field_0x28;
    /* 0x29 */ u8 field_0x29;
    /* 0x2C */ u8* mpResMoon;
    /* 0x30 */ u8* mpResMoon_A;
    /* 0x34 */ u8* mpResMoon_A_A00;
    /* 0x38 */ u8* mpResMoon_A_A01;
    /* 0x3C */ u8* mpResMoon_A_A02;
    /* 0x40 */ u8* mpResMoon_A_A03;
    /* 0x44 */ u32 field_0x44[5];
    /* 0x58 */ int field_0x58;
    /* 0x5C */ f32 mVisibility;
    /* 0x60 */ f32 mSunAlpha;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 mMoonAlpha;
    /* 0x6C */ f32 field_0x6c;
    /* 0x70 */ GXColor mColor;
    /* 0x74 */ GXColor field_0x74;
};  // Size: 0x78

class dKankyo_sunlenz_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_sunlenz_Packet();

    /* 0x10 */ GXColor mColor;
    /* 0x14 */ u8* mpResBall;
    /* 0x18 */ u8 field_0x18[4];
    /* 0x1C */ u8* mpResRing_A;
    /* 0x20 */ u8* mpResLenz;
    /* 0x24 */ cXyz mPositions[8];
    /* 0x84 */ f32 field_0x84;
    /* 0x88 */ f32 field_0x88;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 mDistFalloff;
    /* 0x9C */ u8 field_0x9c[2];
    /* 0x9E */ bool mDrawLenzInSky;
};  // Size: 0xA0

struct RAIN_EFF {
    ~RAIN_EFF();
    RAIN_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 mAlpha;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ s16 mTimer;
};  // Size: 0x38

class dKankyo_rain_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_rain_Packet();

    /* 0x0010 */ u8* mpTex;
    /* 0x0014 */ u8* mpTex2;
    /* 0x0018 */ RAIN_EFF mRainEff[250];
    /* 0x36C8 */ f32 field_0x36c8;
    /* 0x36CC */ f32 field_0x36cc;
    /* 0x36D0 */ s16 raincnt;
    /* 0x36D2 */ u8 field_0x36D2[6];
    /* 0x36D8 */ cXyz mCamEyePos;
    /* 0x36E4 */ cXyz mCamCenterPos;
    /* 0x36F0 */ cXyz mCenterDelta;
    /* 0x36FC */ f32 mCenterDeltaMul;
    /* 0x3700 */ f32 field_0x3700;
    /* 0x3704 */ f32 mSibukiAlpha;
    /* 0x3708 */ f32 mOverheadFade;
    /* 0x370C */ f32 mFwdFade1;
    /* 0x3710 */ f32 mFwdFade2;
    /* 0x3714 */ u8 mStatus;
};  // Size: 0x3718

struct SNOW_EFF {
    ~SNOW_EFF();
    SNOW_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ f32 mPosWaveX;
    /* 0x20 */ f32 mPosWaveZ;
    /* 0x24 */ f32 mGravity;
    /* 0x28 */ f32 mWindSpeed;
    /* 0x2C */ f32 mScale;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ s16 mTimer;
};  // Size: 0x38

class dKankyo_snow_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_snow_Packet();

    /* 0x0010 */ u8* mpTex;
    /* 0x0014 */ SNOW_EFF mSnowEff[500];
    /* 0x6D74 */ cXyz field_0x6d74;
    /* 0x6D80 */ f32 field_0x6d80;
    /* 0x6D84 */ f32 field_0x6d84;
    /* 0x6D88 */ s16 field_0x6d88;
    /* 0x6D8A */ u16 field_0x6d8a;
    /* 0x6D8C */ u32 field_0x6d8c;
    /* 0x6D90 */ u8 field_0x6d90;
    /* 0x6D91 */ u8 field_0x6d91;
    /* 0x6D92 */ u8 field_0x6d92;
    /* 0x6D93 */ u8 field_0x6d93;
};

struct STAR_EFF {
    ~STAR_EFF();
    STAR_EFF();

    /* 0x00 */ u8 field_0x0[4];
    /* 0x04 */ cXyz field_0x04;
    /* 0x10 */ cXyz field_0x10;
    /* 0x1C */ u8 field_0x1c[12];
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ u8 field_0x2c[8];
};  // Size: 0x34

class dKankyo_star_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_star_Packet();

    /* 0x10 */ u8* mpTex;
    /* 0x14 */ STAR_EFF mEffect[1];
    /* 0x48 */ s16 mEffectNum;
};

struct HOUSI_EFF {
    ~HOUSI_EFF();
    HOUSI_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ cXyz mSpeed;
    /* 0x28 */ cXyz mScale;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ u16 field_0x3c;
    /* 0x40 */ f32 mAlpha;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ u16 field_0x4c;
};  // Size: 0x50

class dKankyo_housi_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_housi_Packet();

    /* 0x0010 */ cXyz field_0x10;
    /* 0x001C */ u8* mpResTex;
    /* 0x0020 */ HOUSI_EFF mHousiEff[300];
    /* 0x5DE0 */ u8 field_0x5de0[8];
    /* 0x5DE8 */ f32 field_0x5de8;
    /* 0x5DEC */ s16 field_0x5dec;
};  // Size: 0x5DF0

struct CLOUD_EFF {
    ~CLOUD_EFF();
    CLOUD_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ cXyz mPntWindSpeed;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ u16 field_0x2c;
    /* 0x30 */ f32 mAlpha;
    /* 0x34 */ f32 mSize;
};  // Size: 0x38

class dKankyo_cloud_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_cloud_Packet();

    /* 0x10 */ u8* mpResTex;
    /* 0x14 */ int mCount;
    /* 0x18 */ CLOUD_EFF mCloudEff[50];
};  // Size: 0xB08

struct VRKUMO_EFF {
    ~VRKUMO_EFF();
    VRKUMO_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ f32 mHeight;
    /* 0x20 */ f32 mAlpha;
    /* 0x24 */ f32 mDistFalloff;
    /* 0x28 */ f32 mSpeed;
};  // Size: 0x2C

class dKankyo_vrkumo_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_vrkumo_Packet();

    /* 0x0010 */ GXColor mColor;
    /* 0x0014 */ u8* mpResCloudtx_01;
    /* 0x0018 */ u8* mpResCloudtx_02;
    /* 0x001C */ u8* mpResCloudtx_03;
    /* 0x0020 */ VRKUMO_EFF mVrkumoEff[100];
    /* 0x1150 */ f32 field_0x1150;
    /* 0x1154 */ f32 field_0x1154;
};  // Size: 0x1158

// might be fake
class dKankyo_shstar_Packet : public J3DPacket {};

struct EF_ODOUR_EFF {
    ~EF_ODOUR_EFF();
    EF_ODOUR_EFF();

    /* 0x00 */ u8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ s16 mRotX;
    /* 0x1E */ s16 mRotY;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
};  // Size: 0x30

class dKankyo_odour_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_odour_Packet();

    /* 0x00010 */ u8* mpResTex;
    /* 0x00014 */ EF_ODOUR_EFF mOdourEff[2000];
    /* 0x17714 */ cXyz field_0x17714;
    /* 0x17720 */ u8 field_0x17720[0x17724 - 0x17720];
    /* 0x17724 */ s16 field_0x17724;
    /* 0x17720 */ s16 field_0x17726;
};  // Size: 0x17728

struct EF_MUD_EFF {
    ~EF_MUD_EFF();
    EF_MUD_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ cXyz field_0x1c;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ s16 field_0x40;
    /* 0x42 */ s16 field_0x42;
    /* 0x44 */ u8 field_0x44[0x48 - 0x44];
};  // Size: 0x48

class dKankyo_mud_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_mud_Packet();

    /* 0x0010 */ int mEffectNum;
    /* 0x0014 */ u8* mpMoyaRes;
    /* 0x0018 */ EF_MUD_EFF mEffect[100];
    /* 0x1C38 */ u8 field_0x1c38[4];
    /* 0x1C3C */ f32 field_0x1c3c;
};

struct EF_EVIL_EFF {
    ~EF_EVIL_EFF();
    EF_EVIL_EFF();

    /* 0x00 */ s8 mStatus;
    /* 0x04 */ cXyz mPosition;
    /* 0x10 */ cXyz mBasePos;
    /* 0x1C */ csXyz field_0x1c;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ u8 field_0x30[0x38 - 0x30];
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ u16 field_0x3c;
    /* 0x3E */ u16 field_0x3e;
    /* 0x40 */ u16 field_0x40;
    /* 0x42 */ u16 field_0x42;
};  // Size: 0x44

class dKankyo_evil_Packet : public J3DPacket {
public:
    virtual void draw();
    virtual ~dKankyo_evil_Packet();

    /* 0x00010 */ int mEffectNum;
    /* 0x00014 */ u8* mpMoyaRes;
    /* 0x00018 */ u8* mpKumoLightRes;
    /* 0x0001C */ EF_EVIL_EFF mEffect[2000];
    /* 0x2135C */ u8 field_0x2135c[8];
};

#endif /* D_KANKYO_D_KANKYO_WETHER_H */
