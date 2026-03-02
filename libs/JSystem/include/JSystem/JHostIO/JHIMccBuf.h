#ifndef JORMCCBUF_H_H
#define JORMCCBUF_H_H

#include "JSystem/JHostIO/JHIComm.h"
#include "JSystem/JKernel/JKRHeap.h"

class JHIMccBuf {
public:
    JHIMccBuf(u16 channel, u16, u32);

    void setHeaderToBuf(u32 data, u32 position, u16 size);
    u32 setDataToBuf(void* pData, u16 size);
    int readData(u32 position, u32* pOutData);
    int readDataFromBuf(void* pBuf, u32 size);

    int JHIRead(u32 channel, u32 offset, void* buffer, u32 size);
    int JHIWrite(u32 channel, u32 offset, void* buffer, u32 size);

    int JHIMCCWrite(u32 channel, u32 offset, void* buffer, u32 size);
    int JHIMCCRead(u32 channel, u32 offset, void* buffer, u32 size);

    virtual ~JHIMccBuf();
    virtual void initInstance(u16 channel, u16, u32);
    virtual void init();
    virtual void initBuf();
    virtual void enablePort() { mPortEnabled = true; }
    virtual void disablePort() { mPortEnabled = false; }
    virtual bool isPort() { return mPortEnabled; }

    static u8* mTempBuf;
    static u16 mRefCount;

    /* 0x04 */ u32 mTag;
    /* 0x08 */ u16 field_0x8;
    /* 0x0A */ u16 field_0xa;
    /* 0x0C */ u16 field_0xc;
    /* 0x0E */ u16 mChannel;
    /* 0x10 */ u16 field_0x10;
    /* 0x14 */ int mReadOffset;
    /* 0x18 */ int mWriteOffset;
    /* 0x1C */ int field_0x1c;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ u32 mBeginPos;
    /* 0x28 */ u32 mEndPos;
    /* 0x2C */ bool mPortEnabled;
};

class JHIMccBufReader : public JHIMccBuf {
public:
    JHIMccBufReader(u16 channel, u16, u32);
    virtual ~JHIMccBufReader();

    u32 available();
    int readBegin();
    int read(void* pBuf);
    void readEnd();
};

class JHIMccBufWriter : public JHIMccBuf {
public:
    JHIMccBufWriter(u16 channel, u16, u32);
    virtual ~JHIMccBufWriter();

    int writeBegin();
    int write(void* pBuf, u32 size);
    void writeEnd();
};

struct JHIMccContext {
    JHIMccBufReader* mp_reader;
    JHIMccBufWriter* mp_writer;
};

void JHIReport(const char* fmt, ...);
void JHIHalt(const char* fmt, ...);

#endif /* JORMCCBUF_H_H */
