#ifndef JASDRUMSET_H
#define JASDRUMSET_H

#include "JSystem/JAudio2/JASBasicInst.h"

struct JKRHeap;

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASDrumSet : public JASInst {
    struct TPerc {
        TPerc();
        void setRelease(u32);

        void setVolume(f32 volume) { mVolume = volume; }
        void setPitch(f32 pitch) { mPitch = pitch; }
        void setPan(f32 pan) { mPan = pan; }

        /* 0x00 */ f32 mVolume;
        /* 0x04 */ f32 mPitch;
        /* 0x08 */ f32 mPan;
        /* 0x0C */ u16 field_0xc;
        /* 0x0E */ u16 field_0xe;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
    };

    JASDrumSet();
    virtual ~JASDrumSet();
    void newPercArray(u8, JKRHeap*);
    virtual bool getParam(int, int, JASInstParam*) const;
    void setPerc(int, JASDrumSet::TPerc*);
    JASDrumSet::TPerc* getPerc(int);
    JASDrumSet::TPerc* getPerc(int) const;
    virtual u32 getType() const;
    
    /* 0x4 */ TPerc** mPercArray;
    /* 0x8 */ u8 mPercNumMax;
};

#endif /* JASDRUMSET_H */
