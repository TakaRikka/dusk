#ifndef _MSL_COMMON_FILE_POS_H
#define _MSL_COMMON_FILE_POS_H

#include <ansi_files.h>

#ifdef __cplusplus
extern "C" {
#endif

int fseek(FILE* file, unsigned long offset, int mode);
int _fseek(FILE* file, fpos_t offset, int mode);
long ftell(FILE* file);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_FILE_POS_H */
