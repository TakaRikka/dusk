#include <dolphin/dolphin.h>
#include <dolphin/mtx.h>
#include "sdk_math.h"

void MTXInitStack(MTXStack* sPtr, u32 numMtx) {
    ASSERTMSGLINE(74, sPtr, "MTXInitStack():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(75, sPtr->stackBase, "MTXInitStack():  'sPtr' contains a NULL ptr to stack memory ");
    ASSERTMSGLINE(76, numMtx, "MTXInitStack():  'numMtx' is 0 ");

    sPtr->numMtx = numMtx;
    sPtr->stackPtr = 0;
}

MtxPtr MTXPush(MTXStack* sPtr, const Mtx m) {
    ASSERTMSGLINE(104, sPtr, "MTXPush():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(105, sPtr->stackBase, "MTXPush():  'sPtr' contains a NULL ptr to stack memory ");
    ASSERTMSGLINE(106, m, "MTXPush():  NULL MtxPtr 'm' ");

    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(m, sPtr->stackPtr);
    } else {
        ASSERTMSGLINE(121, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPush():  stack overflow ");
        MTXCopy(m, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPushFwd(MTXStack* sPtr, const Mtx m) {
    ASSERTMSGLINE(157, sPtr, "MTXPushFwd():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(158, sPtr->stackBase, "MTXPushFwd():  'sPtr' contains a NULL ptr to stack memory ");
    ASSERTMSGLINE(159, m, "MTXPushFwd():  NULL MtxPtr 'm' ");

    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(m, sPtr->stackPtr);
    } else {
        ASSERTMSGLINE(174, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPushFwd():  stack overflow");
        MTXConcat(sPtr->stackPtr, m, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPushInv(MTXStack* sPtr, const Mtx m) {
    Mtx mInv;
    ASSERTMSGLINE(216, sPtr, "MTXPushInv():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(217, sPtr->stackBase, "MTXPushInv():  'sPtr' contains a NULL ptr to stack memory ");
    ASSERTMSGLINE(218, m, "MTXPushInv():  NULL MtxPtr 'm' ");

    MTXInverse(m, mInv);
    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(mInv, sPtr->stackPtr);
    } else {
        ASSERTMSGLINE(236, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPushInv():  stack overflow");
        MTXConcat(mInv, sPtr->stackPtr, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPushInvXpose(MTXStack* sPtr, const Mtx m) {
    Mtx mIT;
    ASSERTMSGLINE(279, sPtr, "MTXPushInvXpose():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(280, sPtr->stackBase, "MTXPushInvXpose():  'sPtr' contains a NULL ptr to stack memory ");
    ASSERTMSGLINE(281, m, "MTXPushInvXpose():  NULL MtxPtr 'm' ");

    MTXInverse(m, mIT);
    MTXTranspose(mIT, mIT);
    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(mIT, sPtr->stackPtr);
    } else {
        ASSERTMSGLINE(300, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPushInvXpose():  stack overflow ");
        MTXConcat(sPtr->stackPtr, mIT, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPop(MTXStack* sPtr) {
    ASSERTMSGLINE(328, sPtr, "MTXPop():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(329, sPtr->stackBase, "MTXPop():  'sPtr' contains a NULL ptr to stack memory ");
    
    if (sPtr->stackPtr == NULL) {
        return NULL;
    }

    if (sPtr->stackBase == sPtr->stackPtr) {
        sPtr->stackPtr = NULL;
        return NULL;
    }

    sPtr->stackPtr -= 3;
    return sPtr->stackPtr;
}

MtxPtr MTXGetStackPtr(const MTXStack* sPtr) {
    ASSERTMSGLINE(366, sPtr, "MTXGetStackPtr():  NULL MtxStackPtr 'sPtr' ");
    ASSERTMSGLINE(367, sPtr->stackBase, "MTXGetStackPtr():  'sPtr' contains a NULL ptr to stack memory ");
    return sPtr->stackPtr;
}
