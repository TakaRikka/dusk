#ifndef D_A_NPC_RAFREL_H
#define D_A_NPC_RAFREL_H

#include "d/actor/d_a_npc4.h"

/**
 * @ingroup actors-npcs
 * @class daNpcRafrel_c
 * @brief Auru
 *
 * @details
 *
*/

struct daNpcRafrel_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 demo_start_dist;
    /* 0x70 */ u8 demo_start_area;
};

class daNpcRafrel_Param_c {
public:
    virtual ~daNpcRafrel_Param_c() {}

    static const daNpcRafrel_HIOParam m;
};

class daNpcRafrel_HIO_c : public mDoHIO_entry_c {
public:
    daNpcRafrel_HIO_c();
    void genMessage(JORMContext*);

    /* 0x8 */ daNpcRafrel_HIOParam m;
};

#if DEBUG
#define NPC_RAFREL_HIO_CLASS daNpcRafrel_HIO_c
#else
#define NPC_RAFREL_HIO_CLASS daNpcRafrel_Param_c
#endif

class daNpcRafrel_c : public daNpcF_c {
public:
    daNpcRafrel_c();
    
    int Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void reset();
    inline void setWaitAction();
    inline bool isSneaking();
    bool wait_type01(void*);
    bool wait_type2(void*);
    bool talk(void*);
    bool demo(void*);
    bool leave(void*);
    inline int EvCut_Introduction(int);
    inline int EvCut_Meeting(int);
    inline int EvCut_Appear(int);
    inline int EvCut_WiretapSponsor(int);
    inline int EvCut_WiretapEntrant(int);

    inline void playMotion();
    inline void lookat();
    inline void playExpression();

    virtual ~daNpcRafrel_c();
    virtual inline void setParam();
    virtual inline BOOL main();
    virtual inline BOOL ctrlBtk();
    virtual inline void setAttnPos();
    virtual inline bool setExpressionAnm(int, bool);
    virtual inline bool setExpressionBtp(int);
    virtual void setExpression(int, f32);
    virtual void setMotionAnm(int, f32);
    virtual void setMotion(int, f32, int);
    virtual inline BOOL drawDbgInfo();

    int setAction(bool (daNpcRafrel_c::*i_actionFn)(void*)) {
        field_0xe10 = 3;
        if (field_0xde0 != NULL) {
            (this->*field_0xde0)(NULL);
        }
    
        field_0xe10 = 0;
        field_0xde0 = i_actionFn;

        if (field_0xde0 != NULL) {
            (this->*field_0xde0)(NULL);
        }
    
        return 1;
    }

    inline void setLookMode(int i_lookMode, fopAc_ac_c* i_talkPartner);
    inline bool step(s16 i_angY, int param_1, f32 param_2);
    inline void searchActors();
    inline BOOL chkFindPlayer();
    inline void setExpressionTalkAfter();

    s16 getMessageNo() { return (fopAcM_GetParam(this) >> 8) & 0xFFFF; }

    static int (daNpcRafrel_c::*mEvtSeqList[])(int);

private:
    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ J3DModel* mpItemModel;
    /* 0xBDC */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBE0 */ daNpcF_Lookat_c mLookat;
    /* 0xC7C */ daNpcF_ActorMngr_c field_0xc7c[4];
    /* 0xC9C */ fopAc_ac_c* field_0xc9c;
    /* 0xCA0 */ NPC_RAFREL_HIO_CLASS* mpHIO;
    /* 0xCA4 */ dCcD_Cyl mCyl;
    /* 0xDE0 */ bool (daNpcRafrel_c::*field_0xde0)(void*);
    /* 0xDEC */ request_of_phase_process_class mPhase[2];
    /* 0xE04 */ JPABaseEmitter* field_0xdfc;
    /* 0xE08 */ fpc_ProcID field_0xe00;
    /* 0xE04 */ int field_0xe04;
    /* 0xE08 */ int field_0xe08;
    /* 0xE0C */ s16 field_0xe0c;
    /* 0xE0E */ s16 field_0xe0e;
    /* 0xE10 */ u16 field_0xe10;
    /* 0xE12 */ s16 field_0xe12;
    /* 0xE14 */ u8 field_0xe14;
    /* 0xE15 */ bool field_0xe15;
    /* 0xE16 */ u8 field_0xe16;
    /* 0xE17 */ u8 mType;
};

STATIC_ASSERT(sizeof(daNpcRafrel_c) == 0xe18);

#endif /* D_A_NPC_RAFREL_H */
