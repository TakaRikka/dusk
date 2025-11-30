#ifndef D_A_TAG_WLJUMP_H
#define D_A_TAG_WLJUMP_H

#include "d/d_msg_flow.h"
#include "d/d_com_inf_game.h"

struct dPath;

class daTagWljump_c : public fopAc_ac_c {
public:
    int create();
    ~daTagWljump_c();
    int execute();
    int draw();

    const cXyz* getLockPos() const {
        if (field_0x568 < 0) {
            return NULL;
        } else {
            return &eyePos;
        }
    }

    f32 getLandArea() const { return mLandArea; }
    void onNextCheckFlg() { mNextCheckFlg = true; }
    s16 getNotSlideFlg() const { return shape_angle.z; }

    /* 0x568 */ s8 field_0x568;
    /* 0x569 */ s8 field_0x569;
    /* 0x56A */ u8 field_0x56a;
    /* 0x56B */ u8 mNextCheckFlg;
    /* 0x56C */ u8 field_0x56c;
    /* 0x56D */ u8 field_0x56d;
    /* 0x56E */ u8 field_0x56e;
    /* 0x56F */ u8 field_0x56f;
    /* 0x570 */ s8 field_0x570;
    /* 0x571 */ u8 field_0x571;
    /* 0x572 */ u8 field_0x572;
    /* 0x573 */ u8 field_0x573;
    /* 0x574 */ s16 field_0x574;
    /* 0x574 */ u16 field_0x576;
    /* 0x578 */ dMsgFlow_c mMsgFlow;
    /* 0x5C4 */ dPath* field_0x5c4;
    /* 0x5C8 */ f32 mLandArea;
};

#endif /* D_A_TAG_WLJUMP_H */
