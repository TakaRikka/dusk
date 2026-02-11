#ifndef JSTUDIOCAMERAEDITOR_CONTROLSET_CSB_VALUESET_H
#define JSTUDIOCAMERAEDITOR_CONTROLSET_CSB_VALUESET_H

#include "JSystem/JStudio/JStudioToolLibrary/controlset.h"
#include "JSystem/JStudio/JStudioCameraEditor/csb.h"

namespace JStudioCameraEditor {
    struct TControlSet_csb_valueSet : public JStudioToolLibrary::TControlSet {
        /* 0x14 */ bool bChanged_;
        /* 0x15 */ u8 field_0x15;
        /* 0x18 */ csb::TValueSet* pValueSet_;
        /* 0x1C */ csb::TValueSet field_0x1c;
    };
}

#endif
