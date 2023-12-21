
int second_pass(int memory[], int data_memory[], int data_counter, int command_memory[], int command_counter, label_operand *labels_operands_head,
                labels *labels_head, externals *ext_head, int *ext_size, int *labels_size);
void encode_labels_operands(label_operand *labels_operands_head, int command_counter, int memory[], labels *labels_head,
                            externals *ext_head, int *ext_size);
void merge_tables(int data_memory[], int data_counter, int command_memory[], int command_counter, int memory[]);
void add_to_data_labels(labels *labels_head, int commands_amount, int *labels_size);