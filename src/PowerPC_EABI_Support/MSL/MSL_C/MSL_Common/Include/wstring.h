#ifndef MSL_WSTRING_H
#define MSL_WSTRING_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wString {
    wchar_t* buffer;
    size_t field_0x4;
    size_t field_0x8;
} wString;

size_t wcslen(const wchar_t*);
wchar_t* wcscpy(wchar_t*, const wchar_t*);
wchar_t* wcsncpy(wchar_t*, const wchar_t*, size_t);
wchar_t* wcscat(wchar_t*, const wchar_t*);
int wcscmp(const wchar_t*, const wchar_t*);
wchar_t* wcschr(const wchar_t*, wchar_t);

#ifdef __cplusplus
}
#endif

#endif
