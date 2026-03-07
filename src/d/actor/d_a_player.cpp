/**
 * d_a_player.cpp
 * Base Player Actor functionality
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_player.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_alink.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_boomerang.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_spinner.h"

bool daPy_frameCtrl_c::checkAnmEnd() {
    if (getEndFlg() != 0 && getNowSetFlg() == 0) {
        return true;
    }

    if (getRate() == 0.0f) {
        onEndFlg();
        onNowSetFlg();
    }

    return false;
}

void daPy_frameCtrl_c::updateFrame() {
    update();
    offNowSetFlg();
}

void daPy_frameCtrl_c::setFrameCtrl(u8 i_attribute, s16 i_start, s16 i_end, f32 i_rate, f32 i_frame) {
    setAttribute(i_attribute);
    setEnd(i_end);
    setRate(i_rate);
    setStart(i_start);
    setFrame(i_frame);

    if (i_rate >= 0.0f) {
        setLoop(i_start);
    } else {
        setLoop(i_end);
    }

    offEndFlg();
}

void daPy_boomerangMove_c::initOffset(const cXyz* i_pos) {
    daBoomerang_c* boomerang_p = daPy_py_c::getThrowBoomerangActor();

    if (boomerang_p != NULL) {
        bgCheckAfterOffset(i_pos);
        boomerang_p->onWindCatch();
    } else {
        m_offsetY = cM_rndF(70.0f) + 30.0f;
        m_offsetXZ = cM_rndF(50.0f) + 50.0f;
        field_0x2 = cM_rndF(65536.0f);
    }

    field_0x0 = 0;
}

daMidna_c* daPy_py_c::m_midnaActor;

s16 daPy_boomerangMove_c::m_dropAngleY;

s16 daPy_boomerangMove_c::m_eventKeepFlg;

int daPy_boomerangMove_c::posMove(cXyz* o_pos, s16* o_rotY, fopAc_ac_c* i_objActor, s16 i_rotStep) {
    daBoomerang_c* boomerang_p = daPy_py_c::getThrowBoomerangActor();

    field_0x2 -= i_rotStep;
    *o_rotY -= i_rotStep;

    if (m_offsetXZ > 300.0f) {
        m_offsetXZ = 300.0f;
    }

    if (m_offsetY < -700.0f) {
        m_offsetY = -700.0f;
    } else if (m_offsetY > 50.0f) {
        m_offsetY = 50.0f;
    }

    if (boomerang_p != NULL) {
        if (boomerang_p->checkForceDelete()) {
            return 0;
        }

        if (dComIfGp_event_runCheck()) {
            field_0x0 = 1;
        } else if (m_eventKeepFlg != 0 && field_0x0 == 0) {
            return 0;
        }

        o_pos->set(boomerang_p->current.pos.x + m_offsetXZ * cM_ssin(field_0x2),
                   boomerang_p->current.pos.y + m_offsetY,
                   boomerang_p->current.pos.z + m_offsetXZ * cM_scos(field_0x2));
        boomerang_p->onWindCatch();
        return 1;
    }

    daAlink_c* player_p = daAlink_getAlinkActorClass();
    if ((i_objActor == NULL || !player_p->checkBoomerangCarry(i_objActor)) &&
        (m_eventKeepFlg == 0 || field_0x0 != 0))
    {
        s16 tmp = (m_dropAngleY + player_p->shape_angle.y) + cM_rndF(2048.0f);

        o_pos->x = player_p->current.pos.x + m_offsetXZ * cM_ssin(tmp);
        o_pos->z = player_p->current.pos.z + m_offsetXZ * cM_scos(tmp);
        m_dropAngleY += 0x2000;
    }

    return 0;
}

void daPy_boomerangMove_c::bgCheckAfterOffset(const cXyz* i_pos) {
    daBoomerang_c* boomerang_p = daPy_py_c::getThrowBoomerangActor();
    if (boomerang_p != NULL) {
        m_offsetY = i_pos->y - boomerang_p->current.pos.y;
        m_offsetXZ = boomerang_p->current.pos.absXZ(*i_pos);
        field_0x2 = cM_atan2s(i_pos->x - boomerang_p->current.pos.x,
                              i_pos->z - boomerang_p->current.pos.z);
    }
}

u32 daPy_py_c::setParamData(int i_roomNo, int i_mode, int param_2, int param_3) {
    return (i_roomNo & 0x3F) | (i_mode & 0x1F) << 0xC | ((param_2 & 0xFF) << 0x18) | param_3;
}

BOOL daPy_py_c::checkFishingRodItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_FISHING_ROD_1 || (i_itemNo >= fpcNm_ITEM_BEE_ROD && i_itemNo <= fpcNm_ITEM_JEWEL_WORM_ROD);
}

BOOL daPy_py_c::checkBombItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_NORMAL_BOMB || i_itemNo == fpcNm_ITEM_WATER_BOMB || i_itemNo == fpcNm_ITEM_POKE_BOMB;
}

BOOL daPy_py_c::checkBottleItem(int i_itemNo) {
    return (i_itemNo >= fpcNm_ITEM_EMPTY_BOTTLE && i_itemNo <= fpcNm_ITEM_FAIRY)
            || i_itemNo == fpcNm_ITEM_BEE_CHILD
            || i_itemNo == fpcNm_ITEM_WORM
            || i_itemNo == fpcNm_ITEM_FAIRY_DROP
            || i_itemNo == fpcNm_ITEM_CHUCHU_BLACK
            || (i_itemNo >= fpcNm_ITEM_CHUCHU_RARE && i_itemNo <= fpcNm_ITEM_LV3_SOUP);
}

BOOL daPy_py_c::checkDrinkBottleItem(int i_itemNo) {
    return (i_itemNo >= fpcNm_ITEM_RED_BOTTLE && i_itemNo <= fpcNm_ITEM_HALF_MILK_BOTTLE)
            || i_itemNo == fpcNm_ITEM_RED_BOTTLE_2
            || i_itemNo == fpcNm_ITEM_UGLY_SOUP
            || i_itemNo == fpcNm_ITEM_HOT_SPRING
            || i_itemNo == fpcNm_ITEM_BEE_CHILD
            || i_itemNo == fpcNm_ITEM_CHUCHU_BLACK
            || i_itemNo == fpcNm_ITEM_FAIRY_DROP
            || (i_itemNo >= fpcNm_ITEM_CHUCHU_RARE && i_itemNo != fpcNm_ITEM_CHUCHU_YELLOW && i_itemNo <= fpcNm_ITEM_LV3_SOUP);
}

BOOL daPy_py_c::checkOilBottleItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_CHUCHU_YELLOW || i_itemNo == fpcNm_ITEM_OIL_BOTTLE_2 || i_itemNo == fpcNm_ITEM_OIL_BOTTLE;
}

BOOL daPy_py_c::checkOpenBottleItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_WATER_BOTTLE || i_itemNo == fpcNm_ITEM_WORM || i_itemNo == fpcNm_ITEM_FAIRY;
}

BOOL daPy_py_c::checkBowItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_BOW || i_itemNo == fpcNm_ITEM_BOMB_ARROW || i_itemNo == fpcNm_ITEM_HAWK_ARROW;
}

BOOL daPy_py_c::checkHookshotItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_HOOKSHOT || i_itemNo == fpcNm_ITEM_W_HOOKSHOT;
}

BOOL daPy_py_c::checkTradeItem(int i_itemNo) {
    return (i_itemNo >= fpcNm_ITEM_LETTER && i_itemNo <= fpcNm_ITEM_IRIAS_PENDANT)
            || i_itemNo == fpcNm_ITEM_ANCIENT_DOCUMENT
            || i_itemNo == fpcNm_ITEM_AIR_LETTER
            || i_itemNo == fpcNm_ITEM_ANCIENT_DOCUMENT2
            || i_itemNo == fpcNm_ITEM_RAFRELS_MEMO
            || i_itemNo == fpcNm_ITEM_TKS_LETTER
            || i_itemNo == fpcNm_ITEM_ASHS_SCRIBBLING;
}

BOOL daPy_py_c::checkDungeonWarpItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_DUNGEON_EXIT || i_itemNo == fpcNm_ITEM_DUNGEON_BACK;
}

void daPy_actorKeep_c::setActor() {
    if (mID != fpcM_ERROR_PROCESS_ID_e) {
        mActor = fopAcM_SearchByID(mID);
        if (mActor == NULL) {
            mID = fpcM_ERROR_PROCESS_ID_e;
        }
    } else {
        mActor = NULL;
    }
}

void daPy_actorKeep_c::setData(fopAc_ac_c* i_actor) {
    if (i_actor != NULL) {
        mActor = i_actor;
        mID = fopAcM_GetID(i_actor);
    } else {
        clearData();
    }
}

void daPy_actorKeep_c::clearData() {
    mID = fpcM_ERROR_PROCESS_ID_e;
    mActor = NULL;
}

daPy_anmHeap_c::daPy_anmHeap_c(u32 i_bufferSize) {
    initData();
    mBufferSize = i_bufferSize;
}

daPy_anmHeap_c::~daPy_anmHeap_c() {
    if (mAnimeHeap != NULL) {
        mDoExt_destroySolidHeap(mAnimeHeap);
    }
}

void daPy_anmHeap_c::initData() {
    resetIdx();
    resetPriIdx();
    resetArcNo();
}

void* daPy_anmHeap_c::mallocBuffer() {
    mBuffer = new (0x20) u8[mBufferSize];
    return mBuffer;
}

void daPy_anmHeap_c::createHeap(daPy_anmHeap_c::daAlinkHEAP_TYPE i_heapType) {
    u32 size;

    if (i_heapType == 4) {
        size = mBufferSize;
    } else if (i_heapType == 3) {
        size = 0x50;
    } else if (i_heapType == 0) {
        size = 0x40;
    } else if (i_heapType == 1) {
        size = 0x40;
    } else {
        size = 0xA0;
    }

    char* tmpWork;
    mDoExt_transAnmBas* tmpTransBas;
    J3DAnmTransformKey* tmpTrans;
    J3DAnmTexPattern* tmpTp;
    J3DAnmTextureSRTKey* tmpTk;

    JKRHeap* tmp;
    mAnimeHeap = mDoExt_createSolidHeapFromGameToCurrent(&tmp, size, 0x20);
    
    if (i_heapType == 4) {
        tmpWork = new char[size];
        JUT_ASSERT(669, tmpWork != NULL);
    } else if (i_heapType == 3) {
        tmpTransBas = new mDoExt_transAnmBas(NULL);
        JUT_ASSERT(672, tmpTransBas != NULL);
    } else if (i_heapType == 0) {
        tmpTrans = new J3DAnmTransformKey();
        JUT_ASSERT(675, tmpTrans != NULL);
    } else if (i_heapType == 1) {
        tmpTp = new J3DAnmTexPattern();
        JUT_ASSERT(678, tmpTp != NULL);
    } else {
        tmpTk = new J3DAnmTextureSRTKey();
        JUT_ASSERT(681, tmpTk != NULL);
    }

    mDoExt_setCurrentHeap(tmp);
    mDoExt_adjustSolidHeap(mAnimeHeap);
}

void daPy_aramBufferCheck(void* i_buffer, u32 i_size) {
    daPy_anmHeap_c* heap = (daPy_anmHeap_c*)i_buffer;
    if (heap->mBufferSize > i_size) {
        OS_REPORT("%x, %x\n", heap->mBufferSize, i_size);
        JUT_ASSERT(699, FALSE);
    }
}

void* daPy_anmHeap_c::loadData(u16 i_resId) {
    static const char twglArcName[] = "TWGate_Lk";
    static const char twgwArcName[] = "TWGate_Wf";
    static const char sumouArcName[] = "alSumou";
    static const char ocArcName[] = "B_oh";
    static const char drArcName[] = "B_DR";
    static const char msDemoArcName[] = "Lv6Gate";
    static const char lastGanonArcName[] = "B_gnd";
    static const char pigGanonArcName[] = "B_mgn";

    static const char* arcName[] = {
        sumouArcName, ocArcName,     twglArcName,      twgwArcName,
        drArcName,    msDemoArcName, lastGanonArcName, pigGanonArcName,
    };

    if (mArcNo == 0xFFFF) {
        JKRReadIdxResource(mBuffer, mBufferSize, i_resId, dComIfGp_getAnmArchive());
        #if DEBUG
        daPy_aramBufferCheck(mBuffer, mBufferSize);
        #endif

        JKRHeap* heap = setAnimeHeap();
        void* data = J3DAnmLoaderDataBase::load(mBuffer, J3DLOADER_UNK_FLAG0);
        mDoExt_setCurrentHeap(heap);
        return data;
    }

    if (mArcNo == 0) {
        return dComIfG_getObjectIDRes(dStage_roomControl_c::getDemoArcName(), i_resId);
    }

    return dComIfG_getObjectRes(arcName[mArcNo - 1], i_resId);
}

void* daPy_anmHeap_c::loadDataIdx(u16 i_resID) {
    if (i_resID == mIdx && mArcNo == 0xFFFF) {
        return NULL;
    }

    mIdx = i_resID;
    mArcNo = 0xFFFF;
    if (mPriIdx == 0xFFFF) {
        return loadData(i_resID);
    }

    return NULL;
}

void* daPy_anmHeap_c::loadDataPriIdx(u16 i_resID) {
    if (i_resID == mPriIdx || mArcNo != 0xFFFF) {
        return NULL;
    }

    mPriIdx = i_resID;
    return loadData(i_resID);
}

void* daPy_anmHeap_c::loadDataDemoRID(u16 i_resID, u16 i_arcNo) {
    if (i_resID == mIdx && i_arcNo == mArcNo) {
        return NULL;
    }

    mIdx = i_resID;
    mArcNo = i_arcNo;
    return loadData(i_resID);
}

JKRHeap* daPy_anmHeap_c::setAnimeHeap() {
    mAnimeHeap->freeAll();
    return mDoExt_setCurrentHeap(mAnimeHeap);
}

#if !PLATFORM_WII
#include "assets/l_sightDL__d_a_player.h"

void daPy_sightPacket_c::draw() {
    GXTexObj texObj;

    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB565, 0);

    GXColor reg0 = {255, 128, 0, 255};
    GXColor reg1 = {50, 20, 0, 255};

    GXSetTevColor(GX_TEVREG0, reg0);
    GXSetTevColor(GX_TEVREG1, reg1);
    GXInitTexObj(&texObj, mpData, mpImg->width, mpImg->height, (GXTexFmt)mpImg->format,
                 (GXTexWrapMode)mpImg->wrapS, (GXTexWrapMode)mpImg->wrapT, mpImg->mipmapCount > 1 ? GX_ENABLE : GX_DISABLE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadPosMtxImm(mProjMtx, GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXCallDisplayList(l_sightDL, 0x80);
    J3DShape::resetVcdVatCache();
}

void daPy_sightPacket_c::setSight() {
    Vec proj;

    mDoLib_project(&mPos, &proj);
    mDoMtx_stack_c::transS(proj.x, proj.y, proj.z);
    mDoMtx_stack_c::scaleM(32.0f, 32.0f, 32.0f);
    mDoMtx_copy(mDoMtx_stack_c::get(), mProjMtx);
    dComIfGd_set2DXlu(this);
}

void daPy_sightPacket_c::setSightImage(ResTIMG* i_img) {
    mpImg = i_img;
    mpData = (u8*)i_img + i_img->imageOffset;
}
#endif

BOOL daPy_py_c::checkMasterSwordEquip() {
    u16 equipSword = dComIfGs_getSelectEquipSword();
    return equipSword == fpcNm_ITEM_MASTER_SWORD || equipSword == fpcNm_ITEM_LIGHT_SWORD;
}

BOOL daPy_py_c::checkWoodShieldEquip() {
    u16 equipShield = dComIfGs_getSelectEquipShield();
    return equipShield == fpcNm_ITEM_WOOD_SHIELD || equipShield == fpcNm_ITEM_SHIELD;
}

f32 daPy_py_c::getAttentionOffsetY() {
    if (checkNowWolf()) {
        return -12.5f;
    }

    return 57.5f;
}

int daPy_py_c::checkNowWolfEyeUp() {
    return daAlink_getAlinkActorClass()->checkWolfEyeUp();
}

void daAlink_c::startRestartRoomFromOut(int i_dmgAmount, u32 i_mode, int param_2) {
    startRestartRoom(i_mode, param_2, i_dmgAmount, dComIfGp_event_runCheck());
}

void daPy_py_c::forceRestartRoom(int i_dmgAmount, u32 i_mode, int param_2) {
    daAlink_getAlinkActorClass()->startRestartRoomFromOut(i_dmgAmount, i_mode, param_2);
}

BOOL daPy_py_c::setFmChainPos(fopAc_ac_c* param_0, cXyz* param_1, int param_2) {
    return daAlink_getAlinkActorClass()->setFmChainPosFromOut(param_0, param_1, param_2);
}

void daPy_py_c::cancelFmChainGrab() {
    daAlink_getAlinkActorClass()->cancelFmChainGrabFromOut();
}

void daPy_py_c::setLookPos(cXyz* i_pos) {
    daAlink_getAlinkActorClass()->setLookPosFromOut(i_pos);
}

void daPy_py_c::setPlayerSe(u32 i_soundID) {
    daAlink_getAlinkActorClass()->seStartOnlyReverb(i_soundID);
}

bool daPy_py_c::linkGrabSubjectNoDraw(fopAc_ac_c* i_actor) {
    return (fopAcM_checkCarryNow(i_actor) && daPy_getPlayerActorClass()->getGrabActorID() == fopAcM_GetID(i_actor))
            && dComIfGp_checkCameraAttentionStatus(dComIfGp_getPlayerCameraID(0), 2);
}

bool daPy_py_c::wolfGrabSubjectNoDraw(fopAc_ac_c* i_actor) {
    return checkNowWolf() && linkGrabSubjectNoDraw(i_actor);
}

bool daPy_py_c::checkRoomRestartStart() {
    u32 mode = getLastSceneMode();
    if (mode == 4 || mode == 8 || mode == 5) {
        return true;
    }

    return false;
}

u32 daPy_py_c::checkCarryStartLightBallA() {
    if (checkRoomRestartStart()
        || !dComIfGp_getStage()
        || !dComIfGp_getStage()->getStagInfo()
        || dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) != dStage_SaveTbl_LV8)
    {
        return FALSE;
    }

    return dComIfGs_getLastSceneMode() & 0x100000;
}

u32 daPy_py_c::checkCarryStartLightBallB() {
    if (checkRoomRestartStart()
        || !dComIfGp_getStage()
        || !dComIfGp_getStage()->getStagInfo()
        || dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) != dStage_SaveTbl_LV8)
    {
        return FALSE;
    }

    return dComIfGs_getLastSceneMode() & 0x80000;
}

f32 daPy_py_c::getSpinnerRideSpeed() const {
    if (checkSpinnerRide()) {
        return speedF;
    } else {
        return 0.0f;
    }
}

bool daPy_py_c::checkSpinnerReflectEffect() {
    daSpinner_c* spinner_p = getSpinnerActor();

    if (spinner_p != NULL && spinner_p->reflectAccept()) {
        return true;
    }

    return false;
}

bool daPy_py_c::checkBoomerangCharge() {
    return daAlink_getAlinkActorClass()->checkBoomerangChargeEnd();
}

bool daPy_py_c::checkBoomerangChargeTime() {
    return daAlink_getAlinkActorClass()->checkBoomerangChargeNow();
}

daBoomerang_c* daPy_py_c::getThrowBoomerangActor() {
    return static_cast<daBoomerang_c*>(daAlink_getAlinkActorClass()->getThrowBoomerangAcKeep()->getActor());
}

void daPy_py_c::cancelBoomerangLockActor(fopAc_ac_c* i_actor) {
    daAlink_getAlinkActorClass()->cancelBoomerangLock(i_actor);
}

void daPy_py_c::setPlayerDamage(int i_dmgAmount, BOOL i_setDmgTimer) {
    daAlink_getAlinkActorClass()->setDamagePoint(i_dmgAmount, FALSE, i_setDmgTimer, 0);
}

BOOL daPy_py_c::checkAcceptDungeonWarp() {
    return daAlink_getAlinkActorClass()->checkAcceptDungeonWarpAlink(1);
}

void daPy_py_c::setMidnaMotionNum(int i_motionNum) {
    if (m_midnaActor != NULL) {
        m_midnaActor->setMotionNum(i_motionNum);
    }
}

void daPy_py_c::setMidnaFaceNum(int i_faceNum) {
    if (m_midnaActor != NULL) {
        m_midnaActor->setFaceNum(i_faceNum);
    }
}

int daPy_addCalcShort(s16* i_value, s16 i_target, s16 i_scale, s16 i_maxStep, s16 i_minStep) {
    int delta = i_target - *i_value;

    if (*i_value != i_target) {
        int step = delta / i_scale;
        if (step > i_minStep || step < -i_minStep) {
            *i_value += cLib_minMaxLimit<int>(step, -i_maxStep, i_maxStep);
        } else if (delta >= 0) {
            *i_value += i_minStep;
            if (i_target - *i_value <= 0) {
                *i_value = i_target;
            }
        } else {
            *i_value -= i_minStep;
            if (i_target - *i_value >= 0) {
                *i_value = i_target;
            }
        }
    }

    return i_target - *i_value;
}
