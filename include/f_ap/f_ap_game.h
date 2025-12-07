#ifndef F_AP_F_AP_GAME_H
#define F_AP_F_AP_GAME_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JFramework/JFWDisplay.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "CaptureScreen.h"
#include <string.h>

void fapGm_After();
void fapGm_Create();
void fapGm_Execute();

class fapGm_HIO_c : public JORReflexible {
public:
    fapGm_HIO_c();

    #if DEBUG
    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void genMessage(JORMContext*);
    #endif
    virtual ~fapGm_HIO_c();

    #if DEBUG
    static void startCpuTimer();
    static void stopCpuTimer(const char*);
    static void printCpuTimer(const char*);
    static void executeCaptureScreen();

    static void createCaptureScreen() {
        mCaptureScreen = new CaptureScreen(JFWDisplay::getManager());
        JUT_ASSERT(46, mCaptureScreen != NULL);
    }

    static u8 isCaptureScreen() {
        return mCaptureScreenFlag;
    }

    static void onCpuTimer() {
        m_CpuTimerOn = TRUE;
    }

    static void offCpuTimer() {
        m_CpuTimerOff = FALSE;
    }
    #endif

    #if DEBUG
    static u8 m_CpuTimerOn;
    static u8 m_CpuTimerOff;
    static u8 m_CpuTimerStart;
    static u32 m_CpuTimerTick;

    static CaptureScreen* mCaptureScreen;
    static void* mCaptureScreenBuffer;
    static s16 mCaptureScreenFlag;
    static u16 mCaptureScreenWidth;
    static u16 mCaptureScreenHeight;
    static u16 mCaptureScreenLinePf;
    static u16 mCaptureScreenLineNum;
    static u8 mCaptureScreenNumH;
    static u8 mCaptureScreenNumV;

    static u8 mParticle254Fix;
    #endif

    /* 0x04 */ s8 field_0x04;
    /* 0x05 */ u8 field_0x03[3];
    /* 0x08 */ JUtility::TColor mColor;
    /* 0x0C */ f32 mLROnValue;
    /* 0x10 */ f32 mLROffValue;
    /* 0x14 */ u8 mUsingHostIO;
    /* 0x15 */ u8 mDisplayMeter;
    /* 0x16 */ u8 mDisplayPrint;
    /* 0x17 */ u8 mDisplay2D;
    /* 0x18 */ u8 mDisplayParticle;
    /* 0x19 */ u8 mDisplayProcessID;
    #if PLATFORM_WII || PLATFORM_SHIELD
    /* 0x1A */ u8 mTrapFilter;
    /* 0x1B */ u8 mGammaCorrection;
    #endif
    /* 0x1A */ u8 mMemBlockOff;
    /* 0x1C */ s16 mRegister0;
    /* 0x1E */ s16 mRegister1;
    /* 0x20 */ s16 mRegister2;
    /* 0x22 */ s16 mRegister3;
    #if DEBUG
    /* 0x26 */ u8 field_0x26[2];
    #endif
    /* 0x24 */ JUtility::TColor mLetterTopColor;
    /* 0x28 */ JUtility::TColor mLetterBottomColor;
    /* 0x2C */ JUtility::TColor mLetterTopShadowColor;
    /* 0x30 */ JUtility::TColor mLetterBottomShadowColor;
    /* 0x34 */ s16 mLetterPositionX;
    /* 0x36 */ s16 mLetterPositionY;
    /* 0x38 */ s16 mLineSpacing;
    /* 0x3A */ s16 mLetterSpacing;
    /* 0x3C */ s16 mLetterFontSize;
    /* 0x3E */ u8 mBackgroundAlpha;
};  // Size: 0x40

extern fapGm_HIO_c g_HIO;

inline void fapGmHIO_offMenu() {
    g_HIO.mDisplayPrint &= (u8)~2;
}

inline void fapGmHIO_onMenu() {
    g_HIO.mDisplayPrint |= (u8)2;
}

inline BOOL fapGmHIO_isMenu() {
    return g_HIO.mDisplayPrint & 2;
}

inline BOOL fapGmHIO_isPrint() {
    return g_HIO.mDisplayPrint & 1;
}

inline u8 fapGmHIO_getMeter() {
    return g_HIO.mDisplayMeter;
}

inline f32 fapGmHIO_getLROnValue() {
    return g_HIO.mLROnValue;
}

inline f32 fapGmHIO_getLROffValue() {
    return g_HIO.mLROffValue;
}

inline u8 fapGmHIO_getHostIO() {
    return g_HIO.mUsingHostIO;
}

inline void fapGmHIO_set2Ddraw(u8 param_0) {
    g_HIO.mDisplay2D = param_0;
}

inline u8 fapGmHIO_get2Ddraw() {
    #if DEBUG
    return g_HIO.mDisplay2D;
    #else
    return TRUE;
    #endif
}

inline void fapGmHIO_offPrint() {
    g_HIO.mDisplayPrint &= (u8)~0x1;
}

inline void fapGmHIO_onPrint() {
    g_HIO.mDisplayPrint |= (u8)0x1;
}

inline u8 fapGmHIO_getParticle() {
    #if DEBUG
    return g_HIO.mDisplayParticle;
    #else
    return TRUE;
    #endif
}

#if PLATFORM_WII || PLATFORM_SHIELD
inline u8 fapGmHIO_getTrapFilter() {
    return g_HIO.mTrapFilter;
}

inline u8 fapGmHIO_getGamma() {
    return g_HIO.mGammaCorrection;
}
#endif

struct fapGm_dataMem {
    enum HeapType_e {
        HeapType_Error_e,
        HeapType_ARAM_e,
        HeapType_Root_e,
        HeapType_System_e,
        HeapType_Zelda_e,
        HeapType_Archive_e,
        HeapType_Game_e,
        HeapType_Command_e,
        HeapType_Stage_e,
        HeapType_J2D_e,
        HeapType_Dynamic_e,
    };

    enum TagAttribute_e {
        TagAtt_Fixed_e,
        TagAtt_Variable_e,
    };

    enum TagType_e {
        TagType_Audio_e,
        TagType_Particle_e,
        TagType_System_e,
        TagType_Unknown_e,
        TagType_Message_e,
        TagType_2D_e,
        TagType_Player_e,
        TagType_Archive_e,
        TagType_ArchiveSub_e,
        TagType_ArchiveStage_e,
        TagType_ArchiveStageSub_e,
        TagType_Program_e,
        TagType_Object_e,
        TagType_Stage_e,
        TagType_Heap_e,
    };

    static void printfTag(int, int, int, const char*, void*, u32, const char*, const char*);
    static int findParentHeap(void*);
    static void dumpTag();
    static void dumpCsv();

    static int getCsvSize() { return strlen(mCsv); }
    static char* getCsv() { return mCsv; }
    static inline char* getCsvEnd() { return mCsv + strlen(mCsv); }

    static char mCsv[0x8000];
};

#endif /* F_AP_F_AP_GAME_H */
