//
// Created by Halis Şahin on 29.08.2021.
//

#ifndef C_ALGORITHMS_BINARYSEARCH_H
#define C_ALGORITHMS_BINARYSEARCH_H

#include <stddef.h>

int binarySearch(void*  array,
                 size_t elemSize,
                 int    len,
                 void*  key,
                 int (*cmp)(void* a, void* b));

#endif //C_ALGORITHMS_BINARYSEARCH_H
