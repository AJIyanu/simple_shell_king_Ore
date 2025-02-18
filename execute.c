#include "main.h"

/**
 * execute - execute the command
 *
 * @argv: args to be carried out
 * @env: environmental variable
 */

void execute(char **argv, char **env)
{
	pid_t child_pid;
	int check, check_child = 0, i = 0, rest = 0;
	char **path_ptr;
	struct stat buf;
	retrn_node *result = NULL;

	check = check_command(argv, env);
	if (check == 1)
		return;
	if (strstr(argv[0], "/"))
	{
		result = malloc(sizeof(retrn_node));
		result->status = stat(argv[0], &buf), result->cmd_path = strdup(argv[0]);
	}
	else
	{
		path_ptr = split_path();
		while (path_ptr[i])
		{
			result = executable_check(path_ptr[i], argv[0]);
			if (result->status == 1)
			{
				rest = 0;
				break;
			}
			rest = 1, free(result), i++;
		}
		cleanup(path_ptr);
	}
	if (rest == 0)
	{
		child_pid = fork();
		if (child_pid == 0)
			check_child = execve(result->cmd_path, argv, env);
		if (check_child == -1)
			puts("No such file or directory");
		cleanup(argv), free(result->cmd_path), free(result), wait(&child_pid);
	}
	else
		cleanup(argv), puts("No such file or directory");
}



/**
 * count_array - coutns the number of words are
 * in an array of pointers
 * @cmd: array of pointers
 * Return: number of arrays
 */

int count_array(char **cmd)
{
	int i = 0;

	while (cmd[i] != NULL)
	{
		i++;
	}
	return (i);
}

/**
 * cleanup - cleans up memory for array of pointers
 * @argv: array of pointers that is to be cleared
 * Return: NULL
 */

void cleanup(char **argv)
{
	int i = 0;

	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);

}


/**
 * executable_check - checks using stat, if binary file is present
 *
 * @path_ptr: array of paths
 * @argv: command passed
 * Return: struct
 */

retrn_node *executable_check(char *path_ptr, char *argv)
{
	int len_cmd = 0, result;
	char *new_pth;
	struct stat buf;
	retrn_node *retrn_value;

	retrn_value = malloc(sizeof(retrn_node));
	if (!retrn_value)
	{
		perror("malloc");
		return (NULL);
	}
	len_cmd = strlen(argv) + 2;
	new_pth = malloc((strlen(path_ptr) + len_cmd));
	strcpy(new_pth, path_ptr);
	strcat(new_pth, "/");
	strcat(new_pth, argv);
	result = stat(new_pth, &buf);
	if (result == 0)
	{
		retrn_value->status = 1;
		retrn_value->cmd_path = strdup(new_pth);
		free(new_pth);
		return (retrn_value);
	}
	else
	{
		retrn_value->status = 0;
		retrn_value->cmd_path = NULL;
		free(new_pth);
	}
	return (retrn_value);
}
