#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JHIMccBuf.h"
#include <revolution/hio2.h>

HIO2DeviceType gExiDevice = HIO2_DEVICE_INVALID;
u8 data_8074bd04 = 1;

s32 ghHIO2;
JHIMccContext tContext_old;
JHIMccContext tContext_new;
bool data_8074d138;
u8 data_8074d139;

BOOL JHIhio2CallbackEnum(HIO2DeviceType type) {
    gExiDevice = type;
    return 0;
}

void JHIhio2DisconnectCallback(s32) {
    gExiDevice = HIO2_DEVICE_INVALID;
}

u32 JHIInitInterface() {
    if (data_8074bd04) {
        if (!HIO2Init()) {
            return 0;
        }

        if (!HIO2EnumDevices(JHIhio2CallbackEnum)) {
            return 0;
        }

        data_8074bd04 = 0;
    }

    if (gExiDevice == HIO2_DEVICE_INVALID) {
        return 0;
    }

    if (data_8074d139 != 0) {
        HIO2Close(ghHIO2);
        data_8074d139 = 0;
    }

    if (data_8074d139 == 0) {
        ghHIO2 = HIO2Open(gExiDevice, 0, JHIhio2DisconnectCallback);
        if (ghHIO2 == -1) {
            return 0;
        }

        data_8074d139 = 1;
    }

    return 1;
}

bool JHINegotiateInterface(u32) {
    return 0;
}

JHIMccContext JHIGetHiSpeedContext() {
    if (tContext_new.mp_reader == NULL) {
        tContext_new.mp_reader = new JHIMccBufReader(1, 0x18, 0x6000);
    }
    
    if (tContext_new.mp_writer == NULL) {
        tContext_new.mp_writer = new JHIMccBufWriter(1, 0x18, 0x6000);
    }

    return tContext_new;
}

JHIMccContext JHIGetLowSpeedContext() {
    if (tContext_old.mp_reader == NULL) {
        tContext_old.mp_reader = new JHIMccBufReader(1, 2, 0);
    }
    
    if (tContext_old.mp_writer == NULL) {
        tContext_old.mp_writer = new JHIMccBufWriter(1, 2, 0);
    }

    return tContext_old;
}

BOOL JHIInitMCC(JHIMccContext* pCtx, bool* param_1) {
    JHIGetHiSpeedContext();
    JHIGetLowSpeedContext();

    if (!JHIInitInterface()) {
        *pCtx = tContext_old;
        return FALSE;
    }

    data_8074d138 = JHINegotiateInterface(800);
    if (data_8074d138) {
        tContext_new.mp_reader->enablePort();
        tContext_new.mp_writer->enablePort();
        tContext_new.mp_reader->init();
        tContext_new.mp_writer->init();
    } else {
        tContext_old.mp_reader->enablePort();
        tContext_old.mp_writer->enablePort();
        tContext_old.mp_reader->init();
        tContext_old.mp_writer->init();
    }

    if (param_1 != NULL) {
        *param_1 = data_8074d138;
    }

    if (data_8074d138) {
        *pCtx = tContext_new;
    } else {
        *pCtx = tContext_old;
    }

    return TRUE;
}

s32 JHIGetHIO2Handle() {
    return ghHIO2;
}
