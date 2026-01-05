#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/std-streambuf.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <algorithm>

namespace JGadget {
TStreamBuffer::~TStreamBuffer() {}

void TStreamBuffer::setbuf(char*, s32) {}

s32 TStreamBuffer::sync() {
    return 0;
}

int TStreamBuffer::underflow() {
    return TTrait_char<char>::eof();
}

int TStreamBuffer::uflow() {
    int var_r30 = TTrait_char<char>::eof();
    int var_r29;

    if (TTrait_char<char>::eq_int_type(underflow(), var_r30)) {
        return var_r30;
    }

    var_r29 = TTrait_char<char>::to_int_type(*pCurrent_get_);
    pCurrent_get_++;
    return var_r29;
}

s32 TStreamBuffer::xsputn(const char* param_0, s32 param_1) {
    s32 var_r29 = std::min(param_1, pEnd_put_ - pCurrent_put_);
    if (var_r29 > 0) {
        const char* var_r27 = param_0 + var_r29;
        JUT_ASSERT(70, pCurrent_put_!=NULL);
        pCurrent_put_ = std::copy<char>(param_0, var_r27, pCurrent_put_);
        param_0 = var_r27;
        param_1 -= var_r29;
    }

    while (param_1 > 0) {
        if (TTrait_char<char>::eq_int_type(sputc(*param_0), TTrait_char<char>::eof())) {
            break;
        }

        param_1--;
        param_0++;
        var_r29++;
        s32 var_r25 = var_r29;
    } 

    return var_r29;
}

int TStreamBuffer::overflow(int) {
    return TTrait_char<char>::eof();
}
}
