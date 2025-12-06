/**
 * d_a_alink_bomb.inc
 * Player Bomb action handling
 */

#include "d/actor/d_a_alink.h"

s16 daAlink_c::getBombExplodeTime() const {
    return mpHIO->mItem.mBomb.m.mExplodeTime;
}

f32 daAlink_c::getBombGravity() const {
    return mpHIO->mItem.mBomb.m.mGravity;
}

f32 daAlink_c::getBombMaxFallSpeed() const {
    return mpHIO->mItem.mBomb.m.mMaxFallSpeed;
}

f32 daAlink_c::getBombBoundRate() const {
    return mpHIO->mItem.mBomb.m.mBoundRate;
}

f32 daAlink_c::getBombStopSpeedY() const {
    return mpHIO->mItem.mBomb.m.mStopSpeedY;
}

f32 daAlink_c::getBombMaxSpeedY() const {
    return mpHIO->mItem.mBomb.m.mMaxSpeedY;
}

f32 daAlink_c::getBombEffScale() const {
    return mpHIO->mItem.mBomb.m.mEffScale;
}

f32 daAlink_c::getBombAtR() const {
    return mpHIO->mItem.mBomb.m.mAttackRadius;
}

s16 daAlink_c::getEnemyBombColorR() const {
    return mpHIO->mItem.mBomb.m.mEnemyBombColorR;
}

f32 daAlink_c::getBombWaterGravity() const {
    return mpHIO->mItem.mBomb.m.mWaterGravity;
}

f32 daAlink_c::getBombWaterMaxFallSpeed() const {
    return mpHIO->mItem.mBomb.m.mWaterMaxFallSpeed;
}

f32 daAlink_c::getBombExplodeWaterEffectLimit() const {
    return mpHIO->mItem.mBomb.m.mExplodeWaterEffectLimit;
}

s16 daAlink_c::getBombInsectLimitAngle() const {
    return mpHIO->mItem.mBomb.m.mBombInsectLimitAngle;
}
