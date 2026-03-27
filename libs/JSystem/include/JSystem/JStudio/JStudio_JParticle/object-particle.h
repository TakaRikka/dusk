#ifndef OBJECT_PARTICLE_H
#define OBJECT_PARTICLE_H

#include "JSystem/JStudio/JStudio_JParticle/control.h"

namespace JStudio_JParticle {

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

        JPAEmitterCallBack* getOld() const { return pOld; }
        void setOld(JPAEmitterCallBack* cb) { pOld = cb; }

        /* 0x4 */ TAdaptor_particle* pThis_;
        /* 0x8 */ JPAEmitterCallBack* pOld;
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

    static const char* toString_state_(int param_1);

    static bool state_isFade_(u8 param_1) {
        return param_1 == 1 || param_1 == 3;
    }

    bool state_isFade_() const {
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
    /* 0x1B5 */ bool field_0x1b5;
    /* 0x1B6 */ bool field_0x1b6;
    /* 0x1B7 */ u8 field_0x1b7;
    /* 0x1B8 */ u8 field_0x1b8;
    /* 0x1BC */ u32 field_0x1bc;
    /* 0x1C0 */ u32 u32FadeTime_end_;
    /* 0x1C4 */ u32 u32FadeTime_current_;
    /* 0x1C8 */ u32 field_0x1c8;
    /* 0x1CC */ u32 field_0x1cc;
    /* 0x1D0 */ JStage::TObject* field_0x1d0;
    /* 0x1D4 */ u32 field_0x1d4;
    /* 0x1D8 */ bool field_0x1d8;
};

} // namespace JStudio_JParticle

#endif /* OBJECT_PARTICLE_H */
