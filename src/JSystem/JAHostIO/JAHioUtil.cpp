#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAHostIO/JAHioUtil.h"
#include <stdio>

char JAHioUtil::mStringBuffer[256];

char* JAHioUtil::getString(const char* msg, ...) {
    va_list args;
    va_start(msg, args);
    vsprintf(mStringBuffer, msg, args);
    va_end(args);
    return mStringBuffer;
}
