//
// Created by Halis Şahin on 15.08.2021.
//

#ifndef C_ALGORITHMS_LOCAL_HOOK_H
#define C_ALGORITHMS_LOCAL_HOOK_H

#include "dll.h"

typedef struct
{
    dllist_gt     list;
    void*         func_addr;
    unsigned long arg;
} hook_node_gt;

#endif
