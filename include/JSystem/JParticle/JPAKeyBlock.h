#ifndef JPAKEYBLOCK_H
#define JPAKEYBLOCK_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAKeyBlock {
    JPAKeyBlock(u8 const*);
    f32 calc(f32);

    u8 getID() { return mDataStart[8]; }

    const u8* mDataStart;
	const f32* field_0x4;
};


#endif /* JPAKEYBLOCK_H */
