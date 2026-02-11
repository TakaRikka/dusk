#ifndef OS_DOLPHIN_DOLPHIN_TRK_GLUE_H
#define OS_DOLPHIN_DOLPHIN_TRK_GLUE_H

#include <dolphin/os.h>
#include "trk.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*DBCommFunc)();
typedef int (*DBCommInitFunc)(void*, __OSInterruptHandler);
typedef int (*DBCommReadFunc)(u8*, int);
typedef int (*DBCommWriteFunc)(const u8*, int);

typedef struct DBCommTable {
    DBCommInitFunc initialize_func;
    DBCommFunc init_interrupts_func;
    DBCommFunc shutdown_func;
    DBCommFunc peek_func;
    DBCommReadFunc read_func;
    DBCommWriteFunc write_func;
    DBCommFunc open_func;
    DBCommFunc close_func;
    DBCommFunc pre_continue_func;
    DBCommFunc post_stop_func;
} DBCommTable;

void UnreserveEXI2Port();
void ReserveEXI2Port();
UARTError TRKWriteUARTN(const void*, u32);
void TRKLoadContext(OSContext* ctx, u32 r4);
int InitMetroTRKCommTable(int hwId);

#ifdef __cplusplus
}
#endif


#endif /* OS_DOLPHIN_DOLPHIN_TRK_GLUE_H */
