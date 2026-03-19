#ifndef D_BG_D_BG_S_H
#define D_BG_D_BG_S_H

#include "d/d_bg_w_base.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include <os.h>
#include "global.h"

class dBgW;
class fopAc_ac_c;

class cBgS_ChkElm {
public:
    /* 0x00 */ dBgW_Base* m_bgw_base_ptr;
    /* 0x04 */ bool m_used;
    /* 0x08 */ fpc_ProcID m_actor_id;
    /* 0x0C */ fopAc_ac_c* m_actor_ptr;
    /* 0x10 vtable */

public:
    cBgS_ChkElm() { this->Init(); }
    void Init();
    void Release();

    virtual ~cBgS_ChkElm() {}
    virtual void Regist2(dBgW_Base*, fpc_ProcID, void*);

    bool ChkUsed() const { return m_used; }
};  // Size: 0x14

STATIC_ASSERT(sizeof(cBgS_ChkElm) == 0x14);

class cBgS {
public:
    /* 0x0000 */ cBgS_ChkElm m_chk_element[256];
    /* 0x1400 vtable */

public:
    cBgS() {}
    bool Regist(dBgW_Base*, fpc_ProcID, void*);
    bool Release(dBgW_Base*);
    bool LineCross(cBgS_LinChk*);
    f32 GroundCross(cBgS_GndChk*);
    static void* ConvDzb(void*);
    fopAc_ac_c* GetActorPointer(int) const;
    dBgW_Base* GetBgWBasePointer(cBgS_PolyInfo const&) const;
    bool ChkPolySafe(cBgS_PolyInfo const&);
    s32 GetGrpRoomId(cBgS_PolyInfo const&) const;
    bool GetTriPla(cBgS_PolyInfo const&, cM3dGPla*) const;
    bool GetTriPnt(cBgS_PolyInfo const&, cXyz*, cXyz*, cXyz*) const;
    void ShdwDraw(cBgS_ShdwDraw*);
    u32 GetGrpInf(cBgS_PolyInfo const&) const;
    void Draw();

    virtual ~cBgS() {}
    virtual void Ct();
    virtual void Dt();

    fopAc_ac_c* GetActorPointer(cBgS_PolyInfo const& info) const {
        return GetActorPointer(info.GetBgIndex());
    }
};  // Size: 0x1404

class dBgS_Acch;

u8 dKy_pol_sound_get(cBgS_PolyInfo const* param_0);

class dBgS_HIO : public JORReflexible {
public:
    enum flags_e {
        FLAG_ACCH_WALL_OFF_e   = 0x2,
        FLAG_CHECK_COUNTER_e   = 0x4,
        FLAG_ACCH_WALL_TIMER_e = 0x8,
        FLAG_OBJ_LINE_CHECK_e  = 0x10,
        FLAG_ACCH_DRAW_WALL_e  = 0x400,
        FLAG_ROOF_TIMER_e      = 0x2000,
        FLAG_ROOF_OFF_e        = 0x4000,

        FLAG2_GND_CHK_e            = 0x1,
        FLAG2_SHAPE_DISP_e         = 0x2,
        FLAG2_GROUND_CHECK_TIMER_e = 0x4,
        FLAG2_SPL_OFF_e            = 0x8,
        FLAG2_SPL_TIMER_e          = 0x10,
        FLAG2_LINE_OFF_e           = 0x40,
        FLAG2_LINE_TIMER_e         = 0x80,
        FLAG2_SHDW_DRAW_OFF_e      = 0x100,
        FLAG2_SHDW_DRAW_TIMER_e    = 0x200,
        FLAG2_CAPTPOLY_OFF_e       = 0x800,
        FLAG2_CAPTPOLY_TIMER_e     = 0x1000,
        FLAG2_SPH_CHK_OFF_e        = 0x2000,
        FLAG2_SPH_CHK_TIMER_e      = 0x4000,
    };

    dBgS_HIO() {
        m_flags = 0;
        m_flags2 = 0;

        m_linecheck_start.x = m_linecheck_start.y = m_linecheck_start.z = 0.0f;
        m_linecheck_end.x = m_linecheck_end.y = m_linecheck_end.z = 0.0f;
        m_gndcheck_pos.x = m_gndcheck_pos.y = m_gndcheck_pos.z = 0.0f;

        m_errorCheck_actor_num = -1;
    }

    virtual void genMessage(JORMContext*);
    virtual ~dBgS_HIO() {}

    bool ChkLineOff() { return m_flags2 & FLAG2_LINE_OFF_e; }
    bool ChkCheckCounter() { return m_flags & FLAG_CHECK_COUNTER_e; }
    bool ChkLineTimer() { return m_flags2 & FLAG2_LINE_TIMER_e; }
    bool ChkGroundCheckTimer() { return m_flags2 & FLAG2_GROUND_CHECK_TIMER_e; }
    bool ChkObjLineCheck() { return m_flags & FLAG_OBJ_LINE_CHECK_e; }
    bool ChkGndCheck() { return m_flags2 & FLAG2_GND_CHK_e; }
    bool ChkCaptPolyTimer() { return m_flags2 & FLAG2_CAPTPOLY_TIMER_e; }
    bool ChkCaptPolyOff() { return m_flags2 & FLAG2_CAPTPOLY_OFF_e; }
    bool ChkShdwDrawOff() { return m_flags2 & FLAG2_SHDW_DRAW_OFF_e; }
    bool ChkShdwDrawTimer() { return m_flags2 & FLAG2_SHDW_DRAW_TIMER_e; }
    bool ChkAcchWallOff() { return m_flags & FLAG_ACCH_WALL_OFF_e; }
    bool ChkAcchWallTimer() { return m_flags & FLAG_ACCH_WALL_TIMER_e; }
    bool ChkRoofTimer() { return m_flags & FLAG_ROOF_TIMER_e; }
    bool ChkRoofOff() { return m_flags & FLAG_ROOF_OFF_e; }
    bool ChkSplOff() { return m_flags2 & FLAG2_SPL_OFF_e; }
    bool ChkSplTimer() { return m_flags2 & FLAG2_SPL_TIMER_e; }
    bool ChkSphChkOff() { return m_flags2 & FLAG2_SPH_CHK_OFF_e; }
    bool ChkSphChkTimer() { return m_flags2 & FLAG2_SPH_CHK_TIMER_e; }
    bool ChkShapeDisp() { return m_flags2 & FLAG2_SHAPE_DISP_e; }
    bool ChkAcchDrawWall() { return m_flags & FLAG_ACCH_DRAW_WALL_e; }

    /* 0x04 */ s8 id;
    /* 0x06 */ u16 m_flags;
    /* 0x08 */ u16 m_flags2;
    /* 0x0C */ cXyz m_linecheck_start;
    /* 0x18 */ cXyz m_linecheck_end;
    /* 0x24 */ cXyz m_gndcheck_pos;
    /* 0x30 */ s32 m_errorCheck_actor_num;
};

class dBgS_InsideHIO : public JORReflexible {
public:
    enum flags_e {
        FLAG_DISP_POLY_e           = 0x1,
        FLAG_DISP_DP_AREA_e        = 0x2,
        FLAG_WHITE_WIRE_e          = 0x4,
        FLAG_PLAYER_AROUND_e       = 0x8,
        FLAG_DISP_WATER_POLY_e     = 0x10,
        FLAG_GROUND_OFF_e          = 0x20,
        FLAG_WALL_OFF_e            = 0x40,
        FLAG_ROOF_OFF_e            = 0x80,
        FLAG_GNDCHK_PLAYER_UNDER_e = 0x100,
    };

    dBgS_InsideHIO() {
        m_flags = FLAG_PLAYER_AROUND_e;

        m_raise_amount = 1.0f;

        m_p0.x = 100.0f;
        m_p0.y = 0.0f;
        m_p0.z = 0.0f;

        m_p1.x = 0.0f;
        m_p1.y = 0.0f;
        m_p1.z = 0.0f;
    }

    virtual void genMessage(JORMContext*);
    virtual ~dBgS_InsideHIO();

    BOOL ChkWallOff() { return m_flags & FLAG_WALL_OFF_e; }
    BOOL ChkRoofOff() { return m_flags & FLAG_ROOF_OFF_e; }
    BOOL ChkGroundOff() { return m_flags & FLAG_GROUND_OFF_e; }
    BOOL ChkGndChkPlayerUnder() { return m_flags & FLAG_GNDCHK_PLAYER_UNDER_e; }
    BOOL ChkWhiteWire() { return m_flags & FLAG_WHITE_WIRE_e; }
    BOOL ChkDispWaterPoly() { return m_flags & FLAG_DISP_WATER_POLY_e; }
    BOOL ChkDispDpArea() { return m_flags & FLAG_DISP_DP_AREA_e; }
    BOOL ChkPlayerAround() { return m_flags & FLAG_PLAYER_AROUND_e; }
    BOOL ChkDispPoly() { return m_flags & FLAG_DISP_POLY_e; }

    /* 0x04 */ s8 id;
    /* 0x06 */ u16 m_flags;
    /* 0x08 */ f32 m_raise_amount;
    /* 0x0C */ cXyz m_p0;
    /* 0x18 */ cXyz m_p1;
};

class dBgS : public cBgS {
public:
    dBgS() {}
    ~dBgS() {}
    void Ct();
    void Dt();
    void ClrMoveFlag();
    void Move();
    bool Regist(dBgW_Base*, fopAc_ac_c*);
    bool ChkMoveBG(cBgS_PolyInfo const&);
    bool ChkMoveBG_NoDABg(cBgS_PolyInfo const&);
    s32 GetExitId(cBgS_PolyInfo const&);
    s32 GetPolyColor(cBgS_PolyInfo const&);
    BOOL GetHorseNoEntry(cBgS_PolyInfo const&);
    int GetSpecialCode(cBgS_PolyInfo const&);
    int GetMagnetCode(cBgS_PolyInfo const&);
    int GetMonkeyBarsCode(cBgS_PolyInfo const&);
    u32 GetUnderwaterRoofCode(cBgS_PolyInfo const&);
    s32 GetWallCode(cBgS_PolyInfo const&);
    int GetPolyAtt0(cBgS_PolyInfo const&);
    int GetPolyAtt1(cBgS_PolyInfo const&);
    int GetGroundCode(cBgS_PolyInfo const&);
    s32 GetCamMoveBG(cBgS_PolyInfo const&);
    s32 GetRoomCamId(cBgS_PolyInfo const&);
    s32 GetRoomPathId(cBgS_PolyInfo const&);
    s32 GetRoomPathPntNo(cBgS_PolyInfo const&);
    int GetGrpSoundId(cBgS_PolyInfo const&);
    u32 ChkGrpInf(cBgS_PolyInfo const&, u32);
    int GetRoomId(cBgS_PolyInfo const&);
    bool GetPolyAttackThrough(cBgS_PolyInfo const&);
    u32 ChkPolyHSStick(cBgS_PolyInfo const&);
    void WallCorrect(dBgS_Acch*);
    void WallCorrectSort(dBgS_Acch*);
    f32 RoofChk(dBgS_RoofChk*);
    bool SplGrpChk(dBgS_SplGrpChk*);
    bool SphChk(dBgS_SphChk*, void*);
    void MoveBgCrrPos(cBgS_PolyInfo const& i_poly, bool param_1, cXyz* i_pos, csXyz* i_angle,
                      csXyz* i_shapeAngle, bool param_5, bool param_6);
    void MoveBgTransPos(cBgS_PolyInfo const& i_poly, bool param_1, cXyz* i_pos, csXyz* i_angle,
                        csXyz* i_shapeAngle);
    void MoveBgMatrixCrrPos(cBgS_PolyInfo const&, bool, cXyz*, csXyz*, csXyz*);
    void RideCallBack(cBgS_PolyInfo const&, fopAc_ac_c*);
    void ArrowStickCallBack(cBgS_PolyInfo const&, fopAc_ac_c*, cXyz&);
    fopAc_ac_c* PushPullCallBack(cBgS_PolyInfo const&, fopAc_ac_c*, s16, dBgW_Base::PushPullLabel);

    bool WaterChk(dBgS_SplGrpChk* chk) { return SplGrpChk(chk); }
    u32 GetMtrlSndId(const cBgS_PolyInfo& param_0) { return dKy_pol_sound_get(&param_0); }
    void DrawPoly(cBgS_PolyInfo const& param_0, GXColor const& param_1);
    fopAc_ac_c* GetActorPointer(cBgS_PolyInfo const& param_0) const { return cBgS::GetActorPointer(param_0); }
    
    void DebugDrawPoly(const dBgW_Base& param_1);

    #if DEBUG
    bool LineCross(cBgS_LinChk* i_linChk);
    f32 GroundCross(cBgS_GndChk* i_gndChk);
    void ShdwDraw(cBgS_ShdwDraw*);
    #else
    bool LineCross(cBgS_LinChk* i_linChk) {
        return cBgS::LineCross(i_linChk);
    }

    f32 GroundCross(cBgS_GndChk* i_gndChk) {
        return cBgS::GroundCross(i_gndChk);
    }
    #endif

    void ChkDeleteActorRegist(fopAc_ac_c*);

    void Draw();
    void CaptPoly(dBgS_CaptPoly&);

    /* 0x1404 */ u8 field_0x1404[0x1408 - 0x1404];
    /* 0x1408 */ dBgS_HIO m_hio;
};  // Size: 0x1404

bool dBgS_CheckBGroundPoly(cBgS_PolyInfo const&);
bool dBgS_CheckBRoofPoly(cBgS_PolyInfo const&);
bool dBgS_CheckBWallPoly(cBgS_PolyInfo const&);
void dBgS_MoveBGProc_Typical(dBgW* param_0, void* param_1, cBgS_PolyInfo const& param_2,
                             bool param_3, cXyz* param_4, csXyz* param_5, csXyz* param_6);
void dBgS_MoveBGProc_TypicalRotY(dBgW* param_0, void* param_1, cBgS_PolyInfo const& param_2,
                                 bool param_3, cXyz* param_4, csXyz* param_5, csXyz* param_6);
void dBgS_MoveBGProc_Trans(dBgW* i_bgw, void* i_actor_ptr, cBgS_PolyInfo const& i_poly,
                           bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle);
f32 dBgS_GetNY(cBgS_PolyInfo const& poly);

#endif /* D_BG_D_BG_S_H */
