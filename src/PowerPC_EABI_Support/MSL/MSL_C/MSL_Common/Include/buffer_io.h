#ifndef _MSL_COMMON_BUFFER_IO_H
#define _MSL_COMMON_BUFFER_IO_H

#include <cstdio>

enum { __align_buffer, __dont_align_buffer };

void __prep_buffer(FILE* file);
int __flush_buffer(FILE* file, size_t* bytes_flushed);

#endif /* _MSL_COMMON_BUFFER_IO_H */
