/*
* Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
*/
/** @file ite_media_player.c
*
* @author Wei-Lun Hsu
* @version 0.1
* @date 2013/08/03
* @license
* @description
*/


#include "i_media_player.h"
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
typedef struct IMP_DEV_T
{
    IMP_HANDLE      hImp;
}IMP_DEV;
//=============================================================================
//                Global Data Definition
//=============================================================================
uint32_t  impMsgOnFlag = 0x1;
//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
IMP_ERR
imp_CreateHandle(
    IMP_HANDLE  **ppHImp,
    void        *extraData)
{
    IMP_ERR     result = IMP_ERR_OK;

    do{
        IMP_DEV     *pImpDev = 0;

        if( *ppHImp != 0 )
        {
            printf("error, Exist imp handle !!");
            break;
        }

        //----------------------
        (*ppHImp) = &pImpDev->hImp;

    }while(0);

    if( result != IMP_ERR_OK )
    {
        printf("%s, err 0x%x !", __FUNCTION__, result);
    }

    return result;
}

IMP_ERR
imp_DestroyHandle(
    IMP_HANDLE  **ppHImp,
    void        *extraData)
{
    IMP_ERR     result = IMP_ERR_OK;


    return result;
}

/*
IMP_ERR
imp_tamplete(
    IMP_HANDLE  *pHImp,
    void        *extraData)
{
    IMP_ERR     result = IMP_ERR_OK;


    if( result != IMP_ERR_OK )
    {
        printf("%s, err 0x%x !", __FUNCTION__, result);
    }

    return result;
}
*/