//
// object-ambientlight
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/object-ambientlight.h"

JStudio_JStage::TAdaptor_ambientLight::TAdaptor_ambientLight(JStage::TSystem const* param_1,
                                                             JStage::TAmbientLight* param_2)
    : TAdaptor_object_(this, param_1, param_2) {}

JStudio_JStage::TAdaptor_ambientLight::~TAdaptor_ambientLight() {
    adaptor_do_end();
}

void JStudio_JStage::TAdaptor_ambientLight::adaptor_do_prepare() {
}

void JStudio_JStage::TAdaptor_ambientLight::adaptor_do_begin() {
    adaptor_object_begin_();
    JStage::TAmbientLight* light = get_pJSG_();
    GXColor color = light->JSGGetColor();
    adaptor_setVariableValue_GXColor(sauVariableValue_4_COLOR_RGBA, color);
}

void JStudio_JStage::TAdaptor_ambientLight::adaptor_do_end() {
    adaptor_object_end_();
}

void JStudio_JStage::TAdaptor_ambientLight::adaptor_do_update(u32 param_1) {
    JStage::TAmbientLight* lightObj = get_pJSG_();
    GXColor color;
    adaptor_getVariableValue_GXColor(&color, sauVariableValue_4_COLOR_RGBA);
    lightObj->JSGSetColor(color);
    pJSGObject_->JSGUpdate();
}

void JStudio_JStage::TAdaptor_ambientLight::adaptor_do_data(void const* param_1, u32 param_2,
                                                            void const* param_3, u32 param_4) {
    adaptor_object_data_(param_1, param_2, param_3, param_4);
}
