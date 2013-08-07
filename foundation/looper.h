/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file looper.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __looper_H_wNu4H9Il_l3bc_Hs4y_skbw_uTFN1QM0ipJx__
#define __looper_H_wNu4H9Il_l3bc_Hs4y_skbw_uTFN1QM0ipJx__

#ifdef __cplusplus
extern "C" {
#endif

#include "basic_type.h"
#include "cmd_msg.h"
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
/**
 * looper initial info
 **/
typedef struct LOOPER_INIT_INFO_T
{
    uint32_t    loop_time_gap; // ms

    uint32_t    cmdmsg_queue_size; // the amount of cmd receive queue

    void        *pTunnel_Info[2];
}LOOPER_INIT_INFO;

/**
 * looper handle info
 **/
typedef struct LOOPER_HANDLE_T
{
    uint32_t        handler_id;
    void            *pTunnel_Info;
}LOOPER_HANDLE;
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
looper_CreateHandle(
    LOOPER_HANDLE       **ppHLooper,
    LOOPER_INIT_INFO    *pInit_info);


BERR
looper_DestroyHandle(
    LOOPER_HANDLE   **ppHLooper,
    void            *extraData);


BERR
looper_Post_CmdMsg(
    LOOPER_HANDLE   *pHLooper,
    CMD_BOX         *pCmd_box);


#ifdef __cplusplus
}
#endif

#endif
