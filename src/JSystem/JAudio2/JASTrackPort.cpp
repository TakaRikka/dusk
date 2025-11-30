#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASTrackPort.h"

void JASTrackPort::init() {
    for (int i = 0; i < 16; i++) {
        field_0x4[i] = 0;
    }
    field_0x0 = 0;
    field_0x2 = 0;
}

u16 JASTrackPort::readImport(u32 port_num) {
    field_0x0 = field_0x0 & ~(1 << port_num);
    return field_0x4[port_num];
}

u16 JASTrackPort::readExport(u32 port_num) {
    field_0x2 = field_0x2 & ~(1 << port_num);
    return field_0x4[port_num];
}

void JASTrackPort::writeImport(u32 port_num, u16 param_1) {
    field_0x0 = field_0x0 | (1 << port_num);
    field_0x4[port_num] = param_1;
}

void JASTrackPort::writeExport(u32 port_num, u16 param_1) {
    field_0x2 = field_0x2 | (1 << port_num);
    field_0x4[port_num] = param_1;
}

u32 JASTrackPort::checkImport(u32 param_0) const {
    return field_0x0 & 1 << param_0 ? 1 : 0;
}

u32 JASTrackPort::checkExport(u32 param_0) const {
    return field_0x2 & 1 << param_0 ? 1 : 0;
}
