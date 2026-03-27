#include "JSystem/JAHostIO/JAHUGlobal.h"

static void dummy(JSUInputStream& input) {
    JSUMemoryOutputStream stream(NULL, 0);
    stream.getPosition();
    stream << u8(0);
    u8 b;
    input >> b;
    u32 a;
    input >> a;
    JAHUDialog_OK("", "", "");
}
