#ifndef D_A_OBJ_GLOWSPHERE_H
#define D_A_OBJ_GLOWSPHERE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

class daGlwSph_c;
class _GlSph_LstInfo_c{
public:
    _GlSph_LstInfo_c() {
        mpSph = NULL;
        mIsSet = false;
    }

    ~_GlSph_LstInfo_c() {}

    void Set(daGlwSph_c* i_sph) {
        if (!mIsSet && i_sph != NULL) {
            mpSph = i_sph;
            mIsSet = true;
        }
    }

    void Remove() {
        mpSph = NULL;
        mIsSet = false;
    }

    bool isSet() { return mIsSet; }
    daGlwSph_c* getpSph() { return mpSph; }

private:
    /* 0x0 */ daGlwSph_c* mpSph;
    /* 0x4 */ s8 mIsSet;
};

class _GlSph_Mng_c {
public:
    _GlSph_Mng_c() {
        field_0x0 = 0;
    }

    ~_GlSph_Mng_c() {
        reset();
    }

    void _clrLstBuf();
    int entry(daGlwSph_c*);
    void remove(daGlwSph_c*);
    int _setting_main();
    BOOL _chkAllGet_main();
    void SphSeProc();
    static void clrSphSe();
    static u16 getSphSe();
    static void incSphSe();

    void setting() {
        _setting_main();
    }

    BOOL isAllGet() {
        return _chkAllGet_main();
    }

    void reset() {
        field_0x0 = 0;
        _clrLstBuf();
    }

    static u16 mSphSe;
    static s16 mSeClrTmr;

    /* 0x0 */ int field_0x0;
    /* 0x4 */ _GlSph_LstInfo_c mListBuf[120];
};

/**
 * @ingroup actors-objects
 * @class daGlwSph_c
 * @brief STAR Game Glow Sphere
 *
 * @details
 *
 */
class daGlwSph_c : public fopAc_ac_c {
public:
    enum COLOR_e {
        COLOR_GRAY_e,
        COLOR_RED_e,
        COLOR_YELLOW_e,
        COLOR_BLUE_e,

        COLOR_DEFAULT_e = 0xFF,
    };

    enum ACTION_e {
        ACTION_WAIT_e,
        ACTION_GET_e,
        ACTION_MOVE_e,
    };

    static _GlSph_Mng_c& getSphMng();
    void setBaseMtx();
    static int createHeapCallBack(fopAc_ac_c*);
    int CreateHeap();
    int create();
    int execute();
    void actionMain();
    void effectSet();
    void getSE();
    void getVibe();
    void actionWaitInit();
    void actionWait();
    void actionGetInit();
    void actionGet();
    void actionMoveInit();
    void actionMove();
    int draw();
    int _delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getArg0() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getColor() { return fopAcM_GetParamBit(this, 0x10, 8); }

    void onGetFlag() { mGetFlag = true; }
    void offGetFlag() { mGetFlag = false; }
    void saveGetFlag() { field_0x951 = mGetFlag; }
    bool is_getted() { return mGetFlag == true; }

    void reset() {
        current.pos = home.pos;
        mSphCollider.OnCoSetBit();
        offGetFlag();
        saveGetFlag();
    }

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcSph mCcDSph;
    static _GlSph_Mng_c mSphMng;

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_btkAnm mBtk;
    /* 0x58C */ mDoExt_brkAnm mBrk;
    /* 0x5A4 */ dBgS_ObjAcch mAcch;
    /* 0x77C */ dBgS_AcchCir mAcchCir;
    /* 0x7BC */ dCcD_Stts mColliderStts;
    /* 0x7F8 */ dCcD_Sph mSphCollider;
    /* 0x930 */ LIGHT_INFLUENCE mLight;
    /* 0x950 */ u8 mGetFlag;
    /* 0x951 */ u8 field_0x951;
    /* 0x952 */ u8 mAction;
    /* 0x953 */ u8 mIsNoMoveHome;
    /* 0x954 */ u8 mColor;
    /* 0x955 */ u8 mMoveTimer;
    /* 0x958 */ f32 mMoveSpeed;
    /* 0x95C */ cXyz mMoveVec;
    /* 0x968 */ cXyz field_0x968;
    /* 0x974 */ cXyz field_0x974;
    /* 0x980 */ s16 field_0x980;
    /* 0x984 */ u32 mEmitterIDs[2];
};

STATIC_ASSERT(sizeof(daGlwSph_c) == 0x98C);

class daGlwSph_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daGlwSph_HIO_c();
    virtual ~daGlwSph_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ f32 speed;
    /* 0x8 */ f32 speed2;
};

#endif /* D_A_OBJ_GLOWSPHERE_H */
