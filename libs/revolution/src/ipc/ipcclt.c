#include <revolution/private/iostypes.h>
#include <revolution/private/iosrestypes.h>
#include <revolution/os.h>
#include <revolution/ipc.h>
#include <cstring>

#if SDK_AUG2010
#define RESPONSE_REQ_BUF_LEN 0x30
#define IPC_BUF_CNT 0x40
#else
#define RESPONSE_REQ_BUF_LEN 0x10
#define IPC_BUF_CNT 0x10
#endif

/* macro for matching __ipcQueueRequest */
#define diff(a, b)                                                           \
    ((a) < (b)) ? ((u32)0xffffffff - (b) + (a) + 1) : ((a) - (b))

static s32 __mailboxAck = 1;
static u32 __relnchFl = 0;

#define OSVirtualToPhysical(addr)    OSCachedToPhysical(addr)
#define OSPhysicalToVirtual(addr)    OSPhysicalToCached(addr)

typedef struct IOSRpcRequest {
    IOSResourceRequest request;
    IOSIpcCb cb ATTRIBUTE_ALIGN(32); // I am assuming this is aligned due to where cbArg is stored, and I see nothing between cb and callback_arg?
    void* callback_arg;
    u32 relaunch_flag;
    OSThreadQueue thread_queue;
} IOSRpcRequest;

static IOSRpcRequest* __relnchRpc = 0;
#if SDK_AUG2010
static IOSRpcRequest* __relnchRpcSave = 0;
#endif

#define ROUNDUP(sz)     (((u32)(sz) + (IPC_BUF_CNT / 2 - 1)) & ~(u32)(IPC_BUF_CNT / 2 - 1))

static u8 __rpcBuf[ROUNDUP(sizeof(IOSRpcRequest))] ATTRIBUTE_ALIGN(32);

static struct {
    u32 rcount;
    u32 wcount;
    u32 rptr;
    u32 wptr;
    IOSResourceRequest* buf[RESPONSE_REQ_BUF_LEN];
} __responses;

static OSAlarm __timeout_alarm;

static IOSHeapId hid = -1;

extern void ACRWriteReg(u32 param_0, u32 param_1);
extern void IPCiProfQueueReq(void*, s32);

/* the MSL_C version of strnlen doesn't match when inlined, cool */
u32 strnlen(const u8 *str, u32 n) {
    const u8 *s = str;
    while (*s && n-- > 0)
        ++s;
    return (s - str);
}

static IOSRpcRequest* ipcAllocReq(void) {
    IOSRpcRequest* req = NULL;
    req = iosAllocAligned(hid, 0x40, 0x20);
    return req;
}

static IOSError ipcFree(IOSRpcRequest *rpc) {
    IOSError ret = 0;
    iosFree(hid, rpc);
    return ret;
}

static IOSError __ipcQueueRequest(IOSResourceRequest *req) {
    IOSError ret = 0;

    if (diff(__responses.wcount, __responses.rcount) >= sizeof(__responses.buf) / sizeof(__responses.buf[0])) {
        ret = -8;
    } else {
        __responses.buf[__responses.wptr] = req;
        __responses.wptr = (__responses.wptr + 1) % (sizeof(__responses.buf) / sizeof(__responses.buf[0]));
        __responses.wcount++;
        IPCiProfQueueReq(req, (s32)req->handle);
    }

    return ret;
}

static void __ipcSendRequest(void) {
    IOSRpcRequest *rpc;

    if (diff(__responses.wcount, __responses.rcount) <= 0) {
        return;
    }

    rpc = (IOSRpcRequest*)__responses.buf[__responses.rptr];

    if (rpc == 0) {
        return;
    }

    if (rpc->relaunch_flag) {
#if !SDK_AUG2010
        __relnchFl = 1;
        __relnchRpc = rpc;
#endif
        __mailboxAck--;
    }

    IPCWriteReg(0, OSVirtualToPhysical(rpc));
    __responses.rptr = (__responses.rptr + 1) % (sizeof(__responses.buf) / sizeof(__responses.buf[0]));
    __responses.rcount++;
    __mailboxAck--;

    IPCWriteReg(1, (IPCReadReg(1) & (1 << 5 | 1 << 4)) | 1 << 0);
}

void IpcReplyHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;
    IOSResourceRequest* req;
    IOSRpcRequest* rep;
    u32 addr;

    addr = IPCReadReg(2);

    if (!addr) {
        goto err;
    }

    rep = (IOSRpcRequest*)OSPhysicalToVirtual(addr);
    IPCWriteReg(1, (IPCReadReg(1) & (1 << 5 | 1 << 4) | 1 << 2));
    ACRWriteReg(0x30, 0x40000000);
    req = &rep->request;

    DCInvalidateRange(req, sizeof(*req));

    switch (req->handle) {
    case 3:
        req->args.read.outPtr = (req->args.read.outPtr) ? OSPhysicalToVirtual((u32)req->args.read.outPtr) : 0;

        if (req->status > 0) {
            DCInvalidateRange(req->args.read.outPtr, (u32)req->status);
        }

        break;
    case 6:
        req->args.ioctl.outPtr = (req->args.ioctl.outPtr) ? OSPhysicalToVirtual((u32)req->args.ioctl.outPtr) : 0;
        DCInvalidateRange(req->args.ioctl.inPtr, req->args.ioctl.inLen);
        DCInvalidateRange(req->args.ioctl.outPtr, req->args.ioctl.outLen);
        break;
    case 7:
    {
        int i;
        IOSResourceIoctlv* v = &req->args.ioctlv;
        req->args.ioctlv.vector = (req->args.ioctlv.vector) ? (IOSIoVector*)OSPhysicalToVirtual((u32)req->args.ioctlv.vector) : 0;
        DCInvalidateRange(&v->vector[0], (req->args.ioctlv.readCount + req->args.ioctlv.writeCount) * sizeof(IOSIoVector));

        for (i = 0; i < (req->args.ioctlv.readCount + req->args.ioctlv.writeCount); ++i) {
            v->vector[i].base = (v->vector[i].base) ? (u8*)OSPhysicalToVirtual((u32)v->vector[i].base) : 0;
            DCInvalidateRange(v->vector[i].base, v->vector[i].length);
        }

#if SDK_AUG2010
        if (__relnchFl && __relnchRpcSave == rep)
#else
        if (__relnchFl && __relnchRpc == rep)
#endif
        {
            __relnchFl = 0;

            if (__mailboxAck < 1) {
                __mailboxAck++;
            }
        }

        break;
    }
    default:
        break;
    }

    if (rep->cb) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        rep->cb(req->status, (void*)rep->callback_arg);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
        ipcFree(rep);
    } else {
        OSWakeupThread(&rep->thread_queue);
    }

    IPCWriteReg(1, (IPCReadReg(1) & (1 << 5 | 1 << 4)) | 1 << 3);
    IPCiProfReply(req, (s32)req->handle);

err:
    return;
}

static void IpcAckHandler(__OSInterrupt interrupt, OSContext* context) {
    IPCWriteReg(1, (IPCReadReg(1) & (1 << 5 | 1 << 4)) | 1 << 1);
    ACRWriteReg(0x30, 0x40000000);

    if (__mailboxAck < 1) {
        __mailboxAck++;
        IPCiProfAck();
    }

    if (__mailboxAck > 0) {
        if (__relnchFl) {
            IOSResourceRequest* req = &__relnchRpc->request;
            req->status = 0;
            __relnchFl = 0;

            OSWakeupThread(&__relnchRpc->thread_queue);
            IPCWriteReg(1, (IPCReadReg(1) & (1 << 5 | 1 << 4)) | 1 << 3);
        }

        __ipcSendRequest();
    }
}


void IPCInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    if ((IPCReadReg(1) & (1 << 4 | 1 << 2)) == (1 << 4 | 1 << 2)) {
        IpcReplyHandler(interrupt, context);
    }

    if ((IPCReadReg(1) & (1 << 5 | 1 << 1)) == (1 << 5 | 1 << 1)) {
        IpcAckHandler(interrupt, context);
    }
}

IOSError IPCCltInit(void) {
    static u32 initialized = 0;
    u32 i;
    IOSError ret = 0;
    void* bufferLo;

    if (initialized) {
        goto out;
    }

    initialized = 1;

    IPCInit();

    i = ROUNDUP(IPC_BUF_CNT * (ROUNDUP(sizeof(IOSRpcRequest)) + 64));
    bufferLo = IPCGetBufferLo();

    if ((void*)((u8*)bufferLo + i) > IPCGetBufferHi()) {
        ret = -22;
        goto out;
    }

    hid = iosCreateHeap(bufferLo, i);
    IPCSetBufferLo((void*)((u8*)bufferLo + i));

    __OSSetInterruptHandler(27, IPCInterruptHandler);
    __OSUnmaskInterrupts(16);

    IPCWriteReg(1, (1 << 5 | 1<< 4 | 1 << 3));
    IPCiProfInit();
#if SDK_AUG2010
    OSCreateAlarm(&__timeout_alarm);
#endif

out:
    return ret;
}

#if SDK_AUG2010
IOSError IPCCltReInit(void) {
    u32 i;
    IOSError ret = 0;
    void* bufferLo;

    i = ROUNDUP(IPC_BUF_CNT * ROUNDUP(sizeof(IOSRpcRequest)));
    bufferLo = IPCGetBufferLo();

    if ((void*)((u8*)bufferLo + i) > IPCGetBufferHi()) {
        ret = -22;
        goto out;
    }

    hid = iosCreateHeap(bufferLo, i);
    IPCSetBufferLo((void*)((u8*)bufferLo + i));

out:
    return ret;
}
#endif

static IOSError __ios_Ipc1(IOSFd fd, u32 cmd, IOSIpcCb cb, void* cbArg, IOSRpcRequest** rpc) {
    IOSError ret = 0;
    IOSResourceRequest *req;

    if (rpc == 0) {
        ret = -4;
        goto error;
    }
    
    *rpc = (IOSRpcRequest*)ipcAllocReq();
    
    if (*rpc == 0) {
        ret = -22;
        goto error;
    }

    req = &(*rpc)->request;
    (*rpc)->cb = cb;
    (*rpc)->callback_arg = cbArg;
    (*rpc)->relaunch_flag = 0;
    req->cmd = cmd;
    req->handle = (u32)fd;

error:
    return ret;
}

static IOSError __ios_Ipc2(IOSRpcRequest* rpc, IOSIpcCb cb) {
    IOSError ret = 0;
    u32 inten;
    IOSResourceRequest* req;

    if (rpc == 0) {
        ret = -4;
    } else {
        req = &rpc->request;

        if (!cb) {
            OSInitThreadQueue(&rpc->thread_queue);
        }

        DCFlushRange(req, sizeof(*req));
        inten = OSDisableInterrupts();
        ret = __ipcQueueRequest(req);

        if (ret != 0) {
            OSRestoreInterrupts(inten);

            if (cb) {
                ipcFree(rpc);
            }
        } else {
            if (__mailboxAck > 0) {
                __ipcSendRequest();
            }

            if (!cb) {
                OSSleepThread(&rpc->thread_queue);
            }

            OSRestoreInterrupts(inten);

            if (!cb) {
                ret = req->status;
            }
        }
    }

    if (rpc && !cb) {
        ipcFree(rpc);
    }

    return ret;
}

static IOSError __ios_Open(IOSRpcRequest* rpc, const char* path, u32 flags) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;
    DCFlushRange((void*)path, strnlen((const u8*)path, 64) + 1);
    req->args.open.path = (u8*)OSVirtualToPhysical((void*)path);
    req->args.open.flags = flags;

error:
    return ret;
}

IOSError IOS_OpenAsync(const char* pPath, u32 flags, IOSIpcCb cb, void* callback_arg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(0, 1, cb, callback_arg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Open(rpc, pPath, flags);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);
error:
    return ret;
}

IOSError IOS_Open(const char* path, u32 flags) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(0, 1, 0, 0, &rpc);
    
    if (ret != 0) {
        goto error;
    }

    ret = __ios_Open(rpc, path, flags);
    
    if (ret != 0) {
        goto error;
    }
    
    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

IOSError IOS_CloseAsync(IOSFd fd, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 2, cb, cbArg, &rpc);

    if (ret == 0) {
        ret = __ios_Ipc2(rpc, cb);
    }

    return ret;
}

IOSError IOS_Close(IOSFd fd) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 2, 0, 0, &rpc);

    if (ret == 0) {
        ret = __ios_Ipc2(rpc, 0);
    }

    return ret;
}

static IOSError __ios_Read(IOSRpcRequest* rpc, void* buf, u32 len) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;

    DCInvalidateRange(buf, len);
    req->args.read.outPtr = (buf) ? (u8*)OSVirtualToPhysical(buf) : 0;
    req->args.read.outLen = len;

error:
    return ret;
}

IOSError IOS_ReadAsync(IOSFd fd, void* buf, u32 len, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 3, cb, cbArg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Read(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);

error:
    return ret;
}

IOSError IOS_Read(IOSFd fd, void* buf, u32 len) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 3, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Read(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Write(IOSRpcRequest* rpc, void* buf, u32 len) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;
    req->args.write.inPtr = (buf) ? (u8*)OSVirtualToPhysical(buf) : 0;
    req->args.write.inLen = len;
    DCFlushRange(buf, len);

error:
    return ret;
}

IOSError IOS_WriteAsync(IOSFd fd, void* buf, u32 len, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 4, cb, cbArg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Write(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);

error:
    return ret;
}

IOSError IOS_Write(IOSFd fd, void* buf, u32 len) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 4, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Write(rpc, buf, len);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Seek(IOSRpcRequest* rpc, s32 offset, u32 whence) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;
    req->args.seek.offset = offset;
    req->args.seek.whence = whence;

error:
    return ret;
}

IOSError IOS_SeekAsync(IOSFd fd, s32 offset, u32 whence, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 5, cb, cbArg, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Seek(rpc, offset, whence);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, cb);

error:
    return ret;
}

IOSError IOS_Seek(IOSFd fd, s32 offset, u32 whence) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 5, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Seek(rpc, offset, whence);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Ioctl(IOSRpcRequest* rpc, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen) {
    IOSError ret = 0;
    IOSResourceRequest* req;

    if (!rpc) {
        ret = -4;
        goto error;
    }

    req = &rpc->request;

    req->args.ioctl.cmd = (u32)cmd;
    req->args.ioctl.outPtr = (output) ? (u8*)OSVirtualToPhysical(output) : 0;
    req->args.ioctl.outLen = outputLen;
    req->args.ioctl.inPtr = (input) ? (u8*)OSVirtualToPhysical(input) : 0;
    req->args.ioctl.inLen = inputLen;
    
    DCFlushRange(input, inputLen);
    DCFlushRange(output, outputLen);

error:
    return ret;
}

IOSError IOS_IoctlAsync(IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 6, cb, cbArg, &rpc);

    if (ret != 0) {
        goto err;
    }

    ret = __ios_Ioctl(rpc, cmd, input, inputLen, output, outputLen);

    if (ret != 0) {
        goto err;
    }

    ret = __ios_Ipc2(rpc, cb);

err:
    return ret;
}

IOSError IOS_Ioctl(IOSFd fd, s32 cmd, void* input, u32 inputLen, void* output, u32 outputLen) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 6, 0, 0, &rpc);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ioctl(rpc, cmd, input, inputLen, output, outputLen);

    if (ret != 0) {
        goto error;
    }

    ret = __ios_Ipc2(rpc, 0);

error:
    return ret;
}

static IOSError __ios_Ioctlv(IOSRpcRequest* rpc, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect) {
    IOSError ret = 0;
    IOSResourceRequest* req;
    IOSResourceIoctlv* v;
    u32 i, j;

    if (!rpc) {
        ret = -4;
        goto err;
    }

    req = &rpc->request;
    req->args.ioctlv.cmd = (u32)cmd;
    req->args.ioctlv.readCount = readCount;
    req->args.ioctlv.writeCount = writeCount;
    req->args.ioctlv.vector = vect;

    v = &req->args.ioctlv;

    for (i = 0, j = v->readCount; i < req->args.ioctlv.writeCount; ++i) {
        DCFlushRange(v->vector[j + i].base, v->vector[j + i].length);
        v->vector[j + i].base = (v->vector[j + i].base) ? (u8*)OSVirtualToPhysical(v->vector[j + i].base) : 0;
    }

    for (i = 0; i < req->args.ioctlv.readCount; ++i) {
        DCFlushRange(v->vector[i].base, v->vector[i].length);
        v->vector[i].base = (v->vector[i].base) ? (u8*)OSVirtualToPhysical(v->vector[i].base) : 0;
    }

    DCFlushRange(&v->vector[0], (v->readCount + v->writeCount) * sizeof(IOSIoVector));
    req->args.ioctlv.vector = (vect) ? (IOSIoVector*)OSVirtualToPhysical(vect) : 0;

err:
    return ret;
}

IOSError IOS_IoctlvAsync(IOSFd fd, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect, IOSIpcCb cb, void* cbArg) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 7, cb, cbArg, &rpc);

    if (ret != 0) {
        goto err;
    }

    ret = __ios_Ioctlv(rpc, cmd, readCount, writeCount, vect);

    if (ret != 0) {
        goto err;
    }

    ret = __ios_Ipc2(rpc, cb);

err:
    return ret;
}

IOSError IOS_Ioctlv(IOSFd fd, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;

    ret = __ios_Ipc1(fd, 7, 0, 0, &rpc);

    if (ret != 0) {
        goto err;
    }

    ret = __ios_Ioctlv(rpc, cmd, readCount, writeCount, vect);

    if (ret != 0) {
        goto err;
    }

    ret = __ios_Ipc2(rpc, 0);

err:
    return ret;
}

IOSError IOS_IoctlvReboot(IOSFd fd, s32 cmd, u32 readCount, u32 writeCount, IOSIoVector* vect) {
    IOSRpcRequest* rpc;
    IOSError ret = 0;
    u32 inten;
    IOSResourceRequest* req;

#if SDK_AUG2010
    inten = OSDisableInterrupts();

    if (__relnchFl) {
        OSRestoreInterrupts(inten);
        ret = -10;
        goto finish;
    }

    __relnchFl = 1;
    OSRestoreInterrupts(inten);
#endif

    ret = __ios_Ipc1(fd, 7, 0, 0, &rpc);

    if (ret != 0) {
        goto err;
    }

#if SDK_AUG2010
    __relnchRpcSave = rpc;
#endif
    rpc->relaunch_flag = 1;

    ret = __ios_Ioctlv(rpc, cmd, readCount, writeCount, vect);

    if (ret != 0) {
        goto err;
    }

#if SDK_AUG2010
    memcpy(&__rpcBuf, rpc, sizeof(IOSRpcRequest));
    __relnchRpc = (IOSRpcRequest*)&__rpcBuf;
    req = &rpc->request;

    OSInitThreadQueue(&__relnchRpc->thread_queue);
    DCFlushRange(req, sizeof(*req));

    inten = OSDisableInterrupts();
    ret = __ipcQueueRequest(req);

    if (ret != 0) {
        OSRestoreInterrupts(inten);
        goto err;
    }

    if (__mailboxAck > 0) {
        __ipcSendRequest();
    }

    OSSleepThread(&__relnchRpc->thread_queue);
    OSRestoreInterrupts(inten);
    ret = (&__relnchRpc->request)->status;
#else
    ret = __ios_Ipc2(rpc, NULL);
#endif

err:
#if SDK_AUG2010
    __relnchFl = 0;
    __relnchRpcSave = NULL;

    if (rpc && (ret != 0)) {
        ipcFree(rpc);
    }
#endif

finish:
    return ret;
}
