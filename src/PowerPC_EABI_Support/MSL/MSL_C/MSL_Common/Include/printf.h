#ifndef _MSL_COMMON_PRINTF_H
#define _MSL_COMMON_PRINTF_H

#include <ansi_files.h>
#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif

int fprintf(FILE* stream, const char* format, ...);
int printf(const char* format, ...);
int sprintf(char* s, const char* format, ...);
int snprintf(char* s, size_t n, const char* format, ...);
int vsnprintf(char* s, size_t n, const char* format, va_list arg);
int vsprintf(char* s, const char* format, va_list arg);
int vprintf(const char* format, va_list arg);

int vswprintf(wchar_t* s, size_t n, const wchar_t* format, va_list arg);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_PRINTF_H */
