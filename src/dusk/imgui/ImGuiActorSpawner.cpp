#include "imgui.h"

#include "ImGuiMenuTools.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "SSystem/SComponent/c_sxyz.h"
#include "SSystem/SComponent/c_xyz.h"

namespace dusk {
namespace {

struct ActorSpawnerState {
    int actorId    = 0;
    int params     = -1;
    int argument   = -1;
    int angleX     = 0;
    int angleY     = 0;
    int angleZ     = 0;
    float scaleX   = 1.0f;
    float scaleY   = 1.0f;
    float scaleZ   = 1.0f;
    bool usePlayerRoom = true;
    int manualRoom = 0;
    int spawnCount = 1;
    bool hasResult = false;
    unsigned int lastResult = 0;
    int lastAttempted = 0;
};

ActorSpawnerState s_state;

}  // namespace

void ImGuiMenuTools::ShowActorSpawner() {
    if (!m_showActorSpawner) {
        return;
    }

    if (!ImGui::Begin("Actor Spawner", &m_showActorSpawner)) {
        ImGui::End();
        return;
    }

    daAlink_c* player = (daAlink_c*)dComIfGp_getPlayer(0);

    ImGui::SeparatorText("Actor");
    ImGui::InputInt("Actor ID", &s_state.actorId);
    ImGui::InputInt("Params (hex)", &s_state.params, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
    ImGui::InputInt("Argument", &s_state.argument);
    s_state.argument = (s_state.argument < -128) ? -128 : (s_state.argument > 127) ? 127 : s_state.argument;

    ImGui::SeparatorText("Angle");
    ImGui::InputInt("Angle X", &s_state.angleX);
    ImGui::InputInt("Angle Y", &s_state.angleY);
    ImGui::InputInt("Angle Z", &s_state.angleZ);

    ImGui::SeparatorText("Scale");
    ImGui::InputFloat("Scale X", &s_state.scaleX, 0.1f, 1.0f);
    ImGui::InputFloat("Scale Y", &s_state.scaleY, 0.1f, 1.0f);
    ImGui::InputFloat("Scale Z", &s_state.scaleZ, 0.1f, 1.0f);

    ImGui::SeparatorText("Spawn");
    ImGui::InputInt("Count", &s_state.spawnCount);
    if (s_state.spawnCount < 1) {
        s_state.spawnCount = 1;
    }

    ImGui::SeparatorText("Position");
    ImGui::Checkbox("Use player room", &s_state.usePlayerRoom);
    if (!s_state.usePlayerRoom) {
        ImGui::InputInt("Room No", &s_state.manualRoom);
    }

    if (player != nullptr) {
        ImGui::Text("Spawn pos: %.2f, %.2f, %.2f",
            player->current.pos.x, player->current.pos.y, player->current.pos.z);
    } else {
        ImGui::TextDisabled("Player not available");
    }

    ImGui::Separator();

    bool canSpawn = player != nullptr;
    if (!canSpawn) {
        ImGui::BeginDisabled();
    }

    if (ImGui::Button("Spawn", ImVec2(-1, 0))) {
        cXyz pos = player->current.pos;
        csXyz angle;
        angle.set((s16)s_state.angleX, (s16)s_state.angleY, (s16)s_state.angleZ);
        cXyz scale(s_state.scaleX, s_state.scaleY, s_state.scaleZ);
        int roomNo = s_state.usePlayerRoom ? player->current.roomNo : s_state.manualRoom;

        layer_class* savedLayer = fpcLy_CurrentLayer();
        base_process_class* playScene = fpcM_SearchByName(fpcNm_PLAY_SCENE_e);
        if (playScene != nullptr) {
            fpcLy_SetCurrentLayer(&((process_node_class*)playScene)->layer);
        }

        s_state.lastResult = 0;
        s_state.lastAttempted = s_state.spawnCount;
        for (int i = 0; i < s_state.spawnCount; ++i) {
            unsigned int result = fopAcM_create(
                (s16)s_state.actorId,
                (u32)s_state.params,
                &pos,
                roomNo,
                &angle,
                &scale,
                (s8)s_state.argument
            );
            if (result != 0) {
                s_state.lastResult = result;
            }
        }
        s_state.hasResult = true;

        fpcLy_SetCurrentLayer(savedLayer);
    }

    if (!canSpawn) {
        ImGui::EndDisabled();
    }

    if (s_state.hasResult) {
        if (s_state.lastResult != 0) {
            if (s_state.lastAttempted == 1) {
                ImGui::Text("Spawned: proc ID %u", s_state.lastResult);
            } else {
                ImGui::Text("Spawned %d (last proc ID %u)", s_state.lastAttempted, s_state.lastResult);
            }
        } else {
            ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Spawn failed (returned 0)");
        }
    }

    ImGui::End();
}

}  // namespace dusk
