#ifndef _MSL_COMMON_MEM_FUNCS_H
#define _MSL_COMMON_MEM_FUNCS_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

void __copy_longs_rev_unaligned(void* dst, const void* src, size_t n);
void __copy_longs_unaligned(void* dst, const void* src, size_t n);
void __copy_longs_rev_aligned(void* dst, const void* src, size_t n);
void __copy_longs_aligned(void* dst, const void* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* _MSL_COMMON_MEM_FUNCS_H */
