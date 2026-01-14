#ifndef D_D_ERROR_MSG_H
#define D_D_ERROR_MSG_H

struct dShutdownErrorMsg_c {
    static bool execute();
};

struct dDvdErrorMsg_c {
    static void draw(s32);
    static bool execute();
};

#if !PLATFORM_GCN
struct dConnectErrorMsg_c {
    static void disable();
};
#endif

#endif /* D_D_ERROR_MSG_H */
