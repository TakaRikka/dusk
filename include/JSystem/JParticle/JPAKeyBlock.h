#ifndef JPAKEYBLOCK_H
#define JPAKEYBLOCK_H

#include <dolphin/types.h>

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAKeyBlock {
    JPAKeyBlock(u8 const*);
    f32 calc(f32);

    u8 getID() const { return mDataStart[8]; }
    u8 getKeyNum() const { return mDataStart[9]; }
    BOOL isLoop() const { return mDataStart[11]; }
    const f32* getKeyData() const { return field_0x4; }

    const u8* mDataStart;
	const f32* field_0x4;
};


#endif /* JPAKEYBLOCK_H */
