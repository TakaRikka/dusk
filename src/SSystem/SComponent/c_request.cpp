/**
 * c_request.cpp
 *
 */

#include "SSystem/SComponent/c_request.h"

int cReq_Is_Done(request_base_class* i_this) {
    if (i_this->flag1 == 1) {
        i_this->flag1 = 0;
        return 1;
    }
    return 0;
}

void cReq_Done(request_base_class* i_this) {
    i_this->flag0 = 0;
    i_this->flag1 = 1;
    i_this->flag2 = 0;
}

void cReq_Command(request_base_class* i_this, u8 param_1) {
    cReq_Create(i_this, param_1);
}

void cReq_Create(request_base_class* i_this, u8 param_1) {
    i_this->flag0 = 1;
    i_this->flag1 = 0;
    i_this->flag2 = param_1;
}
