#include <array>

#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "imgui.h"
#include "imgui.hpp"

static bool Active = false;

static void DrawTableCore();

void DuskImguiHeaps() {
    if (ImGui::BeginMenu(MenuView)) {
        ImGui::MenuItem("Heaps", "F4", &Active);

        ImGui::EndMenu();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_F4)) {
        Active = !Active;
    }

    if (!Active) {
        return;
    }

    if (ImGui::Begin("Heaps", &Active)) {
        if (ImGui::BeginTable(
            "heaps",
            5,
            ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {

            DrawTableCore();

            ImGui::EndTable();
        }
    }

    ImGui::End();
}

static void DrawHeap(JKRHeap* heap, int depth = 0);

static void DrawTableCore() {
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Heap name");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Use%");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Available");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Total");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted("Type");

    DrawHeap(reinterpret_cast<JKRHeap*>(JFWSystem::rootHeap));
}

static std::array<char, 4> GetHeapType(JKRHeap* heap) {
    auto type = heap->getHeapType();

    return {
        (char)(type >> 24 & 0xFF),
        (char)(type >> 16 & 0xFF),
        (char)(type >> 8 & 0xFF),
        (char)(type >> 0 & 0xFF),
    };
}

static const char* GetHeapName(const JKRHeap* heap) {
    const auto name = heap->getName();
    if (strlen(name) == 0) {
        return "Unknown";
    }

    return name;
}

static void DrawHeap(JKRHeap* heap, const int depth) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    auto indentSize = depth * 16;
    if (indentSize != 0)
        ImGui::Indent(indentSize);
    ImGui::TextUnformatted(GetHeapName(heap));
    if (indentSize != 0)
        ImGui::Unindent(indentSize);

    ImGui::TableNextColumn();
    ImGui::ProgressBar(
        1 - (f32)heap->getFreeSize() / (f32)heap->getSize(),
        ImVec2(ImGui::GetContentRegionAvail().x, 0));

    ImGui::TableNextColumn();
    auto freeSizeString = BytesToString(heap->getFreeSize());
    ImGui::TextUnformatted(freeSizeString.c_str());

    ImGui::TableNextColumn();
    auto totalSizeString = BytesToString(heap->getSize());
    ImGui::TextUnformatted(totalSizeString.c_str());

    ImGui::TableNextColumn();
    auto typeString = GetHeapType(heap);
    ImGui::TextUnformatted(typeString.data(), typeString.data() + 4);

    const JSUTree<JKRHeap>& tree = heap->getHeapTree();
    for (JSUTreeIterator iter(tree.getFirstChild()); iter != tree.getEndChild(); ++iter) {
        DrawHeap(*iter, depth + 1);
    }
}