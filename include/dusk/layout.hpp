#ifndef DUSK_LAYOUT_H
#define DUSK_LAYOUT_H

#include "dolphin/types.h"

namespace dusk {

/**
 * Helper struct for laying things out on the screen. Represents a rectangle via two corner
 * positions.
 */
struct LayoutRect {
    f32 PosX;
    f32 PosY;
    f32 PosX2;
    f32 PosY2;

    [[nodiscard]] constexpr f32 Width() const {
        return PosX2 - PosX;
    }

    [[nodiscard]] constexpr f32 Height() const {
        return PosY2 - PosY;
    }

    /**
     * Calculates the position to render one rectangle inside another, centered and maintaining aspect ratio.
     */
    [[nodiscard]] static LayoutRect FitRectInRect(
        f32 widthOuter,
        f32 heightOuter,
        f32 widthInner,
        f32 heightInner);
};
}

#endif  // DUSK_LAYOUT_H
