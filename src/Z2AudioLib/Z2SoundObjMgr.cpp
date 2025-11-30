#include "Z2AudioLib/Z2SoundObjMgr.h"
#include "Z2AudioLib/Z2Creature.h"
#include "Z2AudioLib/Z2SeqMgr.h"
#include "Z2AudioLib/Z2Calc.h"
#include "Z2AudioLib/Z2SceneMgr.h"
#include "Z2AudioLib/Z2LinkMgr.h"

Z2SoundObjMgr::Z2SoundObjMgr() : JASGlobalInstance<Z2SoundObjMgr>(true) {
    ghostEnemyState_ = 0;
    twilightBattle_ = 0;
    setBattleInit();
    setForceBattleArea(false, 700, 1100, 1500);
}

void Z2SoundObjMgr::setForceBattleArea(bool forceBattle, u16 param_1, u16 param_2, u16 param_3) {
    forceBattle_ = forceBattle;
    enemyArea_.field_0x2 = param_1;
    enemyArea_.field_0x4 = param_2;
    enemyArea_.field_0x6 = param_3;
}

static Z2EnemyArea const sAreaDefault = {true, 700, 1100, 1500};

static Z2EnemyArea const sAreaFloating = {false, 700, 1100, 1500};

static Z2EnemyArea const sAreaWide = {true, 700, 1500, 3300};

static Z2EnemyArea const sAreaWideFloating = {false, 700, 1500, 3300};

static Z2EnemyArea const sAreaSmall = {true, 300, 700, 1000};

static Z2EnemyArea const sAreaSmallFloating = {false, 500, 800, 1200};

struct Z2EnemyInfo {
    char mName[8];
    Z2EnemyArea mEnemyArea;
};

static Z2EnemyInfo mEnemyInfo[64] = {
    {"Dummy", sAreaDefault},
    {"E_s1", {true, 700, 1200, 2000}},
    {"E_yk", sAreaSmallFloating},
    {"E_yr", sAreaWideFloating},
    {"E_yg", {true, 300, 500, 700}},
    {"E_yc", sAreaWideFloating},
    {"E_yd", {true, 500, 900, 1500}},
    {"E_rdy", sAreaDefault},
    {"E_ymb", {false, 10, 20, 30}},
    {"", sAreaDefault},
    {"E_wb", sAreaDefault},
    {"E_rd", sAreaDefault},
    {"E_dn", sAreaDefault},
    {"E_kr", sAreaWideFloating},
    {"E_st", sAreaDefault},
    {"E_tk", {true, 700, 1500, 2500}},
    {"E_tk2", {true, 700, 1500, 2500}},
    {"E_cr", sAreaSmall},
    {"E_df", sAreaDefault},
    {"E_fs", sAreaDefault},
    {"E_SB", sAreaDefault},
    {"E_PM", sAreaDefault},
    {"E_db", {true, 500, 900, 1800}},
    {"E_kg", sAreaSmall},
    {"E_sh", sAreaDefault},
    {"E_sf", sAreaDefault},
    {"E_bs", {true, 200, 400, 600}},
    {"E_SM", sAreaDefault},
    {"E_ba", sAreaSmallFloating},
    {"E_bu", sAreaFloating},
    {"E_HM", sAreaDefault},
    {"E_ge", sAreaFloating},
    {"E_hz", {false, 500, 800, 1300}},
    {"E_ws", sAreaSmallFloating},
    {"E_oc", sAreaDefault},
    {"E_fz", sAreaDefault},
    {"E_fb", sAreaDefault},
    {"E_bg", sAreaSmall},
    {"E_mm", {true, 500, 800, 1500}},
    {"E_nz", {true, 500, 700, 1100}},
    {"E_po", sAreaWide},
    {"E_rb", sAreaDefault},
    {"E_sg", {true, 500, 700, 900}},
    {"E_hb", {true, 500, 900, 1500}},
    {"E_gb", {true, 1600, 3000, 4000}},
    {"E_ms", {true, 500, 700, 1100}},
    {"E_dd", sAreaDefault},
    {"E_bi", {true, 500, 700, 900}},
    {"E_tt", sAreaSmallFloating},
    {"E_ai", sAreaDefault},
    {"E_dk", sAreaSmall},
    {"E_ww", sAreaWide},
    {"E_gi", sAreaDefault},
    {"E_sm2", sAreaDefault},
    {"E_sw", {true, 400, 600, 1500}},
    {"E_rdb", {true, 500, 1000, 1500}},
    {"E_kk", sAreaDefault},
    {"E_hp", sAreaDefault},
    {"E_zh", sAreaDefault},
    {"E_gm", sAreaSmall},
    {"B_GG", {false, 1000, 2000, 4000}},
    {"E_mf", sAreaDefault},
    {"E_zm", sAreaDefault},
    {"B_tn", sAreaDefault},
};

void Z2SoundObjMgr::searchEnemy() {
    twilightBattle_ = 0;

    if (!Z2GetLink()) {
        Z2GetSeqMgr()->stopBattleBgm(1, 1);
        return;
    }

    bool bVar7 = false;
    bool local_96 = false;

    bool isHolyForestBgm = false;
    if (Z2GetSeqMgr()->getMainBgmID() == Z2BGM_HOLY_FOREST) {
        isHolyForestBgm = true;
    }

    u8 subBgmType = 0;
    if (Z2GetSeqMgr()->getSubBgmID() == Z2BGM_HORSE_BATTLE) {
        subBgmType = 1;
    } else if (Z2GetSeqMgr()->getSubBgmID() == Z2BGM_FACE_OFF_BATTLE2) {
        subBgmType = 2;
    }

    field_0x14 = 0;
    enemuNumNear_ = 0;
    field_0x16 = 0;
    enemuNumVeryFar_ = 0;
    field_0x1a = false;
    field_0x18 = 0;

    f32 linkDistScale = Z2Calc::getParamByExp(
        Z2GetLink()->getMoveSpeed(),
        0.0f,
        100.0f,
        0.3f,
        1.0f,
        2.5f,
        Z2Calc::CURVE_POSITIVE);
    f32 veryFarDist = 5000.0f;

    for (JSULink<Z2CreatureEnemy>* i = getFirst(); i != NULL; i = i->getNext()) {
        Z2CreatureEnemy* enemy = i->getObject();
        if (enemy == NULL) {
            JUT_WARN_DEVICE(100, 1, "%s", "[Z2SoundObjMgr::searchEnemy] remain remove enemy\n");
            continue;
        }

        if (enemy->field_0xa1 < 1) {
            continue;
        }

        bool isGhostEnemy = false;

        u8 enemyId = enemy->mEnemyID;
        switch (enemyId) {
        case Z2_ENEMY_SW:
            if (Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_GERUDO_DESERT)
                continue;
            break;
        case Z2_ENEMY_CR:
            if (Z2GetSceneMgr()->getCurrentSceneNum() == Z2SCENE_HYRULE_FIELD)
                continue;
            break;
        case Z2_ENEMY_NZ:
        case Z2_ENEMY_PO:
        case Z2_ENEMY_HP:
            isGhostEnemy = true;
            break;
        case Z2_ENEMY_RDB:
            if (subBgmType == 0)
                continue;
            break;
        case Z2_ENEMY_WB:
        case Z2_ENEMY_RD:
            if (subBgmType != 0)
                continue;
#if VERSION == VERSION_GCN_JPN
        }
        switch (enemyId) {
#endif
        case Z2_ENEMY_DN:
        case Z2_ENEMY_DB:
        case Z2_ENEMY_GE:
        case Z2_ENEMY_OC:
        case Z2_ENEMY_MM:
        case Z2_ENEMY_RB:
        case Z2_ENEMY_HB:
        case Z2_ENEMY_TT:
        case Z2_ENEMY_SM2:
#if VERSION == VERSION_GCN_JPN
        case Z2_ENEMY_WB:
        case Z2_ENEMY_RD:
        case Z2_ENEMY_SH:
        case Z2_ENEMY_HP:
#endif
            if (!Z2GetLink()->isRiding())
                break;
            if (Z2GetLink()->getMoveSpeed() <= 38)
                break;
            continue;
        }

        Vec* enemyPos = enemy->getCurrentPos();
        if (enemyPos != NULL) {
            Vec* linkPos = Z2GetLink()->getCurrentPos();
            if (linkPos != NULL) {
                Z2EnemyArea enemyArea = mEnemyInfo[enemyId].mEnemyArea;
                if (forceBattle_) {
                    enemyArea = enemyArea_;
                }

                f32 y_scale = 2.0f;
                if (enemyArea.field_0x0 == true) {
                    y_scale = 4.0f;
                }

                Vec enemy_linkVec = {0.0f, 0.0f, 0.0f};
                VECSubtract(enemyPos, linkPos, &enemy_linkVec);
                enemy_linkVec.y *= y_scale;

                f32 enemyDist = VECMag(&enemy_linkVec);
                if (enemyDist < enemyArea.field_0x6 * linkDistScale) {
                    if (isGhostEnemy) {
                        field_0x1a++;
                        
                        if (enemyDist < enemyArea.field_0x4 * linkDistScale) {
                            ghostEnemyState_ |= (u8)1;
                            if (ghostEnemyState_ >= 16) {
                                enemuNumNear_++;
                                if (enemyDist < enemyArea.field_0x2 * linkDistScale) {
                                    field_0x14++;
                                }
                            }
                        }

                        if (ghostEnemyState_ >= 17) {
                            field_0x16++;
                            if (enemy->mLinkSearch) {
                                local_96 = true;
                            }
                        }
                    } else {
                        field_0x16++;
                        if (enemyId != 0 && enemyId < 9) {
                            if (twilightBattle_ == 0) {
                                twilightBattle_ = 1;
                            }

                            if (enemyId == Z2_ENEMY_S1 || enemyId == Z2_ENEMY_YC) {
                                twilightBattle_ = 2;
                            }

                            if (enemyId == Z2_ENEMY_YMB) {
                                twilightBattle_ = 2;
                            } else {
                                Z2GetSeqMgr()->changeSubBgmStatus(twilightBattle_);
                            }
                        }

                        if (enemyId != Z2_ENEMY_GB && enemy->mLinkSearch) {
                            local_96 = true;
                        }

                        if (enemyDist < enemyArea.field_0x4 * linkDistScale) {
                            enemuNumNear_++;
                            if (enemyDist < enemyArea.field_0x2 * linkDistScale) {
                                field_0x14++;
                            }
                        }
                    }
                }

                if (enemyDist < veryFarDist * linkDistScale) {
                    enemuNumVeryFar_++;
                }

                if (enemy->field_0xa3 != 0) {
                    bVar7 = true;
                    field_0x18++;
                }
            }
        }
    }

    if (ghostEnemyState_ == 17) {
        if (field_0x1a >= field_0x16) {
            Z2GetSeqMgr()->setBattleGhostMute(false);
        } else {
            Z2GetSeqMgr()->setBattleGhostMute(true);
        }
    } else if (ghostEnemyState_ < 32 && field_0x1a == 0) {
        setGhostEnemyState(0);
    }

    if (subBgmType == 1) {
        if (field_0x14 != 0) {
            Z2GetSeqMgr()->changeSubBgmStatus(1);
        } else {
            Z2GetSeqMgr()->changeSubBgmStatus(0);
        }
    } else if (subBgmType == 2) {
        if (enemuNumNear_ != 0) {
            Z2GetSeqMgr()->changeSubBgmStatus(1);
        } else {
            Z2GetSeqMgr()->changeSubBgmStatus(0);
        }
    } else if (isHolyForestBgm) {
        if (field_0x16 != 0) {
            Z2GetSeqMgr()->changeBgmStatus(1);
        } else {
            Z2GetSeqMgr()->changeBgmStatus(0);
        }
    } else if (!bVar7) {
        Z2GetSeqMgr()->setBattleSearched(local_96);

        if (field_0x14 != 0) {
            Z2GetSeqMgr()->setBattleDistState(0);
        } else if (enemuNumNear_ != 0) {
            Z2GetSeqMgr()->setBattleDistState(1);
        } else if (field_0x16 != 0) {
            Z2GetSeqMgr()->setBattleDistState(2);
        } else {
            Z2GetSeqMgr()->setBattleDistState(3);
        }
    }
}

void Z2SoundObjMgr::setGhostEnemyState(u8 state) {
    if (state == 32) {
        if (field_0x1a >= field_0x16) {
            Z2GetSeqMgr()->setBattleGhostMute(true);
        }
    } else {
        if (field_0x1a == false) {
            ghostEnemyState_ = 0;
            return;
        }
    }

    ghostEnemyState_ = state + (ghostEnemyState_ & 1);
}

u8 Z2SoundObjMgr::getEnemyID(const char* enemyName, JSULink<Z2CreatureEnemy>* enemyLink) {
    if (enemyName != NULL) {
        for (u8 i = 0; i < ARRAY_SIZEU(mEnemyInfo); i++) {
            if (strcmp(enemyName, mEnemyInfo[(u32)i].mName) == 0) {
                append(enemyLink);
                return i;
            }
        }
    }

    return Z2_ENEMY_NULL;
}

void Z2SoundObjMgr::setBattleInit() {
    field_0x14 = 0;
    enemuNumNear_ = 0;
    field_0x16 = 0;
    enemuNumVeryFar_ = 0;
    field_0x18 = 0;
    field_0x1a = false;
}

bool Z2SoundObjMgr::checkBattleFinish() {
    if (enemuNumVeryFar_ - field_0x18 > 0) {
        return false;
    } else {
        return true;
    }
}

void Z2SoundObjMgr::deleteEnemyAll() {
    JSULink<Z2CreatureEnemy>* i;

    // not moving the pointer forward looks like a bug, but deleteObject() actually unlinks the enemy from its owning list
    while (i = getFirst(), i != NULL) {
        Z2CreatureEnemy* enemy = i->getObject();
        if (enemy == NULL) {
            JUT_WARN_DEVICE(367, 1, "%s", "[Z2SoundObjMgr::searchEnemy] remain remove enemy\n");
        } else {
            enemy->deleteObject();
        }
    }
}

bool Z2SoundObjMgr::removeEnemy(JSULink<Z2CreatureEnemy>* enemyLink) {
    if (enemyLink == NULL) {
        return false;
    }

    Z2CreatureEnemy* enemy = enemyLink->getObject();
    if (enemy->field_0xa3 != 0 && field_0x18 != 0) {
        field_0x18--;
    }

    bool result = remove(enemyLink);
    return result;
}

u8 Z2SoundObjMgr::isTwilightBattle() {
    return twilightBattle_;
}
