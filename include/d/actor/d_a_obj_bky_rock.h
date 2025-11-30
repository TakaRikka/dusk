#ifndef D_A_OBJ_BKY_ROCK_H
#define D_A_OBJ_BKY_ROCK_H

#include "d/d_cc_d.h"
#include "d/d_model.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daBkyRock_c
 * @brief Bomb Shack Rock
 *
 * @details
 *
 */
class daBkyRock_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    struct _pieceData {
    public:
        /* 0x00 */ dMdl_obj_c mMdlObj;
        /* 0x34 */ Vec position;
        /* 0x40 */ f32 targetX;
        /* 0x44 */ f32 targetZ;
        /* 0x48 */ f32 scale;
        /* 0x4C */ f32 speedY;
        /* 0x50 */ s16 rotationX;
        /* 0x52 */ s16 rotationZ;
        /* 0x54 */ s16 rotationSpeed;
    };

    typedef void (daBkyRock_c::*exeProc)();

    enum Mode {
        MODE_0,
        MODE_1,
        MODE_2,
    };

    daBkyRock_c();
    virtual ~daBkyRock_c();
    int create();
    int execute();
    int draw();
    int Delete();
    void setModelMtx();
    int createHeap();
    void init();
    void middleExe();
    void exeModeNoDamage();
    void exeModeDamage();
    void exeModePiece();
    bool chkHitBombArrow();
    void initChangeModeBefore();
    void initChangeModeAfter();
    void pieceMove();
    void pieceMoveInit();
    void callBombEmt(int, u16 const*);

    u8 getSwBit0() { return fopAcM_GetParamBit(this, 4, 8); }
    u8 getSwBit1() { return fopAcM_GetParamBit(this, 12, 8); }
    s8 getNameNo() { return fopAcM_GetParamBit(this, 0, 4); }

    static dCcD_SrcCyl const s_CcDCyl;
    static exeProc s_exeProc[3];
    
private:
    /* 0x568 */ int mVibrationTimer;
    /* 0x578 */ u8 mMode;
    /* 0x579 */ bool field_0x579;
    /* 0x57A */ bool field_0x57a;
    /* 0x57B */ Mtx mMtx;
    /* 0x5AC */ dBgW* mBgW;
    /* 0x5B0 */ dCcD_Stts mStts;
    /* 0x5EC */ dCcD_Cyl mCyls[2];
    /* 0x864 */ J3DModel* mModels[3];
    /* 0x870 */ _pieceData mPieces[20];
};

//STATIC_ASSERT(sizeof(daBkyRock_c) == 0xf50);


#endif /* D_A_OBJ_BKY_ROCK_H */
