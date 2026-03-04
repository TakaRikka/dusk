#ifndef _REVOLUTION_CARD_INTERNAL_H_
#define _REVOLUTION_CARD_INTERNAL_H_

#include <revolution/card.h>
#include <revolution/exi.h>

#ifdef __cplusplus
extern "C" {
#endif

// CARDStatEx
s32 __CARDGetStatusEx(s32 chan, s32 fileNo, CARDDir* dirent);
s32 __CARDSetStatusExAsync(s32 chan, s32 fileNo, CARDDir* dirent, CARDCallback callback);
s32 __CARDSetStatusEx(s32 chan, s32 fileNo, CARDDir* dirent);

// CARDUnlock
s32 __CARDUnlock(s32 chan, u8 flashID[12]);

// CARDRead
s32 __CARDSeek(CARDFileInfo* fileInfo, s32 length, s32 offset, CARDControl** pcard);

// CARDRdwr
s32 __CARDRead(s32 chan, u32 addr, s32 length, void* dst, CARDCallback callback);
s32 __CARDWrite(s32 chan, u32 addr, s32 length, void* dst, CARDCallback callback);

// CARDRaw
s32 __CARDRawReadAsync(s32 chan, void* buf, s32 length, s32 offset, CARDCallback callback);
s32 __CARDRawRead(s32 chan, void* buf, s32 length, s32 offset);
s32 __CARDRawErase(s32 chan, s32 offset);
s32 __CARDRawEraseAsync(s32 chan, s32 offset, CARDCallback callback);

// CARDOpen
BOOL __CARDCompareFileName(CARDDir* ent, const char* fileName);
s32 __CARDAccess(CARDControl* card, CARDDir* ent);
s32 __CARDIsPublic(CARDDir* ent);
s32 __CARDGetFileNo(CARDControl* card, const char* fileName, s32* pfileNo);
BOOL __CARDIsOpened(CARDControl* card, s32 fileNo);
s32 __CARDIsWritable(CARDControl* card, CARDDir* ent);
s32 __CARDIsReadable(CARDControl* card, CARDDir* ent);

// CARDNet
extern u16 __CARDVendorID;
extern u8 __CARDPermMask;
int __CARDEnableGlobal(int enable);
int __CARDEnableCompany(int enable);

// CARDMount
void __CARDMountCallback(s32 chan, s32 result);
void __CARDDisable(BOOL disable);

// CARDFormat
s32 CARDFormatAsync(s32 chan, CARDCallback callback);
s32 __CARDFormatRegionAsync(s32 chan, u16 encode, CARDCallback callback);
s32 __CARDFormatRegion(s32 chan, u16 encode);

// CARDDir
CARDDir* __CARDGetDirBlock(CARDControl* card);
s32 __CARDUpdateDir(s32 chan, CARDCallback callback);

// CARDCheck
void __CARDCheckSum(void* ptr, int length, u16* checksum, u16* checksumInv);
s32 __CARDVerify(CARDControl* card);

// CARDBlock
void* __CARDGetFatBlock(CARDControl* card);
s32 __CARDAllocBlock(s32 chan, u32 cBlock, CARDCallback callback);
s32 __CARDFreeBlock(s32 chan, u16 nBlock, CARDCallback callback);
s32 __CARDUpdateFatBlock(s32 chan, u16* fat, CARDCallback callback);

// CARDBios
extern CARDControl __CARDBlock[2];

extern DVDDiskID* __CARDDiskID;
extern DVDDiskID __CARDDiskNone;

void __CARDDefaultApiCallback(s32 chan, s32 result);
void __CARDSyncCallback(s32 chan, s32 result);
void __CARDExtHandler(s32 chan, OSContext* context);
void __CARDExiHandler(s32 chan, OSContext* context);
void __CARDTxHandler(s32 chan, OSContext* context);
void __CARDUnlockedHandler(s32 chan, OSContext* context);
int __CARDReadNintendoID(s32 chan, u32* id);
s32 __CARDEnableInterrupt(s32 chan, BOOL enable);
s32 __CARDReadStatus(s32 chan, u8* status);
int __CARDReadVendorID(s32 chan, u16* id);
s32 __CARDClearStatus(s32 chan);
s32 __CARDSleep(s32 chan);
s32 __CARDWakeup(s32 chan);
s32 __CARDReadSegment(s32 chan, CARDCallback callback);
s32 __CARDWritePage(s32 chan, CARDCallback callback);
s32 __CARDErase(s32 chan, CARDCallback callback);
s32 __CARDEraseSector(s32 chan, u32 addr, CARDCallback callback);
void __CARDSetDiskID(const DVDDiskID* id);
s32 __CARDGetControlBlock(s32 chan, CARDControl **pcard);
s32 __CARDPutControlBlock(CARDControl* card, s32 result);
s32 __CARDSync(s32 chan);
u16 __CARDGetFontEncode(void);
u16 __CARDSetFontEncode(u16 encode);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_CARD_INTERNAL_H_
