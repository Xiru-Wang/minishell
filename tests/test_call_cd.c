#include "../includes/minishell.h"
#include "../criterion/include/criterion/criterion.h"
#include "../criterion/include/criterion/redirect.h"
#include <sys/stat.h>

void setup(void)
{
    // Set up any necessary data or environment for the tests
}

void teardown(void)
{
    // Clean up any allocated resources or reset the environment
}

Test(call_cd, change_to_home_directory)
{
    t_data data;
    t_cmd cmd;

    // Get the actual home directory path
    char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        cr_assert_fail("HOME environment variable not found");
    }

    // Set up the environment variables
    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = ft_strjoin("HOME=", home_dir);
    data.env[1] = ft_strdup("PWD=/current/directory");
    data.env[2] = NULL;

    // Set up the command
    cmd.s = malloc(sizeof(char *) * 2);
    cmd.s[0] = ft_strdup("cd");
    cmd.s[1] = NULL;

    // Set up the pwd variable
    data.pwd = ft_strdup("/current/directory");

    // Call the function
    int ret = call_cd(&data, &cmd);

    // Check the return value
    cr_assert_eq(ret, 0, "Expected return value to be 0");

    // Check if the current directory has changed to the home directory
    cr_assert_str_eq(data.pwd, home_dir, "Expected current directory to be the home directory");

    // Clean up
    free(data.env[0]);
    free(data.env[1]);
    free(data.env);
    free(cmd.s[0]);
    free(cmd.s);
    free(data.pwd);
}

Test(call_cd, change_to_oldpwd)
{
    t_data data;
    t_cmd cmd;

    // Set up the environment variables
    data.env = malloc(sizeof(char *) * 4);
    data.env[0] = ft_strdup("HOME=/path/to/home");
    data.env[1] = ft_strdup("PWD=/current/directory");
    data.env[2] = ft_strdup("OLDPWD=/tmp/old_directory");
    data.env[3] = NULL;

    // Set up the command
    cmd.s = malloc(sizeof(char *) * 3);
    cmd.s[0] = ft_strdup("cd");
    cmd.s[1] = ft_strdup("-");
    cmd.s[2] = NULL;

    // Set up the old_pwd and pwd variables
    data.old_pwd = ft_strdup("/tmp/old_directory");
    data.pwd = ft_strdup("/current/directory");

    // Create the temporary directory
    mkdir("/tmp/old_directory", 0755);

    // Redirect stdout to capture the output
    cr_redirect_stdout();

    // Call the function
    int ret = call_cd(&data, &cmd);

    // Check the return value
    cr_assert_eq(ret, 0, "Expected return value to be 0");

    // Check if the current directory has changed to the old directory
    cr_assert_str_eq(data.pwd, "/tmp/old_directory", "Expected current directory to be the old directory");

    // Check if the old directory path is printed
    cr_assert_stdout_eq_str("/tmp/old_directory\n", "Expected old directory path to be printed");

    // Clean up
    free(data.env[0]);
    free(data.env[1]);
    free(data.env[2]);
    free(data.env);
    free(cmd.s[0]);
    free(cmd.s[1]);
    free(cmd.s);
    free(data.old_pwd);
    free(data.pwd);

    // Clean up the temporary directory
    rmdir("/tmp/old_directory");
}

Test(call_cd, change_to_specified_directory)
{
    t_data data;
    t_cmd cmd;

    // Set up the environment variables
    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = ft_strdup("HOME=/home/user");
    data.env[1] = ft_strdup("PWD=/current/directory");
    data.env[2] = NULL;

    // Set up the command
    cmd.s = malloc(sizeof(char *) * 3);
    cmd.s[0] = ft_strdup("cd");
    cmd.s[1] = ft_strdup("/tmp"); // Change to a directory that is guaranteed to exist
    cmd.s[2] = NULL;

    // Set up the pwd variable
    data.pwd = ft_strdup("/current/directory");

    // Call the function
    int ret = call_cd(&data, &cmd);

    // Check the return value
    cr_assert_eq(ret, 0, "Expected return value to be 0");

    // Check if the current directory has changed to the specified directory
    cr_assert_str_eq(data.pwd, "/tmp", "Expected current directory to be the specified directory");

    // Clean up
    free(data.env[0]);
    free(data.env[1]);
    free(data.env);
    free(cmd.s[0]);
    free(cmd.s[1]);
    free(cmd.s);
    free(data.pwd);
}

Test(call_cd, handle_invalid_directory)
{
    t_data data;
    t_cmd cmd;

    // Set up the environment variables
    data.env = malloc(sizeof(char *) * 3);
    data.env[0] = ft_strdup("HOME=/home/user");
    data.env[1] = ft_strdup("PWD=/current/directory");
    data.env[2] = NULL;

    // Set up the command
    cmd.s = malloc(sizeof(char *) * 3);
    cmd.s[0] = ft_strdup("cd");
    cmd.s[1] = ft_strdup("/invalid/directory");
    cmd.s[2] = NULL;

    // Set up the pwd variable
    data.pwd = ft_strdup("/current/directory");

    // Redirect stderr to capture the error message
    cr_redirect_stderr();

    // Call the function
    int ret = call_cd(&data, &cmd);

    // Check the return value
    cr_assert_eq(ret, 1, "Expected return value to be 1");

    // Check if the current directory remains unchanged
    cr_assert_str_eq(data.pwd, "/current/directory", "Expected current directory to remain unchanged");

    // Check if the error message is printed
    cr_assert_stderr_eq_str("minishell: cd: /invalid/directory: No such file or directory\n", "Expected error message to be printed");

    // Clean up
    free(data.env[0]);
    free(data.env[1]);
    free(data.env);
    free(cmd.s[0]);
    free(cmd.s[1]);
    free(cmd.s);
    free(data.pwd);
}
