#ifndef _REVOLUTION_KPAD_PRIVATE_H_
#define _REVOLUTION_KPAD_PRIVATE_H_

#include <revolution/kpad.h>
#include "types.h"

static void calc_dpd2pos_scale(KPADInsideStatus* kp);
void reset_kpad(KPADInsideStatus* kp);
void calc_button_repeat(KPADInsideStatus* kp, KPADStatus* status, u32 dev_type);
void read_kpad_button(KPADInsideStatus* kp, KPADUnifiedWpadStatus* status, u32 dev_type);
void calc_acc(KPADInsideStatus* kp, f32* acc, f32 acc2);
static void calc_acc_horizon(KPADInsideStatus* kp);
static void calc_acc_vertical(KPADInsideStatus* kp);
static f32 clamp_acc(f32 acc, f32 clamp);
void read_kpad_acc(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp, u32 dev_type, s32 chan);
void get_kobj(KPADInsideStatus* kp, KPADUnifiedWpadStatus* status, u32 param_2);
s8 select_2obj_first(KPADInsideStatus* kp);
static s8 select_2obj_continue(KPADInsideStatus* kp);
static s8 select_1obj_first(KPADInsideStatus* kp);
s8 select_1obj_continue(KPADInsideStatus* kp);
static void calc_dpd_variable(KPADInsideStatus* kp, s8 valid_fg_next);
static void calc_obj_horizon(KPADInsideStatus* kp);
static void check_kobj_outside_frame(KPADObject* kobj_t);
static void check_kobj_same_position(KPADObject* kobj_t);
void read_kpad_dpd(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp, u32 dev_type);
static void clamp_stick_circle(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max);
static void clamp_trigger(f32* trigger, s32 tr, s32 min, s32 max);
static void clamp_stick(Vec2* stick, s32 x, s32 y, s32 min, s32 max);
static f32 calc_horizon(KPADInsideStatus* kp, Vec2* p1, Vec2* p2, Vec2* hori);
static void clamp_stick_cross(Vec2* stick, s32 sx, s32 sy, s32 min, s32 max);
void read_kpad_stick(KPADInsideStatus* kp, KPADUnifiedWpadStatus* uwp);
s32 check_device(s32 chan, KPADInsideStatus* kp);
static BOOL is_valid_device(s32 dev_type);
static void set_dpd_disable(s32 chan, s32 dev_type);
static void set_dpd_enable(s32 chan, s32 dev_type);
static void control_dpd_start_(s32 chan);
static void control_dpd_end_(s32 chan);
static f32 calc_horizon(KPADInsideStatus* kp, Vec2* p1, Vec2* p2, Vec2* hori);
static s32 KPADiRestoreDPD(s32 chan, BOOL enable);
static void KPADiSamplingCallback(s32 chan);
#if SDK_AUG2010
s32 KPADiRead(s32 chan, KPADStatus* sampling_bufs, s32 length, s32 param_3, s32 param_4);
#endif
void KPADiControlDpdCallback(s32 chan, s32 result);

#endif /* _REVOLUTION_KPAD_PRIVATE_H_ */
