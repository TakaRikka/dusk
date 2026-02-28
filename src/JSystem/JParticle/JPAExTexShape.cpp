#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAExTexShape.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include <dolphin/gx.h>

void JPALoadExTex(JPAEmitterWorkData* work) {
    JPAExTexShape* ets = work->mpRes->getEts();

    GXTexCoordID secTexCoordID = GX_TEXCOORD1;
    if (ets->isUseIndirect()) {
        GXSetTexCoordGen(secTexCoordID, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        work->mpResMgr->load(work->mpRes->getTexIdx(ets->getIndTexIdx()), GX_TEXMAP2);
        secTexCoordID = GX_TEXCOORD2;
    }

    if (ets->isUseSecTex()) {
        GXSetTexCoordGen(secTexCoordID, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        work->mpResMgr->load(work->mpRes->getTexIdx(ets->getSecTexIdx()), GX_TEXMAP3);
    }
}

JPAExTexShape::JPAExTexShape(u8 const* data) {
    mpData = (const JPAExTexShapeData*)data;
}
