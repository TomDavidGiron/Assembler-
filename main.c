#include "data.h"



int main(int argc, char *argv[]){

    FILE *after_macro;

    char line[MAX_LINE];
    int line_number, valid, command_counter, data_counter, file_name_length;
    int entries_size, externals_size, labels_size, label_operands_size;
    int data_memory[MAX_MEMORY], command_memory[MAX_MEMORY], memory[MAX_MEMORY];
    entries *entries_head;
    externals *externals_head;
    labels *labels_head;
    label_operand *label_operand_head;


    if(argc == 1){
        printf("No files attached to the program.\n");
        return 1;
    }

    while (argc > 1){

        /**Get the length of file name, before manipulating it in the processFile function.*/
        file_name_length = strlen(argv[argc - 1]);

        printf("File being processed: %s\n", argv[argc - 1]);

        /**If process file failed.*/
        if(!processFile(argv[argc - 1])){
            printf("Failed to process file.\n");
            argv--;
            continue;
        }

        /**Return the file name to it's original.*/
        (argv[argc - 1])[file_name_length] = '\0';

        /**after_macro will hold the file after macro processing.*/
        after_macro = fopen(strcat(argv[argc - 1], ".as"), "r");

        line_number = 0;
        valid = 0;

        /**Variables that holds the size of the labels list.*/
        label_operands_size = 0;
        entries_size = 0;
        externals_size = 0;
        labels_size = 0;
        while (fgets(line, MAX_LINE, after_macro) != NULL){

            /**If line is bigger than 80 characters.*/
            if(strlen(line) == MAX_LINE && line[MAX_LINE - 1] != '\0'){
                printf("Line exceeds the line limit given(80), line number: %d \n", line_number);
                printf("line: %s\n", line);
                valid = -1;
                continue;
            }


            if(is_valid_line(line, line_number, &entries_head, &entries_size, &externals_head, &externals_size,
                             &labels_head, &labels_size, &label_operand_head, &label_operands_size) == -1){
                printf("line: %s\n", line);
                valid = -1;
            }

            /**Increase line number.*/
            line_number++;
        }

        /**After going over the file, and filling entry and externals tables, check if there is collision in tables.*/
        if(valid != -1 && check_collision_in_entries_externals(entries_head, entries_size, externals_head, externals_size))
            valid = -1;

        /**Re-use the file.*/
        rewind(after_macro);

        /**Initialize tables indices of commands and data to 0.*/
        command_counter = 0;
        data_counter = 0;

        line_number = 0;

        while (fgets(line, MAX_LINE, after_macro) != NULL){

            
            if(strlen(line) == MAX_LINE && line[MAX_LINE - 1] != '\0'){
                continue;
            }

            if(first_pass(line, &command_counter, &data_counter, command_memory, data_memory, &labels_head, &labels_size,
                          &entries_head, &entries_size, &externals_head, &externals_size) == -1){
                printf("line number: %d\n", line_number);
                valid = -1;
            }
            line_number++;
        }


        /**Finished processing the file.*/
        fclose(after_macro);

        if(valid == -1){

            /**Free all the linked lists used.*/
            if(entries_size > 0)
                free_list_entry(entries_head);
            if(externals_size > 0)
                free_list_extern(externals_head);
            if(labels_size > 0)
                free_list_labels(labels_head);
            if(label_operands_size > 0)
                free_operand_labels_list(label_operand_head);

            printf("The file was not valid. The program will now continue to the next file if exists, or will terminate if no more files to"
                   " process.\n");
            argc--;

            /**Continue to the next file, if exists.*/
            continue;
        }

        second_pass(memory, data_memory, data_counter, command_memory, command_counter, label_operand_head, labels_head,
                    externals_head, &externals_size, &labels_size);

        /**Return the file name to it's original.*/
        (argv[argc - 1])[file_name_length] = '\0';

        create_files(argv[argc - 1],memory, command_counter, data_counter, entries_head, &entries_size,
                     externals_head, &externals_size, label_operand_head, labels_head);

        /**Free all the linked lists used.*/
        if(entries_size > 0)
            free_list_entry(entries_head);
        if(externals_size > 0)
            free_list_extern(externals_head);
        if(labels_size > 0)
            free_list_labels(labels_head);
        if(label_operands_size > 0)
            free_operand_labels_list(label_operand_head);


        printf("File processed successfully.\n");
        argc--;
    }


    return 0;

}