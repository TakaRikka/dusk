#ifndef D_A_TAG_ALLMATO_H
#define D_A_TAG_ALLMATO_H

#include "d/actor/d_a_npc.h"

class daTag_AllMato_c : public fopAc_ac_c {
public:
    /* 0x0568 */ daNpcT_ActorMngr_c mBouMatoActorMngr[1];
    /* 0x0570 */ daNpcT_ActorMngr_c mItaMatoActorMngr[2];
    /* 0x0580 */ daNpcT_ActorMngr_c mArrowActorMngr;
    /* 0x0588 */ daNpcT_ActorMngr_c mBrkMatoActorMngr;
    /* 0x0590 */ cXyz field_0x590[500];
    /* 0x1D00 */ int mBouMatoActorNum;
    /* 0x1D04 */ int mItaMatoActorNum;
    /* 0x1D08 */ int field_0x1d08;
    /* 0x1D0C */ s16 mEventIdx;
    /* 0x1D0E */ u8 field_0x1d0e;
    /* 0x1D0F */ u8 field_0x1d0f;
    /* 0x1D10 */ u8 field_0x1d10;

    int create();
    int Delete();
    int Execute();
    int Draw();
    static void* srchBouMato(void*, void*);
    static void* srchItaMato(void*, void*);
    static void* srchTaro(void*, void*);
    static void* srchArrow(void*, void*);
    int isDelete();
    void entryBouMatoActors();
    void entryItaMatoActors();
    fopAc_ac_c* getTaroActorP();
    fopAc_ac_c* getArrowActorP();
    int checkCrsMato();
    int checkCrsMato2();
    int checkBrkMato();
    void evtChange(u16);

    virtual ~daTag_AllMato_c() {}

    int getType() { return 0; }

    int getOnEvtBit() {
        u32 prm = fopAcM_GetParam(this) & 0xFFF;

        int bit;
        if (prm == 0xFFF) {
            bit = -1;
        } else {
            bit = prm;
        }

        return bit;
    }

    u32 getOffEvtBit() {
        u32 prm = (fopAcM_GetParam(this) & 0xFFF000) >> 12;

        u32 bit;
        if (prm == 0xFFF) {
            bit = 0xFFFFFFFF;
        } else {
            bit = prm;
        }

        return bit;
    }

    u8 getOnSwBit() {
        return home.angle.x & 0xFF;
    }

    u8 getOffSwBit() {
        return (home.angle.x & 0xFF00) >> 8;
    }
};

#endif /* D_A_TAG_ALLMATO_H */
