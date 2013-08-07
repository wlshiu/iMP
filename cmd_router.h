/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file media_player_err.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __cmd_router_H_wgqpBFGF_lyo7_HpNc_s9uw_urbCbNsGW4va__
#define __cmd_router_H_wgqpBFGF_lyo7_HpNc_s9uw_urbCbNsGW4va__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "media_player_err.h"
//=============================================================================
//				  Constant Definition
//=============================================================================
typedef enum CR_CMD_TYPE_T
{
    CR_CMD_TYPE_TO_APP,         // send cmd to application layer
    CR_CMD_TYPE_TO_DRIVER,      // send cmd to driver layer
}CR_CMD_TYPE;
//=============================================================================
//				  Macro Definition
//=============================================================================

//=============================================================================
//				  Structure Definition
//=============================================================================
typedef struct CMDROUTER_HANDLE_T
{
    uint32_t        reserve;
}CMDROUTER_HANDLE;
//=============================================================================
//				  Global Data Definition
//=============================================================================

//=============================================================================
//				  Private Function Definition
//=============================================================================

//=============================================================================
//				  Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
