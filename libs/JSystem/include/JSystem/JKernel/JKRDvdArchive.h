#ifndef JKRDVDARCHIVE_H
#define JKRDVDARCHIVE_H

#include "JSystem/JKernel/JKRArchive.h"

class JKRDvdFile;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRDvdArchive : public JKRArchive {
public:
    JKRDvdArchive(s32, JKRArchive::EMountDirection);
    virtual ~JKRDvdArchive();

    bool open(s32);

    /* vt[15] */ virtual u32 getExpandedResSize(const void*) const;            /* override */
    /* vt[16] */ virtual void* fetchResource(SDIFileEntry*, u32*);             /* override */
    /* vt[17] */ virtual void* fetchResource(void*, u32, SDIFileEntry*, u32*); /* override */

public:
    static u32 fetchResource_subroutine(s32, u32, u32, u8*, u32, JKRCompression, JKRCompression);
    static u32 fetchResource_subroutine(s32, u32, u32, JKRHeap*, JKRCompression, JKRCompression,
                                        u8**);

private:
    /* 0x00 */  // vtable
    /* 0x04 */  // JKRArchive
    /* 0x64 */ s32 mDataOffset;
    /* 0x68 */ JKRDvdFile* mDvdFile;
};

#endif /* JKRDVDARCHIVE_H */
