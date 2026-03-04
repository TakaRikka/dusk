#include "synprivate.h"


#include <revolution/types.h>

// #include <revolution/ax.h>
// #include <revolution/os.h>



// clang-format off

/* Compromise for enclosing these macros in a clang-format off block: I changed
 * all of the indentation to spaces, so that everybody can see it the same way
 * instead of having to switch to tabs:4 to enjoy it.
 */

#define SET_AXPBADDR(p_, flag_, fmt_, loop_, end_, cur_, gain_, pred_)             \
    do                                                                              \
    {                                                                               \
        /* contains doubled and tripled expressions */                              \
                                                                                    \
        /* addr.{loopFlag,format}     */ *(p_) = (flag_) << 16 | (fmt_); ++(p_);    \
        /* addr.loopAddress{Hi,Lo}    */ *(p_) = (loop_);                ++(p_);    \
        /* addr.endAddress{Hi,Lo}     */ *(p_) = (end_);                 ++(p_);    \
        /* addr.currentAddress{Hi,Lo} */ *(p_) = (cur_);                 ++(p_);    \
                                                                                    \
        /* adpcm.a[0][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[1][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[2][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[3][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[4][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[5][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[6][{0,1}]          */ *(p_) = 0; ++(p_);                         \
        /* adpcm.a[7][{0,1}]          */ *(p_) = 0; ++(p_);                         \
                                                                                    \
        /* adpcm.{gain,pred_scale}    */ *(p_) = (gain_) << 16 | (pred_); ++(p_);   \
        /* adpcm.yn{1,2}              */ *(p_) = 0;                                 \
    } while (0)

#define SET_AXPBADDR_ADPCM(p_, flag_, fmt_, loop_, end_, cur_, adpcm_)             \
    do                                                                              \
    {                                                                               \
        /* contains doubled and tripled expressions */                              \
                                                                                    \
        /* addr.{loopFlag,format}     */ *(p_) = (flag_) << 16 | (fmt_); ++(p_);    \
        /* addr.loopAddress{Hi,Lo}    */ *(p_) = (loop_);                ++(p_);    \
        /* addr.endAddress{Hi,Lo}     */ *(p_) = (end_);                 ++(p_);    \
        /* addr.currentAddress{Hi,Lo} */ *(p_) = (cur_);                 ++(p_);    \
                                                                                    \
        /* adpcm.a[0][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[1][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[2][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[3][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[4][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[5][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[6][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.a[7][{0,1}]          */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
                                                                                    \
        /* adpcm.{gain,pred_scale}    */ *(p_) = *(adpcm_); ++(p_); ++(adpcm_);     \
        /* adpcm.yn{1,2}              */ *(p_) = *(adpcm_);                         \
    } while (0)

// clang-format on


static u32 __HBMSYNGetNibbleAddress(u32 count);
static void __HBMSYNSetupAdpcm(HBMSYNVOICE *voice);
static void __HBMSYNSetupPcm16(HBMSYNVOICE *voice);
static void __HBMSYNSetupPcm8(HBMSYNVOICE *voice);


static u32 __HBMSYNGetNibbleAddress(u32 count)
{
	u32 const samples = count; // same thing as __HBMMIXGetVolume?
	u32 frames = samples / 14;
	u32 samplesLeft = samples % 14;

	return 2 + frames * 16 + samplesLeft;
}

static void __HBMSYNSetupAdpcm(HBMSYNVOICE *voice)
{
	AXVPB *axvpb = voice->axvpb;

	if (voice->region->loopStart + voice->region->loopLength != 0)
	{
		u32 sampleStart;
		u32 sampleLoop;
		u32 sampleEnd;
		u32 *p;
		u32 *adpcm;
		u16 *adpcmloop;

		adpcm = (u32 *)voice->adpcm;
		voice->type = VoiceTypeLooping;

		sampleStart = voice->synth->samplesBaseNibble + voice->sample->offset;
		sampleLoop =
			sampleStart + __HBMSYNGetNibbleAddress(voice->region->loopStart);
		sampleEnd = sampleStart
		          + __HBMSYNGetNibbleAddress(voice->region->loopStart
		                                     + voice->region->loopLength - 1);

		ASSERTLINE(64, (sampleStart & 0x000f) == 0);
		ASSERTLINE(65, (sampleLoop & 0x000f) > 1);
		ASSERTLINE(66, (sampleEnd & 0x000f) > 1);

		sampleStart += sizeof(u16);
		p = (u32 *)&axvpb->pb.addr;

		SET_AXPBADDR_ADPCM(p, TRUE, 0, sampleLoop, sampleEnd, sampleStart,
		                   adpcm);

		adpcmloop = (u16 *)++adpcm;

		// doubled expressions
		axvpb->pb.adpcmLoop.loop_pred_scale	= *adpcmloop; ++adpcmloop;
		axvpb->pb.adpcmLoop.loop_yn1		= *adpcmloop; ++adpcmloop;
		axvpb->pb.adpcmLoop.loop_yn2		= *adpcmloop;

		axvpb->sync &=
			~(AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG | AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR
		      | AX_VPB_SYNC_FLAG_ADDR_END_ADDR
		      | AX_VPB_SYNC_FLAG_ADDR_CURRENT_ADDR);

		axvpb->sync |= AX_VPB_SYNC_FLAG_ADDR | AX_VPB_SYNC_FLAG_ADPCM
		             | AX_VPB_SYNC_FLAG_ADPCM_LOOP;
	}
	else
	{
		u32 sampleStart;
		u32 sampleLoop;
		u32 sampleEnd;
		u32 *p;
		u32 *adpcm;

		adpcm = (u32 *)voice->adpcm;
		voice->type = VoiceTypeNormal;

		sampleStart = voice->synth->samplesBaseNibble + voice->sample->offset;
		sampleLoop = voice->synth->samplesBaseNibble + voice->sample->offset;
		sampleEnd =
			sampleStart + __HBMSYNGetNibbleAddress(voice->sample->length - 1);

		ASSERTLINE(120, (sampleStart & 0x000f) == 0);
		ASSERTLINE(121, (sampleEnd & 0x000f) > 1);

		sampleStart += sizeof(u16);
		p = (u32 *)&axvpb->pb.addr;

		SET_AXPBADDR_ADPCM(p, FALSE, 0, sampleLoop, sampleEnd, sampleStart,
		                   adpcm);

		axvpb->sync &=
			~(AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG | AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR
		      | AX_VPB_SYNC_FLAG_ADDR_END_ADDR
		      | AX_VPB_SYNC_FLAG_ADDR_CURRENT_ADDR);

		axvpb->sync |= AX_VPB_SYNC_FLAG_ADDR | AX_VPB_SYNC_FLAG_ADPCM;
	}
}

static void __HBMSYNSetupPcm16(HBMSYNVOICE *voice)
{
	AXVPB *axvpb = voice->axvpb;

	if (voice->region->loopStart + voice->region->loopLength != 0)
	{
		u32 sampleStart;
		u32 sampleLoop;
		u32 sampleEnd;
		u32 *p;

		voice->type = VoiceTypeLooping;

		sampleStart = voice->synth->samplesBaseWord + voice->sample->offset;
		sampleLoop = sampleStart + voice->region->loopStart;
		sampleEnd = sampleLoop + voice->region->loopLength - 1;
		p = (u32 *)&axvpb->pb.addr;

		SET_AXPBADDR(p, TRUE, 10, sampleLoop, sampleEnd, sampleStart, 0x0800,
		             0);
	}
	else
	{
		u32 sampleStart;
		u32 sampleLoop;
		u32 sampleEnd;
		u32 *p;

		voice->type = VoiceTypeNormal;

		sampleStart = voice->synth->samplesBaseWord + voice->sample->offset;
		sampleLoop = voice->synth->samplesBaseWord + voice->sample->offset;
		sampleEnd = sampleStart + voice->sample->length - 1;
		p = (u32 *)&axvpb->pb.addr;

		SET_AXPBADDR(p, FALSE, 10, sampleLoop, sampleEnd, sampleStart, 0x0800,
		             0);
	}

	axvpb->sync &= ~(
		AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG | AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR
		| AX_VPB_SYNC_FLAG_ADDR_END_ADDR | AX_VPB_SYNC_FLAG_ADDR_CURRENT_ADDR);

	axvpb->sync |= AX_VPB_SYNC_FLAG_ADDR | AX_VPB_SYNC_FLAG_ADPCM;
}

static void __HBMSYNSetupPcm8(HBMSYNVOICE *voice)
{
	AXVPB *axvpb = voice->axvpb;

	if (voice->region->loopStart + voice->region->loopLength != 0)
	{
		u32 sampleStart;
		u32 sampleLoop;
		u32 sampleEnd;
		u32 *p;

		voice->type = VoiceTypeLooping;

		sampleStart = voice->synth->samplesBaseByte + voice->sample->offset;
		sampleLoop = sampleStart + voice->region->loopStart;
		sampleEnd = sampleLoop + voice->region->loopLength - 1;
		p = (u32 *)&axvpb->pb.addr;

		SET_AXPBADDR(p, TRUE, 25, sampleLoop, sampleEnd, sampleStart, 0x0100,
		             0);
	}
	else
	{
		u32 sampleStart;
		u32 sampleLoop;
		u32 sampleEnd;
		u32 *p;

		voice->type = VoiceTypeNormal;

		sampleStart = voice->synth->samplesBaseByte + voice->sample->offset;
		sampleLoop = voice->synth->samplesBaseByte + voice->sample->offset;
		sampleEnd = sampleStart + voice->sample->length - 1;
		p = (u32 *)&axvpb->pb.addr;

		SET_AXPBADDR(p, FALSE, 25, sampleLoop, sampleEnd, sampleStart, 0x0100,
		             0);
	}

	axvpb->sync &= ~(
		AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG | AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR
		| AX_VPB_SYNC_FLAG_ADDR_END_ADDR | AX_VPB_SYNC_FLAG_ADDR_CURRENT_ADDR);

	axvpb->sync |= AX_VPB_SYNC_FLAG_ADDR | AX_VPB_SYNC_FLAG_ADPCM;
}

void __HBMSYNSetupSample(HBMSYNVOICE *voice)
{
	ASSERTLINE(346, voice);

	switch (voice->sample->format)
	{
	case 0:
		__HBMSYNSetupAdpcm(voice);
		break;

	case 1:
		__HBMSYNSetupPcm16(voice);
		break;

	case 2:
		__HBMSYNSetupPcm8(voice);
		break;

	default:
		OSAssertMessage_Line(370, FALSE, "unknown sample format\n");
		break;
	}
}
