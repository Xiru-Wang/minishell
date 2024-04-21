/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xiruwang <xiruwang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 16:23:50 by xiwang            #+#    #+#             */
/*   Updated: 2024/04/21 13:07:22 by jschroed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expander(t_data *data, char *input) {
    char *result = ft_strdup(input);  // Using ft_strdup from libft
    char *pos = ft_strchr(result, '$');

    while (pos) {
        char *end = pos + 1;
        while (isalnum(*end) || *end == '_') end++;  // Find the end of the variable name
        char *var_name = ft_substr(pos, 1, end - (pos + 1));  // ft_substr allocates and extracts the substring
        char *value = find_env_var(data, var_name);
        free(var_name);

        if (value) {
            size_t new_size = ft_strlen(result) - (end - pos) + ft_strlen(value) + 1;
            char *new_result = malloc(new_size);
            if (!new_result) {
                free(result);
                return NULL;  // Handle memory allocation failure
            }
            ft_strlcpy(new_result, result, pos - result + 1);
            ft_strlcpy(new_result + (pos - result), value, ft_strlen(value) + 1);
            ft_strlcpy(new_result + (pos - result) + ft_strlen(value), end, new_size - ((pos - result) + ft_strlen(value)));

            free(result);
            result = new_result;
            pos = ft_strchr(result, '$');  // Search for next variable
        } else {
            pos = ft_strchr(end, '$');  // Move to next '$'
        }
    }

    return result;
}

void ft_error(int code, t_data *data) {
    perror("Error encountered");
    free_data(data);  // Assuming this function frees all allocated resources
    exit(code);
}

void dup_cmd(t_cmd *cmd, t_data *data, int end[2], int fd_in) {
    // First, expand the command
    char *expanded_cmd = expander(data, cmd->s[0]);
    cmd->s[0] = expanded_cmd;  // Update the command with the expanded version

    if (fd_in != STDIN_FILENO) {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    if (end[1] != STDOUT_FILENO) {
        dup2(end[1], STDOUT_FILENO);
        close(end[1]);
    }
    execve(cmd->s[0], cmd->s, data->env);
    ft_error(EXIT_FAILURE, data);  // Handle errors if execve fails
    free(expanded_cmd); // Cleanup the expanded command
}

int pipe_wait(pid_t *pid, int count) {
    int status;
    int i = 0;
    while (i <= count) {
        waitpid(pid[i], &status, 0);
        if (WIFEXITED(status))
            g_exit_status.error_num = WEXITSTATUS(status);
        i++;
    }
    return EXIT_SUCCESS;
}

int executor(t_data *data) {
    int end[2], fd_in = STDIN_FILENO;
    t_cmd *current = data->cmd_list;
    int cmd_count = 0;

    while (current != NULL) {
        if (current->next)
            pipe(end);

        // Expand the current command before forking
        current->s[0] = expander(data, current->s[0]);

        data->pid[cmd_count] = fork();
        if (data->pid[cmd_count] == 0) {  // Child process
            dup_cmd(current, data, end, fd_in);
        } else if (data->pid[cmd_count] < 0) {
            ft_error(5, data);
        }

        if (current->next)
            close(end[1]);
        if (fd_in != STDIN_FILENO)
            close(fd_in);

        fd_in = end[0];
        current = current->next;  // Move to the next command
        cmd_count++;
    }
    pipe_wait(data->pid, cmd_count - 1);
    return 0;
}


// OLD
//
// //if outfile exsit, data goes to file instead of pipe
/*
The strategy for your shell is to have the parent process
do all the piping and redirection before forking the processes.
In this way the children will inherit the redirection.
*/

/*  */
/* int	g_exit_code; */
/* static int	pipe_wait(int *pid, int pipe_num); */
/*  */
/* int	executor(t_cmd *cmd, t_data *data) */
/* { */
/*     int	i; */
/*     int	end[2]; */
/*  */
/*     i = 0; */
/*     while (cmd) */
/*     { */
/*         if (cmd->next) */
/*             if (pipe(end) == -1)//create pipe in parent */
/*                 free_exit("pipe failed", data, STDERR_FILENO); */
/*         check_hd(cmd); */
/*         get_redir_fd_array(cmd); */
/*         redirect_fds(cmd, end); */
/*         data->pid[i] = fork(); */
/*         if (data->pid[i] == 0) */
/*         { */
/*             close(end[0]); */
/*             if (cmd->is_builtin) */
/*                 call_builtin(cmd); */
/*             else */
/*                 call_cmd(data, cmd); */
/*         } */
/*         close(end[1]); */
/*         cmd = cmd->next; */
/*         i++; */
/*     } */
/*     pipe_wait(data->pid, (data->cmd_num - 1)); */
/*     return (0); */
/* } */
/*  */
/* static int	pipe_wait(int *pid, int pipe_num) */
/* { */
/*     int	i; */
/*     int	status; */
/*  */
/*     i = 0; */
/*     while (i < pipe_num) */
/*     { */
/*         waitpid(pid[i], &status, 0); */
/*         i++; */
/*     } */
/*     waitpid(pid[i], &status, 0); */
/*     if (WIFEXITED(status)) */
/*         g_exit_code = WEXITSTATUS(status); */
/*     return (EXIT_SUCCESS); */
/* } */
/*  */
/*  */
/* // Function that handles the execution of commands */
/* void executor(t_cmd *cmd_list, t_data *data) { */
/*     t_cmd *current = cmd_list; */
/*     char *cmd_path; */
/*  */
/*     while (current != NULL) { */
/*         // Find the command path */
/*         cmd_path = find_command_path(current->s[0], data->env); */
/*         if (cmd_path == NULL) { */
/*             fprintf(stderr, "%s: command not found\n", current->s[0]); */
/*         } else { */
/*             execute_command(cmd_path, current->s, data->env); */
/*             free(cmd_path); */
/*         } */
/*         current = current->next; */
/*     } */
/* } */

