#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;   
    int cmd_index = 0; 
    memset(clist, 0, sizeof(command_list_t));
    char* token = strtok(cmd_line, PIPE_STRING);
    int exe_index = 0;
    int arg_index = 0;
    str_trim(token);
    while (token != NULL) {
  
        clist->num += 1;
        if (clist->num > 8) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }  

        exe_index = 0;
        while (token[exe_index] != SPACE_CHAR && token[exe_index] != '\0') {
            exe_index++;
            if (exe_index >= EXE_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }
        strncpy(clist->commands[cmd_index].exe, token, exe_index);

        arg_index = exe_index;
        while (token[arg_index] != '\0') {
            arg_index++;
            if (arg_index >= ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
        }
        strncpy(clist->commands[cmd_index].args, token + exe_index + 1, arg_index);

        token = strtok(NULL, PIPE_STRING);
        if (token == NULL) {
            break;
        }
        str_trim(token);  
        cmd_index++;
  
        
    }
    printf(CMD_OK_HEADER, clist->num);
    int i = 0;
    while (i < clist->num) {
        printf("<%d> %s", i + 1, clist->commands[i].exe);
        if (clist->commands[i].args[0] != '\0') {
            printf(" [%s]", clist->commands[i].args);
        }
        printf("\n");
        i++;
    }

    return OK;
}

void str_trim(char *str) {
    int index_a, index_b;
    index_a = 0;
    while(str[index_a] == ' ') {
        index_a++;
    }

    index_b = 0;
    while(str[index_a + index_b] != '\0') {
        str[index_b] = str[index_a + index_b];
        index_b++;
    }
    str[index_b] = '\0';

    index_a = -1;
    index_b = 0;
    while(str[index_b] != '\0') {
        if(str[index_b] != SPACE_CHAR && str[index_b] != '\n') {
            index_a = index_b;
        }
        index_b++;
    }

    str[index_a + 1] = '\0';
}