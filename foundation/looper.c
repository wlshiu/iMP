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

    bool                bUse_thread;
    pthread_t           my_thread;
    bool                bExit_thread;
    pthread_mutex_t     my_mutex;
    pthread_mutex_t     msg_mutex;
    pthread_mutex_t     desc_mutex;

    HANDLER_DESC        cur_handler_desc;
    HANDLER_CTRL_INFO   handler_ctrl_info;

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
DEFINE_QUEUE_TEMPLATE(CMDMSG_NODE);

static uint32_t
_looper_handler_desc_proc(
    LOOPER          *pLooper,
    CMDMSG_BOX      *pCmdMsg_box)
{
    uint32_t    result = 0;

    b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->desc_mutex);

    do{
        if( !pCmdMsg_box || !pLooper )     break;

        pLooper->handler_ctrl_info.cmdmsg_box = (*pCmdMsg_box);

        //-------------------------------------
        // call process
        if( pLooper->cur_handler_desc.process )
        {
            result = pLooper->cur_handler_desc.process(&pLooper->handler_ctrl_info, 0);
            if( result )    break;
        }

        //-------------------------------------
        // call cmd receiver
        if( pLooper->cur_handler_desc.cmd_receive )
        {
            result = pLooper->cur_handler_desc.cmd_receive(&pLooper->handler_ctrl_info, 0);
            if( result )    break;
        }

    }while(0);

    b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->desc_mutex);
    return result;
}

static uint32_t
_looper_proc(
    LOOPER      *pLooper)
{
    uint32_t        result = 0;
    CMDMSG_NODE     *pAct_node = 0;

    do{
        if( !pLooper->pRecv_cmdmsg_header )     break;

        b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->msg_mutex);
        //-------------------------------------
        // get cmd msg from queue
        pAct_node = pLooper->pRecv_cmdmsg_header;
        pLooper->pRecv_cmdmsg_header = QUEUE_DEL(CMDMSG_NODE, pAct_node);
        pLooper->recv_cmdmsg_num--;
        if( pLooper->recv_cmdmsg_num < 0 )
            pLooper->recv_cmdmsg_num = 0;

        if( !pLooper->pRecv_cmdmsg_header )
            pLooper->pRecv_cmdmsg_queue = 0;

        b_mutex_unlock(B_MSG_TRACE_LOOPER, pLooper->msg_mutex);

        //-------------------------------------
        // process cmd msg
        result = _looper_handler_desc_proc(pLooper, &pAct_node->cmdmsg_box);
        if( result )    break;

    }while(0);

    if( pAct_node )  free(pAct_node);

    usleep(pLooper->loop_time_gap);

    return result;
}

static void*
_looper_thread(
    void* arg)
{
    LOOPER      *pLooper = (LOOPER*)arg;

    while( pLooper->bExit_thread == false )
    {
        _looper_proc(pLooper);
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
        pLooper->bUse_thread       = false;
        pLooper->bExit_thread      = false;

        //----------------------
        b_mutex_init(B_MSG_TRACE_LOOPER, pLooper->msg_mutex);

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

        pLooper->bExit_thread = true;

        if( pLooper->bUse_thread == true )
            b_thread_join(pLooper->my_thread, NULL);

        b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->msg_mutex);
        _disable_irq();

        tmp_mutex = pLooper->msg_mutex;

        free(pLooper);

        (*ppHLooper) = 0;
        b_mutex_unlock(B_MSG_TRACE_LOOPER, tmp_mutex);
        b_mutex_deinit(B_MSG_TRACE_LOOPER, tmp_mutex);

        _enable_irq();
    }while(0);

    return result;
}

BERR
looper_Register_Handler(
    LOOPER_HANDLE   *pHLooper,
    HANDLER_DESC    *pHandler_desc)
{
    BERR        result = BERR_OK;

    do{
        LOOPER      *pLooper = 0;

        if( !pHLooper || !pHandler_desc )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);

        pLooper->cur_handler_desc = (*pHandler_desc);

        if( pLooper->cur_handler_desc.init )
        {
            uint32_t    ret = 0;
            ret = pLooper->cur_handler_desc.init(&pLooper->handler_ctrl_info, 0);
            if( ret )   break;

            b_mutex_init(B_MSG_TRACE_LOOPER, pLooper->desc_mutex);
        }

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}

BERR
looper_UnRegister_Handler(
    LOOPER_HANDLE   *pHLooper)
{
    BERR        result = BERR_OK;

    do{
        LOOPER      *pLooper = 0;

        if( !pHLooper )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);

        if( pLooper->cur_handler_desc.deinit )
        {
            uint32_t    ret = 0;
            ret = pLooper->cur_handler_desc.deinit(&pLooper->handler_ctrl_info, 0);
            if( ret )   break;
        }

        //-------------------------
        // need mutex to protect pLooper->cur_handler_des ????
        b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->desc_mutex);
        _disable_irq();
        memset(&pLooper->cur_handler_desc, 0x0, sizeof(HANDLER_DESC));
        b_mutex_unlock(B_MSG_TRACE_LOOPER, pLooper->desc_mutex);
        b_mutex_deinit(B_MSG_TRACE_LOOPER, pLooper->desc_mutex);
        _enable_irq();

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

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

        if( !pHLooper )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);

        if( bBrake == true )
        {
            uint32_t    ret = 0;

            ret = _looper_proc(pLooper);
            if( ret )   break;
        }
        else
        {
            b_thread_create(&pLooper->my_thread, NULL, _looper_thread, (void*)pLooper);
            pLooper->bUse_thread = true;
            usleep(10000);
        }

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
    CMDMSG_BOX      *pCmdMsg_box)
{
    BERR        result = BERR_OK;

    do{
        LOOPER          *pLooper = 0;
        CMDMSG_NODE     *pNew_cmdmsg_node = 0;

        if( !pHLooper )    break;

        pLooper = DOWN_CAST(LOOPER, pHLooper, hLooper);

        switch( pCmdMsg_box->cmdmsg_priority )
        {
            case CMDMSG_PRIORITY_APPT:
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

                pNew_cmdmsg_node->cmdmsg_box = (*pCmdMsg_box);

                b_mutex_lock(B_MSG_TRACE_LOOPER, pLooper->msg_mutex);

                if( !pLooper->pRecv_cmdmsg_header )
                {
                    pLooper->pRecv_cmdmsg_header = pNew_cmdmsg_node;
                    pLooper->pRecv_cmdmsg_queue  = pNew_cmdmsg_node;
                    pLooper->recv_cmdmsg_num     = 1;
                }
                else
                {
                    pLooper->pRecv_cmdmsg_queue = QUEUE_ADD(CMDMSG_NODE, pLooper->pRecv_cmdmsg_queue, pNew_cmdmsg_node);
                    pLooper->recv_cmdmsg_num++;
                }

                b_mutex_unlock(B_MSG_TRACE_LOOPER, pLooper->msg_mutex);
                break;

            case CMDMSG_PRIORITY_ER:
                _looper_handler_desc_proc(pLooper, pCmdMsg_box);
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