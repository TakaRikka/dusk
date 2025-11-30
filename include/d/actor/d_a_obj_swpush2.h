#ifndef D_A_OBJ_SWPUSH2_H
#define D_A_OBJ_SWPUSH2_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_a_obj.h"

/**
 * @ingroup actors-objects
 * @class daObjSwpush2
 * @brief Push Switch 2
 *
 * @details
 *
 */
namespace daObjSwpush2 {
    enum AttrFlag_e {
        AttrFlag_UNK_0x1_e = 0x1,
        AttrFlag_UNK_0x2_e = 0x2,
        AttrFlag_REV_SW_e = 0x4,
        AttrFlag_HEAVY_e = 0x8,
        AttrFlag_UNK_0x10_e = 0x10,
        AttrFlag_UNK_0x20_e = 0x20,
    };

    class Attr_c {
    public:
        /* 0x00 */ u32 heap_size;
        /* 0x04 */ AttrFlag_e flags;
        /* 0x08 */ f32 size;
        /* 0x0C */ const char* arcname;
        /* 0x10 */ f32 unk_0x10;
        /* 0x14 */ f32 unk_0x14;
        /* 0x18 */ f32 unk_0x18;
        /* 0x1C */ f32 unk_0x1c;
        /* 0x20 */ s16 unk_0x20;
        /* 0x22 */ s16 ride_time;
        /* 0x24 */ s16 heavy_ride_time;
        /* 0x26 */ s16 field_0x26;
        /* 0x28 */ f32 unk_0x28;
        /* 0x2C */ f32 unk_0x2c;
        /* 0x30 */ s16 demo_run_time;
        /* 0x34 */ f32 effect_distance;
    };

    class Hio_c : public JORReflexible {
    public:
        void genMessage(JORMContext*);

        void ct();
        void dt();
        void default_set();

        /* 0x4 */ int field_0x4;
        /* 0x8 */ Attr_c M_attr[4];
    };

    class Act_c : public fopAc_ac_c {
    public:
        enum Prm_e {
            PRM_SAVESW_SHIFT_e = 0x8,
            PRM_SAVESW_NBITS_e = 0x8,

            PRM_EVID_SHIFT_e = 0x0,
            PRM_EVID_NBITS_e = 0x8,

            PRM_TYPE_SHIFT_e = 0x18,
            PRM_TYPE_NBITS_e = 0x3,
        };

        const char* getArcName();
        const char* getBmdName();
        const char* getDzbName();
        const char* getNmlBtkName();
        const char* getNmlBrkName();
        const char* getOnBtkName();
        const char* getRunBpkName();
        const char* getRunBrkName();
        const char* getRunBtkName();
        void prmZ_init();
        bool is_switch2() const;
        static int solidHeapCB(fopAc_ac_c*);
        bool create_heap();
        bool create_init();
        int create_res_load();
        int Mthd_Create();
        int Mthd_Delete();
        void set_mtx();
        void init_mtx();
        static void rideCB(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
        void calc_top_pos();
        void set_push_flag();
        void mode_upper_init();
        void mode_upper();
        void mode_u_l_init();
        void mode_u_l();
        void mode_lower_init();
        void mode_lower();
        void mode_l_u();
        void demo_non_init();
        void demo_non();
        void demo_reqPause_init();
        void demo_reqPause();
        void demo_runPause_init(int);
        void demo_runPause();
        void demo_stop_puase();
        void demo_reqSw_init();
        void demo_reqSw();
        void demo_runSw_init();
        void demo_runSw();
        int Mthd_Execute();
        int Mthd_Draw();

        const Attr_c& attr() const {
            #if DEBUG
            return M_hio.M_attr[mType];
            #else
            return M_attr[mType];
            #endif
        }

        u8 prmZ_get_swSave2() const { return mPrmZ & 0xFF; }
        int prm_get_swSave() const { return daObj::PrmAbstract(this, PRM_SAVESW_NBITS_e, PRM_SAVESW_SHIFT_e); }
        u8 prm_get_evId() const { return daObj::PrmAbstract(this, PRM_EVID_NBITS_e, PRM_EVID_SHIFT_e); }
        int prm_get_type() const { return daObj::PrmAbstract(this, PRM_TYPE_NBITS_e, PRM_TYPE_SHIFT_e); }

        bool is_switch() const { return fopAcM_isSwitch(this, prm_get_swSave()); }
        void on_switch() const { fopAcM_onSwitch(this, prm_get_swSave()); }
        void off_switch() const { fopAcM_offSwitch(this, prm_get_swSave()); }
        void rev_switch() const { fopAcM_revSwitch(this, prm_get_swSave()); }

        static const char M_arcname[];
        static const Attr_c M_attr[4];

        #if DEBUG
        static Hio_c M_hio;
        #endif

        /* 0x568 */ request_of_phase_process_class mPhase;
        /* 0x570 */ dBgW* mpBgw;
        /* 0x574 */ Mtx mBgMtx;
        /* 0x5A4 */ J3DModel* mpModel;
        /* 0x5A8 */ mDoExt_bpkAnm* mpRunBpk;
        /* 0x5AC */ mDoExt_btkAnm* mpRunBtk;
        /* 0x5B0 */ mDoExt_brkAnm* mpRunBrk;
        /* 0x5B4 */ mDoExt_btkAnm* mpOnBtk;
        /* 0x5B8 */ mDoExt_brkAnm* mpNmlBrk;
        /* 0x5BC */ mDoExt_btkAnm* mpNmlBtk;
        /* 0x5C0 */ int mType;
        /* 0x5C4 */ int mModeProc;
        /* 0x5C8 */ int mDemoProc;
        /* 0x5CC */ s16 mDemoRunTime;
        /* 0x5CE */ s16 mEventIdx;
        /* 0x5D0 */ u16 mPrmZ;
        /* 0x5D2 */ u8 mPrmInit;
        /* 0x5D3 */ u8 mShockTimer;
        /* 0x5D4 */ u8 mRideType;
        /* 0x5D5 */ u8 mPrevRideType;
        /* 0x5D6 */ s16 mRideTimer;
        /* 0x5D8 */ u8 mPushFlag;
        /* 0x5D9 */ u8 mIsPlayerRideHeavy;
        /* 0x5DA */ u8 mPrevPlayerRideHeavy;
        /* 0x5DC */ s16 mHeavyRideTimer;
        /* 0x5DE */ u8 mHeavyPushFlag;
        /* 0x5DF */ u8 field_0x5df;
        /* 0x5E0 */ s16 mPauseDemoTimer;
        /* 0x5E4 */ f32 mTopPos;
        /* 0x5E8 */ f32 field_0x5e8;
        /* 0x5EC */ f32 field_0x5ec;
        /* 0x5F0 */ u8 field_0x5f0;
    };
};

STATIC_ASSERT(sizeof(daObjSwpush2::Act_c) == 0x5f4);

#endif /* D_A_OBJ_SWPUSH2_H */
