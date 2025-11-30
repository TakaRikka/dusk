#ifndef _DOLPHIN_DVD_INTERNAL_H_
#define _DOLPHIN_DVD_INTERNAL_H_

#include <dolphin/os.h>
#include <dolphin/dvd.h>

#ifdef __cplusplus
extern "C" {
#endif

// DVD
DVDCommandChecker __DVDSetOptionalCommandChecker(DVDCommandChecker func);
void __DVDSetImmCommand(u32 command);
void __DVDSetDmaCommand(u32 command);
void* __DVDGetIssueCommandAddr(void);
void __DVDAudioBufferConfig(DVDCommandBlock* block, u32 enable, u32 size, DVDCBCallback callback);
void __DVDPrepareResetAsync(DVDCBCallback callback);
int __DVDTestAlarm(const OSAlarm* alarm);

// DVD ERROR
void __DVDStoreErrorCode(u32 error);

// DVD FATAL
void __DVDPrintFatalMessage(void);

// DVD FS
extern OSThreadQueue __DVDThreadQueue;
extern u32 __DVDLongFileNameFlag;

void __DVDFSInit(void);

// DVD LOW
void __DVDInitWA(void);
void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext* context);
void __DVDLowSetWAType(u32 type, s32 seekLoc);
int __DVDLowTestAlarm(const OSAlarm* alarm);

// DVD QUEUE
void __DVDClearWaitingQueue(void);
int __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
int __DVDCheckWaitingQueue(void);
int __DVDDequeueWaitingQueue(DVDCommandBlock* block);
int __DVDIsBlockInWaitingQueue(DVDCommandBlock* block);

// FST LOAD
void __fstLoad(void);

// unsorted
u32 __DVDGetCoverStatus(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DVD_INTERNAL_H_
