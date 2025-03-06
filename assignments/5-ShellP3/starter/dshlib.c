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
    command_list_t clist;

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
            rc = build_cmd_list(cmd_buff, &clist);
            if (rc < 0) {
                free_cmd_list(&clist);
                if (rc == -2) {
                    printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                }
                if (rc == -8) {
                    printf(CMD_ERR_PIPE_PARSING);
                }

            }
            else {
                execute_pipeline(&clist);
                free_cmd_list(&clist);
            }

            //clear_cmd_buff(&cmd);
        }
    }
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;   
    int build_cmd_buff_rc = 0;
    memset(clist, 0, sizeof(command_list_t));
    char* token = strtok(cmd_line, PIPE_STRING);
    if (token == NULL) {
        return ERR_PIPE_PARSING;
    }
    str_trim(token);
    while (token != NULL) {

        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }  

        cmd_buff_t *cmd = malloc(sizeof(cmd_buff_t));
        build_cmd_buff_rc = build_cmd_buff(token, cmd);
        if (build_cmd_buff_rc < 0) {
            return build_cmd_buff_rc;
        }
        clist->commands[clist->num] = *cmd;
        clist->num++;

        //printf("%d\n", clist->num);
        token = strtok(NULL, PIPE_STRING);
        if (token == NULL) {
            break;
        }
        str_trim(token);
        
    }

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd)
{   
    cmd->argc = 0;  
    int arg_index = 0;
    int start_index = 0;
    int copy_len = 0; 
    int str_index = 0;

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

int free_cmd_list(command_list_t *clist) {

    for (int i = 0; i < clist->num; i++) {
        free_cmd_buff(&clist->commands[i]);
    }

    clist->num = 0;

    return OK;

}

int free_cmd_buff(cmd_buff_t *cmd) {
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->argv[i]); 
        cmd->argv[i] = NULL;
    }

    free(cmd->_cmd_buffer);  // Free the command buffer
    cmd->_cmd_buffer = NULL;

    cmd->argc = 0;
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

// From the professor's example, modified 

int execute_pipeline(command_list_t *clist) {
    int pipes[clist->num - 1][2];  // Array of pipes
    pid_t pids[clist->num];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < clist->num - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < clist->num; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < clist->num - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < clist->num - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(clist->commands[i]._cmd_buffer, clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < clist->num - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < clist->num; i++) {
        waitpid(pids[i], NULL, 0);
    }
    return OK;
}