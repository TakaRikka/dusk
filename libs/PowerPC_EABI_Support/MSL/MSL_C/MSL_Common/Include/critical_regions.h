#ifndef _MSL_COMMON_CRITICAL_REGIONS_H
#define _MSL_COMMON_CRITICAL_REGIONS_H

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

enum critical_regions {
	atexit_funcs_access,
	malloc_pool_access,
	stdin_access,
	stdout_access,
	stderr_access,
	files_access,
	console_status_access,
	signal_funcs_access,
	thread_access,
	num_critical_regions
};

void __init_critical_regions(void);
void __kill_critical_regions(void);

#if PLATFORM_GCN
void __begin_critical_region(int region);
void __end_critical_region(int region);
#else
#define __begin_critical_region(region)
#define __end_critical_region(region)
#endif

#ifdef __cplusplus
}
#endif

#endif
