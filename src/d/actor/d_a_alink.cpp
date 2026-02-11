/**
 * d_a_alink.cpp
 * Player (Link) Actor
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_alink.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_item.h"
#include "d/d_meter2_draw.h"
#include "d/d_pane_class.h"
#include "d/d_demo.h"
#include "d/actor/d_a_crod.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_mirror.h"
#include "d/actor/d_a_spinner.h"
#include "d/actor/d_a_tbox.h"
#include "d/actor/d_a_tag_Lv6Gate.h"
#include "d/actor/d_a_tag_kmsg.h"
#include "d/actor/d_a_tag_magne.h"
#include "d/actor/d_a_tag_wljump.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/actor/d_a_cow.h"
#include "d/actor/d_a_obj_crope.h"
#include "d/actor/d_a_obj_wchain.h"
#include "d/actor/d_a_tag_hstop.h"
#include "d/actor/d_a_scene_exit.h"
#include "d/actor/d_a_tag_mhint.h"
#include "d/actor/d_a_tag_mmsg.h"
#include "d/actor/d_a_tag_lantern.h"
#include "d/actor/d_a_horse.h"
#include "m_Do/m_Do_controller_pad.h"
#include "d/d_bomb.h"
#include "d/d_meter2_info.h"
#include "d/actor/d_a_kytag05.h"
#include "d/actor/d_a_b_mgn.h"
#include "d/actor/d_a_npc_bou.h"
#include "d/actor/d_a_npc_kolin.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/actor/d_a_tag_mstop.h"
#include "d/actor/d_a_tag_mwait.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_ni.h"
#include "d/d_s_play.h"

#include "res/Object/Alink.h"

static int daAlink_Create(fopAc_ac_c* i_this);
static int daAlink_Delete(daAlink_c* i_this);
static int daAlink_Execute(daAlink_c* i_this);
static int daAlink_Draw(daAlink_c* i_this);
static fopAc_ac_c* daAlink_searchTagKandelaar(fopAc_ac_c* i_actor, void* i_data);

BOOL daAlink_c::getE3Zhint() {
    return false;
}

#include "d/actor/d_a_alink_HIO.inc"

#if DEBUG
static BOOL l_debugMode;
#endif

static const char l_wArcName[] = "Wmdl";

static const char l_bArcName[] = "Bmdl";

static const char l_kArcName[] = "Kmdl";

static const char l_zArcName[] = "Zmdl";

static const char l_mArcName[] = "Mmdl";

static const char l_hyShdArcName[] = "HyShd";

static const char l_cWShdArcName[] = "CWShd";

static const char l_sWShdArcName[] = "SWShd";

static const char l_arcName[] = "Alink";

const char* daAlink_c::getAlinkArcName() {
    return l_arcName;
}

static void daAlink_tgHitCallback(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf, fopAc_ac_c* i_atActor,
                                  dCcD_GObjInf* i_atObjInf) {
    static_cast<daAlink_c*>(i_tgActor)->tgHitCallback(i_atActor, i_tgObjInf, i_atObjInf);
}

static void daAlink_coHitCallback(fopAc_ac_c* i_coActorA, dCcD_GObjInf* i_coObjInfA, fopAc_ac_c* i_coActorB,
                                  dCcD_GObjInf* i_coObjInfB) {
    UNUSED(i_coObjInfB);
    static_cast<daAlink_c*>(i_coActorA)->coHitCallback(i_coActorB, i_coObjInfA);
}

static cXyz l_waitBaseAnime(1.24279f, 102.00054f, 5.0f);

static cXyz l_ironBallBaseAnime(-4.248938f, 89.0f, -5.267045f);

static cXyz l_halfAtnWaitBaseAnime(3.5f, 97.0f, -7.0f);

static cXyz l_rWaitBaseAnime(4.313951f, 93.94436f, -5.207283f);

static cXyz l_lWaitBaseAnime(-4.300988f, 93.95595f, -5.218504f);

static cXyz l_horseBaseAnime(-l_waitBaseAnime.x, 225.7f, 1.81f - l_waitBaseAnime.z);

static cXyz l_boarBaseAnime(-l_waitBaseAnime.x, 186.17f, -20.29f - l_waitBaseAnime.z);

static cXyz l_localHorseRidePos(-68.208984f, 41.609924f, 0.883789f);

static cXyz l_localBoarRidePos(0.0f, 15.0f, 0.0f);

static cXyz l_canoeBaseAnime(1.24279f - l_waitBaseAnime.x, 56.0f, -72.0f - l_waitBaseAnime.z);

static cXyz l_sumouBaseAnimeSp(0.0f, 0.0f, 32.0f - l_waitBaseAnime.z);

static cXyz l_wolfBaseAnime(1.0f, 88.63934f, -28.497932f);

static cXyz l_wolfRopeBaseAnime(0.115164f, 68.336296f, -7.667817f);

static void dummy_lit_3757() {
    Vec temp = { 0.0f, 0.0f, 0.0f };
}

#include "d/actor/d_a_alink_HIO_data.inc"

static char const l_worldChangeEventName[16] = "WORLD_CHANGE_IN";

static char const l_wolfWorldChangeEventName[21] = "WOLF_WORLD_CHANGE_IN";

static char const l_defaultGetEventName[16] = "DEFAULT_GETITEM";

static char l_peepEventName[10] = "PEEP_HOLE";

daAlink_procInitTable daAlink_c::m_procInitTable[] = {
    { &daAlink_c::procPreActionUnequip, 0x21 },
    { &daAlink_c::procServiceWait, 0x10000085 },
    { &daAlink_c::procTiredWait, 0x10001185 },
    { &daAlink_c::procWait, 0x10001185 },
    { &daAlink_c::procMove, 0x10001184 },
    { &daAlink_c::procAtnMove, 0x10001184 },
    { &daAlink_c::procAtnActorWait, 0x10001185 },
    { &daAlink_c::procAtnActorMove, 0x10001184 },
    { &daAlink_c::procWaitTurn, 0x10001085 },
    { &daAlink_c::procMoveTurn, 0x10001084 },
    { &daAlink_c::procSideStep, 0x10001186 },
    { &daAlink_c::procSideStepLand, 0x10001185 },
    { &daAlink_c::procSlide, 0x0 },
    { &daAlink_c::procSlideLand, 0x4 },
    { &daAlink_c::procFrontRoll, 0x8200 },
    { &daAlink_c::procFrontRollCrash, 0x2 },
    { &daAlink_c::procFrontRollSuccess, 0x0 },
    { &daAlink_c::procSideRoll, 0x8000 },
    { &daAlink_c::procBackJump, 0x8001086 },
    { &daAlink_c::procBackJumpLand, 0x1185 },
    { &daAlink_c::procSlip, 0x0 },
    { &daAlink_c::procAutoJump, 0x1006 },
    { &daAlink_c::procDiveJump, 0x2 },
    { &daAlink_c::procRollJump, 0x2 },
    { &daAlink_c::procFall, 0x1006 },
    { &daAlink_c::procLand, 0x1005 },
    { &daAlink_c::procSmallJump, 0x1000 },
    { &daAlink_c::procStepMove, 0x10001184 },
    { &daAlink_c::procCrouch, 0x800001 },
    { &daAlink_c::procGuardSlip, 0x60000000 },
    { &daAlink_c::procGuardAttack, 0x100 },
    { &daAlink_c::procGuardBreak, 0x100 },
    { &daAlink_c::procTurnMove, 0x8000 },
    { &daAlink_c::procCutNormal, 0x20000300 },
    { &daAlink_c::procCutFinish, 0x20000320 },
    { &daAlink_c::procCutFinishJumpUp, 0x102 },
    { &daAlink_c::procCutFinishJumpUpLand, 0x100 },
    { &daAlink_c::procCutReverse, 0x200 },
    { &daAlink_c::procCutJump, 0x8000202 },
    { &daAlink_c::procCutJumpLand, 0x8000201 },
    { &daAlink_c::procCutTurn, 0x200 },
    { &daAlink_c::procCutTurnCharge, 0x101 },
    { &daAlink_c::procCutTurnMove, 0x101 },
    { &daAlink_c::procCutDown, 0x8000022 },
    { &daAlink_c::procCutDownLand, 0x8002001 },
    { &daAlink_c::procCutHead, 0x222 },
    { &daAlink_c::procCutHeadLand, 0x201 },
    { &daAlink_c::procCutLargeJumpCharge, 0x101 },
    { &daAlink_c::procCutLargeJump, 0x200 },
    { &daAlink_c::procCutLargeJumpLand, 0x201 },
    { &daAlink_c::procDamage, 0x8 },
    { &daAlink_c::procLargeDamageUp, 0xa008008 },
    { &daAlink_c::procLandDamage, 0x9 },
    { &daAlink_c::procCrawlStart, 0x300a000 },
    { &daAlink_c::procCrawlMove, 0xb00e000 },
    { &daAlink_c::procCrawlAutoMove, 0xf00a000 },
    { &daAlink_c::procCrawlEnd, 0x300a000 },
    { &daAlink_c::procPullMove, 0x202000 },
    { &daAlink_c::procHorseRide, 0x4002400 },
    { &daAlink_c::procHorseGetOff, 0x4002400 },
    { &daAlink_c::procHorseWait, 0x10003585 },
    { &daAlink_c::procHorseTurn, 0x2500 },
    { &daAlink_c::procHorseJump, 0x2502 },
    { &daAlink_c::procHorseLand, 0x2500 },
    { &daAlink_c::procHorseSubjectivity, 0x60003404 },
    { &daAlink_c::procHorseCut, 0x2500 },
    { &daAlink_c::procHorseCutChargeReady, 0x2500 },
    { &daAlink_c::procHorseCutTurn, 0x2500 },
    { &daAlink_c::procHorseDamage, 0x2408 },
    { &daAlink_c::procHorseBowSubject, 0x60003404 },
    { &daAlink_c::procHorseBowMove, 0x60003404 },
    { &daAlink_c::procHorseGrabMove, 0x3404 },
    { &daAlink_c::procHorseBoomerangSubject, 0x60003404 },
    { &daAlink_c::procHorseBoomerangMove, 0x60003404 },
    { &daAlink_c::procHorseHookshotSubject, 0x60003404 },
    { &daAlink_c::procHorseHookshotMove, 0x60003404 },
    { &daAlink_c::procHorseBottleDrink, 0x2401 },
    { &daAlink_c::procHorseComeback, 0x2409 },
    { &daAlink_c::procHorseKandelaarPour, 0x2401 },
    { &daAlink_c::procHorseRun, 0x2400 },
    { &daAlink_c::procHorseHang, 0x2400 },
    { &daAlink_c::procHorseGetKey, 0x2401 },
    { &daAlink_c::procHorseLookDown, 0x2401 },
    { &daAlink_c::procBoarRun, 0x2400 },
    { &daAlink_c::procSwordUnequipSp, 0x1 },
    { &daAlink_c::procHangStart, 0x4041 },
    { &daAlink_c::procHangFallStart, 0x4041 },
    { &daAlink_c::procHangUp, 0x4041 },
    { &daAlink_c::procHangWait, 0x4041 },
    { &daAlink_c::procHangMove, 0x4040 },
    { &daAlink_c::procHangClimb, 0x4040 },
    { &daAlink_c::procHangWallCatch, 0x4040 },
    { &daAlink_c::procHangReady, 0x1 },
    { &daAlink_c::procHangLeverDown, 0x21 },
    { &daAlink_c::procBowSubject, 0x20001005 },
    { &daAlink_c::procBowMove, 0x20001004 },
    { &daAlink_c::procBoomerangSubject, 0x30001005 },
    { &daAlink_c::procBoomerangMove, 0x30001004 },
    { &daAlink_c::procBoomerangCatch, 0x1001 },
    { &daAlink_c::procCopyRodSubject, 0x30001005 },
    { &daAlink_c::procCopyRodMove, 0x30001004 },
    { &daAlink_c::procCopyRodSwing, 0x1 },
    { &daAlink_c::procCopyRodRevive, 0x1 },
    { &daAlink_c::procLadderUpStart, 0x10000 },
    { &daAlink_c::procLadderUpEnd, 0x10000 },
    { &daAlink_c::procLadderDownStart, 0x10000 },
    { &daAlink_c::procLadderDownEnd, 0x10000 },
    { &daAlink_c::procLadderMove, 0x10000 },
    { &daAlink_c::procGrabReady, 0x100001 },
    { &daAlink_c::procGrabUp, 0x100001 },
    { &daAlink_c::procGrabMiss, 0x100001 },
    { &daAlink_c::procGrabThrow, 0x100201 },
    { &daAlink_c::procGrabPut, 0x100001 },
    { &daAlink_c::procGrabWait, 0x1105 },
    { &daAlink_c::procGrabRebound, 0x100001 },
    { &daAlink_c::procGrabStand, 0x1 },
    { &daAlink_c::procInsectCatch, 0x21 },
    { &daAlink_c::procPickUp, 0x100001 },
    { &daAlink_c::procPickPut, 0x100001 },
    { &daAlink_c::procStEscape, 0x1 },
    { &daAlink_c::procDkCaught, 0x4000000 },
    { &daAlink_c::procSwimUp, 0x40000 },
    { &daAlink_c::procSwimWait, 0x40105 },
    { &daAlink_c::procSwimMove, 0x40104 },
    { &daAlink_c::procSwimDive, 0x40000 },
    { &daAlink_c::procSwimHookshotSubject, 0x20041005 },
    { &daAlink_c::procSwimHookshotMove, 0x20041004 },
    { &daAlink_c::procSwimDamage, 0x40008 },
    { &daAlink_c::procClimbUpStart, 0x10000 },
    { &daAlink_c::procClimbDownStart, 0x10000 },
    { &daAlink_c::procClimbMoveUpDown, 0x10000 },
    { &daAlink_c::procClimbMoveSide, 0x10000 },
    { &daAlink_c::procClimbWait, 0x10000 },
    { &daAlink_c::procClimbToRoof, 0x10000 },
    { &daAlink_c::procRoofHangStart, 0x10 },
    { &daAlink_c::procRoofHangWait, 0x11 },
    { &daAlink_c::procRoofHangFrontMove, 0x10 },
    { &daAlink_c::procRoofHangSideMove, 0x10 },
    { &daAlink_c::procRoofHangTurn, 0x10 },
    { &daAlink_c::procRoofSwitchHang, 0x2030 },
    { &daAlink_c::procCanoeRide, 0x4002400 },
    { &daAlink_c::procCanoeJumpRide, 0x4002400 },
    { &daAlink_c::procCanoeGetOff, 0x4002400 },
    { &daAlink_c::procCanoeWait, 0x14003504 },
    { &daAlink_c::procCanoeRow, 0x4002504 },
    { &daAlink_c::procCanoePaddleShift, 0x4002504 },
    { &daAlink_c::procCanoePaddlePut, 0x4002400 },
    { &daAlink_c::procCanoePaddleGrab, 0x4002400 },
    { &daAlink_c::procCanoeRodGrab, 0x4002400 },
    { &daAlink_c::procCanoeFishingWait, 0x44002404 },
    { &daAlink_c::procCanoeFishingReel, 0x44002500 },
    { &daAlink_c::procCanoeFishingGet, 0x4002400 },
    { &daAlink_c::procCanoeSubjectivity, 0x64002404 },
    { &daAlink_c::procCanoeBowSubject, 0x64003404 },
    { &daAlink_c::procCanoeBowMove, 0x64003404 },
    { &daAlink_c::procCanoeGrabMove, 0x4103404 },
    { &daAlink_c::procCanoeBoomerangSubject, 0x64003404 },
    { &daAlink_c::procCanoeBoomerangMove, 0x64003404 },
    { &daAlink_c::procCanoeHookshotSubject, 0x64003404 },
    { &daAlink_c::procCanoeHookshotMove, 0x64003404 },
    { &daAlink_c::procCanoeBottleDrink, 0x2401 },
    { &daAlink_c::procCanoeKandelaarPour, 0x2401 },
    { &daAlink_c::procFishingCast, 0x2101 },
    { &daAlink_c::procFishingFood, 0x2001 },
    { &daAlink_c::procSpinnerReady, 0x2002 },
    { &daAlink_c::procSpinnerWait, 0x2500 },
    { &daAlink_c::procBoardRide, 0x22 },
    { &daAlink_c::procBoardWait, 0x10001504 },
    { &daAlink_c::procBoardRow, 0x1504 },
    { &daAlink_c::procBoardTurn, 0x1504 },
    { &daAlink_c::procBoardJump, 0x1406 },
    { &daAlink_c::procBoardSubjectivity, 0x60000404 },
    { &daAlink_c::procBoardCut, 0x504 },
    { &daAlink_c::procBoardCutTurn, 0x400 },
    { &daAlink_c::procFmChainUp, 0x101 },
    { &daAlink_c::procFmChainStrongPull, 0x1001 },
    { &daAlink_c::procDoorOpen, 0x4000 },
    { &daAlink_c::procMonkeyMove, 0x800 },
    { &daAlink_c::procDemoBoomerangCatch, 0x10000101 },
    { &daAlink_c::procBottleDrink, 0x2001 },
    { &daAlink_c::procBottleOpen, 0x2001 },
    { &daAlink_c::procBottleSwing, 0x21 },
    { &daAlink_c::procBottleGet, 0x1 },
    { &daAlink_c::procKandelaarSwing, 0x1 },
    { &daAlink_c::procKandelaarPour, 0x2001 },
    { &daAlink_c::procGrassWhistleGet, 0x21 },
    { &daAlink_c::procGrassWhistleWait, 0x2001 },
    { &daAlink_c::procHawkCatch, 0x1 },
    { &daAlink_c::procHawkSubject, 0x20000021 },
    { &daAlink_c::procFloorDownRebound, 0x10001185 },
    { &daAlink_c::procGoronRideWait, 0x101 },
    { &daAlink_c::procGoatMove, 0x4002121 },
    { &daAlink_c::procGoatCatch, 0x400a021 },
    { &daAlink_c::procGoatStroke, 0x2121 },
    { &daAlink_c::procGoronMove, 0x101 },
    { &daAlink_c::procDemoCommon, 0x1 },
    { &daAlink_c::procHookshotSubject, 0x20001005 },
    { &daAlink_c::procHookshotMove, 0x20001004 },
    { &daAlink_c::procHookshotFly, 0x6002 },
    { &daAlink_c::procHookshotRoofWait, 0x6802 },
    { &daAlink_c::procHookshotRoofShoot, 0x6902 },
    { &daAlink_c::procHookshotRoofBoots, 0x6902 },
    { &daAlink_c::procHookshotWallWait, 0x6802 },
    { &daAlink_c::procHookshotWallShoot, 0x6902 },
    { &daAlink_c::procMagneBootsFly, 0x2 },
    { &daAlink_c::procBootsEquip, 0x1103 },
    { &daAlink_c::procSumouReady, 0xa000 },
    { &daAlink_c::procSumouMove, 0xa000 },
    { &daAlink_c::procSumouSideMove, 0xa000 },
    { &daAlink_c::procSumouAction, 0xa000 },
    { &daAlink_c::procSumouStagger, 0xa000 },
    { &daAlink_c::procSumouWinLose, 0x2002 },
    { &daAlink_c::procSumouShiko, 0xa000 },
    { &daAlink_c::procLookUp, 0x101 },
    { &daAlink_c::procLookUpToGetItem, 0x101 },
    { &daAlink_c::procHandPat, 0x101 },
    { &daAlink_c::procIronBallSubject, 0x20001005 },
    { &daAlink_c::procIronBallMove, 0x20001004 },
    { &daAlink_c::procIronBallThrow, 0x20000000 },
    { &daAlink_c::procIronBallReturn, 0x1 },
    { &daAlink_c::procBossBodyHang, 0x6029 },
    { &daAlink_c::procOctaIealSpit, 0x42008 },
    { &daAlink_c::procScreamWait, 0x1 },
    { &daAlink_c::procGoatStopReady, 0x1 },
    { &daAlink_c::procZoraMove, 0xe000 },
    { &daAlink_c::procLookAroundTurn, 0x1 },
    { &daAlink_c::procTradeItemOut, 0x80121 },
    { &daAlink_c::procNotUseItem, 0x2021 },
    { &daAlink_c::procSwordReady, 0x1 },
    { &daAlink_c::procSwordPush, 0x1 },
    { &daAlink_c::procGanonFinish, 0xe000 },
    { &daAlink_c::procCutFastReady, 0x1 },
    { &daAlink_c::procMasterSwordStick, 0x6001 },
    { &daAlink_c::procMasterSwordPull, 0x6001 },
    { &daAlink_c::procDungeonWarpReady, 0x2021 },
    { &daAlink_c::procDungeonWarp, 0x6003 },
    { &daAlink_c::procDungeonWarpSceneStart, 0x6003 },
    { &daAlink_c::procWolfHowlDemo, 0x800021 },
    { &daAlink_c::procWolfServiceWait, 0x1001 },
    { &daAlink_c::procWolfTiredWait, 0x1101 },
    { &daAlink_c::procWolfMidnaRideShock, 0x1 },
    { &daAlink_c::procWolfWait, 0x1101 },
    { &daAlink_c::procWolfMove, 0x1100 },
    { &daAlink_c::procWolfDash, 0x1100 },
    { &daAlink_c::procWolfDashReverse, 0x2 },
    { &daAlink_c::procWolfWaitTurn, 0x1001 },
    { &daAlink_c::procWolfAtnActorMove, 0x1100 },
    { &daAlink_c::procWolfSideStep, 0x1102 },
    { &daAlink_c::procWolfSideStepLand, 0x1101 },
    { &daAlink_c::procWolfBackJump, 0x1002 },
    { &daAlink_c::procWolfBackJumpLand, 0x1001 },
    { &daAlink_c::procWolfHowl, 0x1 },
    { &daAlink_c::procWolfAutoJump, 0x1002 },
    { &daAlink_c::procWolfFall, 0x1002 },
    { &daAlink_c::procWolfLand, 0x1001 },
    { &daAlink_c::procWolfSit, 0x800101 },
    { &daAlink_c::procWolfLieStart, 0x1000100 },
    { &daAlink_c::procWolfLieMove, 0x1000100 },
    { &daAlink_c::procWolfLieAutoMove, 0x5002000 },
    { &daAlink_c::procWolfHangReady, 0x1001 },
    { &daAlink_c::procWolfStepMove, 0x10001100 },
    { &daAlink_c::procWolfHangWallCatch, 0x10005040 },
    { &daAlink_c::procWolfHangFallStart, 0x10005041 },
    { &daAlink_c::procWolfDamage, 0x8 },
    { &daAlink_c::procWolfLargeDamageUp, 0x12008008 },
    { &daAlink_c::procWolfLandDamage, 0x9 },
    { &daAlink_c::procWolfScreamWait, 0x1 },
    { &daAlink_c::procWolfSlip, 0x10001000 },
    { &daAlink_c::procWolfSlipTurn, 0x10001000 },
    { &daAlink_c::procWolfSlipTurnLand, 0x10001000 },
    { &daAlink_c::procWolfSlideReady, 0x10008000 },
    { &daAlink_c::procWolfSlide, 0x1000 },
    { &daAlink_c::procWolfSlideLand, 0x1000 },
    { &daAlink_c::procWolfWaitSlip, 0x1100 },
    { &daAlink_c::procWolfSlopeStart, 0x1000 },
    { &daAlink_c::procWolfRopeMove, 0x23100 },
    { &daAlink_c::procWolfRopeHang, 0x10023000 },
    { &daAlink_c::procWolfRopeTurn, 0x10023000 },
    { &daAlink_c::procWolfRopeStagger, 0x10023000 },
    { &daAlink_c::procWolfRopeSubjectivity, 0x23001 },
    { &daAlink_c::procWolfTagJump, 0x1022 },
    { &daAlink_c::procWolfTagJumpLand, 0x120 },
    { &daAlink_c::procWolfRollAttackCharge, 0x100 },
    { &daAlink_c::procWolfRollAttackMove, 0x100 },
    { &daAlink_c::procWolfJumpAttack, 0x2 },
    { &daAlink_c::procWolfJumpAttackKick, 0x2 },
    { &daAlink_c::procWolfJumpAttackSlideLand, 0x0 },
    { &daAlink_c::procWolfJumpAttackNormalLand, 0x0 },
    { &daAlink_c::procWolfWaitAttack, 0x0 },
    { &daAlink_c::procWolfRollAttack, 0x0 },
    { &daAlink_c::procWolfDownAttack, 0x22 },
    { &daAlink_c::procWolfDownAtLand, 0x8002021 },
    { &daAlink_c::procWolfDownAtMissLand, 0x0 },
    { &daAlink_c::procWolfLockAttack, 0x2002 },
    { &daAlink_c::procWolfLockAttackTurn, 0x0 },
    { &daAlink_c::procWolfSwimUp, 0x40000 },
    { &daAlink_c::procWolfSwimWait, 0x40101 },
    { &daAlink_c::procWolfSwimMove, 0x40100 },
    { &daAlink_c::procWolfSwimEndWait, 0x1001 },
    { &daAlink_c::procWolfGrabUp, 0x100001 },
    { &daAlink_c::procWolfGrabPut, 0x100001 },
    { &daAlink_c::procWolfGrabThrow, 0x100001 },
    { &daAlink_c::procWolfChainUp, 0x1 },
    { &daAlink_c::procWolfPush, 0x20 },
    { &daAlink_c::procWolfChainReady, 0x20000 },
    { &daAlink_c::procWolfChainWait, 0x10020000 },
    { &daAlink_c::procWolfDig, 0xa020 },
    { &daAlink_c::procWolfDigThrough, 0xe000 },
    { &daAlink_c::procWolfAttackReverse, 0x2 },
    { &daAlink_c::procWolfEnemyThrow, 0x0 },
    { &daAlink_c::procWolfEnemyHangBite, 0xe002 },
    { &daAlink_c::procWolfGiantPuzzle, 0x100 },
    { &daAlink_c::procWolfCargoCarry, 0x400e022 },
    { &daAlink_c::procWolfGetSmell, 0xe021 },
    { &daAlink_c::procDemoCommon, 0x1 },
    { &daAlink_c::procWolfSnowEscape, 0xa000 },
    { &daAlink_c::procWolfGanonCatch, 0x4002021 },
    { &daAlink_c::procCoToolDemo, 0x8000 },
    { &daAlink_c::procCoSubjectivity, 0x20001005 },
    { &daAlink_c::procCoSwimSubjectivity, 0x20041001 },
    { &daAlink_c::procCoPeepSubjectivity, 0x2000200b },
    { &daAlink_c::procCoPolyDamage, 0x9 },
    { &daAlink_c::procCoElecDamage, 0x9 },
    { &daAlink_c::procCoPushPullWait, 0x202001 },
    { &daAlink_c::procCoPushMove, 0x202000 },
    { &daAlink_c::procCoTalk, 0x80101 },
    { &daAlink_c::procCoOpenTreasure, 0xe001 },
    { &daAlink_c::procCoUnequip, 0x1 },
    { &daAlink_c::procCoGetItem, 0x800e001 },
    { &daAlink_c::procCoTurnBack, 0x1 },
    { &daAlink_c::procCoLookWait, 0x1101 },
    { &daAlink_c::procDemoCommon, 0x200001 },
    { &daAlink_c::procDemoCommon, 0x200000 },
    { &daAlink_c::procCoMetamorphose, 0x8001 },
    { &daAlink_c::procCoMetamorphoseOnly, 0x6001 },
    { &daAlink_c::procCoWarp, 0x1 },
    { &daAlink_c::procCoDead, 0xa009 },
    { &daAlink_c::procCoFogDead, 0xa009 },
    { &daAlink_c::procCoLookAround, 0x0 },
    { &daAlink_c::procDemoCommon, 0xe000 },
    { &daAlink_c::procCoSandWallHit, 0xa },
    { &daAlink_c::procCoLavaReturn, 0x42009 },
    { &daAlink_c::procCoSwimFreezeReturn, 0x42009 },
    { &daAlink_c::procCoGetReadySit, 0xa001 },
    { &daAlink_c::procCoTwGate, 0xe002 },
    { &daAlink_c::procCoLargeDamage, 0xa },
    { &daAlink_c::procCoLargeDamageWall, 0xa },
    { &daAlink_c::procCoNod, 0x80001 },
    { &daAlink_c::procDemoCommon, 0x80001 },
    { &daAlink_c::procCoGlare, 0x80001 },
    { &daAlink_c::procCoHorseCallWait, 0x1 },
    { &daAlink_c::procDemoCommon, 0x1 },
};

daAlink_procFunc daAlink_c::m_demoInitTable[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &daAlink_c::commonWaitTurnInit,
    NULL,
    NULL,
    NULL,
    NULL,
    &daAlink_c::procCoOpenTreasureInit,
    &daAlink_c::procCoGetItemInit,
    &daAlink_c::procCoUnequipInit,
    &daAlink_c::commonGrabPutInit,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    &daAlink_c::procMonkeyMoveInit,
    &daAlink_c::procCoLookAroundInit,
    NULL,
    NULL,
    NULL,
    NULL,
    &daAlink_c::procCoTurnBackInit,
    NULL,
    NULL,
    &daAlink_c::procDemoBoomerangCatchInit,
    &daAlink_c::procHawkCatchInit,
    &daAlink_c::procSwordUnequipSpInit,
    NULL,
    &daAlink_c::procCoDemoPushPullWaitInit,
    &daAlink_c::procCoDemoPushMoveInit,
    &daAlink_c::procBossAtnWaitInit,
    &daAlink_c::procDoorOpenInit,
    NULL,
    &daAlink_c::procTradeItemOutInit,
    NULL,
    &daAlink_c::procKandelaarSwingInit,
    &daAlink_c::procFrontRollInit,
    &daAlink_c::procCrouchInit,
    NULL,
    NULL,
    NULL,
    NULL,
    &daAlink_c::procCoCaughtInit,
    &daAlink_c::procLookUpInit,
    &daAlink_c::procLookUpToGetItemInit,
    &daAlink_c::procHandPatInit,
    &daAlink_c::procWolfMidnaRideShockInit,
    &daAlink_c::procSumouShikoInit,
    &daAlink_c::procCoFogDeadInit,
    &daAlink_c::procWolfSmellWaitInit,
    NULL,
    NULL,
    &daAlink_c::procWolfCargoCarryInit,
    &daAlink_c::procCoMetamorphoseInit,
    &daAlink_c::procCoMetamorphoseInit,
    &daAlink_c::procHorseGetKeyInit,
    &daAlink_c::procCoNodInit,
    &daAlink_c::procCoGlareInit,
    &daAlink_c::procCoEyeAwayInit,
    &daAlink_c::procGoatStopReadyInit,
    &daAlink_c::procCoGetReadySitInit,
    NULL,
    &daAlink_c::procCoTwGateInit,
    &daAlink_c::procFmChainStrongPullInit,
    &daAlink_c::procWolfSnowEscapeInit,
    &daAlink_c::procZoraMoveInit,
    &daAlink_c::procCoMetamorphoseOnlyInit,
    &daAlink_c::procCoMetamorphoseOnlyInit,
    &daAlink_c::procLookAroundTurnInit,
    NULL,
    &daAlink_c::procCoQuakeWaitInit,
    &daAlink_c::procGuardAttackInit,
    &daAlink_c::procSwordReadyInit,
    &daAlink_c::procDungeonWarpInit,
    &daAlink_c::procDungeonWarpSceneStartInit,
    &daAlink_c::procMasterSwordStickInit,
    &daAlink_c::procMasterSwordPullInit,
    &daAlink_c::procCutDownInit,
    NULL,
    &daAlink_c::procCutHeadInit,
    NULL,
    NULL,
    &daAlink_c::procCutLargeJumpInit,
    &daAlink_c::procCutFastReadyInit,
    &daAlink_c::procCopyRodReviveInit,
    &daAlink_c::procSwordPushInit,
    &daAlink_c::procGanonFinishInit,
    NULL,
    NULL,
    &daAlink_c::procHorseLookDownInit,
    NULL,
};

static f32 const l_crawlStartFrontOffset[3] = {0.0f, 30.0f, 112.0f};

static Vec const l_crawlFrontOffset = {0.0f, 30.0f, 80.0f};

static Vec const l_crawlBackOffset = {0.0f, 30.0f, -30.0f};

static Vec const l_crawlTopOffset = {0.0f, 30.0f, 0.0f};

static Vec const l_crawlFrontUpOffset = {0.0f, 80.0f, 80.0f};

static Vec const l_wolfLieFrontUpOffset = {0.0f, 75.0f, 100.0f};

static Vec const l_crawlBackUpOffset = {0.0f, 80.0f, -30.0f};

static Vec const l_wolfLieBackUpOffset = {0.0f, 75.0f, -70.0f};

static Vec const l_crawlStandUpOffset = {0.0f, 80.0f, -30.0f};

static Vec const l_wolfLieStandUpOffset = {0.0f, 80.0f, -70.0f};

static Vec const l_crawlTopUpOffset = {0.0f, 80.0f, 0.0f};

static Vec const l_wolfLieTopUpOffset = {0.0f, 75.0f, 0.0f};

static Vec const l_crawlSideOffset = {55.0f, 80.0f, 0.0f};

static Vec const l_wolfLieSideOffset = {55.0f, 75.0f, 0.0f};

static Vec const l_crawlLSideOffset = {55.0f, 0.0f, 0.0f};

static Vec const l_crawlRSideOffset = {-55.0f, 0.0f, 0.0f};

static Vec const l_crawlLSideFrontOffset = {55.0f, 5.0f, 80.0f};

static Vec const l_crawlRSideFrontOffset = {-55.0f, 5.0f, 80.0f};

static Vec const l_crawlMinSideOffset = {35.0f, 0.0f, 0.0f};

static Vec const l_swordTopLocalN = {100.0f, 0.0f, 0.0f};

static Vec const l_swordTopLocalM = {120.0f, 0.0f, 0.0f};

static const Vec l_ironBallChainVec = {0.0f, 0.0f, 10.0f};

static Vec const l_ironBallCenterVec = {0.0f, 0.0f, 42.0f};

static f32 const l_wolfFootOnFrame[6][4] = {
    {0.0f, 16.0f, 19.0f, 6.0f},
    {10.0f, 12.0f, 7.0f, 3.0f},
    {0.0f, 20.0f, 27.0f, 8.0f},
    {17.0f, 0.0f, 24.0f, 11.0f},
    {24.0f, 32.0f, 12.0f, 26.0f},
    {56.0f, 0.0f, 44.0f, 58.0f},
};

static s16 const l_insectNameList[12] = {
    PROC_Obj_Kabuto, PROC_Obj_Cho, PROC_Obj_Kuw, PROC_Obj_Batta, PROC_Obj_Nan, PROC_Obj_Dan,
    PROC_Obj_Kam,    PROC_Obj_Ten, PROC_Obj_Kat, PROC_Obj_Tombo, PROC_Obj_Ari, PROC_Obj_Kag,
};

#if DEBUG
f32 l_jumpTop;
#endif

daAlink_BckData const daAlink_c::m_mainBckShield[20] = {
    {dRes_ID_ALANM_BCK_ATRFWS_e, dRes_ID_ALANM_BCK_ATRFWS_e},
    {dRes_ID_ALANM_BCK_ATRFDS_e, dRes_ID_ALANM_BCK_ATRFDS_e},
    {dRes_ID_ALANM_BCK_ATBW_e, dRes_ID_ALANM_BCK_ATLS_e},
    {dRes_ID_ALANM_BCK_ATBD_e, dRes_ID_ALANM_BCK_ATLS_e},
    {dRes_ID_ALANM_BCK_ATRBWS_e, dRes_ID_ALANM_BCK_ATRBWS_e},
    {dRes_ID_ALANM_BCK_ATRBDS_e, dRes_ID_ALANM_BCK_ATRBDS_e},
    {dRes_ID_ALANM_BCK_ATSWLS_e, dRes_ID_ALANM_BCK_ATSWLS_e},
    {dRes_ID_ALANM_BCK_ATRSWLS_e, dRes_ID_ALANM_BCK_ATRSWLS_e},
    {dRes_ID_ALANM_BCK_ATSWRS_e, dRes_ID_ALANM_BCK_ATSWRS_e},
    {dRes_ID_ALANM_BCK_ATRSWRS_e, dRes_ID_ALANM_BCK_ATRSWRS_e},
    {dRes_ID_ALANM_BCK_ATSDLS_e, dRes_ID_ALANM_BCK_ATSDLS_e},
    {dRes_ID_ALANM_BCK_ATRSDLS_e, dRes_ID_ALANM_BCK_ATRSDLS_e},
    {dRes_ID_ALANM_BCK_ATSDRS_e, dRes_ID_ALANM_BCK_ATSDRS_e},
    {dRes_ID_ALANM_BCK_ATRSDRS_e, dRes_ID_ALANM_BCK_ATRSDRS_e},
    {dRes_ID_ALANM_BCK_WALKH_e, dRes_ID_ALANM_BCK_ATLS_e},
    {dRes_ID_ALANM_BCK_ATRBWH_e, dRes_ID_ALANM_BCK_ATLS_e},
    {dRes_ID_ALANM_BCK_ATLS_e, dRes_ID_ALANM_BCK_ATLS_e},
    {dRes_ID_ALANM_BCK_ATRS_e, dRes_ID_ALANM_BCK_ATRS_e},
    {dRes_ID_ALANM_BCK_WALKS_e, dRes_ID_ALANM_BCK_ATLS_e},
    {dRes_ID_ALANM_BCK_DASHS_e, dRes_ID_ALANM_BCK_ATLS_e},
};

daAlink_BckData const daAlink_c::m_mainBckSword[5] = {
    {dRes_ID_ALANM_BCK_ATL_e, dRes_ID_ALANM_BCK_ATL_e},
    {dRes_ID_ALANM_BCK_ATR_e, dRes_ID_ALANM_BCK_ATR_e},
    {dRes_ID_ALANM_BCK_WALKS_e, dRes_ID_ALANM_BCK_WALKS_e},
    {dRes_ID_ALANM_BCK_DASHS_e, dRes_ID_ALANM_BCK_DASHS_e},
    {dRes_ID_ALANM_BCK_SWIMWAIT_e, dRes_ID_ALANM_BCK_SWIMWAITS_e},
};

daAlink_BckData const daAlink_c::m_mainBckFishing[28] = {
    {dRes_ID_ALANM_BCK_ATRFWS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATRFDS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_ATBW_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATBD_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_ATRBWS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATRBDS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_ATSWLS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATRSWLS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATSWRS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATRSWRS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATSDLS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_ATRSDLS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_ATSDRS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_ATRSDRS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_WALKH_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATRBWH_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATL_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_ATR_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_WALKS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_DASHS_e, dRes_ID_ALANM_BCK_DASHFISHR_e},
    {dRes_ID_ALANM_BCK_SWIMWAIT_e, dRes_ID_ALANM_BCK_SWIMWAITS_e},
    {dRes_ID_ALANM_BCK_STEPL_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_STEPR_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_STEPL_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_STEPR_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_WAITS_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_WAITB_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
    {dRes_ID_ALANM_BCK_WAITBTOA_e, dRes_ID_ALANM_BCK_WALKFISHR_e},
};

daAlink_AnmData const daAlink_c::m_anmDataTable[daAlink_c::ANM_MAX] = {
    {dRes_ID_ALANM_BCK_ATRFWS_e, dRes_ID_ALANM_BCK_ATRFW_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRFDS_e, dRes_ID_ALANM_BCK_ATRFD_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATBW_e, dRes_ID_ALANM_BCK_ATBW_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATBD_e, dRes_ID_ALANM_BCK_ATBD_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRBWS_e, dRes_ID_ALANM_BCK_ATRBW_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRBDS_e, dRes_ID_ALANM_BCK_ATRBD_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSWLS_e, dRes_ID_ALANM_BCK_ATSWL_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRSWLS_e, dRes_ID_ALANM_BCK_ATRSWL_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSWRS_e, dRes_ID_ALANM_BCK_ATSWR_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRSWRS_e, dRes_ID_ALANM_BCK_ATRSWR_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSDLS_e, dRes_ID_ALANM_BCK_ATSDL_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRSDLS_e, dRes_ID_ALANM_BCK_ATRSDL_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSDRS_e, dRes_ID_ALANM_BCK_ATSDR_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRSDRS_e, dRes_ID_ALANM_BCK_ATRSDR_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WALKH_e, dRes_ID_ALANM_BCK_WALKH_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATRBWH_e, dRes_ID_ALANM_BCK_ATL_e, 0xFE, 0xFE, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_ATL_e, dRes_ID_ALANM_BCK_AT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATR_e, dRes_ID_ALANM_BCK_AT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WALKS_e, dRes_ID_ALANM_BCK_WALKS_e, 0x4, 0xA, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_DASHS_e, dRes_ID_ALANM_BCK_DASHA_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMWAIT_e, dRes_ID_ALANM_BCK_SWIMWAIT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FSWIMWAIT_e, 0x0},
    {dRes_ID_ALANM_BCK_STEPL_e, dRes_ID_ALANM_BCK_STEPL_e, 0x4, 0xA, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_STEPR_e, dRes_ID_ALANM_BCK_STEPR_e, 0x4, 0xA, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_STEPL_e, dRes_ID_ALANM_BCK_WAITD_e, 0x4, 0xA, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_STEPR_e, dRes_ID_ALANM_BCK_WAITD_e, 0x4, 0xA, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_WAITS_e, dRes_ID_ALANM_BCK_WAITS_e, 0x4, 0xA, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_WAITB_e, dRes_ID_ALANM_BCK_WAITB_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITBTOA_e, dRes_ID_ALANM_BCK_WAITBTOA_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FWAITBTOA_e, 0x0},
    {dRes_ID_ALANM_BCK_DASHB_e, dRes_ID_ALANM_BCK_DASHB_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSJLST_e, dRes_ID_ALANM_BCK_ATSJLST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FJMPB_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSJRST_e, dRes_ID_ALANM_BCK_ATSJRST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FJMPB_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSJLED_e, dRes_ID_ALANM_BCK_ATSJLED_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSJRED_e, dRes_ID_ALANM_BCK_ATSJRED_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSJBST_e, dRes_ID_ALANM_BCK_ATSJBST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FJMPB_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSJBED_e, dRes_ID_ALANM_BCK_ATSJBED_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROLLF_e, dRes_ID_ALANM_BCK_ROLLF_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FJMPB_e, 0x0},
    {dRes_ID_ALANM_BCK_ROLLFMIS_e, dRes_ID_ALANM_BCK_ROLLFMIS_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_ROLLFGOOD_e, dRes_ID_ALANM_BCK_ROLLFGOOD_e, 0x1, 0x6, FTANM_ROLLFGOOD, dRes_ID_ALANM_BCK_FROLLFGOOD_e, 0x0},
    {dRes_ID_ALANM_BCK_ROLLBST_e, dRes_ID_ALANM_BCK_ROLLBST_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FJMPB_e, 0x0},
    {dRes_ID_ALANM_BCK_ROLLBED_e, dRes_ID_ALANM_BCK_ROLLBED_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SLIP_e, dRes_ID_ALANM_BCK_SLIP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CROUCH_e, dRes_ID_ALANM_BCK_CROUCH_e, 0xFE, 0xFE, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_CROUCHS_e, dRes_ID_ALANM_BCK_CROUCHS_e, 0xFE, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATDEFL_e, dRes_ID_ALANM_BCK_ATDEFL_e, 0xFE, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATDEFR_e, dRes_ID_ALANM_BCK_ATDEFR_e, 0xFE, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CROUCHDEFS_e, dRes_ID_ALANM_BCK_CROUCHDEFS_e, 0xFE, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATGPUSH_e, dRes_ID_ALANM_BCK_ATGPUSH_e, 0xFE, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATGPUSHRE_e, dRes_ID_ALANM_BCK_ATGPUSHRE_e, 0xFE, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATDEFNG_e, dRes_ID_ALANM_BCK_ATDEFNG_e, 0x1, 0x6, FTANM_ATDEFNG, dRes_ID_ALANM_BCK_FATDEFNG_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPST_e, dRes_ID_ALANM_BCK_JUMPST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FJMPA_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPS_e, dRes_ID_ALANM_BCK_JUMPS_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FJMPA_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPSED_e, dRes_ID_ALANM_BCK_JUMPSED_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LIE_e, dRes_ID_ALANM_BCK_LIE_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LIEF_e, dRes_ID_ALANM_BCK_LIEF_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITPP_e, dRes_ID_ALANM_BCK_WAITPP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_PUSHW_e, dRes_ID_ALANM_BCK_PUSHW_e, 0xFE, 0xFE, FTANM_ARELORDTAME, dRes_ID_ALANM_BCK_FPUSHW_e, 0x0},
    {dRes_ID_ALANM_BCK_PUSHTSWST_e, dRes_ID_ALANM_BCK_PUSHTSWST_e, 0xFE, 0xFE, FTANM_MABA01_L, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_PUSHTSW_e, dRes_ID_ALANM_BCK_PUSHTSW_e, 0xFE, 0xFE, FTANM_MABA01_L, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_PULLW_e, dRes_ID_ALANM_BCK_PULLW_e, 0x0, 0x5, FTANM_PUSHW, dRes_ID_ALANM_BCK_FPULLW_e, 0x0},
    {dRes_ID_ALANM_BCK_PULLTSWST_e, dRes_ID_ALANM_BCK_PULLTSWST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_PULLTSW_e, dRes_ID_ALANM_BCK_PULLTSW_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_RIDEHL_e, dRes_ID_ALANM_BCK_RIDEHL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_RIDEHR_e, dRes_ID_ALANM_BCK_RIDEHR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_RIDEHB_e, dRes_ID_ALANM_BCK_RIDEHB_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_FALLHL_e, dRes_ID_ALANM_BCK_FALLHL_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_FALLHR_e, dRes_ID_ALANM_BCK_FALLHR_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_STOPS_e, dRes_ID_ALANM_BCK_STOPS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_STOPSTOSTANDS_e, dRes_ID_ALANM_BCK_STOPSTOSTANDS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_STANDS_e, dRes_ID_ALANM_BCK_STANDS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPHSST_e, dRes_ID_ALANM_BCK_JUMPHSST_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPHS_e, dRes_ID_ALANM_BCK_JUMPHS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPHSED_e, dRes_ID_ALANM_BCK_JUMPHSED_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITHS_e, dRes_ID_ALANM_BCK_WAITHS_e, 0x4, 0x5, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_WALKHAS_e, dRes_ID_ALANM_BCK_WALKHAS_e, 0x4, 0x5, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_WALKHBS_e, dRes_ID_ALANM_BCK_WALKHBS_e, 0x4, 0x5, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_DASHHAS_e, dRes_ID_ALANM_BCK_DASHHAS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_DASHHBS_e, dRes_ID_ALANM_BCK_DASHHBS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WSTARTH_e, dRes_ID_ALANM_BCK_WAITHS_e, 0x4, 0x5, FTANM_0, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_DSTARTHS_e, dRes_ID_ALANM_BCK_DSTARTHS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FCUT_e, 0x0},
    {dRes_ID_ALANM_BCK_LASHS_e, dRes_ID_ALANM_BCK_LASHS_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FCUTE_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTHTB_e, dRes_ID_ALANM_BCK_CUTHTB_e, 0x64, 0x5, FTANM_CUTHTB, dRes_ID_ALANM_BCK_FCUTHTB_e, 0x0},
    {dRes_ID_ALANM_BCK_SWAITHA_e, dRes_ID_ALANM_BCK_SWAITHA_e, 0x4, 0x5, FTANM_SWAITHA, dRes_ID_ALANM_BCK_FSWAITHA_e, 0x0},
    {dRes_ID_ALANM_BCK_SWAITHB_e, dRes_ID_ALANM_BCK_SWAITHB_e, 0x4, 0x5, FTANM_SWIATHB, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_DIEHUP_e, dRes_ID_ALANM_BCK_DIEHUP_e, 0x4, 0xA, FTANM_DIEHUP, dRes_ID_ALANM_BCK_FDIEHUP_e, 0x0},
    {dRes_ID_ALANM_BCK_TURNBS_e, dRes_ID_ALANM_BCK_TURNBS_e, 0x4, 0x5, FTANM_TURNBS, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_WAITHLIE_e, dRes_ID_ALANM_BCK_WAITHLIE_e, 0x4, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SPILLHL_e, dRes_ID_ALANM_BCK_SPILLHL_e, 0x4, 0x5, FTANM_SPILLH, dRes_ID_ALANM_BCK_FSPILLH_e, 0x0},
    {dRes_ID_ALANM_BCK_SPILLHR_e, dRes_ID_ALANM_BCK_SPILLHR_e, 0x4, 0x5, FTANM_SPILLH, dRes_ID_ALANM_BCK_FSPILLH_e, 0x0},
    {dRes_ID_ALANM_BCK_HANGHL_e, dRes_ID_ALANM_BCK_HANGHL_e, 0x4, 0x5, FTANM_HANGH, dRes_ID_ALANM_BCK_FHANGH_e, 0x0},
    {dRes_ID_ALANM_BCK_HANGHR_e, dRes_ID_ALANM_BCK_HANGHR_e, 0x4, 0x5, FTANM_HANGH, dRes_ID_ALANM_BCK_FHANGH_e, 0x0},
    {dRes_ID_ALANM_BCK_TURNLS_e, dRes_ID_ALANM_BCK_TURNLS_e, 0x4, 0x5, FTANM_TURNLS, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_TURNRS_e, dRes_ID_ALANM_BCK_TURNRS_e, 0x4, 0x5, FTANM_TURNRS, 0x0, 0x0},
    {dRes_ID_ALANM_BCK_RIDEWL_e, dRes_ID_ALANM_BCK_RIDEWL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_RIDEWR_e, dRes_ID_ALANM_BCK_RIDEWR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_FALLWL_e, dRes_ID_ALANM_BCK_FALLWL_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_FALLWR_e, dRes_ID_ALANM_BCK_FALLWR_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITWB_e, dRes_ID_ALANM_BCK_WAITWB_e, 0xFE, 0xFE, FTANM_WAITHDB, dRes_ID_ALANM_BCK_FWAITWB_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITWATOWB_e, dRes_ID_ALANM_BCK_WAITWATOWB_e, 0x4, 0x5, FTANM_WAITWATOWB, dRes_ID_ALANM_BCK_FWAITWATOWB_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTA_e, dRes_ID_ALANM_BCK_CUTA_e, 0x64, 0x6, FTANM_CUTA, dRes_ID_ALANM_BCK_FCUTA_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTL_e, dRes_ID_ALANM_BCK_CUTL_e, 0x0, 0x6, FTANM_CUTL, dRes_ID_ALANM_BCK_FCUTL_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTR_e, dRes_ID_ALANM_BCK_CUTR_e, 0x0, 0x6, FTANM_CUTR, dRes_ID_ALANM_BCK_FCUTR_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTFL_e, dRes_ID_ALANM_BCK_CUTFL_e, 0x64, 0x6, FTANM_CUTF, dRes_ID_ALANM_BCK_FCUTF_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTFR_e, dRes_ID_ALANM_BCK_CUTFR_e, 0x64, 0x6, FTANM_CUTF, dRes_ID_ALANM_BCK_FCUTF_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEA_e, dRes_ID_ALANM_BCK_CUTEA_e, 0x64, 0x6, FTANM_CUTEA, dRes_ID_ALANM_BCK_FCUTEA_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEB_e, dRes_ID_ALANM_BCK_CUTEB_e, 0x64, 0x6, FTANM_CUTEB, dRes_ID_ALANM_BCK_FCUTEB_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEC_e, dRes_ID_ALANM_BCK_CUTEC_e, 0x64, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FCUTE_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEF_e, dRes_ID_ALANM_BCK_CUTEF_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEE_e, dRes_ID_ALANM_BCK_CUTEE_e, 0x64, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEG_e, dRes_ID_ALANM_BCK_CUTEG_e, 0x64, 0x5, FTANM_CUTEG, dRes_ID_ALANM_BCK_FCUTEG_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEDST_e, dRes_ID_ALANM_BCK_CUTEDST_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEDED_e, dRes_ID_ALANM_BCK_CUTEDED_e, 0x64, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTREL_e, dRes_ID_ALANM_BCK_CUTREL_e, 0x0, 0x6, FTANM_CUTRE, dRes_ID_ALANM_BCK_FCUTRE_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTRER_e, dRes_ID_ALANM_BCK_CUTRER_e, 0x0, 0x6, FTANM_CUTRE, dRes_ID_ALANM_BCK_FCUTRE_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTJST_e, dRes_ID_ALANM_BCK_CUTJST_e, 0x64, 0x6, FTANM_CUTJST, dRes_ID_ALANM_BCK_FCUTJST_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTJED_e, dRes_ID_ALANM_BCK_CUTJED_e, 0x64, 0x6, FTANM_CUTJED, dRes_ID_ALANM_BCK_FCUTJED_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTB_e, dRes_ID_ALANM_BCK_CUTTB_e, 0x64, 0x6, FTANM_CUTTB, dRes_ID_ALANM_BCK_FCUTTB_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTT_e, dRes_ID_ALANM_BCK_CUTT_e, 0x64, 0x6, FTANM_CUTT, dRes_ID_ALANM_BCK_FCUTT_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTP_e, dRes_ID_ALANM_BCK_CUTTP_e, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTPWFB_e, dRes_ID_ALANM_BCK_CUTTPWFB_e, 0x64, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTPWLR_e, dRes_ID_ALANM_BCK_CUTTPWLR_e, 0x64, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTUST_e, dRes_ID_ALANM_BCK_CUTUST_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FCUTE_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTU_e, dRes_ID_ALANM_BCK_CUTU_e, 0x0, 0x5, FTANM_CUTU, dRes_ID_ALANM_BCK_FCUTU_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTUED_e, dRes_ID_ALANM_BCK_CUTUED_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FCUTUED_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTUNG_e, dRes_ID_ALANM_BCK_CUTUNG_e, 0x64, 0x5, FTANM_CUTUNG, dRes_ID_ALANM_BCK_FCUTUNG_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEHST_e, dRes_ID_ALANM_BCK_CUTEHST_e, 0x0, 0x6, FTANM_CUTEHST, dRes_ID_ALANM_BCK_FCUTEHST_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEH_e, dRes_ID_ALANM_BCK_CUTEH_e, 0x0, 0x6, FTANM_CUTEH, dRes_ID_ALANM_BCK_FCUTEH_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTEHED_e, dRes_ID_ALANM_BCK_CUTEHED_e, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMF_e, dRes_ID_ALANM_BCK_DAMF_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMB_e, dRes_ID_ALANM_BCK_DAMB_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAML_e, dRes_ID_ALANM_BCK_DAML_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMR_e, dRes_ID_ALANM_BCK_DAMR_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFF_e, dRes_ID_ALANM_BCK_DAMFF_e, 0xFE, 0xFE, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFB_e, dRes_ID_ALANM_BCK_DAMFB_e, 0xFE, 0xFE, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFL_e, dRes_ID_ALANM_BCK_DAMFL_e, 0xFE, 0xFE, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFR_e, dRes_ID_ALANM_BCK_DAMFR_e, 0xFE, 0xFE, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFFUP_e, dRes_ID_ALANM_BCK_DAMFFUP_e, 0xFE, 0xFE, FTANM_DAMFFUP, dRes_ID_ALANM_BCK_FDAMFFUP_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFBUP_e, dRes_ID_ALANM_BCK_DAMFBUP_e, 0xFE, 0xFE, FTANM_DAMFBUP, dRes_ID_ALANM_BCK_FDAMFBUP_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFLUP_e, dRes_ID_ALANM_BCK_DAMFLUP_e, 0xFE, 0xFE, FTANM_DAMFRLUP, dRes_ID_ALANM_BCK_FDAMFRLUP_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFRUP_e, dRes_ID_ALANM_BCK_DAMFRUP_e, 0xFE, 0xFE, FTANM_DAMFRLUP, dRes_ID_ALANM_BCK_FDAMFRLUP_e, 0x0},
    {dRes_ID_ALANM_BCK_DAM_e, dRes_ID_ALANM_BCK_DAM_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_LANDDAMA_e, dRes_ID_ALANM_BCK_LANDDAMA_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_LANDDAMAST_e, dRes_ID_ALANM_BCK_LANDDAMAST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_FINISHA_e, dRes_ID_ALANM_BCK_FINISHA_e, 0x0, 0x6, FTANM_MABAGOMA, dRes_ID_ALANM_BCK_FFINISHA_e, 0x0},
    {dRes_ID_ALANM_BCK_FINISHED_e, dRes_ID_ALANM_BCK_FINISHED_e, 0x0, 0xA, FTANM_DAM01, dRes_ID_ALANM_BCK_FFINISHED_e, 0x0},
    {dRes_ID_ALANM_BCK_SWAITA_e, dRes_ID_ALANM_BCK_SWAITA_e, 0x4, 0xA, FTANM_SWAITA, dRes_ID_ALANM_BCK_FSWAITA_e, 0x0},
    {dRes_ID_ALANM_BCK_VJMP_e, dRes_ID_ALANM_BCK_VJMP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_VJMPCHA_e, dRes_ID_ALANM_BCK_VJMPCHA_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_VJMPCHB_e, dRes_ID_ALANM_BCK_VJMPCHB_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_VJMPCL_e, dRes_ID_ALANM_BCK_VJMPCL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_HANG_e, dRes_ID_ALANM_BCK_HANG_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_HANGUP_e, dRes_ID_ALANM_BCK_HANGUP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_HANGL_e, dRes_ID_ALANM_BCK_HANGL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_HANGR_e, dRes_ID_ALANM_BCK_HANGR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADUPST_e, dRes_ID_ALANM_BCK_LADUPST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADUPEDL_e, dRes_ID_ALANM_BCK_LADUPEDL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADUPEDR_e, dRes_ID_ALANM_BCK_LADUPEDR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADDWST_e, dRes_ID_ALANM_BCK_LADDWST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADDWEDL_e, dRes_ID_ALANM_BCK_LADDWEDL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADDWEDR_e, dRes_ID_ALANM_BCK_LADDWEDR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADRTOL_e, dRes_ID_ALANM_BCK_LADRTOL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LADLTOR_e, dRes_ID_ALANM_BCK_LADLTOR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBHANG_e, dRes_ID_ALANM_BCK_CLIMBHANG_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBST_e, dRes_ID_ALANM_BCK_CLIMBST_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBL_e, dRes_ID_ALANM_BCK_CLIMBL_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBR_e, dRes_ID_ALANM_BCK_CLIMBR_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBSLIDEL_e, dRes_ID_ALANM_BCK_CLIMBSLIDEL_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBSLIDER_e, dRes_ID_ALANM_BCK_CLIMBSLIDER_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CLIMBHANGMISS_e, dRes_ID_ALANM_BCK_CLIMBHANGMISS_e, 0x3, 0x9, FTANM_CLIMBHANGMISS, dRes_ID_ALANM_BCK_FCLIMBHANGMISS_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFUP_e, dRes_ID_ALANM_BCK_ROOFUP_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFWAIT_e, dRes_ID_ALANM_BCK_ROOFWAIT_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFF_e, dRes_ID_ALANM_BCK_ROOFF_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFL_e, dRes_ID_ALANM_BCK_ROOFL_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFR_e, dRes_ID_ALANM_BCK_ROOFR_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFTURN_e, dRes_ID_ALANM_BCK_ROOFTURN_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOFHANG_e, dRes_ID_ALANM_BCK_ROOFHANG_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_GRABP_e, dRes_ID_ALANM_BCK_GRABP_e, 0xFE, 0xFE, FTANM_GRABP, dRes_ID_ALANM_BCK_FGRABP_e, 0x0},
    {dRes_ID_ALANM_BCK_GRABUP_e, dRes_ID_ALANM_BCK_GRABUP_e, 0xFE, 0xFE, FTANM_GRABUP, dRes_ID_ALANM_BCK_FGRABUP_e, 0x0},
    {dRes_ID_ALANM_BCK_GRABTHROW_e, dRes_ID_ALANM_BCK_GRABTHROW_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FGRABTHROW_e, 0x0},
    {dRes_ID_ALANM_BCK_HEAVYTHROW_e, dRes_ID_ALANM_BCK_HEAVYTHROW_e, 0xFE, 0xFE, FTANM_HEAVYTHROW, dRes_ID_ALANM_BCK_FHEAVYTHROW_e, 0x0},
    {dRes_ID_ALANM_BCK_GRABNG_e, dRes_ID_ALANM_BCK_GRABNG_e, 0xFE, 0xFE, FTANM_GRABNG, dRes_ID_ALANM_BCK_FGRABNG_e, 0x0},
    {dRes_ID_ALANM_BCK_GRABRE_e, dRes_ID_ALANM_BCK_GRABRE_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FGRABRE_e, 0x0},
    {dRes_ID_ALANM_BCK_CARRY_e, dRes_ID_ALANM_BCK_CARRY_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITD_e, dRes_ID_ALANM_BCK_WAITD_e, 0x4, 0xA, FTANM_MABA01, dRes_ID_ALANM_BCK_FWAITD_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITATOD_e, dRes_ID_ALANM_BCK_WAITATOD_e, 0x4, 0xA, FTANM_MABA01, dRes_ID_ALANM_BCK_FWAITATOD_e, 0x0},
    {dRes_ID_ALANM_BCK_TALKA_e, dRes_ID_ALANM_BCK_TALKA_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITST_e, dRes_ID_ALANM_BCK_WAITST_e, 0x1, 0x6, FTANM_PULLW, dRes_ID_ALANM_BCK_FWAITST_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTST_e, dRes_ID_ALANM_BCK_CUTST_e, 0x1, 0x6, FTANM_WAITST, dRes_ID_ALANM_BCK_FCUTST_e, 0x0},
    {dRes_ID_ALANM_BCK_BOOMCATCH_e, dRes_ID_ALANM_BCK_BOOMCATCH_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WALKSLOPEW_e, dRes_ID_ALANM_BCK_WALKSLOPEW_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SLIDEF_e, dRes_ID_ALANM_BCK_SLIDEF_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SLIDEB_e, dRes_ID_ALANM_BCK_SLIDEB_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SLIDEFED_e, dRes_ID_ALANM_BCK_SLIDEFED_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SLIDEBED_e, dRes_ID_ALANM_BCK_SLIDEBED_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMINGA_e, dRes_ID_ALANM_BCK_SWIMINGA_e, 0xFE, 0xFE, FTANM_SWIMINGA, dRes_ID_ALANM_BCK_FSWIMINGA_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMP_e, dRes_ID_ALANM_BCK_SWIMP_e, 0xFE, 0xFE, FTANM_SWIMP, dRes_ID_ALANM_BCK_FSWIMP_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSSWL_e, dRes_ID_ALANM_BCK_ATSSWL_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATSSWR_e, dRes_ID_ALANM_BCK_ATSSWR_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_ATBSW_e, dRes_ID_ALANM_BCK_ATBSW_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMDASH_e, dRes_ID_ALANM_BCK_SWIMDASH_e, 0xFE, 0xFE, FTANM_MABA03, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMDIVE_e, dRes_ID_ALANM_BCK_SWIMDIVE_e, 0xFE, 0xFE, FTANM_SWIMDIVE, dRes_ID_ALANM_BCK_FSWIMDIVE_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMINGST_e, dRes_ID_ALANM_BCK_SWIMINGST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMINGC_e, dRes_ID_ALANM_BCK_SWIMINGC_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMSWF_e, dRes_ID_ALANM_BCK_DAMSWF_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMSWB_e, dRes_ID_ALANM_BCK_DAMSWB_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMSWL_e, dRes_ID_ALANM_BCK_DAMSWL_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMSWR_e, dRes_ID_ALANM_BCK_DAMSWR_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMSWW_e, dRes_ID_ALANM_BCK_DAMSWW_e, 0x1, 0x6, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_BOXOPSHORT_e, dRes_ID_ALANM_BCK_BOXOPSHORT_e, 0xFE, 0xFE, FTANM_BOXOPSHORT, dRes_ID_ALANM_BCK_FBOXOPSHORT_e, 0x0},
    {dRes_ID_ALANM_BCK_BOXOPKICK_e, dRes_ID_ALANM_BCK_BOXOPKICK_e, 0xFE, 0xFE, FTANM_BOXOPKICK, dRes_ID_ALANM_BCK_FBOXOPKICK_e, 0x0},
    {dRes_ID_ALANM_BCK_BOXOP_e, dRes_ID_ALANM_BCK_BOXOP_e, 0xFE, 0xFE, FTANM_BOXOP, dRes_ID_ALANM_BCK_FBOXOP_e, 0x0},
    {dRes_ID_ALANM_BCK_GETA_e, dRes_ID_ALANM_BCK_GETA_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FGET_e, 0x0},
    {dRes_ID_ALANM_BCK_GETAWAIT_e, dRes_ID_ALANM_BCK_GETAWAIT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FGETWAIT_e, 0x0},
    {dRes_ID_ALANM_BCK_GETAWAIT_e, dRes_ID_ALANM_BCK_WAITS_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FG_e, 0x0},
    {dRes_ID_ALANM_BCK_GETAWAIT_e, dRes_ID_ALANM_BCK_HOLDOUT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FG_e, 0x0},
    {dRes_ID_ALANM_BCK_TURNBACK_e, dRes_ID_ALANM_BCK_TURNBACK_e, 0x4, 0xA, FTANM_TURNBACK, dRes_ID_ALANM_BCK_FTURNBACK_e, 0x0},
    {dRes_ID_ALANM_BCK_VJMPST_e, dRes_ID_ALANM_BCK_VJMPST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FVJMPST_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITSIT_e, dRes_ID_ALANM_BCK_WAITSIT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITCANOER_e, dRes_ID_ALANM_BCK_WAITCANOEL_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITCANOER_e, dRes_ID_ALANM_BCK_WAITCANOER_e, 0x0, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITSIT_e, dRes_ID_ALANM_BCK_ROWL_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITSIT_e, dRes_ID_ALANM_BCK_ROWR_e, 0x0, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITSIT_e, dRes_ID_ALANM_BCK_ROWBL_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITSIT_e, dRes_ID_ALANM_BCK_ROWBR_e, 0x0, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITSIT_e, dRes_ID_ALANM_BCK_CHANGEHAND_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CANOEFISHL_e, dRes_ID_ALANM_BCK_CANOEFISHL_e, 0x2, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FF_e, 0x0},
    {dRes_ID_ALANM_BCK_CANOEFISHR_e, dRes_ID_ALANM_BCK_CANOEFISHR_e, 0x2, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FF_e, 0x0},
    {dRes_ID_ALANM_BCK_CANOEREELING_e, dRes_ID_ALANM_BCK_CANOEREELING_e, 0x2, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CANORELEASE_e, dRes_ID_ALANM_BCK_CANORELEASE_e, 0x2, 0x5, FTANM_CANORELEASE, dRes_ID_ALANM_BCK_FCANORELEASE_e, 0x0},
    {dRes_ID_ALANM_BCK_PADORUTOROD_e, dRes_ID_ALANM_BCK_PADORUTOROD_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_REELING_e, dRes_ID_ALANM_BCK_REELING_e, 0x2, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_REELINGLR_e, dRes_ID_ALANM_BCK_REELINGLR_e, 0x2, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LANDINGUP_e, dRes_ID_ALANM_BCK_LANDINGUP_e, 0x2, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_DOOROPA_e, dRes_ID_ALANM_BCK_DOOROPA_e, 0x4, 0xA, FTANM_DOOROPA, dRes_ID_ALANM_BCK_FDOOROPA_e, 0x0},
    {dRes_ID_ALANM_BCK_DOOROPB_e, dRes_ID_ALANM_BCK_DOOROPB_e, 0x4, 0xA, FTANM_DOOROPB, dRes_ID_ALANM_BCK_FDOOROPB_e, 0x0},
    {dRes_ID_ALANM_BCK_DOORLOCKA_e, dRes_ID_ALANM_BCK_DOORLOCKA_e, 0x3, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_DOORLOCKB_e, dRes_ID_ALANM_BCK_DOORLOCKB_e, 0x3, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_DOOROPC_e, dRes_ID_ALANM_BCK_DOOROPC_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_DOOROPD_e, dRes_ID_ALANM_BCK_DOOROPD_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_PICKUP_e, dRes_ID_ALANM_BCK_PICKUP_e, 0xFE, 0xFE, FTANM_PICKUP, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_ROOLJMP_e, dRes_ID_ALANM_BCK_ROOLJMP_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_MROLLL_e, dRes_ID_ALANM_BCK_MROLLL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_MROLLR_e, dRes_ID_ALANM_BCK_MROLLR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHWAIT_e, dRes_ID_ALANM_BCK_SHWAIT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHFRONT_e, dRes_ID_ALANM_BCK_SHFRONT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHCROUCH_e, dRes_ID_ALANM_BCK_SHCROUCH_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHSTOP_e, dRes_ID_ALANM_BCK_SHSTOP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHJUMPST_e, dRes_ID_ALANM_BCK_SHJUMPST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHJUMP_e, dRes_ID_ALANM_BCK_SHJUMP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHJUMPED_e, dRes_ID_ALANM_BCK_SHJUMPED_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHKICK_e, dRes_ID_ALANM_BCK_SHKICK_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHTP_e, dRes_ID_ALANM_BCK_SHTP_e, 0x64, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SHT_e, dRes_ID_ALANM_BCK_SHT_e, 0x64, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_ROPESWING_e, dRes_ID_ALANM_BCK_ROPESWING_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CHANGEATOW_e, dRes_ID_ALANM_BCK_CHANGEATOW_e, 0xFE, 0xFE, FTANM_CHANGEATOW, dRes_ID_ALANM_BCK_FCHANGEATOW_e, 0x0},
    {dRes_ID_ALANM_BCK_CHANGEWTOA_e, dRes_ID_ALANM_BCK_CHANGEWTOA_e, 0xFE, 0xFE, FTANM_CHANGEWTOA, dRes_ID_ALANM_BCK_FCHANGEWTOA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITWIND_e, dRes_ID_ALANM_BCK_WAITWIND_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FWAITWIND_e, 0x0},
    {dRes_ID_ALANM_BCK_BINDRINKST_e, dRes_ID_ALANM_BCK_BINDRINKST_e, 0x3, 0xFE, FTANM_BINDRINKST, dRes_ID_ALANM_BCK_FBINDRINKST_e, 0x0},
    {dRes_ID_ALANM_BCK_BINDRINK_e, dRes_ID_ALANM_BCK_BINDRINK_e, 0x3, 0xFE, FTANM_BINDRINK, dRes_ID_ALANM_BCK_FBINDRINK_e, 0x0},
    {dRes_ID_ALANM_BCK_BINDRINKED_e, dRes_ID_ALANM_BCK_BINDRINKED_e, 0x3, 0xFE, FTANM_BINDRINKED, dRes_ID_ALANM_BCK_FBINDRINKED_e, 0x0},
    {dRes_ID_ALANM_BCK_BINBAD_e, dRes_ID_ALANM_BCK_BINBAD_e, 0x3, 0xFE, FTANM_BINBAD, dRes_ID_ALANM_BCK_FBINBAD_e, 0x0},
    {dRes_ID_ALANM_BCK_BINOP_e, dRes_ID_ALANM_BCK_BINOP_e, 0x3, 0xFE, FTANM_BINOP, dRes_ID_ALANM_BCK_FBINOP_e, 0x0},
    {dRes_ID_ALANM_BCK_BINOUT_e, dRes_ID_ALANM_BCK_BINOUT_e, 0x3, 0x5, FTANM_BINOUT, dRes_ID_ALANM_BCK_FBINOUT_e, 0x0},
    {dRes_ID_ALANM_BCK_BINFAIRY_e, dRes_ID_ALANM_BCK_BINFAIRY_e, 0x3, 0x5, FTANM_BINFAIRY, dRes_ID_ALANM_BCK_FBINFAIRY_e, 0x0},
    {dRes_ID_ALANM_BCK_BINSWINGS_e, dRes_ID_ALANM_BCK_BINSWINGS_e, 0x3, 0xFE, FTANM_BINSWINGS, dRes_ID_ALANM_BCK_FBINSWINGS_e, 0x0},
    {dRes_ID_ALANM_BCK_BINSWINGU_e, dRes_ID_ALANM_BCK_BINSWINGU_e, 0x3, 0xFE, FTANM_BINSWINGU, dRes_ID_ALANM_BCK_FBINSWINGU_e, 0x0},
    {dRes_ID_ALANM_BCK_BINGET_e, dRes_ID_ALANM_BCK_BINGET_e, 0x3, 0xFE, FTANM_BINGET, dRes_ID_ALANM_BCK_FBINGET_e, 0x0},
    {dRes_ID_ALANM_BCK_BINSCOOP_e, dRes_ID_ALANM_BCK_BINSCOOP_e, 0x3, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_LIGHTING_e, dRes_ID_ALANM_BCK_LIGHTING_e, 0x0, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_POURST_e, dRes_ID_ALANM_BCK_POURST_e, 0x0, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_POUR_e, dRes_ID_ALANM_BCK_POUR_e, 0x0, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_POURED_e, dRes_ID_ALANM_BCK_POURED_e, 0x0, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_GRASSAST_e, dRes_ID_ALANM_BCK_GRASSAST_e, 0x2, 0xFE, FTANM_GRASSAST, dRes_ID_ALANM_BCK_FGRASSAST_e, 0x0},
    {dRes_ID_ALANM_BCK_GRASSAPLAY_e, dRes_ID_ALANM_BCK_GRASSAPLAY_e, 0x2, 0xFE, FTANM_E_A, dRes_ID_ALANM_BCK_FGRASSAPLAY_e, 0x0},
    {dRes_ID_ALANM_BCK_CATCHTAKA_e, dRes_ID_ALANM_BCK_CATCHTAKA_e, 0x2, 0xFE, FTANM_CATCHTAKA, dRes_ID_ALANM_BCK_FCATCHTAKA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITTAKA_e, dRes_ID_ALANM_BCK_WAITTAKA_e, 0x2, 0xFE, FTANM_CATCHTAKA, dRes_ID_ALANM_BCK_FCATCHTAKA_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITDB_e, dRes_ID_ALANM_BCK_WAITDB_e, 0x1, 0x6, FTANM_CUTST, dRes_ID_ALANM_BCK_FWAITST_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITHDB_e, dRes_ID_ALANM_BCK_WAITHDB_e, 0x1, 0x6, FTANM_A_WAITST, dRes_ID_ALANM_BCK_FWAITHDB_e, 0x0},
    {dRes_ID_ALANM_BCK_MGKICK_e, dRes_ID_ALANM_BCK_MGKICK_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_DIE_e, dRes_ID_ALANM_BCK_DIE_e, 0x4, 0xA, FTANM_DIE, dRes_ID_ALANM_BCK_FDIE_e, 0x0},
    {dRes_ID_ALANM_BCK_DIEH_e, dRes_ID_ALANM_BCK_DIEH_e, 0x4, 0xA, FTANM_DIEH, dRes_ID_ALANM_BCK_FDIEH_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMDIEA_e, dRes_ID_ALANM_BCK_SWIMDIEA_e, 0xFE, 0xFE, FTANM_SWIMDIEA, dRes_ID_ALANM_BCK_FSWIMDIEA_e, 0x0},
    {dRes_ID_ALANM_BCK_SWIMDIEP_e, dRes_ID_ALANM_BCK_SWIMDIEP_e, 0xFE, 0xFE, FTANM_SWIMDIEP, dRes_ID_ALANM_BCK_FSWIMDIEP_e, 0x0},
    {dRes_ID_ALANM_BCK_COWAT_e, dRes_ID_ALANM_BCK_COWAT_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FCOWAT_e, 0x0},
    {dRes_ID_ALANM_BCK_COWCATCHST_e, dRes_ID_ALANM_BCK_COWCATCHST_e, 0x3, 0x9, FTANM_COWCATCHST, dRes_ID_ALANM_BCK_FCOWCATCHST_e, 0x0},
    {dRes_ID_ALANM_BCK_COWCATCH_e, dRes_ID_ALANM_BCK_COWCATCH_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_COWAD_e, dRes_ID_ALANM_BCK_COWAD_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_COWPRESS_e, dRes_ID_ALANM_BCK_COWPRESS_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_COWTHROWL_e, dRes_ID_ALANM_BCK_COWTHROWL_e, 0x3, 0x9, FTANM_COWTHROW_L, dRes_ID_ALANM_BCK_FCOWTHROW_e, 0x0},
    {dRes_ID_ALANM_BCK_COWTHROWR_e, dRes_ID_ALANM_BCK_COWTHROWR_e, 0x3, 0x9, FTANM_COWTHROW_R, dRes_ID_ALANM_BCK_FCOWTHROW_e, 0x0},
    {dRes_ID_ALANM_BCK_ATR_e, dRes_ID_ALANM_BCK_COWWAIT_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_COWWLR_e, dRes_ID_ALANM_BCK_COWWLR_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_COWSTROKE_e, dRes_ID_ALANM_BCK_COWSTROKE_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FF_e, 0x0},
    {dRes_ID_ALANM_BCK_ENTRANCE_e, dRes_ID_ALANM_BCK_ENTRANCE_e, 0x4, 0xA, FTANM_ENTRANCE, dRes_ID_ALANM_BCK_FENTRANCE_e, 0x0},
    {dRes_ID_ALANM_BCK_FALL_e, dRes_ID_ALANM_BCK_FALL_e, 0xFE, 0xFE, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_APPEARANCE_e, dRes_ID_ALANM_BCK_APPEARANCE_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_SJMPST_e, dRes_ID_ALANM_BCK_SJMPST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_SJMPED_e, dRes_ID_ALANM_BCK_SJMPED_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_HSJMPL_e, dRes_ID_ALANM_BCK_HSJMPL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSJMPR_e, dRes_ID_ALANM_BCK_HSJMPR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSSHOOT_e, dRes_ID_ALANM_BCK_HSSHOOT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGEDL_e, dRes_ID_ALANM_BCK_HSHANGEDL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGEDR_e, dRes_ID_ALANM_BCK_HSHANGEDR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGL_e, dRes_ID_ALANM_BCK_HSHANGL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGR_e, dRes_ID_ALANM_BCK_HSHANGR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGWAITL_e, dRes_ID_ALANM_BCK_HSHANGWAITL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGWAITR_e, dRes_ID_ALANM_BCK_HSHANGWAITR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGSHOOTL_e, dRes_ID_ALANM_BCK_HSHANGSHOOTL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGSHOOTR_e, dRes_ID_ALANM_BCK_HSHANGSHOOTR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGLH_e, dRes_ID_ALANM_BCK_HSHANGLH_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSHANGRH_e, dRes_ID_ALANM_BCK_HSHANGRH_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLEDL_e, dRes_ID_ALANM_BCK_HSWALLEDL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLEDR_e, dRes_ID_ALANM_BCK_HSWALLEDR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLL_e, dRes_ID_ALANM_BCK_HSWALLL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLR_e, dRes_ID_ALANM_BCK_HSWALLR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLWAITL_e, dRes_ID_ALANM_BCK_HSWALLWAITL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLWAITR_e, dRes_ID_ALANM_BCK_HSWALLWAITR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLSHOOTL_e, dRes_ID_ALANM_BCK_HSWALLSHOOTL_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_HSWALLSHOOTR_e, dRes_ID_ALANM_BCK_HSWALLSHOOTR_e, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_PUTONH_e, dRes_ID_ALANM_BCK_PUTONH_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FPUTONH_e, 0x0},
    {0x102E, 0x102E, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x102D, 0x102D, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x100A, 0x100A, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1008, 0x1008, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1009, 0x1009, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1007, 0x1007, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1021, 0x1021, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1023, 0x1023, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1022, 0x1022, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1024, 0x1024, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1025, 0x1025, 0xFE, 0xFE, FTANM_UNK_94, 0x1011, 0x0},
    {0x1028, 0x1028, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1029, 0x1029, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1026, 0x1026, 0xFE, 0xFE, FTANM_UNK_95, 0x1012, 0x0},
    {0x1027, 0x1027, 0xFE, 0xFE, FTANM_UNK_95, 0x1012, 0x0},
    {0x102A, 0x102A, 0xFE, 0xFE, FTANM_UNK_99, 0x1013, 0x0},
    {0x1019, 0x1019, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x101B, 0x101B, 0xFE, 0xFE, FTANM_UNK_97, 0x100E, 0x0},
    {0x101C, 0x101C, 0xFE, 0xFE, FTANM_UNK_98, 0x100F, 0x0},
    {0x101A, 0x101A, 0xFE, 0xFE, FTANM_UNK_96, 0x100D, 0x0},
    {0x1020, 0x1020, 0xFE, 0xFE, FTANM_UNK_95, 0x1012, 0x0},
    {0x101D, 0x101D, 0xFE, 0xFE, FTANM_UNK_101, 0x1010, 0x0},
    {0x1017, 0x1017, 0xFE, 0xFE, FTANM_UNK_100, 0x100C, 0x0},
    {0x1018, 0x1018, 0xFE, 0xFE, FTANM_UNK_100, 0x100C, 0x0},
    {0x102B, 0x102B, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x102C, 0x102C, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1015, 0x1015, 0xFE, 0xFE, FTANM_UNK_102, 0x100B, 0x0},
    {0x1016, 0x1016, 0xFE, 0xFE, FTANM_UNK_102, 0x100B, 0x0},
    {0x1006, 0x1006, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x1014, 0x1014, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x101E, 0x101E, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x101F, 0x101F, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_DAMFBW_e, dRes_ID_ALANM_BCK_DAMFBW_e, 0xFE, 0xFE, FTANM_DAMFBW, dRes_ID_ALANM_BCK_FDAMFBW_e, 0x0},
    {dRes_ID_ALANM_BCK_IBWAIT_e, dRes_ID_ALANM_BCK_IBWAIT_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBWALK_e, dRes_ID_ALANM_BCK_IBWALK_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBATTACK_e, dRes_ID_ALANM_BCK_IBATTACK_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBTURN_e, dRes_ID_ALANM_BCK_IBTURN_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBTHROW_e, dRes_ID_ALANM_BCK_IBTHROW_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBPULL_e, dRes_ID_ALANM_BCK_IBPULL_e, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBCATCH_e, dRes_ID_ALANM_BCK_IBCATCH_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_IBDEF_e, dRes_ID_ALANM_BCK_IBDEF_e, 0xFE, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x200C, 0x200C, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x200D, 0x200D, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x200E, 0x200E, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x200F, 0x200F, 0x64, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x2009, 0x2009, 0x64, 0x9, FTANM_UNK_103, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x200A, 0x200A, 0x64, 0x9, FTANM_UNK_104, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x200B, 0x200B, 0x64, 0xA, FTANM_UNK_105, 0x2008, 0x0},
    {0x2010, 0x2010, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {0x500D, 0x500D, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x500E, 0x500E, 0x1, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x500A, 0x500A, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x500B, 0x500B, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x500C, 0x500C, 0x64, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x5007, 0x5007, 0x64, 0x9, FTANM_UNK_106, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x5008, 0x5008, 0x64, 0x9, FTANM_UNK_107, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x5009, 0x5009, 0x64, 0xA, FTANM_UNK_108, 0x500F, 0x0},
    {dRes_ID_ALANM_BCK_RODSWING_e, dRes_ID_ALANM_BCK_RODSWING_e, 0x2, 0x5, FTANM_RODSWING, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_RODSWINGL_e, dRes_ID_ALANM_BCK_RODSWINGL_e, 0x2, 0x5, FTANM_RODSWINGL, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_GETSWL_e, dRes_ID_ALANM_BCK_GETSWL_e, 0x2, 0xA, FTANM_GETSWL, dRes_ID_ALANM_BCK_FGETSWL_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITF_e, dRes_ID_ALANM_BCK_WAITF_e, 0x3, 0x9, FTANM_B_A, dRes_ID_ALANM_BCK_FB_e, 0x0},
    {dRes_ID_ALANM_BCK_NUMBA_e, dRes_ID_ALANM_BCK_NUMBA_e, 0x3, 0x9, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_NUMBB_e, dRes_ID_ALANM_BCK_NUMBB_e, 0x3, 0x9, FTANM_C_A, dRes_ID_ALANM_BCK_FC_e, 0x0},
    {dRes_ID_ALANM_BCK_KEYCATCHH_e, dRes_ID_ALANM_BCK_KEYCATCHH_e, 0x4, 0x5, FTANM_KEYCATCHH, dRes_ID_ALANM_BCK_FKEYCATCHH_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITHS_e, dRes_ID_ALANM_BCK_GETAWAIT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FGETWAIT_e, 0x0},
    {0x3004, 0x3004, 0x1, 0x6, FTANM_0, 0x3003, 0x0},
    {dRes_ID_ALANM_BCK_CHAINPICKUP_e, dRes_ID_ALANM_BCK_CHAINPICKUP_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_CHAIN_e, dRes_ID_ALANM_BCK_CHAIN_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CHAINFM_e, dRes_ID_ALANM_BCK_CHAINFM_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CHAINED_e, dRes_ID_ALANM_BCK_CHAINED_e, 0x3, 0x9, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_WAITINSECT_e, dRes_ID_ALANM_BCK_WAITINSECT_e, 0x4, 0xA, FTANM_WAITINSECT, dRes_ID_ALANM_BCK_FWAITINSECT_e, 0x0},
    {dRes_ID_ALANM_BCK_DEMOMHOP_e, dRes_ID_ALANM_BCK_DEMOMHOP_e, 0x4, 0xA, FTANM_DEMOMHOP, dRes_ID_ALANM_BCK_FDEMOMHOP_e, 0x0},
    {dRes_ID_ALANM_BCK_HANGIBT_e, dRes_ID_ALANM_BCK_HANGIBT_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_WARASHIBE_e, dRes_ID_ALANM_BCK_WARASHIBE_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FWARASHIBE_e, 0x0},
    {dRes_ID_ALANM_BCK_WARASHIBE_WAIT_e, dRes_ID_ALANM_BCK_WARASHIBE_WAIT_e, 0x4, 0xA, FTANM_0, dRes_ID_ALANM_BCK_FG_e, 0x0},
    {dRes_ID_ALANM_BCK_DEMO_KAMAE_e, dRes_ID_ALANM_BCK_DEMO_KAMAE_e, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTJP_e, dRes_ID_ALANM_BCK_CUTTJP_e, 0x64, 0x6, FTANM_CUTTJP, dRes_ID_ALANM_BCK_FCUTTJP_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTJST_e, dRes_ID_ALANM_BCK_CUTTJST_e, 0x64, 0x6, FTANM_CUTTJP, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTJ_e, dRes_ID_ALANM_BCK_CUTTJ_e, 0x64, 0x6, FTANM_CUTTJ, dRes_ID_ALANM_BCK_FCUTTJ_e, 0x0},
    {dRes_ID_ALANM_BCK_CUTTJED_e, dRes_ID_ALANM_BCK_CUTTJED_e, 0x64, 0x6, FTANM_CUTTJED, dRes_ID_ALANM_BCK_FCUTTJED_e, 0x0},
    {0x600B, 0x600B, 0x0, 0xA, FTANM_UNK_133, 0x6009, 0x0},
    {0x600C, 0x600C, 0x0, 0x5, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x600A, 0x600A, 0x64, 0x5, FTANM_0, 0x6008, 0x0},
    {0x700A, 0x700A, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x700B, 0x700B, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x700C, 0x700C, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x700E, 0x700E, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x700D, 0x700D, 0x0, 0x6, FTANM_0, dRes_ID_ALANM_BCK_FAT_e, 0x0},
    {0x7008, 0x7008, 0x0, 0x5, FTANM_UNK_134, 0x7010, 0x0},
    {0x7009, 0x7009, 0x0, 0x5, FTANM_0, 0x7011, 0x0},
    {dRes_ID_ALANM_BCK_WAITHS_e, 0x700F, 0x4, 0x5, FTANM_UNK_135, 0x7012, 0x0},
    {dRes_ID_ALANM_BCK_JUMPBST_e, dRes_ID_ALANM_BCK_JUMPBST_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_JUMPB_e, dRes_ID_ALANM_BCK_JUMPB_e, 0xFE, 0xFE, FTANM_0, dRes_ID_ALANM_BCK_FA_e, 0x0},
    {dRes_ID_ALANM_BCK_ODOROKU_e, dRes_ID_ALANM_BCK_ODOROKU_e, 0xFE, 0xFE, FTANM_ODOROKU, dRes_ID_ALANM_BCK_FODOROKU_e, 0x0},
    {dRes_ID_ALANM_BCK_ASHIMOTO_e, dRes_ID_ALANM_BCK_ASHIMOTO_e, 0xFE, 0xFE, FTANM_ASHIMOTO, dRes_ID_ALANM_BCK_FASHIMOTO_e, 0x0},
};

daAlink_WlAnmData const daAlink_c::m_wlAnmDataTable[daAlink_c::WANM_MAX] = {
    {dRes_ID_ALANM_BCK_WL_WAITA_e, 0x0, 0x1, 10, 40, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WALKA_e, 0x0, 0x2, 1, 14, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WALKB_e, 0x0, 0x2, 1, 14, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DASHA_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPAST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPA_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPAED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITPP_e, 0x0, 0x1, 15, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PUSHW_e, 0x0, 0x7, 2, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PUSHTSWST_e, 0x0, 0x7, 6, 32, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PUSHTSW_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SIT_e, 0x1, 0x0, 7, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_LIE_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_LIEF_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITSIT_e, 0x1, 0x0, 1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_BOXOP_e, 0x1, 0x7, 9, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_VJMPST_e, 0x1, 0x5, 9, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_BUMPST_e, 0x1, 0x5, 3, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_BUMPED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_VJMPCL_e, 0x1, 0x6, 25, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_VJMPCH_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_TURNBACK_e, 0x0, 0x8, 3, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAM_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITD_e, 0x0, 0x3, 10, 30, 50, -1},
    {dRes_ID_ALANM_BCK_WL_WAITATOD_e, 0x0, 0x3, 6, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HANG_e, 0x1, 0x5, 6, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SLIP_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_TURN_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_AT_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WALKSLOPEW_e, 0x1, 0x2, 1, 18, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SLIDEB_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SLIDEBED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SLIDEFST_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROPEW_e, 0x0, 0x2, 1, 14, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROPECLL_e, 0x1, 0x5, 20, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROPECLR_e, 0x1, 0x5, 26, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROPETURN_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROPEWAIT_e, 0x8, 0x1, 25, 50, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROPEFALTER_e, 0x9, 0x2, 1, 14, 30, 45},
    {dRes_ID_ALANM_BCK_WL_DASHB_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFF_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFB_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFL_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFR_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFFUP_e, 0x2, 0xA, 32, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFBUP_e, 0x2, 0xB, 20, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFLUP_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMFRUP_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWAITA_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITB_e, 0x0, 0x1, 10, 40, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPBST_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPBTURNST_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPB_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPBED_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWIMP_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWIMWAIT_e, 0x0, 0x1, 13, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWIMING_e, 0x0, 0x2, 1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWIMDASH_e, 0x0, 0x4, 3, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWAITB_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITST_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMF_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMB_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAML_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DAMR_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKBL_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKBR_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKBS_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKBT_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATRBW_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATRW_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATRD_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PICKUPA_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATSJLST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATSJLED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATSJRST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATSJRED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATSJBST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATSJBED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PICKUPB_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PICKUPTHROW_e, 0x0, 0x9, 7, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKAST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKA_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKAED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKAEDF_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKAEDB_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKTST_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKT_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HDHANG_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HDHANGWAIT_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HDHANGSW_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CUTSTL_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CUTSTR_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HOWLA_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HOWLB_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HOWLC_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_HOWLED_e, 0x0, 0x1, 30, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_LANDDAMA_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_LANDDAMAST_e, 0x1, 0xA, 4, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKUST_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKU_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKUED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKUNG_e, 0x1, 0xB, 40, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROLLBST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ROLLBED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DIG_e, 0x0, 0xB, 65, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DIGIN_e, 0xD, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_THROUGH_e, 0x0, 0x5, 57, 70, 85, -1},
    {dRes_ID_ALANM_BCK_WL_SLIDEW_e, 0x1, 0x6, 1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_JUMPKICK_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_PULLOUT_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CUTSTP_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CHANGEATOW_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CHANGEWTOA_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITWIND_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SMELL_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DASHST_e, 0x2, 0x4, 1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKREST_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKREED_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_FLINGL_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_FLINGR_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CATCHAST_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CATCHA_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ATTACKREGD_e, 0x1, 0x5, 16, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_DIE_e, 0x6, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWIMDIEA_e, 0x7, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWIMDIEP_e, 0x7, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITF_e, 0x2, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITATOSD_e, 0x0, 0x1, 15, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_SWAITD_e, 0x0, 0x1, 15, 45, -1, -1},
    {dRes_ID_ALANM_BCK_WL_MDSHOCK_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_ENTRANCE_e, 0x3, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_GETA_e, 0x1, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_GET_e, 0x1, 0x0, 5, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_WAITYC_e, 0x0, 0x1, 1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CATCHYC_e, 0x5, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_NUMBA_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_NUMBB_e, 0x2, 0xC, -1, -1, -1, -1},
    {0x4003, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CHAIN_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_CHAINED_e, 0x0, 0xC, -1, -1, -1, -1},
    {dRes_ID_ALANM_BCK_WL_LISTEN_e, 0x1, 0x0, 20, 58, -1, -1},
    {0x8028, 0xA, 0xC, -1, -1, -1, -1},
    {0x8027, 0xA, 0xC, -1, -1, -1, -1},
    {0x802A, 0xB, 0xC, -1, -1, -1, -1},
    {0x8029, 0xB, 0xC, -1, -1, -1, -1},
    {0x802C, 0xC, 0xC, -1, -1, -1, -1},
    {0x802B, 0xC, 0xC, -1, -1, -1, -1},
};

daAlink_FaceTexData const daAlink_c::m_faceTexDataTable[] = {
    {dRes_ID_ALANM_BTP_FMABA01_e, dRes_ID_ALANM_BTK_FMABA01_e},
    {dRes_ID_ALANM_BTP_FMABA02_e, dRes_ID_ALANM_BTK_FMABA02_e},
    {dRes_ID_ALANM_BTP_FMABA03_e, dRes_ID_ALANM_BTK_FMABA03_e},
    {dRes_ID_ALANM_BTP_FMABA01_e, dRes_ID_ALANM_BTK_FL_e},
    {dRes_ID_ALANM_BTP_FMABA01_e, dRes_ID_ALANM_BTK_FR_e},
    {dRes_ID_ALANM_BTP_FMABAGOMA_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FDAM01_e, dRes_ID_ALANM_BTK_FFINISHA_e},
    {dRes_ID_ALANM_BTP_FFINISHA_e, dRes_ID_ALANM_BTK_FFINISHED_e},
    {dRes_ID_ALANM_BTP_FARELORD_e, dRes_ID_ALANM_BTK_FARELORD_e},
    {dRes_ID_ALANM_BTP_FARELORDTAME_e, dRes_ID_ALANM_BTK_FARELORDTAME_e},       
    {dRes_ID_ALANM_BTP_FPUSHW_e, dRes_ID_ALANM_BTK_FPUSHW_e},
    {dRes_ID_ALANM_BTP_FPULLW_e, dRes_ID_ALANM_BTK_FPULLW_e},
    {dRes_ID_ALANM_BTP_FWAITST_e, dRes_ID_ALANM_BTK_FWAITST_e},
    {dRes_ID_ALANM_BTP_FCUTST_e, dRes_ID_ALANM_BTK_FCUTST_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FWAITST_e},
    {dRes_ID_ALANM_BTP_FWAITHDB_e, dRes_ID_ALANM_BTK_FWAITHDB_e},
    {dRes_ID_ALANM_BTP_FWAITWB_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FWAITWATOWB_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FCUTUNG_e, dRes_ID_ALANM_BTK_FCUTUNG_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTDL_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTDR_e},
    {dRes_ID_ALANM_BTP_FSWIMINGA_e, dRes_ID_ALANM_BTK_FSWIMINGA_e},
    {dRes_ID_ALANM_BTP_FSWIMP_e, dRes_ID_ALANM_BTK_FSWIMP_e},
    {dRes_ID_ALANM_BTP_FSWIMDIVE_e, dRes_ID_ALANM_BTK_FSWIMDIVE_e},
    {dRes_ID_ALANM_BTP_FGRABP_e, dRes_ID_ALANM_BTK_FGRABP_e},
    {dRes_ID_ALANM_BTP_FGRABUP_e, dRes_ID_ALANM_BTK_FGRABUP_e},
    {dRes_ID_ALANM_BTP_FHEAVYTHROW_e, dRes_ID_ALANM_BTK_FHEAVYTHROW_e},
    {dRes_ID_ALANM_BTP_FGRABNG_e, dRes_ID_ALANM_BTK_FGRABNG_e},
    {dRes_ID_ALANM_BTP_FSWAITA_e, dRes_ID_ALANM_BTK_FSWAITA_e},
    {dRes_ID_ALANM_BTP_FPICKUP_e, dRes_ID_ALANM_BTK_FPICKUP_e},
    {dRes_ID_ALANM_BTP_FDOOROPA_e, dRes_ID_ALANM_BTK_FDOOROPA_e},
    {dRes_ID_ALANM_BTP_FDOOROPB_e, dRes_ID_ALANM_BTK_FDOOROPB_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTHLA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTHLB_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTHRA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTHRB_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTHTB_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FTURNBS_e},
    {dRes_ID_ALANM_BTP_FROLLFGOOD_e, dRes_ID_ALANM_BTK_FROLLFGOOD_e},
    {dRes_ID_ALANM_BTP_FB_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FC_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FTURNBACK_e},
    {dRes_ID_ALANM_BTP_FDAMFFUP_e, dRes_ID_ALANM_BTK_FDAMFFUP_e},
    {dRes_ID_ALANM_BTP_FDAMFBUP_e, dRes_ID_ALANM_BTK_FDAMFBUP_e},
    {dRes_ID_ALANM_BTP_FDAMFRLUP_e, dRes_ID_ALANM_BTK_FDAMFRLUP_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FWAITATOS_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTL_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTR_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTF_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTEA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTEB_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTEG_e},
    {dRes_ID_ALANM_BTP_FCHANGEATOW_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FCHANGEWTOA_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FSWAITHA_e, dRes_ID_ALANM_BTK_FSWAITHA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FSWIATHB_e},
    {dRes_ID_ALANM_BTP_FDASHWIND_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTTB_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTT_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTJST_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTJED_e},
    {dRes_ID_ALANM_BTP_FBINDRINKST_e, dRes_ID_ALANM_BTK_FBINDRINKST_e},
    {dRes_ID_ALANM_BTP_FBINDRINK_e, dRes_ID_ALANM_BTK_FBINDRINK_e},
    {dRes_ID_ALANM_BTP_FBINDRINKED_e, dRes_ID_ALANM_BTK_FBINDRINKED_e},
    {dRes_ID_ALANM_BTP_FBINBAD_e, dRes_ID_ALANM_BTK_FBINBAD_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FBINOP_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FBINOUT_e},
    {dRes_ID_ALANM_BTP_FBINFAIRY_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FBINSWINGS_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FBINSWINGU_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FBINGET_e},
    {dRes_ID_ALANM_BTP_FI_e, dRes_ID_ALANM_BTK_FBINGET_e},
    {dRes_ID_ALANM_BTP_FK_e, dRes_ID_ALANM_BTK_FBINGET_e},
    {dRes_ID_ALANM_BTP_FGRASSAST_e, dRes_ID_ALANM_BTK_FGRASSAST_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCATCHTAKA_e},
    {dRes_ID_ALANM_BTP_FE_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FBOXOPSHORT_e, dRes_ID_ALANM_BTK_FBOXOPSHORT_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FBOXOPKICK_e},
    {dRes_ID_ALANM_BTP_FBOXOP_e, dRes_ID_ALANM_BTK_FBOXOP_e},
    {dRes_ID_ALANM_BTP_FDIE_e, dRes_ID_ALANM_BTK_FDIE_e},
    {dRes_ID_ALANM_BTP_FDIEH_e, dRes_ID_ALANM_BTK_FDIEH_e},
    {dRes_ID_ALANM_BTP_FSWIMDIEA_e, dRes_ID_ALANM_BTK_FSWIMDIEA_e},
    {dRes_ID_ALANM_BTP_FSWIMDIEP_e, dRes_ID_ALANM_BTK_FSWIMDIEP_e},
    {dRes_ID_ALANM_BTP_FENTRANCE_e, dRes_ID_ALANM_BTK_FENTRANCE_e},
    {dRes_ID_ALANM_BTP_FCOWCATCHST_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FCOWTHROW_e, dRes_ID_ALANM_BTK_FCOWTHROWL_e},
    {dRes_ID_ALANM_BTP_FCOWTHROW_e, dRes_ID_ALANM_BTK_FCOWTHROWR_e},
    {dRes_ID_ALANM_BTP_FDIEHUP_e, dRes_ID_ALANM_BTK_FDIEHUP_e},
    {dRes_ID_ALANM_BTP_FCUTRE_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTU_e},
    {dRes_ID_ALANM_BTP_FCUTUED_e, dRes_ID_ALANM_BTK_FCUTUED_e},
    {dRes_ID_ALANM_BTP_FCLIMBHANGMISS_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FDAMFBW_e, dRes_ID_ALANM_BTK_FA_e},
    {0x45 | (1 << 12), 0x3B | (1 << 12)},
    {0x46 | (1 << 12), dRes_ID_ALANM_BTK_FA_e},
    {0x41 | (1 << 12), 0x37 | (1 << 12)},
    {0x42 | (1 << 12), 0x38 | (1 << 12)},
    {0x43 | (1 << 12), 0x39 | (1 << 12)},
    {0x47 | (1 << 12), 0x3C | (1 << 12)},
    {0x40 | (1 << 12), 0x36 | (1 << 12)},
    {0x44 | (1 << 12), 0x3A | (1 << 12)},
    {0x3F | (1 << 12), dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FA_e, 0x32 | (2 << 12)},
    {dRes_ID_ALANM_BTP_FA_e, 0x33 | (2 << 12)},
    {0x39 | (2 << 12), 0x34 | (2 << 12)},
    {dRes_ID_ALANM_BTP_FA_e, 0x54 | (5 << 12)},
    {dRes_ID_ALANM_BTP_FA_e, 0x55 | (5 << 12)},
    {0x59 | (5 << 12), 0x56 | (5 << 12)},
    {dRes_ID_ALANM_BTP_FSPILLH_e, dRes_ID_ALANM_BTK_FSPILLH_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FHANGH_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FRODSWING_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FRODSWINGL_e},
    {dRes_ID_ALANM_BTP_FGETSWL_e, dRes_ID_ALANM_BTK_FGETSWL_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FTURNLS_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FTURNRS_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FKEYCATCHH_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FDEMOTALKA_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FDEMOTALKB_e},
    {dRes_ID_ALANM_BTP_FDEMOTALKC_e, dRes_ID_ALANM_BTK_FDEMOTALKC_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCANORELEASE_e},
    {dRes_ID_ALANM_BTP_FWAITINSECT_e, dRes_ID_ALANM_BTK_FWAITINSECT_e},
    {dRes_ID_ALANM_BTP_FI_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FJ_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FK_e, dRes_ID_ALANM_BTK_FA_e},
    {dRes_ID_ALANM_BTP_FATDEFNG_e, dRes_ID_ALANM_BTK_FATDEFNG_e},
    {dRes_ID_ALANM_BTP_FDEMOMHOP_e, dRes_ID_ALANM_BTK_FDEMOMHOP_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTEHST_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTEH_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTTJP_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTTJST_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTTJ_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FCUTTJED_e},
    {0x18 | (6 << 12), 0x15 | (6 << 12)},
    {dRes_ID_ALANM_BTP_FA_e, 0x70 | (7 << 12)},
    {dRes_ID_ALANM_BTP_FA_e, 0x71 | (7 << 12)},
    {dRes_ID_ALANM_BTP_FODOROKU_e, dRes_ID_ALANM_BTK_FODOROKU_e},
    {dRes_ID_ALANM_BTP_FASHIMOTO_e, dRes_ID_ALANM_BTK_FASHIMOTO_e},
    {dRes_ID_ALANM_BTP_FA_e, dRes_ID_ALANM_BTK_FUNAZUKU_e},
    {dRes_ID_ALANM_BTP_WL_FMABA01_e, dRes_ID_ALANM_BTK_WL_FMABA01_e},
    {dRes_ID_ALANM_BTP_WL_FMABA02_e, dRes_ID_ALANM_BTK_WL_FMABA02_e},
    {dRes_ID_ALANM_BTP_WL_FSWAITA_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FSWIMP_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FSWAITB_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FDAM_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FB_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FDAMFFBUP_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FDAMFLRUP_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FWAITST_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FLANDDAMA_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FLANDDAMAST_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FATTACKUNG_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FDASHWIND_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FTHROUGH_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FATTACKREST_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FATTACKREED_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FDIE_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FSWIMDIEA_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FSWIMDIEP_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FMDSHOCK_e, dRes_ID_ALANM_BTK_WL_FMDSHOCK_e},
    {dRes_ID_ALANM_BTP_WL_FENTRANCE_e, dRes_ID_ALANM_BTK_WL_FENTRANCE_e},       
    {dRes_ID_ALANM_BTP_WL_FHOWLC_e, dRes_ID_ALANM_BTK_WL_FA_e},
    {dRes_ID_ALANM_BTP_WL_FC_e, dRes_ID_ALANM_BTK_WL_FA_e},
};

#include "d/actor/d_a_alink_link.inc"

#include "d/actor/d_a_alink_cut.inc"

#include "d/actor/d_a_alink_damage.inc"

#include "d/actor/d_a_alink_guard.inc"

#include "d/actor/d_a_alink_bow.inc"

#include "d/actor/d_a_alink_boom.inc"

#include "d/actor/d_a_alink_copyrod.inc"

#include "d/actor/d_a_alink_hvyboots.inc"

#include "d/actor/d_a_alink_bomb.inc"

static f32 l_autoUpHeight = 30.010000228881836f;

static f32 l_autoDownHeight = -30.010000228881836f;

#include "d/actor/d_a_alink_grab.inc"

#include "d/actor/d_a_alink_sumou.inc"

#include "d/actor/d_a_alink_horse.inc"

#include "d/actor/d_a_alink_canoe.inc"

#include "d/actor/d_a_alink_crawl.inc"

static f32 l_ladderAnmBaseTransY = 102.00054168701172f;

#include "d/actor/d_a_alink_hang.inc"

#include "d/actor/d_a_alink_swim.inc"

#include "d/actor/d_a_alink_iceleaf.inc"

#include "d/actor/d_a_alink_hook.inc"

#include "d/actor/d_a_alink_spinner.inc"

#include "d/actor/d_a_alink_bottle.inc"

#include "d/actor/d_a_alink_kandelaar.inc"

#include "d/actor/d_a_alink_whistle.inc"

static dCcD_SrcCyl l_cylSrc = {
    {
        {0, {{AT_TYPE_WOLF_ATTACK, 3, 0x1A}, {0xD8FFFDFF, 5}, 0x73}},
        {dCcD_SE_WOLF_BITE, 3, 1, 0, {1}},
        {dCcD_SE_NONE, 6, 0, 0, {0}},
        {0},
    },
    {
        {
            {0.0f, 0.0f, 0.0f},
            35.0f,
            180.0f,
        },
    }
};

static dCcD_SrcSph l_sphSrc = {
    {
        {0, {{AT_TYPE_NORMAL_SWORD, 3, 0x1A}, {0xD8FBFDFF, 5}, 0x73}},
        {dCcD_SE_SWORD, 3, 1, 0, {1}},
        {dCcD_SE_NONE, 6, 0, 0, {0}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        40.0f,
    }},
};

static dCcD_SrcCyl l_atCylSrc = {
    {
        {0, {{AT_TYPE_NORMAL_SWORD, 2, 0x1B}, {0, 0}, 0}},
        {dCcD_SE_SWORD, 3, 1, 0, {1}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {0},
    },
    {
        {
            {0.0f, 0.0f, 0.0f},
            180.0f,
            130.0f,
        },
    }
};

static dCcD_SrcCps l_atCpsSrc = {
    {
        {0, {{AT_TYPE_NORMAL_SWORD, 1, 0x1B}, {0, 0}, 0}},
        {dCcD_SE_SWORD, 1, 0, 0, {1}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {0},
    },
    {{
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        20.0f,
    }},
};

static Vec l_jntColPos0[] = {
    {0.0f, 0.0f, 0.0f},
    {44.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos1 = {
    0.0f, -10.0f, 0.0f
};

static Vec l_jntColPos2[] = {
    {-5.0f, 0.0f, 0.0f},
    {28.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos4[] = {
    {-4.0f, 0.0f, 0.0f},
    {28.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos6 = { 8.0f, 0.0f, 0.0f };

static Vec l_jntColPos8[] = {
    {0.0f, 0.0f, 0.0f},
    {38.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos9[] = {
    {0.0f, -5.0f, 0.0f},
    {35.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos10[] = {
    {0.0f, 5.0f, 0.0f},
    {35.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos11[] = {
    {0.0f, 0.0f, 0.0f},
    {48.0f, 2.0f, 0.0f}
};

static Vec l_jntColPos12[] = {
    {0.0f, 0.0f, 0.0f},
    {48.0f, -2.0f, 0.0f}
};

static Vec l_jntColPos13[] = {
    {0.0f, 5.0f, 0.0f},
    {22.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos14[] = {
    {0.0f, -5.0f, 0.0f},
    {22.0f, 0.0f, 0.0f}
};

static Vec l_jntColPos15[] = {
    {-15.0f, 0.0f, 17.0f},
    {0.0f, 0.0f, 1.0f}
};

static Vec l_jntColPos16[] = {
    {13.0f, 13.0f, -8.0f},
    {0.0f, 1.0f, 0.0f}
};

static Vec l_jntColPos17 = {
    -15.0f, -10.0f, -30.0f
};

static dJntColData_c l_jntColData[] = {
    {
        1,
        0,
        1,
        18.0f,
        l_jntColPos0,
    },
    {
        0,
        0,
        4,
        16.0f,
        &l_jntColPos1,
    },
    {
        1,
        0,
        7,
        7.0f,
        l_jntColPos2,
    },
    {
        1,
        0,
        12,
        7.0f,
        l_jntColPos2,
    },
    {
        1,
        0,
        8,
        6.0f,
        l_jntColPos4,
    },
    {
        1,
        0,
        13,
        6.0f,
        l_jntColPos4,
    },
    {
        0,
        0,
        9,
        6.0f,
        &l_jntColPos6,
    },
    {
        0,
        0,
        14,
        6.0f,
        &l_jntColPos6,
    },
    {
        1,
        0,
        16,
        18.0f,
        l_jntColPos8,
    },
    {
        1,
        0,
        18,
        8.0f,
        l_jntColPos9,
    },
    {
        1,
        0,
        23,
        8.0f,
        l_jntColPos10,
    },
    {
        1,
        0,
        19,
        7.0f,
        l_jntColPos11,
    },
    {
        1,
        0,
        24,
        7.0f,
        l_jntColPos12,
    },
    {
        1,
        0,
        20,
        6.0f,
        l_jntColPos13,
    },
    {
        1,
        0,
        25,
        6.0f,
        l_jntColPos14,
    },
    {
        2,
        3,
        15,
        40.0f,
        l_jntColPos15,
    },
    {
        2,
        3,
        5,
        40.0f,
        l_jntColPos16,
    },
    {
        0,
        3,
        15,
        45.0f,
        &l_jntColPos17,
    },
};

static Vec l_wolfJntColPos0[] = {
    {-25.0f, 20.0f, 0.0f},
    {80.0f, 0.0f, 0.0f}
};

static Vec l_wolfJntColPos1 = {
    5.0f, 3.0f, 0.0f
};

static Vec l_wolfJntColPos2[] = {
    {0.0f, 0.0f, 0.0f},
    {35.0f, 3.0f, 0.0f}
};

static Vec l_wolfJntColPos4[] = {
    {0.0f, 2.0f, 0.0f},
    {30.0f, 0.0f, 0.0f}
};

static Vec l_wolfJntColPos6[] = {
    {0.0f, 0.0f, 0.0f},
    {20.0f, 0.0f, 0.0f}
};

static Vec l_wolfJntColPos8[] = {
    {0.0f, -2.0f, 0.0f},
    {12.0f, -2.0f, 0.0f}
};

static Vec l_wolfJntColPos10[] = {
    {0.0f, 0.0f, 0.0f},
    {33.0f, -2.0f, 0.0f}
};

static Vec l_wolfJntColPos12[] = {
    {-5.0f, -2.0f, 0.0f},
    {15.0f, 0.0f, 0.0f}
};

static Vec l_wolfJntColPos14[] = {
    {-4.0f, 0.0f, 0.0f},
    {33.0f, 0.0f, 0.0f}
};

static Vec l_wolfJntColPos16[] = {
    {0.0f, -3.0f, 0.0f},
    {13.0f, -3.0f, 0.0f}
};

static dJntColData_c l_wolfJntColData[] = {
    {
        1,
        0,
        1,
        28.0f,
        l_wolfJntColPos0,
    },
    {
        0,
        0,
        4,
        25.0f,
        &l_wolfJntColPos1,
    },
    {
        1,
        0,
        16,
        10.0f,
        l_wolfJntColPos2,
    },
    {
        1,
        0,
        21,
        10.0f,
        l_wolfJntColPos2,
    },
    {
        1,
        0,
        17,
        6.0f,
        l_wolfJntColPos4,
    },
    {
        1,
        0,
        22,
        6.0f,
        l_wolfJntColPos4,
    },
    {
        1,
        0,
        18,
        4.0f,
        l_wolfJntColPos6,
    },
    {
        1,
        0,
        23,
        4.0f,
        l_wolfJntColPos6,
    },
    {
        1,
        0,
        19,
        4.0f,
        l_wolfJntColPos8,
    },
    {
        1,
        0,
        24,
        4.0f,
        l_wolfJntColPos8,
    },
    {
        1,
        0,
        28,
        11.0f,
        l_wolfJntColPos10,
    },
    {
        1,
        0,
        33,
        11.0f,
        l_wolfJntColPos10,
    },
    {
        1,
        0,
        29,
        7.0f,
        l_wolfJntColPos12,
    },
    {
        1,
        0,
        34,
        7.0f,
        l_wolfJntColPos12,
    },
    {
        1,
        0,
        30,
        5.0f,
        l_wolfJntColPos14,
    },
    {
        1,
        0,
        35,
        5.0f,
        l_wolfJntColPos14,
    },
    {
        1,
        0,
        31,
        6.0f,
        l_wolfJntColPos16,
    },
    {
        1,
        0,
        36,
        5.0f,
        l_wolfJntColPos16,
    },
};

#include "d/actor/d_a_alink_ironball.inc"

#include "d/actor/d_a_alink_demo.inc"

#include "d/actor/d_a_alink_effect.inc"

#include "d/actor/d_a_alink_wolf.inc"

#include "d/actor/d_a_alink_swindow.inc"

// TODO: These ctors and dtors below might need to go in a new .inc file that gets included right
// after d_a_alink_swindow.inc to slightly improve function order?
// Making them weak functions in a header doesn't seem to work at least.

daAlinkHIO_c::daAlinkHIO_c() {}

daAlinkHIO_wolf_c::~daAlinkHIO_wolf_c() {}

#if DEBUG
daAlinkHIO_swim_c::~daAlinkHIO_swim_c() {}

daAlinkHIO_grab_c::~daAlinkHIO_grab_c() {}

daAlinkHIO_roofHang_c::~daAlinkHIO_roofHang_c() {}

daAlinkHIO_ladder_c::~daAlinkHIO_ladder_c() {}

daAlinkHIO_item_c::~daAlinkHIO_item_c() {}

daAlinkHIO_canoe_c::~daAlinkHIO_canoe_c() {}

daAlinkHIO_horse_c::~daAlinkHIO_horse_c() {}

daAlinkHIO_damage_c::~daAlinkHIO_damage_c() {}

daAlinkHIO_pushpull_c::~daAlinkHIO_pushpull_c() {}

daAlinkHIO_wallHang_c::~daAlinkHIO_wallHang_c() {}

daAlinkHIO_autoJump_c::~daAlinkHIO_autoJump_c() {}

daAlinkHIO_crouch_c::~daAlinkHIO_crouch_c() {}

daAlinkHIO_guard_c::~daAlinkHIO_guard_c() {}

daAlinkHIO_atnMove_c::~daAlinkHIO_atnMove_c() {}
#endif

daAlinkHIO_cut_c::~daAlinkHIO_cut_c() {}

daAlinkHIO_c::~daAlinkHIO_c() {}

bool daAlink_matAnm_c::m_eye_move_flg;

u8 daAlink_matAnm_c::m_morf_frame;

void daAlink_matAnm_c::init() {
    field_0xf4 = 0.0f;
    field_0xf8 = 0.0f;
    mNowOffsetX = 0.0f;
    mNowOffsetY = 0.0f;
    m_eye_move_flg = false;
    m_morf_frame = 0;
    mSetFlag = 0;
}

void daAlink_matAnm_c::calc(J3DMaterial* i_material) const {
    J3DMaterialAnm::calc(i_material);

    if (!daAlink_getAlinkActorClass()->checkStatusWindowDraw()) {
        for (u32 i = 0; i < 8; i++) {
            if (getTexMtxAnm(i).getAnmFlag()) {
                J3DTexMtxInfo* info = &i_material->getTexGenBlock()->getTexMtx(i)->getTexMtxInfo();
                if (m_morf_frame != 0) {
                    if (!mSetFlag) {
                        f32 var_f31 = 1.0f / (m_morf_frame + 1);
                        info->mSRT.mTranslationX = field_0xf4 * (1.0f - var_f31) + info->mSRT.mTranslationX * var_f31;
                        info->mSRT.mTranslationY = field_0xf8 * (1.0f - var_f31) + info->mSRT.mTranslationY * var_f31;
                        mSetFlag = 1;
                    } else {
                        info->mSRT.mTranslationX = field_0xf4;
                        info->mSRT.mTranslationY = field_0xf8;
                    }
                } else if (m_eye_move_flg) {
                    info->mSRT.mTranslationX = mNowOffsetX;
                    info->mSRT.mTranslationY = mNowOffsetY;
                }

                field_0xf4 = info->mSRT.mTranslationX;
                field_0xf8 = info->mSRT.mTranslationY;
            }
        }
    }
}

bool daAlink_c::checkStageName(const char* i_stageName) {
    return strcmp(dComIfGp_getStartStageName(), i_stageName) == 0;
}

void daAlink_c::tgHitCallback(fopAc_ac_c* i_atActor, dCcD_GObjInf* i_tgObjInf, dCcD_GObjInf* i_atObjInf) {
    int i;
    if (i_atObjInf->ChkAtType(AT_TYPE_40000)) {
        field_0x369c = *i_atObjInf->GetAtVecP();
        for (i = 0; i < 3; i++) {
            if (i_tgObjInf == &mTgCyls[i]) {
                field_0x2fb8 |= (1 << i);
                field_0x36b4 = i_atActor->current.pos;
                return;
            }
        }
    } else {
        for (i = 0; i < 3; i++) {
            if (i_tgObjInf == &mTgCyls[i]) {
                field_0x2fb8 &= ~(1 << i);
                return;
            }
        }
    }
}

void daAlink_c::coHitCallback(fopAc_ac_c* i_coHitActor, dCcD_GObjInf* i_coHitObj) {
    if (!checkWolf() && !checkNoResetFlg3(FLG3_UNK_20000000)) {
        if (((mProcID != PROC_CUT_HEAD && mProcID != PROC_CUT_DOWN) || i_coHitActor != field_0x280c.getActor())
            && (!checkModeFlg(0x10040) || fopAcM_GetName(i_coHitActor) == PROC_CSTAF))
        {
            field_0x372c += *mCcStts.GetCCMoveP();
        }

        field_0x30fe++;
    }

    if (fopAcM_GetName(i_coHitActor) == PROC_Obj_WaterFall) {
        if (checkWolf()) {
            if (i_coHitObj == &mTgCyls[0]) {
                onEndResetFlg1(ERFLG1_WATERFALL_FRONT_HIT);
            } else if (i_coHitObj == &mTgCyls[2]) {
                onEndResetFlg1(ERFLG1_UNK_1000);
            }
        } else {
            onEndResetFlg1(ERFLG1_WATERFALL_FRONT_HIT);
        }
    }
}

void daAlink_c::setMatrixWorldAxisRot(MtxP i_mtx, s16 i_rotX, s16 i_rotY, s16 i_rotZ,
                                      BOOL param_4, const cXyz* param_5) {
    cXyz tmp;
    mDoMtx_multVecZero(i_mtx, &tmp);
    if (param_5 != NULL) {
        mDoMtx_stack_c::transS(*param_5);
    } else {
        mDoMtx_stack_c::transS(tmp);
    }

    concatMagneBootMtx();
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZXYrotM(i_rotX, i_rotY, i_rotZ);
    mDoMtx_stack_c::YrotM(-shape_angle.y);
    concatMagneBootInvMtx();
    mDoMtx_stack_c::transM(-tmp.x, -tmp.y, -tmp.z);
    mDoMtx_stack_c::concat(i_mtx);
    mDoMtx_copy(mDoMtx_stack_c::get(), i_mtx);

    if (param_4) {
        mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    }
}

int daAlink_c::jointControll(int i_jointNo) {
    csXyz sp18(0, 0, 0);
    int var_r27 = 0;

    J3DTransformInfo jointTrans;
    J3DTransformInfo rootTrans = *field_0x2060->getOldFrameTransInfo(i_jointNo);

    Quaternion sp50;
    Quaternion sp40;
    Quaternion sp30;
    Quaternion sp20 = *field_0x2060->getOldFrameQuaternion(i_jointNo);

    csXyz sp10(0, 1, 2);

    J3DTransformInfo* oldTransInfo;
    if (checkWolf()) {
        if (i_jointNo == 0) {
            if (mProcID == PROC_TOOL_DEMO) {
                oldTransInfo = field_0x2060->getOldFrameTransInfo(0);
                oldTransInfo->mTranslate.x = 0.0f;
                oldTransInfo->mTranslate.z = 0.0f;
            } else if (checkRootTransClearMode()) {
                oldTransInfo = field_0x2060->getOldFrameTransInfo(0);

                if (checkRootTransZClearMode()) {
                    oldTransInfo->mTranslate.z = field_0x3588.z;
                }

                if (checkRootTransXClearMode()) {
                    oldTransInfo->mTranslate.x = field_0x3588.x;
                }

                if (checkRootTransYClearMode()) {
                    oldTransInfo->mTranslate.y = field_0x33b0;
                }
            }

            sp18.set(field_0x3080, 0, field_0x3082);
            sp10.set(2, 1, 0);
            jointTrans = *field_0x2060->getOldFrameTransInfo(0);
            var_r27 |= 2;
        } else if (i_jointNo == 3) {
            sp18.set((field_0x30d6 + field_0x30b2), 0, field_0x30b0);
            sp10.set(2, 0, 1);
        } else if (i_jointNo == 26) {
            sp18.set(0, -field_0x2fec, 0);
            sp10.set(2, 1, 0);
        } else if (i_jointNo >= 37 && i_jointNo <= 39) {
            sp18.set(0, field_0x3094[i_jointNo - 37], 0);
            sp10.set(2, 1, 0);
        } else if (i_jointNo == 2) {
            sp18.set(0, -field_0x2fec, 0);
            sp10.set(2, 1, 0);
        } else if (i_jointNo == 4) {
            sp18.set(field_0x3124.x, -field_0x3124.y, field_0x3124.z);
            sp10.set(2, 1, 0);
        }
    } else if (i_jointNo == 4) {
        sp18.set(field_0x3124.x, -field_0x3124.y, field_0x3124.z);
        sp10.set(2, 1, 0);
    } else if (i_jointNo == 16) {
        sp18.set(field_0x3088, 0, -field_0x308a);
        sp10.set(2, 0, 1);
    } else if (i_jointNo == 1) {
        s16 sp8;
        if (checkReinRide() && !checkHorseLieAnime() && mProcID != PROC_HORSE_RUN && mProcID != PROC_BOAR_RUN) {
            sp8 = (s16)(mBodyAngle.x - shape_angle.x);
        } else {
            sp8 = mBodyAngle.x;
        }

        if (sp8 || field_0x30c8 != 0 || mBodyAngle.z != 0) {
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(1), sp8, field_0x30c8, mBodyAngle.z, 1, NULL);
        }
    } else if (i_jointNo == 2) {
        if (field_0x30c8 != 0) {
            setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(2), 0, field_0x30c8, 0, 1, NULL);
        }
    } else if (i_jointNo == 0) {
        if (mProcID == PROC_TOOL_DEMO || mProcID == PROC_GANON_FINISH) {
            oldTransInfo = field_0x2060->getOldFrameTransInfo(0);
            oldTransInfo->mTranslate.x = 0.0f;
            oldTransInfo->mTranslate.z = 0.0f;
        } else if (checkRootTransClearMode()) {
            oldTransInfo = field_0x2060->getOldFrameTransInfo(0);
            if (checkRootTransZClearMode()) {
                oldTransInfo->mTranslate.z = field_0x3588.z;
            }
            if (checkRootTransXClearMode()) {
                oldTransInfo->mTranslate.x = field_0x3588.x;
            }
            if (checkRootTransYClearMode()) {
                oldTransInfo->mTranslate.y = field_0x33b0;
            }
        }

        sp18.set(field_0x3080, 0, field_0x3082);
        sp10.set(2, 0, 1);

        jointTrans = *field_0x2060->getOldFrameTransInfo(0);
        var_r27 |= 2;
        if (field_0x2f99 == 0x60) {
            jointTrans.mTranslate.x -= field_0x384c->x;
            jointTrans.mTranslate.y -= field_0x384c->y;
            jointTrans.mTranslate.z -= field_0x384c->z;
        }
    } else if (i_jointNo == 27) {
        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(i_jointNo), mFootData1[0].field_0x6, 0, 0, 1, NULL);
    } else if (i_jointNo == 29) {
        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(i_jointNo), mFootData1[1].field_0x6, 0, 0, 1, NULL);
    } else if (i_jointNo == 5) {
        if (checkReinRide() && checkBowAnime()) {
            var_r27 = 1;
            sp50 = *field_0x2060->getOldFrameQuaternion(i_jointNo);
            JMAEulerToQuat(0, 0, 5000, &sp40);
            mDoMtx_QuatConcat(&sp50, &sp40, &sp30);
        }
    } else if ((i_jointNo == 13 || i_jointNo == 12) && (checkCanoeFishingWaitAnime() || checkUnderMove0BckNoArc(ANM_CANOE_FISH_LEFT) || checkUnderMove0BckNoArc(ANM_CANOE_FISH_RIGHT))) {
        if (i_jointNo == 13) {
            sp18 = field_0x3160;
        } else {
            sp18 = mFishingArm1Angle;
        }
    }

    if (sp18.x != 0 || sp18.y != 0 || sp18.z != 0) {
        var_r27 |= 1;
        if (sp18.y != 0) {
            sp50 = *field_0x2060->getOldFrameQuaternion(i_jointNo);

            if (sp10.y == 0) {
                JMAEulerToQuat(sp18.y, 0, 0, &sp40);
            } else if (sp10.y == 1) {
                JMAEulerToQuat(0, sp18.y, 0, &sp40);
            } else {
                JMAEulerToQuat(0, 0, sp18.y, &sp40);
            }

            mDoMtx_QuatConcat(&sp50, &sp40, &sp30);
        } else {
            sp30 = *field_0x2060->getOldFrameQuaternion(i_jointNo);
        }

        if (sp18.x != 0) {
            sp50 = sp30;

            if (sp10.x == 0) {
                JMAEulerToQuat(sp18.x, 0, 0, &sp40);
            } else if (sp10.x == 1) {
                JMAEulerToQuat(0, sp18.x, 0, &sp40);
            } else {
                JMAEulerToQuat(0, 0, sp18.x, &sp40);
            }

            mDoMtx_QuatConcat(&sp50, &sp40, &sp30);
        }

        if (sp18.z != 0) {
            sp50 = sp30;

            if (sp10.z == 0) {
                JMAEulerToQuat(sp18.z, 0, 0, &sp40);
            } else if (sp10.z == 1) {
                JMAEulerToQuat(0, sp18.z, 0, &sp40);
            } else {
                JMAEulerToQuat(0, 0, sp18.z, &sp40);
            }

            mDoMtx_QuatConcat(&sp50, &sp40, &sp30);
        }
    }

    if (var_r27 != 0) {
        MtxP temp_r26 = mpLinkModel->getAnmMtx(i_jointNo);

        J3DTransformInfo* var_r25;
        if ((var_r27 & 2)) {
            var_r25 = &jointTrans;
        } else {
            var_r25 = field_0x2060->getOldFrameTransInfo(i_jointNo);
        }

        Quaternion* spC;
        if ((var_r27 & 1)) {
            spC = &sp30;
        } else {
            spC = field_0x2060->getOldFrameQuaternion(i_jointNo);
        }

        mDoMtx_stack_c::transS(rootTrans.mTranslate.x, rootTrans.mTranslate.y, rootTrans.mTranslate.z);
        mDoMtx_stack_c::quatM(&sp20);
        mDoMtx_stack_c::inverse();
        cMtx_concat(temp_r26, mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

        MTXQuat(temp_r26, spC);

        temp_r26[0][3] = var_r25->mTranslate.x;
        temp_r26[1][3] = var_r25->mTranslate.y;
        temp_r26[2][3] = var_r25->mTranslate.z;

        cMtx_concat(J3DSys::mCurrentMtx, temp_r26, J3DSys::mCurrentMtx);
        cMtx_copy(J3DSys::mCurrentMtx, temp_r26);
    }

    return 1;
}

void daAlink_c::setUpperFront() {
    if ((checkReinRide() && checkUpperReadyThrowAnime()) || (checkBoardRide() && !checkNoUpperAnime())) {
        cMtx_copy(J3DSys::mCurrentMtx, mRootMtx);

        J3DAnmTransform* temp_r3 = getNowAnmPackUpper(UPPER_2);
        if (temp_r3 != NULL) {
            MtxP temp_r28 = mDoMtx_stack_c::get();

            J3DTransformInfo sp18;
            temp_r3->getTransform(0, &sp18);

            Quaternion sp8;
            JMAEulerToQuat(sp18.mRotation.x, sp18.mRotation.y, sp18.mRotation.z, &sp8);
            MTXQuat(temp_r28, &sp8);

            cMtx_concat(mpLinkModel->getBaseTRMtx(), temp_r28, J3DSys::mCurrentMtx);
            J3DSys::mCurrentMtx[0][3] = mRootMtx[0][3];
            J3DSys::mCurrentMtx[1][3] = mRootMtx[1][3];
            J3DSys::mCurrentMtx[2][3] = mRootMtx[2][3];
        } else {
            mDoMtx_stack_c::quatS(field_0x2060->getOldFrameQuaternion(0));
            mDoMtx_stack_c::inverse();
            cMtx_concat(J3DSys::mCurrentMtx, mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        }

        field_0x2f90 = 1;
    }
}

void daAlink_c::changeBlendRate(int i_jointNo) {
    if (field_0x2fb6 == 5) {
        if (i_jointNo == 0) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 5) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
        } else if (i_jointNo == 10) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 15) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
        }
    } else if (field_0x2fb6 == 1) {
        if (i_jointNo == 0) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 5) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
        }
    } else if (field_0x2fb6 == 2) {
        if (i_jointNo == 10) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 15) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
        }
    } else if (field_0x2fb6 == 3) {
        if (i_jointNo == 4) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 5) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
        }
    } else if (field_0x2fb6 == 7) {
        if (i_jointNo == 0) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 4) {
            mNowAnmPackUpper[2].setRatio(1.0f);
        } else if (i_jointNo == 5) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 15) {
            mNowAnmPackUpper[2].setRatio(1.0f);
        }
    } else if (field_0x2fb6 == 6) {
        if (i_jointNo == 4) {
            setDoubleAnimeBlendRatio(1.0f);
        } else if (i_jointNo == 5) {
            setDoubleAnimeBlendRatio(0.0f);
        }
    } else if (field_0x2fb6 == 8) {
        if (i_jointNo == 4) {
            mNowAnmPackUpper[2].setRatio(0.0f);
            mNowAnmPackUpper[1].setRatio(1.0f);
        } else if (i_jointNo == 5) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
            mNowAnmPackUpper[1].setRatio(0.0f);
        }
    } else if (field_0x2fb6 == 4) {
        if (i_jointNo == 0) {
            mNowAnmPackUpper[2].setRatio(0.0f);
        } else if (i_jointNo == 13) {
            mNowAnmPackUpper[2].setRatio(field_0x3444);
        }
    }
}

void daAlink_c::resetRootMtx() {
    cMtx_copy(mRootMtx, J3DSys::mCurrentMtx);
    field_0x2f90 = 0;
}

bool daAlink_c::modelCallBack(int i_jointNo) {
    jointControll(i_jointNo);

    if (i_jointNo == 0) {
        setUpperFront();
    } else if (i_jointNo == 26) {
        if (!getSumouMode()) {
            setFootMatrix();
        }
        setArmMatrix();
    }

    if (field_0x2fb6 != 0 && (i_jointNo == 0 || i_jointNo == 5 || i_jointNo == 10 ||
                              i_jointNo == 4 || i_jointNo == 13 || i_jointNo == 15))
    {
        changeBlendRate(i_jointNo);
    }

    return true;
}

static int daAlink_modelCallBack(J3DJoint* i_joint, int param_1) {
    int jntNo = i_joint->getJntNo();
    daAlink_c* i_this = (daAlink_c*)j3dSys.getModel()->getUserArea();

    if (param_1 == 0) {
        i_this->modelCallBack(jntNo);
    } else if (param_1 == 1 && i_this->checkResetRootMtx(jntNo)) {
        i_this->resetRootMtx();
    }

    return 1;
}

int daAlink_c::headModelCallBack(int i_jointNo) {
    if (mpDemoHDTmpBck == NULL || !mpDemoHDTmpBck->getBckAnm() || (checkEndResetFlg1(ERFLG1_UNK_400000) && i_jointNo < 6)) {
        if (checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW)) {
            mDoMtx_stack_c::copy(J3DSys::mCurrentMtx);
            mDoMtx_stack_c::ZrotM(-6500);
            mpLinkHatModel->setAnmMtx(i_jointNo, mDoMtx_stack_c::get());
            cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        } else if (i_jointNo >= 6) {
            mDoMtx_stack_c::copy(J3DSys::mCurrentMtx);

            if (i_jointNo == 6) {
                mDoMtx_stack_c::XYZrotM(0, (field_0x3040[7] >> 1), (field_0x302c[7] >> 1));
            } else {
                int temp_r28 = i_jointNo - 7;
                if (temp_r28 == 0) {
                    mDoMtx_stack_c::XYZrotM(0, (field_0x3040[7] >> 1), ((field_0x302c[7] >> 1) + field_0x3066[0]));
                } else {
                    mDoMtx_stack_c::XYZrotM(0, field_0x3040[i_jointNo], (field_0x302c[i_jointNo] + field_0x3066[temp_r28]));
                }
            }

            if (mProcID == PROC_METAMORPHOSE) {
                mDoMtx_stack_c::scaleM(field_0x347c, 1.0f, 1.0f);
            }

            mpLinkHatModel->setAnmMtx(i_jointNo, mDoMtx_stack_c::get());
            cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        } else {
            s16 temp_r27 = shape_angle.y;
            shape_angle.y = field_0x3062;
            setMatrixWorldAxisRot(mpLinkHatModel->getAnmMtx(i_jointNo), field_0x302c[i_jointNo], 0, field_0x3040[i_jointNo], 0, NULL);
            shape_angle.y = temp_r27;
        }
    }

    if (checkZoraWearAbility() && i_jointNo == 6) {
        mDoMtx_stack_c::copy(J3DSys::mCurrentMtx);
        mDoMtx_stack_c::scaleM(1.2f, 1.0f, 1.0f);
        mpLinkHatModel->setAnmMtx(i_jointNo, mDoMtx_stack_c::get());
        cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    }

    return 1;
}

static int daAlink_headModelCallBack(J3DJoint* i_joint, int param_1) {
    int joint_no = i_joint->getJntNo();
    daAlink_c* i_this = (daAlink_c*)j3dSys.getModel()->getUserArea();

    if (param_1 == 0) {
        i_this->headModelCallBack(joint_no);
    }

    return 1;
}

int daAlink_c::wolfModelCallBack(int i_jointNo) {
    jointControll(i_jointNo);

    if (i_jointNo == 36) {
        setWolfFootMatrix();
    } else if (i_jointNo == 7 || i_jointNo == 2 || i_jointNo == 3) {
        changeWolfBlendRate(i_jointNo);
    }

    return 1;
}

static int daAlink_wolfModelCallBack(J3DJoint* i_joint, int param_1) {
    int joint_no = i_joint->getJntNo();
    daAlink_c* i_this = (daAlink_c*)j3dSys.getModel()->getUserArea();

    if (param_1 == 0) {
        i_this->wolfModelCallBack(joint_no);
    }

    return 1;
}

void daAlink_c::setHatAngle() {
    if (field_0x2060->getOldFrameFlg()) {
        if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
            for (int i = 0; i < 3; i++) {
                field_0x3054[i] = 0;
                field_0x305a[i] = 0;
            }
        }

        cXyz spB4;
        cXyz spA8;
        cXyz sp9C;
        cXyz sp90;

        int sp38;
        if (checkNoResetFlg2(daPy_FLG2(FLG2_UNK_100000 | FLG2_UNK_80000))) {
            sp38 = 0;
        } else {
            sp38 = 7;
        }

        mDoMtx_multVecZero(mpLinkHatModel->getAnmMtx(sp38), &spA8);

        BOOL sp34;
        if (!checkEndResetFlg1(ERFLG1_UNK_80000)
            && (field_0x35b8.abs2() > SQUARE(10.0f) || (checkEndResetFlg0(ERFLG0_UNK_8) && SQUARE(field_0x3400) + SQUARE(field_0x3404) > SQUARE(10.0f))))
        {
            sp34 = TRUE;
        } else {
            sp34 = FALSE;
        }

        f32 sp40;
        dKyw_get_AllWind_vec(&spA8, &spB4, &sp40);

        if (checkModeFlg(0x40000) || checkEndResetFlg1(ERFLG1_UNK_80000)) {
            sp40 = 0.0f;
        } else if (sp34 != 0) {
            sp40 = 1.0f;
        }

        f32 var_f29;
        f32 var_f28;
        f32 temp_f26 = 25.0f * (sp40 * sp40);

        s16 sp14 = field_0x3060;
        s16 sp12 = field_0x3062;
        cXyz sp84 = eyePos - field_0x34e0;

        if (checkMagneBootsOn()) {
            mDoMtx_multVecSR(mMagneBootInvMtx, &sp84, &sp84);
            mDoMtx_multVecZero(mpLinkHatModel->getAnmMtx(0), &sp90);
            sp90 = field_0x34e0 - sp90;
            mDoMtx_multVecSR(mMagneBootInvMtx, &sp90, &sp90);
        }

        field_0x3062 = sp84.atan2sX_Z();

        s16 spE;
        if (mProcID == PROC_GOAT_CATCH) {
            mDoMtx_multVecSR(mpLinkModel->getAnmMtx(1), &cXyz::BaseY, &sp90);
            spE = sp90.atan2sX_Z();
        } else if (checkUnderMove0BckNoArc(ANM_HORSE_WAIT_B) && mUnderFrameCtrl[0].getFrame() < 77.0f) {
            mDoMtx_multVecSR(mpLinkModel->getAnmMtx(4), &cXyz::BaseX, &sp90);
            spE = sp90.atan2sX_Z();
        } else if (mProcID == PROC_HAND_PAT && mProcVar2.field_0x300c == 0) {
            spE = shape_angle.y + mBodyAngle.y + 0x8000;
        } else {
            spE = (shape_angle.y + mBodyAngle.y);
        }

        if (cLib_distanceAngleS(field_0x3062, spE) > 0x7000) {
            field_0x3060 = cM_atan2s(-sp84.y, -sp84.absXZ());
        } else {
            field_0x3060 = sp84.atan2sY_XZ();
        }

        f32 temp_f1 = sp84.absXZ();
        if (temp_f1 < 0.01f) {
            var_f29 = cM_ssin(spE);
            var_f28 = cM_scos(spE);
        } else {
            f32 temp_f31 = 1.0f / temp_f1;
            var_f29 = sp84.x * temp_f31;
            var_f28 = sp84.z * temp_f31;
        }

        s16 spC;
        s16 spA;
        if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
            sp9C = cXyz::Zero;
            spC = 0;
            spA = 0;
        } else {
            spC = ((s16)(field_0x3060 - sp14) >> 1);
            if (abs(field_0x3060) > 0x3000 && abs(field_0x3060) < 0x5000) {
                spA = 0;
            } else {
                spA = ((s16)(field_0x3062 - sp12) >> 1);
            }

            sp9C = field_0x34c8 - spA8;
        }

        if (checkNoResetFlg0(FLG0_SWIM_UP)) {
            sp9C += spB4 * temp_f26;
        } else if (field_0x3798.abs2XZ(current.pos) < 1.0f) {
            sp9C.x = 0.0f;
            sp9C.z = 0.0f;
        }

        sp9C += field_0x35b8;

        if (checkEndResetFlg0(ERFLG0_UNK_8)) {
            sp9C.x += field_0x3400;
            sp9C.z += field_0x3404;
        }

        multVecMagneBootInvMtx(&sp9C);

        if (!checkNoResetFlg0(FLG0_SWIM_UP)) {
            sp9C.y -= 2.0f;
        } else if (checkMagneBootsOn() && getMoveBGActorName(mMagneLineChk, FALSE) == PROC_Obj_MagneArm) {
            sp9C.y += 5.0f;
        } else {
            sp9C.y -= 5.0f;
        }

        if (fabsf(sp9C.x) < 0.01f) {
            sp9C.x = (f32) 0.0f;
        }

        if (fabsf(sp9C.z) < 0.01f) {
            sp9C.z = (f32) 0.0f;
        }

        if (checkMagneBootsOn()) {
            mDoMtx_stack_c::copy(mMagneBootInvMtx);
            mDoMtx_stack_c::concat(mpLinkModel->getAnmMtx(2));
        } else {
            mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(2));
        }

        cXyz sp78;
        cXyz sp6C;
        mDoMtx_stack_c::multVecSR(&cXyz::BaseY, &sp78);
        mDoMtx_stack_c::multVec(&cXyz::BaseX, &sp6C);

        s16 sp18;
        s16 sp16;

        int sp30;
        if (sp6C.y < mDoMtx_stack_c::get()[1][3] && !checkUnderMove0BckNoArc(ANM_DIE)) {
            sp30 = cM_atan2s(-sp78.y, -sp78.absXZ());
        } else {
            sp30 = sp78.atan2sY_XZ();
        }
        sp30 -= 0x3800;

        s16* temp_r29 = &field_0x302c[7];
        s16* temp_r28 = &field_0x3040[7];
        s16* sp2C = &field_0x3054[0];
        s16* sp28 = &field_0x305a[0];
        *temp_r29 -= spC;
        *temp_r28 -= spA;
        sp18 = *temp_r29;
        sp16 = *temp_r28;

        f32 temp_f27 = (sp9C.z * var_f28) + (sp9C.x * var_f29);
        s16 sp10 = cM_atan2s(sp9C.y, -temp_f27);
        int sp24 = cLib_minMaxLimit<int>(sp10 - field_0x3060, -0x3800, 0x3800);

        sp10 = (sp24 + field_0x3060);
        if (sp10 < sp30) {
            sp10 = sp30;
        }

        sp24 = (sp10 - field_0x3060);
        if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
            *temp_r29 = sp24;
        } else {
            cLib_addCalcAngleS2(temp_r29, sp24, 5, 0x400);
        }

        *temp_r29 = cLib_minMaxLimit<s16>((*temp_r29 + *sp2C), -0x3800, 0x3800);
        sp10 = cLib_minMaxLimit<s16>(cM_atan2s(-((sp9C.x * var_f28) - (sp9C.z * var_f29)), JMAFastSqrt(SQUARE(temp_f27) + SQUARE(sp9C.y))), -0x2800, 0x2800);

        if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
            *temp_r28 = sp10;
        } else {
            cLib_addCalcAngleS2(temp_r28, sp10, 5, 0x400);
        }

        *temp_r28 = cLib_minMaxLimit<s16>((*temp_r28 + *sp28), -0x2800, 0x2800);

        *sp2C = 0.2f * (*temp_r29 - sp18);
        *sp28 = 0.2f * (*temp_r28 - sp16);

        sp10 = (*temp_r29 + field_0x3060);
        temp_r29++;
        temp_r28++;
        sp2C++;
        sp28++;

        for (int i = 1; i < 3; i++, temp_r29++, temp_r28++, sp2C++, sp28++) {
            temp_r29[0] -= ((s16)(temp_r29[-1] - sp18) >> 1);
            temp_r28[0] -= ((s16)(temp_r28[-1] - sp16) >> 1);
            sp18 = temp_r29[0];
            sp16 = temp_r28[0];

            if (checkEndResetFlg0(ERFLG0_UNK_800000)) {
                temp_r29[0] = 0;
                temp_r28[0] = 0;
            } else {
                cLib_addCalcAngleS2(temp_r29, 0, 5, 0x400);
                cLib_addCalcAngleS2(temp_r28, 0, 5, 0x400);
            }

            temp_r29[0] = cLib_minMaxLimit<s16>((temp_r29[0] + *sp2C), -0x1000, 0x1000);

            sp10 += temp_r29[0];
            if (sp10 < sp30) {
                temp_r29[0] += (sp30 - sp10);
                sp10 = sp30;
            }

            temp_r28[0] = cLib_minMaxLimit<s16>((temp_r28[0] + *sp28), -0x2000, 0x2000);

            *sp2C = 0.2f * (temp_r29[0] - sp18);
            *sp28 = 0.2f * (temp_r28[0] - sp16);
        }

        f32 var_f30 = (temp_f26 + (0.65f * field_0x34c8.abs(spA8))) / 30.0f;
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }

        f32 var_f25 = var_f30;
        if (sp34 != 0 && !checkNoResetFlg2(daPy_FLG2(FLG2_UNK_100000 | FLG2_UNK_80000))) {
            var_f30 = 3.5f;
            var_f25 = 1.0f;
        }

        s16 sp8 = 1500.0f + (4060.0f * var_f30);
        field_0x3064 += sp8;

        for (int i = 0; i < 3; i++) {
            field_0x3066[i] = var_f25 * cM_deg2s(((i + 1) * 4)) * cM_scos((field_0x3064 - ((i + 3) * sp8)));
        }

        field_0x34c8 = spA8;
        setHairAngle(&sp9C, var_f29, var_f28);
    }
}

void daAlink_c::calcHairAngle(s16* o_angle) {
    cLib_addCalcAngleS(o_angle, 0, 5, 400, 50);
}

void daAlink_c::setHairAngle(cXyz* param_0, f32 param_1, f32 param_2) {
    f32 var_f31 = param_0->abs();
    f32 temp_f27 = param_0->absXZ();

    if (var_f31 < 1.0f || temp_f27 < 1.0f) {
        calcHairAngle(&field_0x302c[1]);
        calcHairAngle(&field_0x3040[1]);
        calcHairAngle(&field_0x302c[2]);
        calcHairAngle(&field_0x3040[2]);
        calcHairAngle(&field_0x302c[3]);
        calcHairAngle(&field_0x3040[3]);
        calcHairAngle(&field_0x302c[4]);
        calcHairAngle(&field_0x3040[4]);
        calcHairAngle(&field_0x302c[5]);
        calcHairAngle(&field_0x3040[5]);
        field_0x3070 = -0x8000;
        field_0x3072 = -0x8000;
        field_0x3074 = -0x8000;
        field_0x3076 = -0x8000;
        return;
    }

    var_f31 *= 0.033333335f;
    if (var_f31 > 1.0f) {
        var_f31 = 1.0f;
    }

    var_f31 = 0.15f + (0.85f * var_f31);

    field_0x3070 += (s16)(1000.0f + cM_rndF(500.0f) + (var_f31 * (3000.0f + cM_rndF(1000.0f))));
    field_0x3072 += (s16)(1000.0f + cM_rndF(500.0f) + (var_f31 * (3000.0f + cM_rndF(1000.0f))));
    field_0x3074 += (s16)(1000.0f + cM_rndF(500.0f) + (var_f31 * (5000.0f + cM_rndF(1500.0f))));
    field_0x3076 += (s16)(1000.0f + cM_rndF(500.0f) + (var_f31 * (5000.0f + cM_rndF(1500.0f))));

    temp_f27 = 1.0f / temp_f27;
    param_0->x *= temp_f27;
    param_0->z *= temp_f27;

    f32 temp_f30 = (param_0->x * param_2) - (param_0->z * param_1);
    f32 temp_f29 = (param_0->x * param_1) + (param_0->z * param_2);
    f32 temp_f28 = var_f31 * (0.5f * (1.0f + cM_scos(field_0x3070)));
    f32 sp10 = var_f31 * (0.5f * (1.0f + cM_scos(field_0x3072)));
    f32 spC = var_f31 * (0.5f * (1.0f + cM_scos(field_0x3074)));
    f32 sp8 = var_f31 * (0.5f * (1.0f + cM_scos(field_0x3076)));

    if (temp_f30 > 0.0f) {
        field_0x3040[1] = 6000.0f * temp_f28 * temp_f30;
        field_0x3040[2] = 8000.0f * temp_f28 * temp_f30;
        field_0x3040[3] = 2000.0f * sp10 * temp_f30;
        field_0x3040[4] = 7000.0f * spC * temp_f30;
        field_0x3040[5] = 2500.0f * sp8 * temp_f30;
    } else {
        field_0x3040[1] = 10000.0f * temp_f28 * temp_f30;
        field_0x3040[2] = 2000.0f * temp_f28 * temp_f30;
        field_0x3040[3] = 8000.0f * sp10 * temp_f30;
        field_0x3040[4] = 2500.0f * spC * temp_f30;
        field_0x3040[5] = 7000.0f * sp8 * temp_f30;
    }

    if (temp_f29 > 0.0f) {
        field_0x302c[1] = -9000.0f * temp_f28 * temp_f29;
        field_0x302c[2] = -15000.0f * temp_f28 * temp_f29;
        field_0x302c[3] = -15000.0f * sp10 * temp_f29;
    } else {
        field_0x302c[1] = -1000.0f * temp_f28 * temp_f29;
        field_0x302c[2] = -5000.0f * temp_f28 * temp_f29;
        field_0x302c[3] = -5000.0f * sp10 * temp_f29;
    }

    field_0x302c[4] = -7000.0f * spC * temp_f29;
    field_0x302c[5] = -7000.0f * sp8 * temp_f29;
}

void daAlink_c::setLookPosFromOut(cXyz* i_pos) {
    if (checkEndResetFlg0(ERFLG0_UNK_4)) {
        if (current.pos.abs2(mLookPosFromOut) < current.pos.abs2(*i_pos)) {
            return;
        }
    }

    onEndResetFlg0(ERFLG0_UNK_4);
    mLookPosFromOut = *i_pos;
}

bool daAlink_c::checkAttentionPosAngle(cXyz* param_0) {
    if (checkNoResetFlg1(FLG1_UNK_4)) {
        return true;
    }

    int search_range_angle;
    if (checkModeFlg(MODE_RIDING) || checkEndResetFlg0(ERFLG0_NECK_SEARCH_WIDE)) {
        search_range_angle = mpHIO->mHorse.m.mSearchRangeAngle;
    } else {
        search_range_angle = 0x6000;
    }

    return cLib_distanceAngleS(cLib_targetAngleY(&field_0x34e0, param_0), mPrevAngleY) <= search_range_angle;
}

bool daAlink_c::checkActorPosAngle(fopAc_ac_c* i_actor, cXyz** o_ppos) {
    if (i_actor != NULL && checkAttentionPosAngle(&i_actor->eyePos)) {
        s16 actor_name = fopAcM_GetName(i_actor);

        if (checkEnemyGroup(i_actor)) {
            field_0x3504 = i_actor->eyePos;
            field_0x311c = 0x50;
        }

        if (actor_name == PROC_Obj_Wchain || (actor_name == PROC_B_MGN && !((daB_MGN_c*)i_actor)->isDown())) {
            onNoResetFlg1(FLG1_MIDNA_HAIR_ATN_POS);
            mMidnaHairAtnPos = i_actor->eyePos;
        }

        *o_ppos = &i_actor->eyePos;
        return 1;
    }

    return 0;
}

cXyz* daAlink_c::getNeckAimPos(cXyz* param_0, int* param_1, int param_2) {
    fopAc_ac_c* look_actor = NULL;
    s16 spA = field_0x311c;
    field_0x311c = 0;

    if (mProcID == PROC_GOAT_CATCH
        || mProcID == PROC_GET_ITEM
        || mProcID == PROC_WOLF_GANON_CATCH
        || mProcID == PROC_NOT_USE_ITEM
        || mProcID == PROC_DUNGEON_WARP_READY
        || mProcID == PROC_GOAT_STROKE)
    {
        look_actor = field_0x280c.getActor();
    } else if (dComIfGp_checkPlayerStatus0(0, 0x10)) {
        if (mProcID != PROC_NOD && mProcID != PROC_EYE_AWAY && mProcID != PROC_GLARE) {
            look_actor = fopAcM_getTalkEventPartner(this);
            if (look_actor != NULL) {
                daMidna_c* midna = (daMidna_c*)getMidnaActor();
                s16 actor_name = fopAcM_GetName(look_actor);

                if (actor_name == PROC_MIDNA
                    || actor_name == PROC_Tag_Wljump
                    || (actor_name == PROC_Tag_Mhint && ((daTagMhint_c*)look_actor)->checkNoAttention())
                    || (actor_name == PROC_Tag_Mstop && ((daTagMstop_c*)look_actor)->checkNoAttention())
                    || (actor_name == PROC_Tag_Mwait && ((daTagMwait_c*)look_actor)->checkEndMessage()))
                {
                    *param_1 = 1;
                    return &midna->eyePos;
                }

                if (actor_name == PROC_Tag_Mhint || actor_name == PROC_Tag_Mstop) {
                    midna->setForceNeckAimPos(look_actor->eyePos);
                }
            }
        }
    } else if (mProcID == PROC_BOTTLE_OPEN && mEquipItem == fpcNm_ITEM_FAIRY) {
        look_actor = field_0x285c.getActor();
    } else if (mProcID == PROC_FISHING_CAST || mProcID == PROC_CANOE_FISHING_WAIT || mProcID == PROC_CANOE_FISHING_REEL || mProcID == PROC_CANOE_FISHING_GET) {
        look_actor = mItemAcKeep.getActor();
    } else {
        look_actor = mTargetedActor;
        if (checkEnemyGroup(mTargetedActor)) {
            fopAc_ac_c* lock_actor;
            if (mAttention->GetLockonList(1) != NULL) {
                lock_actor = mAttention->GetLockonList(1)->getActor();
            } else {
                lock_actor = NULL;
            }

            if (field_0x28fc != fpcM_ERROR_PROCESS_ID_e) {
                fopAc_ac_c* sp10 = fopAcM_SearchByID(field_0x28fc);
                field_0x30f8--;

                if (field_0x30f8 != 0 && sp10 != NULL && sp10 == lock_actor) {
                    look_actor = sp10;
                } else {
                    field_0x30f8 = 0;
                    field_0x28fc = fpcM_ERROR_PROCESS_ID_e;
                }
            } else if (checkEnemyGroup(lock_actor)) {
                if (field_0x30f8 == -30) {
                    if (cM_rnd() < 0.02f) {
                        field_0x30f8 = 30.0f + cM_rndF(30.0f);
                        look_actor = lock_actor;
                        field_0x28fc = fopAcM_GetID(lock_actor);
                    }
                } else {
                    field_0x30f8 -= 1;
                }
            }
        } else {
            field_0x28fc = fpcM_ERROR_PROCESS_ID_e;
            field_0x30f8 = 0;
        }
    }

    fopAc_ac_c* spC = mAttention->LockonTarget(0);
    if (look_actor == NULL) {
        look_actor = dCam_getBody()->GetForceLockOnActor();
    }

    cXyz* sp1C;
    if (checkActorPosAngle(look_actor, &sp1C)) {
        return sp1C;
    }

    if (!checkModeFlg(0x08000100) && param_2 != 0) {
        if (spA > 0) {
            field_0x311c = spA - 1;
        }
        return NULL;
    }

    if (!checkEventRun()) {
        if (dComIfGp_checkPlayerStatus1(0, 0x02010000)) {
            if (mSight.getDrawFlg()) {
                return mSight.getPosP();
            } else {
                return NULL;
            }
        }

        if (dComIfGp_att_getLookTarget() != NULL) {
            if (fopAcM_GetName(dComIfGp_att_getLookTarget()) == PROC_E_NZ) {
                *param_1 = 0;
            }
            return &dComIfGp_att_getLookTarget()->eyePos;
        }

        if ((checkActorPosAngle(field_0x27f0, &sp1C) && (field_0x27f0 == NULL || fopAcM_GetName(field_0x27f0) != PROC_Tag_WaraHowl))
            || checkActorPosAngle(spC, &sp1C)
            || checkActorPosAngle(field_0x27f8, &sp1C))
        {
            return sp1C;
        }

        if (checkActorPosAngle(mThrowBoomerangAcKeep.getActor(), &sp1C)) {
            return sp1C;
        }

        if (checkActorPosAngle(mCopyRodAcKeep.getActor(), &sp1C)) {
            return sp1C;
        }

        if (checkEndResetFlg0(ERFLG0_UNK_4) && checkAttentionPosAngle(&mLookPosFromOut)) {
            return &mLookPosFromOut;
        }

        if (checkActorPosAngle(field_0x285c.getActor(), &sp1C)) {
            return sp1C;
        }

        if (mProcID == PROC_SPINNER_WAIT && mNormalSpeed > 0.0f) {
            param_0->set(current.pos.x + (200.0f * cM_ssin(current.angle.y)), 100.0f + current.pos.y, current.pos.z + (200.0f * cM_scos(current.angle.y)));
            return param_0;
        }

        if (checkModeFlg(1) && checkCopyRodControllAnime() && checkActorPosAngle(getCopyRodControllActor(), &sp1C)) {
            return sp1C;
        }

        if (checkWindSpeedOnAngleAnime(1)) {
            if (checkWolf()) {
                s16 sp8 = ((field_0x30cc + 0x8000) - shape_angle.y);
                if (sp8 >= 0) {
                    sp8 = (shape_angle.y - 0x4000);
                } else {
                    sp8 = (shape_angle.y + 0x4000);
                }

                param_0->set(current.pos.x + (100.0f * cM_ssin(sp8)), current.pos.y, current.pos.z + (100.0f * cM_scos(sp8)));
            } else {
                param_0->set(current.pos.x - (100.0f * field_0x35b8.x), 150.0f + current.pos.y, current.pos.z - (100.0f * field_0x35b8.z));
            }

            if (checkAttentionPosAngle(param_0)) {
                if (checkWolf()) {
                    onEndResetFlg0(ERFLG0_UNK_4000);
                }
                *param_1 = 0;
                return param_0;
            }
        } else if (field_0x35c4.abs2XZ() > 1.0f && checkModeFlg(1)) {
            f32 var_f31;
            if (checkWolf() != NULL) {
                var_f31 = 80.0f;
            } else {
                var_f31 = 150.0f;
            }

            param_0->set(current.pos.x + (100.0f * field_0x35c4.x), current.pos.y + var_f31, current.pos.z + (100.0f * field_0x35c4.z));
            return param_0;
        } else if (field_0x3594.absXZ() > 1.0f && (checkUnderMove0BckNoArc(ANM_SWIM_WAIT) || checkUnderMove0BckNoArcWolf(WANM_SWIM_WAIT))) {
            param_0->set(current.pos.x + (100.0f * field_0x3594.x), 30.0f + current.pos.y, current.pos.z + (100.0f * field_0x3594.z));
            return param_0;
        } else if (checkWolf()) {
            if (mProcID == PROC_WOLF_WAIT_SLIP) {
                if (checkAttentionPosAngle(&field_0x37c8)) {
                    *param_1 = 0;
                    return &field_0x37c8;
                }
            } else if (mTargetedActor == NULL && ((checkAttentionState() && mProcID == PROC_WOLF_ATN_AC_MOVE) || mProcID == PROC_WOLF_ROLL_ATTACK_MOVE)) {
                param_0->set(current.pos.x + (1000.0f * cM_ssin(field_0x2fe4)), eyePos.y, current.pos.z + (1000.0f * cM_scos(field_0x2fe4)));

                if (checkAttentionPosAngle(param_0)) {
                    *param_1 = 0;
                    return param_0;
                }
            }
        }
    } else {
        u32 demoMode = mDemo.getDemoMode();
        spA = 0;

        if (checkEndResetFlg2(ERFLG2_PORTAL_WARP_MIDNA_ATN_KEEP) && daMidna_c::checkMidnaRealBody()) {
            if (checkWolf()) {
                *param_1 = 1;
                return &getMidnaActor()->eyePos;
            }
        }

        if (mProcID == PROC_LOOK_WAIT
            || demoMode == daPy_demo_c::DEMO_UNK_23_e
            || checkDemoMoveMode(demoMode)
            || demoMode == daPy_demo_c::DEMO_UNK_1_e
            || demoMode == daPy_demo_c::DEMO_UNK_4_e
            || demoMode == daPy_demo_c::DEMO_UNK_44_e
            || demoMode == daPy_demo_c::DEMO_LOOK_UP_e
            || demoMode == daPy_demo_c::DEMO_LOOK_UP_TO_GET_ITEM_e
            || demoMode == daPy_demo_c::DEMO_HAND_PAT_e
            || demoMode == daPy_demo_c::DEMO_UNEQUIP_e)
        {
            spC = getDemoLookActor();
            if (spC != NULL && fopAcM_GetName(spC) == PROC_MIDNA) {
                return &spC->eyePos;
            }

            if (checkActorPosAngle(spC, &sp1C)) {
                return sp1C;
            }
        } else if (checkActorPosAngle(field_0x285c.getActor(), &sp1C)) {
            return sp1C;
        }
    }

    if (param_2 == 0) {
        return NULL;
    }

    if (spA > 0) {
        field_0x311c = spA - 1;
        if (checkAttentionPosAngle(&field_0x3504)) {
            return &field_0x3504;
        }
    }

    return NULL;
}

s16 daAlink_c::getNeckAimAngle(cXyz* param_0, s16* param_1, s16* param_2, s16* param_3,
                               s16* param_4) {
    s16 sp1A;
    s16 sp18;
    s16 sp16 = mPrevAngleY + mBodyAngle.y;
    if ((mProcID == PROC_GOAT_CATCH && mProcVar1.field_0x300a == 0) || (mProcID == PROC_HAND_PAT && mProcVar2.field_0x300c == 0)) {
        ADD_ANGLE_2(sp16, 0x8000);
    }

    cXyz sp28 = eyePos - field_0x34e0;
    multVecMagneBootInvMtx(&sp28);

    s16 sp14 = sp28.atan2sY_XZ();
    s16 sp12 = sp28.atan2sX_Z();
    sp1A = sp14 - field_0x3124.x;
    sp18 = (sp12 - sp16) - field_0x3124.y;

    if (checkModeFlg(0x08000100) && param_0 != NULL && !checkWolfHeadDamageAnime() && !checkDashDamageAnime()) {
        cXyz sp1C = *param_0 - field_0x34e0;
        multVecMagneBootInvMtx(&sp1C);

        s16 temp_r24 = sp1C.atan2sY_XZ();
        s16 var_r28 = sp1C.atan2sX_Z() - sp16;
        if (((var_r28 > 0x7000) && (field_0x3124.y < 0)) || ((var_r28 < -0x7000) && (field_0x3124.y > 0))) {
            var_r28 *= -1;
        }

        s16 sp10 = temp_r24;
        s16 spE = var_r28;
        if (sp1C.absXZ() < 30.0f) {
            var_r28 = field_0x3124.y;
        }

        if (checkWolf()) {
            if (mProcID == PROC_WOLF_TIRED_WAIT) {
                temp_r24 = cLib_minMaxLimit<s16>((s16)temp_r24, mpHIO->mWolf.m.mMaxTiredNeckTurnUp, mpHIO->mWolf.m.mMaxTiredNeckTurnDown);
                var_r28 = cLib_minMaxLimit<s16>((s16)var_r28, -mpHIO->mWolf.m.mMaxTiredNeckTurnH, mpHIO->mWolf.m.mMaxTiredNeckTurnH);
            } else {
                temp_r24 = cLib_minMaxLimit<s16>((s16)temp_r24, mpHIO->mWolf.m.mMaxNeckTurnUp, mpHIO->mWolf.m.mMaxNeckTurnDown);
                var_r28 = cLib_minMaxLimit<s16>((s16)var_r28, -mpHIO->mWolf.m.mMaxNeckTurnH, mpHIO->mWolf.m.mMaxNeckTurnH);
            }
        } else {
            temp_r24 = cLib_minMaxLimit<s16>((s16)temp_r24, mpHIO->mBasic.m.mHeadMaxTurnUp, mpHIO->mBasic.m.mHeadMaxTurnDown);

            s16 spC;
            s16 spA;
            if (dComIfGp_checkPlayerStatus1(0, 0x02000000)) {
                if (field_0x3020 == 1) {
                    spC = -0x1000;
                    spA = mpHIO->mBasic.m.mHeadMaxTurnHorizontal;
                } else {
                    spC = -mpHIO->mBasic.m.mHeadMaxTurnHorizontal;
                    spA = 0x1000;
                }
            } else {
                if (checkGrabAnimeCarry()) {
                    spC = 0;
                } else {
                    spC = -mpHIO->mBasic.m.mHeadMaxTurnHorizontal;
                }
                spA = mpHIO->mBasic.m.mHeadMaxTurnHorizontal;
            }

            var_r28 = cLib_minMaxLimit<s16>((s16)var_r28, spC, spA);
        }

        s16 temp_r23 = temp_r24 - sp1A;
        s16 sp8 = (var_r28 - sp18);

        if (checkModeFlg(0x100)) {
            if (checkWolf()) {
                *param_1 = 0.9f * temp_r23;
                *param_2 = 0.9f * sp8;
                *param_3 = temp_r23 - *param_1;
                *param_4 = sp8 - *param_2;
            } else if (field_0x2fc8 != 0) {
                *param_1 = 0.25f * temp_r23;
                *param_2 = 0.5f * sp8;
                *param_3 = temp_r23 - *param_1;
                *param_4 = *param_2;
            } else {
                *param_3 = (temp_r23 >> 1);
                *param_4 = (sp8 >> 1);
                *param_1 = *param_3;
                *param_2 = *param_4;
            }
        } else {
            *param_3 = temp_r23;
            *param_4 = sp8;
        }

        *param_3 += (s16)(sp10 - temp_r24);
        *param_4 += (s16)(spE - var_r28);

        if (checkEndResetFlg0(ERFLG0_UNK_4000)) {
            *param_3 = (sp10 + 0x8000) - sp14;
            *param_4 = (field_0x30cc + 0x8000) - sp12;
        }
    } else if (checkMagneBootsOn()) {
        *param_1 = 0;
        *param_2 = 0;
        field_0x30d4 = 0;
    } else {
        *param_1 = field_0x30a0;
        *param_2 = field_0x30a2;

        if (checkWolf() && *param_2 == 0) {
            *param_2 = field_0x2fec;
        }


        field_0x30d4 = *param_2;
    }

    return sp18;
}

void daAlink_c::setEyeMove(cXyz* param_0, s16 param_1, s16 param_2) {
    u32 temp_r28 = field_0x2fa7;
    f32 sp18 = field_0x3418;
    f32 sp14 = field_0x341c;

    field_0x2fa7 = 75.0f + cM_rndF(30.0f);
    field_0x3418 = 0.0f;
    field_0x341c = 0.0f;
    field_0x33f8 = 0.0f;

    f32 var_f31;
    f32 var_f30;
    if (param_0 != NULL) {
        var_f30 = 0.00012207031f * param_1;
        var_f31 = 0.00012207031f * param_2;
    } else if (0.0f != field_0x33f0 || 0.0f != field_0x33f4) {
        var_f30 = field_0x33f4;
        var_f31 = field_0x33f0;
        field_0x33f8 = field_0x33f0;
    } else if ((mProcID == PROC_MOVE || mProcID == PROC_WOLF_MOVE) && !checkNoResetFlg1(FLG1_UNK_1000000) && field_0x2fee != 0) {
        var_f30 = 0.0f;
        var_f31 = 0.00024414062f * -field_0x2fee;
        field_0x33f8 = var_f31;
    } else if (checkSwimNeckUpDown()) {
        if (field_0x3124.x > 0) {
            var_f30 = 0.5f;
        } else if (field_0x3124.x < 0) {
            var_f30 = -0.5f;
        } else {
            var_f30 = 0.0f;
        }
        var_f31 = 0.0f;
    } else if (!checkEventRun() && checkModeFlg(1) && checkNoResetFlg1(FLG1_UNK_2000) && (checkNoUpperAnime() || checkGrabAnime()) && (mProcID == PROC_WAIT || mProcID == PROC_GRAB_WAIT || mProcID == PROC_CROUCH || mProcID == PROC_HORSE_WAIT || mProcID == PROC_WOLF_WAIT)) {
        if (temp_r28 != 0) {
            field_0x2fa7 = temp_r28 - 1;
            field_0x3418 = sp18;
            field_0x341c = sp14;
        } else if (0.0f != sp18 || 0.0f != sp14) {
            if (checkGrabAnimeCarry() || cM_rnd() < 0.5f) {
                field_0x3418 = 0.0f;
                field_0x341c = 0.0f;
            } else {
                s16 temp_r29_2 = cM_atan2s(field_0x3418, field_0x341c);
                temp_r29_2 += (s16)(((int)cM_rndF(3.0f) << 13) + 0x6000);

                field_0x3418 = cM_ssin(temp_r29_2);
                field_0x341c = cM_scos(temp_r29_2);
            }
        } else if (checkGrabAnimeCarry() && mGrabItemAcKeep.getActor() != NULL) {
            field_0x3418 = -1.0f;
        } else {
            s16 temp_r0 = (int)cM_rndF(8.0f) << 0xD;
            field_0x3418 = cM_ssin(temp_r0);
            field_0x341c = cM_scos(temp_r0);
        }
        var_f31 = field_0x3418;
        var_f30 = field_0x341c;
    } else {
        field_0x2180[0]->setNowOffsetX(0.0f);
        field_0x2180[1]->setNowOffsetX(0.0f);
        field_0x2180[0]->setNowOffsetY(0.0f);
        field_0x2180[1]->setNowOffsetY(0.0f);

        if (daAlink_matAnm_c::getEyeMoveFlg()) {
            daAlink_matAnm_c::offEyeMoveFlg();
            daAlink_matAnm_c::setMorfFrame(3);
        }
        return;
    }

    if (daAlink_matAnm_c::getMorfFrame() == 0) {
        var_f31 = cLib_minMaxLimit<f32>(var_f31, -1.0f, 1.0f);
        var_f30 = cLib_minMaxLimit<f32>(var_f30, -1.0f, 1.0f);

        f32 var_f29;
        f32 var_f28;
        if (var_f31 > 0.0f) {
            var_f29 = 0.25f * var_f31;
            var_f28 = 0.15f * var_f31;
        } else {
            var_f29 = 0.15f * var_f31;
            var_f28 = 0.25f * var_f31;
        }

        f32 sp10;
        if (var_f30 > 0.0f) {
            sp10 = 0.2f * var_f30;
        } else {
            sp10 = 0.1f * var_f30;
        }

        if (checkWolf()) {
            var_f29 *= -1.0f;
            var_f28 *= -1.0f;
        }

        daAlink_matAnm_c::onEyeMoveFlg();
        cLib_addCalc(field_0x2180[0]->getNowOffsetXP(), var_f29, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(field_0x2180[1]->getNowOffsetXP(), var_f28, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(field_0x2180[0]->getNowOffsetYP(), sp10, 0.5f, 0.08f, 0.02f);
        field_0x2180[1]->setNowOffsetY(*field_0x2180[0]->getNowOffsetYP());
    }
}

void daAlink_c::setNeckAngle() {
    cXyz sp18;
    s16 sp10 = 0;
    s16 spE = 0;
    s16 spC = 0;
    s16 spA = 0;
    int sp14 = 2;

    offNoResetFlg1(FLG1_MIDNA_HAIR_ATN_POS);
    cXyz* var_r30 = getNeckAimPos(&sp18, &sp14, 1);

    if (var_r30 != NULL && checkModeFlg(0x08000100)) {
        onNoResetFlg1(FLG1_UNK_4);

        if (sp14 != 0) {
            if (sp14 == 1) {
                mMidnaAtnPos = eyePos;
                onNoResetFlg1(FLG1_MIDNA_ATN_POS);
            } else {
                mMidnaAtnPos = *var_r30;
                onNoResetFlg1(FLG1_MIDNA_ATN_POS);
            }
        }
    } else {
        offNoResetFlg1(daPy_FLG1(FLG1_MIDNA_ATN_POS | FLG1_UNK_4));
    }

    if (checkUnderMove0BckNoArcWolf(WANM_SMELL) || checkUnderMove0BckNoArc(ANM_HORSE_TURN_B) || checkUnderMove0BckNoArc(ANM_WAIT_INSECT) || checkUnderMove0BckNoArc(ANM_HORSE_TURN_LEFT) || checkUnderMove0BckNoArc(ANM_HORSE_TURN_RIGHT)) {
        var_r30 = NULL;
    }

    s16 sp8 = getNeckAimAngle(var_r30, &sp10, &spE, &spC, &spA);
    daPy_addCalcShort(&field_0x3124.x, sp10, 3, 0x1000, 0x100);
    daPy_addCalcShort(&field_0x3124.y, spE, 3, 0x1000, 0x100);
    setEyeMove(var_r30, spC, spA);
}

bool daAlink_c::getStickAngleFromPlayerShape(s16* o_angle) const {
    if (checkInputOnR()) {
        *o_angle = mMoveAngle - shape_angle.y;
        return true;
    }

    *o_angle = 0;
    return false;
}

bool daAlink_c::commonLineCheck(cXyz* i_startPos, cXyz* i_endPos) {
    mLinkLinChk.Set(i_startPos, i_endPos, this);
    return dComIfG_Bgsp().LineCross(&mLinkLinChk);
}

/**
 * Gets the Actor Name of a given polygon's associated actor
 *
 * @param i_polyinf The polygon info to check
 * @param i_forceCheck Forces the check to happen regardless of if the poly is safe or not
 * @return The actor name of the actor associated with the polygon info. defaults to PROC_ALINK
 */
s16 daAlink_c::getMoveBGActorName(cBgS_PolyInfo& i_polyinf, BOOL i_forceCheck) {
    if ((i_forceCheck || dComIfG_Bgsp().ChkPolySafe(i_polyinf)) &&
        dComIfG_Bgsp().ChkMoveBG_NoDABg(i_polyinf) && dComIfG_Bgsp().GetActorPointer(i_polyinf))
    {
        return fopAcM_GetName(dComIfG_Bgsp().GetActorPointer(i_polyinf));
    }

    return PROC_ALINK;
}

fopAc_ac_c* daAlink_c::checkGoronRide() {
    if (mLinkAcch.ChkGroundHit() && getMoveBGActorName(mLinkAcch.m_gnd, FALSE) == PROC_OBJ_GRA) {
        return dComIfG_Bgsp().GetActorPointer(mLinkAcch.m_gnd);
    }

    return NULL;
}

void daAlink_c::setMoveSlantAngle() {
    f32 temp_f31 = fabsf(speedF / mMaxSpeed);

    if (checkModeFlg(0x02020400) || mProcID == PROC_WOLF_CARGO_CARRY) {
        if (!checkBoardRide()) {
            field_0x2fee = 0;
            if (!checkCanoeRide()) {
                mBodyAngle.z = 0;
            }
        }

        if (mProcID == PROC_SLIDE && field_0x3198 != 0) {
            mBodyAngle.z = mProcVar4.field_0x3010 * 0.75f;
            shape_angle.z += (0.25f * mProcVar4.field_0x3010);
        }

        field_0x2fec = 0;
        return;
    }

    s16 var_r29;
    s16 var_r28;

    if ((mProcID == PROC_MOVE && temp_f31 > mpHIO->mMove.m.mRunChangeRate) || (mProcID == PROC_WOLF_MOVE && temp_f31 > mpHIO->mWolf.mWlMove.m.mIdleToWalkRate)) {
        var_r28 = cLib_minMaxLimit<s16>((s16)(mPrevAngleY - shape_angle.y) * 2, -0xC00, 0xC00);
        if (abs(var_r28) > 0x800) {
            int var_r27;
            if (var_r28 > 0) {
                var_r27 = var_r28 - 0x800;
            } else {
                var_r27 = var_r28 + 0x800;
            }

            var_r29 = -3.8999999f * var_r27;
        } else {
            var_r29 = 0;
        }

        if ((mProcID == PROC_WOLF_MOVE && checkAttentionState()) || temp_f31 < mpHIO->mWolf.mWlMove.m.mStandbyRunToRunRate) {
            var_r28 = 0;
        }
    } else if ((mProcID == PROC_WOLF_WAIT || mProcID == PROC_WOLF_SWIM_MOVE) && shape_angle.y != mPrevAngleY) {
        var_r28 = 0;
        var_r29 = (s16)(shape_angle.y - mPrevAngleY);

        if (mProcID == PROC_WOLF_SWIM_MOVE) {
            if (var_r29 >= 0) {
                var_r29 -= 0x100;
                if (var_r29 < 0) {
                    var_r29 = 0;
                }
            } else {
                var_r29 += 0x100;
                if (var_r29 > 0) {
                    var_r29 = 0;
                }
            }
        }

        var_r29 = cLib_minMaxLimit<s16>((s16)var_r29, -0x300, 0x300);
        var_r29 *= 8;
    } else {
        var_r28 = 0;
        var_r29 = 0;
    }

    cLib_addCalcAngleS(&field_0x2fee, var_r28, 4, 1200, 200);

    if (checkWolf()) {
        shape_angle.z = field_0x2fee;
        if (var_r29 == 0) {
            cLib_addCalcAngleS(&field_0x2fec, 0, 2, 4000, 1000);
        } else {
            cLib_addCalcAngleS(&field_0x2fec, var_r29, 4, 1200, 200);
        }
    } else if (mProcID != PROC_CAUGHT) {
        shape_angle.z = (field_0x2fee >> 1);
        mBodyAngle.z = shape_angle.z;

        if (mProcID == PROC_MOVE || mProcID == PROC_WAIT) {
            var_r29 = (s16)(shape_angle.y - mPrevAngleY) * 2;
            if (mProcID == PROC_WAIT) {
                var_r29 *= 2;
            }

            var_r29 = cLib_minMaxLimit<s16>((s16)var_r29, -0xC00, 0xC00);
            if (var_r29 == 0) {
                cLib_addCalcAngleS(&field_0x30a2, 0, 2, 4000, 1000);
            } else {
                cLib_addCalcAngleS(&field_0x30a2, var_r29, 4, 1200, 200);
            }
        }
    }
}

int daAlink_c::setArmMatrix() {
    static Vec const arm1Vec = {29.0f, 0.0f, 0.0f};
    static Vec const arm2Vec = {26.5f, 0.0f, 0.0f};
    static const u16 armJointTable[] = {0x0007,0x000C};

    daAlink_footData_c* var_r29 = mFootData2;

    int i;
    for (i = 0; i < 2; i++, var_r29++) {
        for (int j = 0; j < 3; j++) {
            cMtx_copy(mpLinkModel->getAnmMtx(j + armJointTable[i]), var_r29->field_0x14[j]);
        }

    }

    if (!field_0x2060->getOldFrameFlg()) {
        return 0;
    }

    var_r29 = mFootData2;

    cXyz sp2C;
    csXyz* sp18 = field_0x312a;
    csXyz* sp14 = field_0x3136;

    for (i = 0; i < 2; i++, var_r29++, sp18++, sp14++) {
        u16 temp_r28 = armJointTable[i];
        s16 spA;
        s16 sp8 = 0;

        if ((mProcID == PROC_HOOKSHOT_ROOF_SHOOT || mProcID == PROC_HOOKSHOT_WALL_SHOOT) && field_0x3020 == i) {
            spA = mProcVar3.field_0x300e;
            if (mProcID == PROC_HOOKSHOT_WALL_SHOOT) {
                sp8 = (mProcVar4.field_0x3010 - shape_angle.y);
            }
        } else {
            spA = 0;
        }

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r28), spA, sp8, var_r29->field_0x6, 0, NULL);

        mDoMtx_stack_c::ZXYrotM(*sp18);
        mDoMtx_copy(mDoMtx_stack_c::get(), mpLinkModel->getAnmMtx(temp_r28));
        mDoMtx_stack_c::multVec(&arm1Vec, &sp2C);
        temp_r28++;

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r28), spA, sp8, var_r29->field_0x4, 0, &sp2C);

        mDoMtx_stack_c::ZXYrotM(*sp14);
        mDoMtx_copy(mDoMtx_stack_c::get(), mpLinkModel->getAnmMtx(temp_r28));
        mDoMtx_stack_c::multVec(&arm2Vec, &sp2C);
        temp_r28++;

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r28), spA, sp8, var_r29->field_0x2, 0, &sp2C);
        temp_r28++;

        J3DTransformInfo* temp_r3_3 = field_0x2060->getOldFrameTransInfo(temp_r28);
        cXyz sp20(temp_r3_3->mTranslate.x, temp_r3_3->mTranslate.y, temp_r3_3->mTranslate.z);
        mDoMtx_stack_c::multVec(&sp20, &sp2C);

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r28), spA, sp8, var_r29->field_0x2, 0, &sp2C);
    }

    return 1;
}

int daAlink_c::setFootMatrix() {
    static Vec const leg1Vec = {30.0f, 0.0f, 0.0f};
    static Vec const leg2Vec = {39.363499f, 0.0f, 0.0f};
    static Vec const footVec = {14.18f, 0.0f, 0.0f};
    static const u16 footJointTable[] = {0x0012, 0x0017};

    daAlink_footData_c* var_r30 = mFootData1;

    int i;
    for (i = 0; i < 2; i++, var_r30++) {
        for (int j = 0; j < 3; j++) {
            cMtx_copy(mpLinkModel->getAnmMtx(j + footJointTable[i]), var_r30->field_0x14[j]);
        }

    }

    if (!field_0x2060->getOldFrameFlg()) {
        return 0;
    }

    var_r30 = mFootData1;
    cXyz sp10;

    if (mProcID == PROC_HORSE_GETOFF) {
        current.angle.y = shape_angle.y;
        if (field_0x2fc0 == 0) {
            shape_angle.y -= 0x4000;
        } else {
            shape_angle.y += 0x4000;
        }
    }

    for (i = 0; i < 2; i++, var_r30++) {
        u16 temp_r29 = footJointTable[i];

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r29), var_r30->field_0x6, 0, 0, 0, NULL);
        mDoMtx_stack_c::multVec(&leg1Vec, &sp10);
        temp_r29++;

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r29), var_r30->field_0x4, 0, 0, 0, &sp10);
        mDoMtx_stack_c::multVec(&leg2Vec, &sp10);
        temp_r29++;

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r29), var_r30->field_0x2, 0, 0, 0, &sp10);
        temp_r29++;
        mDoMtx_stack_c::multVec(&footVec, &sp10);

        setMatrixWorldAxisRot(mpLinkModel->getAnmMtx(temp_r29), var_r30->field_0x2, 0, 0, 0, &sp10);
    }

    if (mProcID == PROC_HORSE_GETOFF) {
        shape_angle.y = current.angle.y;
    }

    return 1;
}

void daAlink_c::setMatrixOffset(f32* param_0, f32 param_1) {
    if (param_0 != &mSinkShapeOffset) {
        cLib_addCalc(param_0, param_1, 0.5f, 7.5f, 2.5f);
    } else if (mProcID == PROC_TOOL_DEMO) {
        return;
    }

    mpLinkModel->getBaseTRMtx()[1][3] += *param_0;
    mInvMtx[1][3] -= *param_0;

    mDoMtx_stack_c::XrotS(shape_angle.x);
    mDoMtx_stack_c::concat(mInvMtx);
    mDoMtx_copy(mDoMtx_stack_c::get(), field_0x2be8);
}

int daAlink_c::setLegAngle(f32 param_0, daAlink_footData_c* param_1, s16* param_2,
                           s16* param_3, int param_4) {
    cXyz spA4;
    cXyz sp98;
    cXyz sp8C;
    cXyz sp80;
    if (fabsf(param_0) < 0.1f) {
        return 0;
    }

    if (param_4 != 0) {
        cMtx_concat(field_0x2be8, param_1->field_0x14[0], mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&spA4);

        cMtx_concat(field_0x2be8, param_1->field_0x14[1], mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp98);

        cMtx_concat(field_0x2be8, param_1->field_0x14[2], mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp8C);

        if (mProcID == PROC_HORSE_GETOFF) {
            if (field_0x2fc0 == 0) {
                spA4.z = -spA4.x;
                sp98.z = -sp98.x;
                sp8C.z = -sp8C.x;
            } else {
                spA4.z = spA4.x;
                sp98.z = sp98.x;
                sp8C.z = sp8C.x;
            }
        }

        spA4.x = 0.0f;
        sp98.x = 0.0f;
        sp8C.x = 0.0f;
    } else {
        cMtx_concat(mInvMtx, param_1->field_0x14[0], mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&spA4);

        cMtx_concat(mInvMtx, param_1->field_0x14[1], mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp98);

        cMtx_concat(mInvMtx, param_1->field_0x14[2], mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp8C);

        spA4.z = 0.0f;
        sp98.z = 0.0f;
        sp8C.z = 0.0f;
    }

    cXyz sp74 = sp98 - spA4;
    cXyz sp68 = sp8C - sp98;
    cXyz sp5C(sp8C);
    sp5C.y += param_0;

    if (param_4 != 0) {
        if (sp5C.y >= spA4.y) {
            return 0;
        }
    }

    if (param_4 == 0) {
        if (sp5C.y <= spA4.y) {
            sp5C.y = 1.0f + spA4.y;
        }
    }

    cXyz sp50 = sp5C - spA4;
    f32 temp_f1 = sp50.abs2();
    if (cM3d_IsZero(temp_f1)) {
        return 0;
    }

    f32 temp_f28 = sp74.abs2();
    f32 temp_f26 = sp68.abs2();

    if (JMAFastSqrt(temp_f28) + JMAFastSqrt(temp_f26) <= JMAFastSqrt(temp_f1)) {
        return 0;
    }

    f32 temp_f27 = ((temp_f1 + temp_f28) - temp_f26) / (2.0f * temp_f1);
    cXyz sp44 = spA4 + (sp50 * temp_f27);

    f32 var_f30 = temp_f28 - (temp_f27 * (temp_f1 * temp_f27));
    if (var_f30 < 0.0f) {
        var_f30 = 0.0f;
    }
    var_f30 = JMAFastSqrt(var_f30);

    if (param_4 != 0) {
        if (checkWolf()) {
            sp80.set(0.0f, -sp50.z, sp50.y);
        } else {
            sp80.set(0.0f, sp50.z, -sp50.y);
        }
    } else if (param_1 == mFootData2) {
        sp80.set(sp50.y, -sp50.x, 0.0f);
    } else {
        sp80.set(-sp50.y, sp50.x, 0.0f);
    }

    f32 temp_f1_2 = sp80.abs();
    if (cM3d_IsZero(temp_f1_2)) {
        return 0;
    }

    cXyz sp38 = sp44 + (sp80 * (var_f30 / temp_f1_2));

    cXyz sp2C = sp38 - spA4;
    cXyz sp20 = sp5C - sp38;

    if (param_4 != 0) {
        s16 temp_r30 = cM_atan2s(sp2C.y, sp2C.z);
        s16 var_r29 = cM_atan2s(sp20.y, sp20.z);
        s16 temp_r0 = var_r29 - temp_r30;

        if (checkWolf()) {
            if (temp_r0 < 0) {
                var_r29 = temp_r30;
            } else if (temp_r0 > 0x7000) {
                var_r29 = temp_r30 + 0x7000;
            }
        } else if (temp_r0 > 0) {
            var_r29 = temp_r30;
        } else if (temp_r0 < -0x7000) {
            var_r29 = temp_r30 - 0x7000;
        }

        *param_2 = cM_atan2s(sp74.y, sp74.z) - temp_r30;
        *param_3 = cM_atan2s(sp68.y, sp68.z) - var_r29;
    } else {
        s16 temp_r30_2 = cM_atan2s(sp2C.x, sp2C.y);
        s16 temp_r29 = cM_atan2s(sp20.x, sp20.y);

        *param_2 = cM_atan2s(sp74.x, sp74.y) - temp_r30_2;
        *param_3 = cM_atan2s(sp68.x, sp68.y) - temp_r29;
    }

    return 1;
}

void daAlink_c::footBgCheck() {
    static Vec const localLeftFootOffset = {-3.0f, 13.0f, 0.0f};
    static Vec const localRightFootOffset = {-3.0f, -13.0f, 0.0f};
    static Vec const localLeftToeOffset = {10.0f, 5.0f, 0.0f};
    static Vec const localRightToeOffset = {10.0f, -5.0f, 0.0f};

    if (field_0x2060->getOldFrameFlg()) {
        f32 var_f31 = 0.0f;
        cM3dGPla sp98;
        f32 sp30[2];

        int sp2C;
        u32 sp28 = checkModeFlg(1);
        int i; // r28
        f32* sp24;

        daAlink_footData_c* var_r29 = mFootData1;
        sp24 = sp30;
        s16 sp20[2];

        cXyz sp80[2];
        cXyz sp68[2];
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x14), &localLeftFootOffset, &sp68[0]);
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x15), &localLeftToeOffset, &sp80[0]);
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x19), &localRightFootOffset, &sp68[1]);
        mDoMtx_multVec(mpLinkModel->getAnmMtx(0x1A), &localRightToeOffset, &sp80[1]);

        BOOL sp1C = !mLinkAcch.ChkGroundHit() || checkMagneBootsOn() || (mLinkAcch.ChkGroundHit() && mSinkShapeOffset < 0.0f) || checkModeFlg(0x78C52);

        f32 var_f30;

        for (i = 0; i < 2; i++, var_r29++, sp24++) {
            cXyz sp5C = (sp80[i] + sp68[i]) * 0.5f;

            if (sp1C) {
                var_r29->field_0x1 = 5;
            } else {
                cXyz sp50 = sp5C - var_r29->field_0x8;
                if (sp50.abs2XZ() < 100.0f && sp28 != 0) {
                    if (var_r29->field_0x1 != 0) {
                        var_r29->field_0x1--;
                    } else {
                        sp5C = var_r29->field_0x8;
                    }
                } else {
                    var_r29->field_0x1 = 5;
                }
            }

            var_r29->field_0x8 = sp5C;
            cXyz sp44(sp5C.x, current.pos.y + l_autoUpHeight, sp5C.z);
            mLinkGndChk.SetPos(&sp44);

            f32 temp_f1 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
            if (-G_CM3D_F_INF != temp_f1) {
                dComIfG_Bgsp().GetTriPla(mLinkGndChk, &sp98);
            }

            if (-G_CM3D_F_INF != temp_f1 && cBgW_CheckBGround(sp98.mNormal.y) && sp44.y - temp_f1 < l_autoUpHeight - l_autoDownHeight) {
                *sp24 = temp_f1;
                var_r29->field_0x0 = 1;
                sp20[i] = getGroundAngle(&mLinkGndChk, shape_angle.y);
            } else {
                *sp24 = current.pos.y;
                var_r29->field_0x0 = 0;
            }
        }

        if (sp1C != 0) {
            sp2C = 2;
        } else {
            if (sp30[1] > sp30[0]) {
                sp2C = 0;
            } else {
                sp2C = 1;
            }

            var_f31 = sp30[sp2C];
        }

        setSandShapeOffset();
        setMatrixOffset(&mSinkShapeOffset, mSinkShapeOffset);
        setSandDownBgCheckWallH();

        if (mProcID == PROC_SERVICE_WAIT) {
            var_f31 = field_0x2b94;
        } else if (sp1C != 0 || sp28 == 0 || mProcID == PROC_TOOL_DEMO || mProcID == PROC_GANON_FINISH || fabsf(field_0x2b98) > 1.0f) {
            var_f31 = 0.0f;
        } else {
            var_f31 -= current.pos.y;
        }

        setMatrixOffset(&field_0x2b94, var_f31);
        var_r29 = mFootData1;

        for (i = 0; i < 2; i++, var_r29++) {
            s16 sp10;
            s16 spE;
            if (sp1C != 0) {
                sp10 = 0;
                spE = 0;
            } else {
                var_f30 = sp30[i] - mpLinkModel->getBaseTRMtx()[1][3];
                if (var_f30 > l_autoUpHeight) {
                    var_f30 = l_autoUpHeight;
                }

                if ((sp2C != i && !(var_f30 > 0.0f) && sp28 == 0) || !setLegAngle(var_f30, var_r29, &sp10, &spE, 1)) {
                    sp10 = 0;
                    spE = 0;
                }
            }

            if ((sp10 * var_r29->field_0x6) < 0 && abs(sp10 - var_r29->field_0x6) >= 0x8000) {
                if (sp10 >= 0) {
                    sp10 -= (s16)0x4000;
                } else {
                    sp10 += (s16)0x4000;
                }
            }

            cLib_addCalcAngleS(&var_r29->field_0x6, sp10, 2, 0x1800, 0x10);
            cLib_addCalcAngleS(&var_r29->field_0x4, spE, 2, 0x1800, 0x10);

            s16 spC = 0;
            if (sp2C != 2 && var_r29->field_0x0 != 0 && sp28 != 0 && mGroundCode != 8) {
                spC += sp20[i];
            }

            cLib_addCalcAngleS(&var_r29->field_0x2, spC, 2, 0x1800, 0x10);
        }
    }
}

void daAlink_c::handBgCheck() {
    static Vec const localHandPos[] = {
        {22.096375f, 5.4923248f, 0.0f},
        {-21.900146f, 5.5253749f, 0.0f},
    };

    if (!field_0x2060->getOldFrameFlg() || !checkModeFlg(0x40)) {
        return;
    }

    cXyz* var_r28;
    s16* var_r29;
    int i; // r27
    daAlink_footData_c* var_r26 = mFootData2;
    f32* var_r25;

    if (mProcID == PROC_HANG_CLIMB) {
        if (mUnderFrameCtrl[0].getFrame() >= 9.0f) {
            setMatrixOffset(&field_0x2ba4, 0.0f);
            return;
        }
    }

    cXyz sp38[2];
    f32 sp20[2];
    s16 sp18[2];

    var_r28 = sp38;
    var_r29 = sp18;
    var_r25 = sp20;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    for (i = 0; i < 2; i++, var_r28++, var_r25++, var_r26++, var_r29++) {
        mDoMtx_stack_c::multVec(&localHandPos[i], var_r28);

        cXyz sp28(var_r28->x, var_r28->y + l_autoUpHeight, var_r28->z);
        mLinkGndChk.SetPos(&sp28);

        f32 temp_f1 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        if (-G_CM3D_F_INF != temp_f1 && (sp28.y - temp_f1 < l_autoUpHeight - l_autoDownHeight)) {
            *var_r25 = temp_f1;

            if (dComIfG_Bgsp().GetGroundCode(mLinkGndChk) != 8) {
                *var_r29 = getGroundAngle(&mLinkGndChk, (shape_angle.y - 0x4000));
            } else {
                *var_r29 = 0;
            }
        } else {
            *var_r25 = current.pos.y;
            *var_r29 = 0;
        }
    }

    int sp14;
    if (mProcID == PROC_HANG_FALL_START) {
        sp14 = 0;
        sp20[1] += 500.0f;
    } else if (sp20[1] < sp20[0]) {
        sp14 = 0;
    } else {
        sp14 = 1;
    }

    setMatrixOffset(&field_0x2ba4, sp20[sp14] - current.pos.y);

    var_r28 = sp38;
    var_r26 = mFootData2;
    var_r29 = sp18;
    s16 spA;
    s16 sp8;

    for (i = 0; i < 2; i++, var_r28++, var_r29++, var_r26++) {
        f32 temp_f30 = sp20[i] - mpLinkModel->getBaseTRMtx()[1][3];
        if (!(temp_f30 < -1.0f) || !setLegAngle(temp_f30, var_r26, &spA, &sp8, 0)) {
            spA = 0;
            sp8 = 0;
        }

        cLib_addCalcAngleS(&var_r26->field_0x6, spA, 2, 0x1800, 0x10);
        cLib_addCalcAngleS(&var_r26->field_0x4, sp8, 2, 0x1800, 0x10);
        cLib_addCalcAngleS(&var_r26->field_0x2, *var_r29, 2, 0x1800, 0x10);
    }
}

JKRHeap* daAlink_c::setItemHeap() {
    if (!checkResetFlg0(RFLG0_UNK_4000)) {
        field_0x2fa0 ^= 1;
        onResetFlg0(RFLG0_UNK_4000);
    }

    return mItemHeap[field_0x2fa0].setAnimeHeap();
}

/**
 * Sets the animation archive ID and resource ID based on combined ID
 * The left most digit is the arcNo if not 0
 * The latter 3 digits are the resource ID
 */
void daAlink_c::setIdxMask(u16* o_arcNo, u16* o_resIdx) {
    if (*o_arcNo == 0xFFFF) {
        u16 arc_id = (*o_resIdx >> 12) & 0xF;
        *o_resIdx &= 0xFFF;

        if (arc_id != 0) {
            *o_arcNo = arc_id;
        }
    }
}

J3DAnmTransform* daAlink_c::getAnimeResource(daPy_anmHeap_c* i_anmHeap, u16 i_resIdx, u32 i_bufSize) {
    u16 arcNo;

    i_anmHeap->setBufferSize(i_bufSize);
    arcNo = 0xFFFF;

    setIdxMask(&arcNo, &i_resIdx);

    if (arcNo == 0xFFFF) {
        return (J3DAnmTransform*)i_anmHeap->loadDataIdx(i_resIdx);
    }

    return (J3DAnmTransform*)i_anmHeap->loadDataDemoRID(i_resIdx, arcNo);
}

J3DModel* daAlink_c::initModel(J3DModelData* i_modelData, u32 i_mdlFlags, u32 i_diffFlags) {
    JUT_ASSERT(4091, i_modelData != NULL);

    J3DTexture* tex = i_modelData->getTexture();
    int texNo = tex->getNum() - 1;

    int warpMaterial = false;
    if (texNo >= 0) {
        ResTIMG* timg = tex->getResTIMG(texNo);

        if (mpWarpTexData == (void*)((uintptr_t)timg + timg->imageOffset)) {
            warpMaterial = true;
        }
    }

    if (warpMaterial) {
        dRes_info_c::onWarpMaterial(i_modelData);
        i_diffFlags |= 0x2000400;
    }

    J3DModel* model = mDoExt_J3DModel__create(i_modelData, i_mdlFlags, i_diffFlags | 0x11000084);

    if (warpMaterial) {
        dRes_info_c::offWarpMaterial(i_modelData);
    }

    return model;
}

J3DModel* daAlink_c::initModel(u16 i_resIdx, u32 i_diffFlags) {
    return initModel((J3DModelData*)dComIfG_getObjectRes(l_arcName, i_resIdx), i_diffFlags);
}

J3DModel* daAlink_c::initModelEnv(u16 i_resIdx, u32 i_diffFlags) {
    return initModelEnv((J3DModelData*)dComIfG_getObjectRes(l_arcName, i_resIdx), i_diffFlags);
}

int daAlink_c::initDemoModel(J3DModel** i_ppmodel, char const* i_resName, u32 i_diffFlags) {
    *i_ppmodel = NULL;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), i_resName);
    if (modelData != NULL) {
        *i_ppmodel = initModel(modelData, i_diffFlags);
        if (*i_ppmodel == NULL) {
            return 0;
        }
    }

    return 1;
}

int daAlink_c::initDemoBck(mDoExt_bckAnm** i_ppbck, char const* i_resName) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), i_resName);
    if (bck != NULL) {
        *i_ppbck = new mDoExt_bckAnm();

        if (*i_ppbck == NULL) {
            return 0;
        }

        if (!(*i_ppbck)->init(bck, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
            return 0;
        }
    }

    return 1;
}

int daAlink_c::createHeap() {
    #if DEBUG
    l_HIO_before = NULL;
    #endif

    ResTIMG* warpTex = (ResTIMG*)dComIfG_getObjectRes("Always", dRes_ID_ALWAYS_BTI_WARP_TEX_e);
    mpWarpTexData = (void*)((uintptr_t)warpTex + warpTex->imageOffset);

    if (*dStage_roomControl_c::getDemoArcName() != 0) {
        if (!initDemoModel(&mpDemoHLTmpModel, "demo00_Link_cut00_HL_tmp.bmd", 0x1000000)) {
            return 0;
        }

        if (mpDemoHLTmpModel != NULL && !initDemoBck(&mpDemoHLTmpBck, "demo00_Link_cut00_HL_tmp.bck")) {
            return 0;
        }

        if (!initDemoModel(&mpDemoHRTmpModel, "demo00_Link_cut00_HR_tmp.bmd", 0)) {
            return 0;
        }

        if (mpDemoHRTmpModel != NULL && !initDemoBck(&mpDemoHRTmpBck, "demo00_Link_cut00_HR_tmp.bck")) {
            return 0;
        }

        if (!initDemoModel(&mpDemoFCBlendModel, "demo00_Link_cut00_FC_blend.bmd", 0)) {
            return 0;
        }

        if (!initDemoModel(&mpDemoFCTongueModel, "demo00_Link_cut00_FC_tongue.bmd", 0x200)) {
            return 0;
        }

        if (mpDemoFCTongueModel != NULL && !initDemoBck(&mpDemoFCTmpBck, "demo00_Link_cut00_FC_tmp.bck")) {
            return 0;
        }

        if (!initDemoBck(&mpDemoHDTmpBck, "demo00_Link_cut00_HD_tmp.bck")) {
            return 0;
        }

        if (mpDemoHDTmpBck != NULL) {
            field_0x06b4 = mpDemoHDTmpBck->getBckAnm();
            mpDemoHDTmpBck->changeBckOnly(NULL);
        }
    }

    if (!mSight.create()) {
        return 0;
    }

    mpHIO = new daAlinkHIO_c();
    if (mpHIO == NULL) {
        return 0;
    }

    if (!(mpWlChangeModel = initModel(dRes_ID_ALINK_BMD_WL_CHANGE_e, 0))) {
        return 0;
    }

    if (!(mpSwAModel = initModel(dRes_ID_ALINK_BMD_AL_SWA_e, 0x200))) {
        return 0;
    }

    if (!(mpSwMModel = initModelEnv(dRes_ID_ALINK_BMD_AL_SWM_e, 0x1000200))) {
        return 0;
    }

    if (!(mpSwASheathModel = initModel(dRes_ID_ALINK_BMD_AL_PODA_e, 0))) {
        return 0;
    }

    if (!(mpSwMSheathModel = initModelEnv(dRes_ID_ALINK_BMD_AL_PODM_e, 0))) {
        return 0;
    }

    int sp38 = 40;
    J3DTransformInfo* sp1C = new J3DTransformInfo[sp38];
    if (sp1C == NULL) {
        return 0;
    }

    Quaternion* sp30 = new Quaternion[sp38];
    if (sp30 == NULL) {
        return 0;
    }

    field_0x2060 = new mDoExt_MtxCalcOldFrame(sp1C, sp30);
    if (field_0x2060 == NULL) {
        return 0;
    }

    field_0x1f20 = new mDoExt_MtxCalcAnmBlendTblOld(field_0x2060, 3, mNowAnmPackUnder);
    if (field_0x1f20 == NULL) {
        return 0;
    }

    field_0x1f24 = new mDoExt_MtxCalcAnmBlendTblOld(field_0x2060, 3, mNowAnmPackUpper);
    if (field_0x1f24 == NULL) {
        return 0;
    }

    for (int i = 0; i < 2; i++) {
        field_0x2180[i] = new daAlink_matAnm_c();
        if (field_0x2180[i] == NULL) {
            return 0;
        }
    }

    mUnderAnmHeap[0].setBufferSize(0x10800);
    if (mUnderAnmHeap[0].mallocBuffer() == NULL) {
        return 0;
    }

    field_0x2d78 = new (0x20) u8[0x800];
    if (field_0x2d78 == NULL) {
        return 0;
    }

    if (mFaceBtpHeap.mallocBuffer() == NULL) {
        return 0;
    }

    if (mFaceBtkHeap.mallocBuffer() == NULL) {
        return 0;
    }

    if (mFaceBckHeap.mallocBuffer() == NULL) {
        return 0;
    }

    JKRReadIdxResource(mFaceBckHeap.getBuffer(), 0xC00, dRes_ID_ALANM_BCK_FAT_e, dComIfGp_getAnmArchive());
    J3DAnmTransform* bck = (J3DAnmTransform*)J3DAnmLoaderDataBase::load(mFaceBckHeap.getBuffer());
    if (!mFaceBck.init(bck, FALSE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false)) {
        return 0;
    }

    if (mAnmHeap9.mallocBuffer() == NULL) {
        return 0;
    }

    if (mpDemoFCBlendModel != NULL) {
        field_0x069c = new mDoExt_blkAnm();
        if (field_0x069c == NULL) {
            return 0;
        }

        mpDemoFCTmpBls = (J3DDeformData*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), "demo00_Link_cut00_FC_tmp.bls");
        if (mpDemoFCTmpBls != NULL) {
            s32 sp28 = mpDemoFCBlendModel->setDeformData(mpDemoFCTmpBls, 1);
            if (sp28 != 0) {
                return 0;
            }
        }
    }

    return 1;
}

static int daAlink_createHeap(fopAc_ac_c* i_this) {
    return ((daAlink_c*)i_this)->createHeap();
}

void daAlink_c::setSelectEquipItem(BOOL param_0) {
    J3DModel* prev_model;

    if (!checkWolf()) {
        prev_model = mSwordModel;

        if (checkWoodSwordEquip()) {
            mSwordModel = mWoodSwordModel;
            mSheathModel = mpSwMSheathModel;
        } else if (checkMasterSwordEquip()) {
            mSwordModel = mpSwMModel;
            mSheathModel = mpSwMSheathModel;
        } else {
            if (!checkSwordGet()) {
                mSwordChangeWaitTimer = 100;
            }
            mSwordModel = mpSwAModel;
            mSheathModel = mpSwASheathModel;
        }

        if (!checkSwordGet() || checkWoodSwordEquip() || checkNoResetFlg3(FLG3_UNK_1000000)) {
            if (field_0x06ec != NULL) {
                field_0x06ec->hide();
            }
        } else {
            if (field_0x06ec != NULL) {
                field_0x06ec->show();
            }
        }

        if (mClothesChangeWaitTimer == 0 &&
            (prev_model != mSwordModel || checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW)))
        {
            if (prev_model != mSwordModel) {
                mSwordChangeWaitTimer = 5;
            }

            if (checkWoodSwordEquip()) {
                if (mEquipItem == 0x103 || param_0) {
                    mSwordModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
                } else {
                    mSwordModel->getModelData()->getMaterialNodePointer(1)->getShape()->show();
                }
            } else if (mEquipItem == 0x103 || param_0) {
                mSwordModel->getModelData()->getMaterialNodePointer(0)->getShape()->show();
            } else {
                mSwordModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
            }
        }
    } else if (checkMasterSwordEquip()) {
        mSwordModel = mpSwMModel;
        mSheathModel = mpSwMSheathModel;
    } else {
        mSwordModel = mpSwAModel;
        mSheathModel = mpSwASheathModel;
    }

    if (mClothesChangeWaitTimer == 0) {
        if (checkZoraWearAbility()) {
            if (checkZoraWearMaskDraw()) {
                field_0x06f0->show();
                if (!checkEquipHeavyBoots()) {
                    field_0x06e4->show();
                }
            } else {
                field_0x06f0->hide();
                field_0x06e4->hide();
            }
        }

        simpleAnmPlay(mMagicArmorBodyBrk);
        simpleAnmPlay(mMagicArmorHeadBrk);
    }
}

BOOL daAlink_c::checkBoarStart() {
    // Stage: Bulblin Camp   Room: Outside Arbiter's
    return (checkStageName("F_SP118") && getStartRoomNo() == 3) && dComIfGs_getStartPoint() == 0;
}

BOOL daAlink_c::checkCanoeStart() {
    // Stage: Lake Hylia   Room: Lake
    return (checkStageName("F_SP115") && getStartRoomNo() == 0) && dComIfGs_getStartPoint() == 2;
}

void daAlink_c::playerInit() {
    mHeavySpeedMultiplier = 1.0f;

    if (!checkDungeon() && !checkBossRoom() && checkItemGet(fpcNm_ITEM_DUNGEON_EXIT, 1)) {
        dComIfGs_setItem(SLOT_18, fpcNm_ITEM_TKS_LETTER);
    }

    u16 i;
    dCcD_Cyl* tgCyl = mTgCyls;
    dCcD_Cyl* windTgCyl = mWindTgCyls;
    for (i = 0; i < 3; i++, tgCyl++, windTgCyl++) {
        tgCyl->Set(l_cylSrc);
        tgCyl->SetStts(&mCcStts);
        tgCyl->OnTgNoConHit();
        tgCyl->SetTgShieldFrontRangeYAngle(&field_0x306c);
        tgCyl->SetTgHitCallback(daAlink_tgHitCallback);
        tgCyl->SetCoHitCallback(daAlink_coHitCallback);

        windTgCyl->Set(l_cylSrc);
        windTgCyl->SetStts(&mCcStts);
        windTgCyl->OffCoSetBit();
        windTgCyl->SetTgMtrl(dCcD_MTRL_WIND);
        windTgCyl->OnTgNoHitMark();
        windTgCyl->SetTgType(0xD8FBFFFF);
        windTgCyl->OnTgSPrmBit(0x20);
    }

    mTgCyls[2].SetH(90.0f);
    mTgCyls[1].SetH(90.0f);

    mAtSph.Set(l_sphSrc);
    mAtSph.SetStts(&mCcStts);
    mAtSph.StartCAt(current.pos);

    mAnmHeap3.setBufferSize(0x20000);
    mAnmHeap3.createHeap(daPy_anmHeap_c::HEAP_TYPE_4);

    if (checkWolf()) {
        changeWolf();
    } else {
        changeLink(0);
    }

    mAnmHeap4.setBufferSize(0xB00);
    mAnmHeap4.createHeap(daPy_anmHeap_c::HEAP_TYPE_4);
    setShieldModel();

    #if DEBUG
    l_debugMode = FALSE;
    #endif

    mSwordModel = mpSwAModel;
    mSheathModel = mpSwASheathModel;

    mCcStts.Init(120, 0xFF, this);
    field_0x306c = shape_angle.y + mBodyAngle.y;

    for (i = 0; i < 3; i++) {
        mAtCps[i].Set(l_atCpsSrc);
        mAtCps[i].SetStts(&mCcStts);
    }

    mGuardAtCps.Set(l_atCpsSrc);
    mGuardAtCps.SetStts(&mCcStts);
    mGuardAtCps.SetAtType(AT_TYPE_SHIELD_ATTACK);
    mGuardAtCps.OnAtNoHitMark();
    mGuardAtCps.SetAtHitMark(0);
    mGuardAtCps.SetAtAtp(0);

    mAtCyl.Set(l_atCylSrc);
    mAtCyl.SetStts(&mCcStts);

    mLinkAcch.Set(this, 3, mAcchCir);
    mLinkAcch.ClrWaterNone();
    mLinkAcch.SetWaterCheckOffset(10000.0f);
    mLinkAcch.OnLineCheck();
    mLinkAcch.ClrRoofNone();
    mLinkAcch.SetRoofCrrHeight(mHeight);
    mLinkAcch.SetGndThinCellingOff();
    mLinkAcch.SetWtrChkMode(2);
    mLinkAcch.OnWallSort();

    mAcchCir[0].SetWall(l_autoUpHeight, 35.0f);
    mAcchCir[1].SetWall(129.99000549316406f, 35.0f);
    mAcchCir[2].SetWall(mHeight, 35.0f);
    mPrevAngleY = shape_angle.y;

    for (i = 0; i < 3; i++) {
        mUnderAnmHeap[i].setBuffer(mUnderAnmHeap[0].getBuffer() + (i * 0x2C00));
        mUnderAnmHeap[i].createHeap(daPy_anmHeap_c::HEAP_TYPE_3);
    }
    mUpperAnmHeap[0].setBuffer(mUnderAnmHeap[0].getBuffer() + 0x8400);

    for (i = 0; i < 3; i++) {
        mUpperAnmHeap[i].setBuffer(mUpperAnmHeap[0].getBuffer() + (i * 0x2C00));
        mUpperAnmHeap[i].createHeap(daPy_anmHeap_c::HEAP_TYPE_3);
    }

    mFaceBtpHeap.createHeap(daPy_anmHeap_c::HEAP_TYPE_1);
    mFaceBtkHeap.createHeap(daPy_anmHeap_c::HEAP_TYPE_2);
    mFaceBckHeap.createHeap(daPy_anmHeap_c::HEAP_TYPE_3);

    for (i = 0; i < 2; i++) {
        mItemHeap[i].setBufferSize(0x13200);
        mItemHeap[i].createHeap(daPy_anmHeap_c::HEAP_TYPE_4);
    }
    mAnmHeap9.createHeap(daPy_anmHeap_c::HEAP_TYPE_3);
    resetBasAnime();

    mZ2Link.init(&current.pos, &eyePos, &field_0x3720);
    mZ2Link.initKantera(&mKandelaarFlamePos);
    mZ2Link.setKanteraState(0);
    mProcID = PROC_MAX;

    m_swordBlur.m_blurTex = (ResTIMG*)dComIfG_getObjectRes(l_arcName, dRes_ID_ALINK_BTI_BLUR_e);
    JUT_ASSERT(4753, m_swordBlur.m_blurTex != NULL);

    J3DModelData* modelData = mpSwAModel->getModelData();
    m_nSwordBtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_ALINK_BTK_AL_SWA_e);
    JUT_ASSERT(4759, m_nSwordBtk);
    m_nSwordBtk->searchUpdateMaterialID(modelData);
    modelData->entryTexMtxAnimator(m_nSwordBtk);

    J3DModelData* modelData2 = mpSwMModel->getModelData();
    m_mSwordBtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_ALINK_BTK_AL_SWM_e);
    JUT_ASSERT(4767, m_mSwordBtk);
    m_mSwordBtk->searchUpdateMaterialID(modelData2);
    modelData2->entryTexMtxAnimator(m_mSwordBtk);

    m_mSwordBrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, dRes_ID_ALINK_BRK_AL_SWM_e);
    JUT_ASSERT(4774, m_mSwordBrk != NULL);
    m_mSwordBrk->searchUpdateMaterialID(modelData2);
    modelData2->entryTevRegAnimator(m_mSwordBrk);

    mWaterY = -G_CM3D_F_INF;
    field_0x33b8 = -G_CM3D_F_INF;
    field_0x33bc = -G_CM3D_F_INF;

    mEquipItem = fpcNm_ITEM_NONE;
    offSwordModel();

    mAlinkStaffId = -1;
    mExitID = 0x3F;
    onNoResetFlg0(FLG0_SWIM_UP);
    offOxygenTimer();

    int startMode = getStartMode();
    int startEvent = getStartEvent();

    if (dComIfGp_getStartStagePoint() == -2 || dComIfGp_getStartStagePoint() == -3) {
        mStartEventID = dComIfGp_evmng_startDemo(-1);
    } else if (dComIfGp_getStartStagePoint() == -4) {
        mStartEventID = dComIfGp_evmng_startDemo(0xD5);
    } else {
        if (getLastSceneMode() == 9) {
            mStartEventID = dComIfGp_evmng_startDemo(0xD3);
        } else if (startMode == 10) {
            if (startEvent != 0xFF) {
                mStartEventID = dComIfGp_evmng_startDemo(startEvent);
            } else {
                mStartEventID = dComIfGp_evmng_startDemo(0xCF);
            }
        } else if (startMode == 11) {
            if (startEvent != 0xFF) {
                mStartEventID = dComIfGp_evmng_startDemo(startEvent);
            } else {
                mStartEventID = dComIfGp_evmng_startDemo(0xD0);
            }
        } else if (startMode == 6) {
            mStartEventID = dComIfGp_evmng_startDemo(0xCD);
        } else if (startMode == 7) {
            mStartEventID = dComIfGp_evmng_startDemo(0xCE);
        } else if (startMode == 8) {
            if (startEvent != 0xFF) {
                mStartEventID = dComIfGp_evmng_startDemo(startEvent);
            } else {
                mStartEventID = dComIfGp_evmng_startDemo(0xD4);
            }
        } else if (startMode == 12) {
            mStartEventID = dComIfGp_evmng_startDemo(0xC9);
        } else if (getLastSceneMode() == 11) {
            mStartEventID = dComIfGp_evmng_startDemo(0xFF);
        } else if (getLastSceneMode() == 12) {
            mStartEventID = dComIfGp_evmng_startDemo(0xD1);
        } else {
            mStartEventID = dComIfGp_evmng_startDemo(startEvent);
        }
    }

    dComIfGp_getPEvtManager()->orderStartDemo();
    field_0x2f94 = -1;
    field_0x2f95 = -1;
    field_0x2f96 = -1;
    field_0x2f97 = -1;

    for (int i = 0; i < 16; i++) {
        mShieldArrowIDs[i] = fpcM_ERROR_PROCESS_ID_e;
    }

    mAtnActorID = fpcM_ERROR_PROCESS_ID_e;
    mMsgClassID = fpcM_ERROR_PROCESS_ID_e;
    field_0x28f8 = fpcM_ERROR_PROCESS_ID_e;
    field_0x28fc = fpcM_ERROR_PROCESS_ID_e;

    field_0x2e54.init(&mLinkAcch, mpHIO->mBasic.m.mWaterSurfaceEffectHeight, mHeight);
    field_0x3108 = shape_angle.y;

    field_0x2f20.setOldPosP(&field_0x3624, &field_0x3630);
    field_0x2fc3 = 10;

    mMagneBootsPlight.mPosition = current.pos;
    mMagneBootsPlight.mColor.r = 80;
    mMagneBootsPlight.mColor.g = 80;
    mMagneBootsPlight.mColor.b = 200;

    mMagneBootsPlight.mPow = 0.0f;
    mMagneBootsPlight.mFluctuation = 0.0f;
    dKy_plight_set(&mMagneBootsPlight);

    setSelectEquipItem(FALSE);

    if (checkStageName("D_MN08") || checkStageName("D_MN08B") || checkStageName("D_MN08C")) {
        onNoResetFlg3(FLG3_UNK_40000000);
    }
}

BOOL daAlink_c::checkHorseStart(u32 i_lastMode, int i_startMode) {
    return i_lastMode == 1 || i_startMode == 2 || i_lastMode == 8;
}

int daAlink_c::setStartProcInit() {
    BOOL sp10 = FALSE;
    int start_mode = getStartMode();
    u32 last_mode = getLastSceneMode();
    daHorse_c* horsep = (daHorse_c*)dComIfGp_getHorseActor();
    BOOL isHorseStart = checkHorseStart(last_mode, start_mode);

    setDamagePoint(getLastSceneDamage(), last_mode == 4, FALSE, 1);
    mSwordUpTimer = getLastSceneSwordAtUpTime() * 2;

    if (checkWolf()) {
        setFaceBasicAnime(ANM_WAIT);
    } else {
        mEquipItem = (dComIfGs_getLastSceneMode() >> 0x18) & 0xFF;
        if (mEquipItem == fpcNm_ITEM_SWORD) {
            mEquipItem = 0x103;
        } else if (checkStageName("D_MN09B") && last_mode != 8 && start_mode == 2) {
            mEquipItem = 0x103;
        }

        if (mEquipItem == 0
            || !checkCastleTownUseItem(mEquipItem)
            || (checkCloudSea() && mEquipItem != 0x103)
            || checkCanoeStart()
            || (isHorseStart
                && mEquipItem != 0x103
                && !checkBowAndSlingItem(mEquipItem)
                && mEquipItem != fpcNm_ITEM_BOOMERANG
                && mEquipItem != fpcNm_ITEM_KANTERA
                && !checkHookshotItem(mEquipItem))
            )
        {
            mEquipItem = fpcNm_ITEM_NONE;
        } else {
            setItemModel();
        }

        if (isHorseStart) {
            horsep->setHorsePosAndAngle(&current.pos, shape_angle.y);
            horsep->initHorseMtx();
            initForceRideHorse();
        }
    }

    if (last_mode == 6) {
        procCoPeepSubjectivityInit();
    } else if (last_mode == 11) {
        mLinkAcch.SetGroundHit();
        procBoardWaitInit(fopAcM_SearchByID(mRideActorID));
    } else if (last_mode == 12) {
        procDungeonWarpSceneStartInit();
    } else if (dComIfGp_getStartStagePoint() == -4) {
        procCoWarpInit(1, 1);
    } else if (start_mode == 8 && mStartEventID == 0xFF) {
        procCoWarpInit(1, 0);
    } else if (last_mode == 4 || start_mode == 12 || last_mode == 5) {
        if (checkHorseRide()) {
            procHorseComebackInit();
        } else {
            commonLargeDamageUpInit(-2, TRUE, 0, 0);
            if (checkWolf()) {
                sp10 = TRUE;
            }
        }
    } else if (last_mode == 9) {
        procWolfDigThroughInit(1);
    } else if (last_mode == 8) {
        initForceRideHorse();
        procHorseComebackInit();
    } else if (last_mode == 10) {
        checkWaitAction();
    } else if (start_mode == 9) {
        if (checkWolf()) {
            procWolfTagJumpInit(NULL);
        } else {
            procAutoJumpInit(1);
        }
    } else if (start_mode == 4) {
        if (checkWolf()) {
            procWolfAutoJumpInit(1);
        } else {
            procAutoJumpInit(1);
        }
        mNormalSpeed = 55.0f;
        speed.y = 0.0f;
    } else if (checkCanoeStart()) {
        procCanoeJumpRideInit(NULL);
    } else if (checkBoarStart()) {
        if (!initForceRideBoar()) {
            procWaitInit();
        } else {
            procHorseWaitInit();
        }
    } else if (start_mode == 1 || start_mode == 3 || start_mode == 5 || start_mode == 14 || start_mode == 13 || start_mode == 2 || start_mode == 0) {
        if (start_mode == 3) {
            shape_angle.y = dComIfGs_getLastSceneAngleY();
            current.angle.y = shape_angle.y;
            mPrevAngleY = shape_angle.y;
        }

        if (mStartEventID == 0xFF) {
            if (!checkModeFlg(0x400) && start_mode != 0) {
                mNormalSpeed = dComIfGs_getLastSceneSpeedF();
            }

            mDemo.setStartDemoType();
            mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_14_e);
            mDemo.setMoveAngle(current.angle.y);
            mDemo.setTimer(35);

            if (checkModeFlg(0x400)) {
                daHorse_c* horse = dComIfGp_getHorseActor();
                horse->changeOriginalDemo();
                horse->changeDemoMode(6, 0);

                if (start_mode == 2) {
                    if (checkStageName("F_SP109") && fopAcM_GetRoomNo(this) == 0 && dComIfGs_getStartPoint() == 35) {
                        horse->setSpeedF(horse->getNormalMaxSpeedF());
                    } else {
                        horse->setSpeedF(0.0f);
                    }
                } else if (start_mode == 1) {
                    horse->setWalkSpeedF();
                } else {
                    horse->setSpeedF(dComIfGs_getLastSceneSpeedF());
                }

                procHorseWaitInit();
            } else if (checkSwimAction(1)) {
                if (start_mode == 1) {
                    if (checkWolf()) {
                        mNormalSpeed = 0.5f * mpHIO->mWolf.mWlSwim.m.mMaxSpeed;
                    } else {
                        mNormalSpeed = 0.5f * mpHIO->mSwim.m.mForwardMaxSpeed;
                    }
                }

                speedF = mNormalSpeed;

                if (checkWolf()) {
                    procWolfSwimMoveInit();
                } else if (mNormalSpeed < mpHIO->mSwim.m.mForwardMinSpeed) {
                    procSwimWaitInit(0);
                } else {
                    procSwimMoveInit();
                }

                field_0x2f99 = 0;
            } else if (start_mode == 1) {
                if (checkWolf()) {
                    mNormalSpeed = mpHIO->mWolf.mWlMove.m.mIdleToWalkRate * mpHIO->mWolf.mWlMove.m.mMaxSpeed;
                    speedF = mNormalSpeed;
                    procWolfMoveInit();
                } else {
                    mNormalSpeed = mpHIO->mMove.m.mWalkChangeRate * mpHIO->mMove.m.mMaxSpeed;
                    speedF = mNormalSpeed;
                    procMoveInit();
                }
            } else if (start_mode == 0) {
                checkWaitAction();
            } else if (last_mode == 2 || last_mode == 3) {
                mDemo.setStick(dComIfGs_getLastSceneSpeedF());
                mMoveAngle = shape_angle.y;

                if (last_mode == 3) {
                    shape_angle.y += 0x8000;
                    current.angle.y = shape_angle.y;
                    mPrevAngleY = shape_angle.y;
                }

                if (checkWolf()) {
                    procWolfLieMoveInit(0);
                } else {
                    procCrawlMoveInit(0, 0);
                }
            } else if (checkWolf()) {
                if (mNormalSpeed > mpHIO->mWolf.mWlMove.m.mMaxSpeed) {
                    mNormalSpeed = mpHIO->mWolf.mWlMove.m.mMaxSpeed;
                }
                speedF = mNormalSpeed;
                procWolfMoveInit();
            } else {
                if (mNormalSpeed > mpHIO->mMove.m.mMaxSpeed) {
                    mNormalSpeed = mpHIO->mMove.m.mMaxSpeed;
                }
                speedF = mNormalSpeed;
                procMoveInit();
            }
        } else {
            checkWaitAction();
        }
    } else {
        checkWaitAction();
    }

    return sp10;
}

int daAlink_c::create() {
    fopAcM_ct(this, daAlink_c);

    static BOOL bgWaitFlg = FALSE;

    u32 sceneMode = getLastSceneMode();
    s32 startMode = getStartMode();
    s16 startPoint = dComIfGp_getStartStagePoint();
    BOOL isHorseStart = checkHorseStart(sceneMode, startMode);

    // Stage: City   Room: Entrance   Layer: 0
    BOOL isEnteringLV7 = checkStageName("D_MN07")
                          && dComIfGp_roomControl_getStayNo() == 0
                          && dComIfG_play_c::getLayerNo(0) == 0
                          && current.pos.y > 7500.0f;

    if (!bgWaitFlg) {
        #if DEBUG
        if (g_playerKind == 2) {
            dComIfGs_setSelectEquipClothes(fpcNm_ITEM_WEAR_CASUAL);
        } else if (g_playerKind == 3) {
            dComIfGs_setSelectEquipClothes(fpcNm_ITEM_WEAR_KOKIRI);
        } else if (g_playerKind == 4) {
            dComIfGs_setSelectEquipClothes(fpcNm_ITEM_WEAR_ZORA);
        } else if (g_playerKind == 5) {
            dComIfGs_setSelectEquipClothes(fpcNm_ITEM_ARMOR);
        } else
        #endif
        // Event Flag: Finished Sewers
        if (checkCasualWearFlg() && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[47])) {
            dComIfGs_setSelectEquipClothes(fpcNm_ITEM_WEAR_KOKIRI);
        }

        if (isEnteringLV7 && checkMagicArmorHeavy()) {
            dComIfGs_setSelectEquipClothes(fpcNm_ITEM_WEAR_KOKIRI);
        }

        dComIfGp_setPlayer(0, this);
        dComIfGp_setLinkPlayer(this);
        fopAcM_setStageLayer(this);

        if (sceneMode == 7) {
            current.pos = dComIfGs_getTurnRestartPos();
            shape_angle.y = dComIfGs_getTurnRestartAngleY();
            current.angle.y = shape_angle.y;
        }

        if ((
                (
                    !checkBossOctaIealRoom()
                    #if DEBUG
                    && g_playerKind == 0
                    #endif
                )
                && dComIfGs_Wolf_Change_Check() == TRUE
            )
            ||
            (
                #if DEBUG
                g_playerKind == 1 ||
                #endif
                startPoint == -4
            )
            || sceneMode == 9
            )
        {
            attention_info.position.set(current.pos.x + cM_ssin(shape_angle.y) * 70.0f,
                                         current.pos.y + 80.0f,
                                         current.pos.z + cM_scos(shape_angle.y) * 70.0f);
            onNoResetFlg1(FLG1_IS_WOLF);
        } else if (isHorseStart) {
            attention_info.position.y = current.pos.y + 275.0f;
        } else {
            attention_info.position.y = current.pos.y + 150.0f;
        }
        attention_info.flags = -1;

        if (!dComIfGp_getEventManager().dataLoaded()) {
            return cPhs_INIT_e;
        }

        setArcName(checkWolf());
        setOriginalHeap(&mpArcHeap, 0xA2800);
        if (dComIfG_resLoad(&mPhaseReq, mArcName, mpArcHeap) != cPhs_COMPLEATE_e) {
            return cPhs_INIT_e;
        }

        setShieldArcName();
        setOriginalHeap(&mpShieldArcHeap, 0x7000);
        if (dComIfG_resLoad(&mShieldPhaseReq, mShieldArcName, mpShieldArcHeap) != cPhs_COMPLEATE_e) {
            return cPhs_INIT_e;
        }
        
        u32 heapSize = 0x3E930;
        heapSize |= 0x80000000;
        heapSize |= 0x40000000;

        if (!fopAcM_entrySolidHeap(this, daAlink_createHeap, heapSize)) {
            return cPhs_ERROR_e;
        }

        mAttention = dComIfGp_getAttention();
        field_0x317c = dComIfGp_getPlayerCameraID(0);

        playerInit();
        bgWaitFlg = TRUE;

        if (checkCanoeStart()) {
            mRideActorID = fopAcM_create(PROC_CANOE, 0, &current.pos, fopAcM_GetRoomNo(this),
                                         &shape_angle, NULL, -1);
        } else if (sceneMode == 11) {
            mRideActorID = fopAcM_create(PROC_Obj_IceLeaf, 0x1FFFF, &current.pos,
                                         fopAcM_GetRoomNo(this), &shape_angle, NULL, -1);
        } else {
            mRideActorID = fpcM_ERROR_PROCESS_ID_e;
        }
    }

    mLinkAcch.CrrPos(dComIfG_Bgsp());
    void* portalActor = NULL;

    if (mLinkAcch.GetGroundH() == -G_CM3D_F_INF
        || (startMode == 14 && !dComIfG_Bgsp().ChkMoveBG(mLinkAcch.m_gnd))
        || (startPoint == -4 && !(portalActor = fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchPortal, &current.pos)))
        || (mRideActorID != fpcM_ERROR_PROCESS_ID_e && !fopAcM_SearchByID(mRideActorID))
        || (checkCanoeStart() && !fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchCanoe, NULL))
        || (checkBoarStart() && !fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchBoar, NULL))
        || (startMode == 13 && (!mLinkAcch.ChkWaterHit() || mLinkAcch.m_wtr.GetHeight() < current.pos.y))
        || ((checkCarryStartLightBallA() || checkCarryStartLightBallB()) && !fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchLightBall, NULL))
        || (isHorseStart && dComIfGp_getHorseActor() == NULL)
        )
    {
        return cPhs_INIT_e;
    }

    if (portalActor != NULL) {
        dComIfGp_getEvent()->setPtD(portalActor);
    }

    bgWaitFlg = FALSE;

    dComIfGs_setRestartRoom(current.pos, shape_angle.y, getStartRoomNo());
    field_0x3780 = current.pos;
    mLinkAcch.ClrGndThinCellingOff();

    int bg_roomId = dComIfG_Bgsp().GetRoomId(mLinkAcch.m_gnd);
    fopAcM_SetRoomNo(this, bg_roomId + 1);
    setRoomInfo();
    setWaterY();

    if (checkStageName("F_SP102") && fopAcM_GetRoomNo(this) == 0 &&
        dComIfG_play_c::getLayerNo(0) == 4)
    {
        onNoResetFlg2(FLG2_BOAR_SINGLE_BATTLE_1ST);
    } else if (checkStageName("F_SP123") && fopAcM_GetRoomNo(this) == 13 &&
               dComIfG_play_c::getLayerNo(0) == 0)
    {
        onNoResetFlg2(FLG2_BOAR_SINGLE_BATTLE_2ND);
    }

    J3DAnmTransform* underBck;
    J3DAnmTransform* upperBck;
    getUnderUpperAnime(ANM_WAIT, &underBck, &upperBck, 0, 0x2C00);
    mNowAnmPackUnder[0].setAnmTransform(underBck);

    if (upperBck != NULL) {
        mNowAnmPackUpper[0].setAnmTransform(upperBck);
    } else {
        mNowAnmPackUpper[0].setAnmTransform(underBck);
    }

    int midna_prm = setStartProcInit();
    setSelectEquipItem(FALSE);
    setMatrix();
    allAnimePlay();
    mpLinkModel->calc();
    playFaceTextureAnime();

    if (!checkWolf()) {
        setItemMatrix(0);
    } else {
        setWolfItemMatrix();
    }

    setBodyPartPos();
    setHangWaterY();

    mTgCyls[0].SetC(current.pos);
    field_0x3454 = field_0x3834.y;
    setAttentionPos();
    setItemActor();

    if ((dComIfGs_getLastSceneMode() & 0x400000) && !checkWolf() && !checkNotHeavyBootsStage() &&
        !isHorseStart && !isEnteringLV7)
    {
        setHeavyBoots(1);
    }

    if ((dComIfGs_getLastSceneMode() & 0x200000) && !checkCloudSea()) {
        onNoResetFlg2(FLG2_UNK_1);
        mZ2Link.setKanteraState(2);
    }

    if (checkCarryStartLightBallA() || checkCarryStartLightBallB()) {
        setForceGrab((fopAc_ac_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchLightBall, NULL),
                     1, 1);
    }

    #if DEBUG
    // "Link"
    mpHIO->entryHIO("リンク");
    l_jumpTop = 0.0f;
    #endif

    fopAcM_create(PROC_MIDNA, midna_prm, &current.pos, fopAcM_GetRoomNo(this), &shape_angle, NULL, -1);
    checkSetNpcTks(&current.pos, fopAcM_GetRoomNo(this), 1);

    if (startPoint == -4 && dComIfGp_TargetWarpPt_get() != 0xFF && !dComIfGp_TransportWarp_check()) {
        daTagMhint_c::createPortalWarpMissTag(0xBBE, fopAcM_GetID(this));
    }

    if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) == dStage_SaveTbl_LV2) {
        if (!dComIfGs_isItemFirstBit(fpcNm_ITEM_HYLIA_SHIELD) && !dComIfGs_isItemFirstBit(fpcNm_ITEM_SHIELD) &&
            !dComIfGs_isItemFirstBit(fpcNm_ITEM_WOOD_SHIELD))
        {
            fopAcM_onSwitch(this, 0x6F);
        } else {
            fopAcM_offSwitch(this, 0x6F);
        }
    }

    return cPhs_COMPLEATE_e;
}

static int daAlink_Create(fopAc_ac_c* actor) {
    daAlink_c* i_this = (daAlink_c*)actor;
    fpc_ProcID id = fopAcM_GetID(actor);
    return i_this->create();
}

int daAlink_c::setRoomInfo() {
    s32 roomID;

    if (mProcID != PROC_TW_GATE) {
        roomID = dComIfG_Bgsp().GetRoomId(mLinkAcch.m_gnd);

        if (roomID != fopAcM_GetRoomNo(this)) {
            tevStr.room_no = roomID;
            mVoiceReverbIntensity = dComIfGp_getReverb(roomID);
            mCcStts.SetRoomId(roomID);
            fopAcM_SetRoomNo(this, roomID);
        }

        dStage_RoomCheck(&mLinkAcch.m_gnd);
    } else {
        roomID = fopAcM_GetRoomNo(this);
    }

    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mLinkAcch.m_gnd);
    mGroundCode = dComIfG_Bgsp().GetGroundCode(mLinkAcch.m_gnd);
    mGndPolyAtt0 = dComIfG_Bgsp().GetPolyAtt0(mLinkAcch.m_gnd);
    mGndPolySpecialCode = dComIfG_Bgsp().GetSpecialCode(mLinkAcch.m_gnd);

    if (mGndPolySpecialCode == dBgW_SPCODE_HEAVY_SNOW && checkWolf()) {
        mGndPolySpecialCode = dBgW_SPCODE_LIGHT_SNOW;
    }

    if (mGndPolyAtt1 != 0xFF) {
        if (mGndPolyAtt0 == 3) {
            mGndPolyAtt1 = dComIfG_Bgsp().GetPolyAtt1(mLinkAcch.m_gnd);

            if (mGndPolyAtt1 == 4 && checkWolf()) {
                mGndPolyAtt1 = 0;
            }

            if (!checkEventRun() && (mGndPolyAtt1 == 1 || mGndPolyAtt1 == 3) &&
                (checkBootsOrArmorHeavy() || mSinkShapeOffset < 1.0f + field_0x3458))
            {
                mGndPolyAtt1 = 2;
            }
        } else {
            mGndPolyAtt1 = 0;
        }
    }

    return roomID;
}

void daAlink_c::setShapeAngleOnGround() {
    field_0x33cc = cLib_minMaxLimit<f32>(field_0x33cc, 0.0f, 1.0f);

    if (checkModeFlg(0x01000000)) {
        cXyz sp20;
        cXyz sp14;
        cM3dGPla sp2C;

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::multVec(&l_crawlFrontUpOffset, &sp20);
        mDoMtx_stack_c::multVec(&l_crawlBackUpOffset, &sp14);

        mLinkGndChk.SetPos(&sp20);

        sp20.y = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        bool var_r29;
        if (-G_CM3D_F_INF != sp20.y) {
            var_r29 = dComIfG_Bgsp().GetTriPla(mLinkGndChk, &sp2C);
        } else {
            var_r29 = 0;
        }

        f32 var_f31;
        if (var_r29) {
            var_f31 = sp2C.mNormal.y;
        } else {
            var_f31 = -1.0f;
        }

        mLinkGndChk.SetPos(&sp14);

        sp14.y = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        bool var_r29_2;
        if (-G_CM3D_F_INF != sp14.y) {
            var_r29_2 = dComIfG_Bgsp().GetTriPla(mLinkGndChk, &sp2C);
        } else {
            var_r29_2 = 0;
        }

        f32 var_f30;
        if (var_r29_2 != 0) {
            var_f30 = sp2C.mNormal.y;
        } else {
            var_f30 = -1.0f;
        }

        cXyz sp8 = sp20 - sp14;
        s16 temp_r3 = sp8.atan2sY_XZ();

        if (cLib_distanceAngleS(temp_r3, shape_angle.x) < 0x1000 && var_f31 > 0.0f && var_f30 > 0.0f && fabsf(var_f31 - var_f30) > 0.05f) {
            shape_angle.x = temp_r3 * field_0x33cc;
        } else {
            shape_angle.x = field_0x2ff0 * field_0x33cc;
        }
    } else {
        shape_angle.x = field_0x2ff0 * field_0x33cc;
    }

    shape_angle.z = field_0x33cc * getGroundAngle(&mLinkAcch.m_gnd, (shape_angle.y - 0x4000));

    if (mProcID == PROC_SLIDE) {
        mBodyAngle.x = -shape_angle.x;
    }
}

void daAlink_c::setStepsOffset() {
    field_0x2b9c = 0.0f;
    cLib_addCalc(&field_0x2b98, 0.0f, 0.5f, 25.0f, 5.0f);

    if (!(fabsf(speedF) < 1.0f)) {
        s16 temp_r3 = getGroundAngle(&mLinkAcch.m_gnd, current.angle.y);
        f32 temp_f30 = cM_ssin(temp_r3) / cM_scos(temp_r3);

        cXyz sp14(current.pos.x + (speedF * cM_ssin(current.angle.y)), current.pos.y + l_autoUpHeight, current.pos.z + (speedF * cM_scos(current.angle.y)));
        mLinkGndChk.SetPos(&sp14);

        f32 temp_f1 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
        f32 temp_f31 = (temp_f1 - current.pos.y) - (-speedF * temp_f30);
        if (temp_f31 > 1.0f) {
            field_0x2b9c = temp_f1 - current.pos.y;
            field_0x2b98 -= 0.7f * temp_f31;
        } else {
            sp14 = field_0x3798 - current.pos;

            temp_f31 = sp14.y - (temp_f30 * sp14.absXZ());
            if (temp_f31 >= 0.0f) {
                field_0x2b98 += 0.7f * temp_f31;
            }
        }

        field_0x2b98 = cLib_minMaxLimit<f32>(field_0x2b98, l_autoDownHeight, l_autoUpHeight);
    }
}

void daAlink_c::iceSlipBgCheck() {
    if (mLinkAcch.ChkGroundHit() && checkModeFlg(1)) {
        f32 var_f30 = field_0x35c4.absXZ();
        if (var_f30 > 0.1f) {
            cXyz sp3C(field_0x3798.x, field_0x3798.y + 10.0f, field_0x3798.z);
            cXyz sp48(current.pos.x, current.pos.y + 10.0f, current.pos.z);

            f32 var_f31 = mAcchCir[0].GetWallR();

            if (checkWolf()) {
                s16 temp_r29 = field_0x35c4.atan2sX_Z() - shape_angle.y;
                var_f31 += fabsf((90.0f - var_f31) * cM_scos(temp_r29));
            }

            var_f31 *= 1.0f / var_f30;

            sp48.x += field_0x35c4.x * var_f31;
            sp48.z += field_0x35c4.z * var_f31;

            if (commonLineCheck(&sp3C, &sp48) && dBgS_CheckBWallPoly(mLinkLinChk)) {
                current.pos.x = mLinkLinChk.GetCross().x - field_0x35c4.x * var_f31;
                current.pos.z = mLinkLinChk.GetCross().z - field_0x35c4.z * var_f31;
            }
        }
    }
}

void daAlink_c::setIceSlipSpeed() {
    field_0x35d0 = field_0x35c4;

    if (!checkModeFlg(0x1210C52)
        && !checkEventRun()
        && !checkHeavyStateOn(TRUE, TRUE)
        && mGndPolySpecialCode == dBgW_SPCODE_ICE
        && !mLinkAcch.ChkWallHit()
        && mLinkAcch.ChkGroundHit())
    {
        f32 var_f31, var_f30, var_f29;
        if (checkWolf()) {
            var_f31 = 0.1f;
            var_f30 = 1.0f;
            var_f29 = 0.5f;
        } else {
            var_f31 = 0.03f;
            var_f30 = 0.5f;
            var_f29 = 0.75f;
        }

        cLib_addCalc(&field_0x35c4.x, 0.0f, var_f31, 100.0f, var_f30);
        cLib_addCalc(&field_0x35c4.z, 0.0f, var_f31, 100.0f, var_f30);

        cXyz sp58 = field_0x3528 - speed;
        f32 var_f28 = sp58.absXZ();
        if (var_f28 > 30.0f) {
            sp58 *= 30.0f / var_f28;
        }

        field_0x35c4 += sp58 * var_f29;
        speed += sp58 * (1.0f - var_f29);

        if (mProcVar4.field_0x3010 != 0 && (mProcID == PROC_LARGE_DAMAGE_UP || mProcID == PROC_WOLF_LARGE_DAMAGE_UP)) {
            sp58.normalizeZP();
            field_0x35c4 += sp58 * 20.0f;
        }
    } else {
        field_0x35c4.x = 0.0f;
        field_0x35c4.z = 0.0f;
    }

    field_0x35c4.y = 0.0f;
}

void daAlink_c::setPolygonSpeed() {
    offNoResetFlg3(daPy_FLG3(FLG3_UNK_1000 | FLG3_UNK_2000));
    if ((!checkEventRun() || checkNoResetFlg0(daPy_FLG0(FLG0_DEMO_STREAM_ACCEPT | FLG0_UNK_4000))) && !checkMagneBootsOn()) {
        cM3dGPla tripla;
        if (checkNoResetFlg3(FLG3_MIDNA_TALK_POLY_SPEED)) {
            offNoResetFlg3(FLG3_MIDNA_TALK_POLY_SPEED);
            field_0x3594 = field_0x35a0;
            field_0x35a0 = cXyz::Zero;
        }

        if ((mLinkAcch.ChkGroundHit() && !checkHeavyStateOn(TRUE, TRUE) && mWaterY > 30.0f + current.pos.y) || checkModeFlg(0x40000)) {
            cXyz sp18;
            int sp8;
            if (fopAcM_getWaterStream(&current.pos, mLinkAcch.m_gnd, &sp18, &sp8, 0)) {
                onNoResetFlg3(FLG3_UNK_2000);
            }

            f32 var_f31;
            f32 var_f30;
            if (checkHeavyStateOn(TRUE, TRUE)) {
                sp18 = cXyz::Zero;
                var_f30 = 0.5f;
            } else if (!checkModeFlg(0x40000)) {
                onNoResetFlg3(FLG3_UNK_1000);

                var_f31 = fabsf(mNormalSpeed / mpHIO->mMove.m.mMaxSpeed);
                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }

                var_f31 *= cM_scos((sp18.atan2sX_Z() - current.angle.y) + 0x8000);
                if (var_f31 < 0.0f) {
                    var_f31 = 0.0f;
                }

                var_f31 *= 0.2f * (f32)sp8;
                if (var_f31 > 0.5f) {
                    var_f31 = 0.5f;
                }

                sp18 *= var_f31 * mNormalSpeed;
                f32 temp_f25 = sp18.abs2();
                if (field_0x3594.abs2() > temp_f25) {
                    var_f30 = 5.0f;
                } else {
                    var_f30 = 3.0f;
                }
            } else {
                sp18 *= (f32)sp8;
                f32 temp_f25 = sp18.abs2();
                if (field_0x3594.abs2() > temp_f25) {
                    var_f30 = 3.0f;
                } else {
                    var_f30 = 1.0f;
                }
            }

            cLib_addCalcPos(&field_0x3594, sp18, 0.5f, var_f30, 0.5f);
        } else if (mGndPolyAtt1 == 4
                    && mLinkAcch.ChkGroundHit()
                    && dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd)
                    && dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &tripla)
                    && tripla.mNormal.y <= cM_scos(cM_deg2s(29.9f)))
        {
            f32 temp_f26 = tripla.mNormal.y;
            f32 temp_f27 = cM_sht2d(cM_atan2s(tripla.mNormal.absXZ(), tripla.mNormal.y)) - 29.9f;
            tripla.mNormal.y = 0.0f;
            tripla.mNormal.normalizeZP();

            f32 sp3C = cM_deg2s(field_0x3122);
            f32 maxSpeed;
            if (temp_f27 < sp3C - 29.9f) {
                if (checkWolf()) {
                    maxSpeed = mpHIO->mWolf.mWlMoveNoP.m.mMaxSpeed;
                } else {
                    maxSpeed = mpHIO->mMove.m.mMaxSpeed;
                }
            } else if (checkWolf()) {
                maxSpeed = mpHIO->mWolf.mWlSlide.m.mClimbingMaxSpeedWeak;
            } else {
                maxSpeed = mpHIO->mSlide.m.mMaxClimbSpeed;
            }

            f32 var_f28 = 0.016887419f * (temp_f27 * temp_f26);
            if (var_f28 > 1.0f) {
                var_f28 = 1.0f;
            }

            tripla.mNormal *= 0.5f + (maxSpeed * var_f28);
            cLib_addCalcPos(&field_0x3594, tripla.mNormal, 0.5f, 1.0f, 0.2f);
        } else {
            field_0x3594 = cXyz::Zero;
        }
    } else {
        field_0x3594 = cXyz::Zero;
    }
}

bool daAlink_c::checkWindSpeedOnAngle() const {
    if (!checkWindSpeedOnXZ()) {
        return false;
    }

    int angle = cLib_distanceAngleS(field_0x30cc, shape_angle.y);
    return angle >= 0x6000 || (angle >= 0x4000 && checkWindDashAnime());
}

bool daAlink_c::checkWindSpeedOnAngleAnime(int param_0) const {
    return checkWindSpeedOnAngle() &&
           ((field_0x2f8c == 1 || field_0x2f8c == 3) || (param_0 != 0 && field_0x2f8c == 2));
}

bool daAlink_c::checkDashAnime() const {
    return checkUnderMove0BckNoArc(ANM_RUN) || checkUnderMove0BckNoArc(ANM_RUN_B);
}

f32 daAlink_c::checkWindWallRate(cXyz const& param_0) {
    cXyz sp24(current.pos.x, current.pos.y + (0.5f * mHeight), current.pos.z);
    cXyz sp18 = sp24 - (param_0 * mpHIO->mBasic.m.mMaxWindInfluenceDist);

    f32 var_f31;
    if (commonLineCheck(&sp24, &sp18) && dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 0xA) {
        var_f31 = (1.0f / (mpHIO->mBasic.m.mMaxWindInfluenceDist - mpHIO->mBasic.m.mNoWindInfluenceDist)) * (sp24.abs(mLinkLinChk.GetCross()) - mpHIO->mBasic.m.mNoWindInfluenceDist);
        if (var_f31 < 0.0f) {
            var_f31 = 0.0f;
        }
    } else {
        var_f31 = 1.0f;
    }

    return var_f31;
}

void daAlink_c::setWindSpeed() {
    if (!checkModeFlg(0x40000) && !checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        cXyz sp3C;
        f32 var_f31;
        f32 sp8;

        if (mWindTgCyls[0].ChkTgHit() || mWindTgCyls[1].ChkTgHit() || mWindTgCyls[2].ChkTgHit()) {
            int i = 0;
            for (; i < 3; i++) {
                if (mWindTgCyls[i].ChkTgHit()) {
                    break;
                }
            }

            sp3C = *mWindTgCyls[i].GetTgRVecP();
            f32 temp_f1 = sp3C.absXZ();

            if (temp_f1 < 1.0f && !checkGrabGlide()) {
                if (mWindTgCyls[i].GetTgHitAc() != NULL) {
                    sp3C = current.pos - mWindTgCyls[i].GetTgHitAc()->current.pos;
                } else {
                    sp3C = current.pos - *mWindTgCyls[i].GetTgHitPosP();
                }

                sp3C.y = 0.0f;
                sp3C.normalizeZP();
                sp3C *= mWindTgCyls[i].GetTgRVecP()->abs();
            }

            sp8 = sp3C.abs();

            if (temp_f1 >= 1.0f) {
                sp3C.normalizeZP();
                sp8 *= checkWindWallRate(sp3C);
                sp3C *= sp8;
            }

            if (mWindSpeed.abs2() > sp8 * sp8) {
                var_f31 = 4.0f;
            } else {
                var_f31 = 2.5f;
            }
        } else {
            dKyw_get_AllWind_vec(&current.pos, &sp3C, &sp8);

            if (dKy_TeachWind_existence_chk() == 0 || sp8 < 0.3f) {
                sp8 = 0.0f;
            } else if (sp8 > 0.0f && dKy_TeachWind_existence_chk() != -1) {
                sp8 *= checkWindWallRate(sp3C);
            }

            sp8 *= mpHIO->mBasic.m.mMaxWindSpeed;
            sp3C *= sp8;

            if (mWindSpeed.abs2() > sp8 * sp8) {
                var_f31 = 3.0f;
            } else {
                var_f31 = 1.0f;
            }
        }

        cLib_addCalcPos(&field_0x35b8, sp3C, 0.5f, var_f31, 0.5f);
        field_0x30cc = field_0x35b8.atan2sX_Z();

        if (!checkHeavyStateOn(TRUE, TRUE) && !checkNoCollisionCorret() && !checkEventRun() && !checkModeFlg(0x10)) {
            cLib_addCalcPos(&mWindSpeed, sp3C, 0.5f, var_f31, 0.5f);
        } else {
            mWindSpeed = cXyz::Zero;
        }
    } else {
        mWindSpeed = cXyz::Zero;
        field_0x35b8 = cXyz::Zero;
        field_0x30cc = 0;
    }
}

void daAlink_c::setBodyPartPos() {
    static Vec const localHeadCenter = {0.0f, -8.0f, 0.0f};
    static Vec const localEye = {12.0f, -8.0f, 0.0f};
    static Vec const localHeadTop = {0.0f, -28.0f, 0.0f};
    static Vec const wlLocalHeadCenter = {0.0f, -5.0f, 0.0f};
    static Vec const wlLocalEye = {30.0f, -5.0f, 0.0f};
    static Vec const wlLocalHeadTop = {0.0f, -20.0f, 0.0f};
    static Vec const localEyeFromRoot = {0.0f, 55.75f, 15.0f};
    static Vec const boardLocalEyeFromRoot = {9.5f, 47.0f, 24.5f};
    static Vec const horseLocalEyeFromRoot = {1.75f, 55.0f, 25.5f};
    static Vec const canoeLocalEyeFromRoot = {0.0f, 55.0f, 25.0f};

    if (checkWolf()) {
        cMtx_multVec(mpLinkModel->getAnmMtx(4), &wlLocalEye, &eyePos);
        field_0x3768 = eyePos;
        cMtx_multVec(mpLinkModel->getAnmMtx(4), &wlLocalHeadCenter, &field_0x34e0);
        cMtx_multVec(mpLinkModel->getAnmMtx(4), &wlLocalHeadTop, &mHeadTopPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x13), &mLeftHandPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x18), &mRightHandPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x1F), &mLeftFootPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x24), &mRightFootPos);
    } else {
        cMtx_multVec(mpLinkModel->getAnmMtx(field_0x30b4), &localEye, &eyePos);

        if (dComIfGp_checkPlayerStatus0(0, 0x2000) && !dComIfGp_checkPlayerStatus1(0, 0x02010000) && (!checkModeFlg(0x40000) || !checkNoResetFlg0(FLG0_SWIM_UP)) && !dComIfGp_checkPlayerStatus0(0, 0x08000000)) {
            mDoMtx_stack_c::transS(current.pos.x, field_0x3834.y, current.pos.z);
            concatMagneBootMtx();
            mDoMtx_stack_c::ZXYrotM(field_0x310a, field_0x310c, 0);

            if (checkCanoeRide()) {
                mDoMtx_stack_c::multVec(&canoeLocalEyeFromRoot, &field_0x3768);
            } else if (checkBoardRide()) {
                mDoMtx_stack_c::multVec(&boardLocalEyeFromRoot, &field_0x3768);
            } else if (checkReinRide()) {
                mDoMtx_stack_c::multVec(&horseLocalEyeFromRoot, &field_0x3768);
            } else {
                mDoMtx_stack_c::multVec(&localEyeFromRoot, &field_0x3768);
            }
        } else {
            field_0x3768 = eyePos;
        }

        cMtx_multVec(mpLinkModel->getAnmMtx(field_0x30b4), &localHeadCenter, &field_0x34e0);
        cMtx_multVec(mpLinkModel->getAnmMtx(field_0x30b4), &localHeadTop, &mHeadTopPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(mLeftHandJntNo), &mLeftHandPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(mRightHandJntNo), &mRightHandPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(field_0x30bc), &mLeftFootPos);
        mDoMtx_multVecZero(mpLinkModel->getAnmMtx(field_0x30be), &mRightFootPos);
    }

    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0), &field_0x3834);
    mItemPos = mHeadTopPos;
    mViewerCurrentPos = current.pos;

    if (checkRootTransClearMode()) {
        Vec sp8;

        if (checkRootTransZClearMode()) {
            sp8.z = field_0x34d4.z - field_0x3588.z;
        } else {
            sp8.z = 0.0f;
        }

        if (checkRootTransXClearMode()) {
            sp8.x = field_0x34d4.x - field_0x3588.x;
        } else {
            sp8.x = 0.0f;
        }

        if (checkRootTransYClearMode()) {
            sp8.y = field_0x33b0 - field_0x3588.y;
        } else {
            sp8.y = 0.0f;
        }

        mViewerCurrentPos.set((current.pos.x - (sp8.z * cM_ssin(shape_angle.y))) - sp8.x * cM_scos(shape_angle.y), current.pos.y - sp8.y, (current.pos.z - (sp8.z * cM_scos(shape_angle.y))) + sp8.x * cM_ssin(shape_angle.y));
    }
}

int daAlink_c::setRollJump(f32 i_speedH, f32 i_speedV, s16 i_angle) {
    mRollJumpSpeedH = i_speedH;
    mRollJumpSpeedV = i_speedV;
    mRollJumpAngle = i_angle;
    onEndResetFlg0(ERFLG0_UNK_100);
    return 1;
}

void daAlink_c::setAttentionPos() {
    static Vec const wlLocalEyeFromRoot = {0.0f, -6.0f, 85.5f};
    static Vec const normalOffset = {0.0f, 150.0f, 0.0f};
    static Vec const crawlOffset = {0.0f, 40.0f, 30.0f};
    static Vec const crouchOffset = {0.0f, 95.0f, 0.0f};
    static Vec const swimOffset = {0.0f, 14.0f, 0.0f};

    if (mProcID == PROC_METAMORPHOSE) {
        attention_info.position = field_0x34e0;
    } else if (checkModeFlg(0x400)) {
        fopAc_ac_c* rideActor = mRideAcKeep.getActor();

        if (mProcID == PROC_CANOE_RIDE || mProcID == PROC_CANOE_GETOFF || (mProcID == PROC_CANOE_WAIT && mProcVar0.field_0x3008 != 0)) {
            attention_info.position = field_0x34e0;
        } else if (checkSpinnerRide() || checkBoardRide()) {
            attention_info.position.set(current.pos.x, 150.0f + current.pos.y, current.pos.z);
        } else if (rideActor != NULL) {
            if (checkCanoeRide()) {
                f32 var_f31 = l_canoeBaseAnime.z;
                if (((daCanoe_c*)rideActor)->checkTandem()) {
                    var_f31 += 175.0f;
                }

                attention_info.position.set(rideActor->current.pos.x + (var_f31 * cM_ssin(shape_angle.y)), 50.0f + (rideActor->current.pos.y + l_canoeBaseAnime.y), rideActor->current.pos.z + (var_f31 * cM_scos(shape_angle.y)));
            } else {
                attention_info.position.set(rideActor->current.pos.x, 275.0f + rideActor->current.pos.y, rideActor->current.pos.z);
            }
        } else {
            attention_info.position.set(current.pos.x, 50.0f + current.pos.y, current.pos.z);
        }
    } else if (checkWolf()) {
        if (checkModeFlg(0x10000000)) {
            attention_info.position = field_0x34e0;
        } else {
            Vec sp8;
            sp8.x = 0.0f;
            sp8.z = 70.0f;

            if (checkModeFlg(0x40000) || mProcID == PROC_LAVA_RETURN) {
                sp8.y = 20.0f;
            } else if (checkModeFlg(0x01000000)) {
                sp8.y = 50.0f;
            } else {
                sp8.y = 80.0f;
            }

            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &sp8, &attention_info.position);

            if (checkModeFlg(0x40000)) {
                attention_info.position.y = current.pos.y + sp8.y;
            }
        }
    } else if (mProcID == PROC_HOOKSHOT_FLY) {
        attention_info.position = eyePos;
    } else if (dComIfGp_checkPlayerStatus1(0, 0x02000000)) {
        attention_info.position.set(current.pos.x + (100.0f * cM_ssin(shape_angle.y)), -50.0f + current.pos.y, current.pos.z + (100.0f * cM_scos(shape_angle.y)));
    } else if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
        attention_info.position.set(current.pos.x, -100.0f + current.pos.y, current.pos.z);
    } else if (checkModeFlg(0x800)) {
        attention_info.position = current.pos;
    } else if (checkModeFlg(0x40) || mProcID == PROC_BOSS_BODY_HANG) {
        mDoMtx_multVecZero(mpLinkHatModel->getBaseTRMtx(), &attention_info.position);
    } else if (mProcID == PROC_ROLL_JUMP) {
        mDoMtx_multVecZero(mpLinkModel->getBaseTRMtx(), &attention_info.position);
        attention_info.position.y += 150.0f;
    } else {
        const Vec* offset;
        f32 pos_y;
        if (checkModeFlg(0x800000)) {
            offset = &crouchOffset;
            pos_y = current.pos.y;
        } else if (checkModeFlg(0x40000)) {
            offset = &swimOffset;
            pos_y = current.pos.y;
        } else {
            if (mProcID == PROC_LAVA_RETURN) {
                offset = &swimOffset;
            } else if (checkModeFlg(0x01000000)) {
                offset = &crawlOffset;
            } else {
                offset = &normalOffset;
            }

            pos_y = mpLinkModel->getBaseTRMtx()[1][3];
        }

        cMtx_multVecSR(mpLinkModel->getBaseTRMtx(), offset, &attention_info.position);
        attention_info.position.x += current.pos.x;
        attention_info.position.y += pos_y;
        attention_info.position.z += current.pos.z;

        if (mProcID == PROC_CLIMB_MOVE_SIDE) {
            attention_info.position.y = field_0x347c;
        } else if (mProcID == PROC_SWIM_DIVE) {
            attention_info.position.y = (field_0x3478 * attention_info.position.y) + (eyePos.y * (1.0f - field_0x3478));
        }
    }

    if (mSinkShapeOffset < field_0x3458) {
        attention_info.position.y += field_0x3458 - mSinkShapeOffset;
    }
}

void daAlink_c::setMatrix() {
    f32 var_f31;
    if (mGroundCode == 8 && mLinkAcch.ChkGroundHit() && !checkModeFlg(0x70C52)) {
        var_f31 = 20.0f;
    } else {
        var_f31 = 0.0f;
    }

    cLib_addCalc(&field_0x2ba0, var_f31, 0.5f, 12.0f, 3.0f);

    f32 var_f30;
    if (checkBoardRide()) {
        var_f30 = 13.0f;
    } else {
        var_f30 = 0.0f;
    }

    mDoMtx_stack_c::transS(current.pos.x, field_0x2ba0 + (field_0x2b9c + (var_f30 + (current.pos.y + field_0x2b98))), current.pos.z);
    concatMagneBootMtx();
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, (shape_angle.y + field_0x308c), shape_angle.z);

    if (mProcID == PROC_BOAR_RUN || mProcID == PROC_ZORA_MOVE) {
        mDoMtx_stack_c::transM(0.0f, 50.0f, 100.0f);
        mDoMtx_stack_c::ZXYrotM(mProcVar4.field_0x3010, mProcVar5.field_0x3012, (1.5f * mProcVar5.field_0x3012));
        mDoMtx_stack_c::transM(0.0f, -50.0f, -100.0f);
    } else if (mProcID == PROC_HORSE_RUN) {
        mDoMtx_stack_c::transM(0.0f, 20.0f, 70.0f);
        mDoMtx_stack_c::ZXYrotM(mProcVar4.field_0x3010, mProcVar5.field_0x3012, (0.5f * mProcVar5.field_0x3012));
        mDoMtx_stack_c::transM(0.0f, -20.0f, -70.0f);
    } else if (mProcID == PROC_WOLF_CHAIN_WAIT) {
        mDoMtx_stack_c::transM(0.0f, 5.0f, -3.0f);
    }

    if (mProcID == PROC_MAGNE_BOOTS_FLY) {
        mDoMtx_stack_c::YrotM(mProcVar2.field_0x300c);
    }

    mpLinkModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_inverse(mDoMtx_stack_c::get(), mInvMtx);
    mDoMtx_stack_c::XrotS(shape_angle.x);
    mDoMtx_stack_c::concat(mInvMtx);
    mDoMtx_copy(mDoMtx_stack_c::get(), field_0x2be8);

    field_0x2fb6 = 0;

    if (!checkWolf()) {
        if (checkKandelaarEquipAnime()
            || checkSwordTwirlAnime()
            || checkNoResetFlg0(FLG0_UNK_1000000)
            || (checkModeFlg(0x10000) && checkEquipAnime())
            || (checkBoardRide() && checkSwordEquipAnime())
            || (checkUpperGuardAnime() && checkEquipAnime())
            || (checkKandelaarSwingAnime() && mProcID != PROC_KANDELAAR_SWING && mTargetedActor == NULL && !checkModeFlg(0x400))
            )
        {
            field_0x2fb6 = 5;
        } else if (checkReelAnime()) {
            field_0x2fb6 = 4;
        } else if (checkEquipAnime() && !checkModeFlg(1)) {
            if (checkSwordEquipAnime() && !checkShieldGet()) {
                field_0x2fb6 = 5;
            } else {
                field_0x2fb6 = 1;
            }
        } else if (checkAnmEnd(&mUnderFrameCtrl[0]) && (checkUnderMove0BckNoArc(ANM_HORSE_TURN_LEFT) || checkUnderMove0BckNoArc(ANM_HORSE_TURN_B) || checkUnderMove0BckNoArc(ANM_HORSE_TURN_RIGHT))) {
            field_0x2fb6 = 6;
        } else if (checkCokkoGlide()) {
            field_0x2fb6 = 8;
        } else if ((mProcID == PROC_SLIDE && field_0x3198 != 0) || (checkWindDashAnime() && checkUnderMove1BckNoArc(ANM_WALK_HEAVY) && (checkModeFlg(1) || checkZeroSpeedF()))) {
            field_0x2fb6 = 7;
        } else if (checkUpperAnime(dRes_ID_ALANM_BCK_CUTHTP_e) || checkCutDashChargeAnime() || (checkReinRide() && (checkUpperReadyThrowAnime() || checkGrabAnime()))) {
            field_0x2fb6 = 3;
        } else if (checkSwordEquipAnime() && !checkShieldGet()) {
            field_0x2fb6 = 2;
        } else if (checkIronBallWaitAnime()) {
            if (checkUnderMove0BckNoArc(ANM_IRONBALL_WAIT)) {
                mNowAnmPackUpper[2].setRatio(0.0f);
            } else {
                mNowAnmPackUpper[2].setRatio(1.0f);
            }
        }

        if (field_0x2fb6 != 0) {
            field_0x3444 = mNowAnmPackUpper[2].getRatio();
        }
    }
}

int daAlink_c::simpleAnmPlay(J3DAnmBase* i_anm) {
    if (i_anm == NULL) {
        return 0;
    }

    BOOL ret = FALSE;
    f32 frame = i_anm->getFrame() + 1.0f;

    if (frame >= i_anm->getFrameMax()) {
        if (i_anm->getAttribute() == 2) {
            frame -= i_anm->getFrameMax();
        } else {
            frame = i_anm->getFrameMax();
            ret = TRUE;
        }
    }

    i_anm->setFrame(frame);
    return ret;
}

void daAlink_c::setSwordPos() {
    static Vec const swordMoveLocal0 = {0.0f, 0.0f, -1.0f};
    static Vec const swordMoveLocal1 = {0.0f, 0.0f, 1.0f};
    static Vec const copyRodTop = {27.5f, -5.0f, -12.5f};

    field_0x34b0 = mSwordTopPos;
    field_0x34bc = field_0x3498;
    mDoMtx_multVecZero(mSwordModel->getBaseTRMtx(), &field_0x3498);

    if (mCutType == CUT_TYPE_TWIRL || mCutType == CUT_TYPE_FINISH_RIGHT) {
        mDoMtx_multVecSR(mSwordModel->getBaseTRMtx(), &swordMoveLocal1, &field_0x34a4);
    } else {
        mDoMtx_multVecSR(mSwordModel->getBaseTRMtx(), &swordMoveLocal0, &field_0x34a4);
    }

    if (checkMasterSwordEquip()) {
        mDoMtx_multVec(mSwordModel->getBaseTRMtx(), &l_swordTopLocalM, &mSwordTopPos);
    } else {
        mDoMtx_multVec(mSwordModel->getBaseTRMtx(), &l_swordTopLocalN, &mSwordTopPos);
    }

    if (mEquipItem == 0x10B) {
        mDoMtx_multVec(getCanoePaddleMatrix(), getCanoeLocalPaddleTop(), &field_0x3720);
    } else if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        mDoMtx_multVec(mHeldItemModel->getBaseTRMtx(), &copyRodTop, &field_0x3720);
    } else {
        field_0x3720 = mSwordTopPos;
    }
}

void daAlink_c::setItemMatrix(int param_0) {
    mSheathModel->setBaseTRMtx(mpLinkModel->getAnmMtx(field_0x30b6));
    modelCalc(mSheathModel);

    int var_r26;
    if (!checkNoResetFlg3(FLG3_UNK_4000000)) {
        if (mEquipItem == 0x103 || param_0 != 0) {
            mSwordModel->setBaseTRMtx(mpLinkModel->getAnmMtx(mLeftItemJntNo));
            var_r26 = 1;
        } else {
            mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30b6));
            mDoMtx_stack_c::transM(-18.5f, 0.14f, 12.2f);
            mDoMtx_stack_c::XYZrotM(0, cM_deg2s(33.1f), 0);
            mSwordModel->setBaseTRMtx(mDoMtx_stack_c::get());
            var_r26 = 2;
        }

        modelCalc(mSwordModel);
    }

    int var_r28;
    if (!checkSwordGet()) {
        var_r28 = 0;
    } else if (checkLightMasterSwordEquip()) {
        var_r28 = 3;
    } else if (checkMasterSwordEquip()) {
        var_r28 = 2;
    } else {
        var_r28 = 1;
    }

    mZ2Link.setLinkSwordType(var_r28, var_r26);

    if (mShieldChangeWaitTimer == 0) {
        if (param_0 != 0
            || (checkPlayerGuardAndAttack() && mEquipItem != fpcNm_ITEM_IRONBALL && !checkModeFlg(0x400))
            || checkNoResetFlg0(FLG0_UNK_2)
            || (mProcID == PROC_TOOL_DEMO && mProcVar4.field_0x3010 != 0)
            || (mProcID == PROC_CUT_REVERSE && mProcVar2.field_0x300c != 0)
            || mProcID == PROC_GUARD_BREAK
            || (mEquipItem == 0x103 && !checkEndResetFlg1(ERFLG1_SHIELD_BACKBONE) && !checkModeFlg(0x400))
            ) 
        {
            mShieldModel->setBaseTRMtx(mpLinkModel->getAnmMtx(mRightItemJntNo));

            if (checkShieldGet()) {
                field_0x2e44.offPassNum(0xF);
                mDoAud_setLinkShieldType(1, 1);
            } else {
                field_0x2e44.onPassNum(0xF);
                mDoAud_setLinkShieldType(0, 2);
            }

            field_0x2e44.onPassNum(0x10);

            for (int i = 0; i < 16; i++) {
                field_0x2f7c[i] = 0;
            }
        } else {
            mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(field_0x30b6));
            mDoMtx_stack_c::transM(4.2f, -4.4f, -20.0f);
            mDoMtx_stack_c::XYZrotM(cM_deg2s(91.0f), cM_deg2s(57.0f), cM_deg2s(180.0f));
            mShieldModel->setBaseTRMtx(mDoMtx_stack_c::get());

            if (!checkShieldGet()) {
                mDoAud_setLinkShieldType(0, 2);
                field_0x2e44.onPassNum(0x10);
            } else {
                field_0x2e44.offPassNum(0x10);
                mDoAud_setLinkShieldType(1, 2);
            }

            field_0x2e44.onPassNum(0xF);
            stickArrowIncrement(0);
            clearWoodShieldBurnEffect();
        }

        modelCalc(mShieldModel);
    }

    mpLinkFaceModel->setBaseTRMtx(mpLinkModel->getAnmMtx(4));
    modelCalc(mpLinkFaceModel);
    mpLinkHatModel->setBaseTRMtx(mpLinkModel->getAnmMtx(4));

    if ((mpDemoHDTmpBck != NULL) && mpLinkHatModel != NULL && mClothesChangeWaitTimer == 0) {
        if (mpDemoHDTmpBck->getBckAnm() != NULL) {
            mpDemoHDTmpBck->entry(mpLinkHatModel->getModelData());
        } else {
            mpDemoHDTmpBck->remove(mpLinkHatModel->getModelData());
        }
    }

    modelCalc(mpLinkHatModel);

    if (checkEquipHeavyBoots()) {
        for (int i = 0; i < 2; i++) {
            mpLinkBootModels[i]->setBaseTRMtx(mpLinkModel->getBaseTRMtx());
            modelCalc(mpLinkBootModels[i]);
        }

        mpLinkBootModels[0]->setAnmMtx(1, mpLinkModel->getAnmMtx(0x13));
        mpLinkBootModels[0]->setAnmMtx(2, mpLinkModel->getAnmMtx(0x14));
        mpLinkBootModels[0]->setAnmMtx(3, mpLinkModel->getAnmMtx(0x15));

        mDoMtx_stack_c::XrotS(-0x8000);
        mDoMtx_concat(mpLinkModel->getAnmMtx(0x18), mDoMtx_stack_c::get(), mpLinkBootModels[1]->getAnmMtx(1));
        mDoMtx_concat(mpLinkModel->getAnmMtx(0x19), mDoMtx_stack_c::get(), mpLinkBootModels[1]->getAnmMtx(2));
        mDoMtx_concat(mpLinkModel->getAnmMtx(0x1A), mDoMtx_stack_c::get(), mpLinkBootModels[1]->getAnmMtx(3));
    }

    if (!checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW)) {
        if (mpDemoFCBlendModel != NULL) {
            mpDemoFCBlendModel->setBaseTRMtx(mpLinkModel->getAnmMtx(4));
            mpDemoFCBlendModel->calc();
        }

        if (mpDemoFCTongueModel != NULL) {
            mpDemoFCTongueModel->setBaseTRMtx(mpLinkModel->getAnmMtx(4));
            if (mpDemoFCTmpBck != NULL) {
                mpDemoFCTmpBck->entry(mpDemoFCTongueModel->getModelData());
            }
            mpDemoFCTongueModel->calc();
        }

        if (mHeldItemModel != NULL) {
            if (mEquipItem == 0x106) {
                mHeldItemModel->setBaseTRMtx(mpLinkModel->getAnmMtx(4));
            } else if (checkOilBottleItemNotGet(mEquipItem)) {
                mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(mRightItemJntNo));
                mDoMtx_stack_c::transM(1.5f, -7.5f, -1.0f);
                mDoMtx_stack_c::XYZrotM(cM_deg2s(183.0f), cM_deg2s(176.0f), cM_deg2s(167.0f));
                mHeldItemModel->setBaseTRMtx(mDoMtx_stack_c::get());
            } else if (checkBottleItem(mEquipItem)) {
                mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(mLeftItemJntNo));
                mDoMtx_stack_c::transM(-10.0f,-0.5f, -5.5f);
                mDoMtx_stack_c::XYZrotM(cM_deg2s(174.0f), cM_deg2s(-47.0f), cM_deg2s(94.0f));
                mHeldItemModel->setBaseTRMtx(mDoMtx_stack_c::get());

                if (mpHookTipModel != NULL) {
                    simpleAnmPlay(mHookTipBck.getBckAnm());
                    mpHookTipModel->setBaseTRMtx(mDoMtx_stack_c::get());
                    mpHookTipModel->calc();
                }
            } else if (checkBowAndSlingItem(mEquipItem)) {
                if (checkBowGrabLeftHand()) {
                    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(mLeftItemJntNo));
                    mDoMtx_stack_c::transM(-1.3f, 0.0f, -3.0f);
                    mDoMtx_stack_c::XYZrotM(cM_deg2s(-74.0f), cM_deg2s(43.6), cM_deg2s(1.9f));
                    mHeldItemModel->setBaseTRMtx(mDoMtx_stack_c::get());
                } else {
                    mHeldItemModel->setBaseTRMtx(mpLinkModel->getAnmMtx(mRightItemJntNo));
                }
            } else if (checkHookshotItem(mEquipItem)) {
                setHookshotPos();
            } else if (mEquipItem == fpcNm_ITEM_IRONBALL) {
                setIronBallPos();
            } else {
                if (mEquipItem == fpcNm_ITEM_COPY_ROD && mProcID != PROC_COPY_ROD_REVIVE) {
                    if (checkCopyRodTopUse()) {
                        field_0x0724->setFrame(field_0x0724->getFrameMax() - 0.001f);
                    } else {
                        field_0x0724->setFrame(0.0f);
                    }
                }

                mHeldItemModel->setBaseTRMtx(mpLinkModel->getAnmMtx(mLeftItemJntNo));
            }

            if (mItemBck.getBckAnm() != NULL) {
                if (field_0x33dc >= mItemBck.getBckAnm()->getFrameMax()) {
                    field_0x33dc = mItemBck.getBckAnm()->getFrameMax() - 0.001f;
                }

                mItemBck.entry(mHeldItemModel->getModelData(), field_0x33dc);
            }

            mHeldItemModel->calc();
        }

        if (checkNoResetFlg2(FLG2_UNK_1) || checkEndResetFlg1(ERFLG1_UNK_4)) {
            simpleAnmPlay(mpKanteraGlowBtk);

            if (mProcID != PROC_OPEN_TREASURE && !checkEndResetFlg1(ERFLG1_UNK_4) && (mProcID != PROC_GET_ITEM || mProcVar4.field_0x3010 == 0)) {
                if (mEquipItem == fpcNm_ITEM_KANTERA || checkOilBottleItemNotGet(mEquipItem)) {
                    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(mLeftItemJntNo));
                    mDoMtx_stack_c::transM(-2.0f, -0.1f, -0.7f);
                    mDoMtx_stack_c::XYZrotM(cM_deg2s(100.0f), cM_deg2s(9.3f), cM_deg2s(183.0f));
                    mpKanteraModel->setBaseTRMtx(mDoMtx_stack_c::get());
                } else {
                    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(0x10));
                    mDoMtx_stack_c::transM(-1.0f, 4.5f, 9.0f);
                    mDoMtx_stack_c::XYZrotM(cM_deg2s(-75.0f), cM_deg2s(62.0f), cM_deg2s(89.0f));
                    mpKanteraModel->setBaseTRMtx(mDoMtx_stack_c::get());
                }
            }

            modelCalc(mpKanteraModel);
            mDoMtx_stack_c::transS(mKandelaarFlamePos);
            mpKanteraGlowModel->setBaseTRMtx(mDoMtx_stack_c::get());
            modelCalc(mpKanteraGlowModel);
        }

        setSwordPos();
        simpleAnmPlay(m_mSwordBtk);
        simpleAnmPlay(m_mSwordBrk);
        setHorseStirrup();
    }
}

void daAlink_c::setWolfItemMatrix() {
    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(2));
    mDoMtx_stack_c::transM(31.0f, -29.0f, 19.0f);
    mDoMtx_stack_c::XYZrotM(0, cM_deg2s(32.0f), cM_deg2s(157.0f));
    mSwordModel->setBaseTRMtx(mDoMtx_stack_c::get());
    modelCalc(mSwordModel);

    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(2));
    mDoMtx_stack_c::transM(13.0f, -21.0f, 7.0f);
    mDoMtx_stack_c::XYZrotM(0, cM_deg2s(-0.8f), cM_deg2s(157.0f));
    mSheathModel->setBaseTRMtx(mDoMtx_stack_c::get());
    modelCalc(mSheathModel);

    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(2));
    mDoMtx_stack_c::transM(11.0f, -18.0f, -13.0f);
    mDoMtx_stack_c::XYZrotM(cM_deg2s(90.0f), cM_deg2s(58.0f), cM_deg2s(-24.0f));

    if (mShieldChangeWaitTimer == 0) {
        mShieldModel->setBaseTRMtx(mDoMtx_stack_c::get());
        modelCalc(mShieldModel);
    }

    if (!checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW)) {
        setSwordPos();

        if (mHeldItemModel != NULL) {
            if (mEquipItem == 0x109) {
                f32 temp_f31 = 0.01f * mSearchBallScale;
                Vec sp1C = {temp_f31, temp_f31, temp_f31};

                mDoMtx_trans(mHeldItemModel->getBaseTRMtx(), current.pos.x, current.pos.y, current.pos.z);
                mHeldItemModel->setBaseScale(sp1C);

                if (field_0x0718 != NULL) {
                    simpleAnmPlay(field_0x0718);
                }
            } else {
                mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(4));
                mDoMtx_stack_c::transM(28.0f, 14.0f, 0.0f);
                mDoMtx_copy(mDoMtx_stack_c::get(), mHeldItemModel->getBaseTRMtx());
                mIronBallCenterPos = mHookshotTopPos;
                mHookshotTopPos = mHeldItemRootPos;
                mDoMtx_stack_c::multVecZero(&mHeldItemRootPos);

                simpleAnmPlay(field_0x0718);

                mpHookSound->startLevelSound(Z2SE_EN_PO_SOUL, 0, -1);
                mpHookSound->framework(0, mVoiceReverbIntensity);

                if (mProcID != PROC_WOLF_DOWN_AT_LAND) {
                    field_0x32d4 = dComIfGp_particle_set(field_0x32d4, dPa_RM(ID_ZI_S_ID_SETB_D), &mHeldItemRootPos, &tevStr, NULL, NULL, 0xFF, &field_0x2f38, -1, NULL, NULL, NULL);
                }
            }

            mHeldItemModel->calc();
        }

        setWolfChainPos();
    }
}

void daAlink_c::setHandIndex(daAlink_c::daAlink_ANM i_anmID) {
    mLeftHandIndex = getAnmData(i_anmID)->m_handIndexL;
    mRightHandIndex = getAnmData(i_anmID)->m_handIndexR;
}

void daAlink_c::setSwordAtCollision(int param_0) {
    cXyz work;

    if (checkCutDashAnime() && (checkCutDashEnemyHit(mAtCps[0]) ||
                                checkCutDashEnemyHit(mAtCps[1]) || checkCutDashEnemyHit(mAtCps[2])))
    {
        onNoResetFlg1(FLG1_UNK_4000000);
    }

    if (mProcID == PROC_CUT_FINISH_JUMP_UP) {
        work.set(current.pos.x + mpHIO->mCut.mCutFinishJumpUppercut.m.mAttackOffset * cM_ssin(shape_angle.y),
                 field_0x3478,
                 current.pos.z + mpHIO->mCut.mCutFinishJumpUppercut.m.mAttackOffset * cM_scos(shape_angle.y));
        f32 height = mSwordTopPos.y - field_0x3478;

        if (height < 10.0f) {
            height = 10.0f;
        }

        mAtCyl.SetH(height);

        if (param_0 != 0) {
            mAtCyl.StartCAt(work);
        } else {
            mAtCyl.MoveCAt(work);
        }

        dComIfG_Ccsp()->Set(&mAtCyl);
        dComIfG_Ccsp()->SetMass(&mAtCyl, 1);
        return;
    }

    work = ((mSwordTopPos - field_0x3498) * field_0x33d0) + field_0x3498;
    cXyz sp68 = ((field_0x34b0 - field_0x34bc) * field_0x33d0) + field_0x34bc;

    mAtCps[0].SetStartEnd(field_0x3498, work);
    mAtCps[1].SetStartEnd(sp68, work);
    mAtCps[2].SetStartEnd(field_0x3498, sp68);

    cXyz sp5C;
    mAtCps[0].OnAtSetBit();

    if (param_0 != 0) {
        sp5C = cXyz::Zero;
    } else {
        sp5C = mSwordTopPos - field_0x34b0;
        mAtCps[1].OnAtSetBit();
        mAtCps[2].OnAtSetBit();
    }

    mAtCps[0].SetAtVec(sp5C);
    mAtCps[1].SetAtVec(sp5C);
    mAtCps[2].SetAtVec(sp5C);

    for (int i = 0; i < 3; i++) {
        dComIfG_Ccsp()->Set(&mAtCps[i]);
        dComIfG_Ccsp()->SetMass(&mAtCps[i], 1);
    }
}

BOOL daAlink_c::checkNoCollisionCorret() {
    if (checkModeFlg(0x12800)
        || mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_TOOL_e
        || mProcID == PROC_DOOR_OPEN
        || (checkEventRun() &&
                (strcmp(dComIfGp_getEventManager().getRunEventName(), "SCENE_EXIT") == 0
                 || (fopAcM_getTalkEventPartner(this) && fopAcM_getTalkEventPartner(this) == getMidnaActor())
                )
            )
        || eventInfo.checkCommandDoor())
    {
        return true;
    }

    return false;
}

void daAlink_c::decSwordBlur() {
    if (m_swordBlur.field_0x14 < 10) {
        m_swordBlur.field_0x14 = 0;
    } else {
        m_swordBlur.field_0x14 -= 10;
        m_swordBlur.traceBlur(&current.pos, &old.pos, shape_angle.y - mPrevAngleY);
    }
}

void daAlink_c::resetWolfAtCollision() {
    if (checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
        if (!setSwordHitVibration(&mAtCyl) && !setSwordHitVibration(&mTgCyls[0]) &&
            !setSwordHitVibration(&mTgCyls[1]))
        {
            setSwordHitVibration(&mTgCyls[2]);
        }

        mAtSph.OffTgWolfSpNoDamage();
        mAtCyl.ResetAtHit();
        offNoResetFlg0(FLG0_CUT_AT_FLG);

        dCcD_Cyl* cyl = mTgCyls;
        for (int i = 0; i < 3; i++, cyl++) {
            cyl->OffAtSetBit();
            cyl->ResetAtHit();
            cyl->OffTgWolfSpNoDamage();
        }
    }
}

void daAlink_c::setWolfAtCollision() {
    int i;

    if (checkResetFlg0(RFLG0_UNK_2)) {
        if (!setSwordHitVibration(&mAtCyl) && !setSwordHitVibration(&mTgCyls[0]) &&
            !setSwordHitVibration(&mTgCyls[1]))
        {
            setSwordHitVibration(&mTgCyls[2]);
        }

        if (mProcID == PROC_WOLF_ROLL_ATTACK) {
            mAtCyl.SetC(current.pos);

            dComIfG_Ccsp()->Set(&mAtCyl);
            dComIfG_Ccsp()->SetMass(&mAtCyl, 1);
        } else {
            cXyz sp8(current.pos.x + field_0x3438 * cM_ssin(shape_angle.y), current.pos.y,
                     current.pos.z + field_0x3438 * cM_scos(shape_angle.y));

            if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
                mAtCyl.StartCAt(sp8);

                if (mProcID == PROC_WOLF_LOCK_ATTACK || mProcID == PROC_WOLF_JUMP_ATTACK) {
                    mAtSph.OnTgWolfSpNoDamage();

                    for (i = 0; i < 3; i++) {
                        mTgCyls[i].OnTgWolfSpNoDamage();
                    }
                }

                if (mCutType == CUT_TYPE_WOLF_JUMP_S_FINISH || mProcID == PROC_WOLF_LOCK_ATTACK) {
                    for (i = 0; i < 3; i++) {
                        mTgCyls[i].OnAtSetBit();
                    }
                }
            } else {
                mAtCyl.MoveCAt(sp8);
            }

            dComIfG_Ccsp()->Set(&mAtCyl);
            dComIfG_Ccsp()->SetMass(&mAtCyl, 1);
        }

        onNoResetFlg0(FLG0_CUT_AT_FLG);
    } else {
        resetWolfAtCollision();
    }
}

void daAlink_c::resetAtCollision(BOOL param_0) {
    if (checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
        if (param_0 
            && !setSwordHitVibration(&mAtCps[0])
            && !setSwordHitVibration(&mAtCps[1])
            && !setSwordHitVibration(&mAtCps[2])
            && !setSwordHitVibration(&mAtCyl))
        {
            setSwordHitVibration(&mAtSph);
        }

        offNoResetFlg0(FLG0_CUT_AT_FLG);

        for (int i = 0; i < 3; i++) {
            mAtCps[i].ResetAtHit();
            mAtCps[i].OffAtSetBit();
        }

        mAtSph.ResetAtHit();
    }
}

void daAlink_c::setAtCollision() {
    static const Vec bottleTopPos = {
        0.0f, 20.0f, 0.0f
    };

    static Vec rodTopOffset = {
        0.0f, daCrod_c::getFlyInitY(), 0.0f
    };

    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

    cXyz spC8;
    cXyz spBC;
    cXyz spB0;
    cXyz spA4;

    if (mProcID == PROC_COPY_ROD_SWING) {
        if (mProcVar3.field_0x300e != 0) {
            mDoMtx_multVecZero(mHeldItemModel->getBaseTRMtx(), &spBC);
            mAtCps[0].SetStartEnd(spBC, field_0x3720);
            spA4.set(5.0f * cM_ssin(shape_angle.y), 0.0f, 5.0f * cM_scos(shape_angle.y));
            mAtCps[0].SetAtVec(spA4);
            dComIfG_Ccsp()->Set(&mAtCps[0]);
        } else {
            mAtCps[0].ResetAtHit();
        }
    } else if (mProcID == PROC_COPY_ROD_SUBJECT && checkCopyRodTopUse()) {
        mDoMtx_stack_c::transS(field_0x3834);
        mDoMtx_stack_c::ZXYrotM(mBodyAngle.x, (shape_angle.y + mBodyAngle.y), 0);
        mDoMtx_stack_c::multVec(&rodTopOffset, &spBC);

        spC8.set(0.0f, 0.0f, getCopyRodBallDisMax() + daCrod_c::getFlyInitZ());
        mDoMtx_stack_c::multVecSR(&spC8, &spA4);
        spB0 = spBC + spA4;

        mAtCps[0].SetStartEnd(spBC, spB0);
        mAtCps[0].SetAtVec(spA4);
        dComIfG_Ccsp()->Set(&mAtCps[0]);
    } else if (mProcID == PROC_BOTTLE_OPEN) {
        if (mEquipItem == fpcNm_ITEM_WATER_BOTTLE && mProcVar4.field_0x3010 != 0) {
            mDoMtx_multVec(mHeldItemModel->getBaseTRMtx(), &bottleTopPos, &spC8);

            if (field_0x27c8.getKeepMinY() < spC8.y) {
                mAtCyl.SetH(spC8.y - field_0x27c8.getKeepMinY());
                spC8.y = field_0x27c8.getKeepMinY();

                mAtCyl.SetC(spC8);
                dComIfG_Ccsp()->Set(&mAtCyl);
            }
        } else {
            mAtCyl.ResetAtHit();
        }
    } else if (mProcID == PROC_GUARD_ATTACK) {
        if (mProcVar5.field_0x3012 != 0) {
            if (checkMagneBootsOn()) {
                mDoMtx_stack_c::copy(mMagneBootMtx);
                mDoMtx_stack_c::YrotM(shape_angle.y);
            } else {
                mDoMtx_stack_c::YrotS(shape_angle.y);
            }

            mDoMtx_stack_c::multVecSR(&cXyz::BaseZ, &spA4);

            spBC = current.pos + (spA4 * mpHIO->mGuard.m.mAttackPosOffset);
            mDoMtx_stack_c::multVecSR(&cXyz::BaseY, &spC8);

            spB0 = spBC + (spC8 * 130.0f);
            mGuardAtCps.SetStartEnd(spBC, spB0);

            spA4 *= 5.0f;
            mGuardAtCps.SetAtVec(spA4);

            dComIfG_Ccsp()->Set(&mGuardAtCps);
            dComIfG_Ccsp()->SetMass(&mGuardAtCps, 1);
        } else {
            mGuardAtCps.ResetAtHit();
        }
    } else if (mProcID == PROC_FRONT_ROLL) {
        if (checkEquipHeavyBoots() && framectrl->getFrame() >= mpHIO->mFrontRoll.m.mBootsAttackInitF && framectrl->getFrame() < mpHIO->mFrontRoll.m.mBootsAttackEndF) {
            cXyz sp98 = (mLeftFootPos + mRightFootPos) * 0.5f;
            mAtSph.SetC(sp98);
            sp98.set(mNormalSpeed * cM_ssin(shape_angle.y), 0.0f, mNormalSpeed * cM_scos(shape_angle.y));
            mAtSph.SetAtVec(sp98);

            dComIfG_Ccsp()->Set(&mAtSph);
        } else {
            mAtSph.ResetAtHit();
        }
    } else if (checkHookshotItem(mEquipItem)) {
        if (mItemMode == 3
            #if !PLATFORM_GCN
            || (mItemMode == 0 && mSight.getDrawFlg())
            #endif
            || mItemMode == 1
            )
        {
            f32 maxLength;
            f32 shootSpeed;
            if (checkLv7BossRoom()) {
                shootSpeed = mpHIO->mItem.mHookshot.m.mBossShootSpeed;
                maxLength = mpHIO->mItem.mHookshot.m.mBossMaxLength;
            } else {
                shootSpeed = mpHIO->mItem.mHookshot.m.mShootSpeed;
                maxLength = mpHIO->mItem.mHookshot.m.mMaxLength;
            }

            f32 var_f31;
            if (mItemMode == 3) {
                spC8 = mHookshotTopPos - mHeldItemRootPos;

                f32 len = spC8.abs();
                var_f31 = 15.0f + shootSpeed;
                if (var_f31 + len > maxLength) {
                    var_f31 = maxLength - len;
                }

                if (cLib_distanceAngleS(spC8.atan2sX_Z(), field_0x301e) > 0x4000) {
                    var_f31 = -1.0f;
                }
            } else {
                mRopeLinChk.Set(&field_0x3834, &mHeldItemRootPos, this);
                if (dComIfG_Bgsp().LineCross(&mRopeLinChk)) {
                    var_f31 = -1.0f;
                } else {
                    var_f31 = maxLength;
                }
            }

            if (var_f31 > 0.0f) {
                spBC = mHookshotTopPos;
                spB0 = mHookshotTopPos + (mIronBallCenterPos * var_f31);

                mRopeLinChk.Set(&spBC, &spB0, this);
                if (dComIfG_Bgsp().LineCross(&mRopeLinChk)) {
                    spB0 = mRopeLinChk.GetCross();
                }

                mAtCps[0].SetStartEnd(spBC, spB0);
                spA4 = spB0 - spBC;
                mAtCps[0].SetAtVec(spA4);
                dComIfG_Ccsp()->Set(&mAtCps[0]);
            } else {
                mAtCps[0].ResetAtHit();
            }
        } else {
            mAtCps[0].ResetAtHit();
        }
    } else if (checkKandelaarSwing(1)) {
        if (dComIfGs_getOil() != 0 && mUnderFrameCtrl[0].getFrame() >= field_0x347c && mUnderFrameCtrl[0].getFrame() <= field_0x3480) {
            if (mAtSph.ChkAtSet()) {
                mAtSph.MoveCAt(mKandelaarFlamePos);
            } else {
                mAtSph.OnAtSetBit();
                mAtSph.StartCAt(mKandelaarFlamePos);
            }

            dComIfG_Ccsp()->Set(&mAtSph);
            dComIfG_Ccsp()->SetMass(&mAtSph, 1);
        } else {
            mAtSph.ResetAtHit();
        }
    } else if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        if (mItemVar0.field_0x3018 == 2 || mItemVar0.field_0x3018 == 3 || mItemVar0.field_0x3018 == 4 || mItemVar0.field_0x3018 == 5 || mItemVar0.field_0x3018 == 7 || mItemVar0.field_0x3018 == 6) {

            #if DEBUG
            mAtSph.SetR(mpHIO->mItem.mIronBall.m.mAttackRadius);
            #endif

            if (mAtSph.ChkAtSet()) {
                mAtSph.MoveCAt(mIronBallCenterPos);
            } else {
                mAtSph.StartCAt(mIronBallCenterPos);
                mAtSph.OnAtSetBit();
            }

            dComIfG_Ccsp()->Set(&mAtSph);
            dComIfG_Ccsp()->SetMass(&mAtSph, 1);

            if (mItemVar0.field_0x3018 == 5 || mItemVar0.field_0x3018 == 7 || mItemVar0.field_0x3018 == 6) {
                field_0x1778.SetC(mIronBallCenterPos);
                dComIfG_Ccsp()->Set(&field_0x1778);
            }
        } else {
            mAtSph.ResetAtHit();
            field_0x1778.ResetAtHit();
        }
    }

    if (checkResetFlg0(RFLG0_UNK_2)) {
        if (!setSwordHitVibration(&mAtCps[0])
            && !setSwordHitVibration(&mAtCps[1])
            && !setSwordHitVibration(&mAtCps[2])
            && !setSwordHitVibration(&mAtCyl))
        {
            setSwordHitVibration(&mAtSph);
        }

        if (checkCutDashAnime()) {
            m_swordBlur.field_0x20 = mpHIO->mCut.m.mDashBlurAlpha;
            spC8.set(current.pos.x + (50.0f * cM_ssin(shape_angle.y)), 20.0f + current.pos.y, current.pos.z + (50.0f * cM_scos(shape_angle.y)));
        } else {
            m_swordBlur.field_0x20 = mpHIO->mCut.m.mBlurAlpha;
            spC8.set(current.pos.x + (50.0f * cM_ssin(shape_angle.y)), field_0x3478, current.pos.z + (50.0f * cM_scos(shape_angle.y)));
        }

        if (mProcID == PROC_CUT_TURN || (mProcID == PROC_CUT_LARGE_JUMP_LAND && mProcVar2.field_0x300c != 0) || mProcID == PROC_BOARD_CUT_TURN) {
            if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
                onNoResetFlg0(FLG0_CUT_AT_FLG);
            }

            spC8 = current.pos + (mMagneBootsTopVec * 50.0f);
            mAtSph.SetC(spC8);

            spC8 = cXyz::Zero;
            mAtSph.SetAtVec(spC8);
            dComIfG_Ccsp()->Set(&mAtSph);
            dComIfG_Ccsp()->SetMass(&mAtSph, 1);
            decSwordBlur();

            if (mProcID == PROC_BOARD_CUT_TURN) {
                mProcVar5.field_0x3012 = cM_atan2s(mRightFootPos.x - mLeftFootPos.x, mRightFootPos.z - mLeftFootPos.z) - shape_angle.y;
            }
        } else if (mProcID == PROC_HORSE_CUT_TURN) {
            if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
                onNoResetFlg0(FLG0_CUT_AT_FLG);
            }

            spC8.set(current.pos.x, current.pos.y - 80.0f, current.pos.z);
            mAtCyl.SetC(spC8);
            dComIfG_Ccsp()->Set(&mAtCyl);
            dComIfG_Ccsp()->SetMass(&mAtCyl, 1);
            decSwordBlur();
        } else if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
            setSwordAtCollision(1);
            onNoResetFlg0(FLG0_CUT_AT_FLG);

            if (checkCutDashAnime()) {
                mAtCyl.StartCAt(spC8);
                dComIfG_Ccsp()->Set(&mAtCyl);
            }

            if (!checkCutTypeNoBlur()) {
                m_swordBlur.initBlur(0.0f, 0, &mSwordTopPos, &field_0x3498, &field_0x34a4);
                return;
            }

            decSwordBlur();
        } else {
            setSwordAtCollision(0);

            if (checkCutDashAnime()) {
                mAtCyl.MoveCAt(spC8);
                dComIfG_Ccsp()->Set(&mAtCyl);
            }

            if (!checkCutTypeNoBlur()) {
                m_swordBlur.copyBlur(&mSwordTopPos, &field_0x3498, &field_0x34a4);
                return;
            }

            decSwordBlur();
        }
    } else {
        decSwordBlur();
        resetAtCollision(1);
        mAtCyl.ResetAtHit();
    }
}

void daAlink_c::setWolfCollisionPos() {
    static Vec const localHead = {15.0f, 5.0f, 0.0f};

    cXyz sp44;
    mDoMtx_multVec(mpLinkModel->getAnmMtx(4), &localHead, &sp44);
    mAtSph.MoveCAt(sp44);

    cXyz sp38;
    cXyz sp2C;
    f32 sp20[3];
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x10), &sp38);
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x15), &sp2C);

    sp44.x = 0.5f * (sp38.x + sp2C.x);
    sp44.z = 0.5f * (sp38.z + sp2C.z);

    sp20[0] = mAtSph.GetC().y;
    sp44.y = mLeftHandPos.y > mRightHandPos.y ? mRightHandPos.y : mLeftHandPos.y;

    f32 temp_f31;
    if (sp20[0] < sp44.y) {
        temp_f31 = sp20[0];
        sp20[0] = sp44.y;
        sp44.y = temp_f31;
    }

    mTgCyls[0].SetC(sp44);
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x1C), &sp38);
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0x21), &sp2C);

    sp20[2] = 0.5f * (sp38.y + sp2C.y);
    sp44.x = 0.5f * (sp38.x + sp2C.x);
    sp44.z = 0.5f * (sp38.z + sp2C.z);

    sp44.y = mLeftFootPos.y > mRightFootPos.y ? mRightFootPos.y : mLeftFootPos.y;

    if (sp20[2] < sp44.y) {
        temp_f31 = sp20[2];
        sp20[2] = sp44.y;
        sp44.y = temp_f31;
    }

    mTgCyls[2].SetC(sp44);
    sp44 = (mTgCyls[0].GetC() + mTgCyls[2].GetC()) * 0.5f;

    sp20[1] = 0.5f * (sp20[0] + sp20[2]);

    mTgCyls[1].SetC(sp44);

    for (int i = 0; i < 3; i++) {
        f32 var_f30 = 20.0f + (sp20[i] - mTgCyls[i].GetC().y);
        mTgCyls[i].SetH(var_f30);
    }

    field_0x3454 = mTgCyls[0].GetC().y + (mTgCyls[0].GetH() * 0.5f);
}

void daAlink_c::initLockAt() {
    mAtCps[0].OnAtSPrmBit(0x20);
    mAtCps[0].OnAtNoHitMark();
    mAtCps[0].OffAtNoConHit();
    mAtCps[0].ResetAtHit();
}

void daAlink_c::cancelLockAt() {
    mAtCps[0].OffAtSPrmBit(0x20);
    mAtCps[0].OffAtNoHitMark();
    mAtCps[0].OnAtNoConHit();
    mAtCps[0].ResetAtHit();
    mCcStts.SetAtApid(fpcM_ERROR_PROCESS_ID_e);
}

void daAlink_c::setCollisionPos() {
    cXyz sp74;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0), &sp74);
    cXyz sp68 = (sp74 + mHeadTopPos) * 0.5f;

    f32 var_f31;
    if (sp74.y > mHeadTopPos.y) {
        sp68.y = mHeadTopPos.y;
        var_f31 = sp74.y - mHeadTopPos.y;
    } else {
        sp68.y = sp74.y;
        var_f31 = mHeadTopPos.y - sp74.y;
    }

    if (var_f31 < 60.0f) {
        sp68.y -= 0.5f * (60.0f - var_f31);
        var_f31 = 60.0f;
    }

    mTgCyls[2].SetC(sp68);
    mTgCyls[2].SetH(var_f31);

    sp68 = (((mLeftFootPos + mRightFootPos) * 0.5f) + sp74) * 0.5f;
    f32 var_f0 = mLeftFootPos.y > mRightFootPos.y ? mRightFootPos.y : mLeftFootPos.y;

    f32 var_f31_2;
    if (sp74.y > var_f0) {
        sp68.y = var_f0;
        var_f31_2 = sp74.y - var_f0;
    } else {
        sp68.y = sp74.y;
        var_f31_2 = var_f0 - sp74.y;
    }

    if (var_f31_2 < 60.0f) {
        sp68.y -= 0.5f * (60.0f - var_f31_2);
        var_f31_2 = 60.0f;
    }

    mTgCyls[0].SetC(sp68);
    mTgCyls[0].SetH(var_f31_2);

    sp68 = (mTgCyls[0].GetC() + mTgCyls[2].GetC()) * 0.5f;
    f32 temp_f31 = 0.5f * (mTgCyls[0].GetH() + mTgCyls[2].GetH());

    mTgCyls[1].SetC(sp68);
    mTgCyls[1].SetH(temp_f31);
    field_0x3454 = field_0x3834.y;
}

void daAlink_c::setCollision() {
    mCcStts.Move();
    field_0x173c.Move();

    dCcD_Cyl* tgCyl = mTgCyls;
    CcG_Tg_HitMark hitmark;
    if (checkMagicArmorNoDamage()) {
        hitmark = CcG_Tg_UNK_MARK_8;
    } else {
        hitmark = CcG_Tg_UNK_MARK_6;
    }

    if (checkIronBallWaitAnime()) {
        field_0x2e44.offPassNum(0x11);
    } else {
        field_0x2e44.onPassNum(0x11);
    }

    int i;
    for (i = 0; i < 3; i++, tgCyl++) {
        tgCyl->SetTgHitMark(hitmark);
    }

    tgCyl = mTgCyls;
    if (checkIronBallWaitAnime() ||
        (checkPlayerGuardAndAttack() && !checkEndResetFlg1(ERFLG1_UNK_20)))
    {
        BOOL isSpShield;
        if (field_0x3114 >= 0x3000 && field_0x3114 <= 0x5000) {
            isSpShield = TRUE;
        } else {
            isSpShield = FALSE;
        }

        for (i = 0; i < 3; i++, tgCyl++) {
            if (isSpShield) {
                tgCyl->OnTgSpShield();
                tgCyl->OffTgShield();
            } else {
                tgCyl->OffTgSpShield();
                tgCyl->OnTgShield();
            }

            if (checkHorseRide()) {
                tgCyl->OffTgShieldFrontRange();
            } else {
                tgCyl->OnTgShieldFrontRange();
            }
        }
    } else {
        for (i = 0; i < 3; i++, tgCyl++) {
            tgCyl->OffTgShield();
            tgCyl->OffTgSpShield();

            if (checkWolf()) {
                tgCyl->OffTgShieldFrontRange();
            } else {
                tgCyl->OnTgShieldFrontRange();
            }
        }
    }

    if (checkNoCollisionCorret() && !checkOctaIealSpecialCollect()) {
        onNoResetFlg3(FLG3_UNK_20000000);
    }

    if (checkWolf()) {
        setWolfCollisionPos();
    } else {
        setCollisionPos();
    }

    if (mDamageTimer != 0 || checkModeFlg(8)) {
        for (i = 0; i < 3; i++) {
            mTgCyls[i].OffTgSetBit();
            mTgCyls[i].ResetTgHit();
        }

        if (checkWolf()) {
            mAtSph.OffTgSetBit();
            mAtSph.ResetTgHit();
        }
    } else {
        for (i = 0; i < 3; i++) {
            mTgCyls[i].OnTgSetBit();
        }

        if (checkWolf()) {
            mAtSph.OnTgSetBit();
        }
    }

    mDoMtx_multVecSR(getShieldMtx(), &cXyz::BaseZ, &field_0x351c);

    if (!checkIronBallWaitAnime() && field_0x351c.absXZ() > 0.0099999998f) {
        field_0x306c = field_0x351c.atan2sX_Z();
    } else {
        field_0x306c = shape_angle.y + mBodyAngle.y;
    }

    dComIfG_Ccsp()->Set(&mTgCyls[0]);
    dComIfG_Ccsp()->SetMass(&mTgCyls[0], 1);

    if (checkWolf()) {
        setWolfAtCollision();

        for (i = 1; i < 3; i++) {
            dComIfG_Ccsp()->Set(&mTgCyls[i]);
            dComIfG_Ccsp()->SetMass(&mTgCyls[i], 1);
        }

        if (checkModeFlg(0x100000)) {
            mAtSph.OffCoSetBit();
            mAtSph.ResetCoHit();
        } else {
            mAtSph.OnCoSetBit();
        }

        dComIfG_Ccsp()->Set(&mAtSph);
        dComIfG_Ccsp()->SetMass(&mAtSph, 1);
    } else {
        for (i = 1; i < 3; i++) {
            dComIfG_Ccsp()->Set(&mTgCyls[i]);
            dComIfG_Ccsp()->SetMass(&mTgCyls[i], 1);
        }

        setAtCollision();
    }

    tgCyl = mTgCyls;
    dCcD_Cyl* windTgCyl = mWindTgCyls;
    for (i = 0; i < 3; i++, tgCyl++, windTgCyl++) {
        windTgCyl->SetC(tgCyl->GetC());
        windTgCyl->SetR(tgCyl->GetR());
        windTgCyl->SetH(tgCyl->GetH());
        dComIfG_Ccsp()->Set(windTgCyl);
    }
}

f32 daAlink_c::getBaseAnimeFrame() const {
    if (mProcID == PROC_SUMOU_ACTION && mSpeedModifier > 0.0f) {
        return mProcVar2.field_0x300c;
    }

    return mUnderFrameCtrl[0].getFrame();
}

void daAlink_c::setAnimeFrame(f32 i_frame) {
    mUnderFrameCtrl[0].setFrame(i_frame);
    mUnderFrameCtrl[1].setFrame(i_frame);
    mUnderFrameCtrl[2].setFrame(i_frame);

    mUpperFrameCtrl[0].setFrame(i_frame);
    mUpperFrameCtrl[1].setFrame(i_frame);
    mUpperFrameCtrl[2].setFrame(i_frame);
}

void daAlink_c::setFrameCtrl(daPy_frameCtrl_c* i_framectrl, u8 i_attr, s16 i_start, s16 i_end,
                             f32 i_rate, f32 i_frame) {
    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        if (checkZoraWearAbility()) {
            i_rate *= mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed;
        } else {
            i_rate *= mpHIO->mItem.mIronBoots.m.mWaterStartWalkAnmRate;
        }
    }

    i_framectrl->setFrameCtrl(i_attr, i_start, i_end, i_rate, i_frame);
}

const daAlink_BckData* daAlink_c::getMainBckData(daAlink_c::daAlink_ANM i_anmID) const {
    static const daAlink_BckData kandelaarAnm[2] = {
        {dRes_ID_ALANM_BCK_WAITS_e, dRes_ID_ALANM_BCK_WAITK_e},
        {dRes_ID_ALANM_BCK_WAITHS_e, dRes_ID_ALANM_BCK_WAITHK_e},
    };

    if (mEquipItem == fpcNm_ITEM_KANTERA) {
        if (i_anmID == ANM_WAIT) {
            return &kandelaarAnm[0];
        }

        if (i_anmID == ANM_HORSE_WAIT) {
            return &kandelaarAnm[1];
        }
    }

    if (checkUpperGuardAnime() && i_anmID < ANM_SWIM_WAIT) {
        return &m_mainBckShield[i_anmID];
    }

    if ((mEquipItem == 0x103 && i_anmID < ANM_STEP_TURN && i_anmID >= ANM_ATN_WAIT_LEFT) ||
        (i_anmID == ANM_SWIM_WAIT && mEquipItem != fpcNm_ITEM_NONE))
    {
        return &m_mainBckSword[i_anmID - ANM_ATN_WAIT_LEFT];
    }

    if (checkFishingRodAndLureItem() && i_anmID < ANM_RUN_B) {
        return &m_mainBckFishing[i_anmID];
    }

    return &m_anmDataTable[i_anmID].m_bckData;
}

BOOL daAlink_c::checkUnderMove0BckNoArc(daAlink_c::daAlink_ANM i_anmID) const {
    return mUnderAnmHeap[0].checkNoSetArcNo() &&
           mUnderAnmHeap[0].getIdx() == getMainBckData(i_anmID)->m_underID;
}

BOOL daAlink_c::checkUnderMove1BckNoArc(daAlink_c::daAlink_ANM i_anmID) const {
    return mUnderAnmHeap[1].checkNoSetArcNo() &&
           mUnderAnmHeap[1].getIdx() == getMainBckData(i_anmID)->m_underID;
}

int daAlink_c::getUnderUpperAnime(daAlink_c::daAlink_ANM i_anmID, J3DAnmTransform** i_underBck,
                                  J3DAnmTransform** i_upperBck, int i_anmPackID, u32 i_bufSize) {
    s32 var_r31;

    const daAlink_BckData* bck_data = getMainBckData(i_anmID);
    offNoResetFlg0(FLG0_UNK_2);

    *i_underBck = (J3DAnmTransform*)getAnimeResource(&mUnderAnmHeap[i_anmPackID],
                                                     bck_data->m_underID, i_bufSize);

    if (*i_underBck != NULL) {
        var_r31 = 1;
    } else {
        *i_underBck = getNowAnmPackUnder((daAlink_UNDER)i_anmPackID);
        var_r31 = 0;
    }

    if (bck_data->m_underID != bck_data->m_upperID) {
        if (i_bufSize == 0x10800) {
            i_bufSize = (3 - i_anmPackID) * 0x2C00;
        }

        *i_upperBck = (J3DAnmTransform*)getAnimeResource(&mUpperAnmHeap[i_anmPackID],
                                                         bck_data->m_upperID, i_bufSize);
        if (*i_upperBck != NULL) {
            var_r31 |= 1;
        } else {
            *i_upperBck = getNowAnmPackUpper((daAlink_UPPER)i_anmPackID);
        }
    } else {
        if (!mUpperAnmHeap[i_anmPackID].checkNoSetIdx()) {
            var_r31 |= 1;
        }

        *i_upperBck = NULL;
        mUpperAnmHeap[i_anmPackID].resetIdx();
    }

    return var_r31;
}

void daAlink_c::setDoubleAnimeBlendRatio(f32 i_ratio) {
    mNowAnmPackUnder[0].setRatio(1.0f - i_ratio);
    mNowAnmPackUnder[1].setRatio(i_ratio);
    mNowAnmPackUpper[0].setRatio(1.0f - i_ratio);
    mNowAnmPackUpper[1].setRatio(i_ratio);
}

void daAlink_c::commonDoubleAnime(J3DAnmTransform* i_underBck1, J3DAnmTransform* i_upperBck1,
                                  J3DAnmTransform* i_underBck2, J3DAnmTransform* i_upperBck2,
                                  f32 i_blendRatio, f32 i_anmSpeed1, f32 i_anmSpeed2, int param_7) {
    daPy_frameCtrl_c* framectrl0 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* framectrl1 = &mUnderFrameCtrl[1];

    f32 var_f31;
    if (field_0x2f8c == 0) {
        var_f31 = 0.0f;
    } else {
        var_f31 = framectrl0->getFrame() / framectrl0->getEnd();
    }

    setDoubleAnimeBlendRatio(i_blendRatio);

    f32 temp_f30;
    f32 temp_f29 = i_underBck1->getFrameMax();
    f32 temp_f28 = i_underBck2->getFrameMax();
    f32 temp_f27 = 1.0f / temp_f29;
    temp_f30 = i_anmSpeed1 + (i_blendRatio * (((i_anmSpeed2 * temp_f29) / temp_f28) - i_anmSpeed1));

    setFrameCtrl(framectrl0, i_underBck1->getAttribute(), 0, temp_f29, temp_f30, var_f31 * temp_f29);
    i_underBck1->setFrame(framectrl0->getFrame());

    setFrameCtrl(framectrl1, i_underBck2->getAttribute(), 0, temp_f28, temp_f27 * (temp_f30 * temp_f28), var_f31 * temp_f28);
    i_underBck2->setFrame(framectrl1->getFrame());

    mNowAnmPackUnder[0].setAnmTransform(i_underBck1);
    mNowAnmPackUnder[1].setAnmTransform(i_underBck2);

    if (i_upperBck1 != NULL) {
        f32 temp_f26 = i_upperBck1->getFrameMax();
        mNowAnmPackUpper[0].setAnmTransform(i_upperBck1);

        setFrameCtrl(&mUpperFrameCtrl[0], i_upperBck1->getAttribute(), 0, temp_f26, temp_f27 * (temp_f30 * temp_f26), var_f31 * temp_f26);
        i_upperBck1->setFrame(mUpperFrameCtrl[0].getFrame());
    } else {
        mNowAnmPackUpper[0].setAnmTransform(i_underBck1);
    }

    if (i_upperBck2 != NULL) {
        f32 temp_f25 = i_upperBck2->getFrameMax();
        mNowAnmPackUpper[1].setAnmTransform(i_upperBck2);

        setFrameCtrl(&mUpperFrameCtrl[1], i_upperBck2->getAttribute(), 0, temp_f25, temp_f27 * (temp_f30 * temp_f25), var_f31 * temp_f25);
        i_upperBck2->setFrame(mUpperFrameCtrl[1].getFrame());
    } else {
        mNowAnmPackUpper[1].setAnmTransform(i_underBck2);
    }

    if (param_7 == 4 || param_7 == 2) {
        setBasAnime(UNDER_0);
    } else {
        setBasAnime(UNDER_1);
    }

    field_0x2f8c = param_7;
}

int daAlink_c::setDoubleAnime(f32 i_blendRate, f32 i_anmSpeedA, f32 i_anmSpeedB,
                              daAlink_c::daAlink_ANM i_anmA, daAlink_c::daAlink_ANM i_anmB,
                              int param_5, f32 i_morf) {
#if DEBUG
    if (checkWolf()) {
        // "Player is Wolf, but setting Link animation\n"
        OS_REPORT("狼なのにリンクアニメ設定\n");
        JUT_ASSERT(8591, FALSE);
    }
#endif

    J3DAnmTransform* under_bck1;
    J3DAnmTransform* upper_bck1;
    J3DAnmTransform* under_bck2;
    J3DAnmTransform* upper_bck2;

    int temp_r3 = getUnderUpperAnime(i_anmA, &under_bck1, &upper_bck1, 0, 0x2C00);
    temp_r3 |= getUnderUpperAnime(i_anmB, &under_bck2, &upper_bck2, 1, 0x2C00);

    if (temp_r3 != 0 && i_morf < 0.0f) {
        if (i_anmA == ANM_WAIT_B && checkModeFlg(1)) {
            i_morf = mpHIO->mMove.m.mWaitBInterpolation;
        } else {
            i_morf = mpHIO->mBasic.m.mBasicInterpolation;
        }

        if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
            if (checkZoraWearAbility()) {
                i_morf *= (1.0f / mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed);
            } else {
                i_morf *= (1.0f / mpHIO->mItem.mIronBoots.m.mWaterWalkAnmRate);
            }
        }
    } else if (i_anmA == ANM_WAIT_B && checkModeFlg(1) && i_morf > 0.0f) {
        i_morf = mpHIO->mMove.m.mWaitBInterpolation;

        if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
            if (checkZoraWearAbility()) {
                i_morf *= (1.0f / mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed);
            } else {
                i_morf *= (1.0f / mpHIO->mItem.mIronBoots.m.mWaterWalkAnmRate);
            }
        }
    }

    commonDoubleAnime(under_bck1, upper_bck1, under_bck2, upper_bck2, i_blendRate, i_anmSpeedA,
                      i_anmSpeedB, param_5);
    if (i_morf >= 0.0f) {
        field_0x2060->initOldFrameMorf(i_morf, 0, 35);
    }

    setHandIndex(i_anmA);

    if (checkReinRide()) {
        if (checkRestHPAnime() &&
            (checkUnderMove0BckNoArc(ANM_HORSE_WAIT) || checkUnderMove0BckNoArc(ANM_HORSE_WALK_A) ||
             checkUnderMove0BckNoArc(ANM_HORSE_WALK_B)))
        {
            if (checkNoUpperAnime()) {
                u16 resIdx = dRes_ID_ALANM_BCK_WAITHDS_e;
                setUpperAnimeBaseMorf(resIdx, mpHIO->mHorse.m.mTiredWaitInterpolation);
            }

            setFaceBasicAnime(ANM_WAIT_TIRED);
            setTiredVoice(&mUpperFrameCtrl[2]);
        } else {
            if (checkHorseTiredAnime()) {
                resetUpperAnime(UPPER_2, 3.0f);
            }

            setFaceBasicAnime(i_anmA);
        }
    } else if ((i_anmB == ANM_WALK_HEAVY && i_anmA == ANM_WAIT) || i_anmB == ANM_HORSE_DASH_B) {
        setFaceBasicAnime(i_anmB);
    } else {
        setFaceBasicAnime(i_anmA);
    }

    return 1;
}

void daAlink_c::commonSingleAnime(J3DAnmTransform* i_underBck, J3DAnmTransform* i_upperBck,
                                  f32 i_speed, f32 i_startF, s16 i_endF) {
    mUnderAnmHeap[1].resetIdx();
    mUpperAnmHeap[1].resetIdx();
    setDoubleAnimeBlendRatio(0.0f);

    mNowAnmPackUnder[0].setAnmTransform(i_underBck);
    mNowAnmPackUnder[1].setAnmTransform(NULL);

    s16 end_frame;
    if (i_endF < 0) {
        end_frame = i_underBck->getFrameMax();
    } else {
        end_frame = i_endF;
    }

    f32 frame;
    if (i_speed < 0.0f) {
        frame = end_frame;
    } else {
        frame = i_startF;
    }

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE) && checkModeFlg(0x200)) {
        if (checkZoraWearAbility() && mProcID != PROC_FRONT_ROLL && mEquipItem == 0x103) {
            i_speed *= mpHIO->mItem.mIronBoots.m.mWaterVelRateSword * (1.0f / mpHIO->mItem.mIronBoots.m.mZoraWaterAnmSpeed);
        } else if (!checkZoraWearAbility()) {
            i_speed *= mpHIO->mItem.mIronBoots.m.mWaterWalkAnmRate * (1.0f / mpHIO->mItem.mIronBoots.m.mWaterStartWalkAnmRate);
        }
    }

    setFrameCtrl(&mUnderFrameCtrl[0], i_underBck->getAttribute(), i_startF, end_frame, i_speed, frame);
    i_underBck->setFrame(frame);

    if (i_upperBck != NULL) {
        mNowAnmPackUpper[0].setAnmTransform(i_upperBck);

        if (i_endF < 0) {
            end_frame = i_upperBck->getFrameMax();
        } else {
            end_frame = i_endF;
        }

        if (i_speed < 0.0f) {
            frame = end_frame;
        } else {
            frame = i_startF;
        }

        setFrameCtrl(&mUpperFrameCtrl[0], i_upperBck->getAttribute(), i_startF, end_frame, i_speed, frame);
        i_upperBck->setFrame(frame);
    } else {
        mNowAnmPackUpper[0].setAnmTransform(i_underBck);
    }

    mNowAnmPackUpper[1].setAnmTransform(NULL);
    setBasAnime(UNDER_0);
    field_0x2f8c = 0;
}

int daAlink_c::setSingleAnimeBase(daAlink_c::daAlink_ANM i_anmID) {
    return setSingleAnime(i_anmID, 1.0f, 0.0f, -1, 3.0f);
}

int daAlink_c::setSingleAnimeBaseMorf(daAlink_c::daAlink_ANM i_anmID, f32 i_morf) {
    return setSingleAnime(i_anmID, 1.0f, 0.0f, -1, i_morf);
}

int daAlink_c::setSingleAnimeBaseSpeed(daAlink_c::daAlink_ANM i_anmID, f32 i_speed, f32 i_morf) {
    return setSingleAnime(i_anmID, i_speed, 0.0f, -1, i_morf);
}

int daAlink_c::setSingleAnime(daAlink_c::daAlink_ANM i_anmID, f32 i_speed, f32 i_start, s16 i_end,
                              f32 i_morf) {
    J3DAnmTransform* under_bck;
    J3DAnmTransform* upper_bck;

    #if DEBUG
    if (checkWolf()) {
        OS_REPORT("狼なのにリンクアニメ設定\n");
        JUT_ASSERT(8861, FALSE);
    }
    #endif

    getUnderUpperAnime(i_anmID, &under_bck, &upper_bck, 0, 0x10800);
    commonSingleAnime(under_bck, upper_bck, i_speed, i_start, i_end);

    if (i_morf >= 0.0f) {
        field_0x2060->initOldFrameMorf(i_morf, 0, 35);
    }

    setHandIndex(i_anmID);
    setFaceBasicAnime(i_anmID);

    return 1;
}

int daAlink_c::setSingleAnimeParam(daAlink_c::daAlink_ANM i_anmID, const daAlinkHIO_anm_c* i_anmParams) {
    return setSingleAnime(i_anmID, i_anmParams->mSpeed, i_anmParams->mStartFrame, i_anmParams->mEndFrame,
                          i_anmParams->mInterpolation);
}

void daAlink_c::animePlay(J3DAnmTransform* i_anm, daPy_frameCtrl_c* i_frameCtrl) {
    if (i_anm != NULL) {
        i_frameCtrl->updateFrame();
        i_anm->setFrame(i_frameCtrl->getFrame());
    }
}

void daAlink_c::allAnimePlay() {
    J3DAnmTransform* under0_bck = getNowAnmPackUnder(UNDER_0);
    J3DAnmTransform* under1_bck = getNowAnmPackUnder(UNDER_1);
    J3DAnmTransform* upper0_bck = getNowAnmPackUpper(UPPER_0);
    J3DAnmTransform* upper1_bck = getNowAnmPackUpper(UPPER_1);

    if (checkWolf()) {
        setWolfAnmVoice();
    }

    for (int i = 0; i < 3; i++) {
        animePlay(getNowAnmPackUnder((daAlink_UNDER)i), &mUnderFrameCtrl[i]);
    }

    if (upper0_bck != under0_bck) {
        animePlay(upper0_bck, &mUpperFrameCtrl[0]);
    }

    if (upper1_bck != under1_bck) {
        animePlay(upper1_bck, &mUpperFrameCtrl[1]);
    }

    for (int i = 2; i < 3; i++) {
        animePlay(getNowAnmPackUpper((daAlink_UPPER)i), &mUpperFrameCtrl[i]);
    }

    if (mLeftHandIndex == 0xFB && mpDemoHLTmpBck != NULL) {
        mpDemoHLTmpBck->play();
    }

    if (mRightHandIndex == 0xFB && mpDemoHRTmpBck != NULL) {
        mpDemoHRTmpBck->play();
    }

    if (checkNoResetFlg1(FLG1_UNK_10)) {
        if (field_0x069c != NULL && field_0x069c->getBlkAnm() != NULL) {
            simpleAnmPlay(field_0x069c->getBlkAnm());
        }

        if (field_0x068c != NULL) {
            simpleAnmPlay(field_0x068c);
        }

        if (mpDemoFCTmpBck != NULL) {
            mpDemoFCTmpBck->play();
        }
    }

    if (mpDemoHDTmpBck != NULL) {
        mpDemoHDTmpBck->play();
    }

    simpleAnmPlay(field_0x06f4);
    simpleAnmPlay(field_0x06f8);
}

void daAlink_c::setUpperAnimeMorf(f32 i_morf) {
    if (i_morf >= 0.0f) {
        u16 temp_r29;
        u16 temp_r28;

        if (field_0x2060->getOldFrameRate() > 0.1f && field_0x2060->getOldFrameStartJoint() == 0) {
            temp_r29 = 0;
            temp_r28 = field_0x30c6;
        } else {
            temp_r29 = field_0x30a8;
            temp_r28 = field_0x30aa;
        }

        field_0x2060->initOldFrameMorf(i_morf, temp_r29, temp_r28);
    }
}

void daAlink_c::setUpperAnimeBase(u16 i_anmResIdx) {
    setUpperAnime(i_anmResIdx, UPPER_2, 1.0f, 0.0f, -1, 3.0f);
}

void daAlink_c::setUpperAnimeBaseMorf(u16 i_anmResIdx, f32 i_morf) {
    setUpperAnime(i_anmResIdx, UPPER_2, 1.0f, 0.0f, -1, i_morf);
}

void daAlink_c::setUpperAnimeBaseSpeed(u16 i_anmResIdx, f32 i_speed, f32 i_morf) {
    setUpperAnime(i_anmResIdx, UPPER_2, i_speed, 0.0f, -1, i_morf);
}

int daAlink_c::setUpperAnime(u16 i_anmResIdx, daAlink_c::daAlink_UPPER i_upperIdx, f32 i_speed,
                             f32 i_startFrame, s16 i_endFrame, f32 i_morf) {
    BOOL var_r28 = FALSE;

    if (i_upperIdx == UPPER_2) {
        resetFacePriAnime();
        offNoResetFlg1(daPy_FLG1(FLG1_UNK_10000000 | FLG1_UNK_4000000 | FLG1_UNK_100000 | FLG1_UNK_40000 | FLG1_UNK_20000 | FLG1_UNK_10000 | FLG1_UNK_40 | FLG1_UNK_8 | FLG1_UNK_2));
        cancelItemUseQuake(0);

        if (checkFmChainGrabAnime()) {
            var_r28 = TRUE;
        }
    }

    if (checkDkCaught2Anime()) {
        offNoResetFlg0(FLG0_DK_CAUGHT2);
    }

    J3DAnmTransform* bck = getAnimeResource(&mUpperAnmHeap[i_upperIdx], i_anmResIdx, 0x2C00);
    if (bck == NULL) {
        bck = getNowAnmPackUpper(i_upperIdx);
    }

    mNowAnmPackUpper[i_upperIdx].setAnmTransform(bck);
    mNowAnmPackUpper[i_upperIdx].setRatio(1.0f);
    mUpperAnmHeap[i_upperIdx].setIdx(i_anmResIdx);

    if (i_endFrame < 0) {
        i_endFrame = bck->getFrameMax();
    }

    f32 frame;
    if (i_speed < 0.0f) {
        frame = i_endFrame;
    } else {
        frame = i_startFrame;
    }

    if (!checkZoraWearAbility() && (checkEquipAnime() || checkCutDashAnime()) && (checkNoResetFlg0(FLG0_WATER_IN_MOVE) || !checkNoResetFlg0(FLG0_SWIM_UP))) {
        i_speed *= mpHIO->mItem.mIronBoots.m.mWaterWalkAnmRate * (1.0f / mpHIO->mItem.mIronBoots.m.mWaterStartWalkAnmRate);
    }

    setFrameCtrl(&mUpperFrameCtrl[i_upperIdx], bck->getAttribute(), i_startFrame, i_endFrame, i_speed, frame);
    bck->setFrame(frame);
    setUpperAnimeMorf(i_morf);

    if (!checkBowAnime()) {
        setBowNormalAnime();
        deleteArrow();
    }

    if (!checkGrabAnimeAndThrow() && !checkWolfGrabAnime()) {
        freeGrabItem();
    }

    if (!checkWolfEnemyThrowAnime() && checkNoResetFlg2(FLG2_UNK_8)) {
        resetWolfEnemyBiteAll();
    }

    if (var_r28 && !checkFmChainGrabAnime()) {
        cancelFmChainGrabFromOut();
    }

    offNoResetFlg0(FLG0_UNK_1000000);
    return 1;
}

void daAlink_c::setUpperAnimeParam(u16 i_anmResIdx, daAlink_c::daAlink_UPPER i_upperIdx,
                                   const daAlinkHIO_anm_c* i_anmParam) {
    setUpperAnime(i_anmResIdx, i_upperIdx, i_anmParam->mSpeed, i_anmParam->mStartFrame,
                  i_anmParam->mEndFrame, i_anmParam->mInterpolation);
}

int daAlink_c::resetUpperAnime(daAlink_c::daAlink_UPPER i_upperIdx, f32 i_morf) {
    BOOL var_r29 = FALSE;
    if (checkFmChainGrabAnime()) {
        cancelFmChainGrabFromOut();
    } else if (!checkModeFlg(0x100000) && (checkGrabAnime() || checkWolfGrabAnime())) {
        var_r29 = TRUE;
    }

    if (checkDkCaught2Anime()) {
        offNoResetFlg0(FLG0_DK_CAUGHT2);
    } else if (checkWolfEnemyThrowAnime()) {
        resetWolfEnemyBiteAll();
    }

    mNowAnmPackUpper[i_upperIdx].setAnmTransform(NULL);
    mNowAnmPackUpper[i_upperIdx].setRatio(0.0f);
    mUpperAnmHeap[i_upperIdx].resetIdx();
    mUpperFrameCtrl[i_upperIdx].init(0);

    setUpperAnimeMorf(i_morf);

    if (i_upperIdx == UPPER_2) {
        resetFacePriAnime();
    }

    field_0x2f96 = 0xFF;
    field_0x2f97 = 0xFF;
    deleteArrow();
    setBowNormalAnime();
    offNoResetFlg1(daPy_FLG1(FLG1_UNK_10000000 | FLG1_UNK_4000000 | FLG1_UNK_100000 | FLG1_UNK_40000 | FLG1_UNK_20000 | FLG1_UNK_10000 | FLG1_UNK_40 | FLG1_UNK_8 | FLG1_UNK_2));

    if (var_r29) {
        freeGrabItem();
    }

    offNoResetFlg0(FLG0_UNK_1000000);
    cancelItemUseQuake(0);
    return 1;
}

void daAlink_c::setUnderAnimeMorf(f32 i_morf) {
    if (i_morf >= 0.0f) {
        if (field_0x2060->getOldFrameRate() > 0.1f &&
            (field_0x2060->getOldFrameStartJoint() == 0 ||
             field_0x2060->getOldFrameStartJoint() == 1))
        {
            field_0x2060->initOldFrameMorf(i_morf, 0, 35);
        } else {
            field_0x2060->initOldFrameMorf(i_morf, 16, 35);
        }
    }
}

int daAlink_c::setUnderAnime(u16 i_resIdx, daAlink_c::daAlink_UNDER i_underIdx, f32 i_speed,
                             f32 i_startF, s16 i_endF, f32 i_morf) {
    J3DAnmTransform* bck = getAnimeResource(&mUnderAnmHeap[i_underIdx], i_resIdx, 0x2C00);
    if (bck == NULL) {
        bck = getNowAnmPackUnder(i_underIdx);
    }

    mNowAnmPackUnder[i_underIdx].setAnmTransform(bck);
    mNowAnmPackUnder[i_underIdx].setRatio(1.0f);
    mUnderAnmHeap[i_underIdx].setIdx(i_resIdx);

    if (i_endF < 0) {
        i_endF = bck->getFrameMax();
    }

    f32 frame;
    if (i_speed < 0.0f) {
        frame = i_endF;
    } else {
        frame = i_startF;
    }

    setFrameCtrl(&mUnderFrameCtrl[i_underIdx], bck->getAttribute(), i_startF, i_endF, i_speed, frame);
    bck->setFrame(frame);
    setUnderAnimeMorf(i_morf);
    return 1;
}

int daAlink_c::setUnderAnimeParam(u16 i_resIdx, daAlink_c::daAlink_UNDER i_underIdx,
                                  const daAlinkHIO_anm_c* i_anmParam) {
    return setUnderAnime(i_resIdx, i_underIdx, i_anmParam->mSpeed, i_anmParam->mStartFrame,
                         i_anmParam->mEndFrame, i_anmParam->mInterpolation);
}

int daAlink_c::resetUnderAnime(daAlink_c::daAlink_UNDER i_underIdx, f32 i_morf) {
    mNowAnmPackUnder[i_underIdx].setAnmTransform(NULL);
    mNowAnmPackUnder[i_underIdx].setRatio(0.0f);
    mUnderAnmHeap[i_underIdx].resetIdx();
    mUnderFrameCtrl[i_underIdx].init(0);
    setUnderAnimeMorf(i_morf);
    return 1;
}

void daAlink_c::setOldRootQuaternion(s16 param_0, s16 param_1, s16 param_2) {
    Quaternion quat;
    Quaternion quat2;
    Quaternion* old_frame_quat = field_0x2060->getOldFrameQuaternion(0);

    if (param_0 != 0 || param_1 != 0) {
        JMAEulerToQuat(param_0, param_1, 0, &quat);
        quat2 = *old_frame_quat;
        mDoMtx_QuatConcat(&quat, &quat2, old_frame_quat);
    }

    if (param_2 != 0) {
        JMAEulerToQuat(0, 0, param_2, &quat);
        quat2 = *old_frame_quat;
        mDoMtx_QuatConcat(&quat, &quat2, old_frame_quat);
    }
}

BOOL daAlink_c::checkAtnLeftAnime() {
    return checkUnderMove0BckNoArc(ANM_ATN_WAIT_LEFT) ||
           checkUnderMove0BckNoArc(ANM_ATN_WALK_LEFT) ||
           checkUnderMove0BckNoArc(ANM_ATN_RUN_LEFT) ||
           checkUnderMove0BckNoArc(ANM_ATN_LOCK_WALK_LEFT) ||
           checkUnderMove0BckNoArc(ANM_ATN_LOCK_RUN_LEFT);
}

BOOL daAlink_c::checkAtnRightAnime() {
    return checkUnderMove0BckNoArc(ANM_ATN_WAIT_RIGHT) ||
           checkUnderMove0BckNoArc(ANM_ATN_WALK_RIGHT) ||
           checkUnderMove0BckNoArc(ANM_ATN_RUN_RIGHT) ||
           checkUnderMove0BckNoArc(ANM_ATN_LOCK_WALK_RIGHT) ||
           checkUnderMove0BckNoArc(ANM_ATN_LOCK_RUN_RIGHT);
}

f32 daAlink_c::getMoveGroundAngleSpeedRate() {
    s16 gnd_angle;

    if (mGroundCode == 8 || checkMagneBootsOn() || !mLinkAcch.ChkGroundHit()) {
        gnd_angle = 0;
    } else {
        gnd_angle = getGroundAngle(&mLinkAcch.m_gnd, current.angle.y);
    }

    return fabsf((mNormalSpeed * cM_scos(gnd_angle)) / mMaxSpeed);
}

void daAlink_c::setBlendMoveAnime(f32 i_morf) {
    daPy_frameCtrl_c* framectrl0 = &mUnderFrameCtrl[0];
    daPy_frameCtrl_c* framectrl1 = &mUnderFrameCtrl[1];
    f32 var_f31 = getMoveGroundAngleSpeedRate();

    f32 sp2C;
    f32 sp28 = mpHIO->mMove.m.mFootPositionRatio;
    BOOL sp24 = checkEventRun();
    BOOL sp20 = checkBootsMoveAnime(1);

    f32 var_f29;

    if (sp20) {
        var_f29 = 1.0f;
        sp2C = 1.0f;
    } else {
        var_f29 = mpHIO->mMove.m.mWalkAnmSpeed;
        sp2C = mpHIO->mMove.m.mRunAnmSpeed;
    }

    if (field_0x2fb3 != 0) {
        field_0x2fb3--;
    }

    fopAc_ac_c* lockon_actor = mAttention->LockonTarget(0);

    daAlink_ANM var_r29;
    if (checkDkCaught2Anime()) {
        var_r29 = ANM_HORSE_WAIT_D_B;
    } else if ((!sp24 && ((lockon_actor != NULL && (checkEnemyGroup(lockon_actor) || checkGoatCatchActor(lockon_actor))) || field_0x311c != 0)) || checkEndResetFlg0(ERFLG0_BOSS_ROOM_WAIT) || checkBootsOrArmorHeavy()) {
        var_r29 = ANM_WAIT_B;
        field_0x2fb3 = 0x2D;
    } else if (!sp24 && (field_0x2fb3 != 0 || checkNoResetFlg3(FLG3_UNK_2000) || field_0x35c4.absXZ() > 1.0f || (checkUnderMove0BckNoArc(ANM_WAIT_B) && !framectrl0->checkPass(0.0f)))) {
        var_r29 = ANM_WAIT_B;
    } else {
        if (checkRestHPAnime() && !checkUpperGuardAnime()) {
            var_r29 = ANM_WAIT_TIRED;
        } else {
            var_r29 = ANM_WAIT;
        }
        field_0x2fb3 = 0;
    }

    daAlink_ANM var_r28 = ANM_WALK;
    daAlink_ANM sp18;
    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_38_e) {
        sp18 = ANM_RUN_B;
    } else {
        sp18 = ANM_RUN;
    }

    f32 var_f27 = 15.0f;
    f32 var_f26 = 3.0f;

    if (sp20) {
        if (checkBootsMoveAnime(0)) {
            var_r28 = ANM_WALK_HEAVY;
            sp28 = mpHIO->mItem.mIronBoots.m.mFeetPositionRatio;
            var_f31 *= 1.0f / (mHeavySpeedMultiplier * mHeavySpeedMultiplier);

            f32 sp14;
            if (checkHeavyStateOn(TRUE, FALSE) && mSinkShapeOffset < -30.0f) {
                sp14 = 0.5f * mpHIO->mItem.mIronBoots.m.mWalkAnmSpeedMax;
            } else {
                sp14 = mpHIO->mItem.mIronBoots.m.mWalkAnmSpeedMax;
            }

            var_f29 = mpHIO->mItem.mIronBoots.m.mWalkAnmSpeedMin + (var_f31 * (sp14 - mpHIO->mItem.mIronBoots.m.mWalkAnmSpeedMin));
            var_f27 = 0.0f;
            var_f26 = 23.0f;
        } else {
            var_r29 = ANM_IRONBALL_WAIT;
            var_r28 = ANM_IRONBALL_WALK;
            sp28 = mpHIO->mItem.mIronBall.m.mFeetPosRatio;
            var_f31 = var_f31 * (1.0f / (mHeavySpeedMultiplier * mHeavySpeedMultiplier));
            var_f29 = mpHIO->mItem.mIronBall.m.mWalkAnmSpeedMin + (var_f31 * (mpHIO->mItem.mIronBall.m.mWalkAnmSpeedMax - mpHIO->mItem.mIronBall.m.mWalkAnmSpeedMin));
            var_f27 = 18.0f;
            var_f26 = 3.0f;
        }

        var_f31 = var_f31 * mpHIO->mMove.m.mWalkChangeRate;
        if (var_f31 >= mpHIO->mMove.m.mWalkChangeRate) {
            var_f31 = mpHIO->mMove.m.mWalkChangeRate - 0.001f;
        }
    } else if (checkSlope()) {
        var_r28 = ANM_WALK_SLOPE;
        sp18 = ANM_WALK_SLOPE;
        var_f29 = mpHIO->mSlide.m.mClimbAnmMinSpeed + (var_f31 * (mpHIO->mSlide.m.mMaxClimbAnmSpeed - mpHIO->mSlide.m.mClimbAnmMinSpeed));
        sp2C = var_f29;
    }

    int sp10;
    f32 var_f28;
    f32 var_f30;

    if (var_f31 < mpHIO->mMove.m.mWalkChangeRate) {
        var_f28 = var_f31 / mpHIO->mMove.m.mWalkChangeRate;

        if (checkModeFlg(1) || checkZeroSpeedF()) {
            mSpeedModifier = 0.0f;

            if (checkUpperGuardAnime() || (mEquipItem == 0x102 && checkAttentionLock()) || checkFmChainGrabAnime() || checkSmallUpperGuardAnime() || checkCopyRodThrowAnime() || checkBoomerangThrowAnime()) {
                if ((!checkBoomerangThrowAnime() && !checkCopyRodThrowAnime() && (mEquipItem != 0x102 || !checkAttentionLock())) && (field_0x2f98 == 2 || checkAtnLeftAnime())) {
                    var_r29 = ANM_ATN_WAIT_LEFT;
                    if (mTargetedActor != NULL) {
                        var_r28 = ANM_ATN_LOCK_WALK_LEFT;
                    } else {
                        var_r28 = ANM_ATN_WALK_LEFT;
                    }
                } else {
                    var_r29 = ANM_ATN_WAIT_RIGHT;
                    if (mTargetedActor != NULL) {
                        var_r28 = ANM_ATN_LOCK_WALK_RIGHT;
                    } else {
                        var_r28 = ANM_ATN_WALK_LEFT;
                    }
                }

                if (mTargetedActor != NULL) {
                    var_f30 = mpHIO->mAtnMove.m.mWaitAnmSpeed;
                    var_f29 = mpHIO->mAtnMove.m.mWalkAnmSpeed;
                } else {
                    var_f30 = mpHIO->mNoActAtnMove.m.mWaitAnmSpeed;
                    var_f29 = mpHIO->mNoActAtnMove.m.mWalkAnmSpeed;
                }

                if (var_r29 == ANM_IRONBALL_WAIT) {
                    var_f30 = mpHIO->mItem.mIronBall.m.mWaitAnmSpeed;
                }

                setDoubleAnime(0.0f, var_f30, var_f29, var_r29, var_r28, 2, i_morf);
                return;
            }

            sp10 = 2;
            s16 sp8 = (shape_angle.y - mPrevAngleY);

            if (abs(sp8) > 100 && mStickValue < 0.3f && checkInputOnR() && field_0x33a4 > 0.05f && field_0x33a4 < 0.3f && !checkAttentionLock()) {
                if (var_r29 == ANM_WAIT_TIRED) {
                    if (sp8 > 0) {
                        var_r28 = ANM_STEP_TURN_TIRED;
                    } else {
                        var_r28 = ANM_SMALL_GUARD_TIRED;
                    }

                    var_f30 = 1.0f;
                } else {
                    if (var_r29 == ANM_IRONBALL_WAIT) {
                        var_r29 = ANM_WAIT;
                    }

                    if (sp8 > 0) {
                        var_r28 = ANM_STEP_TURN;
                    } else {
                        var_r28 = ANM_SMALL_GUARD;
                    }

                    var_f30 = mpHIO->mMove.m.mWaitAnmSpeed;
                }

                if (sp8 > 0) {
                    var_f27 = 8.0f;
                    var_f26 = 19.0f;
                } else {
                    var_f26 = 8.0f;
                    var_f27 = 19.0f;
                }

                var_f29 = mpHIO->mBasic.m.mRotationASpeed;
                var_f28 = 0.85f + (0.001f * abs(sp8));
                if (var_f28 > 1.0f) {
                    var_f28 = 1.0f;
                }
                sp10 = 1;
            } else if (checkWindSpeedMoveXZ() && !sp24 && (var_r29 == ANM_WAIT || var_r29 == ANM_WAIT_TIRED || var_r29 == ANM_WAIT_B)) {
                var_r29 = ANM_WAIT_WIND;
                var_f30 = 1.0f;
            } else if (!sp24 && (checkEndResetFlg1(ERFLG1_UNK_40000) || (!checkSnowCode() && mSinkShapeOffset < -30.0f))) {
                var_r29 = ANM_WAIT_INSECT;
                voiceStartLevel(Z2SE_AL_V_INSECT_LOOP);
                var_f30 = 1.0f;
            } else {
                var_f30 = mpHIO->mMove.m.mWaitAnmSpeed;
                if (var_r29 == ANM_WAIT_TIRED) {
                    if (checkUnderMove1BckNoArc(ANM_STEP_TURN)) {
                        var_r28 = ANM_STEP_TURN_TIRED;
                        i_morf = 3.0f;
                    } else if (checkUnderMove1BckNoArc(ANM_SMALL_GUARD)) {
                        var_r28 = ANM_SMALL_GUARD_TIRED;
                        i_morf = 3.0f;
                    }
                }
            }
        } else {
            f32 spC;
            if (var_r29 == ANM_WAIT_TIRED) {
                spC = mpHIO->mMove.m.mMinTiredWalkRate;
            } else {
                spC = mpHIO->mMove.m.mMinWalkRate;
            }

            var_f28 = spC + (var_f28 * (1.0f - spC));
            mSpeedModifier = 1.0f - ((1.0f - sp28) * var_f28);
            sp10 = 1;
            var_f30 = mpHIO->mMove.m.mWaitAnmSpeed;
        }

        if (var_r29 == ANM_IRONBALL_WAIT) {
            var_f30 = mpHIO->mItem.mIronBall.m.mWaitAnmSpeed;
        } else if (var_r29 == ANM_HORSE_WAIT_D_B || var_r29 == ANM_WAIT_TIRED) {
            var_f30 = 1.0f;
        }

        setDoubleAnime(var_f28, var_f30, var_f29,var_r29, var_r28, sp10, i_morf);

        if (var_r28 == ANM_WALK_HEAVY && ((field_0x2f8c == 2 && checkWindSpeedOnXZ()) || checkWindSpeedOnAngle())) {
            onEndResetFlg0(ERFLG0_UNK_2000);

            if (checkNoSetUpperAnime()) {
                setUpperAnimeBase(dRes_ID_ALANM_BCK_DASHWIND_e);
                field_0x2f96 = 1;
                field_0x2f97 = 6;
            }
        }
    } else if (var_f31 < mpHIO->mMove.m.mRunChangeRate) {
        var_f28 = (var_f31 - mpHIO->mMove.m.mWalkChangeRate) / (mpHIO->mMove.m.mRunChangeRate - mpHIO->mMove.m.mWalkChangeRate);
        setDoubleAnime(var_f28, var_f29, sp2C, var_r28, sp18, 1, i_morf);

        mSpeedModifier = sp28 * (1.0f - var_f28);
        if (checkNoResetFlg0(FLG0_WATER_IN_MOVE) && var_f28 > 0.5f) {
            if (framectrl1->checkPass(var_f27)) {
                field_0x2f9d = 0x20;
            } else if (framectrl1->checkPass(var_f26)) {
                field_0x2f9d = 0x40;
            }
        }
    } else {
        setDoubleAnime(1.0f, sp2C, sp2C, sp18, sp18, 1, i_morf);
        mSpeedModifier = 0.0f;

        if (framectrl1->checkPass(var_f27)) {
            field_0x2f9d = 0x20;
        } else if (framectrl1->checkPass(var_f26)) {
            field_0x2f9d = 0x40;
        }

        if (checkWindSpeedOnAngle()) {
            onEndResetFlg0(ERFLG0_UNK_2000);

            if (checkNoSetUpperAnime()) {
                setUpperAnimeBase(dRes_ID_ALANM_BCK_DASHWIND_e);
                field_0x2f96 = 1;
                field_0x2f97 = 6;
            }
        }
    }

    if (field_0x2f8c != 2) {
        if (framectrl1->checkPass(var_f27)) {
            onResetFlg1(RFLG1_UNK_10);

            if (checkBootsOrArmorHeavy()) {
                dComIfGp_getVibration().StartShock(1, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            }
        } else if (framectrl1->checkPass(var_f26)) {
            onResetFlg1(RFLG1_UNK_20);

            if (checkBootsOrArmorHeavy()) {
                dComIfGp_getVibration().StartShock(1, 0xF,cXyz(0.0f, 1.0f, 0.0f));
            }
        }
    }

    if (var_f31 < 0.69999999f) {
        setHandIndex(var_r29);
    } else {
        setHandIndex(sp18);
    }
}

void daAlink_c::setBlendAtnMoveAnime(f32 i_morf) {
    s16 spA = (current.angle.y - shape_angle.y);
    f32 sp44 = cM_ssin(spA);
    f32 sp40 = cM_scos(spA);
    u8 sp8 = field_0x2f98;
    BOOL sp3C = 0;

    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_23_e) {
        if (mDemo.getParam1() & 1) {
            field_0x2f98 = 2;
        } else {
            field_0x2f98 = 3;
        }

        if (!checkWolf() && !checkEquipAnime() && mEquipItem != 0x103 && (mDemo.getParam1() == 2 || mDemo.getParam1() == 3)) {
            swordEquip(0);
        }
    } else if (checkInputOnR()) {
        if (mTargetedActor == NULL && !checkGoatStopGame() && (sp40 <= -0.99f || sp40 >= 0.99f)) {
            if (sp40 <= -0.99f) {
                field_0x2f98 = 1;
            } else {
                field_0x2f98 = 0;
                sp3C = 1;
            }
        } else {
            if (checkFmChainGrabAnime()) {
                mMaxSpeed = 13.0f;
            } else if (mTargetedActor != NULL) {
                mMaxSpeed = mpHIO->mAtnMove.m.mMaxSpeed;
            } else {
                mMaxSpeed = mpHIO->mNoActAtnMove.m.mMaxSpeed;
            }

            if (mTargetedActor != NULL && sp40 >= 0.866f) {
                field_0x2f98 = 0;
            } else if (sp40 < -0.866f) {
                field_0x2f98 = 1;
            } else if (sp44 > 0.0f) {
                field_0x2f98 = 2;
            } else if (sp44 < 0.0f) {
                field_0x2f98 = 3;
            }
        }
    } else if (checkModeFlg(1) && (checkGrabAnimeUp() || checkGrabAnimeSide() || field_0x2f98 == 0 || field_0x2f98 == 1)) {
        field_0x2f98 = 3;
    }

    daAlink_ANM var_r28;
    daAlink_ANM var_r29;

    if (checkSetChainPullAnime(shape_angle.y)) {
        daAlink_ANM var_r27;
        if (checkEndResetFlg1(ERFLG1_CHAIN_PULL_END)) {
            var_r27 = ANM_CHAIN_PULL_END;
        } else {
            var_r27 = ANM_CHAIN_PULL;
        }

        if (!checkUnderMove0BckNoArc(var_r27)) {
            f32 sp38;
            if (checkChainBlockPushPull()) {
                sp38 = 0.7f;
            } else {
                sp38 = 1.0f;
            }

            setSingleAnimeBaseSpeed(var_r27, sp38, 3.0f);
        }

        mSpeedModifier = 0.0f;
        return;
    }

    if (field_0x2f98 == 1) {
        setBlendAtnBackMoveAnime(i_morf);
        return;
    }

    if (sp3C) {
        mMaxSpeed = mpHIO->mMove.m.mMaxSpeed;
        setBlendMoveAnime(i_morf);
        return;
    }

    f32 sp34 = getMoveGroundAngleSpeedRate();

    f32 var_f31;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 var_f30;

    if (mTargetedActor != NULL) {
        sp30 = mpHIO->mAtnMove.m.mMinWalkFrame;
        sp2C = mpHIO->mAtnMove.m.mWalkChangeRate;
        sp28 = mpHIO->mAtnMove.m.mRunChangeRate;
        sp24 = mpHIO->mAtnMove.m.mWaitAnmSpeed;
        sp20 = mpHIO->mAtnMove.m.mWalkAnmSpeed;
        var_f30 = mpHIO->mAtnMove.m.mRunAnmSpeed;
    } else {
        sp30 = mpHIO->mNoActAtnMove.m.mMinWalkFrame;
        sp2C = mpHIO->mNoActAtnMove.m.mWalkChangeRate;
        sp28 = mpHIO->mNoActAtnMove.m.mRunChangeRate;
        sp24 = mpHIO->mNoActAtnMove.m.mWaitAnmSpeed;
        sp20 = mpHIO->mNoActAtnMove.m.mWalkAnmSpeed;
        var_f30 = mpHIO->mNoActAtnMove.m.mRunAnmSpeed;
    }

    if (sp34 < sp2C) {
        var_f31 = sp34 / sp2C;

        if (field_0x2f98 == 0) {
            var_r28 = ANM_ATN_WAIT_RIGHT;
            var_r29 = ANM_ATN_RETURN_FROM_WALK;
            sp20 = mpHIO->mAtnMove.m.mWalkForwardAnmSpeed;
        } else if (field_0x2f98 == 2) {
            var_r28 = ANM_ATN_WAIT_LEFT;
            if (mTargetedActor != NULL) {
                var_r29 = ANM_ATN_LOCK_WALK_LEFT;
            } else {
                var_r29 = ANM_ATN_WALK_LEFT;
            }
        } else {
            var_r28 = ANM_ATN_WAIT_RIGHT;
            if (mTargetedActor != NULL) {
                var_r29 = ANM_ATN_LOCK_WALK_RIGHT;
            } else {
                var_r29 = ANM_ATN_WALK_RIGHT;
            }
        }

        int sp1C;
        if (checkModeFlg(1) || checkZeroSpeedF()) {
            sp1C = 2;
            mSpeedModifier = 0.0f;
        } else {
            var_f31 = sp30 + (var_f31 * (1.0f - sp30));
            sp1C = 3;
            mSpeedModifier = 1.0f;
        }

        setDoubleAnime(var_f31, sp24, sp20, var_r28, var_r29, sp1C, i_morf);
    } else if (sp34 < sp28) {
        var_f31 = (sp34 - sp2C) / (sp28 - sp2C);

        if (field_0x2f98 == 0) {
            var_r28 = ANM_ATN_RETURN_FROM_WALK;
            var_r29 = ANM_ATN_RETURN_FROM_RUN;
            sp20 = mpHIO->mAtnMove.m.mWalkForwardAnmSpeed;
            var_f30 = mpHIO->mAtnMove.m.mRunForwardAnmSpeed;
        } else if (field_0x2f98 == 2) {
            if (mTargetedActor != NULL) {
                var_r28 = ANM_ATN_LOCK_WALK_LEFT;
                var_r29 = ANM_ATN_LOCK_RUN_LEFT;
            } else {
                var_r28 = ANM_ATN_WALK_LEFT;
                var_r29 = ANM_ATN_RUN_LEFT;
            }
        } else if (mTargetedActor != NULL) {
            var_r28 = ANM_ATN_LOCK_WALK_RIGHT;
            var_r29 = ANM_ATN_LOCK_RUN_RIGHT;
        } else {
            var_r28 = ANM_ATN_WALK_RIGHT;
            var_r29 = ANM_ATN_RUN_RIGHT;
        }

        setDoubleAnime(var_f31, sp20, var_f30, var_r28, var_r29, 3, i_morf);
        mSpeedModifier = 1.0f - var_f31;
    } else {
        if (field_0x2f98 == 0) {
            var_r29 = ANM_ATN_RETURN_FROM_RUN;
            var_f30 = mpHIO->mAtnMove.m.mRunForwardAnmSpeed;
        } else if (field_0x2f98 == 2) {
            if (mTargetedActor != NULL) {
                var_r29 = ANM_ATN_LOCK_RUN_LEFT;
            } else {
                var_r29 = ANM_ATN_RUN_LEFT;
            }
        } else if (mTargetedActor != NULL) {
            var_r29 = ANM_ATN_LOCK_RUN_RIGHT;
        } else {
            var_r29 = ANM_ATN_RUN_RIGHT;
        }

        setDoubleAnime(1.0f, var_f30, var_f30, var_r29, var_r29, 3, i_morf);
        mSpeedModifier = 0.0f;
    }

    if (field_0x2f8c != 2) {
        daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

        f32 sp14;
        f32 sp10;
        f32 spC;
        if (field_0x2f98 == 0) {
            sp10 = 0.0f;
            sp14 = 11.0f;
        } else {
            if (mTargetedActor != NULL) {
                sp10 = 6.0f;
                sp14 = 11.0f;
            } else {
                sp10 = 9.0f;
                sp14 = 1.0f;
            }

            if (field_0x2f98 != 2) {
                spC = sp14;
                sp14 = sp10;
                sp10 = spC;
            }
        }

        if (framectrl->checkPass(sp14)) {
            onResetFlg1(RFLG1_UNK_10);
        } else if (framectrl->checkPass(sp10)) {
            onResetFlg1(RFLG1_UNK_20);
        }

        if (checkBootsOrArmorHeavy() && sp34 > 0.1f && checkResetFlg1(RFLG1_UNK_30)) {
            dComIfGp_getVibration().StartShock(1, 0xF, cXyz(0.0f, 1.0f, 0.0f));
        }
    }
}

void daAlink_c::setBlendAtnBackMoveAnime(f32 i_morf) {
    f32 var_f27;
    f32 var_f31;
    f32 var_f26;
    f32 var_f24;
    f32 var_f25;
    f32 var_f29;
    daAlink_ANM var_r26;
    daAlink_ANM var_r27;
    daAlink_ANM var_r29;

    if (checkBootsMoveAnime(1)) {
        mMaxSpeed = mpHIO->mAtnMove.m.mMaxBackwardsSpeed;
        var_f27 = mpHIO->mAtnMove.m.mMinBackWalkFrame;
        var_f31 = mpHIO->mAtnMove.m.mBackWalkChangeRate;
        var_f26 = mpHIO->mAtnMove.m.mBackRunChangeRate;
        var_f24 = mpHIO->mAtnMove.m.mWaitAnmSpeed;
        var_f25 = 1.0f;
        var_f29 = 1.0f;
        var_r26 = ANM_ATN_WAIT_RIGHT;
        var_r27 = ANM_ATN_BACK_WALK_HEAVY;
        var_r29 = ANM_ATN_BACK_WALK_HEAVY;
    } else if (mTargetedActor != NULL) {
        mMaxSpeed = mpHIO->mAtnMove.m.mMaxBackwardsSpeed;
        var_f27 = mpHIO->mAtnMove.m.mMinBackWalkFrame;
        var_f31 = mpHIO->mAtnMove.m.mBackWalkChangeRate;
        var_f26 = mpHIO->mAtnMove.m.mBackRunChangeRate;
        var_f24 = mpHIO->mAtnMove.m.mWaitAnmSpeed;
        var_f25 = mpHIO->mAtnMove.m.mBackWalkAnmSpeed;
        var_f29 = mpHIO->mAtnMove.m.mBackRunAnmSpeed;
        var_r26 = ANM_ATN_WAIT_RIGHT;
        var_r27 = ANM_ATN_LOCK_BACK_WALK;
        var_r29 = ANM_ATN_LOCK_BACK_RUN;
    } else {
        mMaxSpeed = mpHIO->mNoActAtnMove.m.mMaxBackwardsSpeed;
        var_f27 = mpHIO->mNoActAtnMove.m.mMinBackWalkFrame;
        var_f31 = mpHIO->mNoActAtnMove.m.mBackWalkChangeRate;
        var_f26 = mpHIO->mNoActAtnMove.m.mBackRunChangeRate;
        var_f24 = mpHIO->mMove.m.mWaitAnmSpeed;
        var_f25 = mpHIO->mNoActAtnMove.m.mBackWalkAnmSpeed;
        var_f29 = mpHIO->mNoActAtnMove.m.mBackRunAnmSpeed;
        var_r26 = ANM_WAIT;
        var_r27 = ANM_ATN_BACK_WALK;
        var_r29 = ANM_ATN_BACK_RUN;
    }

    f32 var_f30;

    f32 temp_f1 = getMoveGroundAngleSpeedRate();
    if (temp_f1 < var_f31) {
        var_f30 = temp_f1 / var_f31;

        int var_r25;
        if (checkModeFlg(1) || checkZeroSpeedF()) {
            var_r25 = 2;
            mSpeedModifier = 0.0f;
        } else {
            var_r25 = 3;
            mSpeedModifier = 1.0f;
            var_f30 = var_f27 + (var_f30 * (1.0f - var_f27));
        }

        setDoubleAnime(var_f30, var_f24, var_f25, var_r26, var_r27, var_r25, i_morf);
    } else if (temp_f1 < var_f26) {
        var_f30 = (temp_f1 - var_f31) / (var_f26 - var_f31);
        setDoubleAnime(var_f30, var_f25, var_f29, var_r27, var_r29, 3, i_morf);
        mSpeedModifier = 1.0f - var_f30;
    } else {
        setDoubleAnime(1.0f, var_f29, var_f29, var_r29, var_r29, 3, i_morf);
        mSpeedModifier = 0.0f;
    }

    if (field_0x2f8c != 2) {
        daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[0];

        if (var_r29 == ANM_ATN_LOCK_BACK_RUN) {
            if (framectrl->checkPass(11.0f)) {
                onResetFlg1(RFLG1_UNK_10);
            } else if (framectrl->checkPass(8.0f)) {
                onResetFlg1(RFLG1_UNK_20);
            }
        } else if (framectrl->checkPass(0.0f)) {
            onResetFlg1(RFLG1_UNK_10);
        } else if (framectrl->checkPass(0.5f * framectrl->getEnd())) {
            onResetFlg1(RFLG1_UNK_20);
        }
    }

    if (checkBootsOrArmorHeavy() && temp_f1 > 0.1f && checkResetFlg1(RFLG1_UNK_30)) {
        dComIfGp_getVibration().StartShock(1, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    }
}

void daAlink_c::setFaceBck(u16 i_resIdx, BOOL i_isPriIdx, u16 i_arcNo) {
    if (!checkNoResetFlg0(FLG0_SWIM_UP) && mProcID != PROC_GET_ITEM) {
        i_resIdx = dRes_ID_ALANM_BCK_FSWIMINGB_e;
    } else if (i_resIdx == 0) {
        fopAc_ac_c* lockon_actor = mAttention->LockonTarget(0);
        if (checkAttentionState() || (checkHorseRide() && checkEnemyGroup(lockon_actor)) || checkEndResetFlg0(ERFLG0_NECK_SEARCH_WIDE)) {
            i_resIdx = dRes_ID_ALANM_BCK_FAT_e;
        } else {
            i_resIdx = dRes_ID_ALANM_BCK_FA_e;
        }
    } else {
        setIdxMask(&i_arcNo, &i_resIdx);
    }

    J3DAnmTransform* bck;
    if (i_arcNo != 0xFFFF) {
        bck = (J3DAnmTransform*)mFaceBckHeap.loadDataDemoRID(i_resIdx, i_arcNo);
    } else if (i_isPriIdx) {
        bck = (J3DAnmTransform*)mFaceBckHeap.loadDataPriIdx(i_resIdx);
    } else {
        bck = (J3DAnmTransform*)mFaceBckHeap.loadDataIdx(i_resIdx);
    }

    if (bck != NULL) {
        mFaceBck.init(bck, FALSE, bck->getAttribute(), 1.0f, 0, -1, 1);
        bck->setFrame(0.0f);
    }
}

void daAlink_c::setFaceBtp(u16 i_resIdx, BOOL i_isPriIdx, u16 i_arcNo) {
    setIdxMask(&i_arcNo, &i_resIdx);

    J3DAnmTexPattern* btp;
    if (i_arcNo != 0xFFFF) {
        btp = (J3DAnmTexPattern*)mFaceBtpHeap.loadDataDemoRID(i_resIdx, i_arcNo);
    } else if (i_isPriIdx) {
        btp = (J3DAnmTexPattern*)mFaceBtpHeap.loadDataPriIdx(i_resIdx);
    } else {
        btp = (J3DAnmTexPattern*)mFaceBtpHeap.loadDataIdx(i_resIdx);
    }

    if (btp != NULL) {
        mpFaceBtp = btp;
        mpFaceBtp->searchUpdateMaterialID(field_0x06c0);
        field_0x06c0->entryTexNoAnimator(mpFaceBtp);

        if (i_arcNo == 0xFFFF) {
            if (i_resIdx == dRes_ID_ALANM_BTP_FMABA03_e || (i_resIdx == dRes_ID_ALANM_BTP_WL_FMABA01_e && checkUnderMove0BckNoArcWolf(WANM_WAIT_WIND))) {
                field_0x3440 = 0.02f;
                onNoResetFlg1(FLG1_UNK_2000);
            } else if (i_resIdx == dRes_ID_ALANM_BTP_WL_FMABA01_e
                        || i_resIdx == dRes_ID_ALANM_BTP_WL_FMABA02_e
                        || i_resIdx == dRes_ID_ALANM_BTP_FMABAGOMA_e
                        || i_resIdx == dRes_ID_ALANM_BTP_FMABA01_e
                        || i_resIdx == dRes_ID_ALANM_BTP_FMABA02_e)
            {
                onNoResetFlg1(FLG1_UNK_2000);
                field_0x3440 = 0.012f;
            } else {
                offNoResetFlg1(FLG1_UNK_2000);
            }
        } else {
            offNoResetFlg1(FLG1_UNK_2000);
        }

        field_0x2fea = 0;
        mpFaceBtp->setFrame(0.0f);
    } else if (i_arcNo == 0xFFFF && i_resIdx == dRes_ID_ALANM_BTP_WL_FMABA01_e) {
        if (checkUnderMove0BckNoArcWolf(WANM_WAIT_WIND)) {
            field_0x3440 = 0.02f;
        } else {
            field_0x3440 = 0.012f;
        }
    }
}

void daAlink_c::setFaceBtk(u16 i_resIdx, BOOL i_isPriIdx, u16 i_arcNo) {
    setIdxMask(&i_arcNo, &i_resIdx);

    J3DAnmTextureSRTKey* btk;
    if (i_arcNo != 0xFFFF) {
        btk = (J3DAnmTextureSRTKey*)mFaceBtkHeap.loadDataDemoRID(i_resIdx, i_arcNo);
        offNoResetFlg1(FLG1_UNK_2000);
    } else if (i_isPriIdx) {
        btk = (J3DAnmTextureSRTKey*)mFaceBtkHeap.loadDataPriIdx(i_resIdx);
    } else {
        btk = (J3DAnmTextureSRTKey*)mFaceBtkHeap.loadDataIdx(i_resIdx);
    }

    if (btk != NULL) {
        mpFaceBtk = btk;
        mpFaceBtk->searchUpdateMaterialID(field_0x06c0);
        field_0x06c0->entryTexMtxAnimator(mpFaceBtk);
        daAlink_matAnm_c::setMorfFrame(3);
        mpFaceBtk->setFrame(0.0f);
    }
}

daAlink_c::daAlink_FTANM daAlink_c::setFaceBasicTexture(daAlink_c::daAlink_FTANM i_ftanm) {
    if (checkWindSpeedOnAngleAnime(0)) {
        if (i_ftanm == FTANM_0) {
            i_ftanm = FTANM_DASHWIND;
        } else if (i_ftanm == FTANM_WL_MABA01) {
            i_ftanm = FTANM_WL_DASHWIND;
        }
    } else if ((!checkPlayerDemoMode() && dComIfGs_getLife() <= 4) || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_18_e) {
        if (i_ftanm == FTANM_0) {
            i_ftanm = FTANM_MABA01;
        } else if (i_ftanm == FTANM_WL_MABA01) {
            i_ftanm = FTANM_WL_MABA02;
        }
    }

    if (checkWolf() && i_ftanm == FTANM_0) {
        i_ftanm = FTANM_WL_MABA01;
    }

    setFaceBasicBtp(getFaceTexData(i_ftanm)->m_btpID);
    setFaceBasicBtk(getFaceTexData(i_ftanm)->m_btkID);
    return i_ftanm;
}

void daAlink_c::setFaceBasicAnime(daAlink_c::daAlink_ANM i_anm) {
    const daAlink_AnmData* anmData = getAnmData(i_anm);

    daAlink_FTANM ftanm = setFaceBasicTexture((daAlink_c::daAlink_FTANM)anmData->m_faceTexID);
    if ((ftanm != anmData->m_faceTexID) && !checkWolf()) {
        if (ftanm == FTANM_DASHWIND) {
            setFaceBasicBck(dRes_ID_ALANM_BCK_FDASHWIND_e);
        } else if (ftanm == FTANM_MABA01) {
            setFaceBasicBck(dRes_ID_ALANM_BCK_FD_e);
        } else if (ftanm == FTANM_MABA01_R) {
            setFaceBasicBck(dRes_ID_ALANM_BCK_FGOMA_e);
        } else {
            setFaceBasicBck(anmData->m_faceBckID);
        }
    } else {
        setFaceBasicBck(anmData->m_faceBckID);
    }
}

void daAlink_c::setFacePriTexture(daAlink_c::daAlink_FTANM i_ftanm) {
    setFacePriBtp(getFaceTexData(i_ftanm)->m_btpID);
    setFacePriBtk(getFaceTexData(i_ftanm)->m_btkID);
}

void daAlink_c::setFacePriAnime(daAlink_c::daAlink_ANM i_anm) {
    const daAlink_AnmData* anm_data = getAnmData(i_anm);

    setFacePriTexture((daAlink_FTANM)anm_data->m_faceTexID);
    setFacePriBck(anm_data->m_faceBckID);
}

void daAlink_c::resetFacePriBck() {
    if (!mFaceBckHeap.checkNoSetPriIdx()) {
        mFaceBckHeap.resetPriIdx();

        u16 resIdx = mFaceBckHeap.getIdx();
        if (resIdx == 0xFFFF) {
            resIdx = dRes_ID_ALANM_BCK_FA_e;
        }

        mFaceBckHeap.resetIdx();
        setFaceBasicBck(resIdx);
    }
}

void daAlink_c::resetFacePriBtp() {
    if (!mFaceBtpHeap.checkNoSetPriIdx()) {
        mFaceBtpHeap.resetPriIdx();

        u16 resIdx = mFaceBtpHeap.getIdx();
        if (resIdx == 0xFFFF) {
            resIdx = dRes_ID_ALANM_BTP_FMABA01_e;
        }

        mFaceBtpHeap.resetIdx();
        setFaceBasicBtp(resIdx);
    }
}

void daAlink_c::resetFacePriBtk() {
    if (!mFaceBtkHeap.checkNoSetPriIdx()) {
        mFaceBtkHeap.resetPriIdx();

        u16 resIdx = mFaceBtkHeap.getIdx();
        if (resIdx == 0xFFFF) {
            resIdx = dRes_ID_ALANM_BTK_FMABA01_e;
        }

        mFaceBtkHeap.resetIdx();
        setFaceBasicBtk(resIdx);
    }
}

void daAlink_c::resetFacePriTexture() {
    resetFacePriBtp();
    resetFacePriBtk();
}

void daAlink_c::resetFacePriAnime() {
    resetFacePriTexture();
    resetFacePriBck();
}

void daAlink_c::playFaceTextureAnime() {
    if (checkNoResetFlg1(FLG1_UNK_200) && !mFaceBtpHeap.checkNoSetPriIdx()) {
        simpleAnmPlay(mpFaceBtp);
        simpleAnmPlay(mpFaceBtk);
    } else {
        u16 btp_frame;
        if (!mFaceBtpHeap.checkNoSetPriIdx()) {
            btp_frame = mUpperFrameCtrl[2].getFrame();
        } else if (checkUnderMove0BckNoArc(ANM_HAWK_WAIT)) {
            btp_frame = mpFaceBtk->getFrameMax();
        } else if (!checkNoResetFlg1(FLG1_UNK_2000)) {
            if (getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
                btp_frame = mUpperFrameCtrl[0].getFrame();
            } else {
                btp_frame = mUnderFrameCtrl[0].getFrame();
            }
        } else {
            int var_r24 = mpFaceBtp->getFrameMax() > mpFaceBtk->getFrameMax() ? mpFaceBtp->getFrameMax() : mpFaceBtk->getFrameMax();

            if (field_0x2fea != 0) {
                field_0x2fea++;
                if (field_0x2fea > var_r24) {
                    field_0x2fea = 0;
                }
            } else if (cM_rnd() < field_0x3440) {
                field_0x2fea++;
            }

            btp_frame = field_0x2fea;
        }

        u16 btk_frame = btp_frame;

        if (btp_frame > mpFaceBtp->getFrameMax()) {
            btp_frame = mpFaceBtp->getFrameMax();
        }

        if (btk_frame > mpFaceBtk->getFrameMax()) {
            btk_frame = mpFaceBtk->getFrameMax();
        }

        mpFaceBtp->setFrame(btp_frame);
        mpFaceBtk->setFrame(btk_frame);
    }

    if (!checkWolf()) {
        J3DAnmTransform* bck = mFaceBck.getBckAnm();

        if (checkNoResetFlg1(FLG1_UNK_200)) {
            if (!mFaceBckHeap.checkNoSetPriIdx()) {
                simpleAnmPlay(bck);
                return;
            }
        }

        f32 frame;
        if (checkUnderMove0BckNoArc(ANM_HAWK_WAIT)) {
            frame = bck->getFrameMax();
        } else if (mFaceBckHeap.checkNoSetPriIdx()) {
            u16 resIdx = mFaceBckHeap.getIdx();

            if (mFaceBckHeap.checkNoSetArcNo()) {
                if (resIdx == dRes_ID_ALANM_BCK_FAT_e) {
                    frame = 1.0f;
                } else if (resIdx == dRes_ID_ALANM_BCK_FSWIMINGB_e
                            || resIdx == dRes_ID_ALANM_BCK_FD_e
                            || (checkModeFlg(0x400) && resIdx == dRes_ID_ALANM_BCK_FWAITD_e))
                {
                    frame = 1.0f + bck->getFrame();
                    if (frame >= bck->getFrameMax()) {
                        frame -= bck->getFrameMax();
                    }
                } else {
                    if (getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
                        frame = mUpperFrameCtrl[0].getFrame();
                    } else {
                        frame = mUnderFrameCtrl[0].getFrame();
                    }
                }
            } else {
                if (getNowAnmPackUnder(UNDER_0) != getNowAnmPackUpper(UPPER_0)) {
                    frame = mUpperFrameCtrl[0].getFrame();
                } else {
                    frame = mUnderFrameCtrl[0].getFrame();
                }
            }
        } else {
            frame = mUpperFrameCtrl[2].getFrame();
        }

        if (frame > bck->getFrameMax()) {
            frame = bck->getFrameMax();
        }

        if (mClothesChangeWaitTimer == 0 && !checkZoraWearMaskDraw()) {
            mFaceBck.entry(mpLinkFaceModel->getModelData(), frame);
        }
    }
}

s16 daAlink_c::getGroundAngle(cBgS_PolyInfo* i_polyinfo, s16 i_angle) {
    if (!dComIfG_Bgsp().ChkPolySafe(*i_polyinfo)) {
        return 0;
    }

    cM3dGPla tripla;
    if (!dComIfG_Bgsp().GetTriPla(*i_polyinfo, &tripla) || !cBgW_CheckBGround(tripla.mNormal.y)) {
        return 0;
    }

    return fopAcM_getPolygonAngle(&tripla, i_angle);
}

s16 daAlink_c::getRoofAngle(cBgS_PolyInfo* i_polyinfo, s16 i_angle) {
    cM3dGPla tripla;

    if (!dComIfG_Bgsp().GetTriPla(*i_polyinfo, &tripla) || !dComIfG_Bgsp().ChkPolySafe(*i_polyinfo)) {
        return 0;
    }

    return fopAcM_getPolygonAngle(&tripla, i_angle) + 0x8000;
}

BOOL daAlink_c::getWallEdgePos(cXyz const& param_0, cM3dGPla* i_planeA, cM3dGPla* i_planeB,
                               cXyz* o_outVec, int unused) {
    UNUSED(unused);

    cXyz normal(i_planeA->mNormal.z, 0.0f, -i_planeA->mNormal.x);
    normal.normalizeZP();

    cM3dGPla planeC(&normal, -((normal.z * param_0.z) + ((normal.x * param_0.x) + (normal.y * param_0.y))));

    return cM3d_3PlaneCrossPos(*i_planeA, *i_planeB, planeC, o_outVec);
}

void daAlink_c::setFrontWallType() {
    f32 var_f31;
    if (checkWolf()) {
        var_f31 = 90.0f;
    } else if (mProcID == PROC_HOOKSHOT_FLY) {
        var_f31 = 50.0f;
    } else if (checkModeFlg(0x200000)) {
        var_f31 = 50.0f;
    } else {
        var_f31 = mAcchCir[0].GetWallR();
    }

    if (field_0x2f91 == 0) {
        field_0x2f91 = 1;
        if (!checkMagneBootsOn()) {
            cXyz line_start_pos;
            cXyz line_end_pos;
            f32 sp64 = cM_ssin(shape_angle.y);
            f32 sp60 = cM_scos(shape_angle.y);

            int wall_code;
            int i;
            BOOL sp5C = FALSE;
            BOOL sp58 = FALSE;

            f32 sp54;
            f32 sp50;

            if (mProcID == PROC_HOOKSHOT_FLY) {
                if (mProcVar2.field_0x300c == 4 && dComIfG_Bgsp().ChkPolySafe(mPolyInfo2)) {
                    cM3dGPla spF4;
                    dComIfG_Bgsp().GetTriPla(mPolyInfo2, &spF4);

                    if (cBgW_CheckBWall(spF4.mNormal.y)) {
                        spF4.mNormal.y = 0.0f;
                        spF4.mNormal.normalizeZP();

                        sp64 = -spF4.mNormal.x;
                        sp60 = -spF4.mNormal.z;
                    }
                }

                line_start_pos.x = mHookshotTopPos.x - (sp64 * var_f31);
                line_start_pos.z = mHookshotTopPos.z - (sp60 * var_f31);
                line_end_pos.x = (2.0f * mHookshotTopPos.x) - line_start_pos.x;
                line_end_pos.z = (2.0f * mHookshotTopPos.z) - line_start_pos.z;
                sp50 = mHookshotTopPos.y;
                sp54 = 10.0f + mHookshotTopPos.y;
            } else {
                line_start_pos.x = current.pos.x;
                line_start_pos.z = current.pos.z;
                line_end_pos.x = line_start_pos.x + (sp64 * (25.0f + var_f31));
                line_end_pos.z = line_start_pos.z + (sp60 * (25.0f + var_f31));
                sp50 = current.pos.y;
                sp54 = 100.0f + current.pos.y;
            }

            BOOL sp4C = FALSE;

            for (i = 0; i < 3; i++) {
                line_start_pos.y = sp50 + mAcchCir[i].GetWallH();
                line_end_pos.y = line_start_pos.y;

                if (commonLineCheck(&line_start_pos, &line_end_pos)) {
                    wall_code = dComIfG_Bgsp().GetWallCode(mLinkLinChk);

                    if (i == 0 && wall_code == 3 && checkStageName("D_MN10") && fopAcM_GetRoomNo(this) == 6) {
                        sp4C = TRUE;
                    } else if (!checkWolf() && !sp5C && ((i == 0 && mLinkAcch.ChkGroundHit() && !checkModeFlg(0x70C52)) || mProcID == PROC_HOOKSHOT_FLY) && wall_code == 0) {
                        sp5C = TRUE;
                        line_start_pos.y = sp54;
                        line_end_pos.y = line_start_pos.y;

                        if (commonLineCheck(&line_start_pos, &line_end_pos) && checkClimbCode(mLinkLinChk)) {
                            sp58 = TRUE;
                            break;
                        }

                        i = -1;
                    } else if (!checkWolf() || ((i != 0 && i != 1) || wall_code != 7)) {
                        break;
                    }
                }
            }

            if (i == 3) {
                if (!sp4C) {
                    offNoResetFlg3(FLG3_UNK_400000);
                    return;
                }

                line_start_pos.y = sp50 + mAcchCir[0].GetWallH();
                line_end_pos.y = line_start_pos.y;

                if (!commonLineCheck(&line_start_pos, &line_end_pos)) {
                    return;
                }
            } else {
                sp4C = FALSE;
            }

            cM3dGPla linchk_tri;
            dComIfG_Bgsp().GetTriPla(mLinkLinChk, &linchk_tri);

            if ((fabsf(linchk_tri.mNormal.y) > 0.05f)) {
                return;
            }

            field_0x306e = linchk_tri.mNormal.atan2sX_Z();
            BOOL sp48 = 0;

            s16 spA;
            if (mProcID == PROC_HOOKSHOT_FLY) {
                spA = 0x7FFF;
            } else if (mProcID == PROC_WOLF_JUMP_ATTACK || (checkWolf() && wall_code == 3)) {
                if (mProcID != PROC_WOLF_JUMP_ATTACK) {
                    sp48 = 1;
                }
                spA = 0x2000;
            } else {
                spA = field_0x30ae;
            }

            int sp44 = cLib_distanceAngleS(field_0x306e, (shape_angle.y + 0x8000));
            if (sp44 > spA) {
                return;
            }

            cM3dGPla spCC;
            f32 sp40;

            if (checkModeFlg(2)) {
                field_0x34ec = mLinkLinChk.GetCross();
            } else {
                sp40 = linchk_tri.getSignedLenPos(&current.pos);
                field_0x34ec.set(current.pos.x - (sp40 * linchk_tri.mNormal.x), current.pos.y, current.pos.z - (sp40 * linchk_tri.mNormal.z));

                int temp_r3 = dComIfG_Bgsp().GetWallCode(mLinkLinChk);
                if (temp_r3 != 4 && temp_r3 != 5) {
                    line_start_pos.x = current.pos.x;
                    line_start_pos.z = current.pos.z;
                    line_end_pos.x = line_start_pos.x - (2.0f * sp40 * linchk_tri.mNormal.x);
                    line_end_pos.z = line_start_pos.z - (2.0f * sp40 * linchk_tri.mNormal.z);

                    for (i = 0; i < 3; i++) {
                        if (i == 0 && sp58 != 0) {
                            line_start_pos.y = sp54;
                        } else {
                            line_start_pos.y = current.pos.y + mAcchCir[i].GetWallH();
                        }

                        line_end_pos.y = line_start_pos.y;

                        if (commonLineCheck(&line_start_pos, &line_end_pos)) {
                            dComIfG_Bgsp().GetTriPla(mLinkLinChk, &spCC);
                            cXyz sp80 = linchk_tri.mNormal - spCC.mNormal;

                            if (sp80.abs() < 0.001f) {
                                break;
                            }
                        }
                    }

                    if (i == 3) {
                        offNoResetFlg3(FLG3_UNK_400000);
                        return;
                    }
                }
            }

            int var_r29 = dComIfG_Bgsp().GetWallCode(mLinkLinChk);
            dBgW_Base* sp3C = dComIfG_Bgsp().GetBgWBasePointer(mLinkLinChk);

            if ((var_r29 == 3 || var_r29 == 1) && (sp4C || (sp3C != NULL && !sp3C->ChkPushPullOk()))) {
                var_r29 = 0;
            } else if (var_r29 == 1 && mMode == 0x2B) {
                var_r29 = 0;
            }

            if (sp48 && var_r29 != 3 && sp44 > field_0x30ae) {
                return;
            }

            cXyz wall_edge_pos;
            if (var_r29 != 1 && var_r29 != 5 && var_r29 != 4) {
                offNoResetFlg3(FLG3_UNK_400000);
            }

            if (!mLinkAcch.ChkWallHit() && !checkWolf() && (var_r29 != 1 || mProcID != PROC_HANG_READY) && (var_r29 != 3 || checkModeFlg(0x40000) || checkModeFlg(2) || !(sp40 <= 51.0f)) && mProcID != PROC_HOOKSHOT_FLY && !checkModeFlg(0x200000)) {
                offNoResetFlg3(FLG3_UNK_400000);
                return;
            }

            if (var_r29 == 2) {
                return;
            }

            if (var_r29 == 3 && dComIfG_Bgsp().ChkMoveBG_NoDABg(mLinkLinChk)) {
                field_0x27fc = dComIfG_Bgsp().GetActorPointer(mLinkLinChk);
            }

            if (checkNoResetFlg0(daPy_FLG0(FLG0_WATER_IN_MOVE | FLG0_SWIM_UP))) {
                if (var_r29 == 6 && !checkModeFlg(0x40002)) {
                    setDoStatus(BUTTON_STATUS_ENTER);
                    setLadderPosInit();
                    return;
                }

                if (var_r29 == 3 && !checkModeFlg(0x40000) && !checkModeFlg(2)) {
                    onResetFlg0(RFLG0_UNK_8);
                    mPolyInfo1.SetPolyInfo(mLinkLinChk);

                    if (checkModeFlg(0x200000)) {
                        return;
                    }

                    if (!mLinkAcch.ChkWallHit() && !checkWolf()) {
                        return;
                    }
                } else if (!checkWolf()) {
                    if (var_r29 == 1) {
                        if (checkModeFlg(0x40002)) {
                            if (checkModeFlg(2) && mProcID != PROC_HOOKSHOT_FLY) {
                                setDoStatus(BUTTON_STATUS_UNK_142);
                            }

                            f32 sp34;
                            if (checkModeFlg(0x40000)) {
                                sp34 = current.pos.y - 90.0f;
                            } else {
                                sp34 = current.pos.y;
                            }

                            line_start_pos.set(field_0x34ec.x - (15.0f * linchk_tri.mNormal.x), 150.0f + sp34, field_0x34ec.z - (15.0f * linchk_tri.mNormal.z));
                            mLinkGndChk.SetPos(&line_start_pos);
                            f32 sp38 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);

                            if (sp38 >= current.pos.y && dBgS_CheckBGroundPoly(mLinkGndChk)) {
                                cM3dGPla spB8;
                                dComIfG_Bgsp().GetTriPla(mLinkGndChk, &spB8);

                                if (getWallEdgePos(field_0x34ec, &linchk_tri, &spB8, &wall_edge_pos, 0)) {
                                    if (wall_edge_pos.y - sp34 <= 150.0f) {
                                        if (checkModeFlg(0x40000)) {
                                            field_0x34ec = wall_edge_pos;
                                            field_0x2f91 = 7;
                                            return;
                                        } else if (mProcID != PROC_HOOKSHOT_FLY) {
                                            field_0x34ec = wall_edge_pos;
                                            field_0x2f91 = 0xA;
                                            return;
                                        } else {
                                            current.pos.y = wall_edge_pos.y - 150.0f;
                                        }
                                    }
                                }
                            }
                        }
                        mPolyInfo1.SetPolyInfo(mLinkLinChk);
                        field_0x2f91 = 3;
                        return;
                    }

                    if (var_r29 == 4 || (var_r29 == 5 && !checkModeFlg(2))) {
                        if (var_r29 == 4) {
                            field_0x2f91 = 4;
                            if (checkModeFlg(2)) {
                                setDoStatus(BUTTON_STATUS_UNK_142);
                            }
                        } else {
                            field_0x2f91 = 5;
                        }
                        setLadderPosInit();
                        return;
                    }
                }
            }

            if (checkRoomOnly() && !checkWolf()) {
                return;
            }

            f32 hang_height;
            f32 small_jump_height;
            f32 climb_height;
            f32 body_height;

            if (checkWolf()) {
                if (checkModeFlg(2)) {
                    hang_height = mpHIO->mWolf.mWlAutoJump.m.mHangLimitHeight;
                } else {
                    hang_height = mpHIO->mWolf.mWlWallHang.m.mClimbHeight;
                }
                small_jump_height = mpHIO->mWolf.mWlWallHang.m.mSmallJumpHeight;
                climb_height = mpHIO->mWolf.mWlWallHang.m.mWallGrabHeight;
            } else {
                if (checkModeFlg(2)) {
                    hang_height = mpHIO->mAutoJump.m.mHangHeightLimit;
                } else {
                    hang_height = mpHIO->mWallHang.m.jump_hang_height;
                }
                small_jump_height = mpHIO->mWallHang.m.small_jump_height;
                climb_height = mpHIO->mWallHang.m.climb_height;
            }

            body_height = current.pos.y + mSinkShapeOffset;
            line_start_pos.set(current.pos.x, body_height + (0.01f + hang_height), current.pos.z);
            line_end_pos.set(line_start_pos.x + (sp64 * (50.0f + var_f31)), line_start_pos.y, line_start_pos.z + (sp60 * (50.0f + var_f31)));

            BOOL sp24 = commonLineCheck(&line_start_pos, &line_end_pos);
            if (sp24) {
                line_start_pos = mLinkLinChk.GetCross();
                dComIfG_Bgsp().GetTriPla(mLinkLinChk, &spCC);
                line_start_pos.x = (line_start_pos.x + (25.0f * spCC.mNormal.x));
                line_start_pos.z = (line_start_pos.z + (25.0f * spCC.mNormal.z));
                mLinkGndChk.SetPos(&line_start_pos);

                if (dComIfG_Bgsp().GroundCross(&mLinkGndChk) > current.pos.y + l_autoUpHeight) {
                    dComIfG_Bgsp().GetTriPla(mLinkGndChk, &spCC);
                    if (cBgW_CheckBGround(spCC.mNormal.y)) {
                        sp24 = FALSE;
                    }
                }
            }

            if (!sp24) {
                mLinkRoofChk.SetPos(current.pos);
                sp24 = dComIfG_Bgsp().RoofChk(&mLinkRoofChk) - body_height > 10.0f + hang_height ? FALSE : TRUE;
            }

            if (sp24) {
                return;
            }

            cM3dGPla gndchk_tri;
            bool found_gnd_tri = false;
            int sp20 = 0;

            line_start_pos.set(field_0x34ec.x - (7.5f * linchk_tri.mNormal.x), body_height + (0.01f + hang_height), field_0x34ec.z - (7.5f * linchk_tri.mNormal.z));
            mLinkGndChk.SetPos(&line_start_pos);
            f32 sp1C = dComIfG_Bgsp().GroundCross(&mLinkGndChk);

            line_start_pos.set(field_0x34ec.x - (1.5f * linchk_tri.mNormal.x), body_height + (0.01f + hang_height), field_0x34ec.z - (1.5f * linchk_tri.mNormal.z));
            mLinkGndChk.SetPos(&line_start_pos);
            f32 sp38 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);

            if (-G_CM3D_F_INF != sp38 && fabsf(sp1C - sp38) < l_autoUpHeight) {
                found_gnd_tri = dComIfG_Bgsp().GetTriPla(mLinkGndChk, &gndchk_tri);
            }
            if ((!checkModeFlg(0x40000) && sp38 < body_height) || !found_gnd_tri) {
                return;
            }
            if (-G_CM3D_F_INF == sp38 || !cBgW_CheckBGround(gndchk_tri.mNormal.y)) {
                return;
            }

            if (!getWallEdgePos(field_0x34ec, &linchk_tri, &gndchk_tri, &wall_edge_pos, sp20)) {
                return;
            }

            if (dComIfG_Bgsp().GetGroundCode(mLinkGndChk) != 6) {
                f32 sp18 = 36.5f;
                line_start_pos.set(wall_edge_pos.x + (10.0f * linchk_tri.mNormal.x), wall_edge_pos.y + l_autoUpHeight, wall_edge_pos.z + (10.0f * linchk_tri.mNormal.z));
                line_end_pos.set(wall_edge_pos.x - (linchk_tri.mNormal.x * sp18), line_start_pos.y, wall_edge_pos.z - (linchk_tri.mNormal.z * sp18));

                if (commonLineCheck(&line_start_pos, &line_end_pos)) {
                    return;
                }
            }

            field_0x34ec = wall_edge_pos;
            f32 temp_f29 = field_0x34ec.y - body_height;

            if (checkModeFlg(0x40000)) {
                f32 climb_height;
                f32 climb_water_surface;
                if (checkWolf()) {
                    climb_height = 0.01f + mpHIO->mWolf.mWlSwim.m.mClimbHeight;
                    climb_water_surface = mpHIO->mWolf.mWlSwim.m.mClimbBelowWaterSurface;
                } else {
                    climb_height = 0.01f + mpHIO->mSwim.m.mClimbHeight;
                    climb_water_surface = mpHIO->mSwim.m.mClimbWaterSurfaceUnder;
                }

                if (checkNoResetFlg0(FLG0_SWIM_UP)) {
                    if (temp_f29 <= climb_height) {
                        if (temp_f29 >= climb_water_surface) {
                            field_0x2f91 = 7;
                        }
                    }
                }
            } else if (checkModeFlg(2)) {
                if (sp38 - mLinkAcch.GetGroundH() > mHeight && temp_f29 < hang_height) {
                    if (!checkWolf() && mProcID != PROC_HOOKSHOT_FLY && temp_f29 < mpHIO->mAutoJump.m.mGrabHeightLimit) {
                        field_0x2f91 = 0xB;
                    } else {
                        field_0x2f91 = 0xA;
                    }
                }
            } else if (!(temp_f29 >= 0.01f + hang_height) && (!checkGrabAnime() || !(temp_f29 >= 0.01f + small_jump_height))) {
                if (temp_f29 < 0.01f + small_jump_height) {
                    field_0x2f91 = 6;
                } else if (temp_f29 < 0.01f + climb_height) {
                    field_0x2f91 = 7;
                } else if (checkWolf() || (temp_f29 < 0.01f + mpHIO->mWallHang.m.jump_climb_height)) {
                    field_0x2f91 = 8;
                } else {
                    field_0x2f91 = 9;
                }
            }
        }
    }
}

BOOL daAlink_c::checkWaterPolygonUnder() {
    return checkModeFlg(MODE_SWIMMING) || current.pos.y < mWaterY;
}

void daAlink_c::setWaterY() {
    if (mLinkAcch.ChkWaterHit()) {
        onNoResetFlg0(FLG0_UNK_80);
        field_0x2fbc = dComIfG_Bgsp().GetPolyAtt0(mLinkAcch.m_wtr);
        mWaterY = mLinkAcch.m_wtr.GetHeight();
        if (field_0x2fbc != 6) {
            checkWaterInKandelaar(mWaterY);
        }
    } else {
        offNoResetFlg0(FLG0_UNK_80);
        field_0x2fbc = 16;
        mWaterY = -G_CM3D_F_INF;
    }
}

void daAlink_c::setHangWaterY() {
    cXyz sp14;
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0), &sp14);

    u32 var_r29;
    u32 var_r28;
    if (checkWolf()) {
        var_r29 = Z2SE_WL_OUTOF_WATER;
        var_r28 = Z2SE_WL_INTO_WATER;
    } else {
        f32 var_f31;
        if (mLeftFootPos.y < mRightFootPos.y) {
            var_f31 = mLeftFootPos.y;
        } else {
            var_f31 = mRightFootPos.y;
        }

        sp14.y = var_f31;
        var_r29 = Z2SE_AL_OUTOF_WATER;
        var_r28 = Z2SE_AL_INTO_WATER;
    }

    if (fopAcM_wt_c::waterCheck(&sp14)) {
        field_0x33b8 = fopAcM_wt_c::getWaterY();
        field_0x2fc5 = fopAcM_wt_c::getPolyAtt0();
        field_0x2fc6 = dKy_pol_argument_get(fopAcM_wt_c::getWaterCheck());

        if (field_0x2fc5 != 6) {
            if (checkModeFlg(0x40)) {
                cXyz sp8(sp14.x, field_0x33b8, sp14.z);

                if (field_0x33bc < field_0x33b8 && sp14.y >= field_0x33b8) {
                    seStartMapInfo(var_r29);
                } else if (field_0x33bc > field_0x33b8 && sp14.y <= field_0x33b8) {
                    seStartMapInfo(var_r28);
                    fopKyM_createWpillar(&sp8, field_0x3420, 0);
                }
                if (field_0x33bc < field_0x33b8) {
                    dComIfGp_particle_setWaterRipple(field_0x31a8, *fopAcM_wt_c::getWaterCheck(), &sp8, 0.0f, &tevStr, NULL, fopAcM_GetRoomNo(this));
                }
            }

            checkWaterInKandelaar(field_0x33b8);
        }
    } else {
        field_0x33b8 = -G_CM3D_F_INF;
        field_0x2fc5 = 0x10;
        field_0x2fc6 = 0;
    }

    field_0x33bc = sp14.y;
}

void daAlink_c::setSandDownBgCheckWallH() {
    if (!checkModeFlg(0x400)) {
        for (int i = 0; i < 3; i++) {
            mAcchCir[i].SetWallH(mAcchCirWallH[i] + mSinkShapeOffset);
        }
    }
}

void daAlink_c::setBgCheckParam() {
    mAcchCir[0].SetWallH(l_autoUpHeight);
    mAcchCir[1].SetWallH(129.99f);
    mAcchCir[2].SetWallH(mHeight);

    if (checkSpinnerRide()) {
        mLinkAcch.SetRoofCrrHeight(80.0f);
    } else {
        mLinkAcch.SetRoofCrrHeight(mHeight);
    }

    if (checkWolf()) {
        if (checkModeFlg(0x40000)) {
            mAcchCir[0].SetWallH(-50.0f);
            mAcchCir[1].SetWallH(-25.0f);
            mAcchCir[2].SetWallH(0.0f);
            mAcchCir[0].SetWallR(35.0f);
        } else if (checkModeFlg(0x1000000)) {
            if (mProcID == PROC_WOLF_LIE_AUTO_MOVE) {
                mAcchCir[0].SetWallR(11.655f);
            } else {
                mAcchCir[0].SetWallR(35.0f);
            }

            mAcchCir[0].SetWallH(10.0f);
            mAcchCir[1].SetWallH(75.0f);
            mAcchCir[2].SetWallH(75.0f);
        } else if (mProcID == PROC_WOLF_TAG_JUMP) {
            mAcchCir[0].SetWallH(0.5f * (mHeight + l_autoUpHeight));
            mAcchCir[1].SetWallH(0.75f * (mHeight + l_autoUpHeight));
            mAcchCir[0].SetWallR(35.0f);
        } else {
            mAcchCir[1].SetWallH(0.5f * (mHeight + l_autoUpHeight));
            mAcchCir[0].SetWallR(35.0f);
        }
    } else if (checkModeFlg(0x200000)) {
        mAcchCir[0].SetWallR(20.0f);
    } else if (mProcID == PROC_LARGE_DAMAGE_WALL || checkModeFlg(0x10000)) {
        mAcchCir[0].SetWallR(5.0f);
    } else if (checkModeFlg(0x10)) {
        mAcchCir[0].SetWallR(50.0f);
    } else if (checkModeFlg(0x40000)) {
        mAcchCir[0].SetWallR(50.0f);
        mAcchCir[0].SetWallH(-60.0f);
        mAcchCir[1].SetWallH(-30.0f);
        mAcchCir[2].SetWallH(0.0f);
        mLinkAcch.SetRoofCrrHeight(40.0f);
    } else if (checkModeFlg(0x1000000) && mProcID != PROC_CRAWL_END) {
        if (mProcID == PROC_CRAWL_AUTO_MOVE) {
            mAcchCir[0].SetWallR(11.655f);
        } else {
            mAcchCir[0].SetWallR(35.0f);
        }

        mAcchCir[0].SetWallH(30.0f);
        mAcchCir[1].SetWallH(80.0f);
        mAcchCir[2].SetWallH(80.0f);
    } else if (mProcID == PROC_DAMAGE) {
        mAcchCir[0].SetWallR(70.0f);
    } else if (mProcID == PROC_HOOKSHOT_FLY) {
        mAcchCir[0].SetWallR(140.0f);
        mAcchCir[0].SetWallH(-25.0f);
        mAcchCir[1].SetWallH(0.0f);
        mAcchCir[2].SetWallH(50.0f);
    } else if (mProcID == PROC_HOOKSHOT_ROOF_WAIT || mProcID == PROC_HOOKSHOT_ROOF_SHOOT) {
        mAcchCir[0].SetWallH(-260.0f);
        mAcchCir[1].SetWallH(-140.0f);
        mAcchCir[2].SetWallH(-20.0f);

        mLinkAcch.SetRoofCrrHeight(0.0f);
        mAcchCir[0].SetWallR(35.0f);
    } else {
        if (checkModeFlg(0x40)) {
            mAcchCir[0].SetWallH(25.0f);
            mAcchCir[1].SetWallH(25.0f);
            mAcchCir[2].SetWallH(25.0f);
        } else if (checkModeFlg(0x400) && !checkSpinnerRide() && !checkBoardRide() && mProcID != PROC_SPINNER_WAIT && mProcID != PROC_BOARD_WAIT) {
            mAcchCir[0].SetWallH(l_autoUpHeight - 80.0f);
            mAcchCir[1].SetWallH(49.990005f);
            mAcchCir[2].SetWallH(mHeight - 80.0f);
        }

        mAcchCir[0].SetWallR(35.0f);
    }

    mAcchCir[1].SetWallR(mAcchCir[0].GetWallR());
    mAcchCir[2].SetWallR(mAcchCir[0].GetWallR());

    for (int i = 0; i < 3; i++) {
        mAcchCirWallH[i] = mAcchCir[i].GetWallH();
    }

    setSandDownBgCheckWallH();
}

BOOL daAlink_c::checkNotJumpSinkLimit() {
    return mSinkShapeOffset < -30.0f || mGndPolyAtt1 == 3 ||
           (mGndPolyAtt1 == 4 && mSinkShapeOffset < -15.0f);
}

BOOL daAlink_c::checkNotItemSinkLimit() {
    return mSinkShapeOffset < -34.0f &&
           (mGndPolyAtt1 == 1 || mGndPolyAtt1 == 2 || mGndPolyAtt1 == 3 || mGndPolyAtt1 == 4);
}

void daAlink_c::setSandShapeOffset() {
    f32 var_f30 = 0.0f;
    f32 sink_speed = 0.0f;
    f32 temp_f29 = mSinkShapeOffset;
    BOOL isEventRun = checkEventRun();

    if (mLinkAcch.ChkGroundHit() && (mGndPolyAtt0 == 3 || checkSnowCode()) && !checkModeFlg(0x440)) {
        if (!isEventRun && !checkModeFlg(0x02000000)) {
            if (checkSnowCode()) {
                if (!checkZeroSpeedF() || mProcID == PROC_TURN_MOVE) {
                    sink_speed = 1.0f;
                    if (mGndPolySpecialCode == dBgW_SPCODE_LIGHT_SNOW) {
                        var_f30 = -15.0f;
                    } else {
                        if (mSinkShapeOffset > -15.0f) {
                            sink_speed = 10.0f;
                        }
                        var_f30 = -35.0f;
                    }
                }
            } else if (mGndPolyAtt1 == 4) {
                var_f30 = -25.0f;
                if (mSinkShapeOffset > -15.0f) {
                    sink_speed = 10.0f;
                } else {
                    sink_speed = mpHIO->mBasic.m.mSandSinkSpeed;
                }
            } else if (mGndPolyAtt1 == 2) {
                var_f30 = field_0x345c;
                sink_speed = 6.0f;
            } else if (mGndPolyAtt1 == 3) {
                var_f30 = field_0x3458;
                if (mSinkShapeOffset > -15.0f) {
                    sink_speed = 10.0f;
                } else {
                    sink_speed = mpHIO->mBasic.m.mSandSinkSpeed;
                    if (checkWolf()) {
                        sink_speed *= 0.42857143f;
                    }
                }
            } else if (mGndPolyAtt1 == 1) {
                if (mNormalSpeed < 2.0f) {
                    var_f30 = field_0x3458;
                    if (mSinkShapeOffset > -15.0f) {
                        sink_speed = 10.0f;
                    } else {
                        sink_speed = mpHIO->mBasic.m.mSandSinkSpeed;
                    }

                    if (checkWolf()) {
                        sink_speed *= 0.42857143f;
                    }
                } else {
                    var_f30 = -15.0f;
                    sink_speed = mpHIO->mBasic.m.mSandSurfaceSpeed;
                }
            } else if (!(mNormalSpeed < 2.0f) || !(mSinkShapeOffset < -15.0f)) {
                var_f30 = -15.0f;
                if (mSinkShapeOffset < -15.0f) {
                    sink_speed = mpHIO->mBasic.m.mSandSurfaceSpeed;
                } else {
                    sink_speed = mpHIO->mBasic.m.mSandSinkSpeed;
                }
            }
        }
    } else {
        var_f30 = 0.0f;
        sink_speed = 10.0f;
    }

    if (sink_speed > 0.0f) {
        cLib_chaseF(&mSinkShapeOffset, var_f30, sink_speed);
    }

    if (mSinkShapeOffset < -15.0f) {
        field_0x2fc9 = mGndPolyAtt0;

        int var_r28;
        if (mProcID == PROC_LAVA_RETURN) {
            var_r28 = 0x7F;
        } else {
            var_r28 = cLib_minMaxLimit<int>((128.0f * ((mSinkShapeOffset - -15.0f) / (field_0x3458 - -15.0f))) - 1.0f, -1, 0x7F);
        }

        mZ2Link.setSinkDepth(var_r28);

        if (fabsf(temp_f29 - mSinkShapeOffset) > 0.01f) {
            if (mGndPolyAtt0 == 0xD) {
                seStartOnlyReverbLevel(Z2SE_AL_SINK_SNOW_LEVEL);
            } else {
                seStartOnlyReverbLevel(Z2SE_AL_SINK_SAND_LEVEL);
            }
        }
    } else if (field_0x2fc9 != 0x10) {
        if (field_0x2fc9 == 0xD) {
            seStartOnlyReverb(Z2SE_AL_SINK_SNOW_OUT);
        } else {
            seStartOnlyReverb(Z2SE_AL_SINK_SAND_OUT);
        }

        field_0x2fc9 = 0x10;
    }

    if (checkWolf() && !isEventRun && mSinkShapeOffset < -30.0f) {
        getMidnaActor()->onForcePanic();
    }
}

bool daAlink_c::checkLv2MiddleBossBgRide(s16 i_procName) {
    return i_procName == PROC_OBJ_MSIMA || i_procName == PROC_B_ZANTS;
}

bool daAlink_c::getSlidePolygon(cM3dGPla* o_tripla) {
    if (!checkMagneBootsOn()
        && mLinkAcch.ChkGroundHit()
        && !checkModeFlg(0x40000)
        && mGndPolySpecialCode != 6
        && dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd))
    {
        if ((checkEquipHeavyBoots() && daTagMagne_c::checkMagnetCode(mLinkAcch.m_gnd)) || mProcID == PROC_GORON_MOVE) {
            return 0;
        }

        s16 movebg_actorName = getMoveBGActorName(mLinkAcch.m_gnd, TRUE);
        if (dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, o_tripla)
            && (!checkBootsOrArmorHeavy() || (movebg_actorName != PROC_Obj_RotStair && movebg_actorName != PROC_Obj_Lv3R10Saka))
            && ((mGroundCode != 8 && (mGndPolySpecialCode == 1 || (o_tripla->mNormal.y < field_0x3470 && mGndPolySpecialCode != 2)))
                || (mGndPolySpecialCode == 5 && !checkWolf() && (!checkInputOnR() || mProcID == PROC_SLIDE) && (o_tripla->mNormal.y < cM_scos(field_0x3122)))
                || (!checkEquipHeavyBoots()
                    && !dComIfGp_checkPlayerStatus0(0, 0x100)
                    && checkLv2MiddleBossBgRide(movebg_actorName)
                    && (o_tripla->mNormal.y < cM_scos(cM_deg2s(mpHIO->mSlide.m.mLV2MinibossFloorSlideAngle)))
                    )
                )
            )
        {
            return true;
        }
    }

    return false;
}

BOOL daAlink_c::checkSlope() const {
    return mGroundCode != 8 && field_0x2ff0 < -field_0x3122;
}

void daAlink_c::setPlayerPosAndAngle(const cXyz* i_pos, s16 i_angle, BOOL param_2) {
    if (checkEventRun() || param_2 || mMode != 0) {
        if (i_pos != NULL) {
            current.pos = *i_pos;
            old.pos = current.pos;
            field_0x3798 = current.pos;
            onEndResetFlg0(ERFLG0_UNK_800000);
            onEndResetFlg2(ERFLG2_UNK_100);
            if (mDemo.getDemoMode() != daPy_demo_c::DEMO_SWORD_PUSH_e) {
                speed.y = 0.0f;
            }
        }

        shape_angle.y = i_angle;
        current.angle.y = i_angle;
        mPrevAngleY = shape_angle.y;
        if ((mProcID == PROC_TALK || mProcID == PROC_TRADE_ITEM_OUT) && !checkWolf()) {
            if (field_0x3198 != 0) {
                field_0x37c8.x = current.pos.x;
                field_0x37c8.z = current.pos.z;
            }
        }

        if (checkMagneBootsOn() && cBgW_CheckBGround(mMagneBootsTopVec.y)) {
            field_0x3118 = shape_angle.y;
        }

        if (checkHorseRide()) {
            daHorse_c* horse = dComIfGp_getHorseActor();
            horse->setHorsePosAndAngle(&current.pos, shape_angle.y);
        } else if (checkSpinnerRide()) {
            fopAc_ac_c* rideActor = mRideAcKeep.getActor();
            rideActor->current.pos = current.pos;
            rideActor->shape_angle.y = shape_angle.y;
            rideActor->current.angle.y = shape_angle.y;
            rideActor->speed.y = 0.0f;
        }

        mCcStts.ClrCcMove();
    }
}

void daAlink_c::setPlayerPosAndAngle(const cXyz* i_pos, const csXyz* i_angle) {
    if (checkEventRun() || mMode != 0) {
        if (i_pos != NULL) {
            current.pos = *i_pos;
            old.pos = current.pos;
            field_0x3798 = current.pos;
            onEndResetFlg0(ERFLG0_UNK_800000);
            onEndResetFlg2(ERFLG2_UNK_100);
            speed.y = 0.0f;
        }

        if (i_angle != NULL) {
            shape_angle = *i_angle;
            current.angle.y = shape_angle.y;
            mPrevAngleY = shape_angle.y;
        }

        mCcStts.ClrCcMove();
    }
}

void daAlink_c::setPlayerPosAndAngle(Mtx i_mtx) {
    if (checkEventRun() || mMode != 0) {
        mDoMtx_multVecZero(i_mtx, &current.pos);
        old.pos = current.pos;
        field_0x3798 = current.pos;
        onEndResetFlg0(ERFLG0_UNK_800000);
        onEndResetFlg2(ERFLG2_UNK_100);
        mDoMtx_MtxToRot(i_mtx, &shape_angle);
        current.angle.y = shape_angle.y;
        mPrevAngleY = shape_angle.y;
        speed.y = 0.0f;
        mCcStts.ClrCcMove();
    }
}

#if DEBUG
BOOL daAlink_c::checkDebugMoveInput() {
    if (mDoCPd_c::isConnect(PAD_3)) {
        return mDoCPd_c::getHoldB(PAD_1)
                && mDoCPd_c::getAnalogR(PAD_1) > 0.8f
                && mDoCPd_c::getTrigA(PAD_1);
    }

    return FALSE;
}
#endif

BOOL daAlink_c::itemTriggerCheck(u8 i_btnFlag) {
    mUseButtonFlags |= i_btnFlag;
    return mItemTrigger & i_btnFlag;
}

BOOL daAlink_c::itemButtonCheck(u8 i_btnFlag) {
    mUseButtonFlags |= i_btnFlag;
    return mItemButton & i_btnFlag;
}

BOOL daAlink_c::itemButton() {
    return itemButtonCheck(1 << mSelectItemId);
}

BOOL daAlink_c::itemTrigger() {
    return itemTriggerCheck(1 << mSelectItemId);
}

BOOL daAlink_c::spActionButton() {
    return itemButtonCheck(BTN_R);
}

BOOL daAlink_c::spActionTrigger() {
    return itemTriggerCheck(BTN_R);
}

BOOL daAlink_c::midnaTalkTrigger() const {
    return mItemTrigger & BTN_Z;
}

BOOL daAlink_c::swordSwingTrigger() {
    return swordTrigger();
}

void daAlink_c::setItemActionButtonStatus(u8 i_status) {
    setRStatusEmphasys(i_status);
}

BOOL daAlink_c::itemActionTrigger() {
    return spActionTrigger();
}

void daAlink_c::setStickData() {
    BOOL var_r31 = false;
    field_0x2f8f = mItemButton;
    mItemTrigger = 0;
    mItemButton = 0;
    mHeavySpeedMultiplier = 1.0f;

    if (checkEventRun()
        && mProcID != PROC_GRASS_WHISTLE_WAIT
        && mProcID != PROC_WOLF_DIG_THROUGH
        && mProcID != PROC_GOAT_CATCH
        && mProcID != PROC_WOLF_GANON_CATCH
        && mProcID != PROC_TOOL_DEMO
        && (mProcID != PROC_METAMORPHOSE || mFallVoiceInit == 0)
        )
    {
        u32 demoMode = mDemo.getDemoMode();

        if (checkDemoMoveMode(mDemo.getDemoMode())) {
            if (dDemo_c::getActor(demoActorID) != NULL) {
                mStickValue = 1.0f;
            } else {
                mStickValue = mDemo.getStick();
            }
        } else if (demoMode == daPy_demo_c::DEMO_UNK_14_e || demoMode == daPy_demo_c::DEMO_UNK_26_e || demoMode == daPy_demo_c::DEMO_UNK_65_e) {
            if (demoMode == daPy_demo_c::DEMO_UNK_26_e) {
                mStickValue = mDemo.getStick();
                mDemo.setMoveAngle(getSceneExitMoveAngle());

                if (checkHorseRide()) {
                    s16 angle = mDemo.getMoveAngle();
                    dComIfGp_getHorseActor()->changeDemoMoveAngle(angle);
                }
            } else {
                mStickValue = 0.0f;
            }
            mItemButton = field_0x2f8f;
        } else {
            mStickValue = 0.0f;
        }
        mMoveAngle = mDemo.getMoveAngle();
        mMoveValue = mStickValue;
    } else if (checkDeadHP() || dMeter2Info_getPauseStatus() == 1) {
        mStickValue = 0.0f;
        mMoveValue = 0.0f;
        mStickAngle = 0;
        mMoveAngle = 0;
    } else if (checkMidnaLockJumpPoint() && getMidnaActor()->checkNoInput()) {
        mStickValue = 0.0f;
        mMoveValue = 0.0f;
        mStickAngle = 0;
        dComIfGp_2dShowOff();
        onNoResetFlg2(FLG2_UNK_80);
        var_r31 = TRUE;
        field_0x2fe4 = shape_angle.y;
    } else {
        if (!checkInputOnR()) {
            field_0x2fb9 = 0;
        }

        BOOL usingFishRod = checkFishingRodAndLureItem()
                            && mItemAcKeep.getActor() != NULL
                            && (checkCanoeRide() || mProcID == PROC_FISHING_CAST);

        if (usingFishRod) {
            dmg_rod_class* mg_rod = (dmg_rod_class*)mItemAcKeep.getActor();
            mStickValue = JMAFastSqrt(SQUARE(mg_rod->getRodStickX()) + SQUARE(mg_rod->getRodStickY()));
            mStickAngle = cM_atan2s(-mg_rod->getRodStickX(), mg_rod->getRodStickY());
        } else {
            mStickValue = mDoCPd_c::getStickValue(PAD_1);
            mStickAngle = mDoCPd_c::getStickAngle3D(PAD_1) - -0x8000;
        }

        mMoveValue = mStickValue;
        mMoveAngle = mStickAngle + dCam_getControledAngleY(dComIfGp_getCamera(field_0x317c));

        if (checkMagneBootsOn()) {
            if (field_0x2fb9 == 1 ||
                (field_0x2fb9 == 0 && field_0x3114 != -0x8000 && field_0x3114 <= 0x6000))
            {
                field_0x2fb9 = 1;
            } else {
                field_0x2fb9 = 2;
            }

            if ((field_0x2fb9 == 1 && (field_0x3114 > 0x6000 || field_0x3114 == -0x8000)) ||
                (field_0x2fb9 == 2 && field_0x3114 != -0x8000 && field_0x3114 < 0x4000))
            {
                mMoveAngle = getMagneBootsLocalAngleY(mMoveAngle, 0) + 0x8000;
            } else {
                mMoveAngle = getMagneBootsLocalAngleY(mMoveAngle, field_0x2fb9);
            }
        } else {
            field_0x2fb9 = 1;
        }

        if (mDoCPd_c::getTrigB(PAD_1)) {
            mItemTrigger |= BTN_B;
        }
        if (mDoCPd_c::getTrigA(PAD_1)) {
            mItemTrigger |= BTN_A;
        }
        if (mDoCPd_c::getTrigX(PAD_1)) {
            mItemTrigger |= BTN_X;
        }
        if (mDoCPd_c::getTrigY(PAD_1)) {
            mItemTrigger |= BTN_Y;
        }
        if (mDoCPd_c::getTrigZ(PAD_1)) {
            mItemTrigger |= BTN_Z;
        }
        if (mDoCPd_c::getTrigL(PAD_1)) {
            mItemTrigger |= BTN_L;
        }
        if (mDoCPd_c::getTrigLockR(PAD_1)) {
            mItemTrigger |= BTN_R;
        }

        if (mDoCPd_c::getHoldA(PAD_1)) {
            mItemButton |= BTN_A;
        }
        if (mDoCPd_c::getHoldB(PAD_1)) {
            mItemButton |= BTN_B;
        }
        if (mDoCPd_c::getHoldX(PAD_1)) {
            mItemButton |= BTN_X;
        }
        if (mDoCPd_c::getHoldY(PAD_1)) {
            mItemButton |= BTN_Y;
        }
        if (mDoCPd_c::getHoldZ(PAD_1)) {
            mItemButton |= BTN_Z;
        }
        if (mDoCPd_c::getHoldL(PAD_1)) {
            mItemButton |= BTN_L;
        }
        if (mDoCPd_c::getHoldLockR(PAD_1)) {
            mItemButton |= BTN_R;
        }

        if (checkHeavyStateOn(TRUE, TRUE) &&
            (!checkBootsOrArmorHeavy() || !checkNoResetFlg0(FLG0_WATER_IN_MOVE)))
        {
            if (checkWolf() && checkHeavyStateOn(TRUE, FALSE) && mSinkShapeOffset < -30.0f) {
                mHeavySpeedMultiplier = mpHIO->mItem.mIronBoots.m.mInputFactor * 0.7f;
            } else {
                mHeavySpeedMultiplier = mpHIO->mItem.mIronBoots.m.mInputFactor;
            }
            mStickValue *= mHeavySpeedMultiplier;
        } else if (checkBootsOrArmorHeavy()) {
            if (checkZoraWearAbility()) {
                mHeavySpeedMultiplier = mpHIO->mItem.mIronBoots.m.mZoraWaterInputFactor;
            } else {
                mHeavySpeedMultiplier = mpHIO->mItem.mIronBoots.m.mWaterInputFactor;
            }
            mStickValue *= mHeavySpeedMultiplier;
        } else if ((checkWolf() && field_0x2fbc == 11 && checkWaterPolygonUnder()) ||
                   mGndPolyAtt0 == 11)
        {
            onNoResetFlg0(FLG0_HEAVY_STATE);
            mStickValue *= mpHIO->mWolf.m.mUnderwaterInputRate;
            mHeavySpeedMultiplier = mpHIO->mWolf.m.mUnderwaterInputRate;
        }
    }

    s16 angle_diff = mStickAngle - mPrevStickAngle;
    int abs_v = abs(angle_diff);

    if (abs_v < 0x4000 && abs_v > 0x6D4 && field_0x3180 * angle_diff >= 0) {
        field_0x3180 += angle_diff;
        mStickSpinTimer = 4;
    } else if (field_0x3180 * angle_diff < 0) {
        field_0x3180 = angle_diff;
        mStickSpinTimer = 4;
    } else if (mStickSpinTimer > 0) {
        mStickSpinTimer--;
    } else {
        field_0x3180 = 0;
    }

    if (!doButton()) {
        offNoResetFlg0(FLG0_UNK_10000000);
    }

    if (!var_r31 && checkNoResetFlg2(FLG2_UNK_80)) {
        offNoResetFlg2(FLG2_UNK_80);
        dComIfGp_2dShowOn();
    }
}

void daAlink_c::setAtnList() {
    mAttList = NULL;
    mAttList2 = NULL;
    mTargetedActor = NULL;
    field_0x27f0 = NULL;
    field_0x27f4 = NULL;
    field_0x27f8 = NULL;

    if (checkEventRun() || checkAttentionLock() || checkInputOnR()) {
        dComIfGp_clearPlayerStatus0(0, 0x400000);
        offNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER);
    } else {
        if (mThrowBoomerangAcKeep.getActor() == NULL) {
            dComIfGp_clearPlayerStatus0(0, 0x400000);
        }
        if (mCopyRodAcKeep.getActor() == NULL || getCopyRodControllActor() != NULL) {
            offNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER);
        }
    }

    field_0x27fc = NULL;

    if (checkFmChainGrabAnime()) {
        mTargetedActor = field_0x2844.getActor();
        field_0x27f4 = mTargetedActor;
        field_0x27fc = field_0x2844.getActor();
    } else if (mProcID == PROC_GORON_MOVE) {
        mTargetedActor = mCargoCarryAcKeep.getActor();
        field_0x27f4 = mTargetedActor;
    } else if (mProcID == PROC_CUT_FINISH && field_0x280c.getActor() != NULL) {
        mTargetedActor = field_0x280c.getActor();
        field_0x27f4 = mTargetedActor;
    } else if (dComIfGp_checkPlayerStatus0(0, 0x400000)) {
        mTargetedActor = mThrowBoomerangAcKeep.getActor();
        field_0x27f4 = mTargetedActor;
    } else if (checkNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER)) {
        mTargetedActor = mCopyRodAcKeep.getActor();
        field_0x27f4 = mTargetedActor;
    } else if (checkAttentionLock()) {
        mAttList = mAttention->GetLockonList(0);
        if (mAttList != NULL && mAttention->LockonTruth()) {
            mTargetedActor = mAttList->getActor();
            field_0x27f4 = mTargetedActor;
            onResetFlg0(RFLG0_ATTENTION_LOCK);

            if (checkEnemyGroup(mTargetedActor)) {
                onResetFlg0(RFLG0_ENEMY_ATTN_LOCK);
            }
        }

        if (!checkWolf() || !checkNoResetFlg0(FLG0_UNK_400000)) {
            field_0x2fe4 = shape_angle.y;
        }

        onNoResetFlg0(FLG0_UNK_400000);
    } else {
        offNoResetFlg0(FLG0_UNK_400000);
    }

    if (mTargetedActor == NULL) {
        mAttList = mAttention->getActionBtnB();
        if (mAttList != NULL) {
            field_0x27f0 = mAttList->getActor();
            field_0x27f4 = field_0x27f0;
        }
    }

    mAttList2 = mAttention->getActionBtnXY();

    if (mAttList2 != NULL) {
        field_0x27f8 = mAttList2->getActor();
    }

    u8 var_r30;
    if (mTargetedActor != NULL) {
        if (fopAcM_GetGroup(mTargetedActor) == fopAc_ENEMY_e) {
            var_r30 = 3;
        } else {
            var_r30 = 2;
        }
        mAtnActorID = fopAcM_GetID(mTargetedActor);
    } else {
        if (checkAttentionLock()) {
            var_r30 = 1;
        } else {
            var_r30 = 0;
        }
        mAtnActorID = fpcM_ERROR_PROCESS_ID_e;
    }

    mZ2Link.setMarkState(var_r30);
}

void daAlink_c::setRStatus(u8 i_status) {
    dComIfGp_setRStatus(i_status, BUTTON_STATUS_FLAG_NONE);
}

void daAlink_c::setRStatusEmphasys(u8 i_status) {
    dComIfGp_setRStatus(i_status, BUTTON_STATUS_FLAG_EMPHASIS);
}

void daAlink_c::setDoStatus(u8 i_status) {
    dComIfGp_setDoStatus(i_status, BUTTON_STATUS_FLAG_NONE);
}

void daAlink_c::setDoStatusEmphasys(u8 i_status) {
    dComIfGp_setDoStatus(i_status, BUTTON_STATUS_FLAG_EMPHASIS);
}

void daAlink_c::setDoStatusContinuation(u8 i_status) {
    dComIfGp_setDoStatus(i_status, BUTTON_STATUS_FLAG_CONTINUATION);
}

void daAlink_c::setBStatus(u8 i_status) {
    dComIfGp_setAStatus(i_status, BUTTON_STATUS_FLAG_NONE);
}

BOOL daAlink_c::checkAtnWaitAnime() {
    if ((mTargetedActor != NULL
        && (checkEnemyGroup(mTargetedActor)
            || checkGoatCatchActor(mTargetedActor)
            || fopAcM_GetName(mTargetedActor) == PROC_CROD
            || fopAcM_GetName(mTargetedActor) == PROC_BOOMERANG
            )
        )
        || (checkGrabAnimeUp()
            || checkGrabAnimeSide()
            || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_23_e
            || checkGoatStopGame()
            )
        )
    {
        return true;
    }

    return false;
}

void daAlink_c::setTiredVoice(daPy_frameCtrl_c* i_frameCtrl) {
    if (i_frameCtrl->checkPass(12.0f)) {
        if (dComIfGs_getLife() <= 2) {
            voiceStart(Z2SE_AL_V_TIRED_L);
        } else {
            voiceStart(Z2SE_AL_V_TIRED_S);
        }
    }
}

BOOL daAlink_c::checkRestHPAnime() {
    if (!checkPlayerGuard()
        && (checkNoUpperAnime() || checkHorseTiredAnime())
        && mTargetedActor == NULL
        && !checkWindSpeedOnAngle()
        && ((!checkPlayerDemoMode() && dComIfGs_getLife() <= 4) || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_18_e)
        )
    {
        return true;
    }

    return false;
}

int daAlink_c::getDirectionFromAngle(s16 i_angle) {
    if (abs(i_angle) > 0x6000) {
        return DIR_BACKWARD;
    }

    if (i_angle >= 0x2000) {
        return DIR_LEFT;
    }

    if (i_angle <= -0x2000) {
        return DIR_RIGHT;
    }

    return DIR_FORWARD;
}

bool daAlink_c::checkAttentionState() {
    return (checkAttentionLock() || checkFmChainGrabAnime())
            || (mTargetedActor != NULL && (mTargetedActor == mThrowBoomerangAcKeep.getActor() || mTargetedActor == mCopyRodAcKeep.getActor()))
            || (checkUpperReadyThrowAnime() && mEquipItem != 0x102)
            || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_23_e;
}

s16 daAlink_c::getShapeAngleYAtnActor() {
    s16 var_r29;

    if (mTargetedActor != NULL) {
        if (mProcID == PROC_HOOKSHOT_WALL_SHOOT) {
            cXyz sp1C;

            int joint_no;
            if (field_0x3020 == 0) {
                joint_no = 7;
            } else {
                joint_no = 12;
            }

            mDoMtx_multVecZero(mpLinkModel->getAnmMtx(joint_no), &sp1C);
            var_r29 = cLib_targetAngleY(&sp1C, &mTargetedActor->eyePos);
        } else {
            cXyz sp28 = mTargetedActor->eyePos - current.pos;
            multVecMagneBootInvMtx(&sp28);
            var_r29 = sp28.atan2sX_Z();
        }

        if (checkWolfShapeReverse()) {
            var_r29 += 0x8000;
        }
    } else {
        var_r29 = shape_angle.y;
    }

    return var_r29;
}

bool daAlink_c::setShapeAngleToAtnActor(BOOL param_0) {
    if (mTargetedActor != NULL) {
        if ((mTargetedActor == mThrowBoomerangAcKeep.getActor() && (checkBoomerangThrowAnime() || !checkInputOnR())) ||
            (mTargetedActor == mCopyRodAcKeep.getActor() && (checkCopyRodThrowAnime() || !checkInputOnR())))
        {
            return true;
        }

        s16 angle = getShapeAngleYAtnActor();
        if (param_0 && abs((s16)(angle - shape_angle.y)) > 0x3000) {
            return true;
        }

        cLib_addCalcAngleS(&shape_angle.y, angle, 2, 0x2000, 0x800);
        return true;
    }

    return false;
}

void daAlink_c::initServiceWaitTime() {
    field_0x30ca = cM_rndF(150.0f) + 300.0f;

    if (checkWolf()) {
        field_0x30ca >>= 2;  // divide by 4
    } else if (checkReinRide()) {
        field_0x30ca *= 0.75f;
    }
}

bool daAlink_c::checkZeroSpeedF() const {
    return fabsf(mNormalSpeed) < 0.001f;
}

void daAlink_c::setNormalSpeedF(f32 i_speed, f32 i_deceleration) {
    f32 max_speed;
    if (getSumouMode()) {
        max_speed = mMoveValue * (mMaxSpeed * mMoveValue);
    } else {
        max_speed = mStickValue * (mMaxSpeed * mStickValue);
    }

    if (checkWolfSwimDashAnime() || checkUnderMove0BckNoArc(ANM_SWIM_DASH) || getZoraSwim()) {
        max_speed = mMaxSpeed;
    }

    f32 temp_f30;
    if ((checkUpperReadyThrowAnime() && mLinkAcch.ChkGroundHit() && field_0x2f8c == 1)
        || ((checkRoomOnly()
            || checkCopyRodControllAnime()
            || checkGrabSlowMoveActor()
            || (checkUpperGuardAnime() && field_0x2f8c == 1)
            || (mGndPolyAtt1 == 4 && mSinkShapeOffset < -15.0f)
            )
            && (mProcID == PROC_MOVE
                || mProcID == PROC_ATN_MOVE
                || mProcID == PROC_ATN_ACTOR_MOVE
                || mProcID == PROC_WOLF_MOVE
                || mProcID == PROC_WOLF_ATN_AC_MOVE
                || mProcID == PROC_MOVE_TURN
                )
            )
        )
    {
        temp_f30 = mMaxSpeed * mpHIO->mBasic.m.mIndoorSpeedFactor;
        if (max_speed > temp_f30) {
            max_speed = temp_f30;
        }
    }

    if (mGndPolyAtt1 == 3 && mSinkShapeOffset < -15.0f) {
        temp_f30 = 0.5f * mMaxSpeed;
        if (max_speed > temp_f30) {
            max_speed = temp_f30;
        }
    }

    if (checkNoResetFlg3(FLG3_UNK_1000)) {
        f32 sp18;
        if (checkWolf()) {
            sp18 = mpHIO->mWolf.mWlSwim.m.mStartHeight;
        } else {
            sp18 = mpHIO->mSwim.m.mStartHeight;
        }

        f32 sp14 = cLib_minMaxLimit<f32>((mWaterY - (30.0f + current.pos.y)) / (sp18 - 30.0f), 0.0f, 1.0f);
        temp_f30 = mMaxSpeed * (1.0f - (0.5f * sp14));
        if (max_speed > temp_f30) {
            max_speed = temp_f30;
        }
    }

    cM3dGPla slide_poly;
    if ((mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_14_e && mDemo.getParam1() == 0)
        || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_65_e
        || getSlidePolygon(&slide_poly)
        )
    {
        return;
    }

    s16 var_r29 = (current.angle.y + 0x8000) - field_0x30fa;
    if ((mLinkAcch.ChkWallHit() || fabsf(field_0x342c) > 1.0f || fabsf(field_0x3430) > 1.0f || (field_0x30fc != 0 && abs(var_r29) < 0x4000))
        && !checkMagneBootsOn()
        && (!getZoraSwim() || abs(field_0x3080) <= 0x2000)
        )
    {
        dBgS_AcchCir* acch_cir = mAcchCir;
        if (mLinkAcch.ChkWallHit()) {
            for (int i = 0; i < 3; i++, acch_cir++) {
                if (acch_cir->ChkWallHit()) {
                    var_r29 = (current.angle.y + 0x8000) - acch_cir->GetWallAngleY();
                    break;
                }
            }
        } else if (fabsf(field_0x342c) > 1.0f || fabsf(field_0x3430) > 1.0f) {
            field_0x30fc = 3;
            field_0x30fa = cM_atan2s(field_0x342c, field_0x3430);
            var_r29 = (current.angle.y + 0x8000) - field_0x30fa;
        } else if (field_0x30fc == 1 && mGrabItemAcKeep.getActor() != NULL && current.pos.abs2(field_0x3540) < 1.0f && dComIfG_Bgsp().ChkPolySafe(mPolyInfo4)) {
            field_0x30fc = 3;
        }

        int temp_r3 = abs(var_r29);
        if (temp_r3 < 0x400) {
            max_speed = 0.0f;
        } else if (temp_r3 < 0x4000) {
            max_speed *= 1.0f - cM_scos(var_r29);
        }
    }

    if (checkNoResetFlg1(FLG1_UNK_20000000)) {
        s16 temp_r29 = (current.angle.y + 0x8000) - field_0x3092;

        int temp_r3_2 = abs(temp_r29);
        if (temp_r3_2 < 0x4000) {
            if (checkModeFlg(0x40000)) {
                max_speed *= 1.0f - (0.8f * cM_scos(temp_r29));
            } else if (temp_r3_2 < 0x600) {
                max_speed = 0.0f;
            } else {
                max_speed *= 1.0f - cM_scos(temp_r29);
            }

            if (!checkWolf() && !checkMagneBootsOn()) {
                offNoResetFlg1(FLG1_UNK_20000000);
            }
        }
    }

    f32 move_speed;
    f32 target_speed;
    if (max_speed < mNormalSpeed || 0.0f == max_speed) {
        move_speed = mNormalSpeed - max_speed;
        move_speed = move_speed > i_deceleration ? i_deceleration : move_speed;
        i_speed = 0.0f;
        target_speed = max_speed;
    } else {
        move_speed = i_deceleration;
        target_speed = 0.0f;
    }

    if (!cM3d_IsZero(i_speed)) {
        mNormalSpeed += i_speed;
        if (mNormalSpeed > max_speed) {
            mNormalSpeed = max_speed;
        }
    } else {
        cLib_chaseF(&mNormalSpeed, target_speed, move_speed);
    }
}

f32 daAlink_c::getStickAngleDistanceRate() {
    f32 rate;

    if (abs((s16)(mPrevStickAngle - mStickAngle)) > 0x1000) {
        rate = cM_scos(mMoveAngle - current.angle.y);

        if (mNormalSpeed > 0.5f * mMaxSpeed) {
            if (rate < 0.69999999f) {
                rate = 0.69999999f;
            }
        } else if (rate < 0.0f) {
            rate = 0.0f;
        }
    } else {
        rate = 1.0f;
    }

    return rate;
}

void daAlink_c::setSpeedAndAngleNormal() {
    bool var_r26 = false;

    if ((mLinkAcch.ChkWallHit() || field_0x30fc != 0) && !checkMagneBootsOn()) {
        if (mLinkAcch.ChkWallHit()) {
            dBgS_AcchCir* acchCir = mAcchCir;
            for (int i = 0; i < 3; i++, acchCir++) {
                if (acchCir->ChkWallHit()) {
                    abs((s16)((current.angle.y + 0x8000) - acchCir->GetWallAngleY()));
                    break;
                }
            }
        }

        if (field_0x30fc != 0) {
            if (abs((s16)((current.angle.y + 0x8000) - field_0x30fa)) < 0x4000) {
                var_r26 = true;
            }
        }
    }

    if (checkMagneBootsOn() || checkNoResetFlg1(FLG1_UNK_20000000)) {
        var_r26 = true;
    }

    f32 speed;
    if (checkInputOnR()) {
        BOOL var_r24 = false;
        f32 temp_f30 = mMoveValue * mMoveValue;

        if (!checkAttentionLock() && cLib_distanceAngleS(mMoveAngle, current.angle.y) > 0x7800 && mProcID != PROC_MOVE_TURN) {
            if (checkModeFlg(1) || speedF < 0.001f) {
                return;
            }

            if (mProcID == PROC_MOVE) {
                if ((speedF / mMaxSpeed) > mpHIO->mMove.m.mSlideThresholdSpeed) {
                    if (getDirectionFromAngle(mPrevStickAngle - mStickAngle) == DIR_BACKWARD) {
                        return;
                    }
                }

                if ((speedF / mMaxSpeed) <= mpHIO->mMove.m.mSlideThresholdSpeed) {
                    cLib_addCalcAngleS(&current.angle.y, mMoveAngle, mpHIO->mMove.m.mTurnAngleRate, mpHIO->mMove.m.mMaxTurnAngle, mpHIO->mMove.m.mMinTurnAngle);
                    return;
                }
                var_r24 = true;
            } else {
                cLib_addCalcAngleS(&current.angle.y, mMoveAngle, mpHIO->mMove.m.mTurnAngleRate, mpHIO->mMove.m.mMaxTurnAngle, mpHIO->mMove.m.mMinTurnAngle);
            }
        } else {
            s16 maxStep;
            s16 minStep;

            if (checkEventRun()) {
                maxStep = mpHIO->mMove.m.mMaxTurnAngle;
                minStep = mpHIO->mMove.m.mMinTurnAngle;
            } else {
                maxStep = mpHIO->mMove.m.mMaxTurnAngle * temp_f30;
                if (maxStep < 10) {
                    maxStep = 10;
                }

                minStep = mpHIO->mMove.m.mMinTurnAngle * temp_f30;
                if (minStep < 1) {
                    minStep = 1;
                }
            }

            cLib_addCalcAngleS(&current.angle.y, mMoveAngle, mpHIO->mMove.m.mTurnAngleRate, maxStep, minStep);
        }

        if (!var_r24) {
            f32 temp_f28 = getStickAngleDistanceRate();
            if (checkEventRun()) {
                speed = temp_f28 * (mpHIO->mMove.m.mAcceleration * mStickValue);
            } else {
                f32 var_f29 = 0.3f * (1.0f - fabsf(mNormalSpeed / mMaxSpeed));
                if (checkHeavyStateOn(TRUE, TRUE)) {
                    var_f29 *= mHeavySpeedMultiplier;
                }

                if (mStickValue > var_f29) {
                    speed = temp_f28 * (mpHIO->mMove.m.mAcceleration * temp_f30);
                } else {
                    speed = 0.0f;
                }
            }
        } else {
            speed = 0.0f;
        }
    } else {
        speed = 0.0f;
    }

    if (!checkAttentionLock() && mProcID != PROC_MOVE_TURN && checkInputOnR()) {
        s16 sp8 = shape_angle.y;
        cLib_addCalcAngleS(&shape_angle.y, mMoveAngle, mpHIO->mMove.m.mTurnAngleRate, mpHIO->mMove.m.mMaxTurnAngle * 2, mpHIO->mMove.m.mMinTurnAngle * 2);

        int temp_r23 = (s16)(sp8 - current.angle.y) * (s16)(shape_angle.y - current.angle.y);
        if (temp_r23 <= 0 && temp_r23 >= -0x1000000) {
            shape_angle.y = current.angle.y;
        }
    }

    setNormalSpeedF(speed, mpHIO->mMove.m.mDeceleration);
}

void daAlink_c::setSpeedAndAngleAtn() {
    int var_r29 = field_0x2f98;

    if (checkInputOnR()) {
        s16 temp_r0 = mMoveAngle - shape_angle.y;
        f32 temp_f27 = cM_ssin(temp_r0);
        f32 temp_f29 = cM_scos(temp_r0);

        if (mTargetedActor == NULL && !checkGoatStopGame() && (temp_f29 <= -0.99f || temp_f29 >= 0.99f)) {
            if (temp_f29 <= -0.99f) {
                var_r29 = 1;
            } else {
                var_r29 = 0;
            }
        } else if (temp_f29 < -0.866f) {
            var_r29 = 1;
        } else if (temp_f27 > 0.0f) {
            var_r29 = 2;
        } else if (temp_f27 < 0.0f) {
            var_r29 = 3;
        }
    }

    s16 angleRate;
    s16 angleMaxStep;
    s16 angleMinStep;

    f32 acceleration;
    f32 deceleration;
    f32 speed;

    if (mTargetedActor != NULL) {
        angleRate = mpHIO->mAtnMove.m.mTurnAngleRate;
        angleMaxStep = mpHIO->mAtnMove.m.mMaxTurnAngle;
        angleMinStep = mpHIO->mAtnMove.m.mMinTurnAngle;

        if (var_r29 == 1) {
            acceleration = mpHIO->mAtnMove.m.mBackAcceleration;
            deceleration = mpHIO->mAtnMove.m.mBackDeceleration;
        } else {
            acceleration = mpHIO->mAtnMove.m.mAcceleration;
            deceleration = mpHIO->mAtnMove.m.mDeceleration;
        }
    } else {
        if (var_r29 == 0) {
            if (checkInputOnR()) {
                if (getDirectionFromCurrentAngle() == DIR_BACKWARD) {
                    current.angle.y += 0x8000;
                    mNormalSpeed *= -1.0f;
                }
                if (checkZeroSpeedF()) {
                    current.angle.y = mMoveAngle;
                }
            }

            setSpeedAndAngleNormal();
            return;
        }

        angleRate = mpHIO->mNoActAtnMove.m.mTurnAngleRate;
        angleMaxStep = mpHIO->mNoActAtnMove.m.mMaxTurnAngle;
        angleMinStep = mpHIO->mNoActAtnMove.m.mMinTurnAngle;

        if (var_r29 == 1) {
            acceleration = mpHIO->mNoActAtnMove.m.mBackAcceleration;
            deceleration = mpHIO->mNoActAtnMove.m.mBackDeceleration;
        } else {
            acceleration = mpHIO->mNoActAtnMove.m.mAcceleration;
            deceleration = mpHIO->mNoActAtnMove.m.mDeceleration;
        }
    }

    if (checkInputOnR()) {
        if (getDirectionFromCurrentAngle() == DIR_BACKWARD) {
            current.angle.y += 0x8000;
            mNormalSpeed *= -1.0f;
        }

        s16 targetAngle;
        if (checkUnderMove0BckNoArc(ANM_CHAIN_PULL_END)) {
            targetAngle = getChainStickAngleY(shape_angle.y);
        } else {
            targetAngle = mMoveAngle;
        }

        if (checkZeroSpeedF()) {
            current.angle.y = targetAngle;
        }

        s16 temp_r23 = current.angle.y;
        cLib_addCalcAngleS(&current.angle.y, targetAngle, angleRate, angleMaxStep, angleMinStep);

        if (mStickValue > (0.3f * (1.0f - fabsf(mNormalSpeed / mMaxSpeed)))) {
            speed = acceleration * mStickValue * cM_scos((current.angle.y - temp_r23));
        } else {
            speed = 0.0f;
        }
    } else {
        speed = 0.0f;
    }

    if (mTargetedActor != NULL) {
        setShapeAngleToAtnActor(0);
    } else {
        shape_angle.y = field_0x2fe4;
    }

    setNormalSpeedF(speed, deceleration);
}

int daAlink_c::checkRequestTalkActor(dAttList_c* i_attList, fopAc_ac_c* i_actor) {
    if (i_attList != NULL &&
        (i_attList->mType == fopAc_attn_SPEAK_e || (i_attList->mType == fopAc_attn_TALK_e && mTargetedActor == i_actor)))
    {
        return true;
    }

    return false;
}

bool daAlink_c::checkServiceWaitMode() {
    return !checkEventRun() &&
           !checkEndResetFlg0(ERFLG0_BOSS_ROOM_WAIT) &&
           checkNoUpperAnime() &&
           !checkWindSpeedOnXZ() &&
           field_0x3124.y == 0 &&
           field_0x3124.z == 0 &&
           field_0x3124.x == 0 &&
           !checkFishingRodAndLureItem() &&
           mSinkShapeOffset >= -30.0f;
}

void daAlink_c::setJumpMode() {
    onModeFlg(MODE_JUMP);
    mLastJumpPos = current.pos;
    mFallHeight = mLastJumpPos.y;
    field_0x33c8 = mLastJumpPos.y;
}

f32 daAlink_c::getMetamorphoseNearDis() const {
    return mpHIO->mBasic.m.mTransformBlockNearDis;
}

f32 daAlink_c::getMetamorphoseFarDis() const {
    return mpHIO->mBasic.m.mTransformBlockFarDis;
}

s16 daAlink_c::getMetamorphoseFarAngle() const {
    return mpHIO->mBasic.m.mTransformBlockFarAngle;
}

void daAlink_c::setMidnaMsg() {
    if (mMidnaMsg != NULL) {
        mMidnaMsg->onUseFlg();
    }
}

BOOL daAlink_c::notTalk() {
    BOOL isSafePoly;
    if (dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd) && dBgS_CheckBGroundPoly(mLinkAcch.m_gnd)) {
        isSafePoly = true;
    } else {
        isSafePoly = false;
    }

    if ((mMidnaTalkDelayTimer != 0 || (checkMagneBootsOn() && !cBgW_CheckBGround(mMagneBootsTopVec.y)))
        || (!checkModeFlg(MODE_SWIMMING | MODE_ROPE_WALK | MODE_VINE_CLIMB | MODE_UNK_800 | MODE_RIDING | MODE_NO_COLLISION | MODE_CLIMB | MODE_JUMP)
            && !isSafePoly
            )
        )
    {
        return true;
    }

    return false;
}

BOOL daAlink_c::setTalkStatus() {
    if (checkEndResetFlg0(ERFLG0_BEE_FOLLOW) || field_0x27f4 == NULL) {
        return 0;
    }

    if (checkRequestTalkActor(mAttList, field_0x27f4)) {
        if (notTalk()) {
            setDoStatus(BUTTON_STATUS_UNK_144);
        } else {
            if (field_0x27f4->attention_info.flags & fopAc_AttnFlag_NOTALK_e) {
                return 0;
            }

            if (field_0x27f4->attention_info.flags & fopAc_AttnFlag_UNK_0x800000) {
                setDoStatus(BUTTON_STATUS_LISTEN);
            } else if (field_0x27f4->attention_info.flags & fopAc_AttnFlag_TALKCHECK_e) {
                if (fopAcM_GetName(field_0x27f4) == PROC_OBJ_SSDRINK) {
                    setDoStatus(BUTTON_STATUS_DRINK);
                } else {
                    setDoStatus(BUTTON_STATUS_CHECK);
                }
            } else if (field_0x27f4->attention_info.flags & fopAc_AttnFlag_TALKREAD_e) {
                setDoStatus(BUTTON_STATUS_UNK_128);
            } else if (field_0x27f4->attention_info.flags & fopAc_AttnFlag_UNK_0x8000000) {
                setDoStatus(BUTTON_STATUS_LOOK);
            } else {
                setDoStatus(BUTTON_STATUS_SPEAK);
            }
        }

        return 1;
    }

    return 0;
}

f32 daAlink_c::getFrontRollRate() {
    if (checkHeavyStateOn(TRUE, TRUE)) {
        return 0.75f * mHeavySpeedMultiplier;
    } else {
        return 0.75f;
    }
}

void daAlink_c::decideCommonDoStatus() {
    if (!checkFmChainGrabAnime() && dComIfGp_getDoStatus() == BUTTON_STATUS_NONE) {
        bool isFshopStage = checkStageName("R_SP127");

        if (checkRoomOnly() && !checkWolf() && !isFshopStage) {
            if ((checkNoUpperAnime() || checkIronBallWaitAnime()) &&
                (mEquipItem != fpcNm_ITEM_NONE && checkModeFlg(4) && mEquipItem != 0x102))
            {
                if (checkCopyRodControllAnime()) {
                    setDoStatus(BUTTON_STATUS_QUIT);
                } else {
                    setDoStatus(BUTTON_STATUS_PUT_AWAY);
                }
            } else if (mFastShotTime != 0) {
                setDoStatus(BUTTON_STATUS_BACK);
            }
        } else if (checkWolf()) {
            if (checkDownAttackState()) {
                setDoStatusEmphasys(BUTTON_STATUS_FINISH);
            } else if (checkCutHeadState()) {
                setDoStatusEmphasys(BUTTON_STATUS_HELM_SPLITTER);
            } else if (checkWolfSideStep()) {
                setDoStatusEmphasys(BUTTON_STATUS_JUMP);
            } else {
                if (mTargetedActor != NULL) {
                    if (fopAcM_GetName(mTargetedActor) == PROC_Tag_Wljump) {
                        if (static_cast<daTagWljump_c*>(mTargetedActor)->getLockPos() != NULL) {
                            if (!getMidnaActor()->checkNoInput()) {
                                setDoStatus(BUTTON_STATUS_UNK_147);
                            }

                            onResetFlg0(RFLG0_WOLF_TAG_LOCK_JUMP_READY);
                            field_0x3738 = *static_cast<daTagWljump_c*>(mTargetedActor)->getLockPos();
                        }
                    } else {
                        setDoStatus(BUTTON_STATUS_UNK_139);
                    }
                } else if (checkAttentionLock()) {
                    setDoStatus(BUTTON_STATUS_UNK_139);
                } else if (field_0x30d2 == 0 &&
                           (mStickValue > getFrontRollRate() || checkAttentionLock()))
                {
                    setDoStatus(BUTTON_STATUS_DASH);
                }
            }

            if (checkNotJumpSinkLimit() &&
                (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_139
                    || dComIfGp_getDoStatus() == BUTTON_STATUS_FINISH
                    || dComIfGp_getDoStatus() == BUTTON_STATUS_HELM_SPLITTER
                    || dComIfGp_getDoStatus() == BUTTON_STATUS_JUMP
                    || dComIfGp_getDoStatus() == BUTTON_STATUS_DASH
                    || dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_147
                )
                )
            {
                setDoStatus(BUTTON_STATUS_NONE);
            }
        } else {
            int direction = getDirectionFromShapeAngle();
            f32 frontRollRate = getFrontRollRate();

            if (checkAttentionLock() ||
                (mTargetedActor != NULL && (mTargetedActor == mThrowBoomerangAcKeep.getActor() ||
                                            mTargetedActor == mCopyRodAcKeep.getActor())))
            {
                if (checkInputOnR() && direction != DIR_FORWARD) {
                    if (mEquipItem == 0x103 && checkDownAttackState()) {
                        setDoStatusEmphasys(BUTTON_STATUS_FINISH);
                    } else if (mEquipItem == 0x103 && checkCutHeadState()) {
                        setDoStatusEmphasys(BUTTON_STATUS_HELM_SPLITTER);
                    } else {
                        setDoStatusEmphasys(BUTTON_STATUS_JUMP);
                    }
                } else if (!checkSmallUpperGuardAnime() && (mEquipItem == 0x103 || mEquipItem == 0x102)) {
                    if (mEquipItem == 0x102) {
                        setDoStatus(BUTTON_STATUS_THROW);
                    } else if (mEquipItem == 0x103 && checkDownAttackState()) {
                        setDoStatusEmphasys(BUTTON_STATUS_FINISH);
                    } else if (mEquipItem == 0x103 && checkCutHeadState()) {
                        setDoStatusEmphasys(BUTTON_STATUS_HELM_SPLITTER);
                    } else {
                        setDoStatus(BUTTON_STATUS_UNK_134);
                    }
                } else {
                    setDoStatus(BUTTON_STATUS_UNK_121);
                }
            } else if (((checkNoUpperAnime()) || checkIronBallWaitAnime() ||
                        checkCopyRodControllAnime()) &&
                       mEquipItem != fpcNm_ITEM_NONE && checkModeFlg(4) && mStickValue <= frontRollRate)
            {
                if (mEquipItem == 0x102) {
                    if (checkAttentionState()) {
                        setDoStatus(BUTTON_STATUS_THROW);
                    } else {
                        setDoStatus(BUTTON_STATUS_PLACE);
                    }
                } else {
                    setDoStatus(BUTTON_STATUS_PUT_AWAY);
                }
            } else if (mStickValue > frontRollRate) {
                if (mEquipItem == 0x102) {
                    setDoStatus(BUTTON_STATUS_THROW);
                } else {
                    setDoStatus(BUTTON_STATUS_UNK_121);
                }
            } else if (mFastShotTime != 0) {
                setDoStatus(BUTTON_STATUS_BACK);
            }

            if ((((checkMagneBootsOn() || checkIronBallWaitAnime() || checkNotJumpSinkLimit())
                    && (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_121
                        || dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_134
                        || dComIfGp_getDoStatus() == BUTTON_STATUS_FINISH
                        || dComIfGp_getDoStatus() == BUTTON_STATUS_HELM_SPLITTER
                        || dComIfGp_getDoStatus() == BUTTON_STATUS_JUMP
                        )
                 ) || (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_121
                        && (checkKandelaarSwingAnime()
                            || mGndPolySpecialCode == dBgW_SPCODE_HEAVY_SNOW
                            || checkCopyRodThrowAnime()
                            || checkBoomerangThrowAnime()
                            )
                        )
                 ) && (!checkMagneBootsOn() || dComIfGp_getDoStatus() != BUTTON_STATUS_UNK_121 || !cBgW_CheckBGround(mMagneBootsTopVec.y))
                )
            {
                if (mEquipItem == 0x42 && checkModeFlg(4)) {
                    setDoStatus(BUTTON_STATUS_PUT_AWAY);
                } else {
                    setDoStatus(BUTTON_STATUS_NONE);
                }
            }

            if (isFshopStage && dComIfGp_getDoStatus() == BUTTON_STATUS_JUMP) {
                setDoStatus(BUTTON_STATUS_NONE);
            }

            if (dComIfGp_getDoStatus() == BUTTON_STATUS_PUT_AWAY && checkCopyRodControllAnime()) {
                setDoStatus(BUTTON_STATUS_QUIT);
            }
        }
    }
}

void daAlink_c::decideDoStatus() {
    if (!checkFmChainGrabAnime()) {
        if (checkNoResetFlg0(FLG0_UNK_1000000)) {
            setDoStatusEmphasys(BUTTON_STATUS_DRAW);
        }

        if (mTargetedActor == NULL && mAttList == NULL && checkResetFlg0(RFLG0_UNK_8)) {
            setWallGrabStatus(BUTTON_STATUS_GRAB, BUTTON_STATUS_FLAG_EMPHASIS);

            if (field_0x2f91 == 7 || field_0x2f91 == 8 || field_0x2f91 == 6 || field_0x2f91 == 9) {
                setDoStatus(BUTTON_STATUS_UNK_32);
            }
        } else {
            if (searchFmChainPos()) {
                setChainGrabStatus(BUTTON_STATUS_UNK_150);
            } else if (mAttList != NULL) {
                s16 actor_name = fopAcM_GetName(field_0x27f4);

                if (mAttList->mType == fopAc_attn_DOOR_e ||
                    (mAttList->mType == fopAc_attn_JUEL_e &&
                     (!checkWolf() || static_cast<daTbox_c*>(field_0x27f4)->checkSmallTbox())))
                {
                    setDoStatus(BUTTON_STATUS_OPEN);
                } else if (mAttList->mType == fopAc_attn_ETC_e && actor_name == PROC_KYTAG05) {
                    setDoStatus(BUTTON_STATUS_UNK_137);
                } else if (checkWolf()) {
                    if (mAttList->mType == fopAc_attn_CARRY_e && !fopAcM_checkCarryNow(field_0x27f4) &&
                        fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY_LIGHT))
                    {
                        if (actor_name == PROC_B_MGN) {
                            setDoStatus(BUTTON_STATUS_UNK_145);
                        } else if (actor_name == PROC_Obj_Yobikusa) {
                            setDoStatusEmphasys(BUTTON_STATUS_HOWL);
                        } else if (actor_name == PROC_Obj_Stone) {
                            setDoStatus(BUTTON_STATUS_ROLL);
                        } else if (actor_name == PROC_Obj_Drop || actor_name == PROC_Obj_Sword ||
                                   actor_name == PROC_Obj_SmallKey)
                        {
                            setDoStatusEmphasys(BUTTON_STATUS_UNK_57);
                        } else if (!checkGoatCatchActor(field_0x27f4) && !checkGrabLineCheck()) {
                            setDoStatusEmphasys(BUTTON_STATUS_PICK_UP);
                        }
                    } else if (mTargetedActor != NULL &&
                               fopAcM_GetName(mTargetedActor) == PROC_B_MGN &&
                               mAttention->getActionBtnB() != NULL &&
                               mAttention->getActionBtnB()->mType == fopAc_attn_CARRY_e &&
                               mAttention->getActionBtnB()->getActor() == mTargetedActor)
                    {
                        setDoStatus(BUTTON_STATUS_UNK_145);
                    } else if (mTargetedActor != NULL &&
                               fopAcM_GetName(mTargetedActor) == PROC_E_YM &&
                               mAttention->getActionBtnB() != NULL &&
                               mAttention->getActionBtnB()->mType == fopAc_attn_ETC_e &&
                               mAttention->getActionBtnB()->getActor() == mTargetedActor)
                    {
                        onEndResetFlg1(ERFLG1_UNK_100000);
                        if (mWolfEyeUp != 0) {
                            setWolfDigStatus(BUTTON_STATUS_FLAG_EMPHASIS);
                        }
                    } else if (mAttList->mType == fopAc_attn_ETC_e && field_0x27f4 != NULL) {
                        if (actor_name == PROC_Obj_Digpl || actor_name == PROC_Obj_Digholl ||
                            actor_name == PROC_Obj_DigSnow || actor_name == PROC_Obj_Lv4DigSand ||
                            actor_name == PROC_E_YM)
                        {
                            onEndResetFlg1(ERFLG1_UNK_100000);
                            if (mWolfEyeUp != 0) {
                                setWolfDigStatus(BUTTON_STATUS_FLAG_EMPHASIS);
                            }
                        } else if (actor_name == PROC_TAG_HOWL ||
                                   actor_name == PROC_Obj_WindStone ||
                                   actor_name == PROC_Obj_SmWStone ||
                                   actor_name == PROC_Tag_WaraHowl)
                        {
                            setDoStatusEmphasys(BUTTON_STATUS_HOWL);
                        } else if (actor_name == PROC_KYTAG03) {
                            setDoStatusEmphasys(BUTTON_STATUS_SNIFF);
                        }
                    } else if (mTargetedActor != NULL && field_0x27f4 == mTargetedActor &&
                               actor_name == PROC_Obj_Wchain)
                    {
                        setDoStatusEmphasys(BUTTON_STATUS_UNK_123);
                    } else {
                        setTalkStatus();
                    }
                } else if (mAttList->mType == fopAc_attn_ETC_e) {
                    if ((!checkMagicArmorHeavy() &&
                         (actor_name == PROC_HORSE || actor_name == PROC_E_WB) &&
                         checkReinRideBgCheck()) ||
                        actor_name == PROC_CANOE || actor_name == PROC_Obj_IceLeaf)
                    {
                        setDoStatus(BUTTON_STATUS_GET_ON);
                    } else if (actor_name == PROC_COW) {
                        setDoStatus(BUTTON_STATUS_PET);
                    } else if (actor_name == PROC_Obj_YIblltray) {
                        setDoStatusEmphasys(BUTTON_STATUS_UNK_153);
                    } else if (actor_name == PROC_Tag_Lv6Gate ||
                               (actor_name == PROC_TAG_KMSG &&
                                static_cast<daTag_KMsg_c*>(field_0x27f4)->getType() == 3))
                    {
                        if (!checkEquipAnime() && checkMasterSwordEquip()) {
                            setDoStatus(BUTTON_STATUS_STRIKE);
                        }
                    }
                } else if (mTargetedActor != NULL && checkGoatCatchActor(mTargetedActor) &&
                           mAttention->getActionBtnB() != NULL &&
                           mAttention->getActionBtnB()->mType == fopAc_attn_CARRY_e &&
                           mAttention->getActionBtnB()->getActor() == mTargetedActor)
                {
                    setDoStatus(BUTTON_STATUS_UNK_145);
                } else if (mAttList->mType == fopAc_attn_CARRY_e) {
                    if (!fopAcM_checkCarryNow(field_0x27f4)) {
                        if (checkGoatCatchActor(field_0x27f4)) {
                            setDoStatus(BUTTON_STATUS_UNK_145);
                        } else if (!checkMagneBootsOn()) {
                            if (checkInsectActorName(field_0x27f4)) {
                                setDoStatusEmphasys(BUTTON_STATUS_UNK_152);
                            } else if (actor_name == PROC_Obj_SmallKey) {
                                setDoStatusEmphasys(BUTTON_STATUS_UNK_57);
                            } else if (!checkGrabLineCheck()) {
                                if (actor_name == PROC_Obj_Yobikusa) {
                                    setDoStatus(BUTTON_STATUS_PICK);
                                } else if (fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY_TYPE_8))
                                {
                                    setDoStatusEmphasys(BUTTON_STATUS_UNK_52);
                                } else if (actor_name != PROC_B_MGN) {
                                    setDoStatusEmphasys(BUTTON_STATUS_UNK_31);
                                }
                            }
                        }
                    }
                } else {
                    setTalkStatus();
                }
            }
            decideCommonDoStatus();
        }
    }
}

BOOL daAlink_c::checkWaitAction() {
    if (checkWolf()) {
        if (checkModeFlg(0x01000000)) {
            if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_START_e) {
                return 0;
            }
            return procWolfWaitInit();
        }

        if (checkNoUpperAnime() && checkRestHPAnime() && shape_angle.y == mPrevAngleY && (!checkAttentionState() || shape_angle.y == field_0x2fe4)) {
            return procWolfTiredWaitInit();
        }

        return procWolfWaitInit();
    }

    if (checkModeFlg(0x400) && mRideStatus != 0) {
        if (checkCanoeRide()) {
            if (checkGrabAnimeAndThrow()) {
                return procCanoeGrabMoveInit();
            }
            return procCanoeWaitInit(0);
        }

        if (checkBoardRide()) {
            return procBoardWaitInit(NULL);
        }

        if (checkSpinnerRide()) {
            return procSpinnerWaitInit();
        }

        if (checkGrabAnimeAndThrow()) {
            return procHorseGrabMoveInit();
        }

        return procHorseWaitInit();
    }

    if (dComIfGp_checkPlayerStatus1(0, 0x10000)) {
        return procHookshotRoofWaitInit(0, mCargoCarryAcKeep.getActor(), 0);
    }

    if (dComIfGp_checkPlayerStatus1(0, 0x02000000)) {
        return procHookshotWallWaitInit(0, 0, 0);
    }

    if (checkModeFlg(0x40000) && !checkEndResetFlg1(ERFLG1_UNK_100)) {
        return procSwimWaitInit(0);
    }

    if (mMode == 1) {
        return procSumouReadyInit();
    }

    if (checkGrabAnime()) {
        return procGrabWaitInit();
    }

    if (checkModeFlg(0x01000000) && mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_START_e) {
        return 0;
    }

    if (checkRestHPAnime()
        && !checkPlayerGuardAndAttack()
        && shape_angle.y == mPrevAngleY
        && !checkEndResetFlg0(ERFLG0_UNK_8000000)
        && !checkUnderMove0BckNoArc(ANM_WAIT_B)
        && !checkUnderMove0BckNoArc(ANM_WAIT_B_TO_A))
    {
        return procTiredWaitInit();
    }

    return procWaitInit();
}

void daAlink_c::setFallVoice() {
    if (mFallVoiceInit == 0 && 0.0099999998f * (mFallHeight - current.pos.y) > 20.0f) {
        voiceStart(Z2SE_AL_V_FALL);
        mFallVoiceInit = 1;
    }
}

void daAlink_c::setLandPassiveData() {
    if (field_0x30f0 == 0) {
        if (doTrigger()) {
            field_0x30f0 = 65;
        }
    } else {
        field_0x30f0--;
    }
}

void daAlink_c::setStepLandVibration() {
    int shockMode;
    if (checkBootsOrArmorHeavy()) {
        shockMode = VIBMODE_S_POWER3;
    } else {
        shockMode = VIBMODE_S_POWER1;
    }

    dComIfGp_getVibration().StartShock(shockMode, 15, cXyz(0.0f, 1.0f, 0.0f));
}

BOOL daAlink_c::checkLandAction(int param_0) {
    f32 temp_f31 = 0.0099999998f * (mFallHeight - current.pos.y);
    int face_direction = getDirectionFromCurrentAngle();
    int move_direction = getDirectionFromAngle(current.angle.y - shape_angle.y);
    BOOL isRollLand = mStickValue > 0.8f && move_direction != DIR_BACKWARD && !checkGrabAnime() &&
                   mProcID != PROC_CUT_HEAD && face_direction == DIR_FORWARD;

    if ((param_0 || temp_f31 >= mpHIO->mDamage.mDamFall.m.mMinRollHeight) &&
        temp_f31 < mpHIO->mDamage.mDamFall.m.mMaxRollHeight && isRollLand)
    {
        if (move_direction == DIR_FORWARD) {
            procFrontRollInit();
        } else {
            procSideRollInit(move_direction);
        }
    } else if (checkNoLandDamageSlidePolygon() ||
               temp_f31 < mpHIO->mDamage.mDamFall.m.mSmallDmgHeight)
    {
        if (param_0) {
            procLandDamageInit(0);
        } else if (isRollLand && field_0x30f0 > 50) {
            if (move_direction == DIR_FORWARD) {
                procFrontRollInit();
            } else {
                procSideRollInit(move_direction);
            }
        } else if (checkInputOnR() && move_direction == DIR_FORWARD && face_direction == DIR_FORWARD &&
                   mNormalSpeed > 5.0f)
        {
            if (mNormalSpeed > mpHIO->mMove.m.mMaxSpeed) {
                mNormalSpeed = mpHIO->mMove.m.mMaxSpeed;
            }
            mNormalSpeed *= 0.6f;
            current.angle.y = shape_angle.y;
            procMoveInit();
            setFootEffectProcType(2);
            field_0x2f9d = 4;
            resetBasAnime();
            seStartMapInfo(Z2SE_FN_JUMP_DUMMY);

            dComIfGp_getVibration().StartShock(1, 15, cXyz(0.0f, 1.0f, 0.0f));
        } else {
            procLandInit(temp_f31);
        }
    } else {
        procLandDamageInit(temp_f31 < mpHIO->mDamage.mDamFall.m.mBigDmgHeight ? 1 : 2);
    }

    return 1;
}

BOOL daAlink_c::checkSlideAction() {
    cM3dGPla tripla;
    if (getSlidePolygon(&tripla)) {
        s16 angle = tripla.mNormal.atan2sX_Z();
        if (field_0x2f8c != 0) {
            cLib_chaseF(&mNormalSpeed, 0.0f, field_0x3424);
            field_0x3090--;

            if (mNormalSpeed > 5.0f || field_0x3090 > 0) {
                return false;
            }
        }

        if (checkWolf()) {
            return procWolfSlideInit(angle, 0);
        } else {
            return procSlideInit(angle);
        }
    } else {
        field_0x3090 = 8;
    }

    return false;
}

BOOL daAlink_c::checkAutoJumpAction() {
    if (checkMagneBootsFly()) {
        return 1;
    }

    if (mGndPolyAtt1 == 0xFF || checkFlyAtnWait()) {
        return 1;
    }

    if ((mProcID == PROC_WOLF_LOCK_ATTACK || mProcID == PROC_WOLF_LOCK_ATTACK_TURN) && mProcVar5.field_0x3012 != 0) {
        return 0;
    }

    if (!checkModeFlg(0x70C12)
        && mProcID != PROC_DOOR_OPEN
        && mProcID != PROC_WARP
        && !getSumouMode()
        && mDemo.getDemoType() != daPy_demo_c::DEMO_TYPE_SPECIAL_e
        && !checkMagneBootsOn()
        && !mLinkAcch.ChkGroundHit())
    {
        if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
            speed.y = 0.0f;
            return procSwimWaitInit(0);
        }

        f32 sp28 = mLinkAcch.GetGroundH() - current.pos.y;
        if (sp28 < l_autoDownHeight) {
            if (checkFmChainGrabAnime()) {
                resetUpperAnime(UPPER_2, -1.0f);
            }

            f32 var_f31 = field_0x35c4.x + (mWindSpeed.x + (speed.x + field_0x3594.x));
            f32 var_f30 = field_0x35c4.z + (mWindSpeed.z + (speed.z + field_0x3594.z));
            BOOL var_r28 = true;

            f32 var_f29 = JMAFastSqrt(SQUARE(var_f31) + SQUARE(var_f30));
            if (var_f29 < 0.001f) {
                var_f31 = current.pos.x - field_0x3540.x;
                var_f30 = current.pos.z - field_0x3540.z;

                var_f29 = JMAFastSqrt(SQUARE(var_f31) + SQUARE(var_f30));
                if (var_f29 < 0.001f) {
                    var_r28 = false;
                }
            }

            f32 sp24;
            if (var_r28) {
                s16 temp_r26 = cM_atan2s(var_f31, var_f30);
                var_f29 = 1.0f / var_f29;
                f32 sp20 = var_f31 * var_f29;
                f32 sp1C = var_f30 * var_f29;

                current.pos.y -= speed.y;

                cXyz sp38;
                cXyz sp2C;
                sp38.set(field_0x3798.x - (30.0f * sp20), 100.0f + field_0x3798.y, field_0x3798.z - (30.0f * sp1C));
                mLinkGndChk.SetPos(&sp38);

                cM3dGPla sp58;
                bool var_r27;
                if (dComIfG_Bgsp().GroundCross(&mLinkGndChk) > current.pos.y - 100.0f) {
                    var_r27 = dComIfG_Bgsp().GetTriPla(mLinkGndChk, &sp58);
                } else {
                    var_r27 = false;
                }

                sp38.set(current.pos.x + (30.0f * sp20), current.pos.y - 5.0f, current.pos.z + (30.0f * sp1C));
                sp2C.set(current.pos.x - (100.0f * sp20), sp38.y, current.pos.z - (100.0f * sp1C));

                cM3dGPla sp44;
                bool var_r29;
                if (commonLineCheck(&sp38, &sp2C)) {
                    var_r29 = dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp44);
                } else {
                    var_r29 = false;
                }

                if (var_r29 && !cBgW_CheckBWall(sp44.mNormal.y)) {
                    sp38.y -= 50.0f;
                    sp2C.y -= 50.0f;

                    if (commonLineCheck(&sp38, &sp2C)) {
                        var_r29 = dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp44);
                    } else {
                        var_r29 = false;
                    }
                }

                f32 sp18;
                f32 sp14 = mNormalSpeed;
                f32 sp10;
                if (checkWolf()) {
                    if (checkWolfDashMode()) {
                        sp10 = mpHIO->mWolf.mWlAutoJump.m.mJumpLimitSpeed;
                    } else {
                        sp10 = mpHIO->mWolf.mWlAutoJump.m.mWeakJumpLimitSpeed;
                    }
                    sp18 = 0.0f;
                } else {
                    if (checkIronBallWaitAnime()) {
                        sp10 = 3.0f;
                    } else {
                        sp10 = mpHIO->mAutoJump.m.mJumpSpeedLimit;
                    }
                    sp18 = 60.0f;
                }

                if (dComIfGs_getLife() != 0 || dComIfGs_checkBottle(fpcNm_ITEM_FAIRY)) {
                    if (!checkGoronSideMove() && (!checkNotAutoJumpStage() || checkWolf())) {
                        if (field_0x3178 != 3 && !checkEndResetFlg0(ERFLG0_NOT_AUTO_JUMP)) {
                            if (sp14 >= sp10
                                && (!checkBootsOrArmorHeavy() || checkIronBallWaitAnime())
                                && (mStickValue > 0.85f
                                    || (checkIronBallWaitAnime() && mMoveValue > 0.85f)
                                    || mProcID == PROC_WOLF_GIANT_PUZZLE
                                    || checkEndResetFlg0(ERFLG0_FORCE_AUTO_JUMP)
                                    )
                                && cLib_distanceAngleS(temp_r26, shape_angle.y) < 0x800)
                            {
                                if (var_r29) {
                                    current.pos.x = mLinkLinChk.GetCross().x;
                                    current.pos.z = mLinkLinChk.GetCross().z;
                                }

                                if (checkWolf()) {
                                    return procWolfAutoJumpInit(0);
                                }

                                if (!checkGrabAnime() && field_0x3178 == 7) {
                                    return procDiveJumpInit();
                                }

                                return procAutoJumpInit(0);
                            }
                        }

                        if (!checkEndResetFlg0(ERFLG0_NOT_HANG)
                            && mMode != 0x2B
                            && var_r29
                            && dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 2
                            && sp28 < -mpHIO->mWallHang.m.hang_foot_pos_height
                            && current.pos.y - mWaterY > sp18
                            && (fabsf(sp44.mNormal.y) <= 0.05f
                                || (sp44.mNormal.y < 0.05f && var_r27 && getWallEdgePos(mLinkLinChk.GetCross(), &sp44, &sp58, mLinkLinChk.GetCrossP(), 0))
                                )
                            )
                        {
                            current.pos.x = mLinkLinChk.GetCross().x;
                            current.pos.z = mLinkLinChk.GetCross().z;

                            if (!checkWolf() && checkClimbCode(mLinkLinChk)) {
                                mPolyInfo1.SetPolyInfo(mLinkLinChk);
                                return procClimbDownStartInit(sp44.mNormal.atan2sX_Z());
                            }

                            if ((checkWolf() && procWolfHangFallStartInit(&sp44)) || (!checkWolf() && procHangFallStartInit(&sp44))) {
                                return 1;
                            }
                        }
                    }
                }

                if (mProcID == PROC_FRONT_ROLL) {
                    sp24 = 0.0f;
                } else {
                    sp24 = mpHIO->mAutoJump.m.mFallInterpolation;
                }

                f32 spC, sp8;
                if (var_r29) {
                    spC = sp44.mNormal.x;
                    sp8 = sp44.mNormal.z;
                } else {
                    spC = sp20;
                    sp8 = sp1C;
                }

                current.pos.x += field_0x3428 * spC;
                current.pos.z += field_0x3428 * sp8;

                if (checkWolf()) {
                    int var_r25 = cLib_distanceAngleS(cM_atan2s(spC, sp8), shape_angle.y);
                    if (var_r25 >= 0x4000) {
                        current.pos.x -= 75.0f * cM_ssin(shape_angle.y);
                        current.pos.z -= 75.0f * cM_scos(shape_angle.y);
                    }
                }
            } else {
                sp24 = mpHIO->mAutoJump.m.mFallInterpolation;
            }

            if (checkWolf()) {
                return procWolfFallInit(1, mpHIO->mWolf.mWlAutoJump.m.mNormalFallInterp);
            } else {
                return procFallInit(1, sp24);
            }
        }
    } else if (checkGrabGlide() && mWindSpeed.y > 10.0f) {
        mLinkAcch.ClrGroundHit();
        return procAutoJumpInit(1);
    } else if (checkEndResetFlg0(ERFLG0_UNK_100) && mLinkAcch.ChkGroundHit() && !checkWolf()) {
        return procRollJumpInit();
    } else if (checkMagneBootsOn() && !checkNoResetFlg0(FLG0_UNK_2000) && cancelMagneBootsOn()) {
        return procFallInit(1, 5.0f);
    }

    return 0;
}

BOOL daAlink_c::checkCutJumpInFly() {
    if ((mEquipItem == 0x103 && checkResetFlg0(RFLG0_UNK_80)) || checkForceSwordSwing()) {
        return procCutJumpInit(TRUE);
    }
    return false;
}

BOOL daAlink_c::checkFrontWallTypeAction() {
    setFrontWallType();

    s16 var_r27 = field_0x3078;
    field_0x3078 = 0;

    if (!checkInputOnR() && !checkModeFlg(2)) {
        return 0;
    }

    if (checkModeFlg(2)) {
        if (field_0x2f91 == 10) {
            dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));

            if (checkWolf()) {
                return procWolfHangWallCatchInit(8);
            } else {
                return procHangStartInit();
            }
        } else if (field_0x2f91 == 11) {
            dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));
            return procHangWallCatchInit();
        }

        if (!checkWolf()) {
            if (field_0x2f91 == 3) {
                shape_angle.y = field_0x306e + 0x8000;
                dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));
                return setClimbStartNotGround();
            } else if (field_0x2f91 == 4) {
                int var_r28 = getLadderUnitCount() - 1;
                if (var_r28 < 1) {
                    return 0;
                }

                setLadderPos(var_r28);
                deleteEquipItem(TRUE, FALSE);
                dComIfGp_getVibration().StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));

                field_0x33b0 = l_ladderAnmBaseTransY;

                if (current.pos.y > field_0x33c0 - 150.0f) {
                    current.pos.y = field_0x33c0 - 150.0f;
                    procLadderUpEndInit(1);
                } else {
                    procLadderMoveInit(1, 0, &current.pos);
                    mUnderFrameCtrl[0].setRate(0.0f);
                }

                field_0x2060->initOldFrameMorf(5.0f, 0, 0x23);
                return 1;
            }
        }
    } else if (!checkResetFlg0(RFLG0_UNK_8)) {
        if (!checkWolf()) {
            if (checkNoResetFlg3(FLG3_UNK_400000) && abs((s16)(mMoveAngle - field_0x306e)) < 0x7800) {
                return 0;
            }

            if (field_0x2f91 == 3) {
                return setClimbInit();
            }

            if (field_0x2f91 == 4) {
                return setLadderInit();
            }

            if (field_0x2f91 == 5) {
                daTag_Lantern_c* tag = NULL;
                if (fopAcM_GetRoomNo(this) == 4 && checkStageName("R_SP01")) {
                    tag = (daTag_Lantern_c*)fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchTagKandelaar, NULL);
                }

                if (tag == NULL || tag->chkClimbDownLadderStatus()) {
                    return procLadderDownStartInit();
                }

                onNoResetFlg3(FLG3_UNK_400000);
                return procWaitInit();
            }
        } else if (!checkModeFlg(2) && getDirectionFromShapeAngle() != DIR_FORWARD) {
            return 0;
        }

        field_0x3078 = var_r27 + 1;

        if (field_0x2f91 == 6) {
            if (field_0x3078 > mpHIO->mWallHang.m.small_jump_input_time) {
                if (checkWolf()) {
                    return procWolfStepMoveInit();
                } else {
                    return procStepMoveInit();
                }
            }
        } else if (field_0x2f91 == 7 || field_0x2f91 == 8 || field_0x2f91 == 9) {
            if (field_0x3078 > mpHIO->mWallHang.m.grab_input_time) {
                if (checkWolf()) {
                    if (field_0x2f91 == 7 || field_0x2f91 == 8) {
                        if (checkModeFlg(0x40000)) {
                            return procWolfHangWallCatchInit(7);
                        } else {
                            return procWolfHangReadyInit();
                        }
                    }
                } else {
                    if (field_0x2f91 == 7) {
                        return procHangWallCatchInit();
                    } else {
                        return procHangReadyInit();
                    }
                }
            }
        } else {
            field_0x3078 = 0;
        }
    }

    return 0;
}

int daAlink_c::checkItemActionInitStart() {
    if (checkBowAndSlingItem(mEquipItem)) {
        return checkNextActionBow();
    } else if (mEquipItem == fpcNm_ITEM_BOOMERANG) {
        return checkNextActionBoomerang();
    } else if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        if (mCopyRodAcKeep.getActor() == NULL) {
            return checkNextActionCopyRod();
        }

        if (checkCopyRodControllAnime() || mProcID == PROC_COPY_ROD_SWING) {
            return procCopyRodSwingInit();
        }
    } else if (checkFishingRodItem(mEquipItem)) {
        onResetFlg1(RFLG0_FISHINGROD_USE_START);
        return 1;
    } else if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        if (!checkIronBallAnime()) {
            return checkNextActionIronBall();
        }
    } else if (mEquipItem == fpcNm_ITEM_EMPTY_BOTTLE) {
        return procBottleSwingInit(NULL, 0);
    } else if (mEquipItem == fpcNm_ITEM_KANTERA) {
        if (!checkModeFlg(0x400)) {
            return procKandelaarSwingInit();
        }
    } else if (checkHookshotItem(mEquipItem) && !checkHookshotAnime()) {
        return checkNextActionHookshot();
    }

    return -1;
}

int daAlink_c::checkItemChangeAutoAction() {
    if (checkResetFlg0(RFLG0_UNK_80)) {
        if (mEquipItem == 0x103) {
            if (checkModeFlg(0x40000)) {
                return 0;
            }

            resetUpperAnime(UPPER_2, -1.0f);

            if (checkBoardRide()) {
                return checkBoardSwordTriggerAction();
            } else if (checkReinRide()) {
                return procHorseCutChargeReadyInit();
            }

            return procCutTurnChargeInit();
        }

        int ivar2 = checkItemActionInitStart();
        if (ivar2 != -1) {
            return ivar2;
        }
    }

    return 0;
}

void daAlink_c::setFastShotTimer() {
    if (!dComIfGp_checkPlayerStatus0(0, 0x2000)) {
        mFastShotTime = mpHIO->mItem.m.mItemFPTransitionTimer;
    }
}

void daAlink_c::cancelItemUseQuake(int param_0) {
    if (checkNoResetFlg3(FLG3_UNK_4) && (param_0 != 0 || mProcID != PROC_HOOKSHOT_FLY)) {
        offNoResetFlg3(FLG3_UNK_4);
        dComIfGp_getVibration().StopQuake(31);
    }
}

int daAlink_c::cancelUpperItemReadyAnime(BOOL param_0) {
    if ((dComIfGp_getDoStatus() == BUTTON_STATUS_BACK || param_0)
        && ((param_0 == 0 && doTrigger())
            || (checkCanoeSlider() && (subjectCancelTrigger() || checkEndResetFlg1(ERFLG1_CANOE_ITEM_CANCEL)))
            )
        )
    {
        seStartSystem(Z2SE_SY_ITEM_USE_CANCEL);

        if (mEquipItem == fpcNm_ITEM_IRONBALL) {
            mItemVar0.field_0x3018 = 8;
            procIronBallReturnInit();
        } else {
            resetUpperAnime(UPPER_2, 3.0f);
            checkWaitAction();
        }

        return 1;
    }

    return 0;
}

BOOL daAlink_c::checkItemActorPointer() {
    if (mItemAcKeep.getActor() != NULL) {
        return true;
    }

    mItemAcKeep.clearData();
    resetUpperAnime(UPPER_2, -1.0f);
    mEquipItem = fpcNm_ITEM_NONE;

    if (checkModeFlg(MODE_RIDING)) {
        if (checkCanoeRide()) {
            procCanoeWaitInit(0);
        } else {
            procHorseWaitInit();
        }
    } else {
        procWaitInit();
    }

    return false;
}

bool daAlink_c::checkSwordTwirlAction() {
    daPy_frameCtrl_c* frame_ctrl = &mUpperFrameCtrl[2];

    if (checkSwordTwirlAnime()) {
        if (checkAnmEnd(frame_ctrl)) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else if (frame_ctrl->getFrame() >= 7.0f || frame_ctrl->getFrame() < 11.0f) {
            field_0x2f96 = 0xFE;
        } else {
            field_0x2f96 = 0xFF;
        }
    } else {
        if (checkEnemyGroup(mTargetedActor) && mEquipItem == 0x103 && field_0x2f98 != 1 && checkNoUpperAnime() && cM_rnd() < 0.012f) {
            u16 anm;
            if (field_0x2f98 == 2) {
                anm = 0x3F;
            } else {
                anm = 0x40;
            }

            setUpperAnimeBaseSpeed(anm, mpHIO->mAtnMove.m.mWaitAnmSpeed, 3.0f);
            seStartSwordCut(Z2SE_AL_WAIT_SWORD_SWING);
        }
    }

    return true;
}

BOOL daAlink_c::checkUpperItemActionFly() {
    if (checkBowAndSlingItem(mEquipItem)) {
        checkUpperItemActionBowFly();
    } else if (mEquipItem == fpcNm_ITEM_BOOMERANG) {
        checkUpperItemActionBoomerangFly();
    } else if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        checkUpperItemActionCopyRodFly();
    }

    return checkCanoeJumpRide();
}

void daAlink_c::checkItemButtonChange() {
    if (mProcID != PROC_CANOE_PADDLE_PUT && mEquipItem != fpcNm_ITEM_NONE && !checkEquipAnime()) {
        u8 temp_r0;
        for (u8 i = 0; i < 2; i++) {
            temp_r0 = (i + 1) % 2;
            if (mEquipItem == dComIfGp_getSelectItem(i) &&
                (mEquipItem != dComIfGp_getSelectItem(temp_r0) || mSelectItemId != temp_r0))
            {
                mSelectItemId = i;
            }
        }
    }
}

BOOL daAlink_c::checkUpperItemAction() {
    if (checkNoUpperAnime()) {
        if (checkModeFlg(0x1000)) {
            if (mEquipItem == fpcNm_ITEM_IRONBALL) {
                setIronBallWaitUpperAnime(0);
            } else if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
                setCopyRodControllAnime();
            }
        }

        return false;
    }

    if (checkFmChainGrabAnime()) {
        if (field_0x2fa3 == 0) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else {
            if (checkEndResetFlg1(ERFLG1_CHAIN_FORCE_PULL) && !checkWolf()) {
                return procFmChainStrongPullInit();
            }

            if (!wallGrabButton()) {
                resetUpperAnime(UPPER_2, 3.0f);

                if (checkWolf()) {
                    return procWolfWaitInit();
                } else {
                    return procWaitInit();
                }
            }

            setChainGrabStatus(BUTTON_STATUS_UNK_150);

            if ((!checkWolf() && (checkUnderMove0BckNoArc(ANM_CHAIN_PULL) || checkUnderMove0BckNoArc(ANM_CHAIN_PULL_END)))
                || (checkWolf() && (checkUnderMove0BckNoArcWolf(WANM_CHAIN_PULL) || checkUnderMove0BckNoArcWolf(WANM_CHAIN_PULL_END))))
            {
                mNowAnmPackUpper[2].setRatio(0.0f);
            } else {
                mNowAnmPackUpper[2].setRatio(1.0f);
            }
        }
    } else if (checkWolf()) {
        if (checkWolfGrabAnime()) {
            if (!notTalk() && orderZTalk()) {
                return true;
            }

            if (mGrabItemAcKeep.getActor() == NULL) {
                return false;
            }

            u8 btn_status;
            if (mStickValue > getGrabThrowRate() || checkAttentionLock()) {
                btn_status = BUTTON_STATUS_THROW;
            } else {
                btn_status = BUTTON_STATUS_PLACE;
            }

            setBStatus(btn_status);
            setDoStatus(btn_status);

            if ((btn_status == dComIfGp_getDoStatus() && doTrigger()) || (btn_status == getBStatus() && swordTrigger())) {
                if (btn_status == BUTTON_STATUS_THROW) {
                    return procWolfGrabThrowInit();
                }
                return procWolfGrabPutInit();
            }
        }
    } else if (checkDkCaught2Anime()) {
        if (checkCaughtEscapeCutTurn()) {
            return true;
        }

        field_0x30f6--;

        if (swordTrigger() || doTrigger()) {
            field_0x30f6 -= mpHIO->mDamage.mDamCaught.m.mInputFadeTime;
        }

        if (checkInputOnR() && abs((s16)(mStickAngle - mPrevStickAngle)) > 0x1000) {
            field_0x30f6 -= mpHIO->mDamage.mDamCaught.m.mInputFadeTime;
        }

        if (field_0x30f6 < 0 || !checkNoResetFlg0(FLG0_DK_CAUGHT2)) {
            voiceStart(Z2SE_AL_V_ZENTEN_FAIL_2);

            if (mLinkAcch.ChkGroundHit()) {
                return procStEscapeInit();
            }
            resetUpperAnime(UPPER_2, 3.0f);
        } else {
            voiceStartLevel(Z2SE_AL_V_BITTEN_LOOP);
        }
    } else if (checkBowAnime()) {
        if (checkUpperItemActionBow()) {
            return true;
        }
    } else if (checkGrabAnime()) {
        if (checkNextActionGrab()) {
            return true;
        }
    } else if (checkBoomerangReadyAnime()) {
        if (checkUpperItemActionBoomerang()) {
            return true;
        }
    } else if (checkCopyRodReadyAnime()) {
        if (checkUpperItemActionCopyRod()) {
            return true;
        }
    } else if (checkHookshotAnime()) {
        if (checkUpperItemActionHookshot()) {
            return true;
        }
    } else if (checkIronBallAnime()) {
        if (checkUpperItemActionIronBall()) {
            return true;
        }
    } else if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        setIronBallWaitUpperAnime(0);
    } else if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        setCopyRodControllAnime();
    }

    return false;
}

void daAlink_c::orderPeep() {
    mPeepExitID = ((kytag05_class*)field_0x27f4)->getSceneListID();
    field_0x2ff2 = AREG_S(4) + 20;

    fopAcM_orderOtherEvent(this, field_0x27f4, l_peepEventName, 0xFFFF, 1, 0);

    if (checkWolf()) {
        procWolfWaitInit();
    } else {
        procWaitInit();
    }
}

int daAlink_c::orderTalk(int i_checkZTalk) {
    static const u16 itemTalkType[2] = {6, 7};

    if (notTalk()) {
        return 0;
    }

    if ((dComIfGp_getDoStatus() == BUTTON_STATUS_SPEAK
        || dComIfGp_getDoStatus() == BUTTON_STATUS_LOOK
        || dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_128
        || dComIfGp_getDoStatus() == BUTTON_STATUS_LISTEN
        || dComIfGp_getDoStatus() == BUTTON_STATUS_DRINK
        || dComIfGp_getDoStatus() == BUTTON_STATUS_CHECK
        )
        && talkTrigger())
    {
        fopAcM_orderTalkEvent(this, field_0x27f4, 0, 0);
        return 1;
    }

    if (!checkWolf() && checkRequestTalkActor(mAttList2, field_0x27f8)) {
        for (int i = 0; i < 2; i++) {
            // check if pressed X or Y and if item on button is a trade item
            if (checkTradeItem(dComIfGp_getSelectItem(i)) && itemTriggerCheck(1 << i)) {
                fopAcM_orderTalkItemBtnEvent(itemTalkType[i], this, field_0x27f8, 0, 0);
                return 1;
            }
        }
    }

    if (i_checkZTalk) {
        return orderZTalk();
    }

    return 0;
}

static void* daAlink_searchBouDoor(fopAc_ac_c* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_GetName(i_actor) == PROC_NPC_BOU && ((daNpc_Bou_c*)i_actor)->speakTo()) {
        return i_actor;
    }

    return NULL;
}

static void* daAlink_searchKolin(fopAc_ac_c* i_actor, void* i_data) {
    UNUSED(i_data);

    if (fopAcM_GetName(i_actor) == PROC_NPC_KOLIN && ((daNpc_Kolin_c*)i_actor)->orderNoRideEvt()) {
        return i_actor;
    }

    return NULL;
}

int daAlink_c::orderZTalk() {
    if ((!checkReinRide() && !checkModeFlg(0x40000) && !checkMagneBootsOn() && (!mLinkAcch.ChkGroundHit() || checkModeFlg(0x70C52)))
        || mThrowBoomerangAcKeep.getActor() != NULL
        || dComIfGp_checkPlayerStatus0(0, 0x8000000)
        || mProcID == PROC_CRAWL_END
        || checkHorseZelda()
        || checkCloudSea()
        || checkStageName("D_MN08A")
        || (checkStageName("D_MN09A") && ((fopAcM_GetRoomNo(this) == 50 && dComIfG_play_c::getLayerNo(0) == 0) || fopAcM_GetRoomNo(this) == 51))
        || (checkMagneBootsOn() && (!checkStageName("D_MN04B") || !cBgW_CheckBGround(mMagneBootsTopVec.y)))
        || dComIfGs_isEventBit(dSv_event_flag_c::F_0800))
    {
        return 0;
    }

    if (checkMidnaRide()) {
        fopAc_ac_c* zhint = dComIfGp_att_getZHint();
        if (zhint != NULL) {
            setMidnaTalkStatus(BUTTON_STATUS_CHECK);
        }

        dMeter2Info_onUseButton(METER2_USEBUTTON_Z);

        if (midnaTalkTrigger()
#if DEBUG
            && (!mDoCPd_c::getHoldL(PAD_1) || !mDoCPd_c::getHoldR(PAD_1))
#endif
           )
        {
            if (zhint != NULL) {
                if (fopAcM_GetName(zhint) == PROC_Tag_Mhint && ((daTagMhint_c*)zhint)->checkEventID()) {
                    fopAcM_orderOtherEventId(zhint, ((daTagMhint_c*)zhint)->getEventID(), ((daTagMhint_c*)zhint)->getToolEventID(), 0xFFFF, 0, 1);
                } else {
                    fopAcM_orderTalkEvent(this, zhint, 0, 0);
                }
            } else {
                if (mMidnaMsg != NULL) {
                    dComIfGp_setMesgCameraInfoActor(mMidnaMsg, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                }

                fopAcM_orderTalkEvent(this, getMidnaActor(), 0, 0);
            }

            field_0x35a0 = field_0x3594;
            return 1;
        }
    }

    return 0;
}

int daAlink_c::checkNormalAction() {
    int wall_grab_status = getWallGrabStatus();

    if (wallGrabTrigger()) {
        if (wall_grab_status == BUTTON_STATUS_GRAB) {
            return procCoPushPullWaitInit(1);
        }

        if (wall_grab_status == BUTTON_STATUS_UNK_150 && searchFmChainPos()) {
            if (checkWolf()) {
                return procWolfChainUpInit();
            } else {
                return procFmChainUpInit();
            }
        }
    }

    if (orderTalk(TRUE)) {
        return 1;
    }

    if (doTrigger()) {
        if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_137) {
            orderPeep();
            return 1;
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_ENTER) {
            if (checkWolf()) {
                return procWolfLieStartInit(1);
            } else {
                return procCrawlStartInit();
            }
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_PICK_UP) {
            return procWolfGrabUpInit();
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_123) {
            return procWolfChainReadyInit();
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_ROLL) {
            return procWolfPushInit();
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_GET_ON) {
            if (fopAcM_GetName(field_0x27f4) == PROC_Obj_IceLeaf) {
                return procBoardRideInit();
            }

            if (fopAcM_GetName(field_0x27f4) == PROC_CANOE) {
                if (checkModeFlg(0x40000)) {
                    return procCanoeRideInit();
                } else {
                    return procSmallJumpInit(1);
                }
            }

            if (checkStageName("F_SP103") &&
                fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchKolin, NULL))
            {
                return procWaitInit();
            } else {
                return procHorseRideInit();
            }
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_32) {
            if (checkWolf()) {
                return procWolfHangReadyInit();
            } else {
                if (field_0x2f91 == 7) {
                    return procHangWallCatchInit();
                } else {
                    return procHangReadyInit();
                }
            }
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_OPEN) {
            if (mAttList->mType == fopAc_attn_DOOR_e) {
                if (!checkStageName("F_SP103") ||
                    !fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchBouDoor, NULL))
                {
                    fopAcM_orderDoorEvent(this, field_0x27f4, 0, 0);
                }
                checkWaitAction();
            } else {
                fopAcM_orderTreasureEvent(this, field_0x27f4, 0, 0);
            }

            return 1;
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_PICK) {
            return procGrassWhistleGetInit();
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_PET) {
            return procGoatStrokeInit();
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_STRIKE) {
            if (fopAcM_GetName(field_0x27f4) == PROC_Tag_Lv6Gate) {
                static_cast<daTagLv6Gate_c*>(field_0x27f4)->stabMasterSword();
            } else {
                static_cast<daTag_KMsg_c*>(field_0x27f4)->stabMasterSword();
            }

            procWaitInit();
            return 1;
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_145) {
            onNoResetFlg0(FLG0_UNK_10000000);

            if (field_0x27f4->current.pos.abs2XZ(current.pos) < getGoatCatchDistance2()) {
                if (checkWolf()) {
                    return procWolfGanonCatchInit();
                } else {
                    return procGoatCatchInit(field_0x27f4, 0.0f);
                }
            }
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_152) {
            return procInsectCatchInit();
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_31 || dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_57 ||
                   dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_52)
        {
            if (checkWolf()) {
                return procWolfGrabUpInit();
            } else {
                if (fopAcM_CheckCarryType(field_0x27f4, fopAcM_CARRY_ITEM)) {
                    return procPickUpInit();
                } else {
                    return procGrabReadyInit();
                }
            }
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_153) {
            return procHangLeverDownInit();
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_FINISH) {
            if (checkWolf()) {
                return procWolfDownAttackInit();
            } else {
                return checkDoCutAction();
            }
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_HELM_SPLITTER) {
            if (checkWolf()) {
                return procWolfJumpAttackInit(1);
            } else {
                return checkDoCutAction();
            }
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_139) {
            return procWolfJumpAttackInit(1);
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_DRAW) {
            changeCutFast();
            return 1;
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_134) {
            return checkDoCutAction();
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_HOWL) {
            return procWolfHowlDemoInit();
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_SNIFF) {
            return procWolfGetSmellInit();
        } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_147) {
            return procWolfTagJumpInit(field_0x27f4);
        }
    } else if (checkNoResetFlg0(FLG0_UNK_10000000) && dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_145) {
        if (field_0x27f4->current.pos.abs2XZ(current.pos) < getGoatCatchDistance2()) {
            if (checkWolf()) {
                return procWolfGanonCatchInit();
            } else {
                return procGoatCatchInit(field_0x27f4, 0.0f);
            }
        }
    }

    return 0;
}

bool daAlink_c::checkReadyItem() {
    #if PLATFORM_GCN
    if (mEquipItem == fpcNm_ITEM_NONE) {
        return false;
    }
    #endif

    return getReadyItem() == mEquipItem;
}

BOOL daAlink_c::checkItemAction() {
    if (checkEquipAnime() || checkKandelaarSwingAnime() || checkCopyRodThrowAnime() || checkBoomerangThrowAnime()) {
        return false;
    }

    if (checkWolf()) {
        if (checkResetFlg0(RFLG0_UNK_40)) {
            return procWolfRollAttackChargeInit();
        }

        if (checkCutTurnInputTrigger()) {
            return procWolfRollAttackInit(2, 0);
        }

        if (swordSwingTrigger() || checkComboReserb()) {
            if (mComboCutCount == 0) {
                return procWolfRollAttackChargeInit();
            }

            return checkWolfAttackAction();
        }

        if ((checkEndResetFlg1(ERFLG1_UNK_100000) || mTargetedActor == NULL) && wolfClawTrigger()) {
            if (checkEndResetFlg1(ERFLG1_UNK_100000) && field_0x27f4 != NULL && fopAcM_GetName(field_0x27f4) == PROC_Obj_Digholl) {
                return procWolfDigThroughInit(0);
            }

            return procWolfDigInit();
        }
    } else {
        if (mEquipItem == 0x103) {
            daPy_frameCtrl_c* frame_ctrl = &mUpperFrameCtrl[2];

            if ((checkUpperAnime(dRes_ID_ALANM_BCK_CUTDL_e) && frame_ctrl->getFrame() <= mpHIO->mCut.mCutDashLeft.m.mCutAnm.mCancelFrame) ||
                (checkUpperAnime(dRes_ID_ALANM_BCK_CUTDR_e) && frame_ctrl->getFrame() <= mpHIO->mCut.mCutDashRight.m.mCutAnm.mCancelFrame) ||
                (checkUpperAnime(dRes_ID_ALANM_BCK_CUTDT_e) && frame_ctrl->getFrame() <= mpHIO->mCut.mCutDashCharge.m.mCutAnm.mCancelFrame) ||
                checkModeFlg(0x40000))
            {
                return false;
            }

            if (checkNoResetFlg2(FLG2_UNK_4) && !checkCutDashAnime()) {
                offNoResetFlg2(FLG2_UNK_4);
            }

            if (checkResetFlg0(RFLG0_UNK_40)) {
                offNoResetFlg2(FLG2_UNK_4);

                if (checkBoardRide()) {
                    return checkBoardSwordTriggerAction();
                }

                if (checkReinRide()) {
                    return procHorseCutChargeReadyInit();
                }

                return procCutTurnChargeInit();
            }

            if (checkNoResetFlg2(FLG2_UNK_4)) {
                offNoResetFlg2(FLG2_UNK_4);
                setUpperAnimeBase(dRes_ID_ALANM_BCK_CUTDTP_e);
                return true;
            }

            if (checkCutTurnInputTrigger()) {
                if (checkBoardRide()) {
                    return procBoardCutTurnInit();
                }

                if (checkReinRide()) {
                    return procHorseCutTurnInit();
                }

                return procCutTurnInit(1, 2);
            }

            if (swordSwingTrigger() || checkComboReserb()) {
                if (checkNoResetFlg2(FLG2_COMBO_RESERB)) {
                    clearComboReserb();

                    if (checkBoardRide()) {
                        return procBoardCutInit();
                    }

                    if (checkDashAnime()) {
                        setCutDash(0, 0);
                        return true;
                    }

                    return checkCutAction();
                }

                if (mComboCutCount == 0) {
                    if (checkBoardRide()) {
                        return checkBoardSwordTriggerAction();
                    }

                    if (checkReinRide()) {
                        return procHorseCutChargeReadyInit();
                    }

                    return procCutTurnChargeInit();
                }

                return checkCutAction();
            }
        } else if (checkFishingRodItem(mEquipItem) && mLinkAcch.ChkGroundHit()) {
            if (checkNoResetFlg2(FLG2_FISHING_CAST_WAIT)) {
                return procFishingCastInit();
            }
        } else if (mEquipItem == 0x102) {
            if (doTrigger()) {
                if (dComIfGp_getDoStatus() == BUTTON_STATUS_THROW) {
                    setThrowBoomerangAnime();
                    return true;
                }

                if (dComIfGp_getDoStatus() == BUTTON_STATUS_PLACE) {
                    return procPickPutInit(0);
                }
            }
        } else if (doTrigger() && dComIfGp_getDoStatus() == BUTTON_STATUS_QUIT && mCopyRodAcKeep.getActor() != NULL) {
            ((daCrod_c*)mCopyRodAcKeep.getActor())->offControll();
            resetUpperAnime(UPPER_2, 3.0f);
            return true;
        }

        if (!checkFishingRodItem(mEquipItem) || !(mWaterY - current.pos.y > 70.0f)) {
            onResetFlg1(RFLG0_FISHINGROD_USE_ACCEPT);

            if (checkReadyItem() && (itemTrigger() || (checkBoomerangCatchAnime() && itemButton()))) {
                int var_r28 = checkItemActionInitStart();
                if (var_r28 != -1) {
                    return var_r28;
                }
            }
        }

        if (((dComIfGs_isEventBit(dSv_event_flag_c::F_0338) || checkNoResetFlg3(FLG3_TRANING_SHIELD_ATTACK))
                && checkGuardActionChange()
                && !checkUpperReadyThrowAnime()
                && !checkModeFlg(0x70C52)
                && checkShieldGet()
                && !checkNotBattleStage()
            ) && ((mLinkAcch.ChkGroundHit() || checkMagneBootsOn()) && dComIfGp_getRStatus() == 0)
            )
        {
            setRStatus(BUTTON_STATUS_SHIELD_ATTACK);

            if (spActionTrigger()) {
                return procGuardAttackInit();
            }
        }
    }

    return false;
}

BOOL daAlink_c::checkRAction() {
    return false;
}

BOOL daAlink_c::checkMoveDoAction() {
    if (doTrigger()) {
        if (dComIfGp_getDoStatus() == BUTTON_STATUS_JUMP) {
            if (checkWolf()) {
                return procWolfSideStepInit(0);
            }

            int direction = getDirectionFromShapeAngle();
            if (field_0x2fb0 != 0 && direction != DIR_BACKWARD && checkSideRollAction(direction)) {
                return true;
            }

            return procSideStepInit(direction);
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_121) {
            if (!checkAttentionLock() && checkInputOnR()) {
                shape_angle.y = mMoveAngle;
            }

            return procFrontRollInit();
        }

        if (dComIfGp_getDoStatus() == BUTTON_STATUS_DASH) {
            return procWolfDashInit();
        }
    }

    return false;
}

BOOL daAlink_c::checkSideRollAction(int param_0) {
    if (checkNotJumpSinkLimit()
        || checkHeavyStateOn(TRUE, TRUE)
        || !checkInputOnR()
        || !checkCutBackState())
    {
        return false;
    }

    if (mTargetedActor == NULL) {
        return procSideRollInit(param_0);
    }

    if (checkEnemyGroup(mTargetedActor) && mTargetedActor->current.pos.abs2XZ(current.pos) < SQUARE(500.0f)) {
        return procTurnMoveInit(param_0);
    }

    return procSideRollInit(param_0);
}

BOOL daAlink_c::checkNoUpperAnime() const {
    return checkNoSetUpperAnime()
            || checkSwimMoveHandAnime()
            || checkWindDashAnime()
            || checkSwordTwirlAnime()
            || checkKandelaarEquipAnime();
}

BOOL daAlink_c::checkOneHandItemEquipAnime() const {
    return checkUpperAnime(dRes_ID_ALANM_BCK_TAKEL_e) || checkUpperAnime(dRes_ID_ALANM_BCK_TAKER_e);
}

BOOL daAlink_c::checkItemEquipAnime() const {
    return checkTwoHandItemEquipAnime() || checkOneHandItemEquipAnime();
}

BOOL daAlink_c::checkEquipAnime() const {
    return checkSwordEquipAnime() || checkItemEquipAnime();
}

BOOL daAlink_c::checkWindDashAnime() const {
    return checkUpperAnime(dRes_ID_ALANM_BCK_WL_DASHWIND_e) || (checkUpperAnime(dRes_ID_ALANM_BCK_DASHWIND_e) && mProcID != PROC_SLIDE);
}

BOOL daAlink_c::checkSwordTwirlAnime() const {
    return checkUpperAnime(dRes_ID_ALANM_BCK_ATTWIRLR_e) || checkUpperAnime(dRes_ID_ALANM_BCK_ATTWIRLL_e);
}

void daAlink_c::swordEquip(BOOL param_0) {
    if (checkSwordGet()) {
        field_0x2fde = 0x103;

        if (checkReinRide()) {
            setUpperAnimeParam(dRes_ID_ALANM_BCK_WAITHATOHS_e, UPPER_2, &mpHIO->mCut.m.mEquipAnm);
            offNoResetFlg3(FLG3_UNK_8000);
        } else {
            u32 var_r29 = checkNoResetFlg0(FLG0_UNK_1000000);
            setUpperAnimeParam(dRes_ID_ALANM_BCK_WAITATOS_e, UPPER_2, &mpHIO->mCut.m.mEquipAnm);

            if (var_r29) {
                mUpperFrameCtrl[2].setFrame(12);
            }

            if (!param_0) {
                onNoResetFlg3(FLG3_UNK_8000);
            } else {
                offNoResetFlg3(FLG3_UNK_8000);
            }
        }

        setFacePriTexture(FTANM_WAITATOS);
        setFacePriBck(dRes_ID_ALANM_BCK_FWAITATOS_e);

        if (checkEventRun() && checkStageName("F_SP102") && fopAcM_GetRoomNo(this) == 0 && dComIfGp_getStartStageLayer() == 0) {
            mUpperFrameCtrl[2].setRate(-0.8f);
        }

        if (field_0x2f94 == 0xFF) {
            field_0x2f96 = 4;
        }

        if (field_0x2f95 == 0xFF) {
            field_0x2f97 = 10;
        }
    }
}

void daAlink_c::swordUnequip() {
    f32 anm_speed;
    if (checkFastUnequip()) {
        anm_speed = mpHIO->mCut.m.mFastUnequipAnmSpeed;
    } else {
        anm_speed = mpHIO->mCut.m.mUnequipAnm.mSpeed;
    }

    u16 anm_id = checkReinRide() ? (u16)dRes_ID_ALANM_BCK_WAITHATOHS_e : (u16)dRes_ID_ALANM_BCK_WAITATOS_e;

    setUpperAnime(anm_id, UPPER_2, anm_speed, mpHIO->mCut.m.mUnequipAnm.mStartFrame,
                  mpHIO->mCut.m.mUnequipAnm.mEndFrame,
                  mpHIO->mCut.m.mUnequipAnm.mInterpolation);
    setFacePriTexture(FTANM_WAITATOS);
    setFacePriBck(dRes_ID_ALANM_BCK_FWAITATOS_e);
}

void daAlink_c::itemEquip(u16 i_itemID) {
    if (mThrowBoomerangAcKeep.getActor() == NULL || i_itemID != fpcNm_ITEM_BOOMERANG) {
        field_0x2fde = i_itemID;
        dComIfGp_clearPlayerStatus0(0, 0x400000);
        offNoResetFlg3(FLG3_COPY_ROD_THROW_AFTER);
        itemUnequip(field_0x2fde, -1.0f);
    }
}

/**
 * This handles setting the animation for both equipping and unequipping items
 * When `i_playSpeed` is positive, it will play the unequip animation
 * When `i_playSpeed` is negative, it will play the equip animation
 *
 */
void daAlink_c::itemUnequip(u16 i_itemID, f32 i_playSpeed) {
    u16 anm_id;
    const daAlinkHIO_anm_c* anm_data;

    if (i_itemID == fpcNm_ITEM_BOOMERANG || i_itemID == fpcNm_ITEM_COPY_ROD || i_itemID == fpcNm_ITEM_KANTERA ||
        i_itemID == fpcNm_ITEM_HOOKSHOT || checkBottleItem(i_itemID))
    {
        anm_id = dRes_ID_ALANM_BCK_TAKEL_e;
        anm_data = &mpHIO->mItem.m.mOneHandEquipAnm;
    } else if (checkBowAndSlingItem(i_itemID)) {
        if (checkBowGrabLeftHand()) {
            anm_id = (u16)dRes_ID_ALANM_BCK_TAKEL_e;
        } else {
            anm_id = (u16)dRes_ID_ALANM_BCK_TAKER_e;
        }
        anm_data = &mpHIO->mItem.m.mOneHandEquipAnm;
    } else {
        anm_id = dRes_ID_ALANM_BCK_TAKE_e;
        anm_data = &mpHIO->mItem.m.mTwoHandEquipAnm;
    }

    f32 anm_speed;
    if (i_playSpeed > 0.0f && checkFastUnequip()) {
        if (anm_id == dRes_ID_ALANM_BCK_TAKE_e) {
            anm_speed = mpHIO->mItem.m.mTwoHandReleaseAnmSpeed;
        } else {
            anm_speed = mpHIO->mItem.m.mOneHandReleaseAnmSpeed;
        }
    } else {
        anm_speed = anm_data->mSpeed * i_playSpeed;
    }

    setUpperAnime(anm_id, UPPER_2, anm_speed, anm_data->mStartFrame, anm_data->mEndFrame,
                  anm_data->mInterpolation);
}

bool daAlink_c::checkFastUnequip() {
    return mProcID == PROC_PREACTION_UNEQUIP || mProcID == PROC_HORSE_GETOFF || checkEventRun();
}

void daAlink_c::allUnequip(BOOL param_0) {
    if (checkNoResetFlg2(FLG2_UNK_1) && param_0 && !checkCanoeRide() &&
        mEquipItem != fpcNm_ITEM_KANTERA)
    {
        for (u8 i = 0; i < 2; i++) {
            if (dComIfGp_getSelectItem(i) == fpcNm_ITEM_KANTERA) {
                mSelectItemId = i;
            }
        }
        itemEquip(fpcNm_ITEM_KANTERA);
        onNoResetFlg1(FLG1_UNK_40);
        return;
    } else if (mEquipItem == 0x103) {
        swordUnequip();
    } else if (mEquipItem == 0x102) {
        deleteEquipItem(FALSE, FALSE);
        field_0x2060->initOldFrameMorf(5.0f, 0, 35);
    } else {
        itemUnequip(mEquipItem, 1.0f);
    }

    field_0x2fde = 0xFF;
}

BOOL daAlink_c::checkItemChangeFromButton() {
    if (checkModeFlg(4)
        && !checkEquipAnime()
        && !checkBoomerangThrowAnime()
        && !checkCopyRodThrowAnime()
        && !checkKandelaarSwingAnime()
        && !checkKandelaarSwingAnime())
    {
        if (
            #if PLATFORM_GCN
            dComIfGs_getSelectEquipSword() != fpcNm_ITEM_NONE &&
            #endif
            !checkNotBattleStage()
            && !checkCanoeRide()
            && (!checkModeFlg(0x40000) || checkEquipHeavyBoots())
            && mEquipItem != 0x103
            && swordTrigger())
        {
            if (checkEndResetFlg1(ERFLG1_SWORD_TRIGGER_NON)) {
                return 0;
            }

            swordEquip(TRUE);
        } else if (checkCanoeRide()
                    && !checkStageName("F_SP103")
                    && !checkCanoeSlider()
                    && !checkFisingRodLure()
                    && swordTrigger())
        {
            itemEquip(0x105);
        } else {
            u8 i;
            for (i = 0; i < 2; i++) {
                int proc_type = checkNewItemChange(i);
                if (proc_type != 0 && itemTriggerCheck(1 << i)) {
                    BOOL var_r27 = changeItemTriggerKeepProc(i, proc_type);
                    return var_r27;
                }
            }

            if (doTrigger() && dComIfGp_getDoStatus() == BUTTON_STATUS_PUT_AWAY) {
                if (mEquipItem != fpcNm_ITEM_KANTERA && checkNoResetFlg2(FLG2_UNK_1)) {
                    offKandelaarModel();
                } else if (mSwordFlourishTimer != 0 && mEquipItem == 0x103 &&
                           !checkWoodSwordEquip() && !checkModeFlg(0x402))
                {
                    return procSwordUnequipSpInit();
                } else {
                    allUnequip(TRUE);
                }
            } else if (mEquipItem == fpcNm_ITEM_NONE && mThrowBoomerangAcKeep.getActor() == NULL &&
                       !checkCanoeRide() && checkNoUpperAnime() && checkNoResetFlg2(FLG2_UNK_1))
            {
                for (i = 0; i < 2; i++) {
                    if (dComIfGp_getSelectItem(i) == fpcNm_ITEM_KANTERA) {
                        mSelectItemId = i;
                    }
                }

                itemEquip(fpcNm_ITEM_KANTERA);
                onNoResetFlg1(FLG1_UNK_40);
            } else if (mEquipItem != 0x103 && mEquipItem != fpcNm_ITEM_NONE && mEquipItem != 0x10B &&
                       mEquipItem != 0x102 && (!checkCanoeRide() || !checkFisingRodLure()))
            {
                if (!checkEventRun() || strcmp(dComIfGp_getEventManager().getRunEventName(), "ANGER") != 0) {
                    if (strcmp(dComIfGp_getEventManager().getRunEventName(), "ANGER2") != 0 && checkItemSetButton(mEquipItem) == 2) {
                        allUnequip(1);
                    }
                }
            }
        }
    }

    return 0;
}

BOOL daAlink_c::checkNextActionFromButton() {
    if (checkItemChangeAutoAction()) {
        return 1;
    }

    if (checkUpperItemAction()) {
        return 1;
    }

    if (checkDkCaught2Anime()) {
        return 0;
    }

    if (checkModeFlg(0x400)) {
        if (dComIfGp_getDoStatus() == BUTTON_STATUS_NONE && checkCanoeRide() && checkFisingRodLure()) {
            setDoStatus(BUTTON_STATUS_PUT_AWAY);
        }
    } else if (!checkModeFlg(0x40000)) {
        if ((checkWolf() && !checkWolfGrabAnime())
            || (!checkWolf()
                && getCopyRodControllActor() == NULL
                && (checkEquipAnime() || checkNoResetFlg0(FLG0_UNK_1000000) || checkUpperGuardAnime() || checkIronBallWaitAnime() || checkNoUpperAnime())
                )
            )
        {
            decideDoStatus();

            if (checkNormalAction()) {
                return 1;
            }
        } else {
            decideCommonDoStatus();
        }
    }

    if (cancelUpperItemReadyAnime(0)) {
        return 1;
    }

    if (checkItemAction()) {
        return 1;
    }

    if (!checkModeFlg(0x40400)) {
        if (checkRAction()) {
            return 1;
        }

        if (checkMoveDoAction()) {
            return 1;
        }
    }

    if (!checkModeFlg(0x40000) && checkSubjectAction()) {
        return 1;
    }

    return checkItemChangeFromButton();
}

BOOL daAlink_c::checkGroundSpecialMode() {
    if (mLinkAcch.ChkGroundHit() && !checkModeFlg(MODE_PLAYER_FLY) && !checkMagneBootsOn() &&
        checkEndResetFlg0(ERFLG0_FORCE_WOLF_CHANGE))
    {
        return procCoMetamorphoseInit();
    }

    if (mLinkAcch.ChkGroundHit() && !checkModeFlg(MODE_PLAYER_FLY) && checkBoardRestart()) {
        return procBoardWaitInit(mRideAcKeep.getActor());
    }

    if (checkGoatStopGame()) {
        return procGoatMoveInit();
    }

    if (mMode == 1) {
        return procSumouReadyInit();
    }

    if (checkGoronSideMove()) {
        return procGoronMoveInit();
    }

    if (checkCanoeSlider()) {
        return procCanoeJumpRideInit(NULL);
    }

    if (checkSlideAction()) {
        return 1;
    }

    fopAc_ac_c* goronActor = checkGoronRide();
    if (goronActor != NULL) {
        return procGoronRideWaitInit(goronActor);
    }

    if (checkEndResetFlg1(ERFLG1_NS_SCREAM)) {
        return procScreamWaitInit();
    }

    return NULL;
}

int daAlink_c::commonCheckNextAction(int param_0) {
    if (checkWolf()) {
        return checkNextActionWolf(param_0);
    } else {
        return checkNextAction(param_0);
    }
}

int daAlink_c::checkNextAction(int param_0) {
    f32 prev_maxSpeed = mMaxSpeed;

    if (checkDeadAction(1)) {
        return 1;
    }

    if (checkGroundSpecialMode()) {
        return 1;
    }

    if (mProcID != PROC_MOVE_TURN && (param_0 == 0 || checkInputOnR()) &&
        (mProcID != PROC_HOOKSHOT_MOVE || checkHookshotWait()))
    {
        if (mTargetedActor != NULL || checkAttentionLock()) {
            setSpeedAndAngleAtn();
        } else if (((!checkUpperReadyThrowAnime() || mFastShotTime != 0)
                    || (checkBoomerangThrowAnime()
                        && (mThrowBoomerangAcKeep.getActor() != NULL || mEquipItem == 0x102 || checkNoResetFlg1(FLG1_UNK_2))
                        )
                    ) || (checkCopyRodThrowAnime() && (mCopyRodAcKeep.getActor() != NULL || checkNoResetFlg1(FLG1_UNK_10000))))
        {
            setSpeedAndAngleNormal();
        }
    }

    setFrontWallType();

    bool isAttention = checkAttentionState();
    if (isAttention) {
        if (checkFmChainGrabAnime()) {
            mMaxSpeed = 13.0f;
        } else if (mTargetedActor != NULL) {
            mMaxSpeed = mpHIO->mAtnMove.m.mMaxSpeed;
        } else {
            mMaxSpeed = mpHIO->mNoActAtnMove.m.mMaxSpeed;
        }
    } else if (checkSlope()) {
        mMaxSpeed = mpHIO->mSlide.m.mMaxClimbSpeed;
    } else {
        mMaxSpeed = mpHIO->mMove.m.mMaxSpeed;
    }

    if (checkEndResetFlg0(ERFLG0_UNK_100000) && mLinkAcch.ChkGroundHit() &&
        !checkModeFlg(MODE_PLAYER_FLY))
    {
        return procFloorDownReboundInit();
    }

    if (checkCutDashChargeAnime() && !checkDashAnime()) {
        return procCutTurnMoveInit(0);
    }

    if (checkNextActionFromButton()) {
        return 1;
    }

    if (param_0 != 0 && !checkInputOnR() && !checkEquipAnime()) {
        mMaxSpeed = prev_maxSpeed;
        return 0;
    }

    int ret;
    if (isAttention) {
        if (checkBowAnime()) {
            ret = checkNextActionBow();
        } else if (checkBoomerangAnimeAndReturnWait() && mEquipItem != 0x102) {
            ret = checkNextActionBoomerang();
        } else if (checkCopyRodAnime()) {
            ret = checkNextActionCopyRod();
        } else if (checkHookshotAnime()) {
            ret = checkNextActionHookshot();
        } else if (checkIronBallAnime()) {
            ret = checkNextActionIronBall();
        } else if (mTargetedActor != NULL || mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_23_e) {
            if (checkZeroSpeedF()) {
                ret = procAtnActorWaitInit();
            } else {
                ret = procAtnActorMoveInit();
            }
        } else if (checkZeroSpeedF()) {
            ret = checkWaitAction();
        } else {
            ret = procAtnMoveInit();
        }
    } else {
        field_0x2f98 = 4;

        if (checkZeroSpeedF()) {
            if (cLib_distanceAngleS(mMoveAngle, current.angle.y) > 0x7800 && checkInputOnR()) {
                ret = procWaitTurnInit();
                if (!ret && !checkEventRun()) {
                    ret = checkWaitAction();
                }
            } else {
                ret = checkWaitAction();
            }
        } else if (mProcID == PROC_MOVE_TURN && current.angle.y != shape_angle.y) {
            ret = procMoveTurnInit(0);
        } else if (checkInputOnR() && cLib_distanceAngleS(mMoveAngle, current.angle.y) > 0x7800) {
            if (speedF / mMaxSpeed > mpHIO->mMove.m.mSlideThresholdSpeed &&
                mGndPolySpecialCode != dBgW_SPCODE_ICE && !checkGrabAnime() &&
                getDirectionFromAngle(mPrevStickAngle - mStickAngle) == DIR_BACKWARD)
            {
                ret = procSlipInit();
            } else {
                ret = procMoveTurnInit(1);
            }
        } else if (checkInputOnR() && getDirectionFromCurrentAngle() == DIR_BACKWARD) {
            ret = procMoveTurnInit(1);
        } else {
            ret = procMoveInit();
        }
    }

    return ret;
}

void daAlink_c::commonChangeItem() {
    if (mEquipItem == fpcNm_ITEM_KANTERA && field_0x2fde == 0xFF && (mProcID != PROC_PREACTION_UNEQUIP || mProcVar4.field_0x3010 != 0)) {
        offKandelaarModel();
    }

    if ((mEquipItem == fpcNm_ITEM_NONE && field_0x2fde != 0x103) || (mEquipItem == 0x103 && field_0x2fde != 0xFF)) {
        seStartOnlyReverb(Z2SE_AL_ITEM_TAKEOUT);
    }

    if (mEquipItem == 0x103) {
        if (checkWoodSwordEquip()) {
            seStartSwordCut(Z2SE_AL_ITEM_TAKEOUT_FAST);
        } else {
            seStartSwordCut(Z2SE_AL_SWORD_PUTIN);
        }
    }

    if (checkSwordEquipAnime()) {
        field_0x2f96 = 0xFF;
        field_0x2f97 = 0xFF;
    }

    BOOL var_r30;
    if (mEquipItem != fpcNm_ITEM_NONE) {
        var_r30 = true;
        deleteEquipItem(TRUE, FALSE);
    } else {
        var_r30 = false;
    }

    mEquipItem = field_0x2fde;
    field_0x2fde = fpcNm_ITEM_NONE;

    if (checkReinRide()) {
        field_0x2060->initOldFrameMorf(3.0f, 0, 0x23);
    }

    onNoResetFlg1(FLG1_UNK_40000);

    if (!checkEventRun()
        && !checkNoResetFlg1(FLG1_UNK_40)
        && (mEquipItem != 0x103 || !checkNoResetFlg3(FLG3_UNK_8000))
        && !checkFishingRodAndLureItem()
        && (checkAttentionLock() || mEquipItem != 0x103 || var_r30))
    {
        onResetFlg0(RFLG0_UNK_80);
    }

    if (mEquipItem == 0x103) {
        if (checkWoodSwordEquip()) {
            seStartSwordCut(Z2SE_AL_ITEM_TAKEOUT_FAST);
        } else {
            seStartSwordCut(Z2SE_AL_SWORD_PULLOUT);
        }

        setSwordModel();
    } else {
        makeItemType();
    }
}

void daAlink_c::setItemAction() {
    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        checkIronBallDelete();
    }

    daPy_frameCtrl_c* frame_ctrl = &mUpperFrameCtrl[2];

    if (!checkMagneBootsOn() && checkEquipHeavyBoots() && mLinkAcch.ChkGroundHit() && !checkModeFlg(0x70C52)) {
        if (dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd) && daTagMagne_c::checkMagnetCode(mLinkAcch.m_gnd)) {
            onNoResetFlg0(FLG0_MAGNE_BOOTS_ON);
            setMagneBootsMtx(&mLinkAcch.m_gnd, 1);
            mZ2Link.setMagnetized(true);

            if (checkUpperReadyThrowAnime() && !checkBowAnime()) {
                resetUpperAnime(UPPER_2, 3.0f);
            }

            if (mEquipItem != 0x103 && !checkBowItem(mEquipItem)) {
                deleteEquipItem(FALSE, FALSE);
            }

            offKandelaarModel();
            freeGrabItem();
        }
    }

    if (checkGrabAnime()) {
        return setGrabUpperSpeedRate();
    } else if (checkCopyRodControllAnime()) {
        if (getCopyRodControllActor() == NULL) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else {
            setCopyRodControllUpperSpeedRate();
        }
    } else if (checkBoomerangCatchAnime()) {
        if (checkAnmEnd(frame_ctrl) || (checkInputOnR() && frame_ctrl->getFrame() > mpHIO->mItem.mBoomerang.m.mCatchAnm.mCancelFrame)) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else if (frame_ctrl->getFrame() >= 20.0f) {
            field_0x2f97 = 6;
        }
    } else if (checkBoomerangThrowAnime()) {
        if (checkAnmEnd(frame_ctrl) || (checkInputOnR() && frame_ctrl->getFrame() > mpHIO->mItem.mBoomerang.m.mThrowAnm.mCancelFrame)) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else if (frame_ctrl->checkPass(5.5f)) {
            throwBoomerang();
        }
    } else if (checkCopyRodThrowAnime()) {
        if (checkAnmEnd(frame_ctrl) || (checkInputOnR() && frame_ctrl->getFrame() > mpHIO->mItem.mBoomerang.m.mThrowAnm.mCancelFrame)) {
            resetUpperAnime(UPPER_2, 3.0f);
        } else if (frame_ctrl->checkPass(6.5f)) {
            throwCopyRod();
        }
    } else if (checkKandelaarSwingAnime()) {
        if (checkAnmEnd(frame_ctrl) || (checkInputOnR() && frame_ctrl->getFrame() > mpHIO->mItem.mLantern.m.mShakeAnm.mCancelFrame)) {
            resetUpperAnime(UPPER_2, 3.0f);
        }
    } else if (checkEquipAnime()) {
        f32 change_item_frame;
        if (checkSwordEquipAnime()) {
            change_item_frame = frame_ctrl->getRate() + 9.5f;
        } else if (checkTwoHandItemEquipAnime()) {
            change_item_frame = 7.0f;
        } else {
            change_item_frame = 6.0f;
        }

        if (checkNoResetFlg1(FLG1_UNK_40000)) {
            f32 var_f30;
            if (checkSwordEquipAnime()) {
                var_f30 = 9.5f;
            } else if (checkTwoHandItemEquipAnime()) {
                var_f30 = 7.0f;
            } else {
                var_f30 = 6.0f;
            }

            f32 ratio = mNowAnmPackUpper[2].getRatio();
            cLib_chaseF(&ratio, 0.0f, fabsf(frame_ctrl->getRate()) / (frame_ctrl->getEnd() - var_f30));
            mNowAnmPackUpper[2].setRatio(ratio);
        }

        if (checkAnmEnd(frame_ctrl)) {
            resetUpperAnime(UPPER_2, 3.0f);
            setHorseSwordUp(1);
        } else if (frame_ctrl->checkPass(change_item_frame)) {
            commonChangeItem();
        }
    } else if (checkCutDashAnime()) {
        field_0x307e = mpHIO->mCut.m.mComboDuration;

        if (checkAnmEnd(frame_ctrl)) {
            resetUpperAnime(UPPER_2, 3.0f);
            onResetFlg0(RFLG0_UNK_8000000);
        } else {
            const daAlinkHIO_cutDash_c1* cut_hio;

            if (checkUpperAnime(dRes_ID_ALANM_BCK_CUTDL_e)) {
                cut_hio = &mpHIO->mCut.mCutDashLeft.m;
            } else if (checkUpperAnime(dRes_ID_ALANM_BCK_CUTDR_e)) {
                cut_hio = &mpHIO->mCut.mCutDashRight.m;
            } else {
                cut_hio = &mpHIO->mCut.mCutDashCharge.m;
            }

            if (frame_ctrl->getFrame() >= cut_hio->mAttackStartFrame && frame_ctrl->getFrame() < cut_hio->mAttackEndFrame) {
                if (!checkNoResetFlg0(FLG0_CUT_AT_FLG)) {
                    seStartSwordCut(Z2SE_AL_SWORD_SWING_S);
                }

                onResetFlg0(RFLG0_UNK_2);
            }

            if (!checkDashAnime() && mProcID != PROC_BOARD_CUT) {
                clearComboReserb();
            } else if (field_0x2fce == 0 && swordSwingTrigger()) {
                onNoResetFlg2(FLG2_COMBO_RESERB);
            }

            offNoResetFlg2(FLG2_UNK_4);

            if (swordButton() && !checkNoResetFlg1(FLG1_UNK_4000000)) {
                if (field_0x2fb7 < 3) {
                    field_0x2fb7++;
                } else {
                    onNoResetFlg2(FLG2_UNK_4);
                }
            } else {
                field_0x2fb7 = 0;
            }
        }
    } else if (checkCutDashChargeAnime() && !swordButton()) {
        setCutDash(1, checkAnmEnd(frame_ctrl));
    }
}

BOOL daAlink_c::checkNextActionFromCrouch(int param_0) {
    if (checkNextAction(param_0)) {
        if (field_0x2f8c == 1 || field_0x2f8c == 2 || field_0x2f8c == 3) {
            field_0x2060->initOldFrameMorf(mpHIO->mCrouch.m.mStandInterpolation, 0, 0x23);
        }

        return true;
    }

    return false;
}

int daAlink_c::checkUpperReadyThrowAnime() const {
    return checkBoomerangAnime()
            || checkCopyRodAnime()
            || checkHookshotAnime()
            || checkIronBallAnime()
            || checkBowAnime();
}

void daAlink_c::getBodyAngleXBasePos(cXyz* o_pos) {
    int jointNo;

    if (mProcID == PROC_HOOKSHOT_WALL_SHOOT || mProcID == PROC_HOOKSHOT_ROOF_SHOOT) {
        if (field_0x3020 == 0) {
            jointNo = 7;
        } else {
            jointNo = 12;
        }
    } else {
        jointNo = 1;
    }

    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(jointNo), o_pos);
}

s16 daAlink_c::getBodyAngleXAtnActor(int param_0) {
    s16 var_r30;

    if (mTargetedActor != NULL) {
        cXyz sp18;
        getBodyAngleXBasePos(&sp18);

        cXyz spC = mTargetedActor->eyePos - sp18;
        multVecMagneBootInvMtx(&spC);

        spC.y -= 40.0f;

        if (spC.abs() < 1.0f) {
            var_r30 = 0;
        } else {
            var_r30 = spC.atan2sY_XZ();
            if (param_0 != 0) {
                var_r30 = cLib_minMaxLimit<s16>((s16)var_r30, mpHIO->mCut.m.mUnkBodyUpwards, mpHIO->mCut.m.mUnkBodyDownwards);
            } else {
                var_r30 = cLib_minMaxLimit<s16>((s16)var_r30, -0x2000, 0x2000);
            }
        }
    } else {
        var_r30 = 0;
    }

    if (checkNotItemSinkLimit() && var_r30 > 0) {
        var_r30 = 0;
    }

    return var_r30;
}

void daAlink_c::setBodyAngleXReadyAnime(int param_0) {
    s16 angle_x = getBodyAngleXAtnActor(param_0);

    if (checkUpperGuardAnime()) {
        angle_x >>= 1;
    }

    cLib_addCalcAngleS(&mBodyAngle.x, angle_x, 4, 0x0C00, 0x0180);
    field_0x310a = mBodyAngle.x;
}

void daAlink_c::setMagicArmorBrk(int i_status) {
    static const char* bodyBrkName[3] = {
        "ml_body_power_down.brk",
        "ml_body_power_up_a.brk",
        "ml_body_power_up_b.brk",
    };

    static const char* headBrkName[3] = {
        "ml_head_power_down.brk",
        "ml_head_power_up_a.brk",
        "ml_head_power_up_b.brk",
    };

    J3DModelData* modelData = mpLinkModel->getModelData();
    mMagicArmorBodyBrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_mArcName, bodyBrkName[i_status]);
    mMagicArmorBodyBrk->searchUpdateMaterialID(modelData);
    modelData->entryTevRegAnimator(mMagicArmorBodyBrk);
    mMagicArmorBodyBrk->setFrame(0.0f);

    modelData = mpLinkHatModel->getModelData();
    mMagicArmorHeadBrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_mArcName, headBrkName[i_status]);
    mMagicArmorHeadBrk->searchUpdateMaterialID(modelData);
    modelData->entryTevRegAnimator(mMagicArmorHeadBrk);
    mMagicArmorHeadBrk->setFrame(0.0f);

    field_0x2fd7 = i_status;
}

BOOL daAlink_c::checkMagicArmorHeavy() const {
    return checkMagicArmorWearAbility() && dComIfGs_getRupee() == 0;
}

BOOL daAlink_c::checkBootsOrArmorHeavy() const {
    return checkEquipHeavyBoots() || checkMagicArmorHeavy() || checkIronBallWaitAnime();
}

// param_0 something related to boots
BOOL daAlink_c::checkHeavyStateOn(BOOL param_0, BOOL param_1) {
    if (!checkEventRun()) {
        if ((param_1 && mSinkShapeOffset < -30.0f)
            || (param_0 && checkIronBallWaitAnime())
            || checkIronBallAnime()
            || checkMagicArmorHeavy()
            || getHeavyStateAndBoots())
        {
            return true;
        }
    }

    return false;
}

void daAlink_c::setOutPower(f32 param_0, s16 param_1, int param_2) {
    if (!checkFmChainGrabAnime() || !checkBossRoom() || fopAcM_GetRoomNo(this) != 50 ||
        !checkEquipHeavyBoots())
    {
        field_0x3400 += param_0 * cM_ssin(param_1);
        field_0x3404 += param_0 * cM_scos(param_1);

        if (param_2) {
            onEndResetFlg0(ERFLG0_UNK_8);
        }
    }
}

void daAlink_c::initGravity() {
    f32 gravity;
    f32 max_fall_speed;

    if (checkWolf()) {
        if (checkModeFlg(MODE_SWIMMING)) {
            if (checkHeavyStateOn(TRUE, TRUE)) {
                gravity = -mpHIO->mWolf.mWlSwim.m.mHeavyBuoyancy;
                max_fall_speed = mpHIO->mWolf.mWlSwim.m.mHeavyMaxFallSpeed;
            } else {
                gravity = -mpHIO->mWolf.mWlSwim.m.mBuoyancy;
                max_fall_speed = mpHIO->mWolf.mWlSwim.m.mMaxFallSpeed;
            }
        } else {
            gravity = mpHIO->mWolf.mWlAutoJump.m.mGravity;
            max_fall_speed = mpHIO->mWolf.mWlAutoJump.m.mMaxFallSpeed;
        }
    } else if (checkModeFlg(MODE_SWIMMING) || checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        max_fall_speed = getSwimMaxFallSpeed();
        if (checkHeavyStateOn(TRUE, TRUE) && mProcID != PROC_OCTAIEAL_SPIT) {
            gravity = mpHIO->mSwim.m.mBootsGravity;
        } else {
            gravity = -mpHIO->mSwim.m.mBuoyancy;
        }
    } else {
        gravity = mpHIO->mAutoJump.m.mGravity;
        max_fall_speed = mpHIO->mAutoJump.m.mMaxFallSpeed;
    }

    setSpecialGravity(gravity, max_fall_speed, TRUE);
}

void daAlink_c::setSpecialGravity(f32 i_gravity, f32 i_speed, BOOL i_isOffFlag) {
    if (i_isOffFlag) {
        offNoResetFlg3(FLG3_UNK_4000);
    } else {
        onNoResetFlg3(FLG3_UNK_4000);
    }

    gravity = i_gravity;
    maxFallSpeed = i_speed;
}

void daAlink_c::transAnimeProc(cXyz* param_0, f32 param_1, f32 param_2) {
    mDoExt_MtxCalcAnmBlendTbl* temp_r26 = field_0x1f20;
    *param_0 = field_0x34d4;

    J3DTransformInfo sp60;
    J3DTransformInfo sp40;
    temp_r26->getAnm(0)->getTransform(0, &sp60);

    for (int i = 1; i < 3; i++) {
        J3DAnmTransform* sp8 = temp_r26->getAnm(i);
        if (sp8 != NULL) {
            sp8->getTransform(0, &sp40);

            f32 temp_f1 = temp_r26->getRatio(i);
            f32 temp_f30 = 1.0f - temp_f1;
            sp60.mTranslate.x = (sp60.mTranslate.x * temp_f30) + (sp40.mTranslate.x * temp_f1);
            sp60.mTranslate.y = (sp60.mTranslate.y * temp_f30) + (sp40.mTranslate.y * temp_f1);
            sp60.mTranslate.z = (sp60.mTranslate.z * temp_f30) + (sp40.mTranslate.z * temp_f1);
        }
    }

    J3DTransformInfo* temp_r29 = field_0x2060->getOldFrameTransInfo(0);
    cXyz sp30;

    if (field_0x2f99 == 0x50) {
        sp30 = current.pos - field_0x3798;

        if (checkModeFlg(0x40) || mProcID == PROC_STEP_MOVE || mProcID == PROC_WOLF_STEP_MOVE) {
            sp30.y = sp30.y - mSinkShapeOffset;
            mSinkShapeOffset = 0.0f;
            old.pos = current.pos;
            field_0x3798 = current.pos;
        }

        temp_r29->mTranslate.x -= (-param_1 * sp30.z) + (param_2 * sp30.x);
        temp_r29->mTranslate.y -= sp30.y;
        temp_r29->mTranslate.z -= (param_2 * sp30.z) + (param_1 * sp30.x);
    } else if (field_0x2f99 == 0x40 || field_0x2f99 == 0x90 || field_0x2f99 == 0x80 || field_0x2f99 == 0xA0 || field_0x2f99 == 0x70) {
        if (field_0x2f99 == 0x80) {
            sp30.set(0.0f, sp60.mTranslate.y - temp_r29->mTranslate.y, 0.0f);
        } else if (field_0x2f99 == 0xA0) {
            sp30.set(field_0x384c->x + (sp60.mTranslate.x - temp_r29->mTranslate.x), field_0x384c->y + (sp60.mTranslate.y - temp_r29->mTranslate.y), field_0x384c->z + (sp60.mTranslate.z - temp_r29->mTranslate.z));
        } else if (field_0x2f99 == 0x90) {
            sp30.set(sp60.mTranslate.x - temp_r29->mTranslate.x, 0.0f, sp60.mTranslate.z - temp_r29->mTranslate.z);
        } else {
            sp30.set(sp60.mTranslate.x - temp_r29->mTranslate.x, sp60.mTranslate.y - temp_r29->mTranslate.y, sp60.mTranslate.z - temp_r29->mTranslate.z);
        }

        cXyz sp24;
        sp24.x = (param_1 * sp30.z) + (param_2 * sp30.x);
        sp24.y = sp30.y;
        sp24.z = (param_2 * sp30.z) - (param_1 * sp30.x);

        if (checkMagneBootsOn()) {
            mDoMtx_stack_c::copy(mMagneBootMtx);
            mDoMtx_stack_c::YrotM(shape_angle.y);
            mDoMtx_stack_c::multVec(&sp30, &sp24);
        }

        current.pos -= sp24;
        field_0x3798 -= sp24;

        if (field_0x2f99 == 0xA0 && mProcID == PROC_BACK_JUMP) {
            mLastJumpPos -= sp24;
        }

        if (field_0x2f99 == 0x70) {
            cXyz sp18(current.pos.x, current.pos.y + sp24.y, current.pos.z);
            mLinkGndChk.SetPos(&sp18);

            f32 temp_f1_2 = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
            if (current.pos.y < temp_f1_2) {
                sp18.y = temp_f1_2 - current.pos.y;
                current.pos.y += sp18.y;
                field_0x3798.y += sp18.y;
            }
        }

        temp_r29->mTranslate.y = sp60.mTranslate.y;

        if (field_0x2f99 != 0x80) {
            temp_r29->mTranslate.x = sp60.mTranslate.x;
            temp_r29->mTranslate.z = sp60.mTranslate.z;
        }
    } else if ((field_0x2f99 == 0x20) || (field_0x2f99 == 0x30)) {
        temp_r29->mTranslate.z = sp60.mTranslate.z;
        if (field_0x2f99 != 0x20) {
            temp_r29->mTranslate.x = sp60.mTranslate.x;
        }
    } else if (field_0x2f99 == 0x10) {
        field_0x34d4.x = 0.0f;
        field_0x34d4.z = 0.0f;

        param_0->x = 0.0f;
        param_0->z = 0.0f;
        param_0->y = sp60.mTranslate.y;

        temp_r29->mTranslate.x = 0.0f;
        temp_r29->mTranslate.z = 0.0f;
        temp_r29->mTranslate.y = sp60.mTranslate.y;

        field_0x33b0 = sp60.mTranslate.y;

        if (checkModeFlg(0x10000)) {
            field_0x2f99 = 6;
        } else {
            field_0x2f99 = 7;
        }

        if (mProcID == PROC_HORSE_RIDE || mProcID == PROC_SWIM_DIVE) {
            param_0->y = 0.0f;
            field_0x33b0 = 0.0f;
        } else if (mProcID == PROC_OCTAIEAL_SPIT) {
            field_0x33b0 = -45.0f;
            param_0->y = -45.0f;
        }
    } else if (checkRootTransClearContinueMode()) {
        param_0->x = sp60.mTranslate.x;
        param_0->y = sp60.mTranslate.y;
        param_0->z = sp60.mTranslate.z;
        field_0x2f99 &= ~0x8;
    }

    field_0x34d4.x = sp60.mTranslate.x;
    field_0x34d4.y = sp60.mTranslate.y;
    field_0x34d4.z = sp60.mTranslate.z;
}

void daAlink_c::setFootSpeed() {
    cXyz sp18[2];

    f32 var_f31;
    if (field_0x2060->getOldFrameFlg()) {
        mDoMtx_concat(mInvMtx, mpLinkModel->getAnmMtx(field_0x30bc), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp18[0]);

        mDoMtx_concat(mInvMtx, mpLinkModel->getAnmMtx(field_0x30be), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp18[1]);

        int var_r28;
        if (sp18[0].y < sp18[1].y) {
            var_r28 = 0;
        } else {
            var_r28 = 1;
        }

        cXyz sp8 = sp18[var_r28] - field_0x37b0[var_r28];
        s16 temp_r0 = current.angle.y - shape_angle.y;
        var_f31 = fabsf(sp8.z * cM_scos(temp_r0)) + fabsf(sp8.x * cM_ssin(temp_r0));

        if (fabsf(mSpeedModifier) < 1.0f && checkInputOnR() && fabsf(field_0x33a4 - mStickValue) < 0.2f) {
            var_f31 = (0.3f * var_f31) + (0.7f * field_0x33a0);
        }
    } else {
        var_f31 = 0.0f;

        for (int i = 0; i < 2; i++) {
            sp18[i] = current.pos;
        }
    }

    for (int i = 0; i < 2; i++) {
        field_0x37b0[i] = sp18[i];
    }

    field_0x33a0 = var_f31;
}

void daAlink_c::posMove() {
    f32 temp_f30 = cM_ssin(shape_angle.y);
    f32 temp_f29 = cM_scos(shape_angle.y);
    cXyz sp108;

    if (!checkNoResetFlg3(FLG3_UNK_4000)) {
        initGravity();
    }

    transAnimeProc(&sp108, temp_f30, temp_f29);

    s16 var_r26;
    if (checkModeFlg(MODE_SWIMMING)) {
        var_r26 = field_0x3080;
    } else if (!mLinkAcch.ChkGroundHit() || checkMagneBootsOn() ||
               (checkModeFlg(MODE_PLAYER_FLY) && !checkBoardRide()) || mGroundCode == 8)
    {
        var_r26 = 0;
    } else {
        var_r26 = getGroundAngle(&mLinkAcch.m_gnd, current.angle.y);
    }

    setFootSpeed();
    if (checkEndResetFlg0(ERFLG0_WOLF_FCHAIN_PULL)) {
        speedF = 0.0f;
        mNormalSpeed = 0.0f;
    }

    speedF = mNormalSpeed * (1.0f - fabsf(mSpeedModifier));

    f32 mod = field_0x33a0 * (1.0f - field_0x2060->getOldFrameRate()) * mSpeedModifier;
    if (speedF < 0.0f) {
        speedF -= mod;
    } else {
        speedF += mod;
    }

    if (getZoraSwim() && !checkZoraWearAbility()) {
        speedF *= mpHIO->mSwim.m.mFloatUpSwimSpeedRate;
    }

    f32 var_f31 = speedF;
    speedF *= cM_scos(var_r26);

    if (var_r26 < 0 && !checkBoardRide() && !checkModeFlg(MODE_SWIMMING)) {
        speedF *= 0.85f;
    }

    if (mProcID == PROC_AUTO_JUMP && checkGrabGlide() &&
        (mWindTgCyls[0].ChkTgHit() || mWindTgCyls[1].ChkTgHit() || mWindTgCyls[2].ChkTgHit()))
    {
        speedF *= 0.5f;
    }

    speed.x = speedF * cM_ssin(current.angle.y);
    speed.z = speedF * cM_scos(current.angle.y);

    if (checkNoCollisionCorret() || (checkModeFlg(0x40) && checkModeFlg(0x4000))) {
        field_0x342c = 0.0f;
        field_0x3430 = 0.0f;
    }

    BOOL eventRunning = checkEventRun();
    if (!checkEquipHeavyBoots() && mLinkAcch.ChkGroundHit() && !checkModeFlg(0x40) &&
        !eventRunning)
    {
        if (checkLv2MiddleBossBgRide(getMoveBGActorName(mLinkAcch.m_gnd, FALSE))) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &plane);

            f32 temp_f0 = cM_sht2d(cM_atan2s(plane.mNormal.absXZ(), plane.mNormal.y));

            if (temp_f0 >= mpHIO->mSlide.m.mLV2MinibossFloorWeakSlideAngle &&
                temp_f0 <= mpHIO->mSlide.m.mLV2MinibossFloorSlideAngle)
            {
                temp_f0 = ((temp_f0 - mpHIO->mSlide.m.mLV2MinibossFloorWeakSlideAngle) *
                           mpHIO->mSlide.m.mLV2MinibossFloorWeakSlideSpeed) /
                          (mpHIO->mSlide.m.mLV2MinibossFloorSlideAngle -
                           mpHIO->mSlide.m.mLV2MinibossFloorWeakSlideAngle);
                plane.mNormal.y = 0.0f;
                plane.mNormal.normalizeZP();

                speed.x += temp_f0 * plane.mNormal.x;
                speed.z += temp_f0 * plane.mNormal.z;
            }
        }
    }

    if (checkModeFlg(MODE_SWIMMING)) {
        cLib_chasePos(&field_0x3750, cXyz::Zero, mpHIO->mDamage.mDamSwim.m.mDeceleration);
        current.pos += field_0x3750;

        if (checkNoResetFlg0(FLG0_SWIM_UP) && mProcID != PROC_SWIM_DIVE) {
            current.pos.y = mWaterY;
        } else if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_START_e || mProcID == PROC_ELEC_DAMAGE ||
                   dComIfGp_checkPlayerStatus0(0, 0x10))
        {
            speed.y = 0.0f;
        } else if (checkWolf()) {
            if (checkHeavyStateOn(TRUE, TRUE)) {
                speed.y += mpHIO->mWolf.mWlSwim.m.mHeavyBuoyancy;

                if (speed.y > mpHIO->mWolf.mWlSwim.m.mHeavyMaxSurfacingSpeed) {
                    speed.y = mpHIO->mWolf.mWlSwim.m.mHeavyMaxSurfacingSpeed;
                }
            } else {
                speed.y += mpHIO->mWolf.mWlSwim.m.mBuoyancy;

                if (speed.y > mpHIO->mWolf.mWlSwim.m.mMaxSurfacingSpeed) {
                    speed.y = mpHIO->mWolf.mWlSwim.m.mMaxSurfacingSpeed;
                }
            }
        } else if (!checkEquipHeavyBoots() && getZoraSwim()) {
            speed.y = -var_f31 * cM_ssin(var_r26);
        } else if ((checkBootsOrArmorHeavy() && mProcID != PROC_DEAD) || mProcID == PROC_SWIM_DIVE)
        {
            speed.y += gravity;

            if (speed.y < maxFallSpeed) {
                speed.y = maxFallSpeed;
            }
        } else if (speed.y > mpHIO->mSwim.m.mMaxFloatUpSpeed) {
            speed.y += gravity;
        } else if (speed.y < maxFallSpeed) {
            speed.y += 1.0f;
        } else {
            if (checkZoraWearAbility() &&
                mWaterY > current.pos.y + mpHIO->mSwim.m.mNormalBuoyancyWaterDepth)
            {
                speed.y += mpHIO->mSwim.m.mZoraClothesBuoyancy;

                if (speed.y < 0.0f) {
                    speed.y = 0.0f;
                }
            } else {
                speed.y += mpHIO->mSwim.m.mBuoyancy;
            }

            if (speed.y > mpHIO->mSwim.m.mMaxFloatUpSpeed) {
                speed.y = mpHIO->mSwim.m.mMaxFloatUpSpeed;
            }
        }
    } else if (!checkModeFlg(0x400)) {
        if (checkHeavyStateOn(TRUE, TRUE) && mProcID != PROC_SPINNER_READY &&
            !checkNoResetFlg0(FLG0_WATER_IN_MOVE))
        {
            speed.y += gravity * 2.25f;

            if (speed.y < maxFallSpeed * 1.5f) {
                speed.y = maxFallSpeed * 1.5f;
            }
        } else {
            speed.y += gravity;

            if (speed.y < maxFallSpeed) {
                speed.y = maxFallSpeed;
            }
        }
    } else if (checkBoardRide()) {
        mLinkAcch.OffLineCheck();
        speed.y += gravity;

        if (speed.y < maxFallSpeed) {
            speed.y = maxFallSpeed;
        }
    }

    setIceSlipSpeed();
    var_f31 = current.pos.y;

    if (!checkModeFlg(0x400) || checkBoardRide()) {
        if (checkMagneBootsOn()) {
            mDoMtx_stack_c::copy(mMagneBootMtx);
            mDoMtx_stack_c::YrotM(current.angle.y);

            Vec spFC = {0.0f, 0.0f, 0.0f};
            spFC.z = speedF;
            mDoMtx_stack_c::multVecSR(&spFC, &speed);
            current.pos += speed;
            current.pos.x += field_0x342c;
            current.pos.z += field_0x3430;
        } else {
            current.pos += speed;
            current.pos.x += field_0x342c;
            current.pos.z += field_0x3430;

            if (checkEndResetFlg1(ERFLG1_WATERFALL_FRONT_HIT) && checkStageName("F_SP113")) {
                current.pos.y -= 50.0f;
            }
        }
    }

    if (checkBoardRide() && !checkModeFlg(2)) {
        current.pos.y -= speedF * cM_ssin(var_r26);
    }

    if (getSumouMode() && mProcID != PROC_SUMOU_WIN_LOSE) {
        current.pos.y = var_f31 - 1.0f;
        speed.y = gravity;
    }

    field_0x342c = 0.0f;
    field_0x3430 = 0.0f;

    setPolygonSpeed();
    setWindSpeed();
    BOOL var_r27 = FALSE;

    if (!checkNoCollisionCorret() && !checkModeFlg(0x40)) {
        if (!checkNoResetFlg3(FLG3_UNK_20000000)) {
            current.pos += *mCcStts.GetCCMoveP();
        }

        if (!eventRunning) {
            if (!checkEndResetFlg0(ERFLG0_UNK_8) || !checkHeavyStateOn(TRUE, TRUE)) {
                current.pos.x += field_0x3400;
                current.pos.z += field_0x3404;
            }

            if (mLinkAcch.ChkGroundHit() && dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd)) {
                s16 angle1 = getGroundAngle(&mLinkAcch.m_gnd, 0);
                current.pos.z += field_0x35c4.z * cM_scos(angle1);

                s16 angle2 = getGroundAngle(&mLinkAcch.m_gnd, 0x4000);
                current.pos.x += field_0x35c4.x * cM_scos(angle2);

                if (checkZeroSpeedF() && field_0x35c4.abs2() > SQUARE(3.0f)) {
                    seStartOnlyReverbLevel(Z2SE_AL_ICE_SLIP);
                }
            }

            current.pos += field_0x3594;

            if (checkModeFlg(2)) {
                current.pos += mWindSpeed;
            } else {
                current.pos.x += mWindSpeed.x;
                current.pos.z += mWindSpeed.z;
            }
        } else if (checkNoResetFlg0(daPy_FLG0(FLG0_DEMO_STREAM_ACCEPT | FLG0_UNK_4000))) {
            current.pos += field_0x3594;
        }
    } else if (checkOctaIealSpecialCollect()) {
        f32 temp_f1 = getHookshotTopPos()->abs(current.pos);

        current.pos += *mCcStts.GetCCMoveP();

        f32 temp_f31 = getHookshotTopPos()->abs(current.pos);
        if (temp_f31 > temp_f1) {
            current.pos.y += temp_f31 - temp_f1;
        }
    } else if (!eventRunning && (mProcID == PROC_HANG_MOVE || mProcID == PROC_CLIMB_MOVE_SIDE ||
                                 mProcID == PROC_CLIMB_MOVE_UPDOWN || mProcID == PROC_CLIMB_WAIT ||
                                 mProcID == PROC_HANG_WAIT))
    {
        f32 temp_f31 = mCcStts.GetCCMoveP()->absXZ();
        var_r27 = TRUE;

        if (temp_f31 > 1.0f) {
            s16 var_r23 = (s16)(mCcStts.GetCCMoveP()->atan2sX_Z() - shape_angle.y);
            if (var_r23 >= 0) {
                current.pos.x += temp_f31 * temp_f29;
                current.pos.z -= temp_f31 * temp_f30;
            } else {
                current.pos.x -= temp_f31 * temp_f29;
                current.pos.z += temp_f31 * temp_f30;
            }
        }
    }

    if (checkModeFlg(MODE_SWIMMING) && checkNoResetFlg0(FLG0_SWIM_UP)) {
        cXyz sp84 = current.pos - field_0x3798;

        f32 temp_f3 = sp84.absXZ() * cM_ssin(getGroundAngle(&mLinkAcch.m_wtr, sp84.atan2sX_Z()));
        if (temp_f3 < 0.0f) {
            temp_f3 = 0.0f;
        }

        if (!(current.pos.y > mWaterY + l_autoUpHeight + temp_f3) && mProcID != PROC_SWIM_DIVE) {
            current.pos.y = mWaterY;
        }
    }

    field_0x3400 = 0.0f;
    field_0x3404 = 0.0f;
    mCcStts.ClrCcMove();
    offNoResetFlg3(FLG3_UNK_20000000);
    field_0x3464 = 0.0f;
    field_0x3468 = 0.0f;

    if (checkRootTransClearMode()) {
        cXyz sp78 = field_0x34d4 - sp108;
        if (mProcID == PROC_CLIMB_UP_START && mProcVar5.field_0x3012 != 0) {
            sp78 = cXyz::Zero;
        }

        f32 var_f0;
        if (checkModeFlg(MODE_PLAYER_FLY)) {
            var_f0 = 1.0f;
        } else {
            var_f0 = cM_scos(field_0x2ff0);
        }

        if (!checkRootTransZClearMode()) {
            sp78.z = 0.0f;
        }

        if (!checkRootTransXClearMode()) {
            sp78.x = 0.0f;
        }

        field_0x3464 = var_f0 * (sp78.z * temp_f30 + sp78.x * temp_f29);
        field_0x3468 = var_f0 * (sp78.z * temp_f29 - sp78.x * temp_f30);

        if (checkMagneBootsOn()) {
            cXyz spD8;
            mDoMtx_stack_c::copy(mMagneBootMtx);
            mDoMtx_stack_c::YrotM(shape_angle.y);
            mDoMtx_stack_c::multVec(&sp78, &spD8);
            current.pos += spD8;
        } else {
            current.pos.x += field_0x3464;
            current.pos.z += field_0x3468;

            if (checkRootTransYClearMode()) {
                current.pos.y += sp78.y;
            }
        }
    }

    if (checkModeFlg(2) && current.pos.y > mFallHeight) {
        mFallHeight = current.pos.y;
    }

    if (checkMagneBootsOn()) {
        cXyz spCC;
        cXyz spC0 = current.pos + (mMagneBootsTopVec * l_autoUpHeight);
        cXyz spB4;
        Vec spA8 = {0.0f, 0.0f, 0.0f};

        s16 var_r27 = -0x8000;
        cM3dGPla plane;

        for (int i = 0; i < 8; i++) {
            spA8.x = mAcchCir[0].GetWallR() * cM_ssin(var_r27);
            spA8.z = mAcchCir[0].GetWallR() * cM_scos(var_r27);
            mDoMtx_multVec(mMagneBootMtx, &spA8, &spCC);
            spB4 = spC0 + spCC;

            if (commonLineCheck(&spC0, &spB4)) {
                dComIfG_Bgsp().GetTriPla(mLinkLinChk, &plane);
                f32 temp_f31 = spB4.abs(mLinkLinChk.GetCross());

                mDoMtx_multVecSR(mMagneBootInvMtx, &plane.mNormal, &spCC);
                if (cBgW_CheckBWall(spCC.y)) {
                    f32 temp_f1 = spCC.absXZ();

                    f32 div = temp_f31 / temp_f1;
                    spCC.x *= div;
                    spCC.y = 0.0f;
                    spCC.z *= div;

                    mDoMtx_multVecSR(mMagneBootMtx, &spCC, &spCC);
                    current.pos += spCC;
                    spC0 += spCC;
                }
            }

            var_r27 += 0x2000;
        }
    } else if (var_r27) {
        cXyz sp9C = cXyz::Zero;

        if (hangMoveBgCheck(shape_angle.y + 0x4000, &sp9C) ||
            hangMoveBgCheck(shape_angle.y - 0x4000, &sp9C))
        {
            current.pos.x += sp9C.x;
            current.pos.z += sp9C.z;
        }
    }
}

void daAlink_c::autoGroundHit() {
    offNoResetFlg0(FLG0_UNK_2000);

    if (checkMagneBootsOn()) {
        BOOL var_r27 = FALSE;
        cXyz sp68;
        cXyz sp5C;
        cXyz sp50;

        mDoMtx_stack_c::copy(mMagneBootMtx);
        mDoMtx_stack_c::multVecSR(&cXyz::BaseY, &sp50);
        sp68 = current.pos + (sp50 * 30.0f);
        sp5C = current.pos - (sp50 * 100.0f);

        if (commonMagneLineCheck(&sp68, &sp5C)) {
            current.pos = mMagneLineChk.GetCross();
            onNoResetFlg0(FLG0_UNK_2000);

            if (checkNoResetFlg1(FLG1_UNK_20000000)) {
                mDoMtx_stack_c::YrotM(current.angle.y);
                mDoMtx_stack_c::multVecSR(&cXyz::BaseZ, &sp50);
                sp50 *= 10.0f;
                sp68 += sp50;
                sp5C += sp50;

                if (commonLineCheck(&sp68, &sp5C) && daTagMagne_c::checkMagnetCode(mLinkLinChk)) {
                    offNoResetFlg1(FLG1_UNK_20000000);
                } else {
                    mNormalSpeed = 0.0f;
                    speedF = 0.0f;
                }
            }
        } else if (!cBgW_CheckBGround(mMagneBootsTopVec.y)) {
            sp50 = current.pos - field_0x3798;
            sp68 -= sp50;
            sp5C -= sp50;

            if (commonMagneLineCheck(&sp68, &sp5C)) {
                onNoResetFlg0(FLG0_UNK_2000);
                mNormalSpeed = 0.0f;
                speedF = 0.0f;
                onNoResetFlg1(FLG1_UNK_20000000);
                field_0x3092 = current.angle.y + 0x8000;
                current.pos = field_0x3798;
            }
        }
    } else if (!checkModeFlg(0x70C52) || (checkBoardRide() && !checkModeFlg(2)) || checkModeFlg(0x40)) {
        cM3dGPla sp74;
        f32 temp_f30 = mLinkAcch.GetGroundH() - current.pos.y;

        if (!mLinkAcch.ChkGroundHit()) {
            if (checkBoardRide() && shape_angle.x < 0) {
                return;
            }

            cXyz sp44 = current.pos - field_0x3798;
            s16 var_r29;

            f32 temp_f1 = sp44.absXZ();
            if (temp_f1 > 1.0f) {
                var_r29 = getGroundAngle(&mLinkAcch.m_gnd, sp44.atan2sX_Z());
            } else {
                var_r29 = field_0x2ff0;
            }

            f32 var_f31 = temp_f1 * cM_ssin(var_r29);
            if (var_f31 < 0.0f) {
                var_f31 = 0.0f;
            }

            BOOL var_r28;
            if (-G_CM3D_F_INF != mLinkAcch.GetGroundH()) {
                dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &sp74);
                var_r28 = cBgW_CheckBGround(sp74.mNormal.y);
            } else {
                var_r28 = 0;
            }

            if (var_r28 != 0) {
                if (temp_f30 <= 0.000001f) {
                    if (temp_f30 >= l_autoDownHeight - var_f31) {
                        current.pos.y = mLinkAcch.GetGroundH();
                        mLinkAcch.SetGroundHit();
                        speed.y = 0.0f;
                    }
                }
            }
        }
    } else if (checkModeFlg(0x40000) && checkNoResetFlg0(FLG0_UNK_80) && current.pos.y > mWaterY && current.pos.y - mWaterY < 1000.0f) {
        current.pos.y = mWaterY;
    }

    if (checkReinRide() || checkSpinnerRide()) {
        if ((current.pos.y - mLinkAcch.GetGroundH() > 1500.0f) || (checkSpinnerRide() && mRideAcKeep.getActor() != NULL && ((daSpinner_c*)mRideAcKeep.getActor())->getJumpFlg())) {
            if (!checkNoResetFlg3(FLG3_UNK_80000)) {
                onNoResetFlg3(FLG3_UNK_80000);
                mLastJumpPos = current.pos;
                mFallHeight = mLastJumpPos.y;
                field_0x33c8 = mLastJumpPos.y;
            }
        } else {
            offNoResetFlg3(FLG3_UNK_80000);
        }
    } else {
        offNoResetFlg3(FLG3_UNK_80000);
    }
}

bool daAlink_c::startPeepChange() {
    if (!checkNoResetFlg0(FLG0_UNK_4000)) {
        onNoResetFlg0(FLG0_UNK_4000);

        u32 mode;
        if (mProcID == PROC_PEEP_SUBJECTIVITY) {
            mode = 7;
        } else {
            mode = 6;
            u32 param = fopAcM_GetParam(this);
            dComIfGs_setTurnRestart(current.pos, shape_angle.y, fopAcM_GetRoomNo(this), param);
        }

        dStage_changeScene(mPeepExitID, 0.0f, mode, fopAcM_GetRoomNo(this),
                           shape_angle.y, -1);
        return 1;
    }

    return 0;
}

void daAlink_c::setLastSceneDamage(int i_dmgAmount, u32* o_mode) {
    if (
        #if DEBUG
        !mpHIO->mDamage.m.mInvincible &&
        #endif
        i_dmgAmount > 0
        )
    {
        if (i_dmgAmount >= 128) {
            JUT_ASSERT(19795, FALSE);

            #if !DEBUG
            i_dmgAmount = 127;
            #endif
        }

        *o_mode |= i_dmgAmount << 4;
    }
}

void daAlink_c::setLastSceneMode(u32* o_mode) {
    if (checkEquipHeavyBoots()) {
        *o_mode |= 0x400000;
    }

    if (checkWolf()) {
        *o_mode |= 0x800000;
    }

    if (checkNoResetFlg2(FLG2_UNK_1) && mEquipItem != fpcNm_ITEM_KANTERA) {
        *o_mode |= 0x200000;
    }

    if (mEquipItem == 0x103) {
        *o_mode |= 0x28000000;
    } else if (mEquipItem < fpcNm_ITEM_NONE) {
        *o_mode |= mEquipItem << 0x18;
    }

    if (mGrabItemAcKeep.getActor() != NULL) {
        if (daAlink_checkLightBallA(mGrabItemAcKeep.getActor())) {
            *o_mode |= 0x100000;
        }

        if (daAlink_checkLightBallB(mGrabItemAcKeep.getActor())) {
            *o_mode |= 0x80000;
        }
    }

    if (mSwordUpTimer != 0) {
        *o_mode |= (mSwordUpTimer & 0xFFFE) << 0xA;
    }
}

int daAlink_c::startRestartRoom(u32 i_mode, int param_1, int i_dmgAmount, BOOL i_isEventRun) {
    if (!checkNoResetFlg0(FLG0_UNK_4000) &&
        (i_isEventRun || dComIfGp_event_compulsory(this, NULL, 0xFFFF)))
    {
        if (!checkRestartDead(i_dmgAmount, i_mode == 4)) {
            onNoResetFlg0(FLG0_UNK_4000);

            if (!i_isEventRun) {
                mDemo.setOriginalDemoType();
                mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_1_e);
            }

            int start_mode = getStartMode();
            if ((start_mode != 13 && start_mode != 2 && start_mode != 14) || checkFinalBattle() ||
                field_0x3780.abs(dComIfGs_getRestartRoomPos()) > 0.1f)
            {
                start_mode = 0;
            }

            if (!dComIfGs_isEventBit(dSv_event_flag_c::F_0266) && (checkBoardRide() || checkBoardRestart())) {
                i_mode = 11;
            } else if (checkBoarSingleBattle() ||
                       (start_mode == 2 && !checkFinalBattle() && i_mode == 5))
            {
                i_mode = 8;
            }

            setLastSceneDamage(i_dmgAmount, &i_mode);
            dStage_restartRoom(setParamData(dComIfGs_getRestartRoomNo(), start_mode, param_1, 0), i_mode, 0);

            if (mProcID != PROC_DEAD) {
                seStartSystem(Z2SE_FORCE_BACK);
            }
        }

        return 1;
    }

    return 0;
}

BOOL daAlink_c::checkCoachGuardGame() {
    return !checkBoarSingleBattle() &&
           fopAcIt_Judge((fopAcIt_JudgeFunc)daAlink_searchCoach, NULL);
}

void daAlink_c::checkRoofRestart() {
    daSpinner_c* spinner;

    if (checkSpinnerRide()) {
        spinner = (daSpinner_c*)mRideAcKeep.getActor();
    } else {
        spinner = NULL;
    }

    if (mLinkAcch.ChkRoofHit()
        && (mLinkAcch.ChkGroundHit() || (spinner != NULL && spinner->checkGroundHit() && spinner->getJumpFlg() == 0))
        && !checkMagneBootsOn()
        && (mProcID != PROC_CRAWL_START && mProcID != PROC_CRAWL_END && !checkCoachGuardGame()))
    {
        s16 bg_actorName = getMoveBGActorName(mLinkAcch.m_roof, TRUE);
        if (bg_actorName == PROC_OBJ_SO || bg_actorName == PROC_Obj_SCannon) {
            return;
        }

        f32 roof_height = mLinkAcch.GetRoofHeight();
        if (roof_height > mLinkAcch.GetGroundH()) {
            f32 head_pos = mHeadTopPos.y - 10.0f;
            if (head_pos < 10.0f + current.pos.y) {
                head_pos = 10.0f + current.pos.y;
            }

            if (roof_height < head_pos) {
                startRestartRoom(5, 0xC9, 4, FALSE);
            }
        }
    }
}

BOOL daAlink_c::checkRestartRoom() {
    if (checkMagneBootsOn()) {
        if (checkNoResetFlg0(FLG0_UNK_2000) && (dKy_pol_argument_get(&mMagneLineChk) & 0x80)) {
            startRestartRoom(5, 0xC9, 4, FALSE);
            return true;
        }
    } else if (mLinkAcch.ChkGroundHit() && mGndPolyAtt1 == 2) {
        return procCoLavaReturnInit(TRUE);
    } else if ((checkModeFlg(0x40000) || (checkNoResetFlg0(FLG0_UNK_80) && mWaterY - current.pos.y > mpHIO->mSwim.m.mStartHeight))
                && fopAcM_GetRoomNo(this) == 0
                && checkStageName("F_SP114"))
    {
        return procCoSwimFreezeReturnInit();
    } else if (!checkCargoCarry() && (mGroundCode == 4 || mGroundCode == 10 || (-G_CM3D_F_INF == mLinkAcch.GetGroundH() && !checkModeFlg(0x40000)))) {
        BOOL temp_r28 = mWaterY > mLinkAcch.GetGroundH();

        f32 var_f31;
        if (mGroundCode == 10) {
            var_f31 = current.pos.y - mLinkAcch.GetGroundH();
        } else {
            var_f31 = field_0x33c8 - current.pos.y;
        }

        f32 var_f30;
        if (checkStageName("F_SP114")) {
            var_f30 = 3000.0f;
        } else if (checkBoarSingleBattle()) {
            var_f30 = 1000.0f;
        } else {
            var_f30 = 2000.0f;
        }

        if ((mGroundCode != 10 && ((temp_r28 && checkModeFlg(0x40000)) || (!temp_r28 && (checkModeFlg(2) || checkNoResetFlg3(FLG3_UNK_80000)) && mProcID != PROC_HOOKSHOT_FLY && !checkModeFlg(0x800) && var_f31 > var_f30)))
            || (mGroundCode == 10 && var_f31 <= 3000.0f)
            || (checkSpinnerRide() && mRideAcKeep.getActor() != NULL && ((daSpinner_c*)mRideAcKeep.getActor())->checkGroundHit())
            || (mLinkAcch.ChkGroundHit() && mProcID != PROC_HOOKSHOT_FLY))
        {
            int exitID;
            s8 roomNo;

            if (mGroundCode == 4 || mGroundCode == 10) {
                if (checkCoachGuardGame()) {
                    exitID = 0;
                    roomNo = -1;
                } else {
                    exitID = dComIfG_Bgsp().GetExitId(mLinkAcch.m_gnd);
                    roomNo = fopAcM_GetRoomNo(this);
                }

                if (checkModeFlg(2)) {
                    mLinkAcch.ClrGroundHit();
                }
            } else {
                exitID = 0x3F;
            }

            if (!checkNoResetFlg0(FLG0_UNK_40000) && checkWolf()) {
                voiceStart(Z2SE_WL_V_FALL_TO_RESTART);
                onNoResetFlg0(FLG0_UNK_40000);
            }

            if (exitID != 0x3F) {
                if (!checkNoResetFlg0(FLG0_UNK_4000) && dComIfGp_event_compulsory(this, NULL, 0xFFFF) && !checkRestartDead(4, FALSE)) {
                    mDemo.setOriginalDemoType();
                    if (checkSpinnerRide()) {
                        mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_14_e);
                        mDemo.setParam0(2);
                    } else {
                        mDemo.setDemoMode(1);
                    }

                    onNoResetFlg0(FLG0_UNK_4000);

                    u32 mode = 5;
                    setLastSceneDamage(4, &mode);
                    seStartSystem(Z2SE_FORCE_BACK);
                    dStage_changeScene(exitID, 0.0f, mode, roomNo, shape_angle.y, -1);
                }
            } else if (startRestartRoom(5, 0xC9, 4, FALSE) && checkSpinnerRide()) {
                mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_14_e);
                mDemo.setParam0(2);
            }

            return true;
        }
    } else if ((mLinkAcch.ChkGroundHit() && (dKy_pol_argument_get(&mLinkAcch.m_gnd) & 0x80))
                || (checkWaterPolygonUnder() && checkNoResetFlg0(FLG0_UNK_80) && (dKy_pol_argument_get(&mLinkAcch.m_wtr) & 0x80))
                || (checkModeFlg(0x40) && field_0x33bc < field_0x33b8 && -G_CM3D_F_INF != field_0x33b8 && (field_0x2fc6 & 0x80))
                || (field_0x2fbc == 6 && checkNoResetFlg0(FLG0_UNK_80) && checkWaterPolygonUnder() && mWaterY > (mpHIO->mBasic.m.mLavaDeathDepth + mLinkAcch.GetGroundH()))
                || (field_0x2fc5 == 6 && -G_CM3D_F_INF != field_0x33b8 && field_0x33bc < field_0x33b8 && checkModeFlg(0x40) && field_0x33b8 > (field_0x33d8 + mpHIO->mBasic.m.mLavaDeathDepth))
            )
    {
        if (field_0x2fbc == 6 || (checkModeFlg(0x40) && field_0x2fc5 == 6)) {
            return procCoLavaReturnInit(FALSE);
        }

        startRestartRoom(5, 0xC9, 4, FALSE);
        return true;
    }

    return false;
}

s16 daAlink_c::getSceneExitMoveAngle() {
    if (field_0x2f58 == NULL) {
        return current.angle.y;
    } else if (field_0x2f58->m_num == field_0x2fc2) {
        return mDemo.getMoveAngle();
    }

    dPnt* var_r30 = &field_0x2f58->m_points[field_0x2fc2];
    f32 dx = var_r30->m_position.x - current.pos.x;
    f32 dz = var_r30->m_position.z - current.pos.z;

    f32 var_f29;
    if (checkHorseRide()) {
        var_f29 = SQUARE(100.0f);
    } else {
        var_f29 = SQUARE(50.0f);
    }

    if (SQUARE(dx) + SQUARE(dz) < var_f29) {
        field_0x2fc2++;

        if (field_0x2f58->m_num == field_0x2fc2) {
            return mDemo.getMoveAngle();
        }

        var_r30++;
        dx = var_r30->m_position.x - current.pos.x;
        dz = var_r30->m_position.z - current.pos.z;
    }

    return cM_atan2s(dx, dz);
}

int daAlink_c::checkSceneChange(int i_exitID) {
    BOOL var_r3 = mProcID != PROC_FOG_DEAD && (mGroundCode == 9 || checkNoResetFlg2(FLG2_FOG_FADE));

    if (mGroundCode == 4 || mGroundCode == 10) {
        i_exitID = 0x3F;
    }

    if (var_r3 ||
        ((i_exitID != 0x3F || mExitID != 0x3F)
            && (checkModeFlg(0x40000)
                || checkModeFlg(0x400)
                || mLinkAcch.ChkGroundHit()
                || mProcID == PROC_DOOR_OPEN
                || mProcID == PROC_WARP
                || mProcID == PROC_WOLF_DIG_THROUGH
                || (mGroundCode == 5 && field_0x33c8 - current.pos.y > 500.0f)
                )
            ) || ((mExitID & 0x8000) && checkModeFlg(2))
        )
    {
        if (mGroundCode == 5 || ((mExitID & 0x8000) && mExitDirection == 0xFF)) {
            mLinkAcch.ClrGroundHit();
        }

        if (checkNoResetFlg0(FLG0_UNK_4000)) {
            return 1;
        }

        if (checkHorseRide()) {
            if (dComIfG_Bgsp().GetHorseNoEntry(mLinkAcch.m_gnd)) {
                return 0;
            }

            if (daTagHstop_c::getTop() != NULL) {
                daTagHstop_c* hstop = daTagHstop_c::getTop();
                cXyz pos;
                for (; hstop != NULL; hstop = hstop->getNext()) {
                    if (hstop->getActiveFlg()) {
                        fpoAcM_relativePos(hstop, &current.pos, &pos);

                        if (pos.y >= -200.0f && pos.y <= hstop->scale.y + 600.0f &&
                            fabsf(pos.x) <= hstop->scale.x && fabsf(pos.z) <= hstop->scale.z)
                        {
                            return 0;
                        }
                    }
                }
            }
        }

        BOOL is_horse_ride = checkHorseRide() && mProcID != PROC_HORSE_GETOFF;

        f32 exit_speed;
        f32 demo_stick;
        if (is_horse_ride) {
            if (mNormalSpeed < 15.0f) {
                exit_speed = 15.0f;
                demo_stick = 0.6f;
            } else {
                exit_speed = dComIfGp_getHorseActor()->getNormalMaxSpeedF();
                demo_stick = 1.0f;
            }
        } else if (checkWolf()) {
            if (mNormalSpeed < 10.0f || var_r3) {
                exit_speed = 10.0f;
                demo_stick = 0.6f;
            } else {
                exit_speed = mpHIO->mWolf.mWlMoveNoP.m.mMaxSpeed;
                demo_stick = 1.0f;
            }
        } else if (mNormalSpeed < 10.0f || var_r3) {
            exit_speed = 10.0f;
            demo_stick = 0.65f;
        } else {
            exit_speed = mpHIO->mMove.m.mMaxSpeed;
            demo_stick = 1.0f;
        }

        int exit_mode;
        if (is_horse_ride) {
            exit_mode = 1;
        } else if (checkModeFlg(0x1000000)) {
            exit_speed = mStickValue;
            if (mUnderFrameCtrl[0].getRate() >= 0.0f) {
                exit_mode = 2;
            } else {
                exit_mode = 3;
            }
        } else if (mProcID == PROC_WOLF_DIG_THROUGH) {
            exit_mode = 9;
        } else {
            exit_mode = 0;
        }

        if (eventInfo.checkCommandDoor()
            || mProcID == PROC_WARP
            || mProcID == PROC_WOLF_DIG
            || mProcID == PROC_WOLF_DIG_THROUGH
            || field_0x3106 != 0
            || dComIfGp_event_compulsory(this, NULL, -1))
        {
            BOOL isScnChange = false;

            if (var_r3) {
                mDemo.setOriginalDemoType();
                mDemo.setDemoMode(daPy_demo_c::DEMO_FOG_DEAD_e);
            } else {
                if (mExitID != 0x3F) {
                    isScnChange = dStage_changeScene(mExitID & 0xFF, exit_speed, exit_mode,
                                                      fopAcM_GetRoomNo(this), shape_angle.y, -1);
                    if (isScnChange) {
                        onNoResetFlg2(FLG2_SCENE_CHANGE_START);
                        if (mpScnChg != NULL && fopAcM_GetName(mpScnChg) == PROC_SCENE_EXIT) {
                            mpScnChg->setSceneChangeOK();
                        }
                    }

                    if (mExitDirection != 0xFF) {
                        field_0x2f58 = dPath_GetRoomPath(mExitDirection, fopAcM_GetRoomNo(this));
                    }
                } else {
                    isScnChange = dStage_changeSceneExitId(mLinkAcch.m_gnd, exit_speed, exit_mode,
                                                            fopAcM_GetRoomNo(this), shape_angle.y);
                    field_0x2f58 = dPath_GetRoomPath(dComIfG_Bgsp().GetRoomPathId(mLinkAcch.m_gnd),
                                                     fopAcM_GetRoomNo(this));
                }
            }

            if (isScnChange) {
                onNoResetFlg0(FLG0_UNK_4000);

                if (!eventInfo.checkCommandDoor()) {
                    mDemo.setOriginalDemoType();

                    if (checkUpperReadyThrowAnime()) {
                        resetUpperAnime(UPPER_2, 3.0f);
                    }

                    if (mGroundCode == 5) {
                        mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_17_e);
                    } else {
                        mDemo.setDemoMode(daPy_demo_c::DEMO_UNK_26_e);
                        mDemo.setStick(demo_stick);
                    }

                    mDemo.setMoveAngle(getSceneExitMoveAngle());
                    if (is_horse_ride) {
                        daHorse_c* horse = dComIfGp_getHorseActor();
                        horse->changeOriginalDemo();
                        horse->changeDemoMode(6, 0);
                        horse->changeDemoMoveAngle(mDemo.getMoveAngle());
                        horse->setDemoStickR(demo_stick);
                    }
                }
                return 1;
            }
        }
    }

    return 0;
}

void daAlink_c::voiceStart(u32 i_soundID) {
    mZ2Link.startLinkVoice(i_soundID, mVoiceReverbIntensity);
}

void daAlink_c::voiceStartLevel(u32 i_soundID) {
    mZ2Link.startLinkVoiceLevel(i_soundID, mVoiceReverbIntensity);
}

void daAlink_c::seStartSwordCut(u32 i_soundID) {
    mZ2Link.startLinkSwordSound(i_soundID, 0, mVoiceReverbIntensity);
}

void daAlink_c::seStartOnlyReverb(u32 i_soundID) {
    mZ2Link.startLinkSound(i_soundID, 0, mVoiceReverbIntensity);
}

void daAlink_c::seStartOnlyReverbLevel(u32 i_soundID) {
    mZ2Link.startLinkSoundLevel(i_soundID, 0, mVoiceReverbIntensity);
}

void daAlink_c::seStartMapInfo(u32 i_soundID) {
    mZ2Link.startLinkSound(i_soundID, mPolySound, mVoiceReverbIntensity);
}

void daAlink_c::seStartMapInfoLevel(u32 i_soundID) {
    mZ2Link.startLinkSoundLevel(i_soundID, mPolySound, mVoiceReverbIntensity);
}

void daAlink_c::setBasAnime(daAlink_c::daAlink_UNDER i_underIdx) {
    daPy_anmHeap_c* anmHeap = &mUnderAnmHeap[i_underIdx];
    daPy_frameCtrl_c* framectrl = &mUnderFrameCtrl[i_underIdx];

    if (!anmHeap->checkNoSetArcNo()) {
        void* bas = ((mDoExt_transAnmBas*)getNowAnmPackUnder(UNDER_0))->getBas();
        if (bas == NULL) {
            resetBasAnime();
        } else {
            if (field_0x3084 == anmHeap->getIdx() && field_0x3086 == anmHeap->getArcNo()) {
                if (field_0x33d4 * framectrl->getRate() >= 0.0f) {
                    field_0x2d7c = framectrl;
                    return;
                }
            }

            field_0x2d80 = bas;
            field_0x2d7c = framectrl;
            field_0x3084 = anmHeap->getIdx();
            field_0x3086 = anmHeap->getArcNo();
            field_0x33d4 = framectrl->getRate();
            initBasAnime();
        }
    } else {
        u8* temp_r3_2 = anmHeap->getBuffer();
        if (*(u32*)(temp_r3_2 + 0x1C) == 0xFFFFFFFF) {
            resetBasAnime();
            return;
        }

        if (field_0x3084 == anmHeap->getIdx() && field_0x3086 == anmHeap->getArcNo()) {
            if (field_0x33d4 * framectrl->getRate() >= 0.0f) {
                field_0x2d7c = framectrl;
                return;
            }
        }

        u32 dataSize = *(u32*)(temp_r3_2 + 0x8) - *(u32*)(temp_r3_2 + 0x1C);

        const u32 l_basAnmBufferSize = 0x800;
        JUT_ASSERT(20661, dataSize < l_basAnmBufferSize);

        cLib_memCpy(field_0x2d78, anmHeap->getBuffer() + *(u32*)(temp_r3_2 + 0x1C), dataSize);
        field_0x2d7c = framectrl;
        field_0x3084 = anmHeap->getIdx();
        field_0x3086 = anmHeap->getArcNo();
        field_0x33d4 = framectrl->getRate();
        field_0x2d80 = 0;
        initBasAnime();
    }
}

void daAlink_c::initBasAnime() {
    if (field_0x2d7c != NULL) {
        f32 var_f31;
        if (field_0x2d7c->getAttribute() == J3DFrameCtrl::EMode_LOOP) {
            var_f31 = field_0x2d7c->getFrame() - field_0x2d7c->getRate();
            if (var_f31 <= field_0x2d7c->getStart()) {
                var_f31 = field_0x2d7c->getEnd() - var_f31;
            } else if (var_f31 >= field_0x2d7c->getEnd()) {
                var_f31 -= field_0x2d7c->getEnd();
            }
        } else {
            var_f31 = 0.0f;
        }

        if (field_0x2d80 != NULL) {
            mZ2Link.initAnime(field_0x2d80, field_0x2d7c->getRate() >= 0.0f, field_0x2d7c->getLoop(), var_f31);
        } else {
            mZ2Link.initAnime(field_0x2d78, field_0x2d7c->getRate() >= 0.0f, field_0x2d7c->getLoop(), var_f31);
        }
    }
}

void daAlink_c::resetBasAnime() {
    field_0x3084 = 0xFFFF;
    field_0x3086 = 0xFFFF;
    field_0x2d7c = NULL;
    field_0x2d80 = NULL;
}

BOOL daAlink_c::checkSightLine(f32 i_maxDist, cXyz* o_sightPos) {
    camera_class* camera = dComIfGp_getCamera(field_0x317c);
    cXyz* line_start_pos = fopCamM_GetEye_p(camera);
    cXyz sp3C;
    cXyz sp30(mHeldItemRootPos);

    if (checkHookshotItem(mEquipItem) || mEquipItem == fpcNm_ITEM_PACHINKO) {
        line_start_pos = &sp30;
    } else if (checkBowItem(mEquipItem) && mItemAcKeep.getActor() != NULL) {
        line_start_pos = &mItemAcKeep.getActor()->current.pos;
    }

    s16 temp_r27 = shape_angle.y + mBodyAngle.y;
    f32 temp_f31 = cM_scos(mBodyAngle.x);
    sp3C.set(temp_f31 * cM_ssin(temp_r27), -cM_ssin(mBodyAngle.x), temp_f31 * cM_scos(temp_r27));

    if (checkMagneBootsOn()) {
        mDoMtx_multVecSR(mMagneBootMtx, &sp3C, &sp3C);
    }

    *o_sightPos = (sp3C * i_maxDist) + *line_start_pos;

    if (checkHookshotItem(mEquipItem)) {
        mRopeLinChk.Set(&field_0x3834, &mHeldItemRootPos, this);

        if (dComIfG_Bgsp().LineCross(&mRopeLinChk)) {
            *o_sightPos = mRopeLinChk.GetCross();
            onResetFlg0(RFLG0_ITEM_SIGHT_BG_HIT);
            return 1;
        }

        sp30 -= sp3C * 100.0f;
    } else if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        return 0;
    }

    dBgS_LinChk* linechk;
    if (mEquipItem == fpcNm_ITEM_BOOMERANG) {
        linechk = &mBoomerangLinChk;
    } else if (mProcID == PROC_HAWK_SUBJECT) {
        linechk = &mLinkLinChk;
    } else if (checkBowAndSlingItem(mEquipItem)) {
        linechk = &mArrowLinChk;
    } else {
        linechk = &mRopeLinChk;
    }

    linechk->Set(line_start_pos, o_sightPos, this);

    BOOL isLineCross = dComIfG_Bgsp().LineCross(linechk);
    if (isLineCross) {
        *o_sightPos = linechk->GetCross();
        onResetFlg0(RFLG0_ITEM_SIGHT_BG_HIT);
    }

    return isLineCross;
}

void daAlink_c::setMetamorphoseModel(BOOL i_isChangeToWolf) {
    J3DAnmTransform* bck = (J3DAnmTransform*)mAnmHeap9.loadDataIdx(i_isChangeToWolf ? (u16)dRes_ID_ALANM_BCK_WFCHANGEATOW_e : (u16)dRes_ID_ALANM_BCK_WFCHANGEWTOA_e);
    JKRHeap* heap = setItemHeap();

    mHeldItemModel = initModel(loadAramBmd(dRes_ID_ALANM_BMD_AL_WF_e, 0x6000), 0);
    
    if (!mItemBck.init(bck, FALSE, 2, 1.0f, 0, -1, false)) {
        JUT_ASSERT(20842, FALSE);
    }

    mDoExt_setCurrentHeap(heap);
    mEquipItem = 0x106;
    field_0x33dc = mUnderFrameCtrl[0].getFrame();
}

void daAlink_c::keepItemData() {
    mKeepItem = mEquipItem;
    deleteEquipItem(FALSE, FALSE);
}

void daAlink_c::returnKeepItemData() {
    deleteEquipItem(FALSE, FALSE);
    mEquipItem = mKeepItem;
    mKeepItem = fpcNm_ITEM_NONE;
    makeItemType();
}

BOOL daAlink_c::setItemModel() {
    if (checkBowItem(mEquipItem)) {
        setBowModel();
        return 1;
    }

    if (mEquipItem == fpcNm_ITEM_PACHINKO) {
        setSlingModel();
        return 1;
    }

    if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        setCopyRodModel();
        if (mCopyRodAcKeep.getActor() == NULL) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    if (mEquipItem == fpcNm_ITEM_KANTERA) {
        setKandelaarModel();
        return 1;
    }

    if (mEquipItem == fpcNm_ITEM_EMPTY_BOTTLE) {
        setBottleModel(mEquipItem);
        return 1;
    }

    if (checkHookshotItem(mEquipItem)) {
        setHookshotModel();
        return 1;
    }

    if (mEquipItem == 0x103) {
        setSwordModel();
        return 1;
    }

    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        setIronBallModel();
        setIronBallWaitUpperAnime(1);
        if (mItemAcKeep.getActor() == NULL) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    return 0;
}

BOOL daAlink_c::setItemActor() {
    fopAc_ac_c* actor;
    if (mEquipItem == fpcNm_ITEM_BOOMERANG) {
        actor = (fopAc_ac_c*)fopAcM_fastCreate(PROC_BOOMERANG, 0, &current.pos, -1,
                                               NULL, NULL, -1, NULL, NULL);
        if (actor == NULL) {
            deleteEquipItem(FALSE, FALSE);
            return 0;
        }

        mItemAcKeep.setData(actor);
        field_0x2f94 = 2;
        return 1;
    }

    if (mEquipItem == fpcNm_ITEM_COPY_ROD) {
        actor = (fopAc_ac_c*)fopAcM_fastCreate(PROC_CROD, 0, &current.pos, -1, NULL,
                                               NULL, -1, NULL, NULL);
        if (actor == NULL) {
            deleteEquipItem(FALSE, FALSE);
            return 0;
        }

        mItemAcKeep.setData(actor);
        field_0x2f94 = 2;
        return 1;
    }

    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        actor = daCrod_c::makeIronBallDummy(this);

        if (actor == NULL) {
            if (checkIronBallWaitAnime()) {
                resetUpperAnime(UPPER_2, -1.0f);
            }

            deleteEquipItem(FALSE, FALSE);
            return 0;
        }

        mItemAcKeep.setData(actor);
        field_0x173c.SetActor(actor);
        return 1;
    }

    if (checkFisingRodLure()) {
        mItemAcKeep.setID(fopAcM_create(PROC_MG_ROD, 0x10D, &mLeftHandPos, -1, NULL, NULL, 0xFF));
        initFishingRodHand();
        return 1;
    }

    if (checkFishingRodItem(mEquipItem)) {
        setGroundFishingRodActor();
        return 1;
    }

    if (checkBombItem(mEquipItem)) {
        if (checkHorseRide()) {
            if (dComIfGp_getHorseActor()->checkNoBombProc()
                #if PLATFORM_GCN
                && (mProcID != PROC_HORSE_TURN || !checkModeFlg(MODE_DISABLE_ITEMS))
                #endif
                )
            {
                mEquipItem = fpcNm_ITEM_NONE;
                return 1;
            }
        }

        cXyz create_pos = (mLeftHandPos + mRightHandPos) * 0.5f;
        if (checkReadyItem()) {
            fopAc_ac_c* actor;
            if (mEquipItem == fpcNm_ITEM_NORMAL_BOMB) {
                actor = dBomb_c::createNormalBombPlayer(&create_pos);
            } else {
                actor = dBomb_c::createWaterBombPlayer(&create_pos);
            }

            if (actor != NULL) {
                mActiveBombNum++;
                setGrabItemActor(actor);
                dComIfGp_addSelectItemNum(mSelectItemId, -1);
                field_0x33e4 = 38.0f;
                setGrabUpperAnime(mpHIO->mBasic.m.mBasicInterpolation);
            }
        }
        mEquipItem = fpcNm_ITEM_NONE;
        return 1;
    }

    return 0;
}

void daAlink_c::makeItemType() {
    if (!setItemModel()) {
        setItemActor();
    }
}

BOOL daAlink_c::checkZoraWearAbility() const {
    return (!checkWolf() && checkZoraWearFlg()) && !checkNoResetFlg2(FLG2_UNK_80000);
}

BOOL daAlink_c::checkMagicArmorWearAbility() const {
    return (!checkWolf() && checkMagicArmorWearFlg()) && !checkNoResetFlg2(FLG2_UNK_80000);
}

J3DModelData* daAlink_c::loadAramBmd(u16 i_resIdx, u32 i_bufSize) {
    JKRArchive* anmArchive = dComIfGp_getAnmArchive();
    u8* tmpBuffer = new (0x20) u8[i_bufSize];

    JKRReadIdxResource(tmpBuffer, i_bufSize, i_resIdx, anmArchive);
    #if DEBUG
    daPy_aramBufferCheck(tmpBuffer, i_bufSize);
    #endif

    u32 type = 'BMWR';
    JKRArchive::SDIDirEntry* dir = anmArchive->mNodes;
    for (int i = 0; i < anmArchive->countDirectory(); i++, dir++) {
        if (i_resIdx >= dir->first_file_index && i_resIdx < dir->first_file_index + dir->num_entries)
        {
            type = dir->type;
            break;
        }
    }

    return dRes_info_c::loaderBasicBmd(type, tmpBuffer);
}

void* daAlink_c::loadAram(u16 i_resIdx, u32 i_bufSize) {
    u8* tmpBuffer = new (0x20) u8[i_bufSize];
    JKRReadIdxResource(tmpBuffer, i_bufSize, i_resIdx, dComIfGp_getAnmArchive());
    #if DEBUG
    daPy_aramBufferCheck(tmpBuffer, i_bufSize);
    #endif

    return J3DAnmLoaderDataBase::load(tmpBuffer);
}

J3DAnmTevRegKey* daAlink_c::loadAramItemBrk(u16 i_resIdx, J3DModel* i_model) {
    J3DModelData* model_data = i_model->getModelData();
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)loadAram(i_resIdx, 0x400);

    brk->setFrame(0.0f);
    brk->searchUpdateMaterialID(model_data);
    model_data->entryTevRegAnimator(brk);

    return brk;
}

J3DAnmTextureSRTKey* daAlink_c::loadAramItemBtk(u16 i_resIdx, J3DModel* i_model) {
    J3DModelData* model_data = i_model->getModelData();
    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)loadAram(i_resIdx, 0x400);

    btk->setFrame(0.0f);
    btk->searchUpdateMaterialID(model_data);
    model_data->entryTexMtxAnimator(btk);

    return btk;
}

J3DAnmTexPattern* daAlink_c::loadAramItemBtp(u16 i_resIdx, J3DModel* i_model) {
    J3DModelData* model_data = i_model->getModelData();
    J3DAnmTexPattern* btp = (J3DAnmTexPattern*)loadAram(i_resIdx, 0x400);

    btp->setFrame(0.0f);
    btp->searchUpdateMaterialID(model_data);
    model_data->entryTexNoAnimator(btp);

    return btp;
}

void daAlink_c::changeItemBck(u16 i_resIdx, f32 param_1) {
    J3DAnmTransform* bck = (J3DAnmTransform*)mAnmHeap9.loadDataIdx(i_resIdx);
    if (bck != NULL) {
        mItemBck.changeBckOnly(bck);
    }

    if (param_1 < 0.0f) {
        field_0x33dc = mItemBck.getBckAnm()->getFrameMax();
    } else {
        field_0x33dc = param_1;
    }
}

BOOL daAlink_c::checkGroupItem(int i_itemNo, int i_selItem) const {
    if (i_itemNo == 0x107) {
        return checkDrinkBottleItem(i_selItem);
    } else if (i_itemNo == 0x108) {
        return checkFishingRodItem(i_selItem);
    } else {
        return i_itemNo == i_selItem;
    }
}

int daAlink_c::checkSetItemTrigger(int i_itemNo) {
    for (u8 i = 0; i < 2; i++) {
        if (checkGroupItem(i_itemNo, dComIfGp_getSelectItem(i)) && itemTriggerCheck(1 << i)) {
            if (i_itemNo != fpcNm_ITEM_HVY_BOOTS) {
                mSelectItemId = i;
            }
            return 1;
        }
    }

    return 0;
}

int daAlink_c::checkItemSetButton(int i_itemNo) {
    for (u8 i = 0; i < 2; i++) {
        if (checkGroupItem(i_itemNo, dComIfGp_getSelectItem(i))) {
            return i;
        }
    }

    return 2;
}

bool daAlink_c::checkField() {
    return dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_FIELD;
}

bool daAlink_c::checkBossRoom() {
    return dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_BOSS_ROOM;
}

bool daAlink_c::checkDungeon() {
    return dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_DUNGEON;
}

bool daAlink_c::checkCastleTown() {
    return dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_CASTLE_TOWN;
}

bool daAlink_c::checkCloudSea() {
    return dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_CLOUD_SEA;
}

bool daAlink_c::checkRoomOnly() {
    return dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_ROOM ||
           checkLv7DungeonShop();
}

bool daAlink_c::checkLv2DungeonRoomSpecial() {
    return checkStageName("D_MN04") &&
           (dComIfGp_roomControl_getStayNo() == 14 || dComIfGp_roomControl_getStayNo() == 16 ||
            dComIfGp_roomControl_getStayNo() == 17);
}

bool daAlink_c::checkRoomSpecial() {
    return (checkStageName("D_MN11") &&
            (dComIfGp_roomControl_getStayNo() == 1 || dComIfGp_roomControl_getStayNo() == 2)) ||
           checkLv2DungeonRoomSpecial();
}

bool daAlink_c::checkRoom() {
    return checkRoomOnly() || checkRoomSpecial() ||
           (checkStageName("R_SP161") && !dComIfGs_isOneZoneSwitch(14, -1));
}

bool daAlink_c::checkNotBattleStage() {
    return checkRoom() || checkCastleTown();
}

bool daAlink_c::checkNotHeavyBootsStage() {
    return (checkNotBattleStage() || checkCloudSea()) && !checkLv2DungeonRoomSpecial();
}

bool daAlink_c::checkNotAutoJumpStage() {
    return checkRoomOnly() || checkCastleTown();
}

bool daAlink_c::checkCastleTownUseItem(u16 i_itemNo) {
    if (checkNotBattleStage()) {
        if (i_itemNo == fpcNm_ITEM_KANTERA
            || checkTradeItem(i_itemNo)
            || (i_itemNo == fpcNm_ITEM_DUNGEON_BACK && checkLv7DungeonShop())
            || (checkRoomSpecial() && (i_itemNo == fpcNm_ITEM_EMPTY_BOTTLE || checkDungeonWarpItem(i_itemNo)))
            || (checkStageName("R_SP128") && i_itemNo == fpcNm_ITEM_COPY_ROD)
            || (checkLv2DungeonRoomSpecial() && i_itemNo == fpcNm_ITEM_HVY_BOOTS)
            || (checkBottleItem(i_itemNo) && i_itemNo != fpcNm_ITEM_EMPTY_BOTTLE))
        {
            return true;
        } else {
            return false;
        }
    }

    return true;
}

enum daAlink_ItemProc {
    /*  0 */ ITEM_PROC_NONE,
    /*  1 */ ITEM_PROC_BOOTS_EQUIP,
    /*  2 */ ITEM_PROC_SET_HVYBOOTS,
    /*  3 */ ITEM_PROC_BOTTLE_DRINK,
    /*  4 */ ITEM_PROC_SPINNER_READY,
    /*  5 */ ITEM_PROC_DUNGEON_WARP_READY,
    /*  6 */ ITEM_PROC_BOTTLE_OPEN,
    /*  7 */ ITEM_PROC_FISHING_FOOD,
    /*  8 */ ITEM_PROC_KANDELAAR_POUR,
    /*  9 */ ITEM_PROC_SUBJECTIVITY,
    /* 10 */ ITEM_PROC_PICK_PUT,
    /* 11 */ ITEM_PROC_OFF_KANDELAAR,
    /* 12 */ ITEM_PROC_COMMON_CHANGE_ITEM,
    /* 13 */ ITEM_PROC_BOTTLE_SWING,
    /* 14 */ ITEM_PROC_NOT_USE_ITEM,
    /* 15 */ ITEM_PROC_GRASS_WHISTLE,
};

int daAlink_c::changeItemTriggerKeepProc(u8 i_selItemIdx, int i_procType) {
    u32 sel_item = dComIfGp_getSelectItem(i_selItemIdx);
    mSelectItemId = i_selItemIdx;

    if (i_procType == ITEM_PROC_GRASS_WHISTLE) {
        procGrassWhistleWaitInit(3, -1, 0, 0, NULL);
    } else if (i_procType == ITEM_PROC_BOTTLE_DRINK) {
        if (checkReinRide()) {
            procHorseBottleDrinkInit(sel_item);
        } else if (checkCanoeRide()) {
            procCanoeBottleDrinkInit(sel_item);
        } else {
            procBottleDrinkInit(sel_item);
        }
    } else if (i_procType == ITEM_PROC_KANDELAAR_POUR) {
        if (checkReinRide()) {
            procHorseKandelaarPourInit();
        } else if (checkCanoeRide()) {
            procCanoeKandelaarPourInit();
        } else {
            procKandelaarPourInit();
        }
    } else if (i_procType == ITEM_PROC_FISHING_FOOD) {
        procFishingFoodInit();
    } else if (i_procType == ITEM_PROC_BOOTS_EQUIP) {
        procBootsEquipInit();
    } else if (i_procType == ITEM_PROC_SET_HVYBOOTS) {
        setHeavyBoots(1);
    } else if (i_procType == ITEM_PROC_SPINNER_READY) {
        procSpinnerReadyInit();
    } else if (i_procType == ITEM_PROC_DUNGEON_WARP_READY) {
        procDungeonWarpReadyInit();
    } else if (i_procType == ITEM_PROC_BOTTLE_OPEN) {
        procBottleOpenInit(sel_item);
    } else if (i_procType == ITEM_PROC_BOTTLE_SWING) {
        procBottleSwingInit(NULL, 0);
    } else if (i_procType == ITEM_PROC_NOT_USE_ITEM) {
        procNotUseItemInit((u16)sel_item);
    } else if (i_procType == ITEM_PROC_SUBJECTIVITY) {
        procCoSubjectivityInit();
        dComIfGp_setPlayerStatus0(0, 0x200000);
        seStartSystem(Z2SE_AL_HAWK_EYE_PUTON);
    } else if (i_procType == ITEM_PROC_PICK_PUT) {
        procPickPutInit(1);
    } else if (i_procType == ITEM_PROC_OFF_KANDELAAR) {
        offKandelaarModel();
    } else if (i_procType == ITEM_PROC_COMMON_CHANGE_ITEM) {
        field_0x2fde = fpcNm_ITEM_NONE;
        itemEquip(sel_item);

        if (dComIfGp_checkPlayerStatus0(0, 0x2000) &&
            ((checkBowAndSlingItem(field_0x2fde) || checkHookshotItem(field_0x2fde) ||
              field_0x2fde == fpcNm_ITEM_COPY_ROD) ||
             field_0x2fde == fpcNm_ITEM_BOOMERANG))
        {
            commonChangeItem();
            resetUpperAnime(UPPER_2, -1.0f);
            checkItemActionInitStart();
        }
    }

    return 1;
}

/**
 * Determines whether an item action can occur, and which action to use.
 *
 * @return a `daAlink_ItemProc` value corresponding to the `PROC` function to run in
 * `changeItemTriggerKeepProc`
 */
int daAlink_c::checkNewItemChange(u8 i_selItemIdx) {
    u16 sel_item = dComIfGp_getSelectItem(i_selItemIdx);

    if (checkSpinnerRide()
        || sel_item == fpcNm_ITEM_BOMB_BAG_LV1
        || ((sel_item == fpcNm_ITEM_KANTERA || checkOilBottleItem(sel_item)) && checkWaterInKandelaarOffset(mWaterY))
        || (checkCanoeRide() && checkStageName("F_SP127"))
        || checkCloudSea()
        || ((checkModeFlg(0x40000) || checkNoResetFlg0(FLG0_WATER_IN_MOVE)) && !checkAcceptUseItemInWater(sel_item))
        || (checkModeFlg(0x40000) && sel_item == fpcNm_ITEM_WATER_BOMB)
        || !checkCastleTownUseItem(sel_item)
        || (checkBoardRide() && sel_item != 0x103)
        || (checkModeFlg(0x400) && (sel_item == fpcNm_ITEM_EMPTY_BOTTLE || sel_item == fpcNm_ITEM_POKE_BOMB || sel_item == fpcNm_ITEM_IRONBALL || sel_item == fpcNm_ITEM_COPY_ROD || checkFishingRodItem(sel_item)))
        || ((mGndPolySpecialCode == dBgW_SPCODE_HEAVY_SNOW || mGndPolyAtt1 == 1 || mGndPolyAtt1 == 2 || mWaterY - current.pos.y > (daSpinner_c::getWaterSinkLimit() - 5.0f) || (field_0x2fbc == 6 && mWaterY - current.pos.y >= 0.0f) || mGndPolyAtt1 == 3) && sel_item == fpcNm_ITEM_SPINNER)
        || (checkBossRoom() && checkDungeonWarpItem(sel_item))
        || (sel_item == fpcNm_ITEM_DUNGEON_EXIT && (checkLv7DungeonShop() || (checkStageName("D_MN07") && fopAcM_isSwitch(this, 0x4D) && !fopAcM_isSwitch(this, 0x18)) || (checkStageName("D_MN10") && fopAcM_GetRoomNo(this) == 15)))
        || (checkMagneBootsOn() && sel_item != 0x103 && !checkDrinkBottleItem(sel_item) && sel_item != fpcNm_ITEM_HVY_BOOTS && !checkBowItem(sel_item))
        )
    {
        return ITEM_PROC_NONE;
    } else if (sel_item == fpcNm_ITEM_HVY_BOOTS
                || checkDungeonWarpItem(sel_item)
                || checkTradeItem(sel_item)
                || (checkBottleItem(sel_item) && sel_item != fpcNm_ITEM_EMPTY_BOTTLE)
                || sel_item == fpcNm_ITEM_SPINNER
                || sel_item == fpcNm_ITEM_POKE_BOMB
                || sel_item == fpcNm_ITEM_HORSE_FLUTE
                || sel_item == fpcNm_ITEM_HAWK_EYE
            )
    {
        if (checkReinRide() || checkCanoeRide()) {
            if (checkDrinkBottleItem(sel_item)) {
                return ITEM_PROC_BOTTLE_DRINK;
            }

            if (checkOilBottleItem(sel_item) && checkItemSetButton(fpcNm_ITEM_KANTERA) != 2) {
                return ITEM_PROC_KANDELAAR_POUR;
            }
        } else if (sel_item == fpcNm_ITEM_HVY_BOOTS) {
            if (!checkBoardRide()) {
                if ((mLinkAcch.ChkGroundHit() && !checkModeFlg(0x70C52)) ||
                    (checkMagneBootsOn() && cBgW_CheckBGround(mMagneBootsTopVec.y)) ||
                    mProcID == PROC_HANG_CLIMB)
                {
                    return ITEM_PROC_BOOTS_EQUIP;
                }
                return ITEM_PROC_SET_HVYBOOTS;
            }
        } else if (checkDrinkBottleItem(sel_item) && checkMagneBootsOn()) {
            if (cBgW_CheckBGround(mMagneBootsTopVec.y)) {
                return ITEM_PROC_BOTTLE_DRINK;
            }
        } else if (mLinkAcch.ChkGroundHit()) {
            if (!checkModeFlg(0x70C52)) {
                if (sel_item == fpcNm_ITEM_SPINNER) {
                    cXyz sp38(current.pos.x, current.pos.y + l_autoUpHeight, current.pos.z);
                    cXyz sp2C(sp38);
                    cXyz sp20;

                    s16 var_r30 = 0;
                    for (int i = 0; i < 4; i++, var_r30 += 0x2000) {
                        sp2C.x = sp38.x + cM_ssin(var_r30) * 120.0f;
                        sp2C.z = sp38.z + cM_scos(var_r30) * 120.0f;

                        if (commonLineCheck(&sp38, &sp2C)) {
                            sp2C.x = (sp38.x * 2.0f) - sp2C.x;
                            sp2C.z = (sp38.z * 2.0f) - sp2C.z;
                            sp20 = mLinkLinChk.GetCross();

                            if (commonLineCheck(&sp38, &sp2C)) {
                                if (mLinkLinChk.GetCross().abs2XZ(sp20) < 14400.0f) {
                                    return ITEM_PROC_NONE;
                                }
                            }
                        }
                    }

                    return ITEM_PROC_SPINNER_READY;
                } else if (checkDungeonWarpItem(sel_item)) {
                    return ITEM_PROC_DUNGEON_WARP_READY;
                } else if (checkItemSetButton(0x108) != 2 &&
                           (sel_item == fpcNm_ITEM_WORM || sel_item == fpcNm_ITEM_BEE_CHILD))
                {
                    int itemNo = dComIfGp_getSelectItem(checkItemSetButton(0x108));
                    if (itemNo == fpcNm_ITEM_WORM_ROD || itemNo == fpcNm_ITEM_JEWEL_WORM_ROD) {
                        if (sel_item == fpcNm_ITEM_BEE_CHILD) {
                            return ITEM_PROC_BOTTLE_DRINK;
                        }
                        return ITEM_PROC_NONE;
                    }
                    if (sel_item == fpcNm_ITEM_BEE_CHILD &&
                        (itemNo == fpcNm_ITEM_BEE_ROD || itemNo == fpcNm_ITEM_JEWEL_BEE_ROD))
                    {
                        return ITEM_PROC_BOTTLE_DRINK;
                    }
                    return ITEM_PROC_FISHING_FOOD;
                } else if (checkDrinkBottleItem(sel_item)) {
                    return ITEM_PROC_BOTTLE_DRINK;
                } else if (checkOpenBottleItem(sel_item)) {
                    return ITEM_PROC_BOTTLE_OPEN;
                } else if (checkTradeItem(sel_item)) {
                    return ITEM_PROC_NOT_USE_ITEM;
                } else if (sel_item == fpcNm_ITEM_HORSE_FLUTE) {
                    return ITEM_PROC_GRASS_WHISTLE;
                } else if (checkOilBottleItem(sel_item) && checkItemSetButton(0x48) != 2) {
                    return ITEM_PROC_KANDELAAR_POUR;
                } else if (sel_item == fpcNm_ITEM_HAWK_EYE) {
                    if (acceptSubjectModeChange()) {
                        return ITEM_PROC_SUBJECTIVITY;
                    }
                } else if (sel_item == fpcNm_ITEM_POKE_BOMB && dComIfGp_getSelectItemNum(i_selItemIdx) &&
                           field_0x2fcf < 2)
                {
                    return ITEM_PROC_PICK_PUT;
                }
            }
        }
    } else if (sel_item != fpcNm_ITEM_NONE && mEquipItem != sel_item) {
        if ((checkBombItem(sel_item) && !dComIfGp_getSelectItemNum(i_selItemIdx))
            || ((sel_item == fpcNm_ITEM_NORMAL_BOMB || sel_item == fpcNm_ITEM_WATER_BOMB) && mActiveBombNum >= 3)
            || (sel_item == fpcNm_ITEM_IRONBALL && (!mLinkAcch.ChkGroundHit() || checkModeFlg(0x70C52)))
            || (sel_item == fpcNm_ITEM_KANTERA && (checkNoResetFlg0(FLG0_WATER_IN_MOVE) || checkEndResetFlg1(ERFLG1_UNK_4) || checkModeFlg(0x40000))))
        {
            return ITEM_PROC_NONE;
        }
        return ITEM_PROC_COMMON_CHANGE_ITEM;
    }

    if (mEquipItem == sel_item && mSelectItemId != i_selItemIdx && mEquipItem == fpcNm_ITEM_EMPTY_BOTTLE) {
        return ITEM_PROC_BOTTLE_SWING;
    }

    return ITEM_PROC_NONE;
}

void daAlink_c::deleteEquipItem(BOOL i_isPlaySound, BOOL i_isDeleteKantera) {
    if (i_isDeleteKantera || mProcID == PROC_UNEQUIP) {
        offKandelaarModel();
    } else if (mEquipItem == fpcNm_ITEM_KANTERA && checkNoResetFlg2(FLG2_UNK_1)) {
        mZ2Link.setKanteraState(2);
    }

    if (mEquipItem == fpcNm_ITEM_NONE || mEquipItem == 0x10B) {
        return;
    }

    fopAc_ac_c* item_actor = mItemAcKeep.getActor();

    if (i_isPlaySound) {
        if (mEquipItem == 0x103) {
            if (checkWoodSwordEquip()) {
                seStartOnlyReverb(Z2SE_AL_ITEM_TAKEOUT_FAST);
            } else {
                seStartOnlyReverb(Z2SE_AL_SWORD_PUTIN);
            }
        } else if (mEquipItem < fpcNm_ITEM_NONE) {
            seStartOnlyReverb(Z2SE_AL_ITEM_TAKEOUT);
        }
    }

    if (mEquipItem == 0x103) {
        offSwordModel();
        stickArrowIncrement(1);
    } else if (mEquipItem == 0x102) {
        if (item_actor != NULL) {
            fopAcM_cancelCarryNow(item_actor);

            if (fopAcM_lc_c::lineCheck(&field_0x34e0, &item_actor->current.pos, this)) {
                cM3dGPla tri;
                fopAcM_lc_c::getTriPla(&tri);

                item_actor->current.pos = *fopAcM_lc_c::getCrossP() + (tri.mNormal * 5.0f);
            }
        }
    } else if (item_actor != NULL) {
        fopAcM_delete(item_actor);
    }

    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        mZ2Link.setUsingIronBall(false);
    } else if (checkHookshotItem(mEquipItem)) {
        cancelHookshotCarry();
    }

    if (((mEquipItem == fpcNm_ITEM_BOOMERANG || mEquipItem == 0x102) && checkBoomerangAnime()) ||
         (mEquipItem == fpcNm_ITEM_COPY_ROD && checkCopyRodAnime()) ||
         (checkHookshotItem(mEquipItem) && checkHookshotAnime()) ||
         (mEquipItem == fpcNm_ITEM_IRONBALL && (checkIronBallAnime() || checkIronBallWaitAnime())) ||
         (checkBowAndSlingItem(mEquipItem) && checkBowAnime()))
    {
        resetUpperAnime(UPPER_2, -1.0f);
    }

    if (mCopyRodAcKeep.getActor() != NULL) {
        ((daCrod_c*)mCopyRodAcKeep.getActor())->offControll();
    }

    mItemAcKeep.clearData();
    mEquipItem = fpcNm_ITEM_NONE;
    mHeldItemModel = NULL;
    mpHookTipModel = NULL;
    field_0x0710 = NULL;
    field_0x0714 = NULL;

    if (mItemBck.getBckAnm() != NULL) {
        mItemBck.changeBckOnly(NULL);
    }

    if (mHookTipBck.getBckAnm() != NULL) {
        mHookTipBck.changeBckOnly(NULL);
    }

    mAnmHeap9.resetIdx();

    field_0x0718 = NULL;
    field_0x071c = NULL;
    field_0x0720 = NULL;
    field_0x0724 = NULL;
    field_0x0728 = NULL;
    field_0x072c = NULL;
    mpItemModelData = NULL;
    mpHookChain = NULL;
    mIronBallChainPos = NULL;
    mIronBallChainAngle = NULL;
    field_0x3848 = NULL;
    field_0x0774 = NULL;
    field_0x0778 = NULL;
    mpHookshotLinChk = NULL;
    field_0x780 = NULL;

    mAtCps[0].SetAtType(getSwordAtType());
    mAtCps[0].SetAtHitCallback(NULL);
    mAtCps[0].OffAtSetBit();
    cancelLockAt();
    mAtCps[0].SetAtMtrl(dCcD_MTRL_NONE);

    field_0x173c.SetActor(this);

    field_0x2f1c = NULL;
    field_0x2f94 = 0xFF;
    field_0x2f95 = 0xFF;

    offNoResetFlg2(FLG2_FISHING_CAST_WAIT);
    offNoResetFlg1(FLG1_FISHING_ROD_CASTING_END);

    if (mpHookSound != NULL) {
        mpHookSound->deleteObject();
        mpHookSound = NULL;
    }
}

void daAlink_c::setLight() {
    const daAlinkHIO_huLight_c1* light_m = &mpHIO->mItem.mLanternPL.m;
    BOOL var_r28 = false;

    if (checkWolf()) {
        offNoResetFlg1(FLG1_UNK_80);
    } else {
        if (checkNoResetFlg2(FLG2_UNK_1) || checkEndResetFlg1(ERFLG1_UNK_4)) {
            if (dComIfGs_getOil() != 0 && !checkNoResetFlg2(FLG2_KANDELAAR_LIGHT_OFF) && ((checkNoResetFlg2(FLG2_UNK_1) && !checkFreezeDamage()) || checkEndResetFlg1(ERFLG1_UNK_10))) {
                onNoResetFlg1(FLG1_UNK_80);

                if (!checkEventRun() && !checkEndResetFlg1(ERFLG1_UNK_4)) {
                    dComIfGp_setItemOilCount(-mpHIO->mItem.mLantern.m.mNormalOilLoss);
                }

                u16 effName;
                dPa_levelEcallBack* callbackp;
                if (checkKandelaarSwingAnime() || mProcID == PROC_KANDELAAR_SWING) {
                    effName = ID_ZI_J_KANTERA_SWINGFIRE;
                    callbackp = &field_0x2f20;

                    JPABaseEmitter* emitterp = dComIfGp_particle_getEmitter(field_0x31c4);
                    if (emitterp != NULL && emitterp->getEmitterCallBackPtr() == NULL) {
                        emitterp->stopDrawParticle();
                    }
                } else {
                    effName = ID_ZI_J_KANTERA_FIRE;
                    callbackp = NULL;
                }

                field_0x31c4 = dComIfGp_particle_set(field_0x31c4, effName, &mKandelaarFlamePos, &tevStr, &shape_angle, NULL, 0xFF, callbackp, -1, NULL, NULL, NULL);
                mZ2Link.getKantera().startLevelSound(Z2SE_AL_KANTERA_BURNING, 0, mVoiceReverbIntensity);
            } else if (checkNoResetFlg1(FLG1_UNK_80)) {
                mZ2Link.getKantera().startSound(Z2SE_AL_KANTERA_OFF, 0, mVoiceReverbIntensity);
                offNoResetFlg1(FLG1_UNK_80);
                stopDrawParticle(field_0x31c4);
            }

            f32 scale_targ;
            if (field_0x344c > (f32)field_0x32c8 || !checkNoResetFlg1(FLG1_UNK_80)) {
                scale_targ = 0.0f;
            } else {
                scale_targ = 1.0f;
            }

            cLib_addCalc(&field_0x3448, scale_targ, 0.5f, 0.3f, 0.1f);

            Vec glow_scale = {field_0x3448, field_0x3448, field_0x3448};
            mpKanteraGlowModel->setBaseScale(glow_scale);
        } else {
            offNoResetFlg1(FLG1_UNK_80);
            stopDrawParticle(field_0x31c4);
        }

        if (mEquipItem == 0x103 && checkNoResetFlg3(FLG3_UNK_100000)) {
            onNoResetFlg1(FLG1_UNK_80);
            var_r28 = true;
        }
    }

    f32 var_f31;
    if (var_r28) {
        if (mCutType == 0) {
            var_f31 = 0.4f;
        } else {
            var_f31 = 1.5f;
        }
    } else {
        if (checkNoResetFlg1(FLG1_UNK_80)) {
            var_f31 = light_m->mPower;
        } else {
            var_f31 = 0.0f;
        }
    }

    f32 var_f30 = 1.0f;
    f32 var_f29;
    if (mProcID == PROC_METAMORPHOSE && mProcVar5.field_0x3012 != 0) {
        var_f29 = 0.05f;
    } else if (checkEndResetFlg2(ERFLG2_LIGHT_SWORD_GET_EFFECT)) {
        var_f30 = 0.28f;
        var_f29 = 0.01f;
        var_f31 = 1.5f;
    } else {
        var_f29 = 0.2f;
    }

    cLib_chaseF(&field_0x33fc, var_f31, light_m->mPower * var_f29);

    cXyz spB8;
    f32 var_f27;
    if (field_0x33fc > 0.0f) {
        GXColor spF0 = {0x00, 0x00, 0x00, 0xFF};
        spF0.r = light_m->mColorR;
        spF0.g = light_m->mColorG;
        spF0.b = light_m->mColorB;

        Vec spC4 = {0.0f, 0.0f, 0.0f};
        spC4.y = light_m->mYOffset;
        spC4.z = light_m->mZOffset;

        f32 var_f26;
        if ((uintptr_t)light_m == (uintptr_t)&mpHIO->mWolf.mLight.m) {
            cXyz spD0 = eyePos - field_0x34e0;
            s16 sp104 = spD0.atan2sY_XZ();
            s16 sp106 = spD0.atan2sX_Z();

            mDoMtx_stack_c::transS(eyePos);
            mDoMtx_stack_c::ZXYrotM(sp104, sp106, 0);
            mDoMtx_stack_c::multVec(&spC4, &spB8);

            var_f27 = cM_sht2d((s16)-(sp104 + mBodyAngle.x)) * 0.5f;
            var_f26 = cM_sht2d(-sp106);
        } else {
            if (var_r28) {
                spB8 = (mSwordTopPos + field_0x3498) * 0.5f;

                spF0.r = var_f30 * 150.0f;
                spF0.g = var_f30 * 174.0f;
                spF0.b = var_f30 * 141.0f;
            } else {
                spB8 = mKandelaarFlamePos;
            }

            var_f27 = 0.0f;
            var_f26 = cM_sht2d(-shape_angle.y);
        }

        dKy_WolfEyeLight_set(&spB8, var_f27 + light_m->mXAngle, var_f26, (light_m->mWidth * field_0x33fc) / light_m->mPower, &spF0, field_0x33fc, light_m->mAngleAttenuationType, light_m->mDistanceAttenuationType);
    }
}

void daAlink_c::setFrontRollCrashShock(u8 param_0) {
    dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));

    mZ2Link.startLinkSound(Z2SE_AL_BODYATTACK, param_0, mVoiceReverbIntensity);
    dKy_Sound_set(current.pos, 100, fopAcM_GetID(this), 5);
}

MtxP daAlink_c::getModelJointMtx(u16 i_jointNo) {
    if (i_jointNo >= field_0x30c6) {
        JUT_ASSERT(22489, 0);

        #if !DEBUG
        return mpLinkModel->mBaseTransformMtx;
        #endif
    }

    return mpLinkModel->getAnmMtx(i_jointNo);
}

void daAlink_c::onFrollCrashFlg(u8 i_flg, BOOL param_1) {
    if (param_1) {
        onNoResetFlg0(FLG0_UNK_10);
    } else {
        onNoResetFlg0(FLG0_UNK_8);
    }

    mRollCrashFlg = i_flg;
}

void daAlink_c::changeWarpMaterial(daAlink_c::daAlink_WARP_MAT_MODE i_matMode) {
    void (*mat_func_tbl[])(J3DModelData*) = {dRes_info_c::onWarpMaterial, dRes_info_c::offWarpMaterial};

    void (*mat_func)(J3DModelData*) = mat_func_tbl[i_matMode];
    mat_func(field_0x064C);
    mat_func(mSwordModel->getModelData());
    mat_func(mShieldModel->getModelData());
    mat_func(mSheathModel->getModelData());

    if (checkWolf()) {
        mat_func(mpWlChainModels[0]->getModelData());
    } else {
        mat_func(mpLinkFaceModel->getModelData());
        mat_func(mpLinkHatModel->getModelData());
        mat_func(mpLinkHandModel->getModelData());
        mat_func(mpLinkBootModels[0]->getModelData());
    }

    if (i_matMode == WARP_MAT_MODE_1) {
        for (int i = 0; i < 6; i++) {
            JPABaseEmitter* emitterp = dComIfGp_particle_getEmitter(field_0x3240[i]);
            if (emitterp != NULL) {
                emitterp->stopDrawParticle();
            }
        }
    }
}

void daAlink_c::commonProcInit(daAlink_c::daAlink_PROC i_procID) {
    if (mProcID == PROC_TOOL_DEMO) {
        speed.y = 0.0f;
        resetDemoBck();
        if (checkModeFlg(MODE_RIDING)) {
            initForceRideHorse();
        }

        if (mProcVar2.field_0x300c != 0) {
            changeWarpMaterial(WARP_MAT_MODE_1);
        }
    } else if (mProcID == PROC_GRAB_STAND) {
        for (int i = 0; i < 2; i++) {
            field_0x312a[i] = csXyz::Zero;
            field_0x3136[i] = csXyz::Zero;
        }
    } else if (mProcID == PROC_GRASS_WHISTLE_WAIT) {
        returnKeepItemData();
    } else if (checkModeFlg(MODE_UNK_20)) {
        fopAc_ac_c* actor = field_0x280c.getActor();

        if (actor != NULL) {
            if (mProcID == PROC_WOLF_DOWN_AT_LAND) {
                static_cast<fopEn_enemy_c*>(actor)->offWolfDownPullFlg();
                if (i_procID != PROC_WOLF_BACKJUMP) {
                    resetWolfBallGrab();
                }
            } else if (mProcID == PROC_HAWK_SUBJECT) {
                static_cast<daNPC_TK_c*>(actor)->setAway();
            } else if (mProcID == PROC_GOAT_STROKE) {
                static_cast<daCow_c*>(actor)->setNaderuFinish();
            } else if (mProcID == PROC_TRADE_ITEM_OUT || mProcID == PROC_NOT_USE_ITEM) {
                static_cast<daItemBase_c*>(actor)->dead();
            }
        }
        field_0x280c.clearData();

        if (mProcID == PROC_BOTTLE_SWING) {
            offNoResetFlg2(FLG2_KANDELAAR_LIGHT_OFF);
        }
    } else if (mProcID == PROC_HOOKSHOT_FLY) {
        cancelHookshotCarry();
        setOldRootQuaternion(shape_angle.x, 0, 0);
        cancelItemUseQuake(1);
    } else if (dComIfGp_checkPlayerStatus1(0, 0x02010000)) {
        if (mProcID != PROC_HOOKSHOT_FLY) {
            cancelItemUseQuake(0);
        }

        if (mCargoCarryAcKeep.getActor() != NULL) {
            fopAcM_cancelHookCarryNow(mCargoCarryAcKeep.getActor());
            mCargoCarryAcKeep.clearData();
        }
    } else if (mProcID == PROC_WOLF_ENEMY_HANG_BITE) {
        resetWolfEnemyBiteAll();
    } else if (checkWolfShapeReverse()) {
        setOldRootQuaternion(0, -0x8000, 0);
        shape_angle.y += 0x8000;
        field_0x2fe4 = shape_angle.y;
        shape_angle.x = -shape_angle.x;
        mPrevAngleY = shape_angle.y;
        if (mProcID != PROC_WOLF_SLIDE_READY) {
            current.angle.y = shape_angle.y;
        }
    } else if (mProcID == PROC_WARP) {
        changeWarpMaterial(WARP_MAT_MODE_1);
    } else if (mProcID == PROC_WOLF_BACKJUMP) {
        resetWolfBallGrab();
    } else if (mProcID == PROC_CHAIN_STRONG_PULL) {
        mNowAnmPackUpper[2].setRatio(1.0f);
    } else if (mProcID == PROC_SCREAM_WAIT || mProcID == PROC_WOLF_SCREAM_WAIT ||
               mProcID == PROC_ELEC_DAMAGE)
    {
        dComIfGp_getVibration().StopQuake(0x1F);
    } else if (mProcID == PROC_GUARD_BREAK) {
        mCcStts.SetTgApid(fpcM_ERROR_PROCESS_ID_e);
    }

    if (mProcID == PROC_CHAIN_UP || mProcID == PROC_WOLF_CHAIN_UP) {
        cancelFmChainGrabFromOut();
    }

    BOOL prev_proc_fishcast;
    if (mProcID == PROC_FISHING_CAST) {
        prev_proc_fishcast = TRUE;
    } else {
        prev_proc_fishcast = FALSE;
    }
    offNoResetFlg0(FLG0_UNK_2);

    BOOL prev_flg_2;
    BOOL prev_flg_20000;
    u32 prev_flg_40000;
    u32 prev_flg_400;

    if (checkModeFlg(0x20000) != 0) {
        prev_flg_20000 = TRUE;
    } else {
        prev_flg_20000 = FALSE;
    }
    if (!checkModeFlg(0x2)) {
        prev_flg_2 = TRUE;
    } else {
        prev_flg_2 = FALSE;
    }
    prev_flg_40000 = checkModeFlg(0x40000);
    prev_flg_400 = checkModeFlg(0x400);

    if (checkModeFlg(0x400) && !checkBoardRide() && !checkSpinnerRide()) {
        field_0x2f99 = 0xA0;
    } else if (checkModeFlg(0x40840) || mProcID == PROC_BOSS_BODY_HANG ||
               mProcID == PROC_WOLF_CHAIN_WAIT)
    {
        field_0x2f99 = 0x70;
    }

    mpProcFunc = m_procInitTable[i_procID].m_procFunc;
    mProcID = i_procID;
    mModeFlg = m_procInitTable[i_procID].m_mode;

    if (checkBoarRide()) {
        offModeFlg(4);
    }

    if (mProcID == PROC_DEAD) {
        if (prev_flg_400) {
            if (checkHorseRide()) {
                resetUnderAnime(UNDER_2, -1.0f);
            }
            onModeFlg(MODE_RIDING);
        } else if (prev_flg_40000) {
            onModeFlg(MODE_SWIMMING);
        }
    } else if (mProcID == PROC_BOSS_BODY_HANG) {
        if (checkBossOctaIealRoom()) {
            onModeFlg(MODE_SWIMMING);
        }
    } else if (mProcID == PROC_GET_ITEM) {
        if (prev_flg_400) {
            if (checkHorseRide()) {
                resetUnderAnime(UNDER_2, -1.0f);
            }
            onModeFlg(MODE_RIDING);
        }
    } else if (mProcID == PROC_ELEC_DAMAGE) {
        if (prev_flg_40000) {
            onModeFlg(MODE_SWIMMING);
        }

        if (checkResetFlg1(RFLG1_UNK_2) && checkHookshotItem(mEquipItem)) {
            onModeFlg(MODE_UNK_1000);
        }
    }

    if (prev_proc_fishcast &&
        (mItemAcKeep.getActor() == NULL ||
         !mItemAcKeep.getActor()->eventInfo.checkCommandDemoAccrpt()) &&
        !checkEndResetFlg0(ERFLG0_UNK_1000))
    {
        deleteEquipItem(FALSE, FALSE);
        resetUpperAnime(UPPER_2, -1.0f);
    }

    if (checkModeFlg(MODE_VINE_CLIMB)) {
        mLinkAcch.SetRoofNone();
    } else {
        mLinkAcch.ClrRoofNone();
    }

    field_0x2b9c = 0.0f;
    field_0x2b98 = 0.0f;
    mSpeedModifier = 0.0f;
    field_0x2fb3 = 0;
    mEffProc = EFFPROC_NONE;

    clearCutTurnEffectID();
    if (!checkModeFlg(MODE_SWIMMING)) {
        field_0x3080 = 0;
    }

    field_0x3082 = 0;
    field_0x3088 = 0;
    field_0x308a = 0;

    if (checkWolf()) {
        shape_angle.z = 0;
        mBodyAngle.x = 0;
        mBodyAngle.y = 0;
    } else if (!checkReinRide() && !checkBoardRide()) {
        shape_angle.x = 0;
        shape_angle.z = 0;
    }

    initGravity();

    field_0x308c = 0;
    field_0x33f0 = 0.0f;
    field_0x33f4 = 0.0f;
    field_0x30a0 = 0;
    field_0x30a2 = 0;
    field_0x30f0 = 0;
    m_nSwordBtk->setFrame(0.0f);
    field_0x310a = 0;
    field_0x310c = shape_angle.y;

    if (field_0x3190 != 0) {
        field_0x3190 = 0;
        mBodyAngle.y = 0;
    }

    mFishingArm1Angle = csXyz::Zero;
    field_0x3160 = csXyz::Zero;

    if (field_0x2fc8 != 0) {
        field_0x2fc8 = 0;
        resetFacePriAnime();
    }

    field_0x2fce = 5;
    if (field_0x2f99 == 5) {
        field_0x2f99 = 0x30;
    } else if (field_0x2f99 == 4) {
        field_0x2f99 = 0x20;
    }

    if (checkModeFlg(0x40840) || mProcID == PROC_BOSS_BODY_HANG) {
        field_0x2f99 = 0x50;
    }

    int i;

    if (checkModeFlg(MODE_NO_COLLISION) || mProcID == PROC_STEP_MOVE || mProcID == PROC_WOLF_TAG_JUMP) {
        mLinkAcch.OffLineCheck();
        mLinkAcch.OnLineCheckNone();
    } else {
        mLinkAcch.OnLineCheck();
        mLinkAcch.OffLineCheckNone();

        if (!checkWolf()) {
            daAlink_footData_c* foot_data = mFootData2;
            for (i = 0; i < 2; i++, foot_data++) {
                foot_data->field_0x6 = 0;
                foot_data->field_0x4 = 0;
                foot_data->field_0x2 = 0;
            }

            field_0x2ba4 = 0.0f;
        }
    }

    onResetFlg0(RFLG0_UNK_8000000);

    if (mProcID != PROC_WOLF_FALL && mProcID != PROC_WOLF_LAND) {
        offNoResetFlg2(FLG2_UNK_200);
    }

    offNoResetFlg2(daPy_FLG2(FLG2_WOLF_DASH_AUTO_JUMP | FLG2_PRESSED_DAMAGE | FLG2_UNK_100 | FLG2_COMBO_RESERB));
    offResetFlg0(RFLG0_UNK_2);
    offNoResetFlg1(daPy_FLG1(FLG1_FREEZE_DAMAGE | FLG1_UNK_8000000 | FLG1_THROW_DAMAGE | FLG1_DK_CAUGHT));
    offNoResetFlg0(daPy_FLG0(FLG0_FAST_SWORD_CUT | FLG0_UNK_10 | FLG0_UNK_8));

    field_0x28f8 = -1;
    mSight.offDrawFlg();
    mSight.offLockFlg();

    if (mWolfLockNum != 0 && mProcID != PROC_WOLF_LOCK_ATTACK_TURN && mProcID != PROC_WOLF_LOCK_ATTACK) {
        for (i = 0; i < mWolfLockNum; i++) {
            mWolfLockAcKeep[i].clearData();
        }
        mWolfLockNum = 0;
    }

    field_0x3078 = 0;

    if (((!checkEquipAnime() || !checkModeFlg(4)) && (!checkModeFlg(0x40000) || !checkWolfEnemyThrowAnime()) && !checkModeFlg(0x1000))
        || (checkSwordTwirlAnime() && mProcID != PROC_ATN_ACTOR_WAIT && mProcID != PROC_ATN_ACTOR_MOVE)
        || (checkWolf() && checkFmChainGrabAnime() && checkModeFlg(0x40))
        || checkNoResetFlg1(FLG1_UNK_10000000))
    {
        if ((!checkDkCaught2Anime() || mProcID != PROC_DAMAGE) && !checkNoSetUpperAnime()) {
            resetUpperAnime(UPPER_2, mpHIO->mBasic.m.mBasicInterpolation);
        }

        if (checkBombItem(mEquipItem)) {
            mEquipItem = fpcNm_ITEM_NONE;
        }
    }

    if (!checkModeFlg(MODE_GRAB_PLACE) && !checkGrabAnime() &&
        !checkWolfGrabAnime() && mGrabItemAcKeep.getActor() != NULL) {
        freeGrabItem();
    }

    if (!checkHookshotAnime() && mProcID != PROC_HOOKSHOT_FLY) {
        cancelHookshotCarry();
    }

    if ((dComIfGp_checkPlayerStatus0(0, 8) && !checkModeFlg(MODE_VINE_CLIMB) && mProcID != PROC_HANG_CLIMB) ||
        ((dComIfGp_checkPlayerStatus1(0, 0x2000000) && mProcID != PROC_HOOKSHOT_WALL_SHOOT && mProcID != PROC_HOOKSHOT_WALL_WAIT)))
    {
        if (mProcID == PROC_CLIMB_TO_ROOF) {
            current.pos.x += 10.0f * cM_ssin(shape_angle.y);
            current.pos.z += 10.0f * cM_scos(shape_angle.y);
        } else {
            current.pos.x -= 10.0f * cM_ssin(shape_angle.y);
            current.pos.z -= 10.0f * cM_scos(shape_angle.y);
        }
    }

    if (dComIfGp_checkPlayerStatus0(0, 0x200000)) {
        seStartSystem(Z2SE_AL_HAWK_EYE_PUTOFF);
    }

    if (checkUpperReadyThrowAnime() && mEquipItem != 0x102) {
        dComIfGp_clearPlayerStatus0(0, 0xfeb5ab0f);
    } else {
        dComIfGp_clearPlayerStatus0(0, 0xffbfffcf);
    }

    dComIfGp_clearPlayerStatus1(0, 0x7fb7b78);

    cancelHookshotShot();
    if (mEquipItem == 0x109) {
        deleteEquipItem(FALSE, FALSE);
    }

    if (prev_flg_2 && checkModeFlg(MODE_JUMP)) {
        mLastJumpPos = current.pos;
        mFallHeight = mLastJumpPos.y;
        field_0x33c8 = mLastJumpPos.y;
    }
#if DEBUG
    else if (!prev_flg_2 && !checkModeFlg(MODE_JUMP)) {
        mpHIO->jumpStateUpdate(&mLastJumpPos, &current.pos, l_jumpTop);
        l_jumpTop = 0.0f;
    }
#endif

    if (!checkModeFlg(MODE_RIDING) && prev_flg_400) {
        rideGetOff();
    }

    if (!checkBoarRide() && !checkHorseRide()) {
        resetUnderAnime(UNDER_2, -1.0f);
    }

    if (checkModeFlg(MODE_VINE_CLIMB | MODE_NO_COLLISION)) {
        if (checkWolf()) {
            mCcStts.SetWeight(0xFF);
        } else {
            mCcStts.SetWeight(0xFE);
        }
    } else {
        mCcStts.SetWeight(0x78);
    }

    dComIfGp_setAdvanceDirection(0);

    if (!checkBowAnime()) {
        setBowNormalAnime();
    }

    if (prev_flg_40000 && !checkModeFlg(MODE_SWIMMING)) {
        if (mProcID != PROC_CAUGHT && mProcID != PROC_HOOKSHOT_FLY &&
            !checkNoResetFlg0(FLG0_WATER_IN_MOVE))
        {
            swimOutAfter(0);
        }
    }

    setBgCheckParam();

    if ((checkNoResetFlg2(FLG2_WOLF_ENEMY_HANG_BITE) && mProcID != PROC_WOLF_ENEMY_HANG_BITE) ||
        (checkNoResetFlg2(FLG2_UNK_8) && mProcID != PROC_WOLF_ENEMY_THROW && !checkWolfEnemyThrowAnime()))
    {
        resetWolfEnemyBiteAll();
    }

    fopAc_ac_c* actor = field_0x280c.getActor();
    if (actor != NULL && prev_flg_20000 && !checkModeFlg(MODE_ROPE_WALK)) {
        field_0x280c.clearData();

        if (fopAcM_GetName(actor) == PROC_Obj_Crope) {
            static_cast<daObjCrope_c*>(actor)->offRide();
        } else if (fopAcM_GetName(actor) == PROC_Obj_Wchain) {
            static_cast<daObjWchain_c*>(actor)->offRide();
        }
    }
}

BOOL daAlink_c::commonProcInitNotSameProc(daAlink_PROC i_procID) {
    if (mProcID == i_procID) {
        return false;
    } else {
        commonProcInit(i_procID);
        return true;
    }
}

 int daAlink_c::procPreActionUnequipInit(int i_procAfter, fopAc_ac_c* param_1) {
    commonProcInit(PROC_PREACTION_UNEQUIP);
    mNormalSpeed = 0.0f;
    setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    allUnequip(0);
    mProcVar4.field_0x3010 = 0;

    if (param_1 != NULL) {
        field_0x280c.setData(param_1);
        mProcVar3.field_0x300e = 1;

        if (fopAcM_GetName(param_1) == PROC_Tag_Lv8Gate) {
            if (mEquipItem == fpcNm_ITEM_KANTERA) {
                mProcVar4.field_0x3010 = 1;
            } else if (checkNoResetFlg2(FLG2_UNK_1)) {
                offKandelaarModel();
            }
        }
    } else {
        mProcVar3.field_0x300e = 0;
    }

    field_0x3198 = i_procAfter;
    return 1;
}

int daAlink_c::procPreActionUnequip() {
    if (checkNoUpperAnime()) {
        if (mProcVar3.field_0x300e != 0 && (field_0x27f4 == NULL || field_0x27f4 != field_0x280c.getActor()))
        {
            checkWaitAction();
        } else if (field_0x3198 == PROC_PUSH_PULL_WAIT) {
            procCoPushPullWaitInit(1);
        } else if (field_0x3198 == PROC_LADDER_DOWN_START) {
            procLadderDownStartInit();
        } else if (field_0x3198 == PROC_LADDER_UP_START) {
            procLadderUpStartInit();
        } else if (field_0x3198 == PROC_GRAB_READY) {
            procGrabReadyInit();
        } else if (field_0x3198 == PROC_GET_ITEM) {
            procCoGetItemInit();
        } else if (field_0x3198 == PROC_GET_READY_SIT) {
            procCoGetReadySitInit();
        } else if (field_0x3198 == PROC_CHAIN_UP) {
            procFmChainUpInit();
        } else if (field_0x3198 == PROC_PICK_UP) {
            procPickUpInit();
        } else if (field_0x3198 == PROC_CRAWL_START) {
            procCrawlStartInit();
        } else if (field_0x3198 == PROC_GRASS_WHISTLE_GET) {
            procGrassWhistleGetInit();
        } else if (field_0x3198 == PROC_GOAT_STROKE) {
            procGoatStrokeInit();
        } else if (field_0x3198 == PROC_HANG_READY) {
            procHangReadyInit();
        } else if (field_0x3198 == PROC_INSECT_CATCH) {
            procInsectCatchInit();
        } else if (field_0x3198 == PROC_GORON_MOVE) {
            procGoronMoveInit();
        } else if (field_0x3198 == PROC_BOTTLE_SWING) {
            procBottleSwingInit(NULL, 0);
        } else if (field_0x3198 == PROC_HANG_LEVER_DOWN) {
            procHangLeverDownInit();
        } else if (field_0x3198 == PROC_PICK_PUT) {
            procPickPutInit(1);
        } else {
            checkNextAction(0);
        }
    } else {
        setBlendMoveAnime(-1.0f);
    }

    return 1;
}

int daAlink_c::procServiceWaitInit() {
    commonProcInit(PROC_SERVICE_WAIT);
    setSingleAnimeBase(ANM_SERVICE_WAIT);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;
    return 1;
}

int daAlink_c::procServiceWait() {
    J3DFrameCtrl* frameCtrl_p = mUnderFrameCtrl;
    if ((!checkNextAction(0) && !checkFrontWallTypeAction()) &&
        (checkEventRun() || checkGuardActionChange() || shape_angle.y != mPrevAngleY))
    {
        frameCtrl_p->setRate(0.0f);
        procWaitInit();
    } else {
        if (frameCtrl_p->checkPass(60.0f)) {
            voiceStart(Z2SE_AL_V_RELAX_A);
        } else if (frameCtrl_p->checkPass(150.0f)) {
            voiceStart(Z2SE_AL_V_RELAX_B);
        } else if (frameCtrl_p->checkPass(190.0f)) {
            voiceStart(Z2SE_AL_V_RELAX_C);
        }
    }
    return 1;
}

int daAlink_c::procTiredWaitInit() {
    BOOL no_upper_anm = checkNoSetUpperAnime();

    if (!commonProcInitNotSameProc(PROC_TIRED_WAIT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    BOOL tired_anm = checkUnderMove0BckNoArc(ANM_WAIT_TIRED);

    if (no_upper_anm &&
        ((tired_anm && mNowAnmPackUnder[0].getRatio() < 0.75f) ||
         checkUnderMove1BckNoArc(ANM_STEP_TURN) || checkUnderMove1BckNoArc(ANM_SMALL_GUARD)))
    {
        f32 frame;
        if (tired_anm) {
            frame = mUnderFrameCtrl[0].getFrame();
        } else {
            frame = mUpperFrameCtrl[1].getFrame();
        }

        setSingleAnimeBase(ANM_WAIT_TIRED);
        mUnderFrameCtrl[0].setFrame(frame);
        getNowAnmPackUnder(UNDER_0)->setFrame(frame);
    } else {
        setSingleAnimeBase(ANM_WAIT_TO_TIRED);
    }

    return 1;
}

int daAlink_c::procTiredWait() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    if (!checkNextAction(0)) {
        if (checkAnmEnd(frameCtrl_p)) {
            setSingleAnimeBase(ANM_WAIT_TIRED);
        } else if (frameCtrl_p->getAttribute() == 2) {
            setTiredVoice(frameCtrl_p);
        }
    }

    return 1;
}

int daAlink_c::procWaitInit() {
    if (mProcID == PROC_TURN_BACK) {
        onEndResetFlg0(ERFLG0_UNK_8000000);
    }

    if (mProcID == PROC_WAIT) {
        return 0;
    }

    if (!checkEventRun() && mProcID == PROC_SERVICE_WAIT && !checkAnmEnd(mUnderFrameCtrl) &&
        checkNoUpperAnime())
    {
        return 0;
    }

    BOOL var_r30;
    if (mProcID == PROC_GUARD_ATTACK || mProcID == PROC_CUT_REVERSE) {
        var_r30 = false;
    } else {
        var_r30 = true;
    }

    commonProcInit(PROC_WAIT);
    mNormalSpeed = 0.0f;
    setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    current.angle.y = shape_angle.y;

    if (var_r30) {
        field_0x2f98 = 4;
    }

    initServiceWaitTime();

    if (checkEndResetFlg0(ERFLG0_UNK_8000000) &&
        (checkUnderMove0BckNoArc(ANM_WAIT) || checkUnderMove0BckNoArc(ANM_WAIT_TIRED)))
    {
        setSingleAnimeBase(ANM_WAIT_B_TO_A);
    }

    return 1;
}

int daAlink_c::procWait() {
    BOOL isPrevAnmWaitB = checkUnderMove0BckNoArc(ANM_WAIT_B);

    if (!checkNextAction(0) && !checkFrontWallTypeAction()) {
        daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
        field_0x33f0 = (s16)(shape_angle.y - mPrevAngleY) * 0.005f;

        if (checkAnmEnd(frameCtrl_p) || checkUpperGuardAnime() ||
            !checkUnderMove0BckNoArc(ANM_WAIT_B_TO_A))
        {
            if (checkRestHPAnime() && shape_angle.y == mPrevAngleY) {
                return procTiredWaitInit();
            }

            setBlendMoveAnime(-1.0f);
            if (isPrevAnmWaitB && !checkUnderMove0BckNoArc(ANM_WAIT_B)) {
                setSingleAnimeBase(ANM_WAIT_B_TO_A);
            }
        } else if (frameCtrl_p->checkPass(30.0f)) {
            mLeftHandIndex = 4;
            mRightHandIndex = 10;
        }

        if (checkServiceWaitMode() && checkUnderMove0BckNoArc(ANM_WAIT)) {
            if (field_0x30ca != 0) {
                field_0x30ca--;
            }

            if (field_0x30ca == 0 && frameCtrl_p->checkPass(0.0f)) {
                procServiceWaitInit();
            }
        } else {
            initServiceWaitTime();
        }
    }

    return 1;
}

int daAlink_c::procMoveInit() {
    if (!commonProcInitNotSameProc(PROC_MOVE)) {
        return 0;
    }

    setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    setFootEffectProcType(3);
    return 1;
}

int daAlink_c::procMove() {
    setFootEffectProcType(3);
    if (!checkNextAction(0) && !checkFrontWallTypeAction()) {
        if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_2_e &&
            mNormalSpeed > mMaxSpeed * mpHIO->mMove.m.mWalkChangeRate)
        {
            mNormalSpeed = mMaxSpeed * mpHIO->mMove.m.mWalkChangeRate;
        }

        setBlendMoveAnime(-1.0f);
        if (mGroundCode != 8) {
            field_0x30a0 =
                cLib_minMaxLimit<s16>((s16)(field_0x2ff0 >> 1), mpHIO->mBasic.m.mHeadMaxTurnUp,
                                 mpHIO->mBasic.m.mHeadMaxTurnDown);
        }
    }

    return 1;
}

int daAlink_c::procAtnMoveInit() {
    if (!commonProcInitNotSameProc(PROC_ATN_MOVE)) {
        return 0;
    }

    setBlendAtnMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    return 1;
}

int daAlink_c::procAtnMove() {
    if (!checkNextAction(0)) {
        if (field_0x2f98 != 0 || !checkFrontWallTypeAction()) {
            setBlendAtnMoveAnime(-1.0f);
        }
    }

    return 1;
}

int daAlink_c::procAtnActorWaitInit() {
    if (!commonProcInitNotSameProc(PROC_ATN_ACTOR_WAIT)) {
        return 0;
    }

    s16 var_r30;
    if (mTargetedActor != NULL) {
        var_r30 = fopAcM_searchActorAngleY(this, mTargetedActor);
    } else {
        var_r30 = 0;
    }

    mNormalSpeed = 0.0f;
    if (field_0x2f98 != 2) {
        field_0x2f98 = 3;
    }

    if (checkAtnWaitAnime()) {
        setBlendAtnMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    } else {
        setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    }

    return 1;
}

int daAlink_c::procAtnActorWait() {
    if (!checkNextAction(0)) {
        if (checkAtnWaitAnime()) {
            setBlendAtnMoveAnime(-1.0f);
        } else {
            setBlendMoveAnime(-1.0f);
        }
        checkSwordTwirlAction();
    }

    return 1;
}

int daAlink_c::procAtnActorMoveInit() {
    if (!commonProcInitNotSameProc(PROC_ATN_ACTOR_MOVE)) {
        return 0;
    }

    setBlendAtnMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    return 1;
}

int daAlink_c::procAtnActorMove() {
    if (!checkNextAction(0)) {
        setBlendAtnMoveAnime(-1.0f);
        checkSwordTwirlAction();
    }

    return 1;
}

int daAlink_c::procWaitTurnInit() {
    if (!commonProcInitNotSameProc(PROC_WAIT_TURN)) {
        return 0;
    }

    setSingleAnimeBaseSpeed(ANM_STEP_TURN, mpHIO->mBasic.m.mRotationASpeed,
                            mpHIO->mBasic.m.mBasicInterpolation);
    if (checkEventRun()) {
        mNormalSpeed = 0.0f;
    }

    mProcVar3.field_0x300e = mMoveAngle;
    current.angle.y = shape_angle.y;
    return 1;
}

int daAlink_c::procWaitTurn() {
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);

    if (checkGroundSpecialMode()) {
        return 1;
    } else if (checkEndResetFlg0(ERFLG0_UNK_100000)) {
        return procFloorDownReboundInit();
    }

    s16 angle = cLib_addCalcAngleS(&shape_angle.y, mProcVar3.field_0x300e, 30, 0x3CDF, 8000);
    current.angle.y = shape_angle.y;

    if (checkNextActionFromButton()) {
        return 1;
    } else if (angle == 0) {
        if (checkEventRun()) {
            if (mDemo.getDemoMode() == daPy_demo_c::DEMO_WAIT_TURN_e) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);
            } else if (!checkNextAction(0)) {
                checkWaitAction();
            }
        } else {
            checkNextAction(0);
        }
    }

    return 1;
}

int daAlink_c::procMoveTurnInit(int param_0) {
    if (!commonProcInitNotSameProc(PROC_MOVE_TURN)) {
        return 0;
    }

    setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);
    dComIfGp_setPlayerStatus0(0, 0x800);

    if (param_0 != 0) {
        mProcVar3.field_0x300e = (mpHIO->mMove.m.mMaxTurnAngle * 4) + 19030;
        mProcVar4.field_0x3010 = mpHIO->mMove.m.mMaxTurnAngle * 2;
        mProcVar1.field_0x300a = 2;
        current.angle.y = mMoveAngle;
        mNormalSpeed *= 0.5f;
    } else {
        mProcVar3.field_0x300e = mpHIO->mMove.m.mMaxTurnAngle * 2;
        mProcVar4.field_0x3010 = mpHIO->mMove.m.mMaxTurnAngle;
        mProcVar1.field_0x300a = 3;
    }

    mProcVar3.field_0x300e = mpHIO->mMove.m.mMaxTurnAngle * 2;
    mProcVar4.field_0x3010 = mpHIO->mMove.m.mMaxTurnAngle;
    return 1;
}

int daAlink_c::procMoveTurn() {
    setSpeedAndAngleNormal();
    cLib_addCalcAngleS(&shape_angle.y, current.angle.y, mProcVar1.field_0x300a, mProcVar3.field_0x300e, mProcVar4.field_0x3010);

    if (!checkNextAction(0)) {
        setBlendMoveAnime(-1.0f);
    }

    return 1;
}

int daAlink_c::procSideStepInit(int i_jumpDirection) {
    if (i_jumpDirection == DIR_BACKWARD && !checkHeavyStateOn(TRUE, TRUE) &&
        (checkNoUpperAnime() || checkEquipAnime() || (field_0x2fcc != 0 && checkUpperGuardAnime())))
    {
        return procBackJumpInit(0);
    }

    commonProcInit(PROC_SIDESTEP);
    field_0x2f98 = i_jumpDirection;

    daAlink_ANM anm_id;
    if (field_0x2f98 == DIR_BACKWARD) {
        current.angle.y = shape_angle.y + 0x8000;
        setSingleAnimeParam(ANM_BACK_JUMP, &mpHIO->mSideStep.m.mBackJumpAnm);
        mNormalSpeed = mpHIO->mSideStep.m.mBackJumpSpeedH;
        speed.y = mpHIO->mSideStep.m.mBackJumpSpeedV;
        mProcVar1.field_0x300a = 0;
    } else {
        if (field_0x2f98 == DIR_LEFT) {
            anm_id = ANM_SIDE_JUMP_LEFT;
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            anm_id = ANM_SIDE_JUMP_RIGHT;
            current.angle.y = shape_angle.y - 0x4000;
        }

        setSingleAnimeParam(anm_id, &mpHIO->mSideStep.m.mSideJumpAnm);
        mNormalSpeed = mpHIO->mSideStep.m.mSideJumpSpeedH;
        speed.y = mpHIO->mSideStep.m.mSideJumpSpeedV;
        mProcVar1.field_0x300a = 1;
    }

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
        speed.y *= mpHIO->mItem.mIronBoots.m.mWaterVelocityY;
    }

    voiceStart(Z2SE_AL_V_JUMP_S);
    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = 0;
    return 1;
}

int daAlink_c::procSideStep() {
    if (mTargetedActor != NULL && mProcVar1.field_0x300a != 0) {
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchActorAngleY(this, mTargetedActor), 5, 0x5E8, 0x13C);

        if (field_0x2f98 == 2) {
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            current.angle.y = shape_angle.y - 0x4000;
        }
    }

    if (doTrigger() && mProcVar1.field_0x300a != 0) {
        mProcVar2.field_0x300c = 1;
    }

    if (checkUpperItemActionFly()) {
        return 1;
    } else if (mLinkAcch.ChkGroundHit() && mProcVar3.field_0x300e != 0) {
        if (mProcVar2.field_0x300c != 0 && checkSideRollAction(field_0x2f98)) {
            return 1;
        } else {
            return procSideStepLandInit();
        }
    } else if (checkNoUpperAnime() && checkForceSwordSwing()) {
        setCutDash(1, 0);
    } else if (mDemo.getDemoMode() != daPy_demo_c::DEMO_UNK_16_e &&
               current.pos.y < mLastJumpPos.y - mpHIO->mSideStep.m.mFallHeight)
    {
        return procFallInit(2, mpHIO->mSideStep.m.mFallInterpolation);
    }

    mProcVar3.field_0x300e = 1;
    checkItemChangeFromButton();
    return 1;
}

int daAlink_c::procSideStepLandInit() {
    commonProcInit(PROC_SIDESTEP_LAND);

    if (field_0x2f98 == 1) {
        field_0x2f9d = 4;
        setSingleAnimeParam(ANM_BACK_JUMP_LAND, &mpHIO->mSideStep.m.mBackLandAnm);
        field_0x3478 = mpHIO->mSideStep.m.mBackLandAnm.mCancelFrame;
        field_0x2f98 = 2;
        mProcVar1.field_0x300a = 0;
        field_0x2fb0 = 0;
        field_0x2fcc = 10;
    } else {
        daAlink_ANM anm_id;
        u16 anmResIdx;
        if (field_0x2f98 == 2) {
            anm_id = ANM_SIDE_JUMP_LEFT_LAND;
            anmResIdx = dRes_ID_ALANM_BCK_ATTWIRLL_e;
        } else {
            anm_id = ANM_SIDE_JUMP_RIGHT_LAND;
            anmResIdx = dRes_ID_ALANM_BCK_ATTWIRLR_e;
        }

        setSingleAnimeParam(anm_id, &mpHIO->mSideStep.m.mSideLandAnm);
        field_0x3478 = mpHIO->mSideStep.m.mSideLandAnm.mCancelFrame;
        mProcVar1.field_0x300a = 1;
        field_0x2fb0 = 8;
        field_0x2fcc = 0;

        if (checkEnemyGroup(mTargetedActor) && mEquipItem == 0x103 && checkNoUpperAnime()) {
            setUpperAnimeBaseSpeed(anmResIdx, mpHIO->mAtnMove.m.mWaitAnmSpeed, -1.0f);
            seStartSwordCut(Z2SE_AL_WAIT_SWORD_SWING);
        }
    }

    field_0x2f9d = 4;
    setFootEffectProcType(2);
    onResetFlg1(RFLG1_UNK_30);
    mNormalSpeed = 0.0f;
    mProcVar2.field_0x300c = 0;
    current.angle.y = shape_angle.y;
    setStepLandVibration();
    return 1;
}

int daAlink_c::procSideStepLand() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (field_0x2fcc != 0) {
        field_0x2fcc = 10;
    }

    if (doTrigger() && mProcVar1.field_0x300a != 0) {
        mProcVar2.field_0x300c = 1;
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_16_e) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            checkNextAction(0);
        }
    } else if (frameCtrl_p->getFrame() > field_0x3478) {
        if (mProcVar2.field_0x300c != 0 && checkSideRollAction(field_0x2f98)) {
            return 1;
        }
        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procSlideInit(s16 param_0) {
    commonProcInit(PROC_SLIDE);

    mCcStts.SetWeight(255);
    field_0x3090 = 8;
    current.angle.y = param_0;

    if (getMoveBGActorName(mLinkAcch.m_gnd, FALSE) == PROC_Obj_Lv3R10Saka) {
        mProcVar3.field_0x300e = 1;
    } else {
        mProcVar3.field_0x300e = 0;
    }

    if (cLib_distanceAngleS(param_0, shape_angle.y) < 0x3800 || mProcVar3.field_0x300e != 0) {
        field_0x3198 = 1;
        setSingleAnimeParam(ANM_SLIDE_FORWARD, &mpHIO->mSlide.m.mForwardSlideAnm);
        dComIfGp_setPlayerStatus1(0, 0x100);
        onModeFlg(MODE_UNK_8000);
        onModeFlg(MODE_UNK_2000000);
        onModeFlg(MODE_UNK_20000000);
        mNormalSpeed = 0.0f;
        field_0x2f9d = 4;
        setUpperAnimeBaseMorf(dRes_ID_ALANM_BCK_DASHWIND_e, -1.0f);
        onNoResetFlg1(FLG1_UNK_10000000);
        setFootEffectProcType(0);
    } else {
        field_0x3198 = 0;
        setSingleAnimeParam(ANM_SLIDE_BACKWARD, &mpHIO->mSlide.m.mBackwardSlideAnm);
        dComIfGp_setPlayerStatus1(0, 0x200);
        field_0x2f9d = 0x60;
        setFootEffectProcType(1);
    }

    mMaxSpeed = mpHIO->mSlide.m.mMaxSpeed;
    field_0x33cc = 0.0f;
    mProcVar2.field_0x300c = 0;
    mProcVar4.field_0x3010 = 0;
    return 1;
}

int daAlink_c::procSlide() {
    cM3dGPla slide_poly;

    if (getSlidePolygon(&slide_poly)) {
        s16 angleY = slide_poly.mNormal.atan2sX_Z();
        if (field_0x3198 != 0) {
            f32 sin = mStickValue * cM_ssin(mMoveAngle - shape_angle.y);

            if (checkInputOnR()) {
                mProcVar2.field_0x300c =
                    cLib_minMaxLimit<s16>((s16)(mProcVar2.field_0x300c + sin * 256.0f), -0x1000, 0x1000);
            } else {
                cLib_chaseS(&mProcVar2.field_0x300c, 0, 64);
            }

            angleY += mProcVar2.field_0x300c;
            cLib_addCalcAngleS(&mProcVar4.field_0x3010, sin * -4096.0f, 3, 300, 50);
        }

        cLib_addCalcAngleS(&current.angle.y, angleY, 4, 0x1000, 0x400);

        s16 shape_angleY;
        if (field_0x3198 != 0) {
            shape_angleY = current.angle.y;
            field_0x2f9d = 4;
        } else {
            shape_angleY = current.angle.y + 0x8000;
            field_0x2f9d = 0x60;
        }

        cLib_addCalcAngleS(&shape_angle.y, shape_angleY, 4, 0x1000, 0x400);

        if (field_0x3198 == 0 || mUnderFrameCtrl[0].getFrame() > 6.0f) {
            f32 accel = mpHIO->mSlide.m.mAcceleration;
            f32 max_speed = mMaxSpeed;

            if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
                accel *= 0.3f;
                max_speed *= 0.3f;
            }

            mNormalSpeed += accel * ((1.0f - slide_poly.mNormal.y) * 0.5f + 1.0f) * cM_scos(current.angle.y - angleY);
            if (mNormalSpeed > max_speed) {
                mNormalSpeed = max_speed;
            }

            if (field_0x3198 != 0) {
                field_0x33cc = 1.0f;
                onModeFlg(0x20000000);
            }

            seStartMapInfoLevel(Z2SE_FN_LINK_SLIP);
        }
    } else {
        procSlideLandInit(field_0x3198);
    }

    return 1;
}

int daAlink_c::procSlideLandInit(int param_0) {
    commonProcInit(PROC_SLIDE_LAND);

    if (param_0 != 0) {
        setSingleAnimeParam(ANM_SLIDE_FORWARD_END, &mpHIO->mSlide.m.mForwardLandAnm);
        field_0x3478 = mpHIO->mSlide.m.mForwardLandAnm.mCancelFrame;
        field_0x3198 = 1;
        field_0x2f99 = 4;
        field_0x3588 = l_waitBaseAnime;
    } else {
        setSingleAnimeParam(ANM_SLIDE_BACKWARD_END, &mpHIO->mSlide.m.mBackwardLandAnm);
        field_0x3478 = mpHIO->mSlide.m.mBackwardLandAnm.mCancelFrame;
        field_0x3198 = 0;
    }

    mCcStts.SetWeight(255);
    mNormalSpeed *= 0.5f;
    return 1;
}

int daAlink_c::procSlideLand() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;

    cLib_chaseF(&mNormalSpeed, 0.0f, 4.0f);
    if (field_0x3198 != 0) {
        field_0x2f99 = 4;
    }

    if (checkAnmEnd(frameCtrl_p)) {
        current.angle.y = shape_angle.y;
        checkNextAction(0);
    } else if (frameCtrl_p->getFrame() > field_0x3478) {
        s16 prev_angle = current.angle.y;
        current.angle.y = shape_angle.y;

        if (!checkNextAction(1)) {
            current.angle.y = prev_angle;
        }
    }

    return 1;
}

int daAlink_c::procFrontRollInit() {
    BOOL is_guard_anime = checkUpperGuardAnime();

    if (mProcID == PROC_FRONT_ROLL && mDemo.getDemoMode() == daPy_demo_c::DEMO_FRONT_ROLL_e) {
        return 0;
    }

    BOOL is_dive_jump = mProcID == PROC_DIVE_JUMP;
    commonProcInit(PROC_FRONT_ROLL);

    f32 roll_anm_speed = is_dive_jump ? 6.0f : mpHIO->mFrontRoll.m.mRollAnm.mStartFrame;

    setSingleAnime(ANM_FRONT_ROLL, mpHIO->mFrontRoll.m.mRollAnm.mSpeed, roll_anm_speed,
                   mpHIO->mFrontRoll.m.mRollAnm.mEndFrame,
                   mpHIO->mFrontRoll.m.mRollAnm.mInterpolation);

    mNormalSpeed = speedF * mpHIO->mFrontRoll.m.mSpeedRate + mpHIO->mFrontRoll.m.mInitSpeed;

    f32 max_speed = mpHIO->mFrontRoll.m.mInitSpeed + mpHIO->mMove.m.mMaxSpeed * mpHIO->mFrontRoll.m.mSpeedRate;
    if (mNormalSpeed > max_speed) {
        mNormalSpeed = max_speed;
    }

    if (mNormalSpeed < mpHIO->mFrontRoll.m.mMinSpeed) {
        mNormalSpeed = mpHIO->mFrontRoll.m.mMinSpeed;
    }

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
    } else if (checkHeavyStateOn(TRUE, TRUE)) {
        mNormalSpeed *= mHeavySpeedMultiplier;
    }

    current.angle.y = shape_angle.y;
    voiceStart(Z2SE_AL_V_BACKTEN);
    mProcVar2.field_0x300c = 0;
    setFootEffectProcType(0);

    if (is_guard_anime) {
        onNoResetFlg0(FLG0_UNK_2);
    }

    if (checkEquipHeavyBoots()) {
        mAtSph.SetAtType(AT_TYPE_HEAVY_BOOTS);
        mAtSph.SetAtHitMark(1);
        mAtSph.SetAtSe(dCcD_SE_METAL);
        mAtSph.SetAtAtp(2);
        mAtSph.SetAtMtrl(dCcD_MTRL_NONE);
        mAtSph.ResetAtHit();
        mAtSph.SetR(mpHIO->mFrontRoll.m.mBootsAttackRadius);
        mAtSph.OnAtSetBit();
    }

    return 1;
}

int daAlink_c::procFrontRoll() {
    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    cM3dGPla poly;
    if (getSlidePolygon(&poly)) {
        cLib_chaseF(&mNormalSpeed, 0.0f, 2.5f);
    }

    if (checkInputOnR()) {
        cLib_addCalcAngleS(&current.angle.y, mMoveAngle, mpHIO->mFrontRoll.m.mTurnRate,
                           mpHIO->mFrontRoll.m.mMaxTurnAngle,
                           mpHIO->mFrontRoll.m.mTurnMinAngle);
        shape_angle.y = current.angle.y;
    }

    if (checkNoResetFlg0(FLG0_UNK_2)) {
        setUpperGuardAnime(-1.0f);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mDemo.getDemoMode() == daPy_demo_c::DEMO_FRONT_ROLL_e) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            if (!checkInputOnR()) {
                mNormalSpeed -= mpHIO->mFrontRoll.m.mMinSpeed;
                if (mNormalSpeed < 0.0f) {
                    mNormalSpeed = 0.0f;
                }
            }

            checkNextAction(0);
        }
    } else if (frameCtrl_p->getFrame() > mpHIO->mFrontRoll.m.mRollAnm.mCancelFrame) {
        onModeFlg(4);
        cLib_chaseF(&mNormalSpeed, 0.0f, 2.5f);

        if (checkZeroSpeedF()) {
            onModeFlg(1);
        }

        if (mProcVar2.field_0x300c != 0) {
            procCutFinishInit(2);
        } else if (!checkNextAction(1)) {
            cLib_chaseF(&mNormalSpeed, 0.0f, 2.5f);
        }
    } else if (mDemo.getDemoMode() != daPy_demo_c::DEMO_FRONT_ROLL_e
                && speedF >= mpHIO->mFrontRoll.m.mCrashSpeedThreshold
                &&
               (checkNoResetFlg0(daPy_FLG0(FLG0_UNK_10 | FLG0_UNK_8))
                || ((mAcchCir[0].ChkWallHit() && !dComIfG_Bgsp().GetPolyAttackThrough(mAcchCir[0]))
                    && dComIfG_Bgsp().GetWallCode(mAcchCir[0]) != 7
                    && cLib_distanceAngleS(current.angle.y + 0x8000, mAcchCir[0].GetWallAngleY()) <= mpHIO->mFrontRoll.m.mCrashAngleThreshold
                    && frameCtrl_p->getFrame() >= mpHIO->mFrontRoll.m.mCrashInitF
                    && frameCtrl_p->getFrame() <= mpHIO->mFrontRoll.m.mCrashEndF
                    )
                )
            )
    {
        if (!checkNoResetFlg0(daPy_FLG0(FLG0_UNK_10 | FLG0_UNK_8))) {
            mRollCrashFlg = dKy_pol_sound_get(&mAcchCir[0]);
        }

        if (checkNoResetFlg0(FLG0_UNK_10) ||
            (mLinkAcch.ChkWallHit() && mAcchCir[0].ChkWallHit() &&
             (getMoveBGActorName(mAcchCir[0], FALSE) == PROC_Obj_Pillar ||
              getMoveBGActorName(mAcchCir[0], FALSE) == PROC_Obj_TaFence)))
        {
            procFrontRollSuccessInit();
        } else {
            procFrontRollCrashInit();
        }
    } else {
        if (frameCtrl_p->checkPass(21.0f)) {
            dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (checkForceSwordSwing()) {
            mProcVar2.field_0x300c = 1;
        }

        if (frameCtrl_p->getFrame() > 16.0f) {
            if (frameCtrl_p->getFrame() >= 18.0f) {
                mLeftHandIndex = 1;
                mRightHandIndex = 6;
            }

            cLib_chaseF(&mNormalSpeed, 0.0f, 1.0f);
        } else if (frameCtrl_p->getFrame() > 6.0f) {
            field_0x2f9d = 4;
        }
    }

    return 1;
}

int daAlink_c::procFrontRollCrashInit() {
    commonProcInit(PROC_FRONT_ROLL_CRASH);
    setSingleAnime(ANM_ROLL_CRASH, 0.0f,
                   mpHIO->mFrontRoll.m.mCrashAnm.mStartFrame,
                   mpHIO->mFrontRoll.m.mCrashAnm.mEndFrame,
                   mpHIO->mFrontRoll.m.mCrashAnm.mInterpolation);

    mNormalSpeed = mpHIO->mFrontRoll.m.mCrashSpeedH;
    speed.y = mpHIO->mFrontRoll.m.mCrashSpeedV;

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
        speed.y *= mpHIO->mItem.mIronBoots.m.mWaterVelocityY;
    }

    current.angle.y -= -0x8000;
    setFrontRollCrashShock(mRollCrashFlg);
    voiceStart(Z2SE_AL_V_ZENTEN_FAIL);
    onResetFlg0(RFLG0_FRONT_ROLL_CRASH);

    return 1;
}

int daAlink_c::procFrontRollCrash() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (!checkModeFlg(2)) {
        if (checkAnmEnd(frameCtrl)) {
            checkNextAction(0);
        } else if (frameCtrl->getFrame() > mpHIO->mFrontRoll.m.mCrashAnm.mCancelFrame) {
            onModeFlg(4);
            checkNextAction(1);
        } else if (frameCtrl->getFrame() >= 18.0f) {
            mLeftHandIndex = 4;
        } else if (frameCtrl->getFrame() >= 14.0f) {
            mRightHandIndex = 10;
        }
    } else if ((mLinkAcch.ChkGroundHit() || checkEndResetFlg2(ERFLG2_UNK_100) ||
                checkMagneBootsOn()) &&
               checkModeFlg(2))
    {
        mNormalSpeed = 0.0f;
        setWaterInAnmRate(frameCtrl, mpHIO->mFrontRoll.m.mCrashAnm.mSpeed);
        frameCtrl->offEndFlg();

        #if DEBUG
        mpHIO->jumpStateUpdate(&mLastJumpPos, &current.pos, l_jumpTop);
        #endif

        current.angle.y = shape_angle.y;
        voiceStart(Z2SE_AL_V_ZENTEN_FAIL_2);
        offModeFlg(2);
        onModeFlg(0x8001);
    }

    return 1;
}

int daAlink_c::procFrontRollSuccessInit() {
    BOOL is_prev_dkCaught = mProcID == PROC_DK_CAUGHT;

    commonProcInit(PROC_FRONT_ROLL_SUCCESS);
    setSingleAnimeParam(ANM_BARRIER_RECOIL, &mpHIO->mFrontRoll.m.mCrashHitAnm);
    mNormalSpeed = 0.0f;

    if (!is_prev_dkCaught) {
        setFrontRollCrashShock(mRollCrashFlg);
        onResetFlg0(RFLG0_FRONT_ROLL_CRASH);
    } else {
        mUnderFrameCtrl[0].setFrame(3.0f);
        getNowAnmPackUnder(UNDER_0)->setFrame(3.0f);
    }

    field_0x3588 = l_halfAtnWaitBaseAnime;
    field_0x2f99 = 4;
    setFootEffectProcType(1);
    current.angle.y = shape_angle.y - -0x8000;

    return 1;
}

int daAlink_c::procFrontRollSuccess() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    field_0x2f99 = 4;
    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (checkAnmEnd(frameCtrl)) {
        current.angle.y = shape_angle.y;
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > mpHIO->mFrontRoll.m.mCrashHitAnm.mCancelFrame) {
        onModeFlg(4);
        current.angle.y = shape_angle.y;
        checkNextAction(1);
    } else if (frameCtrl->getFrame() >= 14.0f) {
        mLeftHandIndex = 1;
        mRightHandIndex = 6;
    } else if (frameCtrl->getFrame() > 9.0f) {
        field_0x2f9d = 0x60;
    }

    return 1;
}

int daAlink_c::procSideRollInit(int param_0) {
    BOOL is_prev_guardAnm = checkUpperGuardAnime();

    if (!commonProcInitNotSameProc(PROC_SIDE_ROLL)) {
        return 0;
    }

    field_0x2f98 = param_0;

    daAlink_ANM anmID;
    if (field_0x2f98 == 2) {
        anmID = ANM_SIDE_ROLL_LEFT;
        current.angle.y = shape_angle.y + 0x4000;
    } else {
        anmID = ANM_SIDE_ROLL_RIGHT;
        current.angle.y = shape_angle.y + -0x4000;
    }

    setSingleAnime(anmID, mpHIO->mGuard.mTurnMove.m.mSideRollAnmSpeed,
                   mpHIO->mGuard.mTurnMove.m.mTurnAnm.mStartFrame,
                   mpHIO->mGuard.mTurnMove.m.mTurnAnm.mEndFrame,
                   mpHIO->mGuard.mTurnMove.m.mTurnAnm.mInterpolation);
    mNormalSpeed = mpHIO->mGuard.mTurnMove.m.mSideRollSpeed;

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        mNormalSpeed *= mpHIO->mItem.mIronBoots.m.mWaterVelocityX;
    } else if (checkHeavyStateOn(TRUE, TRUE)) {
        mNormalSpeed *= mHeavySpeedMultiplier;
    }

    setFootEffectProcType(0);
    field_0x2f9d = 4;
    voiceStart(Z2SE_AL_V_SOTOMO_ROLL);

    if (is_prev_guardAnm) {
        onNoResetFlg0(FLG0_UNK_2);
    }

    mProcVar2.field_0x300c = 0;
    return 1;
}

int daAlink_c::procSideRoll() {
    onEndResetFlg0(ERFLG0_UNK_8000000);
    if (mTargetedActor != NULL) {
        cLib_addCalcAngleS(&shape_angle.y, fopAcM_searchActorAngleY(this, mTargetedActor), 5, 0x5E8, 0x13C);

        if (field_0x2f98 == 2) {
            current.angle.y = shape_angle.y + 0x4000;
        } else {
            current.angle.y = shape_angle.y - 0x4000;
        }
    }

    mProcVar2.field_0x300c |= checkCutFinishJumpUp();

    daPy_frameCtrl_c* frameCtrl_p = mUnderFrameCtrl;
    cM3dGPla poly;

    if (getSlidePolygon(&poly)) {
        cLib_chaseF(&mNormalSpeed, 0.0f, 2.5f);
    }

    if (checkNoResetFlg0(FLG0_UNK_2)) {
        setUpperGuardAnime(-1.0f);
    }

    if (checkAnmEnd(frameCtrl_p)) {
        if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_73_e) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            if (!checkAttentionLock()) {
                offNoResetFlg2(FLG2_UNK_8000000);
            }

            checkNextAction(0);
        }
    } else if (mProcVar2.field_0x300c != 0 && !checkNotJumpSinkLimit() &&
               frameCtrl_p->getFrame() > mpHIO->mGuard.mTurnMove.m.mTurnAnm.mCancelFrame)
    {
        procCutFinishJumpUpInit();
    } else if (frameCtrl_p->getFrame() > mpHIO->mGuard.mTurnMove.m.mTurnAnm.mCancelFrame) {
        onModeFlg(4);
        if (!checkNextAction(1)) {
            cLib_chaseF(&mNormalSpeed, 0.0f, 7.5f);
        }
    } else {
        if (frameCtrl_p->checkPass(19.0f)) {
            dComIfGp_getVibration().StartShock(1, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (frameCtrl_p->getFrame() < 18.0f) {
            field_0x2f9d = 4;
        } else {
            cLib_chaseF(&mNormalSpeed, 0.0f, 0.5f);
        }
    }

    return 1;
}

void daAlink_c::backJumpSpeedDec() {
    if (mLinkAcch.ChkGroundHit()) {
        if (dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd)) {
            f32 jump_speed = mNormalSpeed;
            if (dBgS_CheckBWallPoly(mLinkAcch.m_gnd)) {
                jump_speed = 0.0f;
            } else {
                jump_speed = field_0x3478 * cM_scos(field_0x2ff0);
                if (field_0x2ff0 > 0) {
                    jump_speed *= cM_scos(field_0x2ff0);
                }
            }

            if (mNormalSpeed > jump_speed) {
                mNormalSpeed = jump_speed;
            }
        }
    }
}

int daAlink_c::procBackJumpInit(int param_0) {
    u32 isHorseRide = checkHorseRide();
    BOOL is_prev_guardAnm = checkUpperGuardAnime();
    BOOL is_prev_ganonFinish = mProcID == PROC_GANON_FINISH;

    commonProcInit(PROC_BACK_JUMP);

    if (param_0) {
        setSingleAnimeParam(ANM_BACKFLIP, &mpHIO->mCut.mCutDown.m.mRecoverAnm);
        mNormalSpeed = mpHIO->mCut.mCutDown.m.mRecoverSpeedV;
        speed.y = mpHIO->mCut.mCutDown.m.mSpeedV;
        voiceStart(Z2SE_AL_V_TODOME_RETURN);
    } else {
        setSingleAnimeParam(ANM_BACKFLIP, &mpHIO->mBackJump.m.mBackflipAnm);
        mNormalSpeed = mpHIO->mBackJump.m.mBackflipSpeedH;
        speed.y = mpHIO->mBackJump.m.mBackflipSpeedV;
        voiceStart(Z2SE_AL_V_BACKTEN);
    }

    field_0x3478 = mNormalSpeed;

    if (isHorseRide) {
        onModeFlg(0x2000);
        mProcVar0.field_0x3008 = 5;
    } else {
        mProcVar0.field_0x3008 = 0;
    }

    if (is_prev_ganonFinish) {
        shape_angle.y -= -0x8000;
        setOldRootQuaternion(0, -0x8000, 0);
        field_0x2060->getOldFrameTransInfo(0)->mTranslate.z += 55.0f;
        onNoResetFlg3(FLG3_UNK_4000000);
    }

    current.angle.y = shape_angle.y - -0x8000;
    field_0x3198 = 2;

    if (is_prev_guardAnm) {
        onNoResetFlg0(FLG0_UNK_2);
    }

    return 1;
}

int daAlink_c::procBackJump() {
    if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;
    } else {
        offModeFlg(0x2000);
    }

    if (checkCutTurnInput() && mEquipItem == 0x103) {
        field_0x3198 = getCutTurnDirection();
    }

    backJumpSpeedDec();

    if (mLinkAcch.ChkGroundHit() && checkAnmEnd(mUnderFrameCtrl)) {
        procBackJumpLandInit(field_0x3198);
    } else if (mDemo.getDemoMode() != 0x10 &&
               current.pos.y < mLastJumpPos.y - mpHIO->mBackJump.m.mFallHeight)
    {
        procFallInit(2, mpHIO->mBackJump.m.mFallInterpolation);
    }

    checkItemChangeFromButton();
    return 1;
}

int daAlink_c::procBackJumpLandInit(int i_cutDirection) {
    commonProcInit(PROC_BACK_JUMP_LAND);
    setSingleAnimeParam(ANM_BACKFLIP_LAND, &mpHIO->mBackJump.m.mLandAnm);
    mNormalSpeed = 0.0f;

    field_0x2f9d = 4;
    setFootEffectProcType(2);
    onResetFlg1(RFLG1_UNK_30);

    field_0x3198 = i_cutDirection;
    current.angle.y = shape_angle.y;
    field_0x2fcc = 10;
    setStepLandVibration();

    return 1;
}

int daAlink_c::procBackJumpLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (field_0x2fcc != 0) {
        field_0x2fcc = 10;
    }

    onEndResetFlg0(ERFLG0_UNK_8000000);

    if (checkAnmEnd(frameCtrl)) {
        if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_16_e) {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        } else {
            if (!checkAttentionLock()) {
                offNoResetFlg2(FLG2_UNK_8000000);
            }
            checkNextAction(0);
        }
    } else if (frameCtrl->getFrame() > mpHIO->mBackJump.m.mLandAnm.mCancelFrame) {
        if (mEquipItem == 0x103 && (field_0x3198 != 2 || checkCutTurnInput())) {
            return procCutTurnInit(1, field_0x3198);
        }

        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procSlipInit() {
    commonProcInit(PROC_SLIP);
    setSingleAnimeParam(ANM_SLIP, &mpHIO->mMove.m.mSlideAnm);
    mNormalSpeed = speedF * mpHIO->mMove.m.mSlideSpeed;

    field_0x2f9d = 0x40;
    setFootEffectProcType(1);
    mProcVar0.field_0x3008 = 5;

    return 1;
}

int daAlink_c::procSlip() {
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mSlideDeceleration);

    if (checkZeroSpeedF()) {
        if (checkInputOnR()) {
            current.angle.y = shape_angle.y + 0x8000;
            shape_angle.y += 0x100;
            mNormalSpeed = mMaxSpeed * 0.5f;
            procMoveTurnInit(0);
        } else {
            checkNextAction(0);
        }
    } else {
        cXyz sp14;
        cXyz sp8;
        mProcVar0.field_0x3008--;

        if (mLinkAcch.ChkWallHit() || (mGroundCode == 8 && mProcVar0.field_0x3008 == 0)) {
            mNormalSpeed = 0.0f;
            checkNextAction(0);
            return 1;
        }

        if (mNormalSpeed > 2.5f) {
            field_0x2f9d = 0x40;
        }

        sp14 = current.pos;
        sp14.y += 2.5f;

        sp8.set(sp14.x + cM_ssin(current.angle.y) * 50.0f, sp14.y,
                sp14.z + cM_scos(current.angle.y) * 50.0f);
        if (commonLineCheck(&sp14, &sp8) && dBgS_CheckBWallPoly(mLinkLinChk)) {
            mNormalSpeed = 0.0f;
            return checkNextAction(0);
        }

        seStartMapInfoLevel(Z2SE_FN_LINK_SLIP);
    }

    return 1;
}

int daAlink_c::procAutoJumpInit(int param_0) {
    u32 chk_mode_400 = checkModeFlg(0x400);
    BOOL not_front_roll;
    if (mProcID == PROC_FRONT_ROLL) {
        not_front_roll = FALSE;
    } else {
        not_front_roll = TRUE;
    }

    if (checkIronBallWaitAnime()) {
        resetUpperAnime(UPPER_2, -1.0f);
        deleteEquipItem(TRUE, FALSE);
    }

    if (field_0x27f4 != NULL && fopAcM_GetName(field_0x27f4) == PROC_CANOE) {
        return procSmallJumpInit(1);
    }

    if (!commonProcInitNotSameProc(PROC_AUTO_JUMP)) {
        return 0;
    }

    mProcVar0.field_0x3008 = 0;

    if (chk_mode_400 || (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_24_e && mDemo.getParam0() == 1)) {
        if (chk_mode_400) {
            onModeFlg(0x2000);
            mProcVar0.field_0x3008 = 5;
        }

        setSingleAnimeBaseSpeed(ANM_JUMP_LAND, 0.0f,
                                mpHIO->mAutoJump.m.mJumpFallInterpolation);
        field_0x3198 = 0x33;
        current.angle.y = shape_angle.y + 0x8000;
    } else {
        setSingleAnimeParam(ANM_JUMP_START, &mpHIO->mAutoJump.m.mJumpAnm);
        if (!not_front_roll) {
            field_0x2060->initOldFrameMorf(0.0f, 0, 35);
        }

        field_0x3198 = 0x31;
        current.angle.y = shape_angle.y;
    }

    s16 grab_actor_name;
    if (mGrabItemAcKeep.getActor() != NULL) {
        grab_actor_name = fopAcM_GetName(mGrabItemAcKeep.getActor());
    } else {
        grab_actor_name = PROC_ALINK;
    }

    s16 angle = mpHIO->mAutoJump.m.mJumpAngle;
    BOOL isCuccoJump = false;
    field_0x3478 = -10.0f;
    if (grab_actor_name == PROC_NI || grab_actor_name == PROC_NPC_TKJ2) {
        if (grab_actor_name == PROC_NI &&
            ((ni_class*)mGrabItemAcKeep.getActor())->checkGold() == true)
        {
            mMaxSpeed = 30.0f;
        } else {
            if (checkStageName("F_SP115")) {
                mMaxSpeed = 27.0f;
                field_0x3478 = -10.0f;
            } else {
                mMaxSpeed = mpHIO->mAutoJump.m.mCuccoJumpMaxSpeed;
                field_0x3478 = mpHIO->mAutoJump.m.mCuccoFallMaxSpeed;
            }

            angle = mpHIO->mAutoJump.m.mCuccoJumpAngle;
            isCuccoJump = true;
        }
    } else {
        mMaxSpeed = mpHIO->mAutoJump.m.mMaxJumpSpeed;
    }

    if (checkGrabGlide()) {
        offModeFlg(4);
    }

    if (chk_mode_400) {
        speedF = mMaxSpeed * 0.75f;
    } else {
        if (speedF > mMaxSpeed || param_0 ||
            mpHIO->mAutoJump.m.mAlwaysMaxSpeedJump == true)
        {
            speedF = mMaxSpeed;
        } else if (speedF < mpHIO->mAutoJump.m.mMinJumpSpeed) {
            speedF = mpHIO->mAutoJump.m.mMinJumpSpeed;
        }
    }

    mNormalSpeed = speedF * mpHIO->mAutoJump.m.mJumpSpeedRate;
    speed.y = mNormalSpeed * cM_ssin(angle);
    mNormalSpeed *= cM_scos(angle);

    if (isCuccoJump) {
        mNormalSpeed = mpHIO->mAutoJump.m.mCuccoStartSpeed;
    }

    field_0x3588 = l_waitBaseAnime;
    voiceStart(Z2SE_AL_V_JUMP_L);

    offNoResetFlg0(FLG0_UNK_40000);
    mProcVar2.field_0x300c = 0;
    onResetFlg0(RFLG0_AUTO_JUMP_START);
    mFallVoiceInit = false;
    return 1;
}

int daAlink_c::procAutoJump() {
    int direction = getDirectionFromCurrentAngle();

    #if VERSION == VERSION_SHIELD_DEBUG
    if (!checkStageName("F_SP115") && mGrabItemAcKeep.getActor() != NULL) {
        if ((fopAcM_GetName(mGrabItemAcKeep.getActor()) == PROC_NI && ((ni_class*)mGrabItemAcKeep.getActor())->checkGold() != TRUE) ||
            (fopAcM_GetName(mGrabItemAcKeep.getActor()) == PROC_NPC_TKJ2)) 
        {
            mMaxSpeed = mpHIO->mAutoJump.m.mCuccoJumpMaxSpeed;
            field_0x3478 = mpHIO->mAutoJump.m.mCuccoFallMaxSpeed;
        }
    }
    #endif

    if (checkInputOnR() && direction == DIR_BACKWARD) {
        cLib_chaseF(&mNormalSpeed, 0.0f, mStickValue * 0.2f);
    } else if (checkGrabGlide()) {
        cLib_chaseF(&mNormalSpeed, mMaxSpeed, 0.1f);
    } else if (!checkInputOnR()) {
        cLib_chaseF(&mNormalSpeed, 0.0f, 0.1f);
    }

    if (mProcVar0.field_0x3008 != 0) {
        mProcVar0.field_0x3008--;
    } else {
        offModeFlg(0x2000);
    }

    setLandPassiveData();

    if (!checkModeFlg(4) && checkSetItemTrigger(fpcNm_ITEM_HVY_BOOTS)) {
        setHeavyBoots(1);
    }

    if (checkUpperItemActionFly()) {
        return 1;
    }

    if (mLinkAcch.ChkGroundHit()) {
        return checkLandAction(0);
    }

    if (!checkGrabAnime()) {
        dCcD_Cyl* cyl_p = mTgCyls;
        fopAc_ac_c* hit_ac;
        for (int i = 0; i < 3; i++, cyl_p++) {
            hit_ac = cyl_p->GetCoHitAc();
            if (cyl_p->ChkCoHit() && hit_ac != NULL) {
                if (fopAcM_GetName(hit_ac) == PROC_Obj_SwHang) {
                    return procRoofSwitchHangInit(hit_ac);
                }
            }
        }

        if (checkFrontWallTypeAction() || checkCutJumpInFly()) {
            return 1;
        }
    }

    if (field_0x3198 == 0x31 || field_0x3198 == 0x32) {
        field_0x2f99 = 4;
    }

    if (mProcVar2.field_0x300c != 0) {
        setDoStatus(BUTTON_STATUS_LET_GO);

        if (doTrigger()) {
            freeGrabItem();
            onModeFlg(4);
        }

        if (!checkGrabGlide()) {
            setSpecialGravity(mpHIO->mAutoJump.m.mGravity,
                              mpHIO->mAutoJump.m.mMaxFallSpeed, TRUE);
            mProcVar2.field_0x300c = 0;
            resetUpperAnime(UPPER_1, 3.0f);
            field_0x30a0 = 0;
            onModeFlg(4);
        } else {
            field_0x30a0 = 0x1800;

            if (!checkBootsOrArmorHeavy()) {
                mFallHeight = current.pos.y;
                setSpecialGravity(-1.0f, field_0x3478, FALSE);
            } else {
                setSpecialGravity(mpHIO->mAutoJump.m.mGravity,
                                  mpHIO->mAutoJump.m.mMaxFallSpeed * 0.6666667f, FALSE);
            }

            s16 diff;
            if (checkInputOnR() && direction != DIR_BACKWARD) {
                cLib_addCalcAngleS(&current.angle.y, mMoveAngle, 5, 200, 50);
                diff = current.angle.y - shape_angle.y;
                shape_angle.y = current.angle.y;
            } else {
                diff = 0;
            }

            cLib_addCalcAngleS(&field_0x3082, -diff * 7, 10, 1000, 50);
            cLib_addCalcAngleS(&field_0x308a, -diff * 12, 10, 1000, 50);
        }
    }

    setFallVoice();

    f32 temp_f1 = (mFallHeight - current.pos.y) * 0.01f;
    if (field_0x3198 != 0x8C && temp_f1 > mpHIO->mDamage.mDamFall.m.mFallAnmTransitionHeight) {
        setSingleAnimeBaseSpeed(ANM_DMG_FALL, 0.0f, mpHIO->mDamage.mDamFall.m.mFallAnmMorf);
        field_0x3198 = 0x8C;
    } else if (speed.y < -gravity && field_0x2f99 == 4) {
        if (checkGrabGlide()) {
            setSpecialGravity(-1.0f, field_0x3478, FALSE);
            mProcVar2.field_0x300c = 1;
        }

        setSingleAnimeBaseSpeed(ANM_JUMP_LAND, 0.0f,
                                mpHIO->mAutoJump.m.mJumpFallInterpolation);

        if (mProcVar2.field_0x300c != 0) {
            setUpperAnime(dRes_ID_ALANM_BCK_WALKHBS_e, UPPER_1, 1.0f, 0.0f, -1, 3.0f);
        }

        field_0x2f99 = 0x20;
        field_0x3198 = 0x33;
    } else if (field_0x3198 == 0x31 && checkAnmEnd(mUnderFrameCtrl)) {
        setSingleAnimeBaseSpeed(ANM_JUMP, 0.0f, mpHIO->mAutoJump.m.mAirborneInterpolation);
        field_0x3198 = 0x32;
    }

    if (checkGrabGlide() && mWindSpeed.y > 0.0f) {
        speed.y = 0.0f;
    }

    checkItemChangeFromButton();

    return 1;
}

int daAlink_c::procDiveJumpInit() {
    commonProcInit(PROC_DIVE_JUMP);
    setSingleAnimeParam(ANM_DIVE_START, &mpHIO->mAutoJump.m.mDiveAnm);
    deleteEquipItem(TRUE, TRUE);
    setHeavyBoots(0);

    speed.y = mpHIO->mAutoJump.m.mDiveSpeedV;
    mNormalSpeed = mpHIO->mAutoJump.m.mDiveSpeedH;
    gravity = mpHIO->mAutoJump.m.mDiveGravity;

    mProcVar2.field_0x300c = 0;
    current.angle.y = shape_angle.y;
    field_0x33b0 = 92.0f;

    voiceStart(Z2SE_AL_V_JUMP_DIVING);
    return 1;
}

int daAlink_c::procDiveJump() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (mLinkAcch.ChkGroundHit()) {
        return procFrontRollInit();
    }

    if (mProcVar2.field_0x300c == 0) {
        if (checkAnmEnd(frameCtrl)) {
            setSingleAnimeParam(ANM_DIVE, &mpHIO->mAutoJump.m.mDiveConnectAnm);
            mProcVar2.field_0x300c = 1;
            field_0x2f99 = 2;
        }
    } else {
        field_0x2f99 = 2;

        if (frameCtrl->getFrame() >= 3.0f) {
            cLib_chaseF(&mNormalSpeed, 0.0f, 0.5f);
        }
    }

    return 1;
}

int daAlink_c::procRollJumpInit() {
    commonProcInit(PROC_ROLL_JUMP);
    setSingleAnimeBaseSpeed(ANM_ROLL_JUMP, 0.0f,
                            mpHIO->mAutoJump.m.mSpinJumpInterpolation);

    field_0x3478 = mRollJumpSpeedH;
    mNormalSpeed = mRollJumpSpeedH;
    speed.y = mRollJumpSpeedV;

    mPrevAngleY = mRollJumpAngle;
    shape_angle.y = mRollJumpAngle;
    current.angle.y = shape_angle.y;

    mLinkAcch.ClrGroundHit();
    field_0x347c = 0.0f;
    shape_angle.x = 1;

    voiceStart(Z2SE_AL_V_GORONJUMP);
    seStartOnlyReverb(Z2SE_AL_GORON_JUMP_START);
    mProcVar2.field_0x300c = 1;

    return 1;
}

int daAlink_c::procRollJump() {
    if (mLinkAcch.ChkGroundHit()) {
        return checkLandAction(0);
    }

    if (speed.y < 0.0f) {
        procFallInit(3, mpHIO->mAutoJump.m.mSpinJumpFallInterpolation);
    } else {
        s16 old_angle = shape_angle.x;
        shape_angle.x += mpHIO->mAutoJump.m.mSpinJumpRotateSpeed;

        if (shape_angle.x * old_angle <= 0) {
            if (mProcVar2.field_0x300c != 0) {
                seStartOnlyReverb(Z2SE_AL_GORON_JUMP_ROLLING);
                mProcVar2.field_0x300c = 0;
            } else {
                mProcVar2.field_0x300c = 1;
            }
        }

        cLib_chaseF(&field_0x347c,
                    mStickValue * cM_scos(mMoveAngle - shape_angle.y) * mpHIO->mAutoJump.m.mSpinJumpAddSpeed,
                    mpHIO->mAutoJump.m.mSpinJumpAccel);
        mNormalSpeed = field_0x3478 + field_0x347c;
    }

    return 1;
}

int daAlink_c::procFallInit(int param_0, f32 i_morf) {
    if (mDemo.getDemoMode() == daPy_demo_c::DEMO_UNK_17_e) {
        mLinkAcch.SetGrndNone();
        mLinkAcch.OnLineCheckNone();
    }

    u32 var_r3 = checkModeFlg(0x10000) && !dComIfGp_checkPlayerStatus0(0, 8);
    BOOL temp_r30 = mProcID != PROC_CANOE_GETOFF;
    BOOL temp_r29 = mProcID == PROC_ROOF_HANG_FRONT_MOVE;

    if (mProcID == PROC_HORSE_GETOFF) {
        horseGetOffEnd();
    }

    if (!commonProcInitNotSameProc(PROC_FALL)) {
        return 0;
    }

    if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        offNoResetFlg0(FLG0_WATER_IN_MOVE);
    }

    mProcVar5.field_0x3012 = 0;

    if (param_0 == 1) {
        mNormalSpeed = 0.0f;
        speed.y = 0.0f;
        current.angle.y = shape_angle.y;

        if (var_r3) {
            current.pos.x += cM_ssin(shape_angle.y) * 5.0f;
            current.pos.z += cM_scos(shape_angle.y) * 5.0f;
        }
    } else if (param_0 == 4) {
        current.angle.y = field_0x2ffe;
        dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
        setDamagePoint(field_0x318c, FALSE, TRUE, 0);

        onNoResetFlg1(FLG1_THROW_DAMAGE);
        if (mDamageTimer != 0) {
            onModeFlg(8);
        }

        mNormalSpeed = field_0x3408;
        speed.y = field_0x340c;
        mProcVar5.field_0x3012 = 1;
    }

    mProcVar4.field_0x3010 = temp_r30;
    setSingleAnimeBaseSpeed(ANM_JUMP_LAND, 0.0f, i_morf);
    resetBasAnime();

    if (param_0 != 4 && current.angle.y == shape_angle.y && !checkGrabAnime()) {
        if (mNormalSpeed > 1.0f) {
            mProcVar0.field_0x3008 = 2;
        } else {
            mProcVar0.field_0x3008 = 1;
        }
    } else {
        mProcVar0.field_0x3008 = 0;
    }

    if (temp_r29) {
        mProcVar1.field_0x300a = 2;
        field_0x32cc = 1;
    } else {
        mProcVar1.field_0x300a = 7;
        field_0x32cc = 0;
    }

    mProcVar2.field_0x300c = 0;
    mProcVar3.field_0x300e = param_0 == 3;
    offNoResetFlg0(FLG0_UNK_40000);
    mFallVoiceInit = 0;
    return 1;
}

int daAlink_c::procFall() {
    if (mProcVar5.field_0x3012 == 0) {
        cLib_chaseF(&mNormalSpeed, 0.0f, 0.1f);
    } else if (mProcVar5.field_0x3012 > 0) {
        mProcVar5.field_0x3012 = -1;
        return 1;
    }

    setLandPassiveData();
    if (checkUpperItemActionFly()) {
        return 1;
    }

    if (mLinkAcch.ChkGroundHit()) {
        return checkLandAction(mProcVar3.field_0x300e);
    }

    if (dComIfGs_getLife() != 0) {
        if (mProcVar0.field_0x3008 == 1) {
            if (mProcVar1.field_0x300a > 0) {
                mProcVar1.field_0x300a--;
            } else {
                setFrontWallType();

                if ((field_0x32cc != 0 ||
                     ((checkInputOnR() && getDirectionFromShapeAngle() == DIR_FORWARD) || doTrigger())) &&
                    checkFrontWallTypeAction())
                {
                    return 1;
                }
            }
        } else if (mProcVar0.field_0x3008 == 2 && checkFrontWallTypeAction()) {
            return 1;
        }

        setFallVoice();

        f32 tmp_f1 = (mFallHeight - current.pos.y) * 0.0099999998f;
        if (mProcVar2.field_0x300c == 0 && tmp_f1 > mpHIO->mDamage.mDamFall.m.mFallAnmTransitionHeight) {
            setSingleAnimeBaseSpeed(ANM_DMG_FALL, 0.0f,
                                    mpHIO->mDamage.mDamFall.m.mFallAnmMorf);
            mProcVar2.field_0x300c = 1;
        }

        if (current.angle.y == shape_angle.y && checkCutJumpInFly()) {
            return 1;
        }
    }

    if (dComIfGs_getLife() != 0) {
        checkItemChangeFromButton();
    }

    return 1;
}

int daAlink_c::procLandInit(f32 param_0) {
    commonProcInit(PROC_LAND);
    mNormalSpeed = 0.0f;

    setSingleAnimeParam(ANM_JUMP_LAND, &mpHIO->mAutoJump.m.mLandAnm);

    if (checkGrabGlide()) {
        mUnderFrameCtrl[0].setRate(0.5f);
    } else if (checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
        mUnderFrameCtrl[0].setRate(mUnderFrameCtrl[0].getRate() * 0.34999999f);
    }

    field_0x2f9d = 4;
    setFootEffectProcType(4);

    onResetFlg1(RFLG1_UNK_30);
    current.angle.y = shape_angle.y;
    field_0x3478 = param_0;

    return 1;
}

int daAlink_c::procLand() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (checkAnmEnd(frameCtrl)) {
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > mpHIO->mAutoJump.m.mLandAnm.mCancelFrame) {
        checkNextAction(1);
    } else if (frameCtrl->checkPass(4.0f)) {
        int vibmode;
        if (checkBootsOrArmorHeavy()) {
            vibmode = VIBMODE_S_POWER3;
        } else if (checkNoResetFlg0(FLG0_WATER_IN_MOVE) ||
                   field_0x3478 < mpHIO->mDamage.mDamFall.m.mMinRollHeight)
        {
            vibmode = VIBMODE_S_POWER1;
        } else {
            vibmode = VIBMODE_S_POWER2;
        }

        dComIfGp_getVibration().StartShock(vibmode, 15, cXyz(0.0f, 1.0f, 0.0f));
    }

    return 1;
}

int daAlink_c::procSmallJumpInit(int param_0) {
    commonProcInit(PROC_SMALL_JUMP);

    if (param_0 == 2) {
        setSingleAnime(ANM_VJUMP_START, mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm.mSpeed, 6.0f,
                       mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm.mEndFrame,
                       mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm.mInterpolation);
        setJumpMode();
        speed.y = 24.0f;
        voiceStart(Z2SE_AL_V_JUMP_S);
        field_0x2f99 = 15;
        field_0x33b0 = field_0x3588.y;
        field_0x34d4 = l_waitBaseAnime;
    } else {
        setSingleAnimeParam(ANM_VJUMP_START, &mpHIO->mWallHang.mSmallJump.m.mSmallJumpAnm);

        if (param_0 == 1) {
            field_0x3478 = field_0x27f4->current.pos.y + 20.0f;
            if (field_0x3478 < current.pos.y + 30.0f) {
                field_0x3478 = current.pos.y + 30.0f;
            }

            daCanoe_c* canoe_p = (daCanoe_c*)field_0x27f4;
            cXyz sp8(canoe_p->current.pos);
            if (canoe_p->checkTandem()) {
                sp8.x += cM_ssin(canoe_p->shape_angle.y) * 175.0f;
                sp8.z += cM_scos(canoe_p->shape_angle.y) * 175.0f;
            }

            mProcVar2.field_0x300c = cLib_targetAngleY(&current.pos, &sp8);
            mProcVar3.field_0x300e = 1;
            setHeavyBoots(0);
        } else {
            field_0x3478 = field_0x34ec.y;
            mProcVar3.field_0x300e = 0;
        }
    }

    mNormalSpeed = 0.0f;
    field_0x33b0 = l_waitBaseAnime.y;
    field_0x3588 = l_waitBaseAnime;
    return 1;
}

int daAlink_c::procSmallJump() {
    if (checkModeFlg(2)) {
        if (checkCanoeJumpRide()) {
            return 1;
        }

        if (mLinkAcch.ChkGroundHit()) {
            checkLandAction(0);
        } else if (speed.y < -gravity) {
            procFallInit(2, mpHIO->mWallHang.mSmallJump.m.mFallInterpolation);
        } else {
            field_0x2f99 = 7;
        }
    } else if (mUnderFrameCtrl[0].checkPass(6.0f)) {
        setJumpMode();
        mNormalSpeed = mpHIO->mWallHang.mSmallJump.m.mSpeedH;

        if (checkHeavyStateOn(TRUE, TRUE) && !checkNoResetFlg0(FLG0_WATER_IN_MOVE)) {
            f32 temp = mpHIO->mWallHang.mSmallJump.m.mTargetHeightOffset + (field_0x3478 - current.pos.y);  // fakematch
            speed.y = JMAFastSqrt(2.0f * (temp * -(gravity * 2.25f)));
        } else {
            speed.y =
                JMAFastSqrt(2.0f * (-gravity * (mpHIO->mWallHang.mSmallJump.m.mTargetHeightOffset +
                                                 (field_0x3478 - current.pos.y))));
        }

        voiceStart(Z2SE_AL_V_JUMP_S);
        field_0x2f99 = 7;

        if (mProcVar3.field_0x300e != 0) {
            shape_angle.y = mProcVar2.field_0x300c;
            current.angle.y = shape_angle.y;
        }
    } else if (mProcVar3.field_0x300e != 0) {
        cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 3, 0x1000, 0x400);
        current.angle.y = shape_angle.y;
    }

    return 1;
}

int daAlink_c::procStepMoveInit() {
    commonProcInit(PROC_STEP_MOVE);
    setSingleAnimeParam(ANM_S_JUMP_START, &mpHIO->mWallHang.mSmallJump.m.mStepClimbAnm);
    field_0x3478 = 0.5f * mNormalSpeed;
    mNormalSpeed = 0.0f;

    mProcVar2.field_0x300c = 0;
    field_0x3588 = l_waitBaseAnime;
    return 1;
}

int daAlink_c::procStepMove() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (mProcVar2.field_0x300c != 0) {
        field_0x2f99 = 4;

        if (checkAnmEnd(frameCtrl)) {
            return checkNextAction(0);
        }

        if (frameCtrl->getFrame() > mpHIO->mWallHang.mSmallJump.m.mStepLandAnm.mCancelFrame) {
            mNormalSpeed = field_0x3478;

            if (checkNextAction(1)) {
                return 1;
            }

            mNormalSpeed = 0.0f;
        }
    } else if (checkAnmEnd(frameCtrl)) {
        setSingleAnimeParam(ANM_S_JUMP_END, &mpHIO->mWallHang.mSmallJump.m.mStepLandAnm);
        current.pos.set(field_0x34ec.x + cM_ssin(shape_angle.y) * 2.0f, field_0x34ec.y,
                        field_0x34ec.z + cM_scos(shape_angle.y) * 2.0f);
        field_0x2f99 = 80;
        mProcVar2.field_0x300c = 1;
    }

    return 1;
}

int daAlink_c::procCrouchInit() {
    if (!commonProcInitNotSameProc(PROC_CROUCH)) {
        return 0;
    }

    setSingleAnimeBaseSpeed(ANM_CROUCH, mpHIO->mCrouch.m.mCrouchAnmSpeed,
                            mpHIO->mCrouch.m.mCrouchInterpolation);
    current.angle.y = shape_angle.y;
    return 1;
}

int daAlink_c::procCrouch() {
    cLib_chaseF(&mNormalSpeed, 0.0f, mpHIO->mMove.m.mDeceleration);

    if (checkAttentionState()) {
        setFaceBasicBck(dRes_ID_ALANM_BCK_FAT_e);
    } else {
        setFaceBasicBck(0);
    }

    if (mDemo.getDemoMode() != daPy_demo_c::DEMO_CROUCH_e &&
        (mSinkShapeOffset <= -80.0f || (checkAttentionLock() && checkGuardAccept())))
    {
        checkNextActionFromCrouch(0);
    }

    return 1;
}

int daAlink_c::procCoMetamorphoseInit() {
    int var_r29 = 0;

    if (dComIfGp_getEvent()->isOrderOK()) {
        if (!dComIfGp_event_compulsory(this, NULL, 0xFFFF)) {
            return 0;
        }
        mDemo.setSpecialDemoType();
        mDamageTimer = 0;
        var_r29 = 1;
    }

    if (!commonProcInitNotSameProc(PROC_METAMORPHOSE)) {
        return 1;
    }

    field_0x3198 = var_r29;
    mProcVar0.field_0x3008 = 0;
    mProcVar3.field_0x300e = 0;
    mProcVar4.field_0x3010 = 0;
    field_0x347c = 1.0f;
    mFallVoiceInit = 0;

    if ((checkWolf() && mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK1_e) ||
        (!checkWolf() && mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK2_e))
    {
        mProcVar1.field_0x300a = 1;
        speed.y = 0.0f;
        mNormalSpeed = 0.0f;

        if (checkWolf()) {
            mProcVar2.field_0x300c = shape_angle.x;
        }
    } else {
        deleteEquipItem(FALSE, TRUE);

        if (checkEndResetFlg0(ERFLG0_FORCE_WOLF_CHANGE) && checkStageName("D_MN08")) {
            mProcVar4.field_0x3010 = 1;
        }
        mProcVar1.field_0x300a = 0;

        if (checkWolf()) {
            // Transform Wolf -> Human
            setSingleAnimeWolfBase(WANM_TRANSFORM_TO_HUMAN);
            field_0x3588 = l_wolfBaseAnime;
            field_0x347c = 0.5f;
            field_0x3480 = mpHIO->mBasic.m.mWolfToLinkCancelFrame;
            mProcVar2.field_0x300c = shape_angle.x;

            daMidna_c* midna = (daMidna_c*)getMidnaActor();
            if (checkMidnaRide() && daMidna_c::checkMidnaRealBody() && midna->checkDemoTypeNone())
            {
                midna->changeOriginalDemo();
                midna->changeDemoMode(daPy_demo_c::DEMO_UNK_15_e);
            }
        } else {
            // Transform Human -> Wolf
            if (mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK1_e && mDemo.getParam1() == 1) {
                voiceStart(Z2SE_AL_V_TRANSFORM);
            }

            setSingleAnimeBase(ANM_TRANSFORM_TO_WOLF);
            setMetamorphoseModel(TRUE);
            field_0x3588 = l_waitBaseAnime;
            field_0x3480 = mpHIO->mBasic.m.mLinkToWolfCancelFrame;

            cXyz pos(current.pos.x + (cM_ssin(shape_angle.y) * 100.0f), current.pos.y + 300.0f,
                     current.pos.z + (cM_scos(shape_angle.y) * 100.0f));
            mLinkGndChk.SetPos(&pos);

            f32 gnd_cross = dComIfG_Bgsp().GroundCross(&mLinkGndChk);
            if (gnd_cross != -G_CM3D_F_INF) {
                pos.y = gnd_cross;

                cXyz sp14 = current.pos;
                cXyz sp8 = pos;

                BOOL var_r28 = false;
                for (int i = 0; i < 4; i++) {
                    if (i == 3) {
                        sp14.y = gnd_cross + 5.0f;
                    } else {
                        sp14.y = current.pos.y + mAcchCir[i].GetWallH();
                    }
                    sp8.y = sp14.y;

                    if (commonLineCheck(&sp14, &sp8) && dBgS_CheckBWallPoly(mLinkLinChk) &&
                        dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 7)
                    {
                        var_r28 = true;
                        break;
                    }
                }

                if (var_r28) {
                    mProcVar2.field_0x300c = field_0x2ff0;
                } else {
                    pos -= current.pos;
                    mProcVar2.field_0x300c = pos.atan2sY_XZ();

                    if (abs(mProcVar2.field_0x300c) > cM_deg2s(70.0f)) {
                        mProcVar2.field_0x300c = field_0x2ff0;
                    }
                }
            } else {
                mProcVar2.field_0x300c = 0;
            }
        }
        speed.y = 0.0f;
        mNormalSpeed = 0.0f;
        mProcVar5.field_0x3012 = 0;
    }

    field_0x3484 = current.pos.y;
    return 1;
}

int daAlink_c::procCoMetamorphose() {
    if (mProcVar1.field_0x300a != 0) {
        if (!checkEventRun()) {
            checkWaitAction();
        } else {
            dComIfGp_evmng_cutEnd(mAlinkStaffId);
        }
        return 1;
    }

    daPy_frameCtrl_c* framectrl = mUnderFrameCtrl;
    if (!mLinkAcch.ChkGroundHit()) {
        current.pos.y = field_0x3484;
        speed.y = 0.0f;
    } else {
        field_0x3484 = current.pos.y;
    }

    field_0x2f99 = 4;

    if (mProcVar0.field_0x3008 != 0 && mProcVar5.field_0x3012 == 0) {
        if (mClothesChangeWaitTimer == 0) {
            mProcVar5.field_0x3012 = 1;

            if (mDemo.getParam0() == 1 && (mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK1_e || mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK2_e)) {
                dComIfGp_evmng_cutEnd(mAlinkStaffId);

                if (checkWolf()) {
                    setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed, -1.0f);
                } else {
                    setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, -1.0f);
                }

                field_0x2f99 = 0xC;
                mProcVar1.field_0x300a = 1;
                return 1;
            }

            Vec sp8 = {0.0f, 0.0f, 0.0f};
            f32 var_f29 = l_wolfBaseAnime.z - l_waitBaseAnime.z;
            sp8.z = var_f29;

            if (!checkWolf()) {
                setSingleAnimeBaseMorf(ANM_TRANSFORM_TO_HUMAN, -1.0f);
                setMetamorphoseModel(0);
                field_0x3588 = l_waitBaseAnime;
            } else {
                setSingleAnimeWolfBaseMorf(WANM_TRANSFORM_TO_WOLF, -1.0f);
                field_0x3588 = l_wolfBaseAnime;
                sp8.z *= -1.0f;
                offModeFlg(0x8000);
            }

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
            mDoMtx_stack_c::multVec(&sp8, &current.pos);

            field_0x2f99 = 0xC;
            if (mDemo.getDemoType() == daPy_demo_c::DEMO_TYPE_ORIGINAL_e) {
                mFallVoiceInit = 1;
            }

        }
    } else if (checkAnmEnd(framectrl)
                || ((mDemo.getParam0() == 1) && (mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK1_e || mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_UNK2_e))
                || (mProcVar5.field_0x3012 != 0 && framectrl->getFrame() > field_0x3480 && (checkInputOnR() || doTrigger() || grassCancelTrigger())))
    {
        if (mProcVar5.field_0x3012 != 0) {
            deleteEquipItem(FALSE, FALSE);

            if (field_0x3198 != 0) {
                resetSpecialEvent();
                return checkWaitAction();
            }

            dComIfGp_evmng_cutEnd(mAlinkStaffId);

            if (checkWolf()) {
                setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed, -1.0f);
            } else {
                setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, -1.0f);
            }

            field_0x2f99 = 0xC;
            mProcVar1.field_0x300a = 1;
            return 1;
        }

        mClothesChangeWaitTimer = 4;
        mpWlMidnaModel = NULL;
        mProcVar0.field_0x3008 = 1;

        if (mProcVar4.field_0x3010 != 0) {
            dComIfGs_onEventBit(dSv_event_flag_c::F_0776);
        }
    }

    field_0x33dc = framectrl->getFrame();

    f32 var_f31;
    if (mProcVar5.field_0x3012 != 0) {
        if (checkWolf()) {
            var_f31 = 1.0f - (0.03846154f * field_0x33dc);
            if (var_f31 < 0.0f) {
                var_f31 = 0.0f;
            }

            mProcVar3.field_0x300e = -64.0f * var_f31;
        } else if (field_0x33dc < 14.0f) {
            mProcVar3.field_0x300e = 15.0 + (49.0 * (1.0 - (0.07142857142857142 * field_0x33dc)));
        } else {
            var_f31 = 1.0f - (0.2f * (field_0x33dc - 14.0f));
            if (var_f31 < 0.0f) {
                var_f31 = 0.0f;
            }

            mProcVar3.field_0x300e = 15.0f * var_f31;
        }
    } else if (checkWolf() != 0) {
        mProcVar3.field_0x300e = -64.0 * (0.07142857142857142 * field_0x33dc);
    } else if (field_0x33dc < 9.0f) {
        mProcVar3.field_0x300e = -15.0 * (0.1111111111111111 * field_0x33dc);
    } else {
        var_f31 = 0.2f * (field_0x33dc - 9.0f);
        if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        }

        mProcVar3.field_0x300e = -(15.0f + (49.0f * var_f31));
    }

    if (!checkWolf()) {
        f32 var_f30;
        if (mProcVar5.field_0x3012 != 0) {
            var_f30 = (field_0x33dc - 13.0f) / 4.0f;
        } else {
            var_f30 = (field_0x33dc - 8.0f) / 5.0f;
        }

        var_f30 = cLib_minMaxLimit<f32>(var_f30, 0.0f, 1.0f);

        if (mProcVar5.field_0x3012 != 0) {
            field_0x347c = 0.5f + (0.027777778f * field_0x33dc);
            var_f30 = 1.0f - var_f30;
        } else {
            field_0x347c = 1.0f - (0.033333335f * (field_0x33dc - 9.0f));
            if (var_f30 > 0.0f) {
                swimBgCheck(150.0f * var_f30);
            }
        }

        if (field_0x347c > 1.0f) {
            field_0x347c = 1.0f;
        }

        shape_angle.x = var_f30 * mProcVar2.field_0x300c;
    }

    return 1;
}

int daAlink_c::procCoMetamorphoseOnlyInit() {
    if (!commonProcInitNotSameProc(PROC_METAMORPHOSE_ONLY)) {
        return 1;
    }

    deleteEquipItem(FALSE, TRUE);
    if ((checkWolf() && mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_ONLY_UNK1_e) ||
        (!checkWolf() && mDemo.getDemoMode() == daPy_demo_c::DEMO_METAMORPHOSE_ONLY_UNK2_e))
    {
        mProcVar2.field_0x300c = 1;
        mProcVar3.field_0x300e = 1;
    } else {
        mProcVar2.field_0x300c = 0;
        mProcVar3.field_0x300e = 0;
    }

    mNormalSpeed = 0.0f;
    speed.y = 0.0f;
    setSpecialGravity(0.0f, maxFallSpeed, FALSE);

    return 1;
}

int daAlink_c::procCoMetamorphoseOnly() {
    if (mProcVar2.field_0x300c == 0) {
        mClothesChangeWaitTimer = 4;
        mProcVar2.field_0x300c = 1;
    } else if (mClothesChangeWaitTimer == 0) {
        if (mProcVar3.field_0x300e == 0) {
            mProcVar3.field_0x300e = 1;

            if (checkWolf()) {
                setSingleAnimeWolfBaseSpeed(WANM_WAIT, mpHIO->mWolf.mWlMoveNoP.m.mIdleAnmSpeed,
                                            -1.0f);
            } else {
                setSingleAnimeBaseSpeed(ANM_WAIT, mpHIO->mMove.m.mWaitAnmSpeed, -1.0f);
            }
        }

        dComIfGp_evmng_cutEnd(mAlinkStaffId);
    }

    return 1;
}

int daAlink_c::procFloorDownReboundInit() {
    commonProcInit(PROC_FLOOR_DOWN_REBOUND);
    setSingleAnime(ANM_JUMP_LAND, 0.69999999f, mpHIO->mAutoJump.m.mLandAnm.mStartFrame,
                   mpHIO->mAutoJump.m.mLandAnm.mEndFrame, 10.0f);
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    field_0x2f98 = 4;

    return 1;
}

int daAlink_c::procFloorDownRebound() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (checkAnmEnd(frameCtrl)) {
        checkNextAction(0);
    } else if (frameCtrl->getFrame() > mpHIO->mAutoJump.m.mLandAnm.mCancelFrame) {
        checkNextAction(1);
    }

    return 1;
}

int daAlink_c::procGoronRideWaitInit(fopAc_ac_c* i_goronActor) {
    if (!commonProcInitNotSameProc(PROC_GORON_RIDE_WAIT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;
    current.pos.x = i_goronActor->current.pos.x;
    current.pos.z = i_goronActor->current.pos.z;
    field_0x2f99 = 0x50;
    current.angle.y = shape_angle.y;
    setBlendMoveAnime(mpHIO->mBasic.m.mBasicInterpolation);

    return 1;
}

int daAlink_c::procGoronRideWait() {
    if (!checkGoronRide()) {
        checkNextAction(0);
    } else {
        if (checkInputOnR()) {
            cLib_addCalcAngleS(&shape_angle.y, mMoveAngle, 2, 500, 100);
        }

        s16 temp_r0 = shape_angle.y - current.angle.y;
        daAlink_ANM anm;
        if (temp_r0 > 0) {
            anm = ANM_STEP_TURN;
        } else if (temp_r0 < 0) {
            anm = ANM_SMALL_GUARD;
        } else {
            anm = ANM_WAIT;
        }

        f32 blendRate = 0.8f + (0.002f * abs(temp_r0));
        if (blendRate > 1.0f) {
            blendRate = 1.0f;
        }

        blendRate = 1.0f - blendRate;

        setDoubleAnime(blendRate, mpHIO->mBasic.m.mRotationASpeed, mpHIO->mMove.m.mWaitAnmSpeed, anm, ANM_WAIT, 2, -1.0f);
        current.angle.y = shape_angle.y;
    }

    return 1;
}

int daAlink_c::execute() {
    loadModelDVD();

    if (checkEndResetFlg0(ERFLG0_BOSS_ROOM_WAIT) && getMidnaActor() != NULL) {
        getMidnaActor()->onNoServiceWait();
    }

    if (field_0x30fe != 0) {
        f32 var_f26 = 1.0f / (f32)field_0x30fe;
        *mCcStts.GetCCMoveP() = field_0x372c * var_f26;
    }

    if (checkHorseRide() && checkBoarSingleBattle() && dComIfGp_getHorseActor() != NULL) {
        shape_angle.y = dComIfGp_getHorseActor()->shape_angle.y;
        current.angle.y = shape_angle.y;
    } else if (checkMagneBootsOn()) {
        shape_angle.y = field_0x3118;
    } else if (checkModeFlg(MODE_VINE_CLIMB) || checkCargoCarry()) {
        shape_angle.y = field_0x3108;
    }

    #if VERSION == VERSION_SHIELD_DEBUG
    if (checkWolf()) {
        l_autoUpHeight = mpHIO->mWolf.mWlWallHang.m.mAutoWalkHeight + 0.1f;
        field_0x3470 = cM_scos(cM_deg2s(mpHIO->mWolf.mWlSlide.m.mSlidingAngle));
        field_0x3122 = cM_deg2s(mpHIO->mWolf.mWlSlide.m.mClimbingAngle);
    } else {
        l_autoUpHeight = mpHIO->mWallHang.m.auto_walk_height + 0.1f;
        field_0x3470 = cM_scos(cM_deg2s(mpHIO->mSlide.m.mSlideAngle));
        field_0x3122 = cM_deg2s(mpHIO->mSlide.m.mClimbAngle);

        if (checkBoardRide()) {
            mMaxSpeed = mpHIO->mItem.mBoard.m.mMaxSpeed;
        }
    }

    l_autoDownHeight = -l_autoUpHeight;
    #endif

    if (checkNoResetFlg2(FLG2_UNK_1) && mEquipItem != fpcNm_ITEM_KANTERA && checkItemSetButton(fpcNm_ITEM_KANTERA) == 2) {
        offKandelaarModel();
    }

    mSwordChangeWaitTimer = 0;
    setSelectEquipItem(FALSE);

    if (dComIfGp_event_runCheck()) {
        mAlinkStaffId = dComIfGp_evmng_getMyStaffId("Alink", this, 0);

        if (eventInfo.checkCommandDoor() && !dComIfGp_event_chkEventFlag(4) &&
            mEquipItem == 0x102)
        {
            deleteEquipItem(FALSE, FALSE);
        }
    }

    if (mSwordUpTimer != 0) {
        mSwordUpTimer--;
    }

    field_0x317c = dComIfGp_getPlayerCameraID(0);
    field_0x3510 = current.pos;

    if (checkMagneBootsOn()) {
        if (checkNoResetFlg0(FLG0_UNK_2000) && dComIfG_Bgsp().ChkPolySafe(mMagneLineChk) &&
            dComIfG_Bgsp().ChkMoveBG(mMagneLineChk))
        {
            s16 shape_angle_y = shape_angle.y;
            s16 angle_y = current.angle.y;
            f32 pos_y = current.pos.y;

            dComIfG_Bgsp().MoveBgCrrPos(mMagneLineChk, true, &current.pos, &current.angle,
                                        &shape_angle, false, false);

            if (field_0x3114 > 0x6000 || field_0x3114 == -0x8000) {
                shape_angle.y = shape_angle_y - (s16)(shape_angle.y - shape_angle_y);
                current.angle.y = angle_y - (s16)(current.angle.y - angle_y);
            }

            dComIfG_Bgsp().MoveBgCrrPos(mMagneLineChk, true, &old.pos, NULL, NULL, false, false);

            if (getMoveBGActorName(mMagneLineChk, FALSE) == PROC_Obj_MagneArm) {
                field_0x34c8.y += current.pos.y - pos_y;
            } else {
                field_0x34c8.y += (current.pos.y - pos_y) * 0.1f;
            }
        }
    } else if (mProcID != PROC_HOOKSHOT_FLY
                && !checkModeFlg(MODE_VINE_CLIMB | MODE_UNK_800)
                && mProcID != PROC_TOOL_DEMO
                && mLinkAcch.GetGroundH() != -G_CM3D_F_INF
                && dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_gnd)
                && dComIfG_Bgsp().ChkMoveBG(mLinkAcch.m_gnd)
            )
    {
        bool uvar23;
        if (mProcID == PROC_SPINNER_READY) {
            uvar23 = true;
        } else {
            uvar23 = mLinkAcch.ChkGroundHit();
        }
        f32 pos_y = current.pos.y;

        dComIfG_Bgsp().MoveBgCrrPos(mLinkAcch.m_gnd, uvar23, &current.pos, &current.angle,
                                    &shape_angle, false, false);
        dComIfG_Bgsp().MoveBgCrrPos(mLinkAcch.m_gnd, uvar23, &old.pos, NULL, NULL, false, false);
        field_0x34c8.y += (current.pos.y - pos_y) * 0.1f;
    }

    field_0x3798 = old.pos;
    mPrevAngleY = shape_angle.y;
    mPrevStickAngle = mStickAngle;
    field_0x33a4 = mMoveValue;

    if (checkResetFlg0(RFLG0_UNK_8000000) && !checkCutDashAnime()) {
        mCutType = 0;
    }

    mResetFlg0 = 0;
    mResetFlg1 = 0;

    daAlink_matAnm_c::decMorfFrame();
    field_0x2180[0]->offSetFlg();
    field_0x2180[1]->offSetFlg();

    field_0x2f99 = 0;
    field_0x2f9d = 0;
    field_0x2fc1 = 6;

    if (!checkSwordGet() && mEquipItem == 0x103) {
        deleteEquipItem(FALSE, FALSE);
    }

    mUseButtonFlags = 0;

    setDoStatus(BUTTON_STATUS_NONE);
    setRStatus(BUTTON_STATUS_NONE);
    set3DStatus(BUTTON_STATUS_NONE, 0);
    setMidnaTalkStatus(BUTTON_STATUS_NONE);

    if (checkWolf()) {
        setBStatus(BUTTON_STATUS_ATTACK);
        setWolfDigStatus(BUTTON_STATUS_NONE);

        if (dComIfGs_isEventBit(dSv_event_flag_c::F_0550)) {
            setWolfSenceStatus(BUTTON_STATUS_SENSE);
        } else {
            setWolfSenceStatus(BUTTON_STATUS_NONE);
        }
    } else {
        if (checkCanoeRide() && checkStageName("F_SP127")) {
            setBStatus(BUTTON_STATUS_UNK_79);
        } else if (checkSwordGet()) {
            setBStatus(BUTTON_STATUS_UNK_38);
        } else {
            setBStatus(BUTTON_STATUS_NONE);
        }
    }

    field_0x2f91 = 0;

    if (mProcID == PROC_BOTTLE_SWING && eventInfo.checkCommandCatch()) {
        mDemo.setSpecialDemoType();
    }

    field_0x280c.setActor();
    mRideAcKeep.setActor();

    if (checkNoResetFlg2(daPy_FLG2(FLG2_WOLF_ENEMY_HANG_BITE | FLG2_UNK_10 | FLG2_UNK_8))) {
        field_0x281c.setActor();
        if (field_0x281c.getActor() == NULL) {
            offNoResetFlg2(daPy_FLG2(FLG2_WOLF_ENEMY_HANG_BITE | FLG2_UNK_10 | FLG2_UNK_8));
        }
    } else {
        field_0x281c.clearData();
    }

    field_0x2844.setActor();
    mThrowBoomerangAcKeep.setActor();
    mCopyRodAcKeep.setActor();

    if (mCopyRodAcKeep.getActor() != NULL) {
        static_cast<daCrod_c*>(mCopyRodAcKeep.getActor())->setControllActorData();
    }

    mGrabItemAcKeep.setActor();
    mHookTargetAcKeep.setActor();
    field_0x285c.setActor();

    if (checkFishingRodAndLureItem() && mItemAcKeep.getActor() == NULL) {
        fopAc_ac_c* item_actor = fopAcM_SearchByID(mItemAcKeep.getID());
        if (item_actor != NULL) {
            mItemAcKeep.setData(item_actor);
        }
    } else {
        mItemAcKeep.setActor();
    }

    if (checkWolf()) {
        checkWolfLockData();
    }

    setAtnList();
    setDemoData();
    setStickData();

    if (mFastShotTime < 0) {
        mFastShotTime = 0;
    }

    checkOxygenTimer();

    if (field_0x2fce != 0) {
        field_0x2fce--;
    }

    if (field_0x2fcd != 0) {
        field_0x2fcd--;
    }

    if (field_0x3000 != 0) {
        field_0x3000--;
    }

    if (field_0x30fc != 0) {
        field_0x30fc--;
    }

    if (field_0x2fb4 != 0) {
        field_0x2fb4--;
    }

    if (field_0x30a4 != 0) {
        field_0x30a4--;
    }

    if (field_0x30a6 != 0) {
        field_0x30a6--;
    }

    if (field_0x30ec != 0) {
        field_0x30ec--;
    }

    if (mSwordFlourishTimer != 0) {
        mSwordFlourishTimer--;
    }

    if (field_0x3100 != 0) {
        field_0x3100--;
    }

    if (field_0x2fc4 != 0) {
        field_0x2fc4--;
    }

    if (field_0x30d2 != 0) {
        field_0x30d2--;
    }

    if (field_0x30d0 != 0) {
        field_0x30d0--;
    } else {
        offNoResetFlg1(FLG1_DASH_MODE);
    }

    if (field_0x2fd2 != 0) {
        field_0x2fd2--;
    }

    if (field_0x2fd3 != 0 && mProcID != PROC_BOTTLE_DRINK && mProcID != PROC_CANOE_BOTTLE_DRINK &&
        mProcID != PROC_HORSE_BOTTLE_DRINK)
    {
        field_0x2fd3--;
    }

    if (field_0x2fd4 != 0 && mProcID != PROC_GUARD_SLIP) {
        field_0x2fd4--;

        if (field_0x2fd4 == 0) {
            field_0x2fd5 = 0;
        }
    }

    if (checkWolfEyeUp()) {
        fopAc_ac_c* partner = (fopAc_ac_c*)fopAcM_getTalkEventPartner(this);

        const char* event_name;
        if (checkEventRun()) {
            event_name = dComIfGp_getEventManager().getRunEventName();
        } else {
            event_name = NULL;
        }

        if (event_name != NULL &&
            (strcmp(event_name, "kiko_yami") == 0
                || strcmp(event_name, "fire_insect") == 0
                || (checkStageName("F_SP108") && (strcmp(event_name, "MapToolCamera19") == 0 || strcmp(event_name, "yami_mog") == 0))
                || (checkStageName("R_SP109") && (strcmp(event_name, "MapToolCamera1") == 0
                                                                || strcmp(event_name, "MapToolCamera2") == 0
                                                                || strcmp(event_name, "MapToolCamera19") == 0
                                                                || strcmp(event_name, "fire_insect") == 0
                                                                )
                )
                || (checkStageName("F_SP115") && strcmp(event_name, "MapToolCamera23") == 0)
            )
            )
        {
            mWolfEyeUp = mpHIO->mWolf.m.mSensesLingerTime + 1;
        } else if (checkEndResetFlg1(ERFLG1_WOLF_EYE_KEEP)
            || (checkDungeon() && eventInfo.checkCommandDoor())
            || mProcID == PROC_WOLF_GET_SMELL
            || mProcID == PROC_WOLF_DIG
            || mProcID == PROC_WOLF_DIG_THROUGH
            || checkNoResetFlg0(FLG0_UNK_4000)
            || dComIfGp_checkPlayerStatus1(0, 0x1000000)
            || (checkEventRun() && partner != NULL && (partner->attention_info.flags & fopAc_AttnFlag_UNK_0x400000))
            || strcmp(dComIfGp_getEventManager().getRunEventName(), l_defaultGetEventName) == 0)
        {
            mWolfEyeUp = mpHIO->mWolf.m.mSensesLingerTime;
        } else if (mTargetedActor != NULL || dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            mWolfEyeUp = mpHIO->mWolf.m.mSensesLingerTime - 1;
        } else if (!dComIfGp_getEvent()->isOrderOK() && mProcID != PROC_GET_ITEM &&
                   mWolfEyeUp <= mpHIO->mWolf.m.mSensesLingerTime)
        {
            offWolfEyeUp();
        } else {
            mWolfEyeUp = 1;
        }
    }

    if (checkEndResetFlg0(ERFLG0_ENEMY_DEAD) && mEquipItem == 0x103) {
        mSwordFlourishTimer = mpHIO->mCut.m.mFlourishTime;
    }

    if ((checkWolf() && field_0x2fbc == 11 && checkWaterPolygonUnder()) || mGndPolyAtt0 == 11) {
        onNoResetFlg0(FLG0_HEAVY_STATE);
    }

    fopAc_ac_c* item_actor = mGrabItemAcKeep.getActor();
    if (item_actor != NULL && (!fopAcM_checkCarryNow(item_actor) ||
                               (checkNoResetFlg0(FLG0_WATER_IN_MOVE) &&
                                (checkGrabRooster() || fopAcM_GetName(item_actor) == PROC_E_NEST ||
                                 checkGrabCarryActor()))))
    {
        freeGrabItem();
        item_actor = NULL;
    }

    BOOL isTrigDebugMoveInput = FALSE;
    #if DEBUG
    if (daPy_getPlayerActorClass() == this && checkDebugMoveInput()) {
        isTrigDebugMoveInput = TRUE;
        if (l_debugMode) {
            l_debugMode = FALSE;
        } else {
            l_debugMode = TRUE;
        }
    }

    if (l_debugMode) {
        if (checkModeFlg(0x400) && !checkBoardRide() && !checkSpinnerRide()) {
            if (checkCanoeRide()) {
                setSyncCanoePos();
            } else {
                setSyncRide(0);
            }
        } else {
            f32 moveSpeed;
            if (mDoCPd_c::getHoldLockR(PAD_1)) {
                moveSpeed = 100.0f;
            } else {
                moveSpeed = 50.0f;
            }

            if (mDoCPd_c::getHoldY(PAD_1)) {
                current.pos.y += moveSpeed;
            } else if (mDoCPd_c::getHoldX(PAD_1)) {
                current.pos.y -= moveSpeed;
            }

            current.pos.x += moveSpeed * mStickValue * cM_ssin(mMoveAngle);
            current.pos.z += moveSpeed * mStickValue * cM_scos(mMoveAngle);
        }

        setMatrix();
        mpLinkModel->calc();

        if (!checkWolf()) {
            setItemMatrix(0);
        } else {
            setWolfItemMatrix();
        }

        setBodyPartPos();
        setAttentionPos();
    } else
    #endif
    {
        if (isTrigDebugMoveInput) {
            mItemButton = 0;
            mItemTrigger = 0;
        }

        allAnimePlay();

        if (mDamageTimer != 0) {
            damageTimerCount();
        }

        if (checkEquipHeavyBoots()) {
            int itemButton = checkItemSetButton(fpcNm_ITEM_HVY_BOOTS);
            if (itemButton == 2 || checkNotHeavyBootsStage()) {
                if (!dComIfGp_checkPlayerStatus1(0, 0x10000) || !checkHookshotRoofLv7Boss()) {
                    setHeavyBoots(0);
                }
            } else {
                dMeter2Info_onDirectUseItem(itemButton);
            }
        }

        checkWarpStart();

        if (!checkSpecialDemoMode() && mClothesChangeWaitTimer == 0) {
            if (!checkDemoAction() && !checkDeadAction(0) && !checkNoResetFlg0(FLG0_UNK_100000) &&
                !checkAutoJumpAction())
            {
                checkSwimAction(0);
            }

            if (checkDamageAction() && mProcID == PROC_LARGE_DAMAGE && checkMagneBootsOn()) {
                cancelMagneBootsOn();
            }

            checkBoomerangCatchAction();
        }

        offNoResetFlg1(FLG1_UNK_20);

        if (checkWolf()) {
            checkWolfComboCnt();

            if (checkWolfEnemyThrowAnime()) {
                checkWolfEnemyThrowAction();
            }
        } else {
            checkItemButtonChange();
            setItemAction();
            checkComboCnt();
            setShieldGuard();

            if (checkCutFastReady()) {
                if (checkNoUpperAnime()) {
                    setUpperAnime(dRes_ID_ALANM_BCK_WAITATOS_e, UPPER_2, mpHIO->mCut.m.mEquipAnm.mSpeed, 12.0f,
                                mpHIO->mCut.m.mEquipAnm.mEndFrame,
                                mpHIO->mCut.m.mEquipAnm.mInterpolation);
                    onNoResetFlg0(FLG0_UNK_1000000);
                }
            } else if (checkNoResetFlg0(FLG0_UNK_1000000)) {
                resetUpperAnime(UPPER_2, 5.0f);
            }

            if (mEquipItem == fpcNm_ITEM_KANTERA && checkNoUpperAnime() && !checkKandelaarEquipAnime() &&
                (checkModeFlg(MODE_UNK_1000) || mProcID == PROC_CROUCH))
            {
                if (checkReinRide()) {
                    setUpperAnimeBase(dRes_ID_ALANM_BCK_WAITHK_e);
                } else {
                    setUpperAnimeBase(dRes_ID_ALANM_BCK_WAITK_e);
                }

                mNowAnmPackUpper[2].setRatio(0.0f);
            }
        }

        checkLightSwordMtrl();
        (this->*mpProcFunc)();

        if (!checkEndResetFlg0(ERFLG0_UNK_2000) && checkWindDashAnime()) {
            resetUpperAnime(UPPER_2, 3.0f);
        }

        playFaceTextureAnime();

        {
            cXyz old_pos = current.pos;
            posMove();

            if (checkWolf()) {
                wolfBgCheck();
            } else if (checkModeFlg(MODE_SWIMMING)) {
                swimBgCheck(120.0f);
            }

            cXyz pos = current.pos;
            field_0x3528 = speed;

            mLinkAcch.ClrGroundHit();
            mLinkAcch.CrrPos(dComIfG_Bgsp());

            if (checkMagneBootsOn()) {
                current.pos = pos;
            }

            if (!mLinkAcch.ChkGroundHit() && !mLinkAcch.ChkRoofHit() && getZoraSwim()) {
                current.pos.y = pos.y;
            }

            iceSlipBgCheck();
            setWaterY();
            autoGroundHit();

            if (checkNoResetFlg0(FLG0_WATER_IN_MOVE) ||
                (checkPossibleWaterInMode() && !checkNoResetFlg0(FLG0_SWIM_UP)))
            {
                f32 var_f31;
                if (mProcID == PROC_HOOKSHOT_FLY) {
                    var_f31 = current.pos.y - (mHeight * 0.5f);
                } else if (dComIfGp_checkPlayerStatus1(0, 0x2000000)) {
                    var_f31 = (current.pos.y + -65.0f) - (mpHIO->mSwim.m.mStartHeight - 5.0f);
                } else {
                    var_f31 = current.pos.y;
                }

                if (!checkNoResetFlg0(FLG0_UNK_80) ||
                    mWaterY - var_f31 <= mpHIO->mSwim.m.mStartHeight - 5.0f)
                {
                    swimOutAfter(0);
                }
            }

            if (mProcID == PROC_TOOL_DEMO || mProcID == PROC_GANON_FINISH) {
                J3DTransformInfo ti;
                field_0x1f20->getAnm(0)->getTransform(0, &ti);
                mDoMtx_stack_c::transS(old_pos);
                mDoMtx_stack_c::YrotM(shape_angle.y);

                Vec tmp = {0.0f, 0.0f, 0.0f};
                tmp.x = ti.mTranslate.x;
                tmp.z = ti.mTranslate.z;
                mDoMtx_stack_c::multVec(&tmp, &current.pos);
                speed.y = 0.0f;

                if (field_0x3198 != 0) {
                    if (mLinkAcch.GetGroundH() != -G_CM3D_F_INF) {
                        current.pos.y = mLinkAcch.GetGroundH();
                    }
                }
            } else if (mProcID == PROC_HOOKSHOT_FLY
                        || mProcID == PROC_MAGNE_BOOTS_FLY
                        || (checkModeFlg(MODE_ROPE_WALK | MODE_VINE_CLIMB | MODE_UNK_800 | MODE_RIDING)
                            && mProcID != PROC_HORSE_GETOFF
                            && mProcID != PROC_WOLF_CHAIN_READY
                            && !checkBoardRide()
                            )
                        )
            {
                current.pos = pos;
            } else if (checkModeFlg(MODE_UNK_4000)) {
                if (mProcID == PROC_DOOR_OPEN || mProcID == PROC_HANG_LEVER_DOWN) {
                    current.pos.y = old_pos.y;
                    speed.y = 0.0f;
                }
                current.pos.x = pos.x;
                current.pos.z = pos.z;
            } else if (checkFlyAtnWait() || mProcID == PROC_WARP) {
                current.pos.y = old_pos.y;
                speed.y = 0.0f;
            }

            field_0x3178 = mGroundCode;

            if (mLinkAcch.GetGroundH() != -G_CM3D_F_INF) {
                setRoomInfo();

                if (!checkModeFlg(MODE_PLAYER_FLY)
                    && !checkMagneBootsOn()
                    && mProcID != PROC_TOOL_DEMO
                    && mProcID != PROC_GANON_FINISH
                    && mProcID != PROC_DUNGEON_WARP
                    && (field_0x2f8c == 1 || field_0x2f8c == 3))
                {
                    setStepsOffset();
                } else {
                    field_0x2b9c = 0.0f;
                    if (!checkModeFlg(MODE_SWIMMING)) {
                        field_0x2b98 = 0.0f;
                    }
                }

                if (checkModeFlg(MODE_UNK_2000000)) {
                    setShapeAngleOnGround();
                }

                if (!checkRestartRoom() && !checkDeadHP()) {
                    checkSceneChange(dComIfG_Bgsp().GetExitId(mLinkAcch.m_gnd));
                }

                if (mLinkAcch.ChkGroundHit()) {
                    if (dComIfG_Bgsp().ChkMoveBG(mLinkAcch.m_gnd)) {
                        dComIfG_Bgsp().MoveBgTransPos(mLinkAcch.m_gnd, true, &field_0x34ec, NULL, NULL);
                    }

                    if (checkNoResetFlg0(FLG0_UNK_80) && !checkNoResetFlg0(FLG0_WATER_IN_MOVE) &&
                        mWaterY >= mLinkAcch.GetGroundH())
                    {
                        mPolySound = dKy_pol_sound_get(&mLinkAcch.m_wtr);
                    } else if (checkEndResetFlg0(ERFLG0_LEAF_SE)) {
                        mPolySound = 11;
                    } else {
                        mPolySound = dKy_pol_sound_get(&mLinkAcch.m_gnd);
                    }

                    field_0x2ff0 = getGroundAngle(&mLinkAcch.m_gnd, shape_angle.y);
                } else {
                    if (mProcID == PROC_TOOL_DEMO || mProcID == PROC_GANON_FINISH) {
                        if (checkNoResetFlg0(FLG0_UNK_80) && mWaterY >= mLinkAcch.GetGroundH()) {
                            mPolySound = dKy_pol_sound_get(&mLinkAcch.m_wtr);
                        } else if (checkEndResetFlg0(ERFLG0_LEAF_SE)) {
                            mPolySound = 11;
                        } else {
                            mPolySound = dKy_pol_sound_get(&mLinkAcch.m_gnd);
                        }
                    } else if (checkModeFlg(MODE_SWIMMING) && checkNoResetFlg0(FLG0_UNK_80)) {
                        mPolySound = dKy_pol_sound_get(&mLinkAcch.m_wtr);
                    } else {
                        mPolySound = 0;
                    }

                    field_0x2ff0 = 0;
                }
                mDoAud_setLinkGroupInfo(dComIfG_Bgsp().GetGrpSoundId(mLinkAcch.m_gnd));
            } else {
                mGroundCode = -1;
                field_0x2ff0 = 0;
                mGndPolyAtt0 = 16;
                mGndPolyAtt1 = 0;
                mGndPolySpecialCode = dBgW_SPCODE_NORMAL;

                if (checkModeFlg(MODE_SWIMMING) && dComIfG_Bgsp().ChkPolySafe(mLinkAcch.m_wtr)) {
                    mPolySound = dKy_pol_sound_get(&mLinkAcch.m_wtr);
                } else {
                    mPolySound = 0;
                }

                if (!checkRestartRoom() && !checkDeadHP()) {
                    checkSceneChange(0x3F);
                }
            }

            if (checkMagneBootsOn()) {
                mGndPolyAtt1 = 0;
                mMagneBootsPlight.mPosition = current.pos;
                mMagneBootsPlight.mPow = 100.0f;

                if (checkNoResetFlg0(FLG0_UNK_2000)) {
                    mPolySound = dKy_pol_sound_get(&mMagneLineChk);
                    field_0x2ff0 = 0;
                    mGroundCode = dComIfG_Bgsp().GetGroundCode(mMagneLineChk);
                    mGndPolyAtt0 = dComIfG_Bgsp().GetPolyAtt0(mMagneLineChk);
                    mGndPolySpecialCode = dComIfG_Bgsp().GetSpecialCode(mMagneLineChk);
                    setMagneBootsMtx(&mMagneLineChk, 0);
                } else {
                    mPolySound = 0;
                    field_0x2ff0 = 0;
                    mGroundCode = -1;
                    mGndPolyAtt0 = 16;
                    mGndPolySpecialCode = dBgW_SPCODE_NORMAL;
                }
            } else {
                mMagneBootsPlight.mPow = 0.0f;
                if (mProcID != PROC_MAGNE_BOOTS_FLY) {
                    mMagneBootsTopVec = cXyz::BaseY;
                }
            }

            if (dComIfGp_evmng_startCheck(l_peepEventName)) {
                if (field_0x2ff2 > 0) {
                    field_0x2ff2--;
                } else if (dComIfGp_evmng_startCheck(l_peepEventName)) {
                    startPeepChange();
                }
            }

            if (checkModeFlg(MODE_VINE_CLIMB) && mProcVar2.field_0x300c != 0 &&
                dComIfG_Bgsp().ChkPolySafe(mPolyInfo1))
            {
                mZ2Link.framework(dKy_pol_sound_get(&mPolyInfo1), mVoiceReverbIntensity);
            } else {
                mZ2Link.framework(mPolySound, mVoiceReverbIntensity);
            }

            if (field_0x2d7c != NULL) {
                mZ2Link.updateAnime(field_0x2d7c->getFrame(), field_0x2d7c->getRate());
            }

            if (mProcID != PROC_DAMAGE && mProcID != PROC_SWIM_DAMAGE && mProcID != PROC_HORSE_DAMAGE) {
                setMoveSlantAngle();

                if (checkReinRide() && checkUpperGuardAnime()) {
                    setBodyAngleRideReadyAnime();
                } else if (!checkSmallUpperGuardAnime() && !checkWolf()) {
                    if (!checkModeFlg(MODE_UNK_20000000)) {
                        if (checkUpperGuardAnime()) {
                            setBodyAngleXReadyAnime(0);
                        } else if (!checkUpperReadyThrowAnime()) {
                            cLib_addCalcAngleS(&mBodyAngle.x, 0, 4, 0xC00, 0x180);
                        }
                    }

                    if (!checkModeFlg(MODE_UNK_40000000)) {
                        cLib_addCalcAngleS(&mBodyAngle.y, 0, 4, 0xC00, 0x180);
                    }
                }
            }

            setMatrix();
            setNeckAngle();

            if (field_0x2fcc != 0) {
                field_0x2fcc--;
            }

            if (!checkWolf()) {
                setHatAngle();

                if (field_0x2fb0 != 0) {
                    field_0x2fb0--;
                }

                footBgCheck();
                handBgCheck();

                field_0x30c8 = mBodyAngle.y >> 1;
                modelCalc(mpLinkModel);

                if (field_0x2fcb != 0) {
                    if (checkWoodShieldEquip() && mWaterY < mShieldModel->getBaseTRMtx()[1][3]) {
                        field_0x2fcb--;

                        if (field_0x2fcb == 0) {
                            dMeter2Info_setShield(fpcNm_ITEM_NONE, true);
                            stickArrowIncrement(1);
                            setWoodShieldBurnOutEffect();

                            // "Your shield burned up..."
                            dMeter2Info_setFloatingMessage(2047, 90, false);

                            if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) == dStage_SaveTbl_LV2 &&
                                !dComIfGs_isItemFirstBit(fpcNm_ITEM_HYLIA_SHIELD))
                            {
                                fopAcM_onSwitch(this, 0x6F);
                            }
                            seStartOnlyReverb(Z2SE_AL_WOOD_SHIELD_BURN);
                        }
                    } else {
                        clearWoodShieldBurnEffect();
                    }
                }

                if (checkShieldGet() && !checkWoodShieldEquip()) {
                    stickArrowIncrement(1);
                }
                setItemMatrix(0);
            } else {
                daMidna_c* pmidna = (daMidna_c*)getMidnaActor();

                if (mClothesChangeWaitTimer == 0) {
                    if ((pmidna->checkMidnaTired() || pmidna->checkForceTiredColor()) &&
                        !pmidna->checkForceNormalColor())
                    {
                        mpWlMidnaModel->getModelData()->entryTevRegAnimator(mpDMidnaBrk);
                        mpWlMidnaMaskModel->getModelData()->entryTevRegAnimator(mpDMidnaMaskBrk);
                        mpWlMidnaHandModel->getModelData()->entryTevRegAnimator(mpDMidnaHandBrk);
                        mpWlMidnaHairModel->getModelData()->entryTevRegAnimator(mpDMidnaHairHandBrk);
                    } else {
                        mpWlMidnaModel->getModelData()->removeTevRegAnimator(mpDMidnaBrk);
                        mpWlMidnaMaskModel->getModelData()->removeTevRegAnimator(mpDMidnaMaskBrk);
                        mpWlMidnaHandModel->getModelData()->removeTevRegAnimator(mpDMidnaHandBrk);
                        mpWlMidnaHairModel->getModelData()->removeTevRegAnimator(mpDMidnaHairHandBrk);
                    }
                }

                wolfFootBgCheck();
                setWolfTailAngle();
                modelCalc(mpLinkModel);
                checkWolfUseAbility();

                if (checkWolfEyeUp() != 0) {
                    mWolfEyeUpTimer++;
                    if (mWolfEyeUpTimer > 30) {
                        mWolfEyeUpTimer = 30;
                    }
                } else if (mWolfEyeUpTimer != 0) {
                    mWolfEyeUpTimer--;
                }

                setWolfItemMatrix();
            }

            if (checkFmChainGrabAnime()) {
                mNowAnmPackUpper[2].setRatio(1.0f);
            }

            setBodyPartPos();

            if (!checkWolf() && checkEndResetFlg0(ERFLG0_UNK_800000)) {
                setHatAngle();
                modelCalc(mpLinkModel);
            }

            setHangWaterY();
            setCollision();
            setAttentionPos();
            setLight();
            setEffect();

            if (mClothesChangeWaitTimer != 0) {
                mDoMtx_stack_c::copy(mpLinkModel->getBaseTRMtx());

                if (!checkWolf()) {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, 30.0f);
                }

                mpWlChangeModel->setBaseTRMtx(mDoMtx_stack_c::get());
                mpWlChangeModel->calc();
            }

            mSight.update();
            setGrabItemPos();

            checkRoofRestart();
            mExitID = 0x3F;
            mpScnChg = NULL;

            static Vec const localBoomCatchPos = {18.7f, 146.3f, 69.2f};
            cMtx_multVec(mpLinkModel->getAnmMtx(0), &localBoomCatchPos, &field_0x3534);

            if (checkResetFlg0(RFLG0_UNK_10)) {
                mZ2Link.setResumeAttack(true);
            } else {
                mZ2Link.setResumeAttack(false);
            }

            if (!checkEventRun() && checkNoResetFlg0(FLG0_UNK_80) &&
                (mWaterY > current.pos.y + 40.0f || checkModeFlg(MODE_SWIMMING)) &&
                getMoveBGActorName(mLinkAcch.m_wtr, TRUE) == PROC_Obj_Onsen)
            {
                if (mHotspringRecoverTimer != 0) {
                    mHotspringRecoverTimer--;
                } else {
                    mHotspringRecoverTimer = mpHIO->mBasic.m.mHotspringRecoverTime;
                    dComIfGp_setItemLifeCount(1.0f, 1);
                }
            } else {
                mHotspringRecoverTimer = mpHIO->mBasic.m.mHotspringRecoverTime;
            }

            if (checkAttentionLock() || (!checkUpperReadyThrowAnime() &&
                mProcID != PROC_HOOKSHOT_WALL_SHOOT && mProcID != PROC_HOOKSHOT_ROOF_SHOOT))
            {
                mFastShotTime = 0;
            }

            if (checkDeadHP()) {
                eventInfo.offCondition(fopAcCnd_NOEXEC_e);
            } else
            #if DEBUG
            if (!l_debugMode)
            #endif
            {
                if (!checkMagneBootsOn()) {
                    f32 gnd_nrm_y;
                    if (mLinkAcch.ChkGroundHit()) {
                        cM3dGPla tripla;
                        dComIfG_Bgsp().GetTriPla(mLinkAcch.m_gnd, &tripla);
                        gnd_nrm_y = tripla.mNormal.y;
                    } else {
                        gnd_nrm_y = -1.0f;
                    }

                    if (cBgW_CheckBGround(gnd_nrm_y) && !checkModeFlg(MODE_PLAYER_FLY)) {
                        eventInfo.onCondition(dEvtCnd_40_e | dEvtCnd_10_e | dEvtCnd_CANGETITEM_e | dEvtCnd_CANDOOR_e | dEvtCnd_CANTALK_e);
                    } else if (checkModeFlg(MODE_SWIMMING) || mProcID == PROC_WOLF_ROPE_MOVE ||
                            ((checkReinRide() || checkCanoeRide() || checkSpinnerRide()) &&
                                !checkNoResetFlg3(FLG3_UNK_80000) && !checkModeFlg(MODE_JUMP)))
                    {
                        eventInfo.onCondition(dEvtCnd_CANTALK_e);
                        if (checkReinRide() || checkCanoeRide()) {
                            eventInfo.onCondition(dEvtCnd_CANGETITEM_e);
                        }
                    }
                } else if (checkMagneBootsOn() && cBgW_CheckBGround(mMagneBootsTopVec.y)) {
                    eventInfo.onCondition(dEvtCnd_CANTALK_e);
                }
            }

            if (checkEndResetFlg1(ERFLG1_UNK_40000) && checkMidnaRide() && checkWolf()) {
                getMidnaActor()->onForcePanic();
            }

            field_0x30fe = 0;
            field_0x372c = cXyz::Zero;
            field_0x2fb8 = 0;

            if (checkMagicArmorWearAbility() && mClothesChangeWaitTimer == 0) {
                if (checkMagicArmorNoDamage() && !checkEventRun()) {
                    if (field_0x2fc3 == 0) {
                        field_0x2fc3 = 10;
                        dComIfGp_setItemRupeeCount(-1);
                    } else {
                        field_0x2fc3--;
                    }
                }

                if (dComIfGs_getRupee() == 0 && field_0x2fd7 != 0) {
                    setMagicArmorBrk(0);
                    seStartOnlyReverb(Z2SE_AL_M_ARMER_TURNOFF);
                    mZ2Link.setLinkState(5);
                } else if (dComIfGs_getRupee() != 0 && field_0x2fd7 == 0) {
                    setMagicArmorBrk(1);
                    seStartOnlyReverb(Z2SE_AL_M_ARMER_RECOVER);
                    mZ2Link.setLinkState(4);
                }
            }

            if (!checkWolf()) {
                for (u8 i = 0; i < 2; i++) {
                    u8 tmp = (i + 1) % 2;
                    if (dComIfGp_getSelectItem(i) == fpcNm_ITEM_EMPTY_BOTTLE && (mUseButtonFlags & (1 << i)) &&
                        dComIfGp_getSelectItem(tmp) == fpcNm_ITEM_EMPTY_BOTTLE)
                    {
                        mUseButtonFlags |= (u8)(1 << tmp);
                    }
                }
            }

            for (int i = 0; i < 2; i++) {
                if (!(mUseButtonFlags & (1 << i)) && !(field_0x2faf & (1 << i))) {
                    dMeter2Info_offUseButton(METER2_USEBUTTON_X << i);
                }
            }

            if (!(mUseButtonFlags & BTN_R) && !(field_0x2faf & BTN_R)) {
                dMeter2Info_offUseButton(METER2_USEBUTTON_R);
            }

            if (!(mUseButtonFlags & BTN_B) && !(field_0x2faf & BTN_B)) {
                dMeter2Info_offUseButton(METER2_USEBUTTON_B);

                #if PLATFORM_GCN
                if (getSumouMode()) {
                    dMeter2Info_offUseButton(METER2_USEBUTTON_A);
                }
                #endif
            }

            field_0x2faf = mUseButtonFlags;

            for (int i = 0; i < 4; i++) {
                cXyz tmp(current.pos.x + 500.0f, current.pos.y + 500.0f, current.pos.z + 500.0f);
                if (i != field_0x2fa3 - 1) {
                    field_0x354c[i] = tmp;
                }
            }

            if (checkEventRun()) {
                if (mProcID != PROC_GRASS_WHISTLE_WAIT) {
                    setDoStatus(BUTTON_STATUS_NONE);
                }
                setRStatus(BUTTON_STATUS_NONE);
            } else {
                if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_134 || dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_139) {
                    setDoStatus(BUTTON_STATUS_ATTACK);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_137) {
                    setDoStatus(BUTTON_STATUS_CHECK);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_147) {
                    setDoStatusEmphasys(BUTTON_STATUS_JUMP);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_142) {
                    setDoStatusEmphasys(BUTTON_STATUS_GRAB);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_140) {
                    setDoStatus(BUTTON_STATUS_NONE);
                } else if (checkStageName("F_SP127") && checkCanoeRide() && dComIfGp_getDoStatus() == BUTTON_STATUS_PUT_AWAY) {
                    setDoStatus(BUTTON_STATUS_NONE);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_144) {
                    setDoStatus(BUTTON_STATUS_NONE);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_152) {
                    setDoStatusEmphasys(BUTTON_STATUS_UNK_57);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_153) {
                    setDoStatusEmphasys(BUTTON_STATUS_GRAB);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_145) {
                    if (checkWolf() ||
                        (field_0x27f4 != NULL &&
                            (field_0x27f4->speedF > 0.1f ||
                            (checkGoatCatchActor(field_0x27f4) && fopAcM_GetName(field_0x27f4) != PROC_COW))))
                    {
                        setDoStatusEmphasys(BUTTON_STATUS_GRAB);
                    } else {
                        setDoStatus(BUTTON_STATUS_NONE);
                    }
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_UNK_151) {
                    if (current.pos.y + 200.0f < mWaterY) {
                        setDoStatus(BUTTON_STATUS_SWIM);
                    } else {
                        setDoStatus(BUTTON_STATUS_NONE);
                    }
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_JUMP && dComIfGp_getHorseActor() != NULL &&
                            dComIfGp_getHorseActor()->getZeldaActor() != NULL)
                {
                    setDoStatus(BUTTON_STATUS_JUMP);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_HOWL && field_0x27f4 != NULL &&
                            (fopAcM_GetName(field_0x27f4) == PROC_Obj_WindStone ||
                            fopAcM_GetName(field_0x27f4) == PROC_Obj_SmWStone))
                {
                    setDoStatusEmphasys(BUTTON_STATUS_LISTEN);
                } else if (dComIfGp_getDoStatus() == BUTTON_STATUS_PLACE) {
                    if (checkEndResetFlg1(ERFLG1_DO_EXCHANGE_PUT_IN)) {
                        setDoStatusEmphasys(BUTTON_STATUS_INSERT);
                    } else if (checkEndResetFlg1(ERFLG1_DO_PUT_EMPHASYS)) {
                        setDoStatusEmphasys(BUTTON_STATUS_PLACE);
                    }
                }

                int grab_status = getWallGrabStatus();
                if (checkWolf()) {
                    if (grab_status == BUTTON_STATUS_GRAB) {
                        setWallGrabStatus(BUTTON_STATUS_PUSH, BUTTON_STATUS_FLAG_EMPHASIS);
                    } else if (grab_status == BUTTON_STATUS_UNK_150) {
                        setWallGrabStatus(BUTTON_STATUS_PICK_UP, checkChainEmphasys());
                    }
                } else {
                    if (grab_status == BUTTON_STATUS_UNK_150) {
                        setWallGrabStatus(BUTTON_STATUS_GRAB, checkChainEmphasys());
                    }

                    if (dComIfGp_getRStatus() == BUTTON_STATUS_SHIELD_ATTACK && checkShieldAttackEmphasys() == 0) {
                        setRStatus(BUTTON_STATUS_NONE);
                    }
                }
            }

            mDoAud_setLinkHp(dComIfGs_getLife(), dComIfGs_getMaxLifeGauge());
        }
    }

    if (dComIfGp_att_getCatghTarget() != NULL && !checkWolf()) {
        if (!checkRideOn() && checkCastleTownUseItem(fpcNm_ITEM_EMPTY_BOTTLE)) {
            dComIfGp_setBottleStatus(BUTTON_STATUS_SCOOP, 0);
        }
    }

    if (checkEndResetFlg2(ERFLG2_UNK_20) && dComIfGp_checkPlayerStatus0(0, 0x200000) &&
        (field_0x310e != field_0x310a || field_0x3110 != field_0x310c))
    {
        mDoAud_seStartLevel(Z2SE_AL_HAWK_EYE_PAN, NULL, 0, 0);
    }

    offNoResetFlg0(FLG0_HEAVY_STATE);
    mEndResetFlg0 = 0;

    if (checkEndResetFlg1(ERFLG1_UNK_4)) {
        onNoResetFlg2(FLG2_UNK_20000);
    } else {
        offNoResetFlg2(FLG2_UNK_20000);
    }

    mEndResetFlg1 = 0;
    mEndResetFlg2 = 0;

    field_0x3540 = old.pos;
    field_0x3108 = shape_angle.y;

    if (checkHorseRide() && checkBoarSingleBattle()) {
        if (abs(shape_angle.y) < 0x4000) {
            shape_angle.y = 0;
        } else {
            shape_angle.y = -0x8000;
        }
    } else if (checkMagneBootsOn()) {
        field_0x3118 = shape_angle.y;
        shape_angle.y = getMagneBootsLocalAngleY(shape_angle.y, 0);
    } else if (checkModeFlg(MODE_VINE_CLIMB)) {
        field_0x3108 = shape_angle.y;
        shape_angle.y += field_0x308c;
    } else if (checkCargoCarry() && mCargoCarryAcKeep.getActor() != NULL) {
        field_0x3108 = shape_angle.y;
        shape_angle.y = mCargoCarryAcKeep.getActor()->shape_angle.y;
    }

    #if DEBUG
    if (checkModeFlg(2)) {
        f32 var_f27 = current.pos.y - mLastJumpPos.y;
        if (var_f27 > l_jumpTop) {
            l_jumpTop = var_f27;
        }
    }
    #endif

    return 1;
}

static int daAlink_Execute(daAlink_c* i_this) {
    return i_this->execute();
}

void daAlink_c::setDrawHand() {
    field_0x06d0->hide();
    field_0x06d4->hide();

    u8 var_r30;
    u8 var_r29;
    if (checkNoResetFlg2(FLG2_STATUS_WINDOW_DRAW)) {
        if (checkSwordGet()) {
            var_r30 = 0;
        } else {
            var_r30 = 0xFE;
        }

        if (checkShieldGet()) {
            var_r29 = 6;
        } else {
            var_r29 = 0xFE;
        }
    } else if (mProcID == PROC_TOOL_DEMO) {
        var_r30 = mLeftHandIndex;
        var_r29 = mRightHandIndex;
    } else {
        if (mLeftHandIndex == 0x64) {
            if (mProcID == PROC_BOSS_BODY_HANG || mProcID == PROC_MASTER_SWORD_PULL || (mProcID == PROC_CUT_DOWN_LAND && mProcVar2.field_0x300c != 0)) {
                var_r30 = mProcVar4.field_0x3010;
            } else if (checkUnderMove0BckNoArc(ANM_RIDE_CUT_TURN_CHARGE) && mUnderFrameCtrl[0].getFrame() < 6.0f) {
                var_r30 = 0;
            } else {
                var_r30 = 2;
            }
        } else if (field_0x2f96 != 0xFF && (!checkNoResetFlg1(FLG1_UNK_100000) || field_0x2f94 == 0xFF)) {
            var_r30 = field_0x2f96;
        } else if (field_0x2f94 != 0xFF) {
            if (field_0x2f94 == 0x67) {
                if (checkBowGrabLeftHand()) {
                    var_r30 = 0;
                } else {
                    var_r30 = mLeftHandIndex;
                }
            } else if (field_0x2f94 == 0x65) {
                if (mUpperAnmHeap[0].getIdx() == 0xC7) {
                    var_r30 = 0;
                } else if (mItemVar0.field_0x3018 == 1) {
                    var_r30 = 4;
                } else {
                    var_r30 = 2;
                }
            } else if (field_0x2f94 == 0 && mProcID == PROC_SWORD_UNEQUIP_SP) {
                var_r30 = mLeftHandIndex;
            } else if (field_0x2f94 == 0 && mEquipItem == 0x103 && (checkHorseRide() || checkUnderMove0BckNoArc(ANM_SWIM_DIE))) {
                var_r30 = 2;
            } else {
                var_r30 = field_0x2f94;
            }
        } else {
            var_r30 = mLeftHandIndex;
        }

        if ((mCutType == CUT_TYPE_FINISH_STAB || mCutType == CUT_TYPE_DOWN) && mProcID != PROC_BOSS_BODY_HANG) {
            var_r29 = 5;
        } else if (field_0x2f97 != 0xFF && (!checkNoResetFlg1(FLG1_UNK_100000) || (field_0x2f95 == 0xFF && field_0x2e44.checkPassNum(0xF)))) {
            var_r29 = field_0x2f97;
        } else if (field_0x2f95 != 0xFF) {
            if (field_0x2f95 == 0x65) {
                if (mItemVar0.field_0x3018 == 1) {
                    var_r29 = 5;
                } else {
                    var_r29 = mRightHandIndex;
                }
            } else {
                var_r29 = field_0x2f95;
            }
        } else if (!field_0x2e44.checkPassNum(0xF)) {
            var_r29 = 6;
        } else {
            var_r29 = mRightHandIndex;
        }
    }

    mpLinkHandModel->setBaseTRMtx(mpLinkModel->getBaseTRMtx());
    mpLinkHandModel->calc();

    if (var_r30 == 0xFE || var_r30 == 0xFB) {
        field_0x06d0 = field_0x06d8;
    } else {
        field_0x06d0 = mpLinkHandModel->getModelData()->getMaterialNodePointer(var_r30)->getShape();
        mpLinkHandModel->setAnmMtx(1, mpLinkModel->getAnmMtx(9));
    }

    if (var_r30 == 0xFB) {
        mpDemoHLTmpModel->setBaseTRMtx(mpLinkModel->getAnmMtx(9));
        if (mpDemoHLTmpBck != NULL) {
            mpDemoHLTmpBck->entry(mpDemoHLTmpModel->getModelData());
        }

        mpDemoHLTmpModel->calc();
        field_0x06d0->hide();
    } else {
        field_0x06d0->show();
    }

    if (var_r29 == 0xFE || var_r29 == 0xFB) {
        field_0x06d4 = field_0x06dc;
    } else {
        field_0x06d4 = mpLinkHandModel->getModelData()->getMaterialNodePointer(var_r29)->getShape();
        mpLinkHandModel->setAnmMtx(2, mpLinkModel->getAnmMtx(0xE));
    }

    if (var_r29 == 0xFB) {
        mpDemoHRTmpModel->setBaseTRMtx(mpLinkModel->getAnmMtx(0xE));
        if (mpDemoHRTmpBck != NULL) {
            mpDemoHRTmpBck->entry(mpDemoHRTmpModel->getModelData());
        }

        mpDemoHRTmpModel->calc();
        field_0x06d4->hide();
    } else {
        field_0x06d4->show();
    }
}

bool daAlink_c::checkSwordDraw() {
    return ((checkSwordGet() && mSwordChangeWaitTimer == 0) && !checkNoResetFlg2(FLG2_UNK_2080000))
            && (!checkWolf() || !dComIfGs_isEventBit(dSv_event_flag_c::M_068));
}

bool daAlink_c::checkShieldDraw() {
    return ((checkShieldGet() && mShieldChangeWaitTimer == 0) && !checkNoResetFlg2(FLG2_UNK_4080000)) &&
           (!checkWolf() || !dComIfGs_isEventBit(dSv_event_flag_c::M_068));
}

bool daAlink_c::checkItemDraw() {
    if (mHeldItemModel != NULL && (mEquipItem != fpcNm_ITEM_IRONBALL || !checkSwordEquipAnime())) {
        if (!checkBowAndSlingItem(mEquipItem) || checkBowGrabLeftHand() || !checkShieldGet() ||
            field_0x2e44.checkPassNum(15))
        {
            return true;
        }
    }

    return false;
}

int daAlink_c::initShadowScaleLight() {
    dKy_shadow_mode_set(4);

    if (checkWolf()) {
        f32 temp_f26 = 0.5f * mTgCyls[0].GetH();
        mTgCyls[0].GetC();
    }

    cXyz sp20(tevStr.mLightPosWorld.x - field_0x3834.x, tevStr.mLightPosWorld.y - field_0x3454, tevStr.mLightPosWorld.z - field_0x3834.z);
    field_0x2ff6 = sp20.atan2sX_Z();

    fopAc_ac_c* talkActor = fopAcM_getTalkEventPartner(this);

    f32 var_f30;
    if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
        var_f30 = 0.0f;
    } else {
        var_f30 = 150.0f;
    }

    BOOL var_r28;
    f32 var_f31;
    if (talkActor != NULL && ((fopAcM_GetName(talkActor) == PROC_Tag_Mhint && !((daTagMhint_c*)talkActor)->checkNoAttention()) || (fopAcM_GetName(talkActor) == PROC_Tag_Mstop && !((daTagMstop_c*)talkActor)->checkNoAttention()))) {
        field_0x2ff8 = cLib_targetAngleY(&talkActor->eyePos, &current.pos);
        var_r28 = 1;
    } else {
        field_0x2ff8 = shape_angle.y + 0x8000;

        if (checkWolf()) {
            var_f31 = 200.0f;
        } else {
            var_f31 = 120.0f;
        }

        cXyz sp14(current.pos.x, current.pos.y + var_f30, current.pos.z);
        cXyz sp8(sp14.x + ((30.0f + var_f31) * cM_ssin(shape_angle.y)), sp14.y, sp14.z + ((30.0f + var_f31) * cM_scos(shape_angle.y)));

        if (fopAcM_lc_c::lineCheck(&sp14, &sp8, this)) {
            var_r28 = 1;
        } else {
            var_r28 = 0;
        }
    }

    field_0x2ffa = field_0x2ff8 + 0x8000;
    field_0x2ff4 = cM_atan2s(sp20.y, sp20.absXZ());
    offNoResetFlg2(FLG2_UNK_800);
    field_0x2ffc = 0;
    field_0x375c.y = current.pos.y + var_f30;

    f32 temp_f29 = cM_ssin(field_0x2ff8);
    f32 temp_f28 = cM_scos(field_0x2ff8);

    if (var_r28 == 0) {
        field_0x375c.x = current.pos.x - (var_f31 * temp_f29);
        field_0x375c.z = current.pos.z - (var_f31 * temp_f28);
    } else {
        field_0x375c.x = current.pos.x - (temp_f29 * -30.0f) - (temp_f28 * 65.0f);
        // likely fakematch - debug indicates there probably isn't a temp
        f32 temp = current.pos.z - (temp_f28 * -30.0f);
        field_0x375c.z = temp + (temp_f29 * 65.0f);
    }

    if (checkReinRide()) {
        var_r28 = 1;
    }

    return var_r28;
}

int daAlink_c::moveShadowScaleLight() {
    int var_r29 = 0;

    if (field_0x2ff8 != field_0x2ffa) {
        s16 temp_r0 = field_0x2ff6 - field_0x2ff8;
        if (abs(temp_r0) < 0x4000) {
            if (temp_r0 >= 0) {
                field_0x2ff8 -= 0x4000;
            } else {
                field_0x2ff8 += 0x4000;
            }

            if (cLib_distanceAngleS(field_0x2ff8, field_0x2ffa) < 0x100) {
                field_0x2ff8 = field_0x2ffa;
            }
        }
    } else if (cLib_distanceAngleS(field_0x2ff8, field_0x2ff6) < 0x1600) {
        onNoResetFlg2(FLG2_UNK_800);
    }

    if (cLib_addCalcAngleS(&field_0x2ff6, field_0x2ff8, 4, 0x1800, 0x200) == 0) {
        if (cLib_chaseS(&field_0x2ff4, 0x4000, 0x650)) {
            dKy_shadow_mode_reset(4);
            tevStr.field_0x344 = 1.0f;
        } else {
            tevStr.field_0x344 = 2.0f;
        }

        if (field_0x2ff4 > 0x3000) {
            var_r29 = 1;
        }
    } else {
        if (!checkNoResetFlg2(FLG2_UNK_800)) {
            cLib_chaseS(&field_0x2ff4, 0x2600, 0x800);
        } else {
            cLib_chaseS(&field_0x2ffc, 0x800, 0x100);
            cLib_chaseS(&field_0x2ff4, 0, field_0x2ffc);
        }

        cLib_chaseF(&tevStr.field_0x344, 2.0f, 0.1f);
    }

    f32 temp_f31 = 50000.0f * cM_scos(field_0x2ff4);
    tevStr.mLightPosWorld.set(field_0x3834.x + (temp_f31 * cM_ssin(field_0x2ff6)), field_0x3454 + (50000.0f * cM_ssin(field_0x2ff4)), field_0x3834.z + (temp_f31 * cM_scos(field_0x2ff6)));
    return var_r29;
}

void daAlink_c::shadowDraw() {
    daMidna_c* midna = getMidnaActor();
    if (midna != NULL && ((midna->checkShadowModelDraw() && (field_0x2ff4 >= 0x3FF0 || !checkMidnaRide())) || midna->checkShadowModelDrawDemoForce())) {
        return;
    }

    if (!checkBoarRide()) {
        if (mProcID == PROC_WARP) {
            tevStr.field_0x344 = field_0x3484;
        }

        u32 shadowID;
        if (checkHorseRide()) {
            shadowID = ((daHorse_c*)dComIfGp_getHorseActor())->getShadowID();
            if (shadowID != 0) {
                dComIfGd_addRealShadow(shadowID, mpLinkModel);
            }
        } else {
            f32 var_f31;
            if (checkModeFlg(0x40)) {
                var_f31 = field_0x33d8;
            } else {
                var_f31 = mLinkAcch.GetGroundH();
            }

            cXyz spC(field_0x3834.x, field_0x3454, field_0x3834.z);

            f32 var_f30;
            if (!checkWolf()) {
                if (mTgCyls[0].GetC().y > mTgCyls[2].GetC().y) {
                    var_f30 = mTgCyls[2].GetC().y;
                } else {
                    var_f30 = mTgCyls[0].GetC().y;
                }
            } else {
                var_f30 = mTgCyls[0].GetC().y;
            }

            s8 sp8;
            if (mWaterY > var_f31) {
                sp8 = 1;
            } else {
                sp8 = 0;
            }

            field_0x31a4 = dComIfGd_setShadow(field_0x31a4, sp8, mpLinkModel, &spC, 800.0f, 0.0f, var_f30, var_f31, mLinkAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
            shadowID = field_0x31a4;
        }

        if (shadowID != 0) {
            if (checkSwordDraw()) {
                if (!checkNoResetFlg3(FLG3_UNK_80000000)) {
                    dComIfGd_addRealShadow(shadowID, mSwordModel);
                }

                if (!checkWoodSwordEquip()) {
                    dComIfGd_addRealShadow(shadowID, mSheathModel);
                }
            }

            if (checkShieldDraw()) {
                dComIfGd_addRealShadow(shadowID, mShieldModel);
            }

            if (checkWolf()) {
                if (checkMidnaRide() && !checkCloudSea() && !midna->checkShadowNoDraw() && !midna->checkShadowModelDraw()) {
                    dComIfGd_addRealShadow(shadowID, mpWlMidnaModel);

                    if (!midna->checkNoMaskDraw() ) {
                        dComIfGd_addRealShadow(shadowID, mpWlMidnaMaskModel);
                    }

                    dComIfGd_addRealShadow(shadowID, mpWlMidnaHairModel);
                }
            } else {
                dComIfGd_addRealShadow(shadowID, mpLinkFaceModel);
                dComIfGd_addRealShadow(shadowID, mpLinkHatModel);
                dComIfGd_addRealShadow(shadowID, mpLinkHandModel);

                if (mLeftHandIndex == 0xFB) {
                    dComIfGd_addRealShadow(shadowID, mpDemoHLTmpModel);
                }

                if (mRightHandIndex == 0xFB) {
                    dComIfGd_addRealShadow(shadowID, mpDemoHRTmpModel);
                }

                if (mEquipItem == fpcNm_ITEM_BOOMERANG) {
                    if (mItemAcKeep.getActor() != NULL) {
                        dComIfGd_addRealShadow(shadowID, mItemAcKeep.getActor()->model);
                    }
                } else if (checkItemDraw()) {
                    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
                        if (mItemVar0.field_0x3018 == 0 || mItemVar0.field_0x3018 == 8) {
                            dComIfGd_addRealShadow(shadowID, mHeldItemModel);
                        } else {
                            if (fopAcM_gc_c::gndCheck(&mIronBallBgChkPos)) {
                                field_0x32d4 = dComIfGd_setShadow(field_0x32d4, 0, mHeldItemModel, &mIronBallCenterPos, 150.0f, 32.0f, mIronBallBgChkPos.y, fopAcM_gc_c::getGroundY(), *fopAcM_gc_c::getGroundCheck(), &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
                            }
                        }
                    } else {
                        changeHookshotDrawModel();
                        dComIfGd_addRealShadow(shadowID, mHeldItemModel);

                        if (checkHookshotItem(mEquipItem)) {
                            if (checkHookshotWait() && !dComIfGp_checkPlayerStatus1(0, 0x10000)) {
                                dComIfGd_addRealShadow(shadowID, mpHookTipModel);
                            }

                            if (mEquipItem == fpcNm_ITEM_W_HOOKSHOT && !checkPlayerGuardAndAttack() && !checkNoResetFlg0(FLG0_UNK_2)) {
                                dComIfGd_addRealShadow(shadowID, field_0x0710);

                                if (!dComIfGp_checkPlayerStatus1(0, 0x10000)) {
                                    dComIfGd_addRealShadow(shadowID, field_0x0714);
                                }
                            }
                        }

                        changeHookshotDrawModel();
                    }
                }

                if (checkNoResetFlg2(FLG2_UNK_1)) {
                    dComIfGd_addRealShadow(shadowID, mpKanteraModel);
                }

                if (checkEquipHeavyBoots()) {
                    for (int i = 0; i < 2; i++) {
                        dComIfGd_addRealShadow(shadowID, mpLinkBootModels[i]);
                    }
                }

                if (checkSpinnerRide()) {
                    fopAc_ac_c* spinnerActor = fopAcM_SearchByID(mRideAcKeep.getID());
                    if (spinnerActor != NULL) {
                        dComIfGd_addRealShadow(shadowID, spinnerActor->model);
                    }
                }
            }

            if (mGrabItemAcKeep.getID() != fpcM_ERROR_PROCESS_ID_e) {
                fopAc_ac_c* grabActor = fopAcM_SearchByID(mGrabItemAcKeep.getID());
                if (grabActor != NULL && grabActor->model != NULL) {
                    dComIfGd_addRealShadow(shadowID, grabActor->model);
                }
            }

            if (dComIfGp_checkPlayerStatus1(0, 0x10000) && mCargoCarryAcKeep.getID() != fpcM_ERROR_PROCESS_ID_e) {
                fopAc_ac_c* carryActor = fopAcM_SearchByID(mCargoCarryAcKeep.getID());
                if (carryActor != NULL && carryActor->model != NULL) {
                    dComIfGd_addRealShadow(shadowID, carryActor->model);
                }
            }
        }
    }
}

void daAlink_c::modelCalc(J3DModel* i_model) {
    if (mClothesChangeWaitTimer == 0) {
        i_model->calc();
    }
}

void daAlink_c::basicModelDraw(J3DModel* i_model) {
    g_env_light.setLightTevColorType_MAJI(i_model, &tevStr);
    mDoExt_modelEntryDL(i_model);
}

void daAlink_c::modelDraw(J3DModel* i_model, int param_1) {
    g_env_light.setLightTevColorType_MAJI(i_model, &tevStr);

    if (param_1 == 0) {
        mDoExt_modelEntryDL(i_model);
    } else {
        i_model->calcMaterial();
        i_model->diff();
    }

    daMirror_c::entry(i_model);
}

void daAlink_c::setWaterDropColor(const J3DGXColorS10* i_color) {
    static const GXColorS10 notColor0 = {0x00, 0x00, 0x00, 0xFF};
    J3DGXColorS10* var_r31;

    if (&field_0x32a0[0] == i_color) {
        if (checkNoResetFlg2(FLG2_UNK_80000) || checkZoraWearAbility() ||
            checkMagicArmorWearAbility())
        {
            var_r31 = (J3DGXColorS10*)&notColor0;
            i_color = (J3DGXColorS10*)&notColor0;
        } else {
            var_r31 = (J3DGXColorS10*)&i_color[1];
        }
    } else {
        var_r31 = (J3DGXColorS10*)i_color;
    }

    if (!checkNoResetFlg2(FLG2_UNK_80000)) {
        if (checkZoraWearAbility()) {
            field_0x064C->getMaterialNodePointer(13)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(0)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(1)->setTevColor(1, i_color);
            mpLinkHatModel->getModelData()->getMaterialNodePointer(1)->setTevColor(1, i_color);
        } else if (checkMagicArmorWearAbility()) {
            field_0x064C->getMaterialNodePointer(11)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(10)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(9)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(8)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(6)->setTevColor(1, i_color);
            mpLinkHatModel->getModelData()->getMaterialNodePointer(2)->setTevColor(1, i_color);
            mpLinkHatModel->getModelData()->getMaterialNodePointer(1)->setTevColor(1, i_color);
        } else if (checkCasualWearFlg()) {
            field_0x064C->getMaterialNodePointer(7)->setTevColor(1, i_color);
            mpLinkHatModel->getModelData()->getMaterialNodePointer(0)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(5)->setTevColor(1, var_r31);
        } else {
            field_0x064C->getMaterialNodePointer(17)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(9)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(0)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(1)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(2)->setTevColor(1, i_color);
            mpLinkHatModel->getModelData()->getMaterialNodePointer(0)->setTevColor(1, i_color);
            field_0x064C->getMaterialNodePointer(16)->setTevColor(1, var_r31);
            field_0x064C->getMaterialNodePointer(15)->setTevColor(1, var_r31);
            field_0x064C->getMaterialNodePointer(14)->setTevColor(1, var_r31);
        }
    }
}

void daAlink_c::initTevCustomColor() {
    tevStr.mLightInf.a = 0;
    tevStr.TevColor.r = 0;
    tevStr.TevColor.g = 0;
    tevStr.TevColor.b = 0;
    tevStr.TevKColor.r = 0;
    tevStr.TevKColor.b = 0;
}

int daAlink_c::draw() {
    if (checkWolf()) {
        g_env_light.settingTevStruct(9, &current.pos, &tevStr);
    } else {
        g_env_light.settingTevStruct(10, &current.pos, &tevStr);
    }

    initTevCustomColor();

    if (mSight.getDrawFlg() && !checkEventRun()) {
        #if PLATFORM_GCN
        mSight.setSight();
        #endif
    }

    if (checkNoResetFlg1(FLG1_UNK_80)) {
        JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(field_0x31c4);
        if (emitter_p != NULL) {
            if (checkPlayerNoDraw() && !checkEndResetFlg1(ERFLG1_UNK_4)) {
                emitter_p->stopDrawParticle();
            } else {
                emitter_p->playDrawParticle();
            }
        }
    }

    if (mClothesChangeWaitTimer != 0) {
        if (mProcID == PROC_METAMORPHOSE) {
            if (mProcVar3.field_0x300e > 0) {
                tevStr.TevColor.r = 255;
            } else {
                tevStr.TevColor.r = -255;
            }

            tevStr.TevColor.g = tevStr.TevColor.r;
            tevStr.TevColor.b = tevStr.TevColor.r;

            g_env_light.setLightTevColorType_MAJI(mpWlChangeModel, &tevStr);
            mDoExt_modelEntryDL(mpWlChangeModel);
        }
        return 1;
    }

    BOOL isPlayerNoDraw = checkPlayerNoDraw();
    BOOL var_r29 = FALSE;
    BOOL var_r31 = TRUE;

    if (field_0x06f4 != NULL) {
        field_0x06f4->getTevColorReg(0, &tevStr.TevColor);
    } else if (mProcID == PROC_METAMORPHOSE || mProcID == PROC_DUNGEON_WARP ||
               mProcID == PROC_DUNGEON_WARP_SCN_START)
    {
        tevStr.TevColor.r = mProcVar3.field_0x300e;
        tevStr.TevColor.g = mProcVar3.field_0x300e;
        tevStr.TevColor.b = mProcVar3.field_0x300e;
    } else if (field_0x346c != 0.0f) {
        f32 var_f2 = fabsf(field_0x346c);
        tevStr.TevColor.r = var_f2 * mpHIO->mWolf.m.mLightDropR;
        tevStr.TevColor.g = var_f2 * mpHIO->mWolf.m.mLightDropG;
        tevStr.TevColor.b = var_f2 * mpHIO->mWolf.m.mLightDropB;
    } else if (checkFreezeDamage()) {
        tevStr.TevColor.r = mpHIO->mDamage.m.mFreezeR;
        tevStr.TevColor.g = mpHIO->mDamage.m.mFreezeG;
        tevStr.TevColor.b = mpHIO->mDamage.m.mFreezeB;
    } else if (mDamageTimer != 0) {
        if (!checkMagicArmorNoDamage()) {
            int color_timer = mDamageTimer + mDamageColorTime;

            int r, g, b;
            if (color_timer > 32) {
                r = mpHIO->mDamage.m.mDamageR0;
                g = mpHIO->mDamage.m.mDamageG0;
                b = mpHIO->mDamage.m.mDamageB0;
            } else if (color_timer > 16) {
                r = mpHIO->mDamage.m.mDamageR1;
                g = mpHIO->mDamage.m.mDamageG1;
                b = mpHIO->mDamage.m.mDamageB1;
            } else {
                r = mpHIO->mDamage.m.mDamageR2;
                g = mpHIO->mDamage.m.mDamageG2;
                b = mpHIO->mDamage.m.mDamageB2;
            }

            f32 var_f2 = fabsf(cM_ssin(color_timer * 0x800));
            tevStr.TevKColor.r = r * var_f2;
            tevStr.TevKColor.g = g * var_f2;
            tevStr.TevKColor.b = b * var_f2;
            var_r31 = 0;
        }
    } else if (mIceDamageWaitTimer != 0) {
        f32 temp_f4 = 1.0f - (mIceDamageWaitTimer * (1.0f / mpHIO->mDamage.m.mFreezeTime));
        f32 temp_f3 = 1.0f - temp_f4;

        tevStr.TevColor.r = (temp_f4 * mpHIO->mDamage.m.mFreezeInitR) + (temp_f3 * mpHIO->mDamage.m.mFreezeR);
        tevStr.TevColor.g = (temp_f4 * mpHIO->mDamage.m.mFreezeInitG) + (temp_f3 * mpHIO->mDamage.m.mFreezeG);
        tevStr.TevColor.b = (temp_f4 * mpHIO->mDamage.m.mFreezeInitB) + (temp_f3 * mpHIO->mDamage.m.mFreezeB);
    } else if (field_0x32b0[0].r != 0) {
        tevStr.TevColor.r = field_0x32b0[0].r;
        tevStr.TevColor.g = field_0x32b0[0].g;
        tevStr.TevColor.b = field_0x32b0[0].b;
        var_r29 = TRUE;
    } else {
        var_r31 = FALSE;
    }

    if (!checkWolf()) {
        if (var_r31) {
            if (checkMagicArmorWearAbility() && mClothesChangeWaitTimer == 0) {
                mpLinkModel->getModelData()->removeTevRegAnimator(mMagicArmorBodyBrk);
                mpLinkHatModel->getModelData()->removeTevRegAnimator(mMagicArmorHeadBrk);
            }

            if (var_r29) {
                setWaterDropColor(&field_0x32b0[1]);
            } else {
                mSwordUpColorIntensity = 0.0f;
                setWaterDropColor((J3DGXColorS10*)&tevStr.TevColor);
            }
        } else {
            if (checkMagicArmorWearAbility()) {
                mpLinkModel->getModelData()->entryTevRegAnimator(mMagicArmorBodyBrk);
                mpLinkHatModel->getModelData()->entryTevRegAnimator(mMagicArmorHeadBrk);
            }

            setWaterDropColor(&field_0x32a0[0]);
        }
    } else if (var_r31 && !var_r29) {
        mSwordUpColorIntensity = 0.0f;
    }

    if (checkWolf()) {
        dComIfGd_setListDark();
        modelDraw(mpLinkModel, isPlayerNoDraw);

        if (dComIfGs_isEventBit(dSv_event_flag_c::M_011)) {
            for (int i = 0; i < 4; i++) {
                modelDraw(mpWlChainModels[i], isPlayerNoDraw);
            }
        }

        tevStr.TevKColor.r = 0;
        tevStr.TevKColor.g = 0;
        tevStr.TevKColor.b = 0;

        if (checkSwordDraw()) {
            modelDraw(mSwordModel, isPlayerNoDraw);
            modelDraw(mSheathModel, isPlayerNoDraw);
        }

        if (checkShieldDraw()) {
            modelDraw(mShieldModel, isPlayerNoDraw);
        }

        if (mHeldItemModel != NULL) {
            if (mEquipItem == 0x109) {
                dComIfGd_setListMiddle();
            } else if (mEquipItem == 0x10A) {
                mHeldItemModel->getModelData()->entryTexMtxAnimator(field_0x0718);
            }

            modelDraw(mHeldItemModel, isPlayerNoDraw);
        }

        dComIfGd_setList();
    } else {
        setDrawHand();

        if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x20)) {
            if (field_0x06e8 != NULL) {
                field_0x06e8->hide();
            }

            field_0x06f0->hide();

            if (checkHookshotReadyMaterialOffMode()) {
                for (u16 i = 0; i < mFallVoiceInit; i++) {
                    if (!(field_0x32cc & (1 << i))) {
                        field_0x064C->getMaterialNodePointer(i)->getShape()->hide();
                    }
                }
            }
        }

        modelDraw(mpLinkModel, isPlayerNoDraw);

        if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x20)) {
            if (checkHookshotReadyMaterialOffMode()) {
                for (u16 i = 0; i < mFallVoiceInit; i++) {
                    if (!(field_0x32cc & (1 << i))) {
                        field_0x064C->getMaterialNodePointer(i)->getShape()->show();
                    }
                }

                if (!checkZoraWearMaskDraw() && checkZoraWearAbility()) {
                    field_0x06f0->hide();
                }
            }

            if (field_0x06e8 != NULL) {
                field_0x06e8->show();
            }

            if (checkZoraWearMaskDraw() || !checkZoraWearAbility()) {
                field_0x06f0->show();
            }
        }

        modelDraw(mpLinkHandModel, isPlayerNoDraw);
        if (mLeftHandIndex == 0xFB) {
            modelDraw(mpDemoHLTmpModel, isPlayerNoDraw);
        }

        if (mRightHandIndex == 0xFB) {
            modelDraw(mpDemoHRTmpModel, isPlayerNoDraw);
        }

        BOOL var_r3 = isPlayerNoDraw || dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x20);

        modelDraw(mpLinkHatModel, var_r3);

        if (checkNoResetFlg1(FLG1_UNK_10)) {
            modelDraw(mpDemoFCBlendModel, var_r3);
            modelDraw(mpDemoFCTongueModel, var_r3);
        } else {
            modelDraw(mpLinkFaceModel, var_r3);
        }

        tevStr.TevKColor.r = 0;
        tevStr.TevKColor.g = 0;
        tevStr.TevKColor.b = 0;

        if (checkSwordDraw()) {
            if (!checkNoResetFlg3(FLG3_UNK_80000000)) {
                modelDraw(mSwordModel, var_r3);
            }

            if (!checkWoodSwordEquip()) {
                modelDraw(mSheathModel, var_r3);
            }
        }

        if (checkShieldDraw()) {
            BOOL var_r24_2;
            if (checkWoodShieldEquip() && field_0x2fcb != 0 && mProcID != PROC_METAMORPHOSE) {
                tevStr.TevColor.r = -((120 - field_0x2fcb) * 32) / 120;
                tevStr.TevColor.g = tevStr.TevColor.r;
                tevStr.TevColor.b = tevStr.TevColor.r;
                var_r24_2 = 1;
            } else {
                var_r24_2 = 0;
            }

            modelDraw(mShieldModel, var_r3);
            if (var_r24_2) {
                tevStr.TevColor.r = 0;
                tevStr.TevColor.g = 0;
                tevStr.TevColor.b = 0;
            }
        }

        if (var_r29) {
            tevStr.TevColor.r = 0;
            tevStr.TevColor.g = 0;
            tevStr.TevColor.b = 0;
        }

        if (checkItemDraw()) {
            changeHookshotDrawModel();
            modelDraw(mHeldItemModel, isPlayerNoDraw);

            if (mpHookTipModel != NULL) {
                modelDraw(mpHookTipModel, isPlayerNoDraw);
            }

            if (mEquipItem == fpcNm_ITEM_W_HOOKSHOT && !checkPlayerGuardAndAttack() &&
                !checkNoResetFlg0(FLG0_UNK_2))
            {
                if (field_0x0710 != NULL) {
                    modelDraw(field_0x0710, isPlayerNoDraw);
                }

                if (field_0x0714 != NULL) {
                    modelDraw(field_0x0714, isPlayerNoDraw);
                }
            }

            changeHookshotDrawModel();

            if (mpHookChain != NULL && !isPlayerNoDraw &&
                ((checkHookshotItem(mEquipItem) && (mHeldItemRootPos.abs(mHookshotTopPos) > 1.0f || field_0x3810.abs(mIronBallBgChkPos) > 1.0f))
                    || mEquipItem == fpcNm_ITEM_IRONBALL)
                )
            {
                dComIfGd_getOpaListDark()->entryImm(mpHookChain, 0);
            }
        }

        if (checkNoResetFlg2(FLG2_UNK_1)) {
            modelDraw(mpKanteraModel, isPlayerNoDraw);
            preKandelaarDraw();
            modelDraw(mpKanteraGlowModel, isPlayerNoDraw);
        } else if (checkNoResetFlg2(FLG2_UNK_20000)) {
            basicModelDraw(mpKanteraModel);
            preKandelaarDraw();
            basicModelDraw(mpKanteraGlowModel);
        }

        if (checkEquipHeavyBoots()) {
            GXColorS10 color = tevStr.TevColor;
            for (int i = 0; i < 2; i++) {
                JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(field_0x3228[i][0]);

                if (emitter_p != NULL) {
                    tevStr.TevColor.r = 5;
                    tevStr.TevColor.g = tevStr.TevColor.r;
                    tevStr.TevColor.b = tevStr.TevColor.r;
                } else {
                    tevStr.TevColor = color;
                }

                modelDraw(mpLinkBootModels[i], isPlayerNoDraw);
            }

            tevStr.TevColor = color;
        }
    }

    if (!checkCanoeRide() && !dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x10000) &&
        !checkNoResetFlg2(FLG2_PLAYER_SHADOW_NO_DRAW))
    {
        shadowDraw();
    }

    if (m_swordBlur.field_0x14 > 0) {
        dComIfGd_entryZSortXluList(&m_swordBlur, m_swordBlur.field_0x308[0]);
    }

    return 1;
}

static int daAlink_Draw(daAlink_c* i_this) {
    return i_this->draw();
}

daAlink_c::~daAlink_c() {
    dComIfGp_clearPlayerStatus0(0, ~0x400030);
    dComIfGp_clearPlayerStatus1(0, 0x7FB7B78);

    #if DEBUG
    mpHIO->removeHIO();
    #endif

    mZ2Link.deleteKantera();
    mZ2Link.deleteObject();
    if (mpHookSound != NULL) {
        mpHookSound->deleteObject();
        mpHookSound = NULL;
    }

    if (checkNoResetFlg2(FLG2_UNK_80)) {
        offNoResetFlg2(FLG2_UNK_80);
        dComIfGp_2dShowOn();
    }

    if (mProcID == PROC_WARP || (mProcID == PROC_TOOL_DEMO && mProcVar2.field_0x300c != 0)) {
        changeWarpMaterial(WARP_MAT_MODE_1);
    }

    dComIfG_resDelete(&mPhaseReq, mArcName);
    if (mpArcHeap != NULL) {
        mDoExt_destroyExpHeap(mpArcHeap);
    }

    dComIfG_resDelete(&mShieldPhaseReq, mShieldArcName);
    if (mpShieldArcHeap != NULL) {
        mDoExt_destroyExpHeap(mpShieldArcHeap);
    }

    dKy_plight_cut(&mMagneBootsPlight);

    dComIfGp_setPlayer(0, NULL);
    dComIfGp_setLinkPlayer(NULL);
}

static int daAlink_Delete(daAlink_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);

    if (i_this->getClothesChangeWaitTimer() != 0) {
        i_this->loadModelDVD();
        return 0;
    } else if (i_this->getShieldChangeWaitTimer() != 0) {
        i_this->loadShieldModelDVD();
        return 0;
    } else {
        i_this->~daAlink_c();
        return 1;
    }
}

static actor_method_class l_daAlink_Method = {
    (process_method_func)daAlink_Create,  (process_method_func)daAlink_Delete,
    (process_method_func)daAlink_Execute, (process_method_func)NULL,
    (process_method_func)daAlink_Draw,
};

actor_process_profile_definition g_profile_ALINK = {
    fpcLy_CURRENT_e,
    5,
    fpcPi_CURRENT_e,
    PROC_ALINK,
    &g_fpcLf_Method.base,
    sizeof(daAlink_c),
    0,
    0,
    &g_fopAc_Method.base,
    91,
    &l_daAlink_Method,
    0x60400,
    fopAc_PLAYER_e,
    fopAc_CULLBOX_0_e,
};
