/*
 * Copyright (c) 2013 Wei-Lun Hsu. All Rights Reserved.
 */
/** @file list_template.h
 *
 * @author Wei-Lun Hsu
 * @version 1.0
 * @date 2013/08/03
 * @license
 * @description
 */

#ifndef __list_template_H_wWJGN9wp_lAs8_HafX_svgy_uVXWVeJU7d38__
#define __list_template_H_wWJGN9wp_lAs8_HafX_svgy_uVXWVeJU7d38__

#ifdef __cplusplus
extern "C" {
#endif

#define DEFINE_LIST_TEMPLATE(type)                         \
    typedef int (*compare)(int cmpMode,                       \
                           type *pNode,void *pattern);        \
    static type*                                              \
    list_find_##type##_node(                                  \
        int (*compare)(int cmpMode,                           \
                       type *pNode,void *pattern),            \
        int cmpMode,type *pNode,void *pattern){               \
        type    *pCurNode = pNode;                            \
        type    *pEndNode = pNode;                            \
        while(pCurNode){                                      \
            if( compare(cmpMode, pCurNode, pattern) )         \
                return pCurNode;                              \
            pCurNode = pCurNode->next;                        \
            if(pCurNode == pEndNode) break;                   \
        }                                                     \
        return 0;                                             \
    }                                                         \
    static type*                                              \
    list_add_##type##_node(                                   \
        type *pCurNode,type *pInsNode){                       \
        if( pCurNode && pInsNode ){                           \
            pCurNode->next->prev = pInsNode;                  \
            pInsNode->next = pCurNode->next;                  \
            pInsNode->prev = pCurNode;                        \
            pCurNode->next = pInsNode;                        \
            return pInsNode;                                  \
        }                                                     \
        return 0;                                             \
    }                                                         \
    static type*                                              \
    list_del_##type##_node(                                   \
        type *pCurNode){                                      \
        if( pCurNode ){                                       \
            pCurNode->prev->next = pCurNode->next;            \
            pCurNode->next->prev = pCurNode->prev;            \
            if( pCurNode->prev != pCurNode )                  \
                return pCurNode->prev;                        \
        }                                                     \
        return 0;                                             \
    }                                                         \
    static void                                               \
    list_swap_##type##_node(                                  \
        type *pNode1, type *pNode2){                          \
        pNode2->next->prev = pNode1->next;                    \
        pNode1->next->prev = pNode2;                          \
        pNode1->next = pNode2->next;                          \
        pNode2->next = pNode1->next->prev;                    \
        pNode1->next->prev = pNode1;                          \
        pNode2->prev->next = pNode1;                          \
        pNode1->prev->next = pNode2->prev;                    \
        pNode2->prev = pNode1->prev;                          \
        pNode1->prev = pNode2->prev->next;                    \
        pNode2->prev->next = pNode2;                          \
    }

#define LIST_INIT(ptr)                                      do {(ptr)->next = (ptr); (ptr)->prev = (ptr);} while (0)
#define LIST_ADD(type, pCur, pIns)                          list_add_##type##_node(pCur, pIns)
#define LIST_DEL(type, pCur)                                list_del_##type##_node(pCur)
#define LIST_FIND(type, cmpFunc, cmpMode, pStart, pattern)  list_find_##type##_node(cmpFunc, cmpMode, pStart, (void*)pattern)
#define LIST_SWAP(type, pItm1, pItm2)                       list_swap_##type##_node(pItm1, pItm2)


#ifdef __cplusplus
}
#endif

#endif


/************************************************
// example
struct _node;
typedef struct _node{
    struct _node    *prev, *next;  // MUST
    int             uid;           // MUST

    char            str[32];
}node;

int
_cmpNode(int cmpMode, node *pCur,void *pattern){
    return (pCur->uid == (int)pattern);
}

DEFINE_LIST_TEMPLETE(node);

void
main()
{
    node *first_node, *cur_node, *tmp_node;
    int  i;

    first_node = malloc(sizeof(node));
    first_node->uid = 0;
    tmp_node->str[0] = 'A';
    cur_node = first_node;

    LIST_INIT(cur_node);

    for(i = 0; i < 5; i++){
        tmp_node = malloc(sizeof(node));
        memset(tmp_node, 0, sizeof(node));
        tmp_node->uid = i+1;
        tmp_node->str[0] = 0x42 + i;

        LIST_ADD(node, cur_node, tmp_node);
        cur_node = tmp_node;
    }

    tmp_node = LIST_FIND(node, _cmpNode, 0, cur_node, 3);
    LIST_SWAP(node, first_node, tmp_node);

    for(i = 0; i < 6; i++){
        tmp_node = LIST_FIND(node, _cmpNode, 0, cur_node, i);
        if(tmp_node) printf("find %d, %c\n", i, tmp_node->str[0]);
    }

    for(i = 0; i < 6; i++){
        tmp_node = LIST_FIND(node, _cmpNode, 0, cur_node, i);
        cur_node = LIST_DEL(node, tmp_node);
    }

    for(i = 0; i < 6; i++){
        tmp_node = LIST_FIND(node, _cmpNode, 0, cur_node, i);
        if(tmp_node) printf("find %d, %c\n", i, tmp_node->str[0]);
    }
    return 0;
}
************************************************/

