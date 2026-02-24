#ifndef _SRC_EXTRAS_H_
#define _SRC_EXTRAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MSC_VER
int strnicmp(const char* str1, const char* str2, int n);
int stricmp(const char* str1, const char* str2);
#endif

#ifdef __cplusplus
}
#endif

#endif // _SRC_EXTRAS_H_
