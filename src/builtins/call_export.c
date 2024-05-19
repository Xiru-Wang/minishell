#include "../../includes/minishell.h"

/**
 * Finds the position of the '=' character in a given string.
 *
 * This function iterates through the characters of the input string until it
 * finds the '=' character.
 *
 * @param str The input string to search for the '=' character.
 * @return The position of the '=' character in the string, or -1 if the
 * character is not found.
 */
static int	equal_sign(char *str)
{
	int i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (str[i] == '=' ? i : -1);
}

/**
 * Removes quotes from a given string.
 *
 * This function takes a string and a quote character as input, and removes all
 * occurrences of the quote character from the string.
 *
 * @param str The string from which quotes are to be removed
 * @param quote The quote character to be removed from the string
 */
void	delete_quotes(char *str, char quote)
{
	char *read;
	char *write;

	read = str;
	write = str;
	while (*read)
	{
		if (*read != quote)
			*write++ = *read;
		read++;
	}
	*write = '\0';
}

// Check if the environment variable name is valid
static int	is_valid_identifier(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * Prints an error message indicating that the provided string is not a valid
 * identifier for export.
 *
 * This function takes a string as input and prints an error message to the
 * standard error stream indicating that the provided string is not a valid
 * identifier for export in the context of a minishell. The error message
 * includes the string that was provided as part of the message.
 *
 * @param str The string that is not a valid identifier for export
 * @return An integer representing the exit status of the function
 * (EXIT_FAILURE)
 */
static int	export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (EXIT_FAILURE);
}


/**
 * @brief Validates the environment variable string.
 *
 * This function checks if the given string is a valid environment variable
 * string. It ensures that the string does not start with a digit or an equal
 * sign, and that all characters in the string are valid identifiers. It also
 * checks if the string contains an equal sign, which is required for a valid
 * environment variable string.
 *
 * @param str The environment variable string to be validated.
 * @return Returns EXIT_SUCCESS if the string is a valid environment variable
 * string, otherwise returns an error code.
 */
static int	check_parameter(char *str)
{
    int i;

    if (ft_isdigit(str[0]) || str[0] == '=')
        return (export_error(str));
    i = 0;
    while (str[i] && str[i] != '=')
    {
        if (!is_valid_identifier(str[i]))
            return (export_error(str));
        i++;
    }
    return (str[i] == '=' ? EXIT_SUCCESS : export_error(str));
}

static char	**add_var(char **env, char *str)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	i = 0;
	while (env[len])
		len++;
	new_env = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (NULL);
	while (i < len)
	{
		new_env[i] = ft_strdup(env[i]);
		if (new_env[i] == NULL)
		{
			while (i > 0)
				free(new_env[--i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[len] = ft_strdup(str);
	if (new_env[len] == NULL)
	{
		while (--i >= 0)
			free(new_env[i]);
		free(new_env);
		return (NULL);
	}
	new_env[len + 1] = NULL;
	return (new_env);
}

/**
 * Update or add a new variable in the environment.
 *
 * This function updates an existing variable in the environment with the
 * provided string, or adds a new variable if the string does not match any
 * existing variable.
 *
 * @param data A pointer to the data structure containing the environment
 * variables.
 * @param str The string containing the variable to update or add in the format
 * "variable=value".
 * @return Returns EXIT_SUCCESS if the variable was successfully updated or
 * added, or EXIT_FAILURE if an error occurred.
 */
static int	update_or_add_var(t_data *data, char *str)
{
	int pos;
	int i;
	char **new_env;

	pos = equal_sign(str);
	if (pos == -1)
		return (EXIT_FAILURE);
	if (str[pos + 1] == '\"' || str[pos + 1] == '\'')
		delete_quotes(str + pos + 1, str[pos + 1]);
	i = 0;
	while (data->env[i]) {
		if (ft_strncmp(data->env[i], str, pos) == 0)
		{
			free(data->env[i]);
			data->env[i] = ft_strdup(str);
			if (data->env[i] == NULL)
				return (EXIT_FAILURE);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	new_env = add_var(data->env, str);
	if (!new_env)
		return (EXIT_FAILURE);
	if (data->env != NULL)
		free_double_ptr(data->env);
	data->env = new_env;
	return (EXIT_SUCCESS);
}

/**
 * Calls the export command to display or modify environment variables.
 *
 * This function takes in a command structure `cmd` and a data structure `data`.
 * If no additional arguments are provided in the command, it prints all
 * environment variables to the standard output. If additional arguments are
 * provided, it checks each argument and updates or adds the corresponding
 * environment variable in the data structure.
 *
 * @param cmd A pointer to the command structure containing the arguments for
 * the export command.
 * @param data A pointer to the data structure containing the environment
 * variables.
 * @return Returns EXIT_SUCCESS if the export command is executed successfully,
 * EXIT_FAILURE otherwise.
 */
int	call_export(t_cmd *cmd, t_data *data)
{
	int i;

	if (!cmd->s[1])
	{
		i = 0;
		while (data->env && data->env[i])
		{
			ft_putstr_fd(data->env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
			i++;
		}
		return (EXIT_SUCCESS);
	}
	i = 1;
	while (cmd->s[i])
	{
		if (check_parameter(cmd->s[i]) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (update_or_add_var(data, cmd->s[i]) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

