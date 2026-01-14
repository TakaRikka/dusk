#ifndef D_A_OBJ_AUTOMATA_H
#define D_A_OBJ_AUTOMATA_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

struct daObj_AutoMata_HIOParam {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

class daObj_AutoMata_Param_c {
public:
    virtual ~daObj_AutoMata_Param_c() {}

    static daObj_AutoMata_HIOParam const m;
};

#if DEBUG
class daObj_AutoMata_HIO_c : public mDoHIO_entry_c {
public:
    daObj_AutoMata_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daObj_AutoMata_HIOParam m;
};

#define OBJ_AUTOMATA_HIO_CLASS daObj_AutoMata_HIO_c
#else
#define OBJ_AUTOMATA_HIO_CLASS daObj_AutoMata_Param_c
#endif

/**
 * @ingroup actors-objects
 * @class daObj_AutoMata_c
 * @brief Falbi's Music Box
 *
 * @details
 *
 */
class daObj_AutoMata_c : public fopAc_ac_c {
private:
    /* 0x568 */ OBJ_AUTOMATA_HIO_CLASS* mpHIO;
    /* 0x56C */ mDoExt_McaMorfSO* mpMorf;
    /* 0x570 */ Z2Creature mCreature;
    /* 0x600 */ mDoExt_btkAnm mBtk;
    /* 0x618 */ request_of_phase_process_class mPhase;
    /* 0x630 */ dBgS_ObjAcch mAcch;
    /* 0x7F8 */ dCcD_Stts mStts;
    /* 0x834 */ dBgS_AcchCir field_0x834;
    /* 0x874 */ cBgS_GndChk mGndChk;
    /* 0x8B0 */ dCcD_Cyl mCyl;
    /* 0x9EC */ dCcD_Sph mSph;
    /* 0xB24 */ int mShadowId;
    /* 0xB28 */ f32 mGroundH;
    /* 0xB2C */ f32 mAnmPlaySpeed;
    /* 0xB30 */ u8 field_0xb30;
    /* 0xB31 */ u8 mCoNone;

public:
    virtual ~daObj_AutoMata_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    static void* srchTHouse(fopAc_ac_c*, void*);
    int setAnm(int, bool);
    void setEnvTevColor();
    void setRoomNo();
    void setMtx();
    void setSe();

    void setAnmPlaySpeed(f32 i_playspeed) {
        mAnmPlaySpeed = i_playspeed;
    }

    void setCoNone() {
        mCoNone = true;
    }
};

STATIC_ASSERT(sizeof(daObj_AutoMata_c) == 0xb38);

#endif /* D_A_OBJ_AUTOMATA_H */
