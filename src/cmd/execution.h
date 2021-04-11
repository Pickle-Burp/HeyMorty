#ifndef EXECUTION_H
#define EXECUTION_H

char **cut_text(char **text, int nb_word);
const char *convert_to_command(char **cutting_command[]);
void command_exec(const char *command);

#endif
