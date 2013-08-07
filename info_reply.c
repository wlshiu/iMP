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

#include "info_reply.h"
#include "media_player_def.h"
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct INFO_REPLY_T
{
    INFOREPLY_HANDLE    hInforeply;
}INFO_REPLY;
//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
IMP_ERR
inforeply_CreateHandle(
    INFOREPLY_HANDLE    **ppHInforeply,
    void                *extraData)
{
    IMP_ERR     result = IMP_ERR_OK;

    do{
        INFO_REPLY      *pInforeply = 0;

        if( *ppHInforeply != 0 )
        {
            imp_msg_ex(IMP_MSG_ERR, "error, Exist inforeply handle !!");
            result = IMP_INVALID_PARAMETER;
            break;
        }

        //----------------------
        (*ppHInforeply) = &pInforeply->hInforeply;

    }while(0);

    if( result != IMP_ERR_OK )
    {
        imp_msg_ex(IMP_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}

IMP_ERR
inforeply_DestroyHandle(
    INFOREPLY_HANDLE    **ppHInforeply,
    void        *extraData)
{
    IMP_ERR     result = IMP_ERR_OK;


    return result;
}

/*
IMP_ERR
inforeply_tamplete(
    INFOREPLY_HANDLE    *pHInforeply,
    void        *extraData)
{
    IMP_ERR     result = IMP_ERR_OK;


    if( result != IMP_ERR_OK )
    {
        imp_msg_ex(IMP_MSG_ERR, "err 0x%x !", result);
    }

    return result;
}
*/