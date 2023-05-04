#include <stdio.h>
#include <string.h>

#include "command.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("argc < 4 or argc > 4\n");
        return 0;
    }    
    const char *command = argv[1];
    const char *input_file = argv[2];
    const char *output_file = argv[3];

    if (strcmp(command, "encode") == 0) 
        encode_file(input_file, output_file);
    else if (strcmp(command, "decode") == 0) 
        decode_file(input_file, output_file);
        
    else
    {
        printf("Wrong command name (expect <<decode>> or <<encode>>)\n");
        return 0;
    }
    
}