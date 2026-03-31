#include <array>
#include <optional>

#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "JSystem/JFramework/JFWSystem.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "absl/container/flat_hash_map.h"
#include "imgui.h"

struct OpenHeapData {
    bool Safe;
    bool HeapCheckRan;
    bool HeapCheckFailed;
};

static absl::flat_hash_map<JKRHeap*, OpenHeapData> OpenHeapWindows;

namespace dusk {
    static void DrawTableCore();
    void ShowHeapDetailed(JKRHeap* heap, OpenHeapData& data, bool& open);

    void ImGuiMenuTools::ShowHeapOverlay() {
        if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F4, m_showHeapOverlay)) {
            return;
        }

        if (ImGui::Begin("Heaps", &m_showHeapOverlay)) {
            for (auto& x : OpenHeapWindows) {
                x.second.Safe = false;
            }

            if (ImGui::BeginTable(
                "heaps",
                6,
                ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {

                DrawTableCore();

                ImGui::EndTable();
            }
        }

        ImGui::End();

        std::vector<JKRHeap*> closeQueue;
        for (auto& [heap, safe] : OpenHeapWindows) {
            auto open = true;
            ShowHeapDetailed(heap, safe, open);
            if (!open) {
                closeQueue.push_back(heap);
            }
        }

        for (auto toRemove : closeQueue) {
            OpenHeapWindows.erase(toRemove);
        }
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
        ImGui::TableNextColumn();

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

    static std::optional<const char*> GetHeapName(const JKRHeap* heap) {
        const auto name = heap->getName();
        if (strlen(name) == 0) {
            return std::nullopt;
        }

        return name;
    }

    static void DrawHeap(JKRHeap* heap, const int depth) {
        ImGui::TableNextRow();
        char idBuf[32];
        snprintf(idBuf, sizeof(idBuf), "%p", heap);
        ImGui::PushID(idBuf);
        ImGui::TableNextColumn();

        if (OpenHeapWindows.find(heap) != OpenHeapWindows.end()) {
            OpenHeapWindows[heap].Safe = true;
        }

        auto indentSize = depth * 16;
        if (indentSize != 0)
            ImGui::Indent(indentSize);
        auto heapName = GetHeapName(heap);
        if (heapName.has_value()) {
            ImGui::TextUnformatted(heapName.value());
        } else {
            char unkNameBuf[32];
            snprintf(unkNameBuf, sizeof(unkNameBuf), "Unknown (%p)", heap);
            ImGui::TextUnformatted(unkNameBuf);
        }
        if (indentSize != 0)
            ImGui::Unindent(indentSize);

        ImGui::TableNextColumn();
        ImGui::ProgressBar(
            heap->getSize() > 0 ? 1 - (f32)heap->getFreeSize() / (f32)heap->getSize() : 0.0f,
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

        ImGui::TableNextColumn();
        if (ImGui::Button("View")) {
            OpenHeapWindows[heap].Safe = true;
        }

        const JSUTree<JKRHeap>& tree = heap->getHeapTree();
        for (JSUTreeIterator iter(tree.getFirstChild()); iter != tree.getEndChild(); ++iter) {
            DrawHeap(*iter, depth + 1);
        }

        ImGui::PopID();
    }

    struct MemBlockPair {
        JKRExpHeap::CMemBlock* block;
        bool used;

        auto& operator->() {
            return block;
        }
    };

    static std::vector<MemBlockPair> FindAllHeapBlocks(JKRExpHeap* heap) {
        std::vector<MemBlockPair> result;

        for (JKRExpHeap::CMemBlock* b = heap->getFreeHead(); b; b = b->getNextBlock()) {
            result.push_back({b, false});
        }

        for (JKRExpHeap::CMemBlock* b = heap->getUsedHead(); b; b = b->getNextBlock()) {
            result.push_back({b, true});
        }

        std::ranges::sort(result, [](auto a, auto b) { return a.block < b.block; });

        return result;
    }

    static void ShowHeapDetailed(JKRHeap* heap, OpenHeapData& data, bool& open) {
        char title[128];
        const char* name = data.Safe ? heap->getName() : "INVALID";
        snprintf(title, sizeof(title), "Heap %s##%p", heap->getName(), static_cast<const void*>(heap));

        if (!ImGui::Begin(name, &open)) {
            ImGui::End();
            return;
        }

        if (!data.Safe) {
            ImGui::TextUnformatted("Heap no longer exists");
            ImGui::End();
            return;
        }

        heap->lock();

        ImGui::Text("Name: %s", heap->getName());
        const auto size = BytesToString(heap->getSize());
        const auto freeSize = BytesToString(heap->getFreeSize());
        ImGui::Text("Size: %08X (%s), free: %08X (%s)", heap->getSize(), size.c_str(), heap->getFreeSize(), freeSize.c_str());

        if (ImGui::Button("Check")) {
            data.HeapCheckFailed = !heap->check();
            data.HeapCheckRan = true;
        }

        if (data.HeapCheckFailed) {
            ImGui::SameLine();
            ImColor red = IM_COL32(0xFF, 0, 0, 0xFF);
            ImGui::TextColored(red, "Heap check failed");
        } else if (data.HeapCheckRan) {
            ImGui::SameLine();
            ImColor red = IM_COL32(0, 0xFF, 0, 0xFF);
            ImGui::TextColored(red, "Heap check passed");
        }

        if (heap->getHeapType() == 'EXPH') {
            auto expHeap = dynamic_cast<JKRExpHeap*>(heap);

            ImGui::SeparatorText("Blocks");

            if (ImGui::BeginTable("Blocks", 5, ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
                ImGui::TableSetupColumn("Start", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("End", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Size");
                ImGui::TableSetupColumn("Allocated", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("Allocated").x);
                ImGui::TableSetupColumn("IsValid", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("IsValid").x);
                ImGui::TableHeadersRow();

                const auto blocks = FindAllHeapBlocks(expHeap);

                for (auto block : blocks) {
                    assert(block->getSize() != 0);
                    ImGui::TableNextRow();

                    if (block.used) {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, IM_COL32(0xFF, 0, 0, 0x44));
                    } else {
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, IM_COL32(0, 0xFF, 0, 0x44));
                    }

                    char bufId[32];
                    snprintf(bufId, sizeof(bufId), "%p", block);
                    ImGui::PushID(bufId);
                    ImGui::TableNextColumn();
                    ImGui::Text("%08X", (u32)((uintptr_t)block.block - (uintptr_t)expHeap->getStartAddr()));
                    ImGui::TableNextColumn();
                    ImGui::Text("%08X", (u32)((uintptr_t)block.block + block->getSize() - (uintptr_t)expHeap->getStartAddr()));
                    ImGui::TableNextColumn();
                    auto sizeNice = BytesToString(block->getSize());
                    ImGui::Text("%08X (%s)", block->getSize(), sizeNice.c_str());
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(block.used ? "True" : "False");
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(block->isValid() ? "True" : "False");
                    if (block->isValid() != block.used) {
                        ImGui::SameLine();
                        ImGui::TextUnformatted("(!!!)");
                    }
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }
        }

        ImGui::End();

        heap->unlock();
    }
}
