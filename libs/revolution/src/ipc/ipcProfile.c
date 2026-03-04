#include <revolution.h>
#include <cstring>

#if SDK_AUG2010
#define IPC_REQ_MAX 0x60
#define IPC_PATH_MAX 0x30
#else
#define IPC_REQ_MAX 0x20
#endif

#if SDK_AUG2010
// either release or debug config does something weird to swap the order of these in .bss
#if VERSION == VERSION_SHIELD_DEBUG
static IOSResourceRequest* IpcReqPtrArray[IPC_REQ_MAX] = {0};
static IOSResourceRequest IpcReqArray[IPC_REQ_MAX] = {0};
#else
static IOSResourceRequest IpcReqArray[IPC_REQ_MAX] = {0};
static IOSResourceRequest* IpcReqPtrArray[IPC_REQ_MAX] = {0};
#endif
static OSTime IpcStartTimeArray[IPC_REQ_MAX];
static char IpcHandlePathBuf[0x80][0x30];
static char IpcOpenPathBuf[IPC_REQ_MAX][0x30];
#else
static s32 IpcFdArray[IPC_REQ_MAX] = {0};
static u32 IpcReqPtrArray[IPC_REQ_MAX] = {0};
#endif

static u32 IpcNumPendingReqs = 0;
static u32 IpcNumUnIssuedReqs = 0;

#if SDK_AUG2010
static void AddReqInfo(IOSResourceRequest* req);
static void DelReqInfo(IOSResourceRequest* req);
#else
static void AddReqInfo(IOSResourceRequest* req, s32 fd);
static void DelReqInfo(IOSResourceRequest* req, s32 fd);
#endif

void IPCiProfInit(void) {
    u32 i;

    IpcNumPendingReqs = 0;
    IpcNumUnIssuedReqs = 0;

    for (i = 0; i < IPC_REQ_MAX; ++i) {
#if SDK_AUG2010
        IpcReqPtrArray[i] = 0;
        IpcStartTimeArray[i] = 0;
#else
        IpcReqPtrArray[i] = 0;
        IpcFdArray[i] = -1;
#endif
    }

#if SDK_AUG2010
    memset(IpcHandlePathBuf, 0, sizeof(IpcHandlePathBuf));
    memset(IpcOpenPathBuf, 0, sizeof(IpcOpenPathBuf));
    memset(IpcReqArray, 0, sizeof(IpcReqArray));
#endif
}

void IPCiProfQueueReq(void* req, s32 handle) {
    ++IpcNumPendingReqs;
    ++IpcNumUnIssuedReqs;
#if SDK_AUG2010
    AddReqInfo((IOSResourceRequest*)req);
#else
    AddReqInfo((IOSResourceRequest*)req, handle);
#endif
}

void IPCiProfAck(void) {
    --IpcNumUnIssuedReqs;
}

void IPCiProfReply(void* req, s32 handle) {
    --IpcNumPendingReqs;
#if SDK_AUG2010
    DelReqInfo((IOSResourceRequest*)req);
#else
    DelReqInfo((IOSResourceRequest*)req, handle);
#endif
}

#if SDK_AUG2010
static void AddReqInfo(IOSResourceRequest* req) {
    u32 i;

    for (i = 0; i < IPC_REQ_MAX; i++) {
        if (!IpcReqPtrArray[i]) {
            BOOL irqEnabled = OSDisableInterrupts();

            IpcReqPtrArray[i] = req;
            IpcReqArray[i] = *req;
            IpcStartTimeArray[i] = OSGetTime();
            if (IpcReqArray[i].cmd == 1) {
                const char* path_p_virt = (const char*)OSPhysicalToCached((u32)IpcReqArray[i].args.open.path);
                strncpy(IpcOpenPathBuf[i], path_p_virt,
                        IPC_PATH_MAX - 1);
                IpcOpenPathBuf[i][IPC_PATH_MAX - 1] = '\0';
                IpcReqArray[i].args.open.path = (u8*)IpcOpenPathBuf[i];
            }

            OSRestoreInterrupts(irqEnabled);
            break;
        }
    }
}

static void DelReqInfo(IOSResourceRequest* req) {
    u32 i;

    for (i = 0; i < IPC_REQ_MAX; i++) {
        if (req == IpcReqPtrArray[i] && req->handle == IpcReqArray[i].cmd) {
            BOOL irqEnabled = OSDisableInterrupts();

            if (IpcReqArray[i].cmd == 1) {
                if (req->status >= 0) {
                    strncpy(IpcHandlePathBuf[req->status],
                            (const char*)IpcReqArray[i].args.open.path, IPC_PATH_MAX - 1);
                    IpcHandlePathBuf[req->status][IPC_PATH_MAX - 1] = '\0';
                    memset(&IpcOpenPathBuf[i], 0, IPC_PATH_MAX);
                }
            }

            if (IpcReqArray[i].cmd == 2) {
                memset(&IpcHandlePathBuf[IpcReqArray[i].handle], 0, IPC_PATH_MAX);
            }

            IpcReqPtrArray[i] = NULL;
            memset(&IpcReqArray[i], 0, sizeof(IOSResourceRequest));
            IpcStartTimeArray[i] = 0;

            OSRestoreInterrupts(irqEnabled);
            break;
        }
    }
}
#else
static void AddReqInfo(IOSResourceRequest* ptr, s32 fd) {
    u32 i;

    for (i = 0; i < 32; ++i) {
        if (IpcReqPtrArray[i] == 0 && IpcFdArray[i] == -1) {
            IpcReqPtrArray[i] = (u32)ptr;
            IpcFdArray[i] = fd;
            break;
        }
    }
}

static void DelReqInfo(IOSResourceRequest* ptr, s32 fd) {
    u32 i;

    for (i = 0; i < 32; ++i) {
        if (IpcReqPtrArray[i] == (u32)ptr && IpcFdArray[i] == fd) {
            IpcReqPtrArray[i] = 0;
            IpcFdArray[i] = -1;
            break;
        }
    }
}
#endif
