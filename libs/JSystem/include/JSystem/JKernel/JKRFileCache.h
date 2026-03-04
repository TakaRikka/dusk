#ifndef JKRFILECACHE_H
#define JKRFILECACHE_H

#include "JSystem/JKernel/JKRFileLoader.h"

class JKRHeap;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRFileCache : public JKRFileLoader {
public:
    class CCacheBlock {
    public:
        CCacheBlock(u32, u32, const void*);
        ~CCacheBlock() {}

    public:
        /* 0x00 */ JSULink<CCacheBlock> mCacheBlockLink;
        /* 0x10 */ u32 mReferenceCount;
        /* 0x14 */ u32 mFileId;
        /* 0x18 */ u32 mFileSize;
        /* 0x1C */ void* mMemoryPtr;
    };

protected:
    JKRFileCache(const char*, const char*);
    virtual ~JKRFileCache();

    CCacheBlock* findCacheBlock(const void*) const;
    CCacheBlock* findCacheBlock(u32) const;
    bool findFile(char*, const char*) const;
    char* getDvdPathName(const char*) const;
    void convStrLower(char*) const;

public:
    /* vt[04] */ virtual bool becomeCurrent(const char*);                /* override */
    /* vt[05] */ virtual void* getResource(const char*);                 /* override */
    /* vt[06] */ virtual void* getResource(u32, const char*);            /* override */
    /* vt[07] */ virtual u32 readResource(void*, u32, const char*);      /* override */
    /* vt[08] */ virtual u32 readResource(void*, u32, u32, const char*); /* override */
    /* vt[09] */ virtual void removeResourceAll(void);                   /* override */
    /* vt[10] */ virtual bool removeResource(void*);                     /* override */
    /* vt[11] */ virtual bool detachResource(void*);                     /* override */
    /* vt[12] */ virtual u32 getResSize(const void*) const;              /* override */
    /* vt[13] */ virtual u32 countFile(const char*) const;               /* override */
    /* vt[14] */ virtual JKRFileFinder* getFirstFile(const char*) const; /* override */
    /* vt[15] */ virtual void* getFsResource(const char*);
    /* vt[16] */ virtual void* getNameResource(u32, const char*);
    /* vt[17] */ virtual u32 readFsResource(void*, u32, const char*);
    /* vt[18] */ virtual u32 readNameResource(void*, u32, u32, const char*);

private:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRFileLoader
    /* 0x38 */ JKRHeap* mParentHeap;
    /* 0x3C */ JSUList<CCacheBlock> mCacheBlockList;
    /* 0x48 */ char* mRootPath;
    /* 0x4C */ char* mCurrentPath;
    /* 0x50 */ char* mVolumePath;

public:
    static JKRFileCache* mount(const char*, JKRHeap*, const char*);
};

inline JKRFileCache* JKRMountDvdDrive(const char* path, JKRHeap* heap, const char* param_2) {
    return JKRFileCache::mount(path, heap, param_2);
}

inline void JKRUnmountDvdDrive(JKRFileCache* filecache) {
    filecache->unmount();
}

#endif /* JKRFILECACHE_H */
