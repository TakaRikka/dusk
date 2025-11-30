#ifndef D_A_TAG_THEB_HINT_H
#define D_A_TAG_THEB_HINT_H

// #include "d/actor/d_a_npc_theB.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

// needs to be pulled from d_a_npc_theB.h eventually
struct daNpcTheB_c {
    daNpcTheB_c();
    ~daNpcTheB_c();
    void create();
    void CreateHeap();
    void Delete();
    void Execute();
    void Draw();
    void ctrlJoint(J3DJoint*, J3DModel*);
    void createHeapCallBack(fopAc_ac_c*);
    void ctrlJointCallBack(J3DJoint*, int);
    void getHandPos1(int);
    void getHandPos2(int);
    void setHintEvent(s32, u16, int);
    void main();
    void setParam();
    void ctrlBtk();
    void setAttnPos();
    void setExpressionAnm(int, bool);
    void setExpressionBtp(int);
    bool setMotionAnm(int, f32);
    bool drawDbgInfo();
    void reset();
    void playExpression();
    void playMotion();
    void doNormalAction();
    void doEvent();
    void lookat();
    void wait(void*);
    void setMotion(int, f32, int);
    void setExpression(int, f32);
    void talk(void*);
    void EvCut_PersonalCombatIntro(int);
    void EvCut_PersonalCombatRevenge(int);
    void EvCut_PersonalCombatAfter();
    void EvCut_AnnulationFieldRace(int);
    void EvCut_TheBHint(int);
    void EvCut_CoachGuardGameOver(int);

    static u8 mEvtSeqList[72];
};

class daTagTheBHint_c : public fopAc_ac_c {
public:

    u8 getSwitchNo() {
        return shape_angle.x & 0xff;
    }

    u16 getMessageNo() {
        return shape_angle.z & 0xffff;
    }

    u16 getLinkID() {
        return fopAcM_GetParam(this) & 0xFFFF;
    }

    s8 getRoomNo() {
        return fopAcM_GetParam(this) >> 24;
    }    

    int create() {
        fopAcM_ct(this, daTagTheBHint_c);
        field_0x568 = pow(scale.x * 100.0f,2.0f);
        return cPhs_COMPLEATE_e;
    }

    int execute() {
        if (fopAcM_isSwitch(this,getSwitchNo())) {
            fopAcM_delete(this);
            return 1;
        } else {
            f32 distance = fopAcM_searchPlayerDistanceXZ2(this);

            if (distance < field_0x568) {
                daNpcTheB_c* the_b = (daNpcTheB_c*)fpcM_SearchByName(PROC_NPC_THEB);
                if (the_b) {
                    s8 roomNo = getRoomNo();
                    
                    if (roomNo == 0xFF) { 
                        roomNo = fopAcM_GetRoomNo(this);
                    }

                    the_b->setHintEvent(getMessageNo(),getLinkID(),roomNo);
                }
            }
        }

        return 1;
    }
    /* 0x568 */ f32 field_0x568;
};

#endif /* D_A_TAG_THEB_HINT_H */
