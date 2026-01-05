#ifndef MSL_VA_LIST_H
#define MSL_VA_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char reserved[2];
    char* input_arg_area;
    char* reg_save_area;
} _va_list_struct;

typedef _va_list_struct __va_list[1];

void* __va_arg(_va_list_struct*, int);

#ifndef __MWERKS__
#define __builtin_va_info(...)
#define _var_arg_typeof(...)
#endif

// from defined-string in mwcceppc.exe:
// __builtin_va_info: initialize the __va_list_struct
// _var_arg_typeof: convert type to integer for __va_arg
#define __va_start(list, fmt) __builtin_va_info(&list)
#define __va_arg(list, type) (*((type*)__va_arg(list, _var_arg_typeof(type))))
#define va_start __va_start
#define va_arg __va_arg
#define va_end __va_end
#define va_list __va_list
#define __va_end(list) ((void)0)

#define __va_copy(a, b) (*(a) = *(b))

#ifdef __cplusplus
}
#endif

#endif /* __VA_ARG_H */
