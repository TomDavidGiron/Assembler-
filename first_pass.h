
#define HUNDRED_MEMORY_WORDS 100
#define ARGUMENT_IS_A_NUM 1
#define ARGUMENT_IS_A_LABEL 3
#define ARGUMENT_IS_A_REGISTER 5
#define EXT_ENCODING 1
#define ENT_ENCODING 2
#define MOVE_COMMAND_BITS 5
#define MOVE_SOURCE_OPERAND_BITS 9
#define MOVE_DESTINATION_OPERAND_BITS 2

int first_pass(char *line, int *commands_counter, int *data_counter, int memory[], int data_memory[], labels **labels_head, int *labels_size,
               entries **entries_head, int *entries_size, externals **externals_head, int *externals_size);
int insert_command_into_memory(int memory[], int *commands_counter, int command, char *token, labels **labels_head, int *labels_size,
                               entries **entries_head, int *entries_size, int *externals_size, externals **externals_head);