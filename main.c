
/*
 ============================================================================
 Name        : C-Works.c
 Author      : halis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>

// Supports 0-10 arguments
#define VA_NARGS_IMPL(_0, _1, _2, _3, N, ...) N
// ## deletes preceding comma if _VA_ARGS__ is empty (GCC, Clang)

#define PRIMITIVE_CAT(a, ...)                 a##__VA_ARGS__

#define VA_COUNT(...)                         VA_NARGS_IMPL(_, ##__VA_ARGS__, 1, 1, 1, 0)
#define VA_COND(param, ...)                                                    \
    VA_NARGS_IMPL(_,                                                           \
                  ##__VA_ARGS__,                                               \
                  IIF(VA_COUNT(__VA_ARGS__)),                                  \
                  IIF(VA_COUNT(__VA_ARGS__)),                                  \
                  IIF(VA_COUNT(__VA_ARGS__)))                                  \
    (param, __VA_ARGS__)

#define IIF(c)        PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) t
#define IIF_1(t, ...) __VA_ARGS__, t

void foo_tip1(int arg)
{
    printf("%d\n", arg);
}

void foo_tip3(int a, int b, int arg)
{
    printf("%d-%d-%d\n", a, b, arg);
}

typedef void (*xx_tip1)(int);
typedef void (*xx_tip3)(int, int, int);

#define HOOK_CALL(tip, ...)                                                    \
    do                                                                         \
    {                                                                          \
        xx_##tip func  = &foo_##tip;                                           \
        int      param = 61;                                                   \
        func(VA_COND(param, __VA_ARGS__));                                     \
    } while (0)

int main()
{
    HOOK_CALL(tip1);

    HOOK_CALL(tip3, 1, 2);

    return 0;
}
