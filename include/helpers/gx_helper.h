#pragma once

#include <cstring>

#include <dolphin/gx/GXAurora.h>
#include <dolphin/gx/GXExtra.h>

#if defined(DUSK_BUILDING_GAME)
#include <tracy/Tracy.hpp>
#else
#ifndef ZoneScopedN
#define ZoneScopedN(name)
#endif
#endif

#if DUSK_GFX_DEBUG_GROUPS
#define GX_DEBUG_GROUP(name, ...) \
    do {                          \
        GXPushDebugGroup(#name);  \
        name(__VA_ARGS__);        \
        GXPopDebugGroup();        \
    } while (0)
#else
#define GX_DEBUG_GROUP(name, ...) name(__VA_ARGS__)
#endif

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

class GXTlutObjRAII : public GXTlutObj {
public:
    GXTlutObjRAII() : GXTlutObj() {}
    ~GXTlutObjRAII() { GXDestroyTlutObj(this); }

    void reset() { GXDestroyTlutObj(this); }

    GXTlutObjRAII(const GXTlutObjRAII&) = delete;
    GXTlutObjRAII& operator=(const GXTlutObjRAII&) = delete;
    GXTlutObjRAII(GXTlutObjRAII&&) = delete;
    GXTlutObjRAII& operator=(GXTlutObjRAII&&) = delete;
};
static_assert(sizeof(GXTlutObjRAII) == sizeof(GXTlutObj),
              "GXTlutObjRAII should have the same size as GXTlutObj");
typedef GXTlutObjRAII TGXTlutObj;
#else
typedef GXTexObj TGXTexObj;
typedef GXTlutObj TGXTlutObj;
#endif

struct GXScopedDebugGroup {
    explicit GXScopedDebugGroup(const char* text) {
#if DUSK_GFX_DEBUG_GROUPS
        GXPushDebugGroup(text);
#endif
    }
    ~GXScopedDebugGroup() {
#if DUSK_GFX_DEBUG_GROUPS
        GXPopDebugGroup();
#endif
    }
};

#define GX_AND_TRACY_SCOPED(name) GXScopedDebugGroup scope(name); ZoneScopedN(name);
