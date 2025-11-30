#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASWSParser.h"
#include "JSystem/JAudio2/JASBasicWaveBank.h"
#include "JSystem/JAudio2/JASSimpleWaveBank.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"

u32 JASWSParser::getGroupCount(void const* stream) {
	THeader* header = (THeader*)stream;
	return header->mCtrlGroupOffset.ptr(header)->mGroupCount;
}


JASWaveBank* JASWSParser::createWaveBank(void const* stream, JKRHeap* heap) {
    if (getGroupCount(stream) == 1) {
        return createSimpleWaveBank(stream, heap);
    } else {
        return createBasicWaveBank(stream, heap);
    }
}

u32 JASWSParser::sUsedHeapSize;

JASBasicWaveBank* JASWSParser::createBasicWaveBank(void const* stream, JKRHeap* heap) {
    if (heap == NULL) {
        heap = JASDram;
    }
    u32 free_size = heap->getFreeSize();

    THeader* header = (THeader*)stream;
    JASBasicWaveBank* wave_bank = new (heap, 0) JASBasicWaveBank();
    if (wave_bank == NULL) {
        return NULL;
    }

    TCtrlGroup* ctrl_group = header->mCtrlGroupOffset.ptr(header);
    wave_bank->setGroupCount(ctrl_group->mGroupCount, heap);
    wave_bank->setWaveTableSize(header->mWaveTableSize, heap);
    for (u32 i = 0; i < ctrl_group->mGroupCount; i++) {
        TCtrl* ctrl = ctrl_group->mCtrlSceneOffsets[i].ptr(header)->mCtrlOffset.ptr(header);
        JASBasicWaveBank::TWaveGroup* wave_group = wave_bank->getWaveGroup(i);
        TWaveArchive* archive = header->mArchiveBankOffset.ptr(header)->mArchiveOffsets[i].ptr(header);
        wave_group->setWaveCount(ctrl->mWaveCount, heap);
        for (int j = 0; j < ctrl->mWaveCount; j++) {
            TWave* wave = archive->mWaveOffsets[j].ptr(header);
            JASWaveInfo wave_info;
            wave_info.field_0x00 = wave->_01;
            wave_info.field_0x01 = wave->_02;
            wave_info.field_0x04 = wave->_04;
            wave_info.field_0x08 = wave->mOffset;
            wave_info.field_0x0c = wave->_0C;
            wave_info.field_0x02 = wave->_10 == 0 ? 0 : 0xff;
            wave_info.field_0x10 = wave->_14;
            wave_info.field_0x14 = wave->_18;
            wave_info.field_0x18 = wave->_1C;
            wave_info.field_0x1c = wave->_20;
            wave_info.field_0x1e = wave->_22;
            TCtrlWave* ctrl_wave = ctrl->mCtrlWaveOffsets[j].ptr(header);
            wave_bank->setWaveInfo(wave_group, j, JSULoHalf(ctrl_wave->_00), wave_info);
        }
        wave_group->setFileName(archive->mFileName);
    }

    sUsedHeapSize += (free_size - heap->getFreeSize());
    return wave_bank;
}

JASSimpleWaveBank* JASWSParser::createSimpleWaveBank(void const* stream, JKRHeap* heap) {
    if (heap == NULL) {
        heap = JASDram;
    }
    u32 free_size = heap->getFreeSize();
    
    THeader* header = (THeader*)stream;
    TCtrlGroup* ctrl_group = header->mCtrlGroupOffset.ptr(header);
    if (ctrl_group->mGroupCount != 1) {
        return NULL;
    }

    JASSimpleWaveBank* wave_bank = new (heap, 0) JASSimpleWaveBank();
    if (wave_bank == NULL) {
        return NULL;
    }

    u32 max = 0;
    TCtrl* ctrl = ctrl_group->mCtrlSceneOffsets[0].ptr(header)->mCtrlOffset.ptr(header);
    TWaveArchive* archive = header->mArchiveBankOffset.ptr(header)->mArchiveOffsets[0].ptr(header);
    for (int i = 0; i < ctrl->mWaveCount; i++) {
        u32 tmp = JSULoHalf(ctrl->mCtrlWaveOffsets[i].ptr(header)->_00);
        if (max < tmp) {
            max = tmp;
        }
    }
    wave_bank->setWaveTableSize(max + 1, heap);
    
    for (int i = 0; i < ctrl->mWaveCount; i++) {
        TWave* wave = archive->mWaveOffsets[i].ptr(header);
        JASWaveInfo wave_info;
        wave_info.field_0x00 = wave->_01;
        wave_info.field_0x01 = wave->_02;
        wave_info.field_0x04 = wave->_04;
        wave_info.field_0x08 = wave->mOffset;
        wave_info.field_0x0c = wave->_0C;
        wave_info.field_0x02 = wave->_10 == 0 ? 0 : 0xff;
        wave_info.field_0x10 = wave->_14;
        wave_info.field_0x14 = wave->_18;
        wave_info.field_0x18 = wave->_1C;
        wave_info.field_0x1c = wave->_20;
        wave_info.field_0x1e = wave->_22;
        TCtrlWave* ctrl_wave = ctrl->mCtrlWaveOffsets[i].ptr(header);
        wave_bank->setWaveInfo(JSULoHalf(ctrl_wave->_00), wave_info);
    }
    wave_bank->setFileName(archive->mFileName);

    sUsedHeapSize += (free_size - heap->getFreeSize());
    return wave_bank;
}
