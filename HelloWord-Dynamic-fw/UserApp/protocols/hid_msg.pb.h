/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.7 */

#ifndef PB_HID_MSG_HID_MSG_PB_H_INCLUDED
#define PB_HID_MSG_HID_MSG_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* 消息ID */
typedef enum _hid_msg_MessageId {
    /* 获取版本 */
    hid_msg_MessageId_VERSION = 1,
    /* 获取配置 */
    hid_msg_MessageId_MOTOR_GET_CONFIG = 2,
    /* 设置水墨屏 */
    hid_msg_MessageId_EINK_SET_IMAGE = 3,
    /* 设置电机信息 */
    hid_msg_MessageId_MOTOR_SET_CONFIG = 4
} hid_msg_MessageId;

typedef enum _hid_msg_knobMessage {
    /* 设置PID */
    hid_msg_knobMessage_SetPID = 1
} hid_msg_knobMessage;

/* Struct definitions */
typedef struct _hid_msg_Nil {
    char dummy_field;
} hid_msg_Nil;

/* 版本号消息 */
typedef struct _hid_msg_Version {
    /* git版本号 */
    pb_callback_t GitVer;
    /* githash */
    pb_callback_t GitHash;
    /* git分支 */
    pb_callback_t GitBranch;
} hid_msg_Version;

/* 扩展发来的消息 */
typedef struct _hid_msg_CtrlMessage {
    hid_msg_MessageId id;
    pb_size_t which_payload;
    union {
        hid_msg_Nil nil;
        hid_msg_Version version;
    } payload;
} hid_msg_CtrlMessage;

typedef struct _hid_msg_Eink {
    uint32_t bits_size;
    pb_callback_t bits;
} hid_msg_Eink;

typedef struct _hid_msg_knobPID_Velocity {
    float p;
    float i;
    float d;
} hid_msg_knobPID_Velocity;

typedef struct _hid_msg_knobPID_Angle {
    float p;
    float i;
    float d;
} hid_msg_knobPID_Angle;

typedef struct _hid_msg_knobPID {
    hid_msg_knobPID_Velocity velocity;
    hid_msg_knobPID_Angle angle;
} hid_msg_knobPID;

typedef struct _hid_msg_knob {
    hid_msg_knobMessage id;
    uint32_t knobModel;
    pb_size_t which_payload;
    union {
        hid_msg_knobPID pid;
    } payload;
} hid_msg_knob;

/* 上位机发来的消息 */
typedef struct _hid_msg_PcMessage {
    hid_msg_MessageId id;
    pb_size_t which_payload;
    union {
        hid_msg_Nil nil;
        hid_msg_Eink eink;
        hid_msg_knob knob;
    } payload;
} hid_msg_PcMessage;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _hid_msg_MessageId_MIN hid_msg_MessageId_VERSION
#define _hid_msg_MessageId_MAX hid_msg_MessageId_MOTOR_SET_CONFIG
#define _hid_msg_MessageId_ARRAYSIZE ((hid_msg_MessageId)(hid_msg_MessageId_MOTOR_SET_CONFIG+1))

#define _hid_msg_knobMessage_MIN hid_msg_knobMessage_SetPID
#define _hid_msg_knobMessage_MAX hid_msg_knobMessage_SetPID
#define _hid_msg_knobMessage_ARRAYSIZE ((hid_msg_knobMessage)(hid_msg_knobMessage_SetPID+1))

#define hid_msg_CtrlMessage_id_ENUMTYPE hid_msg_MessageId

#define hid_msg_PcMessage_id_ENUMTYPE hid_msg_MessageId




#define hid_msg_knob_id_ENUMTYPE hid_msg_knobMessage





/* Initializer values for message structs */
#define hid_msg_CtrlMessage_init_default         {_hid_msg_MessageId_MIN, 0, {hid_msg_Nil_init_default}}
#define hid_msg_PcMessage_init_default           {_hid_msg_MessageId_MIN, 0, {hid_msg_Nil_init_default}}
#define hid_msg_Nil_init_default                 {0}
#define hid_msg_Version_init_default             {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define hid_msg_Eink_init_default                {0, {{NULL}, NULL}}
#define hid_msg_knob_init_default                {_hid_msg_knobMessage_MIN, 0, 0, {hid_msg_knobPID_init_default}}
#define hid_msg_knobPID_init_default             {hid_msg_knobPID_Velocity_init_default, hid_msg_knobPID_Angle_init_default}
#define hid_msg_knobPID_Velocity_init_default    {0, 0, 0}
#define hid_msg_knobPID_Angle_init_default       {0, 0, 0}
#define hid_msg_CtrlMessage_init_zero            {_hid_msg_MessageId_MIN, 0, {hid_msg_Nil_init_zero}}
#define hid_msg_PcMessage_init_zero              {_hid_msg_MessageId_MIN, 0, {hid_msg_Nil_init_zero}}
#define hid_msg_Nil_init_zero                    {0}
#define hid_msg_Version_init_zero                {{{NULL}, NULL}, {{NULL}, NULL}, {{NULL}, NULL}}
#define hid_msg_Eink_init_zero                   {0, {{NULL}, NULL}}
#define hid_msg_knob_init_zero                   {_hid_msg_knobMessage_MIN, 0, 0, {hid_msg_knobPID_init_zero}}
#define hid_msg_knobPID_init_zero                {hid_msg_knobPID_Velocity_init_zero, hid_msg_knobPID_Angle_init_zero}
#define hid_msg_knobPID_Velocity_init_zero       {0, 0, 0}
#define hid_msg_knobPID_Angle_init_zero          {0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define hid_msg_Version_GitVer_tag               1
#define hid_msg_Version_GitHash_tag              2
#define hid_msg_Version_GitBranch_tag            3
#define hid_msg_CtrlMessage_id_tag               1
#define hid_msg_CtrlMessage_nil_tag              2
#define hid_msg_CtrlMessage_version_tag          3
#define hid_msg_Eink_bits_size_tag               1
#define hid_msg_Eink_bits_tag                    2
#define hid_msg_knobPID_Velocity_p_tag           1
#define hid_msg_knobPID_Velocity_i_tag           2
#define hid_msg_knobPID_Velocity_d_tag           3
#define hid_msg_knobPID_Angle_p_tag              1
#define hid_msg_knobPID_Angle_i_tag              2
#define hid_msg_knobPID_Angle_d_tag              3
#define hid_msg_knobPID_velocity_tag             4
#define hid_msg_knobPID_angle_tag                5
#define hid_msg_knob_id_tag                      1
#define hid_msg_knob_knobModel_tag               2
#define hid_msg_knob_pid_tag                     3
#define hid_msg_PcMessage_id_tag                 1
#define hid_msg_PcMessage_nil_tag                2
#define hid_msg_PcMessage_eink_tag               3
#define hid_msg_PcMessage_knob_tag               4

/* Struct field encoding specification for nanopb */
#define hid_msg_CtrlMessage_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UENUM,    id,                1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,nil,payload.nil),   2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,version,payload.version),   3)
#define hid_msg_CtrlMessage_CALLBACK NULL
#define hid_msg_CtrlMessage_DEFAULT (const pb_byte_t*)"\x08\x01\x00"
#define hid_msg_CtrlMessage_payload_nil_MSGTYPE hid_msg_Nil
#define hid_msg_CtrlMessage_payload_version_MSGTYPE hid_msg_Version

#define hid_msg_PcMessage_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UENUM,    id,                1) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,nil,payload.nil),   2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,eink,payload.eink),   3) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,knob,payload.knob),   4)
#define hid_msg_PcMessage_CALLBACK NULL
#define hid_msg_PcMessage_DEFAULT (const pb_byte_t*)"\x08\x01\x00"
#define hid_msg_PcMessage_payload_nil_MSGTYPE hid_msg_Nil
#define hid_msg_PcMessage_payload_eink_MSGTYPE hid_msg_Eink
#define hid_msg_PcMessage_payload_knob_MSGTYPE hid_msg_knob

#define hid_msg_Nil_FIELDLIST(X, a) \

#define hid_msg_Nil_CALLBACK NULL
#define hid_msg_Nil_DEFAULT NULL

#define hid_msg_Version_FIELDLIST(X, a) \
X(a, CALLBACK, REQUIRED, STRING,   GitVer,            1) \
X(a, CALLBACK, REQUIRED, STRING,   GitHash,           2) \
X(a, CALLBACK, REQUIRED, STRING,   GitBranch,         3)
#define hid_msg_Version_CALLBACK pb_default_field_callback
#define hid_msg_Version_DEFAULT NULL

#define hid_msg_Eink_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UINT32,   bits_size,         1) \
X(a, CALLBACK, REQUIRED, STRING,   bits,              2)
#define hid_msg_Eink_CALLBACK pb_default_field_callback
#define hid_msg_Eink_DEFAULT NULL

#define hid_msg_knob_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UENUM,    id,                1) \
X(a, STATIC,   REQUIRED, UINT32,   knobModel,         2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (payload,pid,payload.pid),   3)
#define hid_msg_knob_CALLBACK NULL
#define hid_msg_knob_DEFAULT (const pb_byte_t*)"\x08\x01\x00"
#define hid_msg_knob_payload_pid_MSGTYPE hid_msg_knobPID

#define hid_msg_knobPID_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, MESSAGE,  velocity,          4) \
X(a, STATIC,   REQUIRED, MESSAGE,  angle,             5)
#define hid_msg_knobPID_CALLBACK NULL
#define hid_msg_knobPID_DEFAULT NULL
#define hid_msg_knobPID_velocity_MSGTYPE hid_msg_knobPID_Velocity
#define hid_msg_knobPID_angle_MSGTYPE hid_msg_knobPID_Angle

#define hid_msg_knobPID_Velocity_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, FLOAT,    p,                 1) \
X(a, STATIC,   REQUIRED, FLOAT,    i,                 2) \
X(a, STATIC,   REQUIRED, FLOAT,    d,                 3)
#define hid_msg_knobPID_Velocity_CALLBACK NULL
#define hid_msg_knobPID_Velocity_DEFAULT NULL

#define hid_msg_knobPID_Angle_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, FLOAT,    p,                 1) \
X(a, STATIC,   REQUIRED, FLOAT,    i,                 2) \
X(a, STATIC,   REQUIRED, FLOAT,    d,                 3)
#define hid_msg_knobPID_Angle_CALLBACK NULL
#define hid_msg_knobPID_Angle_DEFAULT NULL

extern const pb_msgdesc_t hid_msg_CtrlMessage_msg;
extern const pb_msgdesc_t hid_msg_PcMessage_msg;
extern const pb_msgdesc_t hid_msg_Nil_msg;
extern const pb_msgdesc_t hid_msg_Version_msg;
extern const pb_msgdesc_t hid_msg_Eink_msg;
extern const pb_msgdesc_t hid_msg_knob_msg;
extern const pb_msgdesc_t hid_msg_knobPID_msg;
extern const pb_msgdesc_t hid_msg_knobPID_Velocity_msg;
extern const pb_msgdesc_t hid_msg_knobPID_Angle_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define hid_msg_CtrlMessage_fields &hid_msg_CtrlMessage_msg
#define hid_msg_PcMessage_fields &hid_msg_PcMessage_msg
#define hid_msg_Nil_fields &hid_msg_Nil_msg
#define hid_msg_Version_fields &hid_msg_Version_msg
#define hid_msg_Eink_fields &hid_msg_Eink_msg
#define hid_msg_knob_fields &hid_msg_knob_msg
#define hid_msg_knobPID_fields &hid_msg_knobPID_msg
#define hid_msg_knobPID_Velocity_fields &hid_msg_knobPID_Velocity_msg
#define hid_msg_knobPID_Angle_fields &hid_msg_knobPID_Angle_msg

/* Maximum encoded size of messages (where known) */
/* hid_msg_CtrlMessage_size depends on runtime parameters */
/* hid_msg_PcMessage_size depends on runtime parameters */
/* hid_msg_Version_size depends on runtime parameters */
/* hid_msg_Eink_size depends on runtime parameters */
#define hid_msg_Nil_size                         0
#define hid_msg_knobPID_Angle_size               15
#define hid_msg_knobPID_Velocity_size            15
#define hid_msg_knobPID_size                     34
#define hid_msg_knob_size                        44

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
