#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAResourceLoader.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPABaseShape.h"
#include "JSystem/JParticle/JPAChildShape.h"
#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JParticle/JPAExTexShape.h"
#include "JSystem/JParticle/JPAExtraShape.h"
#include "JSystem/JParticle/JPAFieldBlock.h"
#include "JSystem/JParticle/JPAKeyBlock.h"
#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JParticle/JPAResourceManager.h"

JPAResourceLoader::JPAResourceLoader(u8 const* data, JPAResourceManager* mgr) {
    if (*(u32*)(data + 4) == '2-10') {
        load_jpc(data, mgr);
    }
}

struct JPAResourceHeader {
    /* 0x0 */ u16 mUsrIdx;
    /* 0x2 */ u16 mBlockNum;
    /* 0x4 */ u8 mFieldBlockNum;
    /* 0x5 */ u8 mKeyBlockNum;
    /* 0x6 */ u8 mTDB1Num;
};

void JPAResourceLoader::load_jpc(u8 const* data, JPAResourceManager* mgr) {
    JKRHeap* heap = mgr->mpHeap;
    mgr->mResMax = *(u16*)(data + 8);
    mgr->mTexMax = *(u16*)(data + 0xA);
    mgr->mpResArr = new (heap, 0) JPAResource*[mgr->mResMax];
    mgr->mpTexArr = new (heap, 0) JPATexture*[mgr->mTexMax];

    u32 offset = 0x10;
    for (int i = 0; i < *(u16*)(data + 8); i++) {
        JPAResourceHeader* header = (JPAResourceHeader*)(data + offset);
        JPAResource* res = new (heap, 0) JPAResource();
        res->mFieldBlockNum = header->mFieldBlockNum;
        res->mpFieldBlocks = res->mFieldBlockNum != 0 ?
            new (heap, 0) JPAFieldBlock*[res->mFieldBlockNum] : NULL;
        res->mKeyBlockNum = header->mKeyBlockNum;
        res->mpKeyBlocks = res->mKeyBlockNum != 0 ?
            new (heap, 0) JPAKeyBlock*[res->mKeyBlockNum] : NULL;
        res->mTDB1Num = header->mTDB1Num;
        res->mpTDB1 = NULL;
        res->mUsrIdx = header->mUsrIdx;

        offset += 8;
        u32 field_idx = 0;
        u32 key_idx = 0;

        for (int j = 0; j < header->mBlockNum; j++) {
            u32 magic = *(u32*)(data + offset);
            u32 size = *(u32*)(data + offset + 4);
            switch (magic) {
            case 'FLD1':
                res->mpFieldBlocks[field_idx] = new (heap, 0) JPAFieldBlock(data + offset, heap);
                field_idx++;
                break;
            case 'KFA1':
                res->mpKeyBlocks[key_idx] = new (heap, 0) JPAKeyBlock(data + offset);
                key_idx++;
                break;
            case 'BEM1':
                res->mpDynamicsBlock = new (heap, 0) JPADynamicsBlock(data + offset);
                break;
            case 'BSP1':
                res->mpBaseShape = new (heap, 0) JPABaseShape(data + offset, heap);
                break;
            case 'ESP1':
                res->mpExtraShape = new (heap, 0) JPAExtraShape(data + offset);
                break;
            case 'SSP1':
                res->mpChildShape = new (heap, 0) JPAChildShape(data + offset);
                break;
            case 'ETX1':
                res->mpExTexShape = new (heap, 0) JPAExTexShape(data + offset);
                break;
            case 'TDB1':
                res->mpTDB1 = (const u16*)(data + offset + 8);
                break;
            }
            offset += size;
        }

        res->init(heap);
        mgr->registRes(res);
    }

    offset = *(u32*)(data + 0xC);
    for (int i = 0; i < *(u16*)(data + 0xA); i++) {
        u32 size = *(u32*)(data + offset + 4);
        JPATexture* tex = new (heap, 0) JPATexture(data + offset);
        mgr->registTex(tex);
        offset += size;
    }
}
