#include "data.h"

/**
 * Create the memory of the program.
 * @param line to be traversed.
 * @param commands_counter count the number of commands in the memory.
 * @param data_counter count the number of data commands in the memory.
 * @param memory table to be created by the first_pass function.
 * @param data_memory data table to be created by the first_pass function
 * @param labels_head the head of the labels linked list.
 * @param entries_head the head of the entries linked list.
 * @param externals_head the head of the externals. linked list.
 * @return 0 for success, -1 for failure.
 */
int first_pass(char *line, int *commands_counter, int *data_counter, int memory[], int data_memory[], labels **labels_head, int *labels_size,
               entries **entries_head, int *entries_size, externals **externals_head, int *externals_size){
    char *token, *label;
    int command, num, i;

    /**If line is empty, or the length of the line exceeds the valid amount given for a line, return -1 for failure.*/
    if(line == NULL || strlen(line) > MAX_LINE)
        return -1;

    /**First token of the string.*/
    token = get_first_token(line);

    /**If the line is a comment, or it is a blank line, then skip this line.*/
    if(!strcmp(token, SKIP_LINE) || is_blank_line(line)) { /**strcmp function returns 0 if both strings are equal.*/
        return 0;
    }

    /**If the line is any entry, or extern line, then it was already handled in validate_input.c*/
    if(is_an_entry(token) != -1 || is_an_extern(token) != -1){
        return 0;
    }


    /**First token of the string.*/
    token = strtok(line, " ");

    /**If the first token is a plain command.*/
    if((command = is_a_command(token)) != -1){

        /**The function will return 0 for success, -1 for failure.*/
        return insert_command_into_memory(memory, commands_counter, command, token, labels_head, labels_size,
                                          entries_head, entries_size, externals_size, externals_head);
    }

    /**The only option remains is a label definition.*/
    /**Copy the content of token into label.*/
    label = clean_label(token);

    /**Get second token.*/
    token = strtok(NULL, " ");

    /**If after the label definition there is a command.*/
    if((command = is_a_command(token)) != -1) {

        /**Cut off the ':" from label, since it it a label definition.*/
        label[strlen(label) - 1] = '\0';

        /**The value of the label is it's place in memory.*/
        change_label_value(label, ((*commands_counter) + HUNDRED_MEMORY_WORDS), *labels_head, labels_size);

        /**Free allocated memory.*/
        free(label);

        /**Insert the command after the label to the memory.*/
        /**insert_command_into_memory returns -1 for failure, 0 for success.*/
        return insert_command_into_memory(memory, commands_counter, command, token, labels_head, labels_size,
                                          entries_head, entries_size, externals_size, externals_head);
    }

    /**If after the label definition there is a data command.*/
    if(is_data_command(token)){

        /**Cut off the ':" from label, since it it a label definition.*/
        label[strlen(label) - 1] = '\0';

        /**At the start of the second pass, We'll add to the data label value the command_counter.*/
        change_label_value(label, (*data_counter) + HUNDRED_MEMORY_WORDS, *labels_head, labels_size);

        /**Get the first number.*/
        token = strtok(NULL, " , ");

        while (is_valid_parameter_number(token, &num)){
            /**Insert to data memory the number.*/
            data_memory[*data_counter] = num;
            /**Increase data_counter by 1, so it will point to next free word.*/
            (*data_counter) += 1;

            /**Get next number.*/
            token = strtok(NULL, " , ");
        }

        /**Free allocated memory.*/
        free(label);

        return 0;
    }

    /**If after the label definition there is a string command.*/
    if(is_string_command(token)){

        /**Cut off the ':" from label, since it it a label definition.*/
        label[strlen(label) - 1] = '\0';

        /**At the start of the second pass, We'll add to the data label value the command_counter.*/
        change_label_value(label, (*data_counter) + HUNDRED_MEMORY_WORDS, *labels_head, labels_size);

        /**Get the string representation.*/
        token = strtok(NULL, " ");

        /**Traverse the string representation, and add every char to the data table.*/
        for(i = 0; token[i] != '\0' && token[i] != '\n'; i++){
            if(token[i] != '\"'){
                /**Add the ascii value to the data memory.*/
                data_memory[*data_counter] = token[i];
                /**Increase data_counter to point to the next free element.*/
                (*data_counter) += 1;
            }
        }

        /**Add the ascii value of '\0' to the end of the string.*/
        data_memory[*data_counter] = '\0';
        /**Increase data_counter to point to the next free element.*/
        (*data_counter) += 1;

        /**Free allocated memory.*/
        free(label);

        return 0;
    }

    /**Free allocated memory.*/
    free(label);

    return 0;
}

/**
 * Encodes the commands to binary form, and inserts the binary code to the memory table.
 * @param memory memory table to be inserted the binary code.
 * @param commands_counter count the number of commands in the memory.
 * @param command holds the number of the command.
 * @param token holds the string formation of the command.
 * @param labels_head head of the labels linked list.
 * @param entries_head head of the entries linked list.
 * @param externals_head head of the externals linked list.
 * @return 0 for success, -1 if error was found.
 */
int insert_command_into_memory(int memory[], int *commands_counter, int command, char *token, labels **labels_head, int *labels_size,
                               entries **entries_head, int *entries_size, int *externals_size, externals **externals_head) {

    int num;

    /**Encode the command number in memory.*/
    memory[*commands_counter] = command << MOVE_COMMAND_BITS;

    /**If command have no argument(rts or stop)*/
    if(command >= 14){

        /**Make commands_counter point to the next available memory.*/
        (*commands_counter) += 1;

        /**Process finished. */
        return 0;
    }

    /**If command has one argument(4 to 13, not including 6)*/
    if(command != 6 && command >= 4){

        /**Get second token.*/
        token = strtok(NULL, " , ");

        if(is_valid_parameter_number(token, &num)){

            /**Encode that the destination operand is a number.*/
            memory[*commands_counter] |= ARGUMENT_IS_A_NUM << MOVE_DESTINATION_OPERAND_BITS;

            /**Need extra memory for the number argument.*/
            (*commands_counter) += 1;

            /**Encode the number in the memory. First encoding time.*/
            memory[*commands_counter] = num << 2;

        } else if(is_a_label(token)){

            /**If label doesn't exist in externals linked list, entries linked list or labels linked list,
             * then there is a label that doesn't have a definition in the file. The function will return -1
             * as an indicator for an error.*/
            if(!in_externals_list(token, externals_head, externals_size) && !in_entries_list(token, entries_head, entries_size)
               && !in_labels_list(token, labels_head, labels_size)){
                printf("Label doesn't have a definition.\n");
                return -1;
            }

            /**Encode that the destination operand is a label.*/
            memory[*commands_counter] |= ARGUMENT_IS_A_LABEL << MOVE_DESTINATION_OPERAND_BITS;

            /**Need extra memory for the label argument.*/
            (*commands_counter) += 1;


            /**If label is an external to the file.*/
            if(in_externals_list(token, externals_head, externals_size)){
                memory[*commands_counter] = EXT_ENCODING;
            } /**If label is local in the file.*/
            else{
                memory[*commands_counter] = ENT_ENCODING;
            }

        } else if(is_a_register(token) != -1){

            /**Encode that the destination operand is a register.*/
            memory[*commands_counter] |= ARGUMENT_IS_A_REGISTER << MOVE_DESTINATION_OPERAND_BITS;

            /**Need extra memory for the register argument.*/
            (*commands_counter) += 1;

            /**Get the register number(0 to 7) and encode it.*/
            memory[*commands_counter] = is_a_register(token) << 7;
        }

        /**Make commands_counter point to the next available memory.*/
        (*commands_counter) += 1;

        return 0;
    }

    /**Rest of the commands has two arguments.*/

    /**Get second token of the line - source operand.*/
    token = strtok(NULL, " , ");

    /**If source operand is a number.*/
    if(is_valid_parameter_number(token, &num)){

        /**Encode that the source operand is a number.*/
        memory[*commands_counter] |= ARGUMENT_IS_A_NUM << MOVE_SOURCE_OPERAND_BITS;

        /**Encode the number in the memory - in the next memory word.*/
        /**First encoding time.*/
        memory[*commands_counter + 1] = num << 2;

    } /**If source operand is a label.*/
    else if(is_a_label(token)){
        /**If label doesn't exist in externals linked list, entries linked list or labels linked list,
             * then there is a label that doesn't have a definition in the file. The function will return -1
             * as an indicator for an error.*/
        if(!in_externals_list(token, externals_head, externals_size) && !in_entries_list(token, entries_head, entries_size)
           && !in_labels_list(token, labels_head, labels_size)){
            printf("Label doesn't have a definition.\n");
            return -1;
        }

        /**Encode that the source operand is a label.*/
        memory[*commands_counter] |= ARGUMENT_IS_A_LABEL << MOVE_SOURCE_OPERAND_BITS;

        /**If label is an external to the file.*/
        if(in_externals_list(token, externals_head, externals_size)){
            /**First encoding time.*/
            memory[*commands_counter + 1] = EXT_ENCODING; /**Encode in next memory block.*/
        } /**If label is local in the file.*/
        else{
            /**First encoding time.*/
            memory[*commands_counter + 1] = ENT_ENCODING; /**Encode in next memory block.*/
        }

    } /**If source operand is a register.*/
    else if(is_a_register(token) != -1){

        /**Encode that the source operand is a register.*/
        memory[*commands_counter] |= ARGUMENT_IS_A_REGISTER << MOVE_SOURCE_OPERAND_BITS;

        /**Encode the register number in the next memory word.*/
        /**First encoding time.*/
        memory[*commands_counter + 1] = is_a_register(token) << 7;
    }

    /**Get third token - the destination operand.*/
    token = strtok(NULL, " , ");

    /**If destination operand is a number.*/
    if(is_valid_parameter_number(token, &num)){

        /**Encode that the destination operand is a number.*/
        memory[*commands_counter] |= ARGUMENT_IS_A_NUM << MOVE_DESTINATION_OPERAND_BITS;

        /**Encode the number in the memory - in the next next memory word.*/
        /**First encoding time.*/
        memory[*commands_counter + 2] = num << 2;

    } /**If destination operand is a label.*/
    else if(is_a_label(token)){
        /**If label doesn't exist in externals linked list, entries linked list or labels linked list,
             * then there is a label that doesn't have a definition in the file. The function will return -1
             * as an indicator for an error.*/
        if(!in_externals_list(token, externals_head, externals_size) && !in_entries_list(token, entries_head, entries_size)
           && !in_labels_list(token, labels_head, labels_size)){
            printf("Label doesn't have a definition.\n");
            return -1;
        }

        /**Encode that the destination operand is a label.*/
        memory[*commands_counter] |= ARGUMENT_IS_A_LABEL << MOVE_DESTINATION_OPERAND_BITS;

        /**If label is an external to the file.*/
        if(in_externals_list(token, externals_head, externals_size)){
            /**First encoding time.*/
            memory[*commands_counter + 2] = EXT_ENCODING; /**Encode in next next memory block.*/
        } /**If label is local in the file.*/
        else{
            /**First encoding time.*/
            memory[*commands_counter + 2] = ENT_ENCODING; /**Encode in next next memory block.*/
        }
    } /**If destination operand is a register.*/
    else if(is_a_register(token) != -1){

        /**Encode that the destination operand is a register.*/
        memory[*commands_counter] |= ARGUMENT_IS_A_REGISTER << MOVE_DESTINATION_OPERAND_BITS;

        /**Encode the register number in the next memory word.*/
        memory[*commands_counter + 1] |= is_a_register(token) << 2;
    }

    /**Make commands_counter point to the next available memory.*/
    (*commands_counter) += 2;

    /**If one of the operands is not a register, then 3 words of memory were used. Thus, advance the command_counter
     * to point to the next available memory.*/

    /**We check if the source operand and the destination operand equal 5(== ARGUMENT_IS_A_REGISTER). This test
     * verifies that both operands are registers.*/
    if(((memory[*commands_counter - 2] >> MOVE_DESTINATION_OPERAND_BITS) & ARGUMENT_IS_A_REGISTER)
       != ARGUMENT_IS_A_REGISTER ||  ((memory[*commands_counter - 2] >> MOVE_SOURCE_OPERAND_BITS)
                                      & ARGUMENT_IS_A_REGISTER) != ARGUMENT_IS_A_REGISTER)
        (*commands_counter) += 1;

    return 0;
}