#ifndef C_M3D_G_CIR_H
#define C_M3D_G_CIR_H

#include <dolphin/types.h>

class cM2dGCir {
public:
    f32 mPosX;
    f32 mPosY;
    f32 mRadius;
    
    f32 GetCx() const { return mPosX; }
    f32 GetCy() const { return mPosY; }
    f32 GetR() const { return mRadius; }

    void Set(f32 pos_x, f32 pos_y, f32 radius) {
        mPosX = pos_x;
        mPosY = pos_y;
        mRadius = radius;
    }

    cM2dGCir() {}
    virtual ~cM2dGCir() {}
};

class cM3dGCir : public cM2dGCir {
    f32 mPosZ;

public:
    cM3dGCir(void);
    virtual ~cM3dGCir(void);
    void Set(f32, f32, f32, f32);
};

#endif /* C_M3D_G_CIR_H */
