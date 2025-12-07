#ifndef D_CC_D_CC_S_H
#define D_CC_D_CC_S_H

#include "SSystem/SComponent/c_cc_s.h"
#include "d/d_cc_mass_s.h"

class dCcD_GObjInf;
class dCcD_GStts;

class dCcS : public cCcS {
public:
    dCcS() {}
    void Ct();
    void Dt();
    bool ChkShieldFrontRange(cCcD_Obj*, cCcD_Obj*, int, cXyz const*);
    bool ChkShield(cCcD_Obj*, cCcD_Obj*, dCcD_GObjInf*, dCcD_GObjInf*, cXyz const*);
    void CalcTgPlusDmg(cCcD_Obj*, cCcD_Obj*, cCcD_Stts*, cCcD_Stts*);
    int GetRank(u8);
    void ProcAtTgHitmark(bool, bool, cCcD_Obj*, cCcD_Obj*, dCcD_GObjInf*,
                                        dCcD_GObjInf*, cCcD_Stts*, cCcD_Stts*, dCcD_GStts*,
                                        dCcD_GStts*, cXyz*, bool);
    bool ChkCamera(cXyz&, cXyz&, f32, fopAc_ac_c*, fopAc_ac_c*, fopAc_ac_c*);
    bool chkCameraPoint(cXyz const&, cCcD_ShapeAttr::Shape*, fopAc_ac_c*,
                                       fopAc_ac_c*);
    void DrawAfter();
    void Move();
    void Draw();
    void MassClear();
    BOOL ChkAtTgMtrlHit(u8, u8);

    virtual void SetPosCorrect(cCcD_Obj*, cXyz*, cCcD_Obj*, cXyz*, f32);
    virtual void SetCoGObjInf(bool, bool, cCcD_GObjInf*, cCcD_GObjInf*, cCcD_Stts*,
                                             cCcD_Stts*, cCcD_GStts*, cCcD_GStts*);
    virtual void SetAtTgGObjInf(bool, bool, cCcD_Obj*, cCcD_Obj*, cCcD_GObjInf*,
                                               cCcD_GObjInf*, cCcD_Stts*, cCcD_Stts*, cCcD_GStts*,
                                               cCcD_GStts*, cXyz*);
    virtual bool ChkNoHitGAtTg(cCcD_GObjInf const*, cCcD_GObjInf const*, cCcD_GStts*,
                                              cCcD_GStts*);
    virtual bool ChkAtTgHitAfterCross(bool, bool, cCcD_GObjInf const*,
                                                     cCcD_GObjInf const*, cCcD_Stts*, cCcD_Stts*,
                                                     cCcD_GStts*, cCcD_GStts*);
    virtual bool ChkNoHitGCo(cCcD_Obj*, cCcD_Obj*);
    virtual ~dCcS() {}
    virtual void MoveAfterCheck();
    virtual void CalcParticleAngle(dCcD_GObjInf*, cCcD_Stts*, cCcD_Stts*, csXyz*);

    void SetMass(cCcD_Obj* i_obj, u8 i_priority) { mMass_Mng.Set(i_obj, i_priority); }
    void SetMassCam(cM3dGCps& i_cps) { mMass_Mng.SetCam(i_cps); }
    u32 GetMassResultCam() { return mMass_Mng.GetResultCam(); }
    void GetMassCamTopPos(Vec* o_pos) { mMass_Mng.GetCamTopPos(o_pos); }
    void PrepareMass() { mMass_Mng.Prepare(); }

    u32 ChkMass(cXyz* param_0, fopAc_ac_c** param_1, dCcMassS_HitInf* param_2) {
        u32 rt = mMass_Mng.Chk(param_0, param_1, param_2);
        return rt;
    }

    void SetMassAttr(f32 radius, f32 height, u8 param_2, u8 param_3) {
        mMass_Mng.SetAttr(radius, height, param_2, param_3);
    }

    void ChkActor(fopAc_ac_c*);

    static bool m_mtrl_hit_tbl[64];

    // /* 0x0000 */ cCcS mCCcS;
    /* 0x284C */ dCcMassS_Mng mMass_Mng;
#if DEBUG
    /* 0x2AD0 */ u8 field_0x2ad0;
#endif
};  // Size = 0x2AC4

STATIC_ASSERT(sizeof(dCcS) == 0x2AC4);

#endif /* D_CC_D_CC_S_H */
