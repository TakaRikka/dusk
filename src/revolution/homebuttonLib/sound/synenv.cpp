#include "synprivate.h"


#include <climits>
#include <math>

#include <revolution/types.h>

// #include <revolution/os.h>



// multiplication for negative numbers
#define UpperHalf_(x)	((x) * (1 << 16))


s32 __HBMSYNGetEnvelopeTime(s32 scale, s32 mod, u8 key)
{
	// I do not know what these big numbers are for

	if (scale == LONG_MIN)
		return 0;

	if (mod == LONG_MIN)
		return powf(2.0f, scale / (1.2e3f * 65536.0f)) * 1e3f;

	return powf(2.0f, (mod * __HBMSYNn128[key] + scale) / (1.2e3f * 65535.0f))
	     * 1e3f;
}

void __HBMSYNSetupVolumeEnvelope(HBMSYNVOICE *voice)
{
	ASSERTLINE(38, voice);

	if (voice->art->eg1Attack == LONG_MIN)
	{
		voice->veState = EnvStateDecay;
		voice->veAttn = UpperHalf_(0);

		if (voice->art->eg1Decay == LONG_MIN)
		{
			voice->veState = EnvStateSustain;
			voice->veAttn = voice->art->eg1Sustain;
		}
	}
	else
	{
		s32 frames = __HBMSYNGetEnvelopeTime(voice->art->eg1Attack,
		                                     voice->art->eg1Vel2Attack,
		                                     voice->keyVel) / 3;

		if (frames != 0)
		{
			// I do not know why these numbers are in the upper half
			voice->veAttack = UpperHalf_(0);

			// I think this line could set some of the lower half, though?
			voice->veAttackDelta = UpperHalf_(100) / frames;
			voice->veAttn = UpperHalf_(-960);
			voice->veState = EnvStateAttack;
		}
		else
		{
			voice->veAttack = UpperHalf_(0);
			voice->veAttackDelta = UpperHalf_(100);
			voice->veAttn = UpperHalf_(-960);
			voice->veState = EnvStateAttack;
		}
	}

	// attack or decay
	if (voice->veState < EnvStateSustain)
	{
		s32 frames = __HBMSYNGetEnvelopeTime(voice->art->eg1Decay,
		                                     voice->art->eg1Key2Decay,
		                                     voice->keyNum) / 3;

		if (frames != 0)
			voice->veDecay = UpperHalf_(-960) / frames;
		else
			voice->veDecay = UpperHalf_(-960);
	}

	voice->veSustain = voice->art->eg1Sustain;
	voice->veRelease = voice->art->eg1Release;
}

void __HBMSYNRunVolumeEnvelope(HBMSYNVOICE *voice)
{
	ASSERTLINE(101, voice);

	switch (voice->veState)
	{
	case EnvStateAttack:
		voice->veAttack += voice->veAttackDelta;

		if (voice->veAttack >= UpperHalf_(99))
			voice->veAttn = UpperHalf_(0);
		else
			voice->veAttn = __HBMSYNAttackAttnTable[voice->veAttack >> 16];

		if (voice->veAttn == UpperHalf_(0))
			voice->veState = EnvStateDecay;

		break;

	case EnvStateDecay:
		voice->veAttn += voice->veDecay;

		if (voice->veAttn <= voice->veSustain)
		{
			voice->veAttn = voice->veSustain;
			voice->veState = EnvStateSustain;
		}

		if (voice->veAttn <= UpperHalf_(-720))
		{
			voice->veState = EnvStateEnd;
			voice->synth->voice[voice->midiChannel][voice->keyNum] = NULL;
		}

		break;

	case EnvStateSustain:
		/* TODO: is this handled elsewhere? Or is it just nothing because it's
		 * a sustain state
		 */
		break;

	case EnvStateRelease:
		if (voice->veAttn <= UpperHalf_(-720))
			voice->veState = EnvStateEnd;
		else
			voice->veAttn += voice->veRelease;

		break;
	}
}
