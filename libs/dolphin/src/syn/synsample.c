#include <dolphin/dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

// prototypes
static u32 __SYNGetNibbleAddress(u32 count);
static void __SYNSetupAdpcm(SYNVOICE* voice);
static void __SYNSetupPcm16(SYNVOICE* voice);
static void __SYNSetupPcm8(SYNVOICE* voice);

static u32 __SYNGetNibbleAddress(u32 count) {
    u32 samples = count;
    u32 frames = (samples / 14);
    u32 samplesLeft = (samples % 14);

    return (frames * 0x10) + 2 + samplesLeft;
}

static void __SYNSetupAdpcm(SYNVOICE* voice) {
    AXVPB* axvpb = voice->axvpb;

    if ((voice->region->loopStart + voice->region->loopLength) != 0) {
        u32 sampleStart;
        u32 sampleLoop;
        u32 sampleEnd;
        u32* p;
        u32* adpcm;
        u16* adpcmloop;

        adpcm = (void*)&voice->adpcm->a;
        voice->type = 1;
        sampleStart = voice->synth->aramBaseNibble + voice->sample->offset;
        sampleLoop = sampleStart + __SYNGetNibbleAddress(voice->region->loopStart);
        sampleEnd = sampleStart + __SYNGetNibbleAddress(voice->region->loopStart + voice->region->loopLength - 1);
        ASSERTLINE(79, (sampleStart & 0x000f) == 0);
        ASSERTLINE(80, (sampleLoop & 0x000f) > 1);
        ASSERTLINE(81, (sampleEnd & 0x000f) > 1);

        // the hell? why not just write the members??? what is this doing???
        // why not just write to the members directly?
        sampleStart = sampleStart + 2;
        p = (u32*)&axvpb->pb.addr;
        *(p) = 0x10000; p += 1;
        *(p) = sampleLoop; p += 1;
        *(p) = sampleEnd; p += 1;
        *(p) = sampleStart; p += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        adpcmloop = (void*)(adpcm);
        axvpb->pb.adpcmLoop.loop_pred_scale = *(adpcmloop); adpcmloop += 1;
        axvpb->pb.adpcmLoop.loop_yn1 = *(adpcmloop); adpcmloop += 1;
        axvpb->pb.adpcmLoop.loop_yn2 = *(adpcmloop); adpcmloop += 1;
        axvpb->sync &= 0xFFFE1FFF;
        axvpb->sync |= 0x121000;
    } else {
        u32 sampleStart;
        u32 sampleLoop;
        u32 sampleEnd;
        u32* p;
        u32* adpcm;

        adpcm = (void*)&voice->adpcm->a;
        voice->type = 0;
        sampleStart = voice->synth->aramBaseNibble + voice->sample->offset;
        sampleLoop = sampleStart + __SYNGetNibbleAddress(voice->synth->zeroBaseNibble);
        sampleEnd = sampleStart + __SYNGetNibbleAddress(voice->sample->length - 1);

        ASSERTLINE(135, (sampleStart & 0x000f) == 0);
        ASSERTLINE(136, (sampleEnd & 0x000f) > 1);

        // same wtf writes here
        sampleStart = sampleStart + 2;
        p = (void*)&axvpb->pb.addr;
        *(p) = 0; p += 1;
        *(p) = sampleLoop; p += 1;
        *(p) = sampleEnd; p += 1;
        *(p) = sampleStart; p += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        *(p) = *(adpcm); p += 1, adpcm += 1;
        axvpb->sync &= 0xFFFE1FFF;
        axvpb->sync |= 0x21000;
    }
}

static void __SYNSetupPcm16(SYNVOICE* voice) {
    AXVPB* axvpb = voice->axvpb;

    if ((voice->region->loopStart + voice->region->loopLength) != 0) {
        u32 sampleStart;
        u32 sampleLoop;
        u32 sampleEnd;
        u32* p;

        voice->type = 1;
        sampleStart = voice->synth->aramBaseWord + voice->sample->offset;
        sampleLoop = sampleStart + voice->region->loopStart;
        sampleEnd = sampleLoop + voice->region->loopLength - 1;

        p = (u32*)&axvpb->pb.addr;
        *(p) = 0x1000A; p += 1;
        *(p) = sampleLoop; p += 1;
        *(p) = sampleEnd; p += 1;
        *(p) = sampleStart; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0x08000000; p += 1;
        *(p) = 0; p += 1;
    } else {
        u32 sampleStart;
        u32 sampleLoop;
        u32 sampleEnd;
        u32* p;

        voice->type = 0;
        sampleStart = voice->synth->aramBaseWord + voice->sample->offset;
        sampleLoop = voice->synth->zeroBaseWord;
        sampleEnd = sampleStart + voice->sample->length - 1;
        
        p = (u32*)&axvpb->pb.addr;
        *(p) = 0x0000A; p += 1;
        *(p) = sampleLoop; p += 1;
        *(p) = sampleEnd; p += 1;
        *(p) = sampleStart; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0x08000000; p += 1;
        *(p) = 0; p += 1;
    }
    axvpb->sync &= 0xFFFE1FFF;
    axvpb->sync |= 0x21000;
}

static void __SYNSetupPcm8(SYNVOICE* voice) {
    AXVPB* axvpb = voice->axvpb;

    if ((voice->region->loopStart + voice->region->loopLength) != 0) {
        u32 sampleStart;
        u32 sampleLoop;
        u32 sampleEnd;
        u32* p;

        voice->type = 1;
        sampleStart = voice->synth->aramBaseByte + voice->sample->offset;
        sampleLoop = sampleStart + voice->region->loopStart;
        sampleEnd = sampleLoop + voice->region->loopLength - 1;
        p = (u32*)&axvpb->pb.addr;
        *(p) = 0x10019; p += 1;
        *(p) = sampleLoop; p += 1;
        *(p) = sampleEnd; p += 1;
        *(p) = sampleStart; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0x01000000; p += 1;
        *(p) = 0; p += 1;
    } else {
        u32 sampleStart;
        u32 sampleLoop;
        u32 sampleEnd;
        u32* p;

        voice->type = 0;
        sampleStart = voice->synth->aramBaseByte + voice->sample->offset;
        sampleLoop = voice->synth->zeroBaseByte;
        sampleEnd = sampleStart + voice->sample->length - 1;

        p = (u32*)&axvpb->pb.addr;
        *(p) = 0x19; p += 1;
        *(p) = sampleLoop; p += 1;
        *(p) = sampleEnd; p += 1;
        *(p) = sampleStart; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0; p += 1;
        *(p) = 0x01000000; p += 1;
        *(p) = 0; p += 1;
    }

    axvpb->sync &= 0xFFFE1FFF;
    axvpb->sync |= 0x21000;
}

void __SYNSetupSample(SYNVOICE* voice) {
    ASSERTLINE(361, voice);

    switch(voice->sample->format) {
    case SYN_SAMPLE_FORMAT_ADPCM:
        __SYNSetupAdpcm(voice);
        return;
    case SYN_SAMPLE_FORMAT_PCM16:
        __SYNSetupPcm16(voice);
        return;
    case SYN_SAMPLE_FORMAT_PCM8:
        __SYNSetupPcm8(voice);
        return;
    default:
        ASSERTMSGLINE(385, FALSE, "unknown sample format\n");
        return;
    }
}
