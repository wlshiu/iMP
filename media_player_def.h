/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file media_player_def.h
 *
 * @author Wei-Lun Hsu
 * @version 1.0
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __media_player_def_H_wqWM1j3b_lB0X_HPL9_s8v3_ue6xzp7VvxJy__
#define __media_player_def_H_wqWM1j3b_lB0X_HPL9_s8v3_ue6xzp7VvxJy__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CHECK_MEM_LEAK_ENABLE
    #define CHECK_MEM_LEAK_ENABLE       1
#endif

#if (_MSC_VER && CHECK_MEM_LEAK_ENABLE)
    #define _CRTDBG_MAP_ALLOC
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>

#if (_MSC_VER && CHECK_MEM_LEAK_ENABLE)
    #include <crtdbg.h>
#endif

//=============================================================================
//                Constant Definition
//=============================================================================
#define ENABLE_PTHREAD_MUTEX    1
#define ENABLE_DBG_TRACE        1//0
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


/**
 *  Debug flag
 */
extern uint32_t  impMsgOnFlag;

typedef enum _IMP_MSG_TYPE
{
    IMP_MSG_ERR             = (0x1 << 0),

    IMP_MSG_TRACE           = (0X1 << 8),
}IMP_MSG_TYPE;

/**
 * debug message fucntion
 **/
#ifdef _MSC_VER // WIN32
    #ifndef trace
    #define trace(string, ...)              do{ printf(string, __VA_ARGS__); \
                                                printf("  %s [#%d]\n", __FILE__, __LINE__); \
                                            }while(0)
    #endif

    #define imp_msg(type, string, ...)      ((void)((type & impMsgOnFlag) ? printf(string, __VA_ARGS__) : 0))
    #define imp_msg_ex(type, string, ...)   do{ if(type & impMsgOnFlag){ \
                                                   printf(string, __VA_ARGS__); \
                                                   printf("  %s [#%d]\n", __FILE__, __LINE__);} \
                                            }while(0)
    // #if ENABLE_DBG_TRACE
    //     #define _trace_enter(type, string, ...)    do{imp_msg(type, "enter: %s()[#%d]\n", __FUNCTION__, __LINE__); \
    //                                                   imp_msg(type, string, ...);\
    //                                                 }while(0)
    //                                                 /*do{ if(type & impMsgOnFlag){ \
    //                                                    printf("enter: %s()[#%d]\n", __FUNCTION__, __LINE__);\
    //                                                    printf(string, __VA_ARGS__);}\
    //                                                }while(0)*/
    //     #define _trace_leave(type)                 imp_msg(type, "leave: %s()[#%d]\n", __FUNCTION__, __LINE__);
    // #else /* ENABLE_DBG_TRACE */
    //     #define _trace_enter(string, ...)
    //     #define _trace_leave(type)
    // #endif

    #define _tes_byte_align4                    __attribute__ ((aligned(4)))

    #define _imp_thread_pid()                   -1
#else /* _MSC_VER */
    #ifndef trace
    #define trace(string, args...)              do{ printf(string, ## args); \
                                                    printf("  %s [#%d]\n", __FILE__, __LINE__); \
                                                }while(0)
    #endif

    #define imp_msg(type, string, args...)      ((void)((type & impMsgOnFlag) ? printf(string, ## args) : 0))
    #define imp_msg_ex(type, string, args...)   do{ if(type & impMsgOnFlag){ \
                                                       printf(string, ## args); \
                                                       printf("  %s [#%d]\n", __FILE__, __LINE__);} \
                                                }while(0)

    // #if ENABLE_DBG_TRACE
    //     #define _trace_enter(type, string, args...)    do{ if(type & impMsgOnFlag){ \
    //                                                        printf("  enter: %s()[#%d], ", __FUNCTION__, __LINE__);\
    //                                                        printf(string, ## args);}\
    //                                                    }while(0)
    //     #define _trace_leave(type)                     imp_msg(type, "  leave: %s()[#%d]\n", __FUNCTION__, __LINE__)
    // #else /* ENABLE_DBG_TRACE */
    //     #define _trace_enter(string, args...)
    //     #define _trace_leave(type)
    // #endif

    #define _tes_byte_align4                    __attribute__ ((aligned(4)))

    #define _imp_thread_pid()                   getpid()
#endif

#if ENABLE_DBG_TRACE
        #define _trace_enter(type, string, ...)    do{ imp_msg(type, "  enter: %s()[#%d]\n", __FUNCTION__, __LINE__); \
                                                       imp_msg(type, string, ...);\
                                                   }while(0)
    #define _trace_leave(type)                     imp_msg(type, "  leave: %s()[#%d]\n", __FUNCTION__, __LINE__)
#else /* ENABLE_DBG_TRACE */
    #define _trace_enter(string, args...)
    #define _trace_leave(type)
#endif

#define imp_enable_msg(type)                (impMsgOnFlag |= type)
#define imp_disable_msg(type)               (impMsgOnFlag &= ~(type))

/**
 * pthread fucntion
 **/
#if ENABLE_PTHREAD_MUTEX
    #include "basic_type.h"

    #define _mutex_init(type, mutex)        do{ if(!mutex){\
                                                pthread_mutex_init(&mutex, NULL);\
                                                /*printf(" mutex_init: %s, 0x%x\n", #mutex, mutex);*/}\
                                             }while(0)
    #define _mutex_deinit(type, mutex)      do{if(mutex){pthread_mutex_destroy(&mutex);mutex=0;}}while(0)
    #define _mutex_lock(type, mutex)        do{ if(mutex){\
                                                imp_msg(type, "  lock %s(mutex=0x%x, thread=0x%x)\n", __FUNCTION__, mutex, _imp_thread_pid());\
                                                /*printf("  lock %s\n", __FUNCTION__);*/\
                                                pthread_mutex_lock(&mutex);}\
                                            }while(0)
    #define _mutex_unlock(type, mutex)      do{ if(mutex){\
                                                pthread_mutex_unlock(&mutex);\
                                                imp_msg(type, "  unlock %s(mutex=0x%x, thread=0x%x)\n", __FUNCTION__, mutex, _imp_thread_pid());\
                                                /*printf("  unlock %s\n", __FUNCTION__);*/}\
                                            }while(0)
#else
    #define pthread_mutex_t           uint32_t
    #define _mutex_init(type, mutex)
    #define _mutex_deinit(type, mutex)
    #define _mutex_lock(type, mutex)
    #define _mutex_unlock(type, mutex)
#endif


/**
 * handle check fucntion
 **/
#define _verify_handle(handle, err_code)   do{if(!handle){\
                                              imp_msg_ex(TSE_MSG_ERR, "%s Null pointer !!", #handle);\
                                              return err_code;}\
                                           }while(0)

/**
 * Handle malloc memory. Compiler allow size = 0, but our sysetm can't.
 **/
#define imp_malloc(size)                    ((size) ? malloc(size) : NULL)
//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================
#if (_MSC_VER)
    // measure time
    #define _get_clock(pStartTime)       0
    #define _get_duration(pStartTime)    (-1)

#else
    static void
    _get_clock(
        struct timeval *startT)
    {
        gettimeofday(startT, NULL);
    }

    static uint32_t
    _get_duration(
        struct timeval *startT)
    {
        struct timeval currT = {0};
        uint32_t  duration_time = 0;

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
