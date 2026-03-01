#ifndef D_COM_D_COM_STATIC_H
#define D_COM_D_COM_STATIC_H

#include "d/actor/d_a_suspend.h"

namespace daCanBall_c {
    class Hio_c : public mDoHIO_entry_c {
    public:
        Hio_c();
        virtual void genMessage(JORMContext*);
        virtual ~Hio_c() {}

        /* 0x08 */ f32 field_0x8;
        /* 0x0C */ f32 field_0xc;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ f32 field_0x18;
        /* 0x1C */ f32 field_0x1c;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ f32 field_0x2c;
        /* 0x30 */ f32 field_0x30;
        /* 0x34 */ f32 field_0x34;
        /* 0x38 */ s16 field_0x38;
        /* 0x3C */ f32 field_0x3c;
    };

    extern Hio_c mHIO;
}

u8 daNpcKakashi_getSwdTutorialStep();
void daNpcKakashi_setSwdTutorialStep(u8 iEvtNum);
bool daNpcKakashi_getSwdTutorialResult();
bool daNpcKakashi_chkSwdTutorialStage();
u8 daNpcKakashi_getSuccessCount();
void daNpcKakashi_clrSuccessCount();
void daNpcKakashi_setSwdTutorialResult(bool param_0);
void daNpcKakashi_incSuccessCount();
int daNpcMsg_setEvtNum(u8 iEvtNum);

#endif /* D_COM_D_COM_STATIC_H */
