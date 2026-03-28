//
// object-particle
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JParticle/object-particle.h"
#include "JSystem/JStudio/JStudio_JStage/control.h"

namespace JStudio_JParticle {
namespace {

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

class TToString_u32ID_ {
public:
    TToString_u32ID_() {}

    const char* operator()(u32 param_1) {
        snprintf(buf, sizeof(buf), "0x%08x", param_1);
        return buf;
    }

private:
    char buf[11];
};

} // anonymous namespace

TAdaptor_particle::TAdaptor_particle(TCreateObject* param_1) :
    pCreateObject_(param_1),
    pJPAEmitter_(NULL),
    field_0x1a8(this) {
    field_0x1b4 = 0;
    field_0x1b5 = 0;
    field_0x1b6 = 0;
    field_0x1b7 = 0;
    field_0x1b8 = 0;
    field_0x1bc = -1;
    u32FadeTime_end_ = 0;
    u32FadeTime_current_ = 0;
    field_0x1c8 = 0;
    field_0x1cc = 0;
    field_0x1d0 = NULL;
    field_0x1d4 = -1;
    field_0x1d8 = 0;
    JUT_ASSERT(90, pCreateObject_!=NULL);
}

TAdaptor_particle::~TAdaptor_particle() {
    if (pJPAEmitter_ != NULL) {
        if (field_0x1b6 == 0) {
            pCreateObject_->emitter_destroy(pJPAEmitter_);
        } else if (field_0x1b4 != 0) {
            pJPAEmitter_->setEmitterCallBackPtr(field_0x1a8.getOld());
            pJPAEmitter_->quitImmortalEmitter();
        } else {
            JGADGET_WARNMSG(116, "not permitted : not-end on exit\n  demo-object : " << adaptor_getID_string());
            pCreateObject_->emitter_destroy(pJPAEmitter_);
        }
    }
}

void TAdaptor_particle::adaptor_do_prepare() {
    static TSetVariableValue_immediate aoData[18] = {
        TSetVariableValue_immediate(0, 0.0f),
        TSetVariableValue_immediate(1, 0.0f),
        TSetVariableValue_immediate(2, 0.0f),
        TSetVariableValue_immediate(3, 0.0f),
        TSetVariableValue_immediate(4, 0.0f),
        TSetVariableValue_immediate(5, 0.0f),
        TSetVariableValue_immediate(6, 1.0f),
        TSetVariableValue_immediate(7, 1.0f),
        TSetVariableValue_immediate(8, 1.0f),
        TSetVariableValue_immediate(9, 255.0f),
        TSetVariableValue_immediate(10, 255.0f),
        TSetVariableValue_immediate(11, 255.0f),
        TSetVariableValue_immediate(12, 255.0f),
        TSetVariableValue_immediate(13, 255.0f),
        TSetVariableValue_immediate(14, 255.0f),
        TSetVariableValue_immediate(15, 255.0f),
        TSetVariableValue_immediate(16, 255.0f),
        TSetVariableValue_immediate(),
    };
    adaptor_setVariableValue_immediate(aoData);
}

void TAdaptor_particle::adaptor_do_end() {
#if DEBUG
    if (field_0x1b8 == 0) {
        JUT_ASSERT(160, pJPAEmitter_==NULL);
        return;
    }

    JUT_ASSERT(163, pJPAEmitter_!=NULL);

    if (!state_isFade_()) {
        return;
    }

    JUT_ASSERT(166, u32FadeTime_current_<u32FadeTime_end_);

    JGADGET_WARNMSG(169, "particle not finished : " << toString_state_(field_0x1b8) << "\n  demo-object : " << adaptor_getID_string());
#endif
}

void TAdaptor_particle::adaptor_do_update(u32 param_1) {
    const JStudio::TObject* pObject = adaptor_getObject();
    JUT_ASSERT(179, pObject!=NULL);
    const JStudio::TControl* pControl = pObject->getControl();
    JUT_ASSERT(181, pControl!=NULL);
    if (u32FadeTime_end_ != 0 && u32FadeTime_current_ < u32FadeTime_end_) {
        JUT_ASSERT(187, state_isFade_());
        JUT_ASSERT(188, pJPAEmitter_!=NULL);
        u32FadeTime_current_ += param_1;
        if (u32FadeTime_current_ >= u32FadeTime_end_) {
            switch (field_0x1b8) {
            case 1:
                field_0x1b8 = 2;
                break;
            case 3:
                pCreateObject_->emitter_destroy(pJPAEmitter_);
                pJPAEmitter_ = NULL;
                field_0x1b8 = 0;
                break;
            default:
                JUT_ASSERT(203, false);
            }
            u32FadeTime_end_ = 0;
            u32FadeTime_current_ = 0;
        }
    } else {
        JUT_ASSERT(210, !state_isFade_());
    }
    if (field_0x1b5 != 0 && field_0x1b7 != 0 && pJPAEmitter_ == NULL) {
        beginParticle_fadeIn_();
    }
}

void TAdaptor_particle::adaptor_do_PARTICLE(
    JStudio::data::TEOperationData param_1, const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x19: {
        JUT_ASSERT(232, pContent!=NULL);
        JUT_ASSERT(233, uSize==4);
        u32 var_r29 = *(BE(u32)*)pContent;
        field_0x1bc = var_r29;
        break;
    }
    default:
        JGADGET_WARNMSG(241, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_BEGIN(JStudio::data::TEOperationData param_1,
                                                            const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x1:
        JUT_ASSERT(253, uSize==0);
        beginParticle_();
        break;
    default:
        JGADGET_WARNMSG(259, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_BEGIN_FADE_IN(
    JStudio::data::TEOperationData param_1, const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2: {
        JUT_ASSERT(272, pContent!=NULL);
        JUT_ASSERT(273, uSize==4);
        u32 var_r29 = *(BE(f32)*)pContent;
        beginParticle_fadeIn_(var_r29);
        break;
    }
    default:
        JGADGET_WARNMSG(282, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_END(JStudio::data::TEOperationData param_1,
                                                          const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x1:
        JUT_ASSERT(294, uSize==0);
        endParticle_();
        break;
    default:
        JGADGET_WARNMSG(300, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_END_FADE_OUT(
    JStudio::data::TEOperationData param_1, const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2: {
        JUT_ASSERT(313, pContent!=NULL);
        JUT_ASSERT(314, uSize==4);
        u32 var_r29 = *(BE(f32)*)pContent;
        endParticle_fadeOut_(var_r29);
        break;
    }
    default:
        JGADGET_WARNMSG(323, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_PARENT(
    JStudio::data::TEOperationData param_1, const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x18: {
        JUT_ASSERT(336, pContent!=NULL);
        JUT_ASSERT(337, uSize>0);

        const char* szID = (const char*)pContent;
        JGADGET_ASSERTWARN(339, szID[uSize-1]=='\0');

        field_0x1d0 = NULL;
        const JStage::TSystem* pJSGSystem = pCreateObject_->get_pJSGSystem_();
        JUT_ASSERT(345, pJSGSystem!=NULL);
        JStage::TObject* pJSGObject;
        if (pJSGSystem->JSGFindObject(&pJSGObject, szID, JStage::OBJECT_UNDEFINED) != 0) {
            JGADGET_WARNMSG(351, "object not found as parent : " << szID << "\n  demo-object : " << adaptor_getID_string());
            break;
        }

        JUT_ASSERT(354, pJSGObject!=NULL);
        field_0x1d0 = pJSGObject;
        break;
    }
    default:
        JGADGET_WARNMSG(361, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_PARENT_NODE(
    JStudio::data::TEOperationData param_1, const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x18: {
        if (field_0x1d0 == NULL) {
            JGADGET_WARNMSG(377, "object not specified as parent\n  demo-object : " << adaptor_getID_string());
            break;
        }
        JUT_ASSERT(380, pContent!=NULL);
        JUT_ASSERT(381, uSize>0);
        const char* szID = (const char*)pContent;
        JGADGET_ASSERTWARN(383, szID[uSize-1]=='\0');
        field_0x1d4 = field_0x1d0->JSGFindNodeID(szID);
        if (field_0x1d4 != -1) {
            return;
        } else {
            JGADGET_WARNMSG(388, "node not found about parent : " << szID << "\n  demo-object : " << adaptor_getID_string());
            return;
        }
    }
    case JStudio::data::UNK_0x19:
        JUT_ASSERT(396, uSize==4);
        JUT_ASSERT(397, pContent!=NULL);
        field_0x1d4 = *(BE(u32)*)pContent;
        break;
    default:
        JGADGET_WARNMSG(404, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

const char* TAdaptor_particle::toString_state_(int param_1) {
    static const char* const asz[] = {"none", "begin", "stable", "end"};
    return JGadget::toStringFromIndex(param_1, asz, ARRAY_SIZE(asz), "(undefined)");
}

void TAdaptor_particle::adaptor_do_PARENT_ENABLE(JStudio::data::TEOperationData param_1,
                                                          const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2: {
        JUT_ASSERT(417, uSize==4);
        JUT_ASSERT(418, pContent!=NULL);
        u32 var_r29 = *(BE(u32)*)pContent;
        field_0x1d8 = var_r29 != 0;
        break;
    }
    default:
        JGADGET_WARNMSG(426, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_PARENT_FUNCTION(JStudio::data::TEOperationData param_1,
                                                          const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2: {
        JUT_ASSERT(439, uSize==4);
        JUT_ASSERT(440, pContent!=NULL);
        u32 var_r29 = *(BE(u32)*)pContent;
        field_0x1cc = var_r29;
        break;
    }
    default:
        JGADGET_WARNMSG(447, "unknown data-type : " << param_1);
    }
}

void TAdaptor_particle::adaptor_do_REPEAT(JStudio::data::TEOperationData param_1,
                                                          const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2: {
        JUT_ASSERT(460, uSize==4);
        JUT_ASSERT(461, pContent!=NULL);
        u32 var_r29 = *(BE(u32)*)pContent;
        field_0x1b5 = var_r29 != 0;
        break;
    }
    default:
        JGADGET_WARNMSG(469, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::adaptor_do_ON_EXIT_NOT_END(JStudio::data::TEOperationData param_1,
                                                          const void* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2: {
        JUT_ASSERT(482, uSize==4);
        JUT_ASSERT(483, pContent!=NULL);
        u32 var_r29 = *(BE(u32)*)pContent;
        field_0x1b6 = var_r29 != 0;
        break;
    }
    default:
        JGADGET_WARNMSG(491, "unknown data-type : " << param_1 << "\n  demo-object : " << adaptor_getID_string());
    }
}

void TAdaptor_particle::TJPACallback_emitter_::execute(JPABaseEmitter* pJPAEmitter) {
    JUT_ASSERT(518, pJPAEmitter==pThis_->pJPAEmitter_);
    if (pJPAEmitter->isEnableDeleteEmitter()) {
        pThis_->pCreateObject_->emitter_destroy(pJPAEmitter);
        pThis_->pJPAEmitter_ = NULL;
        pThis_->field_0x1b8 = 0;
        pThis_->u32FadeTime_end_ = 0;
        pThis_->u32FadeTime_current_ = 0;
        return;
    }
    const JStudio::TObject* pObject = pThis_->adaptor_getObject();
    JUT_ASSERT(534, pObject!=NULL);
    const JStudio::TControl* pControl = pObject->getControl();
    JUT_ASSERT(536, pControl!=NULL);
    TJPAEmitter_stopDrawParticle_ aTStack_21c(pJPAEmitter);
    JStudio::TControl::TTransform_translation_rotation_scaling VStack_19c;
    JStudio::TControl::TTransform_translation_rotation_scaling aTStack_1c0;
    pThis_->adaptor_getVariableValue_Vec(
        &VStack_19c.translation,
        sauVariableValue_3_TRANSLATION_XYZ);
    pThis_->adaptor_getVariableValue_Vec(
        &VStack_19c.rotation,
        sauVariableValue_3_ROTATION_XYZ);
    pThis_->adaptor_getVariableValue_Vec(
        &VStack_19c.scaling,
        sauVariableValue_3_SCALING_XYZ);
    if (pThis_->field_0x1d8 == 0) {
        const JStudio::TControl::TTransform_translation_rotation_scaling* pVVar9 = &VStack_19c;
        switch(pThis_->field_0x1cc) {
        case 0:
            pVVar9 = pControl->transformOnSet_transform_ifEnabled(VStack_19c, &aTStack_1c0);
            break;
        case 1:
            break;
        default:
            JUT_ASSERT(566, false);
        }
        pJPAEmitter->setGlobalTranslation(pVVar9->translation);
        pJPAEmitter->setGlobalRotation(JGeometry::TVec3<s16>(
            65536.0 * (pVVar9->rotation.x / 360.0),
            65536.0 * (pVVar9->rotation.y / 360.0),
            65536.0 * (pVVar9->rotation.z / 360.0)
        ));
        pJPAEmitter->setGlobalScale(pVVar9->scaling);
    } else {
        Mtx afStack_1f0;
        if (JStudio_JStage::transform_toGlobalFromLocal(afStack_1f0, VStack_19c, pThis_->field_0x1d0,
                                                        pThis_->field_0x1d4) == 0)
        {
            JGADGET_WARNMSG(583, "can\'t transform about parent\n  demo-object : " << pThis_->adaptor_getID_string());
            return;
        }
        pJPAEmitter->setGlobalSRTMatrix(afStack_1f0);
    }

    f64 alpha = 1.0;
    u32 u32FadeTime_end = pThis_->u32FadeTime_end_;
    u32 u32FadeTime_current = pThis_->u32FadeTime_current_;
    f64 var_f30 = u32FadeTime_end;
    f64 var_f29 = u32FadeTime_current;
    switch (pThis_->field_0x1b8) {
    case 1:
        JUT_ASSERT(606, u32FadeTime_end>0);
        alpha = var_f29 / var_f30;
        break;
    case 2:
        break;
    case 3:
        JUT_ASSERT(611, u32FadeTime_end>0);
        alpha = (var_f30 - var_f29) / var_f30;
        break;
    default:
        JUT_ASSERT(619, false);
    }

    u8 globalAlpha = 255;
    GXColor a_Stack_224;
    pThis_->adaptor_getVariableValue_GXColor(&a_Stack_224, sauVariableValue_4_COLOR_RGBA);
    alpha *= a_Stack_224.a;
    if (alpha < 255.0) {
        globalAlpha = alpha;
    }
    pJPAEmitter->setGlobalAlpha(globalAlpha);
    aTStack_21c.set(NULL);
    if (pOld != NULL) {
        pOld->execute(pJPAEmitter);
    }
}

void TAdaptor_particle::TJPACallback_emitter_::executeAfter(JPABaseEmitter* pJPAEmitter) {
    if (pOld != NULL) {
        pOld->executeAfter(pJPAEmitter);
    }
}

void TAdaptor_particle::TJPACallback_emitter_::draw(JPABaseEmitter* pJPAEmitter) {
    if (pOld != NULL) {
        pOld->draw(pJPAEmitter);
    }
}

void TAdaptor_particle::TJPACallback_emitter_::drawAfter(JPABaseEmitter* pJPAEmitter) {
    if (pOld != NULL) {
        pOld->drawAfter(pJPAEmitter);
    }
}

void TAdaptor_particle::beginParticle_fadeIn_() {
    if (pJPAEmitter_ != NULL) {
        JGADGET_WARNMSG(686,
                        "past emitter already exist\n  demo-object : " << adaptor_getID_string() <<
                        "\n  begin prepare particle-ID : " << TToString_u32ID_()(field_0x1bc));
        pCreateObject_->emitter_destroy(pJPAEmitter_);
    }
    JPABaseEmitter* pEmitter = pCreateObject_->emitter_create(field_0x1bc);
    pJPAEmitter_ = pEmitter;
    if (pEmitter == NULL) {
        JGADGET_WARNMSG(696,
                        "can\'t create emitter : " << TToString_u32ID_()(field_0x1bc) <<
                        "\n  demo-object : " << adaptor_getID_string());
        return;
    }

    field_0x1a8.setOld(pEmitter->getEmitterCallBackPtr());
    pEmitter->setEmitterCallBackPtr(&field_0x1a8);
    pEmitter->becomeImmortalEmitter();
    field_0x1b7 = 1;
    field_0x1b8 = 1;
    if (field_0x1c8 == 0) {
        field_0x1b8 = 2;
    }
    u32FadeTime_end_ = field_0x1c8;
    u32FadeTime_current_ = 0;
}

void TAdaptor_particle::endParticle_fadeOut_(u32 param_1) {
    if (pJPAEmitter_ == NULL) {
        return;
    }
    if (param_1 == 0) {
        pCreateObject_->emitter_destroy(pJPAEmitter_);
        pJPAEmitter_ = NULL;
        field_0x1b8 = 0;
        u32FadeTime_end_ = 0;
        u32FadeTime_current_ = 0;
        return;
    }
    u8 eState_old = field_0x1b8 & 0xff;
    field_0x1b8 = 3;
    if (u32FadeTime_current_ == 0) {
        JUT_ASSERT(731, !state_isFade_(eState_old));
        u32FadeTime_end_ = param_1;
        return;
    }
    JUT_ASSERT(735, state_isFade_(eState_old));
    u32 u32Denominator = 0;

    switch(eState_old) {
    case 1:
        u32Denominator = u32FadeTime_current_;
        break;
    case 3:
        u32Denominator = u32FadeTime_end_ - u32FadeTime_current_;
        break;
    default:
        JUT_ASSERT(791, false);
    }
    JUT_ASSERT(793, u32Denominator!=0);
    u32FadeTime_end_ = ((f64)param_1 * (f64)u32FadeTime_end_) / u32Denominator;
    u32FadeTime_current_ = u32FadeTime_end_ - param_1;
}

} // namespace JStudio_JParticle
