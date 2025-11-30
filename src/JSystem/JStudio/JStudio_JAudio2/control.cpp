//
// control
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio_JAudio2/control.h"
#include "JSystem/JGadget/pointer.h"

JStudio_JAudio2::TCreateObject::~TCreateObject() {}

/* 8028D5B0-8028D624 287EF0 0074+00 1/0 0/0 0/0 .text
 * create__Q215JStudio_JAudio213TCreateObjectFPPQ27JStudio7TObjectRCQ47JStudio3stb4data20TParse_TBlock_object
 */
bool JStudio_JAudio2::TCreateObject::create(
    JStudio::TObject** ppObject, JStudio::stb::data::TParse_TBlock_object const& param_2) {
    JUT_ASSERT(40, ppObject!=NULL);
    CreateFunc func;
    switch (param_2.get_type()) {
    case 'JSND':
        func = createObject_JAI_SOUND_;
        break;
    default:
        return false;
    }
    *ppObject = func(param_2, this);
    return 1;
}

/* 8028D624-8028D728 287F64 0104+00 1/1 0/0 0/0 .text
 * createObject_JAI_SOUND___Q215JStudio_JAudio213TCreateObjectFRCQ47JStudio3stb4data20TParse_TBlock_objectPQ215JStudio_JAudio213TCreateObject
 */
JStudio::TObject* JStudio_JAudio2::TCreateObject::createObject_JAI_SOUND_(
    JStudio::stb::data::TParse_TBlock_object const& param_1,
    JStudio_JAudio2::TCreateObject* param_2) {
    JStudio_JAudio2::TAdaptor_sound* adaptor = new JStudio_JAudio2::TAdaptor_sound(param_2);
    if (adaptor == NULL) {
        return NULL;
    }
    JGadget::TPointer_delete<JStudio_JAudio2::TAdaptor_sound> deleteGuard(adaptor);
    adaptor->set_bPermit_onExit_notEnd_(param_2->isPermit_onExit_notEnd());
    JStudio::TObject_sound* soundObj =
        JStudio::TCreateObject::createFromAdaptor<JStudio_JAudio2::TAdaptor_sound>(param_1,
                                                                                   adaptor);
    if (soundObj == NULL) {
        return NULL;
    }
    deleteGuard.set(NULL);
    return soundObj;
}
