/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file handler_desc.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __handler_desc_H_wxS9pizA_lwQy_H6eq_siic_uB02UdTMHb59__
#define __handler_desc_H_wxS9pizA_lwQy_H6eq_siic_uB02UdTMHb59__

#ifdef __cplusplus
extern "C" {
#endif


#include "cmd_msg.h"
//=============================================================================
//				  Constant Definition
//=============================================================================
#define HANDLER_MAX_NUM         CMDMSG_HANDLER_MAX_NUM

typedef enum HANDLER_ID_T
{
    // HANDLER_ID_MM_DEMUX     = 0,
    // HANDLER_ID_MM_A_DEC,
    // HANDLER_ID_MM_V_DEC,

    HANDLER_ID_TOTAL     = HANDLER_MAX_NUM,
}HANDLER_ID;

//=============================================================================
//				  Macro Definition
//=============================================================================

//=============================================================================
//				  Structure Definition
//=============================================================================
typedef struct HANDLER_CTRL_INFO_T
{
    CMDMSG_BOX     cmdmsg_box;

    void        *pPriv_data;

}HANDLER_CTRL_INFO;

typedef struct HANDLER_DESC_T
{
    struct HANDLER_DESC_T *next;

    char        *name;
    uint32_t    handler_id;
    
    CMDMSG_BOX_NEW  pfCmdMsg_box_New;  // create cmd msg box context
    CMDMSG_BOX_DEL  pfCmdMsg_box_Del;  // delete cmd msg box context


    uint32_t    (*init)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);
    uint32_t    (*deinit)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);

    // uint32_t    (*pre_process)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);
    uint32_t    (*process)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);
    // uint32_t    (*post_process)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);

    uint32_t    (*cmd_receive)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);

    // uint32_t    (*emergency_proc)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);

    uint32_t    (*report)(HANDLER_CTRL_INFO *pCtrl_info, void *extraData);

}HANDLER_DESC;
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
