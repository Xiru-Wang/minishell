#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

// Assume the necessary functions and structures are defined
// (len_single_quo, check_valid_dollar, expand_dollar)

// Helper function to set environment variables
static void set_env_vars(const char *vars[], size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        putenv(strdup(vars[i]));
    }
}

// Helper function to unset environment variables
static void unset_env_vars(const char *vars[], size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        char *var = strdup(vars[i]);
        char *equals_pos = strchr(var, '=');
        if (equals_pos != NULL)
        {
            *equals_pos = '\0';
            unsetenv(var);
        }
        free(var);
    }
}

Test(replace_vars_complex, no_variables)
{
    char *input = "Hello, World!";
    char *expected = "Hello, World!";
    char *result = replace_vars_complex(input);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

Test(replace_vars_complex, single_variable)
{
    char *input = "Hello, $TESTUSER!";
    const char *env_vars[] = { "TESTUSER=John" };
    set_env_vars(env_vars, 1);
    char *expected = "Hello, John!";
    char *result = replace_vars_complex(input);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    unset_env_vars(env_vars, 1);
}

Test(replace_vars_complex, multiple_variables)
{
    char *input = "$GREETING, $TESTUSER! It's $DAY.";
    const char *env_vars[] = {
        "GREETING=Hi",
        "TESTUSER=Alice",
        "DAY=Monday"
    };
    set_env_vars(env_vars, 3);
    char *expected = "Hi, Alice! It's Monday.";
    char *result = replace_vars_complex(input);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    unset_env_vars(env_vars, 3);
}

Test(replace_vars_complex, single_quotes)
{
    char *input = "Hello, '$TESTUSER'!";
    char *expected = "Hello, '$TESTUSER'!";
    char *result = replace_vars_complex(input);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
}

Test(replace_vars_complex, mixed_quotes)
{
    char *input = "bla'$TESTUSER'waw\"$TESTUSER\"\"$TESTHOST\"over";
    const char *env_vars[] = {
        "TESTUSER=USERNAME",
        "TESTHOST=HOSTNAME"
    };
    set_env_vars(env_vars, 2);
    char *expected = "bla$TESTUSERwawUSERNAMEHOSTNAMEover";
    char *result = replace_vars_complex(input);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    unset_env_vars(env_vars, 2);
}

Test(replace_vars_complex, undefined_variable)
{
    char *input = "Hello, $TESTUSER! $UNDEFINED variable.";
    const char *env_vars[] = { "TESTUSER=Max" };
    set_env_vars(env_vars, 1);
    char *expected = "Hello, Max!  variable.";
    char *result = replace_vars_complex(input);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    unset_env_vars(env_vars, 1);
}
