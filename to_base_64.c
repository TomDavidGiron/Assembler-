#include "data.h"


/**
 * Converts a bit to a number in base 64.
 * @param num of type short, will convert first six bits, and second six bits.
 * @return a number in base 64.
 */
char *convert_to_base_64(int num){
    int part1, part2;
    /**Will return the converted num at the end of the function*/
    char *converted_num;
    /**Table for base 64 conversion.*/
    char encoding_table[]  = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                             'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                             'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                             'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                             'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                             'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                             'w', 'x', 'y', 'z', '0', '1', '2', '3',
                             '4', '5', '6', '7', '8', '9', '+', '/'};

    /**Hold second six bits in part1, and move them 6 places to the right.*/
    part1 = num & SECOND_SIX_BITS;
    part1 = part1 >> 6;

    /**Hold first six bits in part 2.*/
    part2 = num & FIRST_SIX_BITS;

    /**Create a String of length 3*/
    converted_num =(char *)malloc(sizeof (char) * 3);

    /**If memory allocation failed.*/
    if(converted_num == NULL){
        printf("Memory allocation failed.\n");
        return NULL;
    }

    /**Put the converted binary form to it's proper place in the String.*/
    converted_num[0] = encoding_table[part1];
    converted_num[1] = encoding_table[part2];
    converted_num[2] = '\0';

    return converted_num;
}
