#ifndef JSTUDIOCAMERAEDITOR_SEQUENCE_H
#define JSTUDIOCAMERAEDITOR_SEQUENCE_H

#include "JSystem/JStudio/JStudioCameraEditor/csb.h"

namespace JStudioCameraEditor {
    struct TSequenceContainer : public csb::TSequenceContainer {
        TSequenceContainer();

        int GetSequenceValueSetDefaultNumber();

        virtual ~TSequenceContainer();
        virtual csb::TSequence* Do_create();
        virtual void Do_destroy(csb::TSequence*);

        /* 0x10 */ int field_0x10;
    };
}

#endif
