/**
 * d_door_param2.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_door_param2.h"
#include "f_op/f_op_actor_mng.h"

int door_param2_c::getKind(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 0, 5);
}

u32 door_param2_c::getDoorModel(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 5, 3);
}

u8 door_param2_c::getFrontOption(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 8, 2);
}

u8 door_param2_c::getBackOption(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 10, 3);
}

u8 door_param2_c::getFRoomNo(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 13, 6);
}

u8 door_param2_c::getBRoomNo(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 19, 6);
}

u8 door_param2_c::getSwbit(fopAc_ac_c* i_actor) {
    return i_actor->home.angle.z & 0xFF;
}

u8 door_param2_c::getSwbit2(fopAc_ac_c* i_actor) {
    return (i_actor->home.angle.z >> 8) & 0xFF;
}

u8 door_param2_c::getSwbit3(fopAc_ac_c* i_actor) {
    return (i_actor->home.angle.x >> 8) & 0xFF;
}

int door_param2_c::isMsgDoor(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 31, 1);
}

u8 door_param2_c::getEventNo(fopAc_ac_c* i_actor) {
    return i_actor->home.angle.x & 0xFF;
}

u8 door_param2_c::getEventNo2(fopAc_ac_c* i_actor) {
    return (i_actor->home.angle.x >> 8) & 0xFF;
}

u16 door_param2_c::getMsgNo(fopAc_ac_c* i_actor) {
    return i_actor->home.angle.x & 0xFFFF;
}

u8 door_param2_c::getExitNo(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 25, 6);
}

u32 door_param2_c::getFLightInf(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 25, 3) & 0xFF;
}

u32 door_param2_c::getBLightInf(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 28, 3) & 0xFF;
}

u32 door_param2_c::getMFLightInf(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 19, 3) & 0xFF;
}

u32 door_param2_c::getMBLightInf(fopAc_ac_c* i_actor) {
    return fopAcM_GetParamBit(i_actor, 22, 3) & 0xFF;
}
