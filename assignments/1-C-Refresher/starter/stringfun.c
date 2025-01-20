#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int  reverse_string(char *, int, int);
int  word_print(char *, int, int);
int  replace_word(char *, int, int, char *, char *);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int i = 0;
    int j = 0;
    int current_char_count = 0;
    int starting_space = 1;
    int curr_is_whitespace = 0;
    int next_is_whitespace = 0;
    while (user_str[i] != '\0') {
        curr_is_whitespace = (user_str[i] == ' ' || user_str[i] == '\t' || user_str[i] == '\n' || user_str[i] == '\r' || user_str[i] == '\v' || user_str[i] == '\f') ? 1 : 0;
        j = i + 1;
        next_is_whitespace = (user_str[j] == ' ' || user_str[j] == '\t' || user_str[j] == '\n' || user_str[j] == '\r' || user_str[j] == '\v' || user_str[j] == '\f' || user_str[j] == '\0') ? 1 : 0;
        if (!curr_is_whitespace) starting_space = 0;
        if (curr_is_whitespace && next_is_whitespace) i++;
        else if (curr_is_whitespace && !next_is_whitespace) {
            if (starting_space) {
                i++;
                starting_space = 0;
            }
            else {
                buff[current_char_count] = user_str[i];
                current_char_count++;
                i++;
            }
        }
        else {
            buff[current_char_count] = user_str[i];
            current_char_count++;
            i++;
        }
    }
    if (current_char_count > len) return -1;
    int rv = current_char_count;
    while (current_char_count < len) {
        buff[current_char_count] = '.';
        current_char_count++;
    }

    return rv; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 1;
    int i = 0;
    while(i < str_len) {
        if (buff[i] == ' ') word_count++;
        i++;
    }

    return word_count;
}

int reverse_string(char *buff, int len, int str_len) {
    char temp;
    int upper_bound = str_len - 1;
    int center = (int)((float)str_len / 2);
    int lower_bound = 0;
    //printf("%d %d\n", str_len, center);
    while (lower_bound < center) {
        temp = buff[lower_bound];
        buff[lower_bound] = buff[upper_bound];
        buff[upper_bound] = temp;
        lower_bound++;
        upper_bound--;
    }

    return 0;
}

int word_print(char *buff, int len, int str_len) {

    int curr_word_char_count = 0;
    int curr_word_line = 1;
    int i = 0;

    printf("Word Print\n");
    printf("----------\n");
    while (i < str_len) {
        printf("%d. ", curr_word_line);
        while (1) {
            if (buff[i] == ' ' || buff[i] == '.' || buff[i] == '\0') break;
            putchar(buff[i]);
            curr_word_char_count++;
            i++;
        }
        printf("(%d)\n", curr_word_char_count);
        curr_word_char_count = 0;
        curr_word_line++;
        i++;
    }
    putchar('\n');
    printf("Number of words returned: %d\n", (curr_word_line - 1));
    return 0;
    
}

int replace_word(char *buff, int len, int str_len, char *search_term, char *replace_term) {
    return 0;
}


//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string
    int  called_replace;    //used to check if replace_word function was called

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      even if argv[1] does not exist, the first expression already checks for such scenario and 
    //      therefore it immediately prevents executing the rest of the conditions after the || which is the concerned safety issue
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //      All of the command including the flags after the -h requires a minimum of 2 arguments (+1 for the executable file)
    //      Therefore anything below 2 arguments would not be enough for the program to know what to do and should be terminated
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char*) malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Failed to allocate %d bytes for buff.", BUFFER_SZ);
        exit(2);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }


    called_replace = 0;
    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options

        case 'r':
            rc = reverse_string(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error reversing words, rc = %d", rc);
                exit(2);
            }
            break;

        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            break;

        case 'x':
            called_replace = 1;
            if (argc != 5) {
                rc = -1;
            }
            else {
                rc = replace_word(buff, BUFFER_SZ, user_str_len, argv[3], argv[4]);
            }

            if (rc < 0) {
                called_replace = 0;
                printf("Error replacing word, rc = %d", rc);
                exit(2);
            }
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    if (called_replace) printf("Not Implemented!\n");
    else print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          To me, this is still a good practice to have as potential changes to the requirements may cause unexpected issues and errors
//          if one were to stick with only one of the ways to iterate over the buff. This allows more flexibility and adaptability to
//          the code in face of changes and thus is a good practice to have. 