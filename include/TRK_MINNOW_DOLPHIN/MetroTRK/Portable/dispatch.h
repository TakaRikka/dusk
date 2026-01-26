#ifndef METROTRK_PORTABLE_DISPATCH_H
#define METROTRK_PORTABLE_DISPATCH_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TRK_DISPATCH_CMD_CONNECT        1 /* Connect to the console */
#define TRK_DISPATCH_CMD_DISCONNECT     2 /* Disconnect from the console */
#define TRK_DISPATCH_CMD_RESET          3 /* Reset the debugger */
#define TRK_DISPATCH_CMD_GETVERSION     4 /* Get debugger version */
#define TRK_DISPATCH_CMD_GETSUPPORTMASK 5 /* Get Support Mask */
#define TRK_DISPATCH_CMD_OVERRIDE       7 /* Override? */
#define TRK_DISPATCH_CMD_READMEM        16 /* Reading from memory */
#define TRK_DISPATCH_CMD_WRITEMEM       17 /* Writing to memory */
#define TRK_DISPATCH_CMD_READREGS       18 /* Read a register value */
#define TRK_DISPATCH_CMD_WRITEREGS      19 /* Set a register */
#define TRK_DISPATCH_CMD_SETOPTION      23 /* Set an option? */
#define TRK_DISPATCH_CMD_CONTINUE       24 /* Continue debugging */
#define TRK_DISPATCH_CMD_STEP           25 /* Step through an instruction */
#define TRK_DISPATCH_CMD_STOP           26 /* Stop the debugger */

typedef struct TRKBuffer TRKBuffer;

BOOL TRKDispatchMessage(TRKBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_DISPATCH_H */
