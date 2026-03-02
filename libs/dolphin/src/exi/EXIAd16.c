#include <dolphin/dolphin.h>
#include <dolphin/exi.h>

static BOOL Initialized;

int AD16Init(void) {
    int err;
    u32 cmd;
    u32 id;

    if (Initialized) {
        return 1;
    }

    if (!EXILock(2, 0, NULL)) {
        return 0;
    }

    if (!EXISelect(2, 0, EXI_FREQ_1M)) {
        EXIUnlock(2);
        return 0;
    }

    cmd = 0;
    err = 0;
    err |= !EXIImm(2, &cmd, 2, EXI_WRITE, NULL);
    err |= !EXISync(2);
    err |= !EXIImm(2, &id, 4, EXI_READ, NULL);
    err |= !EXISync(2);
    err |= !EXIDeselect(2);

    EXIUnlock(2);
    if (err != 0 || (id - 0x04120000) != 0) {
        return 0;
    }

    Initialized = TRUE;
    return 1;
}

int AD16WriteReg(u32 word) {
    int err;
    u32 cmd;

    if (!Initialized || !EXILock(2, 0, NULL)) {
        return 0;
    }

    if (!EXISelect(2, 0, EXI_FREQ_8M)) {
        EXIUnlock(2);
        return 0;
    }

    cmd = 0xA0000000;
    err = 0;
    err |= !EXIImm(2, &cmd, 1, EXI_WRITE, NULL);
    err |= !EXISync(2);
    err |= !EXIImm(2, &word, 4, EXI_WRITE, NULL);
    err |= !EXISync(2);
    err |= !EXIDeselect(2);

    EXIUnlock(2);
    if (err != 0) {
        return 0;
    }

    return 1;
}

int AD16ReadReg(u32* word) {
    int err;
    u32 cmd;

    if (!Initialized || !EXILock(2, 0, NULL)) {
        return 0;
    }

    if (!EXISelect(2, 0, EXI_FREQ_8M)) {
        EXIUnlock(2);
        return 0;
    }

    cmd = 0xA2000000;
    err = 0;
    err |= !EXIImm(2, &cmd, 1, EXI_WRITE, NULL);
    err |= !EXISync(2);
    err |= !EXIImm(2, word, 4, EXI_READ, NULL);
    err |= !EXISync(2);
    err |= !EXIDeselect(2);

    EXIUnlock(2);
    if (err != 0) {
        return 0;
    }

    return 1;
}

BOOL AD16Probe(void) {
    return Initialized;
}
