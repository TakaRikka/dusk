#define PROCS_DUMP_NAMES 1

#include <cstdio>

#include "d/d_procname.h"
#include "f_pc/f_pc_create_iter.h"
#include "f_pc/f_pc_create_req.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_layer_iter.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_node.h"
#include "imgui.h"
#include "imgui.hpp"
#include "imgui_internal.h"

static const char* getProcName(s16 id) {
    for (auto procName : procNames) {
        if (procName.id == id) {
            return procName.name;
        }
    }

    return nullptr;
}

bool showTreeRecursive;

static int ShowProcess(void* p, void*) {
    auto proc = static_cast<base_process_class*>(p);

    char buf[64];
    snprintf(buf, sizeof(buf), "%d", proc->id);

    ImVec2 avail = ImGui::GetContentRegionAvail();

    ImVec2 vec = {avail.x, 0};
    if (ImGui::BeginChild(buf, vec, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
        ImGui::Text("[%d] %s", proc->id, getProcName(proc->profname));
        ImGui::Text("init_state: %d, create_phase: %d", proc->state.init_state, proc->state.create_phase);

        const char* ofTypeName = "unknown";
        if (proc->subtype == g_fpcNd_type) {
            ofTypeName = "Node";
        } else if (proc->subtype == g_fpcLf_type) {
            ofTypeName = "Leaf";
        }

        ImGui::Text("OfType: %d (%s), layer: %d", proc->subtype, ofTypeName, proc->layer_tag.layer->layer_id);

        if (proc->create_req != nullptr) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Pending create request");
        }

        if (showTreeRecursive) {
            if (fpcBs_Is_JustOfType(g_fpcNd_type, proc->subtype)) {
                auto procNode = static_cast<process_node_class*>(p);

                ImGui::Text("Owns layer %d", procNode->layer.layer_id);

                fpcLyIt_OnlyHere(&procNode->layer, ShowProcess, nullptr);
            }
        }
    }

    ImGui::EndChild();
    return 1;
}

static int ShowCreateRequest(void* p, void*) {
    create_request* req = (create_request*)p;

    if (req->process != nullptr) {
        ShowProcess(req->process, nullptr);
    }

    return 1;
}

static bool Visible = false;

void DuskImguiProcesses() {
    if (ImGui::BeginMenu(MenuView)) {
        ImGui::MenuItem("Process management", "F2", &Visible);
        ImGui::EndMenu();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_F2)) {
        Visible = !Visible;
    }

    if (!Visible) {
        return;
    }

    if (ImGui::Begin("Processes")) {
        if (ImGui::BeginTabBar("Tabs")) {
            showTreeRecursive = true;
            if (ImGui::BeginTabItem("Tree")) {
                fpcLyIt_OnlyHere(fpcLy_RootLayer(), ShowProcess, nullptr);
                ImGui::EndTabItem();
            }

            showTreeRecursive = false;
            if (ImGui::BeginTabItem("All layers")) {
                fpcLyIt_All(ShowProcess, nullptr);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Creation queue")) {
                fpcCtIt_Method(ShowCreateRequest, nullptr);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}
