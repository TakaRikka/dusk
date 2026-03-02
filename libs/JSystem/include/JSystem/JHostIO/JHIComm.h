#ifndef JHICOMM_H
#define JHICOMM_H

#include "JSystem/JHostIO/JHICommonMem.h"

class JHICommBufHeader {
public:
    void init();
    void init(JHICommonMem*, u32, u32, u32);
    int load();

    u32 getMsgBufSize() { return m_msgBufSize; }

    /* 0x00 */ JHICommonMem* mp_memBuffer;
    /* 0x04 */ u32 field_0x4;
    /* 0x08 */ u32 m_msgBufSize;
    /* 0x0C */ u32 field_0xc;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u32 field_0x14;
    /* 0x18 */ u32 field_0x18;
    /* 0x1C */ u32 field_0x1c;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ u32 field_0x24;
    /* 0x28 */ u32 m_alignment;
    /* 0x2C */ u32 field_0x2c;
};

class JHICommBufWriter {
public:
    struct Header : public JHICommBufHeader {
        int load();
        int getRemSize();
        void updatePutAdrs();
        u32 getWritebleSize() const;
        void alignPutAdrs();
        void addPutAdrs(int);
        u32 getPutAdrs() const;

        /* 0x30 */ u8 field_0x30;
        /* 0x31 */ u8 field_0x31;
        /* 0x32 */ u8 field_0x32;
        /* 0x33 */ u8 field_0x33;
        /* 0x34 */ u32 field_0x34;
    };

    JHICommBufWriter(u32, u32, u32 alignment);
    int writeBegin();
    void writeEnd();
    int write(void*, int);

    void init() { m_header.init(); }

    /* 0x00 */ Header m_header;
    /* 0x38 */ JHIMemBuf* mp_memBuffer;
};

class JHICommBufReader {
public:
    struct Header : public JHICommBufHeader {
        void updateGetAdrs();
        u32 getReadableSize() const;
        void addGetAdrs(int);
        u32 getGetAdrs() const;
        void alignGetAdrs();
        int getContSize();

        int load() {
            int result = JHICommBufHeader::load();
            if (!result) {
                field_0x30 = field_0xc;
            }
            return result;
        }

        /* 0x30 */ u32 field_0x30;
    };

    JHICommBufReader(u32, u32, u32 alignment);
    int readBegin();
    void readEnd();
    int read(void*, int);

    u32 getMsgBufSize() { return m_header.getMsgBufSize(); }
    void init() { m_header.init(); }

    u32 available() {
        if (m_header.load()) {
            return 0xFFFFFFFF;
        } else {
            return m_header.getReadableSize();
        }
    }

    /* 0x00 */ Header m_header;
    /* 0x34 */ JHIMemBuf* mp_memBuffer;
};

struct JHICmnMem {
    u32 sendBegin() { return mp_writeBuf->writeBegin(); }
    int sendCont(const void* param_0, int param_1) { return mp_writeBuf->write((void*)param_0, param_1); }
    void sendEnd() { mp_writeBuf->writeEnd(); }
    u32 available() { return mp_readBuf->available(); }
    u32 getMaxReaderableSize() { return mp_readBuf->getMsgBufSize(); }

    void setBuf(JHICommBufReader* pReader, JHICommBufWriter* pWriter) {
        mp_readBuf = pReader;
        mp_writeBuf = pWriter;
    }

    void initBuffers() {
        mp_readBuf->init();
        mp_writeBuf->init();
    }

    int receive(void* param_1, int param_2) {
        int result = 0;
        result = mp_readBuf->read(param_1, param_2);
        mp_readBuf->readEnd();
        return result;
    }

    /* 0x0 */ JHICommBufReader* mp_readBuf;
    /* 0x4 */ JHICommBufWriter* mp_writeBuf;
    /* 0x8 */ u8 field_0x8;
};

struct JHIContext {
    JHICommBufReader* mp_reader;
    JHICommBufWriter* mp_writer;
};

#endif /* JHICOMM_H */
