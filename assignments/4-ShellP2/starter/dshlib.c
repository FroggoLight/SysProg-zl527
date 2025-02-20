#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;

    // TODO IMPLEMENT MAIN LOOP
    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        // remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            //free the thing
            break;
        }
        else if (strcmp(cmd_buff, "") == 0) {
            printf(CMD_WARN_NO_CMD);
        }
        else {
            build_cmd_buff(cmd_buff, &cmd);
            exec_cmd(&cmd);
            clear_cmd_buff(&cmd);
        }
    }
    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return OK;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd)
{   
    str_trim(cmd_line);
    cmd->argc = 0;  
    int arg_index = 0;
    int start_index = 0;
    int copy_len = 0; 
    memset(cmd, 0, sizeof(cmd_buff_t));
    int str_index = 0;
    int cmd_buffer_filled = 0;

    while (cmd_line[str_index] == ' ') str_index++;
    start_index = str_index;    
    while (cmd_line[str_index] != ' ' && cmd_line[str_index] != '\0' && cmd_line[str_index] != '"') {
        str_index++;
    }   
    copy_len = str_index - start_index;
    cmd->_cmd_buffer = malloc(copy_len + 1);
    cmd->argv[arg_index] = malloc(copy_len + 1);
    //printf("'%s'\n", &cmd_line[start_index]);
    strncpy(cmd->_cmd_buffer, &cmd_line[start_index], copy_len);
    strncpy(cmd->argv[arg_index], &cmd_line[start_index], copy_len);
    str_index++;
    arg_index++;
    cmd->argc++;
    
    while (cmd_line[str_index] != '\0') {
        while (cmd_line[str_index] == ' ') str_index++;
        start_index = str_index;
        while (cmd_line[str_index] != ' ' && cmd_line[str_index] != '\0' && cmd_line[str_index] != '"') {
            str_index++;
        }
        if (cmd_line[str_index] == '"') {
            str_index++;
            while (cmd_line[str_index] != '"' && cmd_line[str_index] != '\0') str_index++;
            copy_len = str_index - start_index;
            cmd->argv[arg_index] = malloc(copy_len);
            //printf("%s\n", &cmd_line[start_index]);
            strncpy(cmd->argv[arg_index], &cmd_line[start_index + 1], copy_len - 1);
            str_index++;
            arg_index++;
            cmd->argc++;
        }
        else {
            if (strcmp(&cmd_line[start_index], "") == 0) break;
            copy_len = str_index - start_index;
            cmd->argv[arg_index] = malloc(copy_len + 1);
            //printf("'%s'\n", &cmd_line[start_index]);
            strncpy(cmd->argv[arg_index], &cmd_line[start_index], copy_len);
            str_index++;
            arg_index++;
            cmd->argc++;
        }
    }
    return OK;
}

int exec_cmd(cmd_buff_t *cmd) {
    if (strcmp(cmd->_cmd_buffer, "cd") == 0) {
        if (cmd->argc == 2) {
            chdir(cmd->argv[1]);
        }
    }
    else {
        pid_t pid;
        int wait_status;
        pid = fork();

        if (pid < 0) {
            perror("fork error\n");
            exit(1);
        }
        else if (pid == 0) {
            execvp(cmd->_cmd_buffer, cmd->argv);
        }
        else {
            wait(&wait_status);
        }
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd) {
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->argv[i]);
    }
    free(cmd->_cmd_buffer);
    cmd->argc = 0;
    memset(cmd, 0, sizeof(cmd_buff_t));
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
