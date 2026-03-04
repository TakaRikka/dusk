#ifndef NW4HBM_RECT_H
#define NW4HBM_RECT_H

#include <revolution/types.h>

#include "../math/arithmetic.h"

namespace nw4hbm {
    namespace ut {

        class Rect {
        public:
            Rect() : left(), top(), right(), bottom() {}
            Rect(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {}

            ~Rect() {}

            void SetWidth(f32 width) { right = left + width; }
            void SetHeight(f32 height) { bottom = top + height; }

            f32 GetWidth() const { return right - left; }
            f32 GetHeight() const { return bottom - top; }

            void MoveTo(f32 x, f32 y) {
                right = x + GetWidth();
                left = x;

                bottom = y + GetHeight();
                top = y;
            }

            void Normalize() {
                f32 l = left;
                f32 t = top;
                f32 r = right;
                f32 b = bottom;

                left = math::FSelect(r - l, l, r);
                right = math::FSelect(r - l, r, l);
                top = math::FSelect(b - t, t, b);
                bottom = math::FSelect(b - t, b, t);
            }

            /* 0x00 */ f32 left;
            /* 0x04 */ f32 top;
            /* 0x08 */ f32 right;
            /* 0x0C */ f32 bottom;
        };  // size = 0x10

    };  // namespace ut
};  // namespace nw4hbm

#endif
