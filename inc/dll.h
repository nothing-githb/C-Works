//
// Created
// by
// Halis
// Şahin
// on 14.08.2021.
//

#ifndef C_ALGORITHMS_DLL_H
#define C_ALGORITHMS_DLL_H

#include "types.h"

#define DLL_INIT(list)         (dllInit(list))

#define DLL_ADD(list, node)    (dllAdd(list, node))

#define DLL_REMOVE(list, node) (dllRemove(list, node))

#define DLL_GET(list)          (dllGet(list))

#define DLL_IS_EMPTY(list)     (NULL == DLL_FIRST(list))

#define DLL_FIRST(list)        ((tDLLNode*) (list)->head)

#define DLL_NEXT(node)         ((node)->next)

#define DLL_FOREACH(list, tmpNode)                                             \
    for (tmpNode = DLL_FIRST(list); NULL != tmpNode;                           \
         tmpNode = DLL_NEXT((tDLLNode*) tmpNode))

typedef struct dllNode
{
    struct dllNode* next;
    struct dllNode* prev;
} tDLLNode;

typedef struct
{
    tDLLNode* head;
    tDLLNode* tail;
} dllist_gt;

void      dllInit(dllist_gt* dl_list);

void      dllAdd(dllist_gt* dl_list, tDLLNode* node);

BOOL      dllRemove(dllist_gt* dl_list, tDLLNode* node);

tDLLNode* dllGet(dllist_gt* dl_list);

BOOL      dllIsInList(dllist_gt* dl_list, tDLLNode* node);

#endif   // C_ALGORITHMS_DLL_H
