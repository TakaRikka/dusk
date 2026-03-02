#include "wchar_io.h"

int __fwide(FILE* stream, int mode) {
    return fwide(stream, mode);
}

int fwide(FILE* stream, int mode) {
    int orientation;
    int result;

    if (stream == NULL || stream->file_mode.file_kind == __closed_file)
        return 0;

    orientation = stream->file_mode.file_orientation;
    switch (orientation) {
    case __unoriented:
        if (mode > 0)
            stream->file_mode.file_orientation = __wide_oriented;
        else if (mode < 0)
            stream->file_mode.file_orientation = __char_oriented;
        result = mode;
        break;
    case __wide_oriented:
        result = 1;
        break;
    case __char_oriented:
        result = -1;
        break;
    }

    return result;
}
