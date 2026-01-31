#ifndef C_M3C_G_PLA_H_
#define C_M3C_G_PLA_H_

#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_m3d.h"

// Plane with a normal
class cM3dGPla {
public:
    /* 0x00 */ cXyz mNormal;
    /* 0x0C */ f32 mD;
    /* 0x10   vtable */

    cM3dGPla() {}
    cM3dGPla(const cXyz*, f32);
    bool crossInfLin(const cXyz&, const cXyz&, cXyz&) const;
    void SetupNP0(const Vec&, const Vec&);
    void SetupNP(const Vec&, const Vec&);
    bool getCrossY(const cXyz&, f32*) const;
    bool getCrossYLessD(const Vec&, f32*) const;
    void Set(const cM3dGPla*);

    virtual ~cM3dGPla() {}

    f32 getPlaneFunc(const Vec* pPoint) const { return mD + PSVECDotProduct(&mNormal, pPoint); }
    cXyz* GetNP() { return &mNormal; }
    const cXyz* GetNP() const { return &mNormal; }
    f32 GetD() const { return mD; }

    void SetupFrom3Vtx(const Vec* v1, const Vec* v2, const Vec* v3) {
        cM3d_CalcPla(v1, v2, v3, &mNormal, &mD);
    }

    f32 getCrossY_NonIsZero(const cXyz *param_1) {
        return ((-mNormal.x * param_1->x - mNormal.z * param_1->z) - mD) / mNormal.y;
    }

    bool cross(cM3dGLin const& line, Vec& point) {
        return cM3d_Cross_LinPla(&line, this, &point, true, true);
    }

    f32 getSignedLenPos(const cXyz* param_0) const {
        return cM3d_SignedLenPlaAndPos(this, param_0);
    }
};

#endif
