#ifndef D_A_OBJ_SNOWEFFTAG_H
#define D_A_OBJ_SNOWEFFTAG_H

#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daSnowEffTag_c
 * @brief Snow Effect Tag
 *
 * @details
 *
 */
class daSnowEffTag_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    bool playerAreaCheck();
    int Execute();
    int Draw();
    int Delete();

    int getArg0() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ bool mPlayedSound;
    /* 0x56C */ f32 mMaxSize;
    /* 0x570 */ f32 mSize;
    /* 0x574 */ f32 field_0x574;
    /* 0x578 */ u32 mEmitterID;
};

STATIC_ASSERT(sizeof(daSnowEffTag_c) == 0x57C);

class daSnowEffTag_HIO_c : public mDoHIO_entry_c {
public:
    daSnowEffTag_HIO_c();
    virtual ~daSnowEffTag_HIO_c();

    void genMessage(JORMContext*);

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 mDisplayRange;
    /* 0x6 */ u8 mTop;
    /* 0x7 */ u8 mBottom;
};

#endif /* D_A_OBJ_SNOWEFFTAG_H */
