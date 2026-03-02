#include <revolution/base/PPCArch.h>

#ifdef __cplusplus
extern "C" {
#endif

void __init_cpp(void);
void _ExitProcess(void);

typedef void (*voidfunctionptr)();
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void __init_user(void) {
    __init_cpp();
}

void __init_cpp(void) {
    /**
     *	call static initializers
     */
    voidfunctionptr* constructor;
    for (constructor = _ctors; *constructor; constructor++) {
        (*constructor)();
    }
}

static void __fini_cpp(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor; dtor++) {
        (*dtor)();
    }
}

void exit(int status) {
    __fini_cpp();
    _ExitProcess();
}

void _ExitProcess(void) {
    PPCHalt();
}

#ifdef __cplusplus
}
#endif
