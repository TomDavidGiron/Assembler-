#include "data.h"

/**
 * Checks if a line is valid.
 * @param line String to validate.
 * @param line_num line number in the file.
 * @param ent_head the head of the entries linked list.
 * @param ext_head the head of the externals linked list.
 * @return returns 0 for valid line, -1 for invalid line.
 */
int is_valid_line(char *line, int line_num, entries **ent_head, int *entries_size, externals **ext_head, int *externals_size,
                  labels **labels_head, int *labels_size, label_operand **labels_operand_head, int *label_operands_size){
    int command, amount_of_numbers, amount_of_commas, num;
    char *token, *label, *nums, *str_command;
    char copy_line[MAX_LINE];

    /**Supposed to be at most 1024 lines in file. Hence, if line number is over 1024, return that the program is not valid.*/
    if(line_num > MAX_MEMORY){
        printf("Too many lines in file.\n");
        return -1;
    }

    /**If the line is bigger than the limit defined, then print a message that the line is invalid and return an integer
     * indicating that the line is invalid, such as 1.*/
    if(strlen(line) > MAX_LINE){
        printf("Line exceeds the limit given(80), line number: %d \n", line_num);
        return -1;
    }

    /**Count amount of commas, before manipulating the line argument.*/
    amount_of_commas = count_commas(line);

    /**Save a copy of the line argument, before manipulating it.*/
    strcpy(copy_line, line);

    /**First token of the string.*/
    token = get_first_token(line);

    /**If memory allocation failed.*/
    if(token == NULL) {
        return -1;
    }

    /**If the line is a comment, or it is a blank line, then skip this line.*/
    if(!strcmp(token, SKIP_LINE) || is_blank_line(copy_line)) { /**strcmp function returns 0 if both strings are equal.*/
        return 0;
    }

    /**1. Check if it is a command*/
    command = is_a_command(token);

    if(command != -1){
        /**The first token is a command name. Check it's arguments.*/
        /**The function returns -1 for invalid arguments, and 0 for valid arguments.*/
        if(!validate_command_arguments(copy_line, command, labels_operand_head, label_operands_size))
            return 0;
    }

    /**2. Check if it is entry or extern.*/
     /**If the command isn't from the command list.*/
     command = is_an_entry(token);

    if(command == -1) /**If the command isn't from the command list, neither an entry.*/
        command = is_an_extern(token);

    /**3. Check if it is a label*/
    label = is_a_label_definition(token);

    /**If the first token isn't a label, nor a command, then it isn't a valid first token.*/
    if(label == NULL && command == -1){
        printf("The command is not proper.\n");
        printf("Line number: %d\n", line_num);
        return -1;
    }

    /**Get the first token. We've already checked it.*/
    token = strtok(line, " ");

    /**Second token.*/
    token = strtok(NULL, " ");

    /**If there isn't a second token, then the line is invalid.*/
    if(token == NULL){
        printf("Missing parameter, line number: %d \n", line_num);
        return -1;
    }

    /**If the first token is ".entry".*/
    if(command == IS_ENTRY){

        /**Check if the second token is a label definition. If it is, the program will do nothing, and will return 0.*/
        if(is_a_label_definition(token) != NULL){

            /**If it is a label definition, get the third token, which supposed to be a command.*/
            token = strtok(NULL, " ");

            if(token == NULL){
                printf("Not a proper command.\n");
                printf("Line number: %d\n", line_num);
                return -1;
            }

            if(is_a_command(token) != -1){ /**is_a_command returns -1 if the token is not a valid command.*/
                /**Validate the arguments of the commands.*/
                /**validate_command_arguments returns -1 if the command arguments are invalid.*/
                if(validate_command_arguments(copy_line, command, labels_operand_head, label_operands_size) != -1)
                    return 0;
            }

            /**The line is invalid*/
            printf("Not a proper command.\n");
            printf("Line number: %d\n", line_num);
            return -1;
        }

        if(fill_entry_table(token, ent_head, entries_size)){ /**Will return 0 if successful, -1 otherwise.*/
            printf("Undefined entry.\n Line number: %d \n", line_num);
            return -1;
        }
        /**Line is valid, return 0.*/
        return 0;
    }

    /**If the first token is ".extern".*/
    if(command == IS_EXTERN){

        /**Check if the second token is a label definition. If it is, the program will do nothing, and will return 0.*/
        if(is_a_label_definition(token) != NULL){

            /**If it is a label definition, get the third token, which supposed to be a command.*/
            token = strtok(NULL, " ");

            if(token == NULL){
                printf("Not a proper command.\n");
                printf("Line number: %d\n", line_num);
                return -1;
            }

            if(is_a_command(token) != -1){ /**is_a_command returns -1 if the token is not a valid command.*/
                /**Validate that the arguments of the commands.*/
                /**validate_command_arguments returns -1 if the command arguments are invalid.*/
                if(validate_command_arguments(copy_line, command, labels_operand_head, label_operands_size) != -1)
                    return 0;
            }
            /**The line is invalid*/
            return -1;
        }

        if(fill_extern_table(token, ext_head, externals_size)){ /**Will return 0 if successful, -1 otherwise.*/
            printf("Line number: %d \n", line_num);
            return -1;
        }
        /**Line is valid, return 0.*/
        return 0;
    }

    /**If the first token is a label definition, and the second one is ".data.".*/
    if(is_data_command(token)){

        nums = (char *) malloc(sizeof(char) * strlen(copy_line));

        /**If memory allocation failed.*/
        if(nums == NULL){
            printf("Memory allocation failed. Please try again.\n");
            printf("Line number: %d\n", line_num);
            return -1;
        }

        /**Make nums point to 5 places after .data in the String line.*/
        nums = strstr(copy_line, ".data");
        nums += 5;

        /**Initialize amount of nums seen to 0.*/
        amount_of_numbers = 0;

        /**Separate the String by commas.*/
        token = strtok(nums, ",");

        while (is_valid_parameter_number(token, &num)){
            amount_of_numbers++;

            /**If the number is bigger than 2^{12} - 1, or is lower than -(2{12}), then we can't present the number with 12 bits.
             * Hence, return an error.*/
            if(num > MAX_POSITIVE_NUMBER || num < MIN_NEGATIVE_NUMBER){
                printf("ERROR: Unable to present the number %d in 12 bits.\n", num);
                printf("Line number: %d\n", line_num);
                return -1;
            }

            token = strtok(NULL, ",");
        }

        if(amount_of_numbers == 0){
            printf("No parameters given.\n");
            printf("Line number: %d\n", line_num);
            return -1;
        }

        /**Amount of commas in a valid line should be: amount of numbers - 1.*/
        if(amount_of_numbers != amount_of_commas + 1){
            printf("Invalid amount of commas.\n");
            printf("Line number: %d\n", line_num);
            return -1;
        }

        /**insert_to_labels_list return 1 for success.*/
        /**Failure can happen if memory allocation failed, or if there already exists a label definition.*/
        if(insert_to_labels_list(label, labels_head, 1, line_num, labels_size) != 1) {
            printf("Line number: %d\n", line_num);
            return -1;
        }

        return 0;
    }

    /** If there weren't any errors, the fist token is a label, and the second one is".string".*/
    if(is_string_command(token)){

        str_command = (char *) malloc(sizeof(char) * strlen(copy_line));

        /**If memory allocation failed.*/
        if(str_command == NULL){
            printf("Memory allocation failed. Please try again.\n");
            printf("Line number: %d\n", line_num);
            return -1;
        }

        /**Make str_command point to 7 places after .string in the String line.*/
        str_command = strstr(copy_line, ".string");
        str_command += 7;

        /**If the string argument is invalid.*/
        if(!is_string_argument(str_command)) {
            printf("Invalid string argument.\n");
            printf("Line number: %d\n", line_num);
            return -1;
        }

        /**insert_to_labels_list return 1 for success.*/
        /**Failure can happen if memory allocation failed, or if there already exists a label definition.*/
        if(insert_to_labels_list(label, labels_head, 1, line_num, labels_size) != 1) {
            printf("Line number: %d\n", line_num);
            return -1;
        }

        return 0;
    }

    /**If the first token is a label(definition), then the second token must be a command, since we checked already
     * the commands '.data' and '.string'.*/
    if(is_a_command(token) != -1){ /**is_a_command returns -1 if the token wasn't a command.*/
        /**validate_command_arguments checks if the arguments of a command are valid. Returns -1 if they aren't.*/

        if(validate_command_arguments(copy_line, is_a_command(token), labels_operand_head, label_operands_size) != -1) {

            /**The line is valid, only need to verify if the label already exists in labels list.*/
            /**insert_to_labels_list returns 1 for success.*/
            /**Failure can happen if memory allocation failed, or if there already exists a label definition.*/
            if(insert_to_labels_list(label, labels_head, 0, line_num, labels_size) != 1) {
                printf("Line number: %d\n", line_num);
                return -1;
            }
            return 0;
        }
    }

    /**Reached end of program, and the line isn't none defined lines the assembler needs to get.*/
    printf("Not a proper line.\n");
    printf("Line number: %d\n", line_num);
    return -1;
}


/**
 * Checks if the line is a command.
 * @param index points at a given char in the line String.
 * @param line represents the file's line.
 * @return the number of the command if it is a valid command. Otherwise returns -1.
 */
int is_a_command(char *token){
    int i, j, k;
    char temp[4];
    char *commands[TOTAL_NUM_OF_COMMANDS] = {"mov", "cmp", "add", "sub", "not", "clr",
                                             "lea", "inc", "dec", "jmp", "bne", "red",
                                             "prn", "jsr", "rts", "stop"};

    /**If the line's length is less than 3, it can't be a command.*/
    if(strlen(token) < 3)
        return -1;

    for(j = 0, k = 0; token[j] != '\n' && token[j] != '\0'; j++){
        if(token[j] == ' ')
            continue;

        /**If the token has more than 3 non-space characters, we need to check if the token is 'stop'.*/
        if(k == 3){
            temp[k++] = token[j];
            temp[k] = '\0';
            /**Check if the token is stop. Return the number of the command(==15) if yes. Else, return -1, since it can't be a
             * 3 word command, nor stop command.*/
            if(!strcmp(temp, "stop"))
                return 15;
            else
                return -1;
        }

        temp[k++] = token[j];
    }
    temp[k] = '\0';


    /**Check if there is the String is equal to one of the commands, except the last command - stop.*/
    for(i = 0; i < TOTAL_NUM_OF_COMMANDS - 1; i++){
        if(!strcmp(commands[i], temp)) /**The function strcmp returns 0 if both strings are equal.*/
            return i;
    }


    /**The program checked all commands.*/
    return -1;
}


/**
 * Checks if the first token is an entry.
 * @param token first token of the line.
 * @return IS_ENTRY if the token is an entry; otherwise -1.
 */
int is_an_entry(char *token){
    /**".entry" length is 6. If the line's length is lower than 6, return indicator that the line is invalid, i.e. -1.*/
    if(strlen(token) < 6)
        return -1;

    /**Return IS_ENTRY if the input is an entry.*/
    if(!strcmp(token, ".entry")) /**The function strcmp returns 0 if both strings are equal.*/
        return IS_ENTRY;

    /**Not an ".entry", return -1 as an indicator that line doesn't contain an entry command.*/
    return -1;
}

/**
 * Checks if the first token is an extern.
 * @param token first token of the line.
 * @return IS_EXTERN if the token is a an extern; otherwise -1.
 */
int is_an_extern(char *token){
    /**".extern" length is 7. If the line's length is lower than 7, return an indicator that the line is invalid, i.e. -1.*/
    if(strlen(token) < 7)
        return -1;

    /**Return IS_EXTERN if the input is an extern.*/
    if(!strcmp(token, ".extern")) /**The function strcmp returns 0 if both strings are equal.*/
        return IS_EXTERN;

    /**Not an ".extern", return -1 as an indicator that line doesn't contain an external command.*/
    return -1;
}

/**
 * Checks if the token is a label definition.
 * @param token to be checked.
 * @return NULL if it is not a label, or if it wasn't able to allocate memory successfully. If the token is a label,
 * the function will return the label.
 */
char *is_a_label_definition(char *token){
    int i;
    char *label;

    for(i = 0; token[i] != '\0' && token[i] != '\n' && token[i] != ':'; i++);

    /**If it is not a label definition.*/
    if(token[i] != ':') {
        return NULL;
    }

    /**If there aren't any characters before ':'.*/
    if(token[0] == ':') {
        return NULL;
    }

    /**Create a label of size i.*/
    label = (char *) malloc((i + 1)* sizeof(char));

    if(label == NULL){
        printf("Memory allocation failed. Please try again.\n");
        return NULL;
    }

    /**Copy the label from the token String into the label String.*/
    strncpy(label, token, i);
    label[i] = '\0';

    /**If the label isn't valid.*/
    if(!is_a_label(label)){
        return NULL;
    }

    return label;
}

/**
 * Gets the first token of the string.
 * @param line of the file.
 * @return first token of the string.
 */
char *get_first_token(char *line){
    char *token;
    int i, start, end;

    /**If line is NULL.*/
    if(line == NULL) {
        printf("Line is empty.\n");
        return NULL;
    }

    /**Get the first non-space character.*/
    for(i = 0; line[i] == ' ' || line[i] == '\t'; i++);

    /**If it is a comment line or a blank line.*/
    if(line[i] == ';' || i == strlen(line))
        return SKIP_LINE;

    start = i;

    /**Get the first space character.*/
    for(; line[i] != '\0' && line[i] != '\n' && line[i] != ' ' && line[i] != '\t'; i++);

    end = i;
    /**Size of token is (end - start + 1)*/
    token = (char *) malloc(sizeof(char) * (end - start + 1));

    if(token == NULL){
        printf("Memory allocation failed. Please try again.\n");
        return NULL;
    }

    /**Copy the first token of the line into token.*/
    for(i = 0; start < end; start++, i++)
        token[i] = line[start];

    token[i] = '\0';

    return token;
}

/**
 * Returns if a String token is ".data".
 * @param token to be compared with ".data.".
 * @return 1 if the token is a valid ".data" command, 0 otherwise.
 */
int is_data_command(char *token){
    return !strcmp(token, ".data");
}

/**
 * Returns if a String token is ".string".
 * @param token to be compared with ".string.".
 * @return 1 if the token is ".string", 0 otherwise.
 */
int is_string_command(char *token){
    return !strcmp(token, ".string");
}

/**
 * Checks if a string token is a number.
 * @param token String that supposedly represents a  number.
 * @param num fetch the number num, if the token represents a number.
 * @return 1 for success, 0 for failure.
 */
int is_valid_parameter_number(char *token, int *num){
    char *temp;
    int i;
    errno = 0;

    if(is_blank_line(token)) {
        return 0;
    }

    *num = strtol(token, &temp, 10);

    /**If there are whitespaces.*/
    i = 0;
    if(temp != NULL && temp[i] != '\0'){
        while (isspace(temp[i])) {
            i++;
        }
    }

    if(errno != 0 || temp[i] != '\0'){
        return 0;
    }
    return 1;
}

/**
 * Count amount of commas in a line.
 * @param line to count the commas in.
 * @return amount of commas in line given as an argument.
 */
int count_commas(char *line){
    int i, count;

    for(i = 0, count = 0; line[i] != '\0' && line[i] != '\n'; i++)
        if(line[i] == ',')
            count++;
    return count;
}

/**
 * Checks if a line is blank.
 * @param line to check.
 * @return 1 if the line is blank, 0 otherwise.
 */
int is_blank_line(char *line){
    int i;

    /**Vacuously true.*/
    if(line == NULL) {
        return 1;
    }

    for(i = 0; line[i] == ' '; i++);

    return line[i] == '\0' || line[i] == '\n'; /**Return 1 if the condition is met.*/
}

/**
 * Checks if a string represents a valid string , after the '.string' command.
 * @param line that supposedly represents a valid string representation.
 * @return 1 if is is a valid string representation, otherwise 0.
 */
int is_string_argument(char *line){
    char *token;

    token = strtok(line, " ");

    /**If there is no parameter after ".string", or the first character in the parameter isn't '“', then it isn't a string
     * argument.*/

    if(token == NULL || token[0] != '\"')
        return 0;

    /**In case the string argument is two characters at most, or the last character isn't '"', then it isn't a string
     * argument.*/
    if(strlen(token) <= 3 || token[strlen(token) - 2] != '\"')
        return 0;

    token = strtok(NULL, " ");

    /**If there is extraneous text after the string argument.*/
    if(token != NULL)
        return 0;

    return 1;
}

/**
 * Checks if a given String is a label or not.
 * @param label to be checked.
 * @return 1 if it is a label, 0 if not.
 */
int is_a_label(char *label){
    int i;

    /**NULL isn't a label, nor an empty label.*/
    if(label == NULL || strlen(label) == 0)
        return 0;

    /**If String label is bigger than maximum label defined(=31), then it is not a label.*/
    if(strlen(label) > MAX_LABEL)
        return 0;

    /**If the label is a saved word, such as a command name, then it can't be a label.*/
    if(is_a_command(label) != -1) /**is_a_command returns -1 if the argument isn't a valid command.*/
        return 0;

    /**NOTE: no need to verify if label is ".data", nor ".string, nor a register, since they can't be a label anyways.*/
    /**Because they defy the condition right below.*/

    /**Check if first character of the label start with a letter. Return 0 if not.*/
    if(!isalpha(label[0]))
        return 0;

    for (i = 1; label[i] != '\n' && label[i] != '\0'; i++) {
        /**If a character is not a letter, nor a digit, then return 0.*/
        if(!isalpha(label[i]) && !isdigit(label[i]))
            return 0;
    }

    return 1;
}


/**
 * Validates if the command in the line has valid arguments, if they are proper for the command type, and no extraneous
 * text after the command arguments.
 * @param line that contains the command, and the command arguments. The function will validate the line.
 * @param command_number The number of the command the line has, numbered from 0 to 15.
 * @return 0 for valid command arguments, -1 for non-valid.
 */
int validate_command_arguments(char *line, int command_number, label_operand **head, int *label_operand_size){
    int num, commas;
    char *token, *parameter;
    char *commands[TOTAL_NUM_OF_COMMANDS] = {"mov", "cmp", "add", "sub", "not", "clr",
                                             "lea", "inc", "dec", "jmp", "bne", "red",
                                             "prn", "jsr", "rts", "stop"};

    /**Make token point to the command.*/
    token = strstr(line, commands[command_number]);

    /**Make token point to after the command string.*/
    token += 3;
    /**If command was 'stop', we need to make token point one more point.*/
    if(command_number == 15) {
        token += 1;
    }

    /**If command was 'rts' or 'stop', then there are no arguments to these commands. */
    if(command_number == 14 || command_number == 15){
        /**If after the command there are any extraneous text, then the line isn't valid.*/
        if(!is_blank_line(token)) {
            printf("Extraneous text after argument.\n");
            return -1;
        }
        return 0;
    }


    /**If the command has only destination operand.*/
    if(command_number == 4 || command_number == 5 || command_number > 6){

        parameter = strtok(token, " ");

        /**If the parameter is not a register, nor a label.*/
        if(is_a_register(parameter) == -1 && !is_a_label(parameter)) {
            if (command_number != 12) { /**If command is not "prn".*/
                printf("Not a valid command.\n");
                return -1;
            }
            /**If command is 12 ( == "prn"), a number can be a destination operand.*/
            if(!is_valid_parameter_number(parameter, &num)) {
                printf("Not a valid command.\n");
                return -1;
            }
        }


        /**If the parameter is a label operand, then insert it into the labels operand list.*/
        if(is_a_label(parameter) ){
            /**insert_into_labels_operands returns 1 for success, and 0 for failure.*/
            if(!insert_into_labels_operands(parameter, head, label_operand_size))
                return -1;
        }

        parameter = strtok(NULL, " ");

        /**If there is extraneous text after the first argument, then line is invalid.*/
        if(parameter != NULL){
            printf("Extraneous text after argument.\n");
            return -1;
        }

        return 0;
    }

    /**All other commands have a source operand, and a destination operand.*/
    /**Which means the command has one comma.*/
    commas = count_commas(token);

    if(commas != 1){
        printf("Invalid amount of commas.");
        return -1;
    }

    /**Takes the first parameter.*/
    parameter = strtok(token, " , ");

    /**If the first parameter is not a label.*/
    if(!is_a_label(parameter)){
        /**Command 6(== "lea") has only one source operand.*/
        if(command_number == 6) {
            printf("Not a valid parameter.\n");
            return -1;
        }
    }

    /**If source operand is neither a number, nor a register, nor a label.*/
    if(!is_valid_parameter_number(parameter, &num) && is_a_register(parameter) == -1 && !is_a_label(parameter))
        return -1;


    /**If the parameter is a label operand, then insert it into the labels operand list.*/
    if(is_a_label(parameter) ){
        /**insert_into_labels_operands returns 1 for success, and 0 for failure.*/
        if(!insert_into_labels_operands(parameter, head, label_operand_size))
            return -1;
    }


    /**Get second parameter.*/
    parameter = strtok(NULL, " , ");

    /**If second parameter is not a label, nor a register.*/
    if(!is_a_label(parameter) && is_a_register(parameter) == -1){
        if(command_number != 1){ /**If command is not 1(==cmp).*/
            printf("Not a valid parameter.\n");
            return -1;
        } /**Only command 1(==cmp) has three destination operands.*/
        /**If command is 1(==cmp) it can have a number as a destination operand.*/
        if(!is_valid_parameter_number(parameter, &num)){
            printf("Not a valid parameter.\n");
            return -1;
        }
    }


    /**If the parameter is a label operand, then insert it into the labels operand list.*/
    if(is_a_label(parameter) ){

        /**insert_into_labels_operands returns 1 for success, and 0 for failure.*/
        if(!insert_into_labels_operands(parameter, head, label_operand_size))
            return -1;
    }

    /**Check for extraneous text after the two operators..*/
    parameter = strtok(NULL, " ");

    if(parameter != NULL){
        printf("Extraneous text after command.\n");
        return -1;
    }

    return 0;
}

/**
 * Verify if the token given as an argument to the function is a register.
 * @param token to be verified.
 * @return -1 if the token is not a register, otherwise return the number of register(0 - 7).
 */
int is_a_register(char *token){
    char *registers[NUMBER_OF_REGISTERS] = {"@r0", "@r1", "@r2", "@r3",
                                            "@r4", "@r5", "@r6", "@r7"};
    int i;

    /**If token is NULL.*/
    if(token == NULL) {
        return -1;
    }

    /**If the token size is less than 2, then it can't be a register.*/
    if(strlen(token) < 2){
        return -1;
    }

    /**delete extraneous spaces after token.*/
    token[3] = '\0';

    /**Validate if the token is one of the registers.*/
    for (i = 0; i < NUMBER_OF_REGISTERS ; i++) {
        if(!strcmp(token, registers[i])) /**strcmp returns 0 if both strings are equal.*/
            return i;
    }

    return -1;
}

/**
 * Checks if there are labels that appear in both entries and externals table.
 * @param ent_head the head of the entries linked list.
 * @param ext_head the head of the externals linked list.
 * @return 1 if there is a collision, else 0.
 */
int check_collision_in_entries_externals(entries *ent_head, int entries_size, externals *ext_head, int externals_size){
    entries *ent_curr;
    externals *ext_curr;

    if(entries_size == 0 || externals_size == 0)
        return 0;

    for(ent_curr = ent_head; ent_curr != NULL; ent_curr = ent_curr->next){
        for(ext_curr = ext_head; ext_curr != NULL; ext_curr = ext_curr->next){
            if(!strcmp(ext_curr->ext, ent_curr->entry)){ /**If both strings are equals, there is a collision.*/
                return 1;
            }
        }
    }
    return 0;
}