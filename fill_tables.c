#include "data.h"

/**
 * Puts a label in labels list. If label already exists in labels list, the function will do nothing.
 * The working premise is that token is a defined label.
 * @param token the new label String.
 * @param head head of the labels linked list.
 * @return 1 for success, -1 if label already exists, 0 if failed to insert(memory allocation failed).
 */
int insert_to_labels_list(char *token, labels **head, int label_type, int val, int *size_of_table){
    int temp;
    labels *curr, *prev;
    char *label;

    /**Clean the label out of white-space characters and new line character.*/
    label = clean_label(token);

    /**If memory allocation failed.*/
    if(label == NULL)
        return 0;

    curr = *head;
    prev = *head;
    temp = (*size_of_table);
    while (temp > 0) {
        /**If the label is already in the labels list, then return -1.*/
        if (!strcmp(curr->label, label)) { /**strcmp return 0 if both strings are equal.*/
            printf("Label already has a definition.\n");
            return -1;
        }
        temp--;
        prev = curr;
        curr = curr->next;
    }
    /**If the while loop terminates, then prev will point to the last element in the labels linked list.*/

    /**Need to allocate memory for new insertion.*/
    curr = (labels *) malloc(sizeof (labels));

    /**If memory allocation failed.*/
    if(curr == NULL){
        printf("Memory allocation failed. Please try again.\n");
        return 0;
    }

    /**Put new label in the new tail.*/
    curr->label = label;
    curr->val = val;
    /**Make new tail point to NULL.*/
    curr->next = NULL;
    curr->type = 0;
    if(label_type) /**If label is of type data, else label_type == 0, which means label is of type command..*/
        curr->type = 1;


    /**If list is empty.*/
    if((*size_of_table) == 0){
        *head = curr;
        (*size_of_table)++; /**Increase the size of the labels linked list.*/
        return 1;
    }

    /**Make old tail point to new tail..*/
    prev->next = curr;

    /**After inserting successfully a new node to the labels linked list, we'll increase it size by 1.*/
    (*size_of_table)++;
    return 1;
}

/**
 * Returns the address value of the label.
 * @param head of the labels list.
 * @param label the string representation of the label.
 * @return the address value of the label. Return -1 if the label wasn't found or an error occurred.
 */
int get_label_value(labels *head, char *label){
    labels *curr;

    if(label == NULL)
        return -1;

    curr = head;
    while( curr != NULL){
        if(!strcmp(curr->label, label)) /**strcmp returns 0 if both strings are equal.*/
            return curr->val; /**Both strings are equal, hence we found the label and we can return the value of the label.*/

        curr = curr->next;
    }
    return -1;
}

/**
 * Updates the value of the label given as an argument.
 * @param label to be updated.
 * @param value the new value of the label/
 * @param head of the labels list.
 */
void change_label_value(char *label, int value, labels *head, int *labels_size){
    labels *curr_label;

    /**Error check.*/
    if((*labels_size) == 0)
        return;

    curr_label = head;
    while (curr_label != NULL){
        if(!strcmp(curr_label->label, label)){ /**If both labels are the same.*/
            curr_label->val = value; /**Update label's value.*/
            return;
        }
        curr_label = curr_label->next;
    }
}

/**
 * This function first validates that the line is valid. If it is, it will insert into the externals linked list the new entry.
 * @param token the second token, after entry.
 * @param head head of the externals linked list.
 * @return -1 if line is invalid; If operation was successful, return 0.
 */
int fill_extern_table(char *token, externals **head, int *size_of_table){
    int temp;
    char *label;
    externals *prev, *curr;

    /**Validate that the line is correct.*/
    if(!is_a_label(token)) {
        printf("Not a label.\n");
        return -1;
    }

    /**token will be modified to verify if the label is valid, so label will hold token current value inside token.*/

    /**Clean the label out of white-space characters and new line character.*/
    label = clean_label(token);

    /**If memory allocation failed.*/
    if(label == NULL)
        return 0;

    token = strtok(NULL, " ");

    if(!is_blank_line(token)) { /**If there is extraneous text after the second token, the line isn't valid.*/
        printf("Extraneous text after second token.\n");
        return -1;
    }

    /**The token is valid.*/

    prev = *head;
    curr = *head;
    temp = (*size_of_table);

    /**If already inserted the label to the externals table.*/
    while (temp > 0) {
        if(!strcmp(label, curr->ext)){
            printf("Label already defined in extern.\n");
            return -1;
        }
        temp--;
        prev = curr;
        curr = curr->next;
    }
    /**After the while loop terminates, prev will point to the last element in the externals table.*/

    /**Allocate memory for new entry.*/
    curr = (externals *) malloc(sizeof (externals));

    /**If memory allocation failed.*/
    if(curr == NULL) {
        printf("Memory allocation failed. Please try again.\n");
        return -1;
    }

    curr->ext = label;
    curr->next = NULL;

    /**If linked list is empty*/
    if((*size_of_table) == 0){
        *head = curr;
        (*size_of_table)++; /**Increase the size of the externals linked list by 1.*/
        return 0;
    }

    /**Make old tail point to new tail.*/
    prev->next = curr;
    (*size_of_table)++; /**Increase the size of the externals linked list by 1.*/

    return 0;
}


/**
 * This function first validates that the line is valid. If it is, it will insert into the entries linked list the new entry.
 * @param token the second token, after entry.
 * @param head head of the entries linked list.
 * @return -1 if line is invalid; If operation was successful, return 0.
 */
int fill_entry_table(char *token, entries **head, int *size_of_table){
    int temp;
    char *label;
    entries *curr, *prev;

    /**Validate that the line is correct.*/

    /**If the entry is not a label.*/
    if(!is_a_label(token)) {
        printf("Not a label.\n");
        return -1;
    }

    /**token will be modified to verify if the label is valid, so label will hold value inside token, without white-spaces characters and
     * the new line character.*/

    /**Clean the label out of white-space characters and new line character.*/
    label = clean_label(token);

    /**If memory allocation failed.*/
    if(label == NULL)
        return 0;

    token = strtok(NULL, " ");

    if(!is_blank_line(token)) { /**If there is extraneous text after the second token, the line isn't valid.*/
        printf("Extraneous text after second token.\n");
        return -1;
    }

    /**The token is valid.*/

    prev = *head;
    curr = *head;
    temp = (*size_of_table);

    /**If already inserted the label to the entries table.*/
    while (temp > 0) {

        if(!strcmp(label, curr->entry)){
            printf("Label already defined in entry.\n");
            return -1;
        }
        temp--;
        prev = curr;
        curr = curr->next;
    }
    /**After the while loop terminates, prev will point to the last element in the entries table.*/

    /**Allocate memory for new entry.*/
    curr = (entries *) malloc(sizeof (entries));

    /**If memory allocation failed.*/
    if(curr == NULL) {
        printf("Memory allocation failed. Please try again.\n");
        return -1;
    }

    curr->entry = label;
    curr->next = NULL;
    curr->val = 0;

    /**If linked list is empty*/
    if((*size_of_table) == 0){
        *head = curr;
        (*size_of_table)++; /**Increase the size of the externals linked list by 1.*/
        return 0;
    }


    /**Make old tail point to new tail.*/
    prev->next = curr;
    (*size_of_table)++; /**Increase the size of the externals linked list by 1.*/

    return 0;
}

/**
 * Insert a new label operand to the label operands list.
 * @param label_name of the new label operand.
 * @param head head of the label operands linked list.
 * @return 1 for success, 0 for failure(failure to allocate memory).
 */
int insert_into_labels_operands(char *label_name, label_operand **head, int *size_of_table){
    label_operand *curr, *prev;
    int i;
    char *label;

    /**Clean the label out of white-space characters and new line character.*/
    label = clean_label(label_name);

    /**If memory allocation failed.*/
    if(label == NULL)
        return 0;

    prev = *head;

    /**Allocate memory for new entry.*/
    curr = (label_operand *) malloc(sizeof(label_operand));

    /**If memory allocation failed.*/
    if(curr == NULL) {
        printf("Memory allocation failed. Please try again.\n");
        return 0;
    }

    /**Create a new label operand.*/
    curr->label = label;
    curr->next = NULL;

    /**If there are no operand labels.*/
    if((*size_of_table) == 0){
        /**Make head point to the new label operand.*/
        *head = curr;
        /**Increase the size of the label operands by 1.*/
        (*size_of_table)++;
        /**Return 1 for success.*/
        return 1;
    }

    /**Make prev point to the last label operand inserted.*/
    for(i = 0; i < (*size_of_table) - 1; i++)
        prev = prev->next;

    /**Make the last label operand inserted point to the new created label operand.*/
    prev->next = curr;
    /**New label operand was added, hence the number of label operands is increased by 1.*/
    (*size_of_table)++;

    /**Return 1 for success.*/
    return 1;
}

/**
 * Free all nodes of the entry linked list.
 * @param head of the entries linked list.
 */
void free_list_entry(entries *head){
    entries *temp;

    while (head->next != NULL){
        temp = head;
        head = head->next;
        /**Free the label of the entry linked list.*/
        free(temp->entry);
        free(temp);
    }
    free(head);
}

/**
 * Free all nodes of the extern linked list.
 * @param head of the extern linked list.
 */
void free_list_extern(externals *head){
    externals *temp;

    while (head->next != NULL){
        temp = head;
        head = head->next;
        /**Free the label of the externals linked list.*/
        free(temp->ext);
        free(temp);
    }
    free(head);
}

/**
 * Free all nodes of the labels linked list.
 * @param head of the labels linked list.
 */
void free_list_labels(labels *head){
    labels *temp;

    while (head->next != NULL){
        temp = head;
        head = head->next;
        /**Free the label of the labels linked list.*/
        free(temp->label);
        free(temp);
    }
    free(head);
}

/**
 * Free all nodes of the operand labels linked list.
 * @param head of the labels linked list.
 */
void free_operand_labels_list(label_operand *head){
    label_operand *temp;

    while (head->next != NULL){
        temp = head;
        head = head->next;
        /**Free the label of the externals linked list.*/
        free(temp->label);
        free(temp);
    }
    free(head);
}

/**
 * Checks if a label exists in the label linked list.
 * @param label to be verified if exists in the label linked list.
 * @param head of the labels linked list.
 * @return 1 if label exists, and 0 if not exist, or 0 if memory allocation failed.
 */
int in_labels_list(char *label, labels **head, int *list_size){
    labels *curr;
    char *temp;

    /**If list is empty.*/
    if((*list_size) == 0)
        return 0;

    /**Clean the label out of white-space characters and new line character.*/
    temp = clean_label(label);

    /**If memory allocation failed.*/
    if(temp == NULL)
        return 0;

    curr = *head;

    while (curr != NULL){
        /**Check if the current label is the same as the label given as an argument.*/
        if(!strcmp(temp, curr->label)) /**strcmp returns 0 if both strings are equal.*/
            return 1;
        curr = curr->next;
    }
    return 0;
}

/**
 * Checks if an entry exists in the entries linked list.
 * @param entry to be verified if exists in the entries linked list.
 * @param head of the labels linked list.
 * @return 1 if label exists, and 0 if not exist, or 0 if memory allocation failed.
 */
int in_entries_list(char *entry, entries **head, int *list_size){
    entries *curr;
    char *label;

    /**If list is empty.*/
    if((*list_size) == 0)
        return 0;

    /**Clean the label out of white-space characters and new line character.*/
    label = clean_label(entry);

    /**If memory allocation failed.*/
    if(label == NULL)
        return 0;

    curr = *head;

    while (curr != NULL){
        /**Check if the current label is the same as the label given as an argument.*/
        if(!strcmp(label, curr->entry)) /**strcmp returns 0 if both strings are equal.*/
            return 1;
        curr = curr->next;
    }

    return 0;
}

/**
 * Checks if an external exists in the externals linked list.
 * @param ext to be verified if exists in the externals linked list.
 * @param head of the externals linked list.
 * @return 1 if label exists, and 0 if not exist, or 0 if memory allocation failed.
 */
int in_externals_list(char *ext, externals **head, int *list_size){
    externals *curr;
    char *label;

    /**If list is empty.*/
    if((*list_size) == 0)
        return 0;

    /**Clean the label out of white-space characters and new line character.*/
    label = clean_label(ext);

    /**If memory allocation failed.*/
    if(label == NULL)
        return 0;

    curr = *head;

    while (curr != NULL){
        /**Check if the current label is the same as the label given as an argument.*/
        if(!strcmp(label, curr->ext)) /**strcmp returns 0 if both strings are equal.*/
            return 1;
        curr = curr->next;
    }

    return 0;
}

/**
 * Checks if entries linked list is empty.
 * @param head of the entries linked list.
 * @return 1 if the linked list is empty, 0 otherwise.
 */
int entries_is_empty(int *ent_size){
    return (*ent_size) == 0;
}


/**
 * Checks if externals linked list is empty.
 * @param head of the externals linked list.
 * @return 1 if the linked list is empty, 0 otherwise.
 */
int externals_is_empty(int *ext_size){
    return (*ext_size) == 0;
}


/**
 * A function that gets a label, and returns it free of spaces and end of line characters.
 * @param token the label that allegedly contains white-space characters and end of line character.
 * @return NULL if memory allocation failed. If the operation was successful, return the label without white-spaces and end-of-line characters.
 */
char *clean_label(char *token){
    char *label;
    int i, j;

    /**Make label clean out of spaces and end of line characters.*/
    label = (char *) malloc(sizeof(char ) * (strlen(token) + 1));

    /**If memory allocation failed.*/
    if(label == NULL){
        printf("Memory allocation failed. Please try again.\n");
        return NULL;
    }

    /**Copy the contents of token into label.*/
    for(j = 0, i = 0; i < strlen(token); i++){
        if(token[i] != ' ' && token[i] != '\n')
            label[j++] = token[i];
    }
    label[j] = '\0';

    return label;
}