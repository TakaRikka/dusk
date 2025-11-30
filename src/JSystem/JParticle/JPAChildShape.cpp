#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAChildShape.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "dolphin/gx.h"
#include "dolphin/os.h"

void JPARegistChildPrmEnv(JPAEmitterWorkData* work) {
    JPAChildShape* csp = work->mpRes->getCsp();
    JPABaseEmitter* emtr = work->mpEmtr;
    GXColor prm, env;
    csp->getPrmClr(&prm);
    csp->getEnvClr(&env);
    prm.r = COLOR_MULTI(prm.r, emtr->mGlobalPrmClr.r);
    prm.g = COLOR_MULTI(prm.g, emtr->mGlobalPrmClr.g);
    prm.b = COLOR_MULTI(prm.b, emtr->mGlobalPrmClr.b);
    prm.a = COLOR_MULTI(prm.a, emtr->mGlobalPrmClr.a);
    env.r = COLOR_MULTI(env.r, emtr->mGlobalEnvClr.r);
    env.g = COLOR_MULTI(env.g, emtr->mGlobalEnvClr.g);
    env.b = COLOR_MULTI(env.b, emtr->mGlobalEnvClr.b);
    GXSetTevColor(GX_TEVREG0, prm);
    GXSetTevColor(GX_TEVREG1, env);
}

void JPACalcChildAlphaOut(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    f32 anm = (1.0f - ptcl->mTime) * 255.0f;
    OSf32tou8(&anm, &ptcl->mPrmColorAlphaAnm);
}

void JPACalcChildScaleOut(JPAEmitterWorkData* work, JPABaseParticle* ptcl) {
    ptcl->mParticleScaleX = ptcl->mScaleOut * (1.0f - ptcl->mTime);
    ptcl->mParticleScaleY = ptcl->mAlphaWaveRandom * (1.0f - ptcl->mTime);
}

JPAChildShape::JPAChildShape(u8 const* pData) {
    mpData = (JPAChildShapeData*)pData;
}
