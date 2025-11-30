#ifndef D_BG_D_BG_S_ROOF_CHK_H
#define D_BG_D_BG_S_ROOF_CHK_H

#include "SSystem/SComponent/c_bg_s_poly_info.h"
#include "SSystem/SComponent/c_xyz.h"
#include "d/d_bg_s_chk.h"

class dBgS_RoofChk : public cBgS_PolyInfo, public cBgS_Chk, public dBgS_Chk {
public:
    dBgS_RoofChk();
    void Init();

    virtual ~dBgS_RoofChk();

    void SetNowY(f32 y) { mNowY = y; }
    void SetPos(cXyz const& pos) { m_pos = pos; }
    f32 GetNowY() { return mNowY; }
    cXyz* GetPosP() { return &m_pos; }

private:
    /* 0x3C */ cXyz m_pos;
    /* 0x48 */ int field_0x48;
    /* 0x4C */ f32 mNowY;
};  // Size: 0x50

class dBgS_LinkRoofChk : public dBgS_RoofChk {
public:
    dBgS_LinkRoofChk() { SetLink(); }
    virtual ~dBgS_LinkRoofChk() {}
};

class dBgS_ObjRoofChk : public dBgS_RoofChk {
public:
    dBgS_ObjRoofChk() { SetObj(); }

    virtual ~dBgS_ObjRoofChk() {}
};

#endif /* D_BG_D_BG_S_ROOF_CHK_H */
