#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPADynamicsBlock.h"
#include "JSystem/JParticle/JPAResource.h"
#include "string.h"

struct JPAResourceLoader {
    JPAResourceLoader(u8 const*, JPAResourceManager*);
};

JPAResourceManager::JPAResourceManager(void const* pData, JKRHeap* pHeap) {
    mpResArr = NULL;
    mpTexArr = NULL;
    mResMax = 0;
    mResNum = 0;
    mTexMax = 0;
    mTexNum = 0;
    mpHeap = pHeap;
    JPAResourceLoader loader((u8 const*)pData, this);
}

JPAResource* JPAResourceManager::getResource(u16 usrIdx) const {
    for (u16 i = 0; i < mResNum; i++)
        if (mpResArr[i]->getUsrIdx() == usrIdx)
            return mpResArr[i];
    return NULL;
}

bool JPAResourceManager::checkUserIndexDuplication(u16 usrIdx) const {
    for (s32 i = 0; i < mResNum; i++)
        if (mpResArr[i]->getUsrIdx() == usrIdx)
            return true;
    return false;
}

const ResTIMG* JPAResourceManager::swapTexture(ResTIMG const* img, char const* swapName) {
    const ResTIMG* ret = NULL;

    for (s32 i = 0; i < mTexNum; i++) {
        if (strcmp(swapName, mpTexArr[i]->getName()) == 0) {
            JUTTexture* tex = mpTexArr[i]->getJUTTexture();
            ret = tex->getTexInfo();
            tex->storeTIMG(img, (u8)0);
            break;
        }
    }

    return ret;
}

void JPAResourceManager::registRes(JPAResource* res) {
    mpResArr[mResNum] = res;
    mResNum++;
}

void JPAResourceManager::registTex(JPATexture* tex) {
    mpTexArr[mTexNum] = tex;
    mTexNum++;
}

u32 JPAResourceManager::getResUserWork(u16 usrIdx) const {
    u32 ret = 0;

    JPAResource* res = getResource(usrIdx);
    if (res != NULL)
        ret = res->getDyn()->getResUserWork();

    return ret;
}
