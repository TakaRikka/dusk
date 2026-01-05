#ifndef JAUSEQCOLLECTION_H
#define JAUSEQCOLLECTION_H

#include "JSystem/JAudio2/JAISeqDataMgr.h"

class JAISeqData;
class JAISeqDataRegion;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUSeqCollectionData {
    s8 field_0x0;
    s8 field_0x1;
    u16 field_0x2;
    u32 field_0x4;
    u32 field_0x8;
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

    bool isValid() const { return field_0x8; }

    /* 0x00 */ u16 field_0x0;
    /* 0x04 */ const u32* field_0x4;
    /* 0x08 */ const JAUSeqCollectionData* field_0x8;
    /* 0x0C */ int field_0xc;
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

    const void* getResource() const { return field_0x4; }
    void init(const void* param_1) { JAUSeqCollection::init(param_1); }

    /* 0x14 */ JAISeqDataUser* user_;
};

#endif /* JAUSEQCOLLECTION_H */
