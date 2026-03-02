#ifndef M_DO_M_DO_MEMCARD_H
#define M_DO_M_DO_MEMCARD_H

#include <card.h>
#include <os.h>
#include "global.h"

#define SAVEDATA_SIZE      0xA94    // Size of 1 Quest Log
#define SAVEDATA_NUM       3        // Max number of Quest Logs
#define SECTOR_SIZE        0x2000   // MemCard sector size
#define CARD_FILE_SIZE     0x8000   // MemCard file size
#define SAVEDATA_VERSION   6        // Save Data version

#define SAVEFILE_SIZE (SAVEDATA_SIZE * SAVEDATA_NUM)

class mDoMemCd_Ctrl_c {
public:
    enum Command_e {
        COMM_NONE_e,
        COMM_RESTORE_e,
        COMM_STORE_e,
        COMM_FORMAT_e,
        COMM_ATTACH_e,
        COMM_DETACH_e,

        #if PLATFORM_WII || PLATFORM_SHIELD
        COMM_RESTORE_NAND_e,
        COMM_STORE_NAND_e,
        COMM_STORE_SETUP_NAND_e,
        #endif
    };

    enum CardState_e {
        CARD_STATE_NO_CARD_e,
        CARD_STATE_READY_e,
        CARD_STATE_NO_FILE_e,
        CARD_STATE_READ_e,
        CARD_STATE_WRITE_e,
        CARD_STATE_FORMAT_e,
        CARD_STATE_WRONG_ENCODING_e,
        CARD_STATE_BROKEN_e,
        CARD_STATE_INSSPACE_e,
        CARD_STATE_NOENT_e,
        CARD_STATE_WRONG_DEVICE_e,
        CARD_STATE_WRONG_SECTORSIZE_e,
        CARD_STATE_FATAL_ERROR_e,
        CARD_STATE_13_e,
    };

    enum NANDState_e {
        NAND_STATE_AUTHENTICATION_e,
        NAND_STATE_READY_e,
        NAND_STATE_NO_FILE_e,
        NAND_STATE_READ_e,
        NAND_STATE_WRITE_e,
        NAND_STATE_FORMAT_e,
        NAND_STATE_BROKEN_e,
        NAND_STATE_INSSPACE_e,
        NAND_STATE_NOENT_e,
        NAND_STATE_FATAL_ERROR_e,
        NAND_STATE_10_e,
    };

    mDoMemCd_Ctrl_c();

    void ThdInit();
    void main();
    void update();
    void load();
    void restore();
    s32 LoadSync(void* i_buffer, u32 i_size, u32 i_position);
    void save(void* i_buffer, u32 i_size, u32 i_position);
    void store();
    s32 SaveSync();
    u32 getStatus(u32);
    void command_format();
    void format();
    s32 FormatSync();
    void attach();
    void command_attach();
    void detach();
    s32 mount();
    s32 loadfile();
    s32 checkspace();
    void setCardState(s32);

    #if PLATFORM_WII || PLATFORM_SHIELD
    void loadNAND();
    void restoreNAND();
    s32 LoadSyncNAND(void* i_buffer, u32 i_size, u32 i_position);
    void saveNAND(void* i_buffer, u32 i_size, u32 i_position);
    void storeNAND();
    s32 SaveSyncNAND();
    void storeSetUpNAND();
    void setNandState(s32);
    u32 getStatusNAND();
    s32 chekNANDFile();
    s32 checkspaceNAND();
    #endif

    bool isCardCommNone() { return mCardCommand == COMM_NONE_e; }
    u8 getProbeStat() { return mProbeStat; }
    void clearProbeStat() { mProbeStat = 2; }
    void setCopyToPos(u8 i_pos) { mCopyToPos = i_pos; }
    u32 getDataVersion() { return mDataVersion; }
    void setDataVersion(u32 version) { mDataVersion = version; }
    void getCardStatus(s32 fileNo, CARDStat* stat) { CARDGetStatus(mChannel, fileNo, stat); }
    void setCardStatus(s32 fileNo, CARDStat* stat) { CARDSetStatus(mChannel, fileNo, stat); }
    void setSerialNo() {
        u64 serial_no;
        CARDGetSerialNo(mChannel, &serial_no);
        mSerialNo = serial_no;
    }

    /* 0x0000 */ u8 mData[SAVEFILE_SIZE];
    /* 0x1FBC */ u8 mChannel;
    /* 0x1FBD */ u8 mCopyToPos;
    /* 0x1FBE */ u8 mProbeStat;
    /* 0x1FC0 */ s32 mCardCommand;
    /* 0x1FC4 */ s32 mCardState;
    /* 0x1FC8 */ s32 field_0x1fc8;
    /* 0x1FCC */ OSMutex mMutex;
    /* 0x1FE4 */ OSCond mCond;
    /* 0x1FEC */ s32 mNandState;
    /* 0x1FF0 */ u64 mSerialNo;
    /* 0x1FF8 */ u32 mDataVersion;
};  // Size: 0x2000

STATIC_ASSERT(sizeof(mDoMemCd_Ctrl_c) == 8192);

static int mDoMemCd_main(void*);

extern mDoMemCd_Ctrl_c g_mDoMemCd_control;

inline bool mDoMemCd_isCardCommNone() {
    return g_mDoMemCd_control.isCardCommNone();
}

inline void mDoMemCd_ThdInit() {
    g_mDoMemCd_control.ThdInit();
}

inline u8 mDoMemCd_getProbeStat() {
    return g_mDoMemCd_control.getProbeStat();
}

inline void mDoMemCd_clearProbeStat() {
    g_mDoMemCd_control.clearProbeStat();
}

inline void mDoMemCd_save(void* i_data, u32 param_1, u32 param_2) {
    g_mDoMemCd_control.save(i_data,param_1,param_2);
}

inline void mDoMemCd_Format() {
    g_mDoMemCd_control.command_format();
}

inline s32 mDoMemCd_FormatSync() {
    return g_mDoMemCd_control.FormatSync();
}

inline s32 mDoMemCd_SaveSync() {
    return g_mDoMemCd_control.SaveSync();
}

inline void mDoMemCd_Load() {
    g_mDoMemCd_control.load();
}

inline void mDoMemCd_setCopyToPos(u8 i_pos) {
    g_mDoMemCd_control.setCopyToPos(i_pos);
}

inline u32 mDoMemCd_getStatus(u32 param_0) {
    return g_mDoMemCd_control.getStatus(param_0);
}

inline u32 mDoMemCd_LoadSync(void* i_buffer, u32 i_size, u32 i_position) {
    return g_mDoMemCd_control.LoadSync(i_buffer, i_size, i_position);
}

inline u32 mDoMemCd_getDataVersion() {
    return g_mDoMemCd_control.getDataVersion();
}

inline void mDoMemCd_setDataVersion(u32 i_version) {
    g_mDoMemCd_control.setDataVersion(i_version);
}

inline void mDoMemCd_setSerialNo() {
    g_mDoMemCd_control.setSerialNo();
}

inline void mDoMemCd_getCardStatus(s32 fileNo, CARDStat* stat) {
    g_mDoMemCd_control.getCardStatus(fileNo, stat);
}

inline void mDoMemCd_setCardStatus(s32 fileNo, CARDStat* stat) {
    g_mDoMemCd_control.setCardStatus(fileNo, stat);
}

inline void mDoMemCd_UpDate() {
    g_mDoMemCd_control.update();
}

#if PLATFORM_WII || PLATFORM_SHIELD
inline void mDoMemCd_saveNAND(void* data, u32 length, u32 position) {
    g_mDoMemCd_control.saveNAND(data, length, position);
}

inline s32 mDoMemCd_SaveSyncNAND() {
    return g_mDoMemCd_control.SaveSyncNAND();
}

inline u32 mDoMemCd_LoadSyncNAND(void* i_buffer, u32 i_size, u32 i_position) {
    return g_mDoMemCd_control.LoadSyncNAND(i_buffer, i_size, i_position);
}

inline u32 mDoMemCd_getStatusNAND() {
    return g_mDoMemCd_control.getStatusNAND();
}

inline void mDoMemCd_LoadNAND() {
    g_mDoMemCd_control.loadNAND();
}

inline s32 mDoMemCd_checkNANDFile() {
    return g_mDoMemCd_control.chekNANDFile();
}
#endif

#endif /* M_DO_M_DO_MEMCARD_H */
