#ifndef SPELLCHECK_H
#define SPELLCHECK_H

#include <ctype.h>
#include <stdio.h>
#include <err.h>
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
int load(char *dict);
hash_table *hash_create();
int hash_insert(char *word);
int hash_func(char *word);
hash_node *create_hash_node(const char *word);
int check(char *word, char **closest_word);
char *correct(char *word, char **closest_word, int nb_word);
int levenshtein(const char *s, int ls, const char *t, int lt);

#endif
