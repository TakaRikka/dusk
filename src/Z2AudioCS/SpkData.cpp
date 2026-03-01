#include "Z2AudioCS/SpkData.h"

#include "JSystem/JUtility/JUTAssert.h"

SpkData::SpkData(JKRArchive* resArc) {
    mResArc = resArc;
    JUT_ASSERT(21, resArc);
}

void SpkData::loadTable(u16 chan) {
    JUT_ASSERT(32, mResArc);

    void* res = mResArc->getResource(chan);
    JUT_ASSERT(35, res);

    mTableMgr.setResource(res);
}

void SpkData::loadWave(u16 chan) {
    JUT_ASSERT(48, mResArc);

    void* res = mResArc->getResource(chan);
    JUT_ASSERT(51, res);

    mWaveMgr.setResource(res);
}

BOOL SpkData::isValid(void) const {
    return mTableMgr.isValid() && mWaveMgr.isValid();
}

static void dummy(SpkWave* wave, SpkTable* table) {
    if (wave->isValid() && table->isValid()) {
        table->getParams(0);
        table->getNumOfSound();
    }
}
