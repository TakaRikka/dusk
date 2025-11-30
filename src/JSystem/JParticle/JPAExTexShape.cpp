#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAExTexShape.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "dolphin/gx.h"

void JPALoadExTex(JPAEmitterWorkData* work) {
    JPAExTexShape* ets = work->mpRes->getEts();

    GXTexCoordID secTexCoordID = GX_TEXCOORD1;
    if (ets->isUseIndirect()) {
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
                          GX_PTIDENTITY);
        u8 texIdx = ets->getIndTexIdx();
        work->mpResMgr->load(work->mpRes->getTexIdx(texIdx), GX_TEXMAP2);
        secTexCoordID = GX_TEXCOORD2;
    }

    if (ets->isUseSecTex()) {
        GXSetTexCoordGen2(secTexCoordID, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
                          GX_PTIDENTITY);
        u8 texIdx = ets->getSecTexIdx();
        work->mpResMgr->load(work->mpRes->getTexIdx(texIdx), GX_TEXMAP3);
    }
}

JPAExTexShape::JPAExTexShape(u8 const* data) {
    mpData = (const JPAExTexShapeData*)data;
}
