#include <dolphin/dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/syn.h>
#include "sdk_math.h"
#include "sdk_math.h"

#include "__syn.h"

s32 __SYNGetEnvelopeTime(s32 scale, s32 mod, u8 key) {
    if (scale == 0x80000000) {
        return 0;
    }

    if (mod == 0x80000000) {
        return (1000.0f* powf(2.0f, (f32)scale / (65536* 1200)));
    }

    return (1000.0f* powf(2.0f, ((f32)scale + (mod* __SYNn128[key])) / (65535* 1200)));
}

void __SYNSetupVolumeEnvelope(SYNVOICE* voice) {
    ASSERTLINE(46, voice);

    if (voice->art->eg1Attack + 0x80000000 == 0) {
        voice->veState = 1;
        voice->veAttn = 0;
        if (voice->art->eg1Decay + 0x80000000 == 0) {
            voice->veState = 2;
            voice->veAttn = voice->art->eg1Sustain;
        }
    } else {
        s32 frames = __SYNGetEnvelopeTime(voice->art->eg1Attack, voice->art->eg1Vel2Attack, voice->keyVel) / 5;
        if (frames != 0) {
            voice->veAttack = 0;
            voice->veAttackDelta = 0x640000 / frames;
            voice->veAttn = 0xFC400000;
            voice->veState = 0;
        } else {
            voice->veAttack = 0;
            voice->veAttackDelta = 0x640000;
            voice->veAttn = 0xFC400000;
            voice->veState = 0;
        }
    }

    if (voice->veState < 2) {
        s32 frames = __SYNGetEnvelopeTime(voice->art->eg1Decay, voice->art->eg1Key2Decay, voice->keyNum) / 5;
        if (frames != 0) {
            voice->veDecay = -0x03C00000 / frames;
        } else {
            voice->veDecay = -0x03C00000;
        }
    }

    voice->veSustain = voice->art->eg1Sustain;
    voice->veRelease = voice->art->eg1Release;
}

void __SYNSetupPitchEnvelope(SYNVOICE* voice) {
    ASSERTLINE(110, voice);

    voice->peCents = 0;
    voice->pePitch = voice->art->eg2Pitch;
    if (voice->pePitch != 0) {
        if (voice->art->eg2Attack + 0x80000000 == 0) {
            voice->peState = 1;
            voice->peCents = voice->pePitch;
            if (voice->art->eg2Decay + 0x80000000 == 0) {
                voice->peState = 2;
                voice->peCents = voice->art->eg2Sustain;
            }
        } else {
            s32 frames = __SYNGetEnvelopeTime(voice->art->eg2Attack, voice->art->eg2Vel2Attack, voice->keyVel) / 5;
            if (frames != 0) {
                voice->peAttack = voice->pePitch / frames;
                voice->peState = 0;
            } else {
                voice->peAttack = voice->pePitch;
                voice->peState = 0;
            }
        }

        if (voice->peState < 2) {
            s32 frames = __SYNGetEnvelopeTime(voice->art->eg2Decay, voice->art->eg2Key2Decay, voice->keyNum) / 5;
            if (frames != 0) {
                voice->peDecay = voice->pePitch / frames;
            } else {
                voice->peDecay = voice->pePitch;
            }
            voice->peDecay = voice->peDecay * -1;
        }
        voice->peSustain = voice->art->eg2Sustain;
        voice->peRelease = voice->art->eg2Release;
    }
}

void __SYNRunVolumeEnvelope(SYNVOICE* voice) {
    ASSERTLINE(178, voice);

    switch(voice->veState) {
    case 0:
        voice->veAttack = (voice->veAttack + voice->veAttackDelta);
        if (voice->veAttack >= 0x630000) {
            voice->veAttn = 0;
        } else {
            voice->veAttn = __SYNAttackAttnTable[voice->veAttack >> 0x10];
        }
        if (voice->veAttn == 0) {
            voice->veState = 1;
        }
    case 2:
        return;
    case 1:
        voice->veAttn = (voice->veAttn + voice->veDecay);
        if (voice->veAttn <= voice->veSustain) {
            voice->veAttn = voice->veSustain;
            voice->veState = 2;
        }
        if (voice->veAttn <= -0x02D00000) {
            voice->veState = 4;
            voice->synth->voice[voice->midiChannel][voice->keyNum] = 0;
        }
        return;
    case 3:
        if (voice->veAttn <= -0x02D00000) {
            voice->veState = 4;
        } else {
            voice->veAttn = (voice->veAttn + voice->veRelease);
        }
        return;
    }
}

void __SYNRunPitchEnvelope(SYNVOICE* voice) {
    ASSERTLINE(243, voice);

    if (voice->pePitch != 0) {
        switch(voice->peState) {
        case 0:
            voice->peCents = (voice->peCents + voice->peAttack);
            if (voice->pePitch > 0) {
                if (voice->peCents >= voice->pePitch) {
                    voice->pePitch = voice->peCents;
                    voice->peState = 1;
                }
            } else if (voice->peCents <= voice->pePitch) {
                voice->pePitch = voice->peCents;
                voice->peState = 1;
            }
        case 2:
            return;
        case 1:
            voice->peCents = (voice->peCents + voice->peDecay);
            if (voice->pePitch > 0) {
                if (voice->peCents <= 0) {
                    voice->peCents = 0;
                    voice->pePitch = 0;
                } else if (voice->peCents <= voice->peSustain) {
                    voice->peCents = voice->peSustain;
                    voice->peState = 2;
                }
            } else {
                if (voice->peCents >= 0) {
                    voice->peCents = 0;
                    voice->pePitch = 0;
                } else if (voice->peCents >= voice->peSustain) {
                    voice->peCents = voice->peSustain;
                    voice->peState = 2;
                }
            }
            break;
        case 3:
            voice->peCents = (voice->peCents + voice->peRelease);
            if (voice->pePitch > 0) {
                if (voice->peCents <= 0) {
                    voice->peCents = 0;
                    voice->pePitch = 0;
                }
            } else if (voice->peCents >= 0) {
                voice->peCents = 0;
                voice->pePitch = 0;
            }
        }
    }
}
