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


#include "cmd_router.h"
#include "media_player_def.h"
//=============================================================================
//				  Constant Definition
//=============================================================================

//=============================================================================
//				  Macro Definition
//=============================================================================

//=============================================================================
//				  Structure Definition
//=============================================================================
typedef struct CMD_ROUTER_T
{
    CMDROUTER_HANDLE	hCmdrouter;
}CMD_ROUTER;
//=============================================================================
//				  Global Data Definition
//=============================================================================

//=============================================================================
//				  Private Function Definition
//=============================================================================

//=============================================================================
//				  Public Function Definition
//=============================================================================
IMP_ERR
cmdrouter_CreateHandle(
	CMDROUTER_HANDLE	**ppHCmdrouter,
	void		        *extraData)
{
	IMP_ERR		result = IMP_ERR_OK;

	do{
        CMD_ROUTER    *pCmdrouter = 0;

		if( *ppHCmdrouter != 0 )
		{
			imp_msg_ex(IMP_MSG_ERR, "error, Exist cmdrouter handle !!");
			break;
		}

		//----------------------
		(*ppHCmdrouter) = &pCmdrouter->hCmdrouter;

	}while(0);

	if( result != IMP_ERR_OK )
	{
		imp_msg_ex(IMP_MSG_ERR, "err 0x%x !", result);
	}

	return result;
}

IMP_ERR
cmdrouter_DestroyHandle(
	CMDROUTER_HANDLE	**ppHCmdrouter,
	void		*extraData)
{
	IMP_ERR		result = IMP_ERR_OK;


	return result;
}

/*
IMP_ERR
cmdrouter_tamplete(
	CMDROUTER_HANDLE	*pHCmdrouter,
	void		*extraData)
{
	IMP_ERR		result = IMP_ERR_OK;


	if( result != IMP_ERR_OK )
	{
		imp_msg_ex(IMP_MSG_ERR, "err 0x%x !", result);
	}

	return result;
}
*/