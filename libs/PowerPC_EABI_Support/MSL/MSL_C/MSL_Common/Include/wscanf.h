#ifndef _MSL_COMMON_WSCANF_H
#define _MSL_COMMON_WSCANF_H

#include <wstring.h>
#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    wchar_t* WCharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __OutStrCtrl;

wchar_t __wStringRead(wString* src, wchar_t param_1, wchar_t param_2);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_WSCANF_H */
