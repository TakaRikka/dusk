#ifndef D_A_OBJ_LADDER_H
#define D_A_OBJ_LADDER_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_a_obj.h"

/**
 * @ingroup actors-objects
 * @class daObjLadder
 * @brief Ladder
 *
 * @details
 *
 */
namespace daObjLadder {
    class Act_c : public dBgS_MoveBgActor {
    public:
        enum Prm_e {
            PRM_0 = 0,
            PRM_3 = 3,
            PRM_8 = 8,
            PRM_16 = 16,
        };

        enum Type_e {
            TYPE_0,
            TYPE_1,
            TYPE_2,
            TYPE_3,
            TYPE_4,
            TYPE_5,
        };

        enum Mode {
            MODE_WAIT,
            MODE_DEMOREQ,
            MODE_VIB,
            MODE_DROP,
            MODE_FELL,
        };

        typedef void (Act_c::*modeProc)();

        int CreateHeap();
        int Create();
        int Mthd_Create();
        int Delete();
        int Mthd_Delete();
        void demo_end_reset();
        void mode_wait_init();
        void mode_wait();
        void mode_demoreq_init();
        void mode_demoreq();
        void mode_vib_init();
        void mode_vib();
        void mode_drop_init();
        void mode_drop();
        void mode_fell_init();
        void mode_fell();
        void set_mtx();
        void init_mtx();
        int Execute(Mtx**);
        int Draw();

        u8 prm_get_evId() {
            return daObj::PrmAbstract(this, PRM_8, PRM_16);
        }

        int prm_get_swSave() {
            return daObj::PrmAbstract(this,PRM_8, PRM_8);
        }

        Type_e prm_get_type() {
            return(Type_e)daObj::PrmAbstract(this,PRM_3, PRM_0);
        }

        static char const M_arcname[5];
        static Mtx M_tmp_mtx;
private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ Type_e mType;
    /* 0x5B0 */ Mode mMode;
    /* 0x5B4 */ s16 mVibrationTimer;
    /* 0x5B6 */ s16 field_0x5b6;
    /* 0x5B8 */ f32 mHeight;
    /* 0x5BC */ dBgS_ObjGndChk mGndChk;
    /* 0x610 */ s16 field_0x610;
    /* 0x612 */ s16 field_0x612;
    /* 0x614 */ f32 field_0x614;
    /* 0x618 */ f32 field_0x618;
    /* 0x61C */ s16 mEventIdx;
    /* 0x61E */ bool mInDemo;
    };
};

STATIC_ASSERT(sizeof(daObjLadder::Act_c) == 0x620);

#endif /* D_A_OBJ_LADDER_H */
