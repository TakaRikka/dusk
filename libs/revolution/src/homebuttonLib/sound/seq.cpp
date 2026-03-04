#include "seq.h"


#include <revolution/types.h>

#include "syn.h"

// #include <revolution/os.h>
// #include <revolution/os/OSInterrupt.h>



// As specified by General MIDI 1

#define MIDI_TRACK_HEADER_MAGIC					'MThd'
#define MIDI_TRACK_MAGIC						'MTrk'

#define MIDI_FILE_FORMAT_0						0
#define MIDI_FILE_FORMAT_1						1
// #define MIDI_FILE_FORMAT_2					2

#define MIDI_MESSAGE_CHANNEL_MODE_MESSAGE		0xB0
#define MIDI_MESSAGE_SYSEX_BEGIN				0xF0
#define MIDI_MESSAGE_SYSEX_END					0xF7
#define MIDI_MESSAGE_META_EVENT					0xFF

#define MIDI_CHANNEL_MODE_MESSAGE_ALL_NOTES_OFF	0x7B

#define MIDI_META_EVENT_END_OF_TRACK			0x2F
#define MIDI_META_EVENT_SET_TEMPO				0x51


enum HBMSEQTRACKSTATE_et
{
	HBMSEQ_TRACK_STATE_0, // stopped?
	HBMSEQ_TRACK_STATE_1, // playing?
	HBMSEQ_TRACK_STATE_2, // never set to; only checked for
};


static void __HBMSEQPushSequenceList(HBMSEQSEQUENCE *sequence);
static void __HBMSEQRemoveSequenceFromList(HBMSEQSEQUENCE *sequence);
static u32 __HBMSEQGetIntTrack(HBMSEQTRACK *track);
static void __HBMSEQHandleSysExEvent(HBMSEQTRACK *track);
static void __HBMSEQSetTicksPerFrame(HBMSEQTRACK *track, f32 bps);
static void __HBMSEQTempoMetaEvent(HBMSEQTRACK *track);
static void __HBMSEQTrackEnd(HBMSEQTRACK *track);
static void __HBMSEQHandleMetaEvent(HBMSEQTRACK *track);
static void __HBMSEQHandleSynthEvent(HBMSYNSYNTH *synth, HBMSEQTRACK *track);
static void __HBMSEQRunEvent(HBMSYNSYNTH *synth, HBMSEQTRACK *track);
static void __HBMSEQInitTracks(HBMSEQSEQUENCE *sequence, u8 *read,
                               int tracks);
static void __HBMSEQReadHeader(HBMSEQSEQUENCE *sequence, u8 *midiStream);


// .data
static u8 __HBMSEQMidiEventLength[128] =
{
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

	0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// .bss
static HBMSEQSEQUENCE *__HBMSEQSequenceList;
static BOOL __init;


static void __HBMSEQPushSequenceList(HBMSEQSEQUENCE *sequence)
{
	BOOL intrStatus = OSDisableInterrupts(); /* int old; */

	if (__HBMSEQSequenceList)
		sequence->next = __HBMSEQSequenceList;
	else
		sequence->next = NULL;

	__HBMSEQSequenceList = sequence;

	OSRestoreInterrupts(intrStatus);
}

static void __HBMSEQRemoveSequenceFromList(HBMSEQSEQUENCE *sequence)
{
	BOOL intrStatus = OSDisableInterrupts(); /* int old; */
	HBMSEQSEQUENCE *thisSequence = __HBMSEQSequenceList;

	__HBMSEQSequenceList = NULL;

	while (thisSequence)
	{
		HBMSEQSEQUENCE *next = thisSequence->next;

		if (thisSequence != sequence)
			__HBMSEQPushSequenceList(thisSequence);

		thisSequence = next;
	}

	OSRestoreInterrupts(intrStatus);
}

static u32 __HBMSEQGetIntTrack(HBMSEQTRACK *track)
{
	u32 value;

	ASSERTLINE(106, track);

	value = *track->current & 0x7f;

	while (*track->current & 0x80)
	{
		++track->current;

		value = (value << 7) + (*track->current & 0x7f);
	}

	++track->current;

	return value;
}

static void __HBMSEQHandleSysExEvent(HBMSEQTRACK *track)
{
	u32 length;

	ASSERTLINE(129, track);

	length = __HBMSEQGetIntTrack(track);
	track->current += length;
}

static void __HBMSEQSetTicksPerFrame(HBMSEQTRACK *track, f32 bps)
{
	HBMSEQSEQUENCE *sequence;

	ASSERTLINE(143, track);

	sequence = track->sequence;

	track->beatsPerSec = bps;
	track->ticksPerFrame =
		96.0f / (3.2e4f / bps / sequence->timeFormat) * 65536.0f;
}

static void __HBMSEQTempoMetaEvent(HBMSEQTRACK *track)
{
	u32 data;
	f32 beatsPerSec;

	// doubled statements
	data = *track->current; ++track->current;
	data = (data << 8) + *track->current; ++track->current;
	data = (data << 8) + *track->current; ++track->current;

	beatsPerSec = 1e6f / data;

	__HBMSEQSetTicksPerFrame(track, beatsPerSec);
}

static void __HBMSEQTrackEnd(HBMSEQTRACK *track)
{
	HBMSEQSEQUENCE *sequence;

	ASSERTLINE(185, track);

	sequence = track->sequence;

	--sequence->tracksRunning;
	track->state = HBMSEQ_TRACK_STATE_0;

	if (sequence->tracksRunning == 0)
		sequence->end = TRUE;
}

static void __HBMSEQHandleMetaEvent(HBMSEQTRACK *track)
{
	u8 type;
	u32 length;

	ASSERTLINE(204, track);

	// doubled statements
	type = *track->current; ++track->current;
	switch (type)
	{
	case MIDI_META_EVENT_END_OF_TRACK:
		__HBMSEQTrackEnd(track);
		break;

	case MIDI_META_EVENT_SET_TEMPO:
		length = __HBMSEQGetIntTrack(track); // assignment unnecessary?
		__HBMSEQTempoMetaEvent(track);
		break;

	default:
		length = __HBMSEQGetIntTrack(track);
		track->current += length;
		break;
	}
}

static void __HBMSEQHandleSynthEvent(HBMSYNSYNTH *synth, HBMSEQTRACK *track)
{
	u8 ch[3];
	u32 bytes;

	bytes = __HBMSEQMidiEventLength[track->status - 128];
	ch[0] = track->status;

	switch (bytes)
	{
	case 0:
		break;

	case 1:
		// doubled statements
		ch[1] = *track->current; ++track->current;
		break;

	case 2:
		// doubled statements
		ch[1] = *track->current; ++track->current;
		ch[2] = *track->current; ++track->current;
		break;
	}

	HBMSYNMidiInput(synth, ch);
}

static void __HBMSEQRunEvent(HBMSYNSYNTH *synth, HBMSEQTRACK *track)
{
	u8 event;

	ASSERTLINE(280, synth);
	ASSERTLINE(281, track);

	event = *track->current;

	if (event >= 0x80)
	{
		track->status = event;
		++track->current;
	}

	switch (track->status)
	{
	case MIDI_MESSAGE_SYSEX_BEGIN:
	case MIDI_MESSAGE_SYSEX_END:
		__HBMSEQHandleSysExEvent(track);
		break;

	case MIDI_MESSAGE_META_EVENT:
		__HBMSEQHandleMetaEvent(track);
		break;

	default:
		__HBMSEQHandleSynthEvent(synth, track);
		break;
	}

	if (track->current >= track->end)
		__HBMSEQTrackEnd(track);
}

static void __HBMSEQInitTracks(HBMSEQSEQUENCE *sequence, u8 *read,
                               int tracks)
{
	int i = 0;
	u8 *p = read;

	while (tracks)
	{
		while (TRUE)
		{
			u32 chunk;
			u32 bytes;

			// doubled statements
			chunk = *(u32 *)p; p += sizeof(u32);
			bytes = *(u32 *)p; p += sizeof(u32);

			if (chunk == MIDI_TRACK_MAGIC)
			{
				HBMSEQTRACK *track = sequence->track + i;

				track->sequence = sequence;
				track->start = p;
				track->end = p + bytes;
				track->current = p;
				track->defaultTicksPerFrame =
					96.0f / (1.6e4f / sequence->timeFormat) * 65536.0f;
				track->state = HBMSEQ_TRACK_STATE_0;

				p += bytes;
				break;
			}

			p += bytes;
		}

		--tracks;
		++i;
	}
}

static void __HBMSEQReadHeader(HBMSEQSEQUENCE *sequence, u8 *midiStream)
{
	u8 *read = midiStream;
	u32 bytes;
	u32 fileType;

	OSAssertMessage_Line(383, *(u32 *)read == MIDI_TRACK_HEADER_MAGIC,
	                     "!!!midiStream is not a valid MIDI file\n!!!");
	read += sizeof(u32);

	// doubled statements
	bytes = *(u32 *)read; read += sizeof(u32);
	fileType = *(u16 *)read; read += sizeof(u16);
	sequence->nTracks = *(u16 *)read; read += sizeof(u16);
	sequence->timeFormat = *(s16 *)read; read += sizeof(s16);

	OSAssertMessage_Line(398, sequence->timeFormat >= 0,
	                     "!!!HBMSEQ does not support SMPTE time!!!\n");

	// fileType, sequence->nTracks, and sequence->timeFormat, respectively
	bytes -= sizeof(u16) + sizeof(u16) + sizeof(s16);
	read += bytes;

	switch (fileType)
	{
	case MIDI_FILE_FORMAT_0:
		sequence->nTracks = 1;
		__HBMSEQInitTracks(sequence, read, 1);
		break;

	case MIDI_FILE_FORMAT_1:
		OSAssertMessage_Line(420, sequence->nTracks < HBMSEQ_MAX_TRACKS,
		                     "exceeded HBMSEQ_MAX_TRACKS, please increase "
		                     "HBMSEQ_MAX_TRACKS\n");

		__HBMSEQInitTracks(sequence, read, sequence->nTracks);
		break;

	// case MIDI_FILE_FORMAT_2: // not supported
	default:
		OSAssertMessage_Line(428, FALSE, "!!!Invalid MIDI file type\n!!!");
	}

	sequence->tracksRunning = sequence->nTracks;
}

void HBMSEQInit(void)
{
	if (!__init)
	{
		__HBMSEQSequenceList = NULL;
		__init = TRUE;
	}
}

void HBMSEQQuit(void)
{
	__HBMSEQSequenceList = NULL;
	__init = FALSE;
}

void HBMSEQRunAudioFrame(void)
{
	HBMSEQSEQUENCE *sequence = __HBMSEQSequenceList;

	if (!__init)
		return;

	for (; sequence; sequence = sequence->next)
	{
		if (sequence->state == HBM_SEQ_STATE_1
		    || sequence->state == HBM_SEQ_STATE_2)
		{
			u32 i;

			for (i = 0; i < sequence->nTracks; ++i)
			{
				HBMSEQTRACK *track = &sequence->track[i];

				if (track->state == HBMSEQ_TRACK_STATE_1
				    || track->state == HBMSEQ_TRACK_STATE_2)
				{
					u32 ticks = track->ticksPerFrame;

					if (track->delay > ticks)
					{
						track->delay -= ticks;
						continue;
					}

					while (ticks >= track->delay)
					{
						ticks -= track->delay;

						__HBMSEQRunEvent(&sequence->synth, track);

						if (track->state == HBMSEQ_TRACK_STATE_0)
							break;

						track->delay = __HBMSEQGetIntTrack(track) << 16;
					}

					track->delay -= ticks;
				}
			}
		}

		if (sequence->end)
		{
			if (sequence->state == HBM_SEQ_STATE_2)
			{
				HBMSEQSetState(sequence, HBM_SEQ_STATE_0);
				HBMSEQSetState(sequence, HBM_SEQ_STATE_2);
			}
			else
			{
				HBMSEQSetState(sequence, HBM_SEQ_STATE_0);
			}
		}
	}
}

void HBMSEQAddSequence(HBMSEQSEQUENCE *sequence, u8 *midiStream,
                       u8 *wavetable, u8 *samples, u8 *zerobuffer)
{
	ASSERTLINE(551, sequence);
	ASSERTLINE(552, midiStream);
	ASSERTLINE(553, wavetable);
	ASSERTLINE(554, samples);

	HBMSYNInitSynth(&sequence->synth, wavetable, samples, zerobuffer);
	sequence->state = HBM_SEQ_STATE_0;
	__HBMSEQReadHeader(sequence, midiStream);
	__HBMSEQPushSequenceList(sequence);
}

void HBMSEQRemoveSequence(HBMSEQSEQUENCE *sequence)
{
	ASSERTLINE(581, sequence);

	__HBMSEQRemoveSequenceFromList(sequence);
	HBMSYNQuitSynth(&sequence->synth);
}

void HBMSEQSetState(HBMSEQSEQUENCE *sequence, HBMSEQSTATE state)
{
	int i;

	ASSERTLINE(598, sequence);

	switch (state)
	{
	case HBM_SEQ_STATE_1:
	case HBM_SEQ_STATE_2:
		if (sequence->state == HBM_SEQ_STATE_0)
		{
			BOOL intrStatus = OSDisableInterrupts(); /* int old; */

			for (i = 0; i < sequence->nTracks; ++i)
			{
				HBMSEQTRACK *track = &sequence->track[i];

				track->current = track->start;
				track->ticksPerFrame = track->defaultTicksPerFrame;
				track->delay = __HBMSEQGetIntTrack(track) << 16;
				track->state = HBMSEQ_TRACK_STATE_1;
			}

			sequence->tracksRunning = sequence->nTracks;

			OSRestoreInterrupts(intrStatus);
		}

		sequence->end = FALSE;
		break;

	case HBM_SEQ_STATE_0:
	case HBM_SEQ_STATE_3:
		for (i = 0; i < HBMSYN_NUM_MIDI_CHANNELS; ++i)
		{
			BOOL intrStatus = OSDisableInterrupts(); /* int old; */

			u8 ch[3];
			ch[0] = MIDI_MESSAGE_CHANNEL_MODE_MESSAGE | i;
			ch[1] = MIDI_CHANNEL_MODE_MESSAGE_ALL_NOTES_OFF;
			ch[2] = 0; // v = 0 -> all notes off

			HBMSYNMidiInput(&sequence->synth, ch);

			OSRestoreInterrupts(intrStatus);
		}

		break;
	}

	sequence->state = state;
}

HBMSEQSTATE HBMSEQGetState(HBMSEQSEQUENCE *sequence)
{
	ASSERTLINE(666, sequence);

	return sequence->state;
}

void HBMSEQSetVolume(HBMSEQSEQUENCE *sequence, s32 dB)
{
	ASSERTLINE(677, sequence);

	HBMSYNSetMasterVolume(&sequence->synth, dB);
}

s32 HBMSEQGetVolume(HBMSEQSEQUENCE *sequence)
{
	ASSERTLINE(688, sequence);

	return HBMSYNGetMasterVolume(&sequence->synth);
}
