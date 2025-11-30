/**
 * f_op_scene_pause.cpp
 * Scene Process Pause
 */

#include "f_op/f_op_scene_pause.h"
#include "f_op/f_op_scene.h"
#include "f_pc/f_pc_manager.h"

int fopScnPause_Enable(scene_class* i_scene) {
    if (i_scene != NULL) {
        fpcM_PauseEnable(i_scene, 1);
        fpcM_PauseEnable(i_scene, 2);
        return 1;
    }

    return 0;
}

int fopScnPause_Disable(scene_class* i_scene) {
    if (i_scene != NULL) {
        process_node_class* process_node = i_scene->base.base.layer_tag.layer->process_node;

        if (process_node == NULL) {
            fpcM_PauseDisable(i_scene, 1);
            fpcM_PauseDisable(i_scene, 2);
        } else if (fpcEx_IsExist(process_node->base.id) == TRUE) {
            if (!fpcM_IsPause(process_node, 1)) {
                fpcM_PauseDisable(i_scene, 1);
            }

            if (!fpcM_IsPause(process_node, 2)) {
                fpcM_PauseDisable(i_scene, 2);
            }
        }

        return 1;
    }

    return 0;
}
