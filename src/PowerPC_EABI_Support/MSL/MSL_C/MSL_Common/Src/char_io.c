#include "char_io.h"
#include "critical_regions.h"
#include "misc_io.h"
#include "wchar_io.h"

#include "global.h"

int __put_char(int c, FILE* stream) {
    int ret;

    int file_kind = stream->file_mode.file_kind;
    stream->buffer_length = 0;

    if (stream->file_state.error != 0 || file_kind == __closed_file) {
        return -1;
    }

    if (file_kind == __console_file) {
        __stdio_atexit();
    }

    if (stream->file_state.io_state == __neutral && (stream->file_mode.io_mode & __write)) {
        if ((stream->file_mode.io_mode & __append)) {
            #if PLATFORM_GCN
            if (fseek(stream, 0, 2) != 0)
            #else
            if (_fseek(stream, 0, 2) != 0)
            #endif
            {
                return 0;
            }
        }

        stream->file_state.io_state = __writing;
        __prep_buffer(stream);
    }

    if (stream->file_state.io_state != __writing) {
        stream->file_state.error = 1;
        ret = -1;
        stream->buffer_length = 0;
    } else if ((stream->file_mode.buffer_mode == 2 ||
                stream->buffer_size ==
                    (unsigned int)stream->buffer_ptr - (unsigned int)stream->buffer) &&
               __flush_buffer(stream, NULL) != 0)
    {
        stream->file_state.error = 1;
        ret = -1;
        stream->buffer_length = 0;
    } else {
        stream->buffer_length--;
        *stream->buffer_ptr++ = c;

        if (stream->file_mode.buffer_mode != 2) {
            if ((stream->file_mode.buffer_mode == 0 || c == 10) &&
                __flush_buffer(stream, NULL) != 0)
            {
                stream->file_state.error = 1;
                ret = -1;
                stream->buffer_length = 0;
                goto exit;
            }
            stream->buffer_length = 0;
        }

        ret = c & 0xFF;
    }

exit:
    return ret;
}

#if PLATFORM_GCN
#define __putc(c, file) \
	((fwide(file, -1) >= 0) ? -1 : (file)->buffer_length-- ? (int) (*(file)->buffer_ptr++ = (unsigned char)(c)) : __put_char(c, file))
#else
#define __putc(c, file) \
	((__fwide(file, -1) >= 0) ? -1 : (file)->buffer_length-- ? (int) (*(file)->buffer_ptr++ = (unsigned char)(c)) : __put_char(c, file))
#endif

int fputs(const char* s, FILE* stream) {
    char c;
    int ret = 0;

    __begin_critical_region(stdin_access);
    while (c = *s++, c != 0) {
        if (__putc(c, stream) == -1) {
            ret = -1;
            break;
        }
    }
    __end_critical_region(stdin_access);

    return ret;
}
