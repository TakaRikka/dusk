#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubinit.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/mutex_TRK.h"

TRKBuffer gTRKMsgBufs[3];

void TRKSetBufferUsed(TRKBuffer* msg, BOOL state) {
    msg->isInUse = state;
}

DSError TRKInitializeMessageBuffers(void) {
    int i;
    for (i = 0; i < 3; i++) {
        TRKInitializeMutex(&gTRKMsgBufs[i]);
        TRKAcquireMutex(&gTRKMsgBufs[i]);
        TRKSetBufferUsed(&gTRKMsgBufs[i], FALSE);
        TRKReleaseMutex(&gTRKMsgBufs[i]);
    }

    return DS_NoError;
}

DSError TRKGetFreeBuffer(int* msgID, TRKBuffer** outMsg) {
    TRKBuffer* buf;
    DSError error = DS_NoMessageBufferAvailable;
    int i;

    *outMsg = NULL;

    for (i = 0; i < 3; i++) {
        buf = TRKGetBuffer(i);

        TRKAcquireMutex(buf);
        if (!buf->isInUse) {
            TRKResetBuffer(buf, TRUE);
            TRKSetBufferUsed(buf, TRUE);
            error = DS_NoError;
            *outMsg = buf;
            *msgID = i;
            i = 3;  // why not break? weird choice
        }
        TRKReleaseMutex(buf);
    }

    if (error == DS_NoMessageBufferAvailable) {
        usr_puts_serial("ERROR : No buffer available\n");
    }

    return error;
}

void* TRKGetBuffer(int idx) {
    TRKBuffer* buf = NULL;
    if (idx >= 0 && idx < 3) {
        buf = &gTRKMsgBufs[idx];
    }

    return buf;
}

void TRKReleaseBuffer(int idx) {
    TRKBuffer* msg;
    if (idx != -1 && idx >= 0 && idx < 3) {
        msg = &gTRKMsgBufs[idx];
        TRKAcquireMutex(msg);
        TRKSetBufferUsed(msg, FALSE);
        TRKReleaseMutex(msg);
    }
}

void TRKResetBuffer(TRKBuffer* msg, BOOL keepData) {
    msg->length = 0;
    msg->position = 0;

    if (!keepData) {
        TRK_memset(msg->data, 0, 0x880);
    }
}

DSError TRKSetBufferPosition(TRKBuffer* msg, u32 pos) {
    DSError error = DS_NoError;

    if (pos > 0x880) {
        error = DS_MessageBufferOverflow;
    } else {
        msg->position = pos;
        // If the new position is past the current length,
        // update the length
        if (pos > msg->length) {
            msg->length = pos;
        }
    }

    return error;
}

DSError TRKAppendBuffer(TRKBuffer* msg, const void* data, size_t length) {
    DSError error = DS_NoError;  // r31
    u32 bytesLeft;

    // Return if no bytes to append
    if (length == 0) {
        return DS_NoError;
    }

    bytesLeft = 0x880 - msg->position;

    // If there isn't enough space left in the buffer, change the number
    // of bytes to append to the remaning number of bytes
    if (bytesLeft < length) {
        error = DS_MessageBufferOverflow;
        length = bytesLeft;
    }

    if (length == 1) {
        // If the length of bytes to append is 1, just copy the byte over
        msg->data[msg->position] = ((u8*)data)[0];
    } else {
        // Otherwise, use memcpy
        TRK_memcpy(msg->data + msg->position, data, length);
    }

    // Update the position and length
    msg->position += length;
    msg->length = msg->position;

    return error;
}

DSError TRKReadBuffer(TRKBuffer* msg, void* data, size_t length) {
    DSError error = DS_NoError;
    unsigned int bytesLeft;  // this has to be unsigned int not u32 to match lmfao.

    // Return if no bytes to read
    if (length == 0) {
        return DS_NoError;
    }

    bytesLeft = msg->length - msg->position;

    // If the number of bytes to read exceeds the buffer length, change
    // the length to the remaining number of bytes
    if (length > bytesLeft) {
        error = DS_MessageBufferReadError;
        length = bytesLeft;
    }

    TRK_memcpy(data, msg->data + msg->position, length);
    msg->position += length;
    return error;
}

DSError TRKAppendBuffer1_ui8(TRKBuffer* buffer, const u8 data) {
    if (buffer->position >= 0x880) {
        return DS_MessageBufferOverflow;
    }

    buffer->data[buffer->position++] = data;
    buffer->length++;
    return DS_NoError;
}

DSError TRKAppendBuffer1_ui32(TRKBuffer* buffer, const u32 data) {
    u8* bigEndianData;
    u8* byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (u8*)&data;
    } else {
        byteData = (u8*)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[3];
        bigEndianData[1] = byteData[2];
        bigEndianData[2] = byteData[1];
        bigEndianData[3] = byteData[0];
    }

    return TRKAppendBuffer(buffer, (const void*)bigEndianData, sizeof(data));
}

DSError TRKAppendBuffer1_ui64(TRKBuffer* buffer, const u64 data) {
    u8* bigEndianData;
    u8* byteData;
    u8 swapBuffer[sizeof(data)];
    if (gTRKBigEndian) {
        bigEndianData = (u8*)&data;
    } else {
        byteData = (u8*)&data;
        bigEndianData = swapBuffer;

        bigEndianData[0] = byteData[7];
        bigEndianData[1] = byteData[6];
        bigEndianData[2] = byteData[5];
        bigEndianData[3] = byteData[4];
        bigEndianData[4] = byteData[3];
        bigEndianData[5] = byteData[2];
        bigEndianData[6] = byteData[1];
        bigEndianData[7] = byteData[0];
    }

    return TRKAppendBuffer(buffer, (const void*)bigEndianData, sizeof(data));
}

DSError TRKAppendBuffer_ui8(TRKBuffer* buffer, const u8* data, int count) {
    DSError err;
    int i;

    for (i = 0, err = DS_NoError; err == DS_NoError && i < count; i++) {
        err = TRKAppendBuffer1_ui8(buffer, data[i]);
    }

    return err;
}

DSError TRKAppendBuffer_ui32(TRKBuffer* buffer, const u32* data, int count) {
    DSError err;
    int i;

    for (i = 0, err = DS_NoError; err == DS_NoError && i < count; i++) {
        err = TRKAppendBuffer1_ui32(buffer, data[i]);
    }

    return err;
}

DSError TRKReadBuffer1_ui8(TRKBuffer* buffer, u8* data) {
    return TRKReadBuffer(buffer, (void*)data, 1);
}

DSError TRKReadBuffer1_ui32(TRKBuffer* buffer, u32* data) {
    DSError err;

    u8* bigEndianData;
    u8* byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (u8*)data;
    } else {
        bigEndianData = swapBuffer;
    }

    err = TRKReadBuffer(buffer, (void*)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && err == DS_NoError) {
        byteData = (u8*)data;

        byteData[0] = bigEndianData[3];
        byteData[1] = bigEndianData[2];
        byteData[2] = bigEndianData[1];
        byteData[3] = bigEndianData[0];
    }

    return err;
    // UNUSED FUNCTION
}

DSError TRKReadBuffer1_ui64(TRKBuffer* buffer, u64* data) {
    DSError err;

    u8* bigEndianData;
    u8* byteData;
    u8 swapBuffer[sizeof(data)];

    if (gTRKBigEndian) {
        bigEndianData = (u8*)data;
    } else {
        bigEndianData = swapBuffer;
    }

    err = TRKReadBuffer(buffer, (void*)bigEndianData, sizeof(*data));

    if (!gTRKBigEndian && err == 0) {
        byteData = (u8*)data;

        byteData[0] = bigEndianData[7];
        byteData[1] = bigEndianData[6];
        byteData[2] = bigEndianData[5];
        byteData[3] = bigEndianData[4];
        byteData[4] = bigEndianData[3];
        byteData[5] = bigEndianData[2];
        byteData[6] = bigEndianData[1];
        byteData[7] = bigEndianData[0];
    }

    return err;
}

DSError TRKReadBuffer_ui8(TRKBuffer* buffer, u8* data, int count) {
    DSError err;
    int i;

    for (i = 0, err = DS_NoError; err == DS_NoError && i < count; i++) {
        err = TRKReadBuffer1_ui8(buffer, &(data[i]));
    }

    return err;
}

DSError TRKReadBuffer_ui32(TRKBuffer* buffer, u32* data, int count) {
    DSError err;
    s32 i;

    for (i = 0, err = DS_NoError; err == DS_NoError && i < count; i++) {
        err = TRKReadBuffer1_ui32(buffer, &(data[i]));
    }

    return err;
}
