#ifndef JPADRAWINFO_H
#define JPADRAWINFO_H

#include <dolphin/mtx.h>

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPADrawInfo {
public:
    JPADrawInfo(const Mtx param_0, f32 fovY, f32 aspect) {
        MTXCopy(param_0, mCamMtx);
        C_MTXLightPerspective(mPrjMtx, fovY, aspect, 0.5f, -0.5f, 0.5f, 0.5f);
    }

    JPADrawInfo(const Mtx param_0, f32 top, f32 bottom, f32 left, f32 right) {
        MTXCopy(param_0, mCamMtx);
        C_MTXLightOrtho(mPrjMtx, top, bottom, left, right, 0.5f, 0.5f, 0.5f, 0.5f);
    }

    Mtx mCamMtx;
    Mtx mPrjMtx;

    void getCamMtx(Mtx dst) const { MTXCopy(mCamMtx, dst); }
    void getPrjMtx(Mtx dst) const { MTXCopy(mPrjMtx, dst); }

    void setPrjMtx(const Mtx src) { MTXCopy(src, mPrjMtx); }
};

#endif
