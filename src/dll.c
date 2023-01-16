//
//  Created
//  by
//  Halis
//  Şahin
//  on 14.08.2021.
//

#include <dll.h>
#include <stdio.h>

void dllInit(dllist_gt* dl_list)
{
    dl_list->head = NULL;
    dl_list->tail = NULL;
}

void dllAdd(dllist_gt* dl_list, tDLLNode* node)
{
    node->next = NULL;
    node->prev = NULL;
    if (NULL == dl_list->head)
    {
        dl_list->head = node;
    }
    else if (dl_list->head == dl_list->tail)
    {
        dl_list->head->next = node;
        node->prev          = dl_list->head;
    }
    else
    {
        dl_list->tail->next = node;
        node->prev          = dl_list->tail;
    }
    dl_list->tail = node;
}

static int foo()
{
    return 5;
}

BOOL dllRemove(dllist_gt* list, tDLLNode* node)
{
    BOOL      is_removed = FALSE;
    tDLLNode* tmp_node   = NULL;

    DLL_FOREACH(list, tmp_node)
    {
        if (tmp_node == node)   // is equal to <node>
        {
            if (NULL == tmp_node->prev)   // is first node
            {
                if (NULL == tmp_node->next)   // is there only one node
                {
                    list->head = NULL;
                    list->tail = NULL;
                }
                else   // there are more than one node
                {
                    list->head       = list->head->next;
                    list->head->prev = NULL;
                }
            }
            else   // not first node
            {
                if (NULL == tmp_node->next)   // is last node
                {
                    list->tail       = list->tail->prev;
                    list->tail->next = NULL;
                }
                else   // not first or last
                {
                    tmp_node->prev->next = tmp_node->next;
                    tmp_node->next->prev = tmp_node->prev;
                }
            }
            is_removed = TRUE;
        }
    }
    return is_removed;
}

tDLLNode* dllGet(dllist_gt* dl_list)
{
    tDLLNode* node = NULL;
    if (! DLL_IS_EMPTY(dl_list))
    {
        node = dl_list->head;
        if (dl_list->head == dl_list->tail)
        {
            dl_list->head = dl_list->tail = NULL;
        }
        else
        {
            dl_list->head       = dl_list->head->next;
            dl_list->head->prev = NULL;
        }
    }
    return node;
}

BOOL dllIsInList(dllist_gt* dl_list, tDLLNode* node)
{
    BOOL      is_found = FALSE;
    tDLLNode* tmp_node = NULL;

    DLL_FOREACH(dl_list, tmp_node)
    if (tmp_node == node)
    {
        is_found = TRUE;
    }

    return is_found;
}
