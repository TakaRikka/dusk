#ifndef C_REQUEST_H
#define C_REQUEST_H

#include <dolphin/types.h>

struct request_base_class {
    u8 flag0 : 1;
    u8 flag1 : 1;
    u8 flag2 : 6;
};

int cReq_Is_Done(request_base_class*);
void cReq_Done(request_base_class*);
void cReq_Command(request_base_class*, u8);
void cReq_Create(request_base_class*, u8);

#endif /* C_REQUEST_H */
