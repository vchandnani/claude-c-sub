#ifndef STR_REPLACE_H
#define STR_REPLACE_H

#include <stddef.h>

/**
 * Replace all occurrences of 'search' in 'source' with 'replace'.
 *
 * @param source The original string to search in
 * @param search The substring to search for
 * @param replace The string to replace matches with
 * @return A newly allocated string with replacements, or NULL on error.
 *         Caller is responsible for freeing the returned string.
 */
char *str_replace(const char *source, const char *search, const char *replace);

/**
 * Count occurrences of 'search' in 'source'.
 *
 * @param source The string to search in
 * @param search The substring to count
 * @return Number of non-overlapping occurrences
 */
size_t str_count_occurrences(const char *source, const char *search);

#endif /* STR_REPLACE_H */
