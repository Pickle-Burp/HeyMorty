#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "execution.h"

int main(int argc, char **argv){
    if(argc == 1)
        err(EXIT_FAILURE, "no input");
    char text[argc - 1];
    for(int i = 1; i < argc; i++)
        text[i-1] = *argv[i];
    char **cutting_command = cut_command(text, argc - 1);
    const char *command = tab_to_command(**cutting_command);
    command_exec(command);
	return 0;
}
