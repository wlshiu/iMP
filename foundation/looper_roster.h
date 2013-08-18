/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file looper_roster.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __looper_roster_H_wt67XV5w_l2iy_H3km_sxx0_uxLItD6PKVPL__
#define __looper_roster_H_wt67XV5w_l2iy_H3km_sxx0_uxLItD6PKVPL__

#ifdef __cplusplus
extern "C" {
#endif

#include "basic_type.h"
#include "looper.h"
#include "cmd_msg.h"
//=============================================================================
//                Constant Definition
//=============================================================================
typedef enum LOOPERROSTER_CMDMSG_TYPE_T
{
    LOOPERROSTER_CMDMSG_APPT    = 0, // appointment
    LOOPERROSTER_CMDMSG_ER,          // emergency
    LOOPERROSTER_CMDMSG_RESP,        // response

}LOOPERROSTER_CMDMSG_TYPE;
//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
/**
 * looper roster looper info
 **/
typedef struct LOOPERROSTER_LOOPER_INFO_T
{
    LOOPER_HANDLE       *pHLooper;
    void                *pTunnel_Info[2];

}LOOPERROSTER_LOOPER_INFO;

/**
 * looper roster cmd msg info
 **/
typedef struct LOOPERROSTER_CMDMSG_INFO_T
{
    LOOPERROSTER_CMDMSG_TYPE    type;
    CMDMSG_BOX                  cmdmsg_box;

}LOOPERROSTER_CMDMSG_INFO;

/**
 * looper roster handle info
 **/
typedef struct LOOPERROSTER_HANDLE_T
{
    uint32_t        reserve;
}LOOPERROSTER_HANDLE;
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
looperroster_CreateHandle(
    LOOPERROSTER_HANDLE **ppHLooperroster,
    void                *extraData);

BERR
looperroster_DestroyHandle(
    LOOPERROSTER_HANDLE     **ppHLooperroster,
    void                    *extraData);

BERR
looperroster_Add_Looper(
    LOOPERROSTER_HANDLE         *pHLooperroster,
    LOOPERROSTER_LOOPER_INFO    *pLooper_info);

BERR
looperroster_Del_Looper(
    LOOPERROSTER_HANDLE         *pHLooperroster,
    LOOPERROSTER_LOOPER_INFO    *pLooper_info);

BERR
looperroster_Post_CmdMsg(
    LOOPERROSTER_HANDLE         *pHLooperroster,
    LOOPERROSTER_CMDMSG_INFO    *pCmdmsg_info);


#ifdef __cplusplus
}
#endif

#endif
