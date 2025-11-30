#ifndef D_A_OBJ_TIMER_H
#define D_A_OBJ_TIMER_H

#include "d/d_a_obj.h"

/**
 * @ingroup actors-objects
 * @class daObjTimer
 * @brief Timer
 *
 * @details
 *
 */
namespace daObjTimer {
class Act_c : public fopAc_ac_c {
    public:
        typedef void (daObjTimer::Act_c::*modeProc)();

        enum Prm_e {
            PRM_0 = 0,
            PRM_1 = 1,
            PRM_2 = 2,
            PRM_8 = 8,
            PRM_16 = 16,
            PRM_24 = 24,
            PRM_26 = 26,
        };

        int _create();
        bool _delete();
        void mode_wait_init();
        void mode_wait();
        void mode_count_init();
        void mode_count();
        bool _execute();

        int prm_get_swSave() {
            return daObj::PrmAbstract(this,PRM_8, PRM_16);
        }

        int prm_get_sw2Save() {
            return daObj::PrmAbstract(this, PRM_8, PRM_8);
        }

        int prm_get_time() {
            return daObj::PrmAbstract(this, PRM_8, PRM_0);
        }

        int prm_get_SeStop() {
            return daObj::PrmAbstract(this, PRM_1, PRM_26);
        }

        int prm_get_demoStop() {
            return daObj::PrmAbstract(this, PRM_2, PRM_24);
        }
        
    private:
        /* 0x568 */ int field_0x568;
        /* 0x56C */ int field_0x56c;
        /* 0x570 */ u8 field_0x570;
        /* 0x571 */ u8 field_0x571[0x574 - 0x571];
    };
}

STATIC_ASSERT(sizeof(daObjTimer::Act_c) == 0x574);

#endif /* D_A_OBJ_TIMER_H */
