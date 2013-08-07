/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file looper.c
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#include "looper.h"
#include "queue_template.h"
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct LOOPER_T
{
    LOOPER_HANDLE       hLooper;

    uint32_t            loop_time_gap; // ms
    uint32_t            cmdmsg_queue_size;

    pthread_t           my_thread;
    bool                bExit_thread;
    pthread_mutex_t     my_mutext;

    HANDLER_DESC        cur_handler_desc;

    CMDMSG_NODE         *pRecv_cmdmsg_header;
    CMDMSG_NODE         *pRecv_cmdmsg_queue;
    uint32_t            recv_cmdmsg_num;

}LOOPER;
//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================
static uint32_t
_looper_proc(
    LOOPER      *pLooper,
    CMD_BOX     *pCmd_box)
{
    uint32_t    result = 0;

    do{
        if( !pCmd_box || !pLooper )     break;

        //-------------------------------------
        // call process
        if( pLooper->cur_handler_desc.process )
        {
            result = pLooper->cur_handler_desc.process();
            if( result )    break;
        }

        //-------------------------------------
        // call cmd receiver
        if( pLooper->cur_handler_desc.cmd_receive )
        {
            result = pLooper->cur_handler_desc.cmd_receive();
            if( result )    break;
        }

    }while(0);

    return result;
}

static void*
_looper_thread(
    void* arg)
{
    LOOPER      *pLooper = (LOOPER*)arg;

    while( pLooper->bExit_thread == false )
    {
        uint32_t    result = 0;
        CMD_BOX     *pAct_cmd_box = 0;

        do{
            if( !pLooper->pRecv_cmdmsg_header )     break;

            //-------------------------------------
            // get cmd msg from queue
            pAct_cmd_box = pLooper->pRecv_cmdmsg_header;
            pLooper->pRecv_cmdmsg_header = QUEUE_DEL(CMDMSG_NODE, pAct_cmd_box);
            pLooper->recv_cmdmsg_num--;
            if( pLooper->recv_cmdmsg_num < 0 )
                pLooper->recv_cmdmsg_num = 0;

            if( !pLooper->pRecv_cmdmsg_header )
                pLooper->pRecv_cmdmsg_queue = 0;

            //-------------------------------------
            // process cmd msg
            resul = _looper_proc(pLooper, pAct_cmd_box);
            if( result )    break;

        }while(0);

        if( pAct_cmd_box )  free(pAct_cmd_box);

        usleep(pLooper->loop_time_gap);
    }

    printf(" exit %s(%s)\n", __FUNCTION__, pLooper->cur_handler_desc.name);
    b_thread_exit(NULL);
    return 0;
}
//=============================================================================
//                Public Function Definition
//=============================================================================
BERR
looper_CreateHandle(
    LOOPER_HANDLE       **ppHLooper,
    LOOPER_INIT_INFO    *pInit_info)
{
    BERR        result = BERR_OK;

    do{
        LOOPER      *pLooper = 0;

        if( !ppHLooper || (*ppHLooper) || !pInit_info )
        {
            b_msg_ex(B_MSG_ERR, "error, wrong parameters !!");
            result = BERR_INVALID_PARAMETER;
            break;
        }

        pLooper = b_malloc(sizeof(LOOPER));
        if( !pLooper )
        {
            b_msg_ex(B_MSG_ERR, "error, allocate fail !!");
            result = BERR_ALLOC_FAILE;
            break;
        }
        memset(pLooper, 0x0, sizeof(LOOPER));

        //----------------------
        // init parameters
        pLooper->loop_time_gap     = (pInit_info->loop_time_gap) ? pInit_info->loop_time_gap : 30;
        pLooper->cmdmsg_queue_size = (pInit_info->cmdmsg_queue_size) ? pInit_info->cmdmsg_queue_size : 15;

        //----------------------
        b_mutex_init(B_MSG_TRACE_LOOPER, pLooper->my_mutext);

        //----------------------
        (*ppHLooper) = &pLooper->hLooper;

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}

BERR
looper_DestroyHandle(
    LOOPER_HANDLE   **ppHLooper,
    void            *extraData)
{
    BERR                result = BERR_OK;
    pthread_mutex_t     tmp_mutex = 0;

    do{
        LOOPER      *pLooper = 0;

        if( !ppHLooper || !(*ppHLooper) )   break;

        pLooper = DOWN_CAST(LOOPER, (*ppHLooper), hLooper);

        b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->my_mutext);
        _disable_irq();

        tmp_mutex = pLooper->my_mutext;

        free(pLooper);

        (*ppHLooper) = 0;
        b_mutex_unlock(B_MSG_TRACE_LOOPER, tmp_mutex);
        b_mutex_deinit(B_MSG_TRACE_LOOPER, tmp_mutex);

        _enable_irq();
    }while(0);

    return result;
}

BERR
looper_Start(
    LOOPER_HANDLE   *pHLooper,
    bool            bBrake)
{
    BERR        result = BERR_OK;

    do{
        LOOPER      *pLooper = 0;

        if( !pHLooper || !(*pHLooper ) )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);



    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}

BERR
looper_Post_CmdMsg(
    LOOPER_HANDLE   *pHLooper,
    CMD_BOX         *pCmd_box)
{
    BERR        result = BERR_OK;

    do{
        LOOPER          *pLooper = 0;
        CMDMSG_NODE     *pNew_cmdmsg_node = 0;

        if( !pHLooper || !(*pHLooper ) )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);

        switch( pCmd_box->cmd_priority )
        {
            case CMD_PRIORITY_APPT:
                // set cmd_box to queue
                if( pLooper->recv_cmdmsg_num > pLooper->cmdmsg_queue_size )
                {
                    b_msg_ex(B_MSG_ERR, "cmd msg full !!");
                    break;
                }

                pNew_cmdmsg_node = b_malloc(sizeof(CMDMSG_NODE));
                if( !pNew_cmdmsg_node )
                {
                    b_msg_ex(B_MSG_ERR, "error, allocate fail !!");
                    result = BERR_ALLOC_FAILE;
                    break;
                }
                memset(pNew_cmdmsg_node, 0x0, sizeof(CMDMSG_NODE));

                QUEUE_INIT(pNew_cmdmsg_node);

                pNew_cmdmsg_node->cmd_box = (*pCmd_box);

                if( !pLooper->pRecv_cmdmsg_header )
                {
                    pLooper->pRecv_cmdmsg_header = pNew_cmdmsg_node;
                    pLooper->pRecv_cmdmsg_queue  = pNew_cmdmsg_node;
                    recv_cmdmsg_num              = 1;
                }
                else
                {
                    pLooper->pRecv_cmdmsg_queue = QUEUE_ADD(CMDMSG_NODE, pLooper->pRecv_cmdmsg_queue, pNew_cmdmsg_node);
                    recv_cmdmsg_num++;
                }
                break;

            case CMD_PRIORITY_ER:
                _looper_proc(pLooper, pCmd_box);
                break;
        }

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}
/*
BERR
looper_tamplete(
    LOOPER_HANDLE   *pHLooper,
    void        *extraData)
{
    BERR        result = BERR_OK;

    do{
        LOOPER      *pLooper = 0;

        if( !pHLooper || !(*pHLooper ) )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}
*/