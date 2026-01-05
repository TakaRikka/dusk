#ifndef D_A_TAG_STREAM_H
#define D_A_TAG_STREAM_H

#include "d/d_com_inf_game.h"

class daTagStream_c : public fopAc_ac_c {
public:
    u8 getPriority() {
        return mPriority;
    }

    static daTagStream_c* getTop() {
        return m_top;
    }

    daTagStream_c* getNext() {
        return mNext;
    }

    u8 checkStreamOn() const {
        return mStreamOn;
    }

    s32 checkCanoeOn() const {
        return mParameters;
    }

    u8 getPower() const {
        return mPower;
    }

    int create();
    ~daTagStream_c();
    int execute();
    int checkArea(cXyz const*);

    /* 0x568 */ u8 mPriority;
    /* 0x569 */ u8 mStreamOn;
    /* 0x56A */ u8 mPower;
    /* 0x56B */ u8 field_0x56b;
    /* 0x56C */ s32 mParameters;
    /* 0x570 */ daTagStream_c* field_0x570;
    /* 0x574 */ daTagStream_c* mNext;

    static daTagStream_c* m_top;
};

#endif /* D_A_TAG_STREAM_H */
