#ifndef MSL_COMMON_SRC_ERRNO_H
#define MSL_COMMON_SRC_ERRNO_H

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENOERR 0
#define EDOM 33
#define ERANGE 34
#define EFPOS 40
#define ESIGPARM 36

extern int errno;

#ifdef __cplusplus
}
#endif

#endif /* MSL_COMMON_SRC_ERRNO_H */
