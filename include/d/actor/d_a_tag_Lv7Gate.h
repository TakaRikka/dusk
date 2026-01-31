#ifndef D_A_TAG_LV7GATE_H
#define D_A_TAG_LV7GATE_H

#include "d/d_com_inf_game.h"

class daTagLv7Gate_c : public fopAc_ac_c {
public:
    int create();
    inline void create_init();
    inline int execute();
    void calcFly();

    inline ~daTagLv7Gate_c();

    inline int createHeap();
    inline int setPath(int i_path_ID);
    inline void initBaseMtx();
    inline void setBaseMtx();
    inline void flyAnime();
    inline bool checkPoint(cXyz& i_point, f32 i_speed);
    inline bool setNextPoint();
    inline int draw();

    u8 getPathID() { return (fopAcM_GetParam(this) >> 16) & 0xFF; }
    s16 getFlowNodeNo() { return (u16)fopAcM_GetParam(this); }

    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_bckAnm* mpBck;
    /* 0x570 */ dPath* mRoomPath;
    /* 0x574 */ request_of_phase_process_class mPhase;
    /* 0x57C */ cXyz mPos2;
    /* 0x588 */ cXyz mPos1;
    /* 0x594 */ csXyz field_0x594;
    /* 0x59C */ char* mEvName;
    /* 0x5A0 */ int field_0x5a0;
    /* 0x5A4 */ f32 mDistance;
    /* 0x5A8 */ f32 mLastFrame;
    /* 0x5AC */ u16 field_0x5ac;
    /* 0x5AE */ s16 mEvtId;
    /* 0x5B0 */ bool field_0x5b0;
    /* 0x5B1 */ bool field_0x5b1;
};  // Size: 0x5B4

#endif /* D_A_TAG_LV7GATE_H */
