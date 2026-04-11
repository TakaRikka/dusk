#include "ImGuiEngine.hpp"

#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <aurora/imgui.h>
#include <cmath>
#include <cstring>
#include <fmt/format.h>
#include <string>

#include "dusk/logging.h"

#ifdef IMGUI_ENABLE_FREETYPE
#include "misc/freetype/imgui_freetype.h"
#endif

namespace dusk {
namespace {
std::string GetAssetPath(const char* assetName) {
    const char* basePath = SDL_GetBasePath();
    if (basePath != nullptr && basePath[0] != '\0') {
        return std::string(basePath) + "res/" + assetName;
    }
    return std::string("res/") + assetName;
}

bool AssetExists(const std::string& path) {
    SDL_PathInfo pathInfo{};
    return SDL_GetPathInfo(path.c_str(), &pathInfo) && pathInfo.type == SDL_PATHTYPE_FILE;
}
}  // namespace

ImFont* ImGuiEngine::fontNormal;
ImFont* ImGuiEngine::fontLarge;
ImFont* ImGuiEngine::fontExtraLarge;
ImFont* ImGuiEngine::fontMono;
ImTextureID ImGuiEngine::duskIcon = 0;

inline ImFont* CreateFont(float size, const std::string& fontPath, std::string_view fontName) {
    bool fontFileExists = !fontPath.empty() && AssetExists(fontPath);
    ImFontConfig fontConfig{};
    fontConfig.SizePixels = size;
    auto name = fmt::format_to_n(fontConfig.Name, sizeof(fontConfig.Name) - 1, "{}, {}px", fontName,
                                 static_cast<int>(fontConfig.SizePixels));
    *name.out = '\0';
    const ImGuiIO& io = ImGui::GetIO();
    ImFont* outFont =
        fontFileExists ?
            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontConfig.SizePixels, &fontConfig) :
            nullptr;
    if (outFont == nullptr) {
        if (fontFileExists) {
            DuskLog.warn("Failed to load font '{}': {}", fontPath, SDL_GetError());
        }
        outFont = io.Fonts->AddFontDefault(&fontConfig);
    }
    return outFont;
}

void ImGuiEngine_Initialize(float scale) {
    // Round font scale to nearest integer
    scale = std::ceilf(scale);

    ImGui::GetCurrentContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.FontGlobalScale = scale > 0.0f ? 1.0f / scale : 1.0f;

    ImGuiEngine::fontNormal =
        CreateFont(std::floor(18.f * scale), GetAssetPath("Inter-Regular.ttf"), "Inter Regular");
    ImGuiEngine::fontLarge =
        CreateFont(std::floor(26.f * scale), GetAssetPath("Inter-Regular.ttf"), "Inter Regular");
    ImGuiEngine::fontExtraLarge =
        CreateFont(std::floor(40.f * scale), GetAssetPath("Inter-Bold.ttf"), "Inter Bold");
    ImGuiEngine::fontMono =
        CreateFont(std::floor(16.f * scale), GetAssetPath("NotoMono-Regular.ttf"),
                   "Noto Mono Regular");

    auto& style = ImGui::GetStyle();
    style = {};  // Reset sizes
    style.WindowPadding = ImVec2(15, 15);
    style.WindowRounding = 5.0f;
    style.FrameBorderSize = 1.f;
    style.FramePadding = ImVec2(8, 5);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 5.0f;
    style.PopupBorderSize = 1.f;
    style.PopupRounding = 7.0;
    style.TabBorderSize = 1.f;
    style.TabRounding = 3.f;

    auto* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

Image GetImage(const std::string& path) {
    if (!AssetExists(path)) {
        return {};
    }

    SDL_Surface* loadedSurface = SDL_LoadPNG(path.c_str());
    if (loadedSurface == nullptr) {
        DuskLog.warn("Failed to load image '{}': {}", path, SDL_GetError());
        return {};
    }

    SDL_Surface* rgbaSurface = SDL_ConvertSurface(loadedSurface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loadedSurface);
    if (rgbaSurface == nullptr) {
        DuskLog.warn("Failed to convert image '{}': {}", path, SDL_GetError());
        return {};
    }

    const auto iconWidth = static_cast<uint32_t>(rgbaSurface->w);
    const auto iconHeight = static_cast<uint32_t>(rgbaSurface->h);
    const size_t rowSize = static_cast<size_t>(iconWidth) * 4;
    const size_t size = rowSize * static_cast<size_t>(iconHeight);
    auto ptr = std::make_unique<uint8_t[]>(size);
    for (uint32_t row = 0; row < iconHeight; ++row) {
        const auto* src = static_cast<const uint8_t*>(rgbaSurface->pixels) +
                          static_cast<size_t>(row) * static_cast<size_t>(rgbaSurface->pitch);
        auto* dst = ptr.get() + static_cast<size_t>(row) * rowSize;
        std::memcpy(dst, src, rowSize);
    }

    SDL_DestroySurface(rgbaSurface);
    return Image{
        std::move(ptr),
        size,
        iconWidth,
        iconHeight,
    };
}

void ImGuiEngine_AddTextures() {
    if (ImGuiEngine::duskIcon == 0) {
        auto icon = GetImage(GetAssetPath("icon.png"));
        if (icon.data == nullptr || icon.width == 0 || icon.height == 0) {
            ImGuiEngine::duskIcon = 0;
            return;
        }
        ImGuiEngine::duskIcon = aurora_imgui_add_texture(icon.width, icon.height, icon.data.get());
    }
}
}  // namespace dusk
