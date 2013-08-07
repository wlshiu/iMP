/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file queue_template.h
 *
 * @author Wei-Lun Hsu
 * @version 1.0
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __queue_template_H_wFDg5E3w_lRjV_Hm29_svnq_u13QHFvLfbRv__
#define __queue_template_H_wFDg5E3w_lRjV_Hm29_svnq_u13QHFvLfbRv__

#ifdef __cplusplus
extern "C" {
#endif

#define DEFINE_QUEUE_TEMPLATE(type)                         \
    static type*                                                  \
    queue_find_##type##_node(                                     \
        int (*compare)(int cmpMode,                               \
                       type *pNode,void *pattern),                \
        int cmpMode,type *pNode,void *pattern){                   \
        type    *pCurNode = pNode;                                \
        type    *pEndNode = pNode;                                \
        while(pCurNode){                                          \
            if( compare(cmpMode, pCurNode, pattern) )             \
                return pCurNode;                                  \
            pCurNode = pCurNode->next;                            \
            if(pCurNode == 0) break;                              \
        }                                                         \
        return 0;                                                 \
    }                                                             \
    static type*                                                  \
    queue_add_##type##_node(                                      \
        type *pCurNode,type *pInsNode){                           \
        if( pCurNode && pInsNode ){                               \
            if( pCurNode->next )                                  \
                pCurNode->next->prev = pInsNode;                  \
            pInsNode->next = pCurNode->next;                      \
            pInsNode->prev = pCurNode;                            \
            pCurNode->next = pInsNode;                            \
            return pInsNode;                                      \
        }                                                         \
        return 0;                                                 \
    }                                                             \
    static type*                                                  \
    queue_find_##type##_head(                                     \
        type *pCurNode){                                          \
        type *pNode = pCurNode;                                   \
        while( pNode && pNode->prev ){                            \
            pNode = pNode->prev;                                  \
        }                                                         \
        return pNode;                                             \
    }                                                             \
    static type*                                                  \
    queue_find_##type##_tail(                                     \
        type *pCurNode){                                          \
        type *pNode = pCurNode;                                   \
        while( pNode && pNode->next ){                            \
            pNode = pNode->next;                                  \
        }                                                         \
        return pNode;                                             \
    }                                                             \
    static type*                                                  \
    queue_del_##type##_node(                                      \
        type *pCurNode){                                          \
        if( pCurNode ){                                           \
            if( pCurNode->prev )                                  \
                pCurNode->prev->next = pCurNode->next;            \
            if( pCurNode->next )                                  \
                pCurNode->next->prev = pCurNode->prev;            \
            if( pCurNode->prev )                                  \
                return queue_find_##type##_head(pCurNode->prev);  \
            else if( pCurNode->next )                             \
                return queue_find_##type##_head(pCurNode->next);  \
        }                                                         \
        return 0;                                                 \
    }                                                             \


#define QUEUE_INIT(ptr)                                      do{(ptr)->next = 0;(ptr)->prev = 0;}while(0)
#define QUEUE_ADD(type, pCur, pIns)                          queue_add_##type##_node(pCur, pIns)
#define QUEUE_DEL(type, pCur)                                queue_del_##type##_node(pCur)
#define QUEUE_FIND_HEAD(type, pCur)                          queue_find_##type##_head(pCur)
#define QUEUE_FIND_TAIL(type, pCur)                          queue_find_##type##_tail(pCur)
#define QUEUE_FIND(type, cmpFunc, cmpMode, pStart, pattern)  queue_find_##type##_node(cmpFunc, cmpMode, pStart, (void*)pattern)


#ifdef __cplusplus
}
#endif

#endif
