#ifndef C_M3D_G_TRI_H_
#define C_M3D_G_TRI_H_

#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "dusk/endian.h"

class cM3dGCyl;

struct cM3dGTriS {
    Vec a;
    Vec b;
    Vec c;
};

class cM3dGTri : public cM3dGPla {
    // private:
public:
    // cM3dGPla mPlane;
    Vec mA;
    Vec mB;
    Vec mC;

    cM3dGTri() {}
    virtual ~cM3dGTri() {}
    bool cross(const cM3dGCyl*, Vec*) const;
    void setPos(const Vec*, const Vec*, const Vec*);
    void setBg(const Vec*, const Vec*, const Vec*, const cM3dGPla*);
    void set(const Vec*, const Vec*, const Vec*, const Vec*);
    bool Cross(cM3dGCps const& cps, cXyz* xyz) const { return cM3d_Cross_CpsTri(cps, *this, xyz); }
    bool Cross(cM3dGCyl const& cyl, cXyz* xyz) const { return this->cross(&cyl, xyz); }
    bool Cross(cM3dGSph const& sph, cXyz* xyz) const { return cM3d_Cross_SphTri(&sph, this, xyz); }
    bool Cross(cM3dGTri const& other, cXyz* xyz) const {
        return cM3d_Cross_TriTri(*this, other, xyz);
    }
    bool cross(const cM3dGLin* lin, Vec* xyz, bool param_2, bool param_3) const {
        return cM3d_Cross_LinTri(lin, this, xyz, param_2, param_3);
    }

    void setUp() {
        SetupFrom3Vtx(&mA, &mB, &mC);
    }
};

#endif
