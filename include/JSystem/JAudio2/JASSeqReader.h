#ifndef JASSEQREADER_H
#define JASSEQREADER_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASSeqReader {
public:
    void init();
    void init(void*);
    bool call(u32);
    bool loopStart(u32);
    bool loopEnd();
    bool ret();
    int readMidiValue();

    void jump(u32 param_1) {
        field_0x04 = field_0x00 + param_1;
    }

    void jump(void* param_1) {
        field_0x04 = (u8*)param_1;
    }

    u32 get24(int param_0) const {
        return (*(u32*)(field_0x00 + param_0 - 1)) & 0xffffff;
    }

    u32* getBase() { return (u32*)field_0x00; }
    void* getAddr(u32 param_0) { return field_0x00 + param_0; }
    u8 getByte(u32 param_0) const { return *(field_0x00 + param_0); }
    u16 get16(u32 param_0) const { return *(u16*)(field_0x00 + param_0); }
    u32 get32(u32 param_0) const { return *(u32*)(field_0x00 + param_0); }
    u8* getCur() { return field_0x04; }
    u32 readByte() { return *field_0x04++; }
    u32 read16() {
        return *((u16*)field_0x04)++;
    }
    u32 read24() {
        field_0x04--;
        return (*((u32*)field_0x04)++) & 0x00ffffff;
    }
    u16 getLoopCount() const { 
        if (field_0x08 == 0) {
            return 0;
         } 
         return field_0x2c[field_0x08 - 1]; 
    }

    /* 0x00 */ u8* field_0x00;
    /* 0x04 */ u8* field_0x04;
    /* 0x08 */ u32 field_0x08;
    /* 0x0C */ u16* field_0x0c[8];
    /* 0x2C */ u16 field_0x2c[8];
};

#endif /* JASSEQREADER_H */
