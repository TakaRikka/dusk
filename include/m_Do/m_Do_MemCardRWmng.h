#ifndef M_DO_M_DO_MEMCARDRWMNG_H
#define M_DO_M_DO_MEMCARDRWMNG_H

#include "global.h"
#include <card.h>

#if PLATFORM_WII || PLATFORM_SHIELD
#include <revolution/nand.h>
#endif

struct mDoMemCdRWm_HeaderData {
    /* 0x0000 */ u8 mBannerTexData[0xC00];
    /* 0x0C00 */ u8 mBannerPalData[0x200];
    /* 0x0E00 */ u8 mIconTexData0[0x400];
    /* 0x1200 */ u8 mIconTexData1[0x400];
    /* 0x1600 */ u8 mIconTexData2[0x400];
    /* 0x1A00 */ u8 mIconTexData3[0x400];
    /* 0x1E00 */ u8 mIconTexData4[0x400];
    /* 0x2200 */ u8 mIconPalData[0x200];
    /* 0x2400 */ char mTitle[0x20];
    /* 0x2420 */ char mComment[0x20];
};

s32 mDoMemCdRWm_Restore(CARDFileInfo* param_0, void* param_1, u32 param_2);
s32 mDoMemCdRWm_Store(CARDFileInfo* param_0, void* param_1, u32 param_2);
static void mDoMemCdRWm_BuildHeader(mDoMemCdRWm_HeaderData* header);
static void mDoMemCdRWm_SetCardStat(CARDFileInfo* file);
static BOOL mDoMemCdRWm_CheckCardStat(CARDFileInfo* file);
static u32 mDoMemCdRWm_CalcCheckSum(void* data, u32 size);
static u64 mDoMemCdRWm_CalcCheckSumGameData(void* data, u32 size);
BOOL mDoMemCdRWm_TestCheckSumGameData(void* data);
void mDoMemCdRWm_SetCheckSumGameData(u8* data, u8 dataNum);

#if PLATFORM_WII || PLATFORM_SHIELD
s32 mDoMemCdRWm_RestoreNAND(NANDFileInfo* param_0, void* param_1, u32 param_2);
s32 mDoMemCdRWm_StoreNAND(NANDFileInfo* param_0, void* param_1, u32 param_2);
s32 mDoMemCdRWm_StoreBannerNAND(NANDFileInfo*);
#endif

#endif /* M_DO_M_DO_MEMCARDRWMNG_H */
