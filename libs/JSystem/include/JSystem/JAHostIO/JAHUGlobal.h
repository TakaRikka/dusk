#ifndef JAHUGLOBAL_H
#define JAHUGLOBAL_H

#include <cstring>
#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JUtility/JUTAssert.h"

inline void JAHUDialog_CatComment(char* param_1, const char* param_2, const char* param_3) {
    if (param_3) {
        std::strcpy(param_1, param_3);
        std::strcat(param_1, "\n\n");
    }
    std::strcat(param_1, param_2);
}

inline void JAHUDialog_OK(const char* mes, const char* param_2, const char* param_3) {
    char buffer[128];
    if (param_3) {
        JAHUDialog_CatComment(buffer, mes, param_3);
    } else {
        // clang-format off
        JUT_ASSERT(144, std::strlen(mes)<128-1)
        // clang-format on
        std::strcpy(buffer, mes);
    }

    JORMessageBox(buffer, param_2, 0x30);
}

#endif /* JAHUGLOBAL_H */
