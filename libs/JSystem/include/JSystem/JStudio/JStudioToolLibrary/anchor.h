#ifndef JSTUDIOTOOLLIBRARY_ANCHOR_H
#define JSTUDIOTOOLLIBRARY_ANCHOR_H

#include "JSystem/JStudio/JStudioToolLibrary/scroll.h"

namespace JStudioToolLibrary {
    enum TEAnchor {

    };

    struct TPoint {
        TPoint(const TPoint&);
        TPoint(int, int);

        void setX(int);
        void setY(int);

        int getX() const;
        int getY() const;

        /* 0x0 */ int iX_;
        /* 0x4 */ int iY_;
    };

    struct TAnchor_corner {
        TAnchor_corner(TEAnchor, int, int);

        void setAnchor(TEAnchor);
        void setAnchorPosition(const TPoint&);
        void getPosition(TPoint*, const TRectangle&, int, int) const;
        TEAnchor getAnchor() const;
        TPoint* getAnchorPosition() const;

        /* 0x0 */ TEAnchor eAnchor_;
        /* 0x4 */ TPoint oAnchorPosition_;
    };
}

#endif
