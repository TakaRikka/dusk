#ifndef _DOLPHIN_OSMESSAGE_H_
#define _DOLPHIN_OSMESSAGE_H_

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* OSMessage;

#define OS_MESSAGE_NOBLOCK 0
#define OS_MESSAGE_BLOCK   1

typedef struct {
    OSThreadQueue queueSend;
    OSThreadQueue queueReceive;
    void* msgArray;
    s32 msgCount;
    s32 firstIndex;
    s32 usedCount;
} OSMessageQueue;

void OSInitMessageQueue(OSMessageQueue* mq, void* msgArray, s32 msgCount);
int OSSendMessage(OSMessageQueue* mq, void* msg, s32 flags);
int OSReceiveMessage(OSMessageQueue* mq, void* msg, s32 flags);
int OSJamMessage(OSMessageQueue* mq, void* msg, s32 flags);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSMESSAGE_H_
