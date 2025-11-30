#ifndef D_D_EYE_HL_H
#define D_D_EYE_HL_H

#include "dolphin/types.h"

class J3DModelData;
struct ResTIMG;
class dEyeHL_c;

class dEyeHL_mng_c {
public:
    static void update();
    static void entry(dEyeHL_c*);
    static void remove(dEyeHL_c*);

    static dEyeHL_c* m_obj;
};

class dEyeHL_c {
public:
    dEyeHL_c() {
        m_timg = NULL;
        m_pre = NULL;
        m_next = NULL;
    }

    void entry(J3DModelData*, char const*);
    void remove();
    virtual ~dEyeHL_c() { remove(); }

    /* 0x04 */ ResTIMG* m_timg;
    /* 0x08 */ s16 m_lodBias;
    /* 0x0C */ dEyeHL_c* m_pre;
    /* 0x10 */ dEyeHL_c* m_next;
};

#endif /* D_D_EYE_HL_H */
