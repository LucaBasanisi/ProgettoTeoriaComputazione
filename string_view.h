#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>
#include <stdbool.h>

// Struttura non-owning per la gestione moderna delle stringhe
typedef struct {
    const char *data;
    size_t count;
} String_View;

// Costruttori
String_View sv_from_parts(const char *data, size_t count);
String_View sv_from_cstr(const char *cstr);

// Operazioni
String_View sv_sub(String_View sv, size_t start, size_t count);
bool sv_equals(String_View a, String_View b);

#endif // STRING_VIEW_H
