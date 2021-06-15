#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
//#include <gmodule.h>

#define FALSE 0
#define TRUE 1


int is_in(char **text, char *search, int nb_word){
    // TODO ? supp found -> return i / -1
    int i = 0;
    while(i < nb_word){
        if(strcmp(text[i], search) == 0)
            return i;
        i++;
    }
    return i;
}


const char *convert_to_command(char **text, int nb_word){
    // TODO : look for strstr(char, char) & gstring

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

        /* if no browser specified, default browser */
        if((j = is_in(text, "sur", nb_word)) == nb_word &&
           (k = is_in(text, "dans", nb_word)) == nb_word){
            // TODO : find and launch the default browser
            strcat(command, "firefox");
            strcat(command, " \"google.com/search?q=");
            for(int x = i + 1; x < nb_word; x++){
                strcat(command, text[x]);
                strcat(command, " ");
            }
            strcat(command, "\"");
        }

            /* Browser specified */
        else{
            /* browser specified after the search */
            if(j == nb_word - 2 || k == nb_word - 2){
                strcat(command, text[nb_word - 1]);
                strcat(command, " \"google.com/search?q=");
                for(int x = i + 1; x < nb_word - 2; x++){
                    strcat(command, text[x]);
                    strcat(command, " ");
                }
                strcat(command, "\"");
            }

                /* browser specified before the search */
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
        printf("No shell available");
    if(system(command) == -1)
        printf("The command to execute can not be executed");
}
