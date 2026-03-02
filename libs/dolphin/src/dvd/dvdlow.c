#include <dolphin/dolphin.h>
#include <dolphin/dvd.h>

#include "__dvd.h"
#include "__os.h"

#define DVD_WATYPE_MAX 2

static BOOL FirstRead = TRUE;
static volatile BOOL StopAtNextInt = FALSE;
static u32 LastLength = 0;
static DVDLowCallback Callback = NULL;
static DVDLowCallback ResetCoverCallback = NULL;
static volatile OSTime LastResetEnd = 0;
static volatile u32 ResetOccurred = FALSE;
static volatile BOOL WaitingCoverClose = FALSE;
static volatile BOOL Breaking = FALSE;
static volatile u32 WorkAroundType = 0;
static u32 WorkAroundSeekLocation = 0;
static volatile OSTime LastReadFinished = 0;
static OSTime LastReadIssued = 0;
static volatile BOOL LastCommandWasRead = FALSE;
static volatile u32 NextCommandNumber = 0;

typedef struct {
	void* addr;
	u32 length;
	u32 offset;
} DVDBuffer;

typedef struct {
	s32 command;
	void* address;
	u32 length;
	u32 offset;
	DVDLowCallback callback;
} DVDCommand;

static DVDCommand CommandList[3];
static OSAlarm AlarmForWA;
static OSAlarm AlarmForTimeout;
static OSAlarm AlarmForBreak;
static DVDBuffer Prev;
static DVDBuffer Curr;

// prototypes
static void Read(void* address, u32 length, u32 offset, DVDLowCallback callback);
static void SetBreakAlarm(OSTime timeout);

void __DVDInitWA(void) {
	NextCommandNumber = 0;
	CommandList[0].command = -1;
	__DVDLowSetWAType(0, 0);
	OSInitAlarm();
}

static BOOL ProcessNextCommand(void) {
	s32 n = NextCommandNumber;

    ASSERTLINE(310, n < 3);

	if (CommandList[n].command == 1) {
		++NextCommandNumber;
		Read(CommandList[n].address, CommandList[n].length, CommandList[n].offset, CommandList[n].callback);
		return TRUE;
	} else if (CommandList[n].command == 2) {
		++NextCommandNumber;
		DVDLowSeek(CommandList[n].offset, CommandList[n].callback);
		return TRUE;
	}

	return FALSE;
}

void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
	DVDLowCallback cb;
	OSContext exceptionContext;
	u32 cause = 0;
	u32 reg;
	u32 intr;
	u32 mask;

	if (LastCommandWasRead) {
		LastReadFinished = __OSGetSystemTime();
		FirstRead = FALSE;
		Prev.addr = Curr.addr;
		Prev.length = Curr.length;
		Prev.offset = Curr.offset;

		if (StopAtNextInt == TRUE) {
			cause |= 8;
		}
	}

	LastCommandWasRead = FALSE;
	StopAtNextInt = FALSE;
	reg = __DIRegs[0];
	mask = reg & 0x2a;
	intr = (reg & 0x54) & (mask << 1);

	if (intr & 0x40) {
		cause |= 8;
	}

	if (intr & 0x10) {
		cause |= 1;
	}

	if (intr & 4) {
		cause |= 2;
	}

	if (cause) {
		ResetOccurred = FALSE;
		OSCancelAlarm(&AlarmForTimeout);
	}

	__DIRegs[0] = intr | mask;

	if (ResetOccurred && (__OSGetSystemTime() - LastResetEnd) < OSMillisecondsToTicks(200)) {
		reg = __DIRegs[1];
		mask = reg & 0x2;
		intr = (reg & 4) & (mask << 1);
		if (intr & 4) {
			if (ResetCoverCallback) {
				ResetCoverCallback(4);
			}
			ResetCoverCallback = NULL;
		}

		__DIRegs[1] = __DIRegs[1];
	} else if (WaitingCoverClose) {
		reg = __DIRegs[1];
		mask = reg & 2;
		intr = (reg & 4) & (mask << 1);

		if (intr & 4) {
			cause |= 4;
		}

		__DIRegs[1] = intr | mask;
		WaitingCoverClose = FALSE;
	} else {
		__DIRegs[1] = 0;
	}

	if ((cause & 8) && !Breaking) {
		cause &= ~8;
	}

	if (cause & 1) {
		if (ProcessNextCommand()) {
			return;
		}
	} else {
		CommandList[0].command = -1;
		NextCommandNumber = 0;
	}

	OSClearContext(&exceptionContext);
	OSSetCurrentContext(&exceptionContext);

	if (cause) {
		cb = Callback;
		Callback = NULL;
		if (cb) {
			cb(cause);
		}

		Breaking = FALSE;
	}

	OSClearContext(&exceptionContext);
	OSSetCurrentContext(context);
}

static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    BOOL processed = ProcessNextCommand();
    ASSERTLINE(652, processed);
}

static void AlarmHandlerForTimeout(OSAlarm* alarm, OSContext* context) {
	DVDLowCallback cb;
	OSContext exceptionContext;

	__OSMaskInterrupts(0x400);
	OSClearContext(&exceptionContext);
	OSSetCurrentContext(&exceptionContext);
	cb = Callback;
	Callback = NULL;
	if (cb) {
		cb(0x10);
	}

	OSClearContext(&exceptionContext);
	OSSetCurrentContext(context);
}

static void SetTimeoutAlarm(OSTime timeout) {
	OSCreateAlarm(&AlarmForTimeout);
	OSSetAlarm(&AlarmForTimeout, timeout, AlarmHandlerForTimeout);
}

static void Read(void* address, u32 length, u32 offset, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	LastCommandWasRead = TRUE;
	LastReadIssued = __OSGetSystemTime();

	__DIRegs[2] = 0xa8000000;
	__DIRegs[3] = offset / 4;
	__DIRegs[4] = length;
	__DIRegs[5] = (u32)address;
	__DIRegs[6] = length;
	LastLength = length;
	__DIRegs[7] = 3;

	if (length > 0xa00000) {
		SetTimeoutAlarm(OSSecondsToTicks(20));
	} else {
		SetTimeoutAlarm(OSSecondsToTicks(10));
	}
}

static BOOL AudioBufferOn(void) {
	DVDDiskID* id;

    id = DVDGetCurrentDiskID();
    if (id->streaming) {
        return TRUE;
    }

    return FALSE;
}

static BOOL HitCache(DVDBuffer* curr, DVDBuffer* prev) {
	u32 blockNumOfPrevEnd = (prev->offset + prev->length - 1) >> 15;
	u32 blockNumOfCurrStart = (curr->offset >> 15);
	u32 cacheBlockSize = AudioBufferOn() ? 5 : 15;

	if ((blockNumOfCurrStart > blockNumOfPrevEnd - 2) || (blockNumOfCurrStart < blockNumOfPrevEnd + cacheBlockSize + 3)) {
		return TRUE;
	}
	return FALSE;
}

static void DoJustRead(void* addr, u32 length, u32 offset, DVDLowCallback callback) {
	CommandList[0].command = -1;
	NextCommandNumber = 0;
	Read(addr, length, offset, callback);
}

static void SeekTwiceBeforeRead(void* addr, u32 length, u32 offset, DVDLowCallback callback) {
	u32 newOffset;
	if ((offset & ~0x7FFF) == 0) {
		newOffset = 0;
	} else {
        newOffset = (offset & ~0x7FFF) + WorkAroundSeekLocation;
	}

	CommandList[0].command = 2;
	CommandList[0].offset = newOffset;
	CommandList[0].callback = callback;
	CommandList[1].command = 1;
	CommandList[1].address = addr;
	CommandList[1].length = length;
	CommandList[1].offset = offset;
	CommandList[1].callback = callback;
	CommandList[2].command = -1;
	NextCommandNumber = 0;
	DVDLowSeek(newOffset, callback);
}

static void WaitBeforeRead(void* addr, u32 length, u32 offset, DVDLowCallback callback, OSTime wait) {
	CommandList[0].command = 1;
	CommandList[0].address = addr;
	CommandList[0].length = length;
	CommandList[0].offset = offset;
	CommandList[0].callback = callback;
	CommandList[1].command = -1;
	NextCommandNumber = 0;
	OSCreateAlarm(&AlarmForWA);
	OSSetAlarm(&AlarmForWA, wait, AlarmHandler);
}

BOOL DVDLowRead(void* addr, u32 length, u32 offset, DVDLowCallback callback) {
	u32 blockNumOfPrevEnd;
    u32 blockNumOfCurrStart;
	OSTime diff;

    ASSERTMSGLINE(837, (((u32)addr) & 31) == 0, "DVDLowRead(): address must be aligned with 32 byte boundary.");
    ASSERTMSGLINE(838, (length & 31) == 0, "DVDLowRead(): length must be a multiple of 32.");
    ASSERTMSGLINE(839, (offset & 3) == 0, "DVDLowRead(): offset must be a multiple of 4.");
    ASSERTMSGLINE(841, length != 0, "DVD read: 0 was specified to length of the read\n");

	__DIRegs[6] = length;
	Curr.addr = addr;
	Curr.length = length;
	Curr.offset = offset;

	if (WorkAroundType == 0) {
		DoJustRead(addr, length, offset, callback);
	} else if (WorkAroundType == 1) {
		if (FirstRead) {
			SeekTwiceBeforeRead(addr, length, offset, callback);
		} else {
			if (!HitCache(&Curr, &Prev)) {
				DoJustRead(addr, length, offset, callback);
			} else {
				blockNumOfPrevEnd = (u32)((Prev.offset + Prev.length - 1) >> 15) & 0x1FFFF;
                blockNumOfCurrStart = (u32)((Curr.offset >> 15) & 0x1FFFF);
				if (blockNumOfPrevEnd == blockNumOfCurrStart || blockNumOfPrevEnd + 1 == blockNumOfCurrStart) {
					diff = __OSGetSystemTime() - LastReadFinished;
					if (OSMillisecondsToTicks(5) < diff) {
						DoJustRead(addr, length, offset, callback);
					} else {
						WaitBeforeRead(addr, length, offset, callback, OSMillisecondsToTicks(5) - diff + OSMicrosecondsToTicks(500));
					}
				} else {
					SeekTwiceBeforeRead(addr, length, offset, callback);
				}
			}
		}
	} else {
        ASSERTLINE(900, FALSE);
    }
    
	return TRUE;
}

BOOL DVDLowSeek(u32 offset, DVDLowCallback callback) {
    ASSERTMSGLINE(920, (offset & 3) == 0, "DVDLowSeek(): offset must be a multiple of 4.");

	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = 0xab000000;
	__DIRegs[3] = offset / 4;
	__DIRegs[7] = 1;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowWaitCoverClose(DVDLowCallback callback) {
	Callback = callback;
	WaitingCoverClose = TRUE;
	StopAtNextInt = FALSE;
	__DIRegs[1] = 2;
	return TRUE;
}

BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback) {
    ASSERTMSGLINE(986, (((u32)diskID) & 31) == 0, "DVDLowReadID(): id must be aligned with 32 byte boundary.");

	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = 0xa8000040;
	__DIRegs[3] = 0;
	__DIRegs[4] = sizeof(DVDDiskID);
	__DIRegs[5] = (u32)diskID;
	__DIRegs[6] = sizeof(DVDDiskID);
	__DIRegs[7] = 3;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowStopMotor(DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = 0xe3000000;
	__DIRegs[7] = 1;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowRequestError(DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = 0xe0000000;
	__DIRegs[7] = 1;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = 0x12000000;
	__DIRegs[4] = sizeof(DVDDriveInfo);
	__DIRegs[5] = (u32)info;
	__DIRegs[6] = sizeof(DVDDriveInfo);
	__DIRegs[7] = 3;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = subcmd | 0xe1000000;
	__DIRegs[3] = offset >> 2;
	__DIRegs[4] = length;
	__DIRegs[7] = 1;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback) {
	Callback = callback;
	StopAtNextInt = FALSE;
	__DIRegs[2] = subcmd | 0xe2000000;
	__DIRegs[7] = 1;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback) {
#if DEBUG
    u32 bufSize;
    u32 trigger;
#endif

	Callback = callback;
	StopAtNextInt = FALSE;

#if DEBUG
    bufSize = size & 0xF;
    trigger = (size >> 4) & 0xF;
    ASSERTLINE(1242, bufSize < 16);
    ASSERTLINE(1243, trigger <= 2);
#endif

	__DIRegs[2] = 0xe4000000 | (enable != 0 ? 0x10000 : 0) | size;
	__DIRegs[7] = 1;
	SetTimeoutAlarm(OSSecondsToTicks(10));
	return TRUE;
}

void DVDLowReset(void) {
	u32 reg;
	OSTime resetStart;

	__DIRegs[1] = 2;
	reg = __PIRegs[9];
	__PIRegs[9] = (reg & ~4) | 1;

	resetStart = __OSGetSystemTime();
	while ((__OSGetSystemTime() - resetStart) < OSMicrosecondsToTicks(12))
		;

	__PIRegs[9] = reg | 4 | 1;
	ResetOccurred = TRUE;
	LastResetEnd = __OSGetSystemTime();
}

DVDLowCallback DVDLowSetResetCoverCallback(DVDLowCallback callback) {
	DVDLowCallback old;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    old = ResetCoverCallback;
    ResetCoverCallback = callback;
    OSRestoreInterrupts(enabled);
    return old;
}

static void DoBreak(void) {
    u32 statusReg;

    statusReg = __DIRegs[0];
    statusReg |= 0x40 | 1;
	__DIRegs[0] = statusReg;
    Breaking = TRUE;
}

static void AlarmHandlerForBreak(OSAlarm* alarm, OSContext* context) {
	if (__DIRegs[6] < LastLength) {
        DoBreak();
    } else {
        SetBreakAlarm(OSMillisecondsToTicks(20));
    }
}

static void SetBreakAlarm(OSTime timeout) {
	OSCreateAlarm(&AlarmForBreak);
    OSSetAlarm(&AlarmForBreak, timeout, AlarmHandlerForBreak);
}

BOOL DVDLowBreak(void) {
	StopAtNextInt = TRUE;
	Breaking = TRUE;
	return TRUE;
}

DVDLowCallback DVDLowClearCallback(void) {
	DVDLowCallback old;
	__DIRegs[1] = 0;
	WaitingCoverClose = FALSE;
	old = Callback;
	Callback = NULL;
	return old;
}

u32 DVDLowGetCoverStatus(void) {
	if ((__OSGetSystemTime() - LastResetEnd) <  OSMillisecondsToTicks(100)) {
        return 0;
    }

    if (__DIRegs[1] & 1) {
        return 1;
    }

    return 2;
}

void __DVDLowSetWAType(u32 type, s32 seekLoc) {
	BOOL enabled;

	enabled = OSDisableInterrupts();
    ASSERTLINE(1491, type < DVD_WATYPE_MAX);
	WorkAroundType = type;
	WorkAroundSeekLocation = seekLoc;
	OSRestoreInterrupts(enabled);
}

int __DVDLowTestAlarm(const OSAlarm* alarm) {
	if (alarm == &AlarmForBreak) {
        return 1;
    }
    if (alarm == &AlarmForTimeout) {
        return 1;
    }

    return 0;
}
