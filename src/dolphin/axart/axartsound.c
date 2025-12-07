#include <dolphin/axart.h>
#include <dolphin/os.h>
#include <dolphin/mix.h>

void AXARTServiceSound(AXART_SOUND* sound) {
    AXART_ART* articulator;
    AXVPB* axvpb;
    s32 cents;
    s32 atten;
    s32 auxA;
    s32 auxB;
    f32 pitch;
    u8 pan;
    u8 span;
    u8 src;
    u16 itdL;
    u16 itdR;

    cents = atten = auxA = auxB = 0;
    pitch = sound->sampleRate / 32000.0f;
    pan = 64;
    span = 127;
    src = 1;
    itdL = itdR = 0;

    for (articulator = sound->articulators; articulator != 0; articulator = (AXART_ART*)articulator->next) {
        switch (articulator->type) {
        case AXART_TYPE_3D:
            AXART3DSound((AXART_3D*)articulator);
            pan = ((AXART_3D*)articulator)->pan;
            span = ((AXART_3D*)articulator)->span;
            itdL = ((AXART_3D*)articulator)->itdL;
            itdR = ((AXART_3D*)articulator)->itdR;
            src = ((AXART_3D*)articulator)->src;
            pitch += ((AXART_3D*)articulator)->pitch;
            atten += ((AXART_3D*)articulator)->attenuation;
            break;
        case AXART_TYPE_PANNING:
            pan = ((AXART_PANNING*)articulator)->pan;
            span = ((AXART_PANNING*)articulator)->span;
            break;
        case AXART_TYPE_ITD:
            itdL = ((AXART_ITD*)articulator)->itdL;
            itdR = ((AXART_ITD*)articulator)->itdR;
            break;
        case AXART_TYPE_SRC:
            src = ((AXART_SRC*)articulator)->src;
            break;
        case AXART_TYPE_PITCH:
            cents += ((AXART_PITCH*)articulator)->cents;
            break;
        case AXART_TYPE_PITCH_ENV:
            AXARTPitchEnv((AXART_PITCH_ENV*)articulator);
            cents += ((AXART_PITCH_ENV*)articulator)->cents;
            break;
        case AXART_TYPE_PITCH_MOD:
            AXARTLfo(&((AXART_PITCH_MOD*)articulator)->lfo);
            cents += (s32)(((AXART_PITCH_MOD*)articulator)->cents * ((AXART_PITCH_MOD*)articulator)->lfo.output);
            break;
        case AXART_TYPE_VOLUME:
            atten += ((AXART_VOLUME*)articulator)->attenuation;
            break;
        case AXART_TYPE_AUX_A_VOLUME:
            auxA += ((AXART_AUXA_VOLUME*)articulator)->attenuation;
            break;
        case AXART_TYPE_AUX_B_VOLUME:
            auxB += ((AXART_AUXB_VOLUME*)articulator)->attenuation;
            break;
        case AXART_TYPE_VOLUME_ENV:
            AXARTVolumeEnv((AXART_VOLUME_ENV*)articulator);
            atten += ((AXART_VOLUME_ENV*)articulator)->attenuation;
            break;
        case AXART_TYPE_AUX_A_VOLUME_ENV:
            AXARTVolumeEnv((AXART_VOLUME_ENV*)articulator);
            auxA += ((AXART_AUXA_VOLUME_ENV*)articulator)->attenuation;
            break;
        case AXART_TYPE_AUX_B_VOLUME_ENV:
            AXARTVolumeEnv((AXART_VOLUME_ENV*)articulator);
            auxB += ((AXART_AUXB_VOLUME_ENV*)articulator)->attenuation;
            break;
        case AXART_TYPE_VOLUME_MOD:
            AXARTLfo(&((AXART_VOLUME_MOD*)articulator)->lfo);
            atten += (s32)(((AXART_VOLUME_MOD*)articulator)->attenuation * ((AXART_VOLUME_MOD*)articulator)->lfo.output);
            break;
        case AXART_TYPE_AUX_A_VOLUME_MOD:
            AXARTLfo(&((AXART_VOLUME_MOD*)articulator)->lfo);
            auxA += (s32)(((AXART_AUXA_VOLUME_MOD*)articulator)->attenuation * ((AXART_AUXA_VOLUME_MOD*)articulator)->lfo.output);
            break;
        case AXART_TYPE_AUX_B_VOLUME_MOD:
            AXARTLfo(&((AXART_VOLUME_MOD*)articulator)->lfo);
            auxB += (s32)(((AXART_AUXB_VOLUME_MOD*)articulator)->attenuation * ((AXART_AUXB_VOLUME_MOD*)articulator)->lfo.output);
            break;
        case AXART_TYPE_LPF:
            AXARTLpf((AXART_LPF*)articulator, sound->axvpb);
            break;
        default:
#if DEBUG
            OSPanic(__FILE__, 196, "unknown articulator type!\n");
#endif
            break;
        }
    }

    pitch *= AXARTCents(cents >> 16);
    axvpb = sound->axvpb;
    AXSetVoiceSrcType(axvpb, src);
    AXSetVoiceSrcRatio(axvpb, pitch);
    AXSetVoiceItdTarget(axvpb, itdL, itdR);
    MIXSetInput(axvpb, atten >> 16);
    MIXSetAuxA(axvpb, auxA >> 16);
    MIXSetAuxB(axvpb, auxB >> 16);
    MIXSetPan(axvpb, pan);
    MIXSetSPan(axvpb, span);
}

void AXARTAddArticulator(AXART_SOUND* sound, AXART_ART* articulator) {
    ASSERTLINE(232, sound);
    ASSERTLINE(233, articulator);

    articulator->next = sound->articulators;
    sound->articulators = articulator;
}
