#ifndef JAUSEQCOLLECTION_H
#define JAUSEQCOLLECTION_H

#include "JSystem/JAudio2/JAISeqDataMgr.h"

struct JAISeqData;
struct JAISeqDataRegion;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSeqCollectionData {
    s8 mMagic1; // 'S'
    s8 mMagic2; // 'C'
    BE(u16) mNumSoundCategories;
    BE(u32) mSectionSize;
    BE(u32) mTableOffsets[0]; // VLA
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSeqCollection {
public:
    JAUSeqCollection();
    void init(void const*);
    bool getSeqData(int, int, JAISeqData*);
    bool getSeqDataRegion(JAISeqDataRegion*);

    bool isValid() const { return mHeader; }

    /* 0x00 */ u16 mNumSoundCategories;
    /* 0x04 */ const BE(u32)* mTableOffsets;
    /* 0x08 */ const JAUSeqCollectionData* mHeader;
    /* 0x0C */ u32 mSectionSize;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JAUSeqDataMgr_SeqCollection : public JAISeqDataMgr, public JAUSeqCollection {
public:
    JAUSeqDataMgr_SeqCollection();
    bool setSeqDataUser(JAISeqDataUser*);
    int releaseSeqData();
    SeqDataReturnValue getSeqData(JAISoundID, JAISeqData*);
    ~JAUSeqDataMgr_SeqCollection();

    const void* getResource() const { return mTableOffsets; }
    void init(const void* param_1) { JAUSeqCollection::init(param_1); }

    /* 0x14 */ JAISeqDataUser* user_;
};

#endif /* JAUSEQCOLLECTION_H */
