/**
 * c_dylink.cpp
 * REL to process name definitions and REL init functions
 */
#include <cstdio>

#include "c/c_dylink.h"
#include "DynamicLink.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRFileCache.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_ext.h"
#include <cstring>
#include "dusk/logging.h"

#if DEBUG
#include "f_pc/f_pc_debug_sv.h"
#include "m_Do/m_Do_ext2.h"
#endif

static DynamicNameTableEntry const DynamicNameTable[] = {
#if !TARGET_PC
    {fpcNm_ALLDIE_e, "d_a_alldie"},
    {fpcNm_Obj_Swpush_e, "d_a_obj_swpush"},
    {fpcNm_Obj_Swpush2_e, "d_a_obj_swpush2"},
    {fpcNm_Obj_Swpush5_e, "d_a_obj_swpush5"},
    {fpcNm_Tag_Gstart_e, "d_a_tag_gstart"},
    #if PLATFORM_WII || PLATFORM_SHIELD
    {fpcNm_NO_CHG_ROOM_e, "d_a_no_chg_room"},
    #endif
    {fpcNm_Obj_Lv6ElevtA_e, "d_a_obj_lv6elevta"},
    {fpcNm_OBJ_SO_e, "d_a_obj_so"},
    {fpcNm_Obj_Movebox_e, "d_a_obj_movebox"},
    {fpcNm_Obj_SwTurn_e, "d_a_obj_swturn"},
    {fpcNm_Obj_Lv6SwTurn_e, "d_a_obj_lv6swturn"},
    {fpcNm_OBJ_SEKIZOA_e, "d_a_obj_sekizoa"},
    {fpcNm_OBJ_GRA_e, "d_a_obj_gra2"},
    {fpcNm_TAG_GRA_e, "d_a_tag_gra"},
    {fpcNm_TAG_YAMI_e, "d_a_tag_yami"},
    {fpcNm_Obj_Ladder_e, "d_a_obj_ladder"},
    {fpcNm_OBJ_BEF_e, "d_a_obj_brakeeff"},
    {fpcNm_OBJ_FMOBJ_e, "d_a_obj_fmobj"},
    {fpcNm_OBJ_LBOX_e, "d_a_obj_lbox"},
    {fpcNm_OBJ_WEB0_e, "d_a_obj_web0"},
    {fpcNm_OBJ_WEB1_e, "d_a_obj_web1"},
    {fpcNm_OBJ_CB_e, "d_a_obj_cb"},
    {fpcNm_OBJ_MAKI_e, "d_a_obj_maki"},
    {fpcNm_OBJ_BRG_e, "d_a_obj_brg"},
    {fpcNm_OBJ_GB_e, "d_a_obj_gb"},
    {fpcNm_OBJ_GM_e, "d_a_obj_gm"},
    {fpcNm_OBJ_TOBY_e, "d_a_obj_toby"},
    {fpcNm_OBJ_TP_e, "d_a_obj_tp"},
    {fpcNm_TREESH_e, "d_a_obj_treesh"},
    {fpcNm_Obj_ZDoor_e, "d_a_obj_zdoor"},
    {fpcNm_Obj_Pillar_e, "d_a_obj_pillar"},
    {fpcNm_Obj_Cdoor_e, "d_a_obj_cdoor"},
    {fpcNm_GRDWATER_e, "d_a_obj_groundwater"},
    {fpcNm_Obj_RotBridge_e, "d_a_obj_rotBridge"},
    {fpcNm_Obj_MagLift_e, "d_a_obj_magLift"},
    {fpcNm_Obj_MagLiftRot_e, "d_a_obj_magLiftRot"},
    {fpcNm_Obj_Lv1Cdl00_e, "d_a_obj_lv1Candle00"},
    {fpcNm_Obj_Lv1Cdl01_e, "d_a_obj_lv1Candle01"},
    {fpcNm_Obj_TvCdlst_e, "d_a_obj_TvCdlst"},
    {fpcNm_Obj_HsTarget_e, "d_a_obj_hsTarget"},
    {fpcNm_Obj_HeavySw_e, "d_a_obj_heavySw"},
    {fpcNm_Obj_GoGate_e, "d_a_obj_goGate"},
    {fpcNm_Obj_TaFence_e, "d_a_obj_taFence"},
    {fpcNm_Obj_Saidan_e, "d_a_obj_saidan"},
    {fpcNm_Obj_SpinLift_e, "d_a_obj_spinLift"},
    {fpcNm_Obj_BmWindow_e, "d_a_obj_bmWindow"},
    {fpcNm_Obj_RfHole_e, "d_a_obj_rfHole"},
    {fpcNm_Obj_WaterPillar_e, "d_a_obj_waterPillar"},
    {fpcNm_Obj_SyRock_e, "d_a_obj_syRock"},
    {fpcNm_Obj_BsGate_e, "d_a_obj_bsGate"},
    {fpcNm_Obj_AmiShutter_e, "d_a_obj_amiShutter"},
    {fpcNm_Obj_WtGate_e, "d_a_obj_waterGate"},
    {fpcNm_Obj_Lv2Candle_e, "d_a_obj_lv2Candle"},
    {fpcNm_Obj_TogeTrap_e, "d_a_obj_togeTrap"},
    {fpcNm_Obj_RotTrap_e, "d_a_obj_rotTrap"},
    {fpcNm_Obj_SwallShutter_e, "d_a_obj_sWallShutter"},
    {fpcNm_Obj_IceWall_e, "d_a_obj_lv5IceWall"},
    {fpcNm_Obj_Lv5SwIce_e, "d_a_obj_lv5SwIce"},
    {fpcNm_Obj_Lv5FBoard_e, "d_a_obj_lv5FloorBoard"},
    {fpcNm_Obj_Turara_e, "d_a_obj_Turara"},
    {fpcNm_Obj_TwGate_e, "d_a_obj_twGate"},
    {fpcNm_Obj_Digholl_e, "d_a_obj_digholl"},
    {fpcNm_Obj_Digpl_e, "d_a_obj_digplace"},
    {fpcNm_Obj_TestCube_e, "d_a_obj_testcube"},
    {fpcNm_Obj_Kshutter_e, "d_a_obj_kshutter"},
    {fpcNm_NPC_COACH_e, "d_a_npc_coach"},
    {fpcNm_NPC_THEB_e, "d_a_npc_theB"},
    {fpcNm_COACH_FIRE_e, "d_a_coach_fire"},
    {fpcNm_COACH2D_e, "d_a_coach_2D"},
    {fpcNm_BALLOON2D_e, "d_a_balloon_2D"},
    {fpcNm_SKIP2D_e, "d_a_skip_2D"},
    {fpcNm_Obj_MvStair_e, "d_a_obj_mvstair"},
    {fpcNm_Obj_Cowdoor_e, "d_a_obj_cowdoor"},
    {fpcNm_Obj_Swpropeller_e, "d_a_obj_swpropeller"},
    {fpcNm_Obj_BoomShutter_e, "d_a_obj_bmshutter"},
    {fpcNm_NPC_KS_e, "d_a_npc_ks"},
    {fpcNm_Obj_Hfuta_e, "d_a_obj_hfuta"},
    {fpcNm_Obj_BkDoor_e, "d_a_obj_bkdoor"},
    {fpcNm_Obj_Cboard_e, "d_a_obj_cboard"},
    {fpcNm_Obj_MGate_e, "d_a_obj_mgate"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_Obj_Ikada_e, "d_a_obj_ikada"},
    #endif
    {fpcNm_Obj_Ice_l_e, "d_a_obj_ice_l"},
    {fpcNm_Obj_Ice_s_e, "d_a_obj_ice_s"},
    {fpcNm_Obj_E_CREATE_e, "d_a_obj_enemy_create"},
    {fpcNm_Obj_Bhbridge_e, "d_a_obj_bhbridge"},
    {fpcNm_Obj_Kaisou_e, "d_a_obj_kaisou"},
    {fpcNm_Obj_HHASHI_e, "d_a_obj_hhashi"},
    {fpcNm_Obj_BHASHI_e, "d_a_obj_bhashi"},
    {fpcNm_OCTHASHI_e, "d_a_obj_octhashi"},
    {fpcNm_Obj_THASHI_e, "d_a_obj_thashi"},
    {fpcNm_Obj_CRVGATE_e, "d_a_obj_crvgate"},
    {fpcNm_Obj_CRVFENCE_e, "d_a_obj_crvfence"},
    {fpcNm_Obj_CRVHAHEN_e, "d_a_obj_crvhahen"},
    {fpcNm_Obj_CRVSTEEL_e, "d_a_obj_crvsteel"},
    {fpcNm_Obj_CRVLH_UP_e, "d_a_obj_crvlh_up"},
    {fpcNm_Obj_CRVLH_DW_e, "d_a_obj_crvlh_down"},
    {fpcNm_Obj_RIVERROCK_e, "d_a_obj_riverrock"},
    {fpcNm_Obj_DUST_e, "d_a_obj_dust"},
    {fpcNm_Obj_ITA_e, "d_a_obj_ita"},
    {fpcNm_Obj_Window_e, "d_a_obj_window"},
    {fpcNm_Obj_MetalBox_e, "d_a_obj_metalbox"},
    {fpcNm_Obj_BBox_e, "d_a_obj_bbox"},
    {fpcNm_OBJ_MSIMA_e, "d_a_obj_msima"},
    {fpcNm_OBJ_MYOGAN_e, "d_a_obj_myogan"},
    {fpcNm_B_ZANTS_e, "d_a_b_zant_sima"},
    {fpcNm_Obj_ChainBlock_e, "d_a_obj_cblock"},
    {fpcNm_Obj_ChainWall_e, "d_a_obj_cwall"},
    {fpcNm_Obj_KkrGate_e, "d_a_obj_kgate"},
    {fpcNm_Obj_RiderGate_e, "d_a_obj_rgate"},
    {fpcNm_Obj_Onsen_e, "d_a_obj_onsen"},
    {fpcNm_Obj_Chest_e, "d_a_obj_chest"},
    {fpcNm_Obj_Bemos_e, "d_a_obj_bemos"},
    {fpcNm_Obj_RopeBridge_e, "d_a_obj_rope_bridge"},
    {fpcNm_Obj_WellCover_e, "d_a_obj_well_cover"},
    {fpcNm_Obj_GraveStone_e, "d_a_obj_grave_stone"},
    {fpcNm_Obj_ZraRock_e, "d_a_obj_zra_rock"},
    {fpcNm_Obj_GraRock_e, "d_a_obj_gra_rock"},
    {fpcNm_Obj_GrzRock_e, "d_a_obj_grz_rock"},
    {fpcNm_GRA_WALL_e, "d_a_obj_graWall"},
    {fpcNm_OBJ_ONSEN_FIRE_e, "d_a_obj_onsenFire"},
    {fpcNm_Obj_Lv6bemos_e, "d_a_obj_lv6bemos"},
    {fpcNm_Obj_Lv6bemos2_e, "d_a_obj_lv6bemos2"},
    {fpcNm_Obj_BarDesk_e, "d_a_obj_barDesk"},
    {fpcNm_Obj_DigSnow_e, "d_a_obj_digsnow"},
    {fpcNm_Obj_Ytaihou_e, "d_a_obj_Y_taihou"},
    {fpcNm_Obj_Elevator_e, "d_a_obj_dmelevator"},
    {fpcNm_Obj_Lv6TogeRoll_e, "d_a_obj_lv6TogeRoll"},
    {fpcNm_Obj_Lv6TogeTrap_e, "d_a_obj_lv6TogeTrap"},
    {fpcNm_Obj_Lv6Tenbin_e, "d_a_obj_lv6Tenbin"},
    {fpcNm_Obj_Lv6SwGate_e, "d_a_obj_lv6SwGate"},
    {fpcNm_Obj_Lv6Lblock_e, "d_a_obj_lv6Lblock"},
    {fpcNm_Obj_Lv6ChgGate_e, "d_a_obj_lv6ChangeGate"},
    {fpcNm_Obj_Lv6FuriTrap_e, "d_a_obj_lv6FurikoTrap"},
    {fpcNm_Obj_Lv6SzGate_e, "d_a_obj_lv6SzGate"},
    {fpcNm_Obj_Lv4EdShutter_e, "d_a_obj_lv4EdShutter"},
    {fpcNm_Obj_Lv4Gate_e, "d_a_obj_lv4Gate"},
    {fpcNm_Obj_Lv4PoGate_e, "d_a_obj_lv4PoGate"},
    {fpcNm_Obj_Lv4SlideWall_e, "d_a_obj_lv4SlideWall"},
    {fpcNm_Obj_Lv4HsTarget_e, "d_a_obj_lv4HsTarget"},
    {fpcNm_Obj_Lv7PropY_e, "d_a_obj_lv7PropellerY"},
    {fpcNm_Obj_Lv7BsGate_e, "d_a_obj_lv7BsGate"},
    {fpcNm_Obj_Lv8OptiLift_e, "d_a_obj_lv8OptiLift"},
    {fpcNm_Obj_Lv8KekkaiTrap_e, "d_a_obj_lv8KekkaiTrap"},
    {fpcNm_Obj_Lv8Lift_e, "d_a_obj_lv8Lift"},
    {fpcNm_Obj_Lv8UdFloor_e, "d_a_obj_lv8UdFloor"},
    {fpcNm_Obj_Lv9SwShutter_e, "d_a_obj_lv9SwShutter"},
    {fpcNm_Obj_TobyHouse_e, "d_a_obj_tobyhouse"},
    {fpcNm_Obj_poCandle_e, "d_a_obj_poCandle"},
    {fpcNm_Obj_Lv4DigSand_e, "d_a_obj_lv4digsand"},
    {fpcNm_Obj_FallObj_e, "d_a_obj_fallobj"},
    {fpcNm_Obj_SmgDoor_e, "d_a_obj_smgdoor"},
    {fpcNm_Obj_SwLight_e, "d_a_obj_swLight"},
    {fpcNm_Obj_Avalanche_e, "d_a_obj_avalanche"},
    {fpcNm_Obj_MirrorScrew_e, "d_a_obj_mirror_screw"},
    {fpcNm_Obj_MirrorSand_e, "d_a_obj_mirror_sand"},
    {fpcNm_Obj_MirrorTable_e, "d_a_obj_mirror_table"},
    {fpcNm_Obj_MirrorChain_e, "d_a_obj_mirror_chain"},
    {fpcNm_Obj_Mirror6Pole_e, "d_a_obj_mirror_6pole"},
    {fpcNm_Obj_SwSpinner_e, "d_a_obj_swspinner"},
    {fpcNm_Obj_TDoor_e, "d_a_obj_thdoor"},
    {fpcNm_Obj_Lv7Bridge_e, "d_a_obj_lv7bridge"},
    {fpcNm_Obj_zrTurara_e, "d_a_obj_zrTurara"},
    {fpcNm_Obj_TakaraDai_e, "d_a_obj_takaraDai"},
    {fpcNm_Obj_Table_e, "d_a_obj_table"},
    {fpcNm_Obj_CatDoor_e, "d_a_obj_catdoor"},
    {fpcNm_Obj_Gake_e, "d_a_obj_tgake"},
    {fpcNm_CSTAF_e, "d_a_cstaF"},
    {fpcNm_Obj_Lv4RailWall_e, "d_a_obj_lv4RailWall"},
    {fpcNm_Obj_Lv4Sand_e, "d_a_obj_lv4sand"},
    {fpcNm_Obj_PushDoor_e, "d_a_obj_pdoor"},
    {fpcNm_PushDoor_e, "d_a_door_push"},
    {fpcNm_Obj_GanonWall2_e, "d_a_obj_ganonwall2"},
    {fpcNm_Obj_Lv4Bridge_e, "d_a_obj_lv4bridge"},
    {fpcNm_Obj_Lv4Floor_e, "d_a_obj_lv4floor"},
    {fpcNm_Tag_Spinner_e, "d_a_tag_spinner"},
    {fpcNm_Obj_SwHang_e, "d_a_obj_swhang"},
    {fpcNm_Obj_RotStair_e, "d_a_obj_rstair"},
    {fpcNm_Obj_MagneArm_e, "d_a_obj_magne_arm"},
    {fpcNm_Obj_KWheel00_e, "d_a_obj_kwheel00"},
    {fpcNm_Obj_KWheel01_e, "d_a_obj_kwheel01"},
    {fpcNm_Obj_Ychndlr_e, "d_a_obj_lv5ychndlr"},
    {fpcNm_Obj_PRElvtr_e, "d_a_obj_lv4prelvtr"},
    {fpcNm_Obj_MHasu_e, "d_a_obj_hasu2"},
    {fpcNm_Obj_YIblltray_e, "d_a_obj_lv5yiblltray"},
    {fpcNm_Obj_Lv6EGate_e, "d_a_obj_lv6egate"},
    {fpcNm_Obj_PDtile_e, "d_a_obj_pdtile"},
    {fpcNm_Obj_PDwall_e, "d_a_obj_pdwall"},
    {fpcNm_Obj_Lv4PRwall_e, "d_a_obj_lv4prwall"},
    {fpcNm_Obj_KLift00_e, "d_a_obj_klift00"},
    {fpcNm_B_OH_e, "d_a_b_oh"},
    {fpcNm_Obj_Lv4Chan_e, "d_a_obj_lv4chandelier"},
    {fpcNm_Obj_Lv3R10Saka_e, "d_a_obj_lv3saka00"},
    {fpcNm_Obj_Lv3Water_e, "d_a_obj_lv3Water"},
    {fpcNm_Obj_Lv3Water2_e, "d_a_obj_lv3Water2"},
    {fpcNm_OBJ_LV3WATERB_e, "d_a_obj_lv3WaterB"},
    {fpcNm_Obj_HBombkoya_e, "d_a_obj_hbombkoya"},
    {fpcNm_Obj_SZbridge_e, "d_a_obj_szbridge"},
    {fpcNm_Obj_KakarikoBrg_e, "d_a_obj_warp_kbrg"},
    {fpcNm_Obj_OrdinBrg_e, "d_a_obj_warp_obrg"},
    {fpcNm_Obj_BurnBox_e, "d_a_obj_burnbox"},
    {fpcNm_Obj_KJgjs_e, "d_a_obj_kjgjs"},
    {fpcNm_OBJ_IHASI_e, "d_a_obj_ihasi"},
    {fpcNm_Obj_IceBlock_e, "d_a_obj_iceblock"},
    {fpcNm_Obj_VolcanicBall_e, "d_a_obj_volcball"},
    {fpcNm_Obj_VolcanicBomb_e, "d_a_obj_volcbom"},
    {fpcNm_Obj_VolcGnd_e, "d_a_obj_vground"},
    {fpcNm_Obj_KKanban_e, "d_a_obj_kkanban"},
    {fpcNm_E_PH_e, "d_a_e_ph"},
    {fpcNm_NPC_ZRA_e, "d_a_npc_zra"},
    {fpcNm_Obj_Chandelier_e, "d_a_obj_chandelier"},
    {fpcNm_Obj_Stopper2_e, "d_a_obj_stopper2"},
    {fpcNm_DOOR20_e, "d_a_door_shutter"},
    {fpcNm_Tag_Hinit_e, "d_a_tag_hinit"},
    {fpcNm_Tag_Hjump_e, "d_a_tag_hjump"},
    {fpcNm_Tag_AJnot_e, "d_a_tag_ajnot"},
    {fpcNm_Tag_Hstop_e, "d_a_tag_hstop"},
    {fpcNm_CANOE_e, "d_a_canoe"},
    {fpcNm_HORSE_e, "d_a_horse"},
    {fpcNm_E_WB_e, "d_a_e_wb"},
    {fpcNm_OBJ_ITO_e, "d_a_obj_ito"},
    {fpcNm_OBJ_SW_e, "d_a_obj_sw"},
    {fpcNm_SPINNER_e, "d_a_spinner"},
    {fpcNm_B_OB_e, "d_a_b_ob"},
    {fpcNm_KAGO_e, "d_a_kago"},
    {fpcNm_E_YC_e, "d_a_e_yc"},
    {fpcNm_B_DS_e, "d_a_b_ds"},
    {fpcNm_B_DR_e, "d_a_b_dr"},
    {fpcNm_B_ZANTZ_e, "d_a_b_zant_mobile"},
    {fpcNm_B_ZANT_e, "d_a_b_zant"},
    {fpcNm_B_ZANTM_e, "d_a_b_zant_magic"},
    {fpcNm_TBOX_e, "d_a_tbox"},
    {fpcNm_TBOX2_e, "d_a_tbox2"},
    {fpcNm_BOOMERANG_e, "d_a_boomerang"},
    {fpcNm_MIDNA_e, "d_a_midna"},
    {fpcNm_NPC_TK_e, "d_a_npc_tk"},
    {fpcNm_NPC_WORM_e, "d_a_npc_worm"},
    {fpcNm_PPolamp_e, "d_a_ppolamp"},
    {fpcNm_BkyRock_e, "d_a_obj_bky_rock"},
    {fpcNm_HITOBJ_e, "d_a_hitobj"},
    {fpcNm_EP_e, "d_a_ep"},
    {fpcNm_COW_e, "d_a_cow"},
    {fpcNm_PERU_e, "d_a_peru"},
    {fpcNm_NI_e, "d_a_ni"},
    {fpcNm_NPC_TKJ2_e, "d_a_npc_tkj2"},
    {fpcNm_SQ_e, "d_a_sq"},
    {fpcNm_NPC_SQ_e, "d_a_npc_sq"},
    {fpcNm_DO_e, "d_a_do"},
    {fpcNm_NPC_NE_e, "d_a_npc_ne"},
    {fpcNm_NPC_TR_e, "d_a_npc_tr"},
    {fpcNm_NPC_LF_e, "d_a_npc_lf"},
    {fpcNm_OBJ_FOOD_e, "d_a_obj_food"},
    {fpcNm_OBJ_KI_e, "d_a_obj_ki"},
    {fpcNm_OBJ_KITA_e, "d_a_obj_kita"},
    {fpcNm_OBJ_KEY_e, "d_a_obj_key"},
    {fpcNm_OBJ_KEYHOLE_e, "d_a_obj_keyhole"},
    {fpcNm_Obj_Lv5Key_e, "d_a_obj_Lv5Key"},
    {fpcNm_OBJ_LP_e, "d_a_obj_lp"},
    {fpcNm_OBJ_TATIGI_e, "d_a_obj_tatigi"},
    {fpcNm_OBJ_ROCK_e, "d_a_obj_rock"},
    {fpcNm_OBJ_WFLAG_e, "d_a_obj_wflag"},
    {fpcNm_OBJ_KAGE_e, "d_a_obj_kage"},
    {fpcNm_OBJ_KANBAN2_e, "d_a_obj_kanban2"},
    {fpcNm_OBJ_BALLOON_e, "d_a_obj_balloon"},
    {fpcNm_OBJ_SUISYA_e, "d_a_obj_suisya"},
    {fpcNm_OBJ_OILTUBO_e, "d_a_obj_oiltubo"},
    {fpcNm_OBJ_ROTEN_e, "d_a_obj_roten"},
    {fpcNm_OBJ_SSDRINK_e, "d_a_obj_ss_drink"},
    {fpcNm_OBJ_SSITEM_e, "d_a_obj_ss_item"},
    {fpcNm_TAG_SSDRINK_e, "d_a_tag_ss_drink"},
    {fpcNm_TAG_BTLITM_e, "d_a_tag_bottle_item"},
    {fpcNm_TAG_LV5SOUP_e, "d_a_tag_lv5soup"},
    {fpcNm_TAG_MNLIGHT_e, "d_a_tag_myna_light"},
    {fpcNm_TAG_SHOPCAM_e, "d_a_tag_shop_camera"},
    {fpcNm_TAG_SHOPITM_e, "d_a_tag_shop_item"},
    {fpcNm_OBJ_NDOOR_e, "d_a_obj_ndoor"},
    {fpcNm_OBJ_UDOOR_e, "d_a_obj_udoor"},
    {fpcNm_OBJ_USAKU_e, "d_a_obj_usaku"},
    {fpcNm_Obj_SM_DOOR_e, "d_a_obj_sm_door"},
    {fpcNm_OBJ_BED_e, "d_a_obj_bed"},
    {fpcNm_OBJ_BOUMATO_e, "d_a_obj_boumato"},
    {fpcNm_OBJ_ITAMATO_e, "d_a_obj_itamato"},
    {fpcNm_OBJ_NOUGU_e, "d_a_obj_nougu"},
    {fpcNm_OBJ_STICK_e, "d_a_obj_stick"},
    {fpcNm_OBJ_MIE_e, "d_a_obj_mie"},
    {fpcNm_OBJ_SEKIDOOR_e, "d_a_obj_sekidoor"},
    {fpcNm_OBJ_SEKIZO_e, "d_a_obj_sekizo"},
    {fpcNm_OBJ_SMTILE_e, "d_a_obj_smtile"},
    {fpcNm_NPC_FISH_e, "d_a_npc_fish"},
    {fpcNm_MG_FISH_e, "d_a_mg_fish"},
    {fpcNm_FSHOP_e, "d_a_mg_fshop"},
    {fpcNm_NPC_DU_e, "d_a_npc_du"},
    {fpcNm_DISAPPEAR_e, "d_a_disappear"},
    {fpcNm_Obj_Mato_e, "d_a_obj_mato"},
    {fpcNm_Obj_Flag_e, "d_a_obj_flag"},
    {fpcNm_Obj_Flag2_e, "d_a_obj_flag2"},
    {fpcNm_Obj_Flag3_e, "d_a_obj_flag3"},
    {fpcNm_Obj_GOMIKABE_e, "d_a_obj_gomikabe"},
    {fpcNm_Obj_Yousei_e, "d_a_obj_yousei"},
    {fpcNm_Obj_Kabuto_e, "d_a_obj_kabuto"},
    {fpcNm_Obj_Cho_e, "d_a_obj_cho"},
    {fpcNm_Obj_Kuw_e, "d_a_obj_kuwagata"},
    {fpcNm_Obj_Nan_e, "d_a_obj_nan"},
    {fpcNm_Obj_Dan_e, "d_a_obj_dan"},
    {fpcNm_Obj_Kam_e, "d_a_obj_kamakiri"},
    {fpcNm_Obj_Ten_e, "d_a_obj_ten"},
    {fpcNm_Obj_Ari_e, "d_a_obj_ari"},
    {fpcNm_Obj_Kag_e, "d_a_obj_kag"},
    {fpcNm_Obj_Batta_e, "d_a_obj_batta"},
    {fpcNm_Obj_Tombo_e, "d_a_obj_tombo"},
    {fpcNm_Obj_Kat_e, "d_a_obj_katatsumuri"},
    {fpcNm_Obj_H_Saku_e, "d_a_obj_h_saku"},
    {fpcNm_Obj_Yobikusa_e, "d_a_obj_yobikusa"},
    {fpcNm_Obj_KazeNeko_e, "d_a_obj_kazeneko"},
    {fpcNm_Obj_KznkArm_e, "d_a_obj_kznkarm"},
    {fpcNm_Obj_NamePlate_e, "d_a_obj_nameplate"},
    {fpcNm_Obj_OnCloth_e, "d_a_obj_ornament_cloth"},
    {fpcNm_Obj_LndRope_e, "d_a_obj_laundry_rope"},
    {fpcNm_Obj_ItaRope_e, "d_a_obj_sakuita_rope"},
    {fpcNm_Obj_Sakuita_e, "d_a_obj_sakuita"},
    {fpcNm_Obj_Laundry_e, "d_a_obj_laundry"},
    {fpcNm_WarpBug_e, "d_a_warp_bug"},
    {fpcNm_Izumi_Gate_e, "d_a_izumi_gate"},
    {fpcNm_Obj_Fchain_e, "d_a_obj_fchain"},
    {fpcNm_Obj_Wchain_e, "d_a_obj_wchain"},
    {fpcNm_Tag_Attp_e, "d_a_tag_attention"},
    {fpcNm_Obj_Tornado_e, "d_a_obj_tornado"},
    {fpcNm_Obj_Tornado2_e, "d_a_obj_tornado2"},
    {fpcNm_Obj_FirePillar_e, "d_a_obj_firepillar"},
    {fpcNm_Obj_FirePillar2_e, "d_a_obj_firepillar2"},
    {fpcNm_Obj_InoBone_e, "d_a_obj_inobone"},
    {fpcNm_Obj_Stopper_e, "d_a_obj_stopper"},
    {fpcNm_Obj_MHole_e, "d_a_obj_mhole"},
    {fpcNm_Tag_Magne_e, "d_a_tag_magne"},
    {fpcNm_Obj_BossWarp_e, "d_a_obj_bosswarp"},
    {fpcNm_Obj_WoodPendulum_e, "d_a_obj_wood_pendulum"},
    {fpcNm_Obj_WdStick_e, "d_a_obj_wdStick"},
    {fpcNm_Obj_StairBlock_e, "d_a_obj_stairBlock"},
    {fpcNm_Obj_Geyser_e, "d_a_obj_geyser"},
    {fpcNm_Tag_KtOnFire_e, "d_a_obj_ktOnFire"},
    {fpcNm_Obj_FireWood_e, "d_a_obj_fireWood"},
    {fpcNm_Obj_FireWood2_e, "d_a_obj_fireWood2"},
    {fpcNm_Obj_GpTaru_e, "d_a_obj_gpTaru"},
    {fpcNm_Obj_OnsenTaru_e, "d_a_obj_onsenTaru"},
    {fpcNm_Obj_KiPot_e, "d_a_obj_kiPot"},
    {fpcNm_TBOX_SW_e, "d_a_tboxSw"},
    {fpcNm_Obj_SwChain_e, "d_a_obj_swchain"},
    {fpcNm_Obj_WoodenSword_e, "d_a_obj_wsword"},
    {fpcNm_Obj_StoneMark_e, "d_a_obj_stoneMark"},
    {fpcNm_Obj_Lv3Candle_e, "d_a_obj_lv3Candle"},
    {fpcNm_Tag_Lv4Candle_e, "d_a_obj_lv4CandleTag"},
    {fpcNm_Tag_Lv4CandleDm_e, "d_a_obj_lv4CandleDemoTag"},
    {fpcNm_Obj_DamCps_e, "d_a_obj_damCps"},
    {fpcNm_Obj_Smoke_e, "d_a_obj_smoke"},
    {fpcNm_Obj_WaterFall_e, "d_a_obj_waterfall"},
    {fpcNm_Obj_ZoraCloth_e, "d_a_obj_zcloth"},
    {fpcNm_Obj_poFire_e, "d_a_obj_poFire"},
    {fpcNm_Tag_poFire_e, "d_a_tag_poFire"},
    {fpcNm_Obj_glowSphere_e, "d_a_obj_glowSphere"},
    {fpcNm_Tag_LightBall_e, "d_a_tag_lightball"},
    {fpcNm_SwLBall_e, "d_a_swLBall"},
    {fpcNm_SwBall_e, "d_a_swBall"},
    {fpcNm_Obj_WaterEff_e, "d_a_obj_lv3waterEff"},
    {fpcNm_Tag_RiverBack_e, "d_a_tag_river_back"},
    {fpcNm_Tag_KagoFall_e, "d_a_tag_kago_fall"},
    {fpcNm_Tag_Lv2PrChk_e, "d_a_tag_lv2prchk"},
    {fpcNm_Obj_Lv4Gear_e, "d_a_obj_lv4gear"},
    {fpcNm_Obj_MasterSword_e, "d_a_obj_master_sword"},
    {fpcNm_Obj_WoodStatue_e, "d_a_obj_wood_statue"},
    {fpcNm_Obj_Fan_e, "d_a_obj_fan"},
    {fpcNm_Obj_IceLeaf_e, "d_a_obj_iceleaf"},
    {fpcNm_Obj_zrTuraraRc_e, "d_a_obj_zrTuraraRock"},
    {fpcNm_Tag_RetRoom_e, "d_a_tag_ret_room"},
    {fpcNm_Obj_WindStone_e, "d_a_obj_wind_stone"},
    {fpcNm_Tag_WaraHowl_e, "d_a_tag_wara_howl"},
    {fpcNm_Obj_SCannon_e, "d_a_obj_scannon"},
    {fpcNm_Obj_SmWStone_e, "d_a_obj_smw_stone"},
    {fpcNm_Obj_SCannonCrs_e, "d_a_obj_scannon_crs"},
    {fpcNm_Tag_SnowEff_e, "d_a_obj_snowEffTag"},
    {fpcNm_Tag_CstaSw_e, "d_a_tag_CstaSw"},
    {fpcNm_Tag_Lv6CstaSw_e, "d_a_tag_lv6CstaSw"},
    {fpcNm_Obj_awaPlar_e, "d_a_obj_bubblePilar"},
    {fpcNm_Obj_poTbox_e, "d_a_obj_poTbox"},
    {fpcNm_Obj_TimeFire_e, "d_a_obj_timeFire"},
    {fpcNm_Obj_TMoon_e, "d_a_obj_tmoon"},
    {fpcNm_Obj_GanonWall_e, "d_a_obj_ganonwall"},
    {fpcNm_Obj_Prop_e, "d_a_obj_prop"},
    {fpcNm_CSTATUE_e, "d_a_cstatue"},
    {fpcNm_Obj_SwBallA_e, "d_a_obj_swBallA"},
    {fpcNm_Obj_SwBallB_e, "d_a_obj_swBallB"},
    {fpcNm_Obj_SnowSoup_e, "d_a_obj_snow_soup"},
    {fpcNm_Obj_Nagaisu_e, "d_a_obj_nagaisu"},
    {fpcNm_Obj_RCircle_e, "d_a_obj_rcircle"},
    {fpcNm_Obj_Picture_e, "d_a_obj_picture"},
    {fpcNm_Tag_SetBall_e, "d_a_tag_setBall"},
    {fpcNm_Tag_SmkEmt_e, "d_a_tag_smk_emt"},
    {fpcNm_SwTime_e, "d_a_swTime"},
    {fpcNm_Obj_HFtr_e, "d_a_obj_hakai_ftr"},
    {fpcNm_Obj_HBarrel_e, "d_a_obj_hakai_brl"},
    {fpcNm_Obj_Crystal_e, "d_a_obj_crystal"},
    {fpcNm_Obj_SCannonTen_e, "d_a_obj_scannon_ten"},
    {fpcNm_Obj_SwBallC_e, "d_a_obj_swBallC"},
    {fpcNm_SCENE_EXIT2_e, "d_a_scene_exit2"},
    {fpcNm_Obj_Hata_e, "d_a_obj_hata"},
    {fpcNm_Obj_ToaruMaki_e, "d_a_obj_toaru_maki"},
    {fpcNm_Tag_AttackItem_e, "d_a_tag_attack_item"},
    {fpcNm_Tag_RmbitSw_e, "d_a_tag_rmbit_sw"},
    {fpcNm_Obj_Sword_e, "d_a_obj_sword"},
    {fpcNm_Tag_Spring_e, "d_a_tag_spring"},
    {fpcNm_Tag_Statue_e, "d_a_tag_statue_evt"},
    {fpcNm_E_AI_e, "d_a_e_ai"},
    {fpcNm_E_GS_e, "d_a_e_gs"},
    {fpcNm_E_GOB_e, "d_a_e_gob"},
    {fpcNm_E_DD_e, "d_a_e_dd"},
    {fpcNm_E_DN_e, "d_a_e_dn"},
    {fpcNm_E_S1_e, "d_a_e_s1"},
    {fpcNm_E_MF_e, "d_a_e_mf"},
    {fpcNm_E_SG_e, "d_a_e_sg"},
    {fpcNm_E_BS_e, "d_a_e_bs"},
    {fpcNm_E_SF_e, "d_a_e_sf"},
    {fpcNm_E_SH_e, "d_a_e_sh"},
    {fpcNm_E_DF_e, "d_a_e_df"},
    {fpcNm_E_GM_e, "d_a_e_gm"},
    {fpcNm_E_MD_e, "d_a_e_md"},
    {fpcNm_E_SM_e, "d_a_e_sm"},
    {fpcNm_E_SM2_e, "d_a_e_sm2"},
    {fpcNm_E_ST_e, "d_a_e_st"},
    {fpcNm_E_ST_LINE_e, "d_a_e_st_line"},
    {fpcNm_E_SB_e, "d_a_e_sb"},
    {fpcNm_E_TH_e, "d_a_e_th"},
    {fpcNm_E_CR_e, "d_a_e_cr"},
    {fpcNm_E_CR_EGG_e, "d_a_e_cr_egg"},
    {fpcNm_E_DB_e, "d_a_e_db"},
    {fpcNm_E_DB_LEAF_e, "d_a_e_db_leaf"},
    {fpcNm_E_GA_e, "d_a_e_ga"},
    {fpcNm_E_GB_e, "d_a_e_gb"},
    {fpcNm_E_HB_e, "d_a_e_hb"},
    {fpcNm_E_HB_LEAF_e, "d_a_e_hb_leaf"},
    {fpcNm_E_HZELDA_e, "d_a_e_hzelda"},
    {fpcNm_E_YD_e, "d_a_e_yd"},
    {fpcNm_E_YH_e, "d_a_e_yh"},
    {fpcNm_E_YD_LEAF_e, "d_a_e_yd_leaf"},
    {fpcNm_E_HM_e, "d_a_e_hm"},
    {fpcNm_E_TK_e, "d_a_e_tk"},
    {fpcNm_E_TK2_e, "d_a_e_tk2"},
    {fpcNm_E_TK_BALL_e, "d_a_e_tk_ball"},
    {fpcNm_E_RB_e, "d_a_e_rb"},
    {fpcNm_E_RD_e, "d_a_e_rd"},
    {fpcNm_E_RDB_e, "d_a_e_rdb"},
    {fpcNm_E_RDY_e, "d_a_e_rdy"},
    {fpcNm_E_FM_e, "d_a_e_fm"},
    {fpcNm_E_FS_e, "d_a_e_fs"},
    {fpcNm_E_PM_e, "d_a_e_pm"},
    {fpcNm_E_PO_e, "d_a_e_po"},
    {fpcNm_E_MB_e, "d_a_e_mb"},
    {fpcNm_E_MK_e, "d_a_e_mk"},
    {fpcNm_E_MM_e, "d_a_e_mm"},
    {fpcNm_E_FZ_e, "d_a_e_fz"},
    {fpcNm_E_ZS_e, "d_a_e_zs"},
    {fpcNm_E_KK_e, "d_a_e_kk"},
    {fpcNm_E_HP_e, "d_a_e_hp"},
    {fpcNm_E_ZH_e, "d_a_e_zh"},
    {fpcNm_E_ZM_e, "d_a_e_zm"},
    {fpcNm_E_PZ_e, "d_a_e_pz"},
    {fpcNm_E_FB_e, "d_a_e_fb"},
    {fpcNm_E_FK_e, "d_a_e_fk"},
    {fpcNm_E_MS_e, "d_a_e_ms"},
    {fpcNm_E_NEST_e, "d_a_e_nest"},
    {fpcNm_E_NZ_e, "d_a_e_nz"},
    {fpcNm_E_BA_e, "d_a_e_ba"},
    {fpcNm_E_BU_e, "d_a_e_bu"},
    {fpcNm_E_BUG_e, "d_a_e_bug"},
    {fpcNm_E_BEE_e, "d_a_e_bee"},
    {fpcNm_E_IS_e, "d_a_e_is"},
    {fpcNm_E_KG_e, "d_a_e_kg"},
    {fpcNm_E_KR_e, "d_a_e_kr"},
    {fpcNm_E_SW_e, "d_a_e_sw"},
    {fpcNm_E_GE_e, "d_a_e_ge"},
    {fpcNm_Tag_WatchGe_e, "d_a_tag_watchge"},
    {fpcNm_E_YM_e, "d_a_e_ym"},
    {fpcNm_E_YM_TAG_e, "d_a_e_ym_tag"},
    {fpcNm_E_YMB_e, "d_a_e_ymb"},
    {fpcNm_Tag_FWall_e, "d_a_tag_firewall"},
    {fpcNm_Tag_WaterFall_e, "d_a_tag_waterfall"},
    {fpcNm_E_YK_e, "d_a_e_yk"},
    {fpcNm_E_YR_e, "d_a_e_yr"},
    {fpcNm_E_YG_e, "d_a_e_yg"},
    {fpcNm_E_HZ_e, "d_a_e_hz"},
    {fpcNm_E_WS_e, "d_a_e_ws"},
    {fpcNm_E_OC_e, "d_a_e_oc"},
    {fpcNm_E_OT_e, "d_a_e_ot"},
    {fpcNm_E_DT_e, "d_a_e_dt"},
    {fpcNm_E_BG_e, "d_a_e_bg"},
    {fpcNm_E_OctBg_e, "d_a_e_oct_bg"},
    {fpcNm_DR_e, "d_a_L7demo_dr"},
    {fpcNm_L7lowDr_e, "d_a_L7low_dr"},
    {fpcNm_L7ODR_e, "d_a_L7op_demo_dr"},
    {fpcNm_E_TT_e, "d_a_e_tt"},
    {fpcNm_E_DK_e, "d_a_e_dk"},
    {fpcNm_E_VT_e, "d_a_e_vt"},
    {fpcNm_E_WW_e, "d_a_e_ww"},
    {fpcNm_E_GI_e, "d_a_e_gi"},
    {fpcNm_B_BH_e, "d_a_b_bh"},
    {fpcNm_B_BQ_e, "d_a_b_bq"},
    {fpcNm_B_GM_e, "d_a_b_gm"},
    {fpcNm_B_GND_e, "d_a_b_gnd"},
    {fpcNm_B_GO_e, "d_a_b_go"},
    {fpcNm_B_OH2_e, "d_a_b_oh2"},
    {fpcNm_B_YO_e, "d_a_b_yo"},
    {fpcNm_B_YOI_e, "d_a_b_yo_ice"},
    {fpcNm_B_TN_e, "d_a_b_tn"},
    {fpcNm_B_GG_e, "d_a_b_gg"},
    {fpcNm_B_DRE_e, "d_a_b_dre"},
    {fpcNm_B_MGN_e, "d_a_b_mgn"},
    {fpcNm_E_WAP_e, "d_a_e_warpappear"},
    {fpcNm_Obj_SmallKey_e, "d_a_obj_smallkey"},
    {fpcNm_Obj_Kantera_e, "d_a_obj_kantera"},
    {fpcNm_Obj_LifeContainer_e, "d_a_obj_life_container"},
    {fpcNm_Obj_Shield_e, "d_a_obj_shield"},
    {fpcNm_Demo_Item_e, "d_a_demo_item"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_ShopItem_e, "d_a_shop_item"},
    #endif
    {fpcNm_Obj_Drop_e, "d_a_obj_drop"},
    {fpcNm_OBJ_RW_e, "d_a_obj_rw"},
    {fpcNm_NBOMB_e, "d_a_nbomb"},
    {fpcNm_TAG_CSW_e, "d_a_tag_csw"},
    {fpcNm_TAG_QS_e, "d_a_tag_qs"},
    {fpcNm_HOZELDA_e, "d_a_hozelda"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_SWC00_e, "d_a_swc00"},
    #endif
    {fpcNm_KNOB20_e, "d_a_door_knob00"},
    {fpcNm_DBDOOR_e, "d_a_door_dbdoor00"},
    {fpcNm_BOSS_DOOR_e, "d_a_door_boss"},
    {fpcNm_L1BOSS_DOOR_e, "d_a_door_bossL1"},
    {fpcNm_L1MBOSS_DOOR_e, "d_a_door_mbossL1"},
    {fpcNm_L5BOSS_DOOR_e, "d_a_door_bossL5"},
    {fpcNm_DSHUTTER_e, "d_a_dshutter"},
    {fpcNm_SPIRAL_DOOR_e, "d_a_door_spiral"},
    {fpcNm_Tag_ChgRestart_e, "d_a_tag_chgrestart"},
    {fpcNm_Tag_Restart_e, "d_a_tag_setrestart"},
    {fpcNm_ANDSW_e, "d_a_andsw"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_ANDSW2_e, "d_a_andsw2"},
    #endif
    {fpcNm_MYNA_e, "d_a_myna"},
    {fpcNm_NPC_GND_e, "d_a_npc_gnd"},
    {fpcNm_NPC_GRA_e, "d_a_npc_gra"},
    {fpcNm_NPC_GRC_e, "d_a_npc_grc"},
    {fpcNm_NPC_GRD_e, "d_a_npc_grd"},
    {fpcNm_NPC_GRM_e, "d_a_npc_grm"},
    {fpcNm_NPC_GRMC_e, "d_a_npc_grmc"},
    {fpcNm_NPC_GRO_e, "d_a_npc_gro"},
    {fpcNm_NPC_GRR_e, "d_a_npc_grr"},
    {fpcNm_NPC_GRS_e, "d_a_npc_grs"},
    {fpcNm_NPC_GRZ_e, "d_a_npc_grz"},
    {fpcNm_NPC_YAMID_e, "d_a_npc_yamid"},
    {fpcNm_NPC_YAMIT_e, "d_a_npc_yamit"},
    {fpcNm_NPC_YAMIS_e, "d_a_npc_yamis"},
    {fpcNm_NPC_BLUENS_e, "d_a_npc_blue_ns"},
    {fpcNm_NPC_KAKASHI_e, "d_a_npc_kakashi"},
    {fpcNm_NPC_KDK_e, "d_a_npc_kdk"},
    {fpcNm_NPC_ARU_e, "d_a_npc_aru"},
    {fpcNm_NPC_BANS_e, "d_a_npc_bans"},
    {fpcNm_NPC_BESU_e, "d_a_npc_besu"},
    {fpcNm_NPC_BOU_e, "d_a_npc_bou"},
    {fpcNm_NPC_BOU_S_e, "d_a_npc_bouS"},
    {fpcNm_NPC_CLERKA_e, "d_a_npc_clerka"},
    {fpcNm_NPC_CLERKB_e, "d_a_npc_clerkb"},
    {fpcNm_NPC_CLERKT_e, "d_a_npc_clerkt"},
    {fpcNm_NPC_WRESTLER_e, "d_a_npc_wrestler"},
    {fpcNm_Tag_Arena_e, "d_a_tag_arena"},
    {fpcNm_Tag_Instruction_e, "d_a_tag_instruction"},
    {fpcNm_NPC_DOC_e, "d_a_npc_doc"},
    {fpcNm_NPC_GWOLF_e, "d_a_npc_gwolf"},
    {fpcNm_NPC_LEN_e, "d_a_npc_len"},
    {fpcNm_NPC_LUD_e, "d_a_npc_lud"},
    {fpcNm_NPC_FAIRY_SEIREI_e, "d_a_npc_fairy_seirei"},
    {fpcNm_NPC_FAIRY_e, "d_a_npc_fairy"},
    {fpcNm_NPC_HANJO_e, "d_a_npc_hanjo"},
    {fpcNm_NPC_HENNA_e, "d_a_npc_henna"},
    {fpcNm_NPC_HENNA0_e, "d_a_npc_henna0"},
    {fpcNm_NPC_HOZ_e, "d_a_npc_hoz"},
    {fpcNm_NPC_JAGAR_e, "d_a_npc_jagar"},
    {fpcNm_NPC_KKRI_e, "d_a_npc_kkri"},
    {fpcNm_NPC_KN_e, "d_a_npc_kn"},
    {fpcNm_KN_BULLET_e, "d_a_obj_knBullet"},
    {fpcNm_NPC_KNJ_e, "d_a_npc_knj"},
    {fpcNm_NPC_KOLIN_e, "d_a_npc_kolin"},
    {fpcNm_NPC_KOLINB_e, "d_a_npc_kolinb"},
    {fpcNm_NPC_KYURY_e, "d_a_npc_kyury"},
    {fpcNm_NPC_MARO_e, "d_a_npc_maro"},
    {fpcNm_NPC_MIDP_e, "d_a_npc_midp"},
    {fpcNm_NPC_MOI_e, "d_a_npc_moi"},
    {fpcNm_NPC_RACA_e, "d_a_npc_raca"},
    {fpcNm_NPC_SARU_e, "d_a_npc_saru"},
    {fpcNm_NPC_SEIB_e, "d_a_npc_seib"},
    {fpcNm_NPC_SEIC_e, "d_a_npc_seic"},
    {fpcNm_NPC_SEID_e, "d_a_npc_seid"},
    {fpcNm_NPC_SEIRA_e, "d_a_npc_seira"},
    {fpcNm_NPC_SERA2_e, "d_a_npc_seira2"},
    {fpcNm_NPC_SEIREI_e, "d_a_npc_seirei"},
    {fpcNm_NPC_SHAMAN_e, "d_a_npc_shaman"},
    {fpcNm_NPC_SMARO_e, "d_a_npc_shop_maro"},
    {fpcNm_NPC_SOLA_e, "d_a_npc_sola"},
    {fpcNm_NPC_TARO_e, "d_a_npc_taro"},
    {fpcNm_NPC_PACHI_BESU_e, "d_a_npc_pachi_besu"},
    {fpcNm_NPC_PACHI_TARO_e, "d_a_npc_pachi_taro"},
    {fpcNm_NPC_PACHI_MARO_e, "d_a_npc_pachi_maro"},
    {fpcNm_TAG_PATI_e, "d_a_tag_pachi"},
    {fpcNm_NPC_THE_e, "d_a_npc_the"},
    {fpcNm_NPC_TKJ_e, "d_a_npc_tkj"},
    {fpcNm_NPC_TKS_e, "d_a_npc_tks"},
    {fpcNm_NPC_TKC_e, "d_a_npc_tkc"},
    {fpcNm_OBJ_TKS_e, "d_a_obj_tks"},
    {fpcNm_NPC_TOBY_e, "d_a_npc_toby"},
    {fpcNm_NPC_URI_e, "d_a_npc_uri"},
    {fpcNm_NPC_YELIA_e, "d_a_npc_yelia"},
    {fpcNm_NPC_YKM_e, "d_a_npc_ykm"},
    {fpcNm_NPC_YKW_e, "d_a_npc_ykw"},
    {fpcNm_NPC_ZANB_e, "d_a_npc_zanb"},
    {fpcNm_NPC_ZANT_e, "d_a_npc_zant"},
    {fpcNm_NPC_ZELDA_e, "d_a_npc_zelda"},
    {fpcNm_NPC_ZELR_e, "d_a_npc_zelR"},
    {fpcNm_NPC_ZELRO_e, "d_a_npc_zelRo"},
    {fpcNm_OBJ_ZRAFREEZE_e, "d_a_obj_zra_freeze"},
    {fpcNm_NPC_ZRC_e, "d_a_npc_zrc"},
    {fpcNm_NPC_ZRZ_e, "d_a_npc_zrz"},
    {fpcNm_ZRA_MARK_e, "d_a_obj_zraMark"},
    {fpcNm_MYNA2_e, "d_a_npc_myna2"},
    {fpcNm_TAG_MYNA2_e, "d_a_tag_myna2"},
    {fpcNm_NPC_CD3_e, "d_a_npc_cdn3"},
    {fpcNm_Tag_Schedule_e, "d_a_tag_schedule"},
    {fpcNm_Tag_Escape_e, "d_a_tag_escape"},
    {fpcNm_NPC_CHAT_e, "d_a_npc_chat"},
    {fpcNm_NPC_SOLDIERa_e, "d_a_npc_soldierA"},
    {fpcNm_NPC_SOLDIERb_e, "d_a_npc_soldierB"},
    {fpcNm_PASSER_MNG_e, "d_a_passer_mng"},
    {fpcNm_NPC_PASSER_e, "d_a_npc_passer"},
    {fpcNm_NPC_PASSER2_e, "d_a_npc_passer2"},
    {fpcNm_NPC_POST_e, "d_a_npc_post"},
    {fpcNm_NPC_POUYA_e, "d_a_npc_pouya"},
    {fpcNm_FORMATION_MNG_e, "d_a_formation_mng"},
    {fpcNm_NPC_FGUARD_e, "d_a_npc_fguard"},
    {fpcNm_GUARD_MNG_e, "d_a_guard_mng"},
    {fpcNm_TAG_GUARD_e, "d_a_tag_guard"},
    {fpcNm_NPC_GUARD_e, "d_a_npc_guard"},
    {fpcNm_NPC_ASH_e, "d_a_npc_ash"},
    {fpcNm_NPC_ASHB_e, "d_a_npc_ashB"},
    {fpcNm_NPC_SHAD_e, "d_a_npc_shad"},
    {fpcNm_NPC_RAFREL_e, "d_a_npc_rafrel"},
    {fpcNm_NPC_MOIR_e, "d_a_npc_moir"},
    {fpcNm_NPC_IMPAL_e, "d_a_npc_impal"},
    {fpcNm_NPC_SHOE_e, "d_a_npc_shoe"},
    {fpcNm_NPC_DOORBOY_e, "d_a_npc_doorboy"},
    {fpcNm_NPC_PRAYER_e, "d_a_npc_prayer"},
    {fpcNm_NPC_KASIHANA_e, "d_a_npc_kasi_hana"},
    {fpcNm_NPC_KASIKYU_e, "d_a_npc_kasi_kyu"},
    {fpcNm_NPC_KASIMICH_e, "d_a_npc_kasi_mich"},
    {fpcNm_NPC_DRSOL_e, "d_a_npc_drainSol"},
    {fpcNm_NPC_CHIN_e, "d_a_npc_chin"},
    {fpcNm_NPC_INS_e, "d_a_npc_ins"},
    {fpcNm_NPC_SHOP0_e, "d_a_npc_shop0"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_NPC_MK_e, "d_a_npc_mk"},
    #endif
    {fpcNm_NPC_P2_e, "d_a_npc_p2"},
    {fpcNm_KYTAG00_e, "d_a_kytag00"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_KYTAG01_e, "d_a_kytag01"},
    #endif
    {fpcNm_KYTAG02_e, "d_a_kytag02"},
    {fpcNm_KYTAG03_e, "d_a_kytag03"},
    {fpcNm_KYTAG04_e, "d_a_kytag04"},
    {fpcNm_KYTAG05_e, "d_a_kytag05"},
    {fpcNm_KYTAG06_e, "d_a_kytag06"},
    {fpcNm_KYTAG07_e, "d_a_kytag07"},
    {fpcNm_KYTAG08_e, "d_a_kytag08"},
    {fpcNm_KYTAG09_e, "d_a_kytag09"},
    {fpcNm_KYTAG10_e, "d_a_kytag10"},
    {fpcNm_KYTAG11_e, "d_a_kytag11"},
    {fpcNm_KYTAG12_e, "d_a_kytag12"},
    {fpcNm_KYTAG13_e, "d_a_kytag13"},
    {fpcNm_KYTAG14_e, "d_a_kytag14"},
    {fpcNm_KYTAG15_e, "d_a_kytag15"},
    {fpcNm_KYTAG16_e, "d_a_kytag16"},
    {fpcNm_KYTAG17_e, "d_a_kytag17"},
    {fpcNm_Ykgr_e, "d_a_ykgr"},
    {fpcNm_TALK_e, "d_a_talk"},
    {fpcNm_Obj_Crope_e, "d_a_obj_crope"},
    {fpcNm_Obj_Bombf_e, "d_a_obj_bombf"},
    {fpcNm_Obj_BkLeaf_e, "d_a_obj_bk_leaf"},
    {fpcNm_Tag_Mhint_e, "d_a_tag_mhint"},
    {fpcNm_Tag_Mmsg_e, "d_a_tag_mmsg"},
    {fpcNm_Tag_Mwait_e, "d_a_tag_mwait"},
    {fpcNm_Tag_Mstop_e, "d_a_tag_mstop"},
    {fpcNm_Tag_Stream_e, "d_a_tag_stream"},
    {fpcNm_Tag_Sppath_e, "d_a_tag_sppath"},
    {fpcNm_Tag_Wljump_e, "d_a_tag_wljump"},
    {fpcNm_Tag_TWGate_e, "d_a_tag_TWgate"},
    {fpcNm_Tag_Lv6Gate_e, "d_a_tag_Lv6Gate"},
    {fpcNm_Tag_Lv7Gate_e, "d_a_tag_Lv7Gate"},
    {fpcNm_Tag_Lv8Gate_e, "d_a_tag_Lv8Gate"},
    {fpcNm_Tag_TheBHint_e, "d_a_tag_theB_hint"},
    {fpcNm_Tag_Assist_e, "d_a_tag_assistance"},
    {fpcNm_DEMO00_e, "d_a_demo00"},
    {fpcNm_TAG_CAMERA_e, "d_a_tag_camera"},
    {fpcNm_TAG_CHKPOINT_e, "d_a_tag_chkpoint"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_TAG_EVENT_e, "d_a_tag_event"},
    #endif
    {fpcNm_TAG_EVT_e, "d_a_tag_evt"},
    {fpcNm_TAG_TELOP_e, "d_a_tag_telop"},
    {fpcNm_TAG_HOWL_e, "d_a_tag_howl"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_TAG_MSG_e, "d_a_tag_msg"},
    #endif
    {fpcNm_TAG_LANTERN_e, "d_a_tag_lantern"},
    {fpcNm_Tag_Mist_e, "d_a_tag_mist"},
    {fpcNm_DMIDNA_e, "d_a_dmidna"},
    {fpcNm_VRBOX_e, "d_a_vrbox"},
    {fpcNm_VRBOX2_e, "d_a_vrbox2"},
    {fpcNm_BG_e, "d_a_bg"},
    {fpcNm_SET_BG_OBJ_e, "d_a_set_bgobj"},
    {fpcNm_BG_OBJ_e, "d_a_bg_obj"},
    {fpcNm_MIRROR_e, "d_a_mirror"},
    {fpcNm_MOVIE_PLAYER_e, "d_a_movie_player"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_TITLE_e, "d_a_title"},
    #endif
    {fpcNm_FR_e, "d_a_fr"},
    {fpcNm_ECONT_e, "d_a_econt"},
    {fpcNm_MG_ROD_e, "d_a_mg_rod"},
    {fpcNm_E_ARROW_e, "d_a_e_arrow"},
    {fpcNm_BULLET_e, "d_a_bullet"},
    {fpcNm_SWHIT0_e, "d_a_swhit0"},
    {fpcNm_E_TH_BALL_e, "d_a_e_th_ball"},
    {fpcNm_TAG_EVTAREA_e, "d_a_tag_evtarea"},
    {fpcNm_TAG_EVTMSG_e, "d_a_tag_evtmsg"},
    {fpcNm_TAG_KMSG_e, "d_a_tag_kmsg"},
    {fpcNm_TAG_PUSH_e, "d_a_tag_push"},
    {fpcNm_E_MK_BO_e, "d_a_e_mk_bo"},
    {fpcNm_E_MM_MT_e, "d_a_e_mm_mt"},
    {fpcNm_OBJ_KBOX_e, "d_a_obj_kbox"},
    {fpcNm_OBJ_FW_e, "d_a_obj_fw"},
    {fpcNm_B_GOS_e, "d_a_b_gos"},
    {fpcNm_OBJ_YSTONE_e, "d_a_obj_ystone"},
    {fpcNm_MANT_e, "d_a_mant"},
    {fpcNm_CROD_e, "d_a_crod"},
    {fpcNm_OBJ_PLEAF_e, "d_a_obj_pleaf"},
    {fpcNm_OBJ_KBACKET_e, "d_a_obj_kbacket"},
    {fpcNm_OBJ_YBAG_e, "d_a_obj_yel_bag"},
    {fpcNm_OBJ_PUMPKIN_e, "d_a_obj_pumpkin"},
    {fpcNm_OBJ_AUTOMATA_e, "d_a_obj_automata"},
    {fpcNm_OBJ_GADGET_e, "d_a_obj_gadget"},
    {fpcNm_OBJ_KAGO_e, "d_a_obj_kago"},
    {fpcNm_Obj_Carry_e, "d_a_obj_carry"},
    {fpcNm_Obj_Stone_e, "d_a_obj_stone"},
    {fpcNm_OBJ_HB_e, "d_a_obj_hb"},
    {fpcNm_NPC_INKO_e, "d_a_npc_inko"},
    {fpcNm_BD_e, "d_a_bd"},
    {fpcNm_Obj_Eff_e, "d_a_obj_eff"},
    {fpcNm_E_BI_e, "d_a_e_bi"},
    {fpcNm_E_BI_LEAF_e, "d_a_e_bi_leaf"},
    {fpcNm_START_AND_GOAL_e, "d_a_startAndGoal"},
    {fpcNm_NPC_DF_e, "d_a_npc_df"},
    {fpcNm_ARROW_e, "d_a_arrow"},
    {fpcNm_PATH_LINE_e, "d_a_path_line"},
    {fpcNm_TAG_ALLMATO_e, "d_a_tag_allmato"},
    {fpcNm_Obj_Timer_e, "d_a_obj_timer"},
    {fpcNm_SCENE_EXIT_e, "d_a_scene_exit"},
    {fpcNm_SUSPEND_e, "d_a_suspend"},
    #if VERSION != VERSION_SHIELD_DEBUG
    {fpcNm_GRASS_e, "d_a_grass"},
    #endif
#endif
    {-1, NULL},
};

static DynamicModuleControlBase* DMC[fpcNm_MAX_NUM];

static bool DMC_initialized;

static BOOL cDyl_Initialized;

int cCc_Init() {
    JUT_ASSERT(37, !DMC_initialized);

#if !TARGET_PC
    #if PLATFORM_GCN
    JKRHeap* parentHeap = mDoExt_getArchiveHeap();
    #else
    JKRHeap* parentHeap = DynamicModuleControlBase::getHeap();
    #endif
    u32 dmcHeapSize = sizeof(DynamicModuleControl) * fpcNm_MAX_NUM;

    JKRSolidHeap* dmcHeap = JKRCreateSolidHeap(dmcHeapSize, parentHeap, false);
    JKRHeap* prevHeap = JKRSetCurrentHeap(dmcHeap);

    memset(&DMC, 0, sizeof(DMC));

    for (int i = 0; i < ARRAY_SIZEU(DynamicNameTable); i++) {
        const DynamicNameTableEntry& d = DynamicNameTable[i];

        if (d.name != NULL) {
            JUT_ASSERT(52, d.mKey < ARRAY_SIZEU(DMC));
            JUT_ASSERT(53, DMC[d.mKey] == NULL);

            for (int j = 0; j < ARRAY_SIZEU(DMC); j++) {
                if (DMC[j] != NULL) {
                    if (!strcmp(d.name, DMC[j]->getModuleName())) {
                        DMC[d.mKey] = DMC[j];
                        break;
                    }
                }
            }

            if (DMC[d.mKey] == NULL) {
                DMC[d.mKey] = JKR_NEW DynamicModuleControl(d.name);
            }
        }
    }

    dmcHeap->adjustSize();
    JKRSetCurrentHeap(prevHeap);

    #if DEBUG
    FixedMemoryCheck* check = FixedMemoryCheck::easyCreate(DMC, sizeof(DMC));
    #endif

#endif

    DMC_initialized = true;
    return 1;
}

BOOL cCc_Check() {
    BOOL rt = TRUE;
    for (int i = 0; i < ARRAY_SIZEU(DynamicNameTable); i++) {
        uintptr_t ptr = (uintptr_t)DMC[i];
        if (ptr != 0) {
            u32 ptr_hi_byte = ptr >> 0x18;
            if (ptr_hi_byte < 0x80 || ptr_hi_byte > 0x83) {
                // "cCc_Check invalid pointer detected"
                OS_REPORT_ERROR("cCc_Check ポインタ破壊を発見 %d %08x\n", i, ptr);
                rt = FALSE;
            }
        }
    }

    return rt;
}

BOOL cDyl_IsInitialized() {
    return cDyl_Initialized;
}

BOOL cDyl_IsLinked(s16 i_ProfName) {
    JUT_ASSERT(183, cDyl_Initialized);
    

    DynamicModuleControlBase* d = DMC[i_ProfName];
    BOOL rt;
    if (d != NULL) {
        rt = d->isLinked();
    } else {
        rt = TRUE;
    }

    return rt;
}

BOOL cDyl_Unlink(s16 i_ProfName) {
    #if DEBUG
    cCc_Check();
    #endif

    JUT_ASSERT(206, cDyl_Initialized);
    JUT_ASSERT(207, i_ProfName < ARRAY_SIZEU(DMC));

    DynamicModuleControlBase* d = DMC[i_ProfName];
    if (d != NULL) {
        return d->unlink();
    }

    return FALSE;
}

int cDyl_LinkASync(s16 i_ProfName) {
    #if DEBUG
    cCc_Check();
    #endif

    JUT_ASSERT(266, DMC_initialized);

    if (!cDyl_Initialized) {
        DuskLog.debug("cDyl_LinkASync: NOT initialized yet, profName={}", i_ProfName);
        OS_REPORT_ERROR("初期化が終わってないのに呼んでもらっても困ります %d %s\n", i_ProfName, fpcDbSv_getNameString(i_ProfName));
        return cPhs_INIT_e;
    }

    if (i_ProfName >= ARRAY_SIZEU(DMC)) {
        OSReport_Error("cDyl_Link i_ProfName=%d\n", i_ProfName);
        return cPhs_ERROR_e;
    }

    JUT_ASSERT(282, i_ProfName < ARRAY_SIZEU(DMC));

    DynamicModuleControlBase* d = DMC[i_ProfName];
    if (d != NULL) {
        JUT_ASSERT(286, cDyl_Initialized);

        if (d->load_async()) {
            if (d->link()) {
                return cPhs_COMPLEATE_e;
            } else {
                // "cDyl_LinkASync: Link failed. Returning\n"
                DuskLog.debug("cDyl_LinkASync: link FAILED for profName={}", i_ProfName);
                OSReport_Error("cDyl_LinkASync: リンクに失敗しました。諦めます\n");
                return cPhs_ERROR_e;
            }
        } else {
            DuskLog.debug("cDyl_LinkASync: load_async not ready for profName={}", i_ProfName);
            return cPhs_INIT_e;
        }
    }

    return cPhs_COMPLEATE_e;
}

static int cDyl_InitCallback(void* param_0) {
    DuskLog.debug("[DIAG] cDyl_InitCallback: START");
    JUT_ASSERT(335, !cDyl_Initialized);

#ifdef TARGET_PC
    // On PC, the profile list is statically linked (g_fpcPf_ProfileList_p in f_pc_profile.cpp).
    // Skip DVD-based REL loading and string table — OSLink/OSLinkFixed are stubs.
#else
    #if PLATFORM_GCN
    JKRHeap* parentHeap = mDoExt_getArchiveHeap();
    #else
    JKRHeap* parentHeap = DynamicModuleControlBase::getHeap();
    #endif
    DuskLog.debug("cDyl_InitCallback: parentHeap={}", (void*)parentHeap);

    JKRFileCache* loader = JKRMountDvdDrive("/", parentHeap, NULL);
    DuskLog.debug("cDyl_InitCallback: JKRMountDvdDrive loader={}", (void*)loader);

    DynamicModuleControl::initialize();
    DuskLog.debug("cDyl_InitCallback: DynamicModuleControl::initialize done");

    #if PLATFORM_GCN
    void* strTbl = JKRGetResource("/dvd/str/Final/Release/frameworkF.str");
    #elif PLATFORM_WII
    void* strTbl = JKRGetResource("/dvd/str/Rfinal/Release/RframeworkF.str");
    #else
    void* strTbl = JKRGetResource("/dvd/str/Final/Release/frameworkF.str");
    #endif
    DuskLog.debug("cDyl_InitCallback: frameworkF.str={}", strTbl);

    JKRDetachResource(strTbl, loader);
    JKRUnmountDvdDrive(loader);
    OSSetStringTable(strTbl);

    DynamicModuleControl dmc("f_pc_profile_lst");
    DuskLog.debug("cDyl_InitCallback: linking f_pc_profile_lst...");
    dmc.link();
    DuskLog.debug("cDyl_InitCallback: link done");
#endif
    cDyl_Initialized = true;

    fopScnM_CreateReq(fpcNm_LOGO_SCENE_e, 0x7FFF, 0, 0);
    DuskLog.debug("cDyl_InitCallback: fpcNm_LOGO_SCENE_e created, DONE");
    return 1;
}

static mDoDvdThd_callback_c* cDyl_DVD;

void cDyl_InitAsync() {
    cCc_Init();
    JUT_ASSERT(367, cDyl_DVD == NULL);
    cDyl_DVD = mDoDvdThd_callback_c::create((mDoDvdThd_callback_func)cDyl_InitCallback, NULL);
}

BOOL cDyl_InitAsyncIsDone() {
    if (cDyl_DVD == NULL) {
        return TRUE;
    }

    if (cDyl_DVD->sync()) {
        cDyl_DVD->destroy();
        cDyl_DVD = NULL;
        return TRUE;
    }

    return FALSE;
}

int cDylPhs::phase_01(void* param_0) {
    return cPhs_NEXT_e;
}

int cDylPhs::phase_02(s16* p_profName) {
    int ret = cDyl_LinkASync(*p_profName);
    if (ret == cPhs_COMPLEATE_e) {
        return cPhs_NEXT_e;
    }

    return ret;
}

int cDylPhs::phase_03(void* param_0) {
    return cPhs_INIT_e;
}

int cDylPhs::Link(request_of_phase_process_class* i_phase, s16 i_ProfName) {
    static request_of_phase_process_fn l_method[3] = {
        (request_of_phase_process_fn)cDylPhs::phase_01, 
        (request_of_phase_process_fn)cDylPhs::phase_02,
        (request_of_phase_process_fn)cDylPhs::phase_03
    };

    if (i_phase->id == 2) {
        return cPhs_COMPLEATE_e;
    }

    return dComLbG_PhaseHandler(i_phase, l_method, &i_ProfName);
}

int cDylPhs::Unlink(request_of_phase_process_class* i_phase, s16 i_ProfName) {
    JUT_ASSERT(460, i_phase->id != 1);

    if (i_phase->id == 2) {
        int ret = cDyl_Unlink(i_ProfName);
        i_phase->id = 0;
        return ret;
    }

    return 0;
}
