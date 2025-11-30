#ifndef D_A_YKGR_H
#define D_A_YKGR_H

#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_path.h"
#include "d/d_kankyo_rain.h"
#include "d/d_com_inf_game.h"
#include "d/d_particle.h"

struct dPa_YkgrPcallBack : public JPAParticleCallBack {
    dPa_YkgrPcallBack();
    ~dPa_YkgrPcallBack() {}
    void draw(JPABaseEmitter*, JPABaseParticle*);
    void setParam(f32);

    /* 0x04 */ f32 field_0x4[2][3];
    /* 0x1C */ s8 field_0x1c;
};

/**
 * @ingroup actors-unsorted
 * @class daYkgr_c
 * @brief Floor Gravity On/Off
 *
 * @details
 *
 */
class daYkgr_c : public fopAc_ac_c {
public:
    static void init() {
        m_emitter = 0;
        m_flag = false;
        m_alpha_flag = true;
        m_alpha = 255;
    }

    f32 getPosRate();
    static void setAlpha(u8 alpha) {
        m_alpha = alpha;
    }
    static void start() {
        if (m_emitter != NULL) {
            m_alpha_flag = true;
        }
    }
    static void stop() {
        if (m_emitter != NULL) {
            m_alpha_flag = false;
        }
    }

    inline void set_mtx();

    int _create();
    int _delete();
    int _execute();
    int _draw();

    static JPABaseEmitter* m_emitter;
    static bool m_flag;
    static bool m_alpha_flag;
    static u8 m_alpha;
    static f32 m_aim_rate;
    static dPath* m_path;

    /* 0x568 */ u8 field_0x568[0x570 - 0x568];
    /* 0x570 */ Mtx field_0x570;
    /* 0x5A0 */ u32 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
};

STATIC_ASSERT(sizeof(daYkgr_c) == 0x5AC);

#endif /* D_A_YKGR_H */
