/**Include C standard libraries.*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include <stdbool.h>

/**Files created by programmer*/
#include "fill_tables.h"
#include "input_validation.h"
#include "first_pass.h"
#include "macro.h"
#include "second_pass.h"
#include "macro.h"



/**For file to_base_64.c*/
#define FIRST_SIX_BITS 63 /**In binary form 000000111111*/
#define SECOND_SIX_BITS 4032 /**In binary form 111111000000*/

/**For file main.c*/
#define MAX_MEMORY 1024 /** Limit of memory.*/



char *convert_to_base_64(int num);
int create_files(char *file_name, int memory[], int command_counter, int data_counter, entries *entries_head, int *ent_size,
                 externals *externals_head, int *ext_size, label_operand *label_operand_head, labels *labels_head);