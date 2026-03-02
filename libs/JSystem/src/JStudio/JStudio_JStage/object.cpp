//
// object
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/object.h"

bool JStudio_JStage::transform_toGlobalFromLocal(
    f32 (*param_1)[4], JStudio::TControl::TTransform_translation_rotation_scaling const& param_2,
    JStage::TObject const* param_3, u32 param_4) {
    if (param_3 == NULL) {
        return false;
    }
    Mtx nodeTransformMtx;
    if (!param_3->JSGGetNodeTransformation(param_4, nodeTransformMtx)) {
        return false;
    }
    Mtx amStack_70;
    JStudio::math::getTransformation_SRxyzT(amStack_70, param_2.scaling, param_2.rotation,
                                            param_2.translation);
    MTXConcat(nodeTransformMtx, amStack_70, param_1);
    return true;
}

bool JStudio_JStage::transform_toGlobalFromLocal(
    f32 (*param_1)[4], JStudio::TControl::TTransform_position const& param_2,
    JStage::TObject const* param_3, u32 param_4) {
    if (param_3 == NULL) {
        return false;
    }
    Mtx nodeTransformMtx;
    if (!param_3->JSGGetNodeTransformation(param_4, nodeTransformMtx)) {
        return false;
    }
    Mtx amStack_70;
    JStudio::math::getTransformation_T(amStack_70, &param_2);
    MTXConcat(nodeTransformMtx, amStack_70, param_1);
    return true;
}

bool JStudio_JStage::transform_toLocalFromGlobal(
    f32 (*param_1)[4], JStudio::TControl::TTransform_translation_rotation_scaling const& param_2,
    JStage::TObject const* param_3, u32 param_4) {
    if (param_3 == NULL) {
        return false;
    }
    Mtx nodeTransformMtx;
    if (!param_3->JSGGetNodeTransformation(param_4, nodeTransformMtx)) {
        return false;
    }
    Mtx afStack_70;
    JStudio::math::getTransformation_SRxyzT(afStack_70, param_2.scaling, param_2.rotation,
                                            param_2.translation);
    Mtx auStack_a0;
    MTXInverse(nodeTransformMtx, auStack_a0);
    MTXConcat(auStack_a0, afStack_70, param_1);
    return true;
}

bool JStudio_JStage::transform_toLocalFromGlobal(
    f32 (*param_1)[4], JStudio::TControl::TTransform_position const& param_2,
    JStage::TObject const* param_3, u32 param_4) {
    if (param_3 == NULL) {
        return false;
    }
    Mtx nodeTransformMtx;
    if (!param_3->JSGGetNodeTransformation(param_4, nodeTransformMtx)) {
        return false;
    }
    Mtx afStack_70;
    JStudio::math::getTransformation_T(afStack_70, &param_2);
    Mtx auStack_a0;
    MTXInverse(nodeTransformMtx, auStack_a0);
    MTXConcat(auStack_a0, afStack_70, param_1);
    return true;
}

void JStudio_JStage::TAdaptor_object_::adaptor_object_data_(void const* param_1, u32 param_2,
                                                            void const* param_3, u32 param_4) {
    int uVar3;
    if (param_2 == 0) {
        uVar3 = JStage::TActor::ID_NORMAL;
    } else {
        uVar3 = *(int*)param_1;
    }
    pJSGObject_->JSGSetData(uVar3, param_3, param_4);
}

JStage::TObject*
JStudio_JStage::TAdaptor_object_::adaptor_object_findJSGObject_(char const* param_1) {
    JStage::TObject* pJSGObject;
    if (pJSGSystem_->JSGFindObject(&pJSGObject, param_1, JStage::OBJECT_UNDEFINED) == 0) {
        JUT_ASSERT(122, pJSGObject != NULL);
    } else {
        return NULL;
    }
    return pJSGObject;
}

s32 JStudio_JStage::TAdaptor_object_::adaptor_object_findJSGObjectNode_(
    JStage::TObject const* param_1, char const* param_2) {
    if (param_1 == NULL) {
        return -1;
    }
    return param_1->JSGFindNodeID(param_2);
}

void
JStudio_JStage::TAdaptor_object_::adaptor_object_ENABLE_(JStudio::data::TEOperationData param_1,
                                                         void const* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(157, uSize==4);
        JUT_ASSERT(158, pContent!=NULL);
        if (*(u32*)pContent != 0) {
            pJSGObject_->JSGFEnableFlag(2);
        } else {
            pJSGObject_->JSGFDisableFlag(2);
        }
        break;
    }
}
