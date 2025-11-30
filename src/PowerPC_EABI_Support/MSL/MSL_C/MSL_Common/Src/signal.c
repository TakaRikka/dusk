#include "signal.h"
#include "critical_regions.h"

#define __std(ref) ref

__signal_func_ptr signal_funcs[6];

int raise(int sig) {
    __signal_func_ptr signal_func;

    if (sig < 1 || sig > 6) {
        return -1;
    }

    __begin_critical_region(stderr_access);
    signal_func = signal_funcs[sig - 1];

    if (signal_func != ((__std(__signal_func_ptr))1)) {
        signal_funcs[sig - 1] = ((__std(__signal_func_ptr))0);
    }

    __end_critical_region(stderr_access);

    if (signal_func == ((__std(__signal_func_ptr))1) ||
        (signal_func == ((__std(__signal_func_ptr))0) && sig == 1))
    {
        return 0;
    }

    if (signal_func == ((__std(__signal_func_ptr))0)) {
        exit(0);
    }

    (*signal_func)(sig);
    return 0;
}
