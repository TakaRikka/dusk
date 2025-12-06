#ifndef JHIRMCC_H
#define JHIRMCC_H

#include <dolphin/dolphin.h>

struct JHIMccContext;

u32 JHIInitInterface();
bool JHINegotiateInterface(u32);
JHIMccContext JHIGetHiSpeedContext();
JHIMccContext JHIGetLowSpeedContext();
BOOL JHIInitMCC(JHIMccContext* pCtx, bool* param_1);
s32 JHIGetHIO2Handle();

#endif /* JHIRMCC_H */
