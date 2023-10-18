#include "shell.h"

/**
 * print_alias - Prints, adds, or removes aliases.
 * @data: Struct for the program's data.
 * @alias: Name of the alias to be printed (or NULL to print all aliases).
 * Return: 0 on success, or a non-zero number if an error occurred.
 */
int print_alias(data_of_program *data, char *alias)
{
	int i, j, alias_length;
	char buffer[250] = {'\0'};

	if (data->alias_list)
	{
		alias_length = (alias) ? str_length(alias) : 0;
		for (i = 0; data->alias_list[i]; i++)
		{
			if (!alias || (str_compare(data->alias_list[i], alias, alias_length)
				&& data->alias_list[i][alias_length] == '='))
			{
				for (j = 0; data->alias_list[i][j]; j++)
				{
					buffer[j] = data->alias_list[i][j];
					if (data->alias_list[i][j] == '=')
						break;
				}
				buffer[j + 1] = '\0';
				buffer_add(buffer, "'");
				buffer_add(buffer, data->alias_list[i] + j + 1);
				buffer_add(buffer, "'\n");
				_print(buffer);
			}
		}
	}

	return (0);
}

/**
 * get_alias - Retrieves the value of a specific alias.
 * @data: Struct for the program's data.
 * @name: Name of the requested alias.
 * Return: Pointer to the value of the alias if found, NULL otherwise.
 */
char *get_alias(data_of_program *data, char *name)
{
	int i, alias_length;

	/* Validate the arguments */
	if (name == NULL || data->alias_list == NULL)
		return (NULL);

	alias_length = str_length(name);

	for (i = 0; data->alias_list[i]; i++)
	{
		/* Iterates through the alias list and checks for the alias name */
		if (str_compare(name, data->alias_list[i], alias_length) &&
			data->alias_list[i][alias_length] == '=')
		{
			/* Returns the value of the key NAME= if found */
			return (data->alias_list[i] + alias_length + 1);
		}
	}
	/* Returns NULL if the alias is not found */
	return (NULL);
}

/**
 * set_alias - Adds or overrides an alias.
 * @alias_string: Alias to be set in the form (name='value').
 * @data: Struct for the program's data.
 * Return: 0 on success, or a non-zero number if an error occurred.
 */
int set_alias(char *alias_string, data_of_program *data)
{
	int i, j;
	char buffer[250] = {'0'}, *temp = NULL;
	/* Validate the arguments */
	if (alias_string == NULL ||  data->alias_list == NULL)
		return (1);
	/* Iterates through the alias_string to find the '=' character */
	for (i = 0; alias_string[i]; i++)
	{
		if (alias_string[i] != '=')
			buffer[i] = alias_string[i];
		else
		{
			/* Search if the value of the alias is another alias */
			temp = get_alias(data, alias_string + i + 1);
			break;
		}
	}
	/* Iterates through the alias list and checks for the alias name */
	for (j = 0; data->alias_list[j]; j++)
	{
		if (str_compare(buffer, data->alias_list[j], i) &&
			data->alias_list[j][i] == '=')
		{
			/* If the alias already exists, free it first */
			free(data->alias_list[j]);
			break;
		}
	}
	/* Add the alias */
	if (temp)
	{
		/* If the value of the alias is another alias */
		buffer_add(buffer, "=");
		buffer_add(buffer, temp);
		data->alias_list[j] = str_duplicate(buffer);
	}
	else
		/* If the value of the alias is a regular string */
		data->alias_list[j] = str_duplicate(alias_string);
	return (0);
}
