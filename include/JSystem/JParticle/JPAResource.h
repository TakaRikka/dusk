#ifndef JPARESOURCE_H
#define JPARESOURCE_H

#include "dolphin/types.h"

class JKRHeap;
class JPAEmitterWorkData;
class JPABaseEmitter;
class JPABaseParticle;

class JPABaseShape;
class JPAExtraShape;
class JPAChildShape;
class JPAExTexShape;
class JPADynamicsBlock;
class JPAFieldBlock;
class JPAKeyBlock;

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

    JPABaseShape* getBsp() const { return mpBaseShape; }
    JPAExtraShape* getEsp() const { return mpExtraShape; }
    JPAChildShape* getCsp() const { return mpChildShape; }
    JPAExTexShape* getEts() const { return mpExTexShape; }
    JPADynamicsBlock* getDyn() const { return mpDynamicsBlock; }

    u32 getTexIdx(u32 idx) const { return mpTDB1[idx]; }
    u16 getUsrIdx() const { return mUsrIdx; }

public:
    typedef void (*EmitterFunc)(JPAEmitterWorkData*);
    typedef void (*ParticleFunc)(JPAEmitterWorkData*, JPABaseParticle*);
    /* 0x00 */ EmitterFunc* mpCalcEmitterFuncList;
    /* 0x04 */ EmitterFunc* mpDrawEmitterFuncList;
    /* 0x08 */ EmitterFunc* mpDrawEmitterChildFuncList;
    /* 0x0C */ ParticleFunc* mpCalcParticleFuncList;
    /* 0x10 */ ParticleFunc* mpDrawParticleFuncList;
    /* 0x14 */ ParticleFunc* mpCalcParticleChildFuncList;
    /* 0x18 */ ParticleFunc* mpDrawParticleChildFuncList;

    /* 0x1C */ JPABaseShape* mpBaseShape;
    /* 0x20 */ JPAExtraShape* mpExtraShape;
    /* 0x24 */ JPAChildShape* mpChildShape;
    /* 0x28 */ JPAExTexShape* mpExTexShape;
    /* 0x2C */ JPADynamicsBlock* mpDynamicsBlock;
    /* 0x30 */ JPAFieldBlock** mpFieldBlocks;
    /* 0x34 */ JPAKeyBlock** mpKeyBlocks;
    /* 0x38 */ u16 const* mpTDB1;
    /* 0x3C */ u16 mUsrIdx;
    /* 0x3E */ u8 mFieldBlockNum;
    /* 0x3F */ u8 mKeyBlockNum;
    /* 0x40 */ u8 mTDB1Num;
    /* 0x41 */ u8 mpCalcEmitterFuncListNum;
    /* 0x42 */ u8 mpDrawEmitterFuncListNum;
    /* 0x43 */ u8 mpDrawEmitterChildFuncListNum;
    /* 0x44 */ u8 mpCalcParticleFuncListNum;
    /* 0x45 */ u8 mpDrawParticleFuncListNum;
    /* 0x46 */ u8 mpCalcParticleChildFuncListNum;
    /* 0x47 */ u8 mpDrawParticleChildFuncListNum;
};

#endif /* JPARESOURCE_H */
