#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_select_icon.h"
#include "JSystem/J2DGraph/J2DAnimation.h"
#include "dusk/frame_interpolation.h"

dSi_HIO_c::dSi_HIO_c() {}

void dSelect_icon_c::animation() {
    if (field_0x10->getAlpha() != 0) {
#ifdef TARGET_PC
        if (dusk::frame_interp::get_ui_tick_pending())
#endif
        {
            field_0x20 += field_0x2c;
            if (field_0x20 >= field_0x1c->getFrameMax()) {
                field_0x20 = 0.0f;
            }
            field_0x1c->setFrame(field_0x20);

            field_0x28 += field_0x2c;
            if (field_0x28 >= field_0x24->getFrameMax()) {
                field_0x28 = 0.0f;
            }
        }
#ifdef TARGET_PC
        // FRAME INTERP NOTE: Set even if not advancing
        field_0x1c->setFrame(field_0x20);
#endif

        field_0x24->setFrame(field_0x28);
        field_0x8->animation();
        field_0x10->paneScale(field_0x14, field_0x14);
    }
}

void dSelect_icon_c::setAlpha(u8 param_0) {
    field_0x10->setAlpha(param_0);
}

void dSelect_icon_c::setPos(J2DPane* param_0, f32 transX, f32 transY) {
    field_0x14 = param_0->getWidth() / 100.0f;
    field_0x18 = param_0->getHeight() / 100.0f;

    field_0x10->paneScale(field_0x14, field_0x14);
    field_0xc->getPanePtr()->translate(transX, transY);
}

static dSi_HIO_c g_siHIO;
