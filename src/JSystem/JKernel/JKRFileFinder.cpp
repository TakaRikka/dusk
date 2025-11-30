#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRFileFinder.h"
#include "JSystem/JKernel/JKRArchive.h"

JKRArcFinder::JKRArcFinder(JKRArchive* archive, s32 startIndex, s32 numEntries) : JKRFileFinder() {
    mArchive = archive;
    mIsAvailable = numEntries > 0;
    mStartIndex = startIndex;
    mEndIndex = startIndex + numEntries - 1;
    mNextIndex = mStartIndex;
    findNextFile();
}

bool JKRArcFinder::findNextFile(void) {
    JKRArchive::SDirEntry entry;

    if (mIsAvailable) {
        mIsAvailable = !(mNextIndex > mEndIndex);
        if (mIsAvailable) {
            mIsAvailable = mArchive->getDirEntry(&entry, mNextIndex);
            mEntryName = entry.name;
            mEntryFileIndex = mNextIndex;
            mEntryId = entry.id;
            mEntryTypeFlags = entry.flags;
            mIsFileOrDirectory = (mEntryTypeFlags >> 1) & 1;
            mNextIndex++;
        }
    }
    return mIsAvailable;
}

JKRDvdFinder::JKRDvdFinder(const char* directory) : JKRFileFinder() {
    mDvdIsOpen = DVDOpenDir(directory, &mDvdDirectory);
    mIsAvailable = mDvdIsOpen;
    findNextFile();
}

JKRDvdFinder::~JKRDvdFinder() {
    if (mDvdIsOpen) {
        DVDCloseDir(&mDvdDirectory);
    }
}

bool JKRDvdFinder::findNextFile(void) {
    if (mIsAvailable) {
        DVDDirEntry directoryEntry;
        mIsAvailable = DVDReadDir(&mDvdDirectory, &directoryEntry);

        if (mIsAvailable) {
            mIsFileOrDirectory = directoryEntry.isDir != 0;
            mEntryName = directoryEntry.name;
            mEntryFileIndex = directoryEntry.entryNum;
            mEntryId = 0;

            // only matches with enum
            // TODO: placeholder
            enum EntryTypeFlags {
                EntryTypeFlags1 = 1,
                EntryTypeFlags2 = 2,
            };
            mEntryTypeFlags = mIsFileOrDirectory ? EntryTypeFlags2 : EntryTypeFlags1;
        }
    }

    return mIsAvailable;
}
