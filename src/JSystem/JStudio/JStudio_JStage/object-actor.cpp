#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/object-actor.h"

JStudio_JStage::TAdaptor_actor::TAdaptor_actor(JStage::TSystem const* param_1,
                                                   JStage::TActor* param_2)         :
                                                   TAdaptor_object_(this, param_1,param_2) {
    field_0x130 = 0;
    field_0x134 = 0;
    field_0x138 = 0;
    field_0x13c = NULL;
    field_0x140 = -1;
    field_0x144 = 0;
    field_0x148 = NULL;
    field_0x14c = -1;
}

JStudio_JStage::TAdaptor_actor::~TAdaptor_actor() {
    adaptor_do_end();
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_prepare() {
    const TVVOutputObject* pOutput = saoVVOutput_;
    while (!pOutput->isEnd_()) {
        pOutput->adaptor_setOutput_(this);
        pOutput++;
    }

    const TVVOutput_ANIMATION_FRAME_* pAnimOutput = saoVVOutput_ANIMATION_FRAME_;
    while (!pAnimOutput->isEnd_()) {
        pAnimOutput->adaptor_setOutput_(this);
        pAnimOutput++;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_begin() {
    adaptor_object_begin_();
    const JStage::TActor* pActor = get_pJSG_();
#if DEBUG
    { // This block is needed to match the stack in debug
        const JStudio::TObject* pObject = adaptor_getObject();
        JUT_ASSERT(72, pObject!=NULL);
        const JStudio::TControl* pControl = pObject->getControl();
        JUT_ASSERT(74, pControl!=NULL);
        getJSG_SRT_(pControl);
    }
#else
    // Fakematch, load order is wrong in retail when using the inlines shown in debug
    getJSG_SRT_((const JStudio::TControl*)(adaptor_getObject()->pControl));
#endif

    const TVVOutputObject* pOutput = saoVVOutput_;
    while (!pOutput->isEnd_()) {
        pOutput->setVariableValue_(pActor, this);
        pOutput++;
    }

    const TVVOutput_ANIMATION_FRAME_* pAnimOutput = saoVVOutput_ANIMATION_FRAME_;
    while (!pAnimOutput->isEnd_()) {
        pAnimOutput->setVariableValue_(pActor, this);
        pAnimOutput++;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_end() {
    adaptor_object_end_();
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_update(u32 param_1) {
    const JStudio::TObject* pObject = adaptor_getObject();
    JUT_ASSERT(104, pObject != NULL);
    const JStudio::TControl* pControl = pObject->getControl();
    JUT_ASSERT(106, pControl != NULL);
    setJSG_SRT_(pControl);
    pJSGObject_->JSGUpdate();
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_data(void const* param_1, u32 param_2,
                                                     void const* param_3, u32 param_4) {
    adaptor_object_data_(param_1, param_2, param_3, param_4);
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_SHAPE(JStudio::data::TEOperationData operation,
                                                          void const* param_2, u32 param_3) {
    setJSG_ID_(&JStage::TActor::JSGSetShape, operation, param_2, param_3);
}


void JStudio_JStage::TAdaptor_actor::adaptor_do_ANIMATION(JStudio::data::TEOperationData operation,
                                                          void const* param_2, u32 param_3) {
    setJSG_ID_(&JStage::TActor::JSGSetAnimation, operation, param_2, param_3);
} 


void JStudio_JStage::TAdaptor_actor::adaptor_do_ANIMATION_MODE(JStudio::data::TEOperationData operation,
                                                          void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(153, uSize==4);
        JUT_ASSERT(154, pContent!=NULL);
        field_0x130 = *(u32*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_TEXTURE_ANIMATION(JStudio::data::TEOperationData operation,
                                                          void const* param_2, u32 param_3) {
    setJSG_ID_(&JStage::TActor::JSGSetTextureAnimation, operation, param_2, param_3);
} 

void JStudio_JStage::TAdaptor_actor::adaptor_do_TEXTURE_ANIMATION_MODE(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(182, uSize==4);
        JUT_ASSERT(183, pContent!=NULL);
        field_0x134 = *(u32*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_PARENT(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(203, pContent!=NULL);
        JUT_ASSERT(204, uSize!=0);
        field_0x13c = adaptor_object_findJSGObject_((const char*)pContent);
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_PARENT_NODE(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(226, pContent!=NULL);
        JUT_ASSERT(227, uSize!=0);
        field_0x140 = adaptor_object_findJSGObjectNode_(field_0x13c, (const char*)pContent);
        break;
    case JStudio::data::UNK_0x19:
        JUT_ASSERT(237, uSize==4);
        JUT_ASSERT(238, pContent!=NULL);
        field_0x140 = *(u32*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_PARENT_ENABLE(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(257, uSize==4);
        JUT_ASSERT(258, pContent!=NULL);
        bool v0 = (*(u32*)pContent != 0);
        if (field_0x144 != v0) {
            field_0x144 = v0;
            switch (field_0x138) {
            case 0:
                break;
            case 1:
                JStage::TObject* object = NULL;
                u32 v1 = -1;
                if (v0 != 0) {
                    object = field_0x13c;
                    v1     = field_0x140;
                }
                pJSGObject_->JSGSetParent(object, v1);
                break;
            }
        }
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_PARENT_FUNCTION(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(303, uSize==4);
        JUT_ASSERT(304, pContent!=NULL);
        field_0x138 = *(u32*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_RELATION(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(324, pContent!=NULL);
        JUT_ASSERT(325, uSize!=0);
        field_0x148 = adaptor_object_findJSGObject_((const char*)pContent);
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_RELATION_NODE(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x18:
        JUT_ASSERT(347, pContent!=NULL);
        JUT_ASSERT(348, uSize!=0);
        field_0x14c = adaptor_object_findJSGObjectNode_(field_0x13c, (const char*)pContent);
        break;
    case JStudio::data::UNK_0x19:
        JUT_ASSERT(358, uSize==4);
        JUT_ASSERT(359, pContent!=NULL);
        field_0x14c = *(u32*)pContent;
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::adaptor_do_RELATION_ENABLE(
    JStudio::data::TEOperationData operation, void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(378, uSize==4);
        JUT_ASSERT(379, pContent!=NULL);
        pJSGObject_->JSGSetRelation(*(u32*)pContent, field_0x148, field_0x14c);
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::setJSG_ID_(void (JStage::TActor::*function)(u32),
                                                    JStudio::data::TEOperationData operation,
                                                    void const* pContent, u32 uSize) {
    switch (operation) {
    case JStudio::data::UNK_0x19:
        (((JStage::TActor*)pJSGObject_)->*function)(*(u32*)pContent);
        break;
    }
}

void JStudio_JStage::TAdaptor_actor::setJSG_SRT_(JStudio::TControl const* pControl) {
    JStudio::TControl::TTransform_translation_rotation_scaling transform;
    JStudio::TControl::TTransform_translation_rotation_scaling multipliedTransform;
    adaptor_getVariableValue_Vec(&transform.translation, sauVariableValue_3_TRANSLATION_XYZ);
    adaptor_getVariableValue_Vec(&transform.rotation, sauVariableValue_3_ROTATION_XYZ);
    adaptor_getVariableValue_Vec(&transform.scaling, sauVariableValue_3_SCALING_XYZ);
    const JStudio::TControl::TTransform_translation_rotation_scaling* pTransform;
    if (field_0x144 == 0) {
        pTransform = pControl->transformOnSet_transform_ifEnabled(transform, &multipliedTransform);
    } else {
        pTransform = &transform;
        switch (field_0x138) {
        case 0: {
            Mtx mtx;
            bool check;
            if (!transform_toGlobalFromLocal(mtx, *pTransform, field_0x13c, field_0x140)) {
                check = false;
            } else {
                JStudio::math::getFromTransformation_SRxyzT(
                    &multipliedTransform.scaling, &multipliedTransform.rotation,
                    &multipliedTransform.translation, mtx);
                check = true;
            }

            if (!check) {
                return;
            }
            pTransform = &multipliedTransform;
            break;
        }
        case 1:
            break;
        }
    }
    JStage::TActor* pActor = get_pJSG_();
    pActor->JSGSetTranslation(pTransform->translation);
    pActor->JSGSetRotation(pTransform->rotation);
    pActor->JSGSetScaling(pTransform->scaling);
}

void JStudio_JStage::TAdaptor_actor::getJSG_SRT_(JStudio::TControl const* pControl) {
    JStudio::TControl::TTransform_translation_rotation_scaling transform;
    JStudio::TControl::TTransform_translation_rotation_scaling trs;
    Mtx mtx;
    JStage::TActor* pActor = get_pJSG_();
    pActor->JSGGetTranslation(&transform.translation);
    pActor->JSGGetRotation(&transform.rotation);
    pActor->JSGGetScaling(&transform.scaling);

    JStudio::TControl::TTransform_translation_rotation_scaling* outTransform;
    if (!field_0x144) {
        outTransform = pControl->transformOnGet_transform_ifEnabled(&transform, &trs);
    } else {
        outTransform = &transform;
        switch (field_0x138) {
        case 1:
            break;
        case 0:
            bool check;
            if (!transform_toLocalFromGlobal(mtx, *outTransform, field_0x13c, field_0x140)) {
                check = false;
            } else {
                JStudio::math::getFromTransformation_SRxyzT(&trs.scaling, &trs.rotation,
                                                            &trs.translation, mtx);
                check = true;
            }

            if (!check) {
                return;
            }
            outTransform = &trs;
            break;
        }
    }

    adaptor_setVariableValue_Vec(sauVariableValue_3_TRANSLATION_XYZ,
                                 outTransform->translation);
    adaptor_setVariableValue_Vec(sauVariableValue_3_ROTATION_XYZ, outTransform->rotation);
    adaptor_setVariableValue_Vec(sauVariableValue_3_SCALING_XYZ, outTransform->scaling);
}

void JStudio_JStage::TAdaptor_actor::TVVOutput_ANIMATION_FRAME_::operator()(
    f32 param_1, JStudio::TAdaptor* adaptor) const {
    JStage::TActor* actor = static_cast<TAdaptor_actor*>(adaptor)->get_pJSG_();
    // not sure what this bit is
    u32 idx = *(u32*)(((u32)adaptor - 1) + field_0x8);

    u8 idx_lowBytes  = idx;
    u8 idx_highBytes = idx >> 8;

    f32 maxVal = (actor->*mMaxGetter)();
    switch (idx_highBytes) {
    case 1:
        param_1 = maxVal - param_1;
        break;
    case 0:
        break;
    }

    if (maxVal > 0.0f) {
        param_1 = (f32)(*JStudio::TFunctionValue::toFunction_outside(idx_lowBytes))(param_1, maxVal);
    }
    (actor->*mSetter)(param_1);
}

JStudio_JStage::TAdaptor_actor::TVVOutput_ANIMATION_FRAME_::~TVVOutput_ANIMATION_FRAME_() {
}

namespace JStudio_JStage {
    
const TAdaptor_actor::TVVOutputObject TAdaptor_actor::saoVVOutput_[] = {
    TVVOutputObject(TAdaptor_actor::TEACTOR_1, &JStage::TActor::JSGSetAnimationTransition, &JStage::TActor::JSGGetAnimationTransition),
    TVVOutputObject()
};

const TAdaptor_actor::TVVOutput_ANIMATION_FRAME_ TAdaptor_actor::saoVVOutput_ANIMATION_FRAME_[] = {
    TVVOutput_ANIMATION_FRAME_(0, 305, &JStage::TActor::JSGSetAnimationFrame, &JStage::TActor::JSGGetAnimationFrame, &JStage::TActor::JSGGetAnimationFrameMax),
    TVVOutput_ANIMATION_FRAME_(2, 309, &JStage::TActor::JSGSetTextureAnimationFrame, &JStage::TActor::JSGGetTextureAnimationFrame, &JStage::TActor::JSGGetTextureAnimationFrameMax),
    TVVOutput_ANIMATION_FRAME_()
};

};  // namespace JStudio_JStage
