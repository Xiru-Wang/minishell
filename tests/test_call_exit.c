#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

void setup_call_exit(void)
{
    cr_redirect_stderr();  // Redirects stderr to capture output for verification
}

void teardown_call_exit(void)
{
    cr_redirect_stderr();  // Redirects stderr to capture output for verification
}

/**
 * @brief Test function for the call_exit function with no arguments
 *
 * This function initializes the necessary data structures and command for
 * testing the call_exit function with no arguments. It then calls the call_exit
 * function and asserts that the exit status is 0.
 *
 * @param state The testing state for the test case
 *
 * @return void
 */
Test(call_exit, no_arguments, .init = setup_call_exit, .fini = teardown_call_exit)
{
    t_data data;
    t_cmd cmd;

    cmd.s = (char *[]){"exit", NULL};
    data.env = NULL;
    data.line = NULL;
    data.token_list = NULL;
    data.cmd_list = NULL;
    data.pwd = NULL;
    data.old_pwd = NULL;

    int exit_status = call_exit(&cmd, &data);
    cr_assert_eq(exit_status, 0, "Expected exit status to be 0 for no arguments");
}

/**
 * @brief Test function for the call_exit function with a numeric argument
 *
 * This function tests the call_exit function with a specific numeric argument
 * to ensure that it returns the correct exit status.
 *
 * @param state The testing state for the test case
 *
 * @return void
 */
Test(call_exit, numeric_argument, .init = setup_call_exit, .fini = teardown_call_exit)
{
    t_data data;
    t_cmd cmd;

    cmd.s = (char *[]){"exit", "42", NULL};
    data.env = NULL;
    data.line = NULL;
    data.token_list = NULL;
    data.cmd_list = NULL;
    data.pwd = NULL;
    data.old_pwd = NULL;

    int exit_status = call_exit(&cmd, &data);
    cr_assert_eq(exit_status, 42, "Expected exit status to be the numeric argument 42");
}

/**
 * @brief Test case for the call_exit function with a non-numeric argument.
 *
 * This test case initializes the necessary data structures and command
 * arguments to simulate a call to the call_exit function with a non-numeric
 * argument. The expected behavior is that the function should return an exit
 * status of 255.
 *
 * @param state The testing state provided by the CMocka testing framework.
 */
Test(call_exit, non_numeric_argument, .init = setup_call_exit, .fini = teardown_call_exit)
{
    t_data data;
    t_cmd cmd;

    cmd.s = (char *[]){"exit", "abc", NULL};
    data.env = NULL;
    data.line = NULL;
    data.token_list = NULL;
    data.cmd_list = NULL;
    data.pwd = NULL;
    data.old_pwd = NULL;

    int exit_status = call_exit(&cmd, &data);
    cr_assert_eq(exit_status, 255, "Expected exit status to be 255 for non-numeric argument");
}

/**
 * @brief Test function for the call_exit function with too many arguments
 *
 * This function tests the call_exit function with too many arguments provided
 * in the command. It initializes the necessary data structures and command
 * parameters, then calls the call_exit function and checks if the exit status
 * matches the expected value (EXIT_FAILURE).
 *
 * @param state The testing state for the test case
 *
 * @return void
 */
Test(call_exit, too_many_arguments, .init = setup_call_exit, .fini = teardown_call_exit)
{
    t_data data;
    t_cmd cmd;

    cmd.s = (char *[]){"exit", "42", "extra", NULL};
    data.env = NULL;
    data.line = NULL;
    data.token_list = NULL;
    data.cmd_list = NULL;
    data.pwd = NULL;
    data.old_pwd = NULL;

    int exit_status = call_exit(&cmd, &data);
    cr_assert_eq(exit_status, EXIT_FAILURE, "Expected exit status to be EXIT_FAILURE for too many arguments");
}
