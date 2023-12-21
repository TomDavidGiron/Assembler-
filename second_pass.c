#include "data.h"

/**
 * Second pass on the file.
 * @param memory the memory table.
 * @param data_memory the data table.
 * @param data_counter amount of data commands.
 * @param command_memory the command table.
 * @param command_counter amount of commands.
 * @param labels_operands_head head of the label operands linked list.
 * @param labels_head head of the labels linked list.
 * @return Always returns 0.
 */
int second_pass(int memory[], int data_memory[], int data_counter, int command_memory[], int command_counter, label_operand *labels_operands_head,
                labels *labels_head, externals *ext_head, int *ext_size, int *labels_size){

    /**Add to all data labels the variable command_counter, so they will hold their right value.*/
    add_to_data_labels(labels_head, command_counter, labels_size);

    /**Call the merge_tables function to merge the command and data tables into the memory table.*/
    merge_tables(data_memory, data_counter, command_memory, command_counter, memory);
    /**The array memory now holds the command_memory and data_memory by order.*/

    /**Encode the addresses that weren't encoded in the first pass.*/
    encode_labels_operands(labels_operands_head, command_counter, memory, labels_head, ext_head, ext_size);

    return 0;
}

/**
 * Merge data and command table into one table.
 * @param data_memory data table.
 * @param data_counter amount of data commands.
 * @param command_memory command table.
 * @param command_counter amount of commands.
 * @param memory the merged table.
 */
void merge_tables(int data_memory[], int data_counter, int command_memory[], int command_counter, int memory[]){

    int i, j;

    for(i = 0; i < command_counter; i++)
        memory[i] = command_memory[i];

    for(j = 0; j < data_counter; j++, i++)
        memory[i] = data_memory[j];
}

/**
 * Encode the missing values in the memory table.
 * @param labels_operands_head head of the labels operands linked list.
 * @param command_counter amount of commands.
 * @param memory memory table.
 * @param labels_head head of the labels linked list.
 */
void encode_labels_operands(label_operand *labels_operands_head, int command_counter, int memory[], labels *labels_head,
                            externals *ext_head, int *ext_size){
    int label_val, i;
    char *label;
    label_operand *curr_label_operand;

    curr_label_operand = labels_operands_head;
    /**Fill missing addresses in the memory. The missing addresses are label operands as part of a command.*/
    for(i = 0; i < command_counter; i++){

        /**The address whose decimal value is 2, is a label operand, since we encoded them in the first pass with the number 2(decimal).*/
        if(memory[i] == 2){

            /**Get the first label from the label_operand linked list. There is an injection(not including externals operands labels)
             * between the linked list and the missing addresses values.*/

            /**Ignore all externals operands labels.*/
            while (in_externals_list(curr_label_operand->label, &ext_head, ext_size))
                curr_label_operand = curr_label_operand->next;

            label = curr_label_operand->label;

            /**Get the address value of the label.*/
            label_val = get_label_value(labels_head, label);

            /**Encode the address of the label in the memory.*/
            memory[i] |= label_val << 2;

            /**Make curr point to the next label operand.*/
            curr_label_operand = curr_label_operand->next;
        }
    }
}

/**
 * Add too all data labels amount of commands.
 * @param labels_head head of the labels linked list.
 * @param commands_amount amount of commands.
 */
void add_to_data_labels(labels *labels_head, int commands_amount, int *labels_size){
    labels *curr;

    curr = labels_head;

    while (curr != NULL){

        /**If curr label is of type data(== 1).*/
        if(curr->type)
            change_label_value(curr->label, get_label_value(labels_head, curr->label) + commands_amount, labels_head, labels_size);

        curr = curr->next;
    }
}