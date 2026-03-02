#ifndef OS_DOLPHIN_DDH_STUBS_H
#define OS_DOLPHIN_DDH_STUBS_H

#include <amc/AmcExi2Comm.h>

#ifdef __cplusplus
extern "C" {
#endif

int ddh_cc_initialize(void* inputPendingPtrRef, EXICallback monitorCallback);
int ddh_cc_shutdown();
int ddh_cc_open();
int ddh_cc_close();
int ddh_cc_read(u8* data, int size);
int ddh_cc_write(const u8* bytes, int length);
int ddh_cc_pre_continue();
int ddh_cc_post_stop();
int ddh_cc_peek();
int ddh_cc_initinterrupts();

#ifdef __cplusplus
}
#endif

#endif /* OS_DOLPHIN_DDH_STUBS_H */
