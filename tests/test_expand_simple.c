#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

// Assume the necessary functions are defined
// (ft_calloc, ft_strlcat, ft_strlen, ft_isdigit, ft_substr, find_env, char_is_valid, char_to_str)

Test(expand_simple, no_variables)
{
    char *input = "Hello, World!";
    char *expected = "Hello, World!";
    char *env[] = { NULL };
    char *result = expand_simple(input, env);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

Test(expand_simple, single_variable)
{
    char *input = "Hello, $USER!";
    char *expected = "Hello, John!";
    char *env[] = { "USER=John", NULL };
    char *result = expand_simple(input, env);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

Test(expand_simple, multiple_variables)
{
    char *input = "$GREETING, $USER! It's $DAY.";
    char *expected = "Hello, Alice! It's Monday.";
    char *env[] = { "GREETING=Hello", "USER=Alice", "DAY=Monday", NULL };
    char *result = expand_simple(input, env);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

// TODO: CRASH!
/* Test(expand_simple, undefined_variable) */
/* { */
/*     char *input = "Hello, $USER! $UNDEFINED variable."; */
/*     char *expected = "Hello, Bob!  variable."; */
/*     char *env[] = { "USER=Bob", NULL }; */
/*     char *result = expand_simple(input, env); */
/*     cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result); */
/*     free(result); */
/* } */

Test(expand_simple, single_quotes)
{
    char *input = "Hello, '$USER'!";
    char *expected = "Hello, '$USER'!";
    char *env[] = { "USER=John", NULL };
    char *result = expand_simple(input, env);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

Test(expand_simple, double_quotes)
{
    char *input = "Hello, \"$USER\"!";
    char *expected = "Hello, \"John\"!";
    char *env[] = { "USER=John", NULL };
    char *result = expand_simple(input, env);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

Test(expand_simple, mixed_quotes)
{
    char *input = "Hello, '$USER', \"$USER\"!";
    char *expected = "Hello, '$USER', \"John\"!";
    char *env[] = { "USER=John", NULL };
    char *result = expand_simple(input, env);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}
