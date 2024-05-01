#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"

void setup_call_echo(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

TestSuite(call_echo, .init = setup_call_echo);

Test(call_echo, no_arguments)
{
    t_cmd cmd;
    cmd.s = (char *[]){"echo", NULL};

    call_echo(&cmd);
    fflush(stdout);
    FILE *output_file = cr_get_redirected_stdout();
    char output[1024];
    fgets(output, sizeof(output), output_file);
    cr_assert_str_eq(output, "\n", "echo without arguments should print a newline. Expected '\\n', got '%s'", output);
}

Test(call_echo, single_argument)
{
    t_cmd cmd;
    cmd.s = (char *[]){"echo", "Hello", NULL};

    call_echo(&cmd);
    fflush(stdout);
    FILE *output_file = cr_get_redirected_stdout();
    char output[1024];
    fgets(output, sizeof(output), output_file);
    cr_assert_str_eq(output, "Hello\n", "echo with a single argument should print the argument followed by a newline. Expected 'Hello\\n', got '%s'", output);
}

Test(call_echo, multiple_arguments)
{
    t_cmd cmd;
    cmd.s = (char *[]){"echo", "Hello", "World", NULL};

    call_echo(&cmd);
    fflush(stdout);
    FILE *output_file = cr_get_redirected_stdout();
    char output[1024];
    fgets(output, sizeof(output), output_file);
    cr_assert_str_eq(output, "Hello World\n", "echo with multiple arguments should print the arguments separated by spaces followed by a newline. Expected 'Hello World\\n', got '%s'", output);
}

Test(call_echo, n_option)
{
    t_cmd cmd;
    cmd.s = (char *[]){"echo", "-n", "Hello", NULL};

    call_echo(&cmd);
    fflush(stdout);
    FILE *output_file = cr_get_redirected_stdout();
    char output[1024];
    fgets(output, sizeof(output), output_file);
    cr_assert_str_eq(output, "Hello", "echo with -n option should print the arguments without a trailing newline. Expected 'Hello', got '%s'", output);
}

Test(call_echo, n_option_multiple)
{
    t_cmd cmd;
    cmd.s = (char *[]){"echo", "-n", "-n", "Hello", NULL};

    call_echo(&cmd);
    fflush(stdout);
    FILE *output_file = cr_get_redirected_stdout();
    char output[1024];
    fgets(output, sizeof(output), output_file);
    cr_assert_str_eq(output, "Hello", "echo with multiple -n options should print the arguments without a trailing newline. Expected 'Hello', got '%s'", output);
}

Test(call_echo, n_option_invalid)
{
    t_cmd cmd;
    cmd.s = (char *[]){"echo", "-n123", "Hello", NULL};

    call_echo(&cmd);
    fflush(stdout);
    FILE *output_file = cr_get_redirected_stdout();
    char output[1024];
    fgets(output, sizeof(output), output_file);
    cr_assert_str_eq(output, "-n123 Hello\n", "echo with an invalid -n option should treat it as a regular argument. Expected '-n123 Hello\\n', got '%s'", output);
}
