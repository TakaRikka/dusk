#ifndef JKRFILEFINDER_H
#define JKRFILEFINDER_H

#include <dolphin/dvd.h>

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRFileFinder {
public:
    JKRFileFinder() {
        mIsAvailable = false;
        mIsFileOrDirectory = false;
    }

    bool isAvailable() const { return mIsAvailable; }
    bool isFile() const { return mIsFileOrDirectory; }
    bool isDirectory() const { return mIsFileOrDirectory; }

protected:
    /* 0x00 */ const char* mEntryName;
    /* 0x04 */ s32 mEntryFileIndex;
    /* 0x08 */ u16 mEntryId;
    /* 0x0A */ u16 mEntryTypeFlags;
    /* 0x0C */  // vtable
public:
    /* vt[2] */ virtual ~JKRFileFinder() {};
    /* vt[3] */ virtual bool findNextFile(void) = 0;
protected:
    /* 0x10 */ bool mIsAvailable;
    /* 0x11 */ bool mIsFileOrDirectory;
    /* 0x12 */ u8 field_0x12[2];
};

class JKRArchive;

/**
 * @ingroup jsystem-jkernel
 * 
 */
class JKRArcFinder : public JKRFileFinder {
public:
    JKRArcFinder(JKRArchive*, s32, s32);
    inline virtual ~JKRArcFinder() {}

public:
    /* vt[3] */ virtual bool findNextFile(void); /* override */

private:
    /* 0x00 */  // JKRFileFinder_UnknownBase
    /* 0x0C */  // vtable
    /* 0x10 */  // JKRFileFinder
    /* 0x14 */ JKRArchive* mArchive;
    /* 0x18 */ s32 mStartIndex;
    /* 0x1C */ s32 mEndIndex;
    /* 0x20 */ s32 mNextIndex;
};

class JKRDvdFinder : public JKRFileFinder {
public:
    JKRDvdFinder(const char*);
    virtual ~JKRDvdFinder();

public:
    /* vt[3] */ virtual bool findNextFile(void); /* override */

private:
    /* 0x00 */  // JKRFileFinder_UnknownBase
    /* 0x0C */  // vtable
    /* 0x10 */  // JKRFileFinder
    /* 0x14 */ DVDDir mDvdDirectory;
    /* 0x20 */ bool mDvdIsOpen;
    /* 0x21 */ u8 field_0x21[3];
};

#endif /* JKRFILEFINDER_H */
