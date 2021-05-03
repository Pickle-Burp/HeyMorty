#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

int is_in(char *text, char *search, int nb_word){
  int i = 0, found = FALSE;
  while(i <= nb_word && found == FALSE){
    if(text[i] == *search)
      found = TRUE;
    i++;
  }
  return found;
}


const char *convert_to_command(char **text, int nb_word) {
  char *command = "";
  if(is_in(*text, "ouvre", nb_word) == TRUE)
    command = text[nb_word - 1];
  return command;
}

void command_exec(const char *command) {
  if (system(NULL) == 0)
    printf("No shell available");
  if (system(command) == -1)
    printf("The command to execute can not be executed");
}
