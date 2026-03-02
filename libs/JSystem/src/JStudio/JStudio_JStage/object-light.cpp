#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/object-light.h"

JStudio_JStage::TAdaptor_light::TAdaptor_light(JStage::TSystem const* param_1,
                                               JStage::TLight* param_2)
    : JStudio_JStage::TAdaptor_object_(this, param_1, param_2) {
    field_0x11c = 0;
}

JStudio_JStage::TAdaptor_light::~TAdaptor_light() {
    adaptor_do_end();
}

JStudio_JStage::TAdaptor_light::TVVOutput_direction_
    JStudio_JStage::TAdaptor_light::saoVVOutput_direction_[6] = {
        JStudio_JStage::TAdaptor_light::TVVOutput_direction_(
            JStudio_JStage::TAdaptor_light::TE_VALUE_10,
            JStudio_JStage::TAdaptor_light::DIRECTION_1),
        JStudio_JStage::TAdaptor_light::TVVOutput_direction_(
            JStudio_JStage::TAdaptor_light::TE_VALUE_11,
            JStudio_JStage::TAdaptor_light::DIRECTION_1),
        JStudio_JStage::TAdaptor_light::TVVOutput_direction_(
            JStudio_JStage::TAdaptor_light::TE_VALUE_7,
            JStudio_JStage::TAdaptor_light::DIRECTION_2),
        JStudio_JStage::TAdaptor_light::TVVOutput_direction_(
            JStudio_JStage::TAdaptor_light::TE_VALUE_8,
            JStudio_JStage::TAdaptor_light::DIRECTION_2),
        JStudio_JStage::TAdaptor_light::TVVOutput_direction_(
            JStudio_JStage::TAdaptor_light::TE_VALUE_9,
            JStudio_JStage::TAdaptor_light::DIRECTION_2),
        JStudio_JStage::TAdaptor_light::TVVOutput_direction_(),
};

void JStudio_JStage::TAdaptor_light::adaptor_do_prepare() {
    TVVOutput_direction_* pOutput = saoVVOutput_direction_;
    while (!pOutput->isEnd_()) {
        pOutput->adaptor_setOutput_(this);
        pOutput++;
    }
}

void JStudio_JStage::TAdaptor_light::adaptor_do_begin() {
    {
        adaptor_object_begin_();
        JStage::TLight* pLightObj = get_pJSG_();
        const JStudio::TObject* pObject = adaptor_getObject();
        JUT_ASSERT(60, pObject!=NULL);
        const JStudio::TControl* pControl = pObject->getControl();
        JUT_ASSERT(62, pControl!=NULL);
        GXColor lightColor = pLightObj->JSGGetColor();
        adaptor_setVariableValue_GXColor(sauVariableValue_4_COLOR_RGBA, lightColor);
        JStudio::TControl::TTransform_position_direction lightObjTransform;
        JStudio::TControl::TTransform_position_direction aTStack_b8;
        pLightObj->JSGGetPosition(&lightObjTransform.position);
        pLightObj->JSGGetDirection(&lightObjTransform.direction);
        const JStudio::TControl::TTransform_position_direction* finalTransform =
            (const JStudio::TControl::TTransform_position_direction*)
            pControl->transformOnGet_transform_ifEnabled(lightObjTransform, &aTStack_b8);
        adaptor_setVariableValue_Vec(sauVariableValue_3_POSITION_XYZ, finalTransform->position);
        f32 dirx = finalTransform->direction.x;
        f32 diry = finalTransform->direction.y;
        f32 dirz = finalTransform->direction.z;
        f32 sqr = sqrtf(dirx * dirx + dirz * dirz);
        f32 dVar11 = atan2f(dirx, dirz);
        f32 dVar9 = atan2f(diry, sqr);
        adaptor_setVariableValue_immediate(10, MTXRadToDeg(dVar11));
        adaptor_setVariableValue_immediate(11, MTXRadToDeg(dVar9));
        Vec VStack_c4;
        VECAdd(&finalTransform->position, &finalTransform->direction, &VStack_c4);
        adaptor_setVariableValue_Vec(sauVariableValue_3_TARGET_POSITION_XYZ, VStack_c4);
    }
}

void JStudio_JStage::TAdaptor_light::adaptor_do_end() {
    adaptor_object_end_();
}

void JStudio_JStage::TAdaptor_light::adaptor_do_update(u32 param_1) {
    JStage::TLight* lightObj = get_pJSG_();
    const JStudio::TObject* pObject = adaptor_getObject();
    JUT_ASSERT(118, pObject!=NULL);
    const JStudio::TControl* pControl = pObject->getControl();
    JUT_ASSERT(120, pControl!=NULL);
    GXColor lightColor;
    adaptor_getVariableValue_GXColor(&lightColor, sauVariableValue_4_COLOR_RGBA);
    lightObj->JSGSetColor(lightColor);
    JStudio::TControl::TTransform_position_direction VStack_198;
    JStudio::TControl::TTransform_position_direction aTStack_1b0;
    adaptor_getVariableValue_Vec(&VStack_198.position, sauVariableValue_3_POSITION_XYZ);
    switch (field_0x11c) {
    case 1: {
        f32 dVar10 = adaptor_getVariableValue(10)->getValue();
        f32 dVar11 = adaptor_getVariableValue(11)->getValue();
        f32 temp = MTXDegToRad(dVar11);
        f32 dVar12 = cosf(temp);
        f32 sinVal = sinf(temp);
        f32 temp2 = MTXDegToRad(dVar10);
        VStack_198.direction.x = dVar12 * sinf(temp2);
        VStack_198.direction.y = sinVal;
        VStack_198.direction.z = dVar12 * cosf(temp2);
        break;
    }
    case 2:
        Vec VStack_1c0;
        adaptor_getVariableValue_Vec(&VStack_1c0, sauVariableValue_3_TARGET_POSITION_XYZ);
        VECSubtract(&VStack_1c0, &VStack_198.position, &VStack_198.direction);
    }
    const JStudio::TControl::TTransform_position_direction* pTrans =
        pControl->transformOnSet_transform_ifEnabled(VStack_198, &aTStack_1b0);
    lightObj->JSGSetPosition(pTrans->position);
    lightObj->JSGSetDirection(pTrans->direction);
    pJSGObject_->JSGUpdate();
}

void JStudio_JStage::TAdaptor_light::adaptor_do_data(void const* param_1, u32 param_2,
                                                     void const* param_3, u32 param_4) {
    adaptor_object_data_(param_1, param_2, param_3, param_4);
}

void JStudio_JStage::TAdaptor_light::adaptor_do_FACULTY(JStudio::data::TEOperationData param_1,
                                                        void const* pContent, u32 uSize) {
    switch (param_1) {
    case JStudio::data::UNK_0x2:
        JUT_ASSERT(198, uSize==4);
        JUT_ASSERT(199, pContent!=NULL);
        JStage::TELight lightType;
        switch (*(u32*)pContent) {
        case 0x301:
            lightType = JStage::TELIGHT_1;
            break;
        case 0x302:
            lightType = JStage::TELIGHT_2;
            break;
        case 0x303:
            lightType = JStage::TELIGHT_3;
            break;
        default:
            // TODO: find out how to lose the goto
            goto end;
        }
        get_pJSG_()->JSGSetLightType(lightType);
        break;
    }
    end:;
}

void JStudio_JStage::TAdaptor_light::adaptor_do_ENABLE(JStudio::data::TEOperationData param_1,
                                                       void const* param_2, u32 param_3) {
    adaptor_object_ENABLE_(param_1, param_2, param_3);
}

void JStudio_JStage::TAdaptor_light::TVVOutput_direction_::operator()(
    f32 param_0, JStudio::TAdaptor* param_1) const {
    JStudio_JStage::TAdaptor_light* p = (JStudio_JStage::TAdaptor_light*)param_1;
    JUT_ASSERT(246, p!=NULL);
    p->field_0x11c = field_0x8;
}
