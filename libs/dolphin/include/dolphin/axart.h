#ifndef _DOLPHIN_AXART_H_
#define _DOLPHIN_AXART_H_

#include <dolphin/types.h>
#include <dolphin/ax.h>

#ifdef __cplusplus
extern "C" {
#endif

enum __axart_type {
    AXART_TYPE_NONE,
    AXART_TYPE_3D,
    AXART_TYPE_PANNING,
    AXART_TYPE_ITD,
    AXART_TYPE_SRC,
    AXART_TYPE_PITCH,
    AXART_TYPE_PITCH_ENV,
    AXART_TYPE_PITCH_MOD,
    AXART_TYPE_VOLUME,
    AXART_TYPE_AUX_A_VOLUME,
    AXART_TYPE_AUX_B_VOLUME,
    AXART_TYPE_VOLUME_ENV,
    AXART_TYPE_AUX_A_VOLUME_ENV,
    AXART_TYPE_AUX_B_VOLUME_ENV,
    AXART_TYPE_VOLUME_MOD,
    AXART_TYPE_AUX_A_VOLUME_MOD,
    AXART_TYPE_AUX_B_VOLUME_MOD,
    AXART_TYPE_LPF,

    AXART_TYPE_NUM
};

typedef struct {
    void* next;
    u32 type;
} AXART_ART;

typedef struct {
    f32* lfo;
    u32 length;
    f32 delta;
    u32 sampleIndex;
    f32 counter;
    f32 sample1;
    f32 sample;
    f32 output;
} AXART_LFO;

typedef struct {
    AXART_ART art;
    f32 hAngle;
    f32 vAngle;
    f32 dist;
    f32 closingSpeed;
    u32 update;
    u8 pan;
    u8 span;
    u8 src;
    u16 itdL;
    u16 itdR;
    f32 pitch;
    s32 attenuation;
} AXART_3D;

typedef struct {
    AXART_ART art;
    u8 pan;
    u8 span;
} AXART_PANNING;

typedef struct {
    AXART_ART art;
    u16 itdL;
    u16 itdR;
} AXART_ITD;

typedef struct {
    AXART_ART art;
    u8 src;
} AXART_SRC;

typedef struct {
    AXART_ART art;
    s32 cents;
} AXART_PITCH;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 cents;
} AXART_PITCH_ENV;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 cents;
} AXART_PITCH_MOD;

typedef struct {
    AXART_ART art;
    s32 attenuation;
} AXART_VOLUME;

typedef struct {
    AXART_ART art;
    s32 attenuation;
} AXART_AUXA_VOLUME;

typedef struct {
    AXART_ART art;
    s32 attenuation;
} AXART_AUXB_VOLUME;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_VOLUME_ENV;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_AUXA_VOLUME_ENV;

typedef struct {
    AXART_ART art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_AUXB_VOLUME_ENV;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 attenuation;
} AXART_VOLUME_MOD;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 attenuation;
} AXART_AUXA_VOLUME_MOD;

typedef struct {
    AXART_ART art;
    AXART_LFO lfo;
    s32 attenuation;
} AXART_AUXB_VOLUME_MOD;

typedef struct {
    AXART_ART art;
    u32 initLPF;
    u32 frequency;
    u32 update;
} AXART_LPF;

typedef struct {
    void* next;
    void* prev;
    AXVPB* axvpb;
    f32 sampleRate;
    AXART_ART* articulators;
} AXART_SOUND;

#define AXART_SINE_CNT 64
extern f32 AXARTSine[AXART_SINE_CNT];

// axart
void AXARTInit(void);
void AXARTQuit(void);
void AXARTServiceSounds(void);
void AXARTAddSound(AXART_SOUND* sound);
void AXARTRemoveSound(AXART_SOUND* sound);
void AXARTInitLfo(AXART_LFO* lfo, f32* samples, u32 length, f32 delta);
void AXARTInitArt3D(AXART_3D* articulator);
void AXARTInitArtPanning(AXART_PANNING* articulator);
void AXARTInitArtItd(AXART_ITD* articulator);
void AXARTInitArtSrctype(AXART_SRC* articulator);
void AXARTInitArtPitch(AXART_PITCH* articulator);
void AXARTInitArtPitchEnv(AXART_PITCH_ENV* articulator);
void AXARTInitArtPitchMod(AXART_PITCH_MOD* articulator);
void AXARTInitArtVolume(AXART_VOLUME* articulator);
void AXARTInitArtAuxAVolume(AXART_AUXA_VOLUME* articulator);
void AXARTInitArtAuxBVolume(AXART_AUXB_VOLUME* articulator);
void AXARTInitArtVolumeEnv(AXART_VOLUME_ENV* articulator);
void AXARTInitArtAuxAVolumeEnv(AXART_AUXA_VOLUME_ENV* articulator);
void AXARTInitArtAuxBVolumeEnv(AXART_AUXB_VOLUME_ENV* articulator);
void AXARTInitArtVolumeMod(AXART_VOLUME_MOD* articulator);
void AXARTInitArtAuxAVolumeMod(AXART_AUXA_VOLUME_MOD* articulator);
void AXARTInitArtAuxBVolumeMod(AXART_AUXB_VOLUME_MOD* articulator);
void AXARTInitArtLpf(AXART_LPF* articulator);

// axart3d
void AXARTSet3DDistanceScale(f32 scale);
void AXARTSet3DDopplerScale(f32 scale);
void AXART3DSound(AXART_3D* articulator);

// axartcents
f32 AXARTCents(s32 cents);

// axartenv
void AXARTPitchEnv(AXART_PITCH_ENV* articulator);
void AXARTVolumeEnv(AXART_VOLUME_ENV* articulator);

// axartlfo
void AXARTLfo(AXART_LFO* lfo);

// axartsound
void AXARTServiceSound(AXART_SOUND* sound);
void AXARTAddArticulator(AXART_SOUND* sound, AXART_ART* articulator);

// axartlpf
void AXARTLpf(AXART_LPF*, AXVPB*);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_AXART_H_
