#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JHICommonMem.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <dolphin/dolphin.h>

int JHIMemBuf::create() {
    int rt = 1;
    if (mp_buffer == NULL) {
        mp_buffer = new (32) u8[0x20000];

        if (mp_buffer == NULL) {
            rt = 0;
            OS_REPORT("ERROR: hioSync Alloc Mem NG!\n");
        } else {
            OS_REPORT("INFO: hioSync Alloc Mem OK! %08x\n", mp_buffer);
        }
    }

    return rt;
}

int JHIMemBuf::open() {
    return 1;
}

void JHIMemBuf::close() {
    if (mp_buffer != NULL) {
        delete[] mp_buffer;
    }
}

JHIMemBuf::~JHIMemBuf() {
    close();
}

u8* JHIMemBuf::getPointer() const {
    return mp_buffer;
}

u32 JHIMemBuf::readIO(u32 position) const {
    u32 data;
    readIO(position, &data);
    return data;
}

void JHIMemBuf::readIO(u32 position, u32* out_data) const {
    u8* read_ptr = getPointer() + position;
    *out_data = (read_ptr[0] << 0x18) |
                (read_ptr[1] << 0x10) |
                (read_ptr[2] << 0x08) |
                (read_ptr[3] << 0x00);
}

void JHIMemBuf::writeIO(u32 position, u8* src_data, u32 length) const {
    u8* write_ptr = getPointer() + position;

    while (--length != 0) {
        *write_ptr = *src_data++;
        write_ptr++;
    }
}

void JHIMemBuf::writeIO(u32 position, u32 data) const {
    u8* write_ptr = getPointer() + position;

    *write_ptr++ = data >> 0x18;
    *write_ptr++ = data >> 0x10;
    *write_ptr++ = data >> 0x08;
    *write_ptr++ = data >> 0x00;
}
