#ifndef D_COM_D_COM_INF_ACTOR_H
#define D_COM_D_COM_INF_ACTOR_H

class fopAc_ac_c;

class dComIfGoat_info_c {
public:
    /* 0x0 */ fopAc_ac_c* mThrowActor;
};

class dComIfAc_info_c {
public:
    /* 0x0 */ int field_0x0;
    /* 0x4 */ bool mPause;
};

DUSK_GAME_EXTERN dComIfGoat_info_c g_dComIfGoat_gameInfo;
DUSK_GAME_EXTERN dComIfAc_info_c g_dComIfAc_gameInfo;

inline bool dComIfA_PauseCheck() {
    return g_dComIfAc_gameInfo.mPause;
}

inline fopAc_ac_c* dComIfGoat_GetThrow() {
    return g_dComIfGoat_gameInfo.mThrowActor;
}

inline void dComIfGoat_SetThrow(fopAc_ac_c* actor) {
    g_dComIfGoat_gameInfo.mThrowActor = actor;
}

#endif /* D_COM_D_COM_INF_ACTOR_H */
