#ifndef JSTUDIO_JPARTICLE_CONTROL_H
#define JSTUDIO_JPARTICLE_CONTROL_H

#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JStage/JSGSystem.h"
#include "JSystem/JStudio/JStudio/jstudio-object.h"

namespace JStudio_JParticle {
struct TCreateObject : public JStudio::TCreateObject {
    typedef JStudio::TObject* (*CreateFunc)(JStudio::stb::data::TParse_TBlock_object const&,
                                            JStudio_JParticle::TCreateObject*);
    TCreateObject(JPAEmitterManager* p_emitMgr, const JStage::TSystem* p_system) {
        pJPAEmitterManager_ = p_emitMgr;
        pJSGSystem_ = p_system;
        mPermit_onExit_notEnd = false;
        JUT_ASSERT(48, pJPAEmitterManager_!=NULL);
    }

    virtual ~TCreateObject();
    virtual bool create(JStudio::TObject**,
                                       JStudio::stb::data::TParse_TBlock_object const&);
    virtual JPABaseEmitter* emitter_create(u32);
    virtual void emitter_destroy(JPABaseEmitter*);

    static JStudio::TObject*
    createObject_JPA_PARTICLE_(JStudio::stb::data::TParse_TBlock_object const&,
                               JStudio_JParticle::TCreateObject*);

    const JStage::TSystem* get_pJSGSystem_() const { return pJSGSystem_; }
    bool isPermit_onExit_notEnd() { return mPermit_onExit_notEnd; }
    
    /* 0x0C */ JPAEmitterManager* pJPAEmitterManager_;
    /* 0x10 */ const JStage::TSystem* pJSGSystem_;
    /* 0x14 */ bool mPermit_onExit_notEnd;
};

};  // namespace JStudio_JParticle

#endif /* JSTUDIO_JPARTICLE_CONTROL_H */
