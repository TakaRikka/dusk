#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JParticle/JPAKeyBlock.h"
#include "JSystem/JParticle/JPAMath.h"

JPAKeyBlock::JPAKeyBlock(const u8* data)
    : mDataStart(data)
    , field_0x4(reinterpret_cast<const float*>(&data[0xC]))
{
}

f32 JPAKeyBlock::calc(float p1) {
	if (mDataStart[0xB] != '\0') {
		int v1 = (int)field_0x4[(mDataStart[9] - 1) * 4] + 1;
		// p1 -= (v1 * ((int)p1 / v1));
		int v2 = ((int)p1 / v1);
		p1     = p1 - (v2 * v1);
	}
	return JPACalcKeyAnmValue(p1, mDataStart[9], field_0x4);
}
