#ifndef Z2AUDIOCS_SPKDATA_H
#define Z2AUDIOCS_SPKDATA_H

#include "Z2AudioCS/SpkTable.h"
#include "Z2AudioCS/SpkWave.h"
#include "JSystem/JKernel/JKRArchive.h"

class SpkData {
public:
    SpkData(JKRArchive*);
    void loadTable(u16 chan);
    void loadWave(u16 chan);
    BOOL isValid(void) const;

    inline SpkTable& getTableMgr(void) { return mTableMgr; }
    inline SpkWave& getWaveMgr(void) { return mWaveMgr; }

    /* 0x00 */ SpkTable mTableMgr;
    /* 0x10 */ SpkWave mWaveMgr;
    /* 0x14 */ JKRArchive* mResArc;
};

#endif /* Z2AUDIOCS_SPKDATA_H */
