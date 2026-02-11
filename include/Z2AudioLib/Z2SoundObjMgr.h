#ifndef Z2SOUNDOBJMGR_H
#define Z2SOUNDOBJMGR_H

#include "JSystem/JAudio2/JASGadget.h"
#include "JSystem/JSupport/JSUList.h"
#include "Z2AudioLib/Z2SoundObject.h"
#include "global.h"

class Z2CreatureEnemy;

struct Z2EnemyArea {
    /* 0x00 */ bool field_0x0;
    /* 0x02 */ u16 field_0x2;
    /* 0x04 */ u16 field_0x4;
    /* 0x06 */ u16 field_0x6;
};

enum Z2EnemyID {
    /* 0x00 */ Z2_ENEMY_NULL,
    /* 0x01 */ Z2_ENEMY_S1,
    /* 0x02 */ Z2_ENEMY_YK,
    /* 0x03 */ Z2_ENEMY_YR,
    /* 0x04 */ Z2_ENEMY_YG,
    /* 0x05 */ Z2_ENEMY_YC,
    /* 0x06 */ Z2_ENEMY_YD,
    /* 0x07 */ Z2_ENEMY_RDY,
    /* 0x08 */ Z2_ENEMY_YMB,
    /* 0x09 */ Z2_ENEMY_UNK,
    /* 0x0A */ Z2_ENEMY_WB,
    /* 0x0B */ Z2_ENEMY_RD,
    /* 0x0C */ Z2_ENEMY_DN,
    /* 0x0D */ Z2_ENEMY_KR,
    /* 0x0E */ Z2_ENEMY_ST,
    /* 0x0F */ Z2_ENEMY_TK,
    /* 0x10 */ Z2_ENEMY_TK2,
    /* 0x11 */ Z2_ENEMY_CR,
    /* 0x12 */ Z2_ENEMY_DF,
    /* 0x13 */ Z2_ENEMY_FS,
    /* 0x14 */ Z2_ENEMY_SB,
    /* 0x15 */ Z2_ENEMY_PM,
    /* 0x16 */ Z2_ENEMY_DB,
    /* 0x17 */ Z2_ENEMY_KG,
    /* 0x18 */ Z2_ENEMY_SH,
    /* 0x19 */ Z2_ENEMY_SF,
    /* 0x1A */ Z2_ENEMY_BS,
    /* 0x1B */ Z2_ENEMY_SM,
    /* 0x1C */ Z2_ENEMY_BA,
    /* 0x1D */ Z2_ENEMY_BU,
    /* 0x1E */ Z2_ENEMY_HM,
    /* 0x1F */ Z2_ENEMY_GE,
    /* 0x20 */ Z2_ENEMY_HZ,
    /* 0x21 */ Z2_ENEMY_WS,
    /* 0x22 */ Z2_ENEMY_OC,
    /* 0x23 */ Z2_ENEMY_FZ,
    /* 0x24 */ Z2_ENEMY_FB,
    /* 0x25 */ Z2_ENEMY_BG,
    /* 0x26 */ Z2_ENEMY_MM,
    /* 0x27 */ Z2_ENEMY_NZ,
    /* 0x28 */ Z2_ENEMY_PO,
    /* 0x29 */ Z2_ENEMY_RB,
    /* 0x2A */ Z2_ENEMY_SG,
    /* 0x2B */ Z2_ENEMY_HB,
    /* 0x2C */ Z2_ENEMY_GB,
    /* 0x2D */ Z2_ENEMY_MS,
    /* 0x2E */ Z2_ENEMY_DD,
    /* 0x2F */ Z2_ENEMY_BI,
    /* 0x30 */ Z2_ENEMY_TT,
    /* 0x31 */ Z2_ENEMY_AI,
    /* 0x32 */ Z2_ENEMY_DK,
    /* 0x33 */ Z2_ENEMY_WW,
    /* 0x34 */ Z2_ENEMY_GI,
    /* 0x35 */ Z2_ENEMY_SM2,
    /* 0x36 */ Z2_ENEMY_SW,
    /* 0x37 */ Z2_ENEMY_RDB,
    /* 0x38 */ Z2_ENEMY_KK,
    /* 0x39 */ Z2_ENEMY_HP,
    /* 0x3A */ Z2_ENEMY_ZH,
    /* 0x3B */ Z2_ENEMY_GM,
    /* 0x3C */ Z2_ENEMY_GG,
    /* 0x3D */ Z2_ENEMY_MF,
    /* 0x3E */ Z2_ENEMY_ZM,
    /* 0x3F */ Z2_ENEMY_TN,
};
 
class Z2SoundObjMgr : public JASGlobalInstance<Z2SoundObjMgr> {
public:
    Z2SoundObjMgr();
    void setForceBattleArea(bool forceBattle, u16, u16, u16);
    void searchEnemy();
    void setGhostEnemyState(u8 state);
    u8 getEnemyID(const char* enemyName, JSULink<Z2CreatureEnemy>* enemyLink);
    void setBattleInit();
    bool checkBattleFinish();
    void deleteEnemyAll();
    bool removeEnemy(JSULink<Z2CreatureEnemy>* enemyLink);
    u8 isTwilightBattle();

    u8 getEnemyNumNear() const { return enemuNumNear_; }
    u8 getEnemyNumVeryFar() const { return enemuNumVeryFar_; }
    bool isForceBattle() { return forceBattle_; }
    JSUList<Z2CreatureEnemy>* getEnemyList() { return &field_0x0; }

    #if DEBUG
    JSUList<Z2SoundObjBase>* getAllList() { return &allList_; }
    #endif

private:
    /* 0x00 */ JSUList<Z2CreatureEnemy> field_0x0;
    #if DEBUG
    /* 0x0C */ JSUList<Z2SoundObjBase> allList_;
    #endif
    /* 0x0C */ Z2EnemyArea enemyArea_;
    /* 0x14 */ u8 field_0x14;
    /* 0x15 */ u8 enemuNumNear_;
    /* 0x16 */ u8 field_0x16;
    /* 0x17 */ u8 enemuNumVeryFar_;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 ghostEnemyState_;
    /* 0x1A */ u8 field_0x1a;
    /* 0x1B */ u8 twilightBattle_;
    /* 0x1C */ bool forceBattle_;
};  // Size = 0x20

inline Z2SoundObjMgr* Z2GetSoundObjMgr() {
    return JASGlobalInstance<Z2SoundObjMgr>::getInstance();
}

#endif /* Z2SOUNDOBJMGR_H */
