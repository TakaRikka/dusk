#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_map_common.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter_HIO.h"
#include "d/d_select_cursor.h"
#include "global.h"

struct map_icon_size_t {
    f32 size_x;
    f32 size_y;
    u8 icon_no;
};

static map_icon_size_t map_icon_size[] = {
    /* ?              */ {1.0f, 1.0f, 0},
    /* ?              */ {1.0f, 1.0f, 1},
    /* Link           */ {40.0f, 40.0f, 17},
    /* Dungeon Warp   */ {40.0f, 40.0f, 2},
    /* Destination    */ {40.0f, 40.0f, 18},
    /* Boss           */ {40.0f, 40.0f, 4},
    /* Boss (Ganon)   */ {40.0f, 40.0f, 3},
    /* Light Drop     */ {40.0f, 40.0f, 5},
    /* Light Ball     */ {40.0f, 40.0f, 6},
    /* Cannon Ball    */ {40.0f, 40.0f, 7},
    /* Yeto           */ {40.0f, 40.0f, 9},
    /* Yeta           */ {40.0f, 40.0f, 10},
    /* Gold Wolf      */ {40.0f, 40.0f, 11},
    /* Monkey         */ {40.0f, 40.0f, 12},
    /* Ooccoo Sr.     */ {40.0f, 40.0f, 15},
    /* Ooccoo Jr.     */ {40.0f, 40.0f, 16},
    /* Copy Statue    */ {40.0f, 40.0f, 8},
    /* Coach          */ {67.5f, 40.0f, 13},
    /* Small Key      */ {40.0f, 40.0f, 14},
    /* Treasure Chest */ {40.0f, 40.0f, 19},
    /* Dungeon Enter  */ {40.0f, 40.0f, 20},
    /* Link Enter     */ {40.0f, 40.0f, 21},
    /* LV8 Warp       */ {40.0f, 40.0f, 22},
};

dMenuMapCommon_c::dMenuMapCommon_c() {
    for (int i = 0; i < ICON_MAX_e; i++) {
        mPictures[i] = NULL;
    }

    mLightDropPic = NULL;
    mpDrawCursor = NULL;
    mpPortalIcon = NULL;
    _6c = 0;
    mBlinkTimer = 0;
    mBlinkAlpha = 0.0f;
    mLightDropFlashTimer = 0;
    _c80 = 0.0f;
    _c7c = 0.0f;
    _c84 = 0.0f;
    _c88 = 0.0f;
    mCenterPosX = 0.0f;
    _c90 = 0;

    clearIconInfo();
}

dMenuMapCommon_c::~dMenuMapCommon_c() {
    for (int i = 0; i < ICON_MAX_e; i++) {
        if (mPictures[i] != NULL) {
            delete mPictures[i];
            mPictures[i] = NULL;
        }
    }

    if (mLightDropPic != NULL) {
        delete mLightDropPic;
        mLightDropPic = NULL;
    }

    if (mpDrawCursor != NULL) {
        delete mpDrawCursor;
        mpDrawCursor = NULL;
    }

    if (mpPortalIcon != NULL) {
        delete mpPortalIcon;
        mpPortalIcon = NULL;
    }
}

void dMenuMapCommon_c::initiate(JKRArchive* i_archive) {
    ResTIMG* mp_image;

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_boss_s_ci8_16_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_BOSS_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_boss_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_BOSS_e]->append(mp_image, 0.0f);
    mPictures[ICON_BOSS_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_BOSS_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_boss_s_ci8_16_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_BOSS_GANON_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_boss_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_BOSS_GANON_e]->append(mp_image, 0.0f);
    mPictures[ICON_BOSS_GANON_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_BOSS_GANON_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_map_icon_enter_ci8_24_02.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_DUNGEON_ENTER_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_map_icon_enter_ci8_02.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_DUNGEON_ENTER_e]->append(mp_image, 0.0f);
    mPictures[ICON_DUNGEON_ENTER_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_DUNGEON_ENTER_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_enter_s_ci8_24_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LINK_ENTER_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_enter_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LINK_ENTER_e]->append(mp_image, 0.0f);
    mPictures[ICON_LINK_ENTER_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_LINK_ENTER_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_map_icon_warp_24_ci8_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LV8_WARP_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_map_icon_warp_32_ci8_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LV8_WARP_e]->append(mp_image, 0.0f);
    mPictures[ICON_LV8_WARP_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_LV8_WARP_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_box_s_ci8_24_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_TREASURE_CHEST_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_box_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_TREASURE_CHEST_e]->append(mp_image, 0.0f);
    mPictures[ICON_TREASURE_CHEST_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_TREASURE_CHEST_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_gray_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_KEY_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_key_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_KEY_e]->append(mp_image, 0.0f);
    mPictures[ICON_KEY_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_KEY_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_YETO_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "st_yuki_M.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_YETO_e]->append(mp_image, 0.0f);
    mPictures[ICON_YETO_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_YETO_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_YETA_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "st_yuki_W.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_YETA_e]->append(mp_image, 0.0f);
    mPictures[ICON_YETA_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_YETA_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_yellow_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_GOLD_WOLF_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "st_gold_wolf.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_GOLD_WOLF_e]->append(mp_image, 0.0f);
    mPictures[ICON_GOLD_WOLF_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_GOLD_WOLF_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_MONKEY_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_monkey_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_MONKEY_e]->append(mp_image, 0.0f);
    mPictures[ICON_MONKEY_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_MONKEY_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_blue_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_OOCCOO_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "ni_obacyan.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_OOCCOO_e]->append(mp_image, 0.0f);
    mPictures[ICON_OOCCOO_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_OOCCOO_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_blue_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_OOCCOO_JR_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "ni_obacyan.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_OOCCOO_JR_e]->append(mp_image, 0.0f);
    mPictures[ICON_OOCCOO_JR_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_OOCCOO_JR_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[8] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_zelda_map_icon_copy_stone_statue_snup_try_00_04.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[8]->append(mp_image, 0.0f);
    mPictures[8]->setBasePosition(J2DBasePosition_4);
    mPictures[8]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_link_s_ci8_24_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LINK_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_link_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LINK_e]->append(mp_image, 0.0f);
    mPictures[ICON_LINK_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_LINK_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_penant_s_ci8_24_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_DUNGEON_WARP_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_penant_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_DUNGEON_WARP_e]->append(mp_image, 0.0f);
    mPictures[ICON_DUNGEON_WARP_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_DUNGEON_WARP_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_black_32.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LIGHT_DROP_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_black_32.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LIGHT_DROP_e]->append(mp_image, 0.0f);
    mPictures[ICON_LIGHT_DROP_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_LIGHT_DROP_e]->setInfluencedAlpha(false, false);
    mPictures[ICON_LIGHT_DROP_e]->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_hikari_no_shizuku_try_10_00_24x24.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mLightDropPic = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_hikari_no_shizuku_try_10_00_24x24.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mLightDropPic->append(mp_image, 0.0f);
    mLightDropPic->setBasePosition(J2DBasePosition_4);
    mLightDropPic->setInfluencedAlpha(false, false);
    mLightDropPic->setBlackWhite(JUtility::TColor(0, 240, 170, 0), JUtility::TColor(255, 255, 230, 255));

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LIGHT_BALL_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_zelda_map_icon_hikari_ball_03.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_LIGHT_BALL_e]->append(mp_image, 0.0f);
    mPictures[ICON_LIGHT_BALL_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_LIGHT_BALL_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_CANNON_BALL_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_map_icon_iron_ball_ci8_32_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_CANNON_BALL_e]->append(mp_image, 0.0f);
    mPictures[ICON_CANNON_BALL_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_CANNON_BALL_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "tt_map_icon_s_size_circle_ci4_00.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_COACH_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_map_icon_basha_ci8.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_COACH_e]->append(mp_image, 0.0f);
    mPictures[ICON_COACH_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_COACH_e]->setInfluencedAlpha(false, false);

    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_nijumaru_40x40_ind_01.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_DESTINATION_e] = new J2DPicture(mp_image);
    mp_image = (ResTIMG*)i_archive->getResource('TIMG', "im_nijumaru_40x40_ind_01.bti");
    JUT_ASSERT(0, mp_image != NULL);
    mPictures[ICON_DESTINATION_e]->append(mp_image, 0.0f);
    mPictures[ICON_DESTINATION_e]->setBasePosition(J2DBasePosition_4);
    mPictures[ICON_DESTINATION_e]->setInfluencedAlpha(false, false);

    mpDrawCursor = new dSelect_cursor_c(4, 1.0f, NULL);
    JUT_ASSERT(0, mpDrawCursor != NULL);

    if (i_archive == dComIfGp_getFmapResArchive()) {
        mpPortalIcon = new dSelect_cursor_c(5, 1.0f, i_archive);
        JUT_ASSERT(0, mpPortalIcon != NULL);
    }
}

void dMenuMapCommon_c::drawIcon(f32 i_posX, f32 i_posY, f32 param_3, f32 param_4) {
    s16 icon_draw_list[128];
    if (g_fmapHIO.mMapIconHIO.mIconDebug) {
        debugIcon();
    }

    if (mpDrawCursor) {
        mpDrawCursor->onUpdateFlag();
    }

    if (mpPortalIcon) {
        mpPortalIcon->onUpdateFlag();
    }

    int icon_draw_num = 0;
    for (int i = 0; i < 128; i++) {
        icon_draw_list[i] = -1;
    }

    for (int i = 22; i >= 0; i--) {
        for (s16 j = 0; j < 128; j++) {
            if (i == mIconInfo[j].icon_no != 0 && mIconInfo[j]._15 != 0) {
                icon_draw_list[icon_draw_num] = j;
                icon_draw_num++;
            }
        }
    }

    for (int i = 0; i < 128; i++) {
        s16 info_idx = icon_draw_list[i];
        if (info_idx != -1) {
            if (mIconInfo[info_idx].icon_no == 0) {
                if (mpDrawCursor != NULL) {
                    f32 icon_pos_x = mIconInfo[info_idx].pos_x;
                    f32 icon_pos_y = mIconInfo[info_idx].pos_y;

                    if (mpDrawCursor->getUpdateFlag() == 1) {
                        if (mIconInfo[info_idx]._15 == 2) {
                            mpDrawCursor->setAlphaRate(1.0f);
                        } else if (mIconInfo[info_idx]._15 == 3) {
                            mpDrawCursor->setAlphaRate(mBlinkAlpha);
                        } else {
                            mpDrawCursor->setAlphaRate(param_4 * (param_3 * mIconInfo[info_idx].alpha_rate));
                        }
                    }

                    f32 pos_x = icon_pos_x + i_posX;
                    mpDrawCursor->setPos(pos_x, icon_pos_y + i_posY);
                    mpDrawCursor->setScale(mIconInfo[info_idx].scale * g_fmapHIO.mMapIconHIO.mPortalCursorScale);
                    mpDrawCursor->draw();
                    mpDrawCursor->resetUpdateFlag();
                }
            } else if (mIconInfo[info_idx].icon_no == 1) {
                if (mpPortalIcon != NULL) {
                    f32 icon_pos_x = mIconInfo[info_idx].pos_x;
                    f32 icon_pos_y = mIconInfo[info_idx].pos_y;

                    if (mpPortalIcon->getUpdateFlag() == 1) {
                        if (mIconInfo[info_idx]._15 == 2) {
                            mpPortalIcon->setAlphaRate(1.0f);
                        } else if (mIconInfo[info_idx]._15 == 3) {
                            mpPortalIcon->setAlphaRate(mBlinkAlpha);
                        } else {
                            mpPortalIcon->setAlphaRate(param_3 * mIconInfo[info_idx].alpha_rate);
                        }
                    }

                    f32 pos_x = (icon_pos_x + i_posX);
                    mpPortalIcon->setPos(pos_x, icon_pos_y + i_posY);
                    mpPortalIcon->setScale(mIconInfo[info_idx].scale * g_fmapHIO.mMapIconHIO.mPortalIconScale);
                    mpPortalIcon->draw();
                    mpPortalIcon->resetUpdateFlag();
                }
            } else if (mPictures[mIconInfo[info_idx].icon_no] != NULL) {
                f32 icon_pos_x = mIconInfo[info_idx].pos_x;
                f32 icon_pos_y = mIconInfo[info_idx].pos_y;
                f32 icon_size_x = getIconSizeX(mIconInfo[info_idx].icon_no);
                f32 icon_size_y = getIconSizeY(mIconInfo[info_idx].icon_no);

                f32 var_f29 = (4.0f / 7.0f) * getIconSizeX(mIconInfo[info_idx].icon_no);
                f32 var_f28 = (4.0f / 7.0f) * getIconSizeY(mIconInfo[info_idx].icon_no);

                if (mIconInfo[info_idx].icon_no == ICON_LIGHT_DROP_e) {
                    icon_size_x *= _c7c;
                    icon_size_y *= _c7c;
                }

                mPictures[mIconInfo[info_idx].icon_no]->rotate(icon_size_x / 2, icon_size_y / 2, ROTATE_Z,
                                                        mIconInfo[info_idx].rotation);

                if (mIconInfo[info_idx].icon_no == ICON_LIGHT_DROP_e) {
                    mPictures[mIconInfo[info_idx].icon_no]->setAlpha((180.0f * _c80) / 255.0f);
                    mLightDropPic->rotate(var_f29 / 2, var_f28 / 2, ROTATE_Z, mIconInfo[info_idx].rotation);
                    mLightDropPic->setAlpha(_c80);
                } else if (mIconInfo[info_idx]._15 == 2) {
                    mPictures[mIconInfo[info_idx].icon_no]->setAlpha(0xFF);
                } else if (mIconInfo[info_idx]._15 == 3) {
                    mPictures[mIconInfo[info_idx].icon_no]->setAlpha(255.0f * mBlinkAlpha);
                } else {
                    mPictures[mIconInfo[info_idx].icon_no]->setAlpha(param_4 * (255.0f * param_3 * mIconInfo[info_idx].alpha_rate));
                }

                f32 pos_x = i_posX + (icon_pos_x - (icon_size_x / 2));
                mPictures[mIconInfo[info_idx].icon_no]->draw(pos_x, (i_posY + (icon_pos_y - icon_size_y / 2)),
                                                             icon_size_x, icon_size_y, false, false, false);

                if (mIconInfo[info_idx].icon_no == ICON_LIGHT_DROP_e) {
                    mLightDropPic->draw((pos_x + (icon_size_x / 2)) - (var_f29 / 2),
                                        ((icon_size_y / 2) + (i_posY + (icon_pos_y - icon_size_y / 2))) - (var_f28 / 2),
                                        var_f29, var_f28, false, false, false);
                }
            }
        }
    }
}

void dMenuMapCommon_c::iconScale(int param_1, f32 param_2, f32 param_3, f32 param_4) {
    _c88 = 1.0f + param_4;

    f32 icon_scale[ICON_MAX_e];
    icon_scale[ICON_LINK_e] = g_fmapHIO.mMapIconHIO.mLinkZoomScale[param_1] - param_2 *
                              (g_fmapHIO.mMapIconHIO.mLinkZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mLinkScale[param_1]);
    
    icon_scale[ICON_DUNGEON_ENTER_e] = g_fmapHIO.mMapIconHIO.mDungeonEntranceZoomScale[param_1] - param_2 *
                                       (g_fmapHIO.mMapIconHIO.mDungeonEntranceZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mDungeonEntranceScale[param_1]);

    icon_scale[ICON_LINK_ENTER_e] = g_fmapHIO.mMapIconHIO.mRoomEntranceZoomScale[param_1] - param_2 *
                                    (g_fmapHIO.mMapIconHIO.mRoomEntranceZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mRoomEntranceScale[param_1]);

    icon_scale[ICON_LV8_WARP_e] = g_fmapHIO.mMapIconHIO.mLV8EntranceZoomScale[param_1] - param_2 *
                                  (g_fmapHIO.mMapIconHIO.mLV8EntranceZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mLV8EntranceScale[param_1]);

    icon_scale[ICON_BOSS_e] = g_fmapHIO.mMapIconHIO.mBossZoomScale[param_1] - param_2 *
                              (g_fmapHIO.mMapIconHIO.mBossZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mBossScale[param_1]);

    icon_scale[ICON_BOSS_GANON_e] = g_fmapHIO.mMapIconHIO.mGanonZoomScale[param_1] - param_2 *
                                    (g_fmapHIO.mMapIconHIO.mGanonZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mGanonScale[param_1]);

    icon_scale[ICON_KEY_e] = g_fmapHIO.mMapIconHIO.mSmallKeyZoomScale[param_1] - param_2 *
                            (g_fmapHIO.mMapIconHIO.mSmallKeyZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mSmallKeyScale[param_1]);

    icon_scale[ICON_TREASURE_CHEST_e] = g_fmapHIO.mMapIconHIO.mChestZoomScale[param_1] - param_2 *
                                        (g_fmapHIO.mMapIconHIO.mChestZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mChestScale[param_1]);

    icon_scale[ICON_YETO_e] = g_fmapHIO.mMapIconHIO.mYetoZoomScale[param_1] - param_2 *
                              (g_fmapHIO.mMapIconHIO.mYetoZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mYetoScale[param_1]);

    icon_scale[ICON_YETA_e] = g_fmapHIO.mMapIconHIO.mYetaZoomScale[param_1] - param_2 *
                              (g_fmapHIO.mMapIconHIO.mYetaZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mYetaScale[param_1]);

    icon_scale[ICON_GOLD_WOLF_e] = g_fmapHIO.mMapIconHIO.mGoldWolfZoomScale[param_1] - param_2 *
                                   (g_fmapHIO.mMapIconHIO.mGoldWolfZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mGoldWolfScale[param_1]);

    icon_scale[ICON_MONKEY_e] = g_fmapHIO.mMapIconHIO.mMonkeyZoomScale[param_1] - param_2 *
                                (g_fmapHIO.mMapIconHIO.mMonkeyZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mMonkeyScale[param_1]);

    icon_scale[ICON_OOCCOO_e] = g_fmapHIO.mMapIconHIO.mOoccooZoomScale[param_1] - param_2 *
                                (g_fmapHIO.mMapIconHIO.mOoccooZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mOoccooScale[param_1]);

    icon_scale[ICON_OOCCOO_JR_e] = g_fmapHIO.mMapIconHIO.mOoccooPotZoomScale[param_1] - param_2 *
                                   (g_fmapHIO.mMapIconHIO.mOoccooPotZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mOoccooPotScale[param_1]);

    icon_scale[ICON_COPY_STATUE_e] = g_fmapHIO.mMapIconHIO.mCopyRodStatueZoomScale[param_1] - param_2 *
                                     (g_fmapHIO.mMapIconHIO.mCopyRodStatueZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mCopyRodStatueScale[param_1]);

    icon_scale[ICON_DUNGEON_WARP_e] = g_fmapHIO.mMapIconHIO.mDungeonWarpZoomScale[param_1] - param_2 *
                                      (g_fmapHIO.mMapIconHIO.mDungeonWarpZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mDungeonWarpScale[param_1]);

    icon_scale[ICON_LIGHT_DROP_e] = g_fmapHIO.mMapIconHIO.mLightDropZoomScale[param_1] - param_2 *
                                    (g_fmapHIO.mMapIconHIO.mLightDropZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mLightDropScale[param_1]);

    icon_scale[ICON_LIGHT_BALL_e] = g_fmapHIO.mMapIconHIO.mLightOrbZoomScale[param_1] - param_2 *
                                    (g_fmapHIO.mMapIconHIO.mLightOrbZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mLightOrbScale[param_1]);

    icon_scale[ICON_CANNON_BALL_e] = g_fmapHIO.mMapIconHIO.mCannonBallZoomScale[param_1] - param_2 *
                                     (g_fmapHIO.mMapIconHIO.mCannonBallZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mCannonBallScale[param_1]);

    icon_scale[ICON_COACH_e] = g_fmapHIO.mMapIconHIO.mWagonZoomScale[param_1] - param_2 *
                               (g_fmapHIO.mMapIconHIO.mWagonZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mWagonScale[param_1]);

    icon_scale[ICON_DESTINATION_e] = g_fmapHIO.mMapIconHIO.mDestinationZoomScale[param_1] - param_2 *
                                     (g_fmapHIO.mMapIconHIO.mDestinationZoomScale[param_1] - g_fmapHIO.mMapIconHIO.mDestinationScale[param_1]);
    icon_scale[0] = 1.0f;
    icon_scale[1] = 1.0f;

    for (int i = 0; i < ICON_MAX_e; i++) {
        if (mPictures[i] != NULL) {
            if (i == ICON_COACH_e) {
                mPictures[i]->scale(((16.0f / 27.0f) + ((11.0f / 27.0f) * param_4)) * icon_scale[i],
                                    icon_scale[i]);
            } else {
                mPictures[i]->scale(icon_scale[i], icon_scale[i]);

                if (i == ICON_LIGHT_DROP_e) {
                    mLightDropPic->scale(icon_scale[i], icon_scale[i]);
                }
            }
        }
    }

    setBlendRatio(ICON_LINK_e, param_3, param_4);
    setBlendRatio(ICON_DUNGEON_ENTER_e, param_3, param_4);
    setBlendRatio(ICON_LINK_ENTER_e, param_3, param_4);
    setBlendRatio(ICON_LV8_WARP_e, param_3, param_4);
    setBlendRatio(ICON_BOSS_e, param_3, param_4);
    setBlendRatio(ICON_BOSS_GANON_e, param_3, param_4);
    setBlendRatio(ICON_KEY_e, param_3, param_4);
    setBlendRatio(ICON_TREASURE_CHEST_e, param_3, param_4);
    setBlendRatio(ICON_YETO_e, param_3, param_4);
    setBlendRatio(ICON_YETA_e, param_3, param_4);
    setBlendRatio(ICON_GOLD_WOLF_e, param_3, param_4);
    setBlendRatio(ICON_MONKEY_e, param_3, param_4);
    setBlendRatio(ICON_OOCCOO_e, param_3, param_4);
    setBlendRatio(ICON_OOCCOO_JR_e, param_3, param_4);
    setBlendRatio(ICON_COPY_STATUE_e, param_3, param_4);
    setBlendRatio(ICON_DUNGEON_WARP_e, param_3, param_4);
    setBlendRatio(ICON_LIGHT_DROP_e, param_3, param_4);
    setBlendRatio(ICON_LIGHT_BALL_e, param_3, param_4);
    setBlendRatio(ICON_CANNON_BALL_e, param_3, param_4);
    setBlendRatio(ICON_COACH_e, param_3, param_4);
    setBlendRatio(ICON_DESTINATION_e, param_3, param_4);
}

bool dMenuMapCommon_c::setIconInfo(u8 i_iconNo, f32 i_posX, f32 i_posY, f32 i_alpharate, f32 i_rotation,
                                   f32 i_scale, u8 param_7) {
    if (mIconNum >= 128) {
        return false;
    }

    mIconInfo[mIconNum].pos_x = i_posX;
    mIconInfo[mIconNum].pos_y = i_posY;
    mIconInfo[mIconNum].scale = i_scale;
    mIconInfo[mIconNum].rotation = i_rotation;
    mIconInfo[mIconNum].icon_no = i_iconNo;
    mIconInfo[mIconNum]._15 = param_7;
    mIconInfo[mIconNum].alpha_rate = i_alpharate;
    mIconNum++;
    return true;
}

void dMenuMapCommon_c::clearIconInfo() {
    mIconNum = 0;
    for (int i = 0; i < 128; i++) {
        mIconInfo[i].pos_x = 0.0f;
        mIconInfo[i].pos_y = 0.0f;
        mIconInfo[i].scale = 1.0f;
        mIconInfo[i].rotation = 0.0f;
        mIconInfo[i].icon_no = ICON_MAX_e;
        mIconInfo[i]._15 = 0;
        // !@bug alpha_rate is only set to zero in the first icon.
        // Shouldn't really matter though as setIconInfo sets it on init.
        mIconInfo[mIconNum].alpha_rate = 0.0f;
    }
}

void dMenuMapCommon_c::setBlendRatio(u8 i_iconNo, f32 param_2, f32 param_3) {
    if (mPictures[i_iconNo] != NULL) {
        mPictures[i_iconNo]->setBlendRatio(param_2, param_3);

        if (i_iconNo == ICON_LIGHT_DROP_e) {
            mLightDropPic->setBlendRatio(param_2, param_3);
        }
    }
}

void dMenuMapCommon_c::blinkMove(s16 param_1) {
    mBlinkTimer++;
    if (mBlinkTimer >= param_1) {
        mBlinkTimer -= (s16)(param_1 + 15);
    }

    if (mBlinkTimer < 0) {
        mBlinkAlpha = 1.0f;
    } else if (mBlinkTimer <= param_1 / 2.0f) {
        mBlinkAlpha = (param_1 / 2.0f - mBlinkTimer) / (param_1 / 2.0f);
    } else {
        mBlinkAlpha = (mBlinkTimer - param_1 / 2.0f) / (param_1 / 2.0f);
    }
}

void dMenuMapCommon_c::moveLightDropAnime() {
    int bVar6 = 0;
    if (_c88 > 1.0f) {
        bVar6 = 1;
    }

    s16 flash_frame_num = g_fmapHIO.mMapIconHIO.mLightDropFlashFrameNum;
    f32 flash_start_scale = g_fmapHIO.mMapIconHIO.mLightDropFlashStartScaleOut[bVar6];
    f32 flash_end_scale = g_fmapHIO.mMapIconHIO.mLightDropFlashEndScaleOut[bVar6];
    u8 flash_start_alpha = g_fmapHIO.mMapIconHIO.mLightDropFlashStartAlphaOut[bVar6];
    u8 flash_end_alpha = g_fmapHIO.mMapIconHIO.mLightDropFlashEndAlphaOut[bVar6];

    mLightDropFlashTimer++;
    if (mLightDropFlashTimer >= flash_frame_num) {
        mLightDropFlashTimer -= flash_frame_num;
    }

    f32 fVar7;
    if (mLightDropFlashTimer <= flash_frame_num / 2.0f) {
        fVar7 = (flash_frame_num / 2.0f - mLightDropFlashTimer) / (flash_frame_num / 2.0f);
    } else {
        fVar7 = (mLightDropFlashTimer - flash_frame_num / 2.0f) / (flash_frame_num / 2.0f);
    }

    _c80 = flash_start_alpha + fVar7 * (flash_end_alpha - flash_start_alpha);
    _c7c = flash_start_scale + fVar7 * (flash_end_scale - flash_start_scale);
}

f32 dMenuMapCommon_c::getIconSizeX(u8 i_iconNo) {
    for (int i = 0; i < ARRAY_SIZEU(map_icon_size); i++) {
        if (map_icon_size[i].icon_no == i_iconNo) {
            return map_icon_size[i].size_x;
        }
    }

    return 0.0f;
}

f32 dMenuMapCommon_c::getIconSizeY(u8 i_iconNo) {
    for (int i = 0; i < ARRAY_SIZEU(map_icon_size); i++) {
        if (map_icon_size[i].icon_no == i_iconNo) {
            return map_icon_size[i].size_y;
        }
    }

    return 0.0f;
}

void dMenuMapCommon_c::debugIcon() {
    int link_icon_idx = -1;
    for (int i = 0; i < 128; i++) {
        if (mIconInfo[i].icon_no == 17) {
            link_icon_idx = i;
            break;
        }
    }

    if (link_icon_idx < 0) {
        return;
    }

    f32 pos_x = mIconInfo[link_icon_idx].pos_x;
    f32 pos_y = mIconInfo[link_icon_idx].pos_y;
    f32 scale = mIconInfo[link_icon_idx].scale;

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[0]) {
        setIconInfo(0, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
        setIconInfo(1, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (!g_fmapHIO.mMapIconHIO.mIconDisplay[1]) {
        mIconInfo[link_icon_idx]._15 = 0;
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[2]) {
        setIconInfo(ICON_DUNGEON_WARP_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[3]) {
        setIconInfo(ICON_DESTINATION_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[4]) {
        setIconInfo(ICON_BOSS_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[5]) {
        setIconInfo(ICON_BOSS_GANON_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[6]) {
        setIconInfo(ICON_LIGHT_DROP_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[7]) {
        setIconInfo(ICON_LIGHT_BALL_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[8]) {
        setIconInfo(ICON_CANNON_BALL_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[9]) {
        setIconInfo(ICON_YETO_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[10]) {
        setIconInfo(ICON_YETA_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[11]) {
        setIconInfo(ICON_GOLD_WOLF_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[12]) {
        setIconInfo(ICON_MONKEY_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[13]) {
        setIconInfo(ICON_OOCCOO_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[14]) {
        setIconInfo(ICON_OOCCOO_JR_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[15]) {
        setIconInfo(ICON_COPY_STATUE_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[16]) {
        setIconInfo(ICON_COACH_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[17]) {
        setIconInfo(ICON_KEY_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[18]) {
        setIconInfo(ICON_TREASURE_CHEST_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[19]) {
        setIconInfo(ICON_DUNGEON_ENTER_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[20]) {
        setIconInfo(ICON_LINK_ENTER_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }

    if (g_fmapHIO.mMapIconHIO.mIconDisplay[21]) {
        setIconInfo(ICON_LV8_WARP_e, pos_x, pos_y, 0.0f, scale, 1.0f, 1);
    }
}
