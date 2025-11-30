#ifndef D_D_DOOR_PARAM2_H
#define D_D_DOOR_PARAM2_H

#include "dolphin/types.h"

class fopAc_ac_c;

class door_param2_c {
public:
    static int getKind(fopAc_ac_c* i_actor);
    static u32 getDoorModel(fopAc_ac_c* i_actor);
    static u8 getFrontOption(fopAc_ac_c* i_actor);
    static u8 getBackOption(fopAc_ac_c* i_actor);
    static u8 getFRoomNo(fopAc_ac_c* i_actor);
    static u8 getBRoomNo(fopAc_ac_c* i_actor);
    static u8 getSwbit(fopAc_ac_c* i_actor);
    static u8 getSwbit2(fopAc_ac_c* i_actor);
    static u8 getSwbit3(fopAc_ac_c* i_actor);
    static int isMsgDoor(fopAc_ac_c* i_actor);
    static u8 getEventNo(fopAc_ac_c* i_actor);
    static u8 getEventNo2(fopAc_ac_c* i_actor);
    static u16 getMsgNo(fopAc_ac_c* i_actor);
    static u8 getExitNo(fopAc_ac_c* i_actor);
    static u32 getFLightInf(fopAc_ac_c* i_actor);
    static u32 getBLightInf(fopAc_ac_c* i_actor);
    static u32 getMFLightInf(fopAc_ac_c* i_actor);
    static u32 getMBLightInf(fopAc_ac_c* i_actor);
};

#endif /* D_D_DOOR_PARAM2_H */
