#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

#define FALSE 0
#define TRUE 1


int is_in(char *text, char *search, int nb_word){
  int i = 0, found = FALSE;
  while(i < nb_word && found == FALSE){
    if(text[i] == *search)
      found = TRUE;
    else i++;
  }
  return i;
}


const char *convert_to_command(char **text, int nb_word){
  // TODO : look for strstr(char, char) & gstring
  char *command = "";
  int i = 0, j = 0;
  if((i = is_in(*text, "ouvre", nb_word)) != nb_word)
    command = *(text + i + 1);
  else if((i = is_in(*text, "recherche", nb_word)) != nb_word){
    if((j = is_in(*(text), "sur", nb_word)) == nb_word &&
          (j = is_in(*(text), "dans", nb_word)) == nb_word){
      // TODO : find and launch the default browser
      strcat(command, "firefox");
      strcat(command, "\"");
      strcat(command, *(text));
      strcat(command, "\"");
    }
    else{
      if(j == 0){
        strcat(command, text[i + 1]);
        strcat(command, "\"");
        strcat(command, *(text + i + 2));
        strcat(command, "\"");
      }
      else{
        strcat(command, text[nb_word - 1]);
        strcat(command, "\"");
        for(int k = i; k < nb_word - 2; k++)
          strcat(command, text[i]);
        strcat(command, "\"");
      }
    }
  }
  return command;
}


void command_exec(const char *command){
  if(system(NULL) == 0)
    printf("No shell available");
  if(system(command) == -1)
    printf("The command to execute can not be executed");
}
