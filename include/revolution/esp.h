#ifndef ESP_H
#define ESP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>
#include <revolution/private/iostypes.h>

typedef u32 ESId;
typedef u32 ESContentId;
typedef u64 ESTitleId;
typedef u64 ESTicketId;
typedef u8 ESVersion;
typedef u16 ESTitleVersion;
typedef ESTitleId ESSysVersion;
typedef u32 ESTitleType;
typedef u16 ESContentType;
typedef u8 ESTmdReserved[62];
typedef u8 ESTicketReserved[48];
typedef u8 ESSysAccessMask[2];
typedef u8 ESCidxMask[64];

#pragma pack(push, 4)

typedef struct {
    ESVersion version;
    ESSysVersion sysVersion;
    ESTitleId titleId;
    ESTitleType type;
    u16 groupId;
    ESTmdReserved reserved;
    ESTitleVersion titleVersion;
    u16 numContents;
} ESTmdViewHeader;

typedef struct {
    ESContentId cid;
    u16 index;
    ESContentType type;
    u64 size;
} ESCmdView;

typedef struct {
    u32 code;
    u32 limit;
} ESLpEntry;

typedef struct {
    ESVersion version;
    ESTicketId ticketID;
    ESId devID;
    ESTitleId titleID;
    ESSysAccessMask sysAccessMask;
    u16 ticketVer;
    u32 accTitleID;
    u32 accTitleMask;
    u8 license;
    ESTicketReserved reserved;
    u8 audit;
    ESCidxMask cidxMask;
    ESLpEntry limits[8];
} ESTicketView;

typedef struct {
    ESTmdViewHeader head;
    ESCmdView contents[512];
} ESTmdView;

typedef struct {
    ESContentId cid;
    u16 index;
    ESContentType type;
    u64 size;
    IOSCHash hash;
} ESContentMeta;

typedef struct {
    ESVersion version;
    ESVersion caCrlVersion;
    ESVersion signerCrlVersion;
    ESSysVersion sysVersion;
    ESTitleId titleId;
    ESTitleType type;
    u16 groupId;
    ESTmdReserved reserved;
    u32 accessRights;
    ESTitleVersion titleVersion;
    u16 numContents;
    u16 bootIndex;
} ESTitleMetaHeader;

typedef struct {
    IOSCSigRsa2048 sig;
    ESTitleMetaHeader head;
    ESContentMeta contents[512];
} ESTitleMeta;

typedef struct {
    IOSCSigRsa2048 sig; 
    IOSCEccPublicKey serverPubKey;
    ESVersion version;
    ESVersion caCrlVersion;
    ESVersion signerCrlVersion;
    IOSCAesKey titleKey;
    ESTicketId ticketId;
    ESId deviceId;
    ESTitleId titleId;
    ESSysAccessMask sysAccessMask;
    u16 ticketVersion;
    u32 accessTitleId;
    u32 accessTitleMask;
    u8 licenseType;
    ESTicketReserved reserved;
    u8 audit;
    ESCidxMask cidxMask;
    ESLpEntry limits[8];
} ESTicket;

#pragma pack(pop)

s32 ESP_InitLib(void);
s32 ESP_CloseLib(void);
s32 ESP_LaunchTitle(u64, ESTicketView*);
s32 ESP_GetTicketViews(ESTitleId, ESTicketView*, u32*);
s32 ESP_DiGetTicketView(const void*, ESTicketView*);
s32 ESP_DiGetTmd(ESTitleMeta*, u32*);
s32 ESP_GetTmdView(ESTitleId, ESTmdView*, u32*);
s32 ESP_GetDataDir(ESTitleId, char*);
s32 ESP_GetTitleId(ESTitleId*);
s32 ESP_GetConsumption(ESTicketId, ESLpEntry*, u32*);

#ifdef __cplusplus
}
#endif

#endif // ESP_H
