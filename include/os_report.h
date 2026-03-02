#ifndef _OS_REPORT_H
#define _OS_REPORT_H

#include <dolphin/os.h>

DECL_WEAK void OSAttention(const char* msg, ...);
DECL_WEAK void OSReport_Error(const char* fmt, ...);
DECL_WEAK void OSReport_FatalError(const char* fmt, ...);
DECL_WEAK void OSReport_System(const char* fmt, ...);
DECL_WEAK void OSReport_Warning(const char* fmt, ...);
DECL_WEAK void OSReportDisable(void);
DECL_WEAK void OSReportEnable(void);
DECL_WEAK void OSReportForceEnableOff(void);
DECL_WEAK void OSReportForceEnableOn(void);

#if DEBUG
#define OS_REPORT(...) OSReport(__VA_ARGS__)
#define OS_WARNING(...) OSReport_Warning(__VA_ARGS__)
#define OS_REPORT_ERROR(...) OSReport_Error(__VA_ARGS__)
#define OS_PANIC(line, msg) OSPanic(__FILE__, line, msg)
#else
#define OS_REPORT(...)
#define OS_WARNING(...)
#define OS_REPORT_ERROR(...)
#define OS_PANIC(...)
#endif

extern u8 __OSReport_disable;
extern u8 __OSReport_Error_disable;
extern u8 __OSReport_Warning_disable;
extern u8 __OSReport_System_disable;
extern u8 __OSReport_enable;

#endif  // _OS_REPORT_H
