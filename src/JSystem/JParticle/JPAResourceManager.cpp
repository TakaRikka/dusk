#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JParticle/JPAResource.h"
#include <string>

struct JPAResourceLoader {
    JPAResourceLoader(u8 const*, JPAResourceManager*);
};

JPAResourceManager::JPAResourceManager(void const* p_jpc, JKRHeap* pHeap) {
    pResAry = NULL;
    pTexAry = NULL;
    resMaxNum = 0;
    resRegNum = 0;
    texMaxNum = 0;
    texRegNum = 0;
    mpHeap = pHeap;
    JUT_ASSERT(49, (p_jpc != 0) && (pHeap != 0));
    JPAResourceLoader loader((u8 const*)p_jpc, this);
}

JPAResource* JPAResourceManager::getResource(u16 usrIdx) const {
    for (u16 i = 0; i < resRegNum; i++)
        if (pResAry[i]->getUsrIdx() == usrIdx)
            return pResAry[i];
    return NULL;
}

bool JPAResourceManager::checkUserIndexDuplication(u16 usrIdx) const {
    for (s32 i = 0; i < resRegNum; i++)
        if (pResAry[i]->getUsrIdx() == usrIdx)
            return true;
    return false;
}

const ResTIMG* JPAResourceManager::swapTexture(ResTIMG const* img, char const* swapName) {
    const ResTIMG* ret = NULL;
    JUTTexture* tex = NULL;

    for (s32 i = 0; i < texRegNum; i++) {
        if (strcmp(swapName, pTexAry[i]->getName()) == 0) {
            tex = pTexAry[i]->getJUTTexture();
            ret = tex->getTexInfo();
            tex->storeTIMG(img, (u8)0);
            break;
        }
    }

    return ret;
}

void JPAResourceManager::registRes(JPAResource* res) {
    JUT_ASSERT(151, resRegNum < resMaxNum);
    pResAry[resRegNum] = res;
    resRegNum++;
}

void JPAResourceManager::registTex(JPATexture* tex) {
    JUT_ASSERT(166, texRegNum < texMaxNum);
    pTexAry[texRegNum] = tex;
    texRegNum++;
}

u32 JPAResourceManager::getResUserWork(u16 usrIdx) const {
    u32 ret = 0;

    JPAResource* res = getResource(usrIdx);
    if (res != NULL)
        ret = res->getDyn()->getResUserWork();

    return ret;
}
