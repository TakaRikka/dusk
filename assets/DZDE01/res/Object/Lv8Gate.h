#ifndef RES_LV8GATE_H
#define RES_LV8GATE_H

enum dRes_INDEX_LV8GATE {
    /* BCK  */
    dRes_INDEX_LV8GATE_BCK_P_L8WHOLE_e=0x5,
    /* BMDR */
    dRes_INDEX_LV8GATE_BMD_P_L8WHOLE_e=0x8,
    /* EVT  */
    dRes_INDEX_LV8GATE_DAT_EVENT_LIST_e=0xB,
};

enum dRes_ID_LV8GATE {
    /* BCK  */
    dRes_ID_LV8GATE_BCK_P_L8WHOLE_e=0x5,
    /* BMDR */
    dRes_ID_LV8GATE_BMD_P_L8WHOLE_e=0x8,
    /* EVT  */
    dRes_ID_LV8GATE_DAT_EVENT_LIST_e=0xB,
};

enum P_L8WHOLE_JNT {
    P_L8WHOLE_JNT_P_L8WHOLE_e=0x0,
    P_L8WHOLE_JNT_BACE_e=0x1,
    P_L8WHOLE_JNT_CIRCLE_01_e=0x2,
    P_L8WHOLE_JNT_CIRCLE_02_e=0x3,
};

#endif /* !RES_LV8GATE_H */