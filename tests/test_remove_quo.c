#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

Test(remove_quo, empty_string)
{
    char *input = strdup("");
    char *expected = strdup("");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, no_quotes)
{
    char *input = strdup("Hello, World!");
    char *expected = strdup("Hello, World!");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, single_quotes)
{
    char *input = strdup("'Hello', 'World!'");
    char *expected = strdup("Hello, World!");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, double_quotes)
{
    char *input = strdup("\"Hello\", \"World!\"");
    char *expected = strdup("Hello, World!");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, mixed_quotes)
{
    char *input = strdup("'Hello', \"World!\"");
    char *expected = strdup("Hello, World!");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, nested_quotes)
{
	// "'Hello'", '"World"'
    char *input = strdup("\"'Hello'\", '\"World!\"'");
	char *expected = strdup("'Hello', \"World!\"");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, custom_quotes1)
{
    char *input = strdup("hi\"hi\"' '\"hi\"");
    char *expected = strdup("hihi hi");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}

Test(remove_quo, custom_quotes2)
{
    char *input = strdup("hi\"hi\"' '\"hi\"");
    char *expected = strdup("hihi hi");
    char *result = remove_quo(input);
    cr_assert_str_eq(result, expected);
    free(input);
    free(expected);
    free(result);
}
