#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

#define OPEN "open"
#define SHOW "show"
#define SEARCH "search"

char **cut_text(char **text, int nb_word) {
  char **cutting_command;
  if (text[0] == "lance" || text[0] == "ouvre") {
    cutting_command[0] = OPEN;
    cutting_command[1] = text[1];
  }
  if (text[0] == "montre-moi") {
    cutting_command[0] = SHOW;
    cutting_command[1] = text[nb_word - 2];
    cutting_command[2] = text[nb_word - 1];
  }
  if (text[0] == "cherche") {
    cutting_command[0] = SEARCH;
    cutting_command[1] = text[nb_word - 1];
    for (int i = 1; i < nb_word - 2; i++)
      strcat(cutting_command[2], text[i]);
  }
  return cutting_command;
}

const char *convert_to_command(char **tab_command) {
  char *command = "";
  if (tab_command[0] == OPEN)
    command = tab_command[1];
  else if (tab_command[0] == SEARCH) {
    strcat(command, tab_command[1]);
    strcat(command, " ");
    strcat(command, "\"");
    strcat(command, tab_command[2]);
    strcat(command, "\"");
  } else if (tab_command[0] == SHOW) {
    if (tab_command[1] == "dossier") {
      strcat(command, "ls ");
      strcat(command, tab_command[2]);
    } else {
      strcat(command, "cat");
      strcat(command, tab_command[2]);
    }
  }
  return command;
}

void command_exec(const char *command) {
  if (system(NULL) == 0)
    printf("No shell available");
  if (system(command) == -1)
    printf("The command to execute can not be executed");
}
