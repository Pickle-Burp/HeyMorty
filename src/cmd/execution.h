#ifndef EXECUTION_H
#define EXECUTION_H

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

int is_in(char **text, char *search, int nb_word);
const char *convert_to_command(char **text, int nb_word);
void command_exec(const char *command);

#endif