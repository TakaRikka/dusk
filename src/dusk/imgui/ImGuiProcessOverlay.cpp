#define PROCS_DUMP_NAMES 1

#include <cstdio>

#include "f_pc/f_pc_name.h"
#include "f_pc/f_pc_create_iter.h"
#include "f_pc/f_pc_create_req.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_layer_iter.h"
#include "f_pc/f_pc_leaf.h"
#include "f_pc/f_pc_node.h"
#include "d/d_debug_viewer.h"
#include "imgui.h"
#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "imgui_internal.h"

namespace dusk {
    static bool BeginProcTable() {
        static ImGuiTableFlags table_flags =
            ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable |
            ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("proc_table", 7)) {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 128);
            ImGui::TableSetupColumn("En", ImGuiTableColumnFlags_WidthFixed, 32);
            ImGui::TableSetupColumn("Vs", ImGuiTableColumnFlags_WidthFixed, 32);
            ImGui::TableSetupColumn("ProcName");
            ImGui::TableSetupColumn("Param", ImGuiTableColumnFlags_WidthFixed, 128);
            ImGui::TableSetupColumn("Pi", ImGuiTableColumnFlags_WidthFixed, 64);
            ImGui::TableSetupColumn("DwPi", ImGuiTableColumnFlags_WidthFixed, 64);
            ImGui::TableHeadersRow();
            return true;
        }
        return false;
    }

    static int ShowProcess(void* p, void*) {
        auto proc = static_cast<base_process_class*>(p);

        ImGui::TableNextRow();
        ImGui::PushID(proc);
        bool pending = proc->create_req != nullptr;
        if (pending)
            ImGui::PushStyleColor(ImGuiCol_Text, {255, 200, 0, 255});
        ImGui::TableNextColumn();

        char id_buf[32];
        sprintf(id_buf, "%d", proc->id);

        int flags = ImGuiTreeNodeFlags_SpanAllColumns;
        bool isLayer = fpcBs_Is_JustOfType(g_fpcNd_type, proc->subtype);
        if (isLayer) {
            flags |= ImGuiTreeNodeFlags_DefaultOpen;
        } else {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        bool open = ImGui::TreeNodeEx(id_buf, flags);
        fopAc_ac_c* ac = fopAcM_IsActor(proc) ? (fopAc_ac_c*)proc : nullptr;
        if (ac != nullptr && ImGui::IsItemHovered()) {
            fopAcM_DrawCullingBox(ac, {0, 255, 255, 255});
        }

        ImGui::TableNextColumn();
        bool enable = !fpcM_IsPause(proc, 1);
        if (ImGui::Checkbox("##enable", &enable)) {
            if (enable)
                fpcM_PauseDisable(proc, 1);
            else
                fpcM_PauseEnable(proc, 1);
        }

        ImGui::TableNextColumn();
        if (fpcBs_Is_JustOfType(g_fpcLf_type, proc->subtype)) {
            leafdraw_class* lf = (leafdraw_class*)proc;
            bool vis = lf->unk_0xBC == 0;
            if (ImGui::Checkbox("##visible", &vis)) {
                if (vis)
                    lf->unk_0xBC = 0;
                else
                    lf->unk_0xBC = 1;
            }
        }

        ImGui::TableNextColumn();
        ImGui::Text("%s", ac != nullptr ? fopAcM_getProcNameString(ac) : GetProcName(proc->profname));

        ImGui::TableNextColumn();
        if (proc->profname == fpcNm_ROOM_SCENE_e) {
            ImGui::Text("Room %d", proc->parameters);
        } else {
            ImGui::Text("%08x", proc->parameters);
        }

        ImGui::TableNextColumn();
        ImGui::Text("%d", proc->priority.current_info.list_id);

        ImGui::TableNextColumn();
        if (fpcBs_Is_JustOfType(g_fpcLf_type, proc->subtype)) {
            ImGui::Text("%d", fpcM_DrawPriority(proc));
        } else {
            ImGui::Text("--");
        }

        if (isLayer && open) {
            auto procNode = static_cast<process_node_class*>(p);
            fpcLyIt_OnlyHere(&procNode->layer, ShowProcess, nullptr);
            ImGui::TreePop();
        }

        if (pending)
            ImGui::PopStyleColor(1);
        ImGui::PopID();
        return 1;
    }

    static int ShowCreateRequest(void* p, void*) {
        create_request* req = (create_request*)p;

        if (req->process != nullptr) {
            ShowProcess(req->process, nullptr);
        }

        return 1;
    }

    void ImGuiMenuTools::ShowProcessManager() {
        if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F2, m_showProcessManagement)) {
            return;
        }

        if (ImGui::Begin("Processes", &m_showProcessManagement)) {
            if (ImGui::BeginTabBar("Tabs")) {
                if (ImGui::BeginTabItem("Tree")) {
                    if (BeginProcTable()) {
                        fpcLyIt_OnlyHere(fpcLy_RootLayer(), ShowProcess, nullptr);
                        ImGui::EndTable();
                    }
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
}