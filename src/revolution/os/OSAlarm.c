#include <revolution.h>
#include <revolution/os.h>

#include "__os.h"
#include "__dvd.h"

typedef struct {
    OSAlarm* head;
    OSAlarm* tail;
} OSAlarmQueue;

// prototypes
static void SetTimer(OSAlarm * alarm);
static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler);
static void DecrementerExceptionCallback(__REGISTER __OSException exception, __REGISTER OSContext* context);
static void DecrementerExceptionHandler(__OSException exception, OSContext* context);
static BOOL OnReset(BOOL final, u32 event);

static OSShutdownFunctionInfo ResetFunctionInfo = {OnReset, 0xFFFFFFFF, NULL, NULL};
static OSAlarmQueue AlarmQueue;

#define ASSERTREPORT(line, cond) \
    if (!(cond)) { OSReport("OSCheckAlarmQueue: Failed " #cond " in %d", line); return 0; }

BOOL OSCheckAlarmQueue(void) {
    OSAlarm* alarm;

    ASSERTREPORT(181, AlarmQueue.head == NULL && AlarmQueue.tail == NULL || AlarmQueue.head != NULL && AlarmQueue.tail != NULL);
    ASSERTREPORT(182, AlarmQueue.head == NULL || AlarmQueue.head->prev == NULL);
    ASSERTREPORT(183, AlarmQueue.tail == NULL || AlarmQueue.tail->next == NULL);

    for (alarm = AlarmQueue.head; alarm; alarm = alarm->next) {
        ASSERTREPORT(186, alarm->next == NULL || alarm->next->prev == alarm);
        ASSERTREPORT(187, alarm->next != NULL || AlarmQueue.tail == alarm);
    }
    return TRUE;
}

static void SetTimer(OSAlarm* alarm) {
    OSTime delta = alarm->fire - __OSGetSystemTime();

    if (delta < 0) {
        PPCMtdec(0);
    } else if (delta < 0x80000000) {
        PPCMtdec((u32)delta);
    } else {
        PPCMtdec(0x7fffffff);
    }
}

void __OSInitAlarm(void) {
    if (__OSGetExceptionHandler(8) != DecrementerExceptionHandler) {
        AlarmQueue.head = AlarmQueue.tail = NULL;
        __OSSetExceptionHandler(8, DecrementerExceptionHandler);
        OSRegisterShutdownFunction(&ResetFunctionInfo);
    }
}

void OSCreateAlarm(OSAlarm* alarm) {
    alarm->handler = 0;
    alarm->tag = 0;
}

static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler) {
    OSAlarm* next;
    OSAlarm* prev;
    
    if (0 < alarm->period) {
        OSTime time = __OSGetSystemTime();
        
        fire = alarm->start;
        if (alarm->start < time) {
            fire += alarm->period * ((time - alarm->start) / alarm->period + 1);
        }
    }
    
    ASSERTLINE(290, alarm->handler == 0);
    
    alarm->handler = handler;
    alarm->fire = fire;
    
    for (next = AlarmQueue.head; next; next = next->next) {
        if (next->fire <= fire) {
            continue;
        }
        
        alarm->prev = next->prev;
        next->prev = alarm;
        alarm->next = next;
        prev = alarm->prev;

        if (prev) {
            prev->next = alarm;
        } else {
            AlarmQueue.head = alarm;
            SetTimer(alarm);
        }

        return;
    }

    ASSERTLINE(319, next == 0);

    alarm->next = 0;
    prev = AlarmQueue.tail;
    AlarmQueue.tail = alarm;
    alarm->prev = prev;

    if (prev) {
        prev->next = alarm;
    } else {
        AlarmQueue.head = AlarmQueue.tail = alarm;
        SetTimer(alarm);
    }
}

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {
    BOOL enabled;
    ASSERTMSGLINE(352, tick > 0, "OSSetAlarm(): tick was less than zero.");
    ASSERTMSGLINE(353, handler, "OSSetAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
    ASSERTLINE(360, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

void OSSetAbsAlarm(OSAlarm* alarm, OSTime time, OSAlarmHandler handler) {
    BOOL enabled;

    ASSERTMSGLINE(343, handler, "OSSetAbsAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = 0;
    InsertAlarm(alarm, __OSTimeToSystemTime(time), handler);
    ASSERTLINE(350, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period, OSAlarmHandler handler) {
    BOOL enabled;
    ASSERTMSGLINE(413, period > 0, "OSSetPeriodicAlarm(): period was less than zero.");
    ASSERTMSGLINE(414, handler, "OSSetPeriodicAlarm(): null handler was specified.");
    enabled = OSDisableInterrupts();
    alarm->period = period;
    alarm->start = __OSTimeToSystemTime(start);
    InsertAlarm(alarm, 0, handler);
    ASSERTLINE(422, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

void OSCancelAlarm(OSAlarm* alarm) {
    OSAlarm* next;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (alarm->handler == 0) {
        OSRestoreInterrupts(enabled);
        return;
    }

    next = alarm->next;
    if (next == 0) {
        AlarmQueue.tail = alarm->prev;
    } else {
        next->prev = alarm->prev;
    }
    if (alarm->prev) {
        alarm->prev->next = next;
    } else {
        AlarmQueue.head = next;
        if (next) {
            SetTimer(next);
        }
    }
    alarm->handler = 0;
    ASSERTLINE(473, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

static void DecrementerExceptionCallback(__REGISTER __OSException exception,
                                         __REGISTER OSContext* context) {
    OSAlarm* alarm;
    OSAlarm* next;
    OSAlarmHandler handler;
    OSTime time;
    OSContext exceptionContext;

    time = __OSGetSystemTime();
    alarm = AlarmQueue.head;
    if (alarm == 0) {
        OSLoadContext(context);
    }

    if (time < alarm->fire) {
        SetTimer(alarm);
        OSLoadContext(context);
    }

    next = alarm->next;
    AlarmQueue.head = next;
    if (next == 0) {
        AlarmQueue.tail = 0;
    } else {
        next->prev = 0;
    }

    ASSERTLINE(531, OSCheckAlarmQueue());

    handler = alarm->handler;
    alarm->handler = 0;
    if (0 < alarm->period) {
        InsertAlarm(alarm, 0, handler);
        ASSERTLINE(541, OSCheckAlarmQueue());
    }

    if (AlarmQueue.head) {
        SetTimer(AlarmQueue.head);
    }

    OSDisableScheduler();
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    handler(alarm, context);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
    OSEnableScheduler();
    __OSReschedule();
    OSLoadContext(context);
}

#ifdef __GEKKO__
static asm void DecrementerExceptionHandler(__REGISTER __OSException exception,
                                            __REGISTER OSContext* context) {
    nofralloc 
    OS_EXCEPTION_SAVE_GPRS(context)
    stwu r1, -8(r1)
    b DecrementerExceptionCallback
}
#endif

void OSSetAlarmTag(OSAlarm* alarm, u32 tag) {
    alarm->tag = tag;
}

void OSCancelAlarms(u32 tag) {
    BOOL enabled;
    OSAlarm* alarm;
    OSAlarm* next;

    ASSERTMSGLINE(569, tag != 0, "OSCancelAlarms(): invalid tag. (this tag is used by the operating system.)");

    if (tag != 0) {
        enabled = OSDisableInterrupts();
        ASSERTLINE(575, OSCheckAlarmQueue());

        alarm = AlarmQueue.head;
        next = (alarm) ? alarm->next : NULL;

        while (alarm != 0) {
            if (alarm->tag == tag) {
                OSCancelAlarm(alarm);
            }

            alarm = next;
            next = (alarm) ? alarm->next : NULL;
        }

        ASSERTLINE(585, OSCheckAlarmQueue());
        OSRestoreInterrupts(enabled);
    }
}

static BOOL OnReset(BOOL final, u32 event) {
    OSAlarm* alarm;
    OSAlarm* next;

    if (final != FALSE) {
        ASSERTLINE(656, OSCheckAlarmQueue());

        for (alarm = AlarmQueue.head, next = alarm ? alarm->next : 0; alarm; alarm = next, next = alarm ? alarm->next : 0) {
            if (!__DVDTestAlarm(alarm)) {
                OSCancelAlarm(alarm);
            }
        }

        ASSERTLINE(668, OSCheckAlarmQueue());
    }

    return TRUE;
}

void __OSCancelInternalAlarms(void* userdata) {
    BOOL enabled = OSDisableInterrupts();
    OSAlarm* alarm;
    OSAlarm* next;

    ASSERTLINE(723, OSCheckAlarmQueue());

    for (alarm = AlarmQueue.head, next = alarm ? alarm->next : 0; alarm; alarm = next, next = alarm ? alarm->next : 0) {
        if (alarm->tag == -1 && alarm->userData == userdata) {
            OSCancelAlarm(alarm);
        }
    }

    ASSERTLINE(737, OSCheckAlarmQueue());
    OSRestoreInterrupts(enabled);
}

// NOTE: these functions don't exist in tp debug, OSReports are made up to create string data
void OSSetAlarmUserData(OSAlarm* alarm, void* userData) {
    #if DEBUG
    OSReport("OSSetAlarmUserData");
    OSReport("NULL pointer is detected at %s");
    #endif
    alarm->userData = userData;
}

void* OSGetAlarmUserData(const OSAlarm* alarm) {
    #if DEBUG
    OSReport("OSGetAlarmUserData");
    #endif
    return alarm->userData;
}
