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

    const JStage::TSystem* get_pJSGSystem_() { return pJSGSystem_; }
    bool isPermit_onExit_notEnd() { return mPermit_onExit_notEnd; }
    
    /* 0x0C */ JPAEmitterManager* pJPAEmitterManager_;
    /* 0x10 */ const JStage::TSystem* pJSGSystem_;
    /* 0x14 */ bool mPermit_onExit_notEnd;
};

struct TAdaptor_particle : public JStudio::TAdaptor_particle {
    typedef JStudio::TObject_particle ObjectType;

    struct TJPACallback_emitter_ : public JPAEmitterCallBack {
        TJPACallback_emitter_(TAdaptor_particle* param_1) {
            pThis_ = param_1;
            pOld = NULL;
            JUT_ASSERT(113, pThis_!=NULL);
        }
        virtual void execute(JPABaseEmitter*);
        virtual void executeAfter(JPABaseEmitter*);
        virtual void draw(JPABaseEmitter*);
        virtual void drawAfter(JPABaseEmitter*);

        JPAEmitterCallBack* getOld() { return pOld; }
        void setOld(JPAEmitterCallBack* cb) { pOld = cb; }

        /* 0x4 */ TAdaptor_particle* pThis_;
        /* 0x8 */ JPAEmitterCallBack* pOld;
    };

    struct TJPAEmitter_stopDrawParticle_ {
        TJPAEmitter_stopDrawParticle_(JPABaseEmitter* emitter) : pJPAEmitter_(emitter) {}
        ~TJPAEmitter_stopDrawParticle_() {
            if (pJPAEmitter_ != NULL) {
                pJPAEmitter_->stopDrawParticle();
            }
        }
        void set(JPABaseEmitter* emitter) { pJPAEmitter_ = emitter; }
        /* 0x0 */ JPABaseEmitter* pJPAEmitter_;
    };

    TAdaptor_particle(JStudio_JParticle::TCreateObject*);
    void beginParticle_fadeIn_();
    void endParticle_fadeOut_(u32);

    virtual ~TAdaptor_particle();
    virtual void adaptor_do_prepare();
    virtual void adaptor_do_end();
    virtual void adaptor_do_update(u32);
    virtual void adaptor_do_PARTICLE(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_BEGIN(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_BEGIN_FADE_IN(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_END(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_END_FADE_OUT(JStudio::data::TEOperationData, void const*,
                                                u32);
    virtual void adaptor_do_PARENT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_PARENT_NODE(JStudio::data::TEOperationData, void const*,
                                                u32);
    virtual void adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData, void const*,
                                                    u32);
    virtual void adaptor_do_REPEAT(JStudio::data::TEOperationData, void const*, u32);
    virtual void adaptor_do_ON_EXIT_NOT_END(JStudio::data::TEOperationData, void const*,
                                                    u32);
    
    static bool state_isFade_(u8 param_1) {
        return param_1 == 1 || param_1 == 3;
    }

    bool state_isFade_() {
        return state_isFade_(field_0x1b8);
    }

    void beginParticle_fadeIn_(u32 param_1) {
        field_0x1c8 = param_1;
        beginParticle_fadeIn_();
    }

    void beginParticle_() {
        beginParticle_fadeIn_(0);
    }

    void endParticle_() {
        if (pJPAEmitter_ != NULL) {
            pJPAEmitter_->becomeInvalidEmitter();
        }
    }

    void set_bPermit_onExit_notEnd_(bool param_1) { field_0x1b4 = param_1; }
    
    /* 0x1A0 */ TCreateObject* pCreateObject_;
    /* 0x1A4 */ JPABaseEmitter* pJPAEmitter_;
    /* 0x1A8 */ TJPACallback_emitter_ field_0x1a8;
    /* 0x1B4 */ u8 field_0x1b4;
    /* 0x1B5 */ u8 field_0x1b5;
    /* 0x1B6 */ u8 field_0x1b6;
    /* 0x1B7 */ u8 field_0x1b7;
    /* 0x1B8 */ u8 field_0x1b8;
    /* 0x1BC */ u32 field_0x1bc;
    /* 0x1C0 */ u32 u32FadeTime_end;
    /* 0x1C4 */ u32 field_0x1c4;
    /* 0x1C8 */ u32 field_0x1c8;
    /* 0x1CC */ u32 field_0x1cc;
    /* 0x1D0 */ JStage::TObject* field_0x1d0;
    /* 0x1D4 */ u32 field_0x1d4;
    /* 0x1D8 */ u8 field_0x1d8;
};

};  // namespace JStudio_JParticle

#endif /* JSTUDIO_JPARTICLE_CONTROL_H */
