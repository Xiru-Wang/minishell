#include "../../includes/minishell.h"

//display nothing if no args for the cmd

static char	**reset_env(t_data *data, char **env, int k);

int call_unset(t_cmd *cmd, t_data *data) {
    int i = 1;
    int k;
    char *temp;

    if (!cmd->s[1]) {
        return EXIT_SUCCESS;
    }

    while (cmd->s[i]) {
        temp = ft_strjoin(cmd->s[i], "=");
        if (!temp) {
            return EXIT_FAILURE;  // Handle memory allocation failure
        }
        k = 0;
        while (data->env[k]) {
            if (ft_strncmp(data->env[k], temp, ft_strlen(temp)) == 0) {
                data->env = reset_env(data, data->env, k);
                break;
            }
            k++;
        }
        free(temp);
        i++;
    }
    return EXIT_SUCCESS;
}

static char **reset_env(t_data *data, char **env, int k) {
    int i = 0;
    int j = 0;
    char **new_env;

    (void)data;  // Unused parameter

    // Count the number of environment variables
    while (env[i]) {
        i++;
    }

    // Allocate memory for the new environment array
    new_env = (char **)malloc(sizeof(char *) * i);  // i already includes space for NULL terminator
    if (!new_env) {
        return env;  // Handle memory allocation failure by returning the original environment
    }

    i = 0;
    while (env[i]) {
        if (i != k) {
            new_env[j] = env[i];
            j++;
        } else {
            free(env[i]);
        }
        i++;
    }

    new_env[j] = NULL;  // Terminate the new array
    free(env);

    return new_env;
}
