#ifndef STRINGV_H_
#define STRINGV_H_

#define STRINGV_MAX_CAPACITY   1024
#define STRINGVA_MAX_CAPACITY  100

#include <stdlib.h>
#include <stdbool.h>

// structs
typedef struct StringV {
    char* addr;
    char* sv;
    size_t count;
} StringV;

typedef struct StringVA {
    StringV *stringvs;
    size_t count;
} StringVA;

// prototypes
StringV stringv_create(char *str);
void stringv_destroy(StringV *stringv);
void stringv_destroy_stringva(StringVA *stringva);

void stringv_print(StringV stringv);

void stringv_ltrim(StringV *stringv);
void stringv_rtrim(StringV *stringv);
void stringv_trim(StringV *stringv);

StringVA stringv_split_by_delim(StringV stringv, char c);

bool stringv_starts_with(StringV stringv, char* prefix, size_t prefix_sz);
bool stringv_ends_with(StringV stringv, char* sufix, size_t sufix_sz);
bool stringv_contains(StringV stringv, char *substr, size_t substr_sz);

// implementations
#ifdef STRINGV_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

void stringv_print(StringV stringv)
{
    for (size_t i = 0; i < stringv.count; ++i) fputc(stringv.sv[i], stdout);
}

StringV stringv_create(char *str)
{
    StringV stringv;

    // TODO: decide what to do when STRINGV_MAX_CAPACITY is reached.
    //       In this case I guess I should fill stringv in with as
    //       much as I can and let the user know max capacity has
    //       been reached.
    stringv.count = 0;
    for (size_t i = 0; i < STRINGV_MAX_CAPACITY && str[i] != '\0'; i++) stringv.count++;
    stringv.addr = (char*) calloc(stringv.count, sizeof(char));
    memcpy(stringv.addr, str, stringv.count*sizeof(char));
    stringv.sv = stringv.addr;

    return stringv;
}

void stringv_destroy(StringV *stringv)
{
    free(stringv->addr);
}

void stringv_destroy_stringva(StringVA *stringva)
{
    free(stringva->stringvs);
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

StringVA stringv_split_by_delim(StringV stringv, char c)
{
    // asfasfasf-asfasf-asdfasdfsfasdffsd-sdf-asfss-asdfasdfasdf-
    // |       \ |    \ |               \ | \ |   \ |          \

    StringVA stringva;
    stringva.stringvs = (StringV*) calloc(STRINGVA_MAX_CAPACITY, sizeof(StringV));
    stringva.count = 0;

    // TODO: can't put more than STRINGVA_MAX_CAPACITY StringV pointers
    //       at stringva. I should either let user know this or
    //       to reallocate to a bigger size.
    for (size_t i = 0; i < stringv.count; ++i) {
        if ((i == 0 && stringv.sv[i] != c) || (i > 0 && stringv.sv[i-1] == c)) {
            stringva.count++;
            stringva.stringvs[stringva.count-1].addr = stringv.sv + i;
            stringva.stringvs[stringva.count-1].sv   = stringv.sv + i;
        }
        
        if (stringv.sv[i] != c) stringva.stringvs[stringva.count-1].count++;
    }
    
    return stringva;
}

bool stringv_starts_with(StringV stringv, char* prefix, size_t prefix_sz)
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

bool stringv_ends_with(StringV stringv, char* sufix, size_t sufix_sz)
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

bool stringv_contains(StringV stringv, char *substr, size_t substr_sz)
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


#endif // STRINGV_IMPLEMENTATION
#endif // STRINGV_H_
