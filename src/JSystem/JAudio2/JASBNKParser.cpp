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
    void* end = (void*)((intptr_t)stream + header->mSize);
    TChunk* chunk = (TChunk*)((intptr_t)stream + 0x20);
    while (chunk < end) {
        if (chunk->mID == id) {
            return chunk;
        }
        chunk = (TChunk*)(((intptr_t)chunk + 0xb + chunk->mSize) & ~3);
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
    JUT_ASSERT(139, envt_chunk);
    TOscChunk* osc_chunk = (TOscChunk*)findChunk(stream, 'OSCT');
    JUT_ASSERT(142, osc_chunk);
    TListChunk* list_chunk = (TListChunk*)findChunk(stream, 'LIST');
    JUT_ASSERT(145, list_chunk);

    u8* envt = new (heap, 2) u8[envt_chunk->mSize];
    JASCalc::bcopy(envt_chunk->mData, envt, envt_chunk->mSize);

    u32* ptr = &osc_chunk->mCount;
    u32 count = *ptr++;
    JASOscillator::Data* osc_data = new (heap, 0) JASOscillator::Data[count];
    for (int i = 0; i < count; i++, ptr += sizeof(TOsc) >> 2) {
        TOsc* op = (TOsc*)ptr;
        JUT_ASSERT(155, op->id == 'Osci');
        JASOscillator::Data* data = &osc_data[i];
        data->mTarget = op->mTarget;
        data->_04 = op->_08;
        data->mScale = op->mScale;
        data->_14 = op->_18;
        data->mTable = (JASOscillator::Point*)(envt + op->mTableOffset);
        data->rel_table = (JASOscillator::Point*)(envt + op->_10);
    }
    TListChunk* list = list_chunk;
    JUT_ASSERT(172, list->count <= JASBank::PRG_OSC);

    bank->newInstTable(list_chunk->count, heap);
    for (int i = 0; i < list_chunk->count; i++) {
        if (list_chunk->mOffsets[i] != 0) {
            u32* data = (u32*)((intptr_t)stream + list_chunk->mOffsets[i]);
            switch (*data++) {
            case 'Inst': {
                JASBasicInst* instp = new (heap, 0) JASBasicInst();
                JUT_ASSERT(187, instp != NULL);
                u32 count = *data++;
                for (int j = 0; j < count; j++) {
                    u32 index = *data++;
                    instp->setOsc(j, &osc_data[index]);
                }
                count = *data++;
                for (int j = 0; j < count; j++) {
                    data++;
                }
                count = *data++;
                instp->setKeyRegionCount(count, heap);
                for (int j = 0; j < count; j++) {
                    JASBasicInst::TKeymap* keymap = instp->getKeyRegion(j);
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
                instp->setVolume(*(f32*)&data[0]);
                instp->setPitch(*(f32*)&data[1]);
                bank->setInst(i, instp);
                break;
            }

            case 'Perc': {
                JASDrumSet* drump = new (heap, 0) JASDrumSet();
                JUT_ASSERT(264, drump != NULL);
                u32 pmap_count = data[1];
                JUT_ASSERT(268, pmap_count <= 128);
                u32 count = *data++;
                drump->newPercArray(count, heap);
                for (int j = 0; j < count; j++) {
                    u32 offset = *data++;
                    if (offset != 0) {
                        JASDrumSet::TPerc* percp = new (heap, 0) JASDrumSet::TPerc();
                        JUT_ASSERT(277, percp);
                        u32 type = data[0];
                        JUT_ASSERT(282, type == 'Pmap');
                        u32* ptr = (u32*)((intptr_t)stream + offset);
                        TPercData* perc_data = (TPercData*)(ptr + 1);
                        percp->setVolume(perc_data->mVolume);
                        percp->setPitch(perc_data->mPitch);
                        percp->setPan((f32)perc_data->mPan / 127.0f);
                        percp->setRelease(perc_data->mRelease);
                        ptr = (u32*)&perc_data->field_0xc;
                        u32 count2 = *ptr++;
                        for (int k = 0; k < count2; k++) {
                            ptr++;
                        }
                        u32 pVar6 = ptr[0];
                        percp->field_0xe = JSULoHalf(ptr[2]);
                        percp->field_0x10 = *(f32*)&ptr[3];
                        percp->field_0x14 = *(f32*)&ptr[4];
                        for (int k = 0; k < pVar6; k++) {}
                        drump->setPerc(j, percp);
                    }
                }
                bank->setInst(i, drump);
                break;
            }
            default:
                JUT_PANIC(338, "Unexpected Inst Type");
                break;
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
                            osc->rel_table = table;
                        } else {
                            osc->rel_table = NULL;
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

JASOscillator::Data* JASBNKParser::Ver0::findOscPtr(JASBasicBank* bank, THeader const* header,
                                                    TOsc const* osc) {
    TOffsetData const* data = &header->mOffsets;
    for (int i = 0; i < 0x80; i++) {
        TInst* tinst = data->mInstOffset[i].ptr(header);
        if (tinst != NULL) {
            for (int j = 0; j < 2; j++) {
                if (tinst->mOscOffset[j].ptr(header) == osc) {
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
    const JASOscillator::Point* ptr = points;
    while(true) {
        s16 tmp = ptr->_0;
        ptr++;
        if (tmp > 10) {
            break;
        }
    }
    return ptr;
}

// Fakematch? Why is this here?
template<>
JASMemPool_MultiThreaded<JASChannel> JASPoolAllocObject_MultiThreaded<JASChannel>::memPool_;
