#include "synprivate.h"


#include <revolution/types.h>

#include "mix.h"

// #include <revolution/ax.h>
// #include <revolution/os.h>
// #include <revolution/os/OSInterrupt.h>



// MidiChannelVoiceMessage
#define	MIDI_MESSAGE_CHANNEL_VOICE_NOTE_OFF			 8
#define	MIDI_MESSAGE_CHANNEL_VOICE_NOTE_ON			 9
#define	MIDI_MESSAGE_CHANNEL_VOICE_CONTROL_CHANGE	11
#define	MIDI_MESSAGE_CHANNEL_VOICE_PROGRAM_CHANGE	12

// MidiChannelControl
#define	MIDI_CC_CHANNEL_VOLUME						 7
#define	MIDI_CC_PAN									10
#define	MIDI_CC_EXPRESSION							11
#define	MIDI_CC_REVERB_SEND_LEVEL					91
#define	MIDI_CC_TREMOLO_DEPTH						92


static void __HBMSYNProgramChange(HBMSYNSYNTH *synth, u8 midiChannel,
                                  u8 program);

static void __HBMSYNResetController(HBMSYNSYNTH *synth, u8 midiChannel);

static void __HBMSYNNoteOff(HBMSYNSYNTH *synth, u8 midiChannel, u8 keyNum);
static void __HBMSYNNoteOn(HBMSYNSYNTH *synth, u8 midiChannel, u8 keyNum,
                           u8 keyVel);
static void __HBMSYNMidiIn(HBMSYNSYNTH *synth, u8 *input);


// .data

// Why is this here anyways if its only use is in another file
f32 __HBMSYNn128[128] = // x/128, rounded to 5 decimal places
{
	0.0f,     0.007813f, 0.015625f, 0.023438f,
	0.03125f, 0.039063f, 0.046875f, 0.054688f,
	0.0625f,  0.070313f, 0.078125f, 0.085938f,
	0.09375f, 0.101563f, 0.109375f, 0.117188f,
	0.125f,   0.132813f, 0.140625f, 0.148438f,
	0.15625f, 0.164063f, 0.171875f, 0.179688f,
	0.1875f,  0.195313f, 0.203125f, 0.210938f,
	0.21875f, 0.226563f, 0.234375f, 0.242188f,
	0.25f,    0.257813f, 0.265625f, 0.273438f,
	0.28125f, 0.289063f, 0.296875f, 0.304688f,
	0.3125f,  0.320313f, 0.328125f, 0.335938f,
	0.34375f, 0.351563f, 0.359375f, 0.367188f,
	0.375f,   0.382813f, 0.390625f, 0.398438f,
	0.40625f, 0.414063f, 0.421875f, 0.429688f,
	0.4375f,  0.445313f, 0.453125f, 0.460938f,
	0.46875f, 0.476563f, 0.484375f, 0.492188f,
	0.5f,     0.507813f, 0.515625f, 0.523438f,
	0.53125f, 0.539063f, 0.546875f, 0.554688f,
	0.5625f,  0.570313f, 0.578125f, 0.585938f,
	0.59375f, 0.601563f, 0.609375f, 0.617188f,
	0.625f,   0.632813f, 0.640625f, 0.648438f,
	0.65625f, 0.664063f, 0.671875f, 0.679688f,
	0.6875f,  0.695313f, 0.703125f, 0.710938f,
	0.71875f, 0.726563f, 0.734375f, 0.742188f,
	0.75f,    0.757813f, 0.765625f, 0.773438f,
	0.78125f, 0.789063f, 0.796875f, 0.804688f,
	0.8125f,  0.820313f, 0.828125f, 0.835938f,
	0.84375f, 0.851563f, 0.859375f, 0.867188f,
	0.875f,   0.882813f, 0.890625f, 0.898438f,
	0.90625f, 0.914063f, 0.921875f, 0.929688f,
	0.9375f,  0.945313f, 0.953125f, 0.960938f,
	0.96875f, 0.976563f, 0.984375f, 0.992188f
};


static void __HBMSYNProgramChange(HBMSYNSYNTH *synth, u8 midiChannel,
                                  u8 program)
{
	ASSERTLINE(54, synth);
	ASSERTLINE(55, midiChannel < 16);
	ASSERTLINE(56, program < 128);

	synth->inst[midiChannel] = &synth->melodicInst[program];
}

void __HBMSYNSetController(HBMSYNSYNTH *synth, u8 midiChannel, u8 function,
                           u8 value)
{
	ASSERTLINE(67, synth);
	ASSERTLINE(68, midiChannel < 16);
	ASSERTLINE(69, function < 128);
	ASSERTLINE(70, value < 128);

	switch (function)
	{
	case MIDI_CC_CHANNEL_VOLUME:
		synth->volAttn[midiChannel] = __HBMSYNVolumeAttenuation[value];
		break;

	case MIDI_CC_PAN:
		synth->pan[midiChannel] = value;
		break;

	case MIDI_CC_REVERB_SEND_LEVEL:
		synth->auxAAttn[midiChannel] = __HBMSYNVolumeAttenuation[value];
		break;

	default: // the rest aren't supported
		break;
	}
}

static void __HBMSYNResetController(HBMSYNSYNTH *synth, u8 midiChannel)
{
	ASSERTLINE(102, synth);
	ASSERTLINE(103, midiChannel < 16);

	__HBMSYNSetController(synth, midiChannel, MIDI_CC_CHANNEL_VOLUME, 100);
	__HBMSYNSetController(synth, midiChannel, MIDI_CC_PAN, 64);
	__HBMSYNSetController(synth, midiChannel, MIDI_CC_EXPRESSION, 127);
	__HBMSYNSetController(synth, midiChannel, MIDI_CC_REVERB_SEND_LEVEL, 0);
	__HBMSYNSetController(synth, midiChannel, MIDI_CC_TREMOLO_DEPTH, 0);
}

void __HBMSYNResetAllControllers(HBMSYNSYNTH *synth)
{
	u8 midiChannel;

	ASSERTLINE(121, synth);

	for (midiChannel = 0; midiChannel < HBMSYN_NUM_MIDI_CHANNELS; ++midiChannel)
	{
		__HBMSYNProgramChange(synth, midiChannel, 0);
		__HBMSYNResetController(synth, midiChannel);
	}
}

static void __HBMSYNNoteOff(HBMSYNSYNTH *synth, u8 midiChannel, u8 keyNum)
{
	HBMSYNVOICE *voice;

	ASSERTLINE(138, synth);
	ASSERTLINE(139, midiChannel < 16);
	ASSERTLINE(140, keyNum < 128);

	voice = synth->voice[midiChannel][keyNum];

	if (voice)
	{
		__HBMSYNSetVoiceToRelease(voice);
		synth->voice[midiChannel][keyNum] = NULL;
	}
}

static void __HBMSYNNoteOn(HBMSYNSYNTH *synth, u8 midiChannel, u8 keyNum,
                           u8 keyVel)
{
	ASSERTLINE(162, synth);
	ASSERTLINE(163, midiChannel < 16);
	ASSERTLINE(164, keyNum < 128);
	ASSERTLINE(165, keyVel < 128);

	if (keyVel)
	{
		AXVPB *axvpb;

		if (synth->voice[midiChannel][keyNum])
		{
			__HBMSYNSetVoiceToRelease(synth->voice[midiChannel][keyNum]);
			synth->voice[midiChannel][keyNum] = NULL;
		}

		axvpb = AXAcquireVoice(AX_PRIORITY_MAX, &__HBMSYNClearVoiceReferences,
		                       (u32)synth);

		if (axvpb)
		{
			int index = HBMAllocIndex(axvpb->index);

			if (index >= 0)
			{
				HBMSYNVOICE *voice = __HBMSYNVoice + index;

				voice->axvpb = axvpb;
				voice->synth = synth;
				voice->midiChannel = midiChannel;
				voice->keyNum = keyNum;
				voice->keyVel = keyVel;

				if (__HBMSYNGetWavetableData(voice))
				{
					synth->voice[midiChannel][keyNum] = voice;
					++synth->notes;

					__HBMSYNSetupPitch(voice);
					__HBMSYNSetupVolume(voice);
					__HBMSYNSetupPan(voice);
					__HBMSYNSetupVolumeEnvelope(voice);

					HBMMIXInitChannel(axvpb, __HBMSYNGetVoiceInput(voice),
					                  synth->auxAAttn[midiChannel] >> 16,
					                  synth->pan[midiChannel],
					                  __HBMSYNGetVoiceFader(voice));

					__HBMSYNSetupSample(voice);
					__HBMSYNSetupSrc(voice);

					axvpb->pb.state = 1;
					axvpb->sync |= AX_VPB_SYNC_FLAG_STATE;
				}
				else
				{
					voice->synth = NULL;

					HBMMIXReleaseChannel(axvpb);
					HBMFreeIndex(index);
					AXFreeVoice(axvpb);
				}
			}
			else
			{
				AXFreeVoice(axvpb);
			}
		}
	}
	else
	{
		__HBMSYNNoteOff(synth, midiChannel, keyNum);
	}
}

static void __HBMSYNMidiIn(HBMSYNSYNTH *synth, u8 *input)
{
	u8 *ch;
	u8 midiFunction;
	u8 midiChannel;
	u8 _2ndByte;
	u8 _3rdByte;

	ASSERTLINE(275, synth);
	ASSERTLINE(276, input);

	ch = input;

	midiFunction = *ch >> 4;
	midiChannel = *ch & 0x0f;
	++ch;

	_2ndByte = *ch;
	++ch;

	switch (midiFunction)
	{
	case MIDI_MESSAGE_CHANNEL_VOICE_NOTE_OFF:
		__HBMSYNNoteOff(synth, midiChannel, _2ndByte);
		break;

	case MIDI_MESSAGE_CHANNEL_VOICE_NOTE_ON:
		_3rdByte = *ch;
		__HBMSYNNoteOn(synth, midiChannel, _2ndByte, _3rdByte);
		break;

	case MIDI_MESSAGE_CHANNEL_VOICE_CONTROL_CHANGE:
		_3rdByte = *ch;
		__HBMSYNSetController(synth, midiChannel, _2ndByte, _3rdByte);
		break;

	case MIDI_MESSAGE_CHANNEL_VOICE_PROGRAM_CHANGE:
		__HBMSYNProgramChange(synth, midiChannel, _2ndByte);
		break;

	default: // the rest aren't supported
		break;
	}
}

void __HBMSYNRunInputBufferEvents(HBMSYNSYNTH *synth)
{
	u8 (*input)[3];

	for (input = synth->input; synth->inputCounter; --synth->inputCounter)
	{
		__HBMSYNMidiIn(synth, *input);

		++input;
	}

	synth->inputPosition = *synth->input;
}

BOOL __HBMSYNGetWavetableData(HBMSYNVOICE *voice)
{
	HBMSYNSYNTH *synth = voice->synth;
	u32 regionIndex = synth->inst[voice->midiChannel]->keyRegion[voice->keyNum];

	// TODO: a define, maybe?
	if (regionIndex == 0xffff)
		return FALSE;

	voice->region = synth->region + regionIndex;
	voice->art = synth->art + voice->region->articulationIndex;
	voice->sample = synth->sample + voice->region->sampleIndex;
	voice->adpcm = synth->adpcm + voice->sample->adpcmIndex;

	return TRUE;
}
