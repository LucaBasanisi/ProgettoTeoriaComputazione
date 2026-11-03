#include "string_view.h"
#include <string.h>

String_View sv_from_parts(const char *data, size_t count) {
    String_View sv;
    sv.data = data;
    sv.count = count;
    return sv;
}

String_View sv_from_cstr(const char *cstr) {
    if (cstr == NULL) {
        return sv_from_parts(NULL, 0);
    }
    return sv_from_parts(cstr, strlen(cstr));
}

String_View sv_sub(String_View sv, size_t start, size_t count) {
    if (start >= sv.count) {
        return sv_from_parts(NULL, 0);
    }

    if (start + count > sv.count) {
        count = sv.count - start;
    }

    return sv_from_parts(sv.data + start, count);
}

bool sv_equals(String_View a, String_View b) {
    if (a.count != b.count) {
        return false;
    }

    if (a.count == 0) {
        return true;
    }

    return memcmp(a.data, b.data, a.count) == 0;
}
