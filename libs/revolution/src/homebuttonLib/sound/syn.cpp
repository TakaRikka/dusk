#include "syn.h"


#include <macros.h>

#include <revolution/types.h>

#include "synprivate.h"

#include "mix.h"

// #include <revolution/ax.h>
// #include <revolution/os.h>
// #include <revolution/os/OSAddress.h>
// #include <revolution/os/OSInterrupt.h>



#define HBMSYN_MAX_VOICES	0x10


static void __HBMSYNAddSynthToList(HBMSYNSYNTH *synth);
static void __HBMSYNRemoveSynthFromList(HBMSYNSYNTH *synth);


// .bss
static HBMSYNSYNTH *__HBMSYNSynthList;
HBMSYNVOICE *__HBMSYNVoice;
static HBMSYNVOICE __s_HBMSYNVoice[HBMSYN_MAX_VOICES];
static int __init;


static void __HBMSYNAddSynthToList(HBMSYNSYNTH *synth)
{
	BOOL intrStatus = OSDisableInterrupts(); /* int old; */

	if (__HBMSYNSynthList)
		synth->next = __HBMSYNSynthList;
	else
		synth->next = NULL;

	__HBMSYNSynthList = synth;

	OSRestoreInterrupts(intrStatus);
}

static void __HBMSYNRemoveSynthFromList(HBMSYNSYNTH *synth)
{
	HBMSYNSYNTH *tempHead, *tempTail;
	HBMSYNSYNTH *tempSynth;

	BOOL intrStatus = OSDisableInterrupts(); /* int old; */

	tempHead = NULL;
	tempTail = NULL;

	for (tempSynth = __HBMSYNSynthList; tempSynth; tempSynth = tempSynth->next)
	{
		if (tempSynth != synth)
		{
			if (tempHead)
			{
				tempTail->next = tempSynth;
				tempTail = tempSynth;
			}
			else
			{
				tempTail = tempSynth;
				tempHead = tempTail;
			}
		}
	}

	if (tempTail)
		tempTail->next = NULL;

	__HBMSYNSynthList = tempHead;

	OSRestoreInterrupts(intrStatus);

	(void)tempSynth; // TODO: Where would this extra use go?
}

void HBMSYNInit(void)
{
	int i;

	ASSERTLINE(95, AXIsInit());

	if (AXIsInit() && !__init)
	{
		__HBMSYNVoice = __s_HBMSYNVoice;

		for (i = 0; i < HBMSYN_MAX_VOICES; ++i)
			__HBMSYNVoice[i].synth = NULL;

		__HBMSYNSynthList = NULL;
		__init = TRUE;
	}
}

void HBMSYNQuit(void)
{
	__HBMSYNVoice = NULL;
	__init = FALSE;
}

void HBMSYNRunAudioFrame(void)
{
	int i;
	HBMSYNSYNTH *synth;

	if (__init)
	{
		for (i = 0; i < HBMSYN_MAX_VOICES; ++i)
			__HBMSYNServiceVoice(i);

		for (synth = __HBMSYNSynthList; synth; synth = synth->next)
			__HBMSYNRunInputBufferEvents(synth);
	}
}

void HBMSYNInitSynth(HBMSYNSYNTH *synth, u8 *wavetable, u8 *samples,
                     u8 *zerobuffer ATTR_UNUSED)
{
	u32 *p;
	u32 mramBase;
	u32 midiChannel, noteNumber;

	ASSERTLINE(173, synth);
	ASSERTLINE(174, wavetable);
	ASSERTLINE(175, samples);

	p = (u32 *)wavetable;
	mramBase = (u32)OSCachedToPhysical(samples);

	// doubled statements
	synth->percussiveInst		= POINTER_ADD_TYPE(WTINST *,   wavetable, *p); ++p;
	synth->melodicInst			= POINTER_ADD_TYPE(WTINST *,   wavetable, *p); ++p;
	synth->region				= POINTER_ADD_TYPE(WTREGION *, wavetable, *p); ++p;
	synth->art					= POINTER_ADD_TYPE(WTART *,    wavetable, *p); ++p;
	synth->sample				= POINTER_ADD_TYPE(WTSAMPLE *, wavetable, *p); ++p;
	synth->adpcm				= POINTER_ADD_TYPE(WTADPCM *,  wavetable, *p); ++p;

	synth->samplesBaseWord		= mramBase >> 1;
	synth->samplesBaseByte		= mramBase;
	synth->samplesBaseNibble	= mramBase << 1;
	synth->masterVolume			= 0;

	__HBMSYNResetAllControllers(synth);

	synth->inputPosition		= *synth->input;
	synth->inputCounter			= 0;
	synth->notes				= 0;

	for (midiChannel = 0; midiChannel < HBMSYN_NUM_MIDI_CHANNELS; ++midiChannel)
	{
		for (noteNumber = 0; noteNumber < HBMSYN_NUM_MIDI_NOTES; ++noteNumber)
			synth->voice[midiChannel][noteNumber] = NULL;
	}

	__HBMSYNAddSynthToList(synth);
}

void HBMSYNQuitSynth(HBMSYNSYNTH *synth)
{
	int i;
	BOOL intrStatus = OSDisableInterrupts(); /* int old; */

	if (synth->notes)
	{
		for (i = 0; i < HBMSYN_MAX_VOICES; ++i)
		{
			HBMSYNVOICE *voice = __HBMSYNVoice + i;

			if (voice->synth == synth)
			{
				HBMMIXReleaseChannel(voice->axvpb);
				HBMFreeIndexByKey(voice->axvpb->index);
				AXFreeVoice(voice->axvpb);
				voice->synth = NULL;
			}
		}
	}

	__HBMSYNRemoveSynthFromList(synth);

	OSRestoreInterrupts(intrStatus);
}

void HBMSYNMidiInput(HBMSYNSYNTH *synth, u8 *input)
{
	u8 *src;

	ASSERTLINE(254, synth);
	ASSERTLINE(255, input);

	src = input;

	// tripled statements
	*synth->inputPosition = *src; ++synth->inputPosition; ++src;
	*synth->inputPosition = *src; ++synth->inputPosition; ++src;
	*synth->inputPosition = *src; ++synth->inputPosition; ++src;

	++synth->inputCounter;

	OSAssertMessage_Line(
		268, synth->inputCounter < HBMSYN_INPUT_BUFFER_SIZE,
		"synth input buffer exceeded, increase HBMSYN_INPUT_BUFFER_SIZE");
}

void HBMSYNSetMasterVolume(HBMSYNSYNTH *synth, s32 dB)
{
	ASSERTLINE(277, synth);

	synth->masterVolume = dB << 16;
}

s32 HBMSYNGetMasterVolume(HBMSYNSYNTH *synth)
{
	ASSERTLINE(288, synth);

	return synth->masterVolume >> 16;
}
