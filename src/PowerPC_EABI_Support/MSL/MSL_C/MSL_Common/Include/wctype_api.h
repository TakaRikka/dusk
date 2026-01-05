#ifndef _MSL_WCTYPE_H
#define _MSL_WCTYPE_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

#define wctype_alpha 0x0001
#define wctype_blank 0x0002
#define wctype_cntrl 0x0004
#define wctype_digit 0x0008
#define wctype_graph 0x0010
#define wctype_lower 0x0020
#define wctype_print 0x0040
#define wctype_punct 0x0080
#define wctype_space 0x0100
#define wctype_upper 0x0200
#define wctype_xdigit 0x0400

#define wctype_alnum (wctype_alpha | wctype_digit)

extern const unsigned short __wctype_mapC[256];
extern const wchar_t __wlower_mapC[256];
extern const wchar_t __wupper_mapC[256];

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_CTYPE_H */
