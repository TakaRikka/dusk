#ifndef JAISEQDATAMGR_H
#define JAISEQDATAMGR_H

#include "JSystem/JAudio2/JAISound.h"
#include <stdint.h>

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISeqData {
    JAISeqData(const void* param_0, u32 param_1) {
        field_0x0 = (void*)param_0;
        field_0x4 = param_1;
    }

    void set(const void* param_0, u32 param_1) {
        field_0x0 = (void*)param_0;
        field_0x4 = param_1;
    }

    /* 0x00 */ void* field_0x0;
    /* 0x04 */ u32 field_0x4;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISeqDataRegion {
    bool intersects(const JAISeqData& seqData) const {
        if ((uintptr_t)addr + size < (uintptr_t)seqData.field_0x0) {
            return false;
        }
        if ((uintptr_t)seqData.field_0x0 + seqData.field_0x4 < (uintptr_t)addr) {
            return false;
        }
        return true;
    }

    /* 0x00 */ u8* addr;
    /* 0x04 */ u32 size;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISeqDataUser {
    virtual ~JAISeqDataUser();
    virtual bool isUsingSeqData(JAISeqDataRegion const&) = 0;
    virtual int releaseSeqData(JAISeqDataRegion const&) = 0;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAISeqDataMgr {
    enum SeqDataReturnValue{ SeqDataReturnValue_0 = 0, SeqDataReturnValue_1 = 1, SeqDataReturnValue_2 = 2};
    virtual ~JAISeqDataMgr();
    virtual SeqDataReturnValue getSeqData(JAISoundID, JAISeqData*) = 0;
    virtual int releaseSeqData() = 0;
    virtual bool setSeqDataUser(JAISeqDataUser*) = 0;
};

#endif /* JAISEQDATAMGR_H */
