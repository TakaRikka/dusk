#include "mix.h"


#include <string>

#include <revolution/types.h>

// #include <revolution/ax.h>
// #include <revolution/os.h>
// #include <revolution/os/OSInterrupt.h>



#define HBMMIX_MAX_CHANNELS	0x10
#define HBMMIX_MAX_INDICES	0x60
#define HBMMIX_MAX_KEYS		0x10


enum HBMMIXChannelModeFlags
{
	// TODO: figure out what these represent
	HBMMIX_FLAG_31	= 1u << 31,
	HBMMIX_FLAG_30	= 1 << 30,
	HBMMIX_FLAG_29	= 1 << 29,
	HBMMIX_FLAG_28	= 1 << 28,
};

typedef struct HBMMIXChannel
{
	AXVPB			*axvpb;	// size 0x04, offset 0x00
	HBMMIXSoundMode	mode;	// size 0x04, offset 0x04
	int				input;	// size 0x04, offset 0x08
	int				auxA;	// size 0x04, offset 0x0c
	int				pan;	// size 0x04, offset 0x10
	int				fader;	// size 0x04, offset 0x14
	int				l;		// size 0x04, offset 0x18
	int				r;		// size 0x04, offset 0x1c
	int				f;		// size 0x04, offset 0x20
	int				b;		// size 0x04, offset 0x24
	int				l1;		// size 0x04, offset 0x28
	int				r1;		// size 0x04, offset 0x2c
	u16				v;		// size 0x02, offset 0x30
	u16				v1;		// size 0x02, offset 0x32
	u16				vL;		// size 0x02, offset 0x34
	u16				vL1;	// size 0x02, offset 0x36
	u16				vR;		// size 0x02, offset 0x38
	u16				vR1;	// size 0x02, offset 0x3a
	u16				vS;		// size 0x02, offset 0x3c
	u16				vS1;	// size 0x02, offset 0x3e
	u16				vAL;	// size 0x02, offset 0x40
	u16				vAL1;	// size 0x02, offset 0x42
	u16				vAR;	// size 0x02, offset 0x44
	u16				vAR1;	// size 0x02, offset 0x46
	u16				vAS;	// size 0x02, offset 0x48
	u16				vAS1;	// size 0x02, offset 0x4a
	u16				vBL;	// size 0x02, offset 0x4c
	u16				vBL1;	// size 0x02, offset 0x4e
	u16				vBR;	// size 0x02, offset 0x50
	u16				vBR1;	// size 0x02, offset 0x52
	u16				vBS;	// size 0x02, offset 0x54
	u16				vBS1;	// size 0x02, offset 0x56
	u16				vCL;	// size 0x02, offset 0x58
	u16				vCL1;	// size 0x02, offset 0x5a
	u16				vCR;	// size 0x02, offset 0x5c
	u16				vCR1;	// size 0x02, offset 0x5e
	u16				vCS;	// size 0x02, offset 0x60
	u16				vCS1;	// size 0x02, offset 0x62
} HBMMIXChannel; // size 0x64


static void __HBMMIXSetPan(HBMMIXChannel *channel);
static void __HBMMIXResetChannel(HBMMIXChannel *channel);
static int __HBMMIXClampPan(int pan);
static u16 __HBMMIXGetVolume(int db_);


// .data
static u16 __HBMMIXVolumeTable[964 + 1] =
{
	    0,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
	    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
	    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
	    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
	    1,     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
	    1,     1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
	    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	    2,     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	    2,     2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
	    3,     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
	    3,     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
	    3,     4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
	    4,     4,     4,     4,     4,     4,     4,     4,     4,     5,     5,
	    5,     5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
	    5,     5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
	    6,     6,     6,     6,     6,     7,     7,     7,     7,     7,     7,
	    7,     7,     7,     7,     7,     7,     8,     8,     8,     8,     8,
	    8,     8,     8,     8,     8,     9,     9,     9,     9,     9,     9,
	    9,     9,     9,    10,    10,    10,    10,    10,    10,    10,    10,
	   10,    11,    11,    11,    11,    11,    11,    11,    12,    12,    12,
	   12,    12,    12,    12,    13,    13,    13,    13,    13,    13,    13,
	   14,    14,    14,    14,    14,    14,    15,    15,    15,    15,    15,
	   16,    16,    16,    16,    16,    17,    17,    17,    17,    17,    18,
	   18,    18,    18,    18,    19,    19,    19,    19,    19,    20,    20,
	   20,    20,    21,    21,    21,    21,    22,    22,    22,    22,    23,
	   23,    23,    24,    24,    24,    24,    25,    25,    25,    26,    26,
	   26,    26,    27,    27,    27,    28,    28,    28,    29,    29,    29,
	   30,    30,    30,    31,    31,    32,    32,    32,    33,    33,    33,
	   34,    34,    35,    35,    35,    36,    36,    37,    37,    38,    38,
	   38,    39,    39,    40,    40,    41,    41,    42,    42,    43,    43,
	   44,    44,    45,    45,    46,    46,    47,    47,    48,    49,    49,
	   50,    50,    51,    51,    52,    53,    53,    54,    55,    55,    56,
	   56,    57,    58,    58,    59,    60,    61,    61,    62,    63,    63,
	   64,    65,    66,    66,    67,    68,    69,    70,    70,    71,    72,
	   73,    74,    75,    75,    76,    77,    78,    79,    80,    81,    82,
	   83,    84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
	   94,    95,    96,    97,    98,   100,   101,   102,   103,   104,   106,
	  107,   108,   109,   111,   112,   113,   114,   116,   117,   118,   120,
	  121,   123,   124,   126,   127,   128,   130,   131,   133,   135,   136,
	  138,   139,   141,   143,   144,   146,   148,   149,   151,   153,   155,
	  156,   158,   160,   162,   164,   166,   168,   170,   171,   173,   175,
	  178,   180,   182,   184,   186,   188,   190,   192,   195,   197,   199,
	  202,   204,   206,   209,   211,   214,   216,   219,   221,   224,   226,
	  229,   231,   234,   237,   240,   242,   245,   248,   251,   254,   257,
	  260,   263,   266,   269,   272,   275,   278,   282,   285,   288,   292,
	  295,   298,   302,   305,   309,   312,   316,   320,   323,   327,   331,
	  335,   339,   343,   347,   351,   355,   359,   363,   367,   371,   376,
	  380,   384,   389,   393,   398,   403,   407,   412,   417,   422,   427,
	  431,   436,   442,   447,   452,   457,   462,   468,   473,   479,   484,
	  490,   495,   501,   507,   513,   519,   525,   531,   537,   543,   550,
	  556,   562,   569,   576,   582,   589,   596,   603,   610,   617,   624,
	  631,   638,   646,   653,   661,   669,   676,   684,   692,   700,   708,
	  716,   725,   733,   742,   750,   759,   768,   777,   786,   795,   804,
	  813,   823,   832,   842,   852,   861,   871,   881,   892,   902,   912,
	  923,   934,   945,   955,   967,   978,   989,  1001,  1012,  1024,  1036,
	 1048,  1060,  1072,  1085,  1097,  1110,  1123,  1136,  1149,  1162,  1176,
	 1189,  1203,  1217,  1231,  1245,  1260,  1274,  1289,  1304,  1319,  1334,
	 1350,  1365,  1381,  1397,  1414,  1430,  1446,  1463,  1480,  1497,  1515,
	 1532,  1550,  1568,  1586,  1604,  1623,  1642,  1661,  1680,  1700,  1719,
	 1739,  1759,  1780,  1800,  1821,  1842,  1864,  1885,  1907,  1929,  1951,
	 1974,  1997,  2020,  2043,  2067,  2091,  2115,  2140,  2164,  2190,  2215,
	 2241,  2266,  2293,  2319,  2346,  2373,  2401,  2429,  2457,  2485,  2514,
	 2543,  2573,  2602,  2632,  2663,  2694,  2725,  2757,  2789,  2821,  2853,
	 2887,  2920,  2954,  2988,  3023,  3058,  3093,  3129,  3165,  3202,  3239,
	 3276,  3314,  3353,  3391,  3431,  3470,  3511,  3551,  3592,  3634,  3676,
	 3719,  3762,  3805,  3849,  3894,  3939,  3985,  4031,  4078,  4125,  4173,
	 4221,  4270,  4319,  4369,  4420,  4471,  4523,  4575,  4628,  4682,  4736,
	 4791,  4846,  4902,  4959,  5017,  5075,  5133,  5193,  5253,  5314,  5375,
	 5438,  5501,  5564,  5629,  5694,  5760,  5827,  5894,  5962,  6031,  6101,
	 6172,  6243,  6316,  6389,  6463,  6538,  6613,  6690,  6767,  6846,  6925,
	 7005,  7086,  7168,  7251,  7335,  7420,  7506,  7593,  7681,  7770,  7860,
	 7951,  8043,  8136,  8230,  8326,  8422,  8520,  8618,  8718,  8819,  8921,
	 9025,  9129,  9235,  9342,  9450,  9559,  9670,  9782,  9895, 10010, 10126,
	10243, 10362, 10482, 10603, 10726, 10850, 10976, 11103, 11231, 11361, 11493,
	11626, 11761, 11897, 12035, 12174, 12315, 12458, 12602, 12748, 12895, 13045,
	13196, 13349, 13503, 13659, 13818, 13978, 14140, 14303, 14469, 14636, 14806,
	14977, 15151, 15326, 15504, 15683, 15865, 16049, 16234, 16422, 16613, 16805,
	17000, 17196, 17396, 17597, 17801, 18007, 18215, 18426, 18640, 18856, 19074,
	19295, 19518, 19744, 19973, 20204, 20438, 20675, 20914, 21156, 21401, 21649,
	21900, 22153, 22410, 22669, 22932, 23197, 23466, 23738, 24013, 24291, 24572,
	24857, 25144, 25436, 25730, 26028, 26329, 26634, 26943, 27255, 27570, 27890,
	28213, 28539, 28870, 29204, 29542, 29884, 30230, 30580, 30934, 31293, 31655,
	32022, 32392, 32767, 33147, 33531, 33919, 34312, 34709, 35111, 35518, 35929,
	36345, 36766, 37192, 37622, 38058, 38499, 38944, 39395, 39851, 40313, 40780,
	41252, 41730, 42213, 42702, 43196, 43696, 44202, 44714, 45232, 45756, 46286,
	46821, 47364, 47912, 48467, 49028, 49596, 50170, 50751, 51339, 51933, 52535,
	53143, 53758, 54381, 55011, 55648, 56292, 56944, 57603, 58270, 58945, 59627,
	60318, 61016, 61723, 62438, 63161, 63892, 64632, 65380
};

static int __HBMMIXPanTable[128] =
{
	   0,    0,   -1,   -1,   -1,   -2,   -2,   -2,   -3,   -3,   -4,   -4,   -4,
	  -5,   -5,   -5,   -6,   -6,   -7,   -7,   -7,   -8,   -8,   -9,   -9,  -10,
	 -10,  -10,  -11,  -11,  -12,  -12,  -13,  -13,  -14,  -14,  -14,  -15,  -15,
	 -16,  -16,  -17,  -17,  -18,  -18,  -19,  -20,  -20,  -21,  -21,  -22,  -22,
	 -23,  -23,  -24,  -25,  -25,  -26,  -26,  -27,  -28,  -28,  -29,  -30,  -30,
	 -31,  -32,  -33,  -33,  -34,  -35,  -36,  -36,  -37,  -38,  -39,  -40,  -40,
	 -41,  -42,  -43,  -44,  -45,  -46,  -47,  -48,  -49,  -50,  -51,  -52,  -54,
	 -55,  -56,  -57,  -59,  -60,  -61,  -63,  -64,  -66,  -67,  -69,  -71,  -72,
	 -74,  -76,  -78,  -80,  -83,  -85,  -87,  -90,  -93,  -96,  -99, -102, -106,
	-110, -115, -120, -126, -133, -140, -150, -163, -180, -210, -904
};

static s16 __HBMMIX_DPL2_front[128] =
{
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,   -1,   -1,   -1,   -1,   -1,   -2,   -2,   -2,   -2,   -3,   -3,
	  -3,   -4,   -4,   -4,   -5,   -5,   -6,   -6,   -6,   -7,   -7,   -8,
	  -8,   -9,   -9,  -10,  -11,  -11,  -12,  -12,  -13,  -14,  -14,  -15,
	 -16,  -17,  -17,  -18,  -19,  -20,  -21,  -21,  -22,  -23,  -24,  -25,
	 -26,  -27,  -28,  -29,  -30,  -31,  -32,  -34,  -35,  -36,  -37,  -38,
	 -40,  -41,  -42,  -44,  -45,  -47,  -48,  -50,  -52,  -53,  -55,  -57,
	 -58,  -60,  -62,  -64,  -66,  -68,  -70,  -73,  -75,  -77,  -80,  -82,
	 -85,  -88,  -90,  -93,  -96, -100, -103, -106, -110, -114, -118, -122,
	-126, -131, -136, -141, -146, -152, -159, -166, -173, -181, -190, -201,
	-212, -225, -241, -261, -286, -321, -381, -960
};

static s16 __HBMMIX_DPL2_rear[128] =
{
	-61, -61, -60, -59, -59, -58, -58, -57, -56, -56, -55, -55, -54, -53, -53,
	-52, -52, -51, -50, -50, -49, -49, -48, -48, -47, -47, -46, -46, -45, -45,
	-44, -44, -43, -43, -42, -42, -41, -41, -40, -40, -39, -39, -38, -38, -38,
	-37, -37, -36, -36, -35, -35, -35, -34, -34, -33, -33, -32, -32, -32, -31,
	-31, -31, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26,
	-26, -25, -25, -25, -24, -24, -24, -23, -23, -23, -22, -22, -22, -21, -21,
	-21, -20, -20, -20, -20, -19, -19, -19, -18, -18, -18, -18, -17, -17, -17,
	-17, -16, -16, -16, -16, -15, -15, -15, -15, -14, -14, -14, -14, -13, -13,
	-13, -13, -13, -12, -12, -12, -12, -11
};

// .bss
static s8 sIndex2IdTable[HBMMIX_MAX_KEYS];
static s8 sId2IndexTable[HBMMIX_MAX_INDICES];
static HBMMIXSoundMode __HBMMIXSoundMode;
static HBMMIXChannel *__HBMMIXChannel;
static HBMMIXChannel __s_HBMMIXChannel[HBMMIX_MAX_CHANNELS];
static BOOL __init;


static void __HBMMIXSetPan(HBMMIXChannel *channel)
{
	int pan, span, ipan, ispan;

	ASSERTLINE(211, (channel->pan <= 127) && (channel->pan >= 0));

	pan = channel->pan;
	ipan = 127 - pan;
	span = 127;
	ispan = 0;

	if (__HBMMIXSoundMode == HBMMIX_SOUND_MODE_DPL2)
	{
		channel->l = __HBMMIX_DPL2_front[pan];
		channel->r = __HBMMIX_DPL2_front[ipan];
		channel->f = __HBMMIX_DPL2_front[ispan];
		channel->b = __HBMMIX_DPL2_front[span];
		channel->l1 = __HBMMIX_DPL2_rear[ipan];
		channel->r1 = __HBMMIX_DPL2_rear[pan];
	}
	else
	{
		channel->l = __HBMMIXPanTable[pan];
		channel->r = __HBMMIXPanTable[ipan];
		channel->f = __HBMMIXPanTable[ispan];
		channel->b = __HBMMIXPanTable[span];
		channel->l1 = 0;
		channel->r1 = 0;
	}
}

static void __HBMMIXResetChannel(HBMMIXChannel *channel)
{
	channel->mode = HBMMIX_FLAG_30 | HBMMIX_FLAG_28;
	channel->input = 0;
	channel->auxA = -960;
	channel->fader = 0;
	channel->pan = 64;

	// one big long chain assignment
	channel->v =
	channel->vL =
	channel->vR =
	channel->vS =
	channel->vAL =
	channel->vAR =
	channel->vAS =
	channel->vBL =
	channel->vBR =
	channel->vBS =
	channel->vCL =
	channel->vCR =
	channel->vCS = 0;

	__HBMMIXSetPan(channel);
}

static int __HBMMIXClampPan(int pan)
{
	if (pan < 0)
		return 0;

	if (pan > 127)
		return 127;

	return pan;
}

static u16 __HBMMIXGetVolume(int db_)
{
	int const db = db_; // presumably? i can't think of another reason

	if (db <= -904)
		return 0;

	if (db >= 60)
		return 65380;

	return __HBMMIXVolumeTable[db + 904];
}

void HBMMIXInit(void)
{
	int i;

	ASSERTLINE(332, AXIsInit());

	if (AXIsInit() && !__init)
	{
		for (int i = 0; i < HBMMIX_MAX_KEYS; ++i)
			sIndex2IdTable[i] = HBMMIX_KEY_NULL;

		for (int i = 0; i < HBMMIX_MAX_INDICES; ++i)
			sId2IndexTable[i] = HBMMIX_INDEX_NULL;

		__HBMMIXChannel = __s_HBMMIXChannel;

		// what
		for (i = 0; i < HBMMIX_MAX_CHANNELS; ++i)
		{
			__HBMMIXChannel[i].axvpb = NULL;
			__HBMMIXResetChannel(&__HBMMIXChannel[i]);
		}

		__HBMMIXSoundMode = HBMMIX_SOUND_MODE_STEREO;
		__init = TRUE;
	}
}

void HBMMIXQuit(void)
{
	__HBMMIXChannel = NULL;
	__init = FALSE;
}

void HBMMIXSetSoundMode(HBMMIXSoundMode mode)
{
	ASSERTLINE(397, (mode == HBMMIX_SOUND_MODE_MONO)
	                     || (mode == HBMMIX_SOUND_MODE_STEREO)
	                     || (mode == HBMMIX_SOUND_MODE_DPL2));

	__HBMMIXSoundMode = mode;
}

HBMMIXSoundMode HBMMIXGetSoundMode(void)
{
	return __HBMMIXSoundMode;
}

void HBMMIXInitChannel(AXVPB *axvpb, int input, int auxA, int pan, int fader)
{
	BOOL intrStatus; /* int old; */
	HBMMIXChannel *c;
	u32 mixerCtrl;
	u16 *p;

	ASSERTLINE(449, axvpb);

	HBMMIXIndex index = HBMGetIndex(axvpb->index);
	c = &__HBMMIXChannel[index];

	c->axvpb	= axvpb;
	c->mode		= 0;
	c->input	= input;
	c->auxA		= auxA;
	c->pan		= pan;
	c->fader	= fader;

	__HBMMIXSetPan(c);
	c->v = __HBMMIXGetVolume(input);

	mixerCtrl = 0;

	switch (__HBMMIXSoundMode)
	{
	case HBMMIX_SOUND_MODE_MONO:
		c->vL	= __HBMMIXGetVolume(c->fader           + c->f     );
		c->vR	= __HBMMIXGetVolume(c->fader           + c->f     );
		c->vS	= __HBMMIXGetVolume(c->fader           + c->b - 30);
		c->vAL	= __HBMMIXGetVolume(c->fader + c->auxA + c->f     );
		c->vAR	= __HBMMIXGetVolume(c->fader + c->auxA + c->f     );
		c->vAS	= __HBMMIXGetVolume(c->fader + c->auxA + c->b - 30);
		c->vBL	= 0;
		c->vBR	= 0;
		c->vBS	= 0;
		c->vCL	= 0;
		c->vCR	= 0;
		c->vCS	= 0;
		break;

	case HBMMIX_SOUND_MODE_STEREO:
		c->vL	= __HBMMIXGetVolume(c->fader           + c->l + c->f);
		c->vR	= __HBMMIXGetVolume(c->fader           + c->r + c->f);
		c->vS	= __HBMMIXGetVolume(c->fader           + c->b -   30);
		c->vAL	= __HBMMIXGetVolume(c->fader + c->auxA + c->l + c->f);
		c->vAR	= __HBMMIXGetVolume(c->fader + c->auxA + c->r + c->f);
		c->vAS	= __HBMMIXGetVolume(c->fader + c->auxA + c->b -   30);
		c->vBL	= 0;
		c->vBR	= 0;
		c->vBS	= 0;
		c->vCL	= 0;
		c->vCR	= 0;
		c->vCS	= 0;
		break;

	case HBMMIX_SOUND_MODE_DPL2:
		c->vL	= __HBMMIXGetVolume(c->fader           + c->l  + c->f);
		c->vR	= __HBMMIXGetVolume(c->fader           + c->r  + c->f);
		c->vS	= __HBMMIXGetVolume(c->fader           + c->l1 + c->b);
		c->vCL	= __HBMMIXGetVolume(c->fader           + c->r1 + c->b);
		c->vAL	= __HBMMIXGetVolume(c->fader + c->auxA + c->l  + c->f);
		c->vAR	= __HBMMIXGetVolume(c->fader + c->auxA + c->r  + c->f);
		c->vAS	= __HBMMIXGetVolume(c->fader + c->auxA + c->l1 + c->b);
		c->vCR	= __HBMMIXGetVolume(c->fader + c->auxA + c->r1 + c->b);
		c->vBL	= 0;
		c->vBR	= 0;
		c->vBS	= 0;
		c->vCS	= 0;

		mixerCtrl |= HBMMIX_FLAG_31;
		break;
	}

	intrStatus = OSDisableInterrupts();

	axvpb->pb.ve.currentVolume = c->v;
	axvpb->pb.ve.currentDelta = 0;

	p = (u16 *)&axvpb->pb.mix;

#define DO_THING_(src_, flag_)						\
	do												\
	{												\
		if ((*p++ = c->src_))						\
			mixerCtrl |= AX_MIXER_CTRL_ ## flag_;	\
													\
		*p++ = 0;									\
	} while (0)

#define DO_THING_LR_(srcInfix_, flagInfix_)	\
	DO_THING_(v ## srcInfix_ ## L, flagInfix_ ## L);	\
	DO_THING_(v ## srcInfix_ ## R, flagInfix_ ## R)

	DO_THING_LR_( ,   );
	DO_THING_LR_(A, A_);
	DO_THING_LR_(B, B_);
	DO_THING_LR_(C, C_);

#undef DO_THING_LR_

#define DO_THING_S_(srcInfix_, flagInfix_)	\
	DO_THING_(v ## srcInfix_ ## S, flagInfix_ ## S)

	DO_THING_S_( ,   );
	DO_THING_S_(A, A_);
	DO_THING_S_(B, B_);
	DO_THING_S_(C, C_);

#undef DO_THING_S_

#undef DO_THING_

	axvpb->pb.mixerCtrl = mixerCtrl;
	axvpb->sync |= AX_VPB_SYNC_FLAG_MIXER_CTRL | AX_VPB_SYNC_FLAG_MIX
	             | AX_VPB_SYNC_FLAG_VE;

	p = (u16 *)&axvpb->pb.rmtMix;
	memset(p, 0, sizeof axvpb->pb.rmtMix);

	axvpb->pb.rmtMixerCtrl = 0;
	axvpb->sync |= AX_VPB_SYNC_FLAG_RMT_MIXER_CTRL | AX_VPB_SYNC_FLAG_RMT_MIX;

	OSRestoreInterrupts(intrStatus);
}

void HBMMIXReleaseChannel(AXVPB *axvpb)
{
	ASSERTLINE(590, axvpb);

	int index = HBMGetIndex(axvpb->index);
	__HBMMIXChannel[index].axvpb = NULL;
}

void HBMMIXSetInput(AXVPB *p, int dB)
{
	u32 index = HBMGetIndex(p->index);
	HBMMIXChannel *channel = __HBMMIXChannel + index;

	channel->input = dB;
	channel->mode |= HBMMIX_FLAG_28;
}

void HBMMIXSetAuxA(AXVPB *p, int dB)
{
	u32 index = HBMGetIndex(p->index);
	HBMMIXChannel *channel = __HBMMIXChannel + index;

	channel->auxA = dB;
	channel->mode |= HBMMIX_FLAG_30;
}

void HBMMIXSetPan(AXVPB *p, int pan)
{
	u32 index = HBMGetIndex(p->index);
	HBMMIXChannel *channel = __HBMMIXChannel + index;

	channel->pan = __HBMMIXClampPan(pan);
	__HBMMIXSetPan(channel);
	channel->mode |= HBMMIX_FLAG_30;
}

void HBMMIXSetFader(AXVPB *p, int dB)
{
	u32 index = HBMGetIndex(p->index);
	HBMMIXChannel *channel = __HBMMIXChannel + index;

	channel->fader = dB;
	channel->mode |= HBMMIX_FLAG_30;
}

void HBMMIXUpdateSettings(void)
{
	int i;

	if (!__init)
		return;

	for (i = 0; i < HBMMIX_MAX_CHANNELS; ++i)
	{
		int setNewMixLevel;
		int setNewInputLevel;
		HBMMIXChannel *c;
		AXVPB *axvpb;

		setNewInputLevel = 0;
		setNewMixLevel = 0;
		c = &__HBMMIXChannel[i];
		axvpb = c->axvpb;

		if (axvpb)
		{
			u32 mixerCtrl = 0;

			if (c->mode & HBMMIX_FLAG_29)
			{
				c->v = c->v1;

				c->mode &= ~HBMMIX_FLAG_29;
				setNewInputLevel = TRUE;
			}

			if (c->mode & HBMMIX_FLAG_28)
			{
				c->v1 = __HBMMIXGetVolume(c->input);

				c->mode &= ~HBMMIX_FLAG_28;
				c->mode |= HBMMIX_FLAG_29;
				setNewInputLevel = TRUE;
			}

			if (c->mode & HBMMIX_FLAG_31)
			{
				c->vL = c->vL1;
				c->vR = c->vR1;
				c->vS = c->vS1;
				c->vAL = c->vAL1;
				c->vAR = c->vAR1;
				c->vAS = c->vAS1;
				c->vBL = c->vBL1;
				c->vBR = c->vBR1;
				c->vBS = c->vBS1;
				c->vCL = c->vCL1;
				c->vCR = c->vCR1;
				c->vCS = c->vCS1;

				c->mode &= ~HBMMIX_FLAG_31;
				setNewMixLevel = TRUE;
			}

			if (c->mode & HBMMIX_FLAG_30)
			{
				switch (__HBMMIXSoundMode)
				{
				case HBMMIX_SOUND_MODE_MONO:
					c->vL1	= __HBMMIXGetVolume(c->fader           + c->f     );
					c->vR1	= __HBMMIXGetVolume(c->fader           + c->f     );
					c->vS1	= __HBMMIXGetVolume(c->fader           + c->b - 30);
					c->vAL1	= __HBMMIXGetVolume(c->fader + c->auxA + c->f     );
					c->vAR1	= __HBMMIXGetVolume(c->fader + c->auxA + c->f     );
					c->vAS1	= __HBMMIXGetVolume(c->fader + c->auxA + c->b - 30);
					c->vBL1	= 0;
					c->vBR1	= 0;
					c->vBS1	= 0;
					c->vCL1	= 0;
					c->vCR1	= 0;
					c->vCS1	= 0;
					break;

				case HBMMIX_SOUND_MODE_STEREO:
					c->vL1	= __HBMMIXGetVolume(c->fader           + c->l + c->f);
					c->vR1	= __HBMMIXGetVolume(c->fader           + c->r + c->f);
					c->vS1	= __HBMMIXGetVolume(c->fader           + c->b -   30);
					c->vAL1	= __HBMMIXGetVolume(c->fader + c->auxA + c->l + c->f);
					c->vAR1	= __HBMMIXGetVolume(c->fader + c->auxA + c->r + c->f);
					c->vAS1	= __HBMMIXGetVolume(c->fader + c->auxA + c->b -   30);
					c->vBL1	= 0;
					c->vBR1	= 0;
					c->vBS1	= 0;
					c->vCL1	= 0;
					c->vCR1	= 0;
					c->vCS1	= 0;
					break;

				case HBMMIX_SOUND_MODE_DPL2:
					c->vL1	= __HBMMIXGetVolume(c->fader           + c->l  + c->f);
					c->vR1	= __HBMMIXGetVolume(c->fader           + c->r  + c->f);
					c->vS1	= __HBMMIXGetVolume(c->fader           + c->l1 + c->b);
					c->vCL1	= __HBMMIXGetVolume(c->fader           + c->r1 + c->b);
					c->vAL1	= __HBMMIXGetVolume(c->fader + c->auxA + c->l  + c->f);
					c->vAR1	= __HBMMIXGetVolume(c->fader + c->auxA + c->r  + c->f);
					c->vAS1	= __HBMMIXGetVolume(c->fader + c->auxA + c->l1 + c->b);
					c->vCR1	= __HBMMIXGetVolume(c->fader + c->auxA + c->r1 + c->b);
					c->vBL1	= 0;
					c->vBR1	= 0;
					c->vBS1	= 0;
					c->vCS1	= 0;

					mixerCtrl |= HBMMIX_FLAG_31;
					break;
				}

				c->mode &= ~HBMMIX_FLAG_30;
				c->mode |= HBMMIX_FLAG_31;
				setNewMixLevel = TRUE;
			}

			if (setNewInputLevel)
			{
				axvpb->pb.ve.currentVolume = c->v;
				axvpb->pb.ve.currentDelta = (c->v1 - c->v) / 96;
				axvpb->sync |= AX_VPB_SYNC_FLAG_VE;
			}

			if (setNewMixLevel)
			{
				u16 *p = (u16 *)&axvpb->pb.mix;

#define DO_THING_(src_, flag1_, flag2_)				\
	do												\
	{												\
		if ((*p++ = c->src_))						\
			mixerCtrl |= AX_MIXER_CTRL_ ## flag1_;	\
													\
		if ((*p++ = (c->src_##1 - c->src_) / 96))	\
			mixerCtrl |= AX_MIXER_CTRL_ ## flag2_;	\
	} while (0)

#define DO_THING_LR_(srcInfix_, flagInfix_)	\
	DO_THING_(v ## srcInfix_ ## L, flagInfix_ ## L, flagInfix_ ## DELTA);	\
	DO_THING_(v ## srcInfix_ ## R, flagInfix_ ## R, flagInfix_ ## DELTA)

				DO_THING_LR_( ,   );
				DO_THING_LR_(A, A_);
				DO_THING_LR_(B, B_);
				DO_THING_LR_(C, C_);

#undef DO_THING_LR_

#define DO_THING_S_(srcInfix_, flagInfix_)	\
	DO_THING_(v ## srcInfix_ ## S, flagInfix_ ## S, flagInfix_ ## DELTA_S)

				DO_THING_S_( ,   );
				DO_THING_S_(A, A_);
				DO_THING_S_(B, B_);
				DO_THING_S_(C, C_);

#undef DO_THING_S_

#undef DO_THING_

				axvpb->pb.mixerCtrl = mixerCtrl;
				axvpb->sync |=
					AX_VPB_SYNC_FLAG_MIXER_CTRL | AX_VPB_SYNC_FLAG_MIX;
			}
		}
	}
}

HBMMIXIndex HBMAllocIndex(HBMMIXKey key)
{
	for (int i = 0; i < HBMMIX_MAX_KEYS; ++i)
	{
		if (sIndex2IdTable[i] < 0)
		{
			sIndex2IdTable[i] = key;
			sId2IndexTable[key] = i;

			return i;
		}
	}

	return HBMMIX_INDEX_NULL;
}

HBMMIXIndex HBMGetIndex(HBMMIXKey key)
{
	return sId2IndexTable[key];
}

void HBMFreeIndex(HBMMIXIndex index)
{
	HBMMIXKey key = sIndex2IdTable[index];

	sIndex2IdTable[index] = HBMMIX_KEY_NULL;
	sId2IndexTable[key] = HBMMIX_INDEX_NULL;
}

void HBMFreeIndexByKey(HBMMIXKey key)
{
	HBMMIXIndex index = sId2IndexTable[key];

	if (index >= 0)
	{
		sIndex2IdTable[index] = HBMMIX_KEY_NULL;
		sId2IndexTable[key] = HBMMIX_INDEX_NULL;
	}
}
