#include <dolphin.h>
#include <dolphin/am.h>

#include "__am.h"

static u32 __AMStackPointer[AM_STACK_ENTRIES];
static AMReadInfo __AMReadInfo[AM_STACK_ENTRIES];

static u32 __AMStackLocation;
static u32 __AMFreeBytes;
static u32 __AMPendingReads;

static void __AM_dvd_callback(s32 result, DVDFileInfo* handle) {
    u32 i;
    AMReadInfo* ptr;

    for (i = 0; i < AM_STACK_ENTRIES; i++) {
        if (handle == &(__AMReadInfo[i].file_handle)) {
            ptr = &__AMReadInfo[i];
            break;
        }
    }

    ASSERTLINE(136, i != AM_STACK_ENTRIES);

    if (DVDGetCommandBlockStatus(&ptr->file_handle.cb) == 0) {
        ARQPostRequest(&ptr->arq_handle, i, 0, 1, (u32)ptr->buffer, ptr->curr_aram_offset, result, __AM_arq_callback);
        ptr->curr_aram_offset += result;
        ptr->curr_read_offset += result;
    }
}

static void __AM_arq_callback(u32 task) {
    u32 i;
    AMReadInfo* ptr;
    u32 remainder;
    u32 read_request_length;

    for (i = 0; i < AM_STACK_ENTRIES; i++) {
        if ((ARQRequest*)task == &__AMReadInfo[i].arq_handle) {
            ptr = &__AMReadInfo[i];
            break;
        }
    }

    ASSERTLINE(198, i != AM_STACK_ENTRIES);

    if (ptr->curr_read_offset < ptr->file_length) {
      remainder = ptr->file_length - ptr->curr_read_offset;
      read_request_length = OSRoundUp32B(ptr->read_length > remainder ? remainder : ptr->read_length);
      DVDReadAsync(&ptr->file_handle, ptr->buffer, read_request_length, ptr->curr_read_offset, __AM_dvd_callback);
    } else {
      ASSERTMSGLINE(220, __AMPendingReads, "AMPushBuffered(): Dangling read-complete event!\n");
      __AMPendingReads--;

        if (ptr->callback) {
            (*ptr->callback)(ptr->path);
        }
    }
}  

static void __AM_arq_poll_callback(u32 task) {
    u32 i;
    AMReadInfo* ptr;

    for (i = 0; i < AM_STACK_ENTRIES; i++) {
        if ((ARQRequest*)task == &__AMReadInfo[i].arq_handle) {
            ptr = &__AMReadInfo[i];
            break;
        }
    }

    ASSERTLINE(262, i != AM_STACK_ENTRIES);

    ptr->poll_flag = 1;
}

void* AMLoadFile(char* path, u32* length) {
    DVDFileInfo dvdFileInfo;
    u32 roundLength;
    s32 readLength;
    void* buffer;
    int old;

    if (!DVDOpen(path, &dvdFileInfo)) {
        char ch[1024];
        sprintf(ch, "Cannot open %s", path);
        ASSERTMSGLINE(290, 0, ch);
    }

    ASSERTMSGLINE(294, dvdFileInfo.length, "File length is 0\n");

    roundLength = OSRoundUp32B(dvdFileInfo.length);

    old = OSDisableInterrupts();
    buffer = OSAlloc(roundLength);
    OSRestoreInterrupts(old);

    ASSERTMSGLINE(303, buffer, "Unable to allocate buffer\n");
    readLength = DVDReadPrio(&dvdFileInfo, buffer, roundLength, 0, 2);
    ASSERTMSGLINE(307, readLength > 0, "Read length <= 0\n");
    if (length != 0) {
        *length = roundLength;
    }

    return buffer;
}

u32 AMPush(char* path) {
    DVDFileInfo handle;
    void* buffer;
    u32 buffer_length;
    u32 ret_val;
    BOOL old;

    if (!DVDOpen(path, &handle)) {
        OSReport("AMPushBuffered(): Unable to open file '%s'\n", path);
        OSPanic(__FILE__, 343, "AM: FATAL ERROR\n");
    }

    buffer_length = OSRoundUp32B(handle.length);
    if (buffer_length) {
        old = OSDisableInterrupts();
        buffer = OSAlloc(buffer_length);
        OSRestoreInterrupts(old);

        ASSERTMSGLINE(356, buffer, "AMPush(): Memory allocation failure.\n");

        ret_val = __AMPushBuffered(path, buffer, buffer_length, NULL, FALSE);
        OSFree(buffer);
        return ret_val;
    } else {
#if DEBUG
        OSReport("AMPush(): WARNING: File has zero length.\n");
#endif
        return 0;
    }
}

u32 AMPushData(void* buffer, u32 length) {
    BOOL old;
    u32 round_length;
    AMReadInfo* ptr;

    ASSERTMSGLINE(401, ((u32)buffer & 0x1F) == 0, "AMPushData(): buffer is not 32-byte aligned!\n");
    ASSERTLINE(404, length);

    round_length = OSRoundUp32B(length);
    if (__AMFreeBytes >= round_length && __AMStackLocation < AM_STACK_ENTRIES - 1) {
        ptr = &__AMReadInfo[__AMStackLocation];

        old = OSDisableInterrupts();

        ptr->aram_start_addr = __AMStackPointer[__AMStackLocation];
        __AMStackLocation++;
        __AMStackPointer[__AMStackLocation] = ptr->aram_start_addr + round_length;
        __AMFreeBytes -= round_length;

        ptr->poll_flag = 0;

        ARQPostRequest(&ptr->arq_handle, __AMStackLocation - 1, 0, 1, (u32)buffer, ptr->aram_start_addr, round_length, &__AM_arq_poll_callback);

        OSRestoreInterrupts(old);

        do {} while (!ptr->poll_flag);

        return ptr->aram_start_addr;
    }

    return 0;
}

u32 __AMPushBuffered(char* path, void* buffer, u32 buffer_size, AMCallback callback, int async_flag) {
    BOOL old;
    u32 round_file_length;
    u32 stack_index;
    AMReadInfo* ptr;
    u32 remainder;
    u32 read_request_length;
    s32 actual_read_length;

    ASSERTMSGLINE(477, ((u32)buffer & 0x1F) == 0, "AMPushBuffered(): buffer is not 32-byte aligned!\n");
    ASSERTMSGLINE(480, buffer_size > 31, "AMPushBuffered(): buffer_size is less than 32 bytes!\n");

    if (__AMStackLocation < AM_STACK_ENTRIES - 1) {
        ptr = &__AMReadInfo[__AMStackLocation];
        stack_index = __AMStackLocation;

        if (!DVDOpen(path, &ptr->file_handle)) {
            OSReport("AMPushBuffered(): Unable to open file '%s'\n", path);
            OSPanic(__FILE__, 494, "AM: FATAL ERROR\n");
        }

        ptr->file_length = ptr->file_handle.length;
        round_file_length = OSRoundUp32B(ptr->file_length);

        if (__AMFreeBytes >= round_file_length) {
            ptr->aram_start_addr = __AMStackPointer[__AMStackLocation];

            old = OSDisableInterrupts();

            __AMStackLocation++;
            __AMStackPointer[__AMStackLocation] = ptr->aram_start_addr + round_file_length;
            __AMFreeBytes -= round_file_length;

            ptr->curr_read_offset = 0;
            ptr->curr_aram_offset = ptr->aram_start_addr;
            ptr->read_length = buffer_size & ~0x1F;
            ptr->callback = callback;
            ptr->path = path;
            ptr->buffer = buffer;

            OSRestoreInterrupts(old);

            if (async_flag == 1) {
                DVDReadAsync(&ptr->file_handle, ptr->buffer, ptr->read_length, 0, __AM_dvd_callback);
                __AMPendingReads++;
            } else {
                while (ptr->curr_read_offset < ptr->file_length) {
                    remainder = ptr->file_length - ptr->curr_read_offset;
                    read_request_length = OSRoundUp32B(ptr->read_length > remainder ? remainder : ptr->read_length);
                    actual_read_length = DVDReadPrio(&ptr->file_handle, ptr->buffer, read_request_length, ptr->curr_read_offset, 2);

                    ASSERTMSGLINE(558, actual_read_length > 0, "AMPushBuffered(): Fatal Error - synchronuous DVD read\n");

                    ptr->curr_read_offset += actual_read_length;
                    ptr->poll_flag = FALSE;

                    ARQPostRequest(&(ptr->arq_handle), stack_index, 0, 1, (u32)ptr->buffer, ptr->curr_aram_offset, actual_read_length, __AM_arq_poll_callback);

                    ptr->curr_aram_offset += (u32)(actual_read_length);

                    do {} while (!ptr->poll_flag);
                }
            }
        } else {
#if DEBUG
            OSReport("AMPushBuffered(): WARNING: Not enough space in ARAM.\n");
#endif
            return 0;
        }
    } else {
#if DEBUG
        OSReport("AMPushBuffered(): WARNING: Stack table is full.\n");
#endif
        return 0;
    }

    return ptr->aram_start_addr;
}

void AMPop(void) {
    BOOL old;
    old = OSDisableInterrupts();

    if (__AMPendingReads == 0) {
        if (__AMStackLocation > 1) {
            __AMFreeBytes += __AMStackPointer[__AMStackLocation] - __AMStackPointer[__AMStackLocation - 1];
            __AMStackLocation--;
        }
    }

    OSRestoreInterrupts(old);
}

u32 AMGetZeroBuffer(void) {
    return __AMStackPointer[0];
}

u32 AMGetReadStatus(void) {
    BOOL old;
    u32 tmp;

    old = OSDisableInterrupts();
    tmp = __AMPendingReads;

    OSRestoreInterrupts(old);
    return tmp;
}

u32 AMGetFreeSize(void) {
    BOOL old;
    u32 tmp;

    old = OSDisableInterrupts();
    tmp = __AMFreeBytes;

    OSRestoreInterrupts(old);
    return tmp;
}

u32 AMGetStackPointer(void) {
    BOOL old;
    u32 tmp;

    old = OSDisableInterrupts();
    tmp = __AMStackPointer[__AMStackLocation];

    OSRestoreInterrupts(old);
    return tmp;
}

void AMInit(u32 aramBase, u32 aramBytes) {
    u32 i;
    u8 __AMZeroBuffer[256 + 31];
    u8* ptr;

    ASSERTLINE(760, aramBytes);

    ptr = (u8*)OSRoundUp32B(__AMZeroBuffer);

    __AMStackLocation = 0;
    __AMStackPointer[0] = aramBase;
    __AMFreeBytes = aramBytes;
    __AMPendingReads = 0;

    for (i = 0; i < 256; i++) {
        ptr[i] = 0;
    }

    AMPushData(ptr, 256);
}
