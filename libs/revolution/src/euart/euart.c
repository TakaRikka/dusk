#include <revolution/euart.h>
#include <revolution/exi.h>
#include <revolution/os.h>

static BOOL __EUARTInitialized = FALSE;
static EUARTError __EUARTLastErrorCode = EUART_ERROR_NONE;
static BOOL __EUARTSendStop = FALSE;
static u32 Enabled = 0;

BOOL EUARTInit(void) {
    BOOL enabled;
    u8 val;

    if (__EUARTInitialized) {
        return TRUE;
    }

    if (!(OSGetConsoleType() & 0x10000000)) {
        __EUARTLastErrorCode = EUART_ERROR_CANNOT_USE;
        return FALSE;
    }

    enabled = OSDisableInterrupts();

    val = -14;
    if (!EXIWriteReg(0, 1, 0xB0000000, &val, 1)) {
        __EUARTLastErrorCode = EUART_ERROR_INTERNAL;
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    val = -13;
    if (!EXIWriteReg(0, 1, 0xB0000000, &val, 1)) {
        __EUARTLastErrorCode = EUART_ERROR_INTERNAL;
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    OSRestoreInterrupts(enabled);

    __EUARTInitialized = TRUE;
    __EUARTLastErrorCode = EUART_ERROR_NONE;
    __EUARTSendStop = FALSE;
    return TRUE;
}

UARTError InitializeUART(UARTBaudRate rate) {
    if (!(OSGetConsoleType() & 0x10000000)) {
        Enabled = 0;
        return kUARTConfigurationError;
    } else {
        Enabled = 0xA5FF005A;
        return kUARTNoError;
    }
}

static int QueueLength(void) {
    u32 cmd, txCnt;

    if (!EXISelect(0, 1, __EXIFreq)) {
        return -1;
    }

    cmd = 0x30000100;
    EXIImm(0, &cmd, 4, 1, NULL);
    EXISync(0);
    EXIImm(0, &txCnt, sizeof(txCnt), 0, NULL);
    EXISync(0);
    EXIDeselect(0);
    return 32 - (u8)((txCnt >> 24) & 0x3F);
}

UARTError WriteUARTN(const void* buf, unsigned long len) {
    u32 cmd;
    int qLen;
    UARTError error;
    char* sendData = (char*)buf;
    u32 uart_txd;

    if (Enabled != 0xA5FF005A) {
        return kUARTConfigurationError;
    }

    if (!__EUARTInitialized) {
        if (!EUARTInit()) {
            return kUARTConfigurationError;
        }
    }

    if (!__EUARTInitialized) {
        __EUARTLastErrorCode = EUART_ERROR_UNINITIALIZED;
        return kUARTConfigurationError;
    }

    if (!EXILock(0, 1, 0)) {
        return kUARTNoError;
    }

    {
        char* p;

        for (p = sendData; (p - buf) < len; p++) {
            if (*p == '\n') {
                *p = '\r';
            }
        }
    }

    error = kUARTNoError;
    cmd = 0xB0000100;

    while (len != 0) {
        qLen = QueueLength();

        if (qLen < 0) {
            error = kUARTBufferOverflow;
            break;
        }

        if (qLen != 32) {
            continue;
        }

        if (!EXISelect(0, 1, __EXIFreq)) {
            error = kUARTBufferOverflow;
            break;
        }

        EXIImm(0, &cmd, 4, 1, NULL);
        EXISync(0);

        while ((qLen > 0) && (len > 0)) {
            uart_txd = (u32)((*sendData & 0xFF) << 24);

            EXIImm(0, &uart_txd, 4, 1, NULL);
            EXISync(0);

            sendData++;
            qLen--;
            len--;
        }

        EXIDeselect(0);
    }

    EXIUnlock(0);
    return error;
}
