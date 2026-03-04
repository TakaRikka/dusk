//
// object-fog
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/object-fog.h"

JStudio_JStage::TAdaptor_fog::TAdaptor_fog(JStage::TSystem const* param_0, JStage::TFog* param_1)
    : TAdaptor_object_(this, param_0, param_1) {}

JStudio_JStage::TAdaptor_fog::~TAdaptor_fog() {
    adaptor_do_end();
}

JStudio_JStage::TVariableValueOutput_object_<JStudio_JStage::TAdaptor_fog, JStage::TFog> JStudio_JStage::TAdaptor_fog::saoVVOutput_[3] = {
    JStudio_JStage::TVariableValueOutput_object_<JStudio_JStage::TAdaptor_fog, JStage::TFog>(JStudio_JStage::TAdaptor_fog::TEFOG_4, &JStage::TFog::JSGSetStartZ, &JStage::TFog::JSGGetStartZ),
    JStudio_JStage::TVariableValueOutput_object_<JStudio_JStage::TAdaptor_fog, JStage::TFog>(JStudio_JStage::TAdaptor_fog::TEFOG_5, &JStage::TFog::JSGSetEndZ, &JStage::TFog::JSGGetEndZ),
    JStudio_JStage::TVariableValueOutput_object_<JStudio_JStage::TAdaptor_fog, JStage::TFog>(),
};

void JStudio_JStage::TAdaptor_fog::adaptor_do_prepare() {
    TVariableValueOutput_object_<JStudio_JStage::TAdaptor_fog, JStage::TFog>* pOutput =
        saoVVOutput_;
    while (!pOutput->isEnd_()) {
        pOutput->adaptor_setOutput_(this);
        pOutput++;
    }
}

void JStudio_JStage::TAdaptor_fog::adaptor_do_begin() {
    adaptor_object_begin_();
    JStage::TFog* pFog = get_pJSG_();
    adaptor_setVariableValue_GXColor(sauVariableValue_4_COLOR_RGBA, pFog->JSGGetColor());
    TVariableValueOutput_object_<JStudio_JStage::TAdaptor_fog, JStage::TFog>* pOutput =
        saoVVOutput_;
    while (!pOutput->isEnd_()) {
        pOutput->setVariableValue_(pFog, this);
        pOutput++;
    }
}

void JStudio_JStage::TAdaptor_fog::adaptor_do_end() {
    adaptor_object_end_();
}

void JStudio_JStage::TAdaptor_fog::adaptor_do_update(u32 param_1) {
    JStage::TFog* pFog = get_pJSG_();
    GXColor color;
    adaptor_getVariableValue_GXColor(&color, sauVariableValue_4_COLOR_RGBA);
    pFog->JSGSetColor(color);
    pJSGObject_->JSGUpdate();
}

void JStudio_JStage::TAdaptor_fog::adaptor_do_data(void const* param_1, u32 param_2,
                                                       void const* param_3, u32 param_4) {
    adaptor_object_data_(param_1, param_2, param_3, param_4);
}
