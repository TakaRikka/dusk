/**
 * UDP_Stubs.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/Os/dolphin/UDP_Stubs.h"

DECL_WEAK int udp_cc_initialize(void* flagOut, __OSInterruptHandler handler) {
    return -1;
}

DECL_WEAK int udp_cc_shutdown(void) {
    return -1;
}

DECL_WEAK int udp_cc_open(void) {
    return -1;
}

DECL_WEAK int udp_cc_close(void) {
    return -1;
}

DECL_WEAK int udp_cc_read(u8* dest, int size) {
    return 0;
}

DECL_WEAK int udp_cc_write(const u8* src, int size) {
    return 0;
}

DECL_WEAK int udp_cc_peek(void) {
    return 0;
}

DECL_WEAK int udp_cc_pre_continue(void) {
    return -1;
}

DECL_WEAK int udp_cc_post_stop(void) {
    return -1;
}
