#include <revolution/kpad.h>
#include <revolution/kpad/__kpad.h>

#include <revolution/mtx.h>
#include "types.h"
#include <cmath>
#include <cstdio>
#include <cstring>

static const char* __KPADVersion = "<< RVL_SDK - KPAD \trelease build: Oct  4 2006 11:56:50 (0x4199_60726) >>";

static Vec2 icenter_org = {0.000f, 0.000f};
static f32 idist_org = 1.000f;
static Vec2 iaccXY_nrm_hori = {0.000f, -1.000f};
static Vec2 isec_nrm_hori = {1.000f, 0.000f};
f32 kp_obj_interval = 0.200f;

f32 kp_acc_horizon_pw = 0.050f;
f32 kp_ah_circle_radius = 0.070f;
f32 kp_ah_circle_pw = 0.060f;
u16 kp_ah_circle_ct = 100;

f32 kp_err_outside_frame = 0.050f;
f32 kp_err_dist_min;
f32 kp_err_dist_max = 3.000f;
f32 kp_err_dist_speed = 0.040f;
f32 kp_err_first_inpr = 0.900f;
f32 kp_err_next_inpr = 0.900f;
f32 kp_err_acc_inpr = 0.900f;
f32 kp_err_up_inpr = 0.700f;
f32 kp_err_near_pos = 0.100f;

static Vec2 kobj_frame_min, kobj_frame_max;
static f32 kp_err_dist_speed_1;
static f32 kp_err_dist_speedM_1;
static f32 kp_ah_circle_radius2;
static f32 kp_dist_vv1;

static s32 kp_fs_fstick_min = 15;
static s32 kp_fs_fstick_max = 71;
static s32 kp_cl_stick_min = 60;
static s32 kp_cl_stick_max = 308;
static s32 kp_cl_trigger_min = 30;
static s32 kp_cl_trigger_max = 180;
static f32 kp_rm_acc_max = 3.4f;
static f32 kp_fs_acc_max = 2.1f;

KPADInsideStatus inside_kpads[WPAD_MAX_CONTROLLERS];
static Vec2 Vec2_0 = {0.0f, 0.0f};

void DEBUGPrint(const char* fmt, ...) {
    va_list va;
}

void KPADSetFSStickClamp(s8 min, s8 max) {
    kp_fs_fstick_min = min;
    kp_fs_fstick_max = max;
}

// unused, but float order implies it probably exists in this version of the SDK
void KPADSetBtnRepeat(s32 chan, f32 delay_sec, f32 pulse_sec) {
    KPADInsideStatus* kp = &inside_kpads[chan];
    if (pulse_sec) {
        kp->btn_repeat_delay = (u16)(s32)(delay_sec + 0.5f);
        kp->btn_repeat_pulse = (u16)(s32)(pulse_sec + 0.5f);
    } else {
        kp->btn_repeat_delay = KPAD_BTN_NO_RPT_DELAY;
        kp->btn_repeat_pulse = 0;
    }

    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;
    kp->btn_cl_repeat_time = 0;
    kp->btn_cl_repeat_next = kp->btn_repeat_delay;
}

void KPADSetObjInterval(f32 interval) {
#if SDK_SEP2006
    kp_obj_interval = interval;
    // 0.383864f = tan(21 degs), not sure of the significance of this
    kp_err_dist_min = interval / 0.383864f;
    kp_dist_vv1 = interval / 0.383864f;
#endif
}

void KPADSetPosParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].pos_play_radius = play_radius;
    inside_kpads[chan].pos_sensitivity = sensitivity;
}

void KPADSetHoriParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].hori_play_radius = play_radius;
    inside_kpads[chan].hori_sensitivity = sensitivity;
}

void KPADSetDistParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].dist_play_radius = play_radius;
    inside_kpads[chan].dist_sensitivity = sensitivity;
}

void KPADSetAccParam(s32 chan, f32 play_radius, f32 sensitivity) {
    inside_kpads[chan].acc_play_radius = play_radius;
    inside_kpads[chan].acc_sensitivity = sensitivity;
}

static void calc_dpd2pos_scale(KPADInsideStatus* kp) {
    f32 sx, sy;
    sx = 1.0f;
    sy = (f32)KPAD_DPD_RESO_WY / (f32)KPAD_DPD_RESO_WX;

    kp->dpd2pos_scale = sqrt(sx * sx + sy * sy);

    if (kp->center_org.x < 0.0f) {
        sx += kp->center_org.x;
    } else {
        sx -= kp->center_org.x;
    }

    if (kp->center_org.y < 0.0f) {
        sy += kp->center_org.y;
    } else {
        sy -= kp->center_org.y;
    }

    if (sx < sy) {
        kp->dpd2pos_scale /= sx;
    } else {
        kp->dpd2pos_scale /= sy;
    }
}

void reset_kpad(KPADInsideStatus* kp) {
    KPADObject* op;
    KPADStatus* sp = &kp->status;
    KPADEXStatus* ep = &kp->status.ex_status;

    sp->hold = sp->trig = sp->release = 0;
    kp->btn_repeat_time = 0;
    kp->btn_repeat_next = kp->btn_repeat_delay;

    kp->status.dpd_valid_fg = 0;
    kp->dpd_valid2_ct = 0;

    sp->pos = sp->vec = Vec2_0;
    sp->speed = 0.0f;

    sp->horizon.x = kp->acc_horizon.x = kp->obj_horizon.x = 1.0f;
    sp->horizon.y = kp->acc_horizon.y = kp->obj_horizon.y = 0.0f;
    sp->hori_vec = Vec2_0;
    sp->hori_speed = 0.0f;

    sp->acc_vertical.x = 1.0f;
    sp->acc_vertical.y = 0.0f;

    sp->dist = kp->dist_org;
    sp->dist_vec = sp->dist_speed = 0.0f;

    kp->sec_dist = sp->dist;
    kp->sec_length = kp->trust_sec_length = kp_dist_vv1 / kp->sec_dist;
    kp->sec_nrm = kp->sec_nrm_hori;

    sp->acc.x = sp->acc.z = 0.0f;
    sp->acc.y = -1.0f;
    sp->acc_value = 1.0f;
    sp->acc_speed = 0.0f;
    kp->hard_acc = sp->acc;

    kp->ah_circle_pos = kp->acc_horizon;
    kp->ah_circle_ct = kp_ah_circle_ct;

    kp->valid_objs = 0;

    op = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
    do {
        op->error_fg = -1;
    } while (--op >= kp->kobj_sample);

    op = &kp->kobj_regular[KPAD_USE_OBJECTS - 1];
    do {
        op->error_fg = -1;
    } while (--op >= kp->kobj_regular);

    kp->unk_0x116 = -1;
    kp->repeat_count = 0;

    if (kp->active_chan < 0) {
        kp->status.wpad_err = WPADProbe(kp->active_chan + 4, NULL);
        WPADControlMotor(kp->active_chan + 4, 0);
        if (kp->status.wpad_err != -1) {
            kp->active_chan += 4;
        }
    } else {
        kp->status.wpad_err = WPADProbe(kp->active_chan, NULL);
        WPADControlMotor(kp->active_chan, 0);
    }

    switch (kp->status.dev_type) {
    case WPAD_DEV_FS:
        ep->fs.stick = Vec2_0;
        ep->fs.acc.z = 0.0f;
        ep->fs.acc.x = 0.0f;
        ep->fs.acc.y = -1.0f;
        ep->fs.acc_value = 1.0f;
        ep->fs.acc_speed = 0.0f;
        break;
    case WPAD_DEV_CLASSIC:
        ep->cl.lstick = Vec2_0;
        ep->cl.rstick = Vec2_0;
        ep->cl.rtrigger = 0.0f;
        ep->cl.ltrigger = 0.0f;
        ep->cl.release = 0;
        ep->cl.trig = 0;
        ep->cl.hold = 0;
        kp->btn_cl_repeat_time = 0;
        kp->btn_cl_repeat_next = kp->btn_cl_repeat_delay;
        break;
    }
}

void KPADSetSensorHeight(s32 chan, f32 level) {
    KPADInsideStatus* kp = &inside_kpads[chan];

    kp->center_org.x = 0.0f;
    kp->center_org.y = -level;
    calc_dpd2pos_scale(kp);
}

void calc_button_repeat(KPADInsideStatus* kp, KPADStatus* status, u32 dev_type) {
    KPADEXStatus* ep = &status->ex_status;

    if (dev_type == WPAD_DEV_CLASSIC) {
        if (ep->cl.trig != 0 || ep->cl.release != 0) {
            kp->btn_cl_repeat_time = 0;
            kp->btn_cl_repeat_next = kp->btn_cl_repeat_delay;
            if (ep->cl.trig && kp->btn_cl_repeat_pulse) {
                ep->cl.hold |= KPAD_BUTTON_RPT;
            }
        } else if (ep->cl.hold != 0) {
            kp->btn_cl_repeat_time += kp->repeat_count;
            if (kp->btn_cl_repeat_time >= KPAD_BTN_NO_RPT_DELAY) {
                kp->btn_cl_repeat_time -= KPAD_BTN_NO_RPT_DELAY;
            }

            if (kp->btn_cl_repeat_time >= kp->btn_cl_repeat_next) {
                ep->cl.hold |= KPAD_BUTTON_RPT;
                kp->btn_cl_repeat_next += kp->btn_cl_repeat_pulse;
                if (kp->btn_cl_repeat_time >= KPAD_BTN_RPT_TIME_MAX) {
                    kp->btn_cl_repeat_time -= KPAD_BTN_RPT_TIME_MAX;
                    kp->btn_cl_repeat_next -= KPAD_BTN_RPT_TIME_MAX;
                }
            }
        }
    } else {
        if (status->trig != 0 || status->release != 0) {
            kp->btn_repeat_time = 0;
            kp->btn_repeat_next = kp->btn_repeat_delay;
            if (status->trig && kp->btn_repeat_pulse) {
                status->hold |= KPAD_BUTTON_RPT;
            }
        } else if (status->hold != 0) {
            kp->btn_repeat_time += kp->repeat_count;
            if (kp->btn_repeat_time >= KPAD_BTN_NO_RPT_DELAY) {
                kp->btn_repeat_time -= KPAD_BTN_NO_RPT_DELAY;
            }

            if (kp->btn_repeat_time >= kp->btn_repeat_next) {
                status->hold |= KPAD_BUTTON_RPT;
                kp->btn_repeat_next += kp->btn_repeat_pulse;
                if (kp->btn_repeat_time >= KPAD_BTN_RPT_TIME_MAX) {
                    kp->btn_repeat_time -= KPAD_BTN_RPT_TIME_MAX;
                    kp->btn_repeat_next -= KPAD_BTN_RPT_TIME_MAX;
                }
            }
        }
    }
}

void read_kpad_button(KPADInsideStatus* kp, KPADUnifiedWpadStatus* status, u32 dev_type) {
    KPADEXStatus* ep = &kp->status.ex_status;
    u32 old_fg_core, old_fg_cl, change_fg;

    if (kp->active_chan < 0) {
        return;
    }

    old_fg_core = kp->status.hold & KPAD_BUTTON_MASK;

    if (dev_type == WPAD_DEV_CLASSIC) {
        old_fg_cl = ep->cl.hold & KPAD_BUTTON_MASK;
    }

    if (dev_type == WPAD_DEV_FS) {
        if (status->u.core.err != 0) {
            if (status->u.core.err == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        } else {
            kp->status.hold = status->u.core.button;
        }
    } else if (dev_type == WPAD_DEV_CLASSIC) {
        if (status->u.core.err != 0) {
            if (status->u.core.err == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        } else {
            kp->status.hold = status->u.core.button;
            kp->status.ex_status.cl.hold = status->u.cl.clButton;
        }
    } else {
        if (status->u.core.err != 0) {
            if (status->u.core.err == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        } else {
            kp->status.hold = status->u.core.button;
        }
    }

    change_fg = kp->status.hold ^ old_fg_core;
    kp->status.trig = change_fg & kp->status.hold;
    kp->status.release = change_fg & old_fg_core;

    calc_button_repeat(kp, &kp->status, WPAD_DEV_CORE);

    if (dev_type == WPAD_DEV_CLASSIC) {
        change_fg = kp->status.ex_status.cl.hold ^ old_fg_cl;
        ep->cl.trig = change_fg & kp->status.ex_status.cl.hold;
        ep->cl.release = change_fg & old_fg_cl;

        calc_button_repeat(kp, &kp->status, WPAD_DEV_CLASSIC);
    }
}

void calc_acc(KPADInsideStatus* kp, f32* acc, f32 acc2) {
    f32 f1, f2;

    f2 = acc2 - *acc;

    if (f2 < 0.0f) {
        f1 = -f2;
    } else {
        f1 = f2;
    }

    if (f1 >= kp->acc_play_radius) {
        f1 = 1.0f;
    } else {
        f1 /= kp->acc_play_radius;
        f1 *= f1;
        f1 *= f1;
    }
    f1 *= kp->acc_sensitivity;
    *acc += f1 * f2;
}

static void calc_acc_horizon(KPADInsideStatus* kp) {
    f32 f1, vx, vy, ax, ay;
    f1 = sqrt(kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    if (f1 == 0.0f || f1 >= 2.0f) {
        return;
    }
    ax = kp->hard_acc.x / f1;
    ay = kp->hard_acc.y / f1;

    if (f1 > 1.0f) {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    vx = kp->accXY_nrm_hori.x * ax + kp->accXY_nrm_hori.y * ay;
    vy = kp->accXY_nrm_hori.y * ax - kp->accXY_nrm_hori.x * ay;
    ax = (vx - kp->acc_horizon.x) * f1 + kp->acc_horizon.x;
    ay = (vy - kp->acc_horizon.y) * f1 + kp->acc_horizon.y;

    f1 = sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f) {
        return;
    }
    kp->acc_horizon.x = ax / f1;
    kp->acc_horizon.y = ay / f1;

    kp->ah_circle_pos.x += (kp->acc_horizon.x - kp->ah_circle_pos.x) * kp_ah_circle_pw;
    kp->ah_circle_pos.y += (kp->acc_horizon.y - kp->ah_circle_pos.y) * kp_ah_circle_pw;

    vx = kp->acc_horizon.x - kp->ah_circle_pos.x;
    vy = kp->acc_horizon.y - kp->ah_circle_pos.y;
    if (vx * vx + vy * vy <= kp_ah_circle_radius2) {
        if (kp->ah_circle_ct) {
            --kp->ah_circle_ct;
        }
    } else {
        kp->ah_circle_ct = kp_ah_circle_ct;
    }
}

static void calc_acc_vertical(KPADInsideStatus* kp) {
    KPADStatus* sp = &kp->status;
    f32 f1;
    f32 ax;
    f32 f2;
    f32 ay;

    ax = sqrt(f2 = kp->hard_acc.x * kp->hard_acc.x + kp->hard_acc.y * kp->hard_acc.y);
    ay = -kp->hard_acc.z;
    f1 = sqrt(f2 + ay * ay);
    if (f1 == 0.0f || f1 >= 2.0f) {
        return;
    }
    ax = ax / f1;
    ay /= f1;

    if (f1 > 1.0f) {
        f1 = 2.0f - f1;
    }
    f1 *= f1 * kp_acc_horizon_pw;

    ax = (ax - sp->acc_vertical.x) * f1 + sp->acc_vertical.x;
    ay = (ay - sp->acc_vertical.y) * f1 + sp->acc_vertical.y;

    f1 = sqrt(ax * ax + ay * ay);
    if (f1 == 0.0f) {
        return;
    }
    sp->acc_vertical.x = ax / f1;
    sp->acc_vertical.y = ay / f1;
}

static f32 clamp_acc(f32 acc, f32 clamp) {
    if (acc < 0.0f) {
        if (acc < -clamp)
            return -clamp;
    } else {
        if (acc > clamp)
            return clamp;
    }
    return acc;
}

void read_kpad_acc(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp, u32 dev_type, s32 chan) {
    KPADStatus* sp = &kp->status;
    Vec vec;
    s32 probe_res;
    u32 probe_dev_type;
    f32 acc_scale_x;
    f32 acc_scale_y;
    f32 acc_scale_z;
    f32 acc_scale_fs_x;
    f32 acc_scale_fs_y;
    f32 acc_scale_fs_z;

    acc_scale_x = 0.01f;
    acc_scale_y = 0.01f;
    acc_scale_z = 0.01f;
    acc_scale_fs_x = 0.005f;
    acc_scale_fs_y = 0.005f;
    acc_scale_fs_z = 0.005f;

    probe_res = WPADProbe(chan, &probe_dev_type);
    if (probe_res == WPAD_ERR_OK) {
        WPADAcc core1G = {1, 1, 1};

        WPADGetAccGravityUnit(chan, WPAD_DEV_CORE, &core1G);
        if (core1G.x * core1G.y * core1G.z != 0) {
            acc_scale_x = 1.0f / core1G.x;
            acc_scale_y = 1.0f / core1G.y;
            acc_scale_z = 1.0f / core1G.z;
        }

        if (dev_type == WPAD_DEV_FS) {
            WPADAcc fs1G = {1, 1, 1};

            WPADGetAccGravityUnit(chan, WPAD_DEV_FS, &fs1G);
            if (fs1G.x * fs1G.y * fs1G.z != 0) {
                acc_scale_fs_x = 1.0f / fs1G.x;
                acc_scale_fs_y = 1.0f / fs1G.y;
                acc_scale_fs_z = 1.0f / fs1G.z;
            }
        }
    }

    if (kp->active_chan < 0) {
        return;
    }

    if (dev_type == WPAD_DEV_FS) {
        if (probe_res != WPAD_ERR_OK) {
            if (probe_res == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        } else {
            kp->hard_acc.x = clamp_acc((f32)(s32)-uwp->u.core.accX * acc_scale_x, kp_rm_acc_max);
            kp->hard_acc.y = clamp_acc((f32)(s32)-uwp->u.core.accZ * acc_scale_z, kp_rm_acc_max);
            kp->hard_acc.z = clamp_acc((f32)(s32)uwp->u.core.accY * acc_scale_y, kp_rm_acc_max);
        }
    } else {
        if (probe_res != WPAD_ERR_OK) {
            if (probe_res == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        } else {
            kp->hard_acc.x = clamp_acc(-uwp->u.core.accX * acc_scale_x, kp_rm_acc_max);
            kp->hard_acc.y = clamp_acc(-uwp->u.core.accZ * acc_scale_z, kp_rm_acc_max);
            kp->hard_acc.z = clamp_acc(uwp->u.core.accY * acc_scale_y, kp_rm_acc_max);
        }
    }

    vec = sp->acc;

    calc_acc(kp, &sp->acc.x, kp->hard_acc.x);
    calc_acc(kp, &sp->acc.y, kp->hard_acc.y);
    calc_acc(kp, &sp->acc.z, kp->hard_acc.z);
    sp->acc_value = sqrt(sp->acc.x * sp->acc.x + sp->acc.y * sp->acc.y + sp->acc.z * sp->acc.z);

    vec.x -= sp->acc.x;
    vec.y -= sp->acc.y;
    vec.z -= sp->acc.z;
    sp->acc_speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    calc_acc_horizon(kp);
    calc_acc_vertical(kp);

    if (dev_type == WPAD_DEV_FS) {
        vec = sp->ex_status.fs.acc;

        calc_acc(kp, &sp->ex_status.fs.acc.x, clamp_acc(-uwp->u.fs.fsAccX * acc_scale_fs_x, kp_fs_acc_max));
        calc_acc(kp, &sp->ex_status.fs.acc.y, clamp_acc(-uwp->u.fs.fsAccZ * acc_scale_fs_z, kp_fs_acc_max));
        calc_acc(kp, &sp->ex_status.fs.acc.z, clamp_acc(uwp->u.fs.fsAccY * acc_scale_fs_y, kp_fs_acc_max));

        sp->ex_status.fs.acc_value = sqrt(sp->ex_status.fs.acc.x * sp->ex_status.fs.acc.x +
                                          sp->ex_status.fs.acc.y * sp->ex_status.fs.acc.y +
                                          sp->ex_status.fs.acc.z * sp->ex_status.fs.acc.z);

        vec.x -= sp->ex_status.fs.acc.x;
        vec.y -= sp->ex_status.fs.acc.y;
        vec.z -= sp->ex_status.fs.acc.z;

        sp->ex_status.fs.acc_speed = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }
}

void get_kobj(KPADInsideStatus* kp, KPADUnifiedWpadStatus* status, u32 dev_type) {
    DPDObject* wobj_p = &status->u.core.obj[WPAD_DPD_MAX_OBJECTS - 1];
    const f32 dpd_scale = 2.0f / (f32)KPAD_DPD_RESO_WX;
    const f32 dpd_cx = (f32)(KPAD_DPD_RESO_WX - 1) / (f32)KPAD_DPD_RESO_WX;
    const f32 dpd_cy = (f32)(KPAD_DPD_RESO_WY - 1) / (f32)KPAD_DPD_RESO_WX;

    KPADObject* kobj_p;

    kobj_p = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
    do {
        if (wobj_p->size) {
            kobj_p->center.x = (f32)(s32)wobj_p->x * dpd_scale - dpd_cx;
            kobj_p->center.y = (f32)(s32)wobj_p->y * dpd_scale - dpd_cy;

            kobj_p->error_fg = 0;
            kobj_p->state_fg = 0;
        } else {
            kobj_p->error_fg = -1;
        }

        --wobj_p;
    } while (--kobj_p >= kp->kobj_sample);
}

s8 select_2obj_first(KPADInsideStatus* kp) {
    KPADObject *op1, *op2, *rp1, *rp2;
    Vec2 hori;
    f32 f1, max = kp_err_first_inpr;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0) {
                continue;
            }

            f1 = calc_horizon(kp, &op1->center, &op2->center, &hori);

            if (f1 <= kp_err_dist_min || f1 >= kp_err_dist_max) {
                continue;
            }

            f1 = kp->acc_horizon.x * hori.x + kp->acc_horizon.y * hori.y;
            if (f1 < 0.0f) {
                if (-f1 > max) {
                    max = -f1;
                    rp1 = op2;
                    rp2 = op1;
                }
            } else {
                if (f1 > max) {
                    max = f1;
                    rp1 = op1;
                    rp2 = op2;
                }
            }

        } while (++op2 <= &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);
    } while (++op1 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);

    if (max == kp_err_first_inpr) {
        return 0;
    }

    kp->kobj_regular[0] = *rp1;
    kp->kobj_regular[1] = *rp2;

    return 2;
}

static s8 select_2obj_continue(KPADInsideStatus* kp) {
    KPADObject *op1, *op2, *rp1, *rp2;
    Vec2 nrm;
    s32 rev_fg;
    f32 f1, f2, f3, vx, vy, min = 2.0f;
    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0) {
                continue;
            }
            vx = op2->center.x - op1->center.x;
            vy = op2->center.y - op1->center.y;
            f3 = sqrt(vx * vx + vy * vy);
            f1 = 1.0f / f3;
            nrm.x = vx * f1;
            nrm.y = vy * f1;

            f1 *= kp_dist_vv1;
            if (f1 <= kp_err_dist_min || f1 >= kp_err_dist_max) {
                continue;
            }

            f1 -= kp->sec_dist;
            if (f1 < 0.0f) {
                f1 *= kp_err_dist_speedM_1;
            } else {
                f1 *= kp_err_dist_speed_1;
            }
            if (f1 >= 1.0f) {
                continue;
            }

            f2 = kp->sec_nrm.x * nrm.x + kp->sec_nrm.y * nrm.y;
            if (f2 < 0.0f) {
                f2 = -f2;
                rev_fg = 1;
            } else {
                rev_fg = 0;
            }
            if (f2 <= kp_err_next_inpr) {
                continue;
            }
            f2 = (1.0f - f2) / (1.0f - kp_err_next_inpr);
            f1 += f2;
            if (f1 < min) {
                min = f1;
                if (rev_fg) {
                    rp1 = op2;
                    rp2 = op1;
                } else {
                    rp1 = op1;
                    rp2 = op2;
                }
            }

        } while (++op2 <= &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);
    } while (++op1 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1]);

    if (min == 2.0f) {
        return 0;
    }

    kp->kobj_regular[0] = *rp1;
    kp->kobj_regular[1] = *rp2;

    return 2;
}

static s8 select_1obj_first(KPADInsideStatus* kp) {
    KPADObject* op1;
    f32 vx, vy;
    Vec2 p1, p2;

    vx = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    vy = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx *= kp->trust_sec_length;
    vy *= kp->trust_sec_length;

    op1 = kp->kobj_sample;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        p1.x = op1->center.x - vx;
        p1.y = op1->center.y - vy;
        p2.x = op1->center.x + vx;
        p2.y = op1->center.y + vy;

        if (p1.x <= kobj_frame_min.x || p1.x >= kobj_frame_max.x || p1.y <= kobj_frame_min.y || p1.y >= kobj_frame_max.y) {
            if (p2.x > kobj_frame_min.x && p2.x < kobj_frame_max.x && p2.y > kobj_frame_min.y && p2.y < kobj_frame_max.y) {
                kp->kobj_regular[1] = *op1;
                kp->kobj_regular[0].center = p1;
                kp->kobj_regular[0].error_fg = 0;
                kp->kobj_regular[0].state_fg = -1;

                return -1;
            }
        } else {
            if (p2.x <= kobj_frame_min.x || p2.x >= kobj_frame_max.x || p2.y <= kobj_frame_min.y || p2.y >= kobj_frame_max.y) {
                kp->kobj_regular[0] = *op1;

                kp->kobj_regular[1].center = p2;
                kp->kobj_regular[1].error_fg = 0;
                kp->kobj_regular[1].state_fg = -1;

                return -1;
            }
        }
    } while (++op1 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS]);

    return 0;
}

s8 select_1obj_continue(KPADInsideStatus* kp) {
    KPADObject *op1, *op2, *rp1, *rp2;
    f32 f1, vx, vy;
    f32 min = kp_err_near_pos * kp_err_near_pos;

    op1 = kp->kobj_regular;
    do {
        if (op1->error_fg != 0) {
            continue;
        }
        if (op1->state_fg != 0) {
            continue;
        }

        op2 = kp->kobj_sample;
        do {
            if (op2->error_fg != 0) {
                continue;
            }

            vx = op1->center.x - op2->center.x;
            vy = op1->center.y - op2->center.y;
            f1 = vx * vx + vy * vy;
            if (f1 < min) {
                min = f1;
                rp1 = op1;
                rp2 = op2;
            }
        } while (++op2 < &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS]);
    } while (++op1 < &kp->kobj_regular[KPAD_USE_OBJECTS]);

    if (min == kp_err_near_pos * kp_err_near_pos)
        return 0;

    *rp1 = *rp2;
    kp->sec_nrm.x = kp->sec_nrm_hori.x * kp->acc_horizon.x + kp->sec_nrm_hori.y * kp->acc_horizon.y;
    kp->sec_nrm.y = kp->sec_nrm_hori.y * kp->acc_horizon.x - kp->sec_nrm_hori.x * kp->acc_horizon.y;

    vx = kp->sec_length * kp->sec_nrm.x;
    vy = kp->sec_length * kp->sec_nrm.y;
    if (rp1 == &kp->kobj_regular[0]) {
        kp->kobj_regular[1].center.x = rp1->center.x + vx;
        kp->kobj_regular[1].center.y = rp1->center.y + vy;
        kp->kobj_regular[1].error_fg = 0;
        kp->kobj_regular[1].state_fg = -1;
    } else {
        kp->kobj_regular[0].center.x = rp1->center.x - vx;
        kp->kobj_regular[0].center.y = rp1->center.y - vy;
        kp->kobj_regular[0].error_fg = 0;
        kp->kobj_regular[0].state_fg = -1;
    }

    if (kp->status.dpd_valid_fg < 0) {
        return -1;
    } else {
        return 1;
    }
}

static void calc_dpd_variable(KPADInsideStatus* kp, s8 valid_fg_next) {
    KPADStatus* sp = &kp->status;
    f32 f1, f2, f4, dist;
    Vec2 pos, vec;

    if (valid_fg_next == 0) {
        sp->dpd_valid_fg = 0;
        return;
    }

    pos.x = kp->sec_nrm_hori.x * kp->sec_nrm.x + kp->sec_nrm_hori.y * kp->sec_nrm.y;
    pos.y = kp->sec_nrm_hori.y * kp->sec_nrm.x - kp->sec_nrm_hori.x * kp->sec_nrm.y;

    if (!sp->dpd_valid_fg) {
        sp->horizon = pos;
        sp->hori_vec = Vec2_0;
        sp->hori_speed = 0.0f;
    } else {
        vec.x = pos.x - sp->horizon.x;
        vec.y = pos.y - sp->horizon.y;
        f1 = sqrt(vec.x * vec.x + vec.y * vec.y);

        if (f1 >= kp->hori_play_radius) {
            f4 = 1.0f;
        } else {
            f4 = f1 / kp->hori_play_radius;
            f4 *= f4;
            f4 *= f4;
        }
        f4 *= kp->hori_sensitivity;
        vec.x = f4 * vec.x + sp->horizon.x;
        vec.y = f4 * vec.y + sp->horizon.y;
        f4 = sqrt(vec.x * vec.x + vec.y * vec.y);
        vec.x /= f4;
        vec.y /= f4;

        sp->hori_vec.x = vec.x - sp->horizon.x;
        sp->hori_vec.y = vec.y - sp->horizon.y;
        sp->hori_speed = sqrt(sp->hori_vec.x * sp->hori_vec.x + sp->hori_vec.y * sp->hori_vec.y);

        sp->horizon = vec;
    }

    dist = kp_dist_vv1 / kp->sec_length;

    if (!sp->dpd_valid_fg) {
        sp->dist = dist;
        sp->dist_vec = 0.0f;
        sp->dist_speed = 0.0f;
    } else {
        f2 = dist - sp->dist;
        if (f2 < 0.0f) {
            f1 = -f2;
        } else {
            f1 = f2;
        }

        if (f1 >= kp->dist_play_radius) {
            f1 = 1.0f;
        } else {
            f1 /= kp->dist_play_radius;
            f1 *= f1;
            f1 *= f1;
        }
        f1 *= kp->dist_sensitivity;

        sp->dist_vec = f1 * f2;
        if (sp->dist_vec < 0.0f) {
            sp->dist_speed = -sp->dist_vec;
        } else {
            sp->dist_speed = sp->dist_vec;
        }

        sp->dist += sp->dist_vec;
    }

    pos.x = (kp->kobj_regular[0].center.x + kp->kobj_regular[1].center.x) * 0.5f;
    pos.y = (kp->kobj_regular[0].center.y + kp->kobj_regular[1].center.y) * 0.5f;

    f1 = kp->sec_nrm.x * kp->sec_nrm_hori.x + kp->sec_nrm.y * kp->sec_nrm_hori.y;
    f2 = -kp->sec_nrm.y * kp->sec_nrm_hori.x + kp->sec_nrm.x * kp->sec_nrm_hori.y;
    vec.x = f1 * pos.x - f2 * pos.y;
    vec.y = f2 * pos.x + f1 * pos.y;

    vec.x = (kp->center_org.x - vec.x) * kp->dpd2pos_scale;
    vec.y = (kp->center_org.y - vec.y) * kp->dpd2pos_scale;

    pos.x = -kp->accXY_nrm_hori.y * vec.x + kp->accXY_nrm_hori.x * vec.y;
    pos.y = -kp->accXY_nrm_hori.x * vec.x - kp->accXY_nrm_hori.y * vec.y;
    if (sp->dpd_valid_fg == 0) {
        sp->pos = pos;
        sp->vec = Vec2_0;
        sp->speed = 0.0f;
    } else {
        vec.x = pos.x - sp->pos.x;
        vec.y = pos.y - sp->pos.y;
        f1 = sqrt(vec.x * vec.x + vec.y * vec.y);

        if (f1 >= kp->pos_play_radius) {
            f4 = 1.0f;
        } else {
            f4 = f1 / kp->pos_play_radius;
            f4 *= f4;
            f4 *= f4;
        }
        f4 *= kp->pos_sensitivity;

        sp->vec.x = f4 * vec.x;
        sp->vec.y = f4 * vec.y;
        sp->speed = sqrt(sp->vec.x * sp->vec.x + sp->vec.y * sp->vec.y);

        sp->pos.x += sp->vec.x;
        sp->pos.y += sp->vec.y;
    }

    sp->dpd_valid_fg = valid_fg_next;
}

static void calc_obj_horizon(KPADInsideStatus* kp) {
    f32 f1, vx, vy;

    vx = kp->kobj_regular[1].center.x - kp->kobj_regular[0].center.x;
    vy = kp->kobj_regular[1].center.y - kp->kobj_regular[0].center.y;
    kp->sec_length = sqrt(vx * vx + vy * vy);

    f1 = 1.0f / kp->sec_length;
    kp->sec_dist = kp_dist_vv1 * f1;

    kp->sec_nrm.x = (vx *= f1);
    kp->sec_nrm.y = (vy *= f1);

    kp->obj_horizon.x = kp->sec_nrm_hori.x * vx + kp->sec_nrm_hori.y * vy;
    kp->obj_horizon.y = kp->sec_nrm_hori.y * vx - kp->sec_nrm_hori.x * vy;
}

static void check_kobj_outside_frame(KPADObject* kobj_t) {
    KPADObject* kobj_p = &kobj_t[WPAD_DPD_MAX_OBJECTS - 1];

    do {
        if (kobj_p->error_fg < 0) {
            continue;
        }

        if (kobj_p->center.x <= kobj_frame_min.x || kobj_p->center.x >= kobj_frame_max.x || kobj_p->center.y <= kobj_frame_min.y ||
            kobj_p->center.y >= kobj_frame_max.y) {
            kobj_p->error_fg |= 1;
        }
    } while (--kobj_p >= kobj_t);
}

static void check_kobj_same_position(KPADObject* kobj_t) {
    KPADObject *op1, *op2;

    op1 = kobj_t;
    do {
        if (op1->error_fg != 0) {
            continue;
        }

        op2 = op1 + 1;
        do {
            if (op2->error_fg != 0) {
                continue;
            }

            if (op1->center.x == op2->center.x && op1->center.y == op2->center.y) {
                op2->error_fg |= 2;
            }
        } while (++op2 <= &kobj_t[WPAD_DPD_MAX_OBJECTS - 1]);
    } while (++op1 < &kobj_t[WPAD_DPD_MAX_OBJECTS - 1]);
}

void read_kpad_dpd(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp, u32 dev_type) {
    KPADStatus* sp = &kp->status;
    KPADObject* op1;
    s8 valid_fg_next;
    s32 probe_res;
    u32 probe_dev_type;

    if (kp->active_chan < 0) {
        return;
    }

    probe_res = WPADProbe(kp->active_chan, &probe_dev_type);
    if (dev_type == WPAD_DEV_FS) {
        if (probe_res != WPAD_ERR_OK) {
            if (probe_res == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;

                reset_kpad(kp);
                return;
            }
        } else {
            get_kobj(kp, uwp, WPAD_DEV_FS);
        }
    } else if (dev_type == WPAD_DEV_CLASSIC) {
        if (probe_res != WPAD_ERR_OK) {
            if (probe_res == WPAD_ERR_NO_CONTROLLER) {
                kp->active_chan -= 4;

                reset_kpad(kp);
                return;
            }
        } else {
            get_kobj(kp, uwp, WPAD_DEV_CLASSIC);
        }
    } else if (probe_res != WPAD_ERR_OK) {
        if (probe_res == WPAD_ERR_NO_CONTROLLER) {
            kp->active_chan -= 4;

            reset_kpad(kp);
            return;
        }
    } else {
        get_kobj(kp, uwp, WPAD_DEV_CORE);
    }

    check_kobj_outside_frame(kp->kobj_sample);
    check_kobj_same_position(kp->kobj_sample);
    kp->valid_objs = 0;
    op1 = &kp->kobj_sample[WPAD_DPD_MAX_OBJECTS - 1];
    do {
        if (op1->error_fg == 0) {
            ++kp->valid_objs;
        }
    } while (--op1 >= kp->kobj_sample);

    if (!(sp->acc_vertical.x <= kp_err_up_inpr)) {
        if (kp->status.dpd_valid_fg == 2 || kp->status.dpd_valid_fg == -2) {
            if (kp->valid_objs >= 2) {
                valid_fg_next = select_2obj_continue(kp);
                if (valid_fg_next) {
                    goto LABEL_select_OK;
                }
            }
            if (kp->valid_objs >= 1) {
                valid_fg_next = select_1obj_continue(kp);
                if (valid_fg_next) {
                    goto LABEL_select_OK;
                }
            }
        } else if (kp->status.dpd_valid_fg == 1 || kp->status.dpd_valid_fg == -1) {
            if (kp->valid_objs >= 2) {
                valid_fg_next = select_2obj_first(kp);

                if (valid_fg_next) {
                    goto LABEL_select_OK;
                }
            }
            if (kp->valid_objs >= 1) {
                valid_fg_next = select_1obj_continue(kp);

                if (valid_fg_next) {
                    goto LABEL_select_OK;
                }
            }
        } else {
            if (kp->valid_objs >= 2) {
                valid_fg_next = select_2obj_first(kp);

                if (valid_fg_next) {
                    goto LABEL_select_OK;
                }
            }
            if (kp->valid_objs == 1) {
                valid_fg_next = select_1obj_first(kp);

                if (valid_fg_next) {
                    goto LABEL_select_OK;
                }
            }
        }
    }

    valid_fg_next = 0;

LABEL_select_OK:
    if (valid_fg_next) {
        calc_obj_horizon(kp);
        if (kp->ah_circle_ct == 0) {
            if (kp->obj_horizon.x * kp->acc_horizon.x + kp->obj_horizon.y * kp->acc_horizon.y <= kp_err_acc_inpr) {
                valid_fg_next = 0;

                kp->kobj_regular[0].error_fg = kp->kobj_regular[1].error_fg = 1;
            }
        }

        if (sp->dpd_valid_fg == 2 && valid_fg_next == 2) {
            if (kp->dpd_valid2_ct == 200) {
                kp->trust_sec_length = kp->sec_length;
            } else {
                ++kp->dpd_valid2_ct;
            }
        } else {
            kp->dpd_valid2_ct = 0;
        }
    } else {
        kp->dpd_valid2_ct = 0;
    }

    calc_dpd_variable(kp, valid_fg_next);
}

static void clamp_stick_circle(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max) {
    f32 length;
    f32 fx = (f32)sx;
    f32 fy = (f32)sy;
    f32 fmin = (f32)min;
    f32 fmax = (f32)max;

    length = sqrt(fx * fx + fy * fy);

    if (length <= fmin) {
        stick->x = stick->y = 0.0f;

    } else if (length >= fmax) {
        stick->x = fx / length;
        stick->y = fy / length;

    } else {
        length = (length - fmin) / (fmax - fmin) / length;
        stick->x = fx * length;
        stick->y = fy * length;
    }
}

static void clamp_trigger(f32* trigger, s32 tr, s32 min, s32 max) {
    if (tr <= min) {
        *trigger = 0.0f;
    } else if (tr >= max) {
        *trigger = 1.0f;
    } else {
        *trigger = (f32)(tr - min) / (f32)(max - min);
    }
}

static void clamp_stick(Vec2* stick, s32 x, s32 y, s32 min, s32 max) {
    if (x < 0) {
        clamp_trigger(&stick->x, -x, min, max);
        stick->x = -stick->x;
    } else {
        clamp_trigger(&stick->x, x, min, max);
    }
    if (y < 0) {
        clamp_trigger(&stick->y, -y, min, max);
        stick->y = -stick->y;
    } else {
        clamp_trigger(&stick->y, y, min, max);
    }

    if (stick->x * stick->x + stick->y * stick->y > 1.0f) {
        f32 rt = sqrt(stick->x * stick->x + stick->y * stick->y);
        stick->x /= rt;
        stick->y /= rt;
    }
}

static f32 calc_horizon(KPADInsideStatus* kp, Vec2* p1, Vec2* p2, Vec2* hori) {
    f32 f1, f2, vx, vy;

    vx = p2->x - p1->x;
    vy = p2->y - p1->y;
    f2 = sqrt(vx * vx + vy * vy);
    f1 = 1.0f / f2;
    vx *= f1;
    vy *= f1;

    hori->x = kp->sec_nrm_hori.x * vx + kp->sec_nrm_hori.y * vy;
    hori->y = kp->sec_nrm_hori.y * vx - kp->sec_nrm_hori.x * vy;

    return kp_dist_vv1 * f1;
}

static void clamp_stick_cross(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max) {
    f32 length;

    if (sx < 0) {
        clamp_trigger(&stick->x, -sx, min, max);
        stick->x = -stick->x;
    } else {
        clamp_trigger(&stick->x, sx, min, max);
    }
    if (sy < 0) {
        clamp_trigger(&stick->y, -sy, min, max);
        stick->y = -stick->y;
    } else {
        clamp_trigger(&stick->y, sy, min, max);
    }

    length = stick->x * stick->x + stick->y * stick->y;
    if (length > 1.0f) {
        length = sqrt(length);
        stick->x /= length;
        stick->y /= length;
    }
}

void read_kpad_stick(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp) {
    u32 dev_type;
    s32 probe_res;

    if (kp->active_chan < 0) {
        return;
    }

    probe_res = WPADProbe(kp->active_chan, &dev_type);

    if (kp->status.dev_type == WPAD_DEV_FS) {
        if (probe_res != 0) {
            if (probe_res == -1) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        }

        clamp_stick(&kp->status.ex_status.fs.stick, uwp->u.fs.fsStickX, uwp->u.fs.fsStickY, kp_fs_fstick_min, kp_fs_fstick_max);
    } else if (kp->status.dev_type == WPAD_DEV_CLASSIC) {
        if (probe_res != 0) {
            if (probe_res == -1) {
                kp->active_chan -= 4;
                reset_kpad(kp);
                return;
            }
        }

        clamp_stick(&kp->status.ex_status.cl.lstick, uwp->u.cl.clLStickX, uwp->u.cl.clLStickY, kp_cl_stick_min, kp_cl_stick_max);
        clamp_stick(&kp->status.ex_status.cl.rstick, uwp->u.cl.clRStickX, uwp->u.cl.clRStickY, kp_cl_stick_min, kp_cl_stick_max);
        clamp_trigger(&kp->status.ex_status.cl.ltrigger, uwp->u.cl.clTriggerL, kp_cl_trigger_min, kp_cl_trigger_max);
        clamp_trigger(&kp->status.ex_status.cl.rtrigger, uwp->u.cl.clTriggerR, kp_cl_trigger_min, kp_cl_trigger_max);
    }
}

s32 check_device(s32 chan, KPADInsideStatus* kp) {
    u32 dev_type;
    s32 probe_res;
    s32 is_dpd_enabled;

    probe_res = WPADProbe(chan, &dev_type);
    if (probe_res == WPAD_ERR_NO_CONTROLLER) {
        if (kp->active_chan >= 0) {
            WPADSetAutoSamplingBuf(chan, NULL, 0);
            kp->active_chan = chan - WPAD_MAX_CONTROLLERS;
            kp->status.dev_type = WPAD_DEV_NONE;
            control_dpd_end_(chan);
        }
        return -1;
    }

    if (probe_res != WPAD_ERR_OK) {
        return 0;
    }

    if (dev_type == kp->status.dev_type) {
        if (kp->unk_0x3f4) {
            reset_kpad(kp);
            return 1;
        }

        is_dpd_enabled = WPADIsDpdEnabled(chan);
        if (kp->is_dpd_enabled != 0 && is_dpd_enabled == 0) {
            set_dpd_enable(chan, dev_type);
            reset_kpad(kp);
            return 1;
        }
        if (kp->is_dpd_enabled == 0 && is_dpd_enabled != 0) {
            set_dpd_disable(chan, dev_type);
            reset_kpad(kp);
            return 1;
        }

        if (kp->unk_0x3fc) {
            if (kp->is_dpd_enabled != 0) {
                set_dpd_enable(chan, dev_type);
            } else {
                set_dpd_disable(chan, dev_type);
            }
            reset_kpad(kp);
            return 1;
        }
        return 0;
    } else if (is_valid_device(dev_type)) {
        if (kp->is_dpd_enabled != 0) {
            set_dpd_enable(chan, dev_type);
        } else {
            set_dpd_disable(chan, dev_type);
        }
    } else {
        kp->active_chan = chan - WPAD_MAX_CONTROLLERS;
        return -1;
    }

    kp->active_chan = chan;
    kp->status.dev_type = (s32)dev_type;
    reset_kpad(kp);
    return 1;
}

#if SDK_AUG2010
s32 KPADRead(s32 chan, KPADStatus* sampling_bufs, s32 length) {
    return KPADiRead(chan, sampling_bufs, length, 0, 0);
}

s32 KPADiRead(s32 chan, KPADStatus* sampling_bufs, s32 length, s32 param_3, s32 param_4) {
#else
s32 KPADRead(s32 chan, KPADStatus* sampling_bufs, s32 length) {
#endif
    KPADInsideStatus* kp;
    KPADStatus* sampling_buf_status;
    WPADStatus* statusCore;
    WPADFSStatus* statusFs;
    WPADCLStatus* statusCl;
    s32 active_chan;
    s32 var_r25;
    s32 index;
    u32 dev_type;
    s32 var_r24;
    s32 i;

    kp = &inside_kpads[chan];
    kp_err_dist_max = 1.0f + WPADGetDpdSensitivity();
    sampling_buf_status = &sampling_bufs[length - 1];
    do {
        sampling_buf_status->dev_type = kp->status.dev_type;
        sampling_buf_status--;
    } while (sampling_buf_status >= sampling_bufs);

    if (check_device(chan, kp) != 0) {
        return FALSE;
    }

    if (!is_valid_device(kp->status.dev_type)) {
        return FALSE;
    }

    active_chan = kp->active_chan;
    if (active_chan >= 0) {
        index = WPADGetLatestIndexInBuf(active_chan);
    } else {
        index = WPADGetLatestIndexInBuf(active_chan + 4);
    }
    var_r25 = kp->unk_0x116;
    if (var_r25 < 0) {
        var_r25 = index - 1;
        if (var_r25 < 0) {
            var_r25 = 11;
        }
    }
    var_r24 = index - var_r25;
    kp->unk_0x116 = index;
    if (var_r24 == 0) {
        return 0;
    }
    if (var_r24 < 0) {
        var_r24 += ARRAY_SIZE(kp->uniRingBuf);
    }
    if ((u32)var_r24 < length) {
        length = (u32)var_r24;
    }
    kp->repeat_count = 0;
    dev_type = kp->status.dev_type;
    sampling_buf_status = &sampling_bufs[length];
    if (dev_type == WPAD_DEV_CORE ||
        dev_type == WPAD_DEV_FUTURE ||
        dev_type == WPAD_DEV_INITIALIZING ||
        dev_type == WPAD_DEV_NOT_SUPPORTED) {
        do {
            var_r25++;
            if (var_r25 == ARRAY_SIZE(kp->uniRingBuf)) {
                var_r25 = 0;
            }
            statusCore = &((WPADStatus*)kp->uniRingBuf)[var_r25];
            kp->status.wpad_err = statusCore->err;
            read_kpad_acc(kp, (KPADUnifiedWpadStatus*)statusCore, WPAD_DEV_CORE, chan);
            read_kpad_dpd(kp, (KPADUnifiedWpadStatus*)statusCore, WPAD_DEV_CORE);
            if (kp->repeat_count >= var_r24 - length) {
                sampling_buf_status--;
                *sampling_buf_status = kp->status;
            }
        } while (++kp->repeat_count < var_r24);
    } else if (dev_type == WPAD_DEV_FS) {
        do {
            var_r25++;
            if (var_r25 == ARRAY_SIZE(kp->uniRingBuf)) {
                var_r25 = 0;
            }
            statusFs = &((WPADFSStatus*)kp->uniRingBuf)[var_r25];
            kp->status.wpad_err = statusFs->base.err;
            read_kpad_acc(kp, (KPADUnifiedWpadStatus*)statusFs, 1, chan);
            read_kpad_dpd(kp, (KPADUnifiedWpadStatus*)statusFs, 1);
            read_kpad_stick(kp, (KPADUnifiedWpadStatus*)statusFs);
            if (kp->repeat_count >= var_r24 - length) {
                sampling_buf_status--;
                *sampling_buf_status = kp->status;
            }
        } while (++kp->repeat_count < var_r24);
    } else if (dev_type == WPAD_DEV_CLASSIC) {
        do {
            var_r25++;
            if (var_r25 == ARRAY_SIZE(kp->uniRingBuf)) {
                var_r25 = 0;
            }
            statusCl = &((WPADCLStatus*)kp->uniRingBuf)[var_r25];
            kp->status.wpad_err = statusCl->base.err;
            read_kpad_acc(kp, (KPADUnifiedWpadStatus*)statusCl, WPAD_DEV_CLASSIC, chan);
            read_kpad_dpd(kp, (KPADUnifiedWpadStatus*)statusCl, WPAD_DEV_CLASSIC);
            read_kpad_stick(kp, (KPADUnifiedWpadStatus*)statusCl);
            if (kp->repeat_count >= (var_r24 - length)) {
                sampling_buf_status--;
                *sampling_buf_status = kp->status;
            }
        } while (++kp->repeat_count < var_r24);
    }

    if (kp->status.dev_type == WPAD_DEV_CORE ||
        kp->status.dev_type == WPAD_DEV_FUTURE ||
        kp->status.dev_type == WPAD_DEV_NOT_SUPPORTED ||
        kp->status.dev_type == WPAD_DEV_INITIALIZING) {
        read_kpad_button(kp, (KPADUnifiedWpadStatus*)statusCore, WPAD_DEV_CORE);
    } else if (kp->status.dev_type == WPAD_DEV_FS) {
        read_kpad_button(kp, (KPADUnifiedWpadStatus*)statusFs, WPAD_DEV_FS);
    } else if (kp->status.dev_type == WPAD_DEV_CLASSIC) {
        read_kpad_button(kp, (KPADUnifiedWpadStatus*)statusCl, WPAD_DEV_CLASSIC);
    }

    i = length;
    do {
        sampling_buf_status->hold = kp->status.hold;
        sampling_buf_status->trig = kp->status.trig;
        sampling_buf_status->release = kp->status.release;
        sampling_buf_status++;
        if (kp->status.dev_type == WPAD_DEV_CLASSIC) {
            sampling_buf_status->ex_status.cl.hold = kp->status.ex_status.cl.hold;
            sampling_buf_status->ex_status.cl.trig = kp->status.ex_status.cl.trig;
            sampling_buf_status->ex_status.cl.release = kp->status.ex_status.cl.release;
        }
    } while (--i != 0);

    return length;
}

void KPADInit(void) {
    s32 i;
    KPADInsideStatus* kp;

    WPADInit();

    // spin-wait
    while (WPADGetStatus() != 3) {
    }

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        kp = &inside_kpads[i];

        kp->is_dpd_enabled = TRUE;
        kp->unk_0x3fc = FALSE;
        kp->unk_0x3f0 = FALSE;
        kp->unk_0x3f4 = FALSE;
        kp->dpd_ctrl_callback = NULL;
        kp->status.dev_type = WPAD_DEV_NONE;
        kp->active_chan = i - WPAD_MAX_CONTROLLERS;

        check_device(i, kp);

        kp->dist_org = idist_org;
        kp->accXY_nrm_hori = iaccXY_nrm_hori;
        kp->sec_nrm_hori = isec_nrm_hori;
        kp->center_org = icenter_org;

        calc_dpd2pos_scale(kp);

        kp->pos_play_radius = kp->hori_play_radius = kp->dist_play_radius = kp->acc_play_radius = 0.0f;
        kp->pos_sensitivity = kp->hori_sensitivity = kp->dist_sensitivity = kp->acc_sensitivity = 1.0f;
        kp_err_dist_max = 1.0f + WPADGetDpdSensitivity();
        kp->btn_repeat_delay = KPAD_BTN_NO_RPT_DELAY;
        kp->btn_repeat_pulse = 0;
        kp->btn_cl_repeat_delay = KPAD_BTN_NO_RPT_DELAY;
        kp->btn_cl_repeat_pulse = 0;
    }

    KPADReset();
    OSRegisterVersion(__KPADVersion);
}

void KPADReset(void) {
    KPADInsideStatus* kp;

    KPADSetObjInterval(kp_obj_interval);

    kp_err_dist_speedM_1 = -1.0f / kp_err_dist_speed;
    kp_err_dist_speed_1 = 1.0f / kp_err_dist_speed;
    kobj_frame_min.x = -1.0f + kp_err_outside_frame;
    kobj_frame_max.x = 1.0f - kp_err_outside_frame;
    kobj_frame_min.y = -((f32)KPAD_DPD_RESO_WY / KPAD_DPD_RESO_WX) + kp_err_outside_frame;
    kobj_frame_max.y =  ((f32)KPAD_DPD_RESO_WY / KPAD_DPD_RESO_WX) - kp_err_outside_frame;
    kp_ah_circle_radius2 = kp_ah_circle_radius * kp_ah_circle_radius;

    kp = &inside_kpads[WPAD_MAX_CONTROLLERS - 1];
    do {
        reset_kpad(kp);
    } while (--kp >= inside_kpads);
}

BOOL is_valid_device(s32 dev_type) {
    FORCE_DONT_INLINE;
    return dev_type != WPAD_DEV_NONE;
}

void set_dpd_disable(s32 chan, s32 dev_type) {
    KPADInsideStatus* kp;
    s32 fmt;

    if (!is_valid_device(dev_type)) {
        return;
    }

    kp = &inside_kpads[chan];

    WPADSetAutoSamplingBuf(chan, NULL, 0);

    switch (dev_type) {
    case WPAD_DEV_CORE:
    case WPAD_DEV_FUTURE:
    case WPAD_DEV_NOT_SUPPORTED:
    case WPAD_DEV_INITIALIZING:
        fmt = WPAD_FMT_CORE_BTN_ACC;
        break;
    case WPAD_DEV_FS:
        fmt = WPAD_FMT_FS_BTN_ACC;
        break;
    case WPAD_DEV_CLASSIC:
        fmt = WPAD_FMT_CLASSIC_BTN_ACC;
        break;
    default:
        return;
    }

    kp->unk_0x3fc = WPADSetDataFormat(chan, fmt) == WPAD_ERR_BUSY;
    control_dpd_start_(chan);
    kp->unk_0x3f4 = TRUE;
    if (WPADControlDpd(chan, 0, KPADiControlDpdCallback)) {
        kp->unk_0x3fc = TRUE;
    }

    WPADSetAutoSamplingBuf(chan, kp->uniRingBuf, ARRAY_SIZE(kp->uniRingBuf));
}

void set_dpd_enable(s32 chan, s32 dev_type) {
    u32 cmd;
    s32 fmt;
    KPADInsideStatus* kp;

    if (!is_valid_device(dev_type)) {
        return;
    }

    kp = &inside_kpads[chan];

    WPADSetAutoSamplingBuf(chan, NULL, 0);

    switch (dev_type) {
    case WPAD_DEV_CORE:
    case WPAD_DEV_FUTURE:
    case WPAD_DEV_NOT_SUPPORTED:
    case WPAD_DEV_INITIALIZING:
        fmt = WPAD_FMT_CORE_BTN_ACC_DPD;
        cmd = 3;
        break;
    case WPAD_DEV_FS:
        fmt = WPAD_FMT_FS_BTN_ACC_DPD;
        cmd = 1;
        break;
    case WPAD_DEV_CLASSIC:
        fmt = WPAD_FMT_CLASSIC_BTN_ACC_DPD;
        cmd = 1;
        break;
    default:
        return;
    }

    kp->unk_0x3fc = WPADSetDataFormat(chan, fmt) != WPAD_ERR_OK;
    control_dpd_start_(chan);
    kp->unk_0x3f4 = TRUE;
    if (WPADControlDpd(chan, cmd, KPADiControlDpdCallback)) {
        kp->unk_0x3fc = TRUE;
    }

    WPADSetAutoSamplingBuf(chan, kp->uniRingBuf, ARRAY_SIZE(kp->uniRingBuf));
}

void control_dpd_start_(s32 chan) {
    KPADInsideStatus* kp = &inside_kpads[chan];
    BOOL irq_enabled;

    irq_enabled = OSDisableInterrupts();

    if (!kp->unk_0x3f0 && WPADProbe(chan, NULL) != WPAD_ERR_NO_CONTROLLER) {
        kp->unk_0x3f0 = TRUE;
        if (kp->dpd_ctrl_callback != NULL) {
            kp->dpd_ctrl_callback(chan, 0);
        }
    }

    OSRestoreInterrupts(irq_enabled);
}

void control_dpd_end_(s32 chan) {
    KPADInsideStatus* kp = &inside_kpads[chan];
    BOOL irq_enabled;

    irq_enabled = OSDisableInterrupts();

    if (kp->unk_0x3f0) {
        kp->unk_0x3f0 = FALSE;
        if (kp->dpd_ctrl_callback != NULL) {
            kp->dpd_ctrl_callback(chan, 1);
        }
    }

    OSRestoreInterrupts(irq_enabled);
}

void KPADDisableDPD(s32 chan) {
    KPADiRestoreDPD(chan, FALSE);
}

static s32 KPADiRestoreDPD(s32 chan, BOOL enable) {
    KPADInsideStatus *kp;
    s32 old_enable;
    s32 irq_enabled;

    kp = &inside_kpads[chan];

    irq_enabled = OSDisableInterrupts();

    old_enable = kp->is_dpd_enabled;
    kp->is_dpd_enabled = enable;
    if (old_enable != enable) {
        control_dpd_start_(chan);
    }

    OSRestoreInterrupts(irq_enabled);

    return old_enable;
}

void KPADEnableDPD(s32 chan) {
    KPADiRestoreDPD(chan, TRUE);
}

static void KPADiSamplingCallback(s32 chan) {
    u32 idx;
    u32 type;
    static struct {
        u8 dpd;
        u8 fmt;
    } table[] = {{0, WPAD_FMT_CORE_BTN_ACC},          {3, WPAD_FMT_CORE_BTN_ACC_DPD}, {0, WPAD_FMT_FS_BTN_ACC},
                 {0, WPAD_FMT_FS_BTN_ACC_DPD}, {0, WPAD_FMT_CLASSIC_BTN_ACC},  {1, WPAD_FMT_CLASSIC_BTN_ACC_DPD}};

    ASSERT((0 <= chan) && (chan < WPAD_MAX_CONTROLLERS));

    if (WPADProbe(chan, &type) == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    switch (type) {
    case WPAD_DEV_CORE:
    case WPAD_DEV_FUTURE:
    case WPAD_DEV_NOT_SUPPORTED:
    case WPAD_DEV_NONE:
        idx = 0;
        break;

    case WPAD_DEV_FS:
        idx = 2;
        break;

    case WPAD_DEV_CLASSIC:
        idx = 4;
        break;

    default:
        return;
    }

    WPADSetDataFormat(chan, table[idx].fmt);
}

void KPADiControlDpdCallback(s32 chan, s32 result) {
    KPADInsideStatus* kp = &inside_kpads[chan];
    BOOL cur_is_dpd_enabled;

    kp->unk_0x3f4 = FALSE;

    if (result != WPAD_ERR_OK) {
        kp->unk_0x3fc = TRUE;
        return;
    }

    cur_is_dpd_enabled = WPADIsDpdEnabled(chan);
    if (cur_is_dpd_enabled == kp->is_dpd_enabled && !kp->unk_0x3fc) {
        control_dpd_end_(chan);
    }
}
