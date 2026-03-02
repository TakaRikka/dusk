#include <revolution/wenc.h>
#include <cstring>

typedef struct {
    s32 nXN;   // size 0x04, offset 0x00
    s32 nDL;   // size 0x04, offset 0x04
    s32 nQN;   // size 0x04, offset 0x08
    s32 nDN;   // size 0x04, offset 0x0c
    s32 nDLH;  // size 0x04, offset 0x10
    s32 nDLQ;  // size 0x04, offset 0x14
    u8 padding[8];
} WENCBlock;

s32 WENCGetEncodeData(WENCInfo* info, u32 flag, s16 const* pbyPcmData, s32 nSampleNum, u8* pbyAdpcmData) {
    f64 const dTable[2 * 4] = {230.0 / 256.0, 230.0 / 256.0, 230.0 / 256.0, 230.0 / 256.0,
                               307.0 / 256.0, 409.0 / 256.0, 512.0 / 256.0, 614.0 / 256.0};

    u8 by;
    u8* pDst;
    s16 const* pSrc;
    int ii;
    s32 nDA;
    s32 nL3;
    s32 nL2;
    s32 nL1;
    s32 nL0;
    s32 nDLX;
    s32 nXNC;
    s32 nOffset;
    s32 nIndex;
    s32 nEncodeSize;
    s32 nXN;
    s32 nDL;
    s32 nQN;
    s32 nDN;
    s32 nDLH;
    s32 nDLQ;
    WENCBlock* block;

    block = (WENCBlock*)info;
    nEncodeSize = (nSampleNum + 1) / 2;

    memset(pbyAdpcmData, 0, nEncodeSize);

    pSrc = pbyPcmData;
    pDst = pbyAdpcmData;

    if (!(flag & 1)) {
        nXN = 0;
        nDL = 127;
        nQN = 0;
        nDN = 0;
        nDLH = 0;
        nDLQ = 0;
    } else {
        nXN = block->nXN;
        nDL = block->nDL;
        nQN = block->nQN;
        nDN = block->nDN;
        nDLH = block->nDLH;
        nDLQ = block->nDLQ;
    }

    for (ii = 0; ii < nSampleNum; ii++) {
        nL3 = nL2 = nL1 = nL0 = 0;

        nDA = *pSrc++;
        if (nDA < nXN)
            nL3 = 1;

        nDN = __abs(nDA - nXN);
        if (nDN >= nDL) {
            nL2 = 1;
            nDN -= nDL;
        }

        nDLH = nDL / 2;
        if (nDN >= nDLH) {
            nL1 = 1;
            nDN -= nDLH;
        }

        nDLQ = nDLH / 2;
        if (nDN >= nDLQ) {
            nL0 = 1;
            nDN -= nDLQ;
        }

        nDLX = nDLQ / 2;

        nQN = (1 - nL3 * 2) * (nDL * nL2 + nDLH * nL1 + nDLQ * nL0 + nDLX);

        if (nQN > 0xffff)
            nQN = 0xffff;

        if (nQN < -0x10000)
            nQN = -0x10000;

        nXNC = nXN + nQN;

        if (nXNC > 0x7fff)
            nXNC = 0x7fff;

        if (nXNC < -0x8000)
            nXNC = -0x8000;

        nXN = nXNC;
        nOffset = (ii & 1) == 0 ? 4 : 0;

        by = nL3 * 8 + nL2 * 4 + nL1 * 2 + nL0;
        pDst[ii / 2] |= by << nOffset;

        nIndex = nL2 * 4 + nL1 * 2 + nL0;
        nDL = nDL * dTable[nIndex];

        if (nDL <= 0x007f)
            nDL = 0x007f;

        if (nDL >= 0x6000)
            nDL = 0x6000;
    }

    block->nXN = nXN;
    block->nDL = nDL;
    block->nQN = nQN;
    block->nDN = nDN;
    block->nDLH = nDLH;
    block->nDLQ = nDLQ;

    return nSampleNum;
}
