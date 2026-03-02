#include <wscanf.h>
#include <wstring.h>

wchar_t __wStringRead(wString* src, wchar_t param_1, wchar_t param_2) {
    switch (param_2) {
    case 0:
        param_2 = *src->buffer;
        if (param_2 == 0) {
            src->field_0x4 = 1;
            return 0xFFFF;
        } else {
            src->buffer++;
            return param_2;
        }
    case 1:
        if (src->field_0x4 == 0) {
            src->buffer--;
        } else {
            src->field_0x4 = 0;
        }
        return param_1;
    case 2:
        return src->field_0x4;
    default:
        return 0;
    }
}
