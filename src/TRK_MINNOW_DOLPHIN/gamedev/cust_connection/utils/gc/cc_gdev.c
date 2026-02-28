#include "TRK_MINNOW_DOLPHIN/utils/common/CircleBuffer.h"
#include "odemuexi2/DebuggerDriver.h"

#define GDEV_BUF_SIZE (0x500)

static CircleBuffer gRecvCB;
static u8 gRecvBuf[GDEV_BUF_SIZE];
static BOOL gIsInitialized;

int gdev_cc_initinterrupts() {
    DBInitInterrupts();
    return 0;
}

int gdev_cc_peek() {
    u8 buf[GDEV_BUF_SIZE];
    int len;

    if ((len = DBQueryData()) <= 0) {
        return 0;
    }

    if (DBRead((u32*)buf, len) == 0) {
        CircleBufferWriteBytes(&gRecvCB, buf, len);
    } else {
        return 0xFFFFD8E7;
    }

    return len;
}

int gdev_cc_post_stop() {
    DBOpen();
    return 0;
}

int gdev_cc_pre_continue() {
    DBClose();
    return 0;
}

int gdev_cc_write(unsigned char* src, int size) {
    if (!gIsInitialized) {
        return 0xFFFFD8EF;
    }

    while (size > 0) {
        u32 written_cnt = DBWrite(src, size);
        if (written_cnt == 0) {
            break;
        }
        src += written_cnt;
        size -= written_cnt;
    }

    return 0;
}

int gdev_cc_read(u8* buf, u32 len) {
    u8 buf_local[GDEV_BUF_SIZE];
    int rv = 0;
    int var_r29;

    if (!gIsInitialized) {
        return 0xFFFFD8EF;
    }

    while (CBGetBytesAvailableForRead(&gRecvCB) < len) {
        rv = 0;
        if ((var_r29 = DBQueryData()) == 0) {
            continue;
        }
        rv = DBRead((u32*)buf_local, len);
        if (rv == 0) {
            CircleBufferWriteBytes(&gRecvCB, buf_local, var_r29);
        }
    }
    if (rv == 0) {
        CircleBufferReadBytes(&gRecvCB, buf, len);
    }
    return rv;
}

int gdev_cc_close() {
    return 0;
}

int gdev_cc_open() {
    if (gIsInitialized) {
        return 0xFFFFD8EB;
    }

    gIsInitialized = 1;
    return 0;
}

int gdev_cc_shutdown() {
    return 0;
}

int gdev_cc_initialize(u8** a, MTRCallbackType b) {
    DBInitComm(a, b);
    CircleBufferInitialize(&gRecvCB, gRecvBuf, sizeof(gRecvBuf));
    return 0;
}
