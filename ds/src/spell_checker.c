#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf, fgets, strcpy */
#include <string.h>	/* strspan, strcmp, strdup */

#include "hash_table.h"	/* hash table function declerations */


#define DICTIONARY_FILE_PATH "/usr/share/dict/words"
#define HASH_TABLE_SIZE 44
#define VALID_WORD_SIZE 128
#define NUM_WORDS_TO_LOAD 2000

int TestDictionary(void);
int LoadDictionary(hash_table_t *table, const char *filename, char **loaded_words);

static size_t HashFunctionModulu44(const void *key);
static int MatchWords(const void *item, const void *data_to_compare);
static void FreeWords(char **loaded_words);
char *strdup(const char *c);


int main()
{
	
	TestDictionary();
	
	return 0;
}



int TestDictionary(void)
{
	char *loaded_words[NUM_WORDS_TO_LOAD] = {0};
	char user_input[VALID_WORD_SIZE] = {0};
	char *search_result = NULL;
	hash_table_t *table = HashTableCreate(HashFunctionModulu44, MatchWords, HASH_TABLE_SIZE);
	
	if (table == NULL)
	{
		return 1;
	}
	
	
	LoadDictionary(table, DICTIONARY_FILE_PATH, loaded_words);
	
	printf("\n\nLoad factor = %f, Standard deviation = %f\n\n", HashTableLoad(table), HashTableStandardDeviation(table));
	
	
	printf("Enter word to look in the dictionary. Enter 0 to end.\n");
	if (fgets(user_input, sizeof(user_input), stdin) == NULL)
	{
			return 1;
	}
	
	while (user_input[0] != '0')
	{
		search_result = NULL;
		user_input[strcspn(user_input, "\n")] = '\0';

		search_result = (char *)HashTableFind(table, user_input);
		if (NULL == search_result)
		{
			printf("Word not found.\n\n");
		}
		else
		{
			printf("\nSearch result: %s\n\n", search_result);
		}
		
		printf("Enter key to look in the dictionary.\nEnter 0 to end.\n\n");
		if (fgets(user_input, sizeof(user_input), stdin) == NULL)
		{
			return 1;
		}
		
	}
	
	FreeWords(loaded_words);
	HashTableDestroy(table);
	
	return 0;
}




int LoadDictionary(hash_table_t *table, const char *filename, char **loaded_words)
{
	char word[VALID_WORD_SIZE] = {0};
	size_t num_words_loaded = 0;
	char *word_dup = NULL;
	
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		return 1;
	}
	
	
	
	while (fgets(word, sizeof(word), file) != NULL && num_words_loaded < NUM_WORDS_TO_LOAD)
	{
		word[strcspn(word, "\n")] = '\0';
		word_dup = strdup(word);
		
        	if (word_dup == NULL)
		{
			fclose(file);
			return 1;
		}
		
		
		if (HashTableInsert(table, word_dup, word_dup) != 0)
		{
			return 1;
		}
		
		loaded_words[num_words_loaded] = word_dup;
		++num_words_loaded;
	}

	fclose(file);
	
	return 0;
}



static size_t HashFunctionModulu44(const void *key)
{
	char *word = (char *)key;
	return (word[0] % HASH_TABLE_SIZE);
}


static int MatchWords(const void *item, const void *data_to_compare)
{
 	return (!strcmp((char *)item, (char *)data_to_compare));
}



char *strdup(const char *c)
{
	char *dup = malloc(strlen(c) + 1);

	if (dup != NULL)
	{
		strcpy(dup, c);
	}

	return dup;
}



static void FreeWords(char **loaded_words)
{
	size_t i = 0;
	
	for (i = 0; i < NUM_WORDS_TO_LOAD; ++i)
	{
		free(loaded_words[i]);
	}
}
