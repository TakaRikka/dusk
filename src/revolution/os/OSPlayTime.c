#include <revolution/ai.h>
#include <revolution/os.h>
#include <revolution/esp.h>
#include <revolution/nand.h>

OSAlarm __OSExpireAlarm;
OSTime __OSExpireTime;
OSPlayTimeCallbackFunc __OSExpireCallback;
BOOL __OSExpireSetExpiredFlag;

static void __OSPlayTimeRebootCallback(OSAlarm *, OSContext *);
static void*  __OSPlayTimeRebootThread(void *);

extern void __OSHotResetForError(void);

typedef struct {
	s16 new_ai_buffer[2][48*2*3];
	s32 idx_ai_buffer;
	s16* src_buffer;
	u32 frames;
	s16 aLeft;
	s16 aRight;
	AIDCallback gameAIDCallback;
} __OSExpireAIFadeStruct;

static __OSExpireAIFadeStruct* __OSExpireAIFade = NULL;

BOOL OSPlayTimeIsLimited() {
    return __OSExpireTime != 0;
}

static void __OSPlayTimeRebootCallback(OSAlarm* alarm, OSContext* context) {
    void* arenaHi;
	u32 memSize;
	OSThread* thread;
	void* stack;
        
    for (thread = __OSActiveThreadQueue.head; thread; thread = thread->linkActive.next) {
        OSSuspendThread(thread);
    }

    memSize = OSRoundUp32B(sizeof(OSThread)) + 0x1000;
    arenaHi = *(void**)OSPhysicalToCached(0x3128);
    arenaHi = (void*)((u32)arenaHi - memSize);

    thread  = (OSThread*)arenaHi;
    arenaHi = (void*)((u32)arenaHi + OSRoundUp32B(sizeof(OSThread)));
    stack   = arenaHi;

    if (!OSCreateThread(thread, __OSPlayTimeRebootThread, NULL, (void*)((u32)stack + 0x1000), 0x1000, 0, 0)) {
        __OSHotResetForError();
    }

    OSResumeThread(thread);
}

void __OSPlayTimeFadeLastAIDCallback(void) {
    if (__OSExpireAIFade->gameAIDCallback) {
        __OSExpireAIFade->gameAIDCallback();
    }

	if (__OSExpireAIFade->frames == 0) {
	    __OSExpireAIFade->src_buffer = (s16*)OSPhysicalToCached(AIGetDMAStartAddr());
	}

	if (__OSExpireAIFade->frames == 1) {
        DCInvalidateRange(__OSExpireAIFade->src_buffer, 4);
    	__OSExpireAIFade->aLeft = (s16)(*__OSExpireAIFade->src_buffer++);
        __OSExpireAIFade->aRight = (s16)(*__OSExpireAIFade->src_buffer);
	}

	if (__OSExpireAIFade->frames >= 1) {
		s16* buffer = __OSExpireAIFade->new_ai_buffer[__OSExpireAIFade->idx_ai_buffer];
        s16* dest = buffer;
        u32 bytes = AIGetDMALength();
        u32 i = bytes;
        f32 delta = 0.995f;

        while (i) {
            *dest++ = __OSExpireAIFade->aLeft;
            *dest++ = __OSExpireAIFade->aRight;

            __OSExpireAIFade->aLeft *= delta;
            __OSExpireAIFade->aRight *= delta;

            i -= 4;
        }

        DCFlushRange(buffer, bytes);
        AIInitDMA((u32)buffer, bytes);
        __OSExpireAIFade->idx_ai_buffer++;
        __OSExpireAIFade->idx_ai_buffer &= 1;
	}

	__OSExpireAIFade->frames++;
}

BOOL __OSWriteExpiredFlag(void) {
    s32 rv = 0;
    NANDFileInfo nInfo;
    BOOL openNInfo = FALSE;
    u8 titleId[32] ATTRIBUTE_ALIGN(32);

    rv = NANDPrivateCreate("/shared2/expired", 63, 0);

    if (rv != NAND_RESULT_OK && rv != NAND_RESULT_EXISTS) {
        goto out;
    }

    rv = NANDPrivateOpen("/shared2/expired", &nInfo, 2);

    if (rv != NAND_RESULT_OK) {
        goto out;
    }

    openNInfo = TRUE;

    rv = ESP_InitLib();
    if (rv != 0) {
        goto out;
    }

    memset(titleId, 0, 32);
    rv = ESP_GetTitleId((ESTitleId*)titleId);

    if (rv != 0) {
        goto out;
    }

    rv = NANDWrite(&nInfo, titleId, 32);
    if (rv < 0) {
        goto out;
    }
    else if (rv != 32) {
        rv = NAND_RESULT_INVALID;
        goto out;
    }
    else {
        rv = 0;
    }

out:
    if (openNInfo) {
        NANDClose(&nInfo);
    }

    return rv == 0 ? TRUE : FALSE;
}

BOOL __OSWriteExpiredFlagIfSet(void) {
    if (__OSExpireSetExpiredFlag) {
        return __OSWriteExpiredFlag();
    }

    return FALSE;
}

void* __OSPlayTimeRebootThread(void* args) {
    BOOL enabled;
    u32 frames, fadeShift = 1;
    __OSExpireAIFadeStruct aiFade ATTRIBUTE_ALIGN(32);

    __OSExpireAIFade = &aiFade;
    memset(__OSExpireAIFade, 0, sizeof(__OSExpireAIFadeStruct));
    __OSExpireAIFade->gameAIDCallback = AIRegisterDMACallback(__OSPlayTimeFadeLastAIDCallback);

	for (frames = 0; frames < 20; frames++) {
		fadeShift = (frames / 5) + 1;
		if (fadeShift > 7) {
			fadeShift = 7;
        }

		VIWaitForRetrace(); 
		__OSSetVIForceDimming(TRUE, fadeShift, fadeShift);
	}

	AIRegisterDMACallback(NULL);

	VISetBlack(TRUE);
	VIFlush();

	enabled = OSDisableInterrupts();

    __OSWriteExpiredFlagIfSet();
    OSRestoreInterrupts(enabled);
    OSReturnToMenu();
    return NULL;
}

void __OSPlayTimeAlarmExpired(OSAlarm* alarm, OSContext* context) {
	if (__OSExpireCallback != 0) {
        __OSExpireCallback();
    } else {
        __OSPlayTimeRebootCallback(alarm, context);
    }
}

static s32 __OSPlayTimeGetConsumption(ESTicketView *tk, ESLpEntry *lpEntry, u32 *nEntries) {
	s32 rv = ESP_GetConsumption(tk->ticketID, NULL, nEntries);
    
    if (rv > 0) {
        goto out;
    } else if (rv != 0) {
        ASSERTMSGLINE(573, rv != -0x3F9, "Fix by calling ESP_InitLib");
        ASSERTMSG1LINE(574, FALSE, "Failed: %d\n", rv);
        rv = rv;
		goto out;
	}

	if (*nEntries != 0) {
       	rv = ESP_GetConsumption(tk->ticketID, lpEntry, nEntries);
        if (rv != 0) {
            ASSERTMSG1LINE(583, FALSE, "Failed: %d\n", rv);
            rv = rv;
    		goto out;
    	}
	}

out:
	return rv;
}

s32 __OSGetPlayTime(ESTicketView* ticket, __OSPlayTimeType* type, u32* playTime) {
	s32 rv;
	u32 i;
	ESLpEntry lpEntry[8] ATTRIBUTE_ALIGN(32);
	u32 numCc = 0, seenOther = 0;
	ESTicketView ticketAligned ATTRIBUTE_ALIGN(32);

    ASSERTLINE(601, ticket && type && playTime);

	if ((u32)(ticket) & 31) {
		memcpy(&ticketAligned, ticket, sizeof(ESTicketView));
		ticket = &ticketAligned;
	}

	rv = __OSPlayTimeGetConsumption(ticket, lpEntry, &numCc);
	if (rv != 0) {
		goto out;
    }

	for (i = 0; i < 8; i++) {
		if (ticket->limits[i].code == 1) {
			*type = 1;
			
            if (numCc == 0) {
				*playTime = ticket->limits[i].limit;
			} else {
                ASSERTLINE(630, i < numCc);

				if (lpEntry[i].limit >= ticket->limits[i].limit) {
					*playTime = 0;
                } else {
					*playTime = ticket->limits[i].limit - lpEntry[i].limit;
                }
			}
			goto out;
		} else if (ticket->limits[i].code != 0) {
			seenOther = i + 1;
		}
	}

	if (!seenOther) {
		*type = 0;
		*playTime = 0xFFFFFFFF;
	} else {
		seenOther--;

		if (ticket->limits[seenOther].code == 4) {
			*type = 4;
			*playTime = ticket->limits[seenOther].limit;
    
			if (numCc > 0) {
				*playTime -= lpEntry[seenOther].limit;
            }
		} else {
			*type = 9;
		}
	}

out:
	return rv;
}

s32 __OSGetPlayTimeCurrent(__OSPlayTimeType* type, u32* playTime) {
    s32 rv;
    ESTicketView ticket ATTRIBUTE_ALIGN(32);

    ASSERTLINE(676, type && playTime);

    rv = ESP_DiGetTicketView(NULL, &ticket);

    if (rv == -1017) {
        goto out;
    }

    if (rv != 0) {
        ASSERTMSG1LINE(687, FALSE, "Failed: %d\n", rv);
        rv = rv;
        goto out;
    }

    rv = __OSGetPlayTime(&ticket, type, playTime);

out:
    return rv;
}

void __OSInitPlayTime() {
    s32 rv;
    u32 limit;
    __OSPlayTimeType type;

    __OSExpireTime = 0;
    __OSExpireCallback = NULL;
    __OSExpireSetExpiredFlag = TRUE;

    rv = ESP_InitLib();
    if (rv != 0) {
        ASSERTMSG1LINE(710, FALSE, "Failed: %d\n", rv);
        rv = rv;
        goto out;
    }

    rv = __OSGetPlayTimeCurrent(&type, &limit);
    if (rv != 0) {
        if (rv != -1017) {
            ASSERTMSG1LINE(721, FALSE, "Failed: %d\n", rv);
            rv = rv;
        }

        goto out;
    }

    if (type == OSPLAYTIME_PERMANENT) {
        goto out;
    } else if (type == OSPLAYTIME_LIMITED) {
        if (limit == 0) {
            OSPanic(__FILE__, 737, "Expired");
        }

        OSCreateAlarm(&__OSExpireAlarm);
        OSSetAlarm(&__OSExpireAlarm, OSSecondsToTicks((OSTime)limit + 20), __OSPlayTimeAlarmExpired);
        __OSExpireTime = __OSExpireAlarm.fire;
        OSReport("PlayTime: %d seconds left\n", limit);
    }

out:
    ESP_CloseLib();
}
