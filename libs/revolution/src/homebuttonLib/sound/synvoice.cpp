#include "synprivate.h"


#include <revolution/types.h>

#include "mix.h"

// #include <revolution/ax.h>
// #include <revolution/os.h>



void __HBMSYNClearVoiceReferences(void *p)
{
	AXVPB *axvpb;
	HBMSYNSYNTH *synth;
	HBMSYNVOICE *voice;
	int index;

	ASSERTLINE(27, p);

	axvpb = (AXVPB *)p;
	synth = (HBMSYNSYNTH *)axvpb->userContext;

	index = HBMGetIndex(axvpb->index);
	HBMFreeIndex(index);

	voice = __HBMSYNVoice + index;
	HBMMIXReleaseChannel(axvpb);

	if (synth->voice[voice->midiChannel][voice->keyNum] == voice)
		synth->voice[voice->midiChannel][voice->keyNum] = NULL;

	voice->synth = NULL;
	--synth->notes;
}

void __HBMSYNSetVoiceToRelease(HBMSYNVOICE *voice)
{
	ASSERTLINE(53, voice);

	voice->veState = EnvStateRelease;
}

void __HBMSYNServiceVoice(int i)
{
	HBMSYNVOICE *voice = __HBMSYNVoice + i;
	HBMSYNSYNTH *synth = voice->synth;

	if (!synth)
		return;

	if (voice->type == VoiceTypeNormal && voice->axvpb->pb.state == 0)
	{
		if (synth->voice[voice->midiChannel][voice->keyNum] == voice)
			synth->voice[voice->midiChannel][voice->keyNum] = NULL;

		voice->veState = EnvStateEnd;
	}

	__HBMSYNRunVolumeEnvelope(voice);

	if (voice->veState == EnvStateEnd)
	{
		voice->synth = NULL;

		HBMMIXReleaseChannel(voice->axvpb);
		HBMFreeIndexByKey(voice->axvpb->index);
		AXFreeVoice(voice->axvpb);

		--synth->notes;
	}
	else
	{
		__HBMSYNUpdateMix(voice);
		__HBMSYNUpdateSrc(voice);
	}
}
