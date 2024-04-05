#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

// This test verifies that the call_env function correctly prints the
// environment variables and returns EXIT_SUCCESS when given a valid
// environment.
Test(call_env, test_small_env) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {"VAR1=value1", "VAR2=value2", NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("VAR1=value1\nVAR2=value2\n");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}

// This test checks that the call_env function handles an empty environment
// correctly by printing an empty string and returning EXIT_SUCCESS.
Test(call_env, test_empty) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}

// This test ensures that the call_env function correctly prints multiple
// environment variables and returns EXIT_SUCCESS when provided with a larger
// set of environment variables.
Test(call_env, test_middle_env) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {"VAR1=value1", "VAR2=value2", "VAR3=value3", NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("VAR1=value1\nVAR2=value2\nVAR3=value3\n");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}

// Test with a larger number of environment variables:
Test(call_env, test_large_env) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {"VAR1=value1", "VAR2=value2", "VAR3=value3", "VAR4=value4", "VAR5=value5", NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("VAR1=value1\nVAR2=value2\nVAR3=value3\nVAR4=value4\nVAR5=value5\n");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}

// Test with environment variables containing special characters:
Test(call_env, test_special_characters) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {"VAR1=value1", "VAR2=value2 with spaces", "VAR3=value3\nwith\nnewlines", NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("VAR1=value1\nVAR2=value2 with spaces\nVAR3=value3\nwith\nnewlines\n");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}

// Test with environment variables containing empty values:
Test(call_env, test_empty_values) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {"VAR1=", "VAR2=value2", "VAR3=", NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("VAR1=\nVAR2=value2\nVAR3=\n");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}

// Test with environment variables containing only keys (without values):
Test(call_env, test_only_keys) {
    t_cmd cmd;
    cmd.data = malloc(sizeof(t_data));
    char *env[] = {"VAR1", "VAR2", "VAR3", NULL};
    cmd.data->env = env;

    cr_redirect_stdout();
    int result = call_env(&cmd);

    cr_assert_stdout_eq_str("VAR1\nVAR2\nVAR3\n");
    cr_assert_eq(result, EXIT_SUCCESS);

    free(cmd.data);
}
