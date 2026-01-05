#ifndef JAWCHVIEW_H
#define JAWCHVIEW_H

#include "JSystem/JAWExtSystem/JAWWindow.h"

class JAWChView : public JAWWindow {
public:
    JAWChView();

    /* 0x3F0 */ int field_0x3f0;
    /* 0x3F4 */ u8 field_0x3f4[4];
};

#endif /* JAWCHVIEW_H */
