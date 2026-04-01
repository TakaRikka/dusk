#ifndef D_A_MOVIE_PLAYER_H
#define D_A_MOVIE_PLAYER_H

#if !TARGET_PC
#include <thp.h>
#endif
#include "f_op/f_op_actor.h"
#include "d/d_drawlist.h"

struct daMP_THPReadBuffer {
    u8* ptr;
	s32 frameNumber;
	BOOL isValid;
};

#if TARGET_PC
// Copying here because thp.h is probably erroneous in the dolphin lib,
// and it's kind of a problem being there (Aurora owns the headers).
// TODO: Move this stuff in decomp?
typedef struct THPAudioRecordHeader {
    BE(u32) offsetNextChannel;
    BE(u32) sampleSize;
    BE(s16) lCoef[8][2];
    BE(s16) rCoef[8][2];
    BE(s16) lYn1;
    BE(s16) lYn2;
    BE(s16) rYn1;
    BE(s16) rYn2;
} THPAudioRecordHeader;

typedef struct THPAudioDecodeInfo {
    u8* encodeData;
    u32 offsetNibbles;
    u8 predictor;
    u8 scale;
    s16 yn1;
    s16 yn2;
} THPAudioDecodeInfo;

typedef struct THPTextureSet {
	u8* ytexture;
	u8* utexture;
	u8* vtexture;
	s32 frameNumber;
} THPTextureSet;

typedef struct THPAudioBuffer {
	s16* buffer;
	s16* curPtr;
	u32 validSample;
} THPAudioBuffer;

typedef struct THPVideoInfo {
	BE(u32) xSize;
	BE(u32) ySize;
	BE(u32) videoType;
} THPVideoInfo;

typedef struct THPAudioInfo {
	BE(u32) sndChannels;
	BE(u32) sndFrequency;
	BE(u32) sndNumSamples;
	BE(u32) sndNumTracks;
} THPAudioInfo;

typedef struct THPFrameCompInfo {
	BE(u32) numComponents;
	u8 frameComp[16];
} THPFrameCompInfo;

typedef struct THPHeader {
	/* 0x00 */ char magic[4];
	/* 0x04 */ BE(u32) version;
	/* 0x08 */ BE(u32) bufsize;
	/* 0x0C */ BE(u32) audioMaxSamples;
	/* 0x10 */ BE(f32) frameRate;
	/* 0x14 */ BE(u32) numFrames;
	/* 0x18 */ BE(u32) firstFrameSize;
	/* 0x1C */ BE(u32) movieDataSize;
	/* 0x20 */ BE(u32) compInfoDataOffsets;
	/* 0x24 */ BE(u32) offsetDataOffsets;
	/* 0x28 */ BE(u32) movieDataOffsets;
	/* 0x2C */ BE(u32) finalFrameDataOffsets;
} THPHeader;

static u32 THPAudioDecode(s16* audioBuffer, u8* audioFrame, s32 flag);
static s32 __THPAudioGetNewSample(THPAudioDecodeInfo* info);
static void __THPAudioInitialize(THPAudioDecodeInfo* info, u8* ptr);

#define THP_AUDIO_BUFFER_COUNT 3
#define THP_READ_BUFFER_COUNT  10
#define THP_TEXTURE_SET_COUNT  3
#endif

struct daMP_THPPlayer {
    /* 0x000 */ DVDFileInfo fileInfo;
	/* 0x03C */ THPHeader header;
	/* 0x06C */ THPFrameCompInfo compInfo;
	/* 0x080 */ THPVideoInfo videoInfo;
	/* 0x08C */ THPAudioInfo audioInfo;
	/* 0x09C */ void* thpWork;
	/* 0x0A0 */ BOOL open;
	/* 0x0A4 */ u8 state;
	/* 0x0A5 */ u8 internalState;
	/* 0x0A6 */ u8 playFlag;
	/* 0x0A7 */ u8 audioExist;
	/* 0x0A8 */ s32 dvdError;
	/* 0x0AC */ s32 videoError;
	/* 0x0B0 */ BOOL onMemory;
	/* 0x0B4 */ u8* movieData;
	/* 0x0B8 */ s32 initOffset;
	/* 0x0BC */ s32 initReadSize;
	/* 0x0C0 */ s32 initReadFrame;
	/* 0x0C4 */ u32 curField;
	/* 0x0C8 */ s64 retaceCount;
	/* 0x0D0 */ s32 prevCount;
	/* 0x0D4 */ s32 curCount;
#if TARGET_PC
	/* 0x0D8 */ std::atomic<s32> videoDecodeCount;
#else
	/* 0x0D8 */ s32 videoDecodeCount;
#endif
	/* 0x0DC */ f32 curVolume;
	/* 0x0E0 */ f32 targetVolume;
	/* 0x0E4 */ f32 deltaVolume;
	/* 0x0E8 */ s32 rampCount;
	/* 0x0EC */ s32 curAudioTrack;
	/* 0x0F0 */ s32 curVideoNumber;
	/* 0x0F4 */ s32 curAudioNumber;
	/* 0x0F8 */ THPTextureSet* dispTextureSet;
	/* 0x0FC */ THPAudioBuffer* playAudioBuffer;
	/* 0x100 */ daMP_THPReadBuffer readBuffer[10];
	/* 0x000 */ THPTextureSet textureSet[THP_TEXTURE_SET_COUNT];
	/* 0x000 */ THPAudioBuffer audioBuffer[THP_AUDIO_BUFFER_COUNT];
};

/**
 * @ingroup actors-unsorted
 * @class daMP_c
 * @brief Movie Player
 *
 * @details
 *
 */
class daMP_c : public fopAc_ac_c {
public:
    static int daMP_c_THPPlayerPlay();
    static void daMP_c_THPPlayerPause();
    static u32 daMP_c_Get_MovieRestFrame();
    static void daMP_c_Set_PercentMovieVolume(f32);
    int daMP_c_Get_arg_demoNo();
    int daMP_c_Get_arg_movieNo();
    int daMP_c_Init();
    int daMP_c_Finish();
    int daMP_c_Main();
    int daMP_c_Draw();

    static int daMP_c_Callback_Init(fopAc_ac_c*);
    static int daMP_c_Callback_Finish(daMP_c*);
    static int daMP_c_Callback_Main(daMP_c*);
    static int daMP_c_Callback_Draw(daMP_c*);

    static daMP_c* m_myObj;

private:
    /* 0x568 */ u32 (*mpGetMovieRestFrame)(void);
    /* 0x56C */ void (*mpSetPercentMovieVol)(f32);
    /* 0x570 */ u32 (*mpTHPGetTotalFrame)(void);
    /* 0x574 */ int (*mpTHPPlay)(void);
    /* 0x578 */ void (*mpTHPStop)(void);
    /* 0x57C */ int (*mpTHPPause)(void);
};

STATIC_ASSERT(sizeof(daMP_c) == 0x580);

class daMP_Dlst_base_c : public dDlst_base_c {
public:
    daMP_Dlst_base_c() {}

    virtual void draw();
};

void daMP_PrepareReady(int);

#endif /* D_A_MOVIE_PLAYER_H */
