//
// Created by Halis Şahin on 29.08.2021.
//

#include <stdio.h>  // size_t

int binarySearch(void *array, size_t elemSize, int len, void *key, int (*cmp)(void *a, void *b))
{
    int low = 0, high = len - 1, tmp_index;
    void *tmp_elem;
    int compare;

    while (low <= high)
    {
        tmp_index = (low + high) / 2;
        tmp_elem = array + (elemSize * tmp_index);
        compare = cmp(key, tmp_elem);
        if (0 == compare)
            return tmp_index;
        else if (0 > compare)
            high = tmp_index - 1;
        else
            low = tmp_index + 1;
    }

    return -1;
}
