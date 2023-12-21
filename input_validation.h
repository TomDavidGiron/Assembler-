/**Header files for validate_input.c and validate_command_arguments.c*/

#include "errno.h"

#define SKIP_LINE "SKIP_LINE"
#define MAX_POSITIVE_NUMBER 4095 /**The maximum number that 12 bits can represent.*/
#define MIN_NEGATIVE_NUMBER (-4096) /**The minimum number that 12 bits can represent.*/
#define MAX_LINE 81
#define MAX_LABEL 32
#define NUMBER_OF_REGISTERS 8
#define TOTAL_NUM_OF_COMMANDS 16
#define IS_ENTRY 17
#define IS_EXTERN 18


/**validate_input.c functions*/
int is_valid_line(char *line, int line_num, entries **ent_head, int *entries_size, externals **ext_head, int *externals_size,
                  labels **labels_head, int *labels_size, label_operand **labels_operand_head, int *label_operands_size);
int is_a_command(char *token);
int is_an_entry(char *token);
int is_an_extern(char *token);
char *get_first_token(char *line);
char *is_a_label_definition(char *token);
int is_a_label(char *label);
int is_data_command(char *token);
int is_string_command(char *token);
int is_valid_parameter_number(char *token, int *num);
int count_commas(char* line);
int is_blank_line(char *line);
int is_string_argument(char *line);
int validate_command_arguments(char *line, int command_number, label_operand **head, int *label_operand_size);
int is_a_register(char *token);
int check_collision_in_entries_externals(entries *ent_head, int entries_size, externals *ext_head, int externals_size);