#ifndef D_A_MYNA_H
#define D_A_MYNA_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"

// This struct may end up belonging elsewhere
struct ShopItem {
    /* 0x0 */ u32 mTargetActorID;
    /* 0x4 */ u32 mSubActorID;
    /* 0x8 */ u16 mFlowNodeNum;
    /* 0xA */ u16 mValueNum;
    /* 0xC */ u8 mItemType;
    /* 0xD */ u8 mItemStatus;
};

STATIC_ASSERT(sizeof(ShopItem) == 0x10);

/**
 * @ingroup actors-npcs
 * @class daMyna_c
 * @brief Trill
 *
 * @details
 *
 */
class daMyna_c : public fopAc_ac_c {
public:
    int baseMotion00(int);
    int baseMotion01(int);
    int baseMotion02(int);
    int baseMotion03(int);
    int baseMotion04(int);
    int baseMotion05(int);
    int baseMotion06(int);
    int create();
    int destroy();
    int draw();
    int execute();
    int createHeap();
    int jntNodeCB(J3DJoint*, J3DModel*);
    void attack_wait_init();
    void attack_wait_move();
    void attack_before_talk_init();
    void attack_before_talk_move();
    void attack_fly_init();
    void attack_fly_move();
    void attack_init();
    void attack_move();
    void attack_after_talk_init();
    void attack_after_talk_move();
    void greet_wait_init();
    void greet_wait_move();
    void greet_talk_init();
    void greet_talk_move();
    void shopping_wait_init();
    void shopping_wait_move();
    void shopping_talk_init();
    void shopping_talk_move();
    void thanks_talk_init();
    void thanks_talk_move();
    void byebye_talk_init();
    void byebye_talk_move();
    void turn_on_start_init();
    void turn_on_start_move();
    void turn_on_init();
    void turn_on_move();
    void turn_on_end1_init();
    void turn_on_end1_move();
    void turn_on_end2_init();
    void turn_on_end2_move();
    void turn_on_end3_init();
    void turn_on_end3_move();
    void wolf_talk_init();
    void wolf_talk_move();
    void attack_wait2_init();
    void attack_wait2_move();
    void attack_before_talk2_init();
    void attack_before_talk2_move();
    void attack_fly2_init();
    void attack_fly2_move();
    void attack2_init();
    void attack2_move();
    void attack2_talk_init();
    void attack2_talk_move();
    void soldoutItem(fpc_ProcID);
    bool fly_attack_move();
    bool fly_return_move();
    void fly_body_wave();
    void talkAnime(msg_class*);
    int chkEvent();
    int orderEvent();
    void deleteItem(int);
    void deleteItem(fpc_ProcID);
    void initiate();
    void setItemInfo();
    void setRoomNo();
    void setCollision();
    void set_mtx();
    void animePlay();
    int setMcaMorfAnm(J3DAnmTransformKey*, f32, f32, int, int, int);
    void setBtpAnm(J3DAnmTexPattern*, J3DModelData*, f32, int);
    J3DAnmTransformKey* getTrnsfrmKeyAnm(char*);
    J3DAnmTexPattern* getTexPtrnAnm(char*);
    int checkEndAnm(J3DFrameCtrl*);
    int checkEndAnm(f32);
    u8 getItemNumMax();
    u8 getItemType(void*);
    u16 getFlowNodeNum();
    void checkDead();
    int chkPlayerInEvtArea(fopAc_ac_c*, cXyz);
    void animeControl();
    void setAnimeType(u8, f32);
    void playDefaultWaitAnime();
    void setDefaultWaitAnime(u8);
    virtual ~daMyna_c() {}

    daMyna_c() {}

    void onEventFlag(u8 flag) { field_0x92F |= (u8)(1 << flag); }
    void offEventFlag(u8 flag) { field_0x92F &= (u8)~(u8)(1 << flag); }
    bool isEventFlag(u8 flag) {
        return (field_0x92F & (u8)(1 << flag)) ? true : false;
    }

    fopAc_ac_c* getSpeakActorPtr() { return field_0x828; }

    typedef void (daMyna_c::*ProcFunc)();
    typedef int (daMyna_c::*BaseMotionFunc)(int);

    static dCcD_SrcSph const mCcDSph;
    static daMyna_c::BaseMotionFunc mBaseMotionTBL[7];

    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x578 */ mDoExt_btpAnm mBtpAnm;
    /* 0x590 */ Z2Creature mCreature;
    /* 0x620 */ dMsgFlow_c mMsgFlow;
    /* 0x66C */ cBgS_GndChk mGndChk;
    /* 0x6A8 */ dCcD_Stts mStatus;
    /* 0x6E4 */ dCcD_Sph mSph;
    /* 0x81C */ s32 field_0x81C;
    /* 0x820 */ s32 field_0x820;
    /* 0x824 */ u32 field_0x824;
    /* 0x828 */ fopAc_ac_c* field_0x828;
    /* 0x82C */ cXyz field_0x82C;
    /* 0x838 */ cXyz field_0x838;
    /* 0x844 */ cXyz field_0x844;
    /* 0x850 */ cXyz field_0x850;
    /* 0x85C */ cXyz field_0x85C;
    /* 0x868 */ ShopItem mShopItems[10];
    /* 0x908 */ s32 field_0x908;
    /* 0x90C */ f32 field_0x90C;
    /* 0x910 */ f32 field_0x910;
    /* 0x914 */ u16 field_0x914;
    /* 0x916 */ s16 field_0x916;
    /* 0x918 */ s16 field_0x918;
    /* 0x91A */ s16 field_0x91A;
    /* 0x91C */ u16 field_0x91C;
    /* 0x91E */ u16 field_0x91E;
    /* 0x920 */ s16 field_0x920;
    /* 0x922 */ s16 field_0x922;
    /* 0x924 */ s16 field_0x924;
    /* 0x926 */ s16 mDamageTimer;
    /* 0x928 */ s16 field_0x928;
    /* 0x92A */ s16 field_0x92A;
    /* 0x92C */ u8 field_0x92C;
    /* 0x92D */ u8 field_0x92D;
    /* 0x92E */ u8 field_0x92E;
    /* 0x92F */ u8 field_0x92F;
    /* 0x930 */ u8 mNumShopItems;
    /* 0x931 */ u8 field_0x931;
    /* 0x932 */ u8 field_0x932;
    /* 0x933 */ u8 field_0x933;
    /* 0x934 */ u8 field_0x934;
    /* 0x935 */ u8 field_0x935;
    /* 0x936 */ u8 field_0x936;
    /* 0x937 */ u8 field_0x937;
    /* 0x938 */ u8 field_0x938;
    /* 0x939 */ u8 field_0x939;
};

STATIC_ASSERT(sizeof(daMyna_c) == 0x93C);


#endif /* D_A_MYNA_H */
