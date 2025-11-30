#ifndef M_DO_M_DO_LIB_H
#define M_DO_M_DO_LIB_H

#include "JSystem/J3DU/J3DUClipper.h"

typedef struct _GXTexObj GXTexObj;
typedef struct _GXTlutObj GXTlutObj;
typedef struct Vec Vec;
struct ResTIMG;

struct mDoLib_clipper {
    static void setup(f32, f32, f32, f32);

    static void changeFar(f32 far) {
        mClipper.setFar(far);
        mClipper.calcViewFrustum();
    }

    static int clip(const Mtx m, const Vec* param_1, const Vec* param_2) {
        return mClipper.clip(m, (Vec*)param_1, (Vec*)param_2);
    }

    static s32 clip(const Mtx m, Vec param_1, f32 param_2) {
        return mClipper.clip(m, param_1, param_2);
    }

    static f32 getFar() { return mSystemFar; }
    static f32 getFovyRate() { return mFovyRate; }
    
    static void resetFar() {
        mClipper.setFar(mSystemFar);
        mClipper.calcViewFrustum();
    }

    static J3DUClipper mClipper;
    static f32 mSystemFar;
    static f32 mFovyRate;
};

void mDoLib_project(Vec* src, Vec* dst);
u32 mDoLib_setResTimgObj(ResTIMG const* res, GXTexObj* o_texObj, u32 tlut_name,
                          GXTlutObj* o_tlutObj);
void mDoLib_pos2camera(Vec* src, Vec* dst);

#endif /* M_DO_M_DO_LIB_H */
