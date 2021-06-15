#ifndef EXECUTION_H
#define EXECUTION_H

int is_in(char **text, char *search, int nb_word);

const char *convert_to_command(char **text, int nb_word);

void command_exec(const char *command);

#endif
