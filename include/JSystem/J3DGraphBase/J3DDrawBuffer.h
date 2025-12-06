#ifndef J3DDRAWBUFFER_H
#define J3DDRAWBUFFER_H

#include "JSystem/J3DGraphBase/J3DSys.h"

// matches debug
inline f32 J3DCalcZValue(__REGISTER MtxP m, __REGISTER Vec v) {
#ifdef __MWERKS__
    __REGISTER f32 temp_f4;
    __REGISTER f32 out;
    __REGISTER f32 temp_f0;
    __REGISTER f32 temp_f2;
    __REGISTER f32 temp_f1 = 1.0f;

    // clang-format off
    asm {
        psq_l temp_f0, 0(v), 0, 0 /* qr0 */
        lfs temp_f2, 8(v)
        psq_l temp_f4, 32(m), 0, 0 /* qr0 */
        psq_l out, 40(m), 0, 0 /* qr0 */
        ps_merge00 temp_f2, temp_f2, temp_f1
        ps_mul temp_f4, temp_f0, temp_f4
        ps_madd out, temp_f2, out, temp_f4
        ps_sum0 out, out, out, out
    }
    // clang-format on

    return out;
#endif
}

class J3DDrawBuffer;
class J3DPacket;
class J3DDrawPacket;
class J3DMatPacket;
class J3DShapePacket;

enum J3DDrawBufDrawMode {
    J3DDrawBufDrawMode_Head,
    J3DDrawBufDrawMode_Tail,

    J3DDrawBufDrawMode_MAX,
};

enum J3DDrawBufSortMode {
    J3DDrawBufSortMode_Mat,
    J3DDrawBufSortMode_MatAnm,
    J3DDrawBufSortMode_Z,
    J3DDrawBufSortMode_Model,
    J3DDrawBufSortMode_Invalid,
    J3DDrawBufSortMode_Non,

    J3DDrawBufSortMode_MAX,
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DDrawBuffer {
public:
    typedef int (J3DDrawBuffer::*sortFunc)(J3DMatPacket*);
    typedef void (J3DDrawBuffer::*drawFunc)() const;

    J3DDrawBuffer() { initialize(); }
    ~J3DDrawBuffer();
    void initialize();
    int allocBuffer(u32);
    void frameInit();
    int entryMatSort(J3DMatPacket*);
    int entryMatAnmSort(J3DMatPacket*);
    int entryZSort(J3DMatPacket*);
    int entryModelSort(J3DMatPacket*);
    int entryInvalidSort(J3DMatPacket*);
    int entryNonSort(J3DMatPacket*);
    int entryImm(J3DPacket* pPacket, u16 index);
    void draw() const;
    void drawHead() const;
    void drawTail() const;

    u32 getEntryTableSize() { return mEntryTableSize; }
    int getSortMode() { return mSortMode; }

    inline void calcZRatio();
    void setNonSort() { mSortMode = J3DDrawBufSortMode_Non; }
    void setZSort() { mSortMode = J3DDrawBufSortMode_Z; }
    void setZMtx(MtxP mtx) { mpZMtx = mtx; }

public:
    /* 0x00 */ J3DPacket** mpBuffer;
    /* 0x04 */ u32 mEntryTableSize;
    /* 0x08 */ u32 mDrawMode;
    /* 0x0C */ u32 mSortMode;
    /* 0x10 */ f32 mZNear;
    /* 0x14 */ f32 mZFar;
    /* 0x18 */ f32 mZRatio;
    /* 0x1C */ MtxP mpZMtx;
    /* 0x20 */ J3DPacket* mpCallBackPacket;

    static sortFunc sortFuncTable[6];
    static drawFunc drawFuncTable[2];
    static int entryNum;
};

#endif /* J3DDRAWBUFFER_H */
