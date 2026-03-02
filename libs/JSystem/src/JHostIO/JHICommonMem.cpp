#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JHICommonMem.h"

JHIMemBuf* JHICommonMem::instance;

JHIMemBuf* JHICommonMem::Instance() {
    if (instance == NULL) {
        instance = new JHIMemBuf();
    }

    return instance;
}

JHIMemBuf::JHIMemBuf() {
    mp_buffer = NULL;
    create();
}
