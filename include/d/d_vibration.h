#ifndef D_D_VIBRATION_H
#define D_D_VIBRATION_H

#include "SSystem/SComponent/c_xyz.h"
#include "JSystem/JHostIO/JORReflexible.h"

struct vib_pattern {
    /* 0x0 */ u16 rounds;
    /* 0x2 */ u16 length;
    /* 0x4 */ u32 bits;
};

enum VIBMODE_SHOCK {
    VIBMODE_S_NONE = -1,
    VIBMODE_S_CUSTOM,
    VIBMODE_S_POWER1,
    VIBMODE_S_POWER2,
    VIBMODE_S_POWER3,
    VIBMODE_S_POWER4,
    VIBMODE_S_POWER5,
    VIBMODE_S_POWER6,
    VIBMODE_S_POWER7,
    VIBMODE_S_POWER8,
    VIBMODE_S_DOKUTT,
    VIBMODE_S_FOR2D,

    VIBMODE_S_MAX
};

enum VIBMODE_QUAKE {
    VIBMODE_Q_NONE = -1,
    VIBMODE_Q_CUSTOM,
    VIBMODE_Q_POWER1,
    VIBMODE_Q_POWER2,
    VIBMODE_Q_POWER3,
    VIBMODE_Q_POWER4,
    VIBMODE_Q_POWER5,
    VIBMODE_Q_POWER6,
    VIBMODE_Q_POWER7,
    VIBMODE_Q_POWER8,
    VIBMODE_Q_HORSE,

    VIBMODE_Q_MAX
};

class dVibTest_c : public JORReflexible {
public:
    dVibTest_c();

    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void genMessage(JORMContext*);
    virtual ~dVibTest_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x06 */ u16 m_pattern;
    /* 0x08 */ u16 m_pattern2;
    /* 0x0A */ u16 field_0xa;
    /* 0x0C */ s16 m_randombit;
    /* 0x0E */ s16 m_length;
    /* 0x10 */ int field_0x10;
    /* 0x14 */ int m_vibswitch;
    /* 0x18 */ u16 m_displayDbg;
};

class dVibration_c {
public:
    enum {
        MODE_PAUSE = -1,
        MODE_WAIT,
        MODE_RUN,
    };

    int Run();
    bool StartShock(int i_vibmode, int i_flags, cXyz i_pos);
    bool StartQuake(const u8* i_pattern, int i_rounds, int i_flags, cXyz i_pos);
    bool StartQuake(int i_vibmode, int i_flags, cXyz i_pos);
    int StopQuake(int i_flags);
    void Kill();
    bool CheckQuake();
    void setDefault();
    void Init();
    void Pause();
    void Remove();

    static const vib_pattern MS_patt[VIBMODE_S_MAX];
    static const vib_pattern CS_patt[VIBMODE_S_MAX];
    static const vib_pattern MQ_patt[VIBMODE_Q_MAX];
    static const vib_pattern CQ_patt[VIBMODE_Q_MAX];

private:
#if DEBUG
    /* 0x00 */ dVibTest_c mVibTest;
#endif

    class {
    public:
        class {
        public:
            /* 0x00 */ u32 field_0x0; 
            /* 0x04 */ int mVibMode;
            /* 0x08 */ u32 mPattern;
            /* 0x0C */ s32 mLength;
            /* 0x10 */ s32 mRounds;
            /* 0x14 */ s32 mFlags;
            /* 0x18 */ cXyz mPos;
            /* 0x24 */ s32 mFrame;
        } mShock, mQuake;
    } /* 0x00 */ mCamera;
    class {
    public:
        class {
        public:
            /* 0x00 */ u32 field_0x0; 
            /* 0x04 */ int mVibMode;
            /* 0x08 */ u32 mPattern;
            /* 0x0C */ s32 mLength;
            /* 0x10 */ s32 mRounds;
            /* 0x14 */ s32 mFrame;
            /* 0x18 */ s32 mStopFrame;
        } mShock, mQuake;
    } /* 0x50 */ mMotor;
    /* 0x88 */ s32 mFrame;
    /* 0x8C */ s32 mMode;
};  // Size: 0x90

#endif /* D_D_VIBRATION_H */
