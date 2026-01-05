#include <revolution/os.h>
#include <revolution/fs.h>
#include <revolution/nand.h>
#include <cstring>
#include <cstdio>

static IOSFd s_fd = -255;
static IOSError s_err = ISFS_ERROR_UNKNOWN;
static int s_stage;
static char s_message[256] ATTRIBUTE_ALIGN(64);
static NANDLoggingCallback s_callback = 0;

static void asyncRoutine(ISFSError, void*);
static void prepareLine(char line[256], int, const char*);

BOOL reserveFileDescriptor(void) {
    BOOL enabled = FALSE;
    BOOL busy_flag = FALSE;

    enabled = OSDisableInterrupts();

    if (s_fd == -255) {
        s_fd = -254;
        busy_flag = FALSE;
    } else if (s_fd == -254) {
        busy_flag = TRUE;
    } else if (s_fd >= 0) {
        busy_flag = TRUE;
    } else {
        busy_flag = TRUE;
    }

    OSRestoreInterrupts(enabled);
    return busy_flag ? FALSE : TRUE;
}

BOOL NANDLoggingAddMessageAsync(NANDLoggingCallback cb, s32 errorCode, const char* fmt, ...) {
    va_list ap;
    ISFSError err = ISFS_ERROR_UNKNOWN;

    if (!reserveFileDescriptor()) {
        return FALSE;
    }   

    va_start(ap, fmt);
    vsnprintf(s_message, 256, fmt, ap);
    va_end(ap);

    s_callback = cb;
    s_stage = 1;
    
    if (errorCode == ISFS_ERROR_UNKNOWN || errorCode == IOS_ERROR_UNKNOWN) {
        s_err = errorCode;
    }
    
    err = ISFS_OpenAsync((const u8*)"/shared2/test2/nanderr.log", 3, asyncRoutine, 0);

    if (err == ISFS_ERROR_OK) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void callbackRoutine(BOOL result) {
    if (s_callback) {
        s_callback(result, s_err);
    }
}

static void asyncRoutine(ISFSError result, void *ctxt) {
    ISFSError ret = ISFS_ERROR_UNKNOWN;
    static char s_rBuf[256] ATTRIBUTE_ALIGN(64);
    static char s_wBuf[256] ATTRIBUTE_ALIGN(64);
    ++s_stage;

    if (s_stage == 2) {
        if (result >= 0) {
            s_fd = result;
            ret = ISFS_SeekAsync(s_fd, 0, 0, asyncRoutine, 0);

            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 3) {
        if (result == 0) {
            ret = ISFS_ReadAsync(s_fd, (u8*)s_rBuf, 256, asyncRoutine, 0);

            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 4) {
        if (result == 256) {
            ret = ISFS_SeekAsync(s_fd, 0, 0, asyncRoutine, 0);

            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 5) {
        if (result == 0) {
            int n = 0;
            s_rBuf[255] = '\0';
            n = atoi(s_rBuf);
            prepareLine(s_wBuf, n, s_message);
            ret = ISFS_WriteAsync(s_fd, (const u8*)s_wBuf, 256, asyncRoutine, 0);

            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 6) {
        if (result == 256) {
            int n = atoi(s_rBuf);
            ret = ISFS_SeekAsync(s_fd, n * 256, 0, asyncRoutine, 0);

            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 7) {
        int n = atoi(s_rBuf);
        if (result == n * 256) {
            ret = ISFS_WriteAsync(s_fd, (const u8*)s_wBuf, 256, asyncRoutine, 0);
            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 8) {
        if (result == 256) {
            ret = ISFS_CloseAsync(s_fd, asyncRoutine, 0);

            if (ret != ISFS_ERROR_OK) {
                callbackRoutine(FALSE);
            }
        } else {
            callbackRoutine(FALSE);
        }
    } else if (s_stage == 9) {
        if (result == ISFS_ERROR_OK) {
            s_fd = -255;
            callbackRoutine(TRUE);
        } else {
            callbackRoutine(FALSE);
        }
    }
}

static void prepareLine(char line[256], int n, const char* msg) {
    char titleID[64];
    int end = 0;
    struct OSCalendarTime cal;

    memset(line, ' ', 254);
    OSTicksToCalendarTime(OSGetTime(), &cal);
    strncpy(titleID, nandGetHomeDir() + 7, 8 + 1 + 8);
    titleID[8] = '-';
    titleID[8 + 1 + 8] = '\0';
    end = snprintf(line, 256, "%d %04d/%02d/%02d %02d:%02d:%02d %s %s", n % (64 - 1) + 1, cal.year, cal.mon + 1, cal.mday, cal.hour, cal.min, cal.sec, titleID, msg);

    if (end < 256) {
        line[end] = ' ';
    }

    line[254] = '\r';
    line[255] = '\n';
}
