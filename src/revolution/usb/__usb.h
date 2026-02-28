#ifndef _REVOLUTION_USB_PRIVATE_H_
#define _REVOLUTION_USB_PRIVATE_H_

#include <revolution/usb.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ROUNDUP(sz) (((u32)(sz) + 32 - 1) & ~(u32)(32 - 1))

typedef struct {
    char *data;
    u16 wLength;
} IntBlkCtrlReq;

typedef struct iusb_Ctxt {
    USBCallbackFunc  cb;
#if SDK_AUG2010
    USBIsoCallbackFunc icb;
#endif
    void *cbArg;
#if SDK_AUG2010
    void *xfer;
#endif
    void *spare;
    void *clean[8];
    u32 nclean;

    union {
        char path[ROUNDUP(64)];
        char des[ROUNDUP(sizeof(USB_DevDescr))];
        char reqBuf[ROUNDUP(sizeof(IntBlkCtrlReq))];
    } u __attribute__ ((aligned(32)));

} iusb_ctxt;

#ifdef __cplusplus
}
#endif

#endif /* _REVOLUTION_USB_PRIVATE_H_ */
