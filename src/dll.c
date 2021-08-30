//
// Created by Halis Şahin on 14.08.2021.
//

#include <stdio.h>
#include <dll.h>

void dllInit(tDLList *dlList)
{
    dlList->head = NULL;
    dlList->tail = NULL;
}

void dllAdd(tDLList *dlList, tDLLNode *node)
{
    node->next = NULL;
    node->prev = NULL;
    if (NULL == dlList->head)
    {
        dlList->head = node;
    }
    else if (dlList->head == dlList->tail)
    {
        dlList->head->next = node;
        node->prev = dlList->head;
    }
    else
    {
        dlList->tail->next = node;
        node->prev = dlList->tail;
    }
    dlList->tail = node;

}

BOOL dllRemove(tDLList *list, tDLLNode *node)
{
    BOOL isRemoved = FALSE;
    tDLLNode *tmpNode;
    DLL_FOREACH(list, tmpNode)
    {
        if (tmpNode == node)    // is equal to <node>
        {
            if (NULL == tmpNode->prev)  // is first node
            {
                 if (NULL == tmpNode->next) // is there only one node
                 {
                     list->head = NULL;
                     list->tail = NULL;
                 }
                 else   // there are more than one node
                 {
                     list->head = list->head->next;
                     list->head->prev = NULL;

                 }
            }
            else    // not first node
            {
                if (NULL == tmpNode->next)  // is last node
                {
                    list->tail = list->tail->prev;
                    list->tail->next = NULL;
                }
                else    // not first or last
                {
                    tmpNode->prev->next = tmpNode->next;
                    tmpNode->next->prev = tmpNode->prev;
                }
            }
            isRemoved = TRUE;
        }
    }
    return isRemoved;
}

tDLLNode * dllGet(tDLList *dlList)
{
    tDLLNode *node = NULL;
    if (!DLL_IS_EMPTY(dlList))
    {
        node = dlList->head;
        if (dlList->head == dlList->tail)
        {
            dlList->head = dlList->tail = NULL;
        }
        else
        {
            dlList->head = dlList->head->next;
            dlList->head->prev = NULL;
        }
    }
    return node;
}

BOOL dllIsInList(tDLList *dlList, tDLLNode *node)
{
    BOOL isFound = FALSE;
    tDLLNode *tmpNode;

    DLL_FOREACH(dlList, tmpNode)
        if (tmpNode == node)
            isFound = TRUE;

    return isFound;
}

