#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DU/J3DUShadow.h"

#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTTexture.h"

void J3DUShadow_dummy(JUTTexture* tex, JGeometry::TVec3<f32>* vec) {
    JMAAbs(0.0f);
    JMathInlineVEC::C_VECDotProduct(NULL, NULL);
    vec->dot(*vec);
    vec->length();

    tex->getCaptureFlag();
    tex->getFormat();
    JMath::fastSqrt<f32>(0.0f);
    JUTTexture tex2;
    tex2.setCaptureFlag(false);
}
