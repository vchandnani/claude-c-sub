#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_replace.h"

#define MAX_INPUT_LEN 1024

// Helper function to read a line, removing trailing newline
static int read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buffer, size, stdin) == NULL) {
        return -1;
    }

    // Remove trailing newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    return 0;
}

int main(void) {
    char string1[MAX_INPUT_LEN];
    char string2[MAX_INPUT_LEN];
    char string3[MAX_INPUT_LEN];

    printf("String Replacement Program\n");
    printf("==========================\n\n");

    // Read input strings
    if (read_line("Enter string1 (source string): ", string1, sizeof(string1)) != 0) {
        fprintf(stderr, "Error reading string1\n");
        return 1;
    }

    if (read_line("Enter string2 (search string): ", string2, sizeof(string2)) != 0) {
        fprintf(stderr, "Error reading string2\n");
        return 1;
    }

    if (read_line("Enter string3 (replacement string): ", string3, sizeof(string3)) != 0) {
        fprintf(stderr, "Error reading string3\n");
        return 1;
    }

    // Validate search string is not empty
    if (strlen(string2) == 0) {
        printf("\nSearch string is empty. No replacement performed.\n");
        printf("Result: %s\n", string1);
        return 0;
    }

    // Perform replacement
    char *result = str_replace(string1, string2, string3);
    if (result == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 1;
    }

    // Display results
    printf("\n--- Results ---\n");
    printf("Original:    \"%s\"\n", string1);
    printf("Search for:  \"%s\"\n", string2);
    printf("Replace with: \"%s\"\n", string3);

    size_t count = str_count_occurrences(string1, string2);
    if (count > 0) {
        printf("Occurrences: %zu\n", count);
        printf("Result:      \"%s\"\n", result);
    } else {
        printf("Pattern not found. No changes made.\n");
        printf("Result:      \"%s\"\n", result);
    }

    free(result);
    return 0;
}
