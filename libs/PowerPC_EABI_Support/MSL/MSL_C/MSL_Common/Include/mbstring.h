#ifndef _MSL_COMMON_MBSTRING_H
#define _MSL_COMMON_MBSTRING_H

#include <wchar_io.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t wcstombs(char* dst, const wchar_t* src, size_t n);

int mbtowc(wchar_t* pwc, const char* s, size_t n);
int mbstowcs(wchar_t* param_0, const char* param_1, int param_2);

int __mbtowc_noconv(wchar_t*, const char*, size_t);
int __wctomb_noconv(char*, wchar_t);

int mbsrtowcs_s(size_t* retval, wchar_t* dst, unsigned int dstsz, const char** param_4, size_t len,
                int param_6);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_MBSTRING_H */
