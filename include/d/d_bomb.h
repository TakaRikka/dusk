#ifndef D_D_BOMB_H
#define D_D_BOMB_H

#include "f_op/f_op_actor_mng.h"

class dBomb_c : public fopAc_ac_c {
public:
    enum dBomb_PRM {
        PRM_NORMAL_BOMB_EXPLODE,
        PRM_BOMB_CARRY,
        PRM_BOMB_HOOKSHOT,
        PRM_BOMB_WAIT,
        PRM_FLOWER_BOMB,
        PRM_BOMB_BOOMERANG_MOVE,
        PRM_ENEMY_BOMB_BOOMERANG_MOVE,
        PRM_INSECT_BOMB_MOVE,
        PRM_NORMAL_BOMB_PLAYER,
        PRM_WATER_BOMB_PLAYER,
        PRM_INSECT_BOMB_PLAYER,
        PRM_ENEMY_BOMB,
        PRM_ENEMY_BOMB_BOOMERANG,
        PRM_ENEMY_BOMB_HOOKSHOT,
        PRM_ENEMY_WATER_BOMB,
        PRM_UNK_15,
        PRM_ENEMY_WATER_BOMB_HOOKSHOT,
        PRM_BOMB_CARGO_CARRY,
        PRM_WATER_BOMB_EXPLODE,
    };

    bool checkStateCarry();
    static bool checkFlowerBombWait(fopAc_ac_c*);
    static bool checkWaterBomb(fopAc_ac_c*);
    static bool checkInsectBombMove(fopAc_ac_c*);

    virtual u8 checkExplodeNow() { return false; }
    virtual void deleteBombAndEffect() {}
    virtual void setCargoBombExplode() {}

    bool checkStateExplode() { return fopAcM_GetParam(this) == 0; }

    static bool checkBombActor(fopAc_ac_c* i_actor) {
        return fopAcM_GetName(i_actor) == fpcNm_NBOMB_e;
    }

    static fopAc_ac_c* createNormalBombPlayer(cXyz* i_pos) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 8, i_pos, -1, NULL, NULL, -1, NULL, NULL);
    }

    static fopAc_ac_c* createNormalBombExplode(cXyz* i_pos) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 0, i_pos, -1, NULL, NULL, -1, NULL, NULL);
    }

    static fopAc_ac_c* createWaterBombPlayer(cXyz* i_pos) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 9, i_pos, -1, NULL, NULL, -1, NULL, NULL);
    }

    static fopAc_ac_c* createFlowerBomb(cXyz* i_pos, csXyz* i_angle, int param_2) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 4, i_pos, param_2, i_angle, NULL, -1,
                                              NULL, NULL);
    }

    static fopAc_ac_c* createInsectBombPlayer(cXyz* i_pos) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 10, i_pos, -1, NULL, NULL, -1,
                                              NULL, NULL);
    }

    static fopAc_ac_c* createEnemyBombBoomerang(cXyz* i_pos, csXyz* i_angle, int i_roomNo) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 12, i_pos, i_roomNo, i_angle, NULL, -1,
                                              NULL, NULL);
    }

    static fopAc_ac_c* createWaterBombExplode(cXyz* i_pos) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 18, i_pos, -1, NULL, NULL, -1, NULL,
                                              NULL);
    }

    static fopAc_ac_c* createEnemyWaterBomb(cXyz* i_pos, csXyz* i_angle, int i_roomNo) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 14, i_pos, i_roomNo, i_angle, NULL, -1,
                                              NULL, NULL);
    }

    static fopAc_ac_c* createEnemyWaterBombHookshot(cXyz* i_pos, csXyz* i_angle, int i_roomNo) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 16, i_pos, i_roomNo, i_angle, NULL, -1,
                                              NULL, NULL);
    }

    static fopAc_ac_c* createBombCargoCarry(cXyz* i_pos, csXyz* i_angle, int i_roomNo) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 0x11, i_pos, i_roomNo, i_angle, NULL, -1,
                                 NULL, NULL);
    }

    static fopAc_ac_c* createEnemyBombHookshot(cXyz* i_pos, csXyz* i_angle, int i_roomNo) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 13, i_pos, i_roomNo, i_angle, NULL, -1,
                                              NULL, NULL);
    }

    static fopAc_ac_c* createEnemyBomb(cXyz* i_pos, csXyz* i_angle, int i_roomNo) {
        return (fopAc_ac_c*)fopAcM_fastCreate(fpcNm_NBOMB_e, 0xB, i_pos, i_roomNo, i_angle, NULL, -1,
                                              NULL, NULL);
    }
};

#endif /* D_D_BOMB_H */
