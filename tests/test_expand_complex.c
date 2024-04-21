#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

// Assume the necessary functions and structures are defined
// (check_valid_dollar, remove_quo, replace_vars_complex)

// Helper function to create a t_data structure with environment variables
static t_data *create_data(const char *vars[], size_t count)
{
    t_data *data = malloc(sizeof(t_data));
    data->env = malloc(sizeof(char *) * (count + 1));
    for (size_t i = 0; i < count; i++)
    {
        data->env[i] = strdup(vars[i]);
    }
    data->env[count] = NULL;
    return data;
}

// Helper function to free the t_data structure
static void free_test_data(t_data *data)
{
    for (size_t i = 0; data->env[i] != NULL; i++)
    {
        free(data->env[i]);
    }
    free(data->env);
    free(data);
}

Test(expand_complex, no_variables_word)
{
    char *input = "Hello, World!";
    char *expected = "Hello, World!";
    t_data *data = create_data(NULL, 0);
    char *result = expand_complex(input, WORD, data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    free_test_data(data);
}

Test(expand_complex, no_variables_quo)
{
    char *input = "\"Hello, World!\"";
    char *expected = "Hello, World!";
    t_data *data = create_data(NULL, 0);
    char *result = expand_complex(input, QUO, data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    free_test_data(data);
}

Test(expand_complex, single_variable_word)
{
    char *input = "Hello, $USER!";
    const char *env_vars[] = { "USER=John" };
    t_data *data = create_data(env_vars, 1);
    char *expected = "Hello, John!";
    char *result = expand_complex(input, WORD, data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    free_test_data(data);
}

Test(expand_complex, single_variable_quo)
{
    char *input = "\"Hello, $USER!\"";
    const char *env_vars[] = { "USER=Alice" };
    t_data *data = create_data(env_vars, 1);
    char *expected = "Hello, Alice!";
    char *result = expand_complex(input, QUO, data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    free_test_data(data);
}

Test(expand_complex, multiple_variables_word)
{
    char *input = "$GREETING, $USER! It's $DAY.";
    const char *env_vars[] = {
        "GREETING=Hi",
        "USER=Bob",
        "DAY=Monday"
    };
    t_data *data = create_data(env_vars, 3);
    char *expected = "Hi, Bob! It's Monday.";
    char *result = expand_complex(input, WORD, data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    free_test_data(data);
}

Test(expand_complex, multiple_variables_quo)
{
    char *input = "\"$GREETING, $USER! It's $DAY.\"";
    const char *env_vars[] = {
        "GREETING=Hello",
        "USER=Alice",
        "DAY=Sunday"
    };
    t_data *data = create_data(env_vars, 3);
    char *expected = "Hello, Alice! It's Sunday.";
    char *result = expand_complex(input, QUO, data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
    free_test_data(data);
}

// TODO: CRASH!
/* Test(expand_complex, undefined_variable_word) */
/* { */
/*     char *input = "Hello, $USER! $UNDEFINED variable."; */
/*     const char *env_vars[] = { "USER=John" }; */
/*     t_data *data = create_data(env_vars, 1); */
/*     char *expected = "Hello, John!  variable."; */
/*     char *result = expand_complex(input, WORD, data); */
/*     cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result); */
/*     free(result); */
/*     free_test_data(data); */
/* } */

// TODO: CRASH!
/* Test(expand_complex, undefined_variable_quo) */
/* { */
/*     char *input = "\"Hello, $USER! $UNDEFINED variable.\""; */
/*     const char *env_vars[] = { "USER=Alice" }; */
/*     t_data *data = create_data(env_vars, 1); */
/*     char *expected = "Hello, Alice!  variable."; */
/*     char *result = expand_complex(input, QUO, data); */
/*     cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result); */
/*     free(result); */
/*     free_test_data(data); */
/* } */
