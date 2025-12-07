/**
 * c_dylink.cpp
 * REL to process name definitions and REL init functions
 */
#include "c/c_dylink.h"
#include "DynamicLink.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRFileCache.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_ext.h"

#if DEBUG
#include "f_pc/f_pc_debug_sv.h"
#include "m_Do/m_Do_ext2.h"
#endif

static DynamicNameTableEntry const DynamicNameTable[] = {
    {PROC_ALLDIE, "d_a_alldie"},
    {PROC_Obj_Swpush, "d_a_obj_swpush"},
    {PROC_Obj_Swpush2, "d_a_obj_swpush2"},
    {PROC_Obj_Swpush5, "d_a_obj_swpush5"},
    {PROC_Tag_Gstart, "d_a_tag_gstart"},
    #if PLATFORM_WII || PLATFORM_SHIELD
    {PROC_NO_CHG_ROOM, "d_a_no_chg_room"},
    #endif
    {PROC_Obj_Lv6ElevtA, "d_a_obj_lv6elevta"},
    {PROC_OBJ_SO, "d_a_obj_so"},
    {PROC_Obj_Movebox, "d_a_obj_movebox"},
    {PROC_Obj_SwTurn, "d_a_obj_swturn"},
    {PROC_Obj_Lv6SwTurn, "d_a_obj_lv6swturn"},
    {PROC_OBJ_SEKIZOA, "d_a_obj_sekizoa"},
    {PROC_OBJ_GRA, "d_a_obj_gra2"},
    {PROC_TAG_GRA, "d_a_tag_gra"},
    {PROC_TAG_YAMI, "d_a_tag_yami"},
    {PROC_Obj_Ladder, "d_a_obj_ladder"},
    {PROC_OBJ_BEF, "d_a_obj_brakeeff"},
    {PROC_OBJ_FMOBJ, "d_a_obj_fmobj"},
    {PROC_OBJ_LBOX, "d_a_obj_lbox"},
    {PROC_OBJ_WEB0, "d_a_obj_web0"},
    {PROC_OBJ_WEB1, "d_a_obj_web1"},
    {PROC_OBJ_CB, "d_a_obj_cb"},
    {PROC_OBJ_MAKI, "d_a_obj_maki"},
    {PROC_OBJ_BRG, "d_a_obj_brg"},
    {PROC_OBJ_GB, "d_a_obj_gb"},
    {PROC_OBJ_GM, "d_a_obj_gm"},
    {PROC_OBJ_TOBY, "d_a_obj_toby"},
    {PROC_OBJ_TP, "d_a_obj_tp"},
    {PROC_TREESH, "d_a_obj_treesh"},
    {PROC_Obj_ZDoor, "d_a_obj_zdoor"},
    {PROC_Obj_Pillar, "d_a_obj_pillar"},
    {PROC_Obj_Cdoor, "d_a_obj_cdoor"},
    {PROC_GRDWATER, "d_a_obj_groundwater"},
    {PROC_Obj_RotBridge, "d_a_obj_rotBridge"},
    {PROC_Obj_MagLift, "d_a_obj_magLift"},
    {PROC_Obj_MagLiftRot, "d_a_obj_magLiftRot"},
    {PROC_Obj_Lv1Cdl00, "d_a_obj_lv1Candle00"},
    {PROC_Obj_Lv1Cdl01, "d_a_obj_lv1Candle01"},
    {PROC_Obj_TvCdlst, "d_a_obj_TvCdlst"},
    {PROC_Obj_HsTarget, "d_a_obj_hsTarget"},
    {PROC_Obj_HeavySw, "d_a_obj_heavySw"},
    {PROC_Obj_GoGate, "d_a_obj_goGate"},
    {PROC_Obj_TaFence, "d_a_obj_taFence"},
    {PROC_Obj_Saidan, "d_a_obj_saidan"},
    {PROC_Obj_SpinLift, "d_a_obj_spinLift"},
    {PROC_Obj_BmWindow, "d_a_obj_bmWindow"},
    {PROC_Obj_RfHole, "d_a_obj_rfHole"},
    {PROC_Obj_WaterPillar, "d_a_obj_waterPillar"},
    {PROC_Obj_SyRock, "d_a_obj_syRock"},
    {PROC_Obj_BsGate, "d_a_obj_bsGate"},
    {PROC_Obj_AmiShutter, "d_a_obj_amiShutter"},
    {PROC_Obj_WtGate, "d_a_obj_waterGate"},
    {PROC_Obj_Lv2Candle, "d_a_obj_lv2Candle"},
    {PROC_Obj_TogeTrap, "d_a_obj_togeTrap"},
    {PROC_Obj_RotTrap, "d_a_obj_rotTrap"},
    {PROC_Obj_SwallShutter, "d_a_obj_sWallShutter"},
    {PROC_Obj_IceWall, "d_a_obj_lv5IceWall"},
    {PROC_Obj_Lv5SwIce, "d_a_obj_lv5SwIce"},
    {PROC_Obj_Lv5FBoard, "d_a_obj_lv5FloorBoard"},
    {PROC_Obj_Turara, "d_a_obj_Turara"},
    {PROC_Obj_TwGate, "d_a_obj_twGate"},
    {PROC_Obj_Digholl, "d_a_obj_digholl"},
    {PROC_Obj_Digpl, "d_a_obj_digplace"},
    {PROC_Obj_TestCube, "d_a_obj_testcube"},
    {PROC_Obj_Kshutter, "d_a_obj_kshutter"},
    {PROC_NPC_COACH, "d_a_npc_coach"},
    {PROC_NPC_THEB, "d_a_npc_theB"},
    {PROC_COACH_FIRE, "d_a_coach_fire"},
    {PROC_COACH2D, "d_a_coach_2D"},
    {PROC_BALLOON2D, "d_a_balloon_2D"},
    {PROC_SKIP2D, "d_a_skip_2D"},
    {PROC_Obj_MvStair, "d_a_obj_mvstair"},
    {PROC_Obj_Cowdoor, "d_a_obj_cowdoor"},
    {PROC_Obj_Swpropeller, "d_a_obj_swpropeller"},
    {PROC_Obj_BoomShutter, "d_a_obj_bmshutter"},
    {PROC_NPC_KS, "d_a_npc_ks"},
    {PROC_Obj_Hfuta, "d_a_obj_hfuta"},
    {PROC_Obj_BkDoor, "d_a_obj_bkdoor"},
    {PROC_Obj_Cboard, "d_a_obj_cboard"},
    {PROC_Obj_MGate, "d_a_obj_mgate"},
    #if !PLATFORM_SHIELD
    {PROC_Obj_Ikada, "d_a_obj_ikada"},
    #endif
    {PROC_Obj_Ice_l, "d_a_obj_ice_l"},
    {PROC_Obj_Ice_s, "d_a_obj_ice_s"},
    {PROC_Obj_E_CREATE, "d_a_obj_enemy_create"},
    {PROC_Obj_Bhbridge, "d_a_obj_bhbridge"},
    {PROC_Obj_Kaisou, "d_a_obj_kaisou"},
    {PROC_Obj_HHASHI, "d_a_obj_hhashi"},
    {PROC_Obj_BHASHI, "d_a_obj_bhashi"},
    {PROC_OCTHASHI, "d_a_obj_octhashi"},
    {PROC_Obj_THASHI, "d_a_obj_thashi"},
    {PROC_Obj_CRVGATE, "d_a_obj_crvgate"},
    {PROC_Obj_CRVFENCE, "d_a_obj_crvfence"},
    {PROC_Obj_CRVHAHEN, "d_a_obj_crvhahen"},
    {PROC_Obj_CRVSTEEL, "d_a_obj_crvsteel"},
    {PROC_Obj_CRVLH_UP, "d_a_obj_crvlh_up"},
    {PROC_Obj_CRVLH_DW, "d_a_obj_crvlh_down"},
    {PROC_Obj_RIVERROCK, "d_a_obj_riverrock"},
    {PROC_Obj_DUST, "d_a_obj_dust"},
    {PROC_Obj_ITA, "d_a_obj_ita"},
    {PROC_Obj_Window, "d_a_obj_window"},
    {PROC_Obj_MetalBox, "d_a_obj_metalbox"},
    {PROC_Obj_BBox, "d_a_obj_bbox"},
    {PROC_OBJ_MSIMA, "d_a_obj_msima"},
    {PROC_OBJ_MYOGAN, "d_a_obj_myogan"},
    {PROC_B_ZANTS, "d_a_b_zant_sima"},
    {PROC_Obj_ChainBlock, "d_a_obj_cblock"},
    {PROC_Obj_ChainWall, "d_a_obj_cwall"},
    {PROC_Obj_KkrGate, "d_a_obj_kgate"},
    {PROC_Obj_RiderGate, "d_a_obj_rgate"},
    {PROC_Obj_Onsen, "d_a_obj_onsen"},
    {PROC_Obj_Chest, "d_a_obj_chest"},
    {PROC_Obj_Bemos, "d_a_obj_bemos"},
    {PROC_Obj_RopeBridge, "d_a_obj_rope_bridge"},
    {PROC_Obj_WellCover, "d_a_obj_well_cover"},
    {PROC_Obj_GraveStone, "d_a_obj_grave_stone"},
    {PROC_Obj_ZraRock, "d_a_obj_zra_rock"},
    {PROC_Obj_GraRock, "d_a_obj_gra_rock"},
    {PROC_Obj_GrzRock, "d_a_obj_grz_rock"},
    {PROC_GRA_WALL, "d_a_obj_graWall"},
    {PROC_OBJ_ONSEN_FIRE, "d_a_obj_onsenFire"},
    {PROC_Obj_Lv6bemos, "d_a_obj_lv6bemos"},
    {PROC_Obj_Lv6bemos2, "d_a_obj_lv6bemos2"},
    {PROC_Obj_BarDesk, "d_a_obj_barDesk"},
    {PROC_Obj_DigSnow, "d_a_obj_digsnow"},
    {PROC_Obj_Ytaihou, "d_a_obj_Y_taihou"},
    {PROC_Obj_Elevator, "d_a_obj_dmelevator"},
    {PROC_Obj_Lv6TogeRoll, "d_a_obj_lv6TogeRoll"},
    {PROC_Obj_Lv6TogeTrap, "d_a_obj_lv6TogeTrap"},
    {PROC_Obj_Lv6Tenbin, "d_a_obj_lv6Tenbin"},
    {PROC_Obj_Lv6SwGate, "d_a_obj_lv6SwGate"},
    {PROC_Obj_Lv6Lblock, "d_a_obj_lv6Lblock"},
    {PROC_Obj_Lv6ChgGate, "d_a_obj_lv6ChangeGate"},
    {PROC_Obj_Lv6FuriTrap, "d_a_obj_lv6FurikoTrap"},
    {PROC_Obj_Lv6SzGate, "d_a_obj_lv6SzGate"},
    {PROC_Obj_Lv4EdShutter, "d_a_obj_lv4EdShutter"},
    {PROC_Obj_Lv4Gate, "d_a_obj_lv4Gate"},
    {PROC_Obj_Lv4PoGate, "d_a_obj_lv4PoGate"},
    {PROC_Obj_Lv4SlideWall, "d_a_obj_lv4SlideWall"},
    {PROC_Obj_Lv4HsTarget, "d_a_obj_lv4HsTarget"},
    {PROC_Obj_Lv7PropY, "d_a_obj_lv7PropellerY"},
    {PROC_Obj_Lv7BsGate, "d_a_obj_lv7BsGate"},
    {PROC_Obj_Lv8OptiLift, "d_a_obj_lv8OptiLift"},
    {PROC_Obj_Lv8KekkaiTrap, "d_a_obj_lv8KekkaiTrap"},
    {PROC_Obj_Lv8Lift, "d_a_obj_lv8Lift"},
    {PROC_Obj_Lv8UdFloor, "d_a_obj_lv8UdFloor"},
    {PROC_Obj_Lv9SwShutter, "d_a_obj_lv9SwShutter"},
    {PROC_Obj_TobyHouse, "d_a_obj_tobyhouse"},
    {PROC_Obj_poCandle, "d_a_obj_poCandle"},
    {PROC_Obj_Lv4DigSand, "d_a_obj_lv4digsand"},
    {PROC_Obj_FallObj, "d_a_obj_fallobj"},
    {PROC_Obj_SmgDoor, "d_a_obj_smgdoor"},
    {PROC_Obj_SwLight, "d_a_obj_swLight"},
    {PROC_Obj_Avalanche, "d_a_obj_avalanche"},
    {PROC_Obj_MirrorScrew, "d_a_obj_mirror_screw"},
    {PROC_Obj_MirrorSand, "d_a_obj_mirror_sand"},
    {PROC_Obj_MirrorTable, "d_a_obj_mirror_table"},
    {PROC_Obj_MirrorChain, "d_a_obj_mirror_chain"},
    {PROC_Obj_Mirror6Pole, "d_a_obj_mirror_6pole"},
    {PROC_Obj_SwSpinner, "d_a_obj_swspinner"},
    {PROC_Obj_TDoor, "d_a_obj_thdoor"},
    {PROC_Obj_Lv7Bridge, "d_a_obj_lv7bridge"},
    {PROC_Obj_zrTurara, "d_a_obj_zrTurara"},
    {PROC_Obj_TakaraDai, "d_a_obj_takaraDai"},
    {PROC_Obj_Table, "d_a_obj_table"},
    {PROC_Obj_CatDoor, "d_a_obj_catdoor"},
    {PROC_Obj_Gake, "d_a_obj_tgake"},
    {PROC_CSTAF, "d_a_cstaF"},
    {PROC_Obj_Lv4RailWall, "d_a_obj_lv4RailWall"},
    {PROC_Obj_Lv4Sand, "d_a_obj_lv4sand"},
    {PROC_Obj_PushDoor, "d_a_obj_pdoor"},
    {PROC_PushDoor, "d_a_door_push"},
    {PROC_Obj_GanonWall2, "d_a_obj_ganonwall2"},
    {PROC_Obj_Lv4Bridge, "d_a_obj_lv4bridge"},
    {PROC_Obj_Lv4Floor, "d_a_obj_lv4floor"},
    {PROC_Tag_Spinner, "d_a_tag_spinner"},
    {PROC_Obj_SwHang, "d_a_obj_swhang"},
    {PROC_Obj_RotStair, "d_a_obj_rstair"},
    {PROC_Obj_MagneArm, "d_a_obj_magne_arm"},
    {PROC_Obj_KWheel00, "d_a_obj_kwheel00"},
    {PROC_Obj_KWheel01, "d_a_obj_kwheel01"},
    {PROC_Obj_Ychndlr, "d_a_obj_lv5ychndlr"},
    {PROC_Obj_PRElvtr, "d_a_obj_lv4prelvtr"},
    {PROC_Obj_MHasu, "d_a_obj_hasu2"},
    {PROC_Obj_YIblltray, "d_a_obj_lv5yiblltray"},
    {PROC_Obj_Lv6EGate, "d_a_obj_lv6egate"},
    {PROC_Obj_PDtile, "d_a_obj_pdtile"},
    {PROC_Obj_PDwall, "d_a_obj_pdwall"},
    {PROC_Obj_Lv4PRwall, "d_a_obj_lv4prwall"},
    {PROC_Obj_KLift00, "d_a_obj_klift00"},
    {PROC_B_OH, "d_a_b_oh"},
    {PROC_Obj_Lv4Chan, "d_a_obj_lv4chandelier"},
    {PROC_Obj_Lv3R10Saka, "d_a_obj_lv3saka00"},
    {PROC_Obj_Lv3Water, "d_a_obj_lv3Water"},
    {PROC_Obj_Lv3Water2, "d_a_obj_lv3Water2"},
    {PROC_OBJ_LV3WATERB, "d_a_obj_lv3WaterB"},
    {PROC_Obj_HBombkoya, "d_a_obj_hbombkoya"},
    {PROC_Obj_SZbridge, "d_a_obj_szbridge"},
    {PROC_Obj_KakarikoBrg, "d_a_obj_warp_kbrg"},
    {PROC_Obj_OrdinBrg, "d_a_obj_warp_obrg"},
    {PROC_Obj_BurnBox, "d_a_obj_burnbox"},
    {PROC_Obj_KJgjs, "d_a_obj_kjgjs"},
    {PROC_OBJ_IHASI, "d_a_obj_ihasi"},
    {PROC_Obj_IceBlock, "d_a_obj_iceblock"},
    {PROC_Obj_VolcanicBall, "d_a_obj_volcball"},
    {PROC_Obj_VolcanicBomb, "d_a_obj_volcbom"},
    {PROC_Obj_VolcGnd, "d_a_obj_vground"},
    {PROC_Obj_KKanban, "d_a_obj_kkanban"},
    {PROC_E_PH, "d_a_e_ph"},
    {PROC_NPC_ZRA, "d_a_npc_zra"},
    {PROC_Obj_Chandelier, "d_a_obj_chandelier"},
    {PROC_Obj_Stopper2, "d_a_obj_stopper2"},
    {PROC_DOOR20, "d_a_door_shutter"},
    {PROC_Tag_Hinit, "d_a_tag_hinit"},
    {PROC_Tag_Hjump, "d_a_tag_hjump"},
    {PROC_Tag_AJnot, "d_a_tag_ajnot"},
    {PROC_Tag_Hstop, "d_a_tag_hstop"},
    {PROC_CANOE, "d_a_canoe"},
    {PROC_HORSE, "d_a_horse"},
    {PROC_E_WB, "d_a_e_wb"},
    {PROC_OBJ_ITO, "d_a_obj_ito"},
    {PROC_OBJ_SW, "d_a_obj_sw"},
    {PROC_SPINNER, "d_a_spinner"},
    {PROC_B_OB, "d_a_b_ob"},
    {PROC_KAGO, "d_a_kago"},
    {PROC_E_YC, "d_a_e_yc"},
    {PROC_B_DS, "d_a_b_ds"},
    {PROC_B_DR, "d_a_b_dr"},
    {PROC_B_ZANTZ, "d_a_b_zant_mobile"},
    {PROC_B_ZANT, "d_a_b_zant"},
    {PROC_B_ZANTM, "d_a_b_zant_magic"},
    {PROC_TBOX, "d_a_tbox"},
    {PROC_TBOX2, "d_a_tbox2"},
    {PROC_BOOMERANG, "d_a_boomerang"},
    {PROC_MIDNA, "d_a_midna"},
    {PROC_NPC_TK, "d_a_npc_tk"},
    {PROC_NPC_WORM, "d_a_npc_worm"},
    {PROC_PPolamp, "d_a_ppolamp"},
    {PROC_BkyRock, "d_a_obj_bky_rock"},
    {PROC_HITOBJ, "d_a_hitobj"},
    {PROC_EP, "d_a_ep"},
    {PROC_COW, "d_a_cow"},
    {PROC_PERU, "d_a_peru"},
    {PROC_NI, "d_a_ni"},
    {PROC_NPC_TKJ2, "d_a_npc_tkj2"},
    {PROC_SQ, "d_a_sq"},
    {PROC_NPC_SQ, "d_a_npc_sq"},
    {PROC_DO, "d_a_do"},
    {PROC_NPC_NE, "d_a_npc_ne"},
    {PROC_NPC_TR, "d_a_npc_tr"},
    {PROC_NPC_LF, "d_a_npc_lf"},
    {PROC_OBJ_FOOD, "d_a_obj_food"},
    {PROC_OBJ_KI, "d_a_obj_ki"},
    {PROC_OBJ_KITA, "d_a_obj_kita"},
    {PROC_OBJ_KEY, "d_a_obj_key"},
    {PROC_OBJ_KEYHOLE, "d_a_obj_keyhole"},
    {PROC_Obj_Lv5Key, "d_a_obj_Lv5Key"},
    {PROC_OBJ_LP, "d_a_obj_lp"},
    {PROC_OBJ_TATIGI, "d_a_obj_tatigi"},
    {PROC_OBJ_ROCK, "d_a_obj_rock"},
    {PROC_OBJ_WFLAG, "d_a_obj_wflag"},
    {PROC_OBJ_KAGE, "d_a_obj_kage"},
    {PROC_OBJ_KANBAN2, "d_a_obj_kanban2"},
    {PROC_OBJ_BALLOON, "d_a_obj_balloon"},
    {PROC_OBJ_SUISYA, "d_a_obj_suisya"},
    {PROC_OBJ_OILTUBO, "d_a_obj_oiltubo"},
    {PROC_OBJ_ROTEN, "d_a_obj_roten"},
    {PROC_OBJ_SSDRINK, "d_a_obj_ss_drink"},
    {PROC_OBJ_SSITEM, "d_a_obj_ss_item"},
    {PROC_TAG_SSDRINK, "d_a_tag_ss_drink"},
    {PROC_TAG_BTLITM, "d_a_tag_bottle_item"},
    {PROC_TAG_LV5SOUP, "d_a_tag_lv5soup"},
    {PROC_TAG_MNLIGHT, "d_a_tag_myna_light"},
    {PROC_TAG_SHOPCAM, "d_a_tag_shop_camera"},
    {PROC_TAG_SHOPITM, "d_a_tag_shop_item"},
    {PROC_OBJ_NDOOR, "d_a_obj_ndoor"},
    {PROC_OBJ_UDOOR, "d_a_obj_udoor"},
    {PROC_OBJ_USAKU, "d_a_obj_usaku"},
    {PROC_Obj_SM_DOOR, "d_a_obj_sm_door"},
    {PROC_OBJ_BED, "d_a_obj_bed"},
    {PROC_OBJ_BOUMATO, "d_a_obj_boumato"},
    {PROC_OBJ_ITAMATO, "d_a_obj_itamato"},
    {PROC_OBJ_NOUGU, "d_a_obj_nougu"},
    {PROC_OBJ_STICK, "d_a_obj_stick"},
    {PROC_OBJ_MIE, "d_a_obj_mie"},
    {PROC_OBJ_SEKIDOOR, "d_a_obj_sekidoor"},
    {PROC_OBJ_SEKIZO, "d_a_obj_sekizo"},
    {PROC_OBJ_SMTILE, "d_a_obj_smtile"},
    {PROC_NPC_FISH, "d_a_npc_fish"},
    {PROC_MG_FISH, "d_a_mg_fish"},
    {PROC_FSHOP, "d_a_mg_fshop"},
    {PROC_NPC_DU, "d_a_npc_du"},
    {PROC_DISAPPEAR, "d_a_disappear"},
    {PROC_Obj_Mato, "d_a_obj_mato"},
    {PROC_Obj_Flag, "d_a_obj_flag"},
    {PROC_Obj_Flag2, "d_a_obj_flag2"},
    {PROC_Obj_Flag3, "d_a_obj_flag3"},
    {PROC_Obj_GOMIKABE, "d_a_obj_gomikabe"},
    {PROC_Obj_Yousei, "d_a_obj_yousei"},
    {PROC_Obj_Kabuto, "d_a_obj_kabuto"},
    {PROC_Obj_Cho, "d_a_obj_cho"},
    {PROC_Obj_Kuw, "d_a_obj_kuwagata"},
    {PROC_Obj_Nan, "d_a_obj_nan"},
    {PROC_Obj_Dan, "d_a_obj_dan"},
    {PROC_Obj_Kam, "d_a_obj_kamakiri"},
    {PROC_Obj_Ten, "d_a_obj_ten"},
    {PROC_Obj_Ari, "d_a_obj_ari"},
    {PROC_Obj_Kag, "d_a_obj_kag"},
    {PROC_Obj_Batta, "d_a_obj_batta"},
    {PROC_Obj_Tombo, "d_a_obj_tombo"},
    {PROC_Obj_Kat, "d_a_obj_katatsumuri"},
    {PROC_Obj_H_Saku, "d_a_obj_h_saku"},
    {PROC_Obj_Yobikusa, "d_a_obj_yobikusa"},
    {PROC_Obj_KazeNeko, "d_a_obj_kazeneko"},
    {PROC_Obj_KznkArm, "d_a_obj_kznkarm"},
    {PROC_Obj_NamePlate, "d_a_obj_nameplate"},
    {PROC_Obj_OnCloth, "d_a_obj_ornament_cloth"},
    {PROC_Obj_LndRope, "d_a_obj_laundry_rope"},
    {PROC_Obj_ItaRope, "d_a_obj_sakuita_rope"},
    {PROC_Obj_Sakuita, "d_a_obj_sakuita"},
    {PROC_Obj_Laundry, "d_a_obj_laundry"},
    {PROC_WarpBug, "d_a_warp_bug"},
    {PROC_Izumi_Gate, "d_a_izumi_gate"},
    {PROC_Obj_Fchain, "d_a_obj_fchain"},
    {PROC_Obj_Wchain, "d_a_obj_wchain"},
    {PROC_Tag_Attp, "d_a_tag_attention"},
    {PROC_Obj_Tornado, "d_a_obj_tornado"},
    {PROC_Obj_Tornado2, "d_a_obj_tornado2"},
    {PROC_Obj_FirePillar, "d_a_obj_firepillar"},
    {PROC_Obj_FirePillar2, "d_a_obj_firepillar2"},
    {PROC_Obj_InoBone, "d_a_obj_inobone"},
    {PROC_Obj_Stopper, "d_a_obj_stopper"},
    {PROC_Obj_MHole, "d_a_obj_mhole"},
    {PROC_Tag_Magne, "d_a_tag_magne"},
    {PROC_Obj_BossWarp, "d_a_obj_bosswarp"},
    {PROC_Obj_WoodPendulum, "d_a_obj_wood_pendulum"},
    {PROC_Obj_WdStick, "d_a_obj_wdStick"},
    {PROC_Obj_StairBlock, "d_a_obj_stairBlock"},
    {PROC_Obj_Geyser, "d_a_obj_geyser"},
    {PROC_Tag_KtOnFire, "d_a_obj_ktOnFire"},
    {PROC_Obj_FireWood, "d_a_obj_fireWood"},
    {PROC_Obj_FireWood2, "d_a_obj_fireWood2"},
    {PROC_Obj_GpTaru, "d_a_obj_gpTaru"},
    {PROC_Obj_OnsenTaru, "d_a_obj_onsenTaru"},
    {PROC_Obj_KiPot, "d_a_obj_kiPot"},
    {PROC_TBOX_SW, "d_a_tboxSw"},
    {PROC_Obj_SwChain, "d_a_obj_swchain"},
    {PROC_Obj_WoodenSword, "d_a_obj_wsword"},
    {PROC_Obj_StoneMark, "d_a_obj_stoneMark"},
    {PROC_Obj_Lv3Candle, "d_a_obj_lv3Candle"},
    {PROC_Tag_Lv4Candle, "d_a_obj_lv4CandleTag"},
    {PROC_Tag_Lv4CandleDm, "d_a_obj_lv4CandleDemoTag"},
    {PROC_Obj_DamCps, "d_a_obj_damCps"},
    {PROC_Obj_Smoke, "d_a_obj_smoke"},
    {PROC_Obj_WaterFall, "d_a_obj_waterfall"},
    {PROC_Obj_ZoraCloth, "d_a_obj_zcloth"},
    {PROC_Obj_poFire, "d_a_obj_poFire"},
    {PROC_Tag_poFire, "d_a_tag_poFire"},
    {PROC_Obj_glowSphere, "d_a_obj_glowSphere"},
    {PROC_Tag_LightBall, "d_a_tag_lightball"},
    {PROC_SwLBall, "d_a_swLBall"},
    {PROC_SwBall, "d_a_swBall"},
    {PROC_Obj_WaterEff, "d_a_obj_lv3waterEff"},
    {PROC_Tag_RiverBack, "d_a_tag_river_back"},
    {PROC_Tag_KagoFall, "d_a_tag_kago_fall"},
    {PROC_Tag_Lv2PrChk, "d_a_tag_lv2prchk"},
    {PROC_Obj_Lv4Gear, "d_a_obj_lv4gear"},
    {PROC_Obj_MasterSword, "d_a_obj_master_sword"},
    {PROC_Obj_WoodStatue, "d_a_obj_wood_statue"},
    {PROC_Obj_Fan, "d_a_obj_fan"},
    {PROC_Obj_IceLeaf, "d_a_obj_iceleaf"},
    {PROC_Obj_zrTuraraRc, "d_a_obj_zrTuraraRock"},
    {PROC_Tag_RetRoom, "d_a_tag_ret_room"},
    {PROC_Obj_WindStone, "d_a_obj_wind_stone"},
    {PROC_Tag_WaraHowl, "d_a_tag_wara_howl"},
    {PROC_Obj_SCannon, "d_a_obj_scannon"},
    {PROC_Obj_SmWStone, "d_a_obj_smw_stone"},
    {PROC_Obj_SCannonCrs, "d_a_obj_scannon_crs"},
    {PROC_Tag_SnowEff, "d_a_obj_snowEffTag"},
    {PROC_Tag_CstaSw, "d_a_tag_CstaSw"},
    {PROC_Tag_Lv6CstaSw, "d_a_tag_lv6CstaSw"},
    {PROC_Obj_awaPlar, "d_a_obj_bubblePilar"},
    {PROC_Obj_poTbox, "d_a_obj_poTbox"},
    {PROC_Obj_TimeFire, "d_a_obj_timeFire"},
    {PROC_Obj_TMoon, "d_a_obj_tmoon"},
    {PROC_Obj_GanonWall, "d_a_obj_ganonwall"},
    {PROC_Obj_Prop, "d_a_obj_prop"},
    {PROC_CSTATUE, "d_a_cstatue"},
    {PROC_Obj_SwBallA, "d_a_obj_swBallA"},
    {PROC_Obj_SwBallB, "d_a_obj_swBallB"},
    {PROC_Obj_SnowSoup, "d_a_obj_snow_soup"},
    {PROC_Obj_Nagaisu, "d_a_obj_nagaisu"},
    {PROC_Obj_RCircle, "d_a_obj_rcircle"},
    {PROC_Obj_Picture, "d_a_obj_picture"},
    {PROC_Tag_SetBall, "d_a_tag_setBall"},
    {PROC_Tag_SmkEmt, "d_a_tag_smk_emt"},
    {PROC_SwTime, "d_a_swTime"},
    {PROC_Obj_HFtr, "d_a_obj_hakai_ftr"},
    {PROC_Obj_HBarrel, "d_a_obj_hakai_brl"},
    {PROC_Obj_Crystal, "d_a_obj_crystal"},
    {PROC_Obj_SCannonTen, "d_a_obj_scannon_ten"},
    {PROC_Obj_SwBallC, "d_a_obj_swBallC"},
    {PROC_SCENE_EXIT2, "d_a_scene_exit2"},
    {PROC_Obj_Hata, "d_a_obj_hata"},
    {PROC_Obj_ToaruMaki, "d_a_obj_toaru_maki"},
    {PROC_Tag_AttackItem, "d_a_tag_attack_item"},
    {PROC_Tag_RmbitSw, "d_a_tag_rmbit_sw"},
    {PROC_Obj_Sword, "d_a_obj_sword"},
    {PROC_Tag_Spring, "d_a_tag_spring"},
    {PROC_Tag_Statue, "d_a_tag_statue_evt"},
    {PROC_E_AI, "d_a_e_ai"},
    {PROC_E_GS, "d_a_e_gs"},
    {PROC_E_GOB, "d_a_e_gob"},
    {PROC_E_DD, "d_a_e_dd"},
    {PROC_E_DN, "d_a_e_dn"},
    {PROC_E_S1, "d_a_e_s1"},
    {PROC_E_MF, "d_a_e_mf"},
    {PROC_E_SG, "d_a_e_sg"},
    {PROC_E_BS, "d_a_e_bs"},
    {PROC_E_SF, "d_a_e_sf"},
    {PROC_E_SH, "d_a_e_sh"},
    {PROC_E_DF, "d_a_e_df"},
    {PROC_E_GM, "d_a_e_gm"},
    {PROC_E_MD, "d_a_e_md"},
    {PROC_E_SM, "d_a_e_sm"},
    {PROC_E_SM2, "d_a_e_sm2"},
    {PROC_E_ST, "d_a_e_st"},
    {PROC_E_ST_LINE, "d_a_e_st_line"},
    {PROC_E_SB, "d_a_e_sb"},
    {PROC_E_TH, "d_a_e_th"},
    {PROC_E_CR, "d_a_e_cr"},
    {PROC_E_CR_EGG, "d_a_e_cr_egg"},
    {PROC_E_DB, "d_a_e_db"},
    {PROC_E_DB_LEAF, "d_a_e_db_leaf"},
    {PROC_E_GA, "d_a_e_ga"},
    {PROC_E_GB, "d_a_e_gb"},
    {PROC_E_HB, "d_a_e_hb"},
    {PROC_E_HB_LEAF, "d_a_e_hb_leaf"},
    {PROC_E_HZELDA, "d_a_e_hzelda"},
    {PROC_E_YD, "d_a_e_yd"},
    {PROC_E_YH, "d_a_e_yh"},
    {PROC_E_YD_LEAF, "d_a_e_yd_leaf"},
    {PROC_E_HM, "d_a_e_hm"},
    {PROC_E_TK, "d_a_e_tk"},
    {PROC_E_TK2, "d_a_e_tk2"},
    {PROC_E_TK_BALL, "d_a_e_tk_ball"},
    {PROC_E_RB, "d_a_e_rb"},
    {PROC_E_RD, "d_a_e_rd"},
    {PROC_E_RDB, "d_a_e_rdb"},
    {PROC_E_RDY, "d_a_e_rdy"},
    {PROC_E_FM, "d_a_e_fm"},
    {PROC_E_FS, "d_a_e_fs"},
    {PROC_E_PM, "d_a_e_pm"},
    {PROC_E_PO, "d_a_e_po"},
    {PROC_E_MB, "d_a_e_mb"},
    {PROC_E_MK, "d_a_e_mk"},
    {PROC_E_MM, "d_a_e_mm"},
    {PROC_E_FZ, "d_a_e_fz"},
    {PROC_E_ZS, "d_a_e_zs"},
    {PROC_E_KK, "d_a_e_kk"},
    {PROC_E_HP, "d_a_e_hp"},
    {PROC_E_ZH, "d_a_e_zh"},
    {PROC_E_ZM, "d_a_e_zm"},
    {PROC_E_PZ, "d_a_e_pz"},
    {PROC_E_FB, "d_a_e_fb"},
    {PROC_E_FK, "d_a_e_fk"},
    {PROC_E_MS, "d_a_e_ms"},
    {PROC_E_NEST, "d_a_e_nest"},
    {PROC_E_NZ, "d_a_e_nz"},
    {PROC_E_BA, "d_a_e_ba"},
    {PROC_E_BU, "d_a_e_bu"},
    {PROC_E_BUG, "d_a_e_bug"},
    {PROC_E_BEE, "d_a_e_bee"},
    {PROC_E_IS, "d_a_e_is"},
    {PROC_E_KG, "d_a_e_kg"},
    {PROC_E_KR, "d_a_e_kr"},
    {PROC_E_SW, "d_a_e_sw"},
    {PROC_E_GE, "d_a_e_ge"},
    {PROC_Tag_WatchGe, "d_a_tag_watchge"},
    {PROC_E_YM, "d_a_e_ym"},
    {PROC_E_YM_TAG, "d_a_e_ym_tag"},
    {PROC_E_YMB, "d_a_e_ymb"},
    {PROC_Tag_FWall, "d_a_tag_firewall"},
    {PROC_Tag_WaterFall, "d_a_tag_waterfall"},
    {PROC_E_YK, "d_a_e_yk"},
    {PROC_E_YR, "d_a_e_yr"},
    {PROC_E_YG, "d_a_e_yg"},
    {PROC_E_HZ, "d_a_e_hz"},
    {PROC_E_WS, "d_a_e_ws"},
    {PROC_E_OC, "d_a_e_oc"},
    {PROC_E_OT, "d_a_e_ot"},
    {PROC_E_DT, "d_a_e_dt"},
    {PROC_E_BG, "d_a_e_bg"},
    {PROC_E_OctBg, "d_a_e_oct_bg"},
    {PROC_DR, "d_a_L7demo_dr"},
    {PROC_L7lowDr, "d_a_L7low_dr"},
    {PROC_L7ODR, "d_a_L7op_demo_dr"},
    {PROC_E_TT, "d_a_e_tt"},
    {PROC_E_DK, "d_a_e_dk"},
    {PROC_E_VT, "d_a_e_vt"},
    {PROC_E_WW, "d_a_e_ww"},
    {PROC_E_GI, "d_a_e_gi"},
    {PROC_B_BH, "d_a_b_bh"},
    {PROC_B_BQ, "d_a_b_bq"},
    {PROC_B_GM, "d_a_b_gm"},
    {PROC_B_GND, "d_a_b_gnd"},
    {PROC_B_GO, "d_a_b_go"},
    {PROC_B_OH2, "d_a_b_oh2"},
    {PROC_B_YO, "d_a_b_yo"},
    {PROC_B_YOI, "d_a_b_yo_ice"},
    {PROC_B_TN, "d_a_b_tn"},
    {PROC_B_GG, "d_a_b_gg"},
    {PROC_B_DRE, "d_a_b_dre"},
    {PROC_B_MGN, "d_a_b_mgn"},
    {PROC_E_WAP, "d_a_e_warpappear"},
    {PROC_Obj_SmallKey, "d_a_obj_smallkey"},
    {PROC_Obj_Kantera, "d_a_obj_kantera"},
    {PROC_Obj_LifeContainer, "d_a_obj_life_container"},
    {PROC_Obj_Shield, "d_a_obj_shield"},
    {PROC_Demo_Item, "d_a_demo_item"},
    #if !PLATFORM_SHIELD
    {PROC_ShopItem, "d_a_shop_item"},
    #endif
    {PROC_Obj_Drop, "d_a_obj_drop"},
    {PROC_OBJ_RW, "d_a_obj_rw"},
    {PROC_NBOMB, "d_a_nbomb"},
    {PROC_TAG_CSW, "d_a_tag_csw"},
    {PROC_TAG_QS, "d_a_tag_qs"},
    {PROC_HOZELDA, "d_a_hozelda"},
    #if !PLATFORM_SHIELD
    {PROC_SWC00, "d_a_swc00"},
    #endif
    {PROC_KNOB20, "d_a_door_knob00"},
    {PROC_DBDOOR, "d_a_door_dbdoor00"},
    {PROC_BOSS_DOOR, "d_a_door_boss"},
    {PROC_L1BOSS_DOOR, "d_a_door_bossL1"},
    {PROC_L1MBOSS_DOOR, "d_a_door_mbossL1"},
    {PROC_L5BOSS_DOOR, "d_a_door_bossL5"},
    {PROC_DSHUTTER, "d_a_dshutter"},
    {PROC_SPIRAL_DOOR, "d_a_door_spiral"},
    {PROC_Tag_ChgRestart, "d_a_tag_chgrestart"},
    {PROC_Tag_Restart, "d_a_tag_setrestart"},
    {PROC_ANDSW, "d_a_andsw"},
    #if !PLATFORM_SHIELD
    {PROC_ANDSW2, "d_a_andsw2"},
    #endif
    {PROC_MYNA, "d_a_myna"},
    {PROC_NPC_GND, "d_a_npc_gnd"},
    {PROC_NPC_GRA, "d_a_npc_gra"},
    {PROC_NPC_GRC, "d_a_npc_grc"},
    {PROC_NPC_GRD, "d_a_npc_grd"},
    {PROC_NPC_GRM, "d_a_npc_grm"},
    {PROC_NPC_GRMC, "d_a_npc_grmc"},
    {PROC_NPC_GRO, "d_a_npc_gro"},
    {PROC_NPC_GRR, "d_a_npc_grr"},
    {PROC_NPC_GRS, "d_a_npc_grs"},
    {PROC_NPC_GRZ, "d_a_npc_grz"},
    {PROC_NPC_YAMID, "d_a_npc_yamid"},
    {PROC_NPC_YAMIT, "d_a_npc_yamit"},
    {PROC_NPC_YAMIS, "d_a_npc_yamis"},
    {PROC_NPC_BLUENS, "d_a_npc_blue_ns"},
    {PROC_NPC_KAKASHI, "d_a_npc_kakashi"},
    {PROC_NPC_KDK, "d_a_npc_kdk"},
    {PROC_NPC_ARU, "d_a_npc_aru"},
    {PROC_NPC_BANS, "d_a_npc_bans"},
    {PROC_NPC_BESU, "d_a_npc_besu"},
    {PROC_NPC_BOU, "d_a_npc_bou"},
    {PROC_NPC_BOU_S, "d_a_npc_bouS"},
    {PROC_NPC_CLERKA, "d_a_npc_clerka"},
    {PROC_NPC_CLERKB, "d_a_npc_clerkb"},
    {PROC_NPC_CLERKT, "d_a_npc_clerkt"},
    {PROC_NPC_WRESTLER, "d_a_npc_wrestler"},
    {PROC_Tag_Arena, "d_a_tag_arena"},
    {PROC_Tag_Instruction, "d_a_tag_instruction"},
    {PROC_NPC_DOC, "d_a_npc_doc"},
    {PROC_NPC_GWOLF, "d_a_npc_gwolf"},
    {PROC_NPC_LEN, "d_a_npc_len"},
    {PROC_NPC_LUD, "d_a_npc_lud"},
    {PROC_NPC_FAIRY_SEIREI, "d_a_npc_fairy_seirei"},
    {PROC_NPC_FAIRY, "d_a_npc_fairy"},
    {PROC_NPC_HANJO, "d_a_npc_hanjo"},
    {PROC_NPC_HENNA, "d_a_npc_henna"},
    {PROC_NPC_HENNA0, "d_a_npc_henna0"},
    {PROC_NPC_HOZ, "d_a_npc_hoz"},
    {PROC_NPC_JAGAR, "d_a_npc_jagar"},
    {PROC_NPC_KKRI, "d_a_npc_kkri"},
    {PROC_NPC_KN, "d_a_npc_kn"},
    {PROC_KN_BULLET, "d_a_obj_knBullet"},
    {PROC_NPC_KNJ, "d_a_npc_knj"},
    {PROC_NPC_KOLIN, "d_a_npc_kolin"},
    {PROC_NPC_KOLINB, "d_a_npc_kolinb"},
    {PROC_NPC_KYURY, "d_a_npc_kyury"},
    {PROC_NPC_MARO, "d_a_npc_maro"},
    {PROC_NPC_MIDP, "d_a_npc_midp"},
    {PROC_NPC_MOI, "d_a_npc_moi"},
    {PROC_NPC_RACA, "d_a_npc_raca"},
    {PROC_NPC_SARU, "d_a_npc_saru"},
    {PROC_NPC_SEIB, "d_a_npc_seib"},
    {PROC_NPC_SEIC, "d_a_npc_seic"},
    {PROC_NPC_SEID, "d_a_npc_seid"},
    {PROC_NPC_SEIRA, "d_a_npc_seira"},
    {PROC_NPC_SERA2, "d_a_npc_seira2"},
    {PROC_NPC_SEIREI, "d_a_npc_seirei"},
    {PROC_NPC_SHAMAN, "d_a_npc_shaman"},
    {PROC_NPC_SMARO, "d_a_npc_shop_maro"},
    {PROC_NPC_SOLA, "d_a_npc_sola"},
    {PROC_NPC_TARO, "d_a_npc_taro"},
    {PROC_NPC_PACHI_BESU, "d_a_npc_pachi_besu"},
    {PROC_NPC_PACHI_TARO, "d_a_npc_pachi_taro"},
    {PROC_NPC_PACHI_MARO, "d_a_npc_pachi_maro"},
    {PROC_TAG_PATI, "d_a_tag_pachi"},
    {PROC_NPC_THE, "d_a_npc_the"},
    {PROC_NPC_TKJ, "d_a_npc_tkj"},
    {PROC_NPC_TKS, "d_a_npc_tks"},
    {PROC_NPC_TKC, "d_a_npc_tkc"},
    {PROC_OBJ_TKS, "d_a_obj_tks"},
    {PROC_NPC_TOBY, "d_a_npc_toby"},
    {PROC_NPC_URI, "d_a_npc_uri"},
    {PROC_NPC_YELIA, "d_a_npc_yelia"},
    {PROC_NPC_YKM, "d_a_npc_ykm"},
    {PROC_NPC_YKW, "d_a_npc_ykw"},
    {PROC_NPC_ZANB, "d_a_npc_zanb"},
    {PROC_NPC_ZANT, "d_a_npc_zant"},
    {PROC_NPC_ZELDA, "d_a_npc_zelda"},
    {PROC_NPC_ZELR, "d_a_npc_zelR"},
    {PROC_NPC_ZELRO, "d_a_npc_zelRo"},
    {PROC_OBJ_ZRAFREEZE, "d_a_obj_zra_freeze"},
    {PROC_NPC_ZRC, "d_a_npc_zrc"},
    {PROC_NPC_ZRZ, "d_a_npc_zrz"},
    {PROC_ZRA_MARK, "d_a_obj_zraMark"},
    {PROC_MYNA2, "d_a_npc_myna2"},
    {PROC_TAG_MYNA2, "d_a_tag_myna2"},
    {PROC_NPC_CD3, "d_a_npc_cdn3"},
    {PROC_Tag_Schedule, "d_a_tag_schedule"},
    {PROC_Tag_Escape, "d_a_tag_escape"},
    {PROC_NPC_CHAT, "d_a_npc_chat"},
    {PROC_NPC_SOLDIERa, "d_a_npc_soldierA"},
    {PROC_NPC_SOLDIERb, "d_a_npc_soldierB"},
    {PROC_PASSER_MNG, "d_a_passer_mng"},
    {PROC_NPC_PASSER, "d_a_npc_passer"},
    {PROC_NPC_PASSER2, "d_a_npc_passer2"},
    {PROC_NPC_POST, "d_a_npc_post"},
    {PROC_NPC_POUYA, "d_a_npc_pouya"},
    {PROC_FORMATION_MNG, "d_a_formation_mng"},
    {PROC_NPC_FGUARD, "d_a_npc_fguard"},
    {PROC_GUARD_MNG, "d_a_guard_mng"},
    {PROC_TAG_GUARD, "d_a_tag_guard"},
    {PROC_NPC_GUARD, "d_a_npc_guard"},
    {PROC_NPC_ASH, "d_a_npc_ash"},
    {PROC_NPC_ASHB, "d_a_npc_ashB"},
    {PROC_NPC_SHAD, "d_a_npc_shad"},
    {PROC_NPC_RAFREL, "d_a_npc_rafrel"},
    {PROC_NPC_MOIR, "d_a_npc_moir"},
    {PROC_NPC_IMPAL, "d_a_npc_impal"},
    {PROC_NPC_SHOE, "d_a_npc_shoe"},
    {PROC_NPC_DOORBOY, "d_a_npc_doorboy"},
    {PROC_NPC_PRAYER, "d_a_npc_prayer"},
    {PROC_NPC_KASIHANA, "d_a_npc_kasi_hana"},
    {PROC_NPC_KASIKYU, "d_a_npc_kasi_kyu"},
    {PROC_NPC_KASIMICH, "d_a_npc_kasi_mich"},
    {PROC_NPC_DRSOL, "d_a_npc_drainSol"},
    {PROC_NPC_CHIN, "d_a_npc_chin"},
    {PROC_NPC_INS, "d_a_npc_ins"},
    {PROC_NPC_SHOP0, "d_a_npc_shop0"},
    #if !PLATFORM_SHIELD
    {PROC_NPC_MK, "d_a_npc_mk"},
    #endif
    {PROC_NPC_P2, "d_a_npc_p2"},
    {PROC_KYTAG00, "d_a_kytag00"},
    #if !PLATFORM_SHIELD
    {PROC_KYTAG01, "d_a_kytag01"},
    #endif
    {PROC_KYTAG02, "d_a_kytag02"},
    {PROC_KYTAG03, "d_a_kytag03"},
    {PROC_KYTAG04, "d_a_kytag04"},
    {PROC_KYTAG05, "d_a_kytag05"},
    {PROC_KYTAG06, "d_a_kytag06"},
    {PROC_KYTAG07, "d_a_kytag07"},
    {PROC_KYTAG08, "d_a_kytag08"},
    {PROC_KYTAG09, "d_a_kytag09"},
    {PROC_KYTAG10, "d_a_kytag10"},
    {PROC_KYTAG11, "d_a_kytag11"},
    {PROC_KYTAG12, "d_a_kytag12"},
    {PROC_KYTAG13, "d_a_kytag13"},
    {PROC_KYTAG14, "d_a_kytag14"},
    {PROC_KYTAG15, "d_a_kytag15"},
    {PROC_KYTAG16, "d_a_kytag16"},
    {PROC_KYTAG17, "d_a_kytag17"},
    {PROC_Ykgr, "d_a_ykgr"},
    {PROC_TALK, "d_a_talk"},
    {PROC_Obj_Crope, "d_a_obj_crope"},
    {PROC_Obj_Bombf, "d_a_obj_bombf"},
    {PROC_Obj_BkLeaf, "d_a_obj_bk_leaf"},
    {PROC_Tag_Mhint, "d_a_tag_mhint"},
    {PROC_Tag_Mmsg, "d_a_tag_mmsg"},
    {PROC_Tag_Mwait, "d_a_tag_mwait"},
    {PROC_Tag_Mstop, "d_a_tag_mstop"},
    {PROC_Tag_Stream, "d_a_tag_stream"},
    {PROC_Tag_Sppath, "d_a_tag_sppath"},
    {PROC_Tag_Wljump, "d_a_tag_wljump"},
    {PROC_Tag_TWGate, "d_a_tag_TWgate"},
    {PROC_Tag_Lv6Gate, "d_a_tag_Lv6Gate"},
    {PROC_Tag_Lv7Gate, "d_a_tag_Lv7Gate"},
    {PROC_Tag_Lv8Gate, "d_a_tag_Lv8Gate"},
    {PROC_Tag_TheBHint, "d_a_tag_theB_hint"},
    {PROC_Tag_Assist, "d_a_tag_assistance"},
    {PROC_DEMO00, "d_a_demo00"},
    {PROC_TAG_CAMERA, "d_a_tag_camera"},
    {PROC_TAG_CHKPOINT, "d_a_tag_chkpoint"},
    #if !PLATFORM_SHIELD
    {PROC_TAG_EVENT, "d_a_tag_event"},
    #endif
    {PROC_TAG_EVT, "d_a_tag_evt"},
    {PROC_TAG_TELOP, "d_a_tag_telop"},
    {PROC_TAG_HOWL, "d_a_tag_howl"},
    #if !PLATFORM_SHIELD
    {PROC_TAG_MSG, "d_a_tag_msg"},
    #endif
    {PROC_TAG_LANTERN, "d_a_tag_lantern"},
    {PROC_Tag_Mist, "d_a_tag_mist"},
    {PROC_DMIDNA, "d_a_dmidna"},
    {PROC_VRBOX, "d_a_vrbox"},
    {PROC_VRBOX2, "d_a_vrbox2"},
    {PROC_BG, "d_a_bg"},
    {PROC_SET_BG_OBJ, "d_a_set_bgobj"},
    {PROC_BG_OBJ, "d_a_bg_obj"},
    {PROC_MIRROR, "d_a_mirror"},
    {PROC_MOVIE_PLAYER, "d_a_movie_player"},
    #if !PLATFORM_SHIELD
    {PROC_TITLE, "d_a_title"},
    #endif
    {PROC_FR, "d_a_fr"},
    {PROC_ECONT, "d_a_econt"},
    {PROC_MG_ROD, "d_a_mg_rod"},
    {PROC_E_ARROW, "d_a_e_arrow"},
    {PROC_BULLET, "d_a_bullet"},
    {PROC_SWHIT0, "d_a_swhit0"},
    {PROC_E_TH_BALL, "d_a_e_th_ball"},
    {PROC_TAG_EVTAREA, "d_a_tag_evtarea"},
    {PROC_TAG_EVTMSG, "d_a_tag_evtmsg"},
    {PROC_TAG_KMSG, "d_a_tag_kmsg"},
    {PROC_TAG_PUSH, "d_a_tag_push"},
    {PROC_E_MK_BO, "d_a_e_mk_bo"},
    {PROC_E_MM_MT, "d_a_e_mm_mt"},
    {PROC_OBJ_KBOX, "d_a_obj_kbox"},
    {PROC_OBJ_FW, "d_a_obj_fw"},
    {PROC_B_GOS, "d_a_b_gos"},
    {PROC_OBJ_YSTONE, "d_a_obj_ystone"},
    {PROC_MANT, "d_a_mant"},
    {PROC_CROD, "d_a_crod"},
    {PROC_OBJ_PLEAF, "d_a_obj_pleaf"},
    {PROC_OBJ_KBACKET, "d_a_obj_kbacket"},
    {PROC_OBJ_YBAG, "d_a_obj_yel_bag"},
    {PROC_OBJ_PUMPKIN, "d_a_obj_pumpkin"},
    {PROC_OBJ_AUTOMATA, "d_a_obj_automata"},
    {PROC_OBJ_GADGET, "d_a_obj_gadget"},
    {PROC_OBJ_KAGO, "d_a_obj_kago"},
    {PROC_Obj_Carry, "d_a_obj_carry"},
    {PROC_Obj_Stone, "d_a_obj_stone"},
    {PROC_OBJ_HB, "d_a_obj_hb"},
    {PROC_NPC_INKO, "d_a_npc_inko"},
    {PROC_BD, "d_a_bd"},
    {PROC_Obj_Eff, "d_a_obj_eff"},
    {PROC_E_BI, "d_a_e_bi"},
    {PROC_E_BI_LEAF, "d_a_e_bi_leaf"},
    {PROC_START_AND_GOAL, "d_a_startAndGoal"},
    {PROC_NPC_DF, "d_a_npc_df"},
    {PROC_ARROW, "d_a_arrow"},
    {PROC_PATH_LINE, "d_a_path_line"},
    {PROC_TAG_ALLMATO, "d_a_tag_allmato"},
    {PROC_Obj_Timer, "d_a_obj_timer"},
    {PROC_SCENE_EXIT, "d_a_scene_exit"},
    {PROC_SUSPEND, "d_a_suspend"},
    #if !PLATFORM_SHIELD
    {PROC_GRASS, "d_a_grass"},
    #endif
    {-1, NULL},
};

static DynamicModuleControlBase* DMC[PROC_MAX_NUM];

static bool DMC_initialized;

static BOOL cDyl_Initialized;

int cCc_Init() {
    JUT_ASSERT(37, !DMC_initialized);

    #if PLATFORM_GCN
    JKRHeap* parentHeap = mDoExt_getArchiveHeap();
    #else
    JKRHeap* parentHeap = DynamicModuleControlBase::getHeap();
    #endif
    u32 dmcHeapSize = sizeof(DynamicModuleControl) * PROC_MAX_NUM;

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
                DMC[d.mKey] = new DynamicModuleControl(d.name);
            }
        }
    }

    dmcHeap->adjustSize();
    JKRSetCurrentHeap(prevHeap);

    #if DEBUG
    FixedMemoryCheck* check = FixedMemoryCheck::easyCreate(DMC, sizeof(DMC));
    #endif

    DMC_initialized = true;
    return 1;
}

BOOL cCc_Check() {
    BOOL rt = TRUE;
    for (int i = 0; i < ARRAY_SIZEU(DynamicNameTable); i++) {
        uintptr_t ptr = (uintptr_t)DMC[i];
        if (ptr != NULL) {
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
                OSReport_Error("cDyl_LinkASync: リンクに失敗しました。諦めます\n");
                return cPhs_ERROR_e;
            }
        } else {
            return cPhs_INIT_e;
        }
    }

    return cPhs_COMPLEATE_e;
}

static int cDyl_InitCallback(void* param_0) {
    JUT_ASSERT(335, !cDyl_Initialized);

    #if PLATFORM_GCN
    JKRHeap* parentHeap = mDoExt_getArchiveHeap();
    #else
    JKRHeap* parentHeap = DynamicModuleControlBase::getHeap();
    #endif

    JKRFileCache* loader = JKRMountDvdDrive("/", parentHeap, NULL);
    DynamicModuleControl::initialize();

    #if PLATFORM_GCN
    void* strTbl = JKRGetResource("/dvd/str/Final/Release/frameworkF.str");
    #elif PLATFORM_WII
    void* strTbl = JKRGetResource("/dvd/str/Rfinal/Release/RframeworkF.str");
    #else
    void* strTbl = JKRGetResource("/dvd/str/Final/Release/frameworkF.str");
    #endif

    JKRDetachResource(strTbl, loader);
    JKRUnmountDvdDrive(loader);
    OSSetStringTable(strTbl);

    DynamicModuleControl dmc("f_pc_profile_lst");
    dmc.link();
    cDyl_Initialized = true;

    fopScnM_CreateReq(PROC_LOGO_SCENE, 0x7FFF, 0, 0);
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

    if (i_phase->id == cPhs_NEXT_e) {
        return cPhs_COMPLEATE_e;
    }

    return dComLbG_PhaseHandler(i_phase, l_method, &i_ProfName);
}

int cDylPhs::Unlink(request_of_phase_process_class* i_phase, s16 i_ProfName) {
    JUT_ASSERT(460, i_phase->id != cPhs_LOADING_e);

    if (i_phase->id == cPhs_NEXT_e) {
        int ret = cDyl_Unlink(i_ProfName);
        i_phase->id = cPhs_INIT_e;
        return ret;
    }

    return 0;
}
