#ifndef SPELLCHECK_H
#define SPELLCHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <search.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

char *strtolower(char *word);
ENTRY *find(char *word);
int update(char *word);
int readFile(const char* fileName, ENTRY dict);
char *substr(char *str, size_t offset, size_t limit);
static void *checked_malloc(int len);
static void append(char *dst, int *dstLen, const char *src, int srcBegin, int len);
int deletion(char *word, char **array, int start_idx);
int transposition(char *word, char **array, int start_idx);
int alteration(char *word, char **array, int start_idx);
int insertion(char *word, char **array, int start_idx);
size_t edits1_rows(char *word);
char **edits1(char *word);
int array_exist(char **array, int rows, char *word);
char **known_edits2(char **array, int rows, int *e2_rows);
char *max(char **array, int rows);
void array_cleanup(char **array, int rows);
char *correct(char *word);

#endif
