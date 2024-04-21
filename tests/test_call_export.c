#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

void setup_call_export(void)
{
	cr_redirect_stdout();
	cr_redirect_stderr();
}

TestSuite(call_export, .init = setup_call_export);

Test(call_export, valid_export_single_variable)
{
    t_data data;
    t_cmd cmd;

    data.env = malloc(sizeof(char *) * 2);
    data.env[0] = strdup("PATH=/usr/bin");
    data.env[1] = NULL;

    cmd.s = (char *[]){"export", "NEWVAR=testvalue", NULL};

    call_export(&cmd, &data);

    cr_assert_str_eq(data.env[1], "NEWVAR=testvalue", "NEWVAR should be set to 'testvalue'");
    free_arr(data.env);
}

Test(call_export, invalid_export_numeric_start)
{
    t_data data;
    t_cmd cmd;

    data.env = malloc(sizeof(char *));
    data.env[0] = NULL;

    cmd.s = (char *[]){"export", "1INVALID=wrong", NULL};

    int result = call_export(&cmd, &data);

    cr_assert_eq(result, EXIT_FAILURE, "Should fail because variable names cannot start with a digit");
    free_arr(data.env);
}

Test(call_export, update_existing_variable)
{
    t_data data;
    t_cmd cmd;

    data.env = malloc(sizeof(char *) * 2);
    data.env[0] = strdup("VAR=initial");
    data.env[1] = NULL;

    cmd.s = (char *[]){"export", "VAR=updated", NULL};

    call_export(&cmd, &data);

    cr_assert_str_eq(data.env[0], "VAR=updated", "VAR should be updated to 'updated'");
    free_arr(data.env);
}

Test(call_export, export_without_arguments)
{
    t_data data;
    t_cmd cmd;

    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = strdup("VAR1=value1");
    data.env[1] = strdup("VAR2=value2");
    data.env[2] = NULL;

    cmd.s = (char *[]){"export", NULL};

    call_export(&cmd, &data);

    // Adjusted to match direct environment variable output
    cr_assert_stdout_eq_str("VAR1=value1\nVAR2=value2\n", "Should print all environment variables directly");
    free_arr(data.env);
}
