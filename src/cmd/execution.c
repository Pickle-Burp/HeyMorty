#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
//#include <gmodule.h>

#define FALSE 0
#define TRUE 1


int is_in(char *text, char *search, int nb_word){
  int i = 0, found = FALSE;
  while(i < nb_word && found == FALSE){
    if(text[i] == *search)
      found = TRUE;
    i++;
  }
  return i;
}


const char *convert_to_command(char **text, int nb_word){
  // TODO : look for strstr(char, char) & gstring
  char *command = malloc(128*sizeof(char));
  int i = 0, j = 0, k = 0;
  /* to open an application */
  if((i = is_in(*text, "ouvre", nb_word)) != nb_word)
    command = *(text + i);
  /* to make a research */
  else if((i = is_in(*text, "recherche", nb_word)) != nb_word){
    /* if no browser specified, default browser */
    if((j = is_in(*(text + i), "sur", nb_word - i)) == nb_word - i &&
          (k = is_in(*(text + i), "dans", nb_word - i)) == nb_word - i){
      // TODO : find and launch the default browser
      printf("first i:%i j:%i k:%i n:%i\n", i, j, k, nb_word);
      strcat(command, "firefox");
      strcat(command, " \"");
      strcat(command, *(text + 1));
      strcat(command, "\"");
    }
    else{
      /* browser specified before the search */
      if(j == 1 || k == 1){
        printf("second\n");
        strcat(command, text[i + 1]);
        strcat(command, " \"");
        strcat(command, *(text + i + 2)); // didn't work : take just the first
        strcat(command, "\"");
      }
      /* browser specified after the search */
      else{
        printf("third\n");
        strcat(command, text[nb_word - 1]);
        strcat(command, " \"");
        for(int k = i; k < nb_word - 2; k++){
          strcat(command, text[i]);
          strcat(command, " ");
        }
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
