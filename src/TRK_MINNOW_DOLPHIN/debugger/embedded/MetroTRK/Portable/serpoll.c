#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.h"
#include "TRK_MINNOW_DOLPHIN/utils/common/MWTrace.h"
#include "trk.h"

static TRKFramingState gTRKFramingState;

void* gTRKInputPendingPtr;

MessageBufferID TRKTestForPacket() {
    u8 payloadBuf[0x880];
    u8 packetBuf[0x40];
    int bufID;
    TRKBuffer* msg;
    MessageBufferID result;

    if (TRKPollUART() <= 0) {
        return -1;
    }

    result = TRKGetFreeBuffer(&bufID, &msg);

    MWTRACE(4, "TestForPacket : FreeBuffer is  %ld\n", result);

    TRKSetBufferPosition(msg, 0);
    if (TRKReadUARTN(packetBuf, 0x40) == UART_NoError) {
        int readSize;

        TRKAppendBuffer_ui8(msg, packetBuf, 0x40);
        readSize = ((u32*)packetBuf)[0] - 0x40;
        result = bufID;
        if (readSize > 0) {
            MWTRACE(1, "Reading payload %ld bytes\n", readSize);
            if (TRKReadUARTN(payloadBuf, ((u32*)packetBuf)[0] - 0x40) == UART_NoError) {
                TRKAppendBuffer_ui8(msg, payloadBuf, ((u32*)packetBuf)[0]);
            } else {
                MWTRACE(8, "TestForPacket : Invalid size of packet hdr.size\n");
                TRKReleaseBuffer(result);
                result = -1;
            }
        }
    } else {
        MWTRACE(8, "TestForPacket : Invalid size of packet\n");
        TRKReleaseBuffer(result);
        result = -1;
    }

    MWTRACE(1, "TestForPacket returning %ld\n", result);
    return result;
}

void TRKGetInput(void) {
    MessageBufferID id = TRKTestForPacket();
    if (id != -1) {
        TRKEvent event;
        TRKGetBuffer(id);
        TRKConstructEvent(&event, NUBEVENT_Request);
        event.msgBufID = id;
        gTRKFramingState.msgBufID = -1;
        TRKPostEvent(&event);
    }
}

void TRKProcessInput(int bufferIdx) {
    TRKEvent event;

    TRKConstructEvent(&event, NUBEVENT_Request);
    event.msgBufID = bufferIdx;
    gTRKFramingState.msgBufID = -1;
    TRKPostEvent(&event);
}

DSError TRKInitializeSerialHandler() {
    gTRKFramingState.msgBufID = -1;
    gTRKFramingState.receiveState = DSRECV_Wait;
    gTRKFramingState.isEscape = FALSE;

    MWTRACE(1, "TRK_Packet_Header \t    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_ReadMemory     %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_WriteMemory    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_Connect \t    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_ReplyAck\t    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_ReadRegisters\t%ld bytes\n", 0x40);

    return DS_NoError;
}

DSError TRKTerminateSerialHandler(void) {
    return DS_NoError;
}
