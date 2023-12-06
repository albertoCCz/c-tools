#ifndef MERGE_SORT_
#define MERGE_SORT_

#include <stddef.h> // size_t

// prototypes
void ms_merge_sort_int(int arr[], size_t arr_size);
void ms_merge_sort_float(float arr[], size_t arr_size);
void ms_merge_sort_double(double arr[], size_t arr_size);
void ms_merge_sort_string(char *arr[], size_t arr_size);

void ms_merge_int(int arr[], int a[], size_t a_size, int b[], size_t b_size);
void ms_merge_float(float arr[], float a[], size_t a_size, float b[], size_t b_size);
void ms_merge_double(double arr[], double a[], size_t a_size, double b[], size_t b_size);
void ms_merge_string(char *arr[], char * a[], size_t a_size, char * b[], size_t b_size);

#ifdef MERGE_SORT_IMPLEMENTATION

#include <string.h> // memcpy

// implementations
// ms_merge_TYPE
void ms_merge_int(int arr[], int a[], size_t a_size, int b[], size_t b_size)
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

void ms_merge_float(float arr[], float a[], size_t a_size, float b[], size_t b_size)
{
    size_t c = 0, i = 0, j = 0;
    while ((i <= a_size) || (j <= b_size)) {
        if (i == a_size) {
            memcpy(arr+c, b+j, (b_size - j) * sizeof(float));
            c += b_size - j;
            break; // while
        } else if (j == b_size) {
            memcpy(arr+c, a+i, (a_size - i) * sizeof(float));
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

void ms_merge_double(double arr[], double a[], size_t a_size, double b[], size_t b_size)
{
    size_t c = 0, i = 0, j = 0;
    while ((i <= a_size) || (j <= b_size)) {
        if (i == a_size) {
            memcpy(arr+c, b+j, (b_size - j) * sizeof(double));
            c += b_size - j;
            break; // while
        } else if (j == b_size) {
            memcpy(arr+c, a+i, (a_size - i) * sizeof(double));
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

void ms_merge_string(char *arr[], char * a[], size_t a_size, char * b[], size_t b_size)
{
    size_t c = 0, i = 0, j = 0;
    while ((i <= a_size) || (j <= b_size)) {
        if (i == a_size) {
            memcpy(arr+c, b+j, (b_size - j) * sizeof(char *));
            c += b_size - j;
            break; // while
        } else if (j == b_size) {
            memcpy(arr+c, a+i, (a_size - i) * sizeof(char *));
            c += a_size - i;
            break; // while
        } else {
            if (strcmp(a[i], b[j]) <= 0) {
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

// ms_merge_sort_TYPE
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
    
    ms_merge_int(arr, a, len_a, b, len_b);
}

void ms_merge_sort_float(float arr[], size_t arr_size)
{
    size_t q = arr_size/2;
    size_t p_a = 0, q_a = q/2,              r_a = q;
    size_t p_b = q, q_b = (arr_size-q)/2+q, r_b = arr_size;

    size_t len_a = r_a - p_a;
    size_t len_b = r_b - p_b;
    
    float a[len_a];
    float b[len_b];

    memcpy(a, arr + p_a, len_a * sizeof(float));
    memcpy(b, arr + p_b, len_b * sizeof(float));
    
    if (len_a > 1) ms_merge_sort_float(a, len_a);
    if (len_b > 1) ms_merge_sort_float(b, len_b);
    
    ms_merge_float(arr, a, len_a, b, len_b);
}

void ms_merge_sort_double(double arr[], size_t arr_size)
{
    size_t q = arr_size/2;
    size_t p_a = 0, q_a = q/2,              r_a = q;
    size_t p_b = q, q_b = (arr_size-q)/2+q, r_b = arr_size;

    size_t len_a = r_a - p_a;
    size_t len_b = r_b - p_b;
    
    double a[len_a];
    double b[len_b];

    memcpy(a, arr + p_a, len_a * sizeof(double));
    memcpy(b, arr + p_b, len_b * sizeof(double));
    
    if (len_a > 1) ms_merge_sort_double(a, len_a);
    if (len_b > 1) ms_merge_sort_double(b, len_b);
    
    ms_merge_double(arr, a, len_a, b, len_b);
}

void ms_merge_sort_string(char *arr[], size_t arr_size)
{
    size_t q = arr_size/2;
    size_t p_a = 0, q_a = q/2,              r_a = q;
    size_t p_b = q, q_b = (arr_size-q)/2+q, r_b = arr_size;

    size_t len_a = r_a - p_a;
    size_t len_b = r_b - p_b;
    
    char *a[len_a];
    char *b[len_b];

    memcpy(a, arr + p_a, len_a * sizeof(char *));
    memcpy(b, arr + p_b, len_b * sizeof(char *));
    
    if (len_a > 1) ms_merge_sort_string(a, len_a);
    if (len_b > 1) ms_merge_sort_string(b, len_b);
    
    ms_merge_string(arr, a, len_a, b, len_b);
}

#endif // MERGE_SORT_IMPLEMENTATION
#endif // MERGE_SORT_
