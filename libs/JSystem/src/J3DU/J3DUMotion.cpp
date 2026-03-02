#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DU/J3DUMotion.h"

#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JMath/JMath.h"

static void dummy(J3DMtxCalcJ3DSysInitSoftimage* img, Vec* vec) {
    Mtx mtx;
    img->init(*vec, mtx);
    checkScaleOne(*vec);
    J3DMtxCalc::getMtxBuffer();
    JMAFastReciprocal(0.0f);
    JMath::fastReciprocal(0.0f);
}
