#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void OSSetCurrentThreadName(const char* name);

#ifdef __cplusplus
}

namespace dusk {
extern bool OSReportReallyForceEnable;
}
#endif
