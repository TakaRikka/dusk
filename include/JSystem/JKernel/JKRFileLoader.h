#ifndef JKRFILELOADER_H
#define JKRFILELOADER_H

#include "JSystem/JKernel/JKRDisposer.h"

class JKRFileFinder;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRFileLoader : public JKRDisposer {
public:
    JKRFileLoader(void);
    virtual ~JKRFileLoader();

    bool isMounted() { return this->mIsMounted; }
    u32 getVolumeType() { return this->mVolumeType; }

public:
    /* vt[03] */ virtual void unmount(void);
    /* vt[04] */ virtual bool becomeCurrent(const char*) = 0;
    /* vt[05] */ virtual void* getResource(const char*) = 0;
    /* vt[06] */ virtual void* getResource(u32, const char*) = 0;
    /* vt[07] */ virtual u32 readResource(void*, u32, const char*) = 0;
    /* vt[08] */ virtual u32 readResource(void*, u32, u32, const char*) = 0;
    /* vt[09] */ virtual void removeResourceAll(void) = 0;
    /* vt[10] */ virtual bool removeResource(void*) = 0;
    /* vt[11] */ virtual bool detachResource(void*) = 0;
    /* vt[12] */ virtual u32 getResSize(const void*) const = 0;
    /* vt[13] */ virtual u32 countFile(const char*) const = 0;
    /* vt[14] */ virtual JKRFileFinder* getFirstFile(const char*) const = 0;

protected:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRDisposer
    /* 0x18 */ JSULink<JKRFileLoader> mFileLoaderLink;
    /* 0x28 */ const char* mVolumeName;
    /* 0x2C */ u32 mVolumeType;
    /* 0x30 */ bool mIsMounted;
    /* 0x31 */ u8 field_0x31[3];
    /* 0x34 */ u32 mMountCount;

public:
    static void* getGlbResource(const char*);
    static void* getGlbResource(const char*, JKRFileLoader*);
    static bool removeResource(void*, JKRFileLoader*);
    static bool detachResource(void*, JKRFileLoader*);
    static JKRFileLoader* findVolume(const char**);
    static const char* fetchVolumeName(char*, s32, const char*);

    static JKRFileLoader* getCurrentVolume() { return sCurrentVolume; }
    static void setCurrentVolume(JKRFileLoader* fileLoader) { sCurrentVolume = fileLoader; }
    static JSUList<JKRFileLoader>& getVolumeList() { return sVolumeList; }

    static JKRFileLoader* sCurrentVolume;
    static JSUList<JKRFileLoader> sVolumeList;
};

inline bool JKRDetachResource(void* resource, JKRFileLoader* fileLoader) {
    return JKRFileLoader::detachResource(resource, fileLoader);
}

inline void* JKRGetNameResource(const char* name, JKRFileLoader* loader) {
    return JKRFileLoader::getGlbResource(name, loader);
}

inline void* JKRGetResource(const char* name) {
    return JKRFileLoader::getGlbResource(name);
}

#endif /* JKRFILELOADER_H */
