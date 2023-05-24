#ifndef C_ALGORITHMS_HOOK_H
#define C_ALGORITHMS_HOOK_H

typedef enum
{
    PARTITION_SWITCH       = 0,
    MAJOR_SWITCH           = 1,
    PARTITION_MODE         = 2,
    HM_SHUTDOWN            = 3,
    HM_USER_RECORD_MESSAGE = 4,
    NUMBER_OF_HOOK_POINTS  = 5
} hook_mode_gt;

#define PARTITION_SWITCH_ARG_TYPE "%d%d"
#define MAJOR_SWITCH_ARG_TYPE     "%d%d%d"
#define PARTITION_MODE_ARG_TYPE   "%d%d"

typedef void (*PARTITION_SWITCH_FUNC_TYPE)(int, int, int);
typedef void (*MAJOR_SWITCH_FUNC_TYPE)(int, int, int, int);
typedef void (*PARTITION_MODE_FUNC_TYPE)(int, int, int);

#define HOOK_ATTACH(mode, func, ...)                                                                                   \
    ({                                                                                                                 \
        int __ret = 0;                                                                                                 \
        _Pragma("GCC diagnostic push");                                                                                \
        _Pragma("GCC diagnostic error \"-Wincompatible-pointer-types\"");                                              \
        _Pragma("GCC diagnostic error \"-Wformat\"");                                                                  \
        mode##_FUNC_TYPE __func = (func);                                                                              \
        __ret                   = gzis_register_hook(mode, (void*) __func, mode##_ARG_TYPE, __VA_ARGS__);              \
        _Pragma("GCC diagnostic pop");                                                                                 \
        __ret;                                                                                                         \
    })

/* Do not use this function directly */
extern int gzis_register_hook(int hook_value, void* func, const char* format, ...)
    __attribute__((format(printf, 3, 4)));

#endif
