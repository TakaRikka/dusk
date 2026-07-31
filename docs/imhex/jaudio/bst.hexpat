#pragma endian big

#include "std/sys.pat"
#include "type/magic.pat"
#include "std/array.pat"

/**
 * Wrapper for a file-global pointer that allows it to be stored in an array.
 */ 
struct Offset<T> {
    T* offset : u32 [[inline]];
};

struct ItemSoundEffect {
    u8 mPriority;
    u8 mVolume;
    padding[2];
    u32 mSwBit;
    float mPitch;
};

struct ItemSequence {
    u8 mPriority;
    u8 mVolume;
    u16 mResourceId;
};

struct ItemStream {
    u8 mPriority;
    u8 mVolume;
    u16 mStreamPanParameters;
    char* mStreamFilePath[] : u32;
};

struct SoundTableGroupEntry {
    u8 mTypeId;
    if (mTypeId == 0x51) {
        ItemSoundEffect* mOffset : u24 [[inline]];
    } else if (mTypeId == 0x60) {
        ItemSequence* mOffset : u24 [[inline]];
    } else if (mTypeId == 0x70 || mTypeId == 0x71) {
        ItemStream* mOffset : u24 [[inline]];
    } else {
        u24 mOffset;
    }
};

struct TGroup {
    u32 mNumItems;
    padding[4];
    SoundTableGroupEntry mEntries[mNumItems];
};

struct TSection {
    u32 mNumGroups;
    Offset<TGroup> mGroupOffsets[mNumGroups];
};

struct Root {
    u32 mSectionNumber;
    Offset<TSection> mSectionOffsets[mSectionNumber];
};

struct THeader {
    type::Magic<"BST "> mMagic;

    padding[8];
    Root* mRoot : u32;
};

THeader header @ 0x0;