//
// Created by Halis Şahin on 14.08.2021.
//

#ifndef C_ALGORITHMS_DLL_H
#define C_ALGORITHMS_DLL_H

#include <types.h>

#define DLL_INIT(list)                  (dllInit(list))

#define DLL_ADD(list, node)             (dllAdd(list, node))

#define DLL_REMOVE(list, node)          (dllRemove(list, node))

#define DLL_GET(list)                   (dllGet(list))

#define DLL_IS_EMPTY(list)              (NULL == DLL_FIRST(list))

#define DLL_FIRST(list)                 ((tDLLNode *)(list)->head)

#define DLL_NEXT(node)                  ((node)->next)

#define DLL_FOREACH(list, tmpNode)       for(tmpNode = DLL_FIRST(list); \
                                                NULL != tmpNode;        \
                                                tmpNode = DLL_NEXT((tDLLNode *)tmpNode))

typedef struct dllNode
{
    struct dllNode *next;
    struct dllNode *prev;
}tDLLNode;

typedef struct
{
    tDLLNode *head;
    tDLLNode *tail;
}tDLList;

void dllInit(tDLList *dlList);

void dllAdd(tDLList *dlList, tDLLNode *node);

BOOL dllRemove(tDLList *dlList, tDLLNode *node);

tDLLNode * dllGet(tDLList *dlList);

BOOL dllIsInList(tDLList *dlList, tDLLNode *node);


#endif //C_ALGORITHMS_DLL_H
