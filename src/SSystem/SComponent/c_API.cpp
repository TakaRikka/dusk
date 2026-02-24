/**
 * c_API.cpp
 *
 */

#include "SSystem/SComponent/c_API.h"

extern void mDoGph_BlankingON();
extern void mDoGph_BlankingOFF();
extern int mDoGph_BeforeOfDraw();
extern int mDoGph_AfterOfDraw();
extern int mDoGph_Painter();
extern int mDoGph_Create();

cAPI_Interface g_cAPI_Interface = {
    (cAPIGph_Mthd)mDoGph_Create,
    (cAPIGph_Mthd)mDoGph_BeforeOfDraw,
    (cAPIGph_Mthd)mDoGph_AfterOfDraw,
    (cAPIGph_Mthd)mDoGph_Painter,
    mDoGph_BlankingON,
    mDoGph_BlankingOFF,
};
