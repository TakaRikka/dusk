#ifndef JHIHIOASYNC_H
#define JHIHIOASYNC_H

#include <types.h>

struct JHIContext;

BOOL JHIInit(u32 enabled);
u32 JHIEventLoop();
BOOL JOR_CHECKINTERFACE();
void JHISetBuffer(JHIContext* pCtx);

#endif /* JHIHIOASYNC_H */
