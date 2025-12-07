#ifndef D_BG_D_BG_S_H
#define D_BG_D_BG_S_H

#include "d/d_bg_w_base.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include <dolphin/os.h>
#include "global.h"

class dBgW;
class fopAc_ac_c;

class cBgS_ChkElm {
public:
    /* 0x00 */ dBgW_Base* m_bgw_base_ptr;
    /* 0x04 */ bool m_used;
    /* 0x08 */ u32 m_actor_id;
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
#if DEBUG
    dBgS_HIO() {
        field_0x6 = 0;
        field_0x8 = 0;

        field_0xc.x = field_0xc.y = field_0xc.z = 0.0f;
        field_0x18.x = field_0x18.y = field_0x18.z = 0.0f;
        field_0x24.x = field_0x24.y = field_0x24.z = 0.0f;

        field_0x30 = -1;
    }

    virtual void genMessage(JORMContext*);
    virtual ~dBgS_HIO();

    BOOL ChkLineOff();
    BOOL ChkCheckCounter();
    BOOL ChkLineTimer();
    BOOL ChkGroundCheckTimer();

    /* 0x04 */ u8 field_0x4[0x6 - 0x4];
    /* 0x06 */ u16 field_0x6;
    /* 0x08 */ u16 field_0x8;
    /* 0x0C */ cXyz field_0xc;
    /* 0x18 */ cXyz field_0x18;
    /* 0x24 */ cXyz field_0x24;
    /* 0x30 */ int field_0x30;
#endif
};

extern int g_line_counter;
extern OSStopwatch s_line_sw;

extern int g_ground_counter;
extern OSStopwatch s_ground_sw;

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
    void DebugDrawPoly(dBgW_Base *param_1) {}
    fopAc_ac_c* GetActorPointer(cBgS_PolyInfo const& param_0) const { return cBgS::GetActorPointer(param_0); }
    bool LineCross(cBgS_LinChk* i_linChk) {
        #if DEBUG
        if (m_hio.ChkLineOff()) {
            return false;
        }
        if (m_hio.ChkCheckCounter()) {
            g_line_counter++;
        }
        if (m_hio.ChkLineTimer()) {
            OSStartStopwatch(&s_line_sw);
        }
        bool rt = cBgS::LineCross(i_linChk);
        if (m_hio.ChkLineTimer()) {
            OSStopStopwatch(&s_line_sw);
            OSDumpStopwatch(&s_line_sw);
        }
        return rt;
        #else
        return cBgS::LineCross(i_linChk);
        #endif
    }
    f32 GroundCross(cBgS_GndChk* i_gndChk) {
        #if DEBUG
        if (m_hio.ChkCheckCounter()) {
            g_ground_counter++;
        }
        if (m_hio.ChkGroundCheckTimer()) {
            OSStartStopwatch(&s_ground_sw);
        }
        f32 rt = cBgS::GroundCross(i_gndChk);
        if (m_hio.ChkGroundCheckTimer()) {
            OSStopStopwatch(&s_ground_sw);
            OSDumpStopwatch(&s_ground_sw);
        }
        return rt;
        #else
        return cBgS::GroundCross(i_gndChk);
        #endif
    }

    void ChkDeleteActorRegist(fopAc_ac_c*);

#if DEBUG
    /* 0x1404 */ u8 field_0x1404[0x1408 - 0x1404];
    /* 0x1408 */ dBgS_HIO m_hio;
#endif
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
