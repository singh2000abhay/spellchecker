/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

uint32_t hash(const char* word); // source: https://en.wikipedia.org/wiki/Jenkins_hash_function

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node *hashtable[143091];

int wordcount = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int hashindex = hash(word);
    node *cursor = hashtable[hashindex];
    
    while(cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 
bool load(const char *dictionary)
{
    FILE *fp = fopen(dictionary, "r");
    
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }
    
    char word[LENGTH + 1];
    int hashindex;
    
    for (int i = 0; i < 143091; i++)
    {
        hashtable[i] = NULL;
    }
    
    while(fscanf(fp, "%s\n", word) != EOF)
    {
        node *newnode = malloc(sizeof(node));
        if (newnode == NULL)
        {
            unload();
            return false;
        }
        
        strcpy(newnode->word, word);
        
        hashindex = hash(word);
        
        if (hashtable[hashindex] == NULL)
        {
            hashtable[hashindex] = newnode;
            newnode->next = NULL;
        }
        else
        {
            newnode->next = hashtable[hashindex];
            hashtable[hashindex] = newnode;
        }
        wordcount++;
    }
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (wordcount > 0)
        return wordcount;
    else
        return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int i = 0; i < 143091; i++)
    {
        node *cursor = hashtable[i];
        while(cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}

uint32_t hash(const char *word) // source: https://en.wikipedia.org/wiki/Jenkins_hash_function
{    
    size_t length = strlen(word);
    size_t i = 0;
    int hash = 0;
    while (i != length)
    {
        hash += hash << 10;
        hash ^= hash >> 6;
        i++;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return abs(hash) % 143091;
}
