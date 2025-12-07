#ifndef D_BG_D_BG_S_ACCH_H
#define D_BG_D_BG_S_ACCH_H

#include "SSystem/SComponent/c_m3d_g_cir.h"
#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "SSystem/SComponent/c_m3d_g_lin.h"
#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_bg_s_roof_chk.h"
#include "d/d_bg_s_wtr_chk.h"
#include "JSystem/JUtility/JUTAssert.h"

class fopAc_ac_c;

class dBgS_AcchCir : public cBgS_PolyInfo {
private:
    /* 0x10 */ u32 m_flags;
    /* 0x14 */ cM3dGCir m_cir;
    /* 0x28 */ f32 m_wall_rr;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 m_wall_h;
    /* 0x34 */ f32 m_wall_r;
    /* 0x38 */ f32 m_wall_h_direct;
    /* 0x3C */ s16 m_wall_angle_y;

public:
    enum {
        /* 0x2 */ WALL_HIT = 2,
        /* 0x4 */ WALL_H_DIRECT = 4,
    };

    dBgS_AcchCir();
    void ClrWallHit();
    void SetWallR(f32);
    void CalcWallRR();
    void SetWall(f32, f32);
    void SetWallHDirect(f32);

    #ifdef __dt__12dBgS_AcchCirFv_DEFINED
    virtual ~dBgS_AcchCir();
    #else
    virtual ~dBgS_AcchCir() {}
    #endif

    f32 GetWallH() { return m_wall_h; }
    f32 GetWallR() { return m_wall_r; }
    void SetWallH(f32 h) { m_wall_h = h; }
    void ClrWallHDirect() { m_flags &= ~WALL_H_DIRECT; }
    bool ChkWallHit() { return (m_flags & WALL_HIT) != 0; }
    s16 GetWallAngleY() { return m_wall_angle_y; }
    bool ChkWallHDirect() { return (m_flags & WALL_H_DIRECT) != 0; }
    f32 GetWallHDirect() { return m_wall_h_direct; }
    f32 GetWallRR() { return m_wall_rr; }
    void SetWallHit() { m_flags |= WALL_HIT; }
    void SetWallAngleY(s16 i_angle) { m_wall_angle_y = i_angle; }
    cM3dGCir* GetCirP() { return &m_cir; }

    void SetCir(cXyz& pos) { m_cir.Set(pos.x, pos.z, pos.y + GetWallH(), m_wall_r); }
};  // Size: 0x40

class dBgS;
class dBgS_Acch : public cBgS_Chk, public dBgS_Chk {
public:
    enum {
        /* 0x000002 */ GRND_NONE = (1 << 1),
        /* 0x000004 */ WALL_NONE = (1 << 2),
        /* 0x000008 */ ROOF_NONE = (1 << 3),
        /* 0x000010 */ WALL_HIT = (1 << 4),
        /* 0x000020 */ GROUND_HIT = (1 << 5),
        /* 0x000040 */ GROUND_FIND = (1 << 6),
        /* 0x000080 */ GROUND_LANDING = (1 << 7),
        /* 0x000100 */ GROUND_AWAY = (1 << 8),
        /* 0x000200 */ ROOF_HIT = (1 << 9),
        /* 0x000400 */ WATER_NONE = (1 << 10),
        /* 0x000800 */ WATER_HIT = (1 << 11),
        /* 0x001000 */ WATER_IN = (1 << 12),
        /* 0x002000 */ LINE_CHECK = (1 << 13),
        /* 0x004000 */ LINE_CHECK_NONE = (1 << 14),
        /* 0x008000 */ CLR_SPEED_Y = (1 << 15),
        /* 0x010000 */ LINE_CHECK_HIT = (1 << 16),
        /* 0x100000 */ MOVE_BG_ONLY = (1 << 20),
        /* 0x200000 */ GND_THIN_CELLING_OFF = (1 << 21),
        /* 0x400000 */ WALL_SORT = (1 << 22),
        /* 0x800000 */ LINE_DOWN = (1 << 23),
    };

    dBgS_Acch();
    void Init();
    void Set(fopAc_ac_c*, int, dBgS_AcchCir*);
    void Set(cXyz*, cXyz*, fopAc_ac_c*, int, dBgS_AcchCir*, cXyz*, csXyz*, csXyz*);
    void GroundCheckInit(dBgS&);
    void GroundCheck(dBgS&);
    void GroundRoofProc(dBgS&);
    void LineCheck(dBgS&);
    void CrrPos(dBgS&);
    f32 GetWallAllR();
    void SetWallCir();
    void CalcWallBmdCyl();
    void SetGroundUpY(f32);
    f32 GetWallAllLowH();
    f32 GetWallAllLowH_R();
    f32 GetSpeedY();
    f32 GetWallAddY(Vec&);
    void SetNowActorInfo(int, void*, fpc_ProcID);
    void SetWallPolyIndex(int, int);
    void CalcMovePosWork();
    void CalcWallRR();
    void SetMoveBGOnly();
    void ClrMoveBGOnly();
    void SetGndThinCellingOff();
    void ClrGndThinCellingOff();
    bool ChkGndThinCellingOff();
    void OnWallSort();
    bool ChkWallSort();
    bool ChkLineDown();

    virtual ~dBgS_Acch();

    void DrawWall(dBgS&);

    cXyz* GetPos() { return pm_pos; }
    cXyz* GetOldPos() { return pm_old_pos; }
    f32 GetGroundH() const { return m_ground_h; }
    f32 GetRoofHeight() const { return m_roof_height; }
    int GetTblSize() { return m_tbl_size; }
    void SetLin() { m_lin.SetStartEnd(*pm_old_pos, *pm_pos); }
    bool ChkGroundFind() { return (m_flags & GROUND_FIND) != 0; }
    bool ChkGroundHit() const { return (m_flags & GROUND_HIT) != 0; }
    bool ChkGroundLanding() const { return (m_flags & GROUND_LANDING) != 0; }
    void ClrGroundLanding() { m_flags &= ~GROUND_LANDING; }
    void ClrGroundAway() { m_flags &= ~GROUND_AWAY; }
    void ClrWallHit() { m_flags &= ~WALL_HIT; }
    void SetRoofNone() { m_flags |= ROOF_NONE; }
    void SetRoofHit() { m_flags |= ROOF_HIT; }
    void SetWaterNone() { m_flags |= WATER_NONE; }
    u32 ChkWallHit() const { return m_flags & WALL_HIT; }
    void OffLineCheckHit() { m_flags &= ~LINE_CHECK_HIT; }
    void OffLineCheck() { m_flags &= ~LINE_CHECK; }
    u32 ChkLineCheckNone() const { return (m_flags & LINE_CHECK_NONE) != 0; }
    u32 ChkLineCheck() const { return (m_flags & LINE_CHECK) != 0; }
    void ClrRoofHit() { m_flags &= ~ROOF_HIT; }
    void ClrWaterHit() { m_flags &= ~WATER_HIT; }
    void SetWaterHit() { m_flags |= WATER_HIT; }
    void ClrWaterIn() { m_flags &= ~WATER_IN; }
    void SetWaterIn() { m_flags |= WATER_IN; }
    const u32 MaskWaterIn() const { return m_flags & WATER_IN; }
    const bool ChkWaterIn() const { return MaskWaterIn() != 0; }
    void ClrGroundFind() { m_flags &= ~GROUND_FIND; }
    u32 MaskRoofHit() const { return m_flags & ROOF_HIT; }
    bool ChkRoofHit() const { return MaskRoofHit() != 0; }
    void OffClrSpeedY() { m_flags |= CLR_SPEED_Y; }
    void OnClrSpeedY() { m_flags &= ~CLR_SPEED_Y; }
    bool ChkClrSpeedY() const { return !(m_flags & CLR_SPEED_Y); }
    void SetGroundFind() { m_flags |= GROUND_FIND; }
    void SetGroundHit() { m_flags |= GROUND_HIT; }
    void SetGroundLanding() { m_flags |= GROUND_LANDING; }
    void SetGroundAway() { m_flags |= GROUND_AWAY; }
    const u32 MaskWaterHit() const { return m_flags & WATER_HIT; }
    const bool ChkWaterHit() const { return MaskWaterHit() != 0; }
    void ClrWaterNone() { m_flags &= ~WATER_NONE; }
    void SetWaterCheckOffset(f32 offset) { m_wtr_chk_offset = offset; }
    void OnLineCheck() { m_flags |= LINE_CHECK; }
    void ClrRoofNone() { m_flags &= ~ROOF_NONE; }
    void SetRoofCrrHeight(f32 height) { m_roof_crr_height = height; }
    void SetWtrChkMode(int mode) { m_wtr_mode = mode; }
    void SetGrndNone() { m_flags |= GRND_NONE; }
    void ClrGrndNone() { m_flags &= ~GRND_NONE; }
    bool ChkMoveBGOnly() const { return (m_flags & MOVE_BG_ONLY) != 0; }
    void SetWallHit() { m_flags |= WALL_HIT; }
    void ClrWallNone() { m_flags &= ~WALL_NONE; }
    void OnLineCheckNone() { m_flags |= LINE_CHECK_NONE; }
    void OffLineCheckNone() { m_flags &= ~LINE_CHECK_NONE; }
    void SetWallNone() { m_flags |= WALL_NONE; }
    void OnLineCheckHit() { m_flags |= LINE_CHECK_HIT; }
    bool ChkGroundAway() const { return (m_flags & GROUND_AWAY) != 0; }
    void ClrGroundHit() { m_flags &= ~GROUND_HIT; }
    void SetGroundCheckOffset(f32 offset) { m_gnd_chk_offset = offset; }

    cM3dGCyl* GetWallBmdCylP() { return &m_wall_cyl; }
    fopAc_ac_c* getMyAc() { return m_my_ac; }
    
    cM3dGCir* GetWallCirP(int index) {
        JUT_ASSERT(730, index <= m_tbl_size);
        return pm_acch_cir[index].GetCirP();
    }

    f32 GetWallH(int i_no) { return pm_acch_cir[i_no].GetWallH(); }
    f32 GetWallR(int i_no) { return pm_acch_cir[i_no].GetWallR(); }
    bool ChkWallHDirect(int i_no) { return pm_acch_cir[i_no].ChkWallHDirect(); }
    f32 GetWallHDirect(int i_no) { return pm_acch_cir[i_no].GetWallHDirect(); }
    f32 GetWallRR(int i_no) { return pm_acch_cir[i_no].GetWallRR(); }
    void SetWallCirHit(int i_no) { pm_acch_cir[i_no].SetWallHit(); }
    void SetWallAngleY(int i_no, s16 i_angle) { pm_acch_cir[i_no].SetWallAngleY(i_angle); }

    f32 GetCx() const { return pm_pos->x; }
    f32 GetCz() const { return pm_pos->z; }
    
public:
    /* 0x02C */ u32 m_flags;
    /* 0x030 */ cXyz* pm_pos;
    /* 0x034 */ cXyz* pm_old_pos;
#if DEBUG
    /* 0x038 */ cXyz unk_0x38;
#endif
    /* 0x038 */ cXyz* pm_speed;
    /* 0x03C */ csXyz* pm_angle;
    /* 0x040 */ csXyz* pm_shape_angle;
    /* 0x044 */ cM3dGLin m_lin;
    /* 0x060 */ cM3dGCyl m_wall_cyl;
    /* 0x078 */ int m_bg_index;
    /* 0x07C */ void* field_0x7c;
    /* 0x080 */ u32 field_0x80;
    /* 0x084 */ fopAc_ac_c* m_my_ac;
    /* 0x088 */ int m_tbl_size;
    /* 0x08C */ dBgS_AcchCir* pm_acch_cir;
    /* 0x090 */ f32 field_0x90;
    /* 0x094 */ f32 field_0x94;
    /* 0x098 */ f32 m_ground_h;
    /* 0x09C */ f32 m_gnd_chk_offset;
    /* 0x0A0 */ cM3dGPla field_0xa0;
    /* 0x0B4 */ bool field_0xb4;
    /* 0x0B8 */ f32 field_0xb8;
    /* 0x0BC */ f32 field_0xbc;
    /* 0x0C0 */ u8 field_0xc0;
    /* 0x0C4 */ f32 m_roof_height;
    /* 0x0C8 */ f32 m_roof_crr_height;
    /* 0x0CC */ f32 field_0xcc;
    /* 0x0D0 */ f32 m_wtr_chk_offset;
    /* 0x0D4 */ cBgS_PolyInfo* pm_out_poly_info;
#if DEBUG
    /* 0x0E4 */ cXyz unk_0xe4;
#endif
    /* 0x0D8 */ f32 field_0xd8;
    /* 0x0DC */ dBgS_GndChk m_gnd;
    /* 0x130 */ dBgS_RoofChk m_roof;
    /* 0x180 */ dBgS_WtrChk m_wtr;
    /* 0x1D4 */ u8 m_wtr_mode;
}; // Size: 0x1D8

class dBgS_LinkAcch : public dBgS_Acch {
public:
    dBgS_LinkAcch() { SetLink(); }

    virtual ~dBgS_LinkAcch() {}
};

class dBgS_ObjAcch : public dBgS_Acch {
public:
    dBgS_ObjAcch() {
        SetObj();
    }

    virtual ~dBgS_ObjAcch() {}
}; // Size: 0x1D8

STATIC_ASSERT(sizeof(dBgS_ObjAcch) == 0x1D8);

class dBgS_StatueAcch : public dBgS_Acch {
public:
    dBgS_StatueAcch() {
        SetStatue();
    }

    virtual ~dBgS_StatueAcch() {}
};

class dBgS_BombAcch : public dBgS_Acch {
public:
    dBgS_BombAcch() {
        SetBomb();
    }

    virtual ~dBgS_BombAcch() {}
};

class dBgS_HorseAcch : public dBgS_Acch {
public:
    dBgS_HorseAcch() {
        SetHorse();
    }

    virtual ~dBgS_HorseAcch() {}
};

#endif /* D_BG_D_BG_S_ACCH_H */
