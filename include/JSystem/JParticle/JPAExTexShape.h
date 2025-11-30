#ifndef JPAEXTEXSHAPE_H
#define JPAEXTEXSHAPE_H

#include "dolphin/types.h"

class JPAEmitterWorkData;

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAExTexShapeData {
    // Common header.
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u32 mSize;

    /* 0x08 */ u32 mFlags;
    /* 0x0C */ f32 mIndTexMtx[2][3];
    /* 0x24 */ s8 mExpScale;
    /* 0x25 */ s8 mIndTexIdx;
    /* 0x26 */ s8 mSecTexIdx;
};  // Size: 0x28

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAExTexShape {
public:
    JPAExTexShape(u8 const*);

    const f32* getIndTexMtx() const { return &mpData->mIndTexMtx[0][0]; }
    s8 getExpScale() const { return mpData->mExpScale; }
    u8 getIndTexIdx() const { return mpData->mIndTexIdx; }
    u8 getSecTexIdx() const { return mpData->mSecTexIdx; }
    bool isUseIndirect() const { return !!(mpData->mFlags & 0x01); }
    bool isUseSecTex() const { return !!(mpData->mFlags & 0x0100); }

public:
    const JPAExTexShapeData* mpData;
};

void JPALoadExTex(JPAEmitterWorkData*);

#endif /* JPAEXTEXSHAPE_H */
