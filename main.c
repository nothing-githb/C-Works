
/*
 ============================================================================
 Name        : C-Works.c
 Author      : halis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "hook.h"
#include "test.h"

#include <stdio.h>
#include <stdlib.h>

void foo(int a, int b, int x)
{
}

#define __COUNTER(...)           ((int) (sizeof((int[]){__VA_ARGS__}) / sizeof(int)))
#define COUNTER_VA_ARGS(...)     __COUNTER(__VA_ARGS__)

#define PART_SWITCH_TYPES        int*, double, char

#define GET_0(x, ...)            x
#define GET_1(x, y, ...)         y
#define GET_2(x, y, z, ...)      z

#define TYPE_CONTROL(a, b)       __builtin_types_compatible_p(a, typeof(b))

#define T_CONTROL(index, x, ...) TYPE_CONTROL(GET_##index(x), GET_##index(__VA_ARGS__))

#define CHECK(index, x, ...)     T_CONTROL(index, x##_TYPES, __VA_ARGS__)

#define LOOP(mode, ...)                                                                                                \
    ({                                                                                                                 \
        int __is_valid_type = 1;                                                                                       \
        switch (COUNTER_VA_ARGS(__VA_ARGS__))                                                                          \
        {                                                                                                              \
            case (3):                                                                                                  \
                __is_valid_type &= CHECK(2, mode, __VA_ARGS__);                                                        \
            case (2):                                                                                                  \
                __is_valid_type &= CHECK(1, mode, __VA_ARGS__);                                                        \
            case (1):                                                                                                  \
                __is_valid_type &= CHECK(0, mode, __VA_ARGS__);                                                        \
        }                                                                                                              \
        __is_valid_type;                                                                                               \
    })

#define DENEME_0(x, ...) __VA_ARGS__

#define DENEME(...)      DENEME(DENEME(__VA_ARGS__))

#define FOR_LOOP(index, ...)                                                                                           \
    ({                                                                                                                 \
        int a = 0;                                                                                                     \
        if (index >= COUNTER_VA_ARGS(__VA_ARGS__))                                                                     \
        {                                                                                                              \
            a = -1;                                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            a = 61;                                                                                                    \
        }                                                                                                              \
        a;                                                                                                             \
    })

int main()
{
    int  ret;
    int  major_id  = 4;
    int  window_id = 3;
    char x         = 's';
    ret            = HOOK_ATTACH(PARTITION_MODE, foo, major_id, 2);

    printf("ret: %d, %d\n", ret, __builtin_types_compatible_p(typeof(&foo), PARTITION_MODE_FUNC_TYPE));
    typeof(ret) xx;

    xx = (COUNTER_VA_ARGS(1, 2, 3, 'a', 'b', 'c', 4.0, 5.0, 6.0, 1, 0) == 11);
    printf("test: %d \n", xx);

    // printf("test2: %d \n", LOOP(PART_SWITCH, &window_id, 2.0, x));
    window_id = 5;

    window_id = 2;

    testDLL(1);

    return 0;
}
