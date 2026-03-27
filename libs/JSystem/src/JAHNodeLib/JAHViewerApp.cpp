
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JSupport/JSUMemoryStream.h"
#include "JSystem/JSupport/JSUOutputStream.h"

static void dummy(JSUMemoryInputStream& in, JSUOutputStream& out, JAHControl& ctrl) {
    out << s16(0);
    s16 b;
    in >> b;
    ctrl.makeSlider("", (u8*)NULL, (u8)0, (u8)0, 0);
    u16 a;
    in >> a;
    in.getPointer();
}
