/**
 * d_kyeff.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_kyeff.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo_wether.h"

#if DEBUG
class dKyeff_HIO_c : public JORReflexible {
public:
    dKyeff_HIO_c();
    virtual ~dKyeff_HIO_c();

    void genMessage(JORMContext*);

    /* 0x04 */ cXyz camera_eye;
    /* 0x10 */ cXyz camera_cnt;
    /* 0x1C */ f32 wind_power;
    /* 0x20 */ f32 time_speed;
    /* 0x24 */ s8 id;
};

dKyeff_HIO_c g_kyeffHIO;

dKyeff_HIO_c::~dKyeff_HIO_c() {}

dKyeff_HIO_c::dKyeff_HIO_c() {
    g_kyeffHIO.camera_eye.x = 9377.0f;
    g_kyeffHIO.camera_eye.y = 0.0f;
    g_kyeffHIO.camera_eye.z = 7644.0f;

    g_kyeffHIO.camera_cnt.x = 4300.0f;
    g_kyeffHIO.camera_cnt.y = 4800.0f;
    g_kyeffHIO.camera_cnt.z = 1000.0f;

    g_kyeffHIO.wind_power = 0.5f;
    g_kyeffHIO.time_speed = 0.03f;
}

void dKyeff_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genSlider("時刻経過スピード", &time_speed, 0.0f, 100.0f);

    mctx->genSlider("Camera Eye X", &camera_eye.x, -10000.0f, 10000.0f);
    mctx->genSlider("           Y", &camera_eye.y, -10000.0f, 10000.0f);
    mctx->genSlider("           Z", &camera_eye.z, -10000.0f, 10000.0f);

    mctx->genSlider("Camera Cnt X", &camera_cnt.x, -10000.0f, 10000.0f);
    mctx->genSlider("           Y", &camera_cnt.y, -10000.0f, 10000.0f);
    mctx->genSlider("           Z", &camera_cnt.z, -10000.0f, 10000.0f);

    mctx->genSlider("風力", &wind_power, 0.0f, 1.0f);
}
#endif

static int dKyeff_Draw(dKyeff_c* i_this) {
    UNUSED(i_this);

    dKyw_wether_draw();
    return 1;
}

int dKyeff_c::execute() {
    if (strcmp(dComIfGp_getStartStageName(), "Name") != 0) {
        dKyw_wether_move();
    }

    dKyw_wether_move_draw();
    dKy_FiveSenses_fullthrottle_dark();
    mDoAud_mEnvse_framework();
    return 1;
}

static int dKyeff_Execute(dKyeff_c* i_this) {
    return i_this->execute();
}

static int dKyeff_IsDelete(dKyeff_c*) {
    return 1;
}

static int dKyeff_Delete(dKyeff_c* i_this) {
    UNUSED(i_this);

    dKyw_wether_delete();

    #if DEBUG
    if (strcmp(dComIfGp_getStartStageName(), "Name") == 0) {
        mDoHIO_deleteChild(g_kyeffHIO.id);
    }
    #endif

    return 1;
}

// Helper functions to set float literal order
static f32 dummyLiteralOrder() {
    return 0.0f;
}

static f32 dummyLiteralOrder2() {
    return 1.0f;
}

static f32 dummyLiteralOrder3(s32 hours) {
    return hours;
}

static int dKyeff_Create(kankyo_class* i_this) {
    UNUSED(i_this);
    OSCalendarTime calendar;

    dKyw_wether_init();

    if (strcmp(dComIfGp_getStartStageName(), "Name") == 0) {
        camera_class* camera = dComIfGp_getCamera(0);
        OSTime time = OSGetTime();
        OSTicksToCalendarTime(time, &calendar);

        g_env_light.global_wind_influence.vec.x = 1.0f;
        g_env_light.global_wind_influence.vec.y = 0.0f;
        g_env_light.global_wind_influence.vec.z = 0.0f;
        g_env_light.global_wind_influence.pow = 0.7f;
        g_env_light.daytime = calendar.hour * 15.0f;

        #if DEBUG
        g_kyeffHIO.id = mDoHIO_createChild("VR-BOX", &g_kyeffHIO);
        #endif
    }

    return cPhs_COMPLEATE_e;
}

static leafdraw_method_class l_dKyeff_Method = {
    (process_method_func)dKyeff_Create, 
    (process_method_func)dKyeff_Delete,
    (process_method_func)dKyeff_Execute,
    (process_method_func)dKyeff_IsDelete,
    (process_method_func)dKyeff_Draw,
};

kankyo_process_profile_definition g_profile_KYEFF = {
    fpcLy_CURRENT_e,
    12,
    fpcPi_CURRENT_e,
    PROC_KYEFF,
    &g_fpcLf_Method.base,
    sizeof(dKyeff_c),
    0,
    0,
    &g_fopKy_Method,
    5,
    &l_dKyeff_Method,
};
