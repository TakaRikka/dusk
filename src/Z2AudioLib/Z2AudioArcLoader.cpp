#include "Z2AudioLib/Z2AudioArcLoader.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "Z2AudioLib/Z2FxLineMgr.h"
#include "m_Do/m_Do_ext.h"

Z2AudioArcLoader::Z2AudioArcLoader(JAUSection* section) : JAUAudioArcLoader(section) {}

bool Z2AudioArcLoader::readCommandMore(u32 command) {
    bool rv = false;
    u32 read = 0;

    switch (command) {
    case 'bfca':
        read = readU32_();
        readBFCA(getContent_(read));
        rv = true;
        break;
    }

    return rv;
}

void Z2AudioArcLoader::readBFCA(const void* addr) {
    JUT_ASSERT(76, addr);

    JKRHeap* heap = JAUSectionHeap::getInstance()->getHeap();
    JUT_ASSERT(78, heap);
    JUT_ASSERT(79, Z2GetFxLineMgr());

    JKRArchive* archive = JKRMountArchive((void*)addr, JKRGetCurrentHeap(), JKRArchive::MOUNT_DIRECTION_TAIL);
    Z2GetFxLineMgr()->initDataArc(archive, heap);
    JKRUnmountArchive(archive);
}

void Z2AudioArcLoader::readBSTN(const void* addr, u32 param_1) {
#if VERSION == VERSION_SHIELD_DEBUG
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    JUT_ASSERT(97, sectionHeap);

    JUT_ASSERT(99, sectionHeap->isOpen());
    JUT_ASSERT(100, sectionHeap->isBuilding());
    JUT_ASSERT(101, addr);

    JUT_ASSERT(103, sectionHeap->getSectionHeapData().soundNameTable == 0);

    JKRHeap* heap = JKRHeap::getCurrentHeap();
    JKRHeap* gameHeap = (JKRHeap*)mDoExt_getGameHeap();
    JKRSetCurrentHeap(gameHeap);

    u8* bstnDst = (u8*)addr;
    if (param_1 != 0) {
        bstnDst = new (gameHeap, 4) u8[param_1];
        if (bstnDst != NULL) {
            memcpy(bstnDst, addr, param_1);
        }

        JUT_ASSERT(122, bstnDst);
    }

    JAUSoundNameTable* soundNameTable = new (gameHeap, 0) JAUSoundNameTable(true);
    JUT_ASSERT(125, soundNameTable);

    soundNameTable->init(bstnDst);

    JAUSectionHeap::TSectionHeapData& sectionHeapData = (JAUSectionHeap::TSectionHeapData&)sectionHeap->getSectionHeapData();
    sectionHeapData.soundNameTable = soundNameTable;

    JAUSection::TSectionData& sectionData = (JAUSection::TSectionData&)sectionHeap->getSectionData();
    sectionData.mBstnDst = bstnDst;

    JKRSetCurrentHeap(heap);
#endif
}
