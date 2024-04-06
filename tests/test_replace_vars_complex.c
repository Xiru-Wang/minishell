#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

// Assume the necessary functions and structures are defined
// (len_single_quo, check_valid_dollar, expand_dollar, t_data)

// Helper function to create a fake environment
static char **create_fake_env(const char *vars[], size_t count)
{
    char **env = malloc((count + 1) * sizeof(char *));
    for (size_t i = 0; i < count; i++)
    {
        env[i] = strdup(vars[i]);
    }
    env[count] = NULL;
    return env;
}

// Helper function to free the fake environment
static void free_fake_env(char **env)
{
    if (env != NULL)
    {
        for (size_t i = 0; env[i] != NULL; i++)
        {
            free(env[i]);
        }
        free(env);
    }
}

Test(replace_vars_complex, no_variables)
{
    char *input = "Hello, World!";
	t_data data = { .env = create_fake_env(NULL, 0) };
    char *expected = "Hello, World!";
    char *result = replace_vars_complex(input, &data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
	free_fake_env(data.env);
}

Test(replace_vars_complex, single_variable)
{
    char *input = "Hello, $USER!";
	const char *env_vars[] = { "USER=John" };
	t_data data = { .env = create_fake_env(env_vars, 1) };
    char *expected = "Hello, John!";
    char *result = replace_vars_complex(input, &data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
	free_fake_env(data.env);
}

Test(replace_vars_complex, multiple_variables)
{
    char *input = "$GREETING, $USER! It's $DAY.";
	    const char *env_vars[] = {
        "GREETING=Hi",
        "USER=Alice",
        "DAY=Monday"
    };
    t_data data = { .env = create_fake_env(env_vars, 3) };
    char *expected = "Hi, Alice! It's Monday.";
    char *result = replace_vars_complex(input, &data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
	free_fake_env(data.env);
}

Test(replace_vars_complex, single_quotes)
{
    char *input = "Hello, '$USER'!";
	t_data data = { .env = create_fake_env(NULL, 0) };
    char *expected = "Hello, '$USER'!";
    char *result = replace_vars_complex(input, &data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
	free_fake_env(data.env);
}

Test(replace_vars_complex, mixed_quotes)
{
    char *input = "bla'$USER'waw\"$USER\"\"$HOST\"over";
	    const char *env_vars[] = {
        "USER=USERNAME",
        "HOST=HOSTNAME"
    };
    t_data data = { .env = create_fake_env(env_vars, 2) };
    char *expected = "bla$USERwawUSERNAMEHOSTNAMEover";
    char *result = replace_vars_complex(input, &data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
	free_fake_env(data.env);
}

Test(replace_vars_complex, undefined_variable)
{
    char *input = "Hello, $USER! $UNDEFINED variable.";
	const char *env_vars[] = { "USER=Max" };
    t_data data = { .env = create_fake_env(env_vars, 1) };
    char *expected = "Hello, Max!  variable.";
    char *result = replace_vars_complex(input, &data);
    cr_assert_str_eq(result, expected, "Expected: %s, Got: %s", expected, result);
    free(result);
	free_fake_env(data.env);
}

// TODO: also handle this:
    /* char *input = "\"bla'$USER'waw\"$USER\"\"$HOST\"over\""; */
	//bla'janschroeder'wawjanschroederProd-6275621257over
