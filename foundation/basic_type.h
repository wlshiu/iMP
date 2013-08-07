/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file basic_type.h
 *
 * @author Wei-Lun Hsu
 * @version 0.1
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __basic_type_H_wFDfsUAY_lNVv_HxUI_szv9_uVmtS3b4OJKN__
#define __basic_type_H_wFDfsUAY_lNVv_HxUI_szv9_uVmtS3b4OJKN__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHECK_MEM_LEAK_ENABLE
    #define CHECK_MEM_LEAK_ENABLE       1
#endif

#if (_MSC_VER && CHECK_MEM_LEAK_ENABLE)
    #define _CRTDBG_MAP_ALLOC
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>

#include "itp_posix_win32.h"

#if (_MSC_VER && CHECK_MEM_LEAK_ENABLE)
    #include <crtdbg.h>
#endif
//=============================================================================
//                Constant Definition
//=============================================================================
/**
 * basic error type
 **/
#define BERR_NORMAL_BASE        0xbe000000
typedef enum BERR_T
{
    BERR_OK                 = 0,
    BERR_NULL_PTR           = (BERR_NORMAL_BASE | 0x00000001),
    BERR_ALLOC_FAILE        = (BERR_NORMAL_BASE | 0x00000002),
    BERR_INVALID_PARAMETER  = (BERR_NORMAL_BASE | 0x00000003),

}BERR;

#define ENABLE_PTHREAD          1
//=============================================================================
//                Macro Definition
//=============================================================================
#ifndef MEMBER_OFFSET
    #define MEMBER_OFFSET(type, member)     (uint32_t)&(((type *)0)->member)
#endif

#ifndef DOWN_CAST
    #define DOWN_CAST(type, ptr, member)    (type*)((uint32_t)ptr - MEMBER_OFFSET(type, member))
#endif

#ifndef ARRAY_SIZE
    #define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

//----------------------------
/**
 * bit field pull high/low
 */
#define DEFINE_BIT_OP(bit_size)\
    typedef struct ZONE_SET_T{\
        unsigned short bits_field[((bit_size)+0xF)>>4];\
    }ZONE_SET;

#define BOP_SET(pZone_set_member, bit_order)     ((pZone_set_member)->bits_field[(bit_order)>>4] |=  (1<<((bit_order)&0xF)))
#define BOP_CLR(pZone_set_member, bit_order)     ((pZone_set_member)->bits_field[(bit_order)>>4] &= ~(1<<((bit_order)&0xF)))
#define BOP_IS_SET(pZone_set_member, bit_order)  ((pZone_set_member)->bits_field[(bit_order)>>4] &   (1<<((bit_order)&0xF)))
#define BOP_ZERO(pZone_set_member)               memset((void*)(pZone_set_member),0,sizeof(ZONE_SET))
#define BOP_ALL_SET(pZone_set_member)            memset((void*)(pZone_set_member),0xFF,sizeof(ZONE_SET))

#define BIT_OP_T    ZONE_SET

/**
 * irq fucntion
 **/
#if 1 //(ENABLE_SW_SIMULATION)
    #define _disable_irq()
    #define _enable_irq()
#else
    #include "ite/ith.h"
    #include "openrtos/FreeRTOS.h"
    #define _disable_irq()            portENTER_CRITICAL()
    #define _enable_irq()             portEXIT_CRITICAL()
#endif

/**
 *  Debug flag
 */
extern uint32_t  basic_MsgOnFlag;

typedef enum B_MSG_TYPE
{
    B_MSG_ERR                   = (0x1 << 0),

    B_MSG_TRACE_LOOPER_ROSTER   = (0X1 << 8),
    B_MSG_TRACE_LOOPER          = (0X1 << 9),
}B_MSG_TYPE;

/**
 * debug message fucntion
 **/
#ifdef _MSC_VER // WIN32
    #ifndef trace
    #define trace(string, ...)                  do{ printf(string, __VA_ARGS__); \
                                                    printf("  %s [#%d]\n", __FILE__, __LINE__); \
                                                }while(0)
    #endif

    #define b_msg(type, string, ...)            ((void)((type & basic_MsgOnFlag) ? printf(string, __VA_ARGS__) : 0))
    #define b_msg_ex(type, string, ...)         do{ if(type & basic_MsgOnFlag){ \
                                                    printf(string, __VA_ARGS__); \
                                                    printf("  %s [#%d]\n", __FILE__, __LINE__);} \
                                                }while(0)

    #define _b_thread_pid()                     -1

#else /* _MSC_VER */
    #ifndef trace
    #define trace(string, args...)              do{ printf(string, ## args); \
                                                    printf("  %s [#%d]\n", __FILE__, __LINE__); \
                                                }while(0)
    #endif

    #define b_msg(type, string, args...)        ((void)((type & basic_MsgOnFlag) ? printf(string, ## args) : 0))
    #define b_msg_ex(type, string, args...)     do{ if(type & basic_MsgOnFlag){ \
                                                       printf(string, ## args); \
                                                       printf("  %s [#%d]\n", __FILE__, __LINE__);} \
                                                }while(0)

    #define _b_thread_pid()                     getpid()
#endif

#define b_enable_msg(type)                (basic_MsgOnFlag |= type)
#define b_disable_msg(type)               (basic_MsgOnFlag &= ~(type))

/**
 * pthread fucntion
 **/
#if (ENABLE_PTHREAD)
    #include "pthread.h"

    #define b_thread_create(pThread, pAttr, routine, arg)   pthread_create(pThread, pAttr, routine, arg)
    #define b_thread_join(thread, ppRetval)                 pthread_join(thread, ppRetval)
    #define b_thread_exit(retval)                           pthread_exit(retval)

    #define b_mutex_init(type, mutex)                       do{ if(!mutex){\
                                                                pthread_mutex_init(&mutex, NULL);\
                                                                /*printf(" mutex_init: %s, 0x%x\n", #mutex, mutex);*/}\
                                                            }while(0)
    #define b_mutex_deinit(type, mutex)                     do{if(mutex){pthread_mutex_destroy(&mutex);mutex=0;}}while(0)
    #define b_mutex_lock(type, mutex)                       do{ if(mutex){\
                                                                /*printf("  lock %s\n", __FUNCTION__);*/\
                                                                pthread_mutex_lock(&mutex);}\
                                                            }while(0)
    #define b_mutex_unlock(type, mutex)                     do{ if(mutex){\
                                                                pthread_mutex_unlock(&mutex);\
                                                                /*printf("  unlock %s\n", __FUNCTION__);*/}\
                                                            }while(0)
#else
    #define pthread_t                 uint32_t
    #define pthread_attr_t            uint32_t
    #define pthread_mutex_t           uint32_t

    #define b_thread_create(thread, pAttr, routine, arg)    0xdeadbeef
    #define b_thread_join(thread, ppRetval)
    #define b_thread_exit(retval)
    #define b_mutex_init(type, mutex)
    #define b_mutex_deinit(type, mutex)
    #define b_mutex_lock(type, mutex)
    #define b_mutex_unlock(type, mutex)
#endif

/**
 * handle check fucntion
 **/
#define b_verify_handle(handle, err_code)   do{if(!handle){\
                                              b_msg_ex(1, "%s Null pointer !!", #handle);\
                                              return err_code;}\
                                           }while(0)

/**
 * Handle malloc memory. Compiler allow size = 0, but our sysetm can't.
 **/
#define b_malloc(size)                    ((size) ? malloc(size) : NULL)
//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================
#if 0 //(_MSC_VER)
    // measure time
    #define b_get_clock(pStartTime)       0
    #define b_get_duration(pStartTime)    (-1)

#else
    static void
    b_get_clock(
        struct timeval *startT)
    {
        gettimeofday(startT, NULL);
    }

    static uint32_t
    b_get_duration(
        struct timeval *startT)
    {
        struct timeval currT = {0};
        time_t  duration_time = 0;

        gettimeofday(&currT, NULL);
        duration_time = (currT.tv_sec - startT->tv_sec) * 1000;      // sec to ms
        // duration_time += ((currT.tv_usec - startT->tv_usec) / 1000); // us to ms
        return (uint32_t)duration_time;
    }
#endif
//=============================================================================
//                Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif
