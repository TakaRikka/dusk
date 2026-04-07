#pragma once

#include <memory>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace dusk {
class ImGuiEngine {
public:
    static ImFont* fontNormal;
    static ImFont* fontLarge;
    static ImTextureID duskIcon;
};

void ImGuiEngine_Initialize(float scale);
void ImGuiEngine_AddTextures();

struct Icon {
    std::unique_ptr<uint8_t[]> data;
    size_t size;
    uint32_t width;
    uint32_t height;
};
Icon GetIcon();
}  // namespace dusk
