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

int levenshtein(const char *s, int ls, const char *t, int lt);

char *correct(char *word, char **word_list);

char **spell_check(char **text, int nb_word);

void load(char *dict, char **word_list);

#endif
