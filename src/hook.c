
#include "hook.h"

#include "LocalHook.h"

#include <stdio.h>
/* 
hook_node_gt* hooks[NUMBER_OF_HOOK_POINTS];

int           gzis_register_hook(int hook_value, void* func, const char* format, ...)
{
    printf("%d --> %p --> %s\n", hook_value, func, format);

    return 61;
}


#define __COUNTER(...)       ((int) (sizeof((int[]){__VA_ARGS__}) / sizeof(int)))
#define COUNTER_VA_ARGS(...) __COUNTER(__VA_ARGS__)

#define PART_SWITCH_TYPES    int*, double, char

#define GET_0(x, ...)        x
#define GET_1(x, y, ...)     y
#define GET_2(x, y, z, ...)  z

#define TYPE_CONTROL(a, b)   __builtin_types_compatible_p(a, typeof(b))

#define T_CONTROL(index, x, ...)                                               \
    TYPE_CONTROL(GET_##index(x), GET_##index(__VA_ARGS__))

#define CHECK(index, x, ...) T_CONTROL(index, x##_TYPES, __VA_ARGS__)

#define LOOP(mode, ...)                                                        \
    ({                                                                         \
        int __is_valid_type = 1;                                               \
        switch (COUNTER_VA_ARGS(__VA_ARGS__))                                  \
        {                                                                      \
            case (3):                                                          \
                __is_valid_type &= CHECK(2, mode, __VA_ARGS__);                \
            case (2):                                                          \
                __is_valid_type &= CHECK(1, mode, __VA_ARGS__);                \
            case (1):                                                          \
                __is_valid_type &= CHECK(0, mode, __VA_ARGS__);                \
        }                                                                      \
        __is_valid_type;                                                       \
    })

#define DENEME_0(x, ...) __VA_ARGS__

#define DENEME(...)      DENEME(DENEME(__VA_ARGS__))

#define FOR_LOOP(index, ...)                                                   \
    ({                                                                         \
        int a = 0;                                                             \
        if (index >= COUNTER_VA_ARGS(__VA_ARGS__))                             \
        {                                                                      \
            a = -1;                                                            \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            a = 61;                                                            \
        }                                                                      \
        a;                                                                     \
    })
 */