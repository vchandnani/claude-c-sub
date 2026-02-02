#include "str_replace.h"
#include <stdlib.h>
#include <string.h>

size_t str_count_occurrences(const char *source, const char *search) {
    if (source == NULL || search == NULL || *search == '\0') {
        return 0;
    }

    size_t count = 0;
    size_t search_len = strlen(search);
    const char *pos = source;

    while ((pos = strstr(pos, search)) != NULL) {
        count++;
        pos += search_len;
    }

    return count;
}

char *str_replace(const char *source, const char *search, const char *replace) {
    // Handle NULL inputs
    if (source == NULL) {
        return NULL;
    }

    // If search is NULL or empty, return a copy of source
    if (search == NULL || *search == '\0') {
        char *result = malloc(strlen(source) + 1);
        if (result == NULL) {
            return NULL;
        }
        strcpy(result, source);
        return result;
    }

    // If replace is NULL, treat it as empty string
    const char *replace_str = (replace != NULL) ? replace : "";

    size_t source_len = strlen(source);
    size_t search_len = strlen(search);
    size_t replace_len = strlen(replace_str);

    // Count occurrences to calculate result size
    size_t count = str_count_occurrences(source, search);

    // If no occurrences found, return a copy of source
    if (count == 0) {
        char *result = malloc(source_len + 1);
        if (result == NULL) {
            return NULL;
        }
        strcpy(result, source);
        return result;
    }

    // Calculate new string length
    // new_len = source_len - (count * search_len) + (count * replace_len)
    size_t new_len = source_len + count * (replace_len - search_len);

    // Allocate memory for result
    char *result = malloc(new_len + 1);
    if (result == NULL) {
        return NULL;
    }

    // Perform the replacement
    char *dest = result;
    const char *src = source;
    const char *match;

    while ((match = strstr(src, search)) != NULL) {
        // Copy characters before the match
        size_t prefix_len = match - src;
        memcpy(dest, src, prefix_len);
        dest += prefix_len;

        // Copy the replacement string
        memcpy(dest, replace_str, replace_len);
        dest += replace_len;

        // Move past the matched portion in source
        src = match + search_len;
    }

    // Copy remaining characters after last match
    strcpy(dest, src);

    return result;
}
