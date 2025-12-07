#ifndef C_CC_S_H
#define C_CC_S_H

#include "SSystem/SComponent/c_cc_d.h"

enum WeightType {
    WeightType_0 = 0,
    WeightType_1 = 1,
    WeightType_2 = 2,
};

class cCcS {
public:
    /* 0x0000 */ cCcD_Obj* mpObjAt[0x100];
    /* 0x0400 */ cCcD_Obj* mpObjTg[0x300];
    /* 0x1000 */ cCcD_Obj* mpObjCo[0x100];
    /* 0x1400 */ cCcD_Obj* mpObj[0x500];
#if DEBUG
    /* 0x2800 */ int m_debug_code;
#endif
    /* 0x2800 */ u16 mObjAtCount;
    /* 0x2802 */ u16 mObjTgCount;
    /* 0x2804 */ u16 mObjCoCount;
    /* 0x2806 */ u16 mObjCount;
#if DEBUG
    /* 0x280C */ u16 field_0x280c;
    /* 0x280E */ u16 field_0x280e;
    /* 0x2810 */ u16 field_0x2810;
    /* 0x2812 */ u16 field_0x2812;
#endif
    /* 0x2808 */ cCcD_DivideArea mDivideArea;
    /* 0x2848 vtable */

    cCcS();
    void Ct();
    void Dt();
    WeightType GetWt(u8) const;
    void Set(cCcD_Obj*);
    void ClrCoHitInf();
    void ClrTgHitInf();
    void ClrAtHitInf();
    bool ChkNoHitAtTg(cCcD_Obj*, cCcD_Obj*);
    void ChkAtTg();
    bool ChkNoHitCo(cCcD_Obj*, cCcD_Obj*);
    void ChkCo();
    virtual void CalcTgPlusDmg(cCcD_Obj*, cCcD_Obj*, cCcD_Stts*, cCcD_Stts*);
    void SetAtTgCommonHitInf(cCcD_Obj*, cCcD_Obj*, cXyz*);
    void SetCoCommonHitInf(cCcD_Obj*, cXyz*, cCcD_Obj*, cXyz*, f32);
    /* 80265750 vt[3]*/ virtual void SetPosCorrect(cCcD_Obj*, cXyz*, cCcD_Obj*, cXyz*, f32);
    void CalcArea();
    void Move();
    void DrawClear();
    virtual void SetCoGObjInf(bool, bool, cCcD_GObjInf*, cCcD_GObjInf*,
                                                   cCcD_Stts*, cCcD_Stts*, cCcD_GStts*,
                                                   cCcD_GStts*);
    virtual void SetAtTgGObjInf(bool, bool, cCcD_Obj*, cCcD_Obj*,
                                                     cCcD_GObjInf*, cCcD_GObjInf*, cCcD_Stts*,
                                                     cCcD_Stts*, cCcD_GStts*, cCcD_GStts*, cXyz*);
    virtual bool ChkNoHitGAtTg(cCcD_GObjInf const*, cCcD_GObjInf const*,
                                                    cCcD_GStts*, cCcD_GStts*);
    virtual bool ChkAtTgHitAfterCross(bool, bool, cCcD_GObjInf const*,
                                                           cCcD_GObjInf const*, cCcD_Stts*,
                                                           cCcD_Stts*, cCcD_GStts*, cCcD_GStts*);
    virtual bool ChkNoHitGCo(cCcD_Obj*, cCcD_Obj*);
    virtual ~cCcS() {}
    virtual void MoveAfterCheck();
    virtual void SetCoGCorrectProc(cCcD_Obj*, cCcD_Obj*);
};  // Size = 0x284C

STATIC_ASSERT(sizeof(cCcS) == 0x284C);

#endif /* C_CC_S_H */
