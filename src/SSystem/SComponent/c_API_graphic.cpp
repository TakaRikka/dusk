/**
 * c_API_graphic.cpp
 *
 */

#include "SSystem/SComponent/c_API_graphic.h"
#include "SSystem/SComponent/c_API.h"

void cAPIGph_Painter() {
    g_cAPI_Interface.painterMtd();
}

void cAPIGph_BeforeOfDraw() {
    g_cAPI_Interface.beforeOfDrawMtd();
}

void cAPIGph_AfterOfDraw() {
    g_cAPI_Interface.afterOfDrawMtd();
}
