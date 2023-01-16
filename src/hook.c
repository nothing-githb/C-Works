
#include "hook.h"

#include "LocalHook.h"

#include <stdio.h>

hook_node_gt* hooks[NUMBER_OF_HOOK_POINTS];

int           gzis_register_hook(int hook_value, void* func, const char* format, ...)
{
    printf("%d --> %p --> %s\n", hook_value, func, format);

    return 61;
}
