#ifndef D_PARTICLE_D_PARTICLE_H
#define D_PARTICLE_D_PARTICLE_H

#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "d/d_particle_name.h"
#include "d/d_kankyo.h"

void dPa_cleanupGX();

class J3DAnmTexPattern;
class J3DModel;
class J3DModelData;
class JKRExpHeap;
class JKRSolidHeap;
class dKy_tevstr_c;
class fopAc_ac_c;

class dPa_levelEcallBack : public JPAEmitterCallBack {
public:
    virtual ~dPa_levelEcallBack() { cleanup(); }
    virtual void setup(JPABaseEmitter*, const cXyz*, const csXyz*, s8) = 0;
    virtual void cleanup() {}
};

class mDoDvdThd_toMainRam_c;
class cBgS_PolyInfo;
class JPAEmitterManager;
class JPADrawInfo;
class J3DAnmBase;
class dPa_simpleData_c;

class dPa_simpleEcallBack : public JPAEmitterCallBack {
public:
    dPa_simpleEcallBack();
    JPABaseEmitter* create(JPAEmitterManager*, u16, u8);
    JPABaseEmitter* createEmitter(JPAEmitterManager*);
    u32 set(cXyz const*, dKy_tevstr_c const*, u8, _GXColor const&, _GXColor const&,
                            int, f32);

    virtual ~dPa_simpleEcallBack() {}
    virtual void executeAfter(JPABaseEmitter*);
    virtual void draw(JPABaseEmitter*);

    void removeEmitter() { mEmitter = NULL; }
    u16 getID() const { return mID; }

    /* 0x04 */ JPABaseEmitter* mEmitter;
    /* 0x08 */ u16 mID;
    /* 0x0A */ u8 field_0xa;
    /* 0x0C */ s16 field_0xc;
    /* 0x0C */ u16 field_0xe;
    /* 0x10 */ dPa_simpleData_c* mData;
};  // Size: 0x14

class dPa_windPcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_windPcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_modelPcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_modelPcallBack() {}
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_modelEcallBack : public dPa_levelEcallBack {
public:
    struct model_c {
        bool set(J3DModelData*, dKy_tevstr_c const&, u8, void*, u8, u8);
        void setup();
        void cleanup();
        void draw(f32 (*)[4]);
        model_c() { field_0x0 = NULL; }

        void reset() {
            field_0x0 = NULL;
        }

        u8 getRotAxis() { return mRotAxis; }
        J3DModelData* getModelData() { return field_0x0; }

        J3DModelData* field_0x0;
        J3DAnmBase* field_0x4;
        dKy_tevstr_c field_0x8;
        u8 mRotAxis;
        u8 field_0x391;
        u8 field_0x392;
        u8 field_0x393;
    };

    dPa_modelEcallBack() { mModel = NULL; }

    static void create(u8);
    static void remove();
    static int setModel(JPABaseEmitter*, J3DModelData*, dKy_tevstr_c const&, u8,
                                        void*, u8, u8);
    static void resetModel(JPABaseEmitter*);
    static void setupModel(JPABaseEmitter*);
    static void drawModel(JPABaseEmitter*, f32 (*)[4]);
    static void cleanupModel(JPABaseEmitter*);
    static model_c* getModel(JPABaseEmitter*);
    static u8 getRotAxis(JPABaseEmitter*);

    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter* param_0) { cleanupModel(param_0); }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);

    static void setModel(JPABaseEmitter* param_0, J3DModelData* param_1,
                         const dKy_tevstr_c& param_2, u8 param_3, J3DAnmTexPattern* param_4,
                         u8 param_5) {
        setModel(param_0, param_1, param_2, param_3, param_4, 0, param_5);
    }

    static dPa_modelEcallBack& getEcallback() { return mEcallback; }
    static dPa_modelEcallBack mEcallback;

    static dPa_modelPcallBack mPcallback;
    static model_c* mModel;
};

class dPa_selectTexEcallBack : public dPa_levelEcallBack {
public:
    dPa_selectTexEcallBack(u8 param_0) { field_0x4 = param_0; }

    virtual ~dPa_selectTexEcallBack() {}
    virtual void draw(JPABaseEmitter*);
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8) {}

    /* 0x4 */ u8 field_0x4;
};

class dPa_followEcallBack : public dPa_levelEcallBack {
public:
    dPa_followEcallBack(u8 param_0 = 0, u8 param_1 = 0);

    virtual ~dPa_followEcallBack() {}
    virtual void execute(JPABaseEmitter*);
    virtual void draw(JPABaseEmitter*);
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
    virtual void cleanup() { end(); }
    virtual void end();

    void remove() { end(); }
    JPABaseEmitter* getEmitter() { return mpEmitter; }
    bool isEnd() {
        return field_0x10 & 1;
    }

    /* 0x04 */ JPABaseEmitter* mpEmitter;
    /* 0x08 */ const cXyz* field_0x8;
    /* 0x0C */ const csXyz* field_0xc;
    /* 0x10 */ u8 field_0x10;
    /* 0x11 */ u8 field_0x11;
    /* 0x12 */ u8 field_0x12;
    /* 0x13 */ u8 field_0x13;
};

class dPa_fsenthPcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_fsenthPcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_light8EcallBack : public dPa_levelEcallBack {
public:
    virtual ~dPa_light8EcallBack() {}
    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter*) { dPa_cleanupGX(); }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
};

class dPa_gen_b_light8EcallBack : public dPa_levelEcallBack {
public:
    virtual ~dPa_gen_b_light8EcallBack() {}
    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter*) { dPa_cleanupGX(); }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
};

class dPa_gen_d_light8EcallBack : public dPa_levelEcallBack {
public:
    virtual void draw(JPABaseEmitter*);
    virtual void drawAfter(JPABaseEmitter*) { dPa_cleanupGX(); }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);
};

class dPa_light8PcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_light8PcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_gen_b_light8PcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_gen_b_light8PcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_gen_d_light8PcallBack : public JPAParticleCallBack {
public:
    virtual ~dPa_gen_d_light8PcallBack() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
    virtual void draw(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_wbPcallBack_c : public JPAParticleCallBack {
public:
    virtual ~dPa_wbPcallBack_c() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_particleTracePcallBack_c : public JPAParticleCallBack {
public:
    virtual ~dPa_particleTracePcallBack_c() {}
    virtual void execute(JPABaseEmitter*, JPABaseParticle*);
};

class dPa_hermiteEcallBack_c : public dPa_levelEcallBack {
public:
    dPa_hermiteEcallBack_c() {}

    void setOldPosP(cXyz const*, cXyz const*);

    virtual ~dPa_hermiteEcallBack_c() {}
    virtual void executeAfter(JPABaseEmitter*);
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8);

    void setRate(f32 rate) { mRate = rate; }

private:
    /* 0x04 */ f32 mRate;
    /* 0x08 */ int mMaxCnt;
    /* 0x0C */ cXyz const* field_0xc;
    /* 0x10 */ cXyz const* field_0x10;
    /* 0x14 */ cXyz const* field_0x14;
};

class dPa_setColorEcallBack : public dPa_levelEcallBack {
public:
    dPa_setColorEcallBack(const GXColor& color) { mColor = color; }

    virtual ~dPa_setColorEcallBack() {}
    virtual void draw(JPABaseEmitter*) { GXSetTevColor(GX_TEVREG1, mColor); }
    virtual void setup(JPABaseEmitter*, cXyz const*, csXyz const*, s8) {}

    /* 0x4 */ GXColor mColor;
};

class dPa_simpleData_c {
public:
    ~dPa_simpleData_c();
    dPa_simpleData_c();

    cXyz field_0x00;
    GXColor field_0x0c;
    GXColor field_0x10;
};

class dPa_control_c {
public:
    class level_c {
    public:
        class emitter_c {
        public:
            void onActive();
            void entry(u32, u16, JPABaseEmitter*, dPa_levelEcallBack*);
            void cleanup();
            emitter_c() { mId = 0; }

            JPABaseEmitter* getEmitter() { return mEmitter; }
            emitter_c* getPre() { return mPre; }
            emitter_c* getNext() { return mNext; }
            void setPre(emitter_c* pre) { mPre = pre; }
            void setNext(emitter_c* next) { mNext = next; }

            u32 getId() { return mId; }
            void clearStatus() { mStatus = 0; }

            void onEventMove() { mStatus |= 2; }
            void offEventMove() { mStatus &= ~2; }
            bool isEventMove() { return mStatus & 2; }

            void offActive() { mStatus &= ~1; }
            bool isActive() { return mStatus & 1; }
            u16 getNameId() { return mNameId; }
            dPa_levelEcallBack* getCallback() { return mCallback; }

        private:
            /* 0x00 */ u32 mId;
            /* 0x04 */ u16 mNameId;
            /* 0x06 */ u8 mStatus;
            /* 0x08 */ JPABaseEmitter* mEmitter;
            /* 0x0C */ dPa_levelEcallBack* mCallback;
            /* 0x10 */ emitter_c* mPre;
            /* 0x14 */ emitter_c* mNext;
        };  // Size: 0x18

        level_c();
        void execute();
        void execute(dPa_control_c::level_c::emitter_c*);
        void cleanup();
        emitter_c* get(u32);
        void forceOnEventMove(u32);
        void allForceOnEventMove();
        JPABaseEmitter* getEmitter(u32);
        u32 entry(u16, JPABaseEmitter*, dPa_levelEcallBack*);
        void addTable(dPa_control_c::level_c::emitter_c*);
        void cutTable(dPa_control_c::level_c::emitter_c*);

        static const int EMITTER_MAX = 0x80;

        /* 0x000 */ u32 field_0x0;
        /* 0x004 */ u8 mFreeNo;
        /* 0x005 */ u8 field_0x5;
        /* 0x006 */ u8 field_0x6;
        /* 0x008 */ emitter_c mEmitter[0x80];
        /* 0xC08 */ emitter_c* mTable[0x100];
    };  // Size: 0x1008

    dPa_control_c();
    static u8 getRM_ID(u16);
    void createCommon(void const*);
    void createRoomScene();
    bool readScene(u8, mDoDvdThd_toMainRam_c**);
    void createScene(void const*);
    bool removeRoomScene(bool);
    void removeScene(bool);
    void cleanup();
    void calc3D();
    void calc2D();
    void calcMenu();
    void draw(JPADrawInfo*, u8);
    void setHitMark(u16, fopAc_ac_c*, cXyz const*, csXyz const*, cXyz const*, u32);
    void setWaterRipple(u32*, cBgS_PolyInfo&, cXyz const*, f32, dKy_tevstr_c const*,
                                       cXyz const*, s8);
    JPABaseEmitter* set(u8, u16, cXyz const*, dKy_tevstr_c const*, csXyz const*, cXyz const*, u8,
                           dPa_levelEcallBack*, s8, _GXColor const*, _GXColor const*, cXyz const*,
                           f32);
    u32 set(u32, u8, u16, cXyz const*, dKy_tevstr_c const*, csXyz const*,
                           cXyz const*, u8, dPa_levelEcallBack*, s8, _GXColor const*,
                           _GXColor const*, cXyz const*, f32);
    static s32 getPolyColor(cBgS_PolyInfo&, int, _GXColor*, _GXColor*, u8*, f32*);
    u32 setPoly(u32, u16, cBgS_PolyInfo&, cXyz const*, dKy_tevstr_c const*,
                               csXyz const*, cXyz const*, int, dPa_levelEcallBack*, s8,
                               cXyz const*);
    JPABaseEmitter* setPoly(u16, cBgS_PolyInfo&, cXyz const*, dKy_tevstr_c const*, csXyz const*,
                                cXyz const*, int, dPa_levelEcallBack*, s8, cXyz const*);
    bool newSimple(u16, u8, u32*);
    u32 setSimple(u16, cXyz const*, dKy_tevstr_c const*, u8, _GXColor const&,
                                  _GXColor const&, int, f32);
    dPa_simpleEcallBack* getSimple(u16);
    u32 setStopContinue(u32);
    u32 setSimpleFoot(u32, u32*, cBgS_PolyInfo&, cXyz const*, dKy_tevstr_c const*,
                                      int, csXyz const*, cXyz const*, dPa_levelEcallBack*, s8,
                                      cXyz const*);
    u16 setCommonPoly(u32*, cBgS_PolyInfo*, cXyz const*, cXyz const*,
                                      dKy_tevstr_c const*, u32, u32, csXyz const*, cXyz const*, s8);

    void forceOnEventMove(u32 param_0) { field_0x210.forceOnEventMove(param_0); }
    JPABaseEmitter* getEmitter(u32 param_0) { return field_0x210.getEmitter(param_0); }

    u32 setNormal(u32 param_0, u16 param_1, const cXyz* i_pos, const dKy_tevstr_c* param_3,
                  const csXyz* i_rotation, const cXyz* i_scale, u8 i_alpha, dPa_levelEcallBack* param_7,
                  s8 param_8, const GXColor* param_9, const GXColor* param_10, const cXyz* param_11,
                  f32 param_12) {
        return set(param_0, 0, param_1, i_pos, param_3, i_rotation, i_scale, i_alpha, param_7,
                   param_8, param_9, param_10, param_11, param_12);
    }

    JPABaseEmitter* setNormal(u16 param_1, const cXyz* i_pos, const dKy_tevstr_c* param_3,
                              const csXyz* i_rotation, const cXyz* i_scale, u8 i_alpha,
                              dPa_levelEcallBack* param_7, s8 param_8, const GXColor* param_9,
                              const GXColor* param_10, const cXyz* param_11, f32 param_12) {
        return set(0, param_1, i_pos, param_3, i_rotation, i_scale, i_alpha, param_7, param_8,
                   param_9, param_10, param_11, param_12);
    }

    void drawFogPri0_B(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 7); }
    void drawNormalPri0_B(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 5); }
    void drawFogPri0_A(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 6); }
    void drawNormalPri0_A(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 4); }
    void drawFogPri4(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 11); }
    void drawProjection(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 3); }
    void drawFogPri1(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 8); }
    void drawNormal(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 0); }
    void drawFogPri2(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 9); }
    void drawNormalFog(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 1); }
    void drawFogPri3(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 10); }
    void drawNormalP1(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 2); }
    void drawDarkworld(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 12); }
    void drawFogScreen(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 13); }
    void draw2Dgame(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 14); }
    void draw2Dfore(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 15); }
    void draw2Dback(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 16); }
    void draw2DmenuFore(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 17); }
    void draw2DmenuBack(JPADrawInfo* i_drawInfo) { draw(i_drawInfo, 18); }

    JKRSolidHeap* getHeap() { return mHeap; }
    JKRSolidHeap* getSceneHeap() { return m_sceneHeap; }
    JKRExpHeap* getResHeap() { return m_resHeap; }

    void levelAllForceOnEventMove() { field_0x210.allForceOnEventMove(); }

    void levelExecute(u32 param_0) {
        level_c::emitter_c* emitter = field_0x210.get(param_0);
        if (emitter != NULL) {
            field_0x210.execute(emitter);
        }
    }

    static void onStatus(u8 status) { mStatus |= status; }
    static void offStatus(u8 status) { mStatus &= ~status; }
    static bool isStatus(u8 status) { return mStatus & status; }

    static dPa_selectTexEcallBack* getTsuboSelectTexEcallBack(int idx) {
        return &mTsubo[idx];
    }

    static JPAEmitterManager* getEmitterManager() { return mEmitterMng; }

    static dPa_light8PcallBack* getLight8PcallBack() { 
        return &mLight8PcallBack; 
    }

    static dPa_gen_b_light8PcallBack* getGen_b_Light8PcallBack() {
        return &m_b_Light8PcallBack; 
    }

    static dPa_gen_d_light8PcallBack* getGen_d_Light8PcallBack() {
        return &m_d_Light8PcallBack; 
    }

    static dPa_gen_b_light8EcallBack* getGen_b_Light8EcallBack() {
        return &m_b_Light8EcallBack; 
    }

    static dPa_gen_d_light8EcallBack* getGen_d_Light8EcallBack() {
        return &m_d_Light8EcallBack; 
    }

    static dPa_light8EcallBack* getLight8EcallBack() {
        return &mLight8EcallBack; 
    }

    static dPa_particleTracePcallBack_c* getParticleTracePCB() {
        return &mParticleTracePCB; 
    }

    static dPa_fsenthPcallBack* getFsenthPcallBack() {
        return &mFsenthPcallBack; 
    }

    static JPAParticleCallBack* getWaterBubblePcallBack() {
        return &mWaterBubblePcallBack; 
    }

    static dPa_selectTexEcallBack mTsubo[8];
    static dPa_setColorEcallBack mLifeBall[3];
    static Mtx mWindViewMatrix;
    static JPAEmitterManager* mEmitterMng;
    static dPa_wbPcallBack_c mWaterBubblePcallBack;
    static dPa_fsenthPcallBack mFsenthPcallBack;
    static dPa_light8EcallBack mLight8EcallBack;
    static dPa_light8PcallBack mLight8PcallBack;
    static dPa_gen_b_light8EcallBack m_b_Light8EcallBack;
    static dPa_gen_b_light8PcallBack m_b_Light8PcallBack;
    static dPa_gen_d_light8EcallBack m_d_Light8EcallBack;
    static dPa_gen_d_light8PcallBack m_d_Light8PcallBack;
    static dPa_particleTracePcallBack_c mParticleTracePCB;
    static u8 mStatus;

private:
    /* 0x000 */ JKRSolidHeap* mHeap;
    /* 0x004 */ JPAResourceManager* mCommonResMng;
    /* 0x008 */ JKRExpHeap* m_resHeap;
    /* 0x00C */ JKRSolidHeap* m_sceneHeap;
    /* 0x010 */ void* m_sceneRes;
    /* 0x014 */ JPAResourceManager* mSceneResMng;
    /* 0x018 */ u8 field_0x18;
    /* 0x019 */ u8 field_0x19;
    /* 0x01A */ u8 field_0x1a;
    /* 0x01B */ u8 field_0x1b;
    /* 0x01C */ dPa_simpleEcallBack field_0x1c[25];
    /* 0x210 */ level_c field_0x210;
};

#endif /* D_PARTICLE_D_PARTICLE_H */
