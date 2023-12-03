#ifndef MERGE_SORT_
#define MERGE_SORT_

#include <stddef.h> // size_t

// prototypes
void ms_merge_sort_int(int arr[], size_t arr_size);
void ms_merge(int arr[], int a[], size_t a_size, int b[], int b_size);
    
#ifdef MERGE_SORT_IMPLEMENTATION

#include <string.h> // memcpy

// implementations
void ms_merge(int arr[], int a[], size_t a_size, int b[], int b_size)
{
    size_t c = 0, i = 0, j = 0;
    while ((i <= a_size) || (j <= b_size)) {
        if (i == a_size) {
            memcpy(arr+c, b+j, (b_size - j) * sizeof(int));
            c += b_size - j;
            break; // while
        } else if (j == b_size) {
            memcpy(arr+c, a+i, (a_size - i) * sizeof(int));
            c += a_size - i;
            break; // while
        } else {
            if (a[i] <= b[j]) {
                arr[c] = a[i];
                ++i;
            } else {
                arr[c] = b[j];
                ++j;
            }
            ++c;
        }
    }
}

void ms_merge_sort_int(int arr[], size_t arr_size)
{
    size_t q = arr_size/2;
    size_t p_a = 0, q_a = q/2,              r_a = q;
    size_t p_b = q, q_b = (arr_size-q)/2+q, r_b = arr_size;

    size_t len_a = r_a - p_a;
    size_t len_b = r_b - p_b;
    
    int a[len_a];
    int b[len_b];

    memcpy(a, arr + p_a, len_a * sizeof(int));
    memcpy(b, arr + p_b, len_b * sizeof(int));
    
    if (len_a > 1) ms_merge_sort_int(a, len_a);
    if (len_b > 1) ms_merge_sort_int(b, len_b);
    
    ms_merge(arr, a, len_a, b, len_b);
}

#endif // MERGE_SORT_IMPLEMENTATION
#endif // MERGE_SORT_
