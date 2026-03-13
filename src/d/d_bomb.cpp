/**
 * d_bomb.cpp
 * dolzel2 - Bomb
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bomb.h"
#include "d/actor/d_a_nbomb.h"

bool dBomb_c::checkStateCarry() {
    return (fopAcM_checkCarryNow(this) || fopAcM_checkHookCarryNow(this) ||
            fopAcM_GetParam(this) == PRM_BOMB_HOOKSHOT || fopAcM_GetParam(this) == PRM_BOMB_CARRY);
}

bool dBomb_c::checkFlowerBombWait(fopAc_ac_c* i_actor) {
    return (fopAcM_GetName(i_actor) == fpcNm_NBOMB_e && fopAcM_GetParam(i_actor) == PRM_FLOWER_BOMB);
}

bool dBomb_c::checkWaterBomb(fopAc_ac_c* i_actor) {
    return (fopAcM_GetName(i_actor) == fpcNm_NBOMB_e && ((daNbomb_c*)i_actor)->checkWaterBomb());
}

bool dBomb_c::checkInsectBombMove(fopAc_ac_c* i_actor) {
    return (fopAcM_GetName(i_actor) == fpcNm_NBOMB_e &&
            fopAcM_GetParam(i_actor) == PRM_INSECT_BOMB_MOVE);
}
