#ifndef D_KANKYO_D_KANKYO_RAIN_H
#define D_KANKYO_D_KANKYO_RAIN_H

#include <dolphin/mtx.h>

struct cXyz;
struct _GXColor;

void dKyr_drawSun(Mtx param_0, cXyz* param_1, _GXColor& param_2, u8** param_3);
void dKyr_drawLenzflare(Mtx param_0, cXyz* param_1, _GXColor& param_2, u8** param_3);
void dKyr_drawSibuki(Mtx param_0, u8** param_1);
void dKyr_drawRain(Mtx param_0, u8** param_1);
void dKyr_drawSnow(Mtx param_0, u8** param_1);
void dKyr_drawStar(Mtx param_0, u8** param_1);
void drawCloudShadow(Mtx param_0, u8** param_1);
void dKyr_drawHousi(Mtx param_0, u8** param_1);
void drawVrkumo(Mtx param_0, _GXColor& param_1, u8** param_2);
void dKyr_odour_draw(Mtx param_0, u8** param_1);
void dKyr_mud_draw(Mtx param_0, u8** param_1);
void dKyr_evil_draw(Mtx param_0, u8** param_1);
void dKyr_thunder_init();
void dKyr_thunder_move();
void dKyr_snow_init();
void dKyr_snow_move();
void dKyr_evil_init();
void dKyr_evil_move();
void dKyr_mud_init();
void dKyr_mud_move();
void dKyr_rain_init();
void dKyr_rain_move();
void cloud_shadow_move();
void dKyr_odour_init();
void dKyr_odour_move();
void dKyr_housi_init();
void dKyr_housi_move();
void dKyr_sun_move();
void dKyr_lenzflare_move();
void dKyr_star_init();
void dKyr_shstar_init();
void dKyr_star_move();
void dKyr_shstar_move();
void vrkumo_move();

class camera_class;
void dKyr_get_vectle_calc(cXyz* param_0, cXyz* param_1, cXyz* param_2);
void dKy_set_eyevect_calc2(camera_class* i_camera, Vec* o_out, f32 param_2, f32 param_3);

#endif /* D_KANKYO_D_KANKYO_RAIN_H */
