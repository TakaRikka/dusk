#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASBNKParser.h"
#include "JSystem/JAudio2/JASBasicBank.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASDrumSet.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/JSupport/JSupport.h"

JASBank* JASBNKParser::createBank(void const* stream, JKRHeap* heap) {
    return createBasicBank(stream, heap);
}

u32 JASBNKParser::sUsedHeapSize;

JASBasicBank* JASBNKParser::createBasicBank(void const* stream, JKRHeap* heap) {
    if (heap == NULL) {
        heap = JASDram;
    }

    u32 free_size = heap->getFreeSize();
    
    TFileHeader* filep = (TFileHeader*)stream;
    JUT_ASSERT(59, filep->id == 'IBNK');
    JASBasicBank* bank = NULL;
    switch (filep->mVersion) {
    case 0:
        bank = Ver0::createBasicBank(stream, heap);
        break;
    case 1:
        bank = Ver1::createBasicBank(stream, heap);
        break;
    }

    sUsedHeapSize += free_size - heap->getFreeSize();
    return bank;
}

JASBNKParser::Ver1::TChunk* JASBNKParser::Ver1::findChunk(void const* stream, u32 id) {
    TFileHeader* header = (TFileHeader*)stream;
    void* end = (void*)((int)stream + header->mSize);
    TChunk* chunk = (TChunk*)((int)stream + 0x20);
    while (chunk < end) {
        if (chunk->mID == id) {
            return chunk;
        }
        chunk = (TChunk*)(((int)chunk + 0xb + chunk->mSize) & ~3);
    }
    return NULL;
}

JASBasicBank* JASBNKParser::Ver1::createBasicBank(void const* stream, JKRHeap* heap) {
    if (heap == NULL) {
        heap = JASDram;
    }

    JASBasicBank* bank = new (heap, 0) JASBasicBank();
    if (bank == NULL) {
        return NULL;
    }

    TEnvtChunk* envt_chunk = (TEnvtChunk*)findChunk(stream, 'ENVT');
    TOscChunk* osc_chunk = (TOscChunk*)findChunk(stream, 'OSCT');
    TListChunk* list_chunk = (TListChunk*)findChunk(stream, 'LIST');

    u8* envt = new (heap, 2) u8[envt_chunk->mSize];
    JASCalc::bcopy(envt_chunk->mData, envt, envt_chunk->mSize);

    u32* ptr = &osc_chunk->mCount;
    u32 count = *ptr++;
    JASOscillator::Data* osc_data = new (heap, 0) JASOscillator::Data[count];
    for (int i = 0; i < count; i++, ptr += sizeof(TOsc) >> 2) {
        TOsc* osc = (TOsc*)ptr;
        JASOscillator::Data* data = &osc_data[i];
        data->mTarget = osc->mTarget;
        data->_04 = osc->_08;
        data->mScale = osc->mScale;
        data->_14 = osc->_18;
        data->mTable = (JASOscillator::Point*)(envt + osc->mTableOffset);
        data->_0C = (JASOscillator::Point*)(envt + osc->_10);
    }

    bank->newInstTable(list_chunk->mCount, heap);
    for (int i = 0; i < list_chunk->mCount; i++) {
        if (list_chunk->mOffsets[i] != 0) {
            u32* data = (u32*)((int)stream + list_chunk->mOffsets[i]);
            switch (*data++) {
            case 'Inst': {
                JASBasicInst* inst = new (heap, 0) JASBasicInst();
                u32 count = *data++;
                for (int j = 0; j < count; j++) {
                    u32 index = *data++;
                    inst->setOsc(j, &osc_data[index]);
                }
                count = *data++;
                for (int j = 0; j < count; j++) {
                    data++;
                }
                count = *data++;
                inst->setKeyRegionCount(count, heap);
                for (int j = 0; j < count; j++) {
                    JASBasicInst::TKeymap* keymap = inst->getKeyRegion(j);
                    keymap->setHighKey(*data >> 0x18);
                    u32 fVar4 = data[1];
                    keymap->field_0x4 = JSULoHalf(data[3]);
                    keymap->field_0x8 = *(f32*)&data[4];
                    keymap->field_0xc = *(f32*)&data[5];
                    data += 2;
                    for (int k = 0; k < fVar4; k++) {
                        data += 4;
                    }
                }
                inst->setVolume(*(f32*)&data[0]);
                inst->setPitch(*(f32*)&data[1]);
                bank->setInst(i, inst);
                break;
            }

            case 'Perc': {
                JASDrumSet* drum = new (heap, 0) JASDrumSet();
                u32 count = *data++;
                drum->newPercArray(count, heap);
                for (int j = 0; j < count; j++) {
                    u32 offset = *data++;
                    if (offset != 0) {
                        JASDrumSet::TPerc* perc = new (heap, 0) JASDrumSet::TPerc();
                        u32* ptr = (u32*)((int)stream + offset);
                        TPercData* perc_data = (TPercData*)(ptr + 1);
                        perc->setVolume(perc_data->mVolume);
                        perc->setPitch(perc_data->mPitch);
                        perc->setPan((f32)perc_data->mPan / 127.0f);
                        perc->setRelease(perc_data->mRelease);
                        ptr = (u32*)&perc_data->field_0xc;
                        u32 count2 = *ptr++;
                        for (int k = 0; k < count2; k++) {
                            ptr++;
                        }
                        u32 pVar6 = ptr[0];
                        perc->field_0xe = JSULoHalf(ptr[2]);
                        perc->field_0x10 = *(f32*)&ptr[3];
                        perc->field_0x14 = *(f32*)&ptr[4];
                        for (int k = 0; k < pVar6; k++) {}
                        drum->setPerc(j, perc);
                    }
                }
                bank->setInst(i, drum);
                break;
            }
            }
        }
    }

    return bank;
}

JASBasicBank* JASBNKParser::Ver0::createBasicBank(void const* stream, JKRHeap* heap) {
    if (heap == NULL) {
        heap = JASDram;
    }
    
    THeader const* header = (THeader*)stream;
    JASBasicBank* bank = new (heap, 0) JASBasicBank();
    if (bank == NULL) {
        return NULL;
    }

    bank->newInstTable(0x80, heap);

    for (int i = 0; i < 0x80; i++) {
        TInst* tinst = header->mOffsets.mInstOffset[i].ptr(header);
        if (tinst != NULL) {
            JASBasicInst* instp = new (heap, 0) JASBasicInst();
            JUT_ASSERT(368, instp != NULL);
            instp->setVolume(tinst->mVolume);
            instp->setPitch(tinst->mPitch);

            int osc_idx = 0;
            for (int j = 0; j < 2; j++) {
                TOsc* tosc = tinst->mOscOffset[j].ptr(header);
                if (tosc != NULL) {
                    JASOscillator::Data* osc = findOscPtr(bank, header, tosc);
                    if (osc != NULL) {
                        instp->setOsc(osc_idx, osc);
                    } else {
                        osc = new (heap, 0) JASOscillator::Data();
                        JUT_ASSERT(386, osc != NULL);
                        osc->mTarget = tosc->mTarget;
                        osc->_04 = tosc->field_0x4;

                        JASOscillator::Point* points = tosc->mPointOffset.ptr(header);
                        if (points != NULL) {
                            const JASOscillator::Point* endPtr = getOscTableEndPtr(points);
                            int size = endPtr - points;
                            JASOscillator::Point* table = new (heap, 0) JASOscillator::Point[size];
                            JUT_ASSERT(396, table != NULL);
                            JASCalc::bcopy(points, table, size * sizeof(JASOscillator::Point));
                            osc->mTable = table;
                        } else {
                            osc->mTable = NULL;
                        }

                        points = tosc->field_0xc.ptr(header);
                        if (points != NULL) {
                            const JASOscillator::Point* endPtr = getOscTableEndPtr(points);
                            int size = endPtr - points;
                            JASOscillator::Point* table = new (heap, 0) JASOscillator::Point[size];
                            JUT_ASSERT(409, table != NULL);
                            JASCalc::bcopy(points, table, size * sizeof(JASOscillator::Point));
                            osc->_0C = table;
                        } else {
                            osc->_0C = NULL;
                        }

                        osc->mScale = tosc->mScale;
                        osc->_14 = tosc->field_0x14;
                        instp->setOsc(osc_idx, osc);
                    }

                    osc_idx++;
                }
            }

            instp->setKeyRegionCount(tinst->mKeyRegionCount, heap);
            for (int j = 0; j < tinst->mKeyRegionCount; j++) {
                JASBasicInst::TKeymap* keymap = instp->getKeyRegion(j);
                TKeymap* tkeymap = tinst->mKeymapOffset[j].ptr(header);
                keymap->setHighKey(tkeymap->mHighKey);
                TVmap* tvmap = tkeymap->mVmapOffset.ptr(header);
                keymap->field_0x4 = JSULoHalf(tvmap->field_0x4);
                keymap->field_0x8 = tvmap->field_0x8;
                keymap->field_0xc = tvmap->field_0xc;
            }

            bank->setInst(i, instp);
        }
    }

    for (int i = 0; i < 12; i++) {
        TPerc* tperc = header->mOffsets.mPercOffset[i].ptr(header);
        if (tperc != NULL) {
            JASDrumSet* setp = new (heap, 0) JASDrumSet();
            JUT_ASSERT(509, setp != NULL);
            setp->newPercArray(0x80, heap);

            for (int j = 0; j < 0x80; j++) {
                TPmap* tpmap = tperc->mPmapOffset[j].ptr(header);
                if (tpmap != NULL) {
                    JASDrumSet::TPerc* percp = new (heap, 0) JASDrumSet::TPerc();
                    JUT_ASSERT(519, percp);
                    percp->setVolume(tpmap->mVolume);
                    percp->setPitch(tpmap->mPitch);
                    if (tperc->mMagic == 'PER2') {
                        percp->setPan(tperc->mPan[j] / 127.0f);
                        percp->setRelease(tperc->mRelease[j]);
                    }
                    TVmap* vmap = tpmap->mVmapOffset.ptr(header);
                    percp->field_0xe = JSULoHalf(vmap->field_0x4);
                    percp->field_0x10 = vmap->field_0x8;
                    percp->field_0x14 = vmap->field_0xc;
                    setp->setPerc(j, percp);
                }
            }

            bank->setInst(i + 0xe4, setp);
        }
    }

    return bank;
}

/* 80299E68-80299F8C 2947A8 0124+00 1/1 0/0 0/0 .text
 * findOscPtr__Q212JASBNKParser4Ver0FP12JASBasicBankPCQ312JASBNKParser4Ver07THeaderPCQ312JASBNKParser4Ver04TOsc
 */
JASOscillator::Data* JASBNKParser::Ver0::findOscPtr(JASBasicBank* bank, THeader const* header,
                                                    TOsc const* osc) {
    TOffsetData const* data = &header->mOffsets;
    for (int i = 0; i < 0x80; i++) {
        TInst* tinst = data->mInstOffset[i].ptr(header);
        if (tinst != NULL) {
            for (int j = 0; j < 2; j++) {
                TOsc* tosc = tinst->mOscOffset[j].ptr(header);
                if (tosc == osc) {
                    JASInst* inst = bank->getInst(i);
                    if (inst != NULL) {
                        JASInstParam param;
                        inst->getParam(0x3c, 0x7f, &param);
                        if (j < param.field_0x1d) {
                            return param.field_0x14[j];
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

JASOscillator::Point const* JASBNKParser::Ver0::getOscTableEndPtr(JASOscillator::Point const* points) {
    s16 tmp;
    do {
        tmp = points->_0;
        points++;
    } while (tmp <= 10);
    return points;
}
