/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file looper_roster.c
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#include "looper_roster.h"
#include "queue_template.h"
//=============================================================================
//                Constant Definition
//=============================================================================
#define MAX_CMDMSG_QUEUE_SIZE       10
//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
/**
 * one looper item
 **/
typedef struct LOOPER_NODE_T
{
    struct LOOPER_NODE_T    *next, *prev;

    LOOPER_HANDLE       *pHLooper;
    void                *pTunnel_Info[2];

}LOOPER_NODE;

/**
 * major looper roster info
 **/
typedef struct LOOPER_ROSTER_T
{
    LOOPERROSTER_HANDLE     hLooperroster;

    pthread_t               my_thread;
    bool                    bExit_thread;

    pthread_mutex_t         my_mutext;

    LOOPER_NODE             *pLooper_header;
    LOOPER_NODE             *pLooper_queue;

    CMDMSG_NODE             *pTx_cmdmsg_header;
    CMDMSG_NODE             *pTx_cmdmsg_queue;
    uint32_t                tx_cmdmsg_num;

    // CMDMSG_NODE             *pResp_cmdmsg_header;
    // CMDMSG_NODE             *pResp_cmdmsg_queue;
    // uint32_t                resp_cmdmsg_num;
}LOOPER_ROSTER;
//=============================================================================
//                Global Data Definition
//=============================================================================
uint32_t  basic_MsgOnFlag = 0x1;
//=============================================================================
//                Private Function Definition
//=============================================================================
DEFINE_QUEUE_TEMPLATE(CMDMSG_NODE);
DEFINE_QUEUE_TEMPLATE(LOOPER_NODE);

static int
_search_looper(
    int             cmpMode,
    LOOPER_NODE     *pNode,
    void            *pattern)
{
    int     result = 0;

    switch( cmpMode )
    {
        case 0:
            result = ((uint32_t)pNode->pHLooper == (uint32_t)pattern) ? 1 : 0;
            break;
        case 1:
            result = (pNode->pHLooper->handler_id == (uint32_t)pattern) ? 1 : 0;
            break;
    }
    return result;
}

static void*
_looper_roster_proc(
    void* arg)
{
    LOOPER_ROSTER   *pLooperroster = (LOOPER_ROSTER*)arg;

    while( pLooperroster->bExit_thread == false )
    {
        if( pLooperroster->pLooper_header && pLooperroster->pLooper_queue &&
            pLooperroster->pTx_cmdmsg_header && pLooperroster->pTx_cmdmsg_queue )
        {
            uint32_t        i = 0;
            CMDMSG_NODE     *pCur_cmdmsg_node = pLooperroster->pTx_cmdmsg_header;
            LOOPER_NODE     *pCur_looper_node = 0;
            CMD_ARG         *pCmd_arg = 0;

            b_mutex_lock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);

            pLooperroster->pTx_cmdmsg_header = QUEUE_DEL(CMDMSG_NODE, pCur_cmdmsg_node);
            pLooperroster->tx_cmdmsg_num--;
            if( pLooperroster->tx_cmdmsg_num < 0 )
                pLooperroster->tx_cmdmsg_num = 0;

            if( !pLooperroster->pTx_cmdmsg_header )
                pLooperroster->pTx_cmdmsg_queue = 0;

            //-------------------------------
            // send cmd msg to handlers
            pCmd_arg = &pCur_cmdmsg_node->cmd_box.cmd_arg;
            /*******************
             * To Do: How to make that the current looper actually ready
             *        and then send to the next looper ?
             ******************/
            switch( pCmd_arg->cmdmsg_send_order )
            {
                case CMDMSG_SENDORDER_FIFO: // the first node first
                    pCur_looper_node = pLooperroster->pLooper_header;
                    while( pCur_looper_node )
                    {
                        LOOPER_HANDLE       *pCur_HLooper = pCur_looper_node->pHLooper;
                        if( BOP_IS_SET(&pCmd_arg->receiver, pCur_HLooper->handler_id) )
                        {
                            looper_Post_CmdMsg(pCur_HLooper, &pCur_cmdmsg_node->cmd_box);
                        }

                        pCur_looper_node = pCur_looper_node->next;
                    }
                    break;

                case CMDMSG_SENDORDER_LIFO: // the last node first
                    pCur_looper_node = QUEUE_FIND_TAIL(LOOPER_NODE, pLooperroster->pLooper_header);
                    while( pCur_looper_node )
                    {
                        LOOPER_HANDLE       *pCur_HLooper = pCur_looper_node->pHLooper;
                        if( BOP_IS_SET(&pCmd_arg->receiver, pCur_HLooper->handler_id) )
                        {
                            looper_Post_CmdMsg(pCur_HLooper, &pCur_cmdmsg_node->cmd_box);
                        }

                        pCur_looper_node = pCur_looper_node->prev;
                    }
                    break;

                case CMDMSG_SENDORDER_CUSTOMER:
                    for(i = 0; i < pCmd_arg->order_customer_length; i++)
                    {
                        pCur_looper_node = QUEUE_FIND(LOOPER_NODE, _search_looper,
                                                      1, pLooperroster->pLooper_header, (uint32_t)pCmd_arg->pOrder_customer[i]);
                        if( pCur_looper_node )
                        {
                            LOOPER_HANDLE       *pCur_HLooper = pCur_looper_node->pHLooper;

                            looper_Post_CmdMsg(pCur_HLooper, &pCur_cmdmsg_node->cmd_box);
                        }
                    }
                    break;
            }

            //-------------------------------
            free(pCur_cmdmsg_node);

            b_mutex_unlock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);

            usleep(10000);
        }
        else
            usleep(20000);
    }

    printf(" exit %s()\n", __FUNCTION__);
    b_thread_exit(NULL);
    return 0;
}
//=============================================================================
//                Public Function Definition
//=============================================================================
BERR
looperroster_CreateHandle(
    LOOPERROSTER_HANDLE **ppHLooperroster,
    void                *extraData)
{
    BERR        result = BERR_OK;

    do{
        LOOPER_ROSTER   *pLooperroster = 0;

        if( !ppHLooperroster || *ppHLooperroster != 0 )
        {
            b_msg_ex(B_MSG_ERR, "error, wrong parameters !!");
            result = BERR_INVALID_PARAMETER;
            break;
        }

        pLooperroster = b_malloc(sizeof(LOOPER_ROSTER));
        if( !pLooperroster )
        {
            b_msg_ex(B_MSG_ERR, "error, allocate fail !!");
            result = BERR_ALLOC_FAILE;
            break;
        }
        memset(pLooperroster, 0x0, sizeof(LOOPER_ROSTER));

        //----------------------
        // init parameters
        pLooperroster->bExit_thread = false;

        //----------------------
        b_thread_create(&pLooperroster->my_thread, NULL, _looper_roster_proc, (void*)pLooperroster);

        b_mutex_init(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);
        //----------------------
        (*ppHLooperroster) = &pLooperroster->hLooperroster;

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}

BERR
looperroster_DestroyHandle(
    LOOPERROSTER_HANDLE     **ppHLooperroster,
    void                    *extraData)
{
    BERR                result = BERR_OK;
    pthread_mutex_t     tmp_mutex = 0;

    do{
        LOOPER_ROSTER   *pLooperroster = 0;
        LOOPER_NODE     *pCur_looper_node = 0;
        CMDMSG_NODE     *pCur_cmdmsg_node = 0;

        if( !ppHLooperroster || !(*ppHLooperroster) )   break;

        pLooperroster = DOWN_CAST(LOOPER_ROSTER, (*ppHLooperroster), hLooperroster);

        pLooperroster->bExit_thread = true;

        b_thread_join(pLooperroster->my_thread, NULL);

        b_mutex_lock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);
        _disable_irq();

        //----------------------
        // free looper
        pCur_looper_node = pLooperroster->pLooper_header;
        while( pCur_looper_node )
        {
            pLooperroster->pLooper_header = QUEUE_DEL(LOOPER_NODE, pCur_looper_node);
            free(pCur_looper_node);

            pCur_looper_node = pLooperroster->pLooper_header;
        }
        pLooperroster->pLooper_queue = 0;

        // free cmd msg
        pCur_cmdmsg_node = pLooperroster->pTx_cmdmsg_header;
        while( pCur_cmdmsg_node )
        {
            pLooperroster->pTx_cmdmsg_header = QUEUE_DEL(CMDMSG_NODE, pCur_cmdmsg_node);
            if( pCur_cmdmsg_node->cmd_box.cmd_arg.pOrder_customer )
                free(pCur_cmdmsg_node->cmd_box.cmd_arg.pOrder_customer);

            free(pCur_cmdmsg_node);

            pCur_cmdmsg_node = pLooperroster->pTx_cmdmsg_header;
        }
        pLooperroster->pTx_cmdmsg_queue = 0;

        //----------------------
        tmp_mutex = pLooperroster->my_mutext;

        free(pLooperroster);

        (*ppHLooperroster) = 0;
        b_mutex_unlock(B_MSG_TRACE_LOOPER_ROSTER, tmp_mutex);
        b_mutex_deinit(B_MSG_TRACE_LOOPER_ROSTER, tmp_mutex);

        _enable_irq();
    }while(0);

    return result;
}

BERR
looperroster_Add_Looper(
    LOOPERROSTER_HANDLE         *pHLooperroster,
    LOOPERROSTER_LOOPER_INFO    *pLooper_info)
{
    BERR            result = BERR_OK;
    LOOPER_ROSTER   *pLooperroster = 0;

    b_verify_handle(pHLooperroster, result);

    pLooperroster = DOWN_CAST(LOOPER_ROSTER, pHLooperroster, hLooperroster);

    b_mutex_lock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);

    do{
        LOOPER_NODE     *pNew_looper_node = 0;

        if( !pLooper_info )     break;

        pNew_looper_node = b_malloc(sizeof(LOOPER_NODE));
        if( !pNew_looper_node )
        {
            b_msg_ex(B_MSG_ERR, "error, allocate fail !!");
            result = BERR_ALLOC_FAILE;
            break;
        }
        memset(pNew_looper_node, 0x0, sizeof(LOOPER_NODE));

        QUEUE_INIT(pNew_looper_node);

        pNew_looper_node->pHLooper        = pLooper_info->pHLooper;
        pNew_looper_node->pTunnel_Info[0] = pLooper_info->pTunnel_Info[0];
        pNew_looper_node->pTunnel_Info[1] = pLooper_info->pTunnel_Info[1];

        if( !pLooperroster->pLooper_header )
        {
            pLooperroster->pLooper_header = pNew_looper_node;
            pLooperroster->pLooper_queue  = pNew_looper_node;
        }
        else
        {
            pLooperroster->pLooper_queue = QUEUE_ADD(LOOPER_NODE, pLooperroster->pLooper_queue, pNew_looper_node);
        }
    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    b_mutex_unlock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);
    return result;
}

BERR
looperroster_Del_Looper(
    LOOPERROSTER_HANDLE         *pHLooperroster,
    LOOPERROSTER_LOOPER_INFO    *pLooper_info)
{
    BERR            result = BERR_OK;
    LOOPER_ROSTER   *pLooperroster = 0;

    b_verify_handle(pHLooperroster, result);

    pLooperroster = DOWN_CAST(LOOPER_ROSTER, pHLooperroster, hLooperroster);

    b_mutex_lock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);

    do{
        LOOPER_NODE     *pDel_looper_node = 0;

        if( !pLooper_info )     break;

        pDel_looper_node = QUEUE_FIND(LOOPER_NODE, _search_looper,
                                   0, pLooperroster->pLooper_header, (uint32_t)pLooper_info->pHLooper);
        if( !pDel_looper_node )
            break;

        pLooperroster->pLooper_header = QUEUE_DEL(LOOPER_NODE, pDel_looper_node);
        pLooperroster->pLooper_queue  = QUEUE_FIND_TAIL(LOOPER_NODE, pLooperroster->pLooper_header);

        free(pDel_looper_node);
    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    b_mutex_unlock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);
    return result;
}

BERR
looperroster_Post_CmdMsg(
    LOOPERROSTER_HANDLE         *pHLooperroster,
    LOOPERROSTER_CMDMSG_INFO    *pCmdmsg_info)
{
    BERR            result = BERR_OK;
    LOOPER_ROSTER   *pLooperroster = 0;

    b_verify_handle(pHLooperroster, result);

    pLooperroster = DOWN_CAST(LOOPER_ROSTER, pHLooperroster, hLooperroster);

    b_mutex_lock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);

    do{
        CMDMSG_NODE     *pNew_cmdmsg_node = 0;
        CMDMSG_NODE     **ppAct_cmdmsg_head = 0;
        CMDMSG_NODE     **ppAct_cmdmsg_queue = 0;
        uint32_t        *pCmdmsg_num = 0;
        CMD_ARG         *pRecv_cmd_arg = 0;


        if( !pCmdmsg_info )     break;

        if( pLooperroster->tx_cmdmsg_num > MAX_CMDMSG_QUEUE_SIZE )
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

        pNew_cmdmsg_node->cmd_box = pCmdmsg_info->cmd_box;

        pRecv_cmd_arg = &pCmdmsg_info->cmd_box.cmd_arg;

        if( pRecv_cmd_arg->cmdmsg_send_order == CMDMSG_SENDORDER_CUSTOMER &&
            pRecv_cmd_arg->pOrder_customer && pRecv_cmd_arg->order_customer_length )
        {
            CMD_ARG         *pTmp_cmd_arg = &pNew_cmdmsg_node->cmd_box.cmd_arg;

            pTmp_cmd_arg->pOrder_customer = b_malloc(sizeof(uint32_t)*pRecv_cmd_arg->order_customer_length);
            if( !pTmp_cmd_arg->pOrder_customer )
            {
                b_msg_ex(B_MSG_ERR, "error, allocate fail !!");
                result = BERR_ALLOC_FAILE;
                break;
            }
            memcpy(pTmp_cmd_arg->pOrder_customer,
                   pRecv_cmd_arg->pOrder_customer,
                   pRecv_cmd_arg->order_customer_length);
        }

        switch( pCmdmsg_info->type )
        {
            case LOOPERROSTER_CMDMSG_APPT:
                ppAct_cmdmsg_head  = &pLooperroster->pTx_cmdmsg_header;
                ppAct_cmdmsg_queue = &pLooperroster->pTx_cmdmsg_queue;
                pCmdmsg_num        = &pLooperroster->tx_cmdmsg_num;
                break;

            // case LOOPERROSTER_CMDMSG_RESP:
            //     ppAct_cmdmsg_head  = &pLooperroster->pResp_cmdmsg_header;
            //     ppAct_cmdmsg_queue = &pLooperroster->pResp_cmdmsg_queue;
            //     pCmdmsg_num        = &pLooperroster->resp_cmdmsg_num;
            //     break;

            // case LOOPERROSTER_CMDMSG_ER: break;
        }

        if( !ppAct_cmdmsg_head || !ppAct_cmdmsg_queue )     break;

        if( !(*ppAct_cmdmsg_head) )
        {
            (*ppAct_cmdmsg_head)  = pNew_cmdmsg_node;
            (*ppAct_cmdmsg_queue) = pNew_cmdmsg_node;
            (*pCmdmsg_num)        = 1;
        }
        else
        {
            (*ppAct_cmdmsg_queue) = QUEUE_ADD(CMDMSG_NODE, (*ppAct_cmdmsg_queue), pNew_cmdmsg_node);
            (*pCmdmsg_num)++;
        }
    }while(0);

    if( result != BERR_OK )
    {
        if( pNew_cmdmsg_node->cmd_box.cmd_arg.pOrder_customer )
            free(pNew_cmdmsg_node->cmd_box.cmd_arg.pOrder_customer);

        if( pNew_cmdmsg_node )  free(pNew_cmdmsg_node);

        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    b_mutex_unlock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);
    return result;
}


/*
BERR
looperroster_tamplete(
    LOOPERROSTER_HANDLE *pHLooperroster,
    void        *extraData)
{
    BERR            result = BERR_OK;
    LOOPER_ROSTER   *pLooperroster = 0;

    b_verify_handle(pHLooperroster, result);

    pLooperroster = DOWN_CAST(LOOPER_ROSTER, pHLooperroster, hLooperroster);

    b_mutex_lock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);

    do{

    }while(0);

    if( result != BERR_OK )
    {
        b_msg_ex(B_MSG_ERR, "err 0x%x !", result);
    }

    b_mutex_unlock(B_MSG_TRACE_LOOPER_ROSTER, pLooperroster->my_mutext);
    return result;
}
*/