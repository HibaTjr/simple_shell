#include "shell.h"

/**
 * builtin_env - Displays the environment where the shell runs.
 * @data: Struct for the program's data.
 * Return: 0 on success, or a non-zero number if an error occurred.
 */
int builtin_env(data_of_program *data)
{
	int i;
	char cpname[50] = {'\0'};
	char *var_copy = NULL;
	/* If no arguments provided */
	if (data->tokens[1] == NULL)
		print_environ(data);
	else
	{
		/* Iterate through the argument to find '=' character */
		for (i = 0; data->tokens[1][i]; i++)
		{
			if (data->tokens[1][i] == '=')
			{
/* Temporarily change the value of an existing variable with the same name */
				var_copy = str_duplicate(env_get_key(cpname, data));
				if (var_copy != NULL)
					env_set_key(cpname, data->tokens[1] + i + 1, data);
/* Print the environment */
				print_environ(data);
				if (env_get_key(cpname, data) == NULL)
				{
					/* Print the variable if it does not exist in the environment */
					_print(data->tokens[1]);
					_print("\n");
				}
				else
				{
					/* Restore the old value of the variable */
					env_set_key(cpname, var_copy, data);
					free(var_copy);
				}
				return (0);
			}
			cpname[i] = data->tokens[1][i];
		}
		errno = 2;
		perror(data->command_name);
		errno = 127;
	}
	return (0);
}

/**
 * builtin_set_env - Sets or updates an environment variable.
 * @data: Struct for the program's data.
 * Return: 0 on success, or a non-zero number if an error occurred.
 */
int builtin_set_env(data_of_program *data)
{
	/* Validate arguments */
	if (data->tokens[1] == NULL || data->tokens[2] == NULL)
		return (0);
	if (data->tokens[3] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}

	env_set_key(data->tokens[1], data->tokens[2], data);
	return (0);
}

/**
 * builtin_unset_env - Unsets an environment variable.
 * @data: Struct for the program's data.
 * Return: 0 on success, or a non-zero number if an error occurred.
 */
int builtin_unset_env(data_of_program *data)
{
	/* Validate arguments */
	if (data->tokens[1] == NULL)
		return (0);
	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}
	env_remove_key(data->tokens[1], data);
	return (0);
}
