#ifndef JPAEMITTERMANAGER_H
#define JPAEMITTERMANAGER_H

#include "dolphin/types.h"
#include "JSystem/JParticle/JPAList.h"
#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JGeometry.h"

class JPAEmitterCallBack;
class JPAParticleCallBack;
class JPABaseEmitter;
class JPAResourceManager;
class JPABaseParticle;
class JKRHeap;
struct JPAEmitterWorkData;

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAEmitterManager {
public:
    JPAEmitterManager(u32, u32, JKRHeap*, u8, u8);
    JPABaseEmitter* createSimpleEmitterID(JGeometry::TVec3<f32> const&, u16, u8, u8,
                                                         JPAEmitterCallBack*, JPAParticleCallBack*);
    void calc(u8);
    void draw(JPADrawInfo const*, u8);
    void forceDeleteAllEmitter();
    void forceDeleteGroupEmitter(u8);
    void forceDeleteEmitter(JPABaseEmitter*);
    void entryResourceManager(JPAResourceManager*, u8);
    void clearResourceManager(u8);
    void calcYBBCam();
    JPAResourceManager* getResourceManager(u16 idx) { return pResMgrAry[idx]; }

public:
    /* 0x00 */ JSUList<JPABaseEmitter>* pEmtrUseList;
    /* 0x04 */ JSUList<JPABaseEmitter> mFreeEmtrList;
    /* 0x10 */ JPAList<JPABaseParticle> mPtclPool;
    /* 0x1C */ JPAResourceManager** pResMgrAry;
    /* 0x20 */ JPAEmitterWorkData* pWd;
    /* 0x24 */ u32 emtrNum;
    /* 0x28 */ u32 ptclNum;
    /* 0x2C */ u8 gidMax;
    /* 0x2D */ u8 ridMax;
};

#endif /* JPAEMITTERMANAGER_H */
