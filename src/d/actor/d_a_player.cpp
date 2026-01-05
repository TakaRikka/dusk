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

void daPy_frameCtrl_c::setFrameCtrl(u8 i_attribute, s16 i_start, s16 i_end, f32 i_rate,
                                    f32 i_frame) {
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

void daPy_boomerangMove_c::initOffset(cXyz const* i_offset) {
    daBoomerang_c* boomerang_p = daPy_py_c::getThrowBoomerangActor();

    if (boomerang_p != NULL) {
        bgCheckAfterOffset(i_offset);
        boomerang_p->onWindCatch();
    } else {
        field_0x4 = cM_rndF(70.0f) + 30.0f;
        field_0x8 = cM_rndF(50.0f) + 50.0f;
        field_0x2 = cM_rndF(65536.0f);
    }

    field_0x0 = 0;
}

daMidna_c* daPy_py_c::m_midnaActor;

s16 daPy_boomerangMove_c::m_dropAngleY;

s16 daPy_boomerangMove_c::m_eventKeepFlg;

int daPy_boomerangMove_c::posMove(cXyz* param_0, s16* param_1, fopAc_ac_c* param_2, s16 param_3) {
    daBoomerang_c* boomerang_p = daPy_py_c::getThrowBoomerangActor();

    field_0x2 -= param_3;
    *param_1 -= param_3;

    if (field_0x8 > 300.0f) {
        field_0x8 = 300.0f;
    }

    if (field_0x4 < -700.0f) {
        field_0x4 = -700.0f;
    } else if (field_0x4 > 50.0f) {
        field_0x4 = 50.0f;
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

        param_0->set(boomerang_p->current.pos.x + field_0x8 * cM_ssin(field_0x2),
                     boomerang_p->current.pos.y + field_0x4,
                     boomerang_p->current.pos.z + field_0x8 * cM_scos(field_0x2));
        boomerang_p->onWindCatch();
        return 1;
    }

    daAlink_c* player_p = daAlink_getAlinkActorClass();
    if ((param_2 == NULL || !player_p->checkBoomerangCarry(param_2)) &&
        (m_eventKeepFlg == 0 || field_0x0 != 0))
    {
        s16 tmp = (m_dropAngleY + player_p->shape_angle.y) + cM_rndF(2048.0f);

        param_0->x = player_p->current.pos.x + field_0x8 * cM_ssin(tmp);
        param_0->z = player_p->current.pos.z + field_0x8 * cM_scos(tmp);
        m_dropAngleY += 0x2000;
    }

    return 0;
}

void daPy_boomerangMove_c::bgCheckAfterOffset(cXyz const* i_offset) {
    daBoomerang_c* boomerang_p = daPy_py_c::getThrowBoomerangActor();
    if (boomerang_p != NULL) {
        field_0x4 = i_offset->y - boomerang_p->current.pos.y;
        field_0x8 = boomerang_p->current.pos.absXZ(*i_offset);
        field_0x2 = cM_atan2s(i_offset->x - boomerang_p->current.pos.x,
                              i_offset->z - boomerang_p->current.pos.z);
    }
}

u32 daPy_py_c::setParamData(int param_0, int param_1, int param_2, int param_3) {
    return param_3 | (param_0 & 0x3F | (param_1 & 0x1F) << 0xC | param_2 << 0x18);
}

BOOL daPy_py_c::checkFishingRodItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_FISHING_ROD_1 || (i_itemNo >= fpcNm_ITEM_BEE_ROD && i_itemNo <= fpcNm_ITEM_JEWEL_WORM_ROD);
}

BOOL daPy_py_c::checkBombItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_NORMAL_BOMB || i_itemNo == fpcNm_ITEM_WATER_BOMB || i_itemNo == fpcNm_ITEM_POKE_BOMB;
}

BOOL daPy_py_c::checkBottleItem(int i_itemNo) {
    return (i_itemNo >= fpcNm_ITEM_EMPTY_BOTTLE && i_itemNo <= fpcNm_ITEM_FAIRY) || i_itemNo == fpcNm_ITEM_BEE_CHILD ||
           i_itemNo == fpcNm_ITEM_WORM || i_itemNo == fpcNm_ITEM_FAIRY_DROP || i_itemNo == fpcNm_ITEM_CHUCHU_BLACK ||
           (i_itemNo >= fpcNm_ITEM_CHUCHU_RARE && i_itemNo <= fpcNm_ITEM_LV3_SOUP);
}

BOOL daPy_py_c::checkDrinkBottleItem(int i_itemNo) {
    return (i_itemNo >= fpcNm_ITEM_RED_BOTTLE && i_itemNo <= fpcNm_ITEM_HALF_MILK_BOTTLE) || i_itemNo == fpcNm_ITEM_RED_BOTTLE_2 ||
           i_itemNo == fpcNm_ITEM_UGLY_SOUP || i_itemNo == fpcNm_ITEM_HOT_SPRING || i_itemNo == fpcNm_ITEM_BEE_CHILD ||
           i_itemNo == fpcNm_ITEM_CHUCHU_BLACK || i_itemNo == fpcNm_ITEM_FAIRY_DROP ||
           (i_itemNo >= fpcNm_ITEM_CHUCHU_RARE && i_itemNo != fpcNm_ITEM_CHUCHU_YELLOW && i_itemNo <= fpcNm_ITEM_LV3_SOUP);
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
    return (i_itemNo >= fpcNm_ITEM_LETTER && i_itemNo <= fpcNm_ITEM_IRIAS_PENDANT) || i_itemNo == fpcNm_ITEM_ANCIENT_DOCUMENT ||
           i_itemNo == fpcNm_ITEM_AIR_LETTER || i_itemNo == fpcNm_ITEM_ANCIENT_DOCUMENT2 || i_itemNo == fpcNm_ITEM_RAFRELS_MEMO ||
           i_itemNo == fpcNm_ITEM_TKS_LETTER || i_itemNo == fpcNm_ITEM_ASHS_SCRIBBLING;
}

BOOL daPy_py_c::checkDungeonWarpItem(int i_itemNo) {
    return i_itemNo == fpcNm_ITEM_DUNGEON_EXIT || i_itemNo == fpcNm_ITEM_DUNGEON_BACK;
}

void daPy_actorKeep_c::setActor() {
    if (mID != -1) {
        mActor = fopAcM_SearchByID(mID);

        if (mActor == NULL) {
            mID = -1;
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
    mID = 0xffffffff;
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

    JKRHeap* tmp;
    mAnimeHeap = mDoExt_createSolidHeapFromGameToCurrent(&tmp, size, 0x20);

    if (i_heapType == 4) {
        char* tmpWork = new char[size];
    } else if (i_heapType == 3) {
        mDoExt_transAnmBas* tmpTransBas = new mDoExt_transAnmBas(NULL);
    } else if (i_heapType == 0) {
        J3DAnmTransformKey* tmpTrans = new J3DAnmTransformKey();
    } else if (i_heapType == 1) {
        J3DAnmTexPattern* tmpTp = new J3DAnmTexPattern();
    } else {
        J3DAnmTextureSRTKey* tmpTk = new J3DAnmTextureSRTKey();
    }

    mDoExt_setCurrentHeap(tmp);
    mDoExt_adjustSolidHeap(mAnimeHeap);
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
        dComIfGp_getAnmArchive()->readIdxResource(mBuffer, mBufferSize, i_resId);
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
    void* data;

    if (i_resID == mIdx && mArcNo == 0xFFFF) {
        data = NULL;
    } else {
        mIdx = i_resID;
        mArcNo = 0xFFFF;
        if (mPriIdx == 0xFFFF) {
            data = loadData(i_resID);
        } else {
            data = NULL;
        }
    }

    return data;
}

void* daPy_anmHeap_c::loadDataPriIdx(u16 i_resID) {
    void* data;

    if (i_resID == mPriIdx || mArcNo != 0xFFFF) {
        data = NULL;
    } else {
        mPriIdx = i_resID;
        data = loadData(i_resID);
    }

    return data;
}

void* daPy_anmHeap_c::loadDataDemoRID(u16 i_resID, u16 i_arcNo) {
    void* data;

    if (i_resID == mIdx && i_arcNo == mArcNo) {
        data = NULL;
    } else {
        mIdx = i_resID;
        mArcNo = i_arcNo;
        data = loadData(i_resID);
    }

    return data;
}

JKRHeap* daPy_anmHeap_c::setAnimeHeap() {
    mAnimeHeap->freeAll();
    return mDoExt_setCurrentHeap(mAnimeHeap);
}

static u8 l_sightDL[] = {
    0x10, 0x00, 0x00, 0x10, 0x40, 0xFF, 0xFF, 0x42, 0x80, 0x08, 0x30, 0x3C, 0xF3, 0xCF, 0x00, 0x10,
    0x00, 0x00, 0x10, 0x18, 0x3C, 0xF3, 0xCF, 0x00, 0x61, 0x28, 0x38, 0x03, 0xC0, 0x61, 0xC0, 0x08,
    0x42, 0x8F, 0x61, 0xC1, 0x08, 0xE6, 0x70, 0x61, 0x43, 0x00, 0x00, 0x01, 0x61, 0x40, 0x00, 0x00,
    0x06, 0x61, 0x41, 0x00, 0x04, 0xAD, 0x61, 0xF3, 0x64, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x3F,
    0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00,
    0x00, 0x01, 0x98, 0x00, 0x04, 0x01, 0x01, 0x00, 0x01, 0x01, 0xFF, 0x01, 0x00, 0x00, 0x01, 0x01,
    0xFF, 0x00, 0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

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
    u8 tmp2 = mpImg->mipmapCount;
    tmp2 = tmp2 > 1;
    GXInitTexObj(&texObj, mpData, mpImg->width, mpImg->height, (GXTexFmt)mpImg->format,
                 (GXTexWrapMode)mpImg->wrapS, (GXTexWrapMode)mpImg->wrapT, (GXBool)(tmp2));
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadPosMtxImm(field_0x14, GX_PNMTX0);
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
    mDoMtx_copy(mDoMtx_stack_c::get(), field_0x14);
    dComIfGd_set2DXlu(this);
}

void daPy_sightPacket_c::setSightImage(ResTIMG* i_img) {
    mpImg = i_img;
    mpData = (u8*)i_img + i_img->imageOffset;
}

BOOL daPy_py_c::checkMasterSwordEquip() {
    u32 equipSword = dComIfGs_getSelectEquipSword();
    bool isMasterSword = false;

    if (equipSword == fpcNm_ITEM_MASTER_SWORD || equipSword == fpcNm_ITEM_LIGHT_SWORD) {
        isMasterSword = true;
    }

    return isMasterSword;
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


void daAlink_c::startRestartRoomFromOut(int param_0, u32 param_1, int param_2) {
    startRestartRoom(param_1, param_2, param_0, dComIfGp_event_runCheck());
}

void daPy_py_c::forceRestartRoom(int param_0, u32 param_1, int param_2) {
    daAlink_getAlinkActorClass()->startRestartRoomFromOut(param_0, param_1, param_2);
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

void daPy_py_c::setPlayerSe(u32 i_sfxID) {
    daAlink_getAlinkActorClass()->seStartOnlyReverb(i_sfxID);
}

bool daPy_py_c::linkGrabSubjectNoDraw(fopAc_ac_c* i_actor) {
    return (fopAcM_checkCarryNow(i_actor) &&
            daPy_getPlayerActorClass()->getGrabActorID() == fopAcM_GetID(i_actor)) &&
           dComIfGp_checkCameraAttentionStatus(dComIfGp_getPlayerCameraID(0), 2);
}

bool daPy_py_c::wolfGrabSubjectNoDraw(fopAc_ac_c* i_actor) {
    return checkNowWolf() && linkGrabSubjectNoDraw(i_actor);
}

bool daPy_py_c::checkRoomRestartStart() {
    u8 mode = getLastSceneMode();
    if (mode == 4 || mode == 8 || mode == 5) {
        return true;
    }

    return false;
}

u32 daPy_py_c::checkCarryStartLightBallA() {
    if (checkRoomRestartStart() || !dComIfGp_getStage() ||
        !dComIfGp_getStage()->getStagInfo() ||
        dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) != 0x17)
    {
        return FALSE;
    }

    return dComIfGs_getLastSceneMode() & 0x100000;
}

u32 daPy_py_c::checkCarryStartLightBallB() {
    if (checkRoomRestartStart() || !dComIfGp_getStage() ||
        !dComIfGp_getStage()->getStagInfo() ||
        dStage_stagInfo_GetSaveTbl(dComIfGp_getStage()->getStagInfo()) != 0x17)
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
    return 0;
}

daBoomerang_c* daPy_py_c::getThrowBoomerangActor() {
    return static_cast<daBoomerang_c*>(
        daAlink_getAlinkActorClass()->getThrowBoomerangAcKeep()->getActor());
}

void daPy_py_c::cancelBoomerangLockActor(fopAc_ac_c* i_actor) {
    daAlink_getAlinkActorClass()->cancelBoomerangLock(i_actor);
}

void daPy_py_c::setPlayerDamage(int i_dmgAmount, int i_setDmgTimer) {
    daAlink_getAlinkActorClass()->setDamagePoint(i_dmgAmount, FALSE, i_setDmgTimer, 0);
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
