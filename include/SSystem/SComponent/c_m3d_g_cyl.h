#ifndef C_M3D_G_CYL_H
#define C_M3D_G_CYL_H

#include "SSystem/SComponent/c_m3d.h"
#include "SSystem/SComponent/c_xyz.h"
#include "global.h"

// Cylinder
struct cM3dGCylS {
    /* 0x00 */ Vec mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 mHeight;
};  // Size = 0x14
class cM3dGSph;

class cM3dGCyl {
public:
    /* 0x00 */ cXyz mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 mHeight;
    /* 0x14 vtable */
    cM3dGCyl() {}
    cM3dGCyl(const cXyz*, f32, f32);
    virtual ~cM3dGCyl() {}
    void Set(const cM3dGCylS&);
    void Set(const cXyz&, f32, f32);
    void SetC(const cXyz& pos);
    void SetH(f32 h);
    void SetR(f32 r);
    bool cross(const cM3dGSph*, cXyz*) const;
    bool cross(const cM3dGCyl*, cXyz*) const;
    void calcMinMax(cXyz*, cXyz*);
    const cXyz* GetCP() const { return &mCenter; }
    cXyz* GetCP() { return &mCenter; }
    f32 GetR() const { return mRadius; }
    f32* GetRP() { return &mRadius; }
    f32 GetH() const { return mHeight; }
    cXyz& GetC() { return mCenter; }
    const cXyz& GetC() const { return mCenter; }

    bool Cross(const cM3dGCps* cps, cXyz* xyz) const { return cM3d_Cross_CpsCyl(*cps, *this, xyz); }
    bool Cross(const cM3dGTri& tri, cXyz* xyz) const { return cM3d_Cross_CylTri(this, &tri, xyz); }
    bool cross(const cM3dGCyl* other, f32* f) const { return cM3d_Cross_CylCyl(this, other, f); }
    bool cross(const cM3dGSph* sph, f32* f) const { return cM3d_Cross_CylSph(this, sph, f); }
};  // Size = 0x18

STATIC_ASSERT(0x18 == sizeof(cM3dGCyl));

#endif /* C_M3D_G_CYL_H */
