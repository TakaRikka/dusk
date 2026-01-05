#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JStage/control.h"
#include "JSystem/JStage/JSGActor.h"
#include "JSystem/JStage/JSGLight.h"
#include "JSystem/JStage/JSGFog.h"
#include "JSystem/JStage/JSGCamera.h"
#include "JSystem/JStage/JSGAmbientLight.h"

JStudio_JStage::TCreateObject::~TCreateObject() {}

bool JStudio_JStage::TCreateObject::create(
    JStudio::TObject** ppObject, JStudio::stb::data::TParse_TBlock_object const& rParse) {
    JUT_ASSERT(83, ppObject!=NULL);
    *ppObject = NULL;
    ObjCreateFuncT pfnCreateObject;
    JStage::TEObject eObject;
    switch(rParse.get_type()) {
    case 'JACT':
        pfnCreateObject = createObject_JSG_<TAdaptor_actor, JStage::TActor>;
        eObject = JStage::OBJECT_ACTOR;
        break;
    case 'JCMR':
        pfnCreateObject = createObject_JSG_<TAdaptor_camera, JStage::TCamera>;
        eObject = JStage::OBJECT_CAMERA;
        break;
    case 'JABL':
        pfnCreateObject = createObject_JSG_<TAdaptor_ambientLight, JStage::TAmbientLight>;
        eObject = JStage::OBJECT_AMBIENT;
        break;
    case 'JLIT':
        pfnCreateObject = createObject_JSG_<TAdaptor_light, JStage::TLight>;
        eObject = JStage::OBJECT_LIGHT;
        break;
    case 'JFOG':
        pfnCreateObject = createObject_JSG_<TAdaptor_fog, JStage::TFog>;
        eObject = JStage::OBJECT_FOG;
        break;
    default:
        return NULL;
    }
    JUT_ASSERT(99, pfnCreateObject!=NULL);
    JUT_ASSERT(100, eObject!=JStage::OBJECT_UNDEFINED);
    JUT_ASSERT(109, pJSGSystem_!=NULL);
    JStage::TObject* pJSGObject;
    int pJVar2 = pJSGSystem_->JSGFindObject(&pJSGObject, (char const*)rParse.get_ID(), eObject);
    switch(pJVar2) {
    case 0:
        JUT_ASSERT(115, pJSGObject!=NULL);
        *ppObject = pfnCreateObject(rParse, pJSGObject, pJSGSystem_);
        return true;
    case 2:
        return true;
    }
    return false;
}
