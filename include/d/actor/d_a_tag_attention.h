#ifndef D_A_TAG_ATTENTION_H
#define D_A_TAG_ATTENTION_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

class daAttp_HIO_c : public mDoHIO_entry_c {
public:
    daAttp_HIO_c();

    void genMessage(JORMContext*);

    /* 0x06 */ u8 enable_type_switching;
    /* 0x07 */ u8 type_spec;
    /* 0x08 */ u8 enable_distance_display;
    /* 0x0C */ f32 dist_max;
    /* 0x10 */ f32 dist_max_release;
    /* 0x14 */ f32 dist_adjust;
    /* 0x18 */ f32 upper_y;
    /* 0x1C */ f32 lower_y;
};

class daAttp_c : public fopAc_ac_c {
public:
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();

#if DEBUG
    Mtx field_0x568;
    u8 field_0x598;
#endif
};  // Size: 0x568

namespace daAttp_prm {
static inline u8 getArg0(daAttp_c* pActor) {
    return fopAcM_GetParam(pActor) & 0x3;
}
static inline u8 getArg1(daAttp_c* pActor) {
    return (fopAcM_GetParam(pActor) >> 2) & 0x3;
}
static inline u8 getArg2(daAttp_c* pActor) {
    return (fopAcM_GetParam(pActor) >> 4) & 0x7;
}
static inline u8 getArg3(daAttp_c* pActor) {
    return (fopAcM_GetParam(pActor) >> 7) & 0x1;
}
static inline u8 getArg4(daAttp_c* pActor) {
    return (fopAcM_GetParam(pActor) >> 8) & 0x1;
}
static inline u8 getSw1(daAttp_c* pActor) {
    return (fopAcM_GetParam(pActor) >> 0x10) & 0xff;
}
static inline u8 getSw2(daAttp_c* pActor) {
    return (fopAcM_GetParam(pActor) >> 0x18);
}
}  // namespace daAttp_prm

#endif /* D_A_TAG_ATTENTION_H */
