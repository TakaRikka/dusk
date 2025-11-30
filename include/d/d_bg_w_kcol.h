#ifndef D_BG_D_BG_W_KCOL_H
#define D_BG_D_BG_W_KCOL_H

#include "JSystem/JUtility/JUTAssert.h"
#include "SSystem/SComponent/c_m3d_g_aab.h"
#include "d/d_bg_plc.h"
#include "d/d_bg_s_sph_chk.h"
#include "d/d_bg_w_base.h"

class cBgS_GrpPassChk;
class cBgS_PolyPassChk;
struct dBgPc;
struct dBgS_CaptPoly;

struct KC_PrismData {
    /* 0x0 */ f32 height;
    /* 0x4 */ u16 pos_i;
    /* 0x6 */ u16 fnrm_i;
    /* 0x8 */ u16 enrm1_i;
    /* 0xA */ u16 enrm2_i;
    /* 0xC */ u16 enrm3_i;
    /* 0xE */ u16 attribute;
};  // Size: 0x10

struct KC_Header {
    /* 0x00 */ Vec* m_pos_data;
    /* 0x04 */ Vec* m_nrm_data;
    /* 0x08 */ KC_PrismData* m_prism_data;
    /* 0x0C */ KC_PrismData* m_block_data;
    /* 0x10 */ f32 m_prism_thickness;
    /* 0x14 */ Vec m_area_min_pos;
    /* 0x20 */ u32 m_area_x_width_mask;
    /* 0x24 */ u32 m_area_y_width_mask;
    /* 0x28 */ u32 m_area_z_width_mask;
    /* 0x2C */ u32 m_block_width_shift;
    /* 0x30 */ u32 m_area_x_blocks_shift;
    /* 0x34 */ u32 m_area_xy_blocks_shift;
};

class dBgWKCol : public dBgW_Base {
public:
    dBgWKCol();
    static void* initKCollision(void*);
    void create(void* pprism, void* plc);
    void getTriNrm(KC_PrismData*, Vec**) const;
    cM3dGPla GetTriPla(int) const;
    bool GetTriPnt(int, Vec*, Vec*, Vec*) const;
    bool GetTriPnt(KC_PrismData const*, Vec*, Vec*, Vec*) const;
    void getPolyCode(int, dBgPc*) const;
    bool chkPolyThrough(dBgPc*, cBgS_PolyPassChk*, cBgS_GrpPassChk*, cXyz&) const;
    bool ChkShdwDrawThrough(dBgPc*);

    virtual ~dBgWKCol();
    virtual bool ChkNotReady() const;
    virtual bool ChkLock() const;
    virtual bool ChkMoveBg() const;
    virtual u32 ChkMoveFlag() const;
    virtual cM3dGPla GetTriPla(cBgS_PolyInfo const&) const;
    virtual bool GetTriPnt(cBgS_PolyInfo const&, cXyz*, cXyz*, cXyz*) const;
    virtual cM3dGAab* GetBnd() const;
    virtual u32 GetGrpInf(cBgS_PolyInfo const&) const;
    virtual void OffMoveFlag();
    virtual void GetTopUnder(f32*, f32*) const;
    virtual bool LineCheck(cBgS_LinChk*);
    virtual bool GroundCross(cBgS_GndChk*);
    virtual void ShdwDraw(cBgS_ShdwDraw*);
    virtual void CaptPoly(dBgS_CaptPoly&);
    virtual bool WallCorrect(dBgS_Acch*);
    virtual bool WallCorrectSort(dBgS_Acch*);
    virtual bool RoofChk(dBgS_RoofChk*);
    virtual bool SplGrpChk(dBgS_SplGrpChk*);
    virtual bool SphChk(dBgS_SphChk*, void*);
    virtual int GetGrpRoomIndex(cBgS_PolyInfo const&) const;
    virtual int GetExitId(cBgS_PolyInfo const&);
    virtual int GetPolyColor(cBgS_PolyInfo const&);
    virtual BOOL GetHorseNoEntry(cBgS_PolyInfo const&);
    virtual int GetSpecialCode(cBgS_PolyInfo const&);
    virtual int GetSpecialCode(int);
    virtual int GetMagnetCode(cBgS_PolyInfo const&);
    virtual u32 GetPolyObjThrough(int);
    virtual u32 GetPolyCamThrough(int);
    virtual u32 GetPolyLinkThrough(int);
    virtual u32 GetPolyArrowThrough(int);
    virtual u32 GetPolyHSStick(int);
    virtual u32 GetPolyBoomerangThrough(int);
    virtual u32 GetPolyRopeThrough(int);
    virtual u32 GetPolyBombThrough(int);
    virtual bool GetShdwThrough(int);
    virtual u32 GetUnderwaterRoofCode(int);
    virtual int GetMonkeyBarsCode(cBgS_PolyInfo const&);
    virtual int GetLinkNo(cBgS_PolyInfo const&);
    virtual int GetWallCode(cBgS_PolyInfo const&);
    virtual int GetPolyAtt0(cBgS_PolyInfo const&);
    virtual int GetPolyAtt1(cBgS_PolyInfo const&);
    virtual int GetGroundCode(cBgS_PolyInfo const&);
    virtual u32 GetIronBallThrough(int);
    virtual u32 GetAttackThrough(int);
    virtual int GetCamMoveBG(cBgS_PolyInfo const&);
    virtual int GetRoomCamId(cBgS_PolyInfo const&);
    virtual int GetRoomPathId(cBgS_PolyInfo const&);
    virtual int GetRoomPathPntNo(cBgS_PolyInfo const&);
    virtual u8 GetPolyGrpRoomInfId(cBgS_PolyInfo const&);
    virtual int GetGrpSoundId(cBgS_PolyInfo const&);
    virtual void CrrPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*,
                                       csXyz*);
    virtual void TransPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*,
                                         csXyz*);
    virtual void MatrixCrrPos(cBgS_PolyInfo const&, void*, bool, cXyz*,
                                             csXyz*, csXyz*);

    KC_PrismData* getPrismData(int poly_index) const {
        KC_PrismData* pd = &m_pkc_head->m_prism_data[poly_index];
        JUT_ASSERT(0x12E, pd < (KC_PrismData*)m_pkc_head->m_block_data);
        return pd;
    }

    void getTri1Pos(KC_PrismData* pd, Vec** nrm) const { *nrm = &m_pkc_head->m_pos_data[pd->pos_i]; }

private:
    /* 0x18 */ KC_Header* m_pkc_head;
    /* 0x1C */ dBgPlc m_code;
    /* 0x20 */ cM3dGAab m_bnd;
};

#endif /* D_BG_D_BG_W_KCOL_H */
