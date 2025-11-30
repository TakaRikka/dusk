#ifndef J3DUCLIPPER_H
#define J3DUCLIPPER_H

#include "dolphin/mtx.h"

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DUClipper {
public:
    J3DUClipper() { init(); }
    virtual ~J3DUClipper() {}
    void init();
    void calcViewFrustum();
    int clip(f32 const (*)[4], Vec, f32) const;
    int clip(f32 const (*)[4], Vec*, Vec*) const;

    void setFovy(f32 fovy) { mFovY = fovy; }
    void setAspect(f32 aspect) { mAspect = aspect; }
    void setNear(f32 near) { mNear = near; }
    void setFar(f32 far) { mFar = far; }

    f32 getFar() { return mFar; }

private:
    /* 0x04 */ Vec _04;
    /* 0x10 */ Vec _10;
    /* 0x1C */ Vec _1C;
    /* 0x28 */ Vec _28;
    /* 0x34 */ u8 _34[0x4C - 0x34];
    /* 0x4C */ f32 mFovY;
    /* 0x50 */ f32 mAspect;
    /* 0x54 */ f32 mNear;
    /* 0x58 */ f32 mFar;
};

#endif /* J3DUCLIPPER_H */
