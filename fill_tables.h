/**Header file for create_tables.c*/

typedef struct entries_list{
    char *entry;
    int val;
    struct entries_list *next;
} entries;

typedef struct externals_list{
    char *ext;
    struct externals_list *next;
} externals;

typedef struct labels_list{
    char *label; /**Name of the label*/
    int val; /** The address value of the label.*/
    int type; /** type == 0 if label is a command label, otherwise type == 1, which means that the label is a data label.*/
    struct labels_list *next;
} labels;

typedef struct labels_operands{
    char *label; /**Name of the label*/
    struct labels_operands *next; /**Points to the next appearance. */
} label_operand;

int insert_to_labels_list(char *token, labels **head, int label_type, int val, int *size_of_table);
void free_list_labels(labels *head);
int fill_extern_table(char *token, externals **head, int *size_of_table);
void free_list_extern(externals *head);
int fill_entry_table(char *token, entries **head, int *size_of_table);
void free_list_entry(entries *head);
void change_label_value(char *label, int value, labels *head, int *labels_size);
int get_label_value(labels *head, char *label);
int in_labels_list(char *label, labels **head, int *list_size);
int in_entries_list(char *entry, entries **head, int *list_size);
int in_externals_list(char *ext, externals **head, int *list_size);
int insert_into_labels_operands(char *label_name, label_operand **head, int *size_of_table);
void free_operand_labels_list(label_operand *head);
int entries_is_empty(int *ent_size);
int externals_is_empty(int *ext_size);
char *clean_label(char *token);
