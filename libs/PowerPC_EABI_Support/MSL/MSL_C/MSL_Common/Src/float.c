#include "float.h"
#include "global.h"

int __float_nan[] = {0x7FFFFFFF};

int __float_huge[] = {0x7F800000};

#if PLATFORM_GCN
int __float_max[] = {0x7F7FFFFF};

int __float_epsilon[] = {0x34000000};
#endif

#if !PLATFORM_GCN
int __double_huge[] = {0x7FF00000, 0};
#endif
