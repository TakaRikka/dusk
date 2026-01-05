#include <dolphin/axart.h>
#include <dolphin/os.h>
#include <dolphin/mix.h>
#include "sdk_math.h"

static u16 __coefs[48] = {
    0x6A09, 0x15F6, 0x6871, 0x178E, 0x6463, 0x1B9C,
	0x5DB3, 0x224C, 0x5618, 0x29E7, 0x4D7A, 0x3285,
	0x4367, 0x3C98, 0x3A5A, 0x45A5, 0x31C5, 0x4E3A,
	0x2924, 0x56DB, 0x2244, 0x5DBB, 0x1C50, 0x63AF,
	0x16C0, 0x693F, 0x1292, 0x6D6D, 0x0F18, 0x70E7,
	0x0BF5, 0x740A, 0x09A9, 0x7656, 0x07CA, 0x7835,
	0x0646, 0x79B9, 0x04ED, 0x7B12, 0x03F5, 0x7C0A,
	0x032D, 0x7CD2, 0x027D, 0x7D82, 0x01FE, 0x7E01
};

void AXARTLpf(AXART_LPF* articulator, AXVPB* voice) {
    u32 i;
    ASSERTLINE(68, articulator);
    ASSERTLINE(69, voice);

    if (articulator->update != 0) {
        if (articulator->initLPF != 0) {
            articulator->initLPF = 0;
            voice->pb.lpf.on = 1;
            voice->pb.lpf.yn1 = 0;
            voice->sync |= 0x200000;
        } else {
            voice->sync |= 0x400000;
        }

        i = articulator->frequency;
        ASSERTMSGLINE(90, i < 24, "AXART: roll off frequency should be < 24");

        i *= 2;
        voice->pb.lpf.a0 = __coefs[i];

        i++;
        voice->pb.lpf.b0 = __coefs[i];
        articulator->update = 0;
    }
}
