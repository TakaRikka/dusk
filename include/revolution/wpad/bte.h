#ifndef CONTEXT_BTE_H
#define CONTEXT_BTE_H

#include <stdint.h>

// taken from https://github.com/doldecomp/sdk_2009-12-11/blob/main/include/context_bte.h

/* Contains the context of the BTE library that the WPAD library needs to
 * compile.
 *
 * This is not the full context; the other half of the context is in
 * <context_rvl.h>.
 *
 * Most of this code is copyright (C) 2003-2012 Broadcom Corporation under the
 * Apache 2.0 License <http://www.apache.org/licenses/LICENSE-2.0>. The original
 * source can be found at
 * <https://android.googlesource.com/platform/external/bluetooth/bluedroid>;
 * specifically, I used the earliest commit available (late 2012, commit hash
 * 5738f83aeb59361a0a2eda2460113f6dc9194271).
 */

/* License redistribution conditions
 *
 * a. You may obtain a copy of the License at
 *    <http://www.apache.org/licenses/LICENSE-2.0>.
 * b. Some of the code is modified. Comments will be marked with my name
 *    (muff1n) to show what I modified and where.
 * c. See the following comment block, which is copied verbatim from bluedroid
 *    source.
 * d. No NOTICE file is present in the commit that I used.
 */

/******************************************************************************
 *
 *  Copyright (C) 2003-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

// from bluedroid source

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;

typedef int8_t INT8;

typedef unsigned char BOOLEAN;

/* muff1n: It seems like BTE_TRACE_LEVEL_VERBOSE did not exist at the time. Or
 * maybe it was a different logging level. At the very least, one logging level
 * is missing.
 */
#define MAX_TRACE_LEVEL 5

#define BD_ADDR_LEN     6                   /* Device address length */
typedef UINT8 BD_ADDR[BD_ADDR_LEN];         /* Device address */
typedef UINT8 *BD_ADDR_PTR;                 /* Pointer to Device Address */

#define LINK_KEY_LEN    16
typedef UINT8 LINK_KEY[LINK_KEY_LEN];       /* Link Key */

#define DEV_CLASS_LEN   3
typedef UINT8 DEV_CLASS[DEV_CLASS_LEN];     /* Device class */
typedef UINT8 *DEV_CLASS_PTR;               /* Pointer to Device class */

/* muff1n: TODO: bta_dm_pin_cback looks mostly the same except that BD_NAME_LEN
 * is 33 (or it was 32 and BD_NAME_LEN was not decreased by 1 in BCM_STRNCPY_S
 * calls)?
 * confirm this
 *
 * UPDATE: there is a case where it is 248
 */
#define BD_NAME_LEN     248
typedef UINT8 BD_NAME[BD_NAME_LEN];         /* Device name */
typedef UINT8 *BD_NAME_PTR;                 /* Pointer to Device name */

#define BD_FEATURES_LEN 8
typedef UINT8 BD_FEATURES[BD_FEATURES_LEN]; /* LMP features supported by device */

#define BT_DEVICE_TYPE_BREDR   0x01
#define BT_DEVICE_TYPE_BLE     0x02
#define BT_DEVICE_TYPE_DUMO    0x03
typedef UINT8 tBT_DEVICE_TYPE;

/* Maximum UUID size - 16 bytes, and structure to hold any type of UUID. */
#define MAX_UUID_SIZE              16
typedef struct
{
#define LEN_UUID_16     2
#define LEN_UUID_32     4
#define LEN_UUID_128    16

    UINT16          len;

    union
    {
        UINT16      uuid16;
        UINT32      uuid32;
        UINT8       uuid128[MAX_UUID_SIZE];
    } uu;

} tBT_UUID;

/* Status Return Value */
#define BTA_SUCCESS             0           /* Successful operation. */
#define BTA_FAILURE             1           /* Generic failure. */
#define BTA_PENDING             2           /* API cannot be completed right now */
#define BTA_BUSY                3
#define BTA_NO_RESOURCES        4
#define BTA_WRONG_MODE          5

typedef UINT8 tBTA_STATUS;

/*
 * Service ID
 *
 * NOTES: When you add a new Service ID for BTA AND require to change the value of BTA_MAX_SERVICE_ID,
 *        make sure that the correct security ID of the new service from Security service definitions (btm_api.h)
 *        should be added to bta_service_id_to_btm_srv_id_lkup_tbl table in bta_dm_act.c.
 */

#define BTA_RES_SERVICE_ID      0           /* Reserved */
#define BTA_SPP_SERVICE_ID      1           /* Serial port profile. */
#define BTA_DUN_SERVICE_ID      2           /* Dial-up networking profile. */
#define BTA_A2DP_SOURCE_SERVICE_ID      3   /* A2DP Source profile. */
#define BTA_LAP_SERVICE_ID      4           /* LAN access profile. */
#define BTA_HSP_SERVICE_ID      5           /* Headset profile. */
#define BTA_HFP_SERVICE_ID      6           /* Hands-free profile. */
#define BTA_OPP_SERVICE_ID      7           /* Object push  */
#define BTA_FTP_SERVICE_ID      8           /* File transfer */
#define BTA_CTP_SERVICE_ID      9           /* Cordless Terminal */
#define BTA_ICP_SERVICE_ID      10          /* Intercom Terminal */
#define BTA_SYNC_SERVICE_ID     11          /* Synchronization */
#define BTA_BPP_SERVICE_ID      12          /* Basic printing profile */
#define BTA_BIP_SERVICE_ID      13          /* Basic Imaging profile */
#define BTA_PANU_SERVICE_ID     14          /* PAN User */
#define BTA_NAP_SERVICE_ID      15          /* PAN Network access point */
#define BTA_GN_SERVICE_ID       16          /* PAN Group Ad-hoc networks */
#define BTA_SAP_SERVICE_ID      17          /* SIM Access profile */
#define BTA_A2DP_SERVICE_ID     18          /* A2DP Sink */
#define BTA_AVRCP_SERVICE_ID    19          /* A/V remote control */
#define BTA_HID_SERVICE_ID      20          /* HID */
#define BTA_VDP_SERVICE_ID      21          /* Video distribution */
#define BTA_PBAP_SERVICE_ID     22          /* PhoneBook Access Server*/
#define BTA_HSP_HS_SERVICE_ID   23          /* HFP HS role */
#define BTA_HFP_HS_SERVICE_ID   24          /* HSP HS role */
#define BTA_MAP_SERVICE_ID      25          /* Message Access Profile */
#define BTA_MN_SERVICE_ID       26          /* Message Notification Service */
#define BTA_HDP_SERVICE_ID      27          /* Health Device Profile */
#define BTA_PCE_SERVICE_ID      28          /* PhoneBook Access Client*/

#define BTA_USER_SERVICE_ID     29          /* User requested UUID */
#define BTA_MAX_SERVICE_ID      30

/* service IDs (BTM_SEC_SERVICE_FIRST_EMPTY + 1) to (BTM_SEC_MAX_SERVICES - 1)
 * are used by BTA JV */
#define BTA_FIRST_JV_SERVICE_ID     (BTM_SEC_SERVICE_FIRST_EMPTY + 1)
#define BTA_LAST_JV_SERVICE_ID      (BTM_SEC_MAX_SERVICES - 1)

typedef UINT8 tBTA_SERVICE_ID;

/* Service ID Mask */
#define BTA_RES_SERVICE_MASK        0x00000001  /* Reserved */
#define BTA_SPP_SERVICE_MASK        0x00000002  /* Serial port profile. */
#define BTA_DUN_SERVICE_MASK        0x00000004  /* Dial-up networking profile. */
#define BTA_FAX_SERVICE_MASK        0x00000008  /* Fax profile. */
#define BTA_LAP_SERVICE_MASK        0x00000010  /* LAN access profile. */
#define BTA_HSP_SERVICE_MASK        0x00000020  /* HSP AG role. */
#define BTA_HFP_SERVICE_MASK        0x00000040  /* HFP AG role */
#define BTA_OPP_SERVICE_MASK        0x00000080  /* Object push  */
#define BTA_FTP_SERVICE_MASK        0x00000100  /* File transfer */
#define BTA_CTP_SERVICE_MASK        0x00000200  /* Cordless Terminal */
#define BTA_ICP_SERVICE_MASK        0x00000400  /* Intercom Terminal */
#define BTA_SYNC_SERVICE_MASK       0x00000800  /* Synchronization */
#define BTA_BPP_SERVICE_MASK        0x00001000  /* Print server */
#define BTA_BIP_SERVICE_MASK        0x00002000  /* Basic Imaging */
#define BTA_PANU_SERVICE_MASK       0x00004000  /* PAN User */
#define BTA_NAP_SERVICE_MASK        0x00008000  /* PAN Network access point */
#define BTA_GN_SERVICE_MASK         0x00010000  /* PAN Group Ad-hoc networks */
#define BTA_SAP_SERVICE_MASK        0x00020000  /* PAN Group Ad-hoc networks */
#define BTA_A2DP_SERVICE_MASK       0x00040000  /* Advanced audio distribution */
#define BTA_AVRCP_SERVICE_MASK      0x00080000  /* A/V remote control */
#define BTA_HID_SERVICE_MASK        0x00100000  /* HID */
#define BTA_VDP_SERVICE_MASK        0x00200000  /* Video distribution */
#define BTA_PBAP_SERVICE_MASK       0x00400000  /* Phone Book Server */
#define BTA_HSP_HS_SERVICE_MASK     0x00800000  /* HFP HS role */
#define BTA_HFP_HS_SERVICE_MASK     0x01000000  /* HSP HS role */
#define BTA_MAS_SERVICE_MASK        0x02000000  /* Message Access Profile */
#define BTA_MN_SERVICE_MASK         0x04000000  /* Message Notification Profile */
#define BTA_HL_SERVICE_MASK         0x08000000  /* Health Device Profile */
#define BTA_PCE_SERVICE_MASK        0x10000000  /* Phone Book Client */

// btla-specific ++
#define BTA_USER_SERVICE_MASK       0x20000000  /* Message Notification Profile */
// btla-specific --

#define BTA_ALL_SERVICE_MASK        0x1FFFFFFF  /* All services supported by BTA. */

typedef UINT32 tBTA_SERVICE_MASK;

/* SW sub-systems */
#define BTA_ID_SYS          0            /* system manager */
/* BLUETOOTH PART - from 0 to BTA_ID_BLUETOOTH_MAX */
#define BTA_ID_DM           1            /* device manager */
#define BTA_ID_DM_SEARCH    2            /* device manager search */
#define BTA_ID_DM_SEC       3            /* device manager security */
#define BTA_ID_DG           4            /* data gateway */
#define BTA_ID_AG           5            /* audio gateway */
#define BTA_ID_OPC          6            /* object push client */
#define BTA_ID_OPS          7            /* object push server */
#define BTA_ID_FTS          8            /* file transfer server */
#define BTA_ID_CT           9            /* cordless telephony terminal */
#define BTA_ID_FTC          10           /* file transfer client */
#define BTA_ID_SS           11           /* synchronization server */
#define BTA_ID_PR           12           /* Printer client */
#define BTA_ID_BIC          13           /* Basic Imaging Client */
#define BTA_ID_PAN          14           /* Personal Area Networking */
#define BTA_ID_BIS          15           /* Basic Imaging Server */
#define BTA_ID_ACC          16           /* Advanced Camera Client */
#define BTA_ID_SC           17           /* SIM Card Access server */
#define BTA_ID_AV           18           /* Advanced audio/video */
#define BTA_ID_AVK          19           /* Audio/video sink */
#define BTA_ID_HD           20           /* HID Device */
#define BTA_ID_CG           21           /* Cordless Gateway */
#define BTA_ID_BP           22           /* Basic Printing Client */
#define BTA_ID_HH           23           /* Human Interface Device Host */
#define BTA_ID_PBS          24           /* Phone Book Access Server */
#define BTA_ID_PBC          25           /* Phone Book Access Client */
#define BTA_ID_JV           26           /* Java */
#define BTA_ID_HS           27           /* Headset */
#define BTA_ID_MSE          28           /* Message Server Equipment */
#define BTA_ID_MCE          29           /* Message Client Equipment */
#define BTA_ID_HL           30           /* Health Device Profile*/
#define BTA_ID_GATTC        31           /* GATT Client */
#define BTA_ID_GATTS        32           /* GATT Client */
#define BTA_ID_BLUETOOTH_MAX   33        /* last BT profile */

/* FM */
#define BTA_ID_FM           34           /* FM  */
#define BTA_ID_FMTX         35           /* FM TX */

/* SENSOR */
#define BTA_ID_SSR          36           /* Sensor  */

/* GPS */
#define BTA_ID_GPS          37           /* GPS  */

/* GENERIC */
#define BTA_ID_PRM          38
#define BTA_ID_SYSTEM       39           /* platform-specific */
#define BTA_ID_SWRAP        40           /* Insight script wrapper */
#define BTA_ID_MIP          41           /* Multicase Individual Polling */
#define BTA_ID_RT           42           /* Audio Routing module: This module is always on. */

/* JV */
#define BTA_ID_JV1          43           /* JV1 */
#define BTA_ID_JV2          44           /* JV2 */

#define BTA_ID_MAX          (43 + BTA_DM_NUM_JV_ID)

typedef UINT8 tBTA_SYS_ID;

enum
{
    BTM_SUCCESS = 0,                    /* 0  Command succeeded                 */
    BTM_CMD_STARTED,                    /* 1  Command started OK.               */
    BTM_BUSY,                           /* 2  Device busy with another command  */
    BTM_NO_RESOURCES,                   /* 3  No resources to issue command     */
    BTM_MODE_UNSUPPORTED,               /* 4  Request for 1 or more unsupported modes */
    BTM_ILLEGAL_VALUE,                  /* 5  Illegal parameter value           */
    BTM_WRONG_MODE,                     /* 6  Device in wrong mode for request  */
    BTM_UNKNOWN_ADDR,                   /* 7  Unknown remote BD address         */
    BTM_DEVICE_TIMEOUT,                 /* 8  Device timeout                    */
    BTM_BAD_VALUE_RET,                  /* 9  A bad value was received from HCI */
    BTM_ERR_PROCESSING,                 /* 10 Generic error                     */
    BTM_NOT_AUTHORIZED,                 /* 11 Authorization failed              */
    BTM_DEV_RESET,                      /* 12 Device has been reset             */
    BTM_CMD_STORED,                     /* 13 request is stored in control block */
    BTM_ILLEGAL_ACTION,                 /* 14 state machine gets illegal command */
    BTM_DELAY_CHECK,                    /* 15 delay the check on encryption */
    BTM_SCO_BAD_LENGTH,                 /* 16 Bad SCO over HCI data length */
    BTM_SUCCESS_NO_SECURITY,            /* 17 security passed, no security set  */
    BTM_FAILED_ON_SECURITY ,             /* 18 security failed                   */
    BTM_REPEATED_ATTEMPTS               /* 19 repeated attempts for LE security requests */
};
typedef UINT8 tBTM_STATUS;

typedef struct
{
    UINT16          event;
    UINT16          len;
    UINT16          offset;
    UINT16          layer_specific;
} BT_HDR;

#define BT_HDR_SIZE (sizeof (BT_HDR))

/* Security Service Levels [bit mask] (BTM_SetSecurityLevel)
** Encryption should not be used without authentication
*/
#define BTM_SEC_NONE               0x0000 /* Nothing required */
#define BTM_SEC_IN_AUTHORIZE       0x0001 /* Inbound call requires authorization */
#define BTM_SEC_IN_AUTHENTICATE    0x0002 /* Inbound call requires authentication */
#define BTM_SEC_IN_ENCRYPT         0x0004 /* Inbound call requires encryption */
#define BTM_SEC_OUT_AUTHORIZE      0x0008 /* Outbound call requires authorization */
#define BTM_SEC_OUT_AUTHENTICATE   0x0010 /* Outbound call requires authentication */
#define BTM_SEC_OUT_ENCRYPT        0x0020 /* Outbound call requires encryption */
#define BTM_SEC_BOND               0x0040 /* Bonding */
#define BTM_SEC_BOND_CONN          0x0080 /* bond_created_connection */
#define BTM_SEC_FORCE_MASTER       0x0100 /* Need to switch connection to be master */
#define BTM_SEC_ATTEMPT_MASTER     0x0200 /* Try to switch connection to be master */
#define BTM_SEC_FORCE_SLAVE        0x0400 /* Need to switch connection to be master */
#define BTM_SEC_ATTEMPT_SLAVE      0x0800 /* Try to switch connection to be slave */
#define BTM_SEC_IN_MITM            0x1000 /* inbound Do man in the middle protection */
#define BTM_SEC_OUT_MITM           0x2000 /* outbound Do man in the middle protection */

/* Security Setting Mask */
#define BTA_SEC_NONE            BTM_SEC_NONE                                         /* No security. */
#define BTA_SEC_AUTHORIZE       (BTM_SEC_IN_AUTHORIZE )                              /* Authorization required (only needed for out going connection )*/
#define BTA_SEC_AUTHENTICATE    (BTM_SEC_IN_AUTHENTICATE | BTM_SEC_OUT_AUTHENTICATE) /* Authentication required. */
#define BTA_SEC_ENCRYPT         (BTM_SEC_IN_ENCRYPT | BTM_SEC_OUT_ENCRYPT)           /* Encryption required. */

typedef UINT8 tBTA_SEC;

/* Pairable Modes */
#define BTA_DM_PAIRABLE         1
#define BTA_DM_NON_PAIRABLE     0

/* Connectable Paired Only Mode */
#define BTA_DM_CONN_ALL         0
#define BTA_DM_CONN_PAIRED      1

/* Inquiry modes
 * Note: These modes are associated with the inquiry active values (BTM_*ACTIVE) */
#define BTM_GENERAL_INQUIRY         0
#define BTM_LIMITED_INQUIRY         1
#define BTM_BR_INQUIRY_MASK         0x0f
/* high byte of inquiry mode for BLE inquiry mode */
#define BTM_BLE_INQUIRY_NONE         0x00
#define BTM_BLE_GENERAL_INQUIRY      0x10
#define BTM_BLE_LIMITED_INQUIRY      0x20
#define BTM_BLE_INQUIRY_MASK         (BTM_BLE_GENERAL_INQUIRY|BTM_BLE_LIMITED_INQUIRY)

#define BTA_BLE_INQUIRY_NONE    BTM_BLE_INQUIRY_NONE
#define BTA_BLE_GENERAL_INQUIRY BTM_BLE_GENERAL_INQUIRY      /* Perform LE general inquiry. */
#define BTA_BLE_LIMITED_INQUIRY BTM_BLE_LIMITED_INQUIRY      /* Perform LE limited inquiry. */
typedef UINT8 tBTA_DM_INQ_MODE;

/* Inquiry Filter Type */
#define BTA_DM_INQ_CLR          BTM_CLR_INQUIRY_FILTER          /* Clear inquiry filter. */
#define BTA_DM_INQ_DEV_CLASS    BTM_FILTER_COND_DEVICE_CLASS    /* Filter on device class. */
#define BTA_DM_INQ_BD_ADDR      BTM_FILTER_COND_BD_ADDR         /* Filter on a specific  BD address. */

typedef UINT8 tBTA_DM_INQ_FILT;

/* Inquiry filter device class condition */
typedef struct
{
    DEV_CLASS       dev_class;        /* device class of interest */
    DEV_CLASS       dev_class_mask;   /* mask to determine the bits of device class of interest */
} tBTA_DM_COD_COND;

/* Inquiry Filter Condition */
typedef union
{
    BD_ADDR              bd_addr;            /* BD address of  device to filter. */
    tBTA_DM_COD_COND     dev_class_cond;     /* Device class filter condition */
} tBTA_DM_INQ_COND;

/* Inquiry Parameters */
typedef struct
{
    tBTA_DM_INQ_MODE    mode;           /* Inquiry mode, limited or general. */
    UINT8               duration;       /* Inquiry duration in 1.28 sec units. */
    UINT8               max_resps;      /* Maximum inquiry responses.  Set to zero for unlimited responses. */
    BOOLEAN             report_dup;     /* report duplicated inquiry response with higher RSSI value */
    tBTA_DM_INQ_FILT    filter_type;    /* Filter condition type. */
    tBTA_DM_INQ_COND    filter_cond;    /* Filter condition data. */
} tBTA_DM_INQ;

/* Security Callback Events */
#define BTA_DM_ENABLE_EVT               0       /* Enable Event */
#define BTA_DM_DISABLE_EVT              1       /* Disable Event */
#define BTA_DM_PIN_REQ_EVT              2       /* PIN request. */
#define BTA_DM_AUTH_CMPL_EVT            3       /* Authentication complete indication. */
#define BTA_DM_AUTHORIZE_EVT            4       /* Authorization request. */
#define BTA_DM_LINK_UP_EVT              5       /* Connection UP event */
#define BTA_DM_LINK_DOWN_EVT            6       /* Connection DOWN event */
#define BTA_DM_SIG_STRENGTH_EVT         7       /* Signal strength for bluetooth connection */
#define BTA_DM_BUSY_LEVEL_EVT           8       /* System busy level */
#define BTA_DM_BOND_CANCEL_CMPL_EVT     9       /* Bond cancel complete indication */
#define BTA_DM_SP_CFM_REQ_EVT           10      /* Simple Pairing User Confirmation request. */
#define BTA_DM_SP_KEY_NOTIF_EVT         11      /* Simple Pairing Passkey Notification */
#define BTA_DM_SP_RMT_OOB_EVT           12      /* Simple Pairing Remote OOB Data request. */
#define BTA_DM_SP_KEYPRESS_EVT          13      /* Key press notification event. */
#define BTA_DM_ROLE_CHG_EVT             14      /* Role Change event. */
#define BTA_DM_BLE_KEY_EVT              15      /* BLE SMP key event for peer device keys */
#define BTA_DM_BLE_SEC_REQ_EVT          16      /* BLE SMP security request */
#define BTA_DM_BLE_PASSKEY_NOTIF_EVT    17      /* SMP passkey notification event */
#define BTA_DM_BLE_PASSKEY_REQ_EVT      18      /* SMP passkey request event */
#define BTA_DM_BLE_OOB_REQ_EVT          19      /* SMP OOB request event */
#define BTA_DM_BLE_LOCAL_IR_EVT         20      /* BLE local IR event */
#define BTA_DM_BLE_LOCAL_ER_EVT         21      /* BLE local ER event */
// btla-specific ++
#define BTA_DM_BLE_AUTH_CMPL_EVT        22      /* BLE Auth complete */
// btla-specific --
#define BTA_DM_DEV_UNPAIRED_EVT         23
#define BTA_DM_HW_ERROR_EVT             24      /* BT Chip H/W error */
typedef UINT8 tBTA_DM_SEC_EVT;

/* Structure associated with BTA_DM_ENABLE_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address of local device. */
    tBTA_STATUS     status;
} tBTA_DM_ENABLE;

/* Structure associated with BTA_DM_PIN_REQ_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address peer device. */
    BD_NAME         bd_name;            /* Name of peer device. */
    DEV_CLASS       dev_class;          /* Class of Device */
} tBTA_DM_PIN_REQ;

/* Link Key Notification Event (Key Type) definitions */
#define HCI_LKEY_TYPE_COMBINATION       0x00
#define HCI_LKEY_TYPE_LOCAL_UNIT        0x01
#define HCI_LKEY_TYPE_REMOTE_UNIT       0x02
#define HCI_LKEY_TYPE_DEBUG_COMB        0x03
#define HCI_LKEY_TYPE_UNAUTH_COMB       0x04
#define HCI_LKEY_TYPE_AUTH_COMB         0x05
#define HCI_LKEY_TYPE_CHANGED_COMB      0x06

/* Structure associated with BTA_DM_AUTH_CMPL_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address peer device. */
    BD_NAME         bd_name;            /* Name of peer device. */
    BOOLEAN         key_present;        /* Valid link key value in key element */
    LINK_KEY        key;                /* Link key associated with peer device. */
    UINT8           key_type;           /* The type of Link Key */
    BOOLEAN         success;            /* TRUE of authentication succeeded, FALSE if failed. */
    UINT8           fail_reason;        /* The HCI reason/error code for when success=FALSE */
} tBTA_DM_AUTH_CMPL;

/* Structure associated with BTA_DM_AUTHORIZE_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address peer device. */
    BD_NAME         bd_name;            /* Name of peer device. */
    tBTA_SERVICE_ID service;            /* Service ID to authorize. */
// btla-specific ++
    DEV_CLASS       dev_class;
// btla-specific --
} tBTA_DM_AUTHORIZE;

/* Structure associated with BTA_DM_LINK_UP_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address peer device. */
} tBTA_DM_LINK_UP;

/*
**  Defentions for HCI Error Codes that are past in the events
*/
#define HCI_SUCCESS                                     0x00
#define HCI_PENDING                                     0x00
#define HCI_ERR_ILLEGAL_COMMAND                         0x01
#define HCI_ERR_NO_CONNECTION                           0x02
#define HCI_ERR_HW_FAILURE                              0x03
#define HCI_ERR_PAGE_TIMEOUT                            0x04
#define HCI_ERR_AUTH_FAILURE                            0x05
#define HCI_ERR_KEY_MISSING                             0x06
#define HCI_ERR_MEMORY_FULL                             0x07
#define HCI_ERR_CONNECTION_TOUT                         0x08
#define HCI_ERR_MAX_NUM_OF_CONNECTIONS                  0x09
#define HCI_ERR_MAX_NUM_OF_SCOS                         0x0A
#define HCI_ERR_CONNECTION_EXISTS                       0x0B
#define HCI_ERR_COMMAND_DISALLOWED                      0x0C
#define HCI_ERR_HOST_REJECT_RESOURCES                   0x0D
#define HCI_ERR_HOST_REJECT_SECURITY                    0x0E
#define HCI_ERR_HOST_REJECT_DEVICE                      0x0F
#define HCI_ERR_HOST_TIMEOUT                            0x10
#define HCI_ERR_UNSUPPORTED_VALUE                       0x11
#define HCI_ERR_ILLEGAL_PARAMETER_FMT                   0x12
#define HCI_ERR_PEER_USER                               0x13
#define HCI_ERR_PEER_LOW_RESOURCES                      0x14
#define HCI_ERR_PEER_POWER_OFF                          0x15
#define HCI_ERR_CONN_CAUSE_LOCAL_HOST                   0x16
#define HCI_ERR_REPEATED_ATTEMPTS                       0x17
#define HCI_ERR_PAIRING_NOT_ALLOWED                     0x18
#define HCI_ERR_UNKNOWN_LMP_PDU                         0x19
#define HCI_ERR_UNSUPPORTED_REM_FEATURE                 0x1A
#define HCI_ERR_SCO_OFFSET_REJECTED                     0x1B
#define HCI_ERR_SCO_INTERVAL_REJECTED                   0x1C
#define HCI_ERR_SCO_AIR_MODE                            0x1D
#define HCI_ERR_INVALID_LMP_PARAM                       0x1E
#define HCI_ERR_UNSPECIFIED                             0x1F
#define HCI_ERR_UNSUPPORTED_LMP_FEATURE                 0x20
#define HCI_ERR_ROLE_CHANGE_NOT_ALLOWED                 0x21
#define HCI_ERR_LMP_RESPONSE_TIMEOUT                    0x22
#define HCI_ERR_LMP_ERR_TRANS_COLLISION                 0x23
#define HCI_ERR_LMP_PDU_NOT_ALLOWED                     0x24
#define HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE               0x25
#define HCI_ERR_UNIT_KEY_USED                           0x26
#define HCI_ERR_QOS_NOT_SUPPORTED                       0x27
#define HCI_ERR_INSTANT_PASSED                          0x28
#define HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED     0x29
#define HCI_ERR_DIFF_TRANSACTION_COLLISION              0x2A
#define HCI_ERR_UNDEFINED_0x2B                          0x2B
#define HCI_ERR_QOS_UNACCEPTABLE_PARAM                  0x2C
#define HCI_ERR_QOS_REJECTED                            0x2D
#define HCI_ERR_CHAN_CLASSIF_NOT_SUPPORTED              0x2E
#define HCI_ERR_INSUFFCIENT_SECURITY                    0x2F
#define HCI_ERR_PARAM_OUT_OF_RANGE                      0x30
#define HCI_ERR_UNDEFINED_0x31                          0x31
#define HCI_ERR_ROLE_SWITCH_PENDING                     0x32
#define HCI_ERR_UNDEFINED_0x33                          0x33
#define HCI_ERR_RESERVED_SLOT_VIOLATION                 0x34
#define HCI_ERR_ROLE_SWITCH_FAILED                      0x35
#define HCI_ERR_INQ_RSP_DATA_TOO_LARGE                  0x36
#define HCI_ERR_SIMPLE_PAIRING_NOT_SUPPORTED            0x37
#define HCI_ERR_HOST_BUSY_PAIRING                       0x38
#define HCI_ERR_REJ_NO_SUITABLE_CHANNEL                 0x39
#define HCI_ERR_CONTROLLER_BUSY                         0x3A
#define HCI_ERR_UNACCEPT_CONN_INTERVAL                  0x3B
#define HCI_ERR_DIRECTED_ADVERTISING_TIMEOUT            0x3C
#define HCI_ERR_CONN_TOUT_DUE_TO_MIC_FAILURE            0x3D
#define HCI_ERR_CONN_FAILED_ESTABLISHMENT               0x3E
#define HCI_ERR_MAC_CONNECTION_FAILED                   0x3F

#define HCI_ERR_MAX_ERR                                 0x40

#define HCI_HINT_TO_RECREATE_AMP_PHYS_LINK              0xFF

/* Structure associated with BTA_DM_LINK_DOWN_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address peer device. */
    UINT8           status;             /* connection open/closed */
} tBTA_DM_LINK_DOWN;

typedef INT8 tBTA_DM_RSSI_VALUE;
typedef UINT8 tBTA_DM_LINK_QUALITY_VALUE;

/* signal strength mask */
#define BTA_SIG_STRENGTH_RSSI_MASK          1
#define BTA_SIG_STRENGTH_LINK_QUALITY_MASK  2

typedef UINT8 tBTA_SIG_STRENGTH_MASK;

/* Structure associated with BTA_DM_SIG_STRENGTH_EVT */
typedef struct
{
    BD_ADDR         bd_addr;            /* BD address peer device. */
    tBTA_SIG_STRENGTH_MASK mask;        /* mask for the values that are valid */
    tBTA_DM_RSSI_VALUE  rssi_value;
    tBTA_DM_LINK_QUALITY_VALUE link_quality_value;

} tBTA_DM_SIG_STRENGTH;

/* Structure associated with BTA_DM_BUSY_LEVEL_EVT */
typedef struct
{
    UINT8           level;     /* when paging or inquiring, level is 10.
                                    Otherwise, the number of ACL links */
} tBTA_DM_BUSY_LEVEL;

// muff1n: only filled with used members
typedef union
{
    tBTA_DM_ENABLE      enable;         /* BTA enabled */
    tBTA_DM_PIN_REQ     pin_req;        /* PIN request */
    tBTA_DM_AUTH_CMPL   auth_cmpl;      /* Authentication complete indication */
    tBTA_DM_AUTHORIZE   authorize;      /* Authorization request */
    tBTA_DM_LINK_UP     link_up;        /* ACL connection up event */
    tBTA_DM_LINK_DOWN   link_down;      /* ACL connection down event */
    tBTA_DM_SIG_STRENGTH sig_strength;  /* rssi and link quality value */
    tBTA_DM_BUSY_LEVEL  busy_level;     /* System busy level */
} tBTA_DM_SEC;

typedef void (tBTA_DM_SEC_CBACK)(tBTA_DM_SEC_EVT event, tBTA_DM_SEC *p_data);

/* Search callback events */
#define BTA_DM_INQ_RES_EVT              0       /* Inquiry result for a peer device. */
#define BTA_DM_INQ_CMPL_EVT             1       /* Inquiry complete. */
#define BTA_DM_DISC_RES_EVT             2       /* Discovery result for a peer device. */
#define BTA_DM_DISC_BLE_RES_EVT         3       /* Discovery result for BLE GATT based service on a peer device. */
#define BTA_DM_DISC_CMPL_EVT            4       /* Discovery complete. */
#define BTA_DM_DI_DISC_CMPL_EVT         5       /* Discovery complete. */
#define BTA_DM_SEARCH_CANCEL_CMPL_EVT   6       /* Search cancelled */

typedef UINT8 tBTA_DM_SEARCH_EVT;

#define BTA_DM_INQ_RES_IGNORE_RSSI      BTM_INQ_RES_IGNORE_RSSI /* 0x7f RSSI value not supplied (ignore it) */

// muff1n: commented some fields out
/* Structure associated with BTA_DM_INQ_RES_EVT */
typedef struct
{
    BD_ADDR         bd_addr;                /* BD address peer device. */
    DEV_CLASS       dev_class;              /* Device class of peer device. */
    //BOOLEAN         remt_name_not_required; /* Application sets this flag if it already knows the name of the device */
                                            /* If the device name is known to application BTA skips the remote name request */
    //BOOLEAN         is_limited;             /* TRUE, if the limited inquiry bit is set in the CoD */
    INT8            rssi;                   /* The rssi value */
    UINT8           *p_eir;                 /* received EIR */
} tBTA_DM_INQ_RES;

/* Structure associated with BTA_DM_INQ_CMPL_EVT */
typedef struct
{
    UINT8           num_resps;          /* Number of inquiry responses. */
} tBTA_DM_INQ_CMPL;

/* Structure associated with BTA_DM_DI_DISC_CMPL_EVT */
typedef struct
{
    BD_ADDR             bd_addr;        /* BD address peer device. */
    UINT8               num_record;     /* Number of DI record */
    tBTA_STATUS         result;
} tBTA_DM_DI_DISC_CMPL;

/* Structure associated with BTA_DM_DISC_RES_EVT */
typedef struct
{
    BD_ADDR             bd_addr;        /* BD address peer device. */
    BD_NAME             bd_name;        /* Name of peer device. */
    tBTA_SERVICE_MASK   services;       /* Services found on peer device. */
// btla-specific ++
    UINT8           *   p_raw_data;     /* Raw data for discovery DB */
    UINT32              raw_data_size;  /* size of raw data */
    tBT_DEVICE_TYPE     device_type;    /* device type in case it is BLE device */
    UINT32              num_uuids;
    UINT8               *p_uuid_list;
// btla-specific --
    tBTA_STATUS         result;
} tBTA_DM_DISC_RES;

/* Structure associated with tBTA_DM_DISC_BLE_RES */
typedef struct
{
    BD_ADDR             bd_addr;        /* BD address peer device. */
    BD_NAME             bd_name;        /* Name of peer device. */
    tBT_UUID            service;        /* GATT based Services UUID found on peer device. */
} tBTA_DM_DISC_BLE_RES;

/* Union of all search callback structures */
typedef union
{
    tBTA_DM_INQ_RES     inq_res;        /* Inquiry result for a peer device. */
    tBTA_DM_INQ_CMPL    inq_cmpl;       /* Inquiry complete. */
    tBTA_DM_DISC_RES    disc_res;       /* Discovery result for a peer device. */
    tBTA_DM_DISC_BLE_RES    disc_ble_res;   /* discovery result for GATT based service */
    tBTA_DM_DI_DISC_CMPL    di_disc;        /* DI discovery result for a peer device */
} tBTA_DM_SEARCH;

/* Search callback */
typedef void (tBTA_DM_SEARCH_CBACK)(tBTA_DM_SEARCH_EVT event, tBTA_DM_SEARCH *p_data);

/* type of protocol mode */
#define BTA_HH_PROTO_RPT_MODE                   (0x00)
#define BTA_HH_PROTO_BOOT_MODE                  (0x01)
#define BTA_HH_PROTO_UNKNOWN                    (0xff)
typedef UINT8   tBTA_HH_PROTO_MODE;

/* BTA HID Host callback events */
#define BTA_HH_ENABLE_EVT       0       /* HH enabled */
#define BTA_HH_DISABLE_EVT      1       /* HH disabled */
#define BTA_HH_OPEN_EVT         2       /* connection opened */
#define BTA_HH_CLOSE_EVT        3       /* connection closed */
#define BTA_HH_GET_RPT_EVT      4       /* BTA_HhGetReport callback */
#define BTA_HH_SET_RPT_EVT      5       /* BTA_HhSetReport callback */
#define BTA_HH_GET_PROTO_EVT    6       /* BTA_GetProtoMode callback */
#define BTA_HH_SET_PROTO_EVT    7       /* BTA_HhSetProtoMode callback */
#define BTA_HH_GET_IDLE_EVT     8       /* BTA_HhGetIdle comes callback */
#define BTA_HH_SET_IDLE_EVT     9       /* BTA_HhSetIdle finish callback */
#define BTA_HH_GET_DSCP_EVT     10      /* Get report descripotor */
#define BTA_HH_ADD_DEV_EVT      11      /* Add Device callback */
#define BTA_HH_RMV_DEV_EVT      12      /* remove device finished */
#define BTA_HH_VC_UNPLUG_EVT    13      /* virtually unplugged */
#define BTA_HH_UPDATE_UCD_EVT   14
#define BTA_HH_API_ERR_EVT      15      /* API error is caught */

typedef UINT16 tBTA_HH_EVT;

enum
{
    BTA_HH_OK,
    BTA_HH_HS_HID_NOT_READY,    /* handshake error : device not ready */
    BTA_HH_HS_INVALID_RPT_ID,   /* handshake error : invalid report ID */
    BTA_HH_HS_TRANS_NOT_SPT,    /* handshake error : transaction not spt */
    BTA_HH_HS_INVALID_PARAM,    /* handshake error : invalid paremter */
    BTA_HH_HS_ERROR,            /* handshake error : unspecified HS error */
    BTA_HH_ERR,                 /* general BTA HH error */
    BTA_HH_ERR_SDP,             /* SDP error */
    BTA_HH_ERR_PROTO,           /* SET_Protocol error,
                                    only used in BTA_HH_OPEN_EVT callback */
    BTA_HH_ERR_DB_FULL,         /* device database full error, used in
                                   BTA_HH_OPEN_EVT/BTA_HH_ADD_DEV_EVT */
    BTA_HH_ERR_TOD_UNSPT,       /* type of device not supported */
    BTA_HH_ERR_NO_RES,          /* out of system resources */
    BTA_HH_ERR_AUTH_FAILED,     /* authentication fail */
    BTA_HH_ERR_HDL
};
typedef UINT8 tBTA_HH_STATUS;

/* callback event data for BTA_HH_OPEN_EVT */
typedef struct
{
    BD_ADDR         bda;                /* HID device bd address    */
    tBTA_HH_STATUS  status;             /* operation status         */
    UINT8           handle;             /* device handle            */
} tBTA_HH_CONN;

typedef tBTA_HH_CONN tBTA_HH_DEV_INFO;

/* callback event data */
typedef struct
{
    tBTA_HH_STATUS              status;     /* operation status         */
    UINT8                       handle;     /* device handle            */
} tBTA_HH_CBDATA;

// muff1n: only filled with used members
typedef union
{
    tBTA_HH_DEV_INFO        dev_info;           /* BTA_HH_ADD_DEV_EVT, BTA_HH_RMV_DEV_EVT   */
    tBTA_HH_CONN            conn;               /* BTA_HH_OPEN_EVT      */
    tBTA_HH_CBDATA          dev_status;         /* BTA_HH_CLOSE_EVT,
                                                   BTA_HH_SET_PROTO_EVT
                                                   BTA_HH_SET_RPT_EVT
                                                   BTA_HH_SET_IDLE_EVT  */
} tBTA_HH;

/* BTA HH callback function */
typedef void (tBTA_HH_CBACK) (tBTA_HH_EVT event, tBTA_HH *p_data);

/* General callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_CMPL_CB) (void *p1);
/* Structure returned with local version information */

typedef struct
{
    UINT8       hci_version;
    UINT16      hci_revision;
    UINT8       lmp_version;
    UINT16      manufacturer;
    UINT16      lmp_subversion;
} tBTM_VERSION_INFO;

/* Structure returned with Vendor Specific Command complete callback */
typedef struct
{
    UINT16  opcode;
    UINT16  param_len;
    UINT8   *p_param_buf;
} tBTM_VSC_CMPL;

#define  BTM_VSC_CMPL_DATA_SIZE  (BTM_MAX_VENDOR_SPECIFIC_LEN + sizeof(tBTM_VSC_CMPL))
/* Callback function for when device status changes. Appl must poll for
** what the new state is (BTM_IsDeviceUp). The event occurs whenever the stack
** has detected that the controller status has changed. This asynchronous event
** is enabled/disabled by calling BTM_RegisterForDeviceStatusNotif().
*/
enum
{
    BTM_DEV_STATUS_UP,
    BTM_DEV_STATUS_DOWN,
    BTM_DEV_STATUS_CMD_TOUT
};
typedef UINT8 tBTM_DEV_STATUS;

typedef void (tBTM_DEV_STATUS_CB) (tBTM_DEV_STATUS status);

/* Callback function for when a vendor specific event occurs. The length and
** array of returned parameter bytes are included. This asynchronous event
** is enabled/disabled by calling BTM_RegisterForVSEvents().
*/
typedef void (tBTM_VS_EVT_CB) (UINT8 len, UINT8 *p);

/* VSC callback function for notifying an application that a synchronous
** BTM function is complete. The pointer contains the address of any returned data.
*/
typedef void (tBTM_VSC_CMPL_CB) (tBTM_VSC_CMPL *p1);

/* Attributes mask values to be used in HID_HostAddDev API */
#define HID_VIRTUAL_CABLE        0x0001
#define HID_NORMALLY_CONNECTABLE 0x0002
#define HID_RECONN_INIT          0x0004
#define HID_SDP_DISABLE          0x0008
#define HID_BATTERY_POWER        0x0010
#define HID_REMOTE_WAKE          0x0020
#define HID_SUP_TOUT_AVLBL       0x0040
#define HID_SSR_MAX_LATENCY      0x0080
#define HID_SSR_MIN_TOUT         0x0100

#define HID_SEC_REQUIRED         0x8000

#define BTA_HH_VIRTUAL_CABLE           HID_VIRTUAL_CABLE
#define BTA_HH_NORMALLY_CONNECTABLE    HID_NORMALLY_CONNECTABLE
#define BTA_HH_RECONN_INIT             HID_RECONN_INIT
#define BTA_HH_SDP_DISABLE             HID_SDP_DISABLE
#define BTA_HH_BATTERY_POWER           HID_BATTERY_POWER
#define BTA_HH_REMOTE_WAKE             HID_REMOTE_WAKE
#define BTA_HH_SUP_TOUT_AVLBL          HID_SUP_TOUT_AVLBL
#define BTA_HH_SEC_REQUIRED             HID_SEC_REQUIRED
typedef UINT16 tBTA_HH_ATTR_MASK;

typedef struct desc_info
{
    UINT16 dl_len;
    UINT8 *dsc_list;
} tHID_DEV_DSCP_INFO;

typedef tHID_DEV_DSCP_INFO tBTA_HH_DEV_DESCR;

/* Policy settings status */
#define HCI_DISABLE_ALL_LM_MODES        0x0000
#define HCI_ENABLE_MASTER_SLAVE_SWITCH  0x0001
#define HCI_ENABLE_HOLD_MODE            0x0002
#define HCI_ENABLE_SNIFF_MODE           0x0004
#define HCI_ENABLE_PARK_MODE            0x0008

/* HCI mode defenitions */
#define HCI_MODE_ACTIVE                 0x00
#define HCI_MODE_HOLD                   0x01
#define HCI_MODE_SNIFF                  0x02
#define HCI_MODE_PARK                   0x03

/* BTM Power manager status codes */
enum
{
    BTM_PM_STS_ACTIVE = HCI_MODE_ACTIVE,
    BTM_PM_STS_HOLD   = HCI_MODE_HOLD,
    BTM_PM_STS_SNIFF  = HCI_MODE_SNIFF,
    BTM_PM_STS_PARK   = HCI_MODE_PARK,
    BTM_PM_STS_SSR,     /* report the SSR parameters in HCI_SNIFF_SUB_RATE_EVT */
    BTM_PM_STS_PENDING,   /* when waiting for status from controller */
    BTM_PM_STS_ERROR   /* when HCI command status returns error */
};
typedef UINT8 tBTM_PM_STATUS;

/* BTM Power manager modes */

enum
{
    BTM_PM_MD_ACTIVE = BTM_PM_STS_ACTIVE,
    BTM_PM_MD_HOLD   = BTM_PM_STS_HOLD,
    BTM_PM_MD_SNIFF  = BTM_PM_STS_SNIFF,
    BTM_PM_MD_PARK   = BTM_PM_STS_PARK,
    BTM_PM_MD_FORCE  = 0x10 /* OR this to force ACL link to a certain mode */
};
typedef UINT8 tBTM_PM_MODE;

#define BTM_PM_SET_ONLY_ID  0x80

/* Operation codes */
#define BTM_PM_REG_SET      1 /* The module wants to set the desired power mode */
#define BTM_PM_REG_NOTIF    2 /* The module wants to receive mode change event */
#define BTM_PM_DEREG        4 /* The module does not want to involve with PM anymore */

typedef struct
{
    UINT16          max;
    UINT16          min;
    UINT16          attempt;
    UINT16          timeout;
    tBTM_PM_MODE    mode;
} tBTM_PM_PWR_MD;

/*************************************
**  Power Manager Callback Functions
**************************************/
typedef void (tBTM_PM_STATUS_CBACK) (BD_ADDR p_bda, tBTM_PM_STATUS status,
                                     UINT16 value, UINT8 hci_status);

/************************
**  Stored Linkkey Types
*************************/
#define BTM_CB_EVT_RETURN_LINK_KEYS         1
#define BTM_CB_EVT_READ_STORED_LINK_KEYS    2
#define BTM_CB_EVT_WRITE_STORED_LINK_KEYS   3
#define BTM_CB_EVT_DELETE_STORED_LINK_KEYS  4

typedef struct
{
    UINT8          event;

} tBTM_STORED_LINK_KEYS_EVT;


typedef struct
{
    UINT8          event;
    UINT8          num_keys;

} tBTM_RETURN_LINK_KEYS_EVT;


typedef struct
{
    BD_ADDR         bd_addr;
    LINK_KEY        link_key;

} tBTM_BD_ADDR_LINK_KEY_PAIR;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT16         max_keys;
    UINT16         read_keys;

} tBTM_READ_STORED_LINK_KEY_COMPLETE;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT8          num_keys;

} tBTM_WRITE_STORED_LINK_KEY_COMPLETE;


typedef struct
{
    UINT8          event;
    UINT8          status;
    UINT16         num_keys;

} tBTM_DELETE_STORED_LINK_KEY_COMPLETE;

// ---

tBTA_STATUS BTA_EnableBluetooth(tBTA_DM_SEC_CBACK *p_cback);
tBTA_STATUS BTA_DisableBluetooth(void);

// muff1n: most changed prototype so far
tBTA_STATUS BTA_DmAddDevice(BD_ADDR bd_addr, LINK_KEY link_key,
                            tBTA_SERVICE_MASK trusted_mask, BOOLEAN is_trusted);
tBTA_STATUS BTA_DmRemoveDevice(BD_ADDR bd_addr);
void BTA_DmSearch(tBTA_DM_INQ *p_dm_inq, tBTA_SERVICE_MASK services, tBTA_DM_SEARCH_CBACK *p_cback);
void BTA_DmSearchCancel(void);
void BTA_DmPinReply(BD_ADDR bd_addr, BOOLEAN accept, UINT8 pin_len, UINT8 *p_pin);
BOOLEAN BTA_DmIsDeviceUp(void);
void BTA_DmSetDeviceName(char *p_name);

// muff1n: disc_mode and conn_mode might be the , based on usage
void BTA_DmSetVisibility(UINT8, UINT8);

// muff1n: ucd_enabled might be the missing parameter
void BTA_HhEnable(tBTA_SEC sec_mask, tBTA_HH_CBACK *p_cback);
// muff1n: possibly takes just a tBTA_HH_DEV_DESCR in this version? i wrote it like that
void BTA_HhAddDev(BD_ADDR bda, tBTA_HH_ATTR_MASK attr_mask, UINT8 sub_class,
                  UINT8 app_id, tBTA_HH_DEV_DESCR descr);
void BTA_HhRemoveDev(UINT8 dev_handle);
void BTA_HhOpen(BD_ADDR dev_bda, tBTA_HH_PROTO_MODE mode, tBTA_SEC sec_mask);
void BTA_HhClose(UINT8 dev_handle);

// muff1n: dev_bda is likely the missing parameter, as it is not used
void BTA_HhSendData(UINT8 dev_handle, BT_HDR *p_data);

void bta_sys_set_trace_level(UINT8 level);

void BTM_DeviceReset(tBTM_CMPL_CB *p_cb);

tBTM_STATUS BTM_EnableTestMode(void);

tBTM_STATUS BTM_ReadStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb);
tBTM_STATUS BTM_WriteStoredLinkKey(UINT8 num_keys,
                                   BD_ADDR *bd_addr,
                                   LINK_KEY *link_key,
                                   tBTM_CMPL_CB *p_cb);
tBTM_STATUS BTM_DeleteStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB *p_cb);

tBTM_STATUS BTM_SetPowerMode(UINT8 pm_id, BD_ADDR remote_bda, tBTM_PM_PWR_MD *p_mode);
tBTM_STATUS BTM_SetAfhChannels(UINT8 first, UINT8 last);

tBTM_STATUS BTM_VendorSpecificCommand(UINT16 opcode, UINT8 param_len,
                                      UINT8 *p_param_buf, tBTM_VSC_CMPL_CB *p_cb);

tBTM_STATUS BTM_ReadLocalVersion(tBTM_VERSION_INFO *p_vers);
tBTM_STATUS BTM_SetDeviceClass(DEV_CLASS dev_class);

tBTM_STATUS btm_remove_acl(BD_ADDR bd_addr);

// muff1n: is_register is probably the missing parameter here
tBTM_STATUS BTM_RegisterForVSEvents(tBTM_VS_EVT_CB *p_cb);
tBTM_DEV_STATUS_CB *BTM_RegisterForDeviceStatusNotif(tBTM_DEV_STATUS_CB *p_cb);

tBTM_STATUS BTM_PmRegister(UINT8 mask, UINT8 *p_pm_id, tBTM_PM_STATUS_CBACK *p_cb);
tBTM_STATUS BTM_WritePageTimeout(UINT16 timeout);
void BTM_SetDefaultLinkPolicy(UINT16 settings);
void BTM_SetDefaultLinkSuperTout(UINT16 timeout);

BOOLEAN BTM_SecAddDevice(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name,
                         BD_FEATURES features, UINT32 trusted_mask[],
                         LINK_KEY link_key);

void *GKI_getbuf(UINT16 size);

UINT8 L2CA_SetTraceLevel(UINT8 trace_level);

UINT8 SDP_SetTraceLevel(UINT8 new_level);

/* muff1n: not in bluedroid source; might be older removed API functions */
void BTA_DmSendHciReset(void);
void BTA_HhGetAclQueueInfo(void);
void BTA_Init(void);
void BTA_CleanUp(void (*p_cb)(tBTA_STATUS status)); // probably

// ---
/* muff1n: I wrote this definition myself
 * TODO: would this be part of BLE or WUD?
 */

struct small_dev_info
{
    char        devName[20];    // size 0x14? offset 0x00 // might be 0x13?
    char        at_0x14[1];     // size 0x??, offset 0x14?
    char        __pad0[0x20 - (0x14 + 0x01)];
    LINK_KEY    linkKey;        // size 0x10, offset 0x20
    char        __pad1[0x10];
}; // size 0x40

#endif // CONTEXT_BTE_H
