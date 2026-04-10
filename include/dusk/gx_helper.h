#ifndef DUSK_GX_HELPER_H
#define DUSK_GX_HELPER_H

#include <cstring>

#include <dolphin/gx/GXAurora.h>
#include <dolphin/gx/GXExtra.h>

#define GX_DEBUG_GROUP(name, ...) \
    do {                          \
        GXPushDebugGroup(#name);  \
        name(__VA_ARGS__);        \
        GXPopDebugGroup();        \
    } while (0)

#ifdef TARGET_PC
class GXTexObjRAII : public GXTexObj {
public:
    GXTexObjRAII() : GXTexObj() {}
    ~GXTexObjRAII() { GXDestroyTexObj(this); }

    void reset() { GXDestroyTexObj(this); }

    GXTexObjRAII(const GXTexObjRAII&) = delete;
    GXTexObjRAII& operator=(const GXTexObjRAII&) = delete;
    GXTexObjRAII(GXTexObjRAII&& o) = delete;/*noexcept : GXTexObj(o) {
        std::memset(static_cast<GXTexObj*>(&o), 0, sizeof(GXTexObj));
    }*/
    GXTexObjRAII& operator=(GXTexObjRAII&& o) = delete;/*noexcept {
        if (this != &o) {
            GXDestroyTexObj(this);
            std::memcpy(static_cast<GXTexObj*>(this), &o, sizeof(GXTexObj));
            std::memset(static_cast<GXTexObj*>(&o), 0, sizeof(GXTexObj));
        }
        return *this;
    }*/
};
static_assert(sizeof(GXTexObjRAII) == sizeof(GXTexObj),
              "GXTexObjRAII should have the same size as GXTexObj");
typedef GXTexObjRAII TGXTexObj;
#else
typedef GXTexObj TGXTexObj;
#endif

struct GXScopedDebugGroup {
    explicit GXScopedDebugGroup(const char* text) {
        GXPushDebugGroup(text);
    }
    ~GXScopedDebugGroup() {
        GXPopDebugGroup();
    }
};

#endif  // DUSK_GX_HELPER_H
