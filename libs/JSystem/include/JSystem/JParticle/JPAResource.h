#ifndef JPARESOURCE_H
#define JPARESOURCE_H

#include <types.h>
#include "helpers/endian.h"

class JKRHeap;
struct JPAEmitterWorkData;
class JPABaseEmitter;
class JPABaseParticle;

class JPABaseShape;
class JPAExtraShape;
class JPAChildShape;
class JPAExTexShape;
class JPADynamicsBlock;
class JPAFieldBlock;
class JPAKeyBlock;

#if TARGET_PC
struct ParticleDrawCtx;
#endif

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAResource {
public:
    JPAResource();
    void init(JKRHeap*);
    bool calc(JPAEmitterWorkData*, JPABaseEmitter*);
    void draw(JPAEmitterWorkData*, JPABaseEmitter*);
    void drawP(JPAEmitterWorkData*);
    void drawC(JPAEmitterWorkData*);
    void setPTev();
    void setCTev(JPAEmitterWorkData*);
    void calc_p(JPAEmitterWorkData*, JPABaseParticle*);
    void calc_c(JPAEmitterWorkData*, JPABaseParticle*);
    void calcField(JPAEmitterWorkData*, JPABaseParticle*);
    void calcKey(JPAEmitterWorkData*);
    void calcWorkData_c(JPAEmitterWorkData*);
    void calcWorkData_d(JPAEmitterWorkData*);

    JPABaseShape* getBsp() const { return pBsp; }
    JPAExtraShape* getEsp() const { return pEsp; }
    JPAChildShape* getCsp() const { return pCsp; }
    JPAExTexShape* getEts() const { return pEts; }
    JPADynamicsBlock* getDyn() const { return pDyn; }

    const u16 getTexIdx(u8 idx) { return mpTDB1[idx]; }
    u16 getUsrIdx() const { return mUsrIdx; }

public:
    typedef void (*EmitterFunc)(JPAEmitterWorkData*);
    typedef void (*ParticleFunc)(JPAEmitterWorkData*, JPABaseParticle*);
#if TARGET_PC
    typedef void (*DrawParticleFunc)(JPAEmitterWorkData*, JPABaseParticle*,
                                     ParticleDrawCtx*);
#else
    typedef ParticleFunc DrawParticleFunc;
#endif
    /* 0x00 */ EmitterFunc* mpCalcEmitterFuncList;
    /* 0x04 */ EmitterFunc* mpDrawEmitterFuncList;
    /* 0x08 */ EmitterFunc* mpDrawEmitterChildFuncList;
    /* 0x0C */ ParticleFunc* mpCalcParticleFuncList;
    /* 0x10 */ DrawParticleFunc* mpDrawParticleFuncList;
    /* 0x14 */ ParticleFunc* mpCalcParticleChildFuncList;
    /* 0x18 */ DrawParticleFunc* mpDrawParticleChildFuncList;

    /* 0x1C */ JPABaseShape* pBsp;
    /* 0x20 */ JPAExtraShape* pEsp;
    /* 0x24 */ JPAChildShape* pCsp;
    /* 0x28 */ JPAExTexShape* pEts;
    /* 0x2C */ JPADynamicsBlock* pDyn;
    /* 0x30 */ JPAFieldBlock** ppFld;
    /* 0x34 */ JPAKeyBlock** ppKey;
    /* 0x38 */ BE(u16) const* mpTDB1;
    /* 0x3C */ u16 mUsrIdx;
    /* 0x3E */ u8 fldNum;
    /* 0x3F */ u8 keyNum;
    /* 0x40 */ u8 texNum;
    /* 0x41 */ u8 mpCalcEmitterFuncListNum;
    /* 0x42 */ u8 mpDrawEmitterFuncListNum;
    /* 0x43 */ u8 mpDrawEmitterChildFuncListNum;
    /* 0x44 */ u8 mpCalcParticleFuncListNum;
    /* 0x45 */ u8 mpDrawParticleFuncListNum;
    /* 0x46 */ u8 mpCalcParticleChildFuncListNum;
    /* 0x47 */ u8 mpDrawParticleChildFuncListNum;

#if TARGET_PC
    struct BatchInfo {
        f32 vtxPos[8][3];
        f32 vtxUv[8][2];
        u8 vtxCount;        // 4 (quad) or 8 (cross)
        bool supported;     // draw func list contains only batchable funcs
        bool hasPtclColor;  // per-particle JPARegist* func is present
        bool hasPtclTexMtx; // JPALoadCalcTexCrdMtxAnm is present
    };
    BatchInfo mBatchInfo;

    void initBatchInfo();
#endif
};

#endif /* JPARESOURCE_H */
