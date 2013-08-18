/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file test_foundation.c
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

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================
static void
test_foundation()
{
#define LOOPER_NUM      3
    uint32_t                i = 0;
    LOOPERROSTER_HANDLE     *pHLooperRoster = 0;
    CMDMSG_HANDLE           *pHCmdmsg = 0;
    CMDMSG_INIT_INFO        cmdmsg_init_info = {0};
    LOOPER_HANDLE           *pHLooper[LOOPER_NUM] = {0};
    LOOPER_INIT_INFO        looper_init_info = {0};

    //-----------------------------
    // create looper_roster
    looperroster_CreateHandle(&pHLooperRoster, 0);

    //-----------------------------
    // assign looper_roster to cmd_msg and create cmd_msg
    cmdmsg_init_info.pHLooperroster = (void*)pHLooperRoster;
    cmdmsg_CreateHandle(&pHCmdmsg, &cmdmsg_init_info);

    for(i = 0; i < LOOPER_NUM; i++)
    {
        LOOPERROSTER_LOOPER_INFO    looperroster_looper_info = {0};
        //-----------------------------
        // create looper
        looper_CreateHandle(&pHLooper[i], &looper_init_info);
            // attach pHCmdmsg to looper
            // register handler to looper

        //-----------------------------
        // add looper to looper_roster
        looperroster_looper_info.pHLooper = pHLooper[i];
        looperroster_Add_Looper(pHLooperRoster, &looperroster_looper_info);
    }

    //-----------------------------
    // free hanlde
    cmdmsg_DestroyHandle(&pHCmdmsg, 0);
    looperroster_DestroyHandle(&pHLooperRoster, 0);

}
//=============================================================================
//                Public Function Definition
//=============================================================================
void main(void)
{

#ifdef PTW32_STATIC_LIB
    pthread_win32_process_attach_np();
#endif

    ///////////////////////////////
    // Do Test
    test_foundation();

#ifdef PTW32_STATIC_LIB
    pthread_win32_process_detach_np();
#endif
    //-----------------------------------
#if (_MSC_VER)
    _CrtDumpMemoryLeaks();
#endif
    printf("============ end =================\n");
    while(1);
    return ;
}

