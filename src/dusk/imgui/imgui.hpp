#ifndef DUSK_IMGUI_HPP
#define DUSK_IMGUI_HPP

#include <aurora/aurora.h>
#include <string>

inline const char* MenuView = "View";

void DuskImguiDebugOverlay(const AuroraInfo *info);
void DuskImguiProcesses();
void DuskImguiHeaps();
void DuskCameraDebug();
void DuskDebugPad();

void SetOverlayWindowLocation(int corner);
bool ShowCornerContextMenu(int& corner, int avoidCorner);

std::string BytesToString(size_t bytes);

#endif  // DUSK_IMGUI_HPP
