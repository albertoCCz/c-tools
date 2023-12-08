#ifndef STRINGV_H_
#define STRINGV_H_

#define STRINGV_MAX_CAPACITY   1024 // number of char
#define STRINGVA_MAX_CAPACITY  100  // number of StringV elems

#include <stdlib.h>
#include <stdbool.h>

// structs
typedef struct  {
    char* addr;    // to indicate if it's freeable
    char* sv;      // begining of view
    size_t count;  // number of elems in the view
} StringV;

typedef struct StringVA {
    StringV *stringvs;
    size_t count;       // number of elems in views' array
} StringVA;

// prototypes
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

// implementations
#ifdef STRINGV_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

void stringv_print(StringV stringv)
{
    for (size_t i = 0; i < stringv.count; ++i) fputc(stringv.sv[i], stdout);
}

typedef enum STRINGV_CREATE_MODE {
    SINGLE = 0,
    MULTI
} STRINGV_CREATE_MODE;

void stringv_destroy(StringV *stringv)
{
    if (stringv->addr != NULL) free(stringv->addr);
}

void stringv_destroy_stringva(StringVA *stringva)
{
    for (size_t i = 0; i < stringva->count; ++i) {
        if (stringva->stringvs[i].addr != NULL) free(stringva->stringvs[i].addr);
    }

    if (stringva->stringvs != NULL) free(stringva->stringvs);
}

static StringV __stringv_create(const char *str, STRINGV_CREATE_MODE mode)
{
    StringV stringv;

    stringv.count = 0;
    size_t i = 0;
    while (str[i] != '\0') {
        stringv.count++;
        i++;
        if (stringv.count >= STRINGV_MAX_CAPACITY) {
            if (mode == SINGLE) printf("[WARNING]: STRINGV_MAX_CAPACITY (%d bytes) reached when creating StringV. Consider usign stringv_create_stringva\n", sizeof(char)*STRINGV_MAX_CAPACITY);
            break;
        }
    }
    stringv.addr = (char*) calloc(stringv.count, sizeof(char));
    memcpy(stringv.addr, str, stringv.count*sizeof(char));
    stringv.sv = stringv.addr;

    return stringv;
}

StringV stringv_create(const char *str)
{
    return __stringv_create(str, SINGLE);
}

StringVA stringv_create_stringva(const char *str)
{
    StringVA stringva;
    stringva.stringvs = (StringV*) calloc(STRINGVA_MAX_CAPACITY, sizeof(StringV));
    stringva.count = 0;
    
    StringV stringv;

    size_t str_offset = 0;
    while (1) {
         if (stringva.count >= STRINGVA_MAX_CAPACITY) {
            printf("[WARNING]: STRINGVA_MAX_CAPACITY (%d x StringV elements) reached when 'stringv_create_stringva'\n", STRINGVA_MAX_CAPACITY);
            break;
        }
        
         stringv = __stringv_create(str + str_offset, MULTI);
         if (stringv.count > 0) {
             stringva.stringvs[stringva.count] = stringv;
             str_offset += stringv.count;
             stringva.count++;
         } else {
             break;
         }
    }
    return stringva;
}

void stringv_ltrim(StringV *stringv)
{
    while (stringv->count >= 0 && *stringv->sv == ' ') {
        stringv->sv++;
        stringv->count--;
    }
}

void stringv_rtrim(StringV *stringv)
{
    while (stringv->count >= 0 && stringv->sv[stringv->count-1] == ' ') {
        stringv->count--;
    }
}

void stringv_trim(StringV *stringv)
{
    stringv_ltrim(stringv);
    stringv_rtrim(stringv);
}

StringVA stringv_split_by_delim(StringV stringv, const char *c, size_t delim_len)
{
    StringVA stringva;
    stringva.stringvs = (StringV*) calloc(STRINGVA_MAX_CAPACITY, sizeof(StringV));
    stringva.count    = 0;

    char buff[delim_len + 1];
    
    bool store_new = true;
    for (size_t i = 0; i < stringv.count; ++i) {
        if (stringva.count >= STRINGVA_MAX_CAPACITY) {
            printf("[WARNING]: STRINGVA_MAX_CAPACITY (%d x StringV elements) reached when 'stringv_split_by_delim'\n", STRINGVA_MAX_CAPACITY);
            break;
        }

        size_t buff_content_len = (stringv.count - i) >= delim_len ? delim_len : stringv.count - i;
        memcpy(buff, stringv.sv + i, buff_content_len); // update moving buffer
        buff[buff_content_len] = '\0';

        if (strcmp(buff, c) != 0) {
            if (store_new) {
                stringva.stringvs[stringva.count] = (StringV) {
                    .addr  = NULL,
                    .sv    = stringv.sv + i,
                    .count = 1
                };
                stringva.count++;
                store_new = false;
            } else {
                stringva.stringvs[stringva.count - 1].count++;
            }
        } else {
            if (store_new) {
                stringva.stringvs[stringva.count] = (StringV) {
                    .addr  = NULL,
                    .sv    = "",
                    .count = 0
                };
                stringva.count++;
            }

            i += delim_len - 1;
            store_new = true;
        }
    }

    if (store_new) {
        if (stringva.count >= STRINGVA_MAX_CAPACITY) {
            printf("[WARNING]: STRINGVA_MAX_CAPACITY (%d x StringV elements) reached when 'stringv_split_by_delim'\n", STRINGVA_MAX_CAPACITY);
        }
        stringva.stringvs[stringva.count] = (StringV) {
            .addr  = NULL,
            .sv    = "",
            .count = 0
        };
        stringva.count++;
    }

    return stringva;
}

bool stringv_starts_with(StringV stringv, const char *prefix, size_t prefix_sz)
{
    if (stringv.count >= prefix_sz) {
        for (size_t i = 0; i < prefix_sz; ++i) {
            if (stringv.sv[i] != prefix[i]) return false;
        }
    } else {
        return false;
    }
    return true;
}

bool stringv_ends_with(StringV stringv, const char *sufix, size_t sufix_sz)
{
    if (stringv.count >= sufix_sz) {
        for (size_t i = 0; i < sufix_sz; ++i) {
            if (stringv.sv[stringv.count-1 - i] != sufix[sufix_sz-1 - i]) return false;
        }
    } else {
        return false;
    }
    return true;
}

bool stringv_contains(StringV stringv, const char *substr, size_t substr_sz)
{
    if (stringv.count >= substr_sz) {
        for (size_t i = 0; i < stringv.count; ++i) {
            bool found = true;
            for (size_t j = 0; j < substr_sz; ++j) {
                if (i+j >= stringv.count) return false;
                found = found && stringv.sv[i+j] == substr[j];
                if (!found) break;
            }
            if (found) return true;
        }
    } else {
        return false;
    }
}

void stringv_remove_prefix(StringV *stringv, size_t n)
{
    if (n > stringv->count) {
        fprintf(stderr, "[ERROR]: Provided prefix size (%zu) is bigger than stringv count (%zu)\n",
                n, stringv->count);
        return;
    }
    stringv->sv    += n;
    stringv->count -= n;
}

void stringv_remove_sufix(StringV *stringv, size_t n)
{
    if (n > stringv->count) {
        fprintf(stderr, "[ERROR]: Provided sufix size (%zu) is bigger than stringv count (%zu)\n",
                n, stringv->count);
        return;
    }
    stringv->count -= n;
}

#endif // STRINGV_IMPLEMENTATION
#endif // STRINGV_H_
