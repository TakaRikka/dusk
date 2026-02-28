#ifndef _REVOLUTION_USB_H_
#define _REVOLUTION_USB_H_

#include <revolution.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SWAP32(val)                                                                                                                                  \
((u32)((((u32)(val) & (u32)0x000000ffUL) << 24) | (((u32)(val) & (u32)0x0000ff00UL) << 8) | (((u32)(val) & (u32)0x00ff0000UL) >> 8) |            \
(((u32)(val) & (u32)0xff000000UL) >> 24)))

#define SWAP16(val) ((u16)((((u16)(val) & (u16)0x00ffUL) << 8) | (((u16)(val) & (u16)0xff00) >> 8)))

typedef struct {
    u8 bLength;
    u8 bDescriptorType;
    u16 bcdUSB;
    u8 bDeviceClass;
    u8 bDeviceSubClass;
    u8 bDeviceProtocol;
    u8 bMaxPacketSize0;
    u16 idVendor;
    u16 idProduct;
    u16 bcdDevice;
    u8 iManufacturer;
    u8 iProduct;
    u8 iSerialNumber;
    u8 bNumConfigurations;
} USB_DevDescr;

typedef struct IsoTransfer {
    void* buf;
    u8 numPackets;
    u16* packets;
} IsoTransfer;

typedef void (*USBCallbackFunc)(IOSError err, void* cbArg);
typedef void (*USBIsoCallbackFunc)(IOSError err, IsoTransfer* xfer, void* cbArg);

IOSError IUSB_ReadIntrMsgAsync(IOSFd fd, u32 ep, u32 buflen, char* buf, USBCallbackFunc cb, void* cbArg);
IOSError IUSB_WriteCtrlMsgAsync(IOSFd fd, u8 reqType, u8 request, u16 value, u16 index, u16 buflen, char* buf, USBCallbackFunc cb, void* cbArg);
IOSError IUSB_WriteBlkMsgAsync(IOSFd fd, u32 ep, u32 buflen, char* buf, USBCallbackFunc cb, void* cbArg);
IOSError IUSB_OpenDeviceIds(const char* did, u16 vid, u16 pid, IOSFd* fd);
IOSError IUSB_ReadBlkMsgAsync(IOSFd fd, u32 ep, u32 buflen, char* buf, USBCallbackFunc cb, void* cbArg);
IOSError IUSB_OpenLib(void);
IOSError IUSB_CloseDeviceAsync(IOSFd fd, USBCallbackFunc cb, void* cbArg);
IOSError IUSB_CloseLib(void);

#ifdef __cplusplus
}
#endif

#endif  // _REVOLUTION_USB_H_
