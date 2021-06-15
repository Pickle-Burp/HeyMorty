#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

/* Check if a word is in a text */
int is_in(char **text, char *search, int nb_word){
  int i = 0;
  while(i < nb_word){
    if(strcmp(text[i], search) == 0)
      return i;
    i++;
  }
  return i;
}

/* Convert a text into a linux command */
const char *convert_to_command(char **text, int nb_word){
  char *command = calloc(256, sizeof(char));
  printf("%s\n", command);
  int i = 0, j = 0, k = 0;

  /* to open an application */
  if((i = is_in(text, "ouvre", nb_word)) != nb_word){
    if(nb_word == 1)
      err(1, "You must enter the application to open");
    j = is_in(text, "application", nb_word);
    command = *(text + (j == nb_word ? i + 1 : j + 1));
  }
  
  /* to make a research */
  else if((i = is_in(text, "recherche", nb_word)) != nb_word){
    if(nb_word == 1)
      err(1, "You must enter a search");

    // if no browser specified, default browser
    if((j = is_in(text, "sur", nb_word)) == nb_word &&
          (k = is_in(text, "dans", nb_word)) == nb_word){
      strcat(command, "xdg-open \"https://www.google.com/search?q=");
      for(int x = i + 1; x < nb_word; x++){
        strcat(command, text[x]);
        strcat(command, " ");
      }
      strcat(command, "\"");
    }
    
    // Browser specified
    else{ 
      // browser specified after the search
      if(j == nb_word - 2 || k == nb_word - 2){
        strcat(command, text[nb_word - 1]);
        strcat(command, " \"google.com/search?q=");
        for(int x = i + 1; x < nb_word - 2; x++){
          strcat(command, text[x]);
          strcat(command, " ");
        }
        strcat(command, "\"");
      }

      // browser specified before the searc
      else{
        strcat(command, text[i + 2]);
        strcat(command, " \"google.com/search?q=");
        for(int x = 2 + (j == nb_word - 1 ? k : j); x < nb_word; x++){
          strcat(command, text[x]);
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
    err(1, "No shell available");
  if(system(command) == -1)
    err(1, "The command to execute can not be executed");
}