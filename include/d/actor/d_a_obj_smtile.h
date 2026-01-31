#ifndef D_A_OBJ_SMTILE_H
#define D_A_OBJ_SMTILE_H

#include "f_op/f_op_actor_mng.h"

struct daObj_SMTile_HIOParam {
    /* 0x00 */ f32 field_0x0;
    /* 0x04 */ f32 field_0x4;
};

class daObj_SMTile_Param_c {
public:
    virtual ~daObj_SMTile_Param_c() {}

    static daObj_SMTile_HIOParam const m;
};

#if DEBUG
class daObj_SMTile_HIO_c : public mDoHIO_entry_c {
public:
    daObj_SMTile_HIO_c();

    void listenPropertyEvent(const JORPropertyEvent*);

    void genMessage(JORMContext*);

    daObj_SMTile_HIOParam m;
};

#define OBJ_SMTILE_HIO_CLASS daObj_SMTile_HIO_c
#else
#define OBJ_SMTILE_HIO_CLASS daObj_SMTile_Param_c
#endif

/**
 * @ingroup actors-objects
 * @class daObj_SMTile_c
 * @brief Sacred Meadow Tile
 *
 * @details
 *
 */
class daObj_SMTile_c : public fopAc_ac_c {
private:
    /* 0x568 */ mDoExt_brkAnm mBrk;
    /* 0x580 */ OBJ_SMTILE_HIO_CLASS* mpHIO;
    /* 0x584 */ request_of_phase_process_class mPhase;
    /* 0x58C */ J3DModel* mModel;
    /* 0x590 */ cXyz field_0x590[21];
    /* 0x68C */ cXyz field_0x68c[21];
    /* 0x788 */ cXyz field_0x788[42];
    /* 0x980 */ int mParticleTimers[21];
    /* 0x9D4 */ int field_0x9d4[21];
    /* 0xA28 */ int field_0xa28[21];
    /* 0xA7C */ int field_0xa7c;
    /* 0xA80 */ JPABaseEmitter* mEmitters[21];
    /* 0xAD4 */ u32 mParticleIds[21];
    /* 0xB28 */ u8 mType;
    /* 0xB29 */ u8 field_0xb29;
    /* 0xB2A */ u8 field_0xb2a;
    /* 0xB2B */ u8 field_0xb2b;
public:
    virtual ~daObj_SMTile_c();
    int create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    void setDstPos();
    void setPrtcls(int, int);
    void touchPrtcls(f32);
    void setMtx();

    u8 getType() { return 0; }
    u8 getBitSW() { return (fopAcM_GetParam(this) & 0xff00) >> 8; }
    bool checkEndMove() { return field_0xb2a == 1; } 
    void reset() { field_0xb29 = 0; field_0xb2a = 0; field_0xb2b = 1; }
    void slide() { field_0xb29 = 1; }
};

STATIC_ASSERT(sizeof(daObj_SMTile_c) == 0xb30);


#endif /* D_A_OBJ_SMTILE_H */
