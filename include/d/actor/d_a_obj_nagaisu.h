#ifndef D_A_OBJ_NAGAISU_H
#define D_A_OBJ_NAGAISU_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_model.h"

class daObjNagaisu_c;

// child seat / couch?
class daObjIsuChild_c {
public:
    daObjIsuChild_c();
    virtual ~daObjIsuChild_c();
    int createHeap();
    void create(daObjNagaisu_c*, dCcD_Stts*);
    int execute();
    int draw(dMdl_c*);
    void Delete();
    bool chkHit();
    void callEmt();
    
    void setDeadFlg(bool i_flg) { mIsDead = i_flg; }
    bool getDeadFlg() { return mIsDead; }

    void setPos(const cXyz& i_pos) {
        mPos.x = i_pos.x;
        mPos.y = i_pos.y;
        mPos.z = i_pos.z;
    }

    static const dCcD_SrcCyl s_CcDCyl;

    /* 0x004 */ dMdl_obj_c mMdlObj;
    /* 0x038 */ Vec mPos;
    /* 0x044 */ dCcD_Cyl mCcCyl[2];
    /* 0x2BC */ Mtx mBgMtx;
    /* 0x2EC */ dBgW* mpBgW;
    /* 0x2F0 */ bool mIsDead;
};

/**
 * @ingroup actors-objects
 * @class daObjNagaisu_c
 * @brief Couch
 *
 * @details
 *
 */
class daObjNagaisu_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjNagaisu_c();
    virtual ~daObjNagaisu_c();
    int createHeap();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    void setIsu();

    static const int REMOVE_ISU_IDX[];

    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ dCcD_Stts mCcStts;
    /* 0x5B4 */ daObjIsuChild_c mChild[60];
};

STATIC_ASSERT(sizeof(daObjNagaisu_c) == 0xb6e4);

#endif /* D_A_OBJ_NAGAISU_H */
