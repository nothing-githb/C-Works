//
// Created by Halis Şahin on 29.08.2021.
//

#include <stdio.h>

int binarySearch(void*  array,
                 size_t elem_size,
                 int    len,
                 void*  key,
                 int (*cmp)(void* a, void* b))
{
    int   low       = 0;
    int   high      = len - 1;
    int   tmp_index = 0;
    void* tmp_elem  = NULL;
    int   compare   = 0;

    while ( low <= high )
    {
        tmp_index = (low + high) / 2;
        tmp_elem  = ((int*) array) + (elem_size * tmp_index);
        compare   = cmp(key, tmp_elem);
        if ( 0 == compare )
        {
            return tmp_index;
        }
        if ( 0 > compare )
        {
            high = tmp_index - 1;
        }
        else
        {
            low = tmp_index + 1;
        }
    }

    return -1;
}
