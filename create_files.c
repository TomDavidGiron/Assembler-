#include "data.h"

int create_table_file(char *file_name, int memory[], int command_counter, int data_counter);
int create_table_entries(char *file_name, entries *entries_head, labels *labels_head, int *ent_size);
int create_externals_list(char *file_name, externals *externals_head, label_operand *label_operand_head, int memory[],
                          int command_counter, int *ext_size);
#define MAX_NUMBER_REPRESENTATION 5

/**
 * Create all three files.
 * @param file_name name of the file.
 * @param memory memory table.
 * @param command_counter number of commands.
 * @param data_counter number of data commands.
 * @param entries_head head of the entry linked list.
 * @param externals_head head of the externals linked list.
 * @param label_operand_head head of the label operand linked list.
 * @param labels_head head of the labels linked list.
 * @return 1 if files were created(if entries/externals lists are empty - no file will be created, but the return value will still be 1),
 * 0 if the programs didn't open successfully the files.
 */
int create_files(char *file_name, int memory[], int command_counter, int data_counter, entries *entries_head, int *ent_size,
                  externals *externals_head, int *ext_size, label_operand *label_operand_head, labels *labels_head){
    unsigned long file_name_length;
    int valid = 1;

    /**Get number of chars.*/
    file_name_length = strlen(file_name);

    /**Create all necessary files.*/
    valid = create_table_file(file_name, memory, command_counter, data_counter);

    /**If the file didn't open, return 0 for failure.*/
    if(!valid){
        return 0;
    }

    /**file_name was manipulated in last function. Hence, we'll reverse back to it is initial value.*/
    file_name[file_name_length] = '\0';

    valid = create_table_entries(file_name, entries_head, labels_head, ent_size);

    /**If the file didn't open, return 0 for failure.*/
    if(!valid){
        return 0;
    }

    /**file_name was manipulated in last function. Hence, we'll reverse back to it is initial value.*/
    file_name[file_name_length] = '\0';

    valid = create_externals_list(file_name, externals_head, label_operand_head, memory, command_counter, ext_size);

    /**If the file didn't open, return 0 for failure.*/
    if(!valid){
        return 0;
    }

    return 1;
}

/**
 * Create .ob file.
 * @param file_name name of the file to be created.
 * @param memory memory table.
 * @param command_counter amount of commands.
 * @param data_counter amount of data commands.
 * @return 1 if the .ob file was created successfully, 0 otherwise.
 */
int create_table_file(char *file_name, int memory[], int command_counter, int data_counter){
    FILE *output;
    int memory_size, i;

    output = fopen(strcat(file_name, ".ob"), "w");

    /**If opening the file operation was not successful.*/
    if(output == NULL) {
        printf("Couldn't open the file successfully.\n");
        return 0;
    }

    memory_size = command_counter + data_counter;

    fprintf(output, "%d %d\n", command_counter, data_counter);

    for(i = 0; i < memory_size; i++){

        fprintf(output, "%s\n", convert_to_base_64(memory[i]));
    }

    fclose(output);

    return 1;
}

/**
 * Create entries file. If entries list is empty, or an error occurred while opening the file, the file won't be created.
 * @param file_name to be open.
 * @param entries_head head of the entries linked list.
 * @param label_operand_head head of the label operand linked list.
 * @return 1 if no externals operands, or if file was created, and 0 if unable to open the file.
 */
int create_table_entries(char *file_name, entries *entries_head, labels *labels_head, int *ent_size){

    FILE *output;
    entries *curr;
    char temp[MAX_NUMBER_REPRESENTATION];

    /**Checks if entries linked list is empty. No need to output file if the linked list is empty.*/
    if(entries_is_empty(ent_size)) {
        return 1;
    }

    output = fopen(strcat(file_name, ".ent"), "w");

    /**If opening the file operation was not successful.*/
    if(output == NULL) {
        printf("Couldn't open the file successfully.\n");
        return 0;
    }

    curr = entries_head;

    while (curr != NULL)
    {

        /**Copy the entry label value into temp.*/
        sprintf(temp, "%d", get_label_value(labels_head, curr->entry));

        fprintf(output, "%s %s\n", curr->entry, temp);

        curr = curr->next;
    }

    fclose(output);

    return 1;
}

/**
 * Create external file. If no externals operands, or an error occurred while opening the file, the file won't be created.
 * @param file_name to be open.
 * @param externals_head head of the externals linked list.
 * @param label_operand_head head of the label operand linked list.
 * @return 1 if no externals operands, or if file was created, and 0 if unable to open the file.
 */
int create_externals_list(char *file_name, externals *externals_head, label_operand *label_operand_head, int memory[],
                          int command_counter, int *ext_size){

    FILE *output;
    label_operand *curr;
    char line_appearance[MAX_NUMBER_REPRESENTATION];
    int flag, i;

    /**Checks if externals linked list is empty. No need to output file if the linked list is empty.*/
    if(externals_is_empty(ext_size))
        return 1;

    /**Check if there exists at least one external element in label operands.*/

    curr = label_operand_head;
    /**flag == 0 indicates that no external element was found in labels operands.*/
    flag = 0;
    while (curr != NULL && !flag){

        if(in_externals_list(curr->label, &externals_head, ext_size)) {
            flag = 1;
        }

        curr = curr->next;
    }

    /**If no external operand was found, don't create a file.*/
    if(flag == 0)
        return 1;

    output = fopen(strcat(file_name, ".ext"), "w");

    /**If opening the file operation was not successful.*/
    if(output == NULL) {
        printf("Couldn't open the file successfully.\n");
        return 0;
    }

    curr = label_operand_head;
    for(i = 0; i < command_counter; i++){

        if(memory[i] == 1){

            /**Get the first label from the label_operand linked list. There is an injection(not including the non-externals operands labels)
             * between the linked list and the externals operands labels. */

            /**Ignore all non-externals operands labels.*/
            while (!in_externals_list(curr->label, &externals_head, ext_size))
                 curr = curr->next;

            /**Copy the line appearance into temp.*/
            sprintf(line_appearance, "%d", i + HUNDRED_MEMORY_WORDS);

            fprintf(output, "%s %s\n", curr->label, line_appearance);

            /**Make curr point to the next label operand.*/
            curr = curr->next;
        }
    }

    fclose(output);

    return 1;
}