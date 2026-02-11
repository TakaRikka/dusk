#ifndef F_PC_LAYER_ITER_H_
#define F_PC_LAYER_ITER_H_

#include <dolphin/types.h>

typedef struct layer_class layer_class;

typedef struct layer_iter {
    /* 0x0 */ void* func;
    /* 0x4 */ void* data;
} layer_iter;

typedef int (*fpcLyIt_OnlyHereFunc)(void*, void*);
typedef void* (*fpcLyIt_JudgeFunc)(void*, void*);

int fpcLyIt_OnlyHere(layer_class* i_layer, fpcLyIt_OnlyHereFunc i_func, void* i_data);
int fpcLyIt_OnlyHereLY(layer_class* i_layer, fpcLyIt_OnlyHereFunc i_func, void* i_data);
int fpcLyIt_All(fpcLyIt_OnlyHereFunc i_func, void* i_data);
void* fpcLyIt_Judge(layer_class* i_layer, fpcLyIt_JudgeFunc i_func, void* i_data);
void* fpcLyIt_AllJudge(fpcLyIt_JudgeFunc i_func, void* i_data);

#endif
