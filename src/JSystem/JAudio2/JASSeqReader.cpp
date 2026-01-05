/**
 * JASSeqReader.cpp
 * 
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASSeqReader.h"

void JASSeqReader::init() {
    field_0x00 = 0;
    field_0x04 = 0;
    field_0x08 = 0;

    for (int i = 0; i < 8; i++) {
        field_0x0c[i] = NULL;
        field_0x2c[i] = 0;
    }
}

void JASSeqReader::init(void* param_0) {
    field_0x00 = (u8*)param_0;
    field_0x04 = field_0x00;
    field_0x08 = 0;

    for (int i = 0; i < 8; i++) {
        field_0x0c[i] = NULL;
        field_0x2c[i] = 0;
    }
    
}

bool JASSeqReader::call(u32 param_0) {
    if (field_0x08 >= 8) {
        JUT_WARN(42, "%s", "Cannot exec call command");
        return false;
    }

    field_0x0c[field_0x08++] = (u16*)field_0x04;
    field_0x04 = field_0x00 + param_0;

    return true;
}

bool JASSeqReader::loopStart(u32 param_0) {
    if (8 <= field_0x08) {
        JUT_WARN(53, "%s", "Cannot exec loopStart command");
        return false;
    }

    field_0x0c[field_0x08] = (u16*)field_0x04;
    field_0x2c[field_0x08++] = param_0;

    return true;
}


bool JASSeqReader::loopEnd() {
    if (field_0x08 == 0) {
        JUT_WARN(65, "%s", "cannot loopE for call-stack is NULL");
        return false;
    }

    u16 tmp = field_0x2c[field_0x08 - 1];

    if (tmp != 0) {
        tmp--;
    }

    if (!tmp) {
        field_0x08--;
        return true;
    }

    field_0x2c[field_0x08 - 1] = tmp;
    field_0x04 = (u8*)field_0x0c[field_0x08 - 1];
    return true;
}

bool JASSeqReader::ret() {
    if (field_0x08 == 0) {
        return false;
    }

    field_0x04 = (u8*)field_0x0c[--field_0x08];

    return true;
}

int JASSeqReader::readMidiValue() {
    int byte = readByte();

    if (!(byte & 0x80)) {
        return byte;
    }

    byte &= 0x7f;

    int i = 0;

    while (true) {
        if (2 < i) {
            JUT_WARN(100, "%s", "readMidiValue: Too large value");
            return 0;
        }

        u8 newByte = readByte();
        
        byte = byte << 7;
        byte |= newByte & 0x7f;

        if (!(newByte & 0x80)) {
            break;
        }

        i++;
    }

    return byte;
}
