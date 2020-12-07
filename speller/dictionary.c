// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include "strings.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define SIZE 65000


// defining a word counter
unsigned int word_size = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = SIZE;

// Hash table
node *hashtable[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // Creating a variable to be able to lowercase it
    char tmpword[strlen(word)];
    strcpy(tmpword, word);

    // lowering the word before hashing
    for (int i = 0, num = strlen(tmpword); i < num; i++)
    {
        tmpword[i] = tolower(tmpword[i]);
    }

    // Hashing
    int index = hash(tmpword);

    // Checking misspelling
    for (node *trav = (hashtable[index]); trav != NULL; trav = trav->next)
    {
        if (!(strcasecmp(trav->word, tmpword))) // then it is spelled well
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    unsigned int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            n = word[i] - 'a' + 1;
        }
        else
        {
            n = 27;
        }
        hash = ((hash << 3) + n) % N;

    }
    return hash;

}

// Loads dictionary into memory, sreturning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r"); //reading the file
    if (file == NULL)
    {
        return false;
    }

    //initializing the hashtable to nulls
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    char tmpword[LENGTH + 1];

    while ((fscanf(file, "%s", tmpword)) != EOF) //reading all words of dictionary
    {

        // counting words
        word_size++;

        // Preparing a node for linking
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            unload();
            fclose(file);
            return false;
        }

        // Copying the word into a node
        strcpy(n->word, tmpword);

        //hashing the word
        int index = hash(n->word);

        //linking new element
        if (hashtable[index] == NULL) //if it was an empty linked list
        {
            (hashtable[index]) = n;
            n->next = NULL;
        }
        else
        {
            n->next = (hashtable[index])->next;
            (hashtable[index])->next = n;
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_size;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < SIZE; i++) // iterating over the entire hash table
    {
        // for each array index we have a linked list to free
        if (hashtable[i] == NULL)
        {
            continue;
        }

        node *cursor = hashtable[i];
        do
        {
            node *tmp = cursor; // pointing to the to-be-deleted node
            cursor = cursor->next; // pointing to the next before deleting
            free(tmp);
        }
        while (cursor != NULL);
    }
    return true;
}
