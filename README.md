# c-tools
Single header files (stb-style) tools. The aim is just to have in one place these utilities I develop while learning some C and data structures.

## merge_sort.h
Merge Sort sorting algorithm for different types of arrays.

Prototypes:
```c
void ms_merge_sort_int(int arr[], size_t arr_size);
void ms_merge_sort_float(float arr[], size_t arr_size);
void ms_merge_sort_double(double arr[], size_t arr_size);
void ms_merge_sort_string(char *arr[], size_t arr_size);
```

## stringv.h
String View tool.

Prototypes:
```c
StringV stringv_create(const char *str);
StringVA stringv_create_stringva(const char *str);
void stringv_destroy(StringV *stringv);
void stringv_destroy_stringva(StringVA *stringva);

void stringv_print(StringV stringv);

void stringv_ltrim(StringV *stringv);
void stringv_rtrim(StringV *stringv);
void stringv_trim(StringV *stringv);

StringVA stringv_split_by_delim(StringV stringv, const char *c, size_t delim_len);

bool stringv_starts_with(StringV stringv, const char *prefix, size_t prefix_sz);
bool stringv_ends_with(StringV stringv, const char *sufix, size_t sufix_sz);
bool stringv_contains(StringV stringv, const char *substr, size_t substr_sz);

void stringv_remove_prefix(StringV *stringv, size_t n);
void stringv_remove_sufix(StringV *stringv, size_t n);
```

## deque.h
Deque implementation, only suitable for `int` queues (for now).

Prototypes:
```c
Deque deque_create(size_t n_elems);
void deque_destroy(Deque *deque);

void deque_push_front(Deque *deque, int val);
int deque_pop_front(Deque *deque);

void deque_push_back(Deque *deque, int val);
int deque_pop_back(Deque *deque);

int deque_front(Deque deque);
int deque_back(Deque deque);

void deque_print(Deque deque);
```
