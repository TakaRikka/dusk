#ifndef Z2TRACKVIEW_H
#define Z2TRACKVIEW_H

#include "JSystem/JAWExtSystem/JAWWindow.h"

class Z2TrackView : public JAWWindow {
public:
    enum DispMode {
        DISPMODE0
    };

    Z2TrackView(u8, DispMode);
};

#endif /* Z2TRACKVIEW_H */
