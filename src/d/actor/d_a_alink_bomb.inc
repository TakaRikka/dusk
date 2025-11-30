/**
 * d_a_alink_bomb.inc
 * Player Bomb action handling
 */

#include "d/actor/d_a_alink.h"

s16 daAlink_c::getBombExplodeTime() const {
    return daAlinkHIO_bomb_c0::m.mExplodeTime;
}

f32 daAlink_c::getBombGravity() const {
    return daAlinkHIO_bomb_c0::m.mGravity;
}

f32 daAlink_c::getBombMaxFallSpeed() const {
    return daAlinkHIO_bomb_c0::m.mMaxFallSpeed;
}

f32 daAlink_c::getBombBoundRate() const {
    return daAlinkHIO_bomb_c0::m.mBoundRate;
}

f32 daAlink_c::getBombStopSpeedY() const {
    return daAlinkHIO_bomb_c0::m.mStopSpeedY;
}

f32 daAlink_c::getBombMaxSpeedY() const {
    return daAlinkHIO_bomb_c0::m.mMaxSpeedY;
}

f32 daAlink_c::getBombEffScale() const {
    return daAlinkHIO_bomb_c0::m.mEffScale;
}

f32 daAlink_c::getBombAtR() const {
    return daAlinkHIO_bomb_c0::m.mAttackRadius;
}

s16 daAlink_c::getEnemyBombColorR() const {
    return daAlinkHIO_bomb_c0::m.mEnemyBombColorR;
}

f32 daAlink_c::getBombWaterGravity() const {
    return daAlinkHIO_bomb_c0::m.mWaterGravity;
}

f32 daAlink_c::getBombWaterMaxFallSpeed() const {
    return daAlinkHIO_bomb_c0::m.mWaterMaxFallSpeed;
}

f32 daAlink_c::getBombExplodeWaterEffectLimit() const {
    return daAlinkHIO_bomb_c0::m.mExplodeWaterEffectLimit;
}

s16 daAlink_c::getBombInsectLimitAngle() const {
    return daAlinkHIO_bomb_c0::m.mBombInsectLimitAngle;
}
