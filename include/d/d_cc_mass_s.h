#ifndef D_CC_D_CC_MASS_S_H
#define D_CC_D_CC_MASS_S_H

#include "SSystem/SComponent/c_cc_d.h"

typedef void (*dCcMassS_ObjCallback)(fopAc_ac_c*, cXyz*, u32);

class dCcMassS_Obj {
private:
    /* 0x00 */ cCcD_Obj* mpObj;
    /* 0x04 */ u8 mPriority;
    /* 0x08 */ dCcMassS_ObjCallback mpCallback;
    /* 0x0C */ cCcD_DivideInfo mDivideInfo;
    /* 0x1C vtable */

public:
    dCcMassS_Obj();
    virtual ~dCcMassS_Obj() {}
    void Set(cCcD_Obj* p_obj, u8 priority, dCcMassS_ObjCallback callback);
    void Clear();

    cCcD_Obj* GetObj() { return mpObj; }
    u8 GetPriority() const { return mPriority; }
    dCcMassS_ObjCallback GetCallback() const { return mpCallback; }
    cCcD_DivideInfo& GetDivideInfo() { return mDivideInfo; }
};  // Size = 0x20

STATIC_ASSERT(0x20 == sizeof(dCcMassS_Obj));

class dCcMassS_HitInf {
private:
    /* 0x00 */ cCcD_Obj* mpArea;
    /* 0x04 */ cCcD_Obj* mpAtObj;
    /* 0x08 */ cCcD_Obj* mpCoObj;
    /* 0x0C */ f32 mCoHitLen;

public:
    virtual ~dCcMassS_HitInf() {}

    void ClearPointer();
    void SetAreaHitObj(cCcD_Obj* obj) { mpArea = obj; }
    void SetCoHitObj(cCcD_Obj* obj) { mpCoObj = obj; }
    void SetAtHitObj(cCcD_Obj* obj) { mpAtObj = obj; }
    void SetCoHitLen(f32 len) { mCoHitLen = len; }
    cCcD_Obj* GetAtHitObj() const { return mpAtObj; }
};

class dCcMassS_Mng {
public:
    /* 0x0000 */ cCcD_DivideArea mDivideArea;
    /* 0x0040 */ s32 mMassObjCount;
    /* 0x0044 */ dCcMassS_Obj mMassObjs[10];
    /* 0x0184 */ s32 mMassAreaCount;
    /* 0x0188 */ dCcMassS_Obj mMassAreas[2];
    /* 0x01C8 */ cCcD_CylAttr mCylAttr;
    /* 0x0200 */ u8 field_0x200;
    /* 0x0200 */ u8 field_0x201;
    /* 0x0202 */ u8 field_0x202;
    /* 0x0203 */ u8 mResultCam;
    /* 0x0204 */ Vec mCamTopPos;
    /* 0x0210 */ f32 mCamTopDist;
    /* 0x0214 */ Vec mCamBottomPos;
    /* 0x0220 */ f32 mCamBottomDist;
    /* 0x0224 */ cCcD_CpsAttr mCpsAttr;
    /* 0x0264 */ cCcD_DivideInfo mDivideInfo;
    /* 0x0274 vtable */

public:
    dCcMassS_Mng();
    virtual ~dCcMassS_Mng() {}
    void Ct();
    void SetAttr(f32 radius, f32 height, u8 param_2, u8 param_3);
    void Prepare();
    u32 Chk(cXyz* p_xyz, fopAc_ac_c** p_actor, dCcMassS_HitInf* p_hitInf);
    void Clear();
    void Set(cCcD_Obj* p_obj, u8 priority);
    void SetCam(cM3dGCps const& cps);
    u32 GetResultCam() const;
    void GetCamTopPos(Vec* p_out);
};

STATIC_ASSERT(sizeof(dCcMassS_Mng) == 0x278);

#endif /* D_CC_D_CC_MASS_S_H */
