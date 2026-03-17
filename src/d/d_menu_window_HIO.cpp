/**
 * d_menu_window_HIO.cpp
 * Menu HostIO functionality
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_window_HIO.h"
#include "d/d_com_inf_game.h"
#include "d/d_msg_object.h"
#include "d/d_meter2_info.h"

#if DEBUG
#include "JSystem/JHostIO/JORServer.h"
#include "d/d_s_menu.h"
#endif

dMw_HIO_c::dMw_HIO_c() {
    init();
}

void dMw_HIO_c::init() {
    static u8 m_cloth_item[3] = {dItemNo_WEAR_KOKIRI_e, dItemNo_ARMOR_e, dItemNo_WEAR_ZORA_e};
    static u8 m_sword_item[4] = {dItemNo_SWORD_e, dItemNo_MASTER_SWORD_e, dItemNo_LIGHT_SWORD_e, dItemNo_WOOD_STICK_e};
    static u8 m_shield_item[3] = {dItemNo_SHIELD_e, dItemNo_HYLIA_SHIELD_e, dItemNo_WOOD_SHIELD_e};

    mArrowFlag = 0;
    mBombFlag = 0;
    mPachinkoFlag = 0;

    mLife = mLifeCopy = 0;
    mMaxLife = mMaxLifeCopy = 0;
    mMagic = mMagicCopy = 0;
    mMaxMagic = mMaxMagicCopy = 0;
    mOil = mOilCopy = 0;
    mMaxOil = mMaxOilCopy = 0;
    mOxygen = mOxygenCopy = 0;
    mMaxOxygen = mMaxOxygenCopy = 0;
    mEnableMagic = mEnableMagicCopy = false;
    mEnableArrowDrops = mEnableArrowDropsCopy = false;
    mRupees = mRupeesCopy = 0;
    mWalletSize = mWalletSizeCopy = WALLET;
    mKeys = mKeysCopy = 0;
    field_0x23 = field_0x47 = 0;
    mEnableOxygenMeter = mEnableOxygenMeterCopy = false;
    mPoes = mPoesCopy = 0;
    mDonationAmount = mDonationAmountCopy = 0;

    for (int i = 0; i < 24; i++) {
        mItems[i] = mItemsCopy[i] = dItemNo_NONE_e;
        field_0x7a[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        mBottleNum[i] = mBottleNumCopy[i] = 0;
    }

    for (int i = 0; i < 3; i++) {
        mBombNum[i] = mBombNumCopy[i] = 0;
    }

    mScent = mScentCopy = dItemNo_NONE_e;
    field_0xa8 = 0;
    field_0xea = field_0xf4 = 0;

    for (int i = 0; i < 4; i++) {
        mSwords[i] = mSwordsCopy[i] = dComIfGs_isItemFirstBit(m_sword_item[i]) ? 1 : 0;
    }

    for (int i = 0; i < 3; i++) {
        mShields[i] = mShieldsCopy[i] = dComIfGs_isItemFirstBit(m_shield_item[i]) ? 1 : 0;
        mClothes[i] = mClothesCopy[i] = dComIfGs_isItemFirstBit(m_cloth_item[i]) ? 1 : 0;
    }

    mBombBagLV2 = mBombBagLV2Copy = false;

    for (int i = 0; i < 3; i++) {
        mLV2KeyShards[i] = mLV2KeyShardsCopy[i] = false;
    }

    for (int i = 0; i < 2; i++) {
        mLV5Item[i] = mLV5ItemCopy[i] = false;
    }

    for (int i = 0; i < 24; i++) {
        mGoldBug[i] = mGoldBugCopy[i] = false;
    }

    for (int i = 0; i < 8; i++) {
        field_0xf9[i] = field_0x101[i] = 0;
        field_0x109[i] = field_0x111[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        mFusedShadow[i] = mFusedShadowCopy[i] = false;
    }

    for (int i = 0; i < 4; i++) {
        mMirrorShard[i] = mMirrorShardCopy[i] = false;
    }
}

#if DEBUG
void dMw_HIO_c::genMessage(JORMContext* mctx) {
    // "*****Player Status*****"
    mctx->genLabel("\n*****プレイヤーの状態*****", 0);
    mctx->genCheckBox("魔法使える", &mEnableMagic, 1);
    mctx->genCheckBox("矢補給", &mEnableArrowDrops, 1);
    mctx->genCheckBox("矢の数無限", &mArrowFlag, 1);
    mctx->genCheckBox("パチンコ玉の数無限", &mPachinkoFlag, 1);
    mctx->genCheckBox("爆弾の数無限", &mBombFlag, 1);
    mctx->genSlider("ライフ", (s16*)&mLife, 0, 80);
    mctx->genSlider("ライフ最大", (s16*)&mMaxLife, 0, 100);
    mctx->genSlider("魔法メーター", &mMagic, 0, 32);
    mctx->genSlider("魔法メーター最大", &mMaxMagic, 0, 32);
    mctx->genSlider("カンテラ油", &mOil, 0, 100000);
    mctx->genSlider("カンテラ油最大", &mMaxOil, 0, 100000);
    mctx->genCheckBox("酸素メーターＯＮ", &mEnableOxygenMeter, 1);
    mctx->genSlider("酸素メーター", &mOxygen, 0, 100000);
    mctx->genSlider("酸素メーター最大", &mMaxOxygen, 0, 100000);
    mctx->genSlider("ルピ−", &mRupees, 0, 5000);
    mctx->genSlider("財布", &mWalletSize, 0, 2);
    mctx->genSlider("鍵", &mKeys, 0, 99);
    mctx->genSlider("ゴースト", &mPoes, 0, 60);
    mctx->genSlider("お布施総額", &mDonationAmount, 0, 10000);
    mctx->genCheckBox("爆弾袋ＬＶ２", &mBombBagLV2, 1);

    // "*****Item*****"
    mctx->genLabel("\n*****アイテム*****", 0);
    mctx->startComboBox("風のロッド", &mItems[0], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("風のロッド", dItemNo_BOOMERANG_e);
    mctx->endComboBox();

    mctx->startComboBox("カンテラ", &mItems[1], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("カンテラ", dItemNo_KANTERA_e);
    mctx->endComboBox();

    mctx->startComboBox("スピナー", &mItems[2], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("スピナー", dItemNo_SPINNER_e);
    mctx->endComboBox();

    mctx->startComboBox("アイアンブーツ", &mItems[3], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("アイアンブーツ", dItemNo_HVY_BOOTS_e);
    mctx->endComboBox();

    mctx->startComboBox("弓矢", &mItems[4], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("弓矢", dItemNo_BOW_e);
    mctx->genComboBoxItem("光の矢", dItemNo_LIGHT_ARROW_e);
    mctx->genComboBoxItem("ホークアイ＋矢", dItemNo_HAWK_ARROW_e);
    mctx->genComboBoxItem("爆弾＋矢", dItemNo_BOMB_ARROW_e);
    mctx->endComboBox();

    mctx->genSlider("矢の数", &mArrowNum, 0, 100);

    mctx->startComboBox("ホークアイ", &mItems[5], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("ホークアイ", dItemNo_HAWK_EYE_e);
    mctx->endComboBox();

    mctx->startComboBox("鉄球", &mItems[6], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("鉄球", dItemNo_IRONBALL_e);
    mctx->endComboBox();

    mctx->startComboBox("ミラーシールド", &mItems[7], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->endComboBox();

    mctx->startComboBox("コピーロッド", &mItems[8], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("コピーロッド", dItemNo_COPY_ROD_e);
    mctx->endComboBox();

    mctx->startComboBox("フックショット", &mItems[9], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("フックショット", dItemNo_HOOKSHOT_e);
    mctx->endComboBox();

    mctx->startComboBox("Ｗフックショット", &mItems[10], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("Ｗフックショット", dItemNo_W_HOOKSHOT_e);
    mctx->endComboBox();

    for (int i = 0; i < 4; i = i + 1) {
        char sp30[8];
        sprintf(sp30, "ビン%d", i + 1);
        mctx->startComboBox(sp30, &mItems[11 + i], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
        mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
        mctx->genComboBoxItem("空きビン", dItemNo_EMPTY_BOTTLE_e);
        mctx->genComboBoxItem("赤い薬入りビン", dItemNo_RED_BOTTLE_e);
        mctx->genComboBoxItem("緑の薬入りビン", dItemNo_GREEN_BOTTLE_e);
        mctx->genComboBoxItem("青い薬入りビン", dItemNo_BLUE_BOTTLE_e);
        mctx->genComboBoxItem("水入りビン", dItemNo_WATER_BOTTLE_e);
        mctx->genComboBoxItem("妖精入りビン", dItemNo_FAIRY_e);
        mctx->genComboBoxItem("牛乳入りビン", dItemNo_MILK_BOTTLE_e);
        mctx->genComboBoxItem("牛乳半分入りビン", dItemNo_HALF_MILK_BOTTLE_e);
        mctx->genComboBoxItem("油入りビン", dItemNo_OIL_BOTTLE_e);
        mctx->genComboBoxItem("まずいスープ", dItemNo_UGLY_SOUP_e);
        mctx->genComboBoxItem("温泉水入りビン", dItemNo_HOT_SPRING_e);
        mctx->genComboBoxItem("最初にもらうスープ", dItemNo_LV1_SOUP_e);
        mctx->genComboBoxItem("材料が１つ入ったスープ", dItemNo_LV2_SOUP_e);
        mctx->genComboBoxItem("完成したスープ", dItemNo_LV3_SOUP_e);
        mctx->genComboBoxItem("ポウの炎１", dItemNo_POU_FIRE1_e);
        mctx->genComboBoxItem("ポウの炎２", dItemNo_POU_FIRE2_e);
        mctx->genComboBoxItem("ポウの炎３", dItemNo_POU_FIRE3_e);
        mctx->genComboBoxItem("ポウの炎４", dItemNo_POU_FIRE4_e);
        mctx->genComboBoxItem("チュチュゼリー赤", dItemNo_CHUCHU_RED_e);
        mctx->genComboBoxItem("チュチュゼリー青", dItemNo_CHUCHU_BLUE_e);
        mctx->genComboBoxItem("チュチュゼリー緑", dItemNo_CHUCHU_GREEN_e);
        mctx->genComboBoxItem("チュチュゼリー黄", dItemNo_CHUCHU_YELLOW_e);
        mctx->genComboBoxItem("チュチュゼリー紫", dItemNo_CHUCHU_PURPLE_e);
        mctx->genComboBoxItem("チュチュゼリーレア", dItemNo_CHUCHU_RARE_e);
        mctx->genComboBoxItem("チュチュゼリー黒", dItemNo_CHUCHU_BLACK_e);
        mctx->genComboBoxItem("妖精", dItemNo_FAIRY_e);
        mctx->genComboBoxItem("カブトムシ♂", dItemNo_M_BEETLE_e);
        mctx->genComboBoxItem("カブトムシ♀", dItemNo_F_BEETLE_e);
        mctx->genComboBoxItem("チョウチョ♂", dItemNo_M_BUTTERFLY_e);
        mctx->genComboBoxItem("チョウチョ♀", dItemNo_F_BUTTERFLY_e);
        mctx->genComboBoxItem("クワガタ♂", dItemNo_M_STAG_BEETLE_e);
        mctx->genComboBoxItem("クワガタ♀", dItemNo_F_STAG_BEETLE_e);
        mctx->genComboBoxItem("バッタ♂", dItemNo_M_GRASSHOPPER_e);
        mctx->genComboBoxItem("バッタ♀", dItemNo_F_GRASSHOPPER_e);
        mctx->genComboBoxItem("ナナフシ♂", dItemNo_M_NANAFUSHI_e);
        mctx->genComboBoxItem("ナナフシ♀", dItemNo_F_NANAFUSHI_e);
        mctx->genComboBoxItem("ダンゴムシ♂", dItemNo_M_DANGOMUSHI_e);
        mctx->genComboBoxItem("ダンゴムシ♀", dItemNo_F_DANGOMUSHI_e);
        mctx->genComboBoxItem("カマキリ♂", dItemNo_M_MANTIS_e);
        mctx->genComboBoxItem("カマキリ♀", dItemNo_F_MANTIS_e);
        mctx->genComboBoxItem("テントウムシ♂", dItemNo_M_LADYBUG_e);
        mctx->genComboBoxItem("テントウムシ♀", dItemNo_F_LADYBUG_e);
        mctx->genComboBoxItem("カタツムリ♂", dItemNo_M_SNAIL_e);
        mctx->genComboBoxItem("カタツムリ♀", dItemNo_F_SNAIL_e);
        mctx->genComboBoxItem("トンボ♂", dItemNo_M_DRAGONFLY_e);
        mctx->genComboBoxItem("トンボ♀", dItemNo_F_DRAGONFLY_e);
        mctx->genComboBoxItem("アリ♂", dItemNo_M_ANT_e);
        mctx->genComboBoxItem("アリ♀", dItemNo_F_ANT_e);
        mctx->genComboBoxItem("カゲロウ♂", dItemNo_M_MAYFLY_e);
        mctx->genComboBoxItem("カゲロウ♀", dItemNo_F_MAYFLY_e);
        mctx->genComboBoxItem("蜂の子", dItemNo_BEE_CHILD_e);
        mctx->genComboBoxItem("みみず", dItemNo_WORM_e);
        mctx->genComboBoxItem("大妖精の滴", dItemNo_FAIRY_DROP_e);
        mctx->endComboBox();

        mctx->genSlider("アイテムの数", &mBottleNum[i], 0, 10);
    }

    for (int i = 0; i < 3; i = i + 1) {
        char sp38[8];
        sprintf(sp38, "爆弾袋%d", i + 1);
        mctx->startComboBox(sp38, &mItems[15 + i], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
        mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
        mctx->genComboBoxItem("爆弾袋", dItemNo_BOMB_BAG_LV1_e);
        mctx->genComboBoxItem("ノーマル爆弾", dItemNo_NORMAL_BOMB_e);
        mctx->genComboBoxItem("水中爆弾", dItemNo_WATER_BOMB_e);
        mctx->genComboBoxItem("ポケ虫", dItemNo_POKE_BOMB_e);
        mctx->endComboBox();

        mctx->genSlider("爆弾の数", &mBombNum[i], 0, 99);
    }

    mctx->startComboBox("天空人", &mItems[18], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("天空人", dItemNo_DUNGEON_EXIT_e);
    mctx->genComboBoxItem("天空人の息子", dItemNo_DUNGEON_BACK_e);
    mctx->genComboBoxItem("天空人の書置き", dItemNo_TKS_LETTER_e);
    mctx->genComboBoxItem("再開時の天空人", dItemNo_DUNGEON_EXIT_2_e);
    mctx->endComboBox();

    mctx->startComboBox("メモ", &mItems[19], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("ラフレルのメモ", dItemNo_RAFRELS_MEMO_e);
    mctx->genComboBoxItem("アッシュの走り書き", dItemNo_ASHS_SCRIBBLING_e);
    mctx->endComboBox();

    mctx->startComboBox("釣竿", &mItems[20], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("珊瑚の耳飾り", dItemNo_ZORAS_JEWEL_e);
    mctx->genComboBoxItem("釣竿", dItemNo_FISHING_ROD_1_e);
    mctx->genComboBoxItem("釣竿＋蜂の子", dItemNo_BEE_ROD_e);
    mctx->genComboBoxItem("釣竿＋みみず", dItemNo_WORM_ROD_e);
    mctx->genComboBoxItem("珊瑚＋釣竿", dItemNo_JEWEL_ROD_e);
    mctx->genComboBoxItem("珊瑚＋釣竿＋蜂の子", dItemNo_JEWEL_BEE_ROD_e);
    mctx->genComboBoxItem("珊瑚＋釣竿＋みみず", dItemNo_JEWEL_WORM_ROD_e);
    mctx->endComboBox();

    mctx->startComboBox("わらしべアイテム", &mItems[21], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("レナードの手紙", dItemNo_LETTER_e);
    mctx->genComboBoxItem("請求書", dItemNo_BILL_e);
    mctx->genComboBoxItem("木彫りの像", dItemNo_WOOD_STATUE_e);
    mctx->genComboBoxItem("イリアのペンダント", dItemNo_IRIAS_PENDANT_e);
    mctx->genComboBoxItem("馬笛", dItemNo_HORSE_FLUTE_e);
    mctx->endComboBox();

    mctx->startComboBox("古文書", &mItems[22], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("天空の古文書", dItemNo_ANCIENT_DOCUMENT_e);
    mctx->genComboBoxItem("天空文字", dItemNo_AIR_LETTER_e);
    mctx->genComboBoxItem("解封の古文書", dItemNo_ANCIENT_DOCUMENT2_e);
    mctx->endComboBox();

    char sp40[8];
    sprintf(sp40, "パチンコ");
    mctx->startComboBox(sp40, &mItems[23], 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("未装備", dItemNo_NONE_e);
    mctx->genComboBoxItem("パチンコ", dItemNo_PACHINKO_e);
    mctx->endComboBox();

    mctx->genSlider("パチンコ玉の数", &mSeedNum, 0, 50);

    // "*****Collect*****"
    mctx->genLabel("\n*****コレクト*****", 0);
    mctx->genCheckBox("コキリの服", &mClothes[0], 1);
    mctx->genCheckBox("装甲服", &mClothes[1], 1);
    mctx->genCheckBox("ゾーラの服", &mClothes[2], 1);
    mctx->genCheckBox("木刀", &mSwords[3], 1);
    mctx->genCheckBox("金剛の剣", &mSwords[0], 1);
    mctx->genCheckBox("マスターソード", &mSwords[1], 1);
    mctx->genCheckBox("ライトニングソード", &mSwords[2], 1);
    mctx->genCheckBox("木彫りの盾", &mShields[2], 1);
    mctx->genCheckBox("木の盾", &mShields[0], 1);
    mctx->genCheckBox("ハイリアの盾", &mShields[1], 1);
    mctx->genCheckBox("LV2ボス鍵かけら1", &mLV2KeyShards[0], 1);
    mctx->genCheckBox("LV2ボス鍵かけら2", &mLV2KeyShards[1], 1);
    mctx->genCheckBox("LV2ボス鍵かけら3", &mLV2KeyShards[2], 1);
    mctx->genCheckBox("かぼちゃ", &mLV5Item[0], 1);
    mctx->genCheckBox("トアル産チーズ", &mLV5Item[1], 1);

    mctx->startComboBox("におい", &mScent, 0, NULL, 0xFFFF, 0xFFFF, 256, 26);
    mctx->genComboBoxItem("覚えてない", dItemNo_NONE_e);
    mctx->genComboBoxItem("かぼちゃのにおい（削除）", dItemNo_SMELL_PUMPKIN_e);
    mctx->genComboBoxItem("イリアのポーチのにおい", dItemNo_SMELL_YELIA_POUCH_e);
    mctx->genComboBoxItem("魚のにおい", dItemNo_SMELL_FISH_e);
    mctx->genComboBoxItem("ポウのにおい", dItemNo_SMELL_POH_e);
    mctx->genComboBoxItem("子供のにおい", dItemNo_SMELL_CHILDREN_e);
    mctx->genComboBoxItem("薬品のにおい", dItemNo_SMELL_MEDICINE_e);
    mctx->endComboBox();

    for (int i = 0; i < 24; i = i + 1) {
        static const char* insect_name[] = {
            "カブトムシ♂",   "カブトムシ♀",
            "チョウチョ♂",   "チョウチョ♀",
            "クワガタ♂",     "クワガタ♀",
            "バッタ♂",       "バッタ♀",
            "ナナフシ♂",     "ナナフシ♀",
            "ダンゴムシ♂",   "ダンゴムシ♀",
            "カマキリ♂",     "カマキリ♀",
            "テントウムシ♂", "テントウムシ♀",
            "カタツムリ♂",   "カタツムリ♀",
            "トンボ♂",       "トンボ♀",
            "アリ♂",         "アリ♀",
            "カゲロウ♂",     "カゲロウ♀",
        };

        mctx->genCheckBox(insect_name[i], &mGoldBug[i], 1);
    }

    // "*****Fused Shadows*****"
    mctx->genLabel("\n*****影の水晶石*****", 0);
    char sp28[24];
    for (int i = 0; i < 4; i = i + 1) {
        sprintf(sp28, "影の水晶石%d", i + 1);
        mctx->genCheckBox(sp28, &mFusedShadow[i], 1);
    }

    // "*****Mirror Shards*****"
    mctx->genLabel("\n*****陰りの鏡*****", 0);
    for (int i = 0; i < 4; i = i + 1) {
        sprintf(sp28, "陰りの鏡%d", i + 1);
        mctx->genCheckBox(sp28, &mMirrorShard[i], 1);
    }
}

void dMw_HIO_c::update() {
    JORMContext* mctx = attachJORMContext(8);
    mctx->startUpdateNode(this);
    mctx->updateCheckBox(2, &mArrowFlag, 1, 0);
    mctx->updateCheckBox(2, &mBombFlag, 1, 0);
    mctx->endUpdateNode();
    releaseJORMContext(mctx);
}

void dMw_HIO_c::playerStatusHIO() {
    JORMContext* mctx = attachJORMContext(8);

    if (mLife != mLifeCopy) {
        s16 sp10 = mLife - dComIfGs_getLife();
        dComIfGp_clearItemLifeCount();
        dComIfGp_setItemLifeCount(sp10, 0);
        mLifeCopy = mLife;
    } else if (mLifeCopy != dComIfGs_getLife()) {
        mLife = mLifeCopy = dComIfGs_getLife();
        mctx->updateSlider(2, &mLife, 0, 0, 0);
    }

    if (mMaxLifeCopy != mMaxLife) {
        s16 spE = mMaxLife - dComIfGs_getMaxLife();
        dComIfGp_clearItemMaxLifeCount();
        dComIfGp_setItemMaxLifeCount(spE);
        mMaxLifeCopy = mMaxLife;
    } else if (mMaxLifeCopy != dComIfGs_getMaxLife()) {
        mMaxLife = mMaxLifeCopy = dComIfGs_getMaxLife();
        mctx->updateSlider(2, &mMaxLife, 0, 0, 0);
    }

    if (mMagicCopy != mMagic) {
        s16 spC = mMagic - dComIfGs_getMagic();
        dComIfGp_setItemMagicCount(spC);
        mMagicCopy = mMagic;
    } else if (mMagicCopy != dComIfGs_getMagic()) {
        mMagic = mMagicCopy = dComIfGs_getMagic();
        mctx->updateSlider(2, &mMagic, 0, 0, 0);
    }

    if (mMaxMagicCopy != mMaxMagic) {
        s16 spB = mMaxMagic - dComIfGs_getMaxMagic();
        dComIfGp_setItemMaxMagicCount(spB);
        mMaxMagicCopy = mMaxMagic;
    } else if (mMaxMagicCopy != dComIfGs_getMaxMagic()) {
        mMaxMagic = mMaxMagicCopy = dComIfGs_getMaxMagic();
        mctx->updateSlider(2, &mMaxMagic, 0, 0, 0);
    }

    if (mOilCopy != mOil) {
        s32 sp2C = mOil - dComIfGs_getOil();
        dComIfGp_setItemOilCount(sp2C);
        mOilCopy = mOil;
    } else if (mOilCopy != dComIfGs_getOil()) {
        mOil = mOilCopy = dComIfGs_getOil();
        mctx->updateSlider(2, &mOil, 0, 0, 0);
    }

    if (mMaxOilCopy != mMaxOil) {
        s32 sp30 = mMaxOil - dComIfGs_getMaxOil();
        dComIfGp_setItemMaxOilCount(sp30);
        mMaxOilCopy = mMaxOil;
    } else if (mMaxOilCopy != dComIfGs_getMaxOil()) {
        mMaxOil = mMaxOilCopy = dComIfGs_getMaxOil();
        mctx->updateSlider(2, &mMaxOil, 0, 0, 0);
    }

    if (mEnableOxygenMeterCopy != mEnableOxygenMeter) {
        if (mEnableOxygenMeter) {
            dComIfGp_onOxygenShowFlag();
        } else {
            dComIfGp_offOxygenShowFlag();
        }
        mEnableOxygenMeterCopy = mEnableOxygenMeter;
    } else if (mEnableOxygenMeterCopy != dComIfGp_getOxygenShowFlag()) {
        mEnableOxygenMeter = mEnableOxygenMeterCopy = dComIfGp_getOxygenShowFlag();
        mctx->updateSlider(2, &mEnableOxygenMeter, 0, 0, 0);
    }

    if (mOxygenCopy != mOxygen) {
        s32 sp30 = mOxygen - dComIfGp_getOxygen();
        dComIfGp_setOxygenCount(sp30);
        mOxygenCopy = mOxygen;
    } else if (mOxygenCopy != dComIfGp_getOxygen()) {
        mOxygen = mOxygenCopy = dComIfGp_getOxygen();
        mctx->updateSlider(2, &mOxygen, 0, 0, 0);
    }

    if (mMaxOxygenCopy != mMaxOxygen) {
        s32 sp30 = mMaxOxygen - dComIfGp_getMaxOxygen();
        dComIfGp_setMaxOxygenCount(sp30);
        mMaxOxygenCopy = mMaxOxygen;
    } else if (mMaxOxygenCopy != dComIfGp_getMaxOxygen()) {
        mMaxOxygen = mMaxOxygenCopy = dComIfGp_getMaxOxygen();
        mctx->updateSlider(2, &mMaxOxygen, 0, 0, 0);
    }

    if (mRupeesCopy != mRupees) {
        s32 sp30 = mRupees - dComIfGs_getRupee();
        dComIfGp_clearItemRupeeCount();
        dComIfGp_setItemRupeeCount(sp30);
        mRupeesCopy = mRupees;
    } else if (mRupeesCopy != dComIfGs_getRupee()) {
        mRupees = mRupeesCopy = dComIfGs_getRupee();
        mctx->updateSlider(2, &mRupees, 0, 0, 0);
    }

    if (mWalletSizeCopy != mWalletSize) {
        mWalletSizeCopy = mWalletSize;
        dComIfGs_setWalletSize(mWalletSize);
    } else if (mWalletSizeCopy != (s16)dComIfGs_getWalletSize()) {
        mWalletSize = mWalletSizeCopy = dComIfGs_getWalletSize();
        mctx->updateSlider(2, &mWalletSize, 0, 0, 0);
    }

    if (mKeysCopy != mKeys) {
        s16 sp30 = mKeys - dComIfGs_getKeyNum();
        dComIfGp_clearItemKeyNumCount();
        dComIfGp_setItemKeyNumCount(sp30);
        mKeysCopy = mKeys;
    } else if (mKeysCopy != dComIfGs_getKeyNum()) {
        mKeys = mKeysCopy = dComIfGs_getKeyNum();
        mctx->updateSlider(2, &mKeys, 0, 0, 0);
    }

    if (mPoesCopy != mPoes) {
        dComIfGs_setPohSpiritNum(mPoes);
        mPoesCopy = mPoes;
    } else if (mPoesCopy != dComIfGs_getPohSpiritNum()) {
        mPoes = mPoesCopy = dComIfGs_getPohSpiritNum();
        mctx->updateSlider(2, &mPoes, 0, 0, 0);
    }

    if (mDonationAmountCopy != mDonationAmount) {
        dMsgObject_setOffering(mDonationAmount);
        mDonationAmountCopy = mDonationAmount;
    } else if (mDonationAmountCopy != dMsgObject_getOffering()) {
        mDonationAmount = mDonationAmountCopy = dMsgObject_getOffering();
        mctx->updateSlider(2, &mDonationAmount, 0, 0, 0);
    }

    releaseJORMContext(mctx);
}
#endif

u8 dMw_HIO_c::getBombFlag() {
    #if DEBUG
    if (mDoCPd_c::isConnect(PAD_3)) {
        return mBombFlag;
    } else if (mDoCPd_c::isConnect(PAD_3)) {
        return mBombFlag;
    } else {
        return false;
    }
    #else
    return false;
    #endif
}

u8 dMw_HIO_c::getArrowFlag() {
    #if DEBUG
    if (mDoCPd_c::isConnect(PAD_3)) {
        return mArrowFlag;
    } else {
        return false;
    }
    #else
    return false;
    #endif
}

u8 dMw_HIO_c::getPachinkoFlag() {
    #if DEBUG
    if (mDoCPd_c::isConnect(PAD_3)) {
        return mPachinkoFlag;
    } else {
        return false;
    }
    #else
    return false;
    #endif
}

#if DEBUG
void dMw_HIO_c::itemBitCheck() {
    JORMContext* mctx = attachJORMContext(8);
    u8 var_r29;
    
    if (dComIfGs_isGetMagicUseFlag())
        var_r29 = 1;
    else
        var_r29 = 0;

    if (mEnableMagicCopy != var_r29) {
        mEnableMagicCopy = mEnableMagic;
        mEnableMagic = var_r29;
        mctx->updateSlider(2, &mEnableMagic, 0, 0, 0);
    }

    if (mEnableArrowDrops != mEnableArrowDropsCopy) {
        mEnableArrowDropsCopy = mEnableArrowDrops;
        if (mEnableArrowDrops) {
            dComIfGs_onItemFirstBit(dItemNo_BOW_e);
        } else {
            dComIfGs_offItemFirstBit(dItemNo_BOW_e);
        }
    }

    if (mEnableArrowDrops != (u8)dComIfGs_isItemFirstBit(dItemNo_BOW_e)) {
        mEnableArrowDrops = mEnableArrowDropsCopy = dComIfGs_isItemFirstBit(dItemNo_BOW_e);

        JORMContext* mctx = attachJORMContext(8);
        mctx->startUpdateNode(this);
        mctx->updateCheckBox(2, &mEnableArrowDrops, 1, 0);
        mctx->endUpdateNode();
        releaseJORMContext(mctx);
    }

    for (int i = 0; i < 24; i++) {
        if (mItemsCopy[i] != dComIfGs_getItem((u8)i, false)) {
            field_0x7a[i] = 1;
            mItemsCopy[i] = mItems[i];
            mItems[i] = dComIfGs_getItem((u8)i, false);
            mctx->updateComboBox(2, &mItems[i], 0);
        }
    }

    if (mScentCopy != dComIfGs_getCollectSmell()) {
        field_0xa8 = 1;
        mScentCopy = mScent;
        mScent = dComIfGs_getCollectSmell();
        mctx->updateComboBox(2, &mScent, 0);
    }

    var_r29 = field_0x101[0];
    if (dComIfGs_isCollectClothes(2)) {
        var_r29 = 3;
    } else if (dComIfGs_isCollectClothes(1)) {
        var_r29 = 2;
    } else if (dComIfGs_isCollectClothes(0)) {
        var_r29 = 1;
    } else {
        var_r29 = 0;
    }

    if (field_0x101[0] != var_r29) {
        field_0x101[0] = field_0xf9[0];
        field_0xf9[0] = var_r29;
        mctx->updateComboBox(2, &field_0xf9[0], 0);
    }

    for (int i = 0; i < 4; i++) {
        static u8 m_sword_item[] = {dItemNo_SWORD_e, dItemNo_MASTER_SWORD_e, dItemNo_LIGHT_SWORD_e, dItemNo_WOOD_STICK_e};
        if (dComIfGs_isItemFirstBit(m_sword_item[i]))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mSwordsCopy[i] != var_r29) {
            mSwordsCopy[i] = mSwords[i];
            mSwords[i] = var_r29;
            mctx->updateSlider(2, &mSwords[i], 0, 0, 0);
        }
    }

    for (int i = 0; i < 3; i++) {
        static u8 m_cloth_item[] = {dItemNo_WEAR_KOKIRI_e, dItemNo_ARMOR_e, dItemNo_WEAR_ZORA_e};
        if (dComIfGs_isItemFirstBit(m_cloth_item[i]))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mClothesCopy[i] != var_r29) {
            mClothesCopy[i] = mClothes[i];
            mClothes[i] = var_r29;
            mctx->updateSlider(2, &mClothes[i], 0, 0, 0);
        }

        static u8 m_shield_item[] = {dItemNo_SHIELD_e, dItemNo_HYLIA_SHIELD_e, dItemNo_WOOD_SHIELD_e};
        if (dComIfGs_isItemFirstBit(m_shield_item[i]))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mShieldsCopy[i] != var_r29) {
            mShieldsCopy[i] = mShields[i];
            mShields[i] = var_r29;
            mctx->updateSlider(2, &mShields[i], 0, 0, 0);
        }
    }

    if (dComIfGs_isItemFirstBit(dItemNo_BOMB_BAG_LV2_e))
        var_r29 = 1;
    else
        var_r29 = 0;
    
    if (mBombBagLV2Copy != var_r29) {
        mBombBagLV2Copy = mBombBagLV2;
        mBombBagLV2 = var_r29;
        mctx->updateSlider(2, &mBombBagLV2, 0, 0, 0);
    }

    for (int i = 0; i < 3; i++) {
        static u8 m_lv2_dun_key[] = {dItemNo_L2_KEY_PIECES1_e, dItemNo_L2_KEY_PIECES2_e, dItemNo_L2_KEY_PIECES3_e};
        if (dComIfGs_isItemFirstBit(m_lv2_dun_key[i]))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mLV2KeyShardsCopy[i] != var_r29) {
            mLV2KeyShardsCopy[i] = mLV2KeyShards[i];
            mLV2KeyShards[i] = var_r29;
            mctx->updateSlider(2, &mLV2KeyShards[i], 0, 0, 0);
        }
    }

    for (int i = 0; i < 2; i++) {
        static u8 m_lv5_dun_item[] = {dItemNo_TOMATO_PUREE_e, dItemNo_TASTE_e};
        if (dComIfGs_isItemFirstBit(m_lv5_dun_item[i]))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mLV5ItemCopy[i] != var_r29) {
            mLV5ItemCopy[i] = mLV5Item[i];
            mLV5Item[i] = var_r29;
            mctx->updateSlider(2, &mLV5Item[i], 0, 0, 0);
        }
    }

    for (int i = 0; i < 24; i++) {
        if (dComIfGs_isItemFirstBit(i + dItemNo_M_BEETLE_e))
            var_r29 = 1;
        else
            var_r29 = 0;
        
        if (mGoldBugCopy[i] != var_r29) {
            mGoldBugCopy[i] = mGoldBug[i];
            mGoldBug[i] = var_r29;
            mctx->updateSlider(2, &mGoldBug[i], 0, 0, 0);
        }
    }

    var_r29 = field_0x101[1];
    if (dComIfGs_isCollectSword(1)) {
        var_r29 = 2;
    } else if (dComIfGs_isCollectSword(0)) {
        var_r29 = 1;
    } else if (dComIfGs_isCollectSword(2)) {
        var_r29 = 3;
    } else if (dComIfGs_isCollectSword(3)) {
        var_r29 = 4;
    } else {
        var_r29 = 0;
    }

    if (field_0x101[1] != var_r29) {
        field_0x101[1] = field_0xf9[1];
        field_0xf9[1] = var_r29;
        mctx->updateComboBox(2, &field_0xf9[1], 0);
    }

    var_r29 = field_0x101[2];
    if (dComIfGs_isCollectShield(3)) {
        var_r29 = 4;
    } else if (dComIfGs_isCollectShield(2)) {
        var_r29 = 3;
    } else if (dComIfGs_isCollectShield(1)) {
        var_r29 = 2;
    } else if (dComIfGs_isCollectShield(0)) {
        var_r29 = 1;
    } else {
        var_r29 = 0;
    }

    if (field_0x101[2] != var_r29) {
        field_0x101[2] = field_0xf9[2];
        field_0xf9[2] = var_r29;
        mctx->updateComboBox(2, &field_0xf9[2], 0);
    }

    for (int i = 0; i < 4; i++) {
        if (dComIfGs_isCollectCrystal(i))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mFusedShadowCopy[i] != var_r29) {
            mFusedShadowCopy[i] = mFusedShadow[i];
            mFusedShadow[i] = var_r29;
            mctx->updateSlider(2, &mFusedShadow[i], 0, 0, 0);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (dComIfGs_isCollectMirror(i))
            var_r29 = 1;
        else
            var_r29 = 0;

        if (mMirrorShardCopy[i] != var_r29) {
            mMirrorShardCopy[i] = mMirrorShard[i];
            mMirrorShard[i] = var_r29;
            mctx->updateSlider(2, &mMirrorShard[i], 0, 0, 0);
        }
    }

    releaseJORMContext(mctx);
}

void dMw_HIO_c::itemNumHIO() {
    JORMContext* mctx = attachJORMContext(8);

    if (mArrowNumCopy != mArrowNum) {
        s16 count = mArrowNum - dComIfGs_getArrowNum();
        dComIfGp_setItemArrowNumCount(count);
        mArrowNumCopy = mArrowNum;
    } else if (mArrowNumCopy != dComIfGs_getArrowNum()) {
        mArrowNum = mArrowNumCopy = dComIfGs_getArrowNum();
        mctx->updateSlider(2, &mArrowNum, 0, 0, 0);
    }

    for (int i = 0; i < 4; i++) {
        if (mBottleNumCopy[i] != mBottleNum[i]) {
            s16 count = mBottleNum[i] - dComIfGs_getBottleNum(i);
            dComIfGs_addBottleNum(i, count);
            mBottleNumCopy[i] = mBottleNum[i];
        } else if (mBottleNumCopy[i] != dComIfGs_getBottleNum(i)) {
            mBottleNum[i] = mBottleNumCopy[i] = dComIfGs_getBottleNum(i);
            mctx->updateSlider(2, &mBottleNum[i], 0, 0, 0);
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mBombNumCopy[i] != mBombNum[i]) {
            s16 count = mBombNum[i] - dComIfGs_getBombNum(i);
            dComIfGp_setItemBombNumCount(i, count);
            mBombNumCopy[i] = mBombNum[i];
        } else if (mBombNumCopy[i] != dComIfGs_getBombNum(i)) {
            mBombNum[i] = mBombNumCopy[i] = dComIfGs_getBombNum(i);
            mctx->updateSlider(2, &mBombNum[i], 0, 0, 0);
        }
    }

    if (mSeedNumCopy != mSeedNum) {
        s16 count = mSeedNum - dComIfGs_getPachinkoNum();
        dComIfGp_setItemPachinkoNumCount(count);
        mSeedNumCopy = mSeedNum;
    } else if (mSeedNumCopy != dComIfGs_getPachinkoNum()) {
        mSeedNum = mSeedNumCopy = dComIfGs_getPachinkoNum();
        mctx->updateSlider(2, &mSeedNum, 0, 0, 0);
    }

    releaseJORMContext(mctx);
}

void dMw_HIO_c::itemHIO() {
    playerStatusHIO();
    itemNumHIO();
    itemBitCheck();

    JORMContext* mctx = attachJORMContext(8);

    if (mEnableMagic != mEnableMagicCopy) {
        mEnableMagicCopy = mEnableMagic;
        if (mEnableMagic) {
            dComIfGs_onGetMagicUseFlag();
        } else {
            dComIfGs_offGetMagicUseFlag();
        }
    }

    if (mEnableMagic != (u8)dComIfGs_isGetMagicUseFlag()) {
        mEnableMagic = mEnableMagicCopy = dComIfGs_isGetMagicUseFlag();
        
        JORMContext* mctx = attachJORMContext(8);
        mctx->startUpdateNode(this);
        mctx->updateCheckBox(2, &mEnableMagic, 1, 0);
        mctx->endUpdateNode();
        releaseJORMContext(mctx);
    }

    if (mEnableArrowDrops != mEnableArrowDropsCopy) {
        mEnableArrowDropsCopy = mEnableArrowDrops;
        if (mEnableArrowDrops) {
            dComIfGs_onItemFirstBit(dItemNo_BOW_e);
        } else {
            dComIfGs_offItemFirstBit(dItemNo_BOW_e);
        }
    }

    if (mEnableArrowDrops != (u8)dComIfGs_isItemFirstBit(dItemNo_BOW_e)) {
        mEnableArrowDrops = mEnableArrowDropsCopy = dComIfGs_isItemFirstBit(dItemNo_BOW_e);
        
        JORMContext* mctx = attachJORMContext(8);
        mctx->startUpdateNode(this);
        mctx->updateCheckBox(2, &mEnableArrowDrops, 1, 0);
        mctx->endUpdateNode();
        releaseJORMContext(mctx);
    }

    for (int i = 0; i < 24; i++) {
        if (mItems[i] != mItemsCopy[i]) {
            if (i == 20 && field_0x7a[i] == 0) {
                switch (mItems[i]) {
                case dItemNo_NONE_e:
                case dItemNo_FISHING_ROD_1_e:
                case dItemNo_BEE_ROD_e:
                case dItemNo_WORM_ROD_e:
                    dComIfGs_offItemFirstBit(dItemNo_ZORAS_JEWEL_e);
                    break;
                case dItemNo_JEWEL_ROD_e:
                case dItemNo_JEWEL_BEE_ROD_e:
                case dItemNo_JEWEL_WORM_ROD_e:
                case dItemNo_ZORAS_JEWEL_e:
                    dComIfGs_onItemFirstBit(dItemNo_ZORAS_JEWEL_e);
                    break;
                }
            }

            dComIfGs_setItem((u8)i, mItems[i]);

            if (mItems[i] == dItemNo_HOT_SPRING_e) {
                dMeter2Info_setHotSpringTimer(i);
            }

            if (mItems[i] == dItemNo_BEE_CHILD_e && field_0x7a[i] == 0) {
                dComIfGs_setBottleNum(i - 11, dComIfGs_getBottleMax());
            }

            mItemsCopy[i] = mItems[i];
            field_0x7a[i] = 0;
        }
    }

    if (mScent != mScentCopy) {
        dMsgObject_setSmellType(mScent);
        mScentCopy = mScent;
        field_0xa8 = 0;
    }

    for (int i = 0; i < 4; i++) {
        if (mSwords[i] != mSwordsCopy[i]) {
            static u8 m_sword_item[] = {dItemNo_SWORD_e, dItemNo_MASTER_SWORD_e, dItemNo_LIGHT_SWORD_e, dItemNo_WOOD_STICK_e};
            if (mSwords[i] != 0) {
                dComIfGs_onItemFirstBit(m_sword_item[i]);
            } else {
                dComIfGs_offItemFirstBit(m_sword_item[i]);
                if (m_sword_item[i] == dComIfGs_getSelectEquipSword()) {
                    dMeter2Info_setSword(dItemNo_NONE_e, false);
                }
            }

            mSwordsCopy[i] = mSwords[i];
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mClothes[i] != mClothesCopy[i]) {
            static u8 m_cloth_item[] = {dItemNo_WEAR_KOKIRI_e, dItemNo_ARMOR_e, dItemNo_WEAR_ZORA_e};
            if (mClothes[i] != 0) {
                dComIfGs_onItemFirstBit(m_cloth_item[i]);
            } else {
                dComIfGs_offItemFirstBit(m_cloth_item[i]);

            }
            mClothesCopy[i] = mClothes[i];
        }

        if (mShields[i] != mShieldsCopy[i]) {
            static u8 m_shield_item[] = {dItemNo_SHIELD_e, dItemNo_HYLIA_SHIELD_e, dItemNo_WOOD_SHIELD_e};
            if (mShields[i] != 0) {
                dComIfGs_onItemFirstBit(m_shield_item[i]);
            } else {
                dComIfGs_offItemFirstBit(m_shield_item[i]);

            }
            mShieldsCopy[i] = mShields[i];
        }
    }

    if (mBombBagLV2 != mBombBagLV2Copy) {
        if (mBombBagLV2) {
            dComIfGs_onItemFirstBit(dItemNo_BOMB_BAG_LV2_e);
        } else {
            dComIfGs_offItemFirstBit(dItemNo_BOMB_BAG_LV2_e);
        }
        mBombBagLV2Copy = mBombBagLV2;
    }

    for (int i = 0; i < 3; i++) {
        static u8 m_lv2_dun_key[] = {dItemNo_L2_KEY_PIECES1_e, dItemNo_L2_KEY_PIECES2_e, dItemNo_L2_KEY_PIECES3_e};
        if (mLV2KeyShards[i] != mLV2KeyShardsCopy[i]) {
            if (mLV2KeyShards[i] != 0) {
                dComIfGs_onItemFirstBit(m_lv2_dun_key[i]);
            } else {
                dComIfGs_offItemFirstBit(m_lv2_dun_key[i]);
            }
            mLV2KeyShardsCopy[i] = mLV2KeyShards[i];
        }
    }

    for (int i = 0; i < 2; i++) {
        static u8 m_lv5_dun_item[] = {dItemNo_TOMATO_PUREE_e, dItemNo_TASTE_e};
        if (mLV5Item[i] != mLV5ItemCopy[i]) {
            if (mLV5Item[i] != 0) {
                dComIfGs_onItemFirstBit(m_lv5_dun_item[i]);
            } else {
                dComIfGs_offItemFirstBit(m_lv5_dun_item[i]);
            }
            mLV5ItemCopy[i] = mLV5Item[i];
        }
    }

    for (int i = 0; i < 24; i++) {
        if (mGoldBug[i] != mGoldBugCopy[i]) {
            if (mGoldBug[i] != 0) {
                dComIfGs_onItemFirstBit(i + dItemNo_M_BEETLE_e);
            } else {
                dComIfGs_offItemFirstBit(i + dItemNo_M_BEETLE_e);
            }
            mGoldBugCopy[i] = mGoldBug[i];
        }
    }

    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            if (g_playerKind == 2) {
                field_0xf9[i] = 0;
            } else if (g_playerKind == 3) {
                field_0xf9[i] = 1;
            } else if (g_playerKind == 4) {
                field_0xf9[i] = 2;
            }
        }

        if (field_0xf9[i] != field_0x101[i]) {
            switch (i) {
            case 0:
                for (int j = 0; j < 8; j++) {
                    dComIfGs_offCollectClothes(j);
                }

                if (field_0xf9[i] != 0) {
                    dComIfGs_setCollectClothes(field_0xf9[i] - 1);
                } else {
                    dComIfGs_setSelectEquipClothes(dItemNo_WEAR_CASUAL_e);
                }
                break;
            case 1:
                for (int j = 0; j < 8; j++) {
                    dComIfGs_offCollectSword(j);
                }

                if (field_0xf9[i] != 0) {
                    dComIfGs_setCollectSword(field_0xf9[i] - 1);
                } else {
                    dComIfGs_setSelectEquipSword(dItemNo_NONE_e);
                }
                break;
            case 2:
                for (int j = 0; j < 8; j++) {
                    dComIfGs_offCollectShield(j);
                }

                if (field_0xf9[i] != 0) {
                    dComIfGs_setCollectShield(field_0xf9[i] - 1);
                } else {
                    dComIfGs_setSelectEquipShield(dItemNo_NONE_e);
                }
                break;
            }

            field_0x101[i] = field_0xf9[i];
        }
    }

    for (int i = 0; i < 4; i++) {
        if (mFusedShadow[i] != mFusedShadowCopy[i]) {
            if (mFusedShadow[i] != 0) {
                dComIfGs_onCollectCrystal(i);
            } else {
                dComIfGs_offCollectCrystal(i);
            }
            mFusedShadowCopy[i] = mFusedShadow[i];
        }
    }

    for (int i = 0; i < 4; i++) {
        if (mMirrorShard[i] != mMirrorShardCopy[i]) {
            if (mMirrorShard[i] != 0) {
                dComIfGs_onCollectMirror(i);
            } else {
                dComIfGs_offCollectMirror(i);
            }
            mMirrorShardCopy[i] = mMirrorShard[i];
        }
    }

    releaseJORMContext(mctx);
}
#endif

dMw_DHIO_c::dMw_DHIO_c() {
    for (int i = 0; i < 32; i++) {
        mMap[i] = false;
        mCompass[i] = false;
        mBossKey[i] = false;
        mBossClear[i] = false;
        mHeartContainer[i] = false;
        mBossDemo[i] = false;
        mPortal[i] = false;
        mMidBossClear[i] = false;
        mMapCopy[i] = false;
        mCompassCopy[i] = false;
        mBossKeyCopy[i] = false;
        mBossClearCopy[i] = false;
        mHeartContainerCopy[i] = false;
        mBossDemoCopy[i] = false;
        mPortalCopy[i] = false;
        mMidBossClearCopy[i] = false;
    }
}

#if DEBUG
void dMw_DHIO_c::genMessage(JORMContext* mctx) {
    static const char* name_str[] = {
        "\n*****0 ラトアーヌ地方*****",
        "\n*****1 ハイラル城*****",
        "\n*****2 フィローネ地方*****",
        "\n*****3 オルディン地方*****",
        "\n*****4 ラネール地方*****",
        "\n*****5 影の世界*****",
        "\n*****6 環状フィールド*****",
        "\n*****7 空き*****",
        "\n*****8 空き*****",
        "\n*****9 空き*****",
        "\n*****10 空き*****",
        "\n*****11 空き*****",
        "\n*****12 空き*****",
        "\n*****13 空き*****",
        "\n*****14 空き*****",
        "\n*****15 空き*****",
        "\n*****16 L-1ダンジョン*****",
        "\n*****17 L-2ダンジョン*****",
        "\n*****18 L-3ダンジョン*****",
        "\n*****19 L-4ダンジョン*****",
        "\n*****20 L-5ダンジョン*****",
        "\n*****21 L-6ダンジョン*****",
        "\n*****22 L-7ダンジョン*****",
        "\n*****23 L-8ダンジョン*****",
        "\n*****24 最終ダンジョン*****",
        "\n*****25 空き*****",
        "\n*****26 空き*****",
        "\n*****27 空き*****",
        "\n*****28 空き*****",
        "\n*****29 空き*****",
        "\n*****30 空き*****",
        "\n*****31 デフォルト*****",
    };

    for (int i = 16; i < 25; i++) {
        mctx->genLabel(name_str[i], 0);
        mctx->genCheckBox("マップ", &mMap[i], 1);
        mctx->genCheckBox("コンパス", &mCompass[i], 1);
        mctx->genCheckBox("ボス鍵", &mBossKey[i], 1);
        mctx->genCheckBox("ワープ", &mPortal[i], 1);
        mctx->genCheckBox("中ボス攻略", &mMidBossClear[i], 1);
        mctx->genCheckBox("ボス攻略", &mBossClear[i], 1);
        mctx->genCheckBox("ハートの器", &mHeartContainer[i], 1);
        mctx->genCheckBox("ボスデモ", &mBossDemo[i], 1);
    }
}

void dMw_DHIO_c::bitCheck() {
    JORMContext* mctx = attachJORMContext(8);
    u8 var_r28;

    for (int i = 0; i < 32; i++) {
        if (dComIfGs_isDungeonItemMap(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mMapCopy[i] != var_r28) {
            mMapCopy[i] = mMap[i];
            mMap[i] = var_r28;
            mctx->updateSlider(2, &mMap[i], 0, 0, 0);
        }

        if (mMap[i] != mMapCopy[i]) {
            if (mMap[i] != 0) {
                dComIfGs_onDungeonItemMap(i);
            } else {
                dComIfGs_offDungeonItemMap(i);
            }
            mMapCopy[i] = mMap[i];
        }

        if (dComIfGs_isDungeonItemCompass(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mCompassCopy[i] != var_r28) {
            mCompassCopy[i] = mCompass[i];
            mCompass[i] = var_r28;
            mctx->updateSlider(2, &mCompass[i], 0, 0, 0);
        }

        if (mCompass[i] != mCompassCopy[i]) {
            if (mCompass[i] != 0) {
                dComIfGs_onDungeonItemCompass(i);
            } else {
                dComIfGs_offDungeonItemCompass(i);
            }
            mCompassCopy[i] = mCompass[i];
        }

        if (dComIfGs_isDungeonItemBossKey(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mBossKeyCopy[i] != var_r28) {
            mBossKeyCopy[i] = mBossKey[i];
            mBossKey[i] = var_r28;
            mctx->updateSlider(2, &mBossKey[i], 0, 0, 0);
        }

        if (mBossKey[i] != mBossKeyCopy[i]) {
            if (mBossKey[i] != 0) {
                dComIfGs_onDungeonItemBossKey(i);
            } else {
                dComIfGs_offDungeonItemBossKey(i);
            }
            mBossKeyCopy[i] = mBossKey[i];
        }

        if (dComIfGs_isDungeonItemWarp(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mPortalCopy[i] != var_r28) {
            mPortalCopy[i] = mPortal[i];
            mPortal[i] = var_r28;
            mctx->updateSlider(2, &mPortal[i], 0, 0, 0);
        }

        if (mPortal[i] != mPortalCopy[i]) {
            if (mPortal[i] != 0) {
                dComIfGs_onDungeonItemWarp(i);
            } else {
                dComIfGs_offDungeonItemWarp(i);
            }
            mPortalCopy[i] = mPortal[i];
        }

        if (dComIfGs_isStageBossEnemy(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mBossClearCopy[i] != var_r28) {
            mBossClearCopy[i] = mBossClear[i];
            mBossClear[i] = var_r28;
            mctx->updateSlider(2, &mBossClear[i], 0, 0, 0);
        }

        if (mBossClear[i] != mBossClearCopy[i]) {
            if (mBossClear[i] != 0) {
                dComIfGs_onStageBossEnemy(i);
            } else {
                dComIfGs_offStageBossEnemy(i);
            }
            mBossClearCopy[i] = mBossClear[i];
        }

        if (dComIfGs_isStageMiddleBoss(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mMidBossClearCopy[i] != var_r28) {
            mMidBossClearCopy[i] = mMidBossClear[i];
            mMidBossClear[i] = var_r28;
            mctx->updateSlider(2, &mMidBossClear[i], 0, 0, 0);
        }

        if (mMidBossClear[i] != mMidBossClearCopy[i]) {
            if (mMidBossClear[i] != 0) {
                dComIfGs_onStageMiddleBoss(i);
            } else {
                dComIfGs_offStageMiddleBoss(i);
            }
            mMidBossClearCopy[i] = mMidBossClear[i];
        }

        if (dComIfGs_isStageLife(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mHeartContainerCopy[i] != var_r28) {
            mHeartContainerCopy[i] = mHeartContainer[i];
            mHeartContainer[i] = var_r28;
            mctx->updateSlider(2, &mHeartContainer[i], 0, 0, 0);
        }

        if (mHeartContainer[i] != mHeartContainerCopy[i]) {
            if (mHeartContainer[i] != 0) {
                dComIfGs_onStageLife(i);
            } else {
                dComIfGs_offStageLife(i);
            }
            mHeartContainerCopy[i] = mHeartContainer[i];
        }

        if (dComIfGs_isStageBossDemo(i))
            var_r28 = 1;
        else
            var_r28 = 0;

        if (mBossDemoCopy[i] != var_r28) {
            mBossDemoCopy[i] = mBossDemo[i];
            mBossDemo[i] = var_r28;
            mctx->updateSlider(2, &mBossDemo[i], 0, 0, 0);
        }

        if (mBossDemo[i] != mBossDemoCopy[i]) {
            if (mBossDemo[i] != 0) {
                dComIfGs_onStageBossDemo(i);
            } else {
                dComIfGs_offStageBossDemo(i);
            }
            mBossDemoCopy[i] = mBossDemo[i];
        }
    }

    releaseJORMContext(mctx);
}
#endif

dMw_HIO_c g_mwHIO;

dMw_DHIO_c g_mwDHIO;
