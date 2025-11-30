/**
 * f_pc_draw_priority.cpp
 * Framework - Process Draw Priority
 */

#include "f_pc/f_pc_draw_priority.h"

s16 fpcDwPi_Get(const draw_priority_class* i_drawpriority) {
    return i_drawpriority->priority;
}

void fpcDwPi_Set(draw_priority_class* i_drawpriority, s16 i_priority) {
    i_drawpriority->priority = i_priority;
}

void fpcDwPi_Init(draw_priority_class* i_drawpriority, s16 i_priority) {
    fpcDwPi_Set(i_drawpriority, i_priority);
}
