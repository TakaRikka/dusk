//
// control
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JParticle/control.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JGadget/pointer.h"

JStudio_JParticle::TCreateObject::~TCreateObject() {}

/* 8028E400-8028E474 288D40 0074+00 1/0 1/0 0/0 .text
 * create__Q217JStudio_JParticle13TCreateObjectFPPQ27JStudio7TObjectRCQ47JStudio3stb4data20TParse_TBlock_object
 */
bool
JStudio_JParticle::TCreateObject::create(JStudio::TObject** ppObject,
                                         JStudio::stb::data::TParse_TBlock_object const& param_2) {
    JUT_ASSERT(40, ppObject!=NULL);
    CreateFunc func;
    switch (param_2.get_type()) {
    case 'JPTC':
        func = createObject_JPA_PARTICLE_;
        break;
    default:
        return false;
    }
    *ppObject = func(param_2, this);
    return true;
}

JPABaseEmitter* JStudio_JParticle::TCreateObject::emitter_create(u32 param_1) {
    JGeometry::TVec3<f32> local_08;
    Vec local_1c = {0.0f, 0.0f, 0.0f};
    local_08 = local_1c;
    return pJPAEmitterManager_->createSimpleEmitterID(
        local_08, param_1 & 0xffff, (param_1 >> 24) & 0xff, (param_1 >> 16) & 0xff, NULL, NULL);
}

void JStudio_JParticle::TCreateObject::emitter_destroy(JPABaseEmitter* pEmitter) {
    pJPAEmitterManager_->forceDeleteEmitter(pEmitter);
}

/* 8028E508-8028E60C 288E48 0104+00 1/1 0/0 0/0 .text
 * createObject_JPA_PARTICLE___Q217JStudio_JParticle13TCreateObjectFRCQ47JStudio3stb4data20TParse_TBlock_objectPQ217JStudio_JParticle13TCreateObject
 */
JStudio::TObject* JStudio_JParticle::TCreateObject::createObject_JPA_PARTICLE_(
    JStudio::stb::data::TParse_TBlock_object const& param_1,
    JStudio_JParticle::TCreateObject* param_2) {
    JStudio_JParticle::TAdaptor_particle* adaptor = new JStudio_JParticle::TAdaptor_particle(param_2);
    if (adaptor == NULL) {
        return NULL;
    }
    JGadget::TPointer_delete<JStudio_JParticle::TAdaptor_particle> deleteGuard(adaptor);
    adaptor->set_bPermit_onExit_notEnd_(param_2->isPermit_onExit_notEnd());
    JStudio::TObject_particle* particle =
        JStudio::TCreateObject::createFromAdaptor<JStudio_JParticle::TAdaptor_particle>(param_1,
                                                                                   adaptor);
    if (particle == NULL) {
        return NULL;
    }
    deleteGuard.set(NULL);
    return particle;
}
