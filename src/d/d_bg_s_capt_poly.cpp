#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s_capt_poly.h"

#include "JSystem/JUtility/JUTAssert.h"

dBgS_CaptPoly::dBgS_CaptPoly() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());
    field_0x48 = 0;
}

void dBgS_CaptPoly::Set(cXyz& min, cXyz& max) {
    aab.Set(&min, &max);

    JUT_ASSERT(36, !(fpclassify(min.x) == FP_QNAN));
    JUT_ASSERT(37, !(fpclassify(min.y) == FP_QNAN));
    JUT_ASSERT(38, !(fpclassify(min.z) == FP_QNAN));
    JUT_ASSERT(39, !(fpclassify(max.x) == FP_QNAN));
    JUT_ASSERT(40, !(fpclassify(max.y) == FP_QNAN));
    // BUG: Assertion on wrong cXyz member (should be max.z)
    JUT_ASSERT(41, !(fpclassify(max.y) == FP_QNAN));

    JUT_ASSERT(45, -INF < min.x && min.x < INF);
    JUT_ASSERT(46, -INF < min.y && min.y < INF);
    JUT_ASSERT(47, -INF < min.z && min.z < INF);
    JUT_ASSERT(48, -INF < max.x && max.x < INF);
    JUT_ASSERT(49, -INF < max.y && max.y < INF);
    JUT_ASSERT(50, -INF < max.z && max.z < INF);
}
