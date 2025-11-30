/**
 * m_Do_MemCardRWmng.cpp
 * Memcard Saving and Checksum
 */

#include "m_Do/m_Do_MemCardRWmng.h"
#include "m_Do/m_Do_MemCard.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_com_inf_game.h"
#include "d/d_save.h"
#include <stdio.h>

#if VERSION == VERSION_GCN_JPN
#define HEADER_TITLE   "ゼルダの伝説 ﾄﾜｲﾗｲﾄﾌﾟﾘﾝｾｽ"
#define HEADER_COMMENT "%d月%d日のセーブデータです"
#else
#define HEADER_TITLE   "Zelda: Twilight Princess"
#define HEADER_COMMENT "%d/%d Save Data"
#endif

struct data_s {
    int unk_0x0;
    int data_version;
    u8 data[(SAVEDATA_SIZE * 3) + 0x38];  // unsure what the extra 0x38 is
    u32 checksum;
};

static u8 sTmpBuf[SECTOR_SIZE * 2];

s32 mDoMemCdRWm_Store(CARDFileInfo* file, void* data, u32 length) {
    mDoMemCdRWm_BuildHeader((mDoMemCdRWm_HeaderData*)sTmpBuf);

    s32 ret = CARDWrite(file, sTmpBuf, sizeof(sTmpBuf), 0);
    if (ret != CARD_RESULT_READY) { 
        return ret;
    }

    if (!mDoMemCdRWm_CheckCardStat(file)) {
        memset(sTmpBuf, 0, sizeof(sTmpBuf));

        ret = CARDWrite(file, sTmpBuf, SECTOR_SIZE, 0x4000);
        if (ret != CARD_RESULT_READY) {
            return ret;
        }

        ret = CARDWrite(file, sTmpBuf, SECTOR_SIZE, 0x6000);
        if (ret != CARD_RESULT_READY) {
            return ret;
        }
    }

    memset(sTmpBuf, 0, sizeof(sTmpBuf));

    data_s* tmp_data = (data_s*)sTmpBuf;
    tmp_data->data_version = SAVEDATA_VERSION;
    memcpy(tmp_data->data, data, length);
    tmp_data->unk_0x0 = 0;

    u32 checksum = mDoMemCdRWm_CalcCheckSum(tmp_data, sizeof(data_s) - 4);
    tmp_data->checksum = checksum;

    ret = CARDWrite(file, sTmpBuf, SECTOR_SIZE, 0x4000);
    if (ret != CARD_RESULT_READY) {
        return ret;
    }

    ret = CARDRead(file, sTmpBuf, SECTOR_SIZE, 0x4000);
    if (ret != CARD_RESULT_READY) {
        return ret;
    }

    if (checksum != mDoMemCdRWm_CalcCheckSum(sTmpBuf, sizeof(data_s) - 4)) {
        return ret;
    }

    ret = CARDWrite(file, sTmpBuf, SECTOR_SIZE, 0x6000);
    if (ret != CARD_RESULT_READY) {
        return ret;
    }

    ret = CARDRead(file, sTmpBuf, SECTOR_SIZE, 0x6000);
    if (ret != CARD_RESULT_READY) {
        return ret;
    }

    if (checksum != mDoMemCdRWm_CalcCheckSum(sTmpBuf, sizeof(data_s) - 4)) {
        return ret;
    }

    mDoMemCdRWm_SetCardStat(file);
    return ret;
}

s32 mDoMemCdRWm_Restore(CARDFileInfo* file, void* data, u32 length) {
    BOOL rewrite = FALSE;

    data_s* saves = (data_s*)sTmpBuf;
    data_s* backup_saves = (data_s*)(sTmpBuf + SECTOR_SIZE);

    s32 ret = CARDRead(file, saves, SECTOR_SIZE, 0x4000);
    if (ret != CARD_RESULT_READY) {
        return ret;
    }

    BOOL save1_valid = mDoMemCdRWm_TestCheckSumGameData(&saves->data[SAVEDATA_SIZE * 0]);
    BOOL save2_valid = mDoMemCdRWm_TestCheckSumGameData(&saves->data[SAVEDATA_SIZE * 1]);
    BOOL save3_valid = mDoMemCdRWm_TestCheckSumGameData(&saves->data[SAVEDATA_SIZE * 2]);

    ret = CARDRead(file, backup_saves, SECTOR_SIZE, 0x6000);
    if (ret != CARD_RESULT_READY) {
        return ret;
    }

    BOOL backup1_valid = mDoMemCdRWm_TestCheckSumGameData(&backup_saves->data[SAVEDATA_SIZE * 0]);
    BOOL backup2_valid = mDoMemCdRWm_TestCheckSumGameData(&backup_saves->data[SAVEDATA_SIZE * 1]);
    BOOL backup3_valid = mDoMemCdRWm_TestCheckSumGameData(&backup_saves->data[SAVEDATA_SIZE * 2]);

    if (!save1_valid && backup1_valid) {
        memcpy(&saves->data[SAVEDATA_SIZE * 0], &backup_saves->data[SAVEDATA_SIZE * 0], SAVEDATA_SIZE);
        rewrite = TRUE;
    }

    if (!save2_valid && backup2_valid) {
        memcpy(&saves->data[SAVEDATA_SIZE * 1], &backup_saves->data[SAVEDATA_SIZE * 1], SAVEDATA_SIZE);
        rewrite = TRUE;
    }

    if (!save3_valid && backup3_valid) {
        memcpy(&saves->data[SAVEDATA_SIZE * 2], &backup_saves->data[SAVEDATA_SIZE * 2], SAVEDATA_SIZE);
        rewrite = TRUE;
    }

    if (!mDoMemCdRWm_CheckCardStat(file)) {
        return CARD_RESULT_FATAL_ERROR;
    }

    if (rewrite) {
        ret = CARDWrite(file, saves, SECTOR_SIZE, 0x4000);
        if (ret != CARD_RESULT_READY) {
            return ret;
        }

        ret = CARDWrite(file, saves, SECTOR_SIZE, 0x6000);
        if (ret != CARD_RESULT_READY) {
            return ret;
        }
    }

    memcpy(data, saves->data, length);
    mDoMemCd_setDataVersion(saves->data_version);
    mDoMemCd_setSerialNo();

    return CARD_RESULT_READY;
}

static void mDoMemCdRWm_BuildHeader(mDoMemCdRWm_HeaderData* header) {
    snprintf(header->mTitle, sizeof(header->mTitle), HEADER_TITLE);

    OSCalendarTime time;
    OSTicksToCalendarTime(OSGetTime(), &time);

#if VERSION == VERSION_GCN_PAL
    switch (dComIfGs_getPalLanguage()) {
    case dSv_player_config_c::LANGAUGE_ENGLISH:
        snprintf(header->mComment, sizeof(header->mComment), "%d/%d Save Data", time.mon + 1, time.mday);
        break;
    case dSv_player_config_c::LANGAUGE_GERMAN:
        snprintf(header->mComment, sizeof(header->mComment), "%d/%d Spielstand", time.mday, time.mon + 1);
        break;
    case dSv_player_config_c::LANGAUGE_FRENCH:
        snprintf(header->mComment, sizeof(header->mComment), "Donn%ces de jeu %d/%d", 0xE9, time.mday, time.mon + 1);
        break;
    case dSv_player_config_c::LANGAUGE_SPANISH:
        snprintf(header->mComment, sizeof(header->mComment), "Datos guardados el %d/%d", time.mday, time.mon + 1);
        break;
    case dSv_player_config_c::LANGAUGE_ITALIAN:
        snprintf(header->mComment, sizeof(header->mComment), "Dati salvati: %d/%d", time.mday, time.mon + 1);
        break;
    }
#else
    snprintf(header->mComment, sizeof(header->mComment), HEADER_COMMENT, time.mon + 1, time.mday);
#endif

    ResTIMG* banner_data = (ResTIMG*)dComIfGp_getCardIconResArchive()->getResource("zelda2_gc_banner.bti");
    ResTIMG* icon_data = (ResTIMG*)dComIfGp_getCardIconResArchive()->getResource("zelda2_gc_icon.bti");

    memcpy(header->mBannerTexData, (u8*)banner_data + banner_data->imageOffset, (banner_data->numColors * 2) + 0xC00);
    memcpy(header->mIconTexData0, (u8*)icon_data + icon_data->imageOffset, 0x400);
    memcpy(header->mIconTexData1, (u8*)icon_data + icon_data->imageOffset, 0x400);
    memcpy(header->mIconTexData2, (u8*)icon_data + icon_data->imageOffset, 0x400);
    memcpy(header->mIconTexData3, (u8*)icon_data + icon_data->imageOffset, 0x400);
    memcpy(header->mIconTexData4, (u8*)icon_data + icon_data->imageOffset, (icon_data->numColors * 2) + 0x400);

    dComIfGp_getCardIconResArchive()->removeResourceAll();
}

static void mDoMemCdRWm_SetCardStat(CARDFileInfo* file) {
    CARDStat stat;
    mDoMemCd_getCardStatus(file->fileNo, &stat);

    stat.iconAddr = 0;
    stat.commentAddr = 0x2400;
    CARDSetBannerFormat(&stat, CARD_STAT_BANNER_C8);
    CARDSetIconAnim(&stat, CARD_STAT_ANIM_BOUNCE);
    CARDSetIconFormat(&stat, 0, CARD_STAT_ICON_C8);
    CARDSetIconFormat(&stat, 1, CARD_STAT_ICON_C8);
    CARDSetIconFormat(&stat, 2, CARD_STAT_ICON_C8);
    CARDSetIconFormat(&stat, 3, CARD_STAT_ICON_C8);
    CARDSetIconFormat(&stat, 4, CARD_STAT_ICON_C8);
    CARDSetIconFormat(&stat, 5, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 6, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 7, CARD_STAT_ICON_NONE);
    CARDSetIconSpeed(&stat, 0, CARD_STAT_SPEED_FAST);
    CARDSetIconSpeed(&stat, 1, CARD_STAT_SPEED_FAST);
    CARDSetIconSpeed(&stat, 2, CARD_STAT_SPEED_FAST);
    CARDSetIconSpeed(&stat, 3, CARD_STAT_SPEED_FAST);
    CARDSetIconSpeed(&stat, 4, CARD_STAT_SPEED_FAST);
    CARDSetIconSpeed(&stat, 5, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 6, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 7, CARD_STAT_SPEED_END);

    mDoMemCd_setCardStatus(file->fileNo, &stat);
}

static BOOL mDoMemCdRWm_CheckCardStat(CARDFileInfo* file) {
    CARDStat stat;
    mDoMemCd_getCardStatus(file->fileNo, &stat);

    if (stat.iconAddr != 0 || stat.commentAddr != 0x2400
        || CARDGetBannerFormat(&stat) != CARD_STAT_BANNER_C8
        || CARDGetIconAnim(&stat) != CARD_STAT_ANIM_BOUNCE
        || CARDGetIconFormat(&stat, 0) != CARD_STAT_ICON_C8
        || CARDGetIconFormat(&stat, 1) != CARD_STAT_ICON_C8
        || CARDGetIconFormat(&stat, 2) != CARD_STAT_ICON_C8
        || CARDGetIconFormat(&stat, 3) != CARD_STAT_ICON_C8
        || CARDGetIconFormat(&stat, 4) != CARD_STAT_ICON_C8
        || CARDGetIconFormat(&stat, 5) != CARD_STAT_ICON_NONE
        || CARDGetIconFormat(&stat, 6) != CARD_STAT_ICON_NONE
        || CARDGetIconFormat(&stat, 7) != CARD_STAT_ICON_NONE
        || CARDGetIconSpeed(&stat, 0) != CARD_STAT_SPEED_FAST
        || CARDGetIconSpeed(&stat, 1) != CARD_STAT_SPEED_FAST
        || CARDGetIconSpeed(&stat, 2) != CARD_STAT_SPEED_FAST
        || CARDGetIconSpeed(&stat, 3) != CARD_STAT_SPEED_FAST
        || CARDGetIconSpeed(&stat, 4) != CARD_STAT_SPEED_FAST
        || CARDGetIconSpeed(&stat, 5) != CARD_STAT_SPEED_END
        || CARDGetIconSpeed(&stat, 6) != CARD_STAT_SPEED_END
        || CARDGetIconSpeed(&stat, 7) != CARD_STAT_SPEED_END)
    {
        return FALSE;
    }

    return TRUE;
}

static u32 mDoMemCdRWm_CalcCheckSum(void* data, u32 size) {
    u16 high;
    u16 low;

    low = 0;
    high = 0;
    
    u16* d = (u16*)data;
    for (int i = 0; i < size / 2; i++) {    
        high += *d;
        low += ~*d;
        d++;
    }

    return high << 16 | low;
}

static u64 mDoMemCdRWm_CalcCheckSumGameData(void* data, u32 size) {
    u32 high;
    u32 low;

    low = 0;
    high = 0;

    u8* d = (u8*)data;
    for (int i = 0; i < size; i++) {
        high += *d;
        low += ~*d;
        d++;
    }

    return (u64)high << 32 | low;
}

BOOL mDoMemCdRWm_TestCheckSumGameData(void* data) {
    u64 checksum = mDoMemCdRWm_CalcCheckSumGameData(data, (SAVEDATA_SIZE - sizeof(u64)));
    return checksum == *(u64*)((u8*)data + (SAVEDATA_SIZE - sizeof(u64)));
}

void mDoMemCdRWm_SetCheckSumGameData(u8* data, u8 dataNum) {
    u8* file_ptr = data + (dataNum * SAVEDATA_SIZE);
    *(u64*)(file_ptr + (SAVEDATA_SIZE - sizeof(u64))) = mDoMemCdRWm_CalcCheckSumGameData(file_ptr, (SAVEDATA_SIZE - sizeof(u64)));
}
