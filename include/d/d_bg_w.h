#ifndef D_BG_D_BG_W_H
#define D_BG_D_BG_W_H

#include "SSystem/SComponent/c_m3d_g_aab.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "d/d_bg_w_base.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"

class cBgS_GrpPassChk;
class cBgS_PolyPassChk;
class fopAc_ac_c;
struct cBgD_Vtx_t;
struct dBgS_CaptPoly;

class cBgW_TriElm {
public:
    /* 0x00 */ cM3dGPla m_plane;

    cBgW_TriElm();
    virtual ~cBgW_TriElm();

    /* 0x14  vtable */
};  // Size: 0x18

class cBgW_RwgElm {
public:
    /* 0x0 */ u16 m_next;

    cBgW_RwgElm();
    virtual ~cBgW_RwgElm();
};  // Size: 0x8

class cBgW_NodeTree : public cM3dGAab {
public:
    cBgW_NodeTree();

    virtual ~cBgW_NodeTree();
};

class cBgW_GrpElm {
public:
    cBgW_GrpElm();

    virtual ~cBgW_GrpElm();

    /* 0x4 */ cM3dGAab m_aab;
};

struct cBgW_BlkElm {
    /* 0x0 */ u16 m_roof_idx;
    /* 0x2 */ u16 m_wall_idx;
    /* 0x4 */ u16 m_gnd_idx;
};  // Size: 0x6

struct cBgD_Tri_t {
    /* 0x0 */ u16 m_vtx_idx0;
    /* 0x2 */ u16 m_vtx_idx1;
    /* 0x4 */ u16 m_vtx_idx2;
    /* 0x6 */ u16 m_id;
    /* 0x8 */ u16 m_grp;
};  // Size: 0xA

struct cBgD_Ti_t {
    /* 0x0 */ u32 m_info0;
    /* 0x4 */ u32 m_info1;
    /* 0x8 */ u32 m_info2;
    /* 0xC */ u32 m_passFlag;
};

struct cBgD_Blk_t {
    /* 0x0 */ u16 field_0x0;
};

struct cBgD_Tree_t {
    /* 0x0 */ u16 m_flag;
    /* 0x2 */ u16 m_parent_id;
    /* 0x4 */ u16 m_id[8];
};  // Size: 0x14

struct cBgD_Grp_t {
    /* 0x00 */ char* m_name;
    /* 0x04 */ cXyz m_scale;
    /* 0x10 */ csXyz m_rotation;
    /* 0x18 */ cXyz m_translation;
    /* 0x24 */ u16 m_parent;
    /* 0x26 */ u16 m_next_sibling;
    /* 0x28 */ u16 m_first_child;
    /* 0x2A */ u16 m_room_id;
    /* 0x2C */ u16 m_first_vtx_idx;
    /* 0x2E */ u16 m_tree_idx;
    /* 0x30 */ u32 m_info;
}; // Size: 0x34

struct cBgD_t {
    /* 0x00 */ int m_v_num;                // vertex num
    /* 0x04 */ cBgD_Vtx_t* m_v_tbl;        // vertex table
    /* 0x08 */ int m_t_num;                // triangle num
    /* 0x0C */ cBgD_Tri_t* m_t_tbl;        // triangle table
    /* 0x10 */ int m_b_num;
    /* 0x14 */ cBgD_Blk_t* m_b_tbl;
    /* 0x18 */ int m_tree_num;
    /* 0x1C */ cBgD_Tree_t* m_tree_tbl;
    /* 0x20 */ int m_g_num;
    /* 0x24 */ cBgD_Grp_t* m_g_tbl;
    /* 0x28 */ int m_ti_num;
    /* 0x2C */ cBgD_Ti_t* m_ti_tbl;
    /* 0x30 */ int mFlags;
};

class cBgW : public dBgW_Base {
public:
    enum Flags_e {
        MOVE_BG_e = 0x1,
        NO_CALC_VTX_e = 0x2,
        NO_VTX_TBL_e = 0x10,
        GLOBAL_e = 0x20,
        CBGW_UNK_FLAG_40 = 0x40,
        LOCK_e = 0x80,
    };

    cBgW();
    void FreeArea();
    void GlobalVtx();
    bool SetVtx();
    bool SetTri();
    void BlckConnect(u16*, int*, int);
    void MakeBlckTransMinMax(cXyz*, cXyz*);
    void MakeBlckMinMax(int, cXyz*, cXyz*);
    void MakeBlckBnd(int, cXyz*, cXyz*);
    void MakeNodeTreeRp(int);
    void MakeNodeTreeGrpRp(int);
    void MakeNodeTree();
    bool Set(cBgD_t* pdzb, u32 flags, Mtx* pbase_mtx);
    bool RwgLineCheck(u16, cBgS_LinChk*);
    bool LineCheckRp(cBgS_LinChk*, int);
    bool LineCheckGrpRp(cBgS_LinChk*, int, int);
    bool RwgGroundCheckCommon(f32, u16, cBgS_GndChk*);
    bool RwgGroundCheckGnd(u16, cBgS_GndChk*);
    bool RwgGroundCheckWall(u16, cBgS_GndChk*);
    bool GroundCrossRp(cBgS_GndChk*, int);
    bool GroundCrossGrpRp(cBgS_GndChk*, int, int);
    void CopyOldMtx();
    void Move();
    void RwgShdwDraw(int, cBgS_ShdwDraw*);
    void ShdwDrawRp(cBgS_ShdwDraw*, int);
    void ShdwDrawGrpRp(cBgS_ShdwDraw*, int);
    void GetTrans(cXyz*) const;
    int GetPolyInfId(int) const;
    u32 GetPolyInf0(int, u32, u32) const;
    u32 GetMaskPolyInf0_NoShift(int, u32) const;
    u32 GetPolyInf1(int, u32, u32) const;
    u32 GetMaskPolyInf1_NoShift(int, u32) const;
    u32 GetPolyInf2(int, u32, u32) const;
    int GetTriGrp(int) const;

    virtual ~cBgW();
    virtual bool ChkMemoryError();
    virtual bool ChkNotReady() const;
    virtual bool ChkLock() const;
    virtual bool ChkMoveBg() const;
    virtual u32 ChkMoveFlag() const = 0;
    virtual cM3dGPla GetTriPla(cBgS_PolyInfo const&) const;
    virtual bool GetTriPnt(cBgS_PolyInfo const&, cXyz*, cXyz*, cXyz*) const;
    virtual cM3dGAab* GetBnd() const;
    virtual u32 GetGrpInf(cBgS_PolyInfo const&) const;
    virtual void OffMoveFlag() = 0;
    virtual void GetTopUnder(f32*, f32*) const;
    virtual bool LineCheck(cBgS_LinChk*);
    virtual bool GroundCross(cBgS_GndChk*);
    virtual void ShdwDraw(cBgS_ShdwDraw*);
    virtual void CaptPoly(dBgS_CaptPoly&) = 0;
    virtual bool WallCorrect(dBgS_Acch*) = 0;
    virtual bool WallCorrectSort(dBgS_Acch*) = 0;
    virtual bool RoofChk(dBgS_RoofChk*) = 0;
    virtual bool SplGrpChk(dBgS_SplGrpChk*) = 0;
    virtual bool SphChk(dBgS_SphChk*, void*) = 0;
    virtual int GetGrpRoomIndex(cBgS_PolyInfo const&) const;
    virtual int GetExitId(cBgS_PolyInfo const&) = 0;
    virtual int GetPolyColor(cBgS_PolyInfo const&) = 0;
    virtual BOOL GetHorseNoEntry(cBgS_PolyInfo const&) = 0;
    virtual int GetSpecialCode(cBgS_PolyInfo const&) = 0;
    virtual int GetSpecialCode(int) = 0;
    virtual int GetMagnetCode(cBgS_PolyInfo const&) = 0;
    virtual u32 GetPolyObjThrough(int) = 0;
    virtual u32 GetPolyCamThrough(int) = 0;
    virtual u32 GetPolyLinkThrough(int) = 0;
    virtual u32 GetPolyArrowThrough(int) = 0;
    virtual u32 GetPolyHSStick(int) = 0;
    virtual u32 GetPolyBoomerangThrough(int) = 0;
    virtual u32 GetPolyRopeThrough(int) = 0;
    virtual u32 GetPolyBombThrough(int) = 0;
    virtual bool GetShdwThrough(int) = 0;
    virtual u32 GetUnderwaterRoofCode(int) = 0;
    virtual int GetMonkeyBarsCode(cBgS_PolyInfo const&) = 0;
    virtual int GetLinkNo(cBgS_PolyInfo const&) = 0;
    virtual int GetWallCode(cBgS_PolyInfo const&) = 0;
    virtual int GetPolyAtt0(cBgS_PolyInfo const&) = 0;
    virtual int GetPolyAtt1(cBgS_PolyInfo const&) = 0;
    virtual int GetGroundCode(cBgS_PolyInfo const&) = 0;
    virtual u32 GetIronBallThrough(int) = 0;
    virtual u32 GetAttackThrough(int) = 0;
    virtual int GetCamMoveBG(cBgS_PolyInfo const&) = 0;
    virtual int GetRoomCamId(cBgS_PolyInfo const&) = 0;
    virtual int GetRoomPathId(cBgS_PolyInfo const&) = 0;
    virtual int GetRoomPathPntNo(cBgS_PolyInfo const&) = 0;
    virtual u8 GetPolyGrpRoomInfId(cBgS_PolyInfo const&) = 0;
    virtual int GetGrpSoundId(cBgS_PolyInfo const&) = 0;
    virtual void CrrPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*, csXyz*) = 0;
    virtual void TransPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*, csXyz*) = 0;
    virtual void MatrixCrrPos(cBgS_PolyInfo const&, void*, bool, cXyz*, csXyz*, csXyz*) = 0;
    virtual void CalcPlane();
    virtual void ClassifyPlane();
    virtual bool ChkPolyThrough(int, cBgS_PolyPassChk*);
    virtual bool ChkShdwDrawThrough(int, cBgS_PolyPassChk*);
    virtual bool ChkGrpThrough(int, cBgS_GrpPassChk*, int);

    u32 GetOldInvMtx(Mtx m) { return MTXInverse(m_inv_mtx, m); }
    MtxP GetBaseMtxP() { return pm_base; }
    bool ChkNoCalcVtx() { return mFlags & NO_CALC_VTX_e; }
    bool ChkFlush() { return field_0x91 & 8; }
    void SetLock() { mFlags |= LOCK_e; }
    void OffRoofRegist() { field_0x91 |= 0x4; }
    void OnRoofRegist() { field_0x91 &= ~0x04; }
    bool ChkRoofRegist() { return field_0x91 & 4;}
    cBgD_Vtx_t* GetVtxTbl() const { return pm_vtx_tbl; }
    int GetVtxNum() const { return pm_bgd->m_v_num; }
    void ClrNoCalcVtx() { mFlags &= ~NO_CALC_VTX_e; }
    void SetBaseMtxP(MtxP mtx) { pm_base = mtx; }
    void SetNoCalcVtx() { mFlags |= NO_CALC_VTX_e; }

public:
    /* 0x18 */ MtxP pm_base;  // Model Matrix
    /* 0x1C */ Mtx m_inv_mtx;
    /* 0x4C */ Mtx m_mtx;
    /* 0x7C */ cXyz mTransVel;
    /* 0x88 */ u8 mFlags;
    /* 0x89 */ u8 mNeedsFullTransform;
    /* 0x8C */ u32 field_0x8c;
    /* 0x90 */ u8 mMoveCounter;
    /* 0x91 */ u8 field_0x91;
    /* 0x92 */ u16 m_rootGrpIdx;
    /* 0x94 */ cBgW_TriElm* pm_tri;
    /* 0x98 */ cBgW_RwgElm* pm_rwg;
    /* 0x9C */ cBgD_Vtx_t* pm_vtx_tbl;
    /* 0xA0 */ cBgD_t* pm_bgd;
    /* 0xA4 */ cBgW_BlkElm* pm_blk;
    /* 0xA8 */ cBgW_GrpElm* pm_grp;
    /* 0xAC */ cBgW_NodeTree* pm_node_tree;
};

class dBgW;
typedef void (*dBgW_RideCallback)(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
typedef void (*dBgW_ArrowStickCallback)(dBgW*, fopAc_ac_c*, fopAc_ac_c*, cXyz&);
typedef void (*dBgW_CrrFunc)(dBgW*, void*, cBgS_PolyInfo const&, bool, cXyz*, csXyz*, csXyz*);

class dBgW : public cBgW {
public:
    dBgW();
    void Move();
    void positionWallCorrect(dBgS_Acch*, f32, cM3dGPla&, cXyz* pupper_pos, f32);
    bool RwgWallCorrect(dBgS_Acch*, u16);
    bool WallCorrectRp(dBgS_Acch*, int);
    bool WallCorrectGrpRp(dBgS_Acch*, int, int);
    void RwgWallCorrectSort(dBgS_Acch*, u16);
    void WallCorrectRpSort(dBgS_Acch*, int);
    bool WallCorrectGrpRpSort(dBgS_Acch*, int, int);
    bool RwgRoofChk(u16, dBgS_RoofChk*);
    bool RoofChkRp(dBgS_RoofChk*, int);
    bool RoofChkGrpRp(dBgS_RoofChk*, int, int);
    bool RwgSplGrpChk(u16, dBgS_SplGrpChk*);
    bool SplGrpChkRp(dBgS_SplGrpChk*, int);
    bool SplGrpChkGrpRp(dBgS_SplGrpChk*, int, int);
    void RwgCaptPoly(int, dBgS_CaptPoly&);
    void CaptPolyRp(dBgS_CaptPoly&, int);
    void CaptPolyGrpRp(dBgS_CaptPoly&, int, int);
    bool RwgSphChk(u16, dBgS_SphChk*, void*);
    bool SphChkRp(dBgS_SphChk*, void*, int);
    bool SphChkGrpRp(dBgS_SphChk*, void*, int, int);

    virtual ~dBgW() {}
    virtual u32 ChkMoveFlag() const;
    virtual void OffMoveFlag();
    virtual void CaptPoly(dBgS_CaptPoly&);
    virtual bool WallCorrect(dBgS_Acch*);
    virtual bool WallCorrectSort(dBgS_Acch*);
    virtual bool RoofChk(dBgS_RoofChk*);
    virtual bool SplGrpChk(dBgS_SplGrpChk*);
    virtual bool SphChk(dBgS_SphChk*, void*);
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
    virtual void CallRideCallBack(fopAc_ac_c*, fopAc_ac_c*);
    virtual void CallArrowStickCallBack(fopAc_ac_c*, fopAc_ac_c*, cXyz&);
    virtual bool ChkPolyThrough(int, cBgS_PolyPassChk*);
    virtual bool ChkShdwDrawThrough(int, cBgS_PolyPassChk*);
    virtual bool ChkGrpThrough(int, cBgS_GrpPassChk*, int);

    void SetCrrFunc(dBgW_CrrFunc func) { m_crr_func = func; }
    void SetRideCallback(dBgW_RideCallback func) { m_ride_callback = func; }
    void SetArrowStickCallback(dBgW_ArrowStickCallback func) { m_arrow_stick_callback = func; }
    void OnMoveFlag() { m_flags |= 1; }

private:
    /* 0xB0 */ dBgW_CrrFunc m_crr_func;
    /* 0xB4 */ dBgW_RideCallback m_ride_callback;
    /* 0xB8 */ dBgW_ArrowStickCallback m_arrow_stick_callback;
    /* 0xBC */ u8 m_flags;
};

dBgW* dBgW_NewSet(cBgD_t* pbgd, u32 flags, Mtx* pbase_mtx);

#endif /* D_BG_D_BG_W_H */
