/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file cmd_msg.c
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#include "cmd_msg.h"
#include "looper_roster.h"
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct CMD_MSG_T
{
    CMDMSG_HANDLE           hCmdmsg;

    LOOPERROSTER_HANDLE     *pHLooperroster;

}CMD_MSG;
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
    CMDMSG_INIT_INFO    *pCmdmsg_init_info)
{
    BERR        result = BERR_OK;

    do{
        CMD_MSG     *pCmdmsg = 0;

        if( !ppHCmdmsg || (*ppHCmdmsg) || !pCmdmsg_init_info )
        {
            b_msg_ex(B_MSG_ERR, "error, wrong parameters !!");
            result = BERR_INVALID_PARAMETER;
            break;
        }

        pCmdmsg = b_malloc(sizeof(CMD_MSG));
        if( !pCmdmsg )
        {
            b_msg_ex(B_MSG_ERR, "error, allocate fail !!");
            result = BERR_ALLOC_FAILE;
            break;
        }
        memset(pCmdmsg, 0x0, sizeof(CMD_MSG));
        //----------------------
        // init parameters
        pCmdmsg->pHLooperroster = (LOOPERROSTER_HANDLE*)pCmdmsg_init_info->pHLooperroster;


        //----------------------
        (*ppHCmdmsg) = &pCmdmsg->hCmdmsg;

    }while(0);

    if( result != BERR_OK )
    {
        printf("%s, err 0x%x !", __FUNCTION__, result);
    }

    return result;
}

BERR
cmdmsg_DestroyHandle(
    CMDMSG_HANDLE   **ppHCmdmsg,
    void            *extraData)
{
    BERR        result = BERR_OK;

    do{
        CMD_MSG     *pCmdmsg = 0;

        if( !ppHCmdmsg || !(*ppHCmdmsg) )   break;

        pCmdmsg = DOWN_CAST(CMD_MSG, (*ppHCmdmsg), hCmdmsg);

        free(pCmdmsg);

        (*ppHCmdmsg) = 0;
    }while(0);

    return result;
}

BERR
cmdmsg_Post_CmdMsg(
    CMDMSG_HANDLE   *pHCmdmsg,
    CMD_BOX         *pCmd_box)
{
    BERR        result = BERR_OK;

    do{
        CMD_MSG                     *pCmdmsg = 0;
        LOOPERROSTER_CMDMSG_INFO    looperroster_cmdmsg_info = {0};

        if( !pHCmdmsg || !pCmd_box )    break;

        pCmdmsg = DOWN_CAST(CMD_MSG, pHCmdmsg, hCmdmsg);
        
        looperroster_cmdmsg_info.type    = LOOPERROSTER_CMDMSG_APPT;
        looperroster_cmdmsg_info.cmd_box = (*pCmd_box);

        looperroster_Post_CmdMsg(pCmdmsg->pHLooperroster, &looperroster_cmdmsg_info);
    }while(0);

    if( result != BERR_OK )
    {
        printf("%s, err 0x%x !", __FUNCTION__, result);
    }

    return result;
}

BERR
cmdmsg_Post_Resp_CmdMsg(
    CMDMSG_HANDLE   *pHCmdmsg,
    CMD_BOX         *pCmd_box)
{
    BERR        result = BERR_OK;

    do{
        CMD_MSG                     *pCmdmsg = 0;
        LOOPERROSTER_CMDMSG_INFO    looperroster_cmdmsg_info = {0};

        if( !pHCmdmsg || !pCmd_box )    break;

        pCmdmsg = DOWN_CAST(CMD_MSG, pHCmdmsg, hCmdmsg);
        
        looperroster_cmdmsg_info.type    = LOOPERROSTER_CMDMSG_RESP;
        looperroster_cmdmsg_info.cmd_box = (*pCmd_box);

        // response cmd
        looperroster_Post_CmdMsg(pCmdmsg->pHLooperroster, &looperroster_cmdmsg_info);
    }while(0);

    if( result != BERR_OK )
    {
        printf("%s, err 0x%x !", __FUNCTION__, result);
    }

    return result;
}


/*
BERR
cmdmsg_tamplete(
    CMDMSG_HANDLE   *pHCmdmsg,
    void        *extraData)
{
    BERR        result = BERR_OK;


    if( result != BERR_OK )
    {
        printf("%s, err 0x%x !", __FUNCTION__, result);
    }

    return result;
}
*/