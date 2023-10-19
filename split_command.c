#include "main.h"

/**
 * split_commands - split the commands into tokens to be passed
 * @string: command to be split
 * Description: splits the commands into tokens
 *
 * Return: An array of pointers
 */
char **split_commands(char *string)
{
	int len;
	char **words_ptr, *words, *tokens, *delim;
	int i = 0, j = 0;

	delim = " \t\n";
	len = word_count(string, delim); /* gets the length of the string */
	words_ptr = malloc((len + 1) * sizeof(char *));
	if (!words_ptr)
	{
		perror("malloc");
		return (NULL);
	}
	words = strdup(string);
	tokens = split_token(words, delim);
	while (tokens != NULL)
	{
		char *new_tokens = remove_new_line(tokens);

		words_ptr[i] = malloc(sizeof(char) * (_strlen(new_tokens) + 1));
		printf("creating %dth address at %p\n", i, words_ptr[i]);
		if (words_ptr[i] == NULL)
		{
			perror("malloc");
			while (j < i)
			{
				free(words_ptr[j]);
				j++;
			}
			free(words_ptr);
		}
		if (words_ptr[i])
		{
			strcpy(words_ptr[i], new_tokens); /* invalid write size of 1 */
			printf("%s has been copied successfully\n", words_ptr[i]);
			tokens = split_token(NULL, delim);
		}
		i++;
		free(new_tokens);
	}
	words_ptr[i] = NULL;
	free(words);
	return (words_ptr);
}

/**
 * remove_new_line - removes the newline character that is added
 * automatically when getline is used t read data
 * @word: word read by getline
 * Return: Stripped down word
 */

char *remove_new_line(char *word)
{
	int i = 0;
	char *word_copy;

	word_copy = strdup(word);

	for (i = 0; i <= _strlen(word_copy); i++)
	{
		if (word_copy[i] == '\n')
			word_copy[i] = '\0';
		word_copy[i] = word[i];
	}
	return (word_copy);
}
