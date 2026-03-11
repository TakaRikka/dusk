#ifndef DUSK_GX_HELPER_H
#define DUSK_GX_HELPER_H

#include <dolphin/gx/GXAurora.h>

#define GX_DEBUG_GROUP(name, ...) \
    do {                          \
        GXPushDebugGroup(#name);  \
        name(__VA_ARGS__);        \
        GXPopDebugGroup();        \
    } while (0)

#endif  // DUSK_GX_HELPER_H
