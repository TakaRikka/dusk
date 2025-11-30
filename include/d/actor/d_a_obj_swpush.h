#ifndef D_A_OBJ_SWPUSH_H
#define D_A_OBJ_SWPUSH_H

#include "d/d_a_obj.h"
#include "d/d_bg_w_sv.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjSwpush
 * @brief Push Switch
 *
 * @details This is a multi-purpose depression switch actor.
 * Used by: Gold Pressure Switch, Lakebed Pressure Switch
 *
*/
namespace daObjSwpush {
    enum AttrFlag_e {
        FLAG_OBEY_SAVE = 0x1,
        FLAG_STAY_PRESSED = 0x2,
        FLAG_IS_TOGGLE = 0x4,
        FLAG_REQ_HEAVY = 0x8,
        FLAG_ON_IS_UP = 0x10,
        FLAG_UNK20 = 0x20,
    };
    
    class Hio_c : public JORReflexible {
    public:
        class Attr_c {
        public:
            /* § 踏みスイッチ §\n - Foot Switch */
            /* 0x00 */ AttrFlag_e flags;
            /* 0x04 */ f32 scale;
            /* 0x08 */ f32 draw_spring;             // draw_spring
            /* 0x0C */ f32 draw_resist;             // draw_resist
            /* 0x10 */ f32 draw_down_spd0;          // draw_down_spd0
            /* 0x14 */ f32 draw_pypcon_acc;         // draw_pypcon_acc
            /* 0x18 */ s16 bg_time_lag;             // bg_time_lag  
            /* 0x1A */ s16 able_push_time_any;      // able_push_time_any
            /* 0x1C */ s16 able_push_time_heavy;    // able_push_time_heavy
            /* 0x1E */ s16 able_ride_time_middle;   // able_ride_time_middle
            /* 0x20 */ f32 step_up_space;           // step_up_space
            /* 0x24 */ f32 step_down_space;         // step_down_space
            /* 0x28 */ s16 demo_run_time;           // demo_run_time
            /* 0x2A */ u8 field_0x2a;
            /* 0x2B */ u8 field_0x2b;
        };

        #if DEBUG
        void ct() {
            if (mCount++ == 0) {
                daObj::HioVarious_c::init(this, "踏みスイッチ"); // Foot Switch
            }
        }
        
        void default_set();
        void genMessage(JORMContext*);
        void dt();
        #endif

        /* 0x4 */ int mCount;
        /* 0x8 */ Attr_c field_0x8[5];
    };

    class Act_c : public fopAc_ac_c {
    public:
        enum Prm_e {
            /* 0x00 */ PRM_0_e,
            /* 0x01 */ PRM_1_e,
            /* 0x03 */ PRM_3_e = 0x3,
            /* 0x04 */ PRM_4_e,
            /* 0x08 */ PRM_8_e = 0x8,
            /* 0x10 */ PRM_10_e = 0X10,
            /* 0x18 */ PRM_18_e = 0x18,
            /* 0x1D */ PRM_1D_e = 0x1D,
        };

        enum Mode_e {
            MODE_UPPER = 0,
            MODE_U_L = 1,
            MODE_LOWER = 2,
            MODE_L_U = 3
        };

        enum DemoMode_e {
            DEMO_MODE_NON = 0,
            DEMO_MODE_REQ_PAUSE = 1,
            DEMO_MODE_RUN_PAUSE = 2,
            DEMO_MODE_REQ_SW = 3,
            DEMO_MODE_RUN_SW = 4
        };

        const Hio_c::Attr_c& attr() const { return M_attr[mType]; }
        u8 prmZ_get_swSave2() const { return mPrmZ & 0xFF; }
        int prm_get_couple() const { return daObj::PrmAbstract(this, PRM_1_e, PRM_1D_e); }
        u8 prm_get_evId() const { return daObj::PrmAbstract(this, PRM_8_e, PRM_0_e); }
        int prm_get_mdl() const { return daObj::PrmAbstract(this, PRM_4_e, PRM_10_e); }
        int prm_get_swSave() const { return daObj::PrmAbstract(this, PRM_8_e, PRM_8_e); }
        int prm_get_type() const { return daObj::PrmAbstract(this, PRM_3_e, PRM_18_e); }
        bool is_switch() { return fopAcM_isSwitch(this, prm_get_swSave()); }
        void fopAcM_revSwitch(fopAc_ac_c* i_actor, int param) { dComIfGs_revSwitch(param, fopAcM_GetHomeRoomNo(i_actor)); }
        void rev_switch() { fopAcM_revSwitch(this, prm_get_swSave()); }
        void off_switch() { fopAcM_offSwitch(this, prm_get_swSave()); }
        void on_switch() { fopAcM_onSwitch(this, prm_get_swSave()); }

        void prmZ_init();
        bool is_switch2() const;
        static BOOL solidHeapCB(fopAc_ac_c*);
        bool create_heap();
        cPhs__Step create_res_load();
        cPhs__Step Mthd_Create();
        int Mthd_Delete();
        void set_mtx();
        void init_mtx();
        void set_btp_frame();
        static void rideCB(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
        static int jnodeCB(J3DJoint*, int);
        void calc_top_pos();
        void top_bg_aim_req(f32, s16);
        void set_push_flag();
        void mode_upper_init();
        void mode_upper();
        void mode_u_l_init();
        void mode_u_l();
        void mode_lower_init();
        void mode_lower();
        void mode_l_u_init();
        void mode_l_u();
        void demo_non_init();
        void demo_non();
        void demo_reqPause_init();
        void demo_reqPause();
        void demo_runPause_init();
        void demo_runPause();
        void demo_stop_puase();
        void demo_reqSw_init();
        void demo_reqSw();
        void demo_runSw_init();
        void demo_runSw();
        bool check_ride_couple(s16);
        int nr_ride_people(char);
        int Mthd_Execute();
        int Mthd_Draw();

        static s16 const M_bmd[3];
        static s16 const M_dzb[3];
        static u32 const M_heap_size[3];
        static Hio_c::Attr_c const M_attr[5];
        static u8 const M_op_vtx[4];
        static char* M_arcname[3];

        /* 0x568 */ request_of_phase_process_class mPhase;
        /* 0x570 */ dBgWSv* mpBgW;
        /* 0x574 */ Mtx mMtx;
        /* 0x5A4 */ f32 field_0x5a4;
        /* 0x5A8 */ J3DModel* mpModel;
        /* 0x5AC */ int mType;
        /* 0x5B0 */ int mMode;
        /* 0x5B4 */ int mDemoMode;
        /* 0x5B8 */ s16 mPauseTimer;
        /* 0x5BA */ s16 mEventID;
        /* 0x5BC */ u16 mPrmZ;
        /* 0x5BE */ bool mPrmZInit;
        /* 0x5BF */ u8 mVibTimer;
        /* 0x5C0 */ u8 mRidingMode;
        /* 0x5C1 */ bool mPrevRiding;
        /* 0x5C2 */ s16 mMiniPushTimer;
        /* 0x5C4 */ bool mMiniPushFlg;
        /* 0x5C5 */ u8 field_0x5c5;
        /* 0x5C6 */ bool mHeavyRiding;
        /* 0x5C7 */ bool mPrevHeavyRiding;
        /* 0x5C8 */ s16 mPushTimer;
        /* 0x5CA */ bool mPushFlg;
        /* 0x5CB */ bool mChangingState;
        /* 0x5CC */ f32 mTargetHFrac;
        /* 0x5D0 */ f32 mCurHFrac;
        /* 0x5D4 */ f32 mSpeed;
        /* 0x5D8 */ f32 field_0x5d8;
        /* 0x5DC */ f32 field_0x5dc;
        /* 0x5E0 */ s16 field_0x5e0;
        /* 0x5E4 */ f32 field_0x5e4;
        /* 0x5E8 */ f32 field_0x5e8;
        /* 0x5EC */ f32 mTopPos;
        /* 0x5F0 */ s16 mDebounceTimer;
        /* 0x5F2 */ s8 field_0x5f2;
        /* 0x5F3 */ u8 field_0x5f3;
        /* 0x5F4 */ u8 mMdl;
        /* 0x5F8 */ cXyz field_0x5f8;
    };

    #if DEBUG
    void Hio_c::default_set() {
        for (int i = 0; i < 5; i++) {
            field_0x8[i] = Act_c::M_attr[i];
        }
    }

    void Hio_c::dt() {
        if (--mCount == 0) {
            daObj::HioVarious_c::clean(this);
        }
    }
    #endif
};

STATIC_ASSERT(sizeof(daObjSwpush::Act_c) == 0x604);

#endif /* D_A_OBJ_SWPUSH_H */
