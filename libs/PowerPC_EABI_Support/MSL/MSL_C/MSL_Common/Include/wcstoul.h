#ifndef _MSL_COMMON_WCSTOUL_H
#define _MSL_COMMON_WCSTOUL_H

#include <wstring.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t wcstoul(wchar_t* param_1, wchar_t** param_2, int param_3);

size_t wcstol(wchar_t* param_1, wchar_t** param_2, int param_3);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_WCSTOUL_H */
