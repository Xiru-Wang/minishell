#include "../../includes/minishell.h"

static void free_arr(char **arr)
{
    int i;

    if (arr == NULL)
        return;

    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

static char **ft_arrdup(char **arr)
{
    int i;
    int len;
    char **new_arr;

    if (arr == NULL)
        return (NULL);

    len = 0;
    while (arr[len])
        len++;

    new_arr = (char **)malloc(sizeof(char *) * (len + 1));
    if (new_arr == NULL)
        return (NULL);

    i = 0;
    while (arr[i])
    {
        new_arr[i] = ft_strdup(arr[i]);
        if (new_arr[i] == NULL)
        {
            free_arr(new_arr);
            return (NULL);
        }
        i++;
    }
    new_arr[i] = NULL;

    return (new_arr);
}

static void free_data(t_data *data)
{
    free_arr(data->env);
    free(data->line);
    free_token_list(&data->token_list);
    free_cmd_list(&data->cmd_list);
    free(data->pwd);
    free(data->old_pwd);
}

static int is_str_digit(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

static int determine_exit_code(char **str, t_data *data)
{
    int exit_code;
	(void)data;

    if (!str[1])
        exit_code = 0;
    else
    {
        if (is_str_digit(str[1]))
            exit_code = ft_atoi(str[1]);
        else
        {
            ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
            ft_putstr_fd(str[1], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
            exit_code = 255;
        }
    }
    free_double_ptr(str);
    return exit_code;
}

int call_exit(t_cmd *cmd, t_data *data)
{
    char **str;
    int exit_code;

    ft_putendl_fd("exit", STDERR_FILENO);
    if (cmd->s[1] && cmd->s[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
        return (EXIT_FAILURE);
    }
    str = ft_arrdup(cmd->s);
    exit_code = determine_exit_code(str, data);
    free_data(data);
    return exit_code;
}
