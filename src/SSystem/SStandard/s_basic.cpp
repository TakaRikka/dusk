#include "SSystem/SStandard/s_basic.h"

void sBs_FillArea_s(void* pPtr, u32 pNumBytes, s16 pValue) {
    s16* castPtr = (s16*)pPtr;
    for (int i = 0; i < pNumBytes / 2; i++) {
        *castPtr = pValue;
        castPtr++;
    }
}

void sBs_ClearArea(void* pPtr, u32 pNumBytes) {
    sBs_FillArea_s(pPtr, pNumBytes, 0);
}
