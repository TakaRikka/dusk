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

extern u8 cDmr_SkipInfo;
extern u8 data_80450C99;
extern u8 data_80450C9A;
extern u8 data_80450C9B;
extern u8 data_80450C9C;
extern u8 data_80450C9D;
extern u8 data_80450C9E;
extern u8 cDmr_FishingWether;
extern u8 data_80450CA0;
extern JPTraceParticleCallBack4 JPTracePCB4;

void debug_actor_create();

#endif /* C_C_DAMAGEREACTION_H */
