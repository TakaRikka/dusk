#ifndef D_A_OBJ_NAMEPLATE_H
#define D_A_OBJ_NAMEPLATE_H

#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_msg_flow.h"

/**
 * @ingroup actors-objects
 * @class daObjNameplate_c
 * @brief Ordon Village Nameplate
 *
 * @details
 *
 */
class daObjNameplate_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void initCcSphere();
    void setCcSphere();
    void calcAngle();
    void calcSpring();
    int checkHitAngle();
    void messageProc();

    struct M_attrs {
        /* 0x00 */ f32 radius1;
        /* 0x04 */ f32 field_0x04;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0C;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ s16 field_0x18;
        /* 0x1A */ s16 field_0x1A;
        /* 0x1C */ s16 field_0x1C;
        // unknown types
        /* 0x1E */ u8 field_0x1E;
        /* 0x1F */ u8 field_0x1F;
    };

    static M_attrs const M_attr;
    static M_attrs const& attr() { return M_attr; }

    static char* l_arcName;

    ~daObjNameplate_c() { dComIfG_resDelete(&this->mPhase, l_arcName); }

    inline int create();
    inline int draw();
    inline int createHeap();

private:
    /* 0x568 */ J3DModel* model;
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ Mtx mMtx;
    /* 0x5A4 */ dCcD_Stts mCcDStts;
    /* 0x5E0 */ dCcD_Sph mCcDSph;
    /* 0x718 */ f32 field_0x718;
    /* 0x71C */ f32 field_0x71C;
    /* 0x720 */ s16 field_0x720;
    /* 0x722 */ s16 field_0x722;
    /* 0x724 */ s16 field_0x724;
    /* 0x726 */ u16 field_0x726;
    /* 0x728 */ dMsgFlow_c mMsgFlow;
    /* 0x774 */ s16 mShowMessage;
    /* 0x776 */ s16 mMessageFlowId;
    /* 0x778 */ s16 field_0x778;
    /* 0x77A */ s16 field_0x77A;
    /* 0x77C */ s16 field_0x77C;
    /* 0x77E */ s16 field_0x77E;
};

STATIC_ASSERT(sizeof(daObjNameplate_c) == 0x780);

#endif /* D_A_OBJ_NAMEPLATE_H */
