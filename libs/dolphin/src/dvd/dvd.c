#include <dolphin/dolphin.h>
#include <dolphin/os.h>
#include <dolphin/dvd.h>

#include "os/__os.h"
#include "__dvd.h"

// externs
extern void __DVDPrintFatalMessage();
extern int DVDCompareDiskID(const struct DVDDiskID * id1 /* r29 */, const struct DVDDiskID * id2 /* r30 */);
extern int __DVDLowTestAlarm(const OSAlarm * alarm /* r3 */);

#if DEBUG
const char* __DVDVersion = "<< Dolphin SDK - DVD\tdebug build: Apr  5 2004 03:56:07 (0x2301) >>";
#else
const char* __DVDVersion = "<< Dolphin SDK - DVD\trelease build: Apr  5 2004 04:14:51 (0x2301) >>";
#endif

static BOOL autoInvalidation = TRUE;

static void defaultOptionalCommandChecker(DVDCommandBlock*, DVDCommandCheckerCallback);
static DVDCommandChecker checkOptionalCommand = defaultOptionalCommandChecker;

static DVDBB2 BB2;
static DVDDiskID CurrDiskID;
static DVDCommandBlock DummyCommandBlock;
static OSAlarm ResetAlarm;

static DVDCommandBlock* executing;
static DVDDiskID* IDShouldBe;
static OSBootInfo* bootInfo;
static volatile int PauseFlag;
static volatile int PausingFlag;
static int AutoFinishing;
static BOOL FatalErrorFlag;
static volatile u32 CurrCommand;
static volatile u32 Canceling;
static void (*CancelCallback)(s32, DVDCommandBlock*);
static volatile u32 ResumeFromHere;
static volatile u32 CancelLastError;
static u32 LastError;
static volatile s32 NumInternalRetry;
static int ResetRequired;
static int CancelAllSyncComplete;
static volatile u32 ResetCount;
static BOOL FirstTimeInBootrom;
static u32 MotorState;
static int DVDInitialized;
void (*LastState)(DVDCommandBlock*);

// prototypes
static void stateReadingFST();
static void cbForStateReadingFST(u32 intType);
static void cbForStateError(u32 intType);
static void stateError(u32 error);
static void stateTimeout();
static void stateGettingError();
static u32 CategorizeError(u32 error);
static BOOL CheckCancel(u32 resume);
static void cbForStateGettingError(u32 intType);
static void cbForUnrecoveredError(u32 intType);
static void cbForUnrecoveredErrorRetry(u32 intType);
static void stateGoToRetry();
static void cbForStateGoToRetry(u32 intType);
static void stateCheckID();
static void stateCheckID3();
static void stateCheckID2a();
static void stateCheckID2();
static void cbForStateCheckID1(u32 intType);
static void cbForStateCheckID2(u32 intType);
static void cbForStateCheckID3(u32 intType);
static void cbForStateCheckID2a(u32 intType);
static void AlarmHandler(OSAlarm* alarm, OSContext* context);
static void stateCoverClosed();
static void stateCoverClosed_CMD(DVDCommandBlock* command);
static void cbForStateCoverClosed(u32 intType);
static void stateMotorStopped();
static void cbForStateMotorStopped(u32 intType);
static void stateReady();
static void stateBusy(DVDCommandBlock* block);
static BOOL IsImmCommandWithResult(u32 command);
static int IsDmaCommand(u32 command);
static void cbForStateBusy(u32 intType);
static int issueCommand(s32 prio, DVDCommandBlock* block);
static void cbForCancelStreamSync(s32 result, DVDCommandBlock* block);
static void cbForStopStreamAtEndSync(s32 result, DVDCommandBlock* block);
static void cbForGetStreamErrorStatusSync(s32 result, DVDCommandBlock* block);
static void cbForGetStreamPlayAddrSync(s32 result, DVDCommandBlock* block);
static void cbForGetStreamStartAddrSync(s32 result, DVDCommandBlock* block);
static void cbForGetStreamLengthSync(s32 result, DVDCommandBlock* block);
static void cbForChangeDiskSync(s32 result, DVDCommandBlock* block);
static void cbForStopMotorSync(s32 result, DVDCommandBlock* block);
static void cbForInquirySync(s32 result, DVDCommandBlock* block);
static void cbForCancelSync(s32 result, DVDCommandBlock* block);
static void cbForCancelAllSync(s32 result, DVDCommandBlock* block);

static void defaultOptionalCommandChecker(DVDCommandBlock*, DVDCommandCheckerCallback) {}

DVDCommandChecker __DVDSetOptionalCommandChecker(DVDCommandChecker func) {
    DVDCommandChecker old = checkOptionalCommand;
    checkOptionalCommand = func;
    return checkOptionalCommand;
}

void DVDInit(void) {
    if (!DVDInitialized) {
        OSRegisterVersion(__DVDVersion);
        DVDInitialized = TRUE;

        __DVDFSInit();
        __DVDClearWaitingQueue();
        __DVDInitWA();

        MotorState = 0;
        bootInfo = (void*)OSPhysicalToCached(0);
        IDShouldBe = &bootInfo->DVDDiskID;

        __OSSetInterruptHandler(0x15, __DVDInterruptHandler);
        __OSUnmaskInterrupts(0x400);
        OSInitThreadQueue(&__DVDThreadQueue);
        __DIRegs[0] = 0x2A;
        __DIRegs[1] = 0;

        if (bootInfo->magic == 0xE5207C22) {
            OSReport("load fst\n");
            __fstLoad();
        } else if (bootInfo->magic == 0x0D15EA5E) {

        } else {
            FirstTimeInBootrom = TRUE;
        }
    }
}

static void stateReadingFST() {
    LastState = stateReadingFST;
    ASSERTLINE(652, ((u32)(bootInfo->FSTLocation) & (32 - 1)) == 0);
    DVD_ASSERTMSGLINE(661, bootInfo->FSTMaxLength >= BB2.FSTLength, "DVDChangeDisk(): FST in the new disc is too big.   ");
    DVDLowRead(bootInfo->FSTLocation, (u32)(BB2.FSTLength + 0x1F) & 0xFFFFFFE0, BB2.FSTPosition, cbForStateReadingFST);
}

static u32 DmaCommand[1] = {0xFFFFFFFF};

static void cbForStateReadingFST(u32 intType) {
    DVDCommandBlock* finished;

    if (intType == 16) {
        stateTimeout();
        return;
    }

    ASSERTLINE(680, (intType & DVD_INTTYPE_CVR) == 0);

    if (intType & DVD_INTTYPE_TC) {
        ASSERTLINE(685, (intType & DVD_INTTYPE_DE) == 0);
        NumInternalRetry = 0;

        __DVDFSInit();
        finished = executing;
        executing = &DummyCommandBlock;
        finished->state = DVD_STATE_END;

        if (finished->callback) {
            finished->callback(0, finished);
        }

        stateReady();
        return;
    }

    ASSERTLINE(712, intType == DVD_INTTYPE_DE);
    stateGettingError();
}

static void cbForStateError(u32 intType) {
	DVDCommandBlock* finished;
    executing->state = -1;

	if (intType == 16) {
		stateTimeout();
		return;
	}

    __DVDPrintFatalMessage();

	FatalErrorFlag = TRUE;
	finished = executing;
	executing = &DummyCommandBlock;

	if (finished->callback) {
		(finished->callback)(-1, finished);
	}

	if (Canceling) {
		Canceling = FALSE;
		if (CancelCallback)
			(CancelCallback)(0, finished);
	}

	stateReady();
}

static void stateError(u32 error) {
    __DVDStoreErrorCode(error);
    DVDLowStopMotor(&cbForStateError);
}

static void stateTimeout() {
    __DVDStoreErrorCode(0x01234568);
    DVDReset();
    cbForStateError(0);
}

static void stateGettingError() {
    DVDLowRequestError(cbForStateGettingError);
}

static u32 CategorizeError(u32 error) {
    if (error == 0x20400) {
        LastError = error;
        return 1;
    }
    
    error &= 0x00FFFFFF;
    if (error == 0x62800 || error == 0x23A00 || error == 0xB5A01) {
        return 0;
    }

    NumInternalRetry++;
    if (NumInternalRetry == 2) {
        if (error == LastError) {
            LastError = error;
            return 1;
        }
        LastError = error;
        return 2;
    }

    LastError = error;

    if (error == 0x31100 || executing->command == DVD_COMMAND_READID) {
        return 2;
    }

    return 3;
}

static BOOL CheckCancel(u32 resume) {
	DVDCommandBlock* finished;

	if (Canceling) {
		ResumeFromHere = resume;
		Canceling = FALSE;

		finished = executing;
		executing = &DummyCommandBlock;

		finished->state = 10;

		if (finished->callback)
			(*finished->callback)(-3, finished);

		if (CancelCallback)
			(CancelCallback)(0, finished);

		stateReady();
		return TRUE;
	}

	return FALSE;
}

static void cbForStateGettingError(u32 intType) {
	u32 error;
	u32 status;
	u32 errorCategory;
	u32 resume;

	if (intType == 16) {
		stateTimeout();
		return;
	}

	if (intType & 2) {
		stateError(0x1234567);
		return;
	}

    ASSERTLINE(956, intType == DVD_INTTYPE_TC);

	error = __DIRegs[8];
	status = error & 0xff000000;

	errorCategory = CategorizeError(error);

	if (errorCategory == 1) {
		stateError(error);
		return;
	}

	if (errorCategory == 2 || errorCategory == 3) {
		resume = 0;
	} else {
		if (status == 0x01000000)
			resume = 4;
		else if (status == 0x02000000)
			resume = 6;
		else if (status == 0x03000000)
			resume = 3;
		else
			resume = 5;
	}

	if (CheckCancel(resume))
		return;

	if (errorCategory == 2) {
		__DVDStoreErrorCode(error);
		stateGoToRetry();
		return;
	}

	if (errorCategory == 3) {
		if ((error & 0x00ffffff) == 0x00031100) {
			DVDLowSeek(executing->offset, cbForUnrecoveredError);
		} else {
			LastState(executing);
		}
		return;
	}

	if (status == 0x01000000) {
		executing->state = 5;
		stateMotorStopped();
		return;
	} else if (status == 0x02000000) {
		executing->state = 3;
		stateCoverClosed();
		return;
	} else if (status == 0x03000000) {
		executing->state = 4;
		stateMotorStopped();
		return;
	} else {
		stateError(0x1234567);
		return;
	}
}

static void cbForUnrecoveredError(u32 intType) {
	if (intType == 16) {
		stateTimeout();
		return;
	}

	if (intType & 1) {
		stateGoToRetry();
		return;
	}

    ASSERTLINE(1055, intType == DVD_INTTYPE_DE);
	DVDLowRequestError(cbForUnrecoveredErrorRetry);
}

static void cbForUnrecoveredErrorRetry(u32 intType) {
	if (intType == 0x10) {
        stateTimeout();
	} else {
        if (intType & 2) {
            stateError(0x01234567);
            return;
        }

        stateError(__DIRegs[8]);
    }
}

static void stateGoToRetry() {
    DVDLowStopMotor(cbForStateGoToRetry);
}

static void cbForStateGoToRetry(u32 intType) {
	if (intType == 16) {
		stateTimeout();
		return;
	}

	if (intType & 2) {
		stateError(0x1234567);
		return;
	}

    ASSERTLINE(1104, intType == DVD_INTTYPE_TC);
	NumInternalRetry = 0;

	if (CurrCommand == 4 || CurrCommand == 5 || CurrCommand == 13 || CurrCommand == 15) {
		ResetRequired = TRUE;
	}

	if (!CheckCancel(2)) {
		executing->state = 11;
		stateMotorStopped();
	}
}

static void stateCheckID() {
    switch(CurrCommand) {
    case DVD_COMMAND_CHANGE_DISK:
        if (DVDCompareDiskID(&CurrDiskID, executing->id)) {
            memcpy(IDShouldBe, &CurrDiskID, sizeof(DVDDiskID));
            executing->state = DVD_STATE_BUSY;
            DCInvalidateRange(&BB2.bootFilePosition, 0x20);
            LastState = stateCheckID2a;
            stateCheckID2a(executing);
        } else {
            DVDLowStopMotor(cbForStateCheckID1);
        }
        break;
    default:
        if (memcmp(&CurrDiskID, IDShouldBe, sizeof(DVDDiskID)) != 0) {
            DVDLowStopMotor(cbForStateCheckID1);
        } else {
            LastState = stateCheckID3;
            stateCheckID3(executing);
        }
        break;
    }
}

static void stateCheckID3() {
    DVDLowAudioBufferConfig(IDShouldBe->streaming, 0xA, cbForStateCheckID3);
}

static void stateCheckID2a() {
    DVDLowAudioBufferConfig(IDShouldBe->streaming, 0xA, cbForStateCheckID2a);
}

static void cbForStateCheckID2a(u32 intType) {
    if (intType == 16) {
		stateTimeout();
		return;
	}

    ASSERTLINE(1227, (intType & DVD_INTTYPE_CVR) == 0);

    if (intType & DVD_INTTYPE_TC) {
        ASSERTLINE(1232, (intType & DVD_INTTYPE_DE) == 0);
        NumInternalRetry = 0;
        stateCheckID2(executing);
        return;
    }

    ASSERTLINE(1243, intType == DVD_INTTYPE_DE);
    stateGettingError();
}

static void stateCheckID2() {
    DVDLowRead(&BB2, 0x20, 0x420, cbForStateCheckID2);
}

static void cbForStateCheckID1(u32 intType) {
    if (intType == 16) {
		stateTimeout();
		return;
	}

    if (intType & DVD_INTTYPE_DE) {
        stateError(0x01234567);
        return;
    }

    ASSERTLINE(1279, intType == DVD_INTTYPE_TC);
    NumInternalRetry = 0;

    if (CheckCancel(1) == FALSE) {
        executing->state = DVD_STATE_WRONG_DISK;
        stateMotorStopped();
    }
}

static void cbForStateCheckID2(u32 intType) {
    if (intType == 16) {
		stateTimeout();
		return;
	}

    ASSERTLINE(1300, (intType & DVD_INTTYPE_CVR) == 0);

    if (intType & DVD_INTTYPE_TC) {
        ASSERTLINE(1305, (intType & DVD_INTTYPE_DE) == 0);
        NumInternalRetry = 0;
        stateReadingFST();
        return;
    }

    ASSERTLINE(1321, intType == DVD_INTTYPE_DE);
    stateGettingError();
}

static void cbForStateCheckID3(u32 intType) {
    if (intType == 16) {
		stateTimeout();
		return;
	}

    ASSERTLINE(1336, (intType & DVD_INTTYPE_CVR) == 0);

    if (intType & DVD_INTTYPE_TC) {
        ASSERTLINE(1341, (intType & DVD_INTTYPE_DE) == 0);
        NumInternalRetry = 0;
        if (CheckCancel(0) == FALSE) {
            executing->state = DVD_STATE_BUSY;
            stateBusy(executing);
        }
        return;
    }

    ASSERTLINE(1355, intType == DVD_INTTYPE_DE);
    stateGettingError();
}

static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    DVDReset();
    DCInvalidateRange(&CurrDiskID, sizeof(DVDDiskID));
    LastState = &stateCoverClosed_CMD;
    stateCoverClosed_CMD(executing);
}

static void stateCoverClosed() {
    DVDCommandBlock* finished;

    switch(CurrCommand) {
    case DVD_COMMAND_BSREAD:
    case DVD_COMMAND_READID:
    case DVD_COMMAND_AUDIO_BUFFER_CONFIG:
    case DVD_COMMAND_BS_CHANGE_DISK:
        __DVDClearWaitingQueue();
        finished = executing;
        executing = &DummyCommandBlock;
        if (finished->callback) {
            finished->callback(-4, finished);
        }
        stateReady();
        break;
    default:
        MotorState = 0;
        DVDReset();
        OSCreateAlarm(&ResetAlarm);
        OSSetAlarm(&ResetAlarm, OSMillisecondsToTicks(1150), &AlarmHandler);
        break;
    }
}

static void stateCoverClosed_CMD(DVDCommandBlock* command) {
    DVDLowReadDiskID(&CurrDiskID, cbForStateCoverClosed);
}

static void cbForStateCoverClosed(u32 intType) {
    if (intType == 16) {
		stateTimeout();
		return;
	}

    ASSERTLINE(1437, (intType & DVD_INTTYPE_CVR) == 0);

    if (intType & DVD_INTTYPE_TC) {
        ASSERTLINE(1442, (intType & DVD_INTTYPE_DE) == 0);
        NumInternalRetry = 0;
        stateCheckID();
        return;
    }

    ASSERTLINE(1454, intType == DVD_INTTYPE_DE);
    stateGettingError();
}

static void stateMotorStopped() {
    DVDLowWaitCoverClose(cbForStateMotorStopped);
}

static void cbForStateMotorStopped(u32 intType) {
    ASSERTLINE(1483, intType == DVD_INTTYPE_CVR);
    __DIRegs[1] = 0;
    executing->state = DVD_STATE_COVER_CLOSED;
    stateCoverClosed();
}

static void stateReady() {
    if (__DVDCheckWaitingQueue() == 0) {
        executing = NULL;
        return;
    }

    if (PauseFlag != 0) {
        PausingFlag = 1;
        executing = NULL;
        return;
    }

    executing = __DVDPopWaitingQueue();

    if (FatalErrorFlag) {
        DVDCommandBlock* finished;

        executing->state = DVD_STATE_FATAL_ERROR;
        finished = executing;
        executing = &DummyCommandBlock;
        if (finished->callback) {
            (*finished->callback)(-1, finished);
        }

        stateReady();
        return;
    }

    CurrCommand = executing->command;
    if (ResumeFromHere != 0) {
        switch (ResumeFromHere) {
        case 2:
            executing->state = DVD_STATE_RETRY;
            stateMotorStopped();
            break;
        case 3:
            executing->state = DVD_STATE_NO_DISK;
            stateMotorStopped();
            break;
        case 4:
            executing->state = DVD_STATE_COVER_OPEN;
            stateMotorStopped();
            break;
        case 1:
        case 6:
        case 7:
            executing->state = DVD_STATE_COVER_CLOSED;
            stateCoverClosed();
            break;
        case 5:
            stateError(CancelLastError);
            break;
        }

        ResumeFromHere = 0;
        return;
    }

    if (MotorState == 0) {
        executing->state = DVD_STATE_BUSY;
        stateBusy(executing);
    } else {
        stateCoverClosed();
    }
}

static void stateBusy(DVDCommandBlock* block) {
    DVDCommandBlock* finished;
    LastState = stateBusy;

    switch(block->command) {
    case DVD_COMMAND_READID:
        __DIRegs[1] = __DIRegs[1];
        block->currTransferSize = 0x20;
        DVDLowReadDiskID(block->addr, cbForStateBusy);
        return;
    case DVD_COMMAND_READ:
    case DVD_COMMAND_BSREAD:
        if (block->length == 0) {
            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = 0;

            if (finished->callback != 0) {
                (*finished->callback)(0, finished);
            }

            stateReady();
        } else {
            __DIRegs[1] = __DIRegs[1];
            block->currTransferSize = (block->length - block->transferredSize > 0x80000) ? 0x80000 : (block->length - block->transferredSize);
            DVDLowRead((char*)block->addr + block->transferredSize, block->currTransferSize, block->offset + block->transferredSize, cbForStateBusy);
        }
        return;
    case DVD_COMMAND_SEEK:
        __DIRegs[1] = __DIRegs[1];
        DVDLowSeek(block->offset, cbForStateBusy);
        return;
    case DVD_COMMAND_CHANGE_DISK:
        DVDLowStopMotor(cbForStateBusy);
        return;
    case DVD_COMMAND_BS_CHANGE_DISK:
        DVDLowStopMotor(cbForStateBusy);
        return;
    case DVD_COMMAND_INITSTREAM:
        __DIRegs[1] = __DIRegs[1];
        if (AutoFinishing != 0) {
            executing->currTransferSize = 0;
            DVDLowRequestAudioStatus(0, cbForStateBusy);
            return;
        }
        executing->currTransferSize = 1;
        DVDLowAudioStream(0, block->length, block->offset, cbForStateBusy);
        return;
    case DVD_COMMAND_CANCELSTREAM:
        __DIRegs[1] = __DIRegs[1];
        DVDLowAudioStream(0x10000, 0U, 0U, cbForStateBusy);
        return;
    case DVD_COMMAND_STOP_STREAM_AT_END:
        __DIRegs[1] = __DIRegs[1];
        AutoFinishing = 1;
        DVDLowAudioStream(0, 0U, 0U, cbForStateBusy);
        return;
    case DVD_COMMAND_REQUEST_AUDIO_ERROR:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0, cbForStateBusy);
        return;
    case DVD_COMMAND_REQUEST_PLAY_ADDR:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0x10000, cbForStateBusy);
        return;
    case DVD_COMMAND_REQUEST_START_ADDR:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0x20000, cbForStateBusy);
        return;
    case DVD_COMMAND_REQUEST_LENGTH:
        __DIRegs[1] = __DIRegs[1];
        DVDLowRequestAudioStatus(0x30000, cbForStateBusy);
        return;
    case DVD_COMMAND_AUDIO_BUFFER_CONFIG:
        __DIRegs[1] = __DIRegs[1];
        DVDLowAudioBufferConfig(block->offset, block->length, cbForStateBusy);
        return;
    case DVD_COMMAND_INQUIRY:
        __DIRegs[1] = __DIRegs[1];
        block->currTransferSize = 0x20;
        DVDLowInquiry(block->addr, cbForStateBusy);
        return;
    case DVD_COMMAND_UNK_16:
        __DIRegs[1] = __DIRegs[1];
        DVDLowStopMotor(cbForStateBusy);
        return;
    default:
        checkOptionalCommand(block, cbForStateBusy);
        return;
    }
}

static u32 ImmCommand[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

void __DVDSetImmCommand(u32 command) {
    static u32 immCount;
    ASSERTLINE(1790, immCount < sizeof(ImmCommand)/sizeof(ImmCommand[0]));
    ImmCommand[immCount++] = command;
}

void __DVDSetDmaCommand(u32 command) {
    static u32 dmaCount;
    ASSERTLINE(1798, dmaCount < sizeof(DmaCommand)/sizeof(DmaCommand[0]));
    DmaCommand[dmaCount++] = command;
}

static BOOL IsImmCommandWithResult(u32 command) {
    u32 i;

    if (command == 9 || command == 10 || command == 11 || command == 12) {
        return 1;
    }

    for (i = 0; i < 3; i++) {
        if (command == ImmCommand[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static int IsDmaCommand(u32 command) {
    u32 i;

    if (command == 1 || command == 4 || command == 5 || command == 14) {
        return 1;
    }

    for (i = 0; i < 1; i++) {
        if (command == DmaCommand[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

static void cbForStateBusy(u32 intType) {
    DVDCommandBlock* finished;
    s32 result;

    if (intType == 16) {
		stateTimeout();
		return;
	}

    if ((CurrCommand == DVD_COMMAND_CHANGE_DISK) || (CurrCommand == DVD_COMMAND_BS_CHANGE_DISK)) {
        if (intType & DVD_INTTYPE_DE) {
            stateError(0x01234567);
            return;
        }

        ASSERTLINE(1857, intType == DVD_INTTYPE_TC);
        NumInternalRetry = 0;

        if (CurrCommand == DVD_COMMAND_BS_CHANGE_DISK) {
            ResetRequired = 1;
        }

        if (CheckCancel(7) == FALSE) {
            executing->state = DVD_STATE_MOTOR_STOPPED;
            stateMotorStopped();
        }
        return;
    }

    ASSERTLINE(1877, (intType & DVD_INTTYPE_CVR) == 0);

    if (IsDmaCommand(CurrCommand)) {
        executing->transferredSize += executing->currTransferSize - __DIRegs[6];
    }

    if (intType & 8) {
        Canceling = 0;
        finished = executing;
        executing = &DummyCommandBlock;
        finished->state = DVD_STATE_CANCELED;

        if (finished->callback) {
            finished->callback(-3, finished);
        }

        if (CancelCallback) {
            CancelCallback(0, finished);
        }

        stateReady();
        return;
    }

    if (intType & 1) {
        ASSERTLINE(1915, (intType & DVD_INTTYPE_DE) == 0);
        NumInternalRetry = 0;

        if (CurrCommand == 0x10) {
            MotorState = 1;
            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = 0;

            if (finished->callback != 0) {
                (*finished->callback)(0, finished);
            }

            stateReady();
            return;
        }

        if (CheckCancel(0) != FALSE) {
            return;
        }

        if (IsDmaCommand(CurrCommand)) {
            if (executing->transferredSize != executing->length) {
                stateBusy(executing);
                return;
            }

            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = DVD_STATE_END;
            if (finished->callback) {
                finished->callback(finished->transferredSize, finished);
            }

            stateReady();
            return;
        } else if (IsImmCommandWithResult(CurrCommand)) {
            if (CurrCommand == DVD_COMMAND_REQUEST_START_ADDR || CurrCommand == DVD_COMMAND_REQUEST_PLAY_ADDR) {
                result = __DIRegs[8] * 4;
            } else {
                result = __DIRegs[8];
            }

            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = DVD_STATE_END;
            if (finished->callback) {
                finished->callback(result, finished);
            }

            stateReady();
            return;
        } else if (CurrCommand == DVD_COMMAND_INITSTREAM) {
            if (executing->currTransferSize == 0) {
                if (__DIRegs[8] & 1) {
                    finished = executing;
                    executing = &DummyCommandBlock;
                    finished->state = DVD_STATE_IGNORED;
                    if (finished->callback) {
                        finished->callback(-2, finished);
                    }
                    stateReady();
                    return;
                }

                AutoFinishing = 0;
                executing->currTransferSize = 1;
                DVDLowAudioStream(0, executing->length, executing->offset, cbForStateBusy);
                return;
            }

            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = DVD_STATE_END;
            if (finished->callback) {
                finished->callback(0, finished);
            }

            stateReady();
            return;
        } else {
            finished = executing;
            executing = &DummyCommandBlock;
            finished->state = DVD_STATE_END;
            if (finished->callback) {
                finished->callback(0, finished);
            }

            stateReady();
            return;
        }
    } else {
        ASSERTLINE(2063, intType == DVD_INTTYPE_DE);

        if (CurrCommand == 14) {
			stateError(0x01234567);
			return;
		}

		if ((CurrCommand == 1 || CurrCommand == 4 || CurrCommand == 5 || CurrCommand == 14)
		    && (executing->transferredSize == executing->length)) {

			if (CheckCancel(0)) {
				return;
			}
			finished  = executing;
			executing = &DummyCommandBlock;

			finished->state = DVD_STATE_END;
			if (finished->callback) {
				(finished->callback)((s32)finished->transferredSize, finished);
			}
			stateReady();
			return;
		}

        stateGettingError();
    }
}

void* __DVDGetIssueCommandAddr(void) {
    return issueCommand;
}

static int issueCommand(s32 prio, DVDCommandBlock* block) {
    BOOL level;
    int result;

    if (autoInvalidation != 0 && (block->command == DVD_COMMAND_READ || block->command == DVD_COMMAND_BSREAD 
        || block->command == DVD_COMMAND_READID || block->command == DVD_COMMAND_INQUIRY)) {
        DCInvalidateRange(block->addr, block->length);
    }

    level = OSDisableInterrupts();
#if DEBUG
    if (executing == block || block->state == DVD_STATE_WAITING && __DVDIsBlockInWaitingQueue(block)) {
        ASSERTMSGLINE(2151, FALSE, "DVD library: Specified command block (or file info) is already in use\n");
    }
#endif

    block->state = DVD_STATE_WAITING;
    result = __DVDPushWaitingQueue(prio, block);
    if (executing == NULL && PauseFlag == 0) {
        stateReady();
    }

    OSRestoreInterrupts(level);
    return result;
}

int DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* addr, s32 length, s32 offset, DVDCBCallback callback, s32 prio) {
    int idle;

    ASSERTMSGLINE(2191, block, "DVDReadAbsAsync(): null pointer is specified to command block address.");
    ASSERTMSGLINE(2192, addr, "DVDReadAbsAsync(): null pointer is specified to addr.");
    ASSERTMSGLINE(2194, !OFFSET(addr, 32), "DVDReadAbsAsync(): address must be aligned with 32 byte boundary.");
    ASSERTMSGLINE(2196, !(length & (32-1)), "DVDReadAbsAsync(): length must be a multiple of 32.");
    ASSERTMSGLINE(2198, !(offset & (4-1)), "DVDReadAbsAsync(): offset must be a multiple of 4.");
    ASSERTMSGLINE(2200, length >= 0, "DVD read: negative value was specified to length of the read\n");
    
    block->command = DVD_COMMAND_READ;
    block->addr = addr;
    block->length = length;
    block->offset = offset;
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(prio, block);
    ASSERTMSGLINE(2210, idle, "DVDReadAbsAsync(): command block is used for processing previous request.");
    return idle;
}

int DVDSeekAbsAsyncPrio(DVDCommandBlock* block, s32 offset, DVDCBCallback callback, s32 prio) {
    int idle;

    ASSERTMSGLINE(2233, block, "DVDSeekAbs(): null pointer is specified to command block address.");
    ASSERTMSGLINE(2235, !(offset & (4-1)), "DVDSeekAbs(): offset must be a multiple of 4.");
    
    block->command = DVD_COMMAND_SEEK;
    block->offset = offset;
    block->callback = callback;

    idle = issueCommand(prio, block);
    ASSERTMSGLINE(2242, idle, "DVDSeekAbs(): command block is used for processing previous request.");
    return idle;
}

int DVDReadAbsAsyncForBS(DVDCommandBlock* block, void* addr, s32 length, s32 offset, DVDCBCallback callback) {
    int idle;

    ASSERTMSGLINE(2272, block, "DVDReadAbsAsyncForBS(): null pointer is specified to command block address.");
    ASSERTMSGLINE(2273, addr, "DVDReadAbsAsyncForBS(): null pointer is specified to addr.");
    ASSERTMSGLINE(2275, !OFFSET(addr, 32), "DVDReadAbsAsyncForBS(): address must be aligned with 32 byte boundary.");
    ASSERTMSGLINE(2277, !(length & (32-1)), "DVDReadAbsAsyncForBS(): length must be a multiple of 32.");
    ASSERTMSGLINE(2279, !(offset & (4-1)), "DVDReadAbsAsyncForBS(): offset must be a multiple of 4.");
    
    block->command = DVD_COMMAND_BSREAD;
    block->addr = addr;
    block->length = length;
    block->offset = offset;
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(2, block);
    ASSERTMSGLINE(2289, idle, "DVDReadAbsAsyncForBS(): command block is used for processing previous request.");
    return idle;
}

int DVDReadDiskID(DVDCommandBlock* block, DVDDiskID* diskID, DVDCBCallback callback) {
    int idle;

    ASSERTMSGLINE(2312, block, "DVDReadDiskID(): null pointer is specified to command block address.");
    ASSERTMSGLINE(2313, diskID, "DVDReadDiskID(): null pointer is specified to id address.");
    ASSERTMSGLINE(2315, !OFFSET(diskID, 32), "DVDReadDiskID(): id must be aligned with 32 byte boundary.");

    block->command = DVD_COMMAND_READID;
    block->addr = diskID;
    block->length = 0x20;
    block->offset = 0;
    block->transferredSize = 0;
    block->callback = callback;

    idle = issueCommand(2, block);
    ASSERTMSGLINE(2325, idle, "DVDReadDiskID(): command block is used for processing previous request.");
    return idle;
}

int DVDPrepareStreamAbsAsync(DVDCommandBlock* block, u32 length, u32 offset, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_INITSTREAM;
    block->length = length;
    block->offset = offset;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

int DVDCancelStreamAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_CANCELSTREAM;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDCancelStream(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDCancelStreamAsync(block, cbForCancelStreamSync);
    if (result == 0) {
        return -1; 
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            retVal = block->transferredSize;
            break;
        } 

        OSSleepThread(&__DVDThreadQueue);
    }
    
    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForCancelStreamSync(s32 result, DVDCommandBlock* block) {
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDStopStreamAtEndAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_STOP_STREAM_AT_END;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDStopStreamAtEnd(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDStopStreamAtEndAsync(block, cbForStopStreamAtEndSync);
    if (result == 0) {
        return -1; 
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            retVal = block->transferredSize;
            break;
        } 

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForStopStreamAtEndSync(s32 result, DVDCommandBlock* block) {
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDGetStreamErrorStatusAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_REQUEST_AUDIO_ERROR;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDGetStreamErrorStatus(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDGetStreamErrorStatusAsync(block, cbForGetStreamErrorStatusSync);
    if (result == 0) {
        return -1;
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            retVal = block->transferredSize;
            break;
        } 

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForGetStreamErrorStatusSync(s32 result, DVDCommandBlock* block) {
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDGetStreamPlayAddrAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_REQUEST_PLAY_ADDR;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDGetStreamPlayAddr(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDGetStreamPlayAddrAsync(block, cbForGetStreamPlayAddrSync);
    if (result == 0) {
        return -1;
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            retVal = block->transferredSize;
            break;
        } 

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForGetStreamPlayAddrSync(s32 result, DVDCommandBlock* block) {
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDGetStreamStartAddrAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_REQUEST_START_ADDR;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDGetStreamStartAddr(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDGetStreamStartAddrAsync(block, cbForGetStreamStartAddrSync);
    if (result == 0) {
        return -1;
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            retVal = block->transferredSize;
            break;
        } 

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForGetStreamStartAddrSync(s32 result, DVDCommandBlock* block) {
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDGetStreamLengthAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_REQUEST_LENGTH;
    block->callback = callback;
    idle = issueCommand(1, block);
    return idle;
}

s32 DVDGetStreamLength(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDGetStreamLengthAsync(block, cbForGetStreamLengthSync);
    if (result == 0) {
        return -1;
    }
    enabled = OSDisableInterrupts();

    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            retVal = block->transferredSize;
            break;
        } 

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForGetStreamLengthSync(s32 result, DVDCommandBlock* block) {
    block->transferredSize = (u32)result;
    OSWakeupThread(&__DVDThreadQueue);
}

void __DVDAudioBufferConfig(DVDCommandBlock* block, u32 enable, u32 size, DVDCBCallback callback) {
    int idle;

    block->command = DVD_COMMAND_AUDIO_BUFFER_CONFIG;
    block->offset = enable;
    block->length = size;
    block->callback = callback;
    idle = issueCommand(2, block);
}

int DVDChangeDiskAsyncForBS(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;

    ASSERTMSGLINE(2869, block, "DVDChangeDiskAsyncForBS(): null pointer is specified to command block address.");
    
    block->command = DVD_COMMAND_BS_CHANGE_DISK;
    block->callback = callback;
    idle = issueCommand(2, block);
    ASSERTMSGLINE(2875, idle, "DVDChangeDiskAsyncForBS(): command block is used for processing previous request.");
    return idle;
}

int DVDChangeDiskAsync(DVDCommandBlock* block, DVDDiskID* id, DVDCBCallback callback) {
    int idle;

    ASSERTMSGLINE(2896, block, "DVDChangeDisk(): null pointer is specified to command block address.");
    ASSERTMSGLINE(2897, id, "DVDChangeDisk(): null pointer is specified to id address.");

    if (id->company[0] == 0) {
        OSReport("DVDChangeDiskAsync(): You can't specify NULL to company name.  \n");
        DVD_ASSERTMSGLINE(2902, 0, "");
    }

    block->command = DVD_COMMAND_CHANGE_DISK;
    block->id = id;
    block->callback = callback;
    DCInvalidateRange(bootInfo->FSTLocation, bootInfo->FSTMaxLength);
    
    idle = issueCommand(2, block);
    ASSERTMSGLINE(2913, idle, "DVDChangeDisk(): command block is used for processing previous request.");
    return idle;
}

s32 DVDChangeDisk(DVDCommandBlock* block, DVDDiskID* id) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDChangeDiskAsync(block, id, cbForChangeDiskSync);
    if (result == 0) {
        return -1;
    }

    enabled = OSDisableInterrupts();
    while (1) {
        state = block->state;
        if (state == DVD_STATE_END) {
            retVal = 0;
            break;
        } else if (state == DVD_STATE_FATAL_ERROR) {
            retVal = -1;
            break;
        } else if (state == DVD_STATE_CANCELED) {
            retVal = -3;
            break;
        }
        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForChangeDiskSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDStopMotorAsync(DVDCommandBlock* block, DVDCBCallback callback) {
    int idle;
    ASSERTMSGLINE(2996, block, "DVDStopMotor(): Null address was specified for block");

    block->command = DVD_COMMAND_UNK_16;
    block->callback = callback;
    
    idle = issueCommand(2, block);
    ASSERTMSGLINE(3002, idle, "DVDStopMotor(): command block is used for processing previous request.");
    return idle;
}

s32 DVDStopMotor(DVDCommandBlock* block) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDStopMotorAsync(block, cbForStopMotorSync);
    if (result == 0) {
        return -1;
    }

    enabled = OSDisableInterrupts();
    while (1) {
        state = block->state;
        if (state == DVD_STATE_END) {
            retVal = 0;
            break;
        } else if (state == DVD_STATE_FATAL_ERROR) {
            retVal = -1;
            break;
        } else if (state == DVD_STATE_CANCELED) {
            retVal = -3;
            break;
        }
        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForStopMotorSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info, DVDCBCallback callback) {
    int idle;

    ASSERTMSGLINE(3085, block, "DVDInquiry(): Null address was specified for block");
    ASSERTMSGLINE(3086, info, "DVDInquiry(): Null address was specified for info");
    ASSERTMSGLINE(3088, !OFFSET(info, 32), "DVDInquiry(): Address for info is not 32 bytes aligned");

    block->command = DVD_COMMAND_INQUIRY;
    block->addr = info;
    block->length = 0x20;
    block->transferredSize = 0;
    block->callback = callback;
    idle = issueCommand(2, block);
    return idle;
}

s32 DVDInquiry(DVDCommandBlock* block, DVDDriveInfo* info) {
    int result;
    s32 state;
    BOOL enabled;
    s32 retVal;

    result = DVDInquiryAsync(block, info, cbForInquirySync);
    if (result == 0) {
        return -1;
    }

    enabled = OSDisableInterrupts();
    while (1) {
        state = block->state;
        if (state == DVD_STATE_END) {
            retVal = (u32)block->transferredSize;
            break;
        } else if (state == DVD_STATE_FATAL_ERROR) { 
            retVal = -1;
            break;
        } else if (state == DVD_STATE_CANCELED) {
            retVal = -3;
            break;
        }
        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

static void cbForInquirySync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

void DVDReset(void) {
    DVDLowReset();
    __DIRegs[0] = 0x2A;
    __DIRegs[1] = __DIRegs[1];
    ResetRequired = 0;
    ResumeFromHere = 0;
}

int DVDResetRequired(void) {
    return ResetRequired;
}

s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block) {
    BOOL enabled;
    s32 retVal;

    ASSERTMSGLINE(3197, block, "DVDGetCommandBlockStatus(): null pointer is specified to command block address.");
    enabled = OSDisableInterrupts();

	if (block->state == 3) {
		retVal = 1;
	} else {
		retVal = block->state;
	}

	OSRestoreInterrupts(enabled);
	return retVal;
}

s32 DVDGetDriveStatus(void) {
	BOOL enabled = OSDisableInterrupts();
	s32 retVal;
    
	if (FatalErrorFlag != FALSE) {
		retVal = DVD_STATE_FATAL_ERROR;
	} else {
		if (PausingFlag != FALSE) {
			retVal = DVD_STATE_PAUSING;
		} else {
			if (executing == NULL) {
				retVal = DVD_STATE_END;
			} else if (executing == &DummyCommandBlock) {
				retVal = DVD_STATE_END;
			} else {
				retVal = DVDGetCommandBlockStatus((DVDCommandBlock*)executing);
			}
		}
	}
	OSRestoreInterrupts(enabled);
	return retVal;
}

BOOL DVDSetAutoInvalidation(BOOL autoInval) {
    BOOL prev;

    prev = autoInvalidation;
    autoInvalidation = autoInval;
    return prev;
}

void DVDPause(void) {
    BOOL level;

    level = OSDisableInterrupts();
    PauseFlag = 1;
    if (executing == NULL) {
        PausingFlag = 1;
    }
    OSRestoreInterrupts(level);
}

void DVDResume(void) {
    BOOL level;

    level = OSDisableInterrupts();
    PauseFlag = 0;
    if (PausingFlag != 0) {
        PausingFlag = 0;
        stateReady();
    }
    OSRestoreInterrupts(level);
}

int DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback) {
	BOOL enabled;
	DVDLowCallback old;
    DVDCommandBlock* finished;

	enabled = OSDisableInterrupts();

	switch (block->state) {
	case DVD_STATE_FATAL_ERROR:
	case DVD_STATE_END:
	case DVD_STATE_CANCELED:
		if (callback)
			(*callback)(0, block);
		break;
	case DVD_STATE_BUSY:
		if (Canceling) {
			OSRestoreInterrupts(enabled);
			return FALSE;
		}

		Canceling = TRUE;
		CancelCallback = callback;
		if (block->command == DVD_COMMAND_BSREAD || block->command == DVD_COMMAND_READ) {
			DVDLowBreak();
		}
		break;
	case DVD_STATE_WAITING:
		__DVDDequeueWaitingQueue(block);
		block->state = DVD_STATE_CANCELED;

		if (block->callback)
			(block->callback)(-3, block);

		if (callback)
			(*callback)(0, block);
		break;
	case DVD_STATE_COVER_CLOSED:
		switch (block->command) {
		case DVD_COMMAND_READID:
		case DVD_COMMAND_BSREAD:
		case DVD_COMMAND_AUDIO_BUFFER_CONFIG:
		case DVD_COMMAND_BS_CHANGE_DISK:
			if (callback)
				(*callback)(0, block);
			break;

		default:
			if (Canceling) {
				OSRestoreInterrupts(enabled);
				return FALSE;
			}
			Canceling = TRUE;
			CancelCallback = callback;
			break;
		}
		break;
	case DVD_STATE_NO_DISK:
	case DVD_STATE_COVER_OPEN:
	case DVD_STATE_WRONG_DISK:
	case DVD_STATE_MOTOR_STOPPED:
	case DVD_STATE_RETRY:
		old = DVDLowClearCallback();
        ASSERTLINE(3418, old == cbForStateMotorStopped);

		if (old != cbForStateMotorStopped) {
			OSRestoreInterrupts(enabled);
			return FALSE;
		}

		if (block->state == DVD_STATE_NO_DISK)
			ResumeFromHere = 3;
		if (block->state == DVD_STATE_COVER_OPEN)
			ResumeFromHere = 4;
		if (block->state == DVD_STATE_WRONG_DISK)
			ResumeFromHere = 1;
		if (block->state == DVD_STATE_RETRY)
			ResumeFromHere = 2;
		if (block->state == DVD_STATE_MOTOR_STOPPED)
			ResumeFromHere = 7;

        finished = executing;
        executing = &DummyCommandBlock;

		block->state = DVD_STATE_CANCELED;
		if (block->callback) {
			(block->callback)(-3, block);
		}

		if (callback) {
			(callback)(0, block);
		}
		stateReady();
		break;
	}

	OSRestoreInterrupts(enabled);
	return TRUE;
}

s32 DVDCancel(volatile DVDCommandBlock* block) {
    int result;
    s32 state;
    u32 command;
    BOOL enabled;

    result = DVDCancelAsync((void*)block, cbForCancelSync);
    if (result == 0) {
        return -1;
    }

    enabled = OSDisableInterrupts();
    while (1) {
        state = block->state;
        if (state == DVD_STATE_END || state == DVD_STATE_FATAL_ERROR || state == DVD_STATE_CANCELED) {
            break;
        }

        if (state == DVD_STATE_COVER_CLOSED) {
            command = block->command;
            if ((command == DVD_COMMAND_BSREAD) || (command == DVD_COMMAND_READID) || (command == DVD_COMMAND_AUDIO_BUFFER_CONFIG) || (command == DVD_COMMAND_BS_CHANGE_DISK)) {
                break;
            }
        }
        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return 0;
}

static void cbForCancelSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

int DVDCancelAllAsync(DVDCBCallback callback) {
    BOOL enabled;
    DVDCommandBlock* p;
    int retVal;

    enabled = OSDisableInterrupts();
    DVDPause();
    while ((p = __DVDPopWaitingQueue())) {
        DVDCancelAsync(p, NULL);
    }

    if (executing) {
        retVal = DVDCancelAsync(executing, callback);
    } else {
        retVal = 1;
        if (callback) {
            callback(0, NULL);
        }
    }

    DVDResume();
    OSRestoreInterrupts(enabled);
    return retVal;
}

s32 DVDCancelAll(void) {
    int result;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    CancelAllSyncComplete = 0;
    result = DVDCancelAllAsync(cbForCancelAllSync);
    if (result == 0) {
        OSRestoreInterrupts(enabled);
        return -1;
    }

    while (1) {
        if (CancelAllSyncComplete == 0) {
            OSSleepThread(&__DVDThreadQueue);
        } else {
            break;
        }
    }

    OSRestoreInterrupts(enabled);
    return 0;
}

static void cbForCancelAllSync(s32 result, DVDCommandBlock* block) {
    CancelAllSyncComplete = 1;
    OSWakeupThread(&__DVDThreadQueue);
}

DVDDiskID* DVDGetCurrentDiskID(void) {
    return (void*)OSPhysicalToCached(0);
}

BOOL DVDCheckDisk(void) {
	BOOL enabled;
	s32 retVal;
	s32 state;
	u32 coverReg;

	enabled = OSDisableInterrupts();

	if (FatalErrorFlag) {
		state = -1;
	} else if (PausingFlag) {
		state = 8;
	} else {
		if (executing == NULL) {
			state = 0;
		} else if (executing == &DummyCommandBlock) {
			state = 0;
		} else {
			state = executing->state;
		}
	}

	switch (state) {
	case DVD_STATE_BUSY:
	case DVD_STATE_IGNORED:
	case DVD_STATE_CANCELED:
	case DVD_STATE_WAITING:
		retVal = TRUE;
		break;
	case DVD_STATE_FATAL_ERROR:
	case DVD_STATE_RETRY:
	case DVD_STATE_MOTOR_STOPPED:
	case DVD_STATE_COVER_CLOSED:
	case DVD_STATE_NO_DISK:
	case DVD_STATE_COVER_OPEN:
	case DVD_STATE_WRONG_DISK:
		retVal = FALSE;
		break;
	case DVD_STATE_END:
	case DVD_STATE_PAUSING:
		coverReg = __DIRegs[1];
		if (((coverReg >> 2) & 1) || (coverReg & 1)) {
			retVal = FALSE;
		} else if (ResumeFromHere != 0) {
			retVal = FALSE;
		} else {
            retVal = TRUE;
        }
	}

	OSRestoreInterrupts(enabled);
	return retVal;
}

void __DVDPrepareResetAsync(DVDCBCallback callback) {
	BOOL enabled;

	enabled = OSDisableInterrupts();

	__DVDClearWaitingQueue();

	if (Canceling) {
		CancelCallback = callback;
	} else {
		if (executing) {
			executing->callback = NULL;
		}

		DVDCancelAllAsync(callback);
	}

	OSRestoreInterrupts(enabled);
}

int __DVDTestAlarm(const OSAlarm* alarm) {
    if (alarm == &ResetAlarm) {
        return 1;
    }
    return __DVDLowTestAlarm(alarm);
}
