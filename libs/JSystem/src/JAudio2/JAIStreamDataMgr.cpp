#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAIStreamDataMgr.h"

JAIStreamDataMgr::~JAIStreamDataMgr() {
    #if !TARGET_PC
    JUT_ASSERT(11, false);
    #endif
}

JAIStreamAramMgr::~JAIStreamAramMgr() {
    #if !TARGET_PC
    JUT_ASSERT(16, false);
    #endif
}
