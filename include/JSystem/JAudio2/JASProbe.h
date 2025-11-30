#ifndef JASPROBE_H
#define JASPROBE_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASProbe {
    static void start(s32, char const*);
    void start(char const*);
    void stop();
    static void stop(s32);

    static JASProbe* sProbeTable[16];

    /* 0x000 */ char const* mName;
	/* 0x004 */ long mStartTime;
	/* 0x008 */ f32 _08;
	/* 0x00C */ f32 _0C;
	/* 0x010 */ f32 _10;
	/* 0x014 */ f32 _14;
	/* 0x018 */ f32 _18[100]; 
	/* 0x1A8 */ u32 _1A8;
};

#endif /* JASPROBE_H */
