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

#ifndef __media_player_err_H_wtVKremqC_lgn4_HzeK_sxDR_uoWhjmCrbEkW__
#define __media_player_err_H_wtVKremqC_lgn4_HzeK_sxDR_uoWhjmCrbEkW__

#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
//                Constant Definition
//=============================================================================
/**
 * basic error type
 **/
#define IMP_NORMAL_BASE         0xED000000
typedef enum IMP_ERR_T
{
    IMP_ERR_OK             = 0,
    IMP_NULL_PTR           = (IMP_NORMAL_BASE | 0x00000001),
    IMP_ALLOC_FAILE        = (IMP_NORMAL_BASE | 0x00000002),
    IMP_INVALID_PARAMETER  = (IMP_NORMAL_BASE | 0x00000003),
}IMP_ERR;
//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
