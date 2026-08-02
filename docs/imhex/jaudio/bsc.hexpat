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

struct JAUSeqCollectionTable {
    u32 mEntryCount;
    u32 mBmsOffset[mEntryCount];
};

struct JAUSeqCollectionData {
    type::Magic<"SC"> mMagic;
    u16 mNumSoundCategories;
    u32 mSectionSize;
    Offset<JAUSeqCollectionTable> mTableOffsets[mNumSoundCategories];
};

JAUSeqCollectionData data @ 0x0;
