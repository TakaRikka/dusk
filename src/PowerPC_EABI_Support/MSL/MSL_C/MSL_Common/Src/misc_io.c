#include "misc_io.h"
#include "abort_exit.h"
#include "ansi_files.h"

void __stdio_atexit(void) {
    __stdio_exit = __close_all;
}
