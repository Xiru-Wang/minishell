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


Test(call_exit, no_arguments, .init = setup_call_exit, .fini = teardown_call_exit)
{
    t_data data;
    t_cmd cmd;

    memset(&data, 0, sizeof(data)); // Clearing the data structure
    cmd.s = (char *[]){"exit", NULL};

    int exit_status = call_exit(&cmd, &data);
    cr_assert_eq(exit_status, 0, "Expected exit status to be 0 for no arguments, but got %d", exit_status);
}

Test(call_exit, too_many_arguments, .init = setup_call_exit, .fini = teardown_call_exit)
{
    t_data data;
    t_cmd cmd;

    memset(&data, 0, sizeof(data)); // Clearing the data structure
    cmd.s = (char *[]){"exit", "42", "extra", NULL};

    int exit_status = call_exit(&cmd, &data);
    cr_assert_eq(exit_status, EXIT_FAILURE, "Expected exit status to be %d (EXIT_FAILURE) for too many arguments, but got %d", EXIT_FAILURE, exit_status);
}
