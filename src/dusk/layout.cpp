#include "dusk/layout.hpp"

using namespace dusk;

LayoutRect LayoutRect::FitRectInRect(
    const f32 widthOuter,
    const f32 heightOuter,
    const f32 widthInner,
    const f32 heightInner) {

    // Try as if constrained vertically first.
    auto width = widthInner * (heightOuter / heightInner);
    auto height = heightOuter;
    if (width > widthOuter) {
        // Otherwise, constrained horizontally.
        width = widthOuter;
        height = heightOuter * (widthOuter / widthInner);
    }

    // Center it
    const auto posX = (widthOuter - width) / 2;
    const auto posY = (heightOuter - height) / 2;

    return {posX, posY, posX + width, posY + height};
}
