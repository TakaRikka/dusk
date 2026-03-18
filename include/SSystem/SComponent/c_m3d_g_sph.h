#ifndef C_M3D_G_SPH_H_
#define C_M3D_G_SPH_H_

#include "SSystem/SComponent/c_m3d.h"
#include "SSystem/SComponent/c_xyz.h"
#include "global.h"

class cM3dGCyl;

struct cM3dGSphS {
    /* 0x0 */ Vec mCenter;
    /* 0xC */ f32 mRadius;
};  // Size: 0x10

class cM3dGSph {
private:
public:
    /* 0x00 */ cXyz mCenter;
    /* 0x0C */ f32 mRadius;
    /* 0x10 vtable */

    cM3dGSph() {}
    virtual ~cM3dGSph() {}
    void SetC(const cXyz&);
    void Set(const cXyz&, f32);
    void Set(const cM3dGSphS&);
    void SetR(f32);
    bool cross(const cM3dGSph*, cXyz*) const;
    bool cross(const cM3dGCyl*, cXyz*) const;
    inline bool cross(const cM3dGTri *param_1) const {
        return cM3d_Cross_SphTri(this, param_1);
    }
    void GetMinMaxCube(cXyz&, cXyz&) const;
    const cXyz& GetC(void) const { return mCenter; }
    const cXyz* GetCP() const { return &mCenter; }
    cXyz* GetCP() { return &mCenter; }
    const f32 GetR(void) const { return mRadius; }
    f32 GetCX(void) const { return mCenter.x; }
    f32 GetCY(void) const { return mCenter.y; }
    f32 GetCZ(void) const { return mCenter.z; }

    bool cross(const cM3dGSph* other, f32* f) const { return cM3d_Cross_SphSph(this, other, f); }
    bool Cross(const cM3dGCps* cps, cXyz* xyz) const { return cM3d_Cross_CpsSph(*cps, *this, xyz); }
    bool cross(const cM3dGCyl* cyl, f32* f) const { return cM3d_Cross_CylSph(cyl, this, f); }
    bool Cross(const cM3dGTri& tri, cXyz* xyz) const { return cM3d_Cross_SphTri(this, &tri, xyz); }
};  // Size = 0x14

STATIC_ASSERT(0x14 == sizeof(cM3dGSph));

#endif
