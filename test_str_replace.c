#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_replace.h"

// Test framework macros
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) static void name(void)

#define RUN_TEST(name) do { \
    printf("Running %s... ", #name); \
    fflush(stdout); \
    tests_run++; \
    name(); \
} while(0)

#define ASSERT_STR_EQ(expected, actual) do { \
    if ((expected) == NULL && (actual) == NULL) { \
        tests_passed++; \
        printf("PASSED\n"); \
    } else if ((expected) == NULL || (actual) == NULL) { \
        tests_failed++; \
        printf("FAILED\n"); \
        printf("  Expected: %s\n", (expected) ? (expected) : "NULL"); \
        printf("  Actual:   %s\n", (actual) ? (actual) : "NULL"); \
    } else if (strcmp((expected), (actual)) == 0) { \
        tests_passed++; \
        printf("PASSED\n"); \
    } else { \
        tests_failed++; \
        printf("FAILED\n"); \
        printf("  Expected: \"%s\"\n", (expected)); \
        printf("  Actual:   \"%s\"\n", (actual)); \
    } \
} while(0)

#define ASSERT_EQ(expected, actual) do { \
    if ((expected) == (actual)) { \
        tests_passed++; \
        printf("PASSED\n"); \
    } else { \
        tests_failed++; \
        printf("FAILED\n"); \
        printf("  Expected: %zu\n", (size_t)(expected)); \
        printf("  Actual:   %zu\n", (size_t)(actual)); \
    } \
} while(0)

#define ASSERT_NULL(actual) do { \
    if ((actual) == NULL) { \
        tests_passed++; \
        printf("PASSED\n"); \
    } else { \
        tests_failed++; \
        printf("FAILED\n"); \
        printf("  Expected: NULL\n"); \
        printf("  Actual:   non-NULL\n"); \
    } \
} while(0)

#define ASSERT_NOT_NULL(actual) do { \
    if ((actual) != NULL) { \
        tests_passed++; \
        printf("PASSED\n"); \
    } else { \
        tests_failed++; \
        printf("FAILED\n"); \
        printf("  Expected: non-NULL\n"); \
        printf("  Actual:   NULL\n"); \
    } \
} while(0)

// ============================================================================
// HAPPY PATH (SUNNY DAY) TESTS
// ============================================================================

// Test: Single occurrence replacement
TEST(test_single_occurrence) {
    char *result = str_replace("hello world", "world", "universe");
    ASSERT_STR_EQ("hello universe", result);
    free(result);
}

// Test: Multiple occurrences replacement
TEST(test_multiple_occurrences) {
    char *result = str_replace("the cat sat on the mat", "the", "a");
    ASSERT_STR_EQ("a cat sat on a mat", result);
    free(result);
}

// Test: Replacement at beginning of string
TEST(test_replace_at_beginning) {
    char *result = str_replace("hello world", "hello", "goodbye");
    ASSERT_STR_EQ("goodbye world", result);
    free(result);
}

// Test: Replacement at end of string
TEST(test_replace_at_end) {
    char *result = str_replace("hello world", "world", "everyone");
    ASSERT_STR_EQ("hello everyone", result);
    free(result);
}

// Test: Replacement in middle of string
TEST(test_replace_in_middle) {
    char *result = str_replace("I love cats very much", "cats", "dogs");
    ASSERT_STR_EQ("I love dogs very much", result);
    free(result);
}

// Test: Replace with longer string
TEST(test_replace_with_longer_string) {
    char *result = str_replace("ab", "a", "xyz");
    ASSERT_STR_EQ("xyzb", result);
    free(result);
}

// Test: Replace with shorter string
TEST(test_replace_with_shorter_string) {
    char *result = str_replace("abcdef", "bcd", "x");
    ASSERT_STR_EQ("axef", result);
    free(result);
}

// Test: Replace with same length string
TEST(test_replace_same_length) {
    char *result = str_replace("hello", "ell", "opp");
    ASSERT_STR_EQ("hoppo", result);
    free(result);
}

// Test: Replace entire string
TEST(test_replace_entire_string) {
    char *result = str_replace("hello", "hello", "goodbye");
    ASSERT_STR_EQ("goodbye", result);
    free(result);
}

// Test: Consecutive occurrences
TEST(test_consecutive_occurrences) {
    char *result = str_replace("aaa", "a", "bb");
    ASSERT_STR_EQ("bbbbbb", result);
    free(result);
}

// Test: Overlapping pattern (non-overlapping match behavior)
TEST(test_overlapping_pattern) {
    // "aaa" contains "aa" starting at index 0 and 1
    // Non-overlapping should find only one occurrence at index 0
    char *result = str_replace("aaa", "aa", "b");
    ASSERT_STR_EQ("ba", result);
    free(result);
}

// Test: Replace with empty string (deletion)
TEST(test_replace_with_empty_string) {
    char *result = str_replace("hello world", "world", "");
    ASSERT_STR_EQ("hello ", result);
    free(result);
}

// Test: Delete all occurrences
TEST(test_delete_all_occurrences) {
    char *result = str_replace("a1b2c3d4", "1", "");
    // Only removes "1"
    char *result2 = str_replace(result, "2", "");
    char *result3 = str_replace(result2, "3", "");
    char *result4 = str_replace(result3, "4", "");
    ASSERT_STR_EQ("abcd", result4);
    free(result);
    free(result2);
    free(result3);
    free(result4);
}

// Test: Count single occurrence
TEST(test_count_single_occurrence) {
    size_t count = str_count_occurrences("hello world", "world");
    ASSERT_EQ(1, count);
}

// Test: Count multiple occurrences
TEST(test_count_multiple_occurrences) {
    size_t count = str_count_occurrences("the cat and the dog and the bird", "the");
    ASSERT_EQ(3, count);
}

// Test: Special characters
TEST(test_special_characters) {
    char *result = str_replace("hello\tworld\n", "\t", " ");
    ASSERT_STR_EQ("hello world\n", result);
    free(result);
}

// Test: Replace newlines
TEST(test_replace_newlines) {
    char *result = str_replace("line1\nline2\nline3", "\n", " | ");
    ASSERT_STR_EQ("line1 | line2 | line3", result);
    free(result);
}

// ============================================================================
// UNHAPPY PATH (RAINY DAY) TESTS
// ============================================================================

// Test: Search string not found
TEST(test_search_not_found) {
    char *result = str_replace("hello world", "xyz", "abc");
    ASSERT_STR_EQ("hello world", result);
    free(result);
}

// Test: Empty source string
TEST(test_empty_source_string) {
    char *result = str_replace("", "search", "replace");
    ASSERT_STR_EQ("", result);
    free(result);
}

// Test: Empty search string
TEST(test_empty_search_string) {
    char *result = str_replace("hello world", "", "replace");
    // When search is empty, return copy of source
    ASSERT_STR_EQ("hello world", result);
    free(result);
}

// Test: Empty replacement string (valid - should delete)
TEST(test_empty_replacement_deletes) {
    char *result = str_replace("hello world", " ", "");
    ASSERT_STR_EQ("helloworld", result);
    free(result);
}

// Test: NULL source string
TEST(test_null_source) {
    char *result = str_replace(NULL, "search", "replace");
    ASSERT_NULL(result);
}

// Test: NULL search string
TEST(test_null_search) {
    char *result = str_replace("hello world", NULL, "replace");
    // Should return copy of source
    ASSERT_STR_EQ("hello world", result);
    free(result);
}

// Test: NULL replacement string
TEST(test_null_replacement) {
    char *result = str_replace("hello world", "world", NULL);
    // NULL replacement treated as empty string
    ASSERT_STR_EQ("hello ", result);
    free(result);
}

// Test: All parameters empty
TEST(test_all_empty) {
    char *result = str_replace("", "", "");
    ASSERT_STR_EQ("", result);
    free(result);
}

// Test: Search longer than source
TEST(test_search_longer_than_source) {
    char *result = str_replace("hi", "hello world", "x");
    ASSERT_STR_EQ("hi", result);
    free(result);
}

// Test: Case sensitivity (should NOT match different case)
TEST(test_case_sensitive_no_match) {
    char *result = str_replace("Hello World", "hello", "hi");
    // Should not match because of case difference
    ASSERT_STR_EQ("Hello World", result);
    free(result);
}

// Test: Case sensitivity (should match exact case)
TEST(test_case_sensitive_match) {
    char *result = str_replace("Hello World", "Hello", "Hi");
    ASSERT_STR_EQ("Hi World", result);
    free(result);
}

// Test: Search for single character not present
TEST(test_single_char_not_found) {
    char *result = str_replace("hello", "x", "y");
    ASSERT_STR_EQ("hello", result);
    free(result);
}

// Test: Count with empty search
TEST(test_count_empty_search) {
    size_t count = str_count_occurrences("hello", "");
    ASSERT_EQ(0, count);
}

// Test: Count with NULL source
TEST(test_count_null_source) {
    size_t count = str_count_occurrences(NULL, "search");
    ASSERT_EQ(0, count);
}

// Test: Count with NULL search
TEST(test_count_null_search) {
    size_t count = str_count_occurrences("hello", NULL);
    ASSERT_EQ(0, count);
}

// Test: Count with no matches
TEST(test_count_no_matches) {
    size_t count = str_count_occurrences("hello world", "xyz");
    ASSERT_EQ(0, count);
}

// Test: Very long replacement
TEST(test_very_long_replacement) {
    char *result = str_replace("ab", "a", "xxxxxxxxxxxxxxxxxxxx");
    ASSERT_STR_EQ("xxxxxxxxxxxxxxxxxxxxb", result);
    free(result);
}

// Test: Unicode/UTF-8 bytes (basic - treated as raw bytes)
TEST(test_utf8_bytes) {
    // This tests that multi-byte sequences are handled as raw bytes
    char *result = str_replace("cafe", "e", "é");  // 'é' is 2 bytes in UTF-8
    ASSERT_STR_EQ("café", result);
    free(result);
}

// Test: String with only search pattern
TEST(test_only_search_pattern) {
    char *result = str_replace("aaa", "aaa", "b");
    ASSERT_STR_EQ("b", result);
    free(result);
}

// Test: Replacement creates new occurrences (should NOT be replaced)
TEST(test_replacement_creates_pattern) {
    // Replacing "a" with "ba" should not cause infinite loop
    // Each "a" gets replaced once, result should be "bba"
    char *result = str_replace("aa", "a", "ba");
    ASSERT_STR_EQ("baba", result);
    free(result);
}

// Test: Search is substring of replacement (no recursion)
TEST(test_search_substring_of_replacement) {
    char *result = str_replace("x", "x", "xx");
    ASSERT_STR_EQ("xx", result);
    free(result);
}

// Test: White space handling
TEST(test_whitespace_handling) {
    char *result = str_replace("  hello  ", " ", "_");
    ASSERT_STR_EQ("__hello__", result);
    free(result);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

// Test: Single character source, search, and replace
TEST(test_single_char_all) {
    char *result = str_replace("a", "a", "b");
    ASSERT_STR_EQ("b", result);
    free(result);
}

// Test: Numeric strings
TEST(test_numeric_strings) {
    char *result = str_replace("123456789", "456", "000");
    ASSERT_STR_EQ("123000789", result);
    free(result);
}

// Test: Mixed content
TEST(test_mixed_content) {
    char *result = str_replace("user@email.com", "@", " [at] ");
    ASSERT_STR_EQ("user [at] email.com", result);
    free(result);
}

// Test: Path-like strings
TEST(test_path_strings) {
    char *result = str_replace("/home/user/file.txt", "/", "\\");
    ASSERT_STR_EQ("\\home\\user\\file.txt", result);
    free(result);
}

// Test: Multiple consecutive matches then non-match
TEST(test_consecutive_then_non_match) {
    char *result = str_replace("aaab", "a", "x");
    ASSERT_STR_EQ("xxxb", result);
    free(result);
}

// Test: Non-match then multiple consecutive matches
TEST(test_non_match_then_consecutive) {
    char *result = str_replace("baaa", "a", "x");
    ASSERT_STR_EQ("bxxx", result);
    free(result);
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main(void) {
    printf("===========================================\n");
    printf("String Replacement Unit Tests\n");
    printf("===========================================\n\n");

    printf("--- HAPPY PATH (Sunny Day) Tests ---\n\n");

    RUN_TEST(test_single_occurrence);
    RUN_TEST(test_multiple_occurrences);
    RUN_TEST(test_replace_at_beginning);
    RUN_TEST(test_replace_at_end);
    RUN_TEST(test_replace_in_middle);
    RUN_TEST(test_replace_with_longer_string);
    RUN_TEST(test_replace_with_shorter_string);
    RUN_TEST(test_replace_same_length);
    RUN_TEST(test_replace_entire_string);
    RUN_TEST(test_consecutive_occurrences);
    RUN_TEST(test_overlapping_pattern);
    RUN_TEST(test_replace_with_empty_string);
    RUN_TEST(test_delete_all_occurrences);
    RUN_TEST(test_count_single_occurrence);
    RUN_TEST(test_count_multiple_occurrences);
    RUN_TEST(test_special_characters);
    RUN_TEST(test_replace_newlines);

    printf("\n--- UNHAPPY PATH (Rainy Day) Tests ---\n\n");

    RUN_TEST(test_search_not_found);
    RUN_TEST(test_empty_source_string);
    RUN_TEST(test_empty_search_string);
    RUN_TEST(test_empty_replacement_deletes);
    RUN_TEST(test_null_source);
    RUN_TEST(test_null_search);
    RUN_TEST(test_null_replacement);
    RUN_TEST(test_all_empty);
    RUN_TEST(test_search_longer_than_source);
    RUN_TEST(test_case_sensitive_no_match);
    RUN_TEST(test_case_sensitive_match);
    RUN_TEST(test_single_char_not_found);
    RUN_TEST(test_count_empty_search);
    RUN_TEST(test_count_null_source);
    RUN_TEST(test_count_null_search);
    RUN_TEST(test_count_no_matches);
    RUN_TEST(test_very_long_replacement);
    RUN_TEST(test_utf8_bytes);
    RUN_TEST(test_only_search_pattern);
    RUN_TEST(test_replacement_creates_pattern);
    RUN_TEST(test_search_substring_of_replacement);
    RUN_TEST(test_whitespace_handling);

    printf("\n--- EDGE CASE Tests ---\n\n");

    RUN_TEST(test_single_char_all);
    RUN_TEST(test_numeric_strings);
    RUN_TEST(test_mixed_content);
    RUN_TEST(test_path_strings);
    RUN_TEST(test_consecutive_then_non_match);
    RUN_TEST(test_non_match_then_consecutive);

    printf("\n===========================================\n");
    printf("Test Results: %d/%d passed", tests_passed, tests_run);
    if (tests_failed > 0) {
        printf(" (%d FAILED)", tests_failed);
    }
    printf("\n===========================================\n");

    return tests_failed > 0 ? 1 : 0;
}
