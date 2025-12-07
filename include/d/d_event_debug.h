#ifndef D_EVENT_D_EVENT_DEBUG_H
#define D_EVENT_D_EVENT_DEBUG_H

#include "JSystem/JHostIO/JORMContext.h"
#include "d/d_event.h"

struct dEvDb_bit_c {
    /* 0x00 */ char* mFlagName;
    /* 0x04 */ char* mFlagDescription;
    /* 0x08 */ char* mFlagAuthor;
    /* 0x0C */ const u16 mFlagValue;
    /* 0x10 */ char* mArea; // Area in the game where flag is used
    /* 0x14 */ u8 mRootBit;   
    /* 0x15 */ u8 field_0x15;
    /* 0x16 */ u8 mIsSet;
};

struct dEvDb_reg_c {
    /* 0x00 */ char* mFlagName;
    /* 0x04 */ char* mFlagDescription;
    /* 0x08 */ char* mFlagAuthor;
    /* 0x0C */ u16 mFlagValue;
    /* 0x10 */ char* mArea; // Area in the game where flag is used
    /* 0x14 */ u8 mRootReg;
    /* 0x15 */ u8 field_0x15;
    /* 0x16 */ u8 mIsSet;
};

struct dEvDb_flag_base_c {
    /* 0x00 */ dEvDb_bit_c* mBitTable;
    /* 0x04 */ dEvDb_reg_c* mRegTable;
    /* 0x08 */ int mTotalBitNum;
    /* 0x0C */ int mTotalRegNum;
    /* 0x10 */ int mNumRootBits;
    /* 0x14 */ int mNumRootRegs;

    char* searchDirNameBit(int);
    char* searchDirNameReg(int);
};

struct dEvM_play_HIO_c : public JORReflexible {
    dEvM_play_HIO_c();
    virtual ~dEvM_play_HIO_c() {}

    /* 0x04 */ s16 mTargetEvent;
    /* 0x06 */ s16 mEventIdx;
    /* 0x08 */ s16 mEventCameraMode;
    /* 0x0A */ s16 field_0xA;

    void genMessage(JORMContext*);
    void listenPropertyEvent(const JORPropertyEvent*);
    
    void removeComboBox();
    void addComboBox();

    enum JOR_PROPERTY_EVENT_IDs_e {
        LBL_EVENT_PLAYBACK_TESTING = (1 << 31) + 1,
        BTN_PLAYBACK = (1 << 26) + 1,
        BTN_STOP
    };

    enum CAMERA_MODES_e {
        UNSET = 0,
        PLAYBACK,
        STOP
    };
};

STATIC_ASSERT(sizeof(dEvM_play_HIO_c) == 0xC);

struct dEvM_reg_HIO_c : public JORReflexible {
    dEvM_reg_HIO_c();
    virtual ~dEvM_reg_HIO_c() {}

    /* 0x04 */ dEvDb_flag_base_c* mFlagTables;
    /* 0x08 */ u8 field_0x008;
    /* 0x09 */ u8 mRootRegIdx;

    void genMessage(JORMContext*);
    void listenPropertyEvent(const JORPropertyEvent*);

    void update();

    enum JOR_PROPERTY_EVENT_IDs_e {
        LBL_EVENT_SAVE_REGISTER = (1 << 31) + 1,
        BTN_UPDATE = (1 << 28)
    };
};

STATIC_ASSERT(sizeof(dEvM_reg_HIO_c) == 0xC);

struct dEvM_bit_HIO_c : public JORReflexible {
    dEvM_bit_HIO_c();
    virtual ~dEvM_bit_HIO_c() {}

    /* 0x04 */ dEvDb_flag_base_c* mFlagTables;
    /* 0x08 */ u8 field_0x008;
    /* 0x09 */ u8 mRootBitIdx;

    void genMessage(JORMContext*);
    void listenPropertyEvent(const JORPropertyEvent*);

    void update();

    enum JOR_PROPERTY_EVENT_IDs_e {
        LBL_EVENT_SAVE_BIT = (1 << 31) + 1,
        BTN_UPDATE = (1 << 28)
    };
};

STATIC_ASSERT(sizeof(dEvM_bit_HIO_c) == 0xC);

struct dEvM_root_bit_HIO_c : public JORReflexible {
    dEvM_root_bit_HIO_c() {}
    virtual ~dEvM_root_bit_HIO_c() {}

    dEvM_bit_HIO_c mBit[100];
    dEvDb_flag_base_c* mFlagTables;

    void genMessage(JORMContext*);
};

STATIC_ASSERT(sizeof(dEvM_root_bit_HIO_c) == 0x4B8);

struct dEvM_root_reg_HIO_c : public JORReflexible {
    dEvM_root_reg_HIO_c() {}
    virtual ~dEvM_root_reg_HIO_c() {}

    dEvM_reg_HIO_c mReg[100];
    dEvDb_flag_base_c* mFlagTables;

    void genMessage(JORMContext*);
};

STATIC_ASSERT(sizeof(dEvM_root_reg_HIO_c) == 0x4B8);

struct dEvM_HIO_c : public JORReflexible {
    dEvM_HIO_c();
    virtual ~dEvM_HIO_c() {}

    /* 0x004 */ s8 field_0x004;
    /* 0x005 */ u8 m_EVdata_output;
    /* 0x006 */ u8 m_evm_debug;
    /* 0x007 */ u8 m_evd_debug;
    /* 0x008 */ u8 m_eve_debug;
    /* 0x009 */ u8 m_evd_unfinished;
    /* 0x00A */ u8 m_prioritize_file;
    /* 0x00B */ u8 m_enable_skip;
    /* 0x00C */ u8 field_0x00C;
    /* 0x00D */ u8 m_prevent_scene_switch;
    /* 0x010 */ f32 field_0x010;
    /* 0x014 */ f32 field_0x014;
    /* 0x018 */ f32 field_0x018;
    /* 0x01C */ s32 m_corrective_sound_adjustment;
    /* 0x020 */ dEvM_play_HIO_c m_playtest;
    /* 0x02C */ dEvM_root_bit_HIO_c m_save_bit;
    /* 0x4E4 */ dEvM_root_bit_HIO_c m_temp_bit;
    /* 0x99C */ dEvM_root_reg_HIO_c m_save_reg;
    /* 0xE54 */ dEvM_root_reg_HIO_c m_temp_reg;

    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    bool setDebugCameraData(void*);

    enum JOR_PROPERTY_EVENT_IDs_e {
        LBL_EVENT_MANAGER_TESTING = (1 << 31) + 1,
        BTN_READ,
        BTN_FORCED_TERMINATION = (1 << 31) + 4
    };
};

#endif /* D_EVENT_D_EVENT_DEBUG_H */
