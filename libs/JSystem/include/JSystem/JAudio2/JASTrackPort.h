#ifndef JASTRACKPORT_H
#define JASTRACKPORT_H

#include <types.h>

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASTrackPort {
public:
    static const int MAX_PORTS = 16;

    void init();
    u16 readImport(u32);
    u16 readExport(u32);
    void writeImport(u32, u16);
    void writeExport(u32, u16);
    u32 checkImport(u32) const;
    u32 checkExport(u32) const;

    u16 get(u32 param_0) const { return mPortValues[param_0]; }
    void set(u32 param_0, u16 param_1) { mPortValues[param_0] = param_1; }

    u16 field_0x0;
    u16 field_0x2;
    u16 mPortValues[MAX_PORTS];
};

#endif /* JASTRACKPORT_H */
