#include <stdio.h>
#include <stdlib.h>
#include "string_view.h"

int main(void) {
    printf("=== Test String_View ===\n");

    String_View testo = sv_from_cstr("Supercalifragilistichespiralidoso");
    printf("Testo intero: %.*s (lunghezza: %zu)\n", (int)testo.count, testo.data, testo.count);

    // Test sv_sub (slicing in O(1))
    String_View sub = sv_sub(testo, 5, 4); // "cali"
    printf("Sottostringa [5, 5+4]: %.*s (lunghezza: %zu)\n", (int)sub.count, sub.data, sub.count);

    // Test sv_equals
    String_View a = sv_from_cstr("test");
    String_View b = sv_from_cstr("test");
    String_View c = sv_from_cstr("altro");

    printf("a == b? %s\n", sv_equals(a, b) ? "true" : "false");
    printf("a == c? %s\n", sv_equals(a, c) ? "true" : "false");




    return 0;
}
