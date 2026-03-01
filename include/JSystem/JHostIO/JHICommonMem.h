#ifndef JHICOMMONMEM_H
#define JHICOMMONMEM_H

#include <dolphin/types.h>
#include "dusk/endian.h"

inline u32 JHIhtonl(u32 v) {
    return BSWAP32(v);
}

inline u32 JHIntohl(u32 v) {
    return BSWAP32(v);
}

template<typename T> class JHIComPortManager;

template <typename T>
struct JHITag {
    JHITag(u32 tag) {
        m_tag = tag;
        mp_data = NULL;
    }

    s32 send(const void* param_0, s32 param_1) {
        u32 sp10[2];
        sp10[0] = JHIhtonl(m_tag);
        sp10[1] = JHIhtonl(param_1);

        if (mp_data->port.sendBegin() >= param_1 + 8) {
            mp_data->port.sendCont(sp10, 8);
            mp_data->port.sendCont(param_0, param_1);
            mp_data->port.sendEnd();
        }

        return param_1;
    }

    virtual ~JHITag() {}
    virtual void receive(const char*, s32) {}

    /* 0x4 */ u32 m_tag;
    /* 0x8 */ JHIComPortManager<T>* mp_data;
};

class JHIMemBuf;

class JHICommonMem {
public:
    virtual ~JHICommonMem() {}
    virtual int create() = 0;
    virtual int open() = 0;
    virtual void close() = 0;
    virtual u8* getPointer() const = 0;
    virtual void readIO(u32 position, u32* out_data) const = 0;
    virtual u32 readIO(u32 position) const = 0;
    virtual void writeIO(u32 position, u32 data) const = 0;
    virtual void writeIO(u32 position, u8* src_data, u32 length) const = 0;

    static JHIMemBuf* Instance();
    static JHIMemBuf* instance;
};

class JHIMemBuf : public JHICommonMem {
public:
    JHIMemBuf();

    virtual ~JHIMemBuf();
    virtual int create();
    virtual int open();
    virtual void close();
    virtual u8* getPointer() const;
    virtual void readIO(u32 position, u32* out_data) const;
    virtual u32 readIO(u32 position) const;
    virtual void writeIO(u32 position, u32 data) const;
    virtual void writeIO(u32 position, u8* src_data, u32 length) const;

    /* 0x4 */ u8* mp_buffer;
};

#endif /* JHICOMMONMEM_H */
