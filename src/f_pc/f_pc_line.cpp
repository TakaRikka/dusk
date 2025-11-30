/**
 * f_pc_line.cpp
 * Framework - Process Line
 */

#include "f_pc/f_pc_line.h"
#include "f_pc/f_pc_node.h"
#include "global.h"

static node_list_class l_fpcLn_Line[16];

void fpcLn_Create() {
    s32 i = ARRAY_SIZEU(l_fpcLn_Line);
    node_list_class* line_node = l_fpcLn_Line;
    while (i-- > 0)
        cLs_Create(line_node++);
}

node_lists_tree_class g_fpcLn_Queue = {
    l_fpcLn_Line,
    ARRAY_SIZEU(l_fpcLn_Line),
};
