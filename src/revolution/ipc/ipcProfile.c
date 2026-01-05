#include <revolution.h>
#include <cstring>

static u32 IpcReqPtrArray[96] = {0};
static u32 IpcReqArray[0x300] = {0};
static OSTime IpcStartTimeArray[96];
static u8 IpcHandlePathBuf[0x1800];
static u8 IpcOpenPathBuf[0x1200];

static u32 IpcNumPendingReqs = 0;
static u32 IpcNumUnIssuedReqs = 0;

static void AddReqInfo(void* req);
static void DelReqInfo(void* req);

void IPCiProfInit(void) {
    u32 i;

    IpcNumPendingReqs = 0;
    IpcNumUnIssuedReqs = 0;

    for (i = 0; i < 96; ++i) {
        IpcReqPtrArray[i] = 0;
        IpcStartTimeArray[i] = 0;
    }

    memset(IpcHandlePathBuf, 0, sizeof(IpcHandlePathBuf));
    memset(IpcOpenPathBuf, 0, sizeof(IpcOpenPathBuf));
    memset(IpcReqArray, 0, sizeof(IpcReqArray));
}

void IPCiProfQueueReq(void* req, s32 handle) {
    ++IpcNumPendingReqs;
    ++IpcNumUnIssuedReqs;
    AddReqInfo(req);
}

void IPCiProfAck(void) {
    --IpcNumUnIssuedReqs;
}

void IPCiProfReply(void* req, s32 handle) {
    --IpcNumPendingReqs;
    DelReqInfo(req);
}

static void AddReqInfo(void* ptr) {
    // NONMATCHING
}

static void DelReqInfo(void* ptr) {
    // NONMATCHING
}
