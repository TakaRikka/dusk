#ifndef D_A_OBJ_HASU2_H
#define D_A_OBJ_HASU2_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

namespace nObjMHasu {
/**
 * @ingroup actors-objects
 * @class daObjMHasu_c
 * @brief Lily Pad?
 *
 * @details
 *
 */
class daObjMHasu_c : public dBgS_MoveBgActor, public request_of_phase_process_class {
public:
        daObjMHasu_c();
        int create1st();
        void setMtx();
        int CreateHeap();
        int Create();
        int Execute(Mtx**);
        int Draw();
        int Delete();
        void upDownHasu();
        void updateCount(u8);
        void setHasuCount(f32);
        virtual ~daObjMHasu_c() {};

        void setRideFlag(bool i_ride) { mRideFlag = i_ride; }
private:
    /* 0x5A8 */ Mtx mMtx;
    /* 0x5D8 */ Mtx mMtx2;
    /* 0x608 */ J3DModel* mModel;
    /* 0x60C */ bool mRideFlag : 1;
    /* 0x60C */ bool unk60c_1 : 1;
    /* 0x610 */ const f32 mConst1;
    /* 0x614 */ const f32 mConst2;
    /* 0x618 */ const f32 mConst3;
    /* 0x61C */ cXyz mPos;
    /* 0x628 */ f32 mStep;
    /* 0x62C */ f32 unk62C;
    /* 0x630 */ u8 mTimer;
};

STATIC_ASSERT(sizeof(daObjMHasu_c) == 0x634);
}  // namespace nObjMHasu


#endif /* D_A_OBJ_HASU2_H */
