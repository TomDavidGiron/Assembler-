#include "data.h"

/**
 * Create .as file, and get rid of all macros.
 * @param file_name the name of the file.
 * @return 1 for success, 0 for failure.
 */
int processFile(char *file_name)
{
    FILE *inputFile, *outputFile;
    struct MacroNode *head, *tail, *newMacroNode, *current, *currentMacro;
    size_t currentMacroLength, newLineLength;
    char *newMacroContent;
    char line[MAX_LINE_LENGTH];
    bool insideMacro, macroFound;

    /** Open input and output files */
    inputFile = fopen(file_name, "r");
    if (inputFile == NULL)
    {
        printf("Failed to open input file: %s\n", file_name);
        return 0;
    }

    outputFile = fopen(strcat(file_name, ".as"), "w");
    if (outputFile == NULL)
    {
        printf("Failed to create output file: %s\n", file_name);
        fclose(inputFile);
        return 0;
    }

    /** Initialize a linked list to store macro definitions */
    head = NULL; /** Head of the linked list */
    tail = NULL; /** Tail of the linked list */

    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL)
    {
        /** Check if the line starts with "mcro" to indicate the start of a macro definition */
        char macroName[MAX_LINE_LENGTH];
        if (sscanf(line, "mcro %s", macroName) == 1)
        {
            /** Entering macro, find its content and add it to the linked list */

            /** Read the macro content until "endmcro" */
            char *macroContent = (char *)malloc(1); /** Initialize macroContent as an empty string */
            macroContent[0] = '\0';

            while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL)
            {
                if (strcmp(line, "endmcro\n") == 0)
                    break;

                /** Append the line to the macroContent buffer */
                currentMacroLength = strlen(macroContent);
                newLineLength = strlen(line) + 1;
                newMacroContent = (char *)malloc(currentMacroLength + newLineLength);

                /** Copy the previous macro content into the new buffer */
                memcpy(newMacroContent, macroContent, currentMacroLength);

                /** Copy the new line of macro content (including the newline character) */
                strcpy(newMacroContent + currentMacroLength, line);

                /** Free the previous macro content buffer and update the pointer */
                free(macroContent);
                macroContent = newMacroContent;
            }

            /** Create a new linked list node for the macro */
            newMacroNode = (struct MacroNode *)malloc(sizeof(struct MacroNode));
            strcpy(newMacroNode->macroName, macroName);
            newMacroNode->macroContent = macroContent;
            newMacroNode->next = NULL;

            /** If the list is empty, set the new node as the head and tail */
            if (head == NULL)
            {
                head = newMacroNode;
                tail = newMacroNode;
            }
            else
            {
                /** Add the new node to the end of the list */
                tail->next = newMacroNode;
                tail = newMacroNode;
            }
        }
    }

    /** Reset the file pointer to the beginning for further processing */
    fseek(inputFile, 0, SEEK_SET);

    /** Flag to indicate if we are inside a macro definition */
    insideMacro = false;

    /** Process the input file line by line */
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL)
    {
        /** Check if the line is a macro definition start */
        if (strstr(line, "mcro ") != NULL)
        {
            insideMacro = true;
        }
            /** Check if the line is a macro definition end */
        else if (insideMacro && strstr(line, "endmcro") != NULL)
        {
            insideMacro = false;
        }
        else
        {
            /** Check if the line is a macro call */
            if (!insideMacro && line[strlen(line) - 1] == '\n')
                line[strlen(line) - 1] = '\0'; /** Remove the newline character*/

            currentMacro = head;
            macroFound = false;

            while (currentMacro != NULL)
            {
                if (!insideMacro && strcmp(line, currentMacro->macroName) == 0)
                {
                    /** Replace the macro call with its content */
                    fputs(currentMacro->macroContent, outputFile);
                    macroFound = true;
                    break;
                }

                currentMacro = currentMacro->next;
            }

            if (!macroFound && !insideMacro)
            {
                /** If the line is not a macro call and not inside a macro definition, write it to the output file */
                fputs(line, outputFile);
                fputc('\n', outputFile);
            }
        }
    }

    /** Free the linked list nodes and their contents */
    current = head;
    while (current != NULL)
    {
        struct MacroNode *next = current->next;
        free(current->macroContent);
        free(current);
        current = next;
    }

    /** Close the files */
    fclose(inputFile);
    fclose(outputFile);

    return 1;
}

