#ifndef C_C_DAMAGEREACTION_H
#define C_C_DAMAGEREACTION_H

#include "JSystem/JParticle/JPAParticle.h"

class JPTraceParticleCallBack4 : public JPAParticleCallBack {
public:
    void execute(JPABaseEmitter*, JPABaseParticle*);
    void draw(JPABaseEmitter*, JPABaseParticle*);
    ~JPTraceParticleCallBack4() {}
};

BOOL cDmrNowMidnaTalk();

DUSK_GAME_EXTERN u8 cDmr_SkipInfo;
DUSK_GAME_EXTERN u8 data_80450C99;
DUSK_GAME_EXTERN u8 data_80450C9A;
DUSK_GAME_EXTERN u8 data_80450C9B;
DUSK_GAME_EXTERN u8 data_80450C9C;
DUSK_GAME_EXTERN u8 data_80450C9D;
DUSK_GAME_EXTERN u8 data_80450C9E;
DUSK_GAME_EXTERN u8 cDmr_FishingWether;
DUSK_GAME_EXTERN u8 data_80450CA0;

extern "C" {
    DUSK_GAME_EXTERN JPTraceParticleCallBack4 JPTracePCB4;
}

void debug_actor_create();

#endif /* C_C_DAMAGEREACTION_H */
