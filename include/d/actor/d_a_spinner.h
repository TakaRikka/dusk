#ifndef D_A_SPINNER_H
#define D_A_SPINNER_H

#include "d/d_path.h"
#include "d/d_particle_copoly.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-items
 * @class daSpinner_c
 * @brief Spinner
 *
 * @details Arbiter's Grounds dungeon item.
 *
 */
class daSpinner_c : public fopAc_ac_c {
public:
    enum daSpinner_TAG {
        TAG_NONE,
        TAG_START,
        TAG_2,
        TAG_INTO,
        TAG_INTO_INC_ROT,
        TAG_END,
    };

    int createHeap();
    int create();
    ~daSpinner_c();
    void setRoomInfo();
    void setMatrix();
    void setEffect();
    int posMove();
    void setReflectAngle();
    void setWallHit(s16, u32);
    void setAnm();
    int setNextPathNum();
    Vec* getPathNextPos();
    int checkLineWallHit(cXyz*, cXyz*);
    int checkPathMove();
    void setSpreadEffect();
    void clearSpreadEffect();
    int execute();
    int draw();

    void onPathForceRemove() { mPathForceRemove = true; }

    dPath* checkPathMoveNow() { return mpPathMove; }

    static u32 getWaitArg() { return 0; }

    BOOL getDeleteFlg() const { return mDeleteFlg; }

    bool checkSpinnerTagEnd() const { return mSpinnerTag == TAG_END; }
    bool checkSpinnerTagIntoIncRot() const { return mSpinnerTag == TAG_INTO_INC_ROT; }
    bool checkSpinnerTagInto() const { return mSpinnerTag == TAG_INTO; }
    bool checkGroundHit() { return mAcch.ChkGroundHit(); }

    u8 getButtonJump() const { return mButtonJump; }
    u8 getJumpFlg() const { return mJumpFlg; }

    void setMove(f32 i_speed, s16 i_moveTime) {
        fopAcM_SetParam(this, 1);
        speedF = i_speed;
        mRideMoveTime = i_moveTime;
        mCyl.OnAtSetBit();
    }

    void clearPathMove() { mpPathMove = NULL; }

    void setSpinnerTag(const cXyz& param_0) {
        if (mSpinnerTag == TAG_NONE) {
            mSpinnerTag = TAG_START;
        }

        clearPathMove();
        field_0xaa4 = param_0;
    }

    void offSpinnerTag() {
        if (mSpinnerTag != TAG_NONE) {
            mSpinnerTag = TAG_END;
        }
    }

    bool reflectAccept() {
        bool accept = false;
        if (!mBck.isStop() && mSpinnerTag == TAG_NONE) {
            accept = true;
        }

        return accept;
    }

    MtxPtr getModelMtx() { return mpModel->getBaseTRMtx(); }

    s16 getAngleY() { return shape_angle.y + mRotY; }

    void forceDelete() { mDeleteFlg = true; }

private:
    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_bckAnm mBck;
    /* 0x588 */ J3DAnmTransform* mpSpoutBck;
    /* 0x58C */ dBgS_AcchCir mAcchCir[3];
    /* 0x64C */ dBgS_LinkAcch mAcch;
    /* 0x824 */ dBgS_LinkLinChk mLinChk;
    /* 0x894 */ dPaPo_c mPaPo;
    /* 0x8CC */ Z2SoundObjSimple mSound;
    /* 0x8EC */ dCcD_Stts mStts;
    /* 0x928 */ dCcD_Cyl mCyl;
    /* 0xA64 */ dPath* mpPathMove;
    /* 0xA68 */ dPath* field_0xa68;
    /* 0xA6C */ u8 field_0xa6c;
    /* 0xA6D */ u8 mJumpFlg;
    /* 0xA6E */ s8 mReverb;
    /* 0xA6F */ u8 mSpinnerTag;
    /* 0xA70 */ u8 mButtonJump;
    /* 0xA71 */ u8 mDeleteFlg;
    /* 0xA72 */ u8 mPathNo;
    /* 0xA73 */ s8 mPathDirection;
    /* 0xA74 */ u8 field_0xa74;
    /* 0xA75 */ u8 mTrigJump;
    /* 0xA76 */ s8 field_0xa76;
    /* 0xA77 */ u8 mPathForceRemove;
    /* 0xA78 */ u8 field_0xa78;
    /* 0xA79 */ u8 field_0xa79;
    /* 0xA7A */ s16 mRideMoveTime;
    /* 0xA7C */ s16 field_0xa7c;
    /* 0xA7E */ s16 mRotY;
    /* 0xA80 */ s16 field_0xa80;
    /* 0xA82 */ s16 field_0xa82;
    /* 0xA84 */ f32 field_0xa84;
    /* 0xA88 */ f32 field_0xa88;
    /* 0xA8C */ u32 mSpreadEmitterIDs[2];
    /* 0xA94 */ u32 field_0xa94;
    /* 0xA98 */ cXyz field_0xa98;
    /* 0xAA4 */ cXyz field_0xaa4;
    /* 0xAB0 */ cXyz field_0xab0;
};

#endif /* D_A_SPINNER_H */
