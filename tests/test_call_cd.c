#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

void setup(void)
{
    // Setup code to be executed before each test
}

void teardown(void)
{
    // Teardown code to be executed after each test
}

TestSuite(call_cd_tests, .init = setup, .fini = teardown);

Test(call_cd_tests, test_call_cd)
{
    t_data data;
    data.pwd = getcwd(NULL, 0);
    data.old_pwd = NULL;
    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = strdup("HOME=/home/user");
    data.env[1] = strdup("PWD=/home/user");
    data.env[2] = NULL;

    t_cmd cmd;
    cmd.s = malloc(sizeof(char *) * 2);
    cmd.s[0] = "cd";

    cmd.s[1] = NULL;
    cr_assert_eq(call_cd(&data, &cmd), 0);

    cmd.s[1] = "~";
    cr_assert_eq(call_cd(&data, &cmd), 0);

    cmd.s[1] = "-";
    cr_assert_eq(call_cd(&data, &cmd), 1);

    cmd.s[1] = "/tmp";
    cr_assert_eq(call_cd(&data, &cmd), 0);

    cmd.s[1] = "nonexistent";
    cr_assert_eq(call_cd(&data, &cmd), 1);

    free(cmd.s);
    free(data.pwd);
    for (int i = 0; data.env[i] != NULL; i++)
        free(data.env[i]);
    free(data.env);
}

Test(call_cd_tests, test_handle_cd_oldpwd)
{
    t_data data;
    data.old_pwd = NULL;

    cr_assert_eq(handle_cd_oldpwd(&data), NULL);

    data.old_pwd = strdup("/home/user");
    cr_assert_str_eq(handle_cd_oldpwd(&data), "/home/user");

    free(data.old_pwd);
}

Test(call_cd_tests, test_change_directory)
{
    t_data data;
    data.pwd = getcwd(NULL, 0);
    data.old_pwd = NULL;

    cr_assert_eq(change_directory(&data, "/tmp"), 0);
    cr_assert_eq(change_directory(&data, "nonexistent"), -1);

    free(data.pwd);
    free(data.old_pwd);
}

Test(call_cd_tests, test_update_pwd_variables)
{
    t_data data;
    data.pwd = strdup("/home/user");
    data.old_pwd = NULL;
    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = strdup("HOME=/home/user");
    data.env[1] = strdup("PWD=/home/user");
    data.env[2] = NULL;

    update_pwd_variables(&data);
    cr_assert_str_eq(data.pwd, "/home/user");
    cr_assert_eq(data.old_pwd, NULL);

    free(data.pwd);
    for (int i = 0; data.env[i] != NULL; i++)
        free(data.env[i]);
    free(data.env);
}

/**
 * @brief Test function for updating environment variables
 *
 * This function tests the update_env_var function by creating a test
 * environment with three variables and then updating each variable with a new
 * value. It asserts that the updated values are correct.
 *
 * @param call_cd_tests The test suite for call_cd
 * @param test_update_env_var The specific test case for updating environment
 * variables
 *
 * @return void
 */
Test(call_cd_tests, test_update_env_var)
{
    t_data data;
    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = strdup("HOME=/home/user");
    data.env[1] = strdup("PWD=/home/user");
    data.env[2] = NULL;

    update_env_var(&data, "PWD", "/tmp");
    cr_assert_str_eq(find_var("PWD", 3, data.env), "PWD=/tmp");

    update_env_var(&data, "OLDPWD", "/home/user");
    cr_assert_str_eq(find_var("OLDPWD", 6, data.env), "OLDPWD=/home/user");

    update_env_var(&data, "NEW_VAR", "value");
    cr_assert_str_eq(find_var("NEW_VAR", 7, data.env), "NEW_VAR=value");

    for (int i = 0; data.env[i] != NULL; i++)
        free(data.env[i]);
    free(data.env);
}

