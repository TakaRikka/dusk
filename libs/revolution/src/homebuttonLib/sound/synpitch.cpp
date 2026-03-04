#include "synprivate.h"


#include <revolution/types.h>

// #include <revolution/ax.h>



static f32 __HBMSYNCentsTable[100] =
{
	1.0f,      1.000578f, 1.001156f, 1.001734f,
	1.002313f, 1.002892f, 1.003472f, 1.004052f,
	1.004632f, 1.005212f, 1.005793f, 1.006374f,
	1.006956f, 1.007537f, 1.00812f,  1.008702f,
	1.009285f, 1.009868f, 1.010451f, 1.011035f,
	1.011619f, 1.012204f, 1.012789f, 1.013374f,
	1.013959f, 1.014545f, 1.015132f, 1.015718f,
	1.016305f, 1.016892f, 1.01748f,  1.018068f,
	1.018656f, 1.019244f, 1.019833f, 1.020423f,
	1.021012f, 1.021602f, 1.022192f, 1.022783f,
	1.023374f, 1.023965f, 1.024557f, 1.025149f,
	1.025741f, 1.026334f, 1.026927f, 1.02752f,
	1.028114f, 1.028708f, 1.029302f, 1.029897f,
	1.030492f, 1.031087f, 1.031683f, 1.032279f,
	1.032876f, 1.033472f, 1.03407f,  1.034667f,
	1.035265f, 1.035863f, 1.036462f, 1.03706f,
	1.03766f,  1.038259f, 1.038859f, 1.039459f,
	1.04006f,  1.040661f, 1.041262f, 1.041864f,
	1.042466f, 1.043068f, 1.043671f, 1.044274f,
	1.044877f, 1.045481f, 1.046085f, 1.046689f,
	1.047294f, 1.047899f, 1.048505f, 1.049111f,
	1.049717f, 1.050323f, 1.05093f,  1.051537f,
	1.052145f, 1.052753f, 1.053361f, 1.05397f,
	1.054579f, 1.055188f, 1.055798f, 1.056408f,
	1.057018f, 1.057629f, 1.05824f,  1.058851f
};

static f32 __HBMSYNOctavesTableUp[12] =
{
	1.0f,
	2.0f,
	4.0f,
	8.0f,
	16.0f,
	32.0f,
	64.0f,
	128.0f,
	256.0f,
	512.0f,
	1024.0f,
	2048.0f
};

static f32 __HBMSYNSemitonesTableUp[12] =
{
	1.0f,
	1.059463f,
	1.122462f,
	1.189207f,
	1.259921f,
	1.33484f,
	1.414214f,
	1.498307f,
	1.587401f,
	1.681793f,
	1.781797f,
	1.887749f
};

static f32 __HBMSYNSemitonesTableDown[128] =
{
	1.0f,      0.943874f, 0.890899f, 0.840896f,
	0.793701f, 0.749154f, 0.707107f, 0.66742f,
	0.629961f, 0.594604f, 0.561231f, 0.529732f,

	0.5f,      0.471937f, 0.445449f, 0.420448f,
	0.39685f,  0.374577f, 0.353553f, 0.33371f,
	0.31498f,  0.297302f, 0.280616f, 0.264866f,

	0.25f,     0.235969f, 0.222725f, 0.210224f,
	0.198425f, 0.187288f, 0.176777f, 0.166855f,
	0.15749f,  0.148651f, 0.140308f, 0.132433f,

	0.125f,    0.117984f, 0.111362f, 0.105112f,
	0.099213f, 0.093644f, 0.088388f, 0.083427f,
	0.078745f, 0.074325f, 0.070154f, 0.066216f,

	0.0625f,   0.058992f, 0.055681f, 0.052556f,
	0.049606f, 0.046822f, 0.044194f, 0.041714f,
	0.039373f, 0.037163f, 0.035077f, 0.033108f,

	0.03125f,  0.029496f, 0.027841f, 0.026278f,
	0.024803f, 0.023411f, 0.022097f, 0.020857f,
	0.019686f, 0.018581f, 0.017538f, 0.016554f,

	0.015625f, 0.014748f, 0.01392f,  0.013139f,
	0.012402f, 0.011706f, 0.011049f, 0.010428f,
	0.009843f, 0.009291f, 0.008769f, 0.008277f,

	0.007813f, 0.007374f, 0.00696f,  0.00657f,
	0.006201f, 0.005853f, 0.005524f, 0.005214f,
	0.004922f, 0.004645f, 0.004385f, 0.004139f,

	0.003906f, 0.003687f, 0.00348f,  0.003285f,
	0.0031f,   0.002926f, 0.002762f, 0.002607f,
	0.002461f, 0.002323f, 0.002192f, 0.002069f,

	0.001953f, 0.001844f, 0.00174f,  0.001642f,
	0.00155f,  0.001463f, 0.001381f, 0.001304f,
	0.00123f,  0.001161f, 0.001096f, 0.001035f,

	9.77e-4f,  9.22e-4f,  8.7e-4f,   8.21e-4f,
	7.75e-4f,  7.32e-4f,  6.91e-4f,  6.52e-4f
};


f32 __HBMSYNGetRelativePitch(HBMSYNVOICE *voice)
{
	s32 cents = voice->cents;
	cents /= 1 << 16;

	if (cents > 0)
	{
		s32 octaves = cents / 1200;
		s32 semitones = cents % 1200 / 100;

		cents %= 100;

		return __HBMSYNOctavesTableUp[octaves]
		     * __HBMSYNSemitonesTableUp[semitones] * __HBMSYNCentsTable[cents];
	}
	else if (cents < 0)
	{
		s32 semitones = cents / 100;

		cents %= 100;
		if (cents != 0)
		{
			cents += 100;
			semitones -= 1;
		}

		semitones *= -1; // since semitones is negative

		return __HBMSYNSemitonesTableDown[semitones]
		     * __HBMSYNCentsTable[cents];
	}
	else
	{
		return 1.0f;
	}
}

void __HBMSYNSetupPitch(HBMSYNVOICE *voice)
{
	voice->srcRatio = voice->sample->sampleRate / 3.2e4f;

	voice->cents = (voice->keyNum - voice->region->unityNote) * 100;
	voice->cents += voice->region->fineTune;
	voice->cents <<= 16;
}

void __HBMSYNSetupSrc(HBMSYNVOICE *voice)
{
	f32 srcRatio = voice->srcRatio * __HBMSYNGetRelativePitch(voice);
	u32 value = srcRatio * 65536.0f;
	u16 *p;

	voice->axvpb->pb.srcSelect = 1;

	// cool
	p = (u16 *)&voice->axvpb->pb.src;

	/* src.ratioHi             */ *p++ = value >> 16;
	/* src.ratioLo             */ *p++ = value;
	/* src.currentAdddressFrac */ *p++ = 0;
	/* src.last_samples[0]     */ *p++ = 0;
	/* src.last_samples[1]     */ *p++ = 0;
	/* src.last_samples[2]     */ *p++ = 0;
	/* src.last_samples[3]     */ *p   = 0;

	voice->axvpb->sync &= ~AX_VPB_SYNC_FLAG_SRC_RATIO;
	voice->axvpb->sync |= AX_VPB_SYNC_FLAG_SRC_TYPE | AX_VPB_SYNC_FLAG_SRC;
}

void __HBMSYNUpdateSrc(HBMSYNVOICE *voice)
{
	u32 ratio = voice->srcRatio * __HBMSYNGetRelativePitch(voice) * 65536.0f;

	*(u32 *)&voice->axvpb->pb.src.ratioHi = ratio;
	voice->axvpb->sync |= AX_VPB_SYNC_FLAG_SRC_RATIO;
}
