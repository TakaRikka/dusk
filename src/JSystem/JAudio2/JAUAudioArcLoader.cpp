#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAUAudioArcLoader.h"
#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"

JAUAudioArcLoader::JAUAudioArcLoader(JAUSection* section) {
    //JUT_ASSERT(13, section->isOpen());
    //JUT_ASSERT(14, section->isBuilding());
    mSection = section;
}

bool JAUAudioArcLoader::load(void const* param_0) {
    return parse(param_0);
}

void JAUAudioArcLoader::readWS(u32 param_0, void const* param_1, u32 param_2) {
    mSection->newWaveBank(param_0, param_1);
    if (param_2) {
        mSection->loadWaveArc(param_0, param_2);
    }
}

void JAUAudioArcLoader::readBNK(u32 param_0, void const* param_1) {
    mSection->newBank(param_1, param_0);
}

void JAUAudioArcLoader::readBSC(void const* param_0, u32 param_1) {
    mSection->newSeSeqCollection(param_0, param_1);
}

void JAUAudioArcLoader::readBST(void const* param_0, u32 param_1) {
    mSection->newSoundTable(param_0, param_1, true);
}

void JAUAudioArcLoader::readBSTN(void const* param_0, u32 param_1) {
    mSection->newSoundNameTable(param_0, param_1, true);
}

void JAUAudioArcLoader::readBMS(u32 param_0, void const* param_1, u32 param_2) {
    mSection->newStaticSeqData(param_0, param_1, param_2);
}

void JAUAudioArcLoader::readBMS_fromArchive(u32 param_0) {
    mSection->newStaticSeqData(param_0);
}

void JAUAudioArcLoader::newVoiceBank(u32 param_0, u32 param_1) {
    mSection->newVoiceBank(param_1, param_0);
}

void JAUAudioArcLoader::newDynamicSeqBlock(u32 param_0) {
    JAUSectionHeap* sectionHeap = mSection->asSectionHeap();
    JUT_ASSERT(72, sectionHeap);
    sectionHeap->newDynamicSeqBlock(param_0);
}

void JAUAudioArcLoader::readBSFT(void const* param_0) {
    mSection->newStreamFileTable(param_0, true);
}

void JAUAudioArcLoader::beginBNKList(u32 param_0, u32 param_1) {
    mSection->beginNewBankTable(param_0, param_1);
}

void JAUAudioArcLoader::endBNKList() {
    mSection->endNewBankTable();
}

void JAUAudioArcLoader::readMaxSeCategory(int param_0, int param_1, int param_2) {
    JAISeMgr* seMgr = JASGlobalInstance<JAISeMgr>::getInstance();
    if (seMgr) {
        seMgr->getCategory(param_0)->setMaxActiveSe(param_1);
        seMgr->getCategory(param_0)->setMaxInactiveSe(param_2);
    }
}
