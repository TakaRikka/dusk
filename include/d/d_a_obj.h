#ifndef D_A_D_A_OBJ_H
#define D_A_D_A_OBJ_H

#include "f_op/f_op_actor_mng.h"

class dCcD_GObjInf;
namespace daObj {
void eff_break_tsubo(fopAc_ac_c*, cXyz, int);
void make_eff_break_kotubo(fopAc_ac_c*);
void make_eff_break_kotubo2(fopAc_ac_c*);
void make_eff_break_gm_kotubo(fopAc_ac_c*);
void make_eff_break_gm_ootubo(fopAc_ac_c*);
void posMoveF_stream(fopAc_ac_c*, cXyz const*, cXyz const*, f32, f32);
void posMoveF_grade(fopAc_ac_c*, cXyz const*, cXyz const*, f32, f32, cXyz const*, f32, f32,
                    cXyz const*);
void quat_rotBaseY(Quaternion*, cXyz const&);
void HitSeStart(cXyz const*, int, dCcD_GObjInf const*, u32);

template <typename T>
T PrmAbstract(const fopAc_ac_c* i_actor, T i_nbits, T i_shift) {
    u32 param = fopAcM_GetParam(i_actor);
    return T((param >> i_shift) & ((1 << i_nbits) - 1));
}

class HioVarious_c : public JORReflexible {
public:
    HioVarious_c();
    virtual void genMessage(JORMContext*);

    static void init(JORReflexible* node_ptr, const char* node_name);
    static void clean(JORReflexible* node_ptr);

    static HioVarious_c M_hio;

    /* 0x04 */ JORReflexible* node_ptrs[16];
    /* 0x44 */ const char* node_names[16];
    /* 0x84 */ s8 field_0x84;
};

class HioThrow_c : public JORReflexible {
public:
    HioThrow_c();
#if DEBUG
    virtual void listen(u32 command, const JOREvent* event) { JORReflexible::listen(command, event); }
    virtual void genObjectInfo(const JORGenEvent* event) { JORReflexible::genObjectInfo(event); }
    virtual void listenNodeEvent(const JORNodeEvent* event) { JORReflexible::listenNodeEvent(event); }
    virtual void listenPropertyEvent(const JORPropertyEvent* event) { JORReflexible::listenPropertyEvent(event); }
#endif
    virtual void genMessage(JORMContext*);

    static void init(JORReflexible* node_ptr, const char* node_name);
    static void clean(JORReflexible* node_ptr);

    static HioThrow_c M_hio;
    static cXyz M_throw_pos_st;
    static cXyz M_throw_pos_now;

    /* 0x04 */ JORReflexible* node_ptrs[20];
    /* 0x54 */ const char* node_names[20];
    /* 0xA4 */ s8 field_0xa4;
};

class HioField_c : public JORReflexible {
public:
    HioField_c();
    virtual void genMessage(JORMContext*);

    static HioField_c M_hio;

    /* 0x04 */ JORReflexible* node_ptrs[16];
    /* 0x44 */ const char* node_names[16];
    /* 0x84 */ s8 field_0x84;
};

class HioSpot_c : public JORReflexible {
public:
    HioSpot_c();
    virtual void genMessage(JORMContext*);

    static HioSpot_c M_hio;

    /* 0x04 */ JORReflexible* node_ptrs[16];
    /* 0x44 */ const char* node_names[16];
    /* 0x84 */ s8 field_0x84;
};

class HioObj_c : public JORReflexible {
public:
    HioObj_c();
    virtual void genMessage(JORMContext*);

    static void init();
    static void clean();

    static HioObj_c M_hio;
    static const char* const M_node_name[];
    static JORReflexible* const M_node_ptr[];

    /* 0x04 */ s8 M_no;
};

};  // namespace daObj

#endif /* D_A_D_A_OBJ_H */
