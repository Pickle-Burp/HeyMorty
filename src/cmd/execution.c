#include "execution.h"

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
    char *command = calloc(512, sizeof(char));
    int i = 0, j = 0, k = 0, l = 0;

    /* to open an application */
    if((i = is_in(text, "ouvre", nb_word)) != nb_word){
        if(nb_word == 1)
            err(1, "You must enter the application to open");
        j = is_in(text, "application", nb_word);
        k = is_in(text, "l'application", nb_word);
        l = is_in(text, "lapplication", nb_word);
        command = *(text +
                    (j == nb_word
                     ? (k == nb_word
                        ? (l == nb_word
                           ? i + 1
                           : l + 1)
                        : k + 1)
                     : j + 1));
        if(strcmp(command, "google") == 0 || strcmp(command, "chrome") == 0)
            command = "google-chrome-stable"; //google package name on nixos
    }

        /* to find a file or directory */
    else if(is_in(text, "trouve", nb_word) != nb_word){
        if(nb_word == 1)
            err(1, "You must enter the file or directory to find");
        strcat(command, "find ~/ $PWD -type ");
        if((j = is_in(text, "fichier", nb_word)) != nb_word){
            strcat(command, "f -name \"");
            strcat(command, *(text + j + 1));
        } else if((k = is_in(text, "dossier", nb_word)) != nb_word){
            strcat(command, "d -name \"");
            strcat(command, *(text + k + 1));
        }
        strcat(command, "\" ");
        strcat(command, "-exec thunar ");
        strcat(command, "{} \\;");
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
                if(strcmp(text[nb_word - 1], "google") == 0 || strcmp(text[nb_word - 1], "chrome") == 0)
                    strcat(command, "google-chrome-stable"); //google package name on nixos
                else
                    strcat(command, text[nb_word - 1]);
                strcat(command, " \"google.com/search?q=");
                for(int x = i + 1; x < nb_word - 2; x++){
                    strcat(command, text[x]);
                    strcat(command, " ");
                }
                strcat(command, "\"");
            }

                // browser specified before the search
            else{
                if(strcmp(text[i + 2], "google") == 0 || strcmp(text[i + 2], "chrome") == 0)
                    strcat(command, "google-chrome-stable"); //google package name on nixos
                else
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