#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JHIComm.h"

static int min(int a, int b);

void JHICommBufHeader::init() {
    mp_memBuffer->writeIO(field_0x4 + field_0x1c, 0);
    mp_memBuffer->writeIO(field_0x4 + field_0x18, 0);
    mp_memBuffer->writeIO(field_0x4 + field_0x14, 'IS64');
}

void JHICommBufHeader::init(JHICommonMem* buffer, u32 param_1, u32 param_2, u32 alignment) {
    m_alignment = alignment;

    if (alignment == 4) {
        field_0x14 = 0x00;
        field_0x18 = 0x08;
        field_0x1c = 0x14;
        field_0x20 = 0x18;
        field_0x24 = 0x20;
        field_0x2c = ~0x3;
    } else if (alignment == 32) {
        field_0x14 = 0x00;
        field_0x18 = 0x20;
        field_0x1c = 0x40;
        field_0x20 = 0x60;
        field_0x24 = 0x80;
        field_0x2c = ~0x1F;
    }

    mp_memBuffer = buffer;
    field_0x4 = field_0x2c & (param_1 + m_alignment - 1);

    u32 var_r30 = field_0x2c & (param_1 + param_2);
    m_msgBufSize = var_r30 - field_0x4 - field_0x24;

    init();
}

int JHICommBufHeader::load() {
    u32 data;
    mp_memBuffer->readIO(field_0x4 + field_0x14, &data);
    if (data != 'IS64') {
        return 1;
    }

    mp_memBuffer->readIO(field_0x4 + field_0x18, &field_0xc);
    mp_memBuffer->readIO(field_0x4 + field_0x1c, &field_0x10);
    return 0;
}

JHICommBufReader::JHICommBufReader(u32 param_0, u32 param_1, u32 alignment) {
    mp_memBuffer = JHICommonMem::Instance();
    m_header.init(mp_memBuffer, param_0, param_1, alignment);
}

void JHICommBufReader::Header::updateGetAdrs() {
    if (field_0xc != field_0x30) {
        mp_memBuffer->writeIO(field_0x4 + field_0x18, field_0xc);
    }
}

u32 JHICommBufReader::Header::getReadableSize() const {
    u32 size;
    if (field_0xc <= field_0x10) {
        size = field_0x10 - field_0xc;
    } else {
        size = m_msgBufSize - (field_0xc - field_0x10);
    }

    return size;
}

int JHICommBufReader::readBegin() {
    return m_header.load();
}

void JHICommBufReader::readEnd() {
    m_header.updateGetAdrs();
}

int JHICommBufReader::read(void* param_0, int param_1) {
    int sp2C = min(param_1, m_header.getReadableSize());
    u32 sp28;
    int var_r28 = sp2C;
    u8* var_r30 = (u8*)param_0;

    int sp24;
    if (var_r28 > 0 && (sp24 = m_header.getContSize()) > 0) {
        int sp20 = min(var_r28, 4 - sp24);
        m_header.alignGetAdrs();

        mp_memBuffer->readIO(m_header.getGetAdrs(), &sp28);
        
        int sp1C = sp24;
        int sp18 = sp24 + sp20;
        for (; sp1C < sp18; sp1C++) {
            *var_r30 = sp28 >> ((3 - sp1C) * 8);
            var_r30++;
        }

        var_r28 -= sp20;
        m_header.addGetAdrs(sp24 + sp20);
    }

    while (var_r28 >= 4) {
        mp_memBuffer->readIO(m_header.getGetAdrs(), &sp28);
        var_r30[0] = sp28 >> 0x18;
        var_r30[1] = sp28 >> 0x10;
        var_r30[2] = sp28 >> 0x08;
        var_r30[3] = sp28 >> 0x00;

        var_r30 += 4;
        var_r28 -= 4;

        m_header.addGetAdrs(4);
    }

    if (var_r28 > 0) {
        int sp14 = var_r28;
        mp_memBuffer->readIO(m_header.getGetAdrs(), &sp28);

        for (int i = 0; i < sp14; i++) {
            *var_r30 = sp28 >> ((3 - i) * 8);
            var_r30++;
        }

        var_r28 -= sp14;
        m_header.addGetAdrs(sp14);
    }

    return sp2C;
}

void JHICommBufReader::Header::addGetAdrs(int param_0) {
    field_0xc += param_0;
    if (field_0xc >= m_msgBufSize) {
        field_0xc -= m_msgBufSize;
    }
}

u32 JHICommBufReader::Header::getGetAdrs() const {
    return field_0xc + (field_0x4 + field_0x24);
}

void JHICommBufReader::Header::alignGetAdrs() {
    field_0xc &= ~0x3;
}

int JHICommBufReader::Header::getContSize() {
    return field_0xc & 3;
}

static int min(int a, int b) {
    return a < b ? a : b;
}

JHICommBufWriter::JHICommBufWriter(u32 param_0, u32 param_1, u32 param_2) {
    mp_memBuffer = JHICommonMem::Instance();
    m_header.init(mp_memBuffer, param_0, param_1, param_2);
}

int JHICommBufWriter::Header::load() {
    int var_r29 = JHICommBufHeader::load();
    if (var_r29 == 0) {
        field_0x34 = field_0x10;

        if (getRemSize() > 0) {
            u32 sp8;
            mp_memBuffer->readIO(field_0x4 + field_0x20, &sp8);

            field_0x30 = sp8 >> 0x18;
            field_0x31 = sp8 >> 0x10;
            field_0x32 = sp8 >> 0x08;
            field_0x33 = sp8 >> 0x00;
        }
    }

    return var_r29;
}

int JHICommBufWriter::Header::getRemSize() {
    return field_0x10 & 3;
}

void JHICommBufWriter::Header::updatePutAdrs() {
    if (field_0x10 != field_0x34) {
        mp_memBuffer->writeIO(field_0x4 + field_0x1c, field_0x10);

        if (getRemSize() > 0) {
            mp_memBuffer->writeIO(field_0x4 + field_0x20,
                               (field_0x30 << 0x18) |
                                    (field_0x31 << 0x10) |
                                    (field_0x32 << 0x08) |
                                    (field_0x33 << 0x00));
        }
    }
}

u32 JHICommBufWriter::Header::getWritebleSize() const {
    u32 var_r31 = field_0xc & ~0x3;
    u32 var_r30;

    if (field_0x10 < var_r31) {
        var_r30 = var_r31 - field_0x10;
    } else {
        var_r30 = var_r31 + (m_msgBufSize - field_0x10);
    }

    return var_r30 - 4;
}

int JHICommBufWriter::writeBegin() {
    int var_r31;
    if (m_header.load() != 0) {
        var_r31 = -1;
    } else {
        var_r31 = m_header.getWritebleSize();
    }

    return var_r31;
}

void JHICommBufWriter::writeEnd() {
    m_header.updatePutAdrs();
}

int JHICommBufWriter::write(void* param_0, int param_1) {
    int sp28 = min(param_1, m_header.getWritebleSize());
    int var_r27 = sp28;
    u8* var_r29 = (u8*)param_0;
    u8* var_r30 = &m_header.field_0x30;

    int sp24;
    if (var_r27 > 0 && (sp24 = m_header.getRemSize()) > 0) {
        int sp20 = min(4 - sp24, var_r27);
        
        int sp1C = sp24;
        int sp18 = sp24 + sp20;
        for (; sp1C < sp18; sp1C++) {
            var_r30[sp1C] = *var_r29++;
        }

        var_r27 -= sp20;
        m_header.alignPutAdrs();

        mp_memBuffer->writeIO(m_header.getPutAdrs(),
                            (var_r30[0] << 0x18) |
                            (var_r30[1] << 0x10) |
                            (var_r30[2] << 0x08) |
                            (var_r30[3] << 0x00));
        m_header.addPutAdrs(sp24 + sp20);
    }

    while (var_r27 >= 4) {
        mp_memBuffer->writeIO(m_header.getPutAdrs(),
                            (var_r29[0] << 0x18) |
                            (var_r29[1] << 0x10) |
                            (var_r29[2] << 0x08) |
                            (var_r29[3] << 0x00));

        var_r29 += 4;
        var_r27 -= 4;

        m_header.addPutAdrs(4);
    }

    if (var_r27 > 0) {
        int sp14 = var_r27;

        for (int i = 0; i < sp14; i++) {
            var_r30[i] = *var_r29++;
        }

        mp_memBuffer->writeIO(m_header.getPutAdrs(),
                            (var_r30[0] << 0x18) |
                            (var_r30[1] << 0x10) |
                            (var_r30[2] << 0x08) |
                            (var_r30[3] << 0x00));

        var_r27 -= sp14;
        m_header.addPutAdrs(sp14);
    }

    return sp28;
}

void JHICommBufWriter::Header::addPutAdrs(int param_0) {
    field_0x10 += param_0;
    if (field_0x10 >= m_msgBufSize) {
        field_0x10 -= m_msgBufSize;
    }
}

u32 JHICommBufWriter::Header::getPutAdrs() const {
    return field_0x10 + (field_0x4 + field_0x24);
}

void JHICommBufWriter::Header::alignPutAdrs() {
    field_0x10 &= ~0x3;
}
