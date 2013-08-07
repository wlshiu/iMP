

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
    LOOPERROSTER_HANDLE     *pHLooperRoster = 0;
    CMDMSG_HANDLE           *pHCmdmsg = 0;
    CMDMSG_INIT_INFO        cmdmsg_init_info = {0};

    //-----------------------------
    // create looper_roster
    looperroster_CreateHandle(&pHLooperRoster, 0);

    //-----------------------------
    // assign looper_roster to cmd_msg and create cmd_msg
    cmdmsg_init_info.pHLooperroster = (void*)pHLooperRoster;
    cmdmsg_CreateHandle(&pHCmdmsg, &cmdmsg_init_info);

    //-----------------------------
    // create looper
        // attach pHCmdmsg to looper
        // register handler to looper

    //-----------------------------
    // add looper to looper_roster

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

