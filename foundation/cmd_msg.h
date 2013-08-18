/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file cmd_msg.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __cmd_msg_H_wFKZO36L_lcWh_H5Si_s7NC_uAyxb7BvwWu6__
#define __cmd_msg_H_wFKZO36L_lcWh_H5Si_s7NC_uAyxb7BvwWu6__

#ifdef __cplusplus
extern "C" {
#endif


#include "basic_type.h"
//=============================================================================
//                Constant Definition
//=============================================================================
/**
 * the order of send cmd to elements
 **/
typedef enum CMDMSG_SEND_ORDER_T
{
    CMDMSG_SEND_ORDER_FIFO       = 0x00000001,   // First-In-First-Out
    CMDMSG_SEND_ORDER_LIFO       = 0x00000002,   // Last-In-First-Out
    CMDMSG_SEND_ORDER_CUSTOMER   = 0x00000003,

}CMDMSG_SEND_ORDER;

/**
 * the priority of cmd
 *  - CMDMSG_PRIORITY_APPT: Send cmd box to the queue of this elements.
 *  - CMDMSG_PRIORITY_ER  : Be locked at the current elements
 *                          and finish process of this element.
 **/
typedef enum CMDMSG_PRIORITY_T
{
    CMDMSG_PRIORITY_APPT       = 0, // appointment
    CMDMSG_PRIORITY_ER,             // emergency

}CMDMSG_PRIORITY;

/**
 * cmd type of private communication
 **/
typedef enum CMDMSG_TYPE_T
{
    CMDMSG_TYPE_INIT                   = 0x00000001,
    CMDMSG_TYPE_GET_NEXT_SAMPLE        = 0x00000002,

}CMDMSG_TYPE;


#define CMDMSG_HANDLER_MAX_NUM         32
DEFINE_BIT_OP(CMDMSG_HANDLER_MAX_NUM);
//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
/**
 * cmd argument (cmd contain)
 **/
typedef struct CMDMSG_ARG_T
{
    uint32_t                cmd_type;
    BIT_OP_T                receiver;

    CMDMSG_SEND_ORDER       cmdmsg_send_order;
    uint32_t                order_customer_length;
    uint32_t                *pOrder_customer;

    union{
        struct{
            uint32_t    agrv;
        }def;
    }arg;

}CMDMSG_ARG;

/**
 * cmd box
 * - Two cases at CMDMSG_BOX_NEW/CMDMSG_BOX_DEL
 *   1. if pTunnel_Info != 0, 
 *   2. if pTunnel_Info == 0, 
 **/
typedef uint32_t (*CMDMSG_BOX_NEW)(struct CMDMSG_BOX_T **ppCmdMsg_box, void *pTunnel_Info);
typedef uint32_t (*CMDMSG_BOX_DEL)(struct CMDMSG_BOX_T **ppCmdMsg_box, void *pTunnel_Info);

typedef struct CMDMSG_BOX_T
{
    CMDMSG_PRIORITY    cmdmsg_priority;
    CMDMSG_ARG         cmdmsg_arg;
    void               *pTunnel_Info[2];

    CMDMSG_BOX_NEW     pfCmdMsg_box_New;
    CMDMSG_BOX_NEW     pfCmdMsg_box_Del;

}CMDMSG_BOX;

/**
 * one cmd msg item
 **/
typedef struct CMDMSG_NODE_T
{
    struct CMDMSG_NODE_T    *next, *prev;

    CMDMSG_BOX     cmdmsg_box;

}CMDMSG_NODE;

/**
 * cmd message init info
 **/
typedef struct CMDMSG_INIT_INFO_T
{
    void     *pHLooperroster;

}CMDMSG_INIT_INFO;

/**
 * cmd message handle info
 **/
typedef struct CMDMSG_HANDLE_T
{
    uint32_t        reserve;
}CMDMSG_HANDLE;
//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
BERR
cmdmsg_CreateHandle(
    CMDMSG_HANDLE       **ppHCmdmsg,
    CMDMSG_INIT_INFO    *pCmdmsg_init_info);

BERR
cmdmsg_DestroyHandle(
    CMDMSG_HANDLE   **ppHCmdmsg,
    void            *extraData);

BERR
cmdmsg_Post_CmdMsg(
    CMDMSG_HANDLE   *pHCmdmsg,
    CMDMSG_BOX      *pCmdMsg_box);

BERR
cmdmsg_Post_Resp_CmdMsg(
    CMDMSG_HANDLE   *pHCmdmsg,
    CMDMSG_BOX      *pCmdMsg_box);


#ifdef __cplusplus
}
#endif

#endif
