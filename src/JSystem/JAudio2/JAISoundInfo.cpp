#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISoundInfo.h"

JAISoundInfo::JAISoundInfo(bool setInstance) : JASGlobalInstance<JAISoundInfo>(setInstance) {}

JAISoundInfo::~JAISoundInfo() {
    JUT_ASSERT(14, false);
}
