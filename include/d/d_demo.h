#ifndef D_D_DEMO_H
#define D_D_DEMO_H

#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/JStudio/JStudio_JAudio2/control.h"
#include "JSystem/JStudio/JStudio_JParticle/object-particle.h"
#include "JSystem/JStudio/JStudio_JStage/object.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"
#include "Z2AudioLib/Z2SoundStarter.h"
#include "f_op/f_op_actor_mng.h"

class J3DModel;
class dDemo_actor_c;
class dDemo_camera_c;
class dDemo_ambient_c;
class dDemo_light_c;
class dDemo_fog_c;
class fopAc_ac_c;
struct jmessage_tControl;

class dDemo_object_c {
public:
    dDemo_object_c();
    ~dDemo_object_c();
    JStage::TObject* appendActor(fopAc_ac_c*);
    dDemo_actor_c* getActor(u8);
    JStage::TObject* createCamera();
    dDemo_camera_c* getActiveCamera();
    JStage::TObject* createAmbient();
    JStage::TObject* appendLight();
    JStage::TObject* createFog();
    void remove();

    /* 0x00 */ u8 mActorNum;
    /* 0x01 */ u8 mLightNum;
    /* 0x04 */ dDemo_actor_c* mpActors[32];
    /* 0x84 */ dDemo_camera_c* mpCamera;
    /* 0x88 */ dDemo_ambient_c* mpAmbient;
    /* 0x8C */ dDemo_light_c* mpLights[8];
    /* 0xAC */ dDemo_fog_c* mpFog;
};

// TODO: made up, figure out what this is
struct dDemo_prm_data {
    /* 0x0 */ u8 field_0x0[0x4 - 0x0];
    /* 0x4 */ s8 field_0x4;
    /* 0x5 */ u8 field_0x5[0x6 - 0x5];
    /* 0x6 */ s8 field_0x6;
    /* 0x7 */ s8 field_0x7;
    /* 0x8 */ u8 field_0x8[0xB - 0x8];
    /* 0xB */ s8 field_0xb;
    /* 0xC */ u8 field_0xc[0xF - 0xC];
    /* 0xF */ s8 field_0xf;
};

class dDemo_prm_c {
public:
    dDemo_prm_c() { mData = NULL; }
    dDemo_prm_data* getData() { return mData; }

    /* 0x0 */ u32 field_0x0;
    /* 0x4 */ dDemo_prm_data* mData;
};

class dDemo_actor_c : public JStage::TActor {
public:
    enum Enable_e {
        ENABLE_UNK_e = 1,
        ENABLE_TRANS_e = (1 << 1),
        ENABLE_SCALE_e = (1 << 2),
        ENABLE_ROTATE_e = (1 << 3),
        ENABLE_SHAPE_e = (1 << 4),
        ENABLE_ANM_e = (1 << 5),
        ENABLE_ANM_FRAME_e = (1 << 6),
        ENABLE_ANM_TRANSITION_e = (1 << 7),
        ENABLE_TEX_ANM = (1 << 8),
        ENABLE_TEX_ANM_FRAME_e = (1 << 9),
    };

    dDemo_actor_c();
    fopAc_ac_c* getActor();
    void setActor(fopAc_ac_c*);
    f32 getPrm_Morf();
    int getDemoIDData(int*, int*, int*, u16*, u8*);
    
    virtual ~dDemo_actor_c();
    virtual void JSGSetData(u32, void const*, u32);
    virtual s32 JSGFindNodeID(char const* param_0) const {
        JUT_ASSERT(0, mModel != NULL);
        return mModel->getModelData()->getJointName()->getIndex(param_0);
    }
    virtual bool JSGGetNodeTransformation(u32 param_0, Mtx param_1) const {
        JUT_ASSERT(0, mModel != NULL);
        cMtx_copy(mModel->getAnmMtx((u16)param_0), param_1);
        return true;
    }
    virtual void JSGSetTranslation(Vec const&);
    virtual void JSGSetScaling(Vec const&);
    virtual void JSGSetRotation(Vec const&);
    virtual void JSGSetShape(u32);
    virtual void JSGSetAnimation(u32);
    virtual void JSGSetAnimationFrame(f32);
    virtual f32 JSGGetAnimationFrameMax() const {
        return mAnmFrameMax;
    }
    virtual void JSGSetAnimationTransition(f32);
    virtual void JSGSetTextureAnimation(u32);
    virtual void JSGSetTextureAnimationFrame(f32);
    virtual f32 JSGGetTextureAnimationFrameMax() const {
        return mTexAnmFrameMax;
    }
    virtual void JSGGetTranslation(Vec* o_trans) const {
        *o_trans = mTrans;
    }
    virtual void JSGGetScaling(Vec* o_scale) const {
        *o_scale = mScale;
    }
    virtual void JSGGetRotation(Vec* param_0) const {
        param_0->x = mRotate.x * 0.005493164f;
        param_0->y = mRotate.y * 0.005493164f;
        param_0->z = mRotate.z * 0.005493164f;
    }

    void setModel(J3DModel* p_model) { mModel = p_model; }
    BOOL checkEnable(u16 flag) { return mFlags & flag; }
    void onEnable(u16 flag) { mFlags |= flag; }
    void offEnable(u16 flag) { mFlags &= (u16)~flag; }
    cXyz& getTrans() { return mTrans; }
    cXyz& getScale() { return mScale; }
    csXyz& getRatate() { return mRotate; }
    u32 getAnmId() { return mAnmId; }
    u32 getOldAnmId() { return mOldAnmId; }
    void setOldAnmId(u32 id) { mOldAnmId = id; }
    void setAnmFrameMax(f32 max) { mAnmFrameMax = max; }
    f32 getAnmFrame() { return mAnmFrame; }
    dDemo_prm_c* getPrm() { return &mPrm; }
    f32 getAnmTransition() { return mAnmTransition; }
    u32 getShapeId() { return mShape; }

private:
    /* 0x04 */ u16 mFlags;
    /* 0x08 */ cXyz mTrans;
    /* 0x14 */ cXyz mScale;
    /* 0x20 */ csXyz mRotate;
    /* 0x28 */ u32 mShape;
    /* 0x2C */ u32 mAnmId;
    /* 0x30 */ f32 mAnmFrame;
    /* 0x34 */ f32 mAnmTransition;
    /* 0x38 */ f32 mAnmFrameMax;
    /* 0x3C */ u32 mTexAnm;
    /* 0x40 */ f32 mTexAnmFrame;
    /* 0x44 */ f32 mTexAnmFrameMax;
    /* 0x48 */ J3DModel* mModel;
    /* 0x4C */ dDemo_prm_c mPrm;
    /* 0x54 */ u32 field_0x54;
    /* 0x58 */ fpc_ProcID mActorId;
    /* 0x5C */ u32 mOldAnmId;
    /* 0x60 */ u32 mBtpId;
    /* 0x64 */ u32 mBtkId;
    /* 0x68 */ u32 mBrkId;
};

class dDemo_system_c : public JStage::TSystem {
public:
    dDemo_system_c() { mpObject = NULL; }

    virtual int JSGFindObject(JStage::TObject**, char const*,
                                              JStage::TEObject) const;
    virtual ~dDemo_system_c();

    void setObject(dDemo_object_c* i_object) { mpObject = i_object; }

private:
    /* 0x4 */ dDemo_object_c* mpObject;
};

class dDemo_particle_c : public JStudio_JParticle::TCreateObject {
public:
    dDemo_particle_c(JPAEmitterManager* p_emitMgr, const JStage::TSystem* p_system)
        : JStudio_JParticle::TCreateObject(p_emitMgr, p_system) {}

    virtual ~dDemo_particle_c();
    virtual JPABaseEmitter* emitter_create(u32);
};

class dDemo_light_c : public JStage::TLight {
public:
    enum Enable_e {
        ENABLE_LIGHT_TYPE_e = (1 << 0),
        ENABLE_POSITION_e = (1 << 1),
        ENABLE_COLOR_e = (1 << 2),
        ENABLE_DIST_ATTEN_e = (1 << 3),
        ENABLE_ANGLE_ATTEN_e = (1 << 4),
        ENABLE_DIRECTION_e = (1 << 5),
    };

    dDemo_light_c() {
        mFlags = 0;
    }

    virtual void JSGSetLightType(JStage::TELight);
    virtual void JSGSetPosition(Vec const&);
    virtual void JSGSetColor(GXColor);
    virtual void JSGSetDistanceAttenuation(f32, f32, GXDistAttnFn);
    virtual void JSGSetAngleAttenuation(f32, GXSpotFn);
    virtual void JSGSetDirection(Vec const&);
    virtual ~dDemo_light_c();

    void onEnable(u8 flag) { mFlags |= flag; }

private:
    /* 0x04 */ u8 mFlags;
    /* 0x08 */ JStage::TELight mLightType;
    /* 0x0C */ GXDistAttnFn mDistAttenFn;
    /* 0x10 */ GXSpotFn mSpotFn;
    /* 0x14 */ cXyz mPosition;
    /* 0x20 */ GXColor mColor;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ cXyz mDirection;
};

class dDemo_fog_c : public JStage::TFog {
public:
    enum Enable_e {
        ENABLE_FOG_FN_e = (1 << 0),
        ENABLE_START_Z_e = (1 << 1),
        ENABLE_END_Z_e = (1 << 2),
        ENABLE_COLOR_e = (1 << 3),
    };

    dDemo_fog_c() {
        mFlags = 0;
    }

    virtual void JSGSetFogFunction(GXFogType);
    virtual void JSGSetStartZ(f32);
    virtual void JSGSetEndZ(f32);
    virtual void JSGSetColor(GXColor);
    virtual ~dDemo_fog_c();

    void onEnable(u8 flag) { mFlags |= flag; }

private:
    /* 0x04 */ u8 mFlags;
    /* 0x05 */ u8 mFogType;
    /* 0x08 */ f32 mStartZ;
    /* 0x0C */ f32 mEndZ;
    /* 0x10 */ GXColor mColor;
};

class dDemo_camera_c : public JStage::TCamera {
public:
    enum Enable_e {
        ENABLE_PROJ_NEAR_e = (1 << 0),
        ENABLE_PROJ_FAR_e = (1 << 1),
        ENABLE_PROJ_FOVY_e = (1 << 2),
        ENABLE_PROJ_ASPECT_e = (1 << 3),
        ENABLE_VIEW_POS_e = (1 << 4),
        ENABLE_VIEW_UP_VEC_e = (1 << 5),
        ENABLE_VIEW_TARG_POS_e = (1 << 6),
        ENABLE_VIEW_ROLL_e = (1 << 7),
    };

    dDemo_camera_c() {
        mFlags = 0;
    }

    virtual f32 JSGGetProjectionNear() const;
    virtual void JSGSetProjectionNear(f32);
    virtual f32 JSGGetProjectionFar() const;
    virtual void JSGSetProjectionFar(f32);
    virtual f32 JSGGetProjectionFovy() const;
    virtual void JSGSetProjectionFovy(f32);
    virtual f32 JSGGetProjectionAspect() const;
    virtual void JSGSetProjectionAspect(f32);
    virtual void JSGGetViewPosition(Vec*) const;
    virtual void JSGSetViewPosition(Vec const&);
    virtual void JSGGetViewUpVector(Vec*) const;
    virtual void JSGSetViewUpVector(Vec const&);
    virtual void JSGGetViewTargetPosition(Vec*) const;
    virtual void JSGSetViewTargetPosition(Vec const&);
    virtual f32 JSGGetViewRoll() const;
    virtual void JSGSetViewRoll(f32);
    virtual ~dDemo_camera_c();

    void onEnable(u8 flag) { mFlags |= flag; }
    bool checkEnable(u8 flag) { return mFlags & flag; }

    cXyz& getTarget() { return mViewTargetVector; }
    cXyz& getTrans() { return mViewPos; }
    cXyz& getUp() { return mViewUpVector; }
    f32 getFovy() { return mProjFovy; }
    f32 getRoll() { return mViewRoll; }

private:
    /* 0x04 */ u8 mFlags;
    /* 0x08 */ f32 mProjNear;
    /* 0x0C */ f32 mProjFar;
    /* 0x10 */ f32 mProjFovy;
    /* 0x14 */ f32 mProjAspect;
    /* 0x18 */ cXyz mViewPos;
    /* 0x24 */ cXyz mViewUpVector;
    /* 0x30 */ cXyz mViewTargetVector;
    /* 0x3C */ f32 mViewRoll;
};

class dDemo_ambient_c : public JStage::TAmbientLight {
public:
    enum Enable_e {
        ENABLE_COLOR_e = (1 << 0),
    };

    dDemo_ambient_c() {
        mFlags = 0;
    }

    virtual void JSGSetColor(GXColor);
    virtual ~dDemo_ambient_c();

    void onEnable(u8 flag) { mFlags |= flag; }

private:
    /* 0x04 */ u8 mFlags;
    /* 0x05 */ GXColor mColor;
};

namespace {
class jstudio_tCreateObject_message : public JStudio::TCreateObject {
public:
    jstudio_tCreateObject_message() {}

    virtual ~jstudio_tCreateObject_message();
    virtual bool create(JStudio::TObject**, const JStudio::stb::data::TParse_TBlock_object&);
};

class jstudio_tAdaptor_message : public JStudio::TAdaptor_message {
public:
    typedef JStudio::TObject_message ObjectType;

    jstudio_tAdaptor_message() {}

    virtual ~jstudio_tAdaptor_message();
    virtual void adaptor_do_MESSAGE(JStudio::data::TEOperationData, const void*, u32);
};

};  // namespace

int dDemo_setDemoData(fopAc_ac_c*, u8, mDoExt_McaMorf*, char const*, int, u16*, u32, s8);

class dDemo_c {
public:
    static void create();
    static void remove();
    static int start(u8 const*, cXyz*, f32);
    static void end();
    static void branch();
    static int update();
    static void setBranchType(u16);
    static void setBranchId(u16, s16);
    static void reset();

    static dDemo_actor_c* getActor(u8 param_0) {
        JUT_ASSERT(546, m_object != NULL);
        return m_object->getActor(param_0);
    }

    static u32 getFrameNoMsg() { return m_frameNoMsg; }
    static s32 getMode() { return m_mode; }
    static u32 getFrame() { return m_frame; }
    static JStudio::stb::TControl* getControl() { return m_control; }
    static bool isStatus(u32 status) { return m_status & status; }
    static void onStatus(u32 status) { m_status |= status; }
    static void offStatus(u32 status) { m_status &= ~status; }
    static void setBranchData(const u8* p_data) { m_branchData = p_data; }
    static s16 getBranchId() { return m_branchId; }
    static s16 getBranchNum() { return m_branchNum; }
    static jmessage_tControl* getMesgControl() { return m_mesgControl; }

    static dDemo_camera_c* getCamera() {
        JUT_ASSERT(0, m_object != NULL);
        return m_object->getActiveCamera();
    }

    static s16 m_branchId;
    static s16 m_branchNum;
    static dDemo_system_c* m_system;
    static JStudio::TControl* m_control;
    static JStudio_JStage::TCreateObject* m_stage;
    static JStudio_JAudio2::TCreateObject* m_audio;
    static dDemo_particle_c* m_particle;
    static jstudio_tCreateObject_message* m_message;
    static JStudio::TFactory* m_factory;
    static jmessage_tControl* m_mesgControl;
    static dDemo_object_c* m_object;
    static const u8* m_data;
    static int m_frame;
    static cXyz* m_translation;
    static f32 m_rotationY;
    static u32 m_frameNoMsg;
    static s32 m_mode;
    static u32 m_status;
    static u16 m_branchType;
    static const u8* m_branchData;
};

#endif /* D_D_DEMO_H */
