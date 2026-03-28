#include "JSystem/JSystem.h"  // IWYU pragma: keep

#include <cstdarg>
#include <cstdio>
#include "JSystem/JAHostIO/JAHUpdate.h"
#include "JSystem/JAHostIO/JAHioUtil.h"
#include "JSystem/JHostIO/JORFile.h"

char JAHioUtil::mStringBuffer[256];

JAHioNode* JAHUpdate::spNode;
JORMContext* JAHUpdate::spMc;

static char* dummy(JORDir* dir) {
    return std::strrchr(dir->getFilename(), '\n');
}

char* JAHioUtil::getString(const char* msg, ...) {
    va_list args;
    va_start(args, msg);
    vsprintf(mStringBuffer, msg, args);
    va_end(args);
    return mStringBuffer;
}
