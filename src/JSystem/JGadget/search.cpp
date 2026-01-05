#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/search.h"

const char* JGadget::toStringFromIndex(int index, const char* const* pValue, u32 count, const char* fallback) {
    return toValueFromIndex<const char*>(index, pValue, count, fallback);
}

int JGadget::toIndexFromString_linear(const char* sz, const char* const* pValue, u32 count, int fallback) {
    JUT_ASSERT(29, sz!=NULL);
    return toIndexFromValue_linear_if<const char*, search::TPR1IsEqual_string_>(
        search::TPR1IsEqual_string_(sz), pValue, count, fallback);
}
