#ifndef SPELLCHECK_H
#define SPELLCHECK_H

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <err.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define HASH_TABLE_SIZE 65536

typedef struct hash_node{
    char *word;
    struct hash_node *next;
} hash_node;

typedef struct hash_table{
    int numwords;
    hash_node **nodes;
} hash_table;


char **spell_check(char **text, int nb_word);

bool load(char *dict);

hash_table *hash_create();

bool hash_insert(char *word);

int hash_func(char *word);

hash_node *create_hash_node(const char *word);

bool check(char *word);

#endif
