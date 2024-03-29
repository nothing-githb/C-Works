//
// Created by Halis Şahin on 15.08.2021.
//

#include "test.h"

#include "binarySearch.h"
#include "dll.h"
#include "randomNG.h"
#include "stdio.h"    // printf
#include "string.h"   // memcpy

void testDLL(BOOL is_print)
{
    typedef struct
    {
        tDLLNode node;
        int      a;
    } tMyNode;

    int       i;
    int       func_rand;
    time_t    t;
    dllist_gt dl_list;
    tDLLNode* dlNode;
    tMyNode*  tmpNode;
    BOOL      isRemoved;

    INIT_RNG(&t);   // Initialize random number generator

    DLL_INIT(&dl_list);

    for (i = 0; i < MAX_NUMBER_OF_TESTS; i++)
    {
        func_rand = rand() % 3;

        switch (func_rand)
        {
            case 0:
                tmpNode    = (tMyNode*) malloc(sizeof(tMyNode));
                tmpNode->a = rand() % 100;
                DLL_ADD(&dl_list, (tDLLNode*) tmpNode);
                if (is_print)
                {
                    printf("DLL added: %d \n", tmpNode->a);
                }
                break;
            case 1:
                tmpNode = (tMyNode*) DLL_GET(&dl_list);
                if (is_print && NULL != tmpNode)
                {
                    printf("DLL get: %d \n", tmpNode->a);
                }
                break;
            case 2:
                isRemoved = DLL_REMOVE(&dl_list, (tDLLNode*) tmpNode);
                if (is_print && isRemoved)
                {
                    printf("DLL removed: %d \n", tmpNode->a);
                }
                break;
        }

        if ((i % 1000 == 0) && is_print)
        {
            DLL_FOREACH(&dl_list, dlNode)
            printf("%d ", ((tMyNode*) dlNode)->a);
        }
    }
}

typedef struct binarySearchNode
{
    int   a;
    short b;
    long  c;
    char  array[30];
} tBinarySearchNode;

inline static int __cmp(void* addr1, void* addr2)
{
    int*               key   = addr1;
    tBinarySearchNode* node2 = (tBinarySearchNode*) addr2;

    if (*key == node2->a)
    {
        return 0;
    }
    else if (*key < node2->a)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void testBinarySearch(BOOL isPrint)
{
    int               i, index, search_index;
    char              name[30];
    tBinarySearchNode tmp_node;

    tBinarySearchNode list[MAX_NUMBER_OF_ARRAY_SIZE];

    for (i = 0; i < MAX_NUMBER_OF_ARRAY_SIZE; i++)
    {
        sprintf(name, "tBinarySearchNode_%d", i);
        memcpy(tmp_node.array, name, 30);
        tmp_node.a = tmp_node.b = tmp_node.c = i;
        list[i]                              = tmp_node;
    }

    for (i = 0; i < MAX_NUMBER_OF_TESTS; i++)
    {
        search_index = rand() % (MAX_NUMBER_OF_ARRAY_SIZE + 100) - 100;
        index        = binarySearch(list,
                             sizeof(tBinarySearchNode),
                             MAX_NUMBER_OF_ARRAY_SIZE,
                             &search_index,
                             __cmp);

        if (isPrint)
        {
            if (-1 != index)
            {
                printf("Searched elem: %s, index: %d \n",
                       list[index].array,
                       search_index);
            }
            else
            {
                printf("Element not found : %d\n", search_index);
            }
        }
    }
}
