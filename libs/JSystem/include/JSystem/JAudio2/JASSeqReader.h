#ifndef JASSEQREADER_H
#define JASSEQREADER_H

#include <types.h>

#define JAS_SEQ_STACK_SIZE 8

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASSeqReader {
public:
    JASSeqReader() { init(); }
    void init();
    void init(void*);
    bool call(u32);
    bool loopStart(u32);
    bool loopEnd();
    bool ret();
    int readMidiValue();

    void jump(u32 param_1) {
        mCurPos = mBase + param_1;
    }

    void jump(void* param_1) {
        mCurPos = (u8*)param_1;
    }

    u32 get24(u32 param_0) const {
        return (*(BE(u32)*)(mBase + param_0 - 1)) & 0xffffff;
    }

    u32* getBase() { return (u32*)mBase; }
    void* getAddr(u32 param_0) { return mBase + param_0; }
    u8 getByte(u32 param_0) const { return *(mBase + param_0); }
    u16 get16(u32 param_0) const { return *(BE(u16)*)(mBase + param_0); }
    u32 get32(u32 param_0) const { return *(BE(u32)*)(mBase + param_0); }
    u8* getCur() { return mCurPos; }
    u32 readByte() { return *mCurPos++; }
    u32 read16() {
#ifdef __MWERKS__
        return *((u16*)mCurPos)++;
#else
        BE(u16)* value = (BE(u16)*)mCurPos;
        mCurPos += 2;
        return *value;
#endif
    }
    u32 read24() {
        mCurPos--;
#ifdef __MWERKS__
        return (*((u32*)mCurPos)++) & 0x00ffffff;
#else
        BE(u32)* value = (BE(u32)*)mCurPos;
        mCurPos += 4;
        return (*value) & 0x00ffffff;
#endif
    }
    u16 getLoopCount() const { 
        if (mCurStackDepth == 0) {
            return 0;
         } 
         return mLoopCount[mCurStackDepth - 1];
    }

    /* 0x00 */ u8* mBase;
    /* 0x04 */ u8* mCurPos;
    /* 0x08 */ u32 mCurStackDepth;
    /* 0x0C */ u16* mReturnAddr[JAS_SEQ_STACK_SIZE];
    /* 0x2C */ u16 mLoopCount[JAS_SEQ_STACK_SIZE];
};

#endif /* JASSEQREADER_H */
