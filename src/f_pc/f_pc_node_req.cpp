/**
 * f_pc_node_req.cpp
 * Framework - Process Node Request
 */

#include "f_pc/f_pc_node_req.h"
#include "SSystem/SComponent/c_malloc.h"
#include "SSystem/SStandard/s_basic.h"
#include "f_pc/f_pc_deletor.h"
#include "f_pc/f_pc_executor.h"
#include "f_pc/f_pc_node.h"
#include "f_pc/f_pc_stdcreate_req.h"
#include "f_pc/f_pc_manager.h"
#include "f_pc/f_pc_debug_sv.h"

void fpcNdRq_RequestQTo(node_create_request* i_request) {
    fpcLy_CreatedMesg(i_request->layer);
    fpcLy_CancelQTo(&i_request->method_tag);
    cTg_SingleCut(&i_request->create_tag);
}

static node_list_class l_fpcNdRq_Queue = {NULL, NULL, 0};

void fpcNdRq_ToRequestQ(node_create_request* i_request) {
    cTg_Addition(&l_fpcNdRq_Queue, &i_request->create_tag);
    fpcLy_ToCancelQ(i_request->layer, &i_request->method_tag);
    fpcLy_CreatingMesg(i_request->layer);
}

s32 fpcNdRq_phase_IsCreated(node_create_request* i_request) {
    if (fpcCtRq_IsCreatingByID(i_request->creating_id) == TRUE) {
#if DEBUG
        if (i_request->unk_0x64-- <= 0) {
            i_request->unk_0x64 = 0;
            if (g_fpcDbSv_service[8] != NULL) {
                g_fpcDbSv_service[8](&i_request->creating_id);
            }
        }
#endif

        return cPhs_INIT_e;
    } else if (fpcEx_IsExist(i_request->creating_id) == TRUE) {
        return cPhs_NEXT_e;
    } else {
        return cPhs_UNK3_e;
    }
}

s32 fpcNdRq_phase_Create(node_create_request* i_request) {
    i_request->creating_id =
        fpcSCtRq_Request(i_request->layer, i_request->name,
                         (stdCreateFunc)i_request->create_req_methods->post_method, i_request,
                         i_request->data);
    if (i_request->creating_id == fpcM_ERROR_PROCESS_ID_e) {
        return cPhs_UNK3_e;
    }

    return cPhs_NEXT_e;
}

s32 fpcNdRq_phase_IsDeleteTiming(node_create_request* i_request) {
    return cPhs_NEXT_e;
}

s32 fpcNdRq_phase_IsDeleted(node_create_request* i_request) {
    if (fpcDt_IsComplete() == FALSE) {
#if DEBUG
        if (i_request->unk_0x68-- <= 0) {
            i_request->unk_0x68 = 0;
            if (g_fpcDbSv_service[7] != NULL) {
                g_fpcDbSv_service[7](i_request->node_proc.node);
            }
        }
#endif
        return cPhs_INIT_e;
    }

    return cPhs_NEXT_e;
}

s32 fpcNdRq_phase_Delete(node_create_request* i_request) {
    if (i_request->node_proc.node != NULL) {
        if (fpcDt_Delete(&i_request->node_proc.node->base) == 0) {
            return cPhs_INIT_e;
        }
        i_request->node_proc.node = NULL;
    }

    return cPhs_NEXT_e;
}

s32 fpcNdRq_DoPhase(node_create_request* i_request) {
    s32 result = cPhs_Handler(&i_request->phase_request, i_request->phase_handler, i_request);
    if (result == cPhs_NEXT_e) {
        return fpcNdRq_DoPhase(i_request);
    }
    return result;
}

s32 fpcNdRq_Execute(node_create_request* i_request) {
    s32 result = fpcNdRq_DoPhase(i_request);
    switch (result) {
    case cPhs_INIT_e:
    case cPhs_LOADING_e:
        return cPhs_INIT_e;
    case cPhs_COMPLEATE_e:
        return cPhs_NEXT_e;
    case cPhs_ERROR_e:
    case cPhs_UNK3_e:
        return cPhs_UNK3_e;
    default:
        return result;
    }
}

s32 fpcNdRq_Delete(node_create_request* i_request) {
    fpcNdRq_RequestQTo(i_request);
    if (i_request->create_req_methods != NULL && i_request->create_req_methods->delete_method != NULL &&
        fpcMtd_Method(i_request->create_req_methods->delete_method, i_request) == 0)
    {
        return 0;
    }

    cMl::free(i_request);
    return 1;
}

s32 fpcNdRq_Cancel(node_create_request* i_request) {
    if (i_request->create_req_methods != NULL &&
        fpcMtd_Method(i_request->create_req_methods->cancel_method, i_request) == 0)
    {
        return 0;
    }

    return fpcNdRq_Delete(i_request);
}

#define NODE_GET_NEXT(pNode) (pNode ? pNode->mpNextNode : NULL)

s32 fpcNdRq_Handler() {
    node_class* node = l_fpcNdRq_Queue.mpHead;

#if DEBUG
    if (g_fpcDbSv_service[9] != NULL) {
        g_fpcDbSv_service[9](&l_fpcNdRq_Queue.mSize);
    }
#endif

    while (node != NULL) {
        node_create_request* req = ((request_node_class*)node)->node_create_req;
        switch (req->create_req_methods->execute_method(req)) {
        case cPhs_UNK3_e:
        case cPhs_ERROR_e:
            node = NODE_GET_NEXT(node);
            if (fpcNdRq_Cancel(req) == 0) {
                return 0;
            }
            break;
        case cPhs_COMPLEATE_e:
            node = NODE_GET_NEXT(node);
            if (fpcNdRq_Delete(req) == 0) {
                return 0;
            }
            break;
        default:
            node = NODE_GET_NEXT(node);
            break;
        }
    }
    return 1;
}

s32 fpcNdRq_IsPossibleTarget(process_node_class* i_procNode) {
    fpc_ProcID id = i_procNode->base.id;
    request_node_class* req_node = (request_node_class*)l_fpcNdRq_Queue.mpHead;

    while (req_node != NULL) {
        node_create_request* create_req = req_node->node_create_req;
        if ((create_req->parameters == 2 || create_req->parameters == 4 ||
             create_req->parameters == 1) &&
            create_req->node_proc.id == id)
        {
            return 0;
        }
        req_node = (request_node_class*)NODE_GET_NEXT((&req_node->node));
    }
    return 1;
}

s32 fpcNdRq_IsIng(process_node_class* i_procNode) {
    request_node_class* req_node;
    node_create_request* create_req;
    fpc_ProcID id = i_procNode->base.id;

    req_node = (request_node_class*)l_fpcNdRq_Queue.mpHead;
    while (req_node != NULL) {
        create_req = req_node->node_create_req;
        if (create_req->creating_id == id) {
            return 1;
        }
        req_node = (request_node_class*)NODE_GET_NEXT((&req_node->node));
    }
    return 0;
}

node_create_request* fpcNdRq_Create(u32 i_requestSize) {
    static node_create_request clear = {
        NULL, NULL,       NULL, NULL,       0,      NULL, NULL,   NULL, NULL,
        0,    NULL,       NULL, NULL,       0,      0,    NULL,   5,    0xFFFFFFFF,
        NULL, 0xFFFFFFFE, NULL, 0xFFFFFFFE, 0x7FFF, NULL, 0x7FFF,
    };

    node_create_request* req = (node_create_request*)cMl::memalignB(-4, i_requestSize);
    if (req != NULL) {
        static fpc_ProcID request_id = 0;
        sBs_ClearArea(req, i_requestSize);
        *req = clear;
        cTg_Create(&req->create_tag, req);
        fpcMtdTg_Init(&req->method_tag, (process_method_tag_func)fpcNdRq_Cancel, req);
        req->request_id = request_id++;
#if DEBUG
        req->unk_0x64 = 60;
        req->unk_0x68 = 60;
#endif
    }

    return req;
}

node_create_request* fpcNdRq_ChangeNode(u32 i_requestSize, process_node_class* i_procNode,
                                        s16 i_procName, void* i_data) {
    static cPhs__Handler methods[6] = {
        (cPhs__Handler)fpcNdRq_phase_IsDeleteTiming, (cPhs__Handler)fpcNdRq_phase_Delete,
        (cPhs__Handler)fpcNdRq_phase_IsDeleted,      (cPhs__Handler)fpcNdRq_phase_Create,
        (cPhs__Handler)fpcNdRq_phase_IsCreated,      NULL,
    };

    if (fpcNdRq_IsPossibleTarget(i_procNode) == 1 && fpcNdRq_IsIng(i_procNode) == 0) {
        node_create_request* req = fpcNdRq_Create(i_requestSize);
        if (req != NULL) {
            req->phase_handler = methods;
            req->node_proc.node = i_procNode;
            req->node_proc.id = i_procNode->base.id;
            req->layer = i_procNode->base.layer_tag.layer;
            req->name = i_procName;
            req->data = i_data;
        }
        return req;
    }

    return NULL;
}

node_create_request* fpcNdRq_DeleteNode(u32 i_requestSize, process_node_class* i_procNode) {
    static cPhs__Handler methods[4] = {
        (cPhs__Handler)fpcNdRq_phase_IsDeleteTiming,
        (cPhs__Handler)fpcNdRq_phase_Delete,
        (cPhs__Handler)fpcNdRq_phase_IsDeleted,
        NULL,
    };

    if (fpcNdRq_IsPossibleTarget(i_procNode) == 1 && fpcNdRq_IsIng(i_procNode) == 0) {
        node_create_request* req = fpcNdRq_Create(i_requestSize);
        if (req != NULL) {
            req->phase_handler = methods;
            req->node_proc.node = i_procNode;
            req->node_proc.id = i_procNode->base.id;
            req->layer = i_procNode->base.layer_tag.layer;
        }
        return req;
    }

    return NULL;
}

node_create_request* fpcNdRq_CreateNode(u32 i_requestSize, s16 i_procName, void* i_data) {
    static cPhs__Handler methods[3] = {
        (cPhs__Handler)fpcNdRq_phase_Create,
        (cPhs__Handler)fpcNdRq_phase_IsCreated,
        NULL,
    };

    layer_class* layer = fpcLy_CurrentLayer();
    if (layer->layer_id != fpcLy_ROOT_e && fpcNdRq_IsPossibleTarget(layer->process_node) == 0) {
        int sp28;
        return NULL;
    }

    node_create_request* req = fpcNdRq_Create(i_requestSize);
    if (req != NULL) {
        req->phase_handler = methods;
        if (layer->layer_id != fpcLy_ROOT_e) {
            req->node_proc.node = layer->process_node;
            req->node_proc.id = layer->process_node->base.id;
        }
        req->layer = layer;
        req->name = i_procName;
        req->data = i_data;
    }

    return req;
}

node_create_request* fpcNdRq_Request(u32 i_requestSize, int i_reqType,
                                     process_node_class* i_procNode, s16 i_procName, void* i_data,
                                     node_create_request_method_class* i_create_req_methods) {
    node_create_request* req;
    switch (i_reqType) {
    case 0:
        req = fpcNdRq_CreateNode(i_requestSize, i_procName, i_data);
        break;
    case 1:
        req = fpcNdRq_DeleteNode(i_requestSize, i_procNode);
        break;
    case 2:
        req = fpcNdRq_ChangeNode(i_requestSize, i_procNode, i_procName, i_data);
        break;
    case 4:
        break;
    }

    if (req != NULL) {
        req->parameters = i_reqType;
        req->create_req_methods = i_create_req_methods;
        fpcNdRq_ToRequestQ(req);
    }

    return req;
}

s32 fpcNdRq_ReChangeNode(fpc_ProcID i_requestID, s16 i_procName, void* i_data) {
    request_node_class* req_node;

    req_node = (request_node_class*)l_fpcNdRq_Queue.mpHead;
    while (req_node != NULL) {
        node_create_request* found = req_node->node_create_req;
        if (found->parameters == 2 && found->request_id == i_requestID) {
            if (found->creating_id == -2) {
                found->name = i_procName;
                found->data = i_data;
                return 1;
            }
            return 0;
        }
        req_node = (request_node_class*)NODE_GET_NEXT((&req_node->node));
    }

    return 0;
}

s32 fpcNdRq_ReRequest(fpc_ProcID i_requestID, s16 i_procName, void* i_data) {
    return fpcNdRq_ReChangeNode(i_requestID, i_procName, i_data);
}
