#ifndef D_A_NPC_KS_H
#define D_A_NPC_KS_H

#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_obj_so.h"
#include "d/actor/d_a_obj_sw.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-npcs
 * @class npc_ks_class
 * @brief Monkey NPC (Kozaru)
 *
 * @details
 *
 */
class npc_ks_class {
public:
    enum Npc_ks_RES_File_ID {
        /* BCK */
        /* 0x05 */ BCK_SARU_BAKUCHU = 0x5,
        /* 0x06 */ BCK_SARU_BIKKURI,
        /* 0x07 */ BCK_SARU_CALL,
        /* 0x08 */ BCK_SARU_CALLBACK,
        /* 0x09 */ BCK_SARU_CALLBACK_K,
        /* 0x0A */ BCK_SARU_CALL_K,
        /* 0x0B */ BCK_SARU_CLAP_WAIT,
        /* 0x0C */ BCK_SARU_CLIMB,
        /* 0x0D */ BCK_SARU_FALL,
        /* 0x0E */ BCK_SARU_FIRE_K,
        /* 0x0F */ BCK_SARU_GODOWN,
        /* 0x10 */ BCK_SARU_HANGCALL,
        /* 0x11 */ BCK_SARU_HANGWAIT_A,
        /* 0x12 */ BCK_SARU_HANGWAIT_B,
        /* 0x13 */ BCK_SARU_HANGWAIT_C,
        /* 0x14 */ BCK_SARU_HELP,
        /* 0x15 */ BCK_SARU_HELP_B,
        /* 0x16 */ BCK_SARU_HELP_C,
        /* 0x17 */ BCK_SARU_HOLDING,
        /* 0x18 */ BCK_SARU_HUNGING,
        /* 0x19 */ BCK_SARU_INDICATE,
        /* 0x1A */ BCK_SARU_JUMPRUN,
        /* 0x1B */ BCK_SARU_JUMPRUN_K,
        /* 0x1C */ BCK_SARU_JUMPWALK,
        /* 0x1D */ BCK_SARU_JUMPWALK_K,
        /* 0x1E */ BCK_SARU_JUMP_ATTN,
        /* 0x1F */ BCK_SARU_JUMP_L,
        /* 0x20 */ BCK_SARU_JUMP_S,
        /* 0x21 */ BCK_SARU_JUMP_T,
        /* 0x22 */ BCK_SARU_KAGO_JUMP,
        /* 0x23 */ BCK_SARU_LAND,
        /* 0x24 */ BCK_SARU_RECOVER,
        /* 0x25 */ BCK_SARU_RELIEF,
        /* 0x26 */ BCK_SARU_ROTATE,
        /* 0x27 */ BCK_SARU_SCARED,
        /* 0x28 */ BCK_SARU_SCAREDLOOK_K,
        /* 0x29 */ BCK_SARU_SCARED_B,
        /* 0x2A */ BCK_SARU_SCARED_K,
        /* 0x2B */ BCK_SARU_SCARED_LOOK,
        /* 0x2C */ BCK_SARU_SHAKE_K,
        /* 0x2D */ BCK_SARU_STOPCALL,
        /* 0x2E */ BCK_SARU_SURPRISE_K,
        /* 0x2F */ BCK_SARU_SWIM,
        /* 0x30 */ BCK_SARU_SWINGPOSE,
        /* 0x31 */ BCK_SARU_TO_ROTATE,
        /* 0x32 */ BCK_SARU_WAITSTAND,
        /* 0x33 */ BCK_SARU_WAIT_A,
        /* 0x34 */ BCK_SARU_WAIT_A_K,
        /* 0x35 */ BCK_SARU_WAIT_B,
        /* 0x36 */ BCK_SARU_WAIT_C,
        /* 0x37 */ BCK_SARU_WALKCLAP,
        /* 0x38 */ BCK_SARU_YOROKOBI,

        /* BMDR */
        /* 0x3B */ BMDR_SARU = 0x3B,
        /* 0x3C */ BMDR_SARU_BARA,
        /* 0x3D */ BMDR_SARU_STICK,

        /* BTP */
        /* 0x40 */ BTP_SARU = 0x40,
        /* 0x41 */ BTP_SARU_RELIEF,
    };

    /* 0x000 */ fopEn_enemy_c actor;
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 monkey_room_no;
    /* 0x5B5 */ u8 field_0x5b5;
    /* 0x5B6 */ u8 set_id;
    /* 0x5B7 */ u8 field_0x5b7;
    /* 0x5B8 */ int order;
    /* 0x5BC */ int bitTRB;
    /* 0x5C0 */ const char* res_name;
    /* 0x5C4 */ f32 dis;
    /* 0x5C8 */ s16 target_angle;
    /* 0x5CC */ mDoExt_McaMorfSO* model;
    /* 0x5D0 */ int res_id;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ mDoExt_btpAnm* btp;
    /* 0x5DC */ mDoExt_btpAnm* btp2;
    /* 0x5E0 */ s16 field_0x5e0;
    /* 0x5E2 */ s16 field_0x5e2;
    /* 0x5E4 */ s8 field_0x5e4;
    /* 0x5E5 */ u8 field_0x5e5;
    /* 0x5E6 */ s16 count;
    /* 0x5E8 */ s16 action;
    /* 0x5EA */ s16 mode;
    /* 0x5EC */ u32 shadow_key;
    /* 0x5F0 */ s16 timer[4];
    /* 0x5F8 */ s16 field_0x5f8;
    /* 0x5FA */ s16 field_0x5fa;
    /* 0x5FC */ s8 field_0x5fc;
    /* 0x5FD */ s8 field_0x5fd;
    /* 0x5FE */ s16 field_0x5fe;
    /* 0x600 */ s16 field_0x600;
    /* 0x602 */ s16 field_0x602;
    /* 0x604 */ s16 field_0x604;
    /* 0x606 */ s16 field_0x606;
    /* 0x608 */ s16 field_0x608;
    /* 0x60C */ f32 field_0x60c;
    /* 0x610 */ s16 field_0x610;
    /* 0x614 */ cXyz field_0x614;
    /* 0x620 */ s8 field_0x620;
    /* 0x621 */ s8 field_0x621;
    /* 0x622 */ u8 field_0x622;
    /* 0x623 */ u8 field_0x623;
    /* 0x624 */ cXyz field_0x624;
    /* 0x630 */ int field_0x630;
    /* 0x634 */ f32 field_0x634;
    /* 0x638 */ f32 field_0x638;
    /* 0x63C */ f32 waterY;
    /* 0x640 */ dBgS_AcchCir AcchCir;
    /* 0x680 */ dBgS_ObjAcch ObjAcch;
    /* 0x858 */ u32 field_0x858;
    /* 0x85C */ u32 field_0x85c;
    /* 0x860 */ Z2Creature sound;
    /* 0x8F0 */ cXyz guide_path;
    /* 0x8FC */ csXyz current_angle;
    /* 0x904 */ int path_no;
    /* 0x908 */ f32 field_0x908;
    /* 0x90C */ s8 field_0x90c;
    /* 0x910 */ cXyz field_0x910;
    /* 0x91C */ cXyz field_0x91c;
    /* 0x928 */ f32 field_0x928;
    /* 0x92C */ s8 field_0x92c;
    /* 0x930 */ void* field_0x930;
    /* 0x934 */ obj_sw_class* child_no;
    /* 0x938 */ fpc_ProcID search_id;
    /* 0x93C */ int search_time;
    /* 0x940 */ cXyz find_pos;
    /* 0x94C */ s16 anm_time;
    /* 0x94E */ s16 hang_time;
    /* 0x950 */ dCcD_Stts stts;
    /* 0x98C */ dCcD_Cyl cc_cyl;
    /* 0xAC8 */ dCcU_AtInfo mAtInfo; // unused
    /* 0xAEC */ s8 field_0xaec;
    /* 0xAED */ s8 field_0xaed;
    /* 0xAEE */ s16 field_0xaee;
    /* 0xAF0 */ s16 field_0xaf0;
    /* 0xAF4 */ dMsgFlow_c msg_flow;
    /* 0xB40 */ u8 field_0xb40;
    /* 0xB41 */ u8 field_0xb41;
    /* 0xB42 */ s16 demo_mode;
    /* 0xB44 */ s16 demo_camera_no;
    /* 0xB48 */ cXyz demo_way;
    /* 0xB54 */ cXyz field_0xb54;
    /* 0xB60 */ cXyz demo_eye;
    /* 0xB6C */ cXyz field_0xb6c;
    /* 0xB78 */ cXyz field_0xb78;
    /* 0xB84 */ cXyz field_0xb84;
    /* 0xB90 */ cXyz field_0xb90; // unused
    /* 0xB90 */ cXyz field_0xb9c; // unused
    /* 0xBA8 */ cXyz field_0xba8;
    /* 0xBB4 */ f32 field_0xbb4;
    /* 0xBB8 */ f32 target_speed;
    /* 0xBBC */ u8 field_0xbbc[0xbc0 - 0xbbc];
    /* 0xBC0 */ npc_ks_class* field_0xbc0;
    /* 0xBC4 */ f32 field_0xbc4;
    /* 0xBC8 */ cXyz field_0xbc8;
    /* 0xBD4 */ s16 field_0xbd4;
    /* 0xBD6 */ u8 field_0xbd6;
    /* 0xBD7 */ s8 no_draw;
    /* 0xBD8 */ s8 field_0xbd8;
    /* 0xBD9 */ s8 field_0xbd9;
    /* 0xBDA */ u8 field_0xbda;
    /* 0xBDB */ s8 field_0xbdb;
    /* 0xBDC */ s8 field_0xbdc;
    /* 0xBDD */ s8 field_0xbdd;
    /* 0xBDE */ s16 field_0xbde;
    /* 0xBE0 */ s8 field_0xbe0;
    /* 0xBE4 */ u32 w_eff[4];
    /* 0xBF4 */ u32 hammonPrtcl;
    /* 0xBF8 */ u8 field_0xbf8[0xbfc - 0xbf8];
    /* 0xBFC */ J3DModel* bara_model;
    /* 0xC00 */ J3DModel* stick_model;
    /* 0xC04 */ cXyz field_0xc04;
    /* 0xC10 */ f32 field_0xc10;
    /* 0xC14 */ s16 field_0xc14;
    /* 0xC16 */ s8 field_0xc16;
    /* 0xC17 */ s8 field_0xc17;
    /* 0xC18 */ dPath* field_0xc18;
    /* 0xC1C */ u8 hio_init;
};

STATIC_ASSERT(sizeof(npc_ks_class) == 0xc20);

#endif /* D_A_NPC_KS_H */
