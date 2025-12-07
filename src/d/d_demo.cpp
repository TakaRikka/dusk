#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_demo.h"
#include "d/d_msg_class.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_movie_player.h"
#include "JSystem/JGadget/pointer.h"
#include "JSystem/JGadget/define.h"

s16 dDemo_c::m_branchId = -1;

namespace {
jstudio_tAdaptor_message::~jstudio_tAdaptor_message() {}

/* 80037E44-80037E74 032784 0030+00 1/0 0/0 0/0 .text
 * adaptor_do_MESSAGE__Q220@unnamed@d_demo_cpp@24jstudio_tAdaptor_messageFQ37JStudio4data15TEOperationDataPCvUl
 */
void jstudio_tAdaptor_message::adaptor_do_MESSAGE(JStudio::data::TEOperationData iType,
                                                  const void* pContent, u32 uSize) {
    switch (iType) {
    case JStudio::data::UNK_0x19: {
        JUT_ASSERT(107, pContent!=NULL);
        JUT_ASSERT(108, uSize==4);
        u32 content = *(u32*)pContent;
        dMsgObject_setDemoMessage(content);
        break;
    }
    default:
#if DEBUG
        JGadget_outMessage msg(JGadget_outMessage::warning, __FILE__, 124);
        msg << "unknown data-type : " << iType << "\n  demo-object : " << adaptor_getID_string();
        int x = 0;
#else
        break;
#endif
    }
}

jstudio_tCreateObject_message::~jstudio_tCreateObject_message() {}

/* 80037ED4-80038020 032814 014C+00 1/0 0/0 0/0 .text
 * create__Q220@unnamed@d_demo_cpp@29jstudio_tCreateObject_messageFPPQ27JStudio7TObjectRCQ47JStudio3stb4data20TParse_TBlock_object
 */
bool
jstudio_tCreateObject_message::create(JStudio::TObject** ppObject,
                                      const JStudio::stb::data::TParse_TBlock_object& iBlock) {
    JUT_ASSERT(168, ppObject!=NULL);

    u32 type = iBlock.get_type();
    switch (type) {
    case 'JMSG': 
        {
            jstudio_tAdaptor_message* adaptor = new jstudio_tAdaptor_message();
            if (adaptor == NULL) {
                return false;
            }

            JGadget::TPointer_delete<JStudio::TAdaptor_message> sp8(adaptor);
            JStudio::TObject_message* object = JStudio::TCreateObject::createFromAdaptor<jstudio_tAdaptor_message>(iBlock, adaptor);
            if (object == NULL) {
                return false;
            }

            sp8.set(NULL);
            *ppObject = object;
        }
        return true;
    default:
        return false;
    }
}

};  // namespace

dDemo_actor_c::dDemo_actor_c() {
    mFlags = 0;
    mTrans.set(0.0f, 0.0f, 0.0f);
    mScale.set(1.0f, 1.0f, 1.0f);
    mRotate.set(0, 0, 0);
    mModel = NULL;
    mAnmFrameMax = FLT_MAX;
    mTexAnmFrameMax = FLT_MAX;
}

dDemo_actor_c::~dDemo_actor_c() {
    fopAc_ac_c* actor = getActor();
    if (actor != NULL) {
        actor->demoActorID = 0;
    }

    mActorId = -1;
    mOldAnmId = -1;
    mBtpId = -1;
    mBtkId = -1;
    mBrkId = -1;

    #if DEBUG
    if(dComIfGp_event_getMode() == 0) {
        g_dComIfG_gameInfo.play.getEvent().setDebugStb(0);
    }
    #endif
}

fopAc_ac_c* dDemo_actor_c::getActor() {
    return fopAcM_SearchByID(mActorId);
}

void dDemo_actor_c::setActor(fopAc_ac_c* pActor) {
    if (pActor == NULL) {
        mActorId = fpcM_ERROR_PROCESS_ID_e;
    } else {
        mActorId = fopAcM_GetID(pActor);
    }

    mOldAnmId = -1;
    mBtpId = -1;
    mBtkId = -1;
    mBrkId = -1;
}

f32 dDemo_actor_c::getPrm_Morf() {
    if (checkEnable(ENABLE_ANM_TRANSITION_e)) {
        return mAnmTransition;
    }

    if (!checkEnable(1)) {
        return 0.0f;
    }

    dDemo_prm_data* prm = mPrm.mData;

    switch (mPrm.field_0x0) {
    case ID_UNK_1:
        if (field_0x54 < 4) {
            return 0.0f;
        }
        return prm->field_0x4;
    case ID_UNK_2:
        if (field_0x54 < 7) {
            return 0.0f;
        }
        return prm->field_0x7;
    case ID_UNK_4:
        if (field_0x54 < 6) {
            return 0.0f;
        }
        return prm->field_0x6;
    case ID_UNK_5:
        if (field_0x54 < 0xB) {
            return 0.0f;
        }
        return prm->field_0xb;
    case ID_UNK_6:
        if (field_0x54 < 0xF) {
            return 0.0f;
        }
        return prm->field_0xf;
    default:
        return 0.0f;
    }
}

dDemo_system_c* dDemo_c::m_system;

JStudio::TControl* dDemo_c::m_control;

JStudio_JStage::TCreateObject* dDemo_c::m_stage;

JStudio_JAudio2::TCreateObject* dDemo_c::m_audio;

dDemo_particle_c* dDemo_c::m_particle;

jstudio_tCreateObject_message* dDemo_c::m_message;

JStudio::TFactory* dDemo_c::m_factory;

jmessage_tControl* dDemo_c::m_mesgControl;

dDemo_object_c* dDemo_c::m_object;

const u8* dDemo_c::m_data;

int dDemo_c::m_frame;

cXyz* dDemo_c::m_translation;

f32 dDemo_c::m_rotationY;

u32 dDemo_c::m_frameNoMsg;

s32 dDemo_c::m_mode;

u32 dDemo_c::m_status;

u16 dDemo_c::m_branchType;

const u8* dDemo_c::m_branchData;


int dDemo_actor_c::getDemoIDData(int* param_0, int* param_1, int* param_2, u16* param_3,
                                      u8* param_4) {
    JStudio::stb::TParseData_fixed<51, TValueIterator_misaligned<u32> > data(getPrm()->getData());

    static JStudio::stb::TParseData_fixed<51, TValueIterator_misaligned<u32> > dummy;
    static JGadget::binary::TValueIterator_misaligned<u32> it = dummy.begin();

    if (it == data.end()) {
        it = dummy.begin();
        return 0;
    }

    if (it == dummy.begin()) {
        it = data.begin();
    }

    u32 var_r31 = *it;
    *param_0 = var_r31 >> 0x1E;
    *param_1 = (var_r31 >> 0x18) & 0xF;
    *param_2 = (var_r31 >> 0x10) & 0xF;
    *param_3 = var_r31 & 0xFFFF;

    if (param_4 != NULL) {
        *param_4 = (var_r31 >> 0x17) & 1;
    }

    it++;
    return 1;
}

#if DEBUG
s16 dDemo_c::m_branchNum = 0;
#endif

static void* dDemo_getJaiPointer(char const* arcName, u32 anmID, int param_2, u16* param_3) {
    if (param_2 <= 0 || param_3 == NULL) {
        return NULL;
    }

    int i;
    int var_r31;
    for (i = var_r31 = 0; i < param_2; i++, var_r31 += 2) {
        if ((u16)anmID == param_3[var_r31]) {
            return dComIfG_getObjectIDRes(arcName, param_3[var_r31 + 1]);
        }
    }

    return NULL;
}

int dDemo_setDemoData(fopAc_ac_c* i_actor, u8 i_flags, mDoExt_McaMorf* i_morf, char const* i_arcName,
                      int param_4, u16* param_5, u32 param_6, s8 i_reverb) {
    dDemo_actor_c* demo_actor = dDemo_c::getActor(i_actor->demoActorID);

    if (demo_actor == NULL) {
        return 0;
    }

    u8 flags = demo_actor->checkEnable(i_flags);
    if (flags & dDemo_actor_c::ENABLE_TRANS_e) {
        i_actor->old.pos = i_actor->current.pos = demo_actor->getTrans();
    }

    if (flags & dDemo_actor_c::ENABLE_ROTATE_e) {
        i_actor->shape_angle = demo_actor->getRatate();
    }

    if (flags & dDemo_actor_c::ENABLE_SCALE_e) {
        i_actor->scale = demo_actor->getScale();
    }

    if (flags & dDemo_actor_c::ENABLE_ANM_TRANSITION_e) {
        i_actor->gravity = demo_actor->getPrm_Morf();
    }

    if (i_morf == NULL) {
        return 1;
    }

    demo_actor->setModel(i_morf->getModel());

    if (flags & dDemo_actor_c::ENABLE_ANM_e) {
        u32 anmID = demo_actor->getAnmId();
        void* ptr;

        if (anmID != demo_actor->getOldAnmId()) {
            const char* a_name;
            if (anmID & 0x10000) {
                a_name = dStage_roomControl_c::getDemoArcName();
                JUT_ASSERT(0x267, a_name != NULL);
            } else {
                a_name = i_arcName;
            }

            demo_actor->setOldAnmId(anmID);

            J3DAnmTransform* i_key = (J3DAnmTransform*)dComIfG_getObjectIDRes(a_name, anmID & 0xffff);
            JUT_ASSERT(272, i_key != NULL);

            ptr = dDemo_getJaiPointer(a_name, anmID & 0xffff, param_4, param_5);

            f32 prmMorf = demo_actor->getPrm_Morf();
            i_morf->setAnm(i_key, -1, prmMorf, 1.0f, 0.0f, -1.0f, ptr);
            demo_actor->setAnmFrameMax(i_morf->getEndFrame());
        }
    }

    if (flags & dDemo_actor_c::ENABLE_ANM_FRAME_e) {
        f32 anm_frame = demo_actor->getAnmFrame();

        if (anm_frame > 1.0f) {
            anm_frame -= 1.0f;
            i_morf->setFrame(anm_frame);
            i_morf->play(&i_actor->current.pos, param_6, i_reverb);
        } else {
            i_morf->setFrame(anm_frame);
        }
    } else {
        i_morf->play(&i_actor->current.pos, param_6, i_reverb);
    }

    return 1;
}

static void branchFile(char const* resName) {
    u8* branchData = (u8*)dComIfG_getObjectRes(dStage_roomControl_c::getDemoArcName(), resName);
    JUT_ASSERT(712, branchData != NULL);

    dDemo_c::setBranchData(branchData);
}

void dDemo_actor_c::JSGSetData(u32 id, void const* pdata, u32 param_2) {
    if (id == ID_UNK_1) {
        JStudio::stb::TParseData_fixed<50, TValueIterator_misaligned<u16> > data(pdata);
        if (!data.isEnd() && data.isValid()) {
            JGadget::binary::TValueIterator_misaligned<u16> value(data.begin());
            
            dDemo_c::setBranchType(*value);
            OS_REPORT("JSGSetData: 分岐条件<%d>\n", *value);
        }
    } else if (id == ID_UNK_2) {
        JUT_ASSERT(728, dDemo_c::getBranchId() >= 0);

        JStudio::stb::TParseData_string string_data(pdata);

        u32 num = string_data.size();
        // "JSGSetData: Branch<n=%d "
        OS_REPORT("JSGSetData: 分岐<n=%d ", num);
        JUT_ASSERT(0x2DD, num == dDemo_c::getBranchNum());
        JUT_ASSERT(0x2DF, num > dDemo_c::getBranchId());

        const char* str = string_data.getData();
        for (s16 i = 0; i < dDemo_c::getBranchId(); i++) {
            OS_REPORT("\"%s\"", str);
            str += strlen(str) + 1;
        }

        // "Select<%d:\"%s\">\n"
        OS_REPORT(" 選択<%d:\"%s\">\n", dDemo_c::getBranchId(), str);

        if (strcmp(str, "-") != 0) {
            branchFile(str);
        }
    } else if (id == ID_UNK_3) {
        JStudio::stb::TParseData_string data(pdata);

        u32 num = data.size();
        JUT_ASSERT(0x2ED, num == 1);

        const char* str = data.getData();
        // "JSGSetData: Change<\"%s\">\n"
        OS_REPORT("JSGSetData: 切り替え<\"%s\">\n", str);

        branchFile(str);
    } else {
        JUT_ASSERT(0x2F3, id == ID_NORMAL);

        mPrm.field_0x0 = id;
        mPrm.mData = (dDemo_prm_data*)pdata;
        field_0x54 = param_2;
        
        onEnable(1);
    }
}

void dDemo_actor_c::JSGSetTranslation(Vec const& i_trans) {
    if (-1.0E+32f < i_trans.x && i_trans.x < 1.0E+32f && -1.0E+32f < i_trans.y && i_trans.y < 1.0E+32f && -1.0E+32f < i_trans.z && i_trans.z < 1.0E+32f) {
        mTrans.x = i_trans.x;
        mTrans.y = i_trans.y;
        mTrans.z = i_trans.z;
        onEnable(ENABLE_TRANS_e);
    } else {
        // "\n\n!!JSGSetTranslation outside of range. x[%f]y[%f]z[%f]\n"
        OS_REPORT("\n\n!!JSGSetTranslationで範囲外の指定がされました。x[%f]y[%f]z[%f]\n", i_trans.x, i_trans.y, i_trans.z);
        JUT_ASSERT(789, FALSE);
    }
}

void dDemo_actor_c::JSGSetScaling(Vec const& i_scale) {
    mScale = i_scale;
    onEnable(ENABLE_SCALE_e);
}

void dDemo_actor_c::JSGSetRotation(Vec const& i_rotate) {
    mRotate.x = i_rotate.x * 182.04444885253906f;
    mRotate.y = i_rotate.y * 182.04444885253906f;
    mRotate.z = i_rotate.z * 182.04444885253906f;
    onEnable(ENABLE_ROTATE_e);
}

void dDemo_actor_c::JSGSetShape(u32 i_shape) {
    mShape = i_shape;
    onEnable(ENABLE_SHAPE_e);
}

void dDemo_actor_c::JSGSetAnimation(u32 i_anmID) {
    mAnmId = i_anmID;
    mAnmFrameMax = FLT_MAX;
    onEnable(ENABLE_ANM_e);
}

void dDemo_actor_c::JSGSetAnimationFrame(f32 i_frame) {
    mAnmFrame = i_frame;
    onEnable(ENABLE_ANM_FRAME_e);
}

void dDemo_actor_c::JSGSetAnimationTransition(f32 i_trans) {
    mAnmTransition = i_trans;
    onEnable(ENABLE_ANM_TRANSITION_e);
}

void dDemo_actor_c::JSGSetTextureAnimation(u32 i_anm) {
    mTexAnm = i_anm;
    onEnable(ENABLE_TEX_ANM);
}

void dDemo_actor_c::JSGSetTextureAnimationFrame(f32 i_frame) {
    mTexAnmFrame = i_frame;
    onEnable(ENABLE_TEX_ANM_FRAME_e);
}

static view_class* getView() {
    if (dComIfGp_getWindowNum() == 0) {
        return NULL;
    }

    int camID = dComIfGp_getWindow(0)->getCameraID();
    return dComIfGp_getCamera(camID);
}

f32 dDemo_camera_c::JSGGetProjectionNear() const {
    view_class* view = getView();
    
    if (view == NULL) {
        return 0.0f;
    }
    
    return view->near;
}

void dDemo_camera_c::JSGSetProjectionNear(f32 i_projNear) {
    mProjNear = i_projNear;
    onEnable(ENABLE_PROJ_NEAR_e);
}

f32 dDemo_camera_c::JSGGetProjectionFar() const {
    view_class* view = getView();
    
    if (view == NULL) {
        return 1.0f;
    }
    
    return view->far;
}

void dDemo_camera_c::JSGSetProjectionFar(f32 i_projFar) {
    mProjFar = i_projFar;
    onEnable(ENABLE_PROJ_FAR_e);
}

f32 dDemo_camera_c::JSGGetProjectionFovy() const {
    view_class* view = getView();
    
    if (view == NULL) {
        return 60.0f;
    }
    
    return view->fovy;
}

void dDemo_camera_c::JSGSetProjectionFovy(f32 i_projFovy) {
    mProjFovy = i_projFovy;
    onEnable(ENABLE_PROJ_FOVY_e);
}

f32 dDemo_camera_c::JSGGetProjectionAspect() const {
    view_class* view = getView();
    
    if (view == NULL) {
        return 1.3571428f;
    }
    
    return view->aspect;
}

void dDemo_camera_c::JSGSetProjectionAspect(f32 i_aspect) {
    mProjAspect = i_aspect;
    onEnable(ENABLE_PROJ_ASPECT_e);
}

void dDemo_camera_c::JSGGetViewPosition(Vec* o_position) const {
    view_class* view = getView();
    
    if (view == NULL) {
        o_position->x = 0.0f;
        o_position->y = 0.0f;
        o_position->z = 0.0f;
    } else {
        *o_position = view->lookat.eye;
    }
}

void dDemo_camera_c::JSGSetViewPosition(Vec const& i_position) {
    mViewPos = i_position;
    onEnable(ENABLE_VIEW_POS_e);
}

void dDemo_camera_c::JSGGetViewUpVector(Vec* o_upVec) const {
    view_class* view = getView();
    
    if (view == NULL) {
        o_upVec->x = 0.0f;
        o_upVec->y = 1.0f;
        o_upVec->z = 0.0f;
    } else {
        *o_upVec = view->lookat.up;
    }
}

void dDemo_camera_c::JSGSetViewUpVector(Vec const& i_upVec) {
    mViewUpVector = i_upVec;
    onEnable(ENABLE_VIEW_UP_VEC_e);
}

void dDemo_camera_c::JSGGetViewTargetPosition(Vec* o_targetPos) const {
    view_class* view = getView();
    
    if (view == NULL) {
        o_targetPos->x = 0.0f;
        o_targetPos->y = 0.0f;
        o_targetPos->z = 1.0f;
    } else {
        *o_targetPos = view->lookat.center;
    }
}

void dDemo_camera_c::JSGSetViewTargetPosition(Vec const& i_targetPos) {
    mViewTargetVector = i_targetPos;
    onEnable(ENABLE_VIEW_TARG_POS_e);
}

f32 dDemo_camera_c::JSGGetViewRoll() const {
    view_class* view = getView();
    
    if (view == NULL) {
        return 0.0f;
    }
    
    return view->bank * 0.0054931640625f;
}

void dDemo_camera_c::JSGSetViewRoll(f32 i_roll) {
    mViewRoll = i_roll;
    onEnable(ENABLE_VIEW_ROLL_e);
}

void dDemo_ambient_c::JSGSetColor(GXColor i_color) {
    mColor = i_color;
    onEnable(ENABLE_COLOR_e);
}

void dDemo_light_c::JSGSetLightType(JStage::TELight i_lightType) {
    mLightType = i_lightType;
    onEnable(ENABLE_LIGHT_TYPE_e);
}

void dDemo_light_c::JSGSetPosition(Vec const& i_pos) {
    mPosition = i_pos;
    onEnable(ENABLE_POSITION_e);
}

void dDemo_light_c::JSGSetColor(GXColor i_color) {
    mColor = i_color;
    onEnable(ENABLE_COLOR_e);
}

void dDemo_light_c::JSGSetDistanceAttenuation(f32 param_0, f32 param_1, GXDistAttnFn i_distAttenFn) {
    field_0x24 = param_0;
    field_0x28 = param_1;
    mDistAttenFn = i_distAttenFn;
    onEnable(ENABLE_DIST_ATTEN_e);
}

void dDemo_light_c::JSGSetAngleAttenuation(f32 param_0, GXSpotFn i_spotFn) {
    field_0x2c = param_0;
    mSpotFn = i_spotFn;
    onEnable(ENABLE_ANGLE_ATTEN_e);
}

void dDemo_light_c::JSGSetDirection(Vec const& i_direction) {
    mDirection = i_direction;
    onEnable(ENABLE_DIRECTION_e);
}

void dDemo_fog_c::JSGSetFogFunction(GXFogType i_fogType) {
    mFogType = i_fogType;
    onEnable(ENABLE_FOG_FN_e);
}

void dDemo_fog_c::JSGSetStartZ(f32 i_startZ) {
    mStartZ = i_startZ;
    onEnable(ENABLE_START_Z_e);
}

void dDemo_fog_c::JSGSetEndZ(f32 i_endZ) {
    mEndZ = i_endZ;
    onEnable(ENABLE_END_Z_e);
}

void dDemo_fog_c::JSGSetColor(GXColor i_color) {
    mColor = i_color;
    onEnable(ENABLE_COLOR_e);
}

dDemo_object_c::dDemo_object_c() {
    mActorNum = 0;
    mLightNum = 0;
    mpCamera = NULL;
    mpAmbient = NULL;
    mpFog = NULL;
}

dDemo_object_c::~dDemo_object_c() {
    remove();
}

JStage::TObject* dDemo_object_c::appendActor(fopAc_ac_c* p_actor) {
    dDemo_actor_c* demoActor = dDemo_c::getActor(p_actor->demoActorID);

    if (demoActor != NULL) {
        return demoActor;
    }

    if (mActorNum >= 32) {
        // "Too many Demo Actors!!\n"
        OS_REPORT("デモアクター登録数オーバー！！\n");
        return NULL;
    }

    demoActor = new dDemo_actor_c();
    if (demoActor == NULL) {
        // "Failed to get Demo Actor!!\n"
        OS_REPORT("デモアクター確保失敗！！\n");
        return NULL;
    }

    u8 curActorNum = mActorNum;
    mActorNum++;
    mpActors[curActorNum] = demoActor;
    demoActor->setActor(p_actor);
    p_actor->demoActorID = mActorNum;

    if (p_actor->model != NULL) {
        demoActor->setModel((J3DModel*)p_actor->model);
    }

    return demoActor;
}

dDemo_actor_c* dDemo_object_c::getActor(u8 actorId) {
    if (actorId == 0 || actorId > 32) {
        return NULL;
    }

    return mpActors[actorId - 1];
}

JStage::TObject* dDemo_object_c::createCamera() {
    if (mpCamera != NULL) {
        // "2 demo cameras!!\n"
        OS_REPORT("デモカメラが２つも！！\n");
        return mpCamera;
    }

    mpCamera = new dDemo_camera_c();
    return mpCamera;
}

dDemo_camera_c* dDemo_object_c::getActiveCamera() {
    if (mpCamera != NULL) {
        return mpCamera;
    }

    return NULL;
}

JStage::TObject* dDemo_object_c::createAmbient() {
    if (mpAmbient != NULL) {
        // "2 demo ambient lights!\n"
        OS_REPORT("デモアンビエントライトが２つも！！\n");
        return mpAmbient;
    }

    mpAmbient = new dDemo_ambient_c();
    return mpAmbient;
}

JStage::TObject* dDemo_object_c::appendLight() {
    if (mLightNum >= 8) {
        // "Too many Demo Lights!!\n"
        OS_REPORT("デモライト登録数オーバー！！\n");
        return NULL;
    }

    dDemo_light_c* demoLight = new dDemo_light_c();
    if (demoLight == NULL) {
        // "Failed to get Demo Light!!\n"
        OS_REPORT("デモライト確保失敗！！\n");
        return NULL;
    }

    u8 curLightNum = mLightNum;
    mLightNum++;
    mpLights[curLightNum] = demoLight;

    return demoLight;
}

JStage::TObject* dDemo_object_c::createFog() {
    if (mpFog != NULL) {
        // "2 demo fogs!\n"
        OS_REPORT("デモフォグが２つも！！\n");
        return mpFog;
    }

    mpFog = new dDemo_fog_c();
    return mpFog;
}

void dDemo_object_c::remove() {
    while (mActorNum != 0) {
        delete mpActors[--mActorNum];
    }

    if (mpCamera != NULL) {
        delete mpCamera;
        mpCamera = NULL;
    }

    if (mpAmbient != NULL) {
        delete mpAmbient;
        mpAmbient = NULL;
    }

    while (mLightNum != 0) {
        delete mpLights[--mLightNum];
    }

    if (mpFog != NULL) {
        delete mpFog;
        mpFog = NULL;
    }
}

dDemo_fog_c::~dDemo_fog_c() {}

dDemo_light_c::~dDemo_light_c() {}

dDemo_ambient_c::~dDemo_ambient_c() {}

int dDemo_system_c::JSGFindObject(JStage::TObject** p_TObj, char const* actorName,
                                   JStage::TEObject objType) const {
    if (mpObject == NULL) {
        // "No Demo Object Class!!"
        OS_REPORT("デモオブジェクトクラスが無い！！");
        return true;
    }

    if (objType == JStage::OBJECT_ACTOR || objType == JStage::OBJECT_UNDEFINED) {
        fopAc_ac_c* actor = fopAcM_searchFromName(actorName, 0, 0);

        if (actor == NULL) {
            if (objType == JStage::OBJECT_ACTOR && !strncmp(actorName, "d_act", 5)) {
                actor =
                    (fopAc_ac_c*)fopAcM_fastCreate(actorName, 0, NULL, -1, NULL, NULL, NULL, NULL);
                if (actor == NULL) {
                    // "Generic-actor<%s> creation failed!!\n"
                    OS_WARNING("汎用くん<%s>生成失敗！！\n", actorName);
                    return true;
                }

                fopAcM_setStageLayer(actor);
                // "Generated Generic-actor<%s>!!\n"
                OS_REPORT("汎用くん<%s>生成！！\n", actorName);
            } else {
                // "No Demo Actors available!!\n"
                OS_WARNING("デモの出演者<%s>がいません！！\n", actorName);
                return true;
            }
        }

        *p_TObj = mpObject->appendActor(actor);
    } else if (objType == JStage::OBJECT_CAMERA) {
        *p_TObj = mpObject->createCamera();
    } else if (objType == JStage::OBJECT_AMBIENT) {
        *p_TObj = mpObject->createAmbient();
    } else if (objType == JStage::OBJECT_LIGHT) {
        *p_TObj = mpObject->appendLight();
    } else if (objType == JStage::OBJECT_FOG) {
        *p_TObj = mpObject->createFog();
    } else {
        // "Strange Demo Object type!! Type<%d>\n"
        OS_REPORT("へんなデモオブジェクトタイプです！！ タイプ<%d>\n");
        JUT_ASSERT(1755, FALSE);
    }

    return *p_TObj == NULL ? TRUE : FALSE;
}

void dDemo_c::create() {
    m_system = new dDemo_system_c();
    JUT_ASSERT(0, m_system != NULL);

    m_control = new JStudio::TControl();
    JUT_ASSERT(0, m_control != NULL);

    m_mesgControl = new jmessage_tControl();
    JUT_ASSERT(0, m_mesgControl != NULL);

    m_stage = new JStudio_JStage::TCreateObject(m_system);
    JUT_ASSERT(0, m_stage != NULL);

    m_audio = new JStudio_JAudio2::TCreateObject(Z2GetSoundStarter(), m_system);
    JUT_ASSERT(0, m_audio != NULL);
    m_audio->mPermit_onExit_notEnd = true;

    m_particle = new dDemo_particle_c(dPa_control_c::getEmitterManager(), m_system);
    JUT_ASSERT(0, m_particle != NULL);

    m_message = new jstudio_tCreateObject_message();
    JUT_ASSERT(0, m_message != NULL);

    m_factory = new JStudio::TFactory();
    JUT_ASSERT(0, m_factory != NULL);

    m_object = new dDemo_object_c();
    JUT_ASSERT(0, m_object != NULL);

    m_control->setSecondPerFrame(1.0f / 30.0f);
    m_control->setFactory(m_factory);
    m_factory->appendCreateObject(m_stage);
    m_factory->appendCreateObject(m_audio);
    m_factory->appendCreateObject(m_particle);
    m_factory->appendCreateObject(m_message);

    m_system->setObject(m_object);
    m_data = NULL;
    m_mode = 0;
    reset();
}

void dDemo_c::remove() {
    end();
    
    if (m_object != NULL) {
        delete m_object;
        m_object = NULL;
    }

    if (m_factory != NULL) {
        delete m_factory;
        m_factory = NULL;
    }

    if (m_message != NULL) {
        delete m_message;
        m_message = NULL;
    }

    if (m_particle != NULL) {
        delete m_particle;
        m_particle = NULL;
    }

    if (m_audio != NULL) {
        delete m_audio;
        m_audio = NULL;
    }

    if (m_stage != NULL) {
        delete m_stage;
        m_stage = NULL;
    }

    if (m_mesgControl != NULL) {
        delete m_mesgControl;
        m_mesgControl = NULL;
    }

    if (m_control != NULL) {
        delete m_control;
        m_control = NULL;
    }

    if (m_system != NULL) {
        delete m_system;
        m_system = NULL;
    }
}

dDemo_system_c::~dDemo_system_c() {}

jmessage_tControl::~jmessage_tControl() {}

int dDemo_c::start(u8 const* p_data, cXyz* p_translation, f32 rotationY) {
    JUT_ASSERT(0, m_system != NULL);

    m_control->reset();
    JStudio::TParse parser(m_control);

    if (!parser.parse(p_data, 0)) {
        // "Demo data read error!!\n"
        OSReport_Error("デモデータ読み込みエラー！！\n");
        return 0;
    }

    if (m_data == NULL) {
        m_control->setSuspend(0);
    }

    m_control->forward(0);
    m_translation = p_translation;

    if (p_translation != NULL) {
        static cXyz l_translation;
        l_translation = *m_translation;
        m_translation = &l_translation;
        m_rotationY = rotationY;

        m_control->transform_enable(true);
        m_control->transform_setOrigin(*m_translation, m_rotationY);
    }

    m_mode = 1;
    reset();

    m_data = NULL;
    m_branchData = (u8*)p_data;
    return 1;
}

void dDemo_c::end() {
    JUT_ASSERT(0, m_system != NULL);

    m_control->destroyObject_all();
    m_object->remove();
    m_data = NULL;
    m_mode = 0;
}

void dDemo_c::branch() {
    JUT_ASSERT(0, m_system != NULL);

    m_control->destroyObject_all();
    const u8* branchData = m_branchData;
    m_branchData = NULL;

    int rt = start(branchData, m_translation, m_rotationY);
    JUT_ASSERT(0, rt);
}

int dDemo_c::update() {
    JUT_ASSERT(0, m_system != NULL);

    if (m_data == NULL) {
        if (m_branchData == NULL) {
            return 0;
        } else {
            m_data = m_branchData;
            m_branchData = NULL;
            return 1;
        }
    } else if (isStatus(1)) {
        OS_REPORT(">>>>>>>>>>>>>>>>>>>>>>>>>>>> Movie Start Wait !\n");
        daMP_c::daMP_c_THPPlayerPlay();
        if (isStatus(1)) {
            return 1;
        }
    }

    if (m_control->forward(1) != 0) {
        m_frame++;

        if (m_control->getSuspend() <= 0) {
            m_frameNoMsg++;
        }
    } else {
        m_mode = 2;
    }

    if (m_branchData != NULL) {
        branch();
    }

    if (dComIfGs_staffroll_next_go_check() != 0) {
        dScnKy_env_light_c* env_light = &g_env_light;

        if (dComIfGs_staffroll_next_go_check() > 10) {
            env_light->staffroll_next_timer = 0;
            m_mode = 2;
        } else {
            if (dComIfGs_staffroll_next_go_check() == 1) {
                mDoGph_gInf_c::fadeOut(0.1f, g_blackColor);
            }
            env_light->staffroll_next_timer++;
        }
    }

    return 1;
}

void dDemo_c::setBranchType(u16 type) {
    m_branchType = type;
    m_branchId = -1;
}

void dDemo_c::setBranchId(u16 param_0, s16 branchId) {
    if (param_0 != m_branchType) {
        return;
    }

    m_branchId = branchId;
}

void dDemo_c::reset() {
    m_frame = 0;
    m_frameNoMsg = 0;
    m_status = 0;
    m_branchData = NULL;
}

JPABaseEmitter* dDemo_particle_c::emitter_create(u32 i_id) {
    cXyz pos(0.0f, 0.0f, 0.0f);
    return dComIfGp_particle_set(i_id, &pos, NULL, NULL, 0xFF, NULL,
                                 dComIfGp_roomControl_getStayNo(), NULL, NULL, NULL);
}

dDemo_particle_c::~dDemo_particle_c() {}

dDemo_camera_c::~dDemo_camera_c() {}

static void dummyString2() {
    DEAD_STRING("デモデータ再読み込みエラー！！\n");
}
