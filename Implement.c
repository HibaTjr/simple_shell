#include "shell.h"

/**
 * execute - Execute a command with its entire path variables.
 * @data: A pointer to the program's data.
 * Return: If successful, return zero, otherwise, return -1.
 */
int execute(data_of_program *data)
{
	int retval = 0, status;
	pid_t pid;

	/* Check for program in built-ins */
	retval = builtins_list(data);
	if (retval != -1) /* If program was found in built-ins */
		return (retval);

	/* Check for program in file system */
	retval = find_program(data);
	if (retval)
	{ /* If program not found */
		return (retval);
	}
	else
	{ /* If program was found */
		pid = fork(); /* Create a child process */
		if (pid == -1)
		{ /* If the fork call failed */
			perror(data->command_name);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{ /* I am the child process, I execute the program */
			retval = execve(data->tokens[0], data->tokens, data->env);
			if (retval == -1) /* If error when execve */
				perror(data->command_name), exit(EXIT_FAILURE);
		}
		else
		{ /* I am the parent, I wait and check the exit status of the child */
			wait(&status);
			if (WIFEXITED(status))
				errno = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				errno = 128 + WTERMSIG(status);
		}
	}
	return (0);
}
