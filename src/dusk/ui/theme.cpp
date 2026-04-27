#include "theme.hpp"

#include <algorithm>
#include <fmt/format.h>

namespace dusk::ui::theme {

std::string rgba(Color color, int opacity) {
    if (opacity >= 0) {
        color.a = std::clamp(opacity, 0, 255);
    }
    return fmt::format("rgba({}, {}, {}, {})", color.r, color.g, color.b, color.a);
}

std::string dp(float value) {
    return fmt::format("{}dp", value);
}

}  // namespace dusk::ui::theme
