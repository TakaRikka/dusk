#ifndef _REVOLUTION_DVD_INTERNAL_H_
#define _REVOLUTION_DVD_INTERNAL_H_

#include <revolution/os.h>
#include <revolution/dvd.h>

#ifdef __cplusplus
extern "C" {
#endif

// DVD
DVDCommandChecker __DVDSetOptionalCommandChecker(DVDCommandChecker func);
void __DVDSetImmCommand(u32 command);
void __DVDAudioBufferConfig(DVDCommandBlock* block, u32 enable, u32 size, DVDCBCallback callback);
int __DVDTestAlarm(const OSAlarm* alarm);
BOOL __DVDLowBreak(void);
u32 __DVDGetCoverStatus(void);
void __DVDPrepareReset(void);

extern vu32 __DVDLayoutFormat;

// DVD ERROR
void __DVDStoreErrorCode(u32 error, DVDCBCallback callback);
BOOL __DVDCheckDevice(void);

// DVD FATAL
void __DVDPrintFatalMessage(void);
void __DVDShowFatalMessage(void);

// DVD FS
extern OSThreadQueue __DVDThreadQueue;
extern u32 __DVDLongFileNameFlag;

void __DVDFSInit(void);

// DVD BROADWAY
int __DVDLowTestAlarm(const OSAlarm* alarm);

// DVD QUEUE
void __DVDClearWaitingQueue(void);
int __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
int __DVDCheckWaitingQueue(void);
int __DVDDequeueWaitingQueue(DVDCommandBlock* block);
int __DVDIsBlockInWaitingQueue(DVDCommandBlock* block);
DVDCommandBlock* __DVDGetNextWaitingQueue(void);

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_DVD_INTERNAL_H_
