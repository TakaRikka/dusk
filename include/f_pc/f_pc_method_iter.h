
#ifndef F_PC_METHOD_ITER_H_
#define F_PC_METHOD_ITER_H_

#include <dolphin/types.h>

typedef struct node_list_class node_list_class;

typedef int (*fpcMtdIt_MethodFunc)(void*);

int fpcMtdIt_Method(node_list_class* pList, fpcMtdIt_MethodFunc pMethod);

#endif
