#ifndef D_S_D_S_PLAY_H
#define D_S_D_S_PLAY_H

#include "d/d_s_play_env.h"
#include "f_op/f_op_scene.h"
#include "m_Do/m_Do_hostIO.h"

class mDoDvdThd_mountXArchive_c;
class mDoDvdThd_toMainRam_c;

class dScnPly_reg_childHIO_c {
public:
    /* 0x00 */ void* vtable;
    /* 0x04 */ f32 mFloatReg[30];
    /* 0x7C */ s16 mShortReg[10];
};

class dScnPly_reg_HIO_c : public JORReflexible {
public:
    virtual ~dScnPly_reg_HIO_c() {}

#if ENABLE_REGHIO
    void genMessage(JORMContext*);

    /* 0x4 */ s8 id;
    /* 0x8 */ dScnPly_reg_childHIO_c mChildReg[26];
#else
    /* 0x4 */ u8 field_0x4[0x8 - 0x4];
#endif
};

class dScnPly_preLoad_HIO_c : public mDoHIO_entry_c {
public:
    virtual ~dScnPly_preLoad_HIO_c() {}

    void genMessage(JORMContext*);
};

class dScnPly_env_HIO_c {
public:
    virtual ~dScnPly_env_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ dScnPly_env_otherHIO_c mOther;
    /* 0x14 */ dScnPly_env_debugHIO_c mDebug;
};

class dScnPly_preset_HIO_c : public JORReflexible {
public:
    dScnPly_preset_HIO_c();
    void exePreset();

    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void genMessage(JORMContext*);
    virtual ~dScnPly_preset_HIO_c() {}

    /* 0x0004 */ s8 field_0x4;
    /* 0x0005 */ u8 field_0x5;
    /* 0x0006 */ u8 mPresetData[10000];
    /* 0x2716 */ u8 field_0x2716;
    /* 0x2717 */ u8 field_0x2717;
    /* 0x2718 */ u8 filename_buf[100];
};

class dScnPly_c : public scene_class {
public:
    s8 calcPauseTimer();
    bool resetGame();
    void offReset();

    #if VERSION == VERSION_SHIELD_DEBUG
    static s8 isPause() { return pauseTimer | nextPauseTimer; }
    #else
    static s8 isPause() { return pauseTimer; }
    #endif
    static void setPauseTimer(s8 time) { nextPauseTimer = time; }

    static s8 pauseTimer;
    static s8 nextPauseTimer;

    /* 0x1C4 */ request_of_phase_process_class field_0x1c4;
    /* 0x1CC */ mDoDvdThd_toMainRam_c* sceneCommand;
    /* 0x1D0 */ mDoDvdThd_mountXArchive_c* field_0x1d0;
    /* 0x1D4 */ u8 field_0x1d4;
};

extern dScnPly_env_HIO_c g_envHIO;
extern dScnPly_reg_HIO_c g_regHIO;

#if DEBUG
extern dScnPly_preset_HIO_c g_presetHIO;
#endif

/**
 * === Register Usage ===
 * Short Reg(00-09) ... -32768 - +32768
 * Float Reg(00-19) ... -100000.0 - +100000.0
 * Float Reg(20-24) ... 0.0 - 1.0
 * Float Reg(25-29) ... -1.0 - +1.0
 */

#if ENABLE_REGHIO
// Morita
#define TREG_F(i) g_regHIO.mChildReg[0].mFloatReg[i]
#define TREG_S(i) g_regHIO.mChildReg[0].mShortReg[i]

// Matsutani
#define DREG_F(i) g_regHIO.mChildReg[1].mFloatReg[i]
#define DREG_S(i) g_regHIO.mChildReg[1].mShortReg[i]

// Iwawaki
#define IREG_F(i) g_regHIO.mChildReg[2].mFloatReg[i]
#define IREG_S(i) g_regHIO.mChildReg[2].mShortReg[i]

// Sasa
#define SREG_F(i) g_regHIO.mChildReg[3].mFloatReg[i]
#define SREG_S(i) g_regHIO.mChildReg[3].mShortReg[i]

// Umemiya
#define UREG_F(i) g_regHIO.mChildReg[4].mFloatReg[i]
#define UREG_S(i) g_regHIO.mChildReg[4].mShortReg[i]

// Ogawa
#define OREG_F(i) g_regHIO.mChildReg[5].mFloatReg[i]
#define OREG_S(i) g_regHIO.mChildReg[5].mShortReg[i]

// Sakaguchi
#define YREG_F(i) g_regHIO.mChildReg[6].mFloatReg[i]
#define YREG_S(i) g_regHIO.mChildReg[6].mShortReg[i]

// Sakai
#define KREG_F(i) g_regHIO.mChildReg[7].mFloatReg[i]
#define KREG_S(i) g_regHIO.mChildReg[7].mShortReg[i]

// Hara
#define BREG_F(i) g_regHIO.mChildReg[8].mFloatReg[i]
#define BREG_S(i) g_regHIO.mChildReg[8].mShortReg[i]

// Sumiyoshi
#define MREG_F(i) g_regHIO.mChildReg[9].mFloatReg[i]
#define MREG_S(i) g_regHIO.mChildReg[9].mShortReg[i]

// Takeshita
#define AREG_F(i) g_regHIO.mChildReg[10].mFloatReg[i]
#define AREG_S(i) g_regHIO.mChildReg[10].mShortReg[i]

// Yoshida
#define PREG_F(i) g_regHIO.mChildReg[11].mFloatReg[i]
#define PREG_S(i) g_regHIO.mChildReg[11].mShortReg[i]

// Noma
#define NREG_F(i) g_regHIO.mChildReg[12].mFloatReg[i]
#define NREG_S(i) g_regHIO.mChildReg[12].mShortReg[i]

// Nishiwaki
#define WREG_F(i) g_regHIO.mChildReg[13].mFloatReg[i]
#define WREG_S(i) g_regHIO.mChildReg[13].mShortReg[i]

// Masaro
#define JREG_F(i) g_regHIO.mChildReg[14].mFloatReg[i]
#define JREG_S(i) g_regHIO.mChildReg[14].mShortReg[i]

// Hayakawa
#define HREG_F(i) g_regHIO.mChildReg[15].mFloatReg[i]
#define HREG_S(i) g_regHIO.mChildReg[15].mShortReg[i]

// Sadamoto
#define VREG_F(i) g_regHIO.mChildReg[16].mFloatReg[i]
#define VREG_S(i) g_regHIO.mChildReg[16].mShortReg[i]

// Nishikawa
#define XREG_F(i) g_regHIO.mChildReg[17].mFloatReg[i]
#define XREG_S(i) g_regHIO.mChildReg[17].mShortReg[i]

// Suzuki
#define ZREG_F(i) g_regHIO.mChildReg[18].mFloatReg[i]
#define ZREG_S(i) g_regHIO.mChildReg[18].mShortReg[i]

// Nakanishi
#define nREG_F(i) g_regHIO.mChildReg[19].mFloatReg[i]
#define nREG_S(i) g_regHIO.mChildReg[19].mShortReg[i]

// Kuwajima
#define kREG_F(i) g_regHIO.mChildReg[20].mFloatReg[i]
#define kREG_S(i) g_regHIO.mChildReg[20].mShortReg[i]

// Okajima
#define oREG_F(i) g_regHIO.mChildReg[21].mFloatReg[i]
#define oREG_S(i) g_regHIO.mChildReg[21].mShortReg[i]

// Yamazaki
#define yREG_F(i) g_regHIO.mChildReg[22].mFloatReg[i]
#define yREG_S(i) g_regHIO.mChildReg[22].mShortReg[i]

// Ashida
#define aREG_F(i) g_regHIO.mChildReg[23].mFloatReg[i]
#define aREG_S(i) g_regHIO.mChildReg[23].mShortReg[i]

// Okada
#define hREG_F(i) g_regHIO.mChildReg[24].mFloatReg[i]
#define hREG_S(i) g_regHIO.mChildReg[24].mShortReg[i]

// Takahashi
#define tREG_F(i) g_regHIO.mChildReg[25].mFloatReg[i]
#define tREG_S(i) g_regHIO.mChildReg[25].mShortReg[i]
#else
// Morita
#define TREG_F(i) (0)
#define TREG_S(i) (0)

// Matsutani
#define DREG_F(i) (0)
#define DREG_S(i) (0)

// Iwawaki
#define IREG_F(i) (0)
#define IREG_S(i) (0)

// Sasa
#define SREG_F(i) (0)
#define SREG_S(i) (0)

// Umemiya
#define UREG_F(i) (0)
#define UREG_S(i) (0)

// Ogawa
#define OREG_F(i) (0)
#define OREG_S(i) (0)

// Sakaguchi
#define YREG_F(i) (0)
#define YREG_S(i) (0)

// Sakai
#define KREG_F(i) (0)
#define KREG_S(i) (0)

// Hara
#define BREG_F(i) (0)
#define BREG_S(i) (0)

// Sumiyoshi
#define MREG_F(i) (0)
#define MREG_S(i) (0)

// Takeshita
#define AREG_F(i) (0)
#define AREG_S(i) (0)

// Yoshida
#define PREG_F(i) (0)
#define PREG_S(i) (0)

// Noma
#define NREG_F(i) (0)
#define NREG_S(i) (0)

// Nishiwaki
#define WREG_F(i) (0)
#define WREG_S(i) (0)

// Masaro
#define JREG_F(i) (0)
#define JREG_S(i) (0)

// Hayakawa
#define HREG_F(i) (0)
#define HREG_S(i) (0)

// Sadamoto
#define VREG_F(i) (0)
#define VREG_S(i) (0)

// Nishikawa
#define XREG_F(i) (0)
#define XREG_S(i) (0)

// Suzuki
#define ZREG_F(i) (0)
#define ZREG_S(i) (0)

// Nakanishi
#define nREG_F(i) (0)
#define nREG_S(i) (0)

// Kuwajima
#define kREG_F(i) (0)
#define kREG_S(i) (0)

// Okajima
#define oREG_F(i) (0)
#define oREG_S(i) (0)

// Yamazaki
#define yREG_F(i) (0)
#define yREG_S(i) (0)

// Ashida
#define aREG_F(i) (0)
#define aREG_S(i) (0)

// Okada
#define hREG_F(i) (0)
#define hREG_S(i) (0)

// Takahashi
#define tREG_F(i) (0)
#define tREG_S(i) (0)
#endif

#endif /* D_S_D_S_PLAY_H */
