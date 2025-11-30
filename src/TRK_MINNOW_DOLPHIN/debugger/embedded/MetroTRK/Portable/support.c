#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/support.h"
#include "TRK_MINNOW_DOLPHIN/utils/common/MWTrace.h"
#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msgbuf.h"
#include <string.h>

DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count, DSIOResult* io_result,
                          BOOL need_reply, BOOL read) {
    DSError error;
    int replyBufferId;
    TRKBuffer* replyBuffer;
    u32 length;
    int bufferId;
    TRKBuffer* buffer;
    u32 i;
    u8 replyIOResult;
    u32 replyLength;
    BOOL exit;
    CommandReply reply;

    if (data == NULL || *count == 0) {
        return DS_ParameterError;
    }

    exit = FALSE;
    *io_result = DS_IONoError;
    i = 0;
    error = DS_NoError;
    while (!exit && i < *count && error == DS_NoError && *io_result == 0) {
        memset(&reply, 0, sizeof(CommandReply));

        if (*count - i <= 0x800) {
            length = *count - i;
        } else {
            length = 0x800;
        }

        reply.commandID.b = read ? DSMSG_ReadFile : DSMSG_WriteFile;

        if (read) {
            reply._00 = 0x40;
        } else {
            reply._00 = length + 0x40;
        }

        reply.replyError.r = file_handle;
        *(u16*)&reply._0C = length;

        TRKGetFreeBuffer(&bufferId, &buffer);
        error = TRKAppendBuffer_ui8(buffer, (u8*)&reply, 0x40);

        if (!read && error == DS_NoError) {
            error = TRKAppendBuffer_ui8(buffer, data + i, length);
        }

        if (error == DS_NoError) {
            if (need_reply) {
                BOOL b = read && file_handle == 0;

                error = TRKRequestSend(buffer, &replyBufferId, read ? 5 : 5, 3, !b);
                if (error == DS_NoError) {
                    replyBuffer = (TRKBuffer*)TRKGetBuffer(replyBufferId);
                }
                replyIOResult = *(u32*)(replyBuffer->data + 0x10);
                replyLength = *(u16*)(replyBuffer->data + 0x14);
                if (read && error == DS_NoError && replyLength <= length) {
                    TRKSetBufferPosition(replyBuffer, 0x40);
                    error = TRKReadBuffer_ui8(replyBuffer, data + i, replyLength);
                    if (error == DS_MessageBufferReadError) {
                        error = DS_NoError;
                    }
                }

                if (replyLength != length) {
                    length = replyLength;
                    exit = TRUE;
                }

                *io_result = (DSIOResult)replyIOResult;
                TRKReleaseBuffer(replyBufferId);
            } else {
                error = TRKMessageSend(buffer);
            }
        }

        TRKReleaseBuffer(bufferId);
        i += length;
    }

    *count = i;
    return error;
}

DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2, int p3) {
    int error = DS_NoError;
    TRKBuffer* buffer;
    u32 counter;
    int count;
    u8 msgCmd;
    int msgReplyError;
    BOOL badReply = TRUE;

    *bufferId = -1;

    for (count = p2 + 1; count != 0 && *bufferId == -1 && error == DS_NoError; count--) {
        MWTRACE(1, "Calling MessageSend\n");
        error = TRKMessageSend(msgBuf);
        if (error == DS_NoError) {
            if (p3) {
                counter = 0;
            }

            while (TRUE) {
                do {
                    *bufferId = TRKTestForPacket();
                    if (*bufferId != -1)
                        break;
                } while (!p3 || ++counter < 79999980);

                if (*bufferId == -1)
                    break;

                badReply = 0;

                buffer = TRKGetBuffer(*bufferId);
                TRKSetBufferPosition(buffer, 0);
                OutputData(&buffer->data[0], buffer->length);
                msgCmd = buffer->data[4];
                MWTRACE(1, "msg_command : 0x%02x hdr->cmdID 0x%02x\n", msgCmd, msgCmd);

                if (msgCmd >= DSMSG_ReplyACK)
                    break;

                TRKProcessInput(*bufferId);
                *bufferId = -1;
            }

            if (*bufferId != -1) {
                if (buffer->length < 0x40) {
                    // OSReport("MetroTRK - bad reply size %ld\n", buffer->length);
                    badReply = TRUE;
                }
                if (error == DS_NoError && !badReply) {
                    msgReplyError = buffer->data[8];
                    MWTRACE(1, "msg_error : 0x%02x\n", msgReplyError);
                }
                if (error == DS_NoError && !badReply) {
                    if ((int)msgCmd != DSMSG_ReplyACK || msgReplyError != DSREPLY_NoError) {
                        MWTRACE(8,
                                "RequestSend : Bad ack or non ack received msg_command : 0x%02x "
                                "msg_error 0x%02x\n",
                                msgCmd, msgReplyError);
                        badReply = TRUE;
                    }
                }
                if (error != DS_NoError || badReply) {
                    TRKReleaseBuffer(*bufferId);
                    *bufferId = -1;
                }
            }
        }
    }

    if (*bufferId == -1) {
        error = DS_Error800;
    }

    return error;
}

DSError HandleOpenFileSupportRequest(const char* path, u8 replyError, u32* param_3,
                                     DSIOResult* ioResult) {
    DSError error;
    int bufferId2;
    int bufferId1;
    TRKBuffer* tempBuffer;
    TRKBuffer* buffer;
    CommandReply reply;

    memset(&reply, 0, sizeof(CommandReply));
    *param_3 = 0;
    reply.commandID.b = DSMSG_OpenFile;
    reply._00 = strlen(path) + 0x40 + 1;
    reply.replyError.b = replyError;
    *(u16*)&reply._0C = strlen(path) + 1;
    TRKGetFreeBuffer(&bufferId1, &buffer);
    error = TRKAppendBuffer_ui8(buffer, (u8*)&reply, 0x40);

    if (error == DS_NoError) {
        error = TRKAppendBuffer_ui8(buffer, (u8*)path, strlen(path) + 1);
    }

    if (error == DS_NoError) {
        *ioResult = DS_IONoError;
        error = TRKRequestSend(buffer, &bufferId2, 7, 3, 0);

        if (error == DS_NoError) {
            tempBuffer = TRKGetBuffer(bufferId2);
        }

        *ioResult = *(u32*)(tempBuffer->data + 0x10);
        *param_3 = *(u32*)(tempBuffer->data + 0x8);
        TRKReleaseBuffer(bufferId2);
    }
    TRKReleaseBuffer(bufferId1);
    return error;
}

DSError HandleCloseFileSupportRequest(int replyError, DSIOResult* ioResult) {
    DSError error;
    int replyBufferId;
    int bufferId;
    TRKBuffer* buffer1;
    TRKBuffer* buffer2;
    CommandReply reply;

    memset(&reply, 0, sizeof(CommandReply));
    reply.commandID.b = DSMSG_CloseFile;
    reply._00 = 0x40;
    reply.replyError.r = replyError;
    error = TRKGetFreeBuffer(&bufferId, &buffer1);

    if (error == DS_NoError) {
        error = TRKAppendBuffer_ui8(buffer1, (u8*)&reply, sizeof(CommandReply));
    }

    if (error == DS_NoError) {
        *ioResult = DS_IONoError;
        error = TRKRequestSend(buffer1, &replyBufferId, 3, 3, 0);

        if (error == DS_NoError) {
            buffer2 = TRKGetBuffer(replyBufferId);
        }

        if (error == DS_NoError) {
            *ioResult = *(u32*)(buffer2->data + 0x10);
        }

        TRKReleaseBuffer(replyBufferId);
    }

    TRKReleaseBuffer(bufferId);
    return error;
}

DSError HandlePositionFileSupportRequest(DSReplyError replyErr, u32* param_2, u8 param_3,
                                         DSIOResult* ioResult) {
    DSError error;
    int bufferId2;
    int bufferId1;
    TRKBuffer* buffer1;
    TRKBuffer* buffer2;
    CommandReply reply;

    memset(&reply, 0, sizeof(CommandReply));
    reply.commandID.b = DSMSG_PositionFile;
    reply._00 = 0x40;
    reply.replyError.r = replyErr;
    reply._0C = *param_2;
    reply._10[0] = param_3;
    error = TRKGetFreeBuffer(&bufferId1, &buffer1);

    if (error == DS_NoError) {
        error = TRKAppendBuffer_ui8(buffer1, (u8*)&reply, sizeof(CommandReply));
    }

    if (error == DS_NoError) {
        *ioResult = DS_IONoError;
        *param_2 = -1;
        error = TRKRequestSend(buffer1, &bufferId2, 3, 3, 0);

        if (error == DS_NoError) {
            buffer2 = TRKGetBuffer(bufferId2);

            if (buffer2 != NULL) {
                *ioResult = *(u32*)(buffer2->data + 0x10);
                *param_2 = *(u32*)(buffer2->data + 0x18);
            }
        }

        TRKReleaseBuffer(bufferId2);
    }

    TRKReleaseBuffer(bufferId1);
    return error;
}
