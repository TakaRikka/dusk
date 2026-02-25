#include <stdint.h>
#include <dolphin/mtx.h>
#include "JSystem/J3DGraphBase/J3DTransform.h"

// translated to C, should be correct, but not tested.

void J3DPSMtxArrayConcat(Mtx mA, Mtx mB, Mtx mAB, u32 count) {
    for (uint32_t i = 0; i < count; i++) {
        const float* b = (const float*)mB[i];
        float* res = (float*)mAB[i];

        for (int row = 0; row < 3; row++) {
            float a0 = mA[row][0];
            float a1 = mA[row][1];
            float a2 = mA[row][2];
            float a3 = mA[row][3];

            // Standard Matrix Multiply for 3x4 * 3x4 (with implicit 4th row [0,0,0,1])
            res[row * 4 + 0] = a0 * b[0] + a1 * b[4] + a2 * b[8];
            res[row * 4 + 1] = a0 * b[1] + a1 * b[5] + a2 * b[9];
            res[row * 4 + 2] = a0 * b[2] + a1 * b[6] + a2 * b[10];
            // The 4th column includes the translation
            res[row * 4 + 3] = a0 * b[3] + a1 * b[7] + a2 * b[11] + a3;
        }
    }
}
