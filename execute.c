#include "main.h"

/**
 * execute - Execute the command.
 *
 * @argv: Args to be carried out.
 * @env: Environmental variable.
 */
void execute(char **argv, char **env)
{
	int check, exec_check;
	retrn_node *result = NULL;
	pid_t child_pid;

	if (strchr(argv[0], '/'))
		result = check_slash_path(argv);
	else
	{
		check = check_command(argv, env);
		if (check == 1)
			return;
		result = find_executable(argv[0]);
		if (result == NULL)
		{
			fprintf(stderr, "%s: cannot access '%s': No such file or directory\n",
				argv[0], argv[1]);
			cleanup(argv);
			exit(2);
		}
	}
	child_pid = fork();
	if (child_pid == 0)
	{
		exec_check = execve(result->cmd_path, argv, env);
		if (exec_check == -1)
			exit(2);
		_exit(EXIT_SUCCESS);
	}
	printf("before abortion 1 Clean\n");
	cleanup(argv);
	printf("before abortion 2 cmd_path\n");
//	free(result->cmd_path);
	printf("before abortion 3 result\n");
	free(result);
	printf("before abortion 4 waiting\n");
	wait(&child_pid);
}

/**
 * find_executable - Find an executable file in PATH.
 *
 * @filename: The file to search for.
 * Return: A retrn_node struct containing the status and the path
 * to the executable.
 */
retrn_node *find_executable(char *filename)
{
	char **path_ptr = split_path();
	retrn_node *result = NULL;
	int i = 0;

	while (path_ptr[i])
	{
		result = check_executable(path_ptr[i], filename);
		if (result->status == 1)
		{
			cleanup(path_ptr);
			return (result);
		}
		i++;
		free(result);
	}
	cleanup(path_ptr);
	return (NULL);
}

/**
 * check_executable - Check if a file is executable.
 *
 * @dir: Directory path.
 * @filename: The file to check.
 * Return: A retrn_node struct containing the status and
 * the path to the executable.
 */
retrn_node *check_executable(char *dir, char *filename)
{
	int len_cmd = strlen(filename) + 2, result;
	char *new_path = malloc(strlen(dir) + len_cmd);
	retrn_node *retrn_value = malloc(sizeof(retrn_node));

	if (!new_path || !retrn_value)
	{
		perror("malloc");
		free(new_path);
		free(retrn_value);
		return (NULL);
	}

	strcpy(new_path, dir);
	strcat(new_path, "/");
	strcat(new_path, filename);

	result = access(new_path, X_OK);
	if (result == 0)
	{
		retrn_value->status = 1;
		retrn_value->cmd_path = new_path;
	}
	else
	{
		retrn_value->status = 0;
		retrn_value->cmd_path = NULL;
		free(new_path);
	}

return (retrn_value);
}

/**
 * cleanup - Clean up memory for an array of pointers.
 * @argv: Array of pointers to be cleared.
 */
void cleanup(char **argv)
{
	int i = 0;

	while (argv[i])
	{
		printf("freeing memory %p at %d\n", argv[i], i);
		free(argv[i]);
		i++;
	}
	free(argv);
}

/**
 * check_slash_path - check if the full path provided
 * exists
 * @argv: array of pointers
 * 
 * Return: struct
 */

retrn_node *check_slash_path(char **argv)
{
	int result;

	retrn_node *retrn_value = NULL;
	printf("Command is: %s\n", argv[0]);
	result = access(argv[0], X_OK);

	retrn_value = malloc(sizeof(retrn_node));
	if (!retrn_value)
	{
		perror("malloc");
		return (NULL);
	}
	if (result == 0)
	{
		retrn_value->status = 1;
		retrn_value->cmd_path = argv[0];
	}
	else
	{
		fprintf(stderr, "%s: cannot access '%s': No such file or directory\n",
			argv[0], argv[1]);
		cleanup(argv);
		free(retrn_value->cmd_path);
		free(retrn_value);
		exit(2);
	}
	return (retrn_value);
}
